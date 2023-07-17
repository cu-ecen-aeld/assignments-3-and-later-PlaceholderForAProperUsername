cc = $(CROSS_COMPILE)gcc

all: aesdsocket
default: aesdsocket

aesdsocket: aesdsocket.c
	$(cc) -Wall -Werror aesdsocket.c -o aesdsocket
	
clean:
	rm -f aesdsocket
		
