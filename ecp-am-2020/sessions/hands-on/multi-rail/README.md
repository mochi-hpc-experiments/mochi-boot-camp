# Multi-Rail (mrail) on summit

The summit notes have two infiniband cards.  It's possible for one process to drive both of those cards, but we haven't figure it out yet.  Perhaps you can figure it out, or maybe you already know?

## Procedure

1. Observe inter-node performance.  libfabric ships with `fi_pingpong` but the libfabric maintainers prefer the `fi_rma` test from libfabric's 'fabtests'

2. switch to the `ofi_mrail` provider

3. The [`fi_mrail` man page][1] ( mentions several ways to express which ports to use.  See if you can figure out which one works on Summit
[1]https://ofiwg.github.io/libfabric/master/man/fi_mrail.7.html


## Resources

- The `fabtest.lsf` script shows how we ran the `fi_rma` test on summit between two nodes over one Infiniband link.  Probably a good starting point for your experiments
- Several ORNL people publshed a paper at SC 2019 about Summit's networking:  [https://dl.acm.org/doi/10.1145/3295500.3356166]
