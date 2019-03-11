#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

#define INPUT_ARRAY_SIZE 1024

int *userInput;

using namespace std;

struct threadInfo{
	int threadID;
	int initialThreadNumber;
	int numThreadsThisRound;
	int totalRound;
	int roundNumber;
	int endOfEntryThisRound;
	int comparePointerThisRound;
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
	int max[numThreadsThisRound];

	//cout<<"currentRoundNumber"<<roundNumber<<endl;
	//cout<<"threadID: "<<id<<endl;



	//To compare pair of entries and put the maximum in newly allocated array max[]
	if(id < numThreadsThisRound){
	max[id] = userInput[comparePointerThisRound+2*id]>=userInput[comparePointerThisRound+2*id+1]?userInput[comparePointerThisRound+2*id]:userInput[comparePointerThisRound+2*id+1];
	
	userInput[endOfEntryThisRound+id] = max[id];
	}

	if(roundNumber==totalRound){
		return(NULL);
	}
	
	struct threadInfo newThreadInfo;
	newThreadInfo.threadID = id;
	newThreadInfo.initialThreadNumber = initialThreadNumber;
	newThreadInfo.numThreadsThisRound = numThreadsThisRound/2;
	newThreadInfo.totalRound = totalRound;
	newThreadInfo.roundNumber = roundNumber+1;
	newThreadInfo.endOfEntryThisRound = endOfEntryThisRound+numThreadsThisRound;
	newThreadInfo.comparePointerThisRound = endOfEntryThisRound;
	
	//Wait in the barrier untill all the threads arrive
	//cout<<"Entering Thread: "<<id<<" round: "<<roundNumber<<endl;
	//barrierObj.wait(numThreadsThisRound);
	barrierObj.wait();
	//cout<<"Exiting Thread: "<<id<<" round: "<<roundNumber<<endl;

	//We will reuse half the threads in next round and half of them will exit. The first if statement is for discarding half of the threads and the next if statement will check and send half of the threads to recursively call this same maxFinder function. 
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

	
int roundCalculator(int parameter)
{
  int result;
  result = log2 (parameter);
  return result;
}

// compute next smallest powe of two greater than or equal to n
unsigned int nextPowerOf2(unsigned int n){
	unsigned count = 0;
	
	if(n && !(n &(n-1))) return n;
	
	while(n !=0){
		n >>= 1;
		count += 1;
	}
	return 1 << count;
}


int main(int argc, char *argv[]){
	userInput = (int*) malloc(sizeof(int)*INPUT_ARRAY_SIZE);
	

	string userInputString;
	int inputIndex = 0;
	int numEntry = 0;
	int nextPow2 = 0;
	
	while(1){
		
		getline(cin, userInputString);
		if(userInputString==""){
			break;
		}
		else{
			userInput[inputIndex] = atoi(userInputString.c_str());
			inputIndex++;
			numEntry++;
		}
	
	}

	nextPow2 = nextPowerOf2(numEntry); //find what is the next highest power of two based on the number of entries
	//cout<<"nextPow2: "<<nextPow2<<endl;
	int endOfEntry = nextPow2;
	int numRound = roundCalculator(nextPow2);
	//cout<<"totalRoundRequired: "<<numRound<<endl;

	
	//fill the slot between the last entry and next power of 2 with some large negative number;
	for(int i=numEntry; i<nextPow2; i++){
		userInput[i] = -9999;
	}
	
	//The number of threads are half of the number of entries
	int numThreads = nextPow2/2;
	//cout<<"numThreads: "<<numThreads<<endl;

	int resultEntryPositionUserInput=nextPow2;
	int tempNextPow2 = nextPow2;

	// Calculate the position of final result in the userInput array
	for(int i=1; i<=numRound; i++){
		//cout<<"resultEntryPositionUserInput: "<<resultEntryPositionUserInput<<endl;
		resultEntryPositionUserInput = resultEntryPositionUserInput + tempNextPow2/pow(2,i);
	}

	
	pthread_t *tID;
	tID = (pthread_t *)malloc(sizeof(pthread_t)*numThreads);
	
	struct threadInfo tData[numThreads];

	barrierObj.barrierInit(numThreads);

	for(int i=0; i<numThreads; i++){
		tData[i].threadID = i;
		tData[i].roundNumber = 1;
		tData[i].initialThreadNumber = numThreads;
		tData[i].numThreadsThisRound = numThreads;
		tData[i].totalRound = numRound;
		tData[i].endOfEntryThisRound = endOfEntry;
		tData[i].comparePointerThisRound = 0;
		//tData.initialRound = 0;
		//cout<<"i: "<<i<<endl;
		pthread_create(&tID[i], NULL, maxFinder, &tData[i]);
	}

	for(int i=0; i<numThreads; i++){
		pthread_join(tID[i], NULL);
	}
	
	cout<<"Maximum: "<<userInput[resultEntryPositionUserInput-1]<<endl;
}
