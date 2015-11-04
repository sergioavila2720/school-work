# include <iostream>


using namespace std;

class cell{
	int info;
	cell *next;
	cell(int i) {info = i; next = this;}
	cell(int i, cell *n) {info = i; next = n;}
	friend class list;
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

int list::get(){
	if(empty() )
		return 0;
	cell *front = rear ->next;
	rear -> next = front -> next;
	int x = front -> info;
	delete front;
	return x;
}


int main(){


	return 0;
}