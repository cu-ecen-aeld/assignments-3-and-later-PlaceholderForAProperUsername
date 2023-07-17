cc = $(CROSS_COMPILE)gcc

all: aesdsocket
default: aesdsocket

.PHONY: all default clean

aesdsocket: aesdsocket.o
	$(cc) -Wall -Werror -o aesdsocket aesdsocket.o
	
aesdsocket.o: aesdsocket.c
	$(cc) -Wall -Werror -c -o aesdsocket.o aesdsocket.c
	
clean:
	rm -f *.o
	rm -f aesdsocket
		
