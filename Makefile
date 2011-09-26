CC = gcc
LD = gcc
CFLAGS = -D__GNU_SOURCE -Wall -Wextra -std=gnu99 -ggdb3

all: sneaker.so testc tests pysneak_c.so

sneaker.so: sneaker.c
	$(CC) -ldl -shared -fPIC -o $@ $^

pysneak_c.so: pysneak_c.c
	$(CC) $(CFLAGS) -ldl $(shell pkg-config python --cflags --libs) -shared -fPIC -o $@ $^

clean:
	rm -f sneaker.so testc tests pysneak_c.so *.pyc

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

