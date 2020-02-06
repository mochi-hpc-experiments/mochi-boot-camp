CFLAGS += `pkg-config --cflags margo ssg`
LDFLAGS += `pkg-config --libs margo ssg`

all:: ssg-token-ring2 ssg-token-ring1

ssg-token-ring2.o: ssg-token-ring2.c
	mpicc $(CFLAGS) -c ssg-token-ring2.c

ssg-token-ring2: ssg-token-ring2.o
	mpicc ssg-token-ring2.o -o ssg-token-ring2 $(LDFLAGS)

ssg-token-ring1.o: ssg-token-ring1.c
	mpicc $(CFLAGS) -c ssg-token-ring1.c

ssg-token-ring1: ssg-token-ring1.o
	mpicc ssg-token-ring1.o -o ssg-token-ring1 $(LDFLAGS)

clean::
	rm -f ssg-token-ring2 ssg-token-ring1 ssg-token-ring2.o ssg-token-ring1.o
