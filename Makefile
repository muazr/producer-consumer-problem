CC=g++
CFLAGS=-std=c++11 -Wall -Wextra -Werror -Wpedantic
LDFLAGS=-pthread
HEADERS=concurrentqueue.h
SOURCES=producerconsumertest.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: producer-consumer-test

%.o: %.cpp $(HEADERS)
	$(CC) -c -o $@ $(CFLAGS) $<

producer-consumer-test: producerconsumertest.o
	$(CC) -o $@ $(LDFLAGS) $^

clean:
	rm -f $(OBJECTS) producer-consumer-test
