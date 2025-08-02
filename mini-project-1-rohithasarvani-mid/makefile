# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -I.

# Object files
OBJ = 1.o 2.o 3.o 4.o 5.o 7.o 8.o main.o

# Target executable
TARGET = a.out

# Rule to build the target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Rules to build object files
1.o: 1.c 1.h
	$(CC) $(CFLAGS) -c 1.c

2.o: 2.c 2.h
	$(CC) $(CFLAGS) -c 2.c

3.o: 3.c 3.h
	$(CC) $(CFLAGS) -c 3.c

4.o: 4.c 4.h
	$(CC) $(CFLAGS) -c 4.c

5.o: 5.c 5.h
	$(CC) $(CFLAGS) -c 5.c

7.o: 7.c 7.h
	$(CC) $(CFLAGS) -c 7.c

8.o: 8.c 8.h
	$(CC) $(CFLAGS) -c 8.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)
