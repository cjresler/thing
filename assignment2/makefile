CC := g++
WARN := -Wall -Wextra

CCOPTS := $(WARN)

factory : factory.o
	$(CC) -o $@ $^

%.o : $.c
	$(CC) $(CCOPTS) -c -o $@ $<

clean:
	rm -rf *.o factory

factory.o : factory.h
