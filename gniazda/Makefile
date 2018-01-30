CC = gcc
CFLAGS = -Wall

all: server klient 

server: serverm.c
	${CC} -o serverm serverm.c ${CFLAGS}

klient: klient.c
	${CC} -o klient klient.c ${CFLAGS}

clean:
	rm -f serverm klient 	
