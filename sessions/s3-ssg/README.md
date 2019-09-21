# Mochi Boot Camp Session 3: SSG

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

TODO: overview presentation
TODO: example