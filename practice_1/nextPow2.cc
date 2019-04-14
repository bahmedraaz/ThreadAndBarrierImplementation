#include<stdio.h>
#include<iostream>

using namespace std;

unsigned int smallestPowerOf2(unsigned int n){
	unsigned count = 0;
	
	if(n && !(n & (n-1))) return n;
	while(n !=0){
		n >>= 1;
		count += 1;
	}
	return 1 << count;
}
	

unsigned nextPowerOf2(unsigned n){
	n = n-1;
	
	while(n & n - 1){
		n = n & n - 1;
	}
	
	return n << 1;
	
}


int main(int argc, char *argv[]){
	cout<<"nextPow2: "<<nextPowerOf2(atoi(argv[1]))<<endl;
	//cout<<"smallextPow2: "<<smallestPowerOf2(atoi(argv[1]))<<endl;
}
