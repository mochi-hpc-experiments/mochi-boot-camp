CFLAGS += `pkg-config --cflags margo ssg`
LDFLAGS += `pkg-config --libs margo ssg`

all:: ssg-token-ring

ssg-token-ring.o: ssg-token-ring.c
	mpicc $(CFLAGS) -c ssg-token-ring.c

ssg-token-ring: ssg-token-ring.o
	mpicc ssg-token-ring.o -o ssg-token-ring  $(LDFLAGS)

clean::
	rm -f ssg-token-ring ssg-token-ring.o
