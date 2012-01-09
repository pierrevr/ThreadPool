#ifndef THREADPOOLEXCEPTION_HPP
#define THREADPOOLEXCEPTION_HPP

#include <exception>

class ThreadPoolException : public std::exception {
public:
    virtual const char * what() const throw();
};

#endif