﻿/*
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

#ifndef THREADPOOLEXCEPTION_HPP
#define THREADPOOLEXCEPTION_HPP

#include <exception>

class ThreadPoolException : public std::exception {
public:
    virtual const char * what() const throw();
};

#endif
