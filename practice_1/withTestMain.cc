/*
CS 6456: Grad OS
Assignment 2
Name: Bulbul Ahmed

The following code implements a parallel binary reduction algorithm to find the maximum in a list of N numbers
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

//**** Testing ****
#include <fstream>
//****************

#define INPUT_ARRAY_SIZE 20000
#define RESULT_HOLDER_SIZE 20000

int *userInput;
int * result;

using namespace std;

struct threadInfo{
	int threadID;
	int initialThreadNumber;
	int numThreadsThisRound;
	int totalRound;
	int roundNumber;
	int endOfEntryThisRound;
	int comparePointerThisRound;
	int tailOfResultThisRound;
	int headOfResultThisRound;
};

class barrier{
	sem_t mutex;
	sem_t waitq;
	sem_t hs;
	sem_t throttle;
	int totalThread;
	int count;
	
	public:
	void barrierInit(int args){
		sem_init(&mutex,0,1);
		sem_init(&waitq,0,0);
		sem_init(&throttle,0,0);
		sem_init(&hs,0,0);
		count = 0;
		totalThread = args;
		
	}
	
	void wait(){
        sem_wait(&mutex);
		//totalThread=a;
        count++;
        if(count < totalThread){
            sem_post(&mutex);
            sem_wait(&waitq);
            sem_post(&hs);
            return;
        } else{
            for(int i=0; i < totalThread - 1; i++){
                sem_post(&waitq);
                sem_wait(&hs);
            }
            count = 0;
        }
		sem_post(&mutex);	
	}


	void waitTest(int a){
		sem_wait(&mutex);
			totalThread=a;
			count++;
		if(count==totalThread){
			sem_post(&throttle);
		}
		sem_post(&mutex);
		sem_wait(&throttle);
		sem_post(&throttle);
	}
};


barrier barrierObj;

void *maxFinder(void *arg){
	struct threadInfo *p = (struct threadInfo *)arg; //Casted the void type to int type pointer
	int id = p->threadID;
	int initialThreadNumber = p->initialThreadNumber;
	int numThreadsThisRound = p->numThreadsThisRound;
	int totalRound = p->totalRound;
	int roundNumber = p->roundNumber;
	int endOfEntryThisRound = p->endOfEntryThisRound;
	int comparePointerThisRound = p->comparePointerThisRound;
	int headOfResultThisRound = p->headOfResultThisRound;
	int tailOfResultThisRound = p->tailOfResultThisRound;
	int max[numThreadsThisRound];

	//printf("headOfResult: %d\n", headOfResultThisRound);
	//printf("Round number: %d\n", roundNumber);
	//cout<<"currentRoundNumber"<<roundNumber<<endl;
	//cout<<"threadID: "<<id<<endl;



	//To compare pair of entries and put the maximum in newly allocated array max[]. This section is executed by half of the threads than the previous round. The upper half will simply skip this section and go to the barrier and wait.
//	if(id < numThreadsThisRound){
//	max[id] = userInput[comparePointerThisRound+2*id]>=userInput[comparePointerThisRound+2*id+1]?userInput[comparePointerThisRound+2*id]:userInput[comparePointerThisRound+2*id+1];
//	
//	userInput[endOfEntryThisRound+id] = max[id];
//	}

	if(id < numThreadsThisRound){
		//cout<<"Round Number before if: "<<roundNumber<<endl;
		if(roundNumber==1){
			//cout<<"Round Number after if: "<<roundNumber<<endl;
//			printf("round: %d, thread %d entering into comparison\n",roundNumber, id);
			max[id] = userInput[2*id] >= userInput[2*id+1] ? userInput[2*id] : userInput[2*id+1];
	
			//printf("%d+%d = %d: \n",tailOfResultThisRound, id, tailOfResultThisRound+id);
			result[tailOfResultThisRound+id] = max[id];
//			printf("round: %d, result[%d]: %d\n",roundNumber,tailOfResultThisRound+id,result[tailOfResultThisRound+id]);
//			cout<<"round: "<<roundNumber<<" headOfResult: "<<headOfResultThisRound<<endl;
//			cout<<"round: "<<roundNumber<<" tailOfResult: "<<tailOfResultThisRound<<endl;
		}else{
			
//			printf("round: %d, thread %d entering into comparison\n",roundNumber, id);
			max[id] = result[headOfResultThisRound+2*id] >= result[headOfResultThisRound+2*id+1] ? result[headOfResultThisRound+2*id] : result[headOfResultThisRound+2*id+1];
			//cout<<"tailOfResult +"<<id<<" ="<<tailOfResultThisRound+id<<endl;
			result[tailOfResultThisRound+id] = max[id];
//			printf("round: %d, result[%d]: %d\n",roundNumber,tailOfResultThisRound,result[tailOfResultThisRound]);
//			cout<<"round: "<<roundNumber<<" headOfResult: "<<headOfResultThisRound<<endl;
//			cout<<"round: "<<roundNumber<<" tailOfResult: "<<tailOfResultThisRound<<endl;
		}
	}

//	if(id < numThreadsThisRound){
//		max[id] = userInput[2*id] >= userInput[2*id+1] ? userInput[2*id] : userInput[2*id+1];
//		printf("userInput[%d]: %d, userInput[%d]: %d\n",2*id, userInput[2*id], 2*id+1, userInput[2*id+1]);
//		userInput[id] = max[id];
//		printf("round %d, th[%d]: %d\n",roundNumber, id, userInput[id]);
//		
//					
//	}

	if(roundNumber==totalRound){
		return(NULL);
	}
	
	//preparing the thread info for next round
	struct threadInfo newThreadInfo;
	newThreadInfo.threadID = id;
	newThreadInfo.initialThreadNumber = initialThreadNumber;
	newThreadInfo.numThreadsThisRound = numThreadsThisRound/2;
	newThreadInfo.totalRound = totalRound;
	newThreadInfo.roundNumber = roundNumber+1;
	newThreadInfo.endOfEntryThisRound = endOfEntryThisRound+numThreadsThisRound;
	newThreadInfo.comparePointerThisRound = endOfEntryThisRound;
	newThreadInfo.headOfResultThisRound = tailOfResultThisRound;
	newThreadInfo.tailOfResultThisRound = tailOfResultThisRound + numThreadsThisRound;
	
	//Wait in the barrier untill all the threads arrive
	//cout<<"Entering Thread: "<<id<<" round: "<<roundNumber<<endl;
	//barrierObj.wait(numThreadsThisRound);
	//printf("Thread %d entering into the barrier\n", id);
	//printf("Round number before entering barrier: %d\n",newThreadInfo.roundNumber);
	barrierObj.wait();
	//cout<<"Exiting Thread: "<<id<<" round: "<<roundNumber<<endl;

	//if(newThreadInfo.threadID>=numThreadsThisRound/2){
		//cout<<"Thread "<<newThreadInfo.threadID<<" exiting"<<endl;
	//	pthread_exit(0);
	//}
	
	//if(newThreadInfo.threadID<numThreadsThisRound/2){
		maxFinder((void *)&newThreadInfo);
	//}
	

	//cout<<"hello bulbul"<<endl;
	pthread_exit(0);
	
	
}

//This function calculates the total number of round required based on the number of inputs	
int roundCalculator(int parameter)
{
  int result;
  result = log2 (parameter);
  return result;
}

// compute next smallest power of two greater than or equal to n. The idea is if user enters some number of inputs(n) that is not the power of two, then I will calculate the next most recent power of two that is >= n. Then I will fill the places between n and the next recent power of two with some big negative number. In this way I don't need to worry about if the user enters the number that is power of 2 or not. 
unsigned int nextPowerOf2(unsigned int n){
	unsigned count = 0;
	
	if(n && !(n &(n-1))) return n;
	
	while(n !=0){
		n >>= 1;
		count += 1;
	}
	return 1 << count;
}

//Main program starst here
int main(int argc, char *argv[]){
	userInput = (int*) malloc(sizeof(int)*INPUT_ARRAY_SIZE);
	result = (int *) malloc(sizeof(int)*RESULT_HOLDER_SIZE);
	

	string userInputString;
	int inputIndex = 0;
	int numEntry = 0;
	int nextPow2 = 0;
	
	//Wait for the user input
	/*while(1){
		
		getline(cin, userInputString);
		if(userInputString==""){
			break;
		}
		else{
			userInput[inputIndex] = atoi(userInputString.c_str());
			inputIndex++;
			numEntry++;
		}
	
	}*/

	//*********** Testing *****************
	ifstream myFileIn("inputFile.txt");
	int id;
		
	while(myFileIn >> id){
		userInput[inputIndex] = id;
		inputIndex++;
		numEntry++;
		//cout<<id<<endl;
	}

/*	for(int i=0; i<numEntry; i++){
		cout<<userInput[i]<<endl;
	}
*/
//******************** for debugging **********************
	int counter=0;
	for(int i=0; i<numEntry; i++){
		counter++;
//		printf("userInput[%d]: %d\n", i, userInput[i]);
	}

//*************** end of debugging code *******************

	nextPow2 = nextPowerOf2(numEntry); //find what is the next highest power of two based on the number of entries
//	cout<<"nextPow2: "<<nextPow2<<endl; //for debugging
	int endOfEntry = nextPow2;
	int numRound = roundCalculator(nextPow2);
	//cout<<"totalRoundRequired: "<<numRound<<endl; //for debugging

	
	//fill the slot between the last entry and next power of 2 with some large negative number;
	for(int i=numEntry; i<nextPow2; i++){
		userInput[i] = -9999;
	}

	
	//The number of threads are half of the number of entries
	int numThreads = nextPow2/2;
	//cout<<"numThreads: "<<numThreads<<endl; //for debugging

//********************* fro debugging **********************
	for(int i=0; i<nextPow2; i++){
		printf("userInput[%d]: %d\n", i, userInput[i]);
	}
	printf("counter: %d\n", counter);	
	printf("nextPow2: %d\n", nextPow2);
	printf("initial number of threads: %d\n", numThreads);
//**********************************************************
	
	int resultEntryPositionUserInput=nextPow2;
	int tempNextPow2 = nextPow2;

	// Calculate the position of final result in the userInput array
	for(int i=1; i<=numRound; i++){
		//cout<<"resultEntryPositionUserInput: "<<resultEntryPositionUserInput<<endl; //for debugging
		resultEntryPositionUserInput = resultEntryPositionUserInput + tempNextPow2/pow(2,i);
	}

	
	pthread_t *tID;
	tID = (pthread_t *)malloc(sizeof(pthread_t)*numThreads);
	
	struct threadInfo tData[numThreads];

	barrierObj.barrierInit(numThreads);
	
	//Here all the threads are created and send them with necessary argument to execute the function that finds the maximum number
	for(int i=0; i<numThreads; i++){
		//setting the value
		tData[i].threadID = i;
		tData[i].roundNumber = 1;
		tData[i].initialThreadNumber = numThreads;
		tData[i].numThreadsThisRound = numThreads;
		tData[i].totalRound = numRound;
		tData[i].endOfEntryThisRound = endOfEntry;
		tData[i].comparePointerThisRound = 0;
		tData[i].tailOfResultThisRound = 0;
		tData[i].headOfResultThisRound = 0;
		//tData.initialRound = 0;
		//cout<<"i: "<<i<<endl;
		pthread_create(&tID[i], NULL, maxFinder, &tData[i]); //create thread and send to execute maxFinder function with argument tData[i]
	}

	for(int i=0; i<numThreads; i++){
		pthread_join(tID[i], NULL); //join the threads with parent
	}
	
	//cout<<"         Maximum: "<<userInput[resultEntryPositionUserInput-1]<<endl; //print the maximum number
	//cout<<"         Maximum: "<<userInput[0]<<endl; //print the maximum number
	cout<<"         Maximum: "<<result[endOfEntry-2]<<endl; //print the maximum number
	/*cout<<"endOfEntry: \n"<<endOfEntry<<endl;
	for(int i=0; i<endOfEntry-1; i++){
		printf("result[%d]: %d\n", i, result[i]);
	}*/
	free(userInput);
	free(result);
}
