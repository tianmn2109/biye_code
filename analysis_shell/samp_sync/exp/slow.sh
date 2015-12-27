#!/bin/bash
awk '{ 
	if ($1 == -15) 
		print 1;
	else if ($1 == -14) 
		print 2; 
	else if ($1 == -13) 
		print 3; 
	else if ($1 == -12) 
		print 4; 
	else if ($1 == -11) 
		print 5; 
	else if ($1 == 15)
		print -1;
	else if ($1 == 14)
		print -2;
	else if ($1 == 13)
		print -3;
	else if ($1 == 12)
		print -4;
	else print $1;}' 1-3 > slow

