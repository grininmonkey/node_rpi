CC = gcc
CFLAGS = -Wall -O2 -Iinclude $(shell pkg-config --cflags avahi-client)
LDFLAGS = -pthread -lmicrohttpd -ljansson $(shell pkg-config --libs avahi-client) -lm -lcurl -luuid

SRC_DIR = src
OBJDIR = build

# Recursively find all .c files
SRC = $(shell find $(SRC_DIR) -name '*.c' ! -name '._*')
# Replace src/... with build/... and .c with .o
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(SRC))

TARGET = node_rpi

.PHONY: all clean

all: $(TARGET)

# Link target from all object files
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule to compile .c to build/.o with same directory structure
$(OBJDIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
	find . -name '._*' -delete

memcheck:
	@sudo valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=all --suppressions=valgrind.supp ./$(TARGET) --service -v
