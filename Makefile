objects_server = main.o

all: server

server: $(objects_server)
	$(CC) $(objects_server) -o server

main.o: main.c

clean:
	rm -f *.o server