CC ?= $(CROSS_COMPILE)gcc
TARGET ?= aesdsocket
CFLAGS ?= -Wall -Werror -g -O0
LDFLAGS ?= -lpthread -lrt


all: $(TARGET)
default: $(TARGET)

.PHONY: all default clean

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)
	
clean:
	rm -f $(TARGET).o
	rm -f $(TARGET)
		
