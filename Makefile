CC=gcc
SRC=$(wildcard ./src/*.c)
HEADERS=$(wildcard ./src/*.h)
BUILD=build
DEPS=$(BUILD) $(SRC) $(HEADERS)
CFLAGS=-O3 -Werror -Wall -Wextra
DFLAGS=-O0 -g -Wall -Wextra
LFLAGS=-lm
EXE=$(BUILD)/pratt-parsing
TEST=$(BUILD)/pratt-parsing-test
DEBUG=$(BUILD)/pratt-parsing-debug
DEFS=

ifeq ($(OS),Windows_NT)
    CFLAGS += -D__USE_MINGW_ANSI_STDIO
endif

all: $(EXE) $(TEST) $(DEBUG)

$(EXE): $(DEPS)
	$(CC) $(DEFS) $(CFLAGS) -o $(EXE) $(SRC) $(LFLAGS)

$(TEST): $(DEPS)
	$(CC) -DTEST $(DEFS) $(CFLAGS) -o $(TEST) $(SRC) $(LFLAGS)

$(DEBUG): $(DEPS)
	$(CC) $(DEFS) $(DFLAGS) -o $(DEBUG) $(SRC) $(LFLAGS)

$(BUILD):
	mkdir -p $(BUILD)

.PHONY: run debug test clean

run: $(EXE)
	./$(EXE)

debug: $(DEBUG)
	./$(DEBUG)

test: $(TEST)
	./$(TEST)

clean:
	rm -rf build
	rm -f *.txt
