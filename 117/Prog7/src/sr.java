
//frame for the programming assignment 7.
//file name must be same as the main class name, i.e., sr.java
public class sr {
	static sr1 r1 = new sr1(); 
	  static sr2 r2 = new sr2();

	  public static void main(String args[]) 
	  {  
	    //create and start 6 threads for 6 concurrent processes
		  
		  process myThread1 = new process(1, r1, r2);
		  process myThread2 = new process(2, r1, r2);
		  process myThread3 = new process(3, r1, r2);
		  process myThread4 = new process(4, r1, r2);
		  process myThread5 = new process(5, r1, r2);
		  process myThread6 = new process(6, r1, r2);
		  myThread1.start();
		  myThread2.start();
		  myThread3.start();
		  myThread4.start();
		  myThread5.start();
		  myThread6.start();
		  
		 /* for (int i = 0; i < 6; i++){
			  process myThread[] = new process(i,r1,r2);
		  }
		  */

	   }
	} 
	 
	class sr1 //class for shared resource r1
	{ private static int avail1 = 3; 

	  //synchronized method cannot be interrupted 
	  //only one thread can access at a time 
	  public synchronized void acquire(int id) 
	  { 
	    
	   
	   } 
	         
	   //synchronized method cannot be interrupted 
	   //only one thread can access at a time 
	   public synchronized void release(int id) 
	   { 
	     

	    } 
	}//class sr1 
	 
	class sr2 //class for shared resource r2
	{ private static int avail2 = 2; 

	  //synchronized method cannot be interrupted 
	  //only one thread can access at a time 
	  public synchronized void acquire(int id) 
	  { 


	  } 
	         
	   //synchronized method cannot be interrupted 
	   //only one thread can access at a time 
	   public synchronized void release(int id) 
	   { 
	    

	    } 
	}//class sr2 
	 
	class process extends Thread //class name is "process"
	{ static sr1 r1; 
	  static sr2 r2;
	  private int id; 

	  public process(int k, sr1 r1, sr2 r2) //constructor 
	  { this.r1 = r1;
	    this.r2 = r2;
	  //  . . . . . . 
	   } 
	 
	   public void run() 
	   { 
	      //acquire r1 and r2 
	      //display "process-i is working"
	      //release r1 and r2
	    
	    } 
	   
}
