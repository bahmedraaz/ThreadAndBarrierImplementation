from subprocess import call
import os
import random
import time

command = "./max"
rmCommand = "rm "+"inputFile.txt"

for _ in range(50):
	
	limit = random.randint(1,20000)
	myList = []
	for i in range(8192):
		myList.append(random.randint(1,limit))
	
	with open("inputFile.txt", 'w') as inputFile:
		for i in myList:
			inputFile.write("%d\n"%i)

	print("Expected Maximum:",max(myList))	
	os.system(command)
	print("*******")
	time.sleep(1)
#	os.system(rmCommand)
	
	
	
