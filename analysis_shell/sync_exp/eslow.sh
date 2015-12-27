#!/bin/bash
dir=`ls -F | grep "/$"`
for d in $dir
do
    echo $d
    cd $d
    ./slow.sh
    cd ..
done 
