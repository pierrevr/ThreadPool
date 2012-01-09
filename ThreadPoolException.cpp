#include "ThreadPoolException.hpp"

const char * ThreadPoolException::what() const throw() {
    return "ThreadPoolException: threads limit exceeded";
}