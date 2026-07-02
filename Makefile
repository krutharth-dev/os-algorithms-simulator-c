CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = os_simulator
SRC = main.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -pthread

run: all
	./$(TARGET)

clean:
	rm -rf $(TARGET) *.o build output *.dSYM main main.dSYM outDebug
