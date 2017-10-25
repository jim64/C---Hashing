CFLAGS = -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi
INCS = dhash.h
TARGET = spll
SOURCES =  dhash.c $(TARGET).c
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
