objects_server = main.o

all: server

server: $(objects_server)
	$(CC) main.o -o server

main.o: main.c

clean:
	rm -f *.o server