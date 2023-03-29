# Compiler options
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99

# Directories
SRCDIR = src
INCDIR = include
TESTDIR = test
DATADIR = data
OBJDIR = obj
BINDIR = bin

# File extensions
SRCEXT = c
HDREXT = h
OBJEXT = o

# Files
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
HEADERS := $(shell find $(INCDIR) -type f -name *.$(HDREXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
TESTS := $(shell find $(TESTDIR) -type f -name *.$(SRCEXT))
DATA := $(shell find $(DATADIR) -type f)

# Binary name
TARGET = $(BINDIR)/project3

# Default target
all: $(TARGET)

# Compile object files
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Link object files into binary
$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $(TARGET)

# Compile and run unit tests
test: CFLAGS += -DUNIT_TESTING
test: $(OBJECTS) $(TESTS)
	$(CC) $(CFLAGS) -I$(INCDIR) $(OBJECTS) $(TESTS) -o $(TARGET)_test
	$(TARGET)_test

# Clean object files, binary, and test binary
clean:
	@echo "Cleaning up..."
	$(RM) -r $(OBJDIR) $(TARGET) $(TARGET)_test

# Phony targets
.PHONY: all clean test

