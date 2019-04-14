from subprocess import call
import os
import random
import time

command = "./main"
#renameCommand = "mv "+"inputFile.txt "+str(_)+".txt"
rmCommand = "rm "+"inputFile.txt"

for _ in range(10):
	
	limit = random.randint(1,2000)
	myList = []
	for i in range(5):
		myList.append(random.randint(1,limit))
	
	with open("inputFile.txt", 'a') as inputFile:
		for i in myList:
			inputFile.write("%d\n"%i)

	print("Expected Maximum:",max(myList))	
	os.system(command)
	print("*******")
	time.sleep(1)
	os.system("mv "+"inputFile.txt "+str(_)+".txt")
	#os.system(rmCommand)
	
	
	
