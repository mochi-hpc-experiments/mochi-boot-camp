#!/bin/bash

SCRATCHDIR="/gpfs/alpine/`echo $LSB_PROJECT_NAME | tr '[:upper:]' '[:lower:]'`/scratch/$USER"

bake-mkpool -s 100M /mnt/bb/$USER/bake.dat

bake-server-daemon verbs:// /mnt/bb/$USER/bake.dat -f $SCRATCHDIR/addr.dat

strings /mnt/bb/$USER/bake.dat
