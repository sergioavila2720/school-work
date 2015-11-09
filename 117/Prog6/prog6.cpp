/*
# include <iostream>


using namespace std;

class cell{
	int info;
	cell *next;
	cell(int i) {info = i; next = this;}
	cell(int i, cell *n) {info = i; next = n;}
	friend class list;
	friend class NC_list;
};


class list{
	cell *rear;
	public:
		list() {rear = new cell(0);}
		~list() {while (!empty()) get();}
		int empty() {
			return rear == rear->next;
		}
	protected:
		void add(int);
		void push(int);
		int get();
};

void list::push(int x){
	rear -> next = new cell(x, rear -> next);
}
void list::add(int x){
	rear -> info = x;
	rear = rear -> next = new cell(0, rear -> next);
}
class queue: public list {
public:
	queue(){}
	int Qget() {return list::get();}
	void Qput(int x) {add(x);}

};

class stack: private list{
public:
	stack(){}
	int Spop() {return list::get();}
	void Spush(int x){list::push(x);}
	using list::empty;
};
*/

/* In this program we will implement a stack and queue using Non-circular lists.
we will be able to model a stack and a queue. For the stack we will be able to push and pop
using LIFO.
	For queue we will be using FIFO
  */

/* I will be running this code through the terminal in ubuntu.*/

# include <iostream>

using namespace std;

/* instead of using the cell class I made a struct to make things easier for me. */
struct node{
	int key;
	node *next1;
};
/*implenting the Non-circular list made two main pointers called rear and front 
in order to point to the end or the front of the list
I also made two more node pointers because I'm going to be creating new nodes in the push and add functions*/
class NC_list {
	node *rear, *front, *p = NULL, *np = NULL;

/*Just like in the circular list I have my public functions I will be using to push, add, and get*/
public:
	NC_list(): rear(NULL), front(NULL){} 
	void push(int);
	void add(int);
	int get();
	int empty() {
		return rear == rear->next1;
	}
};
/*add function for the queue*/
void NC_list::add(int x){

	np = new node; // first create a new node 
	np -> key = x; // have the new created node point to the key and store that in variable x
	np -> next1 = NULL;

	//this if statement will check if the list is empty and point to the right direction
	if(front == NULL){
		front = rear = np;
		rear -> next1 = NULL;
	}

	//if the list is not empty fill add elements to the corresponding pointer and keep pointing 
	else {
		rear -> next1 = np;
		rear = np;
		rear -> next1 = NULL;
	}
}

/*pish function for the stack*/
void NC_list::push(int x){	

	np = new node; // again create new node 
	np -> key = x; // again store key into x
	np -> next1 = front; // new node pointing to the next element is now going to be the front simulating LIFO
	front = np;
}

/*This function works for both stack and queue*/
int NC_list::get(){

		p = front; //this pointer is set equal to the front(ie. what ver is in the front is going to be deleted)
		int r = p -> key; // store whatever key is into r
		front = front -> next1;// keep pointing to the next node
		delete p; // delete current p
		return r; // return the deleted element
}

/*made a queue class that inherits from NC_list class this class is for using the add and get functions*/
class queue: public NC_list{
public:
	queue(){}
	void Qput(int x){NC_list::add(x);}
	int Qget() {return NC_list::get();}

};
/*This is the stack class that inherits from NC_list (pretty similar to queue) */
class stack: private NC_list{
public:
	stack(){}
	int Spop(){return NC_list::get();}
	void Spush(int x){NC_list::push(x);}
};

int main(){

	// queue and stack objects
	queue q1;
	stack s1;

	// adding to queue
	q1.Qput(1);
	q1.Qput(2);
	q1.Qput(3);
	q1.Qput(4);	
	
	// display the removed queue
	cout << q1.Qget() << endl;
	cout << q1.Qget() << endl;
	cout << q1.Qget() << endl;
	cout << q1.Qget() << endl;

	cout << endl;

	// pushing into stack
	s1.Spush(5);
	s1.Spush(6);
	s1.Spush(7);
	s1.Spush(8);

	// displaying the poped stack
	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;






	return 0;
}