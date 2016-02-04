# include <iostream>
# include <unistd.h>
# include <cstdlib>
# include <cstring>
# include <sys/wait.h> //for wait()
# include <sys/time.h> //for macro GET_TIME(double

using namespace std;
# define MAX 100

# define GET_TIME(now)\
{ struct timeval t; gettimeofday(&t, NULL);\
  now = t.tv_sec + t.tv_usec/1000000.0; }

int fibo_R(int n);
int fibo_I(int n);

int main(){

	
	int pid, status;
	int p1[2], p2[2], p3[2], p4[2];
	double start, stop;
	int Response_R, Response_I;
	int instring;

	pipe(p1); pipe(p2); pipe(p3); pipe(p4);

	for (int i =1; i < 4; i++){
		pid == fork();
		if (pid == 0 && i == 1){
			cout << "choose a number of the fibonacci sequence that you want to know in the Recursive way\n";
			cin >> Response_R;
			write(p1[1], &Response_R, sizeof(Response_R));
			write(p2[1], &Response_I, sizeof(Response_I));
			read(p3[0], /*time from child 2*/, sizeof(int));
			read(p4[0], /*time from child 3*/, sizeof(int));
			exit(0);

		}
		else if (pid ==0 && i == 2){
			read(p1[0], &Response_R, sizeof(Response_R));
			cout << "Fib number in the recursive way is: " << fibo_R(Response_R) << endl;
			write(p3[1], /*time to child 1*/, sizeof(int));
			exit(0);
		}
		else if (pid == 0 && i == 3){
			read(p2[0], &Response_I, sizeof(Response_I));
			cout << "Fib number in the iterative way is: " << fibo_I(Response_I) << endl;
			write(p4[1], /*time to child 1*/, sizeof(int))
			exit(0);
		}
	}

	for (int i = 1; i <4; i++){
		pid = wait(&status);
		cout << "child (pid = " << pid << ") exited status = " << status << endl;
	}
	

	
	return 0;
}

int fibo_R(int n){
	if (n == 1 || n == 2)
		return 1;
	else
		return (fibo_R(n-1) + fibo_R(n-2));
}

int fibo_I(int n){
	int prev, curr, next;
	prev = 1; curr = 1; next = 1;

	for (int i = 3; i <= n; i++){
		next = prev + curr;
		prev = curr;
		curr = next;
	}
	return next;
}

/*cout << fibo_R(50) << endl; // tested that my function worked 
	cout << fibo_I(4) << endl;
	*/