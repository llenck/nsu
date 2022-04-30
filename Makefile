CFLAGS ?= -Wall -Wextra -std=c99
LDFLAGS ?=
CC ?= gcc

SOURCES := nsu.c
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
HEADERS :=
EXECUTABLE := nsu

debug : CFLAGS += -Og -g -Wpedantic
release : CFLAGS += -O3 -s
debug : LDFLAGS +=
release : LDFLAGS += -O -s

debug: $(EXECUTABLE)
release: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c

.PHONY: clean
clean:
	$(RM) $(EXECUTABLE) $(OBJECTS)
