CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
BUILD   = build
TARGET  = $(BUILD)/myPs
SRC     = myPs.c
OBJ     = $(BUILD)/myPs.o

all: $(BUILD) $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD)

.PHONY: all clean