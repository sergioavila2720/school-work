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
    matrix multiplication we will assign processes to different processors
    to try to achieve load balance. that way every processor will ge 
    around the same of tasks with a fair distribution. 
    ******************************************************/

  #include <iostream>
  #include <cstdlib>
  #include <pthread.h>
  #include <sys/time.h> //for macro GET_TIME(double)
  #include <iomanip>
  #include <time.h>

  using namespace std;

  #define GET_TIME(now)\
{ struct timeval t; gettimeofday(&t, NULL);\
  now = t.tv_sec + t.tv_usec/1000000.0; }

  int L, M, N;

  //global vars
  int num_threads; // get number of threads wanted
  pthread_mutex_t mutex1; // to lock the threads and achieve atomic operation
  double **mat_A, **mat_B, **mat_C; // variables for 2D dynamic arrays for matrix

  
  // Functions prototypes (Doumentations at Function declaration)
  double** create2dArray(int, int);
  void** initialize_A();
  void** initialize_B();
  void printMat(double**, int, int);
  void* multiply_mat(void*);

  int main(int argc, char* argv[]){


    double start, stop; // variables for checking the time
    GET_TIME(start); //time checking ends
    //clock_t begin=clock();

    long thread_id; // have to use long for when we type cast 
    num_threads = atoi(argv[1]);
    L = atoi(argv[2]);
    M = atoi(argv[3]);
    N = atoi(argv[4]);

    pthread_t myThreads[num_threads]; // define threads
    pthread_mutex_init(&mutex1, NULL); // initialize both mutex operations

    mat_A = create2dArray(L, M); 
    mat_B = create2dArray(M, N);
    mat_C = create2dArray(L, N);

    initialize_A(); /**/ cout << "\n" << endl; // pass matrix A and B 
    initialize_B(); /**/ cout << endl;

    for (thread_id=0; thread_id < num_threads; thread_id++){
      pthread_create(&myThreads[thread_id], NULL, multiply_mat, (void*)thread_id);      
      pthread_mutex_lock(&mutex1);   //reserve mutex1 for cout atomic operation
      cout<<"thread_"<<thread_id<<" is created"<<endl;
      pthread_mutex_unlock(&mutex1); //release mutex1
  }


    for (thread_id=0; thread_id < num_threads; thread_id++) // for loop to join the threads 
      pthread_join(myThreads[thread_id], NULL);

    // print the first 20 * 10
    cout << "Matrix C first_20 * first_10: \n " << endl;  
    printMat(mat_C, 20, 10); // call print function to print matrix C
    cout << endl;

    // made this for loop to print the last 20 * 10 of matrix C
    cout << "Matrix C last_20 * last_10: \n "<<endl;
    for (int i = L-20; i < L; i++){
      for (int j = N-10; j < N; j++){
        cout << mat_C[i][j] << " ";
      }
      cout << endl;
    }cout <<endl;

    GET_TIME(stop);
    cout<<"++++ time taken (sec) = "<< setprecision(8)<<showpoint <<   stop - start <<endl;
    pthread_mutex_destroy(&mutex1);    
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
  // initialize values to matrix A = i + j as stated in the assignment
  void** initialize_A(){
    for (int i = 0; i < L; i++){
      for (int j = 0; j < M; j++){
        //cout << i + j << " ";
        mat_A[i][j] = i + j; // i can initialize directly because Mat_A is global
      }
    }

  }
  // initialize values to matrix B = i + j + 1 as stated in the assignment
  void** initialize_B(){
    for (int i = 0; i < M; i++){
      for (int j = 0; j < N; j++){
        mat_B[i][j] = i + j + 1; // I can initialieze directly because Mat_B is global.    
      }
    }

  }
  // print the Result of multipliying matA and MatB
  void printMat(double** matrix, int rows, int columns){
    for (int i = 0; i < rows; i++){
      for (int j = 0; j < columns; j++){
        cout << mat_C[i][j] << " ";
      }
      cout << endl;
    } 
  }

  void* multiply_mat(void* rank){

    long my_rank = (long)rank; // get id
    int Q = L / num_threads; //Q
    int Remainder = L%num_threads; //R
    int my_first, my_last, my_count;

    if (my_rank < Remainder){
      my_count = Q + 1;
      my_first = my_rank * my_count;
    }
    else {
      my_count = Q;
      my_first = my_rank * my_count + Remainder; 
    }
    my_last = my_first + my_count - 1;

    double sum;
      for (int i = my_first; i <= my_last; i++){ // scan from i to rows of A
        for (int j = 0; j < N; j++){ // scan from j to columns of B
          sum = 0.0;
          for (int k = 0; k < M; k++){ // scan from k to columns of A
           sum = sum + (mat_A[i][k] * mat_B[k][j]); // multiply matrixA and matrixB
          }
        mat_C[i][j] = sum;
        }
      }    
    return NULL;
  }