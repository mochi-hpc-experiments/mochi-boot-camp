# libfabric-config hands-on exercise

The purpose of this example is to show how to query available
libfabric-compatible interfaces on Summit.

This directory contains a job script to query the libfabric configuration
on a single Summit compute node.  It can be executed as follows. First
load the Mochi software that you intend to use, modify fi\_info.bsub to charge to the correct project (-P), and then submit the job.  The 
-K argument instructs bsub to block until the job is complete.

```
[carns@login1.summit libfabric-config]$ spack load -r margo
[carns@login1.summit libfabric-config]$ bsub -K fi_info.bsub 
Job <861264> is submitted to default queue <batch>.
<<Waiting for dispatch ...>>
<<Starting on batch4>>
<<Job is finished>>
```

Note that execution of this job requires interaction with 3 nodes:
the login node (where you run bsub), a management node (where the
job script is executed), and a compute node (where jsrun spawns the
fi\_info utility).  The last of these is the critical one for HPC
compute job use.

Once the job is complete, you should have a `fi_info.o*` file
that shows the results.  The output should list all interfaces
on the Summit compute node that are compatible with libfabric as
configured by spack.  If you are using packages built following the
`mochi-boot-camp/ecp-am-2020/sessions/hands-on/` instructions, you should
see several entries for `verbs` interfaces and `verbs;ofi_rxm` interfaces.

