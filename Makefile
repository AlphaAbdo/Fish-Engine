# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lSDL2 -lSDL2_image -lm #-fsanitize=address -g

# Source files
SRCS = aquaengine.c aquaSDL.c main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = main

# Make rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
