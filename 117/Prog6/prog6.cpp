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
/*

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

void NC_list::push(int x){
	front -> next = new cell(x,front -> next);

}

void NC_list::add(int x){
}

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
	int Qget() {return NC_list::get();}
	void Qput(int x) {add(x);}

};

class stack: private list{
public:
	stack(){}
	int Spop() {return NC_list::get();}
	void Spush(int x){NC_list::push(x);}
	using NC_list::empty;
};
*/

struct node{
	int key;
	node *next1;
};

class NC_list {
	node *rear, *front, *p = NULL, *np = NULL;

public:
	NC_list(): rear(NULL), front(NULL){} 
	void push(int);
	void add(int);
	int get();
	int empty() {
		return rear == rear->next1;
	}
};

void NC_list::add(int x){

	np = new node;
	np -> key = x;
	np -> next1 = NULL;

	if(front == NULL){
		front = rear = np;
		rear -> next1 = NULL;
	}

	else {
		rear -> next1 = np;
		rear = np;
		rear -> next1 = NULL;
	}
}

void NC_list::push(int x){	

	np = new node;
	np -> key = x;
	np -> next1 = front;
	front = np;



/*
	np = new node;
	np -> key = x;

	if (rear)
		np -> next1 = rear;
	else{
		np -> next1 = NULL;
		front = np;
	}

	front = np;
	*/

}
int NC_list::get(){

		p = front;
		int r = p -> key;
		front = front -> next1;
		delete p;
		return r;
}

class queue: public NC_list{
public:
	queue(){}
	void Qput(int x){NC_list::add(x);}
	int Qget() {return NC_list::get();}

};

class stack: private NC_list{
public:
	stack(){}
	int Spop(){return NC_list::get();}
	void Spush(int x){NC_list::push(x);}
};

int main(){

	queue q1;
	stack s1;
	q1.Qput(1);
	q1.Qput(2);
	q1.Qput(3);
	q1.Qput(4);	
	
	cout << q1.Qget() << endl;
	cout << q1.Qget() << endl;
	cout << q1.Qget() << endl;
	cout << q1.Qget() << endl;

	cout << endl;

	s1.Spush(5);
	s1.Spush(6);
	s1.Spush(7);
	s1.Spush(8);

	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;






	return 0;
}