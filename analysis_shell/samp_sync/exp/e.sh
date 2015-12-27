#!/bin/bash
dir=`ls -F | grep "/$"`
for d in $dir
do
    echo $d
    cd $d
    ./ana.sh
    ./slow.sh
    ./count.sh
    ./matlab.sh
    ./change.sh
    cd ..
done 
