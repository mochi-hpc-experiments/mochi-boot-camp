# Access to JLSE systems for hands-on exercises

## What is the JLSE?

## Logging in

## Using compute nodes

You can get an interactive session on a compute node with the following
command:

`qsub -I -n 1 -t 480 -q it`

* -I means interactive; you will be presented with a command line prompt on
  your allocated node.  You can also ssh to it to open additional terminals.
  Your allocation will end if you log out of the primary terminal, however.
* -n 1 means that you are requesting a single node
* -t 480 means that you are requesting it for 8 hours (480 minutes)
* -q it means that you want to ues the "it" queue.  These are the nodes that
  we will use for all of this week's experiments

## Configuring the software environment

The "it" compute nodes are all running RedHat Enterprise Linux 7.5.  The
easiest way to set up Mochi environment is by building the components that
you need using `spack`.



