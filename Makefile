# A simple Makefile for compiling small SDL projects

# Set the compiler
CC := g++

# Set the compiler flags
CFLAGS := $(shell sdl2-config --libs --cflags) -ggdb3 -O0 --std=c++11 -Wall -lSDL2_image -lm -lSDL2_ttf -fPIC -pie
ASAN_FLAGS := -g -fsanitize=address

# Add header files here
HDRS :=

# Add source files here
SRCS := main.cpp draw.cpp input.cpp init.cpp handlingPlayer.cpp enemies.cpp underLine.cpp walls.cpp scenes.cpp

# Generate names of object files
OBJS := $(SRCS:.cpp=.o)

# Name of the executable
EXEC := game

# Default recipe
# all: $(EXEC)

# Recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(ASAN_FLAGS) $(OBJS) $(CFLAGS)

# Recipe for building object files
$(OBJS): $(@:.o=.cpp) $(HDRS) Makefile
	$(CC) -o $@ $(@:.o=.cpp) -c $(ASAN_FLAGS) $(CFLAGS)

# Declare object files as intermediate
.INTERMEDIATE: $(OBJS)

.PHONY: clean

# Recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

# .PHONY: all clean
