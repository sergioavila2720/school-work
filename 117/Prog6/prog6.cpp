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
struct node{
	int key;
	node *next1;
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
class NC_list{
	cell *rear;
	cell *front;
public:
	NC_list(){front = new cell(0);}
	~NC_list(){while (!empty()) get();}
	int empty(){
		return front == rear ->next;
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

int NC_list::get(){
	if(empty() )
		return 0;
	cell *front = rear ->next;
	rear -> next = front -> next;
	int x = front -> info;
	delete front;
	return x;
}

class queue: public NC_list {
public:
	queue(){}
	int Qget() {return NC_list::get();}
	void Qput(int x) {add(x);}

};

class stack: private NC_list{
public:
	stack(){}
	int Spop() {return NC_list::get();}
	void Spush(int x){NC_list::push(x);}
	using NC_list::empty;
};
*/
class NC_list {
	node *rear, *front;

public:
	NC_list(): rear(NULL), front(NULL){} 
	void push(int);
	void add(int);
	int get();
	void print();
	int empty() {
		return rear == rear->next1;
	}
};

void NC_list::push(int x){
	//rear -> next = new cell(x, rear -> next);

	node *n = new node();
	n -> key = x;

	if (rear)
		n -> next1 = rear;
	else{
		n -> next1 = NULL;
		front = n;
	}

	front = n;

}
int NC_list::get(){

		node *temp = front;
		int r = temp -> key;
		front = front -> next1;
		delete temp;
		return r;
	}
	/*
	if(empty() )
		return 0;
	node *current = front;
	front = front -> next1;
	int x = current -> key;
	delete front;
	return x;
	*/


void NC_list::print(){

	rear = front ->next1;


	node *temp = NULL;
	temp = front;
	while(temp != NULL){
		cout << temp -> key << endl;
		temp = temp -> next1;

		
	}
}


class stack: private NC_list{
public:
	stack(){}
	void Spush(int x){NC_list::push(x);}
	int Spop() {return NC_list::get();}



};

int main(){
/*
	queue q1;
	stack s1;

	q1.Qput(3);
	q1.Qput(5);
	q1.Qput(7);

	cout << q1.Qget() << endl;
	cout << q1.Qget() << endl;
	cout << q1.Qget() << endl;
	cout << q1.Qget() << endl;

	cout << endl;
	

	s1.Spush(2);
	s1.Spush(4);
	s1.Spush(6);

	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;

	//cout << q1.empty();
	//cout << s1.empty();

	//s1.list();
	*/
	/*

	stack s1;
	s1.Spush(1);
	s1.Spop();
	s1.Spush(2);
	s1.Spop();
	s1.Spush(3);
	s1.Spop();
	s1.Spush(4);
	s1.Spop();
*/
	stack s1;
	s1.Spush(1);
	s1.Spush(2);
	s1.Spush(3);
	s1.Spush(5);
	
	
	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;
	cout << s1.Spop() << endl;






	return 0;
}