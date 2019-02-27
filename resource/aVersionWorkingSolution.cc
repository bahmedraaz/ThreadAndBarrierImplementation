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

void *printID(void *arg){
	int *p = (int*)arg;
	int a = *p;
	cout<<"Thread ID: "<<a<<endl;
	//cout<<"Hello"<<endl;
	pthread_exit(0);
}

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
	int totalThread;
	int count;
	
	public:
	void barrierInit(){
		sem_init(&mutex,0,1);
		sem_init(&waitq,0,0);
		sem_init(&hs,0,0);
		count = 0;
	}
	
	void wait(int a){
        sem_wait(&mutex);
		totalThread=a;
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

	cout<<"Round: "<<roundNumber<<endl<<"Num Thread in round "<<roundNumber<<" : "<<numThreadsThisRound<<endl;

	cout<<"ThreadID: "<<id<<endl<<"endOfEntryThisRound: "<<endOfEntryThisRound<<endl<<"comparePointerThisRound: "<<comparePointerThisRound<<endl<<endl;

	//if(roundNumber==3){
	//	cout<<"userINput[12]: "<<userInput[12]<<endl;
	//	cout<<"userInput[13]: "<<userInput[13]<<endl;
	//}
	
	//max[id] = userInput[2*id]>=userInput[2*id+1]?userInput[2*id]:userInput[2*id+1];
	max[id] = userInput[comparePointerThisRound+2*id]>=userInput[comparePointerThisRound+2*id+1]?userInput[comparePointerThisRound+2*id]:userInput[comparePointerThisRound+2*id+1];
	
	cout<<"****************************************"<<endl;
	cout<<userInput[comparePointerThisRound+2*id]<<endl;
	cout<<userInput[comparePointerThisRound+2*id+1]<<endl;
	
	cout<<"max["<<id<<"]: "<<max[id]<<endl;
	userInput[endOfEntryThisRound+id] = max[id];
	cout<<"userInput["<<(endOfEntryThisRound+id)<<"]: "<<userInput[endOfEntryThisRound+id]<<endl<<endl;
		

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
	
	cout<<"****Just befoer barrier******"<<endl;

	barrierObj.wait(numThreadsThisRound);

	cout<<"#####After coming out of Barrier_wait####"<<endl;
	if(newThreadInfo.threadID>=numThreadsThisRound/2){
		cout<<"Thread "<<newThreadInfo.threadID<<" exiting"<<endl;
		pthread_exit(0);
	}
	
	if(newThreadInfo.threadID<numThreadsThisRound/2){
		maxFinder((void *)&newThreadInfo);
	}
	

	//cout<<"hello bulbul"<<endl;
	pthread_exit(0);
	
	
}

	
int roundCalculator(int parameter)
{
  int result;
  result = log2 (parameter);
  return result;
}

// compute power of two greater than or equal to n
unsigned nextPowerOf2(unsigned n)
{
	// decrement n (to handle cases when n itself 
	// is a power of 2)
	n = n - 1;
	
	// do till only one bit is left
	while (n & n - 1)
		n = n & n - 1;	// unset rightmost bit
	// n is now a power of two (less than n)
	// return next power of 2
	return n << 1;
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
	int endOfEntry = nextPow2;
	int numRound = roundCalculator(nextPow2);

	cout<<"Number of Entry: "<<numEntry<<endl;
	cout<<"Next power of 2: "<<nextPow2<<endl;
	cout<<"Total round required: "<<numRound<<endl;
	
	//fill the slot between the last entry and next power of 2 with some large negative number;
	for(int i=numEntry; i<nextPow2; i++){
		userInput[i] = -99999999;
	}
	
	//for(int i=0; i<nextPow2; i++){
	//	cout<<"After filling with negative number: "<<userInput[i]<<endl;
	//}

	//The number of threads are half of the number of entries
	int numThreads = nextPow2/2;
	cout<<"Initial number of threads: "<<numThreads<<endl<<endl;

	int resultEntryPositionUserInput=nextPow2;
	int tempNextPow2 = nextPow2;

	for(int i=1; i<=numRound; i++){
		cout<<"resultEntryPositionUserInput: "<<resultEntryPositionUserInput<<endl;
		resultEntryPositionUserInput = resultEntryPositionUserInput + tempNextPow2/pow(2,i);
		//int tempNextPow2 =tempNextPow2/2;	
	}
	//cout<<"tempNumRound"<<tempNumRound<<endl;
	
//	for(int tempNumRound=numRound; tempNumRound!=1; tempNumRound=tempNumRound/2){
//		//tempNumRound = tempNumRound/2;
//		cout<<"tempNumRound: "<<tempNumRound<<endl;
//		resultEntryPositionUserInput += resultEntryPositionUserInput; 
//	}

	
	pthread_t *tID;
	tID = (pthread_t *)malloc(sizeof(pthread_t)*numThreads);
	
	struct threadInfo tData[numThreads];

	barrierObj.barrierInit();

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
	//for(int i=0; i<=(tData[0].endOfEntryThisRound+tData[0].numThreadsThisRound);i++){
	//	cout<<userInput[i]<<endl;
	//}

	//cout<<"****Final output******: "<<userInput[numEntry*2-2]<<endl;
	cout<<"*#*#*#*#*#: "<<tData[0].endOfEntryThisRound<<endl;
	cout<<"*#*#*#*#:"<<tData[0].numThreadsThisRound<<endl;
	cout<<"****Final output******: "<<userInput[resultEntryPositionUserInput-1]<<endl;
	//cout<<"****Final output******: "<<userInput[endOfEntry+(endOfEntry/2)]<<endl;
	//cout<<"asdfjasdlkfjasdl;fj: "<<resultEntryPositionUserInput<<endl;
	//for(int i=0; i<; i++){
	//	cout<<userInput[i]<<endl;
	//}
	cout<<"resultEntryPositionUserInput: "<<(resultEntryPositionUserInput-1)<<endl;

}

