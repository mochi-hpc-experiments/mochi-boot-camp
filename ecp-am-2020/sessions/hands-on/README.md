# Mochi Boot Camp: Hands-on

## Logging on to Summit

```ssh username@summit.olcf.ornl.gov```

Summit requires two-factor authentication.  Hopefully you have a token from Oak
Ridge.  If you do not, and you are reading this on 6 February at the boot camp
for the first time, you will have to find another machine to work on.

You'll log in with your pin + one-time pass code.

Make a directory for this bootcamp:

```
mkdir ~/bootcamp
cd ~/bootcamp
git clone https://xgitlab.cels.anl.gov/sds/mochi-boot-camp.git
```
## Installing spack

The easiest way to download and compile Mochi components is via the [Spack
package manager](https://spack.io/).  Begin by cloning the spack repository
from github, and then adding spack to your environment:

```
cd ~/bootcamp
git clone https://github.com/spack/spack.git
cd spack
. ~/bootcamp/spack/share/spack/setup-env.sh
```

At this point, the `spack` command line tool is available to you, but we
need just a few more steps to configure it ideally for this envionment.

Summit has a minimal base environment, and relies on `modules`
(http://lmod.readthedocs.org) to populate the environment.

We'll load a recent gcc compiler and teach spack about it:

```
[robl@login1]~% module load gcc/9.1.0
[robl@login1]~% spack compiler find
==> Added 1 new compiler to /home/robl/.spack/linux/compilers.yaml
    gcc@9.1.0
==> Compilers are defined in the following files:
    /ccs/home/robl/.spack/linux/compilers.yaml
```

Other compilers, such as Intel, IBM's XL, or PGI compilers might work for some
or all of the Mochi components, but we know gcc-9 supports the language
features used by any of our components.

## Adding the Mochi software repository to Spack

Many of the Mochi software components have not yet been upstreamed to the
Spack package manager.  They are available as a separate software repository
that can be added to spack with the `spack repo add` command:

```
[robl@login1]~% cd ~/bootcamp
[robl@login1]~/bootcamp% git clone https://xgitlab.cels.anl.gov/sds/sds-repo.git
...
[robl@login1]~/bootcamp% spack repo add sds-repo
[robl@login1]~/bootcamp%
==> Added repo with namespace 'sds'.
```
## Customizing Spack for ORNL/Summit environment

At this point you can install any Mochi software component, but it will
likely download and compile more packages than are strictly necessary, which
is time consuming.  At this point we will install a `packages.yaml` file
that customizes Spack by informing it of system packages that it should
reuse, and specifies a subset of network transports to use for Mochi.

We have provided a pre-configured `packages.yaml` file for this purpose
that is already tailored to the Summit environment. It primarily does
the following:

* informs Spack to use already available software for certain common system
  packages (Spack by default will build these packages itself)
* Configure the Mercury RPC package to use the Infiniband 'verbs' interface for our
  external network fabric package 'libfabric'

You can activate this configuration for your account by doing the following:

```
cp ~/bootcamp/mochi-boot-camp/ecp-am-2020/sessions/hands-on/packages.yaml ~/.spack/linux/
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
you log into a Summit node, or else add to your ~/.bashrc file so that they
are performed automatically.  We recommend the latter to save time:

```
cat ~/bootcamp/mochi-boot-camp/ecp-am-2020/sessions/hands-on/bashrc.mochi.summit >> ~/.bashrc
```

Now when you log into Summit moving forward you will have the correct compiler
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
# re-load spack env to make sure that it finds new packages
. ~/bootcamp/spack/share/spack/setup-env.sh
spack load -r margo
```

... and inspect to confirm that they are present in your environment with:

```
module list
```

## Compiling an example Mochi code

```
cd ~/bootcamp/mochi-boot-camp/ecp-am-2020/sessions/hands-on/sum
make
```

The above example is a very slightly modified (to use Infiniband instead
of TCP for communication) copy of the [Sending arguments, returning
values](https://mochi.readthedocs.io/en/latest/margo/03_sum.html#) example
from the [Mochi Readthedocs
page](https://mochi.readthedocs.io/en/latest/index.html).

This will compile a simple client and server program, linked
against margo.  If you inspect the Makfile you will see pkg-config commands
that are used to find the correct CFLAGS and LDFLAGS for the build.

## Running an interactive job on Summit compute nodes

Summit uses the LSF job scheduler plus some job managment utilites specific to
summit.  One requests an allocation of nodes with `bsub` and runs a program in
that allocation with `jsrun`

To get an interactive allocation for 15 minutes:
```
$ bsub -Is -W 0:15 -nnodes 2  -P CSC332 $SHELL
Job <823109> is submitted to default queue <batch>.
<<Waiting for dispatch ...>>
<<Starting on batch1>>
[\u@batch1.summit \W]\$ 
```

You'll need to replace `-P CSC332` with the name of whichever project you're using.

The job scheduler will drop you into an interactive login on one of the
"monitor" nodes.  Don't run jobs here directly.  Instead, we'll use `jsrun`.

Note that we asked for two nodes from the scheduler.  We'll run one service on
one node in the background, then run the client on the other.

First, start the server.  The job will inherit your environment, so if you
loaded modules and built the client and server, things should work ok.


```
# no need to change directories: interactive bsub drops you in the directory
# from which you executed it
$ jsrun -n 1 -r 1 -g ALL_GPUS ./server &
[1] 129086
Server running at address ofi+verbs;ofi_rxm://10.41.0.103:49201
```

```

Next, run the client, passing it the address string given to you by the server.

```
[\u@batch1.summit \W]\$ jsrun -n 1 -r 1 -g ALL_GPUS ./client 'ofi+verbs;ofi_rxm://10.41.0.103:49201'
Computed 42 + 43 = 85
Computed 44 + 45 = 89
Computed 46 + 47 = 93
Computed 48 + 49 = 97
Got response: 42+43 = 85
Got response: 44+45 = 89
Got response: 46+47 = 93
Got response: 48+49 = 97
[1]  + done       jsrun -n 1 -r 1 -g ALL_GPUS ./server
[\u@batch1.summit \W]\$ exit
```

## Additional resources

- https://docs.olcf.ornl.gov/systems/summit_user_guide.html

