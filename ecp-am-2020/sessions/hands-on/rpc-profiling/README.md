# Mochi Boot Camp: RPC profiling hands-on example

This directory contains a hands-on example of how to perform RPC-level
profiling of Mochi services.

## Prerequisites

Begin by running the initial example (the "sum" service, executed from an
interactive job) from `mochi-boot-camp/ecp-am-2020/sessions/hands-on/`.
This is the example service that we will instrument to demonstrate use of
the profiling tool chain.

## Concepts

The first step is to enable RPC instrumentation at runtime.  There are
some programmatic ways to do this (see the Margo documentation for details),
but the most straightforward method is to simply set the
`MARGO_ENABLE_PROFILING` environment variable for all participating
processes, including both clients and servers.

Margo will detect this environment variable and enable profiling.  In this
mode, data is collected over the duration of execution and then written to a
the current working directory for each process as it exits.

To use profiling in this mode, you must run your processes from a directory
that you have write access to.  Note that on Summit, home directories are
not writeable from compute nodes.  A more appropriate working directory
might be `/gpfs/alpine/<project>/scratch/<user>/`

Once your job is complete, you can then use a python script on the login
node to produce a summary pdf containing RPC performance graphs.  You
probably need to scp this pdf to your local machine to view it.

## Hands-on example steps

Follow the login, compilation, and job submission instructions from the
top-level hands-on example ("sum").  Once you have an interactive terminal,
do the following:

```
# cd to a writeable directory (as in the following example, except replace
#   the csc332 project name and carns user name
$ cd /gpfs/alpine/csc332/scratch/carns/

# run the same example as before, but with additional environment variables
#   set using the -E option to jsrun:
$ jsrun -n 1 -r 1 -g ALL_GPUS -E MARGO_ENABLE_PROFILING=1 ./server &

# don't forget to change the address argument for the client to be the
#   address string emmitted by the server
$ jsrun -n 1 -r 1 -g ALL_GPUS -E MARGO_ENABLE_PROFILING=1 ./client 'ofi+verbs;ofi_rxm://10.41.0.103:49201'

# exit job and return to login node
$ exit
```

On the login node:

```
# return to the directory that you were in when running the above example:
$ cd /gpfs/alpine/csc332/scratch/carns/

# load Python (Python 2 for now):
$ module load python/2.7.15-anaconda2-5.3.0

# generate a profile
$ margo-gen-profile
```

At this point you should have a file named `profile.pdf` that can be copied
to another computer and viewed.  There are multiple pages with different
plots comparing RPC performance.

This example is unusual in that there is only one RPC type observed (called
`sum`), and it was only executed 4 times.  This same work flow should work
for any Mochi example, however.

For more information see the [Margo instrumentation documentation](
https://xgitlab.cels.anl.gov/sds/margo/blob/master/doc/instrumentation.md).

