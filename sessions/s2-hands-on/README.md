# Mochi Boot Camp Session 2: Hands-on

## Logging on to JLSE

Begin by SSH'ing to the JLSE login node:

```ssh login.jlse.anl.gov```

Make sure to use the ssh key that you set up when you registered for your
account.  Once you connect to login.jlse, it will present you with the
following prompt:

```
----------------------------------------------------------------------------

Duo Authentication Required.

----------------------------------------------------------------------------
Duo two-factor login for carns

Enter a passcode or select one of the following options:

 1. Duo Push to XXX-XXX-9834

Passcode or option (1-1):
```

Prese '1' to be prompted for MFA authentication from the Duo app on your
phone.

The login node is where you will generally be compiling and installing
software prior to running it on a compute node.  Importantly, the login node
has external Internet connectivity, but the compute nodes do not.

Next make a subdirectory for your bootcamp exercises and clone the bootcamp
git repository.

```
mkdir ~/bootcamp
cd ~/bootcamp
git clone https://xgitlab.cels.anl.gov/sds/mochi-boot-camp.git
```

At this point, you can continue following directions online or by looking at
this file in `~/bootcamp/mochi-boot-camp/session2/README.md`.

## Installing Spack

The easiest way to download and compile Mochi components is via the [Spack
package manager](https://spack.io/).  Begin by cloning the spack repository
from github, and then adding spack to your environment:

```
cd ~/bootcamp
git clone https://github.com/spack/spack.git
. ~/bootcamp/spack/share/spack/setup-env.sh
```

At this point, the `spack` command line tool is available to you, but we
need just a few more steps to configure it ideally for this envionment.

First run `spack compiler find` to see what compilers are available by
default:

```
[carns@jlselogin2 bootcamp]$ spack compiler find
==> Added 1 new compiler to /home/carns/.spack/linux/compilers.yaml
    gcc@4.8.5
==> Compilers are defined in the following files:
    /home/carns/.spack/linux/compilers.yaml
```

You can see in this case that it only found `gcc 4.8.5`, which will work,
but is quite old.  There are newer compilers available in the JLSE
environment that you can use in slack by putting them in your path and
re-running `spack compiler find`:

```
[carns@jlselogin2 bootcamp]$ export PATH="/soft/compilers/gcc/8.2.0/linux-rhel7-x86_64/bin/:$PATH"
[carns@jlselogin2 bootcamp]$ spack compiler find
==> Added 1 new compiler to /home/carns/.spack/linux/compilers.yaml
    gcc@8.2.0
==> Compilers are defined in the following files:
    /home/carns/.spack/linux/compilers.yaml
```

## Adding the Mochi software repository to Spack

Many of the Mochi software components have not yet been upstreamed to the
Spack package manager.  They are available as a separate software repository
that can be added to spack.

```
[carns@jlselogin2 bootcamp]$ cd ~/bootcamp
[carns@jlselogin2 bootcamp]$ git clone https://xgitlab.cels.anl.gov/sds/sds-repo.git
Cloning into 'sds-repo'...
remote: Enumerating objects: 268, done.
remote: Counting objects: 100% (268/268), done.
remote: Compressing objects: 100% (121/121), done.
remote: Total 1426 (delta 152), reused 191 (delta 102)
Receiving objects: 100% (1426/1426), 174.53 KiB | 0 bytes/s, done.
Resolving deltas: 100% (828/828), done.
[carns@jlselogin2 bootcamp]$ spack repo add sds-repo
==> Added repo with namespace 'sds'.
```

## Customizing Spack for the JLSE environment

At this point you can install any Mochi software component, but it will
likely download and compile more packages than are strictly necessary, which
is time consuming.  At this point we will install a `packages.yaml` file
that customizes Spack by informing if of system packages that it should
reuse, and specifies a subset of network transports to use for Mochi.

We have provided a pre-configured `packages.yaml` file for this purpose in
the JLSE environment.  It primarily does the following:

* informs Spack to use already available software for certain common system
  packages (Spack by default will build these packages itself)
* Configure the Mercury RPC package to not configure support for any
  external network fabric packages (we will just use shared memory for this
  week's experiments)

You can activate this configuration for your account by doing the following:

```
cp ~/bootcamp/mochi-boot-camp/session/s2-hands-on/packages.yaml ~/.spack/linux/
```

At this point you are ready to install and run Mochi software!

## General spack usage for package management

The following are the most important commands to know:

* `spack spec <package>` to see what will be installed if you were to
  install it (including dependencies and version numbers)
* `spack install <package>` to install a package
* `spack load -r <package>` to load the package into your environment
* `module list` to observe what modules you have loaded

## Setting up your profile to retain Spack and compiler settings

There are two critical commands that you will want to either run every time
you log into a JLSE node, or else add to your ~/.bashrc file so that they
are performed automatically.  We recommend the latter to save time:

```
cat ~/bootcamp/mochi-boot-camp/sessions/s2-hands-on/bashrc.mochi.jlse >> ~/.bashrc
```

Now when you log into JLSE moving forward you will have the correct compiler
and Spack command line tools available in your environment.

## Installing your first Mochi components

Run the following to download, compile, and install Margo:

```
spack install margo
```

This will take a few minutes and will install all of the necessary
dependencies, including Mercury and Argobots.  You can now load these
packages by running:

```
spack load -r margo
```

... and inspect to confirm that they are present in your environment with:

```
module list
```

## Compiling an example Mochi code

```
cd ~/bootcamp/mochi-boot-camp/sessions/s2-hands-on/sum
make
```

The above example is a very slightly modified (to use shared memory instead
of TCP for communication) copy of the [Sending arguments, returning
values](https://mochi.readthedocs.io/en/latest/margo/03_sum.html#) example
from the [Mochi Readthedocs
page](https://mochi.readthedocs.io/en/latest/index.html).

This will compile a simple client and server program, linked
against margo.  If you inspect the Makfile you will see pkg-config commands
that are used to find the correct CFLAGS and LDFLAGS for the build.

## running an interactive job on the JLSE compute nodes

The following command will allocate a single JLSE node for your use for 8
hours and open up an interactive terminal on that node.

```
qsub -I -n 1 -t 480 -q R.mochi_day1
```

NOTE: the "R.mochi_day1" part is the name of a reservation that we have in
place to guarantee node availability on the first day from 9am to 5pm.
There are identical reservations in place for day 2 and day 3 called
R.mochi_day2 and R.mochi_day3.

You are welcome to submit jobs after hours as well!  Just use `-q it` during
those times for the general availability queues for the nodes we are using.

Once the terminal is open, you can open additional terminals to that node
with ssh.  For example:

```
ssh it03
```

Note that you only have ssh access to nodes that you currently have
allocated, and your allocation will end once time expires or you log out of
the original interactive terminal that was created with the qsub command.

## Running an example Mochi code

```
cd /home/carns/bootcamp/mochi-boot-camp/sessions/s2-hands-on/sum

# in one terminal:
./server
Server running at address na+sm://156045/0

# in the other terminal, run client with argument matching the address
#    shown above
./client na+sm://156045/0
```

## TODO NOTES (TEMPORARY)

TODO: second example will be an exercise: copy the rpc, rename it, and
  modify it to do something else (increment an integer, for example)
