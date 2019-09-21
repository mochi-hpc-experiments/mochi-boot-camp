# Mochi Boot Camp Session 3: SSG

TODO: overview presentation

Please refer back to documentation from
[Session 2: Hands-on](sessions/s2-hands-on/README.md) for general details on
logging onto JLSE, installing Mochi software, and running jobs. We will build
on your Spack environment from that session, additionally installing SSG before
attempting to run and modify an SSG example.

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
module list
```

## Building and running the SSG example

As an example demonstration of SSG, we have developed a simple token ring
application. In this example, a group of processes is excecuted using MPI,
with the MPI communicator used to create an SSG group. Using group rank
information, a token is passed around the ring, originating and ending at
rank 0. After each rank receives the forwarded token, it shuts down.

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
example output
```

## Modifying the SSG example
