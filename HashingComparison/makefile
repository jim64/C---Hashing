CFLAGS = -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi
INCS = extnd.h
TARGET = extnd
SOURCES =  extndDHfunc.c extndGenfunc.c extndLLfunc.c extndotherhash.c $(TARGET).c
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
