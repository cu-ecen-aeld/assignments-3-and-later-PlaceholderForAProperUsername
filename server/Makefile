CC=$(CROSS_COMPILE)gcc
TARGET=aesdsocket
CFLAGS=-Wall -Werror -g -O0

all: $(TARGET)
default: $(TARGET)

.PHONY: all default clean

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o
	
$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c -o $(TARGET).o $(TARGET).c
	
clean:
	rm -f $(TARGET).o
	rm -f $(TARGET)
		
