CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lsqlite3

SRCS = main.c db.c

OBJS = $(SRCS:.c=.o)

TARGET = tm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
