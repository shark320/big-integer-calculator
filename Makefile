CC = gcc

CFLAGS = -Wall -Wextra -pedantic -ansi -g
LDFLAGS = $(CFLAGS) -lm

BUILD_DIR = build
BIN = calc.exe

all: clean $(BUILD_DIR) $(BUILD_DIR)/$(BIN)

$(BUILD_DIR)/$(BIN): $(BUILD_DIR)/calc.o $(BUILD_DIR)/long_arithmetic.o $(BUILD_DIR)/futils.o $(BUILD_DIR)/vector.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/calc.o: src/calc.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/long_arithmetic.o: src/long_arithmetic/long_arithmetic.c src/long_arithmetic/long_arithmetic.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/futils.o: src/utils/futils/futils.c src/utils/futils/futils.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/vector.o: src/utils/vector/vector.c src/utils/vector/vector.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir $@

# on linux use rm -rf $(BUILD_DIR)
clean:
	del /F /Q $(BUILD_DIR) 