#!/bin/bash
S=1
E=0
V=0
H=0
while getopts "hve:s:" Opt
do
	echo "Opt: $Opt : $OPTARG : $OPTIND"
	echo " "
	case $Opt in
	h)H=1;;
	v)V=1;;
	s)S=$OPTARG;;
	e)E=$OPTARG;;
	esac
done

shift $((OPTIND -1))

if [ $S == 1 ] && [ $E == 0 ] 
then
	H=1
fi

if [ $S -gt $E ]
then
	E=$S
fi



if [ $H == 0 ]
then
	echo 		     "-------------------------shell.bash--------------------"

	if [ $V == 1 ]
	then
		echo "Entering verbose mode"
	fi 

	for (( i= S; i<= E; i++))
	do
	./foo $i
		foo_out=$?
		if [ $foo_out == 0 ]
		then
			echo "$foo_out was successful" 
		elif [ $foo_out == 1 ]
		then
			echo "$foo_out : You did not supply a command line argument"
		elif [ $foo_out == 2 ]
		then
			echo "$foo_out : You supplied an invalid command line argument"
		else
			echo "$foo_out : The program failed for some other reason"
		fi
		if [ $foo_out != 0 ] && [ $V == 1 ]
		then
			echo "-------------The current number $i has failed us"
		fi
		echo " "        
	done
else
	echo "------------------------------Help Menu-------------------------------------"
	echo "Format of shell command: sh shell.bash -s(starting number) -e(ending number) or ./shell.bash -s(starting number) -e(ending number)" 
	echo "For help: sh shell.bash -h or ./shell.bash -h"
	echo "For verbose mode: sh shell.bash -v or ./shell.bash -v -s(starting number) -e(ending number)"
fi
	
	
