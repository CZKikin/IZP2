CFLAGS = -std=c99 -Wall -Wextra -Werror sps.c 
CC = gcc
sps: sps.c
	$(CC) $(CFLAGS) -o sps

debug:
	$(CC) $(CFLAGS) -ggdb3 -D DEBUG -o sps

clean:
	rm sps
