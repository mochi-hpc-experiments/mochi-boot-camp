# Mochi Boot Camp Session 4: Components

## abt-io

In order to execute this example, you must first install the `abt-io`
package from spack:

```
spack install abt-io
```

Now go into the abt-io example directory, compile, and run the example.  It
is based on the `sum` example from session 2, but modified so that the
output from each RPC handler is written to a file instead of to stdout.

```
cd ~/bootcamp/mochi-boot-camp/sessions/s4-components/abt-io
make
```

```
cd ~/bootcamp/mochi-boot-camp/sessions/s4-components/abt-io

# in one terminal:
./server
Server running at address na+sm://156045/0

# in the other terminal, run client with argument matching the address
#    shown above
./client na+sm://156045/0
```

After the example program has completed, you can check the directory in
which the server was executing for log files.

### exercise

## pmdk

In order to execute this example, you must first install the `pmdk`
package from spack:

```
spack install pmdk
```

Now go into the pmdk example directory, compile, and run the example.

```
cd ~/bootcamp/mochi-boot-camp/sessions/s4-components/pmdk
make
```

```
cd ~/bootcamp/mochi-boot-camp/sessions/s4-components/pmdk

# create an empty ramdisk file to emulate an NVRAM device
truncate -s 100M /dev/shm/foo.dat

# format it for libpmemobj use
pmempool create obj /dev/shm/foo.dat

# run the example program
./pmemobj-example /dev/shm/foo.dat

```
### Exercise

## Bake

In order to execute this example, you must first install the `bake`
package from spack:

```
spack install bake
```

There is no example to compile here; the initial Bake hands-on example uses
the demo Bake daemon and client programs installed with the bake package.


```
cd ~/bootcamp/mochi-boot-camp/sessions/s4-components/bake

# create a ramdisk file to use as the backend storage target for the Bake
#   server.  This tool creates a libpmemobj formated pmdk file, and then
#   adds additional metadata needed by Bake.

bake-mkpool -s 100M /dev/shm/bake.dat

# in one terminal, run the example Bake server daemon.  The arguments
#   tell the Bake daemon to use shared memory for communication, to store
#   data in the just-created target file, and store the network address of
#   the server in addr.dat.

bake-server-daemon na+sm:// /dev/shm/bake.dat -f addr.dat

# in another terminal, run a command line tool to copy an example file to
#    Bake.  The arguments specify the network address and target ID.
#    NOTE: the output of this command will display the name of a temporary
#    file that contains the Bake region id.

bake-copy-to example.dat `cat addr.dat` 1 1

# confirm that the data is present in the backend memory file

strings /dev/shm/bake.dat

```

### Exercise

