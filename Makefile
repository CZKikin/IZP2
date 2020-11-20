CFLAGS = -std=c99 -Wall -Wextra -Werror sps.c 
CC = gcc
sps: sps.c
	$(CC) $(CFLAGS) -o sps	

debug:
	$(CC) $(CFLAGS) -g3 sps