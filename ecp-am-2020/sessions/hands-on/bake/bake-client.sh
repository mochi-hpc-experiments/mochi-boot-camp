#!/bin/bash

SCRATCHDIR="/gpfs/alpine/`echo $LSB_PROJECT_NAME | tr '[:upper:]' '[:lower:]'`/scratch/$USER"

# wait for server to start
sleep 10

cp example.dat /mnt/bb/$USER/

bake-copy-to /mnt/bb/$USER/example.dat `cat $SCRATCHDIR/addr.dat` 1 1

bake-shutdown `cat $SCRATCHDIR/addr.dat`
