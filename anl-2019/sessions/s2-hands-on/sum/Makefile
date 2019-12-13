CFLAGS += `pkg-config --cflags margo`
LDFLAGS += `pkg-config --libs margo`

all:: client server

client.o: client.c types.h
	$(CC) $(CFLAGS) -c client.c

client: client.o
	$(CC) client.o -o client  $(LDFLAGS)

server.o: server.c types.h
	$(CC) $(CFLAGS) -c server.c

server: server.o
	$(CC) server.o -o server  $(LDFLAGS)

clean::
	rm -f client server client.o server.o
