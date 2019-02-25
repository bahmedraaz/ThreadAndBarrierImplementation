#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>


using namespace std;

class barrier{
	sem_t mutex;

	public:
	void barrierInit(){
		sem_init(&mutex,0,1);
		int value;
		sem_getvalue(&mutex, &value);
		cout<<"Mutex value: "<<value<<endl;	
	}

};


barrier barrierObj;
int main(int argc, char*argv[]){
	barrierObj.barrierInit();
	//int value;
	//sem_getvalue(&mutex, &value);
	//cout<<"Mutex value: "<<value<<endl;
	return 0;
}
