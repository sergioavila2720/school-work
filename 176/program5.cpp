/*
  Sergio Avila 
  05/01/2016
  program5.cpp
  Environment: I am using Linux operating system
               running this through the terminal
  compile and run:
  $> mpic++ -o xxx program5.cpp
  $> mpiexec -n 4 ./xxx 400

  In this Program I will be inplementing a version of the merge sort in parallel
  in this program i will make use of the MPI library. I'm going to be responsible 
  to send and receive information accordingly between processes so that each process 
  can perform their own tasks locally and at the end combining the results using 
  a tree reduction approach
  */

#include <mpi.h>      //For MPI functions, etc 
#include <iostream>
# include <cstdlib>
# include <sys/time.h>
using namespace std;

int compare(const void*, const void*);
int *mergeResult(int*, int, int*, int);

int main(int argc, char* argv[]){

  // variables i need initialized before initialize them
  srand (time(NULL));
  int n; // numbers for quicksort
  int my_rank, comm_sz, local_n; //communication size, id, and local numbers = n
  MPI_Init(NULL, NULL); // syntax for starting MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); //get my_rank among all
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); //get num processes from -n x

  // get n from command line and
  //broadcast process_0 to all other processes
  if (my_rank == 0){
  	n = atoi(argv[1]); // get problem size from command line
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);


  local_n = n / comm_sz; // each local size
  int *local_array = new int[n]; // dynamic array to user for the processes
  srand(my_rank + 1); // from notes to get random numbers in every process

  // fill the array with random numbers from 0 to 99
  for (int i = 0; i < local_n; i++){
  	local_array[i] = rand() % 100;
  }

  // other processes other than 0 send their information
  if(my_rank != 0)
  	MPI_Send(&local_array[0], local_n, MPI_INT, 0, 0, MPI_COMM_WORLD);

  // this gets and prints the info for process 0 
  if(my_rank == 0){
    cout << "Printing unsorted list:" <<endl;
    cout<<"process_0: ";
    for(int i = 0; i < local_n; i++)
      cout<<local_array[i]<<" ";
    cout<<endl;

    // also process 0 needs to receive the other processes info 
    // to print it before sorting it
    for(int i = 1; i < comm_sz; i++) {
      int *tempArray = new int[n]; // need this temp array because otherwise the local_array will be overriden
      MPI_Recv(&tempArray[0], local_n, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      cout<<"process_"<<i<<": ";
      for(int j = 0; j < local_n; j++)
        cout<<tempArray[j]<<" ";
      cout<<endl;
    }
  }

  // call built-in quicksort on local arrays.
  qsort(local_array, local_n, sizeof(int), compare);

  // from homework one. used HW1 key 
  int divisor = 2, core_difference = 1, partner;
  int size = local_n;
  int *glob_array = local_array;
  int done = 0;

  while (divisor <= comm_sz && done == 0){
  	if (my_rank % divisor == 0){
      partner = my_rank + core_difference;
      if(partner < comm_sz){
    		int *tempArray = new int[size];
        MPI_Recv(&tempArray[0], size, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        size *= 2; //double the size since merging two arrays
        glob_array = mergeResult(glob_array, size, tempArray, size);
      }
  	}
  	else{
  		partner = my_rank - core_difference;
      MPI_Send(&glob_array[0], size, MPI_INT, partner, 0, MPI_COMM_WORLD);
  		done = 1;// need to send
  	}
  	divisor *= 2;
  	core_difference *= 2;
  }

  if(my_rank == 0){
    cout<<endl;
    cout<<"Sorted list: "<<endl;
    for(int i = 0; i < size; i++){
      cout<<glob_array[i]<<" ";
    }
    cout<<endl;
  }
  MPI_Finalize(); //shut down MPI


  return 0;
}

int compare(const void* a_p, const void* b_p){
	int a = *(int* )a_p;
	int b = *(int* )b_p;
	if (a < b)
		return -1;
	else if (a == b)
		return 0;
	else 
		return 1;
}

int *mergeResult(int *arrA, int n, int *arrB, int m){
	int *arrtemp = new int[n + m];
    int i = 0, j = 0, k = 0;

    while (i <= n && j <= m){
        if (arrA[i] < arrB[j]){
            arrtemp[k] = arrA[i];
            i++;
        }else{
            arrtemp[k] = arrB[j];
            j++;
        }k++;

    }while (i < n){
        arrtemp[k] = arrA[i];
        i++;
        k++;
    }
    while (j < m){
        arrtemp[k] = arrB[j];
        j++;
        k++;
    }
    return arrtemp;

}
