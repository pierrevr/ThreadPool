/*
 * Copyright 2011 Pierre Vorhagen, Etienne Doms.
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ThreadPool.hpp"

ThreadPool::ThreadPool(unsigned int size) : _size(0), _tasks(0), _pause(false) {
    pthread_mutex_init(&_mutex_queue, NULL);
    pthread_mutex_init(&_mutex_tasks, NULL);
    pthread_mutex_init(&_mutex_pause, NULL);

    pthread_cond_init(&_cond_todo, NULL);
    pthread_cond_init(&_cond_done, NULL);
    pthread_cond_init(&_cond_pause, NULL);

    if (size == 0) {
        size = 1;
    }

    _threads = new pthread_t[size];

    for (unsigned int i = 0; i < size; i++) {
        if (pthread_create(&_threads[i], NULL, _routine, this) != 0) {
            for (unsigned int i = 0; i < _size; i++) {
                pthread_cancel(_threads[i]);
                pthread_join(_threads[i], NULL);
            }

            delete[] _threads;

            throw ThreadPoolException();
        }

        _size++;
    }
}

ThreadPool::~ThreadPool() {
    flush();

    for (unsigned int i = 0; i < _size; i++) {
        pthread_cancel(_threads[i]);
        pthread_join(_threads[i], NULL);
    }

    delete[] _threads;
}

void ThreadPool::add(Worker * worker) {
    pthread_mutex_lock(&_mutex_queue);
    pthread_mutex_lock(&_mutex_tasks);

    _queue.push(worker);
    _tasks++;

    pthread_mutex_unlock(&_mutex_tasks);
    pthread_mutex_unlock(&_mutex_queue);

    pthread_cond_signal(&_cond_todo);
}

void ThreadPool::flush() {
    resume();

    pthread_mutex_lock(&_mutex_tasks);

    while (_tasks > 0) {
        pthread_cond_wait(&_cond_done, &_mutex_tasks);
    }

    pthread_mutex_unlock(&_mutex_tasks);
}

void ThreadPool::pause() {
    pthread_mutex_lock(&_mutex_pause);
    _pause = true;
    pthread_mutex_unlock(&_mutex_pause);
}

void ThreadPool::resume() {
    pthread_mutex_lock(&_mutex_pause);
    _pause = false;
    pthread_mutex_unlock(&_mutex_pause);

    pthread_cond_broadcast(&_cond_pause);
}

void * ThreadPool::_routine(void * data) {
    ThreadPool * pool = static_cast<ThreadPool *>(data);

    while (true) {
        Worker * worker;

/* -------------------------------------------------------------------------- */
        pthread_cleanup_push(_clean, static_cast<void *>(&pool->_mutex_queue));
        pthread_mutex_lock(&pool->_mutex_queue);

        while (pool->_queue.empty()) {
            pthread_cond_wait(&pool->_cond_todo, &pool->_mutex_queue);
        }

        worker = pool->_queue.front(), pool->_queue.pop();

        pthread_cleanup_pop(1);

/* -------------------------------------------------------------------------- */
        pthread_mutex_lock(&pool->_mutex_pause);

        while (pool->_pause) {
            pthread_cond_wait(&pool->_cond_pause, &pool->_mutex_pause);
        }

        pthread_mutex_unlock(&pool->_mutex_pause);

/* -------------------------------------------------------------------------- */
        worker->run();
        delete worker;

/* -------------------------------------------------------------------------- */
        pthread_mutex_lock(&pool->_mutex_tasks);
        pool->_tasks--;
        pthread_mutex_unlock(&pool->_mutex_tasks);

/* -------------------------------------------------------------------------- */
        pthread_cond_signal(&pool->_cond_done);
    }

    return NULL;
}

void ThreadPool::_clean(void * data) {
    pthread_mutex_unlock(static_cast<pthread_mutex_t *>(data));
}
