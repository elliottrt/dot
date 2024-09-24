
TARGET=main
SOURCE=src

CXXFLAGS=-std=c++11 -Wall -Wextra -Wpedantic -Isrc/

CXXSRC=$(wildcard $(SOURCE)/*.cpp)
CXXOBJ=$(CXXSRC:.cpp=.o)
CXXDEP=$(CXXSRC:.cpp=.d)

all: main

-include $(CXXDEP)

$(TARGET): $(CXXOBJ) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(CXXOBJ)

$(SOURCE)/%.o: $(SOURCE)/%.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

$(BUILD):
	mkdir -p $(BUILD)

clean:
	$(RM) -f $(TARGET)
	$(RM) -f $(SOURCE)/*.o $(SOURCE)/*.d
