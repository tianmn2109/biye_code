#!/bin/bash

DIR=`ls -F ../algorithm | grep "/$"`
for d in $DIR
do
    awk '{print $NF}' ../algorithm/$d/result/conf1/result >> .t_${d/\//}
done
for d in $DIR
do
    echo $d, >> t
done
echo " " >> t

