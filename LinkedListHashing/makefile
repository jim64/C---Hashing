CFLAGS = -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi
INCS = shash.h
TARGET = spll
SOURCES =  shash.c $(TARGET).c
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
