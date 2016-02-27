/*
  Sergio Avila 
  02/26/2016
  program2.cpp
  Environment: I am using Linux operating system
               running this through the terminal
  compile and run:
  $> g++ -o program2 program2.cpp -pthread
  $> ./program2 number


  In this program we will be doing more practice in parallel programming 
  I will be using Pthreads to write a parallel program for 
    matrix multiplication    write more
    ******************************************************/

  #include <iostream>
  #include <cstdlib>
  #include <pthread.h>

  using namespace std;

  #define GET_TIME(now)\
{ struct timeval t; gettimeofday(&t, NULL);\
  now = t.tv_sec + t.tv_usec/1000000.0; }
  int L = 10;
  int M = 20;
  int N =1000; //I still dont know about this one.

  //global vars
  int num_threads; // get number of threads wanted
  pthread_mutex_t mutex1, mutex2; // to lock the threads and achieve atomic operation
  double **mat_A, **mat_B, **mat_C; // variables for 2D dynamic arrays

  // made a helper function that helps me pass a 2D dynamic array
  // to a function instead of initializing the three matrices at a time.
  double** create2dArray(int, int);


  int main(int argc, char* argv[]){

    double start, stop; // variables for checking the time
    long thread_id; // have to use long for when we type cast 
    num_threads = atoi(argv[1]);

    pthread_t myThreads[num_threads]; // define threads
    pthread_mutex_init(&mutex1, NULL); // initialize both mutex operations
    pthread_mutex_init(&mutex2, NULL);

    // initialize three 2D dynamic arrays 
    // matrix A, B, and C with it's respective dimensions.
    mat_A = create2dArray(L, M); 
    mat_B = create2dArray(M, N);
    mat_C = create2dArray(L, N);

/*
    // this is how you initialize 2D dynamic arrays directly
    mat_A = new double* [L];
    for (int i = 0; i < L; i++){
      mat_A[i] = new double[M];
    }  
    mat_B = new double* [M];
    for (int i = 0; i < L; i++){
      mat_B[i] = new double[N];
    }
    mat_C = new double* [L];
    for (int i = 0; i < L; i++){
      mat_C[i] = new double[N];
    }
    */

    return 0;
  }

  // made a helper function that helps me pass a 2D dynamic array
  // to a function instead of initializing the three matrices at a time.
  double** create2dArray(int rows, int cols) {
      double** array = new double*[rows];
      for (int i = 0; i < rows; i++) {
          array[i] = new double[cols];
      }
      return array;
    }