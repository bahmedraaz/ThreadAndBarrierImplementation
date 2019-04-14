from subprocess import call
import os
import random
import time
import sys

#command = "./main"
#renameCommand = "mv "+"inputFile.txt "+str(_)+".txt"
#rmCommand = "rm "+"inputFile.txt"

#for _ in range(10):

#print(sys.argv[1])

if len(sys.argv)<2:
	print("Please enter the number of inputs you want to generate");
else:
	numEntry = int(sys.argv[1])
	
limit = random.randint(1,50)
myList = []
for i in range(numEntry):
	myList.append(random.randint(1,limit))

with open("inputFile.txt", 'w') as inputFile:
	for i in myList:
		inputFile.write("%d\n"%i)

print("Expected Maximum:",max(myList))	
#os.system(command)
#print("*******")
#time.sleep(1)
#os.system("mv "+"inputFile.txt "+str(_)+".txt")
#os.system(rmCommand)
	
	
	
