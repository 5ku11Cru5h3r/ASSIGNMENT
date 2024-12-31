CXX := g++
CXXFLAGS := -std=c++17  -Iinclude   
LIBS := -lcurl

SRC := src/main.cpp src/api_client.cpp #src/websocket_client.cpp
TARGET = bot

all: $(TARGET)
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
