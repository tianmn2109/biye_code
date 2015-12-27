#!/bin/bash
awk '{print NR, $1}' slow > expData
awk '{T[$1] ++;}; END{for (i in T) print i " = " T[i]}' slow | sort -nk 1 > t
