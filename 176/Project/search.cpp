/*
  Sergio Avila 
  05/09/2016
  search.cpp
  Environment: I am using Linux operating system
               running this through the terminal
  compile and run:
  $> mpic++ -o xxx search.cpp
  $> mpiexec -n 1 ./xxx 

  In this final program I will try to put to use some of the things I
  learned in the csci 176 course. 
  I made a search algorithm to find a number between x and y for a
  value j such that f(j) = Z. 

*/
# include <iostream>
# include <stdlib.h>
# include <mpi.h> 
# include <ctime>
# include <sys/time.h>

using namespace std;
#define GET_TIME(now)\
{ struct timeval t; gettimeofday(&t, NULL);\
  now = t.tv_sec + t.tv_usec/1000000.0; }

// function prototypes
int search_serial(int, int, int);
int search_parallel(int, int, int, int, int);
int function(int);

int main(int argc, char *argv[]){

  // variables needed.
  // made different variables for parallel and serial because I
  // didnt want the numbers to change alues between seial and parallel
  int x_serial, y_serial, z_serial, result_serial;
  int x_parallel, y_parallel, z_parallel, result_parallel;
  int comm_size, my_rank;
  double start, start1, stop, stop1;


  // starting MPI
  MPI_Init(NULL, NULL); //start up MPI
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);//get number of process from -n x
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); //get my_rank among all

  // this are the numbers for a specific example.
  x_parallel = 1;
  y_parallel = 2100000000;
  z_parallel = 45;

  x_serial = 1;
  y_serial = 2100000000;
  z_serial = 45;

  // process 0 gets the info 
  if (my_rank == 0){
  cout << "\n";
  cout << "This will be the Parallel implememtation" << endl;
  cout << "X = " << x_parallel << endl;
  cout << "Y = " << y_parallel << endl;
  cout << "Z = " << z_parallel << endl;

  cout << "\n";
  cout << "This will be the Serial implememtation" << endl;
  cout << "X = " << x_serial << endl;
  cout << "Y = " << y_serial << endl;
  cout << "Z = " << z_serial << endl;
  }
  GET_TIME(start); //time checking starts
  result_parallel = search_parallel(x_parallel, y_parallel, z_parallel, my_rank, comm_size);
  GET_TIME(stop); //time checking ends

  GET_TIME(start1); //time checking starts
  result_serial = search_serial(x_serial, y_serial, z_serial);
  GET_TIME(stop1); //time checking ends


  if (result_parallel != -1){
    cout << "\n";
    cout << "Parallel Way" << endl;
    cout << "process " << my_rank << "found j = " << result_parallel << endl;
    cout << "verify f(j) = " << function(result_parallel) << endl;

    cout << "***********************************************" << endl;

  }

  if (result_serial != -1){
    cout <<endl;
    cout << "Serial way" << endl;
    cout << "found j in serial = " << result_serial << endl;
    cout << "verify f(j) = " << function(result_serial) << endl;
  }

  MPI_Finalize(); //shut down MPI

  // get the time in process 0
  if (my_rank == 0){
    cout<<"++++ time taken in serial (sec) = "<< stop1 - start1 << "\n" << endl;

    cout<<"++++ time taken in Parallel (sec) = "<< stop - start << "\n" << endl;

  }
  return 0;
}

// the function to get f(j) through x and y
int function(int n){
  int bigNum = 2100000000;
  int value = n;
  int k;

  for (int i = 1; i <= 5; i++){

      k = value/ 127773;
      value = 16807 * (value - k * 127773) - k * 2836;
      if(value <= 0){
        value += bigNum;
      }
  }
  return value;
}

// serial search for finding j 
int search_serial(int x, int y, int z){
  int function_of_i, j;

  for (int i = x; i <= y; i++){
    function_of_i = function(i);
    if(function_of_i == z){
      j = i;
      break;
    }
  }
  return j;
}

// parallel search for finding j 
int search_parallel(int x, int y, int z, int rank, int n_size){
  int function_of_i, j;

  for (int i = x + rank; i > 0 && i <= y; i += n_size){
    function_of_i = function(i);
    if (function_of_i == z){
      j = i;
      break;
    }
  }
  return j;
}