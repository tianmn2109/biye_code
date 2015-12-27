#!/bin/bash
EXAMPLESDIR=`ls  ../examples`
Dirs=`ls -F | grep "/$"`
for dir in $Dirs
do
	cd $dir
	echo "Entry directory " $PWD

	make
	pro=`ls *.cc`

	if [ -d result ]
	then
		echo "Removing directory $PWD/result"
		rm -rf ./result
	fi
	echo "Creating directory $PWD/result"
	mkdir result


	for d in $EXAMPLESDIR
	do
		echo "Making directory for result " $d
		mkdir ./result/$d
		echo "Making result file $PWD/result/$d/$d"
		touch ./result/$d/result
		for f in `ls -rt ../../examples/$d`
		do
		echo "./${pro/.cc/} $PWD/../../examples/$d/$f >> ./result/$d/result"
		./${pro/.cc/} $PWD/../../examples/$d/$f >> ./result/$d/result
		done
	done

	echo "Leaving directory " $PWD
	cd ..
done
