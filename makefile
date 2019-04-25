CC = gcc
TARGET = main
LDFLAGS = -pthread -lwiringPi

OBJS = main.o SharedMemory.o
main : $(OBJS)
	$(CC) -o $(TARGET) $(LDFLAGS) $(OBJS)
main.o: main.c SharedMemory.h
	$(CC) -c main.c
SharedMemory.o: SharedMemory.c SharedMemory.h
	$(CC) -c main.c SharedMemory.c