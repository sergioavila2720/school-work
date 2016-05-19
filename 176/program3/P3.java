/////////////////
//////////////////////////////////////////////////////////////////
//// Sergio Avila
//// 03/28/2016
//// Environment: I am using Linux operating system
////              running this through the terminal
//// to run it:
//// $>javac P3.java
//// $>java P3 4 1001 2000 3000
////
//// In this program we will be doing more practice in parallel programming 
////  I will be using Pthreads to write a parallel program for 
////  matrix multiplication we will assign processes to different processors
////  to try to achieve load balance. that way every processor will ge 
////  around the same of tasks with a fair distribution. 
//////////////////////////////////////////////////////////////////////////////////

// main class; name be same as the file name (P3.java);
					  //in the single-file version, only main class is public;
public class P3
{
	// these are the "gobal" variables
	static globals glob = new globals();
	static communication comm = new communication();
	static int num_threads, l, m, n;// get number of threads wanted and matrix dimmensions
	static double startTime, endTime, totalTime;// variables for checking the time

	public static void main(String args[]){

		//get input from command line just like in c++
		num_threads = Integer.parseInt(args[0]);
		l = Integer.parseInt(args[1]);
		m = Integer.parseInt(args[2]);
		n = Integer.parseInt(args[3]);
		//// matrix A,B,C memory allocation here;
		glob.A = new double[l][m];
		glob.B = new double[m][n];
		glob.C = new double[l][n];
		//// matrix A initialization here; i + j
		for (int i = 0; i < l; i++){
			for (int j =0; j < m; j++){
				glob.A[i][j] = i + j;
			}
		}
		//// matrix B initialization here; i + j + 1
		for (int i = 0; i < m; i++){
			for (int j =0; j < n; j++){
				glob.B[i][j] = i + j + 1;
			}
		}
		Mythread[] mythread = new Mythread[num_threads]; // define threads
		startTime = System.currentTimeMillis();
		for (int k = 0; k < num_threads; k++){
			mythread[k] = new Mythread(k,glob, comm); //pass thread_id & shared_objs
			mythread[k].start();
		}

		// loop to join the threads 
		for (int i = 0; i < num_threads; i++){
			try{
				mythread[i].join();
			}
			catch(InterruptedException e){
				System.out.println("Thread interrupted.");
			}
		}
		totalTime = (endTime - startTime)/1000;
		System.out.println("++++ time taken (sec) = " + totalTime);
	}//main
}

class globals //shared_obj for keeping matrix A,B,C
{ 
public double[][] A; //A matrix declare
public double[][] B; //B matrix declare
public double[][] C; //C matrix declare
}

class communication //shared_obj for communication/synchronization
{
	private static int counter = 0;
	//mutex
	private static P3 p3 = new P3();
	public synchronized void increment(int id) 
	{
		counter++;
		//// if Thread_1
		////    check for wait() condition
		// in the wait and notify part we do not need to explicitly
		// synchronnize because the function handles it
		if (counter < p3.num_threads && id == 0){
			try{
				wait();
			}
			catch(InterruptedException e){
				System.out.println("Thread interrupted.");
			}
		}
		else if (counter == p3.num_threads){
			notify();
		}
	}
}


class Mythread extends Thread
{
	static globals glob;       //local for shared_ob
	static communication comm; //local for shared_obj
	private int id, myFirst, myLast, q, remainder, myCount;
	private P3 p3 = new P3();

	//contructor
	public Mythread(int k, globals glob, communication comm) 
	{
		//// assign parameters to locals here;
		this.glob = glob;
		this.comm = comm;
		this.id = k;
		remainder = p3.l % p3.num_threads;
		q = p3.l / p3.num_threads;
		// achieve balancing just like in the c++ version
		if (id < remainder){
			myCount = q + 1;
			myFirst = id * myCount;
		}
		else {
			myCount = q;
			myFirst = id * myCount + remainder;
		}
		myLast = myFirst + myCount - 1;

		System.out.println("Thread_"+ id);
	}

	public void run()
	{
		//// here matrix computation
		for (int i = myFirst; i <= myLast; i++){
			for (int j = 0; j < p3.n; j++){
				for (int k = 0; k < p3.m; k++){
					glob.C[i][j] += (glob.A[i][k] * glob.B[k][j]);
				}
			}
		}
		//// start/end index computation based on id
		//// increment done_counter in shared_obj comm;
		comm.increment(id);
		p3.endTime = System.currentTimeMillis();

		//// when all threads are done, display resulting C matrix in Thread_1
		if (id == 0){
			System.out.println("Matrix C first_20 * first_10" + "\n\n");
			printFirst20(20, 10);
			System.out.println("\n");
			System.out.println("Matrix C Last_20 * Last_10" + "\n");
			printLast20(1001, 3000);			
		}
	}//run
	// this prints the first 20 and last 10
	public void printFirst20(int row, int col){
		for (int i = 0; i < row; i++){
				for (int j = 0; j < col; j++){
					System.out.print(glob.C[i][j] + " ");
				}System.out.println();
			}
	}
	// this prnts the last 20 and last 10 
	public void printLast20(int row, int col){
		for (int i = row - 20; i < row; i++){
				for (int j = col - 10; j < col; j++){
					System.out.print(glob.C[i][j] + " ");
				}System.out.println();
			}
	}
}//class mythread


