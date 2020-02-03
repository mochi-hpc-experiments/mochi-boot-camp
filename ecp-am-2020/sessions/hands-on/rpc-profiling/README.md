# Temporary notes

same as initial example (sum), except:

* run from a writeable directory on compute nodes, like `/gpfs/alpine/csc332/scratch/carns`
* Add `-E MARGO_ENABLE_PROFILING=1` to jsrun command line arguments
* after exiting job, generate profile.pdf as follows:

```
[carns@login5.summit carns]$ module load python/2.7.15-anaconda2-5.3.0
[carns@login5.summit carns]$ margo-gen-profile


*******************MARGO Profile Generator******************

Reading CSV files from: /gpfs/alpine/csc332/scratch/carns
Done.

************************************************************
```

* Copy resulting profile.pdf to your computer and look at it. This example from sum looks a little funny because there is only one rpc, but you can see how much time it took to execute.
