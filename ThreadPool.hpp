#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include "ThreadPoolException.hpp"

#include <queue>
#include <pthread.h>

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