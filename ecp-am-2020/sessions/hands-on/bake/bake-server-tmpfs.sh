#!/bin/bash

SCRATCHDIR="/gpfs/alpine/`echo $LSB_PROJECT_NAME | tr '[:upper:]' '[:lower:]'`/scratch/$USER"

bake-mkpool -s 100M /dev/shm/bake.dat

bake-server-daemon verbs:// /dev/shm/bake.dat -f $SCRATCHDIR/addr.dat -p

strings /dev/shm/bake.dat
