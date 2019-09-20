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
cd /home/carns/bootcamp/mochi-boot-camp/sessions/s4-components/abt-io

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

### Exercise

