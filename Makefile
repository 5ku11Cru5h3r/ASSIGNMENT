CXX := g++
CXXFLAGS := -std=c++17  -Iinclude   
LIBS := -lcurl

SRC := main.cpp 
TARGET = bot

all: $(TARGET)
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
