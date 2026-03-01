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

ARCHIVE = $(BUILD)/project.tar.gz
ARCHIVE_SRC = $(wildcard src/*.c headers/*.h) Makefile

$(ARCHIVE): $(ARCHIVE_SRC)
	tar czf $(ARCHIVE) src headers Makefile
	@echo "Archive updated"

archive: $(ARCHIVE)

clean:
	rm -rf $(BUILD)

.PHONY: all clean