# bake hands-on exercise

The purpose of this example is to demonstrate usage of the Bake
microservice on Summit.  For information about local storage
(NVMe) on Summit compute nodes, see [the OLCF burst buffer
documentation](https://docs.olcf.ornl.gov/systems/summit_user_guide.html#burst-buffer), and for more information about the Bake microservice see the [Bake
project repository](https://xgitlab.cels.anl.gov/sds/bake).

## Prerequisites

Please set up the basic Mochi environment (with spack) on Summit following
the instructions in the top level hands on directory
'mochi-boot-camp/ecp-am-2020/sessions/hands-on/' before executing this
example.

## Concepts

This exercise demonstrates how to store data in a Bake microservice on
Summit. Bake is not normally intended for standalone use (it is more
appropriate as a constituent component of a higher-level service), but the
Bake package does include a standalone daemon and basic command
line utilities that will be used for demonstration purposes in this
exercise.

### Summit storage targets

As of this writing (February 2020), Bake primarily supports
[PMDK](https://pmem.io/pmdk/) as its storage back end.  PMDK is intended for
use with persistent memory devices, but for prototyping purposes it can be
executed atop RAM disk (e.g., tmpfs) or any memory mapped file (e.g., ext4 on a
conventional block device).  This is the mode that we will use on Summit,
using either the /dev/shm file system (RAM) or the /mnt/bb/$USER file system
(NVMe burst buffer).

The next official release of Bake will include a modular back-end storage
framework to enable bake implementatoins atop other storage abstractions,
and we have already demonstrated a proof-of-concept implementation atop
a conventional block-based file interface.  This upcoming back-end is
likely to be more efficient than the PMDK back-end on NVMe drives.

It is also important to note that PMDK 1.9 is the first version of PMDK with
experimental support for the POWER CPU architecture.  This is the only
version of PMDK that will work on Summit.

### Example overview

The examples in this exercise use two Summit compute nodes per job.
One initializes a storage target and runs a Bake server daemon.  The other
node copies an example file to a local burst buffer, and then from there
copies the data into the bake service over InfiniBand.  When the server
shuts down, it confirms that the example data was written into the back end
storage by using the `strings` command line utility to find the example
text.

## Hands-on example steps

Begin by compiling and loading the `bake` package from spack (assuming you
have already completed the basic setup steps on Summit):

```
$ spack install bake
$ spack load -r bake

# confirm that it was loaded
$ spack find --loaded
==> 9 installed packages
-- linux-rhel7-power9le / gcc@9.1.0 -----------------------------
argobots@1.0rc2  libfabric@1.9.0  margo@0.6.1     ncurses@6.1  rdma-core@20
bake@0.4         libuuid@2.23.2   mercury@master  pmdk@1.8
```

There are two job scripts in the bake hands-on directory to use as
demonstrations:

```
$ cd ~/bootcamp/mochi-boot-camp/ecp-am-2020/sessions/hands-on/bake
$ ls *.bsub
bake-nvme.bsub  bake-tmpfs.bsub
```

The former will run the bake microservice atop a local NVMe device on the
compute nodes, while the latter will use a ramdisk for storage.  Note that
both request NVMe resources (even in the tmpfs example, the input data is
first copied to local storage before being relayed to Bake).

Please edit both bsub scripts replace `CSC332` in the `#BSUB -P CSC332` line
with the name of your project allocation on Summit.

You can now execute either (or both) jobs as follows:

```
$ bsub -K bake-tmpfs.bsub      
Job <869676> is submitted to default queue <batch>.
<<Waiting for dispatch ...>>
<<Starting on batch1>>
<<Job is finished>>
```

Upon completion of the job, the output will be stored in `bake*.o*` files in
your working directory:

```
$ cat bake-tmpfs.o869676 
# created bake region 0:3820257222315503239:4056592
RID written to /tmp/bb-copy-rid.73vYiq
Provider 0 managing new target at multiplex id 1
PMEMOBJ
MEMORY_HEAP_HDR
Hello world.

------------------------------------------------------------
Sender: LSF System <lsfadmin@batch1>
Subject: Job 869676: <bake-tmpfs> in cluster <summit> Done

Job <bake-tmpfs> was submitted from host <login4> by user <carns> in cluster <summit> at Tue Feb  4 10:29:01 2020
Job was executed on host(s) <1*batch1>, in queue <batch>, as user <carns> in cluster <summit> at Tue Feb  4 10:29:59 2020
                            <42*h31n10>
                            <42*h35n17>
</ccs/home/carns> was used as the home directory.
</ccs/home/carns/bootcamp/mochi-boot-camp/ecp-am-2020/sessions/hands-on/bake> was used as the working directory.
Started at Tue Feb  4 10:29:59 2020
Terminated at Tue Feb  4 10:30:38 2020
Results reported at Tue Feb  4 10:30:38 2020

The output (if any) is above this job summary.

```

The important part of the output (in terms of confirming correct operation)
is the "Hello world." line.  This shows that the target data (a text file
containing that sentence) was correctly written into the Bake back-end.

Note that when Bake is used as a standalone service, there is no auxilliary
mechanism for exchanging metadata, like the network address of the bake
service or the identifier for the Bake region.  Both of these pieces of
information are instead written to local files for exchange between
processes in this demonstration.
