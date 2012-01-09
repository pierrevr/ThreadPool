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

#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include "ThreadPoolException.hpp"

#include <pthread.h>
#include <queue>

class ThreadPool {

public:
    class Worker {

    public:
        virtual void run() = 0;
    };

    ThreadPool(unsigned int size = 1);
    ~ThreadPool();

    void add(Worker * worker);
    void flush();
    void pause();
    void resume();

private:
    unsigned int _size;

    unsigned int _tasks;

    bool _pause;

    pthread_mutex_t _mutex_queue;
    pthread_mutex_t _mutex_tasks;
    pthread_mutex_t _mutex_pause;

    pthread_cond_t _cond_todo;
    pthread_cond_t _cond_done;
    pthread_cond_t _cond_pause;

    pthread_t * _threads;

    std::queue<Worker *> _queue;

    static void * _routine(void * data);
    static void _clean(void * data);
};

#endif
