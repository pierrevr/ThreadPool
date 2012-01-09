CPP=g++
CPPFLAGS=-Wall -Wextra -pedantic
OBJ=ThreadPoolException.o ThreadPool.o
BIN=libthreadpool.a

all: $(BIN)

libthreadpool.a: $(OBJ)
	ar -r $(BIN) $(OBJ)

ThreadPoolException.o: ThreadPoolException.cpp
	$(CPP) $(CPPFLAGS) -c ThreadPoolException.cpp

ThreadPool.o: ThreadPool.cpp
	$(CPP) $(CPPFLAGS) -c ThreadPool.cpp

clean:
	rm -f $(OBJ)
	rm -f $(BIN)

.PHONY: clean
