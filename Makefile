CC=gcc
CFLAGS=-O2 -Wall -Wextra

SRC=src/main.c src/engine.c src/metrics.c src/replay.c
TOOLS_SRC=tools/gen_events.c

all: lab gen_events

lab: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o lab

gen_events: $(TOOLS_SRC)
	$(CC) $(CFLAGS) $(TOOLS_SRC) -o gen_events

clean:
	rm -f lab gen_events events.bin
