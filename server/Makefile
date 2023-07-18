CC?=$(CROSS_COMPILE)gcc
TARGET?=aesdsocket
CFLAGS?=-Wall -Werror -g -O0

all: $(TARGET)
default: $(TARGET)

.PHONY: all default clean

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
	
clean:
	rm -f $(TARGET).o
	rm -f $(TARGET)
		
