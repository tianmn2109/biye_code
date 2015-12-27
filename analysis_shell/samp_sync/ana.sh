#!/bin/bash
tail -n 5000 1.txt > 11
tail -n 5000 3.txt > 33
awk '{print $6}' 11 > 111
awk '{print $6}' 33 > 333
paste  111 333 > dif 
awk '{print $1 - $2;}' dif > 1-3
