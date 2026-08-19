CXX := g++
CXXFLAGS := -O2 -Wall -Wextra

SRCDIR := src
BINDIR := bin
TARGET := $(BINDIR)/main

SOURCES := $(wildcard $(SRCDIR)/*.cpp)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BINDIR)
