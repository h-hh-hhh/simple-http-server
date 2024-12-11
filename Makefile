CC = gcc
CFLAGS = -Wall -g
objects_server = main.o utils/utils.o

all: server

server: $(objects_server)
	$(CC) $(CFLAGS) $(objects_server) -o server

main.o: main.c

utils/utils.o: utils/utils.c 

clean:
	rm -f $(objects_server) server