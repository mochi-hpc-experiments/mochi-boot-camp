# Mochi Boot Camp: SSG

Please set up the basic Mochi environment (with spack) on Summit following the instructions in the top level hands on directory 'mochi-boot-camp/ecp-am-2020/sessions/hands-on/' before executing this example.

## Installing SSG

Run the following to download, compile, and install SSG:

```
spack install ssg
```

Note that this SSG installation depends on MPI, with Spack able to identify
an existing mpich installation on the system.

To load the SSG module, run the following:

```
spack load -r ssg
```

... and again inspect to confirm SSG, Margo, etc. modules have been loaded in
your environment with:

```
spack find --loaded
```

## Building and running the SSG example

As an example demonstration of SSG, we have developed a simple token ring
application. In this example, a group of processes is excecuted using MPI,
with the MPI communicator used to create an SSG group. Using group rank
information, a token is passed around the logical group ring, originating and
ending at rank 0. After each rank receives the forwarded token, it shuts down.

To build this example, run the following:

```
cd ~/bootcamp/mochi-boot-camp/sessions/s3-ssg/token-ring
make
```

Note that this builds 2 different executables: `ssg-token-ring1` and
`ssg-token-ring2`. `ssg-token-ring1` implements the token ring functionality
exactly as we have described so far: a single token is sent forward through the
ring, originating and ending at rank 0, with ranks shutting down on receipt
of the token (i.e., rank 1 shuts down first, rank 0 shuts down last).

Using your test system, try executing the example to confirm the behavior:

```
[snyder@it03 token-ring]$ mpirun -n 4 ./ssg-token-ring1
Member 0 forwarding token 48879 to 1
Member 1 got token 48879
Member 1 forwarding token 48879 to 2
Member 1 shutting down
Member 2 got token 48879
Member 2 forwarding token 48879 to 3
Member 2 shutting down
Member 3 got token 48879
Member 3 forwarding token 48879 to 0
Member 3 shutting down
Member 0 got token 48879
Member 0 shutting down
```

## Modifying the SSG example

Once comfortable with the initial token ring implementation, we will
attempt to modify its behavior. Rather than having all ranks shutdown
immediately on receiving the token, we will instead have rank 0 initiate
shutdowns in  reverse-order through the ring once it receives the token. So,
assuming 4 members, once rank 0 receives the initial token, it will tell rank 3
to shutdown, who will tell rank 2 to shutdown, etc., with rank 0 shutting down
last. This requires definition of a new Margo RPC as well as some additional
logic in the existing token forwarding RPC handlers.

Executable `ssg-token-ring2` fully implements this new functionality, which can
be confirmed by executing:

```
[snyder@it03 token-ring]$ mpirun -n 4 ./ssg-token-ring2
Member 1 got token 48879
Member 0 forwarding token 48879 to 1
Member 1 forwarding token 48879 to 2
Member 2 got token 48879
Member 2 forwarding token 48879 to 3
Member 3 got token 48879
Member 3 forwarding token 48879 to 0
Member 0 got token 48879
Member 0 forwarding shutdown to 3
Member 3 forwarding shutdown to 2
Member 3 shutting down
Member 2 forwarding shutdown to 1
Member 2 shutting down
Member 1 forwarding shutdown to 0
Member 1 shutting down
Member 0 shutting down

```

As an exercise, try extending the initial token ring example to account for this
new functionality by directly modifying `ssg-token-ring1.c` and
re-building/re-running. The source code contains `XXX` markers to help point
participants to the right locations in the code to modify. Participants are also
welcome to examine the `ssg-token-ring2.c` source directly for inspiration.

Let us know if you have any questions!
