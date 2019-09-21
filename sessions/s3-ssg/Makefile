CFLAGS += `pkg-config --cflags margo ssg`
LDFLAGS += `pkg-config --libs margo ssg`

all:: ssg-token-ring-solved

ssg-token-ring-solved.o: ssg-token-ring-solved.c
	mpicc $(CFLAGS) -c ssg-token-ring-solved.c

ssg-token-ring-solved: ssg-token-ring-solved.o
	mpicc ssg-token-ring-solved.o -o ssg-token-ring-solved  $(LDFLAGS)

clean::
	rm -f ssg-token-ring-solved ssg-token-ring-solved.o
