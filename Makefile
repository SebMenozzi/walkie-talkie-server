.PHONY: clean

CC = g++
CFLAGS = -w
BIN = server
SRC = src/main.cpp src/utils.cpp src/UDPServer.cpp src/clock.cpp
OBJS = $(SRC:.cpp=.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

clean:
	$(RM) $(OBJS) $(BIN)
