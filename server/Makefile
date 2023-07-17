cc?=$(CROSS_COMPILE)gcc
TARGET?=aesdsocket
CFLAGS?=-Wall -Werror -g -O0
LDFLAGS?=-pthread

all: $(TARGET)
default: $(TARGET)

.PHONY: all default clean

$(TARGET): $(TARGET).c
	$(cc) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	
clean:
	rm -f *.o
	rm -f aesdsocket
		
