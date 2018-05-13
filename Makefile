CC=g++
CFLAGS=-Wall -Wextra -Werror -Wpedantic
LDFLAGS=-pthread
HEADERS=ConcurrentQueue.h
SOURCES=ProducerConsumerTest.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: ProducerConsumerTest

%.o: %.cpp $(HEADERS)
	$(CC) -c -o $@ $(CFLAGS) $<

ProducerConsumerTest: ProducerConsumerTest.o
	$(CC) -o $@ $(LDFLAGS) $^

clean:
	rm -f $(OBJECTS) ProducerConsumerTest
