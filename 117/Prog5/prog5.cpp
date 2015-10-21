/*
In this program we will be simulating a garbage collection program
we will use linked lists to do the lazy garbage collection
*/
# include <iostream>


using namespace std;

struct list1{

	int key, mark;
	list1 *next;
};
// This is an addition to the insertion sort in order for it to work. 
// This is just for creatin a new node.
list1* createNode(int key);

// This will work for when we start inserting our keys into our arrays. 
list1* Insertion_sort(int *, int);



int main (){

	int option;
	list1 *array;
	array = new list1[10];

	list1 *h1,*h2,*free_list;
	h1 = NULL;
	h2 = NULL;
	free_list = NULL;

	int arr[2] = {1,2};
	

	do {

		cout << "1). Insert a list" << endl;
		cout << "2). Delete" << endl;
		cout << "3). Garbage Collector" << endl;
		cout << "4). Display Memory" << endl;
		cout << "5). Quit" << endl;

		cout << "What do you want to do? ";
		cin>> option;
		cout << endl;

		if (option == 1){
			cout << "We are going to INSERT the list here.\n" <<endl;
			list1 *mylist = Insertion_sort(arr, 3);
			cout << &mylist << endl;

		}else if (option == 2){
			cout << "We are going to DELETE here.\n" << endl;
		
		}else if (option == 3){
			cout << "We are going to call the GARBAGE COLLECTOR.\n" << endl;

		}else if (option == 4){
			cout << "We are going to DISPLAY THE MEMORY.\n" << endl;
		}
		else if (option == 5)
			exit(1);
		else
			cout << "Sorry this is not an option try again.\n" << endl;


	}while (option != 5);





	return 0;


}

list1* createNode(int key){

	list1 * n = new list1;
	n -> key = key;
	n ->next = 0;

	return n;
}


list1* Insertion_sort(int *arr, int size){

	list1* head;
	head = createNode(arr[0]);
	for (int i = 0; i < size; ++i){
		list1 *prev = 0;
		list1 *iter = head;

		while(iter != 0){
			if (iter ->key){
				prev = iter;
				iter = iter ->next;
			}
			else
				break;
		}

		list1 *newNode = createNode(arr[i]);
		if (prev == 0)
			head = newNode;
	}
	return head;


}
