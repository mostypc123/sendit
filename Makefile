CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -MMD -MP
LDFLAGS := -lssl -lcrypto

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=obj/%.o)
DEPS = $(OBJECTS:.o=.dep)
TARGET = sendit

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.cpp | obj
	$(CXX) $(CXXFLAGS) -MF $(@:.o=.dep) -c -o $@ $<

obj:
	mkdir -p $@

clean:
	rm -rf obj

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

-include $(DEPS)

.PHONY: all clean rebuild run
