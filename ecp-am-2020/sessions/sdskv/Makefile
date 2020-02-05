CLIENT_CFLAGS += `pkg-config --cflags sdskv-client`
CLIENT_LDFLAGS += `pkg-config --libs sdskv-client`

SERVER_CFLAGS += `pkg-config --cflags sdskv-server`
SERVER_LDFLAGS += `pkg-config --libs sdskv-server`

all:: client server

client.o: client.c
	$(CC) $(CLIENT_CFLAGS) -c client.c

client: client.o
	$(CC) client.o -o client  $(CLIENT_LDFLAGS)

server.o: server.c
	$(CC) $(SERVER_CFLAGS) -c server.c

server: server.o
	$(CC) server.o -o server  $(SERVER_LDFLAGS)

clean::
	rm -f client server client.o server.o
