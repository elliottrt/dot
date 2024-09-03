
TARGET=main
BUILD=build
SOURCE=src

CXXFLAGS=-std=c++11 -Wall -Wextra -Wpedantic

CXXSRC=$(wildcard $(SOURCE)/*.cpp)
CXXOBJ=$(subst $(SOURCE)/,$(BUILD)/,$(CXXSRC:.cpp=.o))
CXXDEP=$(CXXOBJ:.o=.d)

all: main

$(TARGET): $(CXXOBJ) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(CXXOBJ)

$(BUILD)/%.o: $(SOURCE)/%.cpp $(BUILD) Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

$(BUILD):
	mkdir -p $(BUILD)

clean:
	$(RM) -r -f $(BUILD)
	$(RM) -f $(TARGET)

-include $(CXXDEP)
