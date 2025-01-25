
TARGET=dot
SOURCE=src
TESTS=tests

# DEBUG_FLAG=-DDOT_DEBUG

CXXFLAGS=-std=c++11 -Wall -Wextra -Wpedantic -I$(SOURCE) $(DEBUG_FLAG)

CXXSRC=$(wildcard $(SOURCE)/*.cpp)
CXXOBJ=$(CXXSRC:.cpp=.o)
CXXDEP=$(CXXSRC:.cpp=.d)

all: $(TARGET)

-include $(CXXDEP)

$(TARGET): $(CXXOBJ) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(CXXOBJ)

$(SOURCE)/%.o: $(SOURCE)/%.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

$(BUILD):
	mkdir -p $(BUILD)

clean:
	$(RM) $(TARGET)
	$(RM) $(SOURCE)/*.o $(SOURCE)/*.d

test: ./test.py $(TESTS) $(TARGET) 
	python3 ./test.py ./$(TARGET) ./$(TESTS)

update: ./test.py $(TARGET) 
	python3 ./test.py ./$(TARGET) $(TESTS) -u
