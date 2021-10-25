#!/usr/bin/python3

import sys

name = ""
added = -1
number = 0

i = 0
flag = 0
l_flag = 0
largest = 0
list_arg = sys.argv
name = list_arg[1]
added = int(list_arg[2], 10)

if name is "":
	print("Name does not exist")
elif added is -1:
	print("Numbers to be added does not exist")
else:
	my_file = open(name, "r")
	my_string = my_file.read()
	no_whitespace = ''.join(my_string.split())
	for elem in no_whitespace:
		value = int(elem,16)
		if i != added:
			number  = value + number
			i = i + 1
		else:
			if number > largest:
				largest = number
				l_flag = flag
			i = 1 
			flag = flag + added
			number = value
	print ("File: ", name, "\nThe increment is: ", added, "\nLargest value was: ", largest, end = " ")
	for elem in no_whitespace[l_flag:added+l_flag:1]:
		print (elem, end = "")
	print ()

