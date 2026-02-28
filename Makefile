CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
INCLUDE = -Iheaders

SRC_DIR = src
BUILD   = build

SRCS    = $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRCS))

TARGET  = $(BUILD)/myPs

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)

.PHONY: all clean