#!/usr/local/bin/python3

import re
import sys
from os import listdir
from os.path import isfile, join

def fileio(pattern, reg_file=[''], ignoreCase=True):
	filelist = list()
	total = 0
	if (reg_file == ''):
		filename = listdir('.')
		for reg_file in filename:
			if reg_file.endswith(".txt") or reg_file.endswith(".csv"):
				filelist.append(reg_file)
	else:
		filelist.append(reg_file)
	p = re.compile(pattern, re.IGNORECASE)
	for reg_file in filelist:
		if reg_file.endswith(".csv") or reg_file.endswith(".txt"):
			with open(reg_file) as f:
				line = f.readline()
				while line != '':
					if p.search(line):
						total = total + 1
					line = f.readline()
	return total

def Years19():
	pattern = "19\d\d"
	total = fileio(pattern,'',True)
	print("Years 1900-1999 total: ", total)

def Years1920():
	pattern = "(19|20)\d\d"
	total = fileio(pattern,'',True)
	print("Years 1900-2099 total: ", total)

def old():			
	pattern = "\\bold\\b"
	total = fileio(pattern, '',True)
	print("old total: ", total)

def Grade():
	pattern = "\"[A-D, F][\-?\+?]*\""
	total = fileio(pattern, '',True)
	print("Grades total: ", total)

def Vowel():				
	pattern = "\\b[aeiou][a-z]*it\\b"
	total = fileio(pattern,"wordlist.txt",True)

	print("beginning vowels with it at the end total: ", total)

def NotVowel():				
	pattern = "\\b[^aeiou][a-z]{4,}es\\b"
	total = fileio(pattern,"wordlist.txt",True)
	
	print("No vowels at the beginning with es at the end total: ", total)

def Mons():
	pattern = "\$\d*(.\d\d)?"
	total = fileio(pattern, '',True)
	print("total cash money: ", total)

Years19()
Years1920()
old()
Grade()
Vowel()
NotVowel()
Mons()

