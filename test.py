from subprocess import call
import os
import random
import time

command = "./main"
rmCommand = "rm "+"inputFile.txt"

for _ in range(50):
	
	limit = random.randint(1,20000)
	myList = []
	for i in range(500):
		myList.append(random.randint(1,limit))
	
	with open("inputFile.txt", 'a') as inputFile:
		for i in myList:
			inputFile.write("%d\n"%i)

	print("Expected Maximum:",max(myList))	
	os.system(command)
	print("*******")
	time.sleep(1)
	os.system(rmCommand)
	
	
	
