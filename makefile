CC = gcc
TARGET = main
LDFLAGS = -pthread

OBJS = main.o
test : $(OBJS)
	$(CC) -o $(TARGET) $(LDFLAGS) $(OBJS)
main.o: main.c
	$(CC) -c main.c