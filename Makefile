CC = gcc
CFLAGS = -Wall -Wextra -s -O3

SRCS = src/hcu.c
OBJS = $(SRCS:.c=.o)

TARGET = hcu

PREFIX = /usr/local

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	install -m 755 $(TARGET) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

