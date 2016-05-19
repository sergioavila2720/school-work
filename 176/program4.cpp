/*
  Sergio Avila 
  04/19/2016
  program4.cpp
  Environment: I am using Linux operating system
               running this through the terminal
  compile and run:
  $> g++ -fopenmp -o  program4 program4.cpp
  $> ./program4 number

  In this program I will be implementing the count algorithm.
  the trick to this program is that I will be doing it in a regular serial way
  and I will also try to implement a parallel version of the algorithm. So far we 
  have practiced many ways of making parallel programs; however, I will make use of the OpenMP
  in C++
  */

# include <iostream>
# include <omp.h>
# include <sys/time.h>
# include <cstdlib>

using namespace std;
#define GET_TIME(now)\
{ struct timeval t; gettimeofday(&t, NULL);\
  now = t.tv_sec + t.tv_usec/1000000.0; }

// global variables
int num_threads; // get number of threads wanted
int arrsize; // to get arr size from command line
double start, start2, start3, stop, stop2, stop3;
int *serial, *parallel1, *parallel2, *randNum;

// function prototypes
void serial_count_sort(int [], int);
void parallel_count_sort(int [], int);
void parallel_count_sort2(int [], int);
void display(int [], int);
int main (int argc, char* argv[]){

	// to get different random numbers everytime
	srand (time(NULL));
	num_threads = atoi(argv[1]);
	arrsize = atoi(argv[2]);

	// made this dynamic to get arrsize from command line
	// get random numbers from 0 to size of the array.
	randNum = new int[arrsize];
	for (int i = 0; i < arrsize; i ++){
		randNum[i] = rand() % arrsize;
	}
	//initialize array for serial sort algorithm
	serial = new int[arrsize];
	for (int i =0; i < arrsize; i++)
		serial[i] = randNum[i];

	// initialize array for parallel number 1
	parallel1 = new int[arrsize];
	for (int i =0; i < arrsize; i++)
		parallel1[i] = randNum[i];

	// initialize array for parallel number 2
	parallel2 = new int[arrsize];
	for (int i =0; i < arrsize; i++)
		parallel2[i] = randNum[i];


	cout << "List using Serial count sort" << endl;
	GET_TIME(start); //time checking starts
	serial_count_sort(serial, arrsize); // sort the array
	GET_TIME(stop); //time checking ends
	display(serial, arrsize);
	cout<<"++++ time taken in serial (sec) = "<< stop - start << "\n" << endl;

	cout << "***************************************************\n"  << endl;

	cout << "List using parallel 1" << endl;
	GET_TIME(start2); //time checking starts
	parallel_count_sort(parallel1, arrsize);
	GET_TIME(stop2); //time checking ends
	display(parallel1, arrsize);
	cout<<"++++ time taken in parallel_1 (sec) = "<< stop2 - start2 <<endl;

	cout << "***************************************************\n"  << endl;

	cout << "List using parallel 2" << endl;
	GET_TIME(start3); //time checking starts
	parallel_count_sort2(parallel2, arrsize);
	GET_TIME(stop3); //time checking ends
	display(parallel2, arrsize);
	cout<<"++++ time taken in parallel_2 (sec) = "<< stop3 - start3 <<endl;
	return 0;
}

void serial_count_sort(int a[], int n){
	int count;
	int *temp = new int[n];

	for (int i = 0; i < n; i++){
		count = 0;
		for (int j = 0; j < n; j++){
			if (a[j] < a[i])
				count ++;			
			else if (a[j] == a[i] && j < i)
				count ++;

		}
		temp[count] = a[i];
	}
	for (int i =0; i < n; i++){
		a[i] = temp[i];
	}
	free(temp);
}
//parallel version of the algorithm using openMP
//final memcpy is serial
void parallel_count_sort(int array[], int size){
  int count; 
  int *temp = new int[size];

  # pragma omp parallel for num_threads(num_threads) \
   	 shared(array, size, temp) private(count)
  for(int i = 0; i < size; i++){
    count = 0;
    for(int j = 0; j < size; j++){
      if(array[j] < array[i]) 
        count++;
      else if(array[j] == array[i] && j < i) 
        count++;
    }   
    temp[count] = array[i];
  }

  for(int k = 0; k < size; k++){
    array[k] = temp[k];
  }
  free(temp);
}
//parallel version of the algorithm using openMP
//final memcpy is parallel
void parallel_count_sort2(int array[], int size){
  int count; 
  int *temp = new int[size];

  # pragma omp parallel num_threads(num_threads) \
   	 shared(array, size, temp) private(count)
  for(int i = 0; i < size; i++){
    count = 0;
    for(int j = 0; j < size; j++){
      if(array[j] < array[i]) 
        count++;
      else if(array[j] == array[i] && j < i) 
        count++;
    }   
    temp[count] = array[i];
  }
  # pragma omp parallel num_threads(num_threads) \
   	 shared(array, size, temp)
  for(int k = 0; k < size; k++){
    array[k] = temp[k];
  }
  free(temp);
}

// function to display first 20 and last 20
void display(int array[], int size){
	cout << "First 20 numbers of the sorted array" << endl;
	for (int i =0; i < 20; i++)
		cout << array[i] << " ";
	cout << "\n" << endl;

	cout << "Last 20 numbers of the sorted array" << endl;
	for (int i = size- 20; i < size; i ++)
		cout << array[i] << " ";
	cout << "\n" << endl;
}

