CC=gcc
CFLAGS=-O2 -Wall -Wextra

SRC=src/main.c src/engine.c src/metrics.c src/replay.c

all:
	$(CC) $(CFLAGS) $(SRC) -o lab

clean:
	rm -f lab