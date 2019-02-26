/*******Header Files************/
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <math.h>

/*******Constant Definition******/
#define INITIAL_SIZE 16354
#define BIG_NEGATIVE -123456789

using namespace std;


/***** Global Variables********/

int *input_array; //array that contains the input and intermediate results from each round
int num_of_threads; // initial number of threads
int updated_counter=0; // count of input after conversion to power of 2


/********* Barrier Class Implementation*******/
class Barrier
{
    sem_t mutex; //semaphore mutex
    sem_t waitq; //semaphore wait queue
    sem_t throttle; // semaphore throttle
    int limit; // number of threads to wait for
    int count; // count of number of threads waiting
    int mut_val,wait_val,thr_val; //variable declared for testing purpose


public:
    void Barrier_init(int b)
    {
        sem_init(&mutex,0,1); //initialize to 1
        sem_init(&waitq,0,0); //initialize to 0
        sem_init(&throttle,0,0); // initialize to 0
        limit =b; // set limit
        count =0; // reset count


    }
/****Wait Function
a: limit to check for each thread
b: thread_id
*******/
    void Wait(int a,int b)
    {
        sem_wait(&mutex);

        count++;
        if(count<a) //checking whether all the threads have joinedd
        {
            sem_post(&mutex);  // signal mutex
            sem_wait(&waitq);  // wait on waitqueue
            sem_post(&throttle); // signal throttle
        }

        else // if all the threads have arrived
        {
            for(int i=0; i<a-1; i++)
            {
                sem_post(&waitq); //signal waitqueue
                sem_wait(&throttle); // wait on throttle
            }
            count =0; // reset count
            sem_post(&mutex); // signal mutex

        }

    }

};

Barrier barrier; // barrier global object

/*****Structure Data Type for Passing Thread Data*******/
typedef struct Thread_Data
{
    int thread_id; // thread id
    int count_thread; //number of threads for this round
    int count_arr; //testing purpose
    int round_pow; //power of 2 for this round
    int round_pow_initial; //total updated counter increased to power of 2
    int prev_arr_pos; // array position to search from for comparision
    int thr_count_next; //testing
    //Barrier b;

} data;
/**thread run function***/
void* find_max(void *numbers)
{


    data *temp = (struct Thread_Data *)numbers;
    data new_tmp;
    int maximum =-123456;
    int th_id;

    int arr_pos=0;
    int prev_arr_pos =temp->prev_arr_pos;
    int round_pow= temp->round_pow;
    int count_thread  = temp->count_thread;


    for(int l=temp->round_pow_initial; l>=temp->round_pow; l--)
    {
        arr_pos+= pow(2,l); //position of the array to save
    }

    if(count_thread==0)
        return (NULL);


    th_id  = temp->thread_id;

    maximum = input_array[prev_arr_pos+(2*th_id)]>=input_array[prev_arr_pos+(2*th_id)+1]?input_array[prev_arr_pos+(2*th_id)]:input_array[prev_arr_pos+(2*th_id)+1]; // finding the maximum
    input_array[arr_pos+th_id]= maximum; // store in the array

     /// preparing parameters for next call
    new_tmp.thread_id = th_id;
    new_tmp.count_thread=count_thread/2;
    new_tmp.count_arr=temp->count_arr/2;
    new_tmp.round_pow = round_pow-1;
    new_tmp.round_pow_initial=temp->round_pow_initial;
    new_tmp.prev_arr_pos = arr_pos;
    // cout<< th_id<<" "<<count_thread<<endl;


    barrier.Wait(count_thread,th_id); // barrier wait for synchronization
	
	//debug
	cout<<"Thread_id: "<<th_id<<endl;
	//****
	
    if(th_id>=count_thread/2)
    {
        pthread_exit((void*)&new_tmp); //exit the second half of the array
    }

    if(th_id<count_thread/2)
    {

        find_max((void*)&new_tmp); // recursive call for first half

    }


}

int main()
{

    int i=0,counter,s,num_of_threads;


    input_array =(int*) malloc(INITIAL_SIZE*sizeof(int)); //memory allocation of the storage array
    string str;
	
	//debug
	//int ahmed=0;
	//****
	
    //cout << "Enter Integrs: Empty Line to break" << endl;
    /****** Take Input *****/
    while(1)
    {
        getline(cin,str);
        //cout<<str;
        if(str=="") break;
        else
        {
            input_array[i]= atoi(str.c_str());
            i++;
			//debug
			//ahmed++;
			//****
        }
		
    }

	//debug
	//for(int j=0; j<ahmed; j++){
	//	cout<<input_array[j]<<endl;
	//}
	//****
	
    counter = i;
	//debug
	cout<<"Counter: "<<counter<<endl;
	//****
	
    int ex;
    //int updated_counter =0;
    for(int k=0;; k++)
    {
		//debug
		//	cout<<"K outside the if: "<<k<<endl;
		//****
        if(counter <= pow(2,k))
        {
            updated_counter = pow(2,k);
			//debug
            cout<<"k inside the if: "<<k<<endl;
			cout<<"updated_counter: "<<updated_counter<<endl;
			//****
            ex=k;
            break;
        }


    }
	
	//debug
	cout<<"updated_counter: "<<updated_counter<<endl;
	cout<<"ex: "<<ex<<endl;
	//****
    //cout<<"Power:"<<ex<<endl;
    /****** Convert array input to power of two *******/

    for(int k=counter; k<updated_counter; k++)
    {
        input_array[k]= BIG_NEGATIVE;
		//debug
		cout<<"input_array"<<"["<<k<<"]: "<<input_array[k]<<endl;
		//****
    }

    num_of_threads = updated_counter/2;
    pthread_t *threads;
    threads= (pthread_t*)  malloc(sizeof(pthread_t)*num_of_threads);
    //sem_init(&arr_mutex,0,1);
    data *main_temp;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    data thread_data_array[num_of_threads];
    int round =0;

    i =0;

    //barrier initialization
    barrier.Barrier_init(num_of_threads);

    for(int j=0; j<updated_counter; j+=2)
    {
        thread_data_array[i].thread_id = i;
        thread_data_array[i].count_thread= num_of_threads;
        thread_data_array[i].count_arr = updated_counter;
        thread_data_array[i].round_pow=ex;
        thread_data_array[i].round_pow_initial=ex;
        thread_data_array[i].prev_arr_pos=0;
        //thread_data_array[i].b = barrier;
        pthread_create(&threads[i],&attr,&find_max,&thread_data_array[i]); //Thread Creation
        i++;

    }


    pthread_join(threads[0],(void**)&main_temp); // join the last thread
    //   }
    s=0;
    for(int l=ex; l>=1; l--)
    {
        s+= pow(2,l);
    }

    cout<<input_array[s]<<endl; // print output
    /*for(int i=0;i<32;i++)
    {
        cout<<input_array[i]<<endl;
    }*/
    free(input_array);
    return 0;
}
