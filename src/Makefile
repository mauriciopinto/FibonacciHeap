CC=g++
CFLAGS=-g
ALLOBJ=main.o
DEPS=BNode.h FibonacciHeap.h

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

FibonacciHeap: $(ALLOBJ)
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean

clean:
	rm -f FibonacciHeap
	rm -f $(ALLOBJ)
