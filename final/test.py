#This script takes two arguments. For example, if you type "python test.py 10 2", it will first create an input file named inputFile.txt 
#having 10 inputs and run the program for 2 iteration

from subprocess import call
import os
import random
import time
import sys

command = "./max"
rmCommand = "rm "+"inputFile.txt"

#print(len(sys.argv))

if len(sys.argv)<3:
	print("Please enter the number of iteration as a command line argument\n")
	exit()

numIteration = int(sys.argv[2])
numInput = int(sys.argv[1])

for _ in range(numIteration):
	
	limit = random.randint(1,20000)
	myList = []
	for i in range(numInput):
		myList.append(random.randint(1,limit))
	
	with open("inputFile.txt", 'w') as inputFile:
		for i in myList:
			inputFile.write("%d\n"%i)

	print("Expected Maximum:",max(myList))	
	os.system(command)
	print("*******")
	time.sleep(1)
#	os.system(rmCommand)
	
	
	
