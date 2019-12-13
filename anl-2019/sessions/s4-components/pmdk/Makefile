CFLAGS += `pkg-config --cflags libpmemobj`
LDFLAGS += `pkg-config --libs libpmemobj`

all:: pmemobj-example

pmemobj-example.o: pmemobj-example.c
	$(CC) $(CFLAGS) -c pmemobj-example.c

pmemobj-example: pmemobj-example.o
	$(CC) pmemobj-example.o -o pmemobj-example  $(LDFLAGS)

clean::
	rm -f pmemobj-example pmemobj-example.o
