# If you run the test.py, it will ask you the number of inputs and number of iterations
# This script will first create an input file named inputFile.txt having expected number of inputs and run the program for expected number of iteration iterations

from subprocess import call
import os
import random
import time
import sys

numInput = input("Enter the number of Inputs\n")
numInput = int(numInput)
numIteration = input("Please enter the number of iterations\n")
numIteration = int(numIteration)

command = "./max"
rmCommand = "rm "+"inputFile.txt"

#print(len(sys.argv))

#if len(sys.argv)<3:
#	print("Please enter the number of iteration as a command line argument\n")
#	exit()

#numIteration = int(sys.argv[2])
#numInput = int(sys.argv[1])

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
	
	
	
