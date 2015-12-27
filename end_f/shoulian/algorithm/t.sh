#!/bin/bash
DIRS=`ls -F | grep "/$"`
for d in $DIRS
do
    awk '{print $NF}' $d/result/conf1/result > t_${d/\//}
done
TITLE=
TARGET=
for d in $DIRS
do
    TITLE=$TITLE" ${d/\//}"
    TARGET=$TARGET" t_${d/\//}"
done
echo $TITLE >> t
paste $TARGET >> t 
../result/produceXLS.py t tt.xls
