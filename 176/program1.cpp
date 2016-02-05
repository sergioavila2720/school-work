//Sergio Avila

/*In this program I will be using parallel programming to try to run two functions 
  simultaneously each on one processor. i will be using pipelining to implement this program*/
# include <iostream>
# include <unistd.h>
# include <cstdlib>
# include <cstring>
# include <sys/wait.h> //for wait()
# include <sys/time.h> //for macro GET_TIME(double

using namespace std;
#define GET_TIME(now)\
{ struct timeval t; gettimeofday(&t, NULL);\
  now = t.tv_sec + t.tv_usec/1000000.0; }

long unsigned int fibo_R(int n); // had to make these two long ints to be able to handle N= 47
long unsigned int fibo_I(int n); // even then it does not give me the right answer at 47

int main(){

	
	int pid, status; // initializing pid and status to keep track of child processes
	int p1[2], p2[2], p3[2], p4[2]; // seting variables for 4 pipes
	double start, stop, total, total2; // to keep track of time
	int Response = 47; 
	



	pipe(p1); pipe(p2); pipe(p3); pipe(p4); // initializing 4 pipes
	cout << "\nFibonacci number at " << Response << " is = " << fibo_I(Response) << "\n\n";
	for (int i =1; i <=3; i++){
		pid = fork();
		if (pid == 0 && i == 1){

			write(p1[1], &Response, sizeof(Response)); //writing the response into the first pipe going into child 2
			write(p2[1], &Response, sizeof(Response)); //writing the response into the second pipe going into child 3
			read(p3[0], &total, sizeof(total)); // reading the input from pipe 3
			read(p4[0], &total2, sizeof(total2)); // reading input from pipe 4
			cout << "Fibonacci function using Recursion took: " << total << " sec." << endl;
			cout << "Fibonacci function using Iteration took: " << total2 << " sec." << endl;			
			
			exit(0);

		}
		else if (pid ==0 && i == 2){
			GET_TIME(start); //time checking starts
			read(p1[0], &Response, sizeof(Response)); // read input from pipe one to be used in child 2
			fibo_R(Response); // take the response passed and pass it through fibo function
			GET_TIME(stop); //time checking ends
			total = stop - start; // get total time that function ran.
			write(p3[1], &total, sizeof(total));
			exit(0);
		}
		else if (pid == 0 && i == 3){
			GET_TIME(start); //time checking starts
			read(p2[0], &Response, sizeof(Response));
			fibo_I(Response);
			GET_TIME(stop); //time checking ends
			total2 = stop - start;
			write(p4[1], &total2, sizeof(total2));
			exit(0);
		}
	}

	for (int i = 1; i <=3; i++){
		pid = wait(&status); // wait called 3 times to wait for the child processes.
		cout << "child (pid = " << pid << ") exited status = " << status << "\n" <<  endl;
	}
	
	return 0;
}
// Fibonacci function using recursion
long unsigned int fibo_R(int n){
	if (n == 1 || n == 2)
		return 1;
	else
		return (fibo_R(n-1) + fibo_R(n-2));
}

// Fibonacci function using Iteration
long unsigned int fibo_I(int n){
	int prev, curr, next;
	prev = 1; curr = 1; next;
	if (n ==1 || n == 2){
			return 1;
	}
	else{
		for (int i = 3; i <= n; i++){

		next = prev + curr;
		prev = curr;
		curr = next;
		}
	}	
	return next;
}