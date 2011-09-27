CC = gcc
LD = gcc
CFLAGS = -D__GNU_SOURCE -Wall -Wextra -std=gnu99 -ggdb3

all: sneaker.so testc tests pysneak_c.so \
	libfakepython.so test_dlsym_mucker.so test_dlsym_mucked

%.so: %.c
	$(CC) $(CFLAGS) -ldl -shared -fPIC -o $@ $^

pysneak_c.so: pysneak_c.c
	$(CC) $(CFLAGS) -ldl $(shell pkg-config python --cflags --libs) -shared -fPIC -o $@ $^

test_dlsym_mucker.so: test_dlsym_mucker.c libfakepython.so
	$(CC) $(CFLAGS) -ldl -shared -L. -lfakepython -fPIC -o $@ $^

clean:
	rm -f sneaker.so testc tests pysneak_c.so *.pyc libfakepython.so test_dlsym_mucker.so \
		test_dlsym_mucked

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

