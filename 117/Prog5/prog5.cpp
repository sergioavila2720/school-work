/*
In this program we will be simulating a garbage collection program
we will use linked lists to do the lazy garbage collection
I am running this program through the terminal in ubuntu.
using g++ prog5.cpp -std=c++11

*/
# include <iostream>
# include <list>
# include <stdlib.h>
# include <stdio.h>

using namespace std;

struct list1{

	int key, mark, next1;
	list1 *next;
	list1 *listInfo;
};
// Setting my two linked lists with it's head pointers at NULL
	list1 *h1 = NULL;
	list1 *h2 = NULL;
	list1 *free_list;
	list1 *array1 = new list1[10];

// Function prototypes.
list1* createNode(int);
void insertionSort (list1 **, list1*);
void inserting();
void Delete(list1*, int);
void GarbageCollector(list1*, list1*, list1*);
void Display();


int main (){

	int option, deleteuser;

/*Implemented a do while loop to build the menu driven program */
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
			inserting();

		}else if (option == 2){
			cout << "We are going to DELETE here.\n" << endl;
			cout << "What key do you want to delete? ";
			cin >> deleteuser;
			Delete(h1, deleteuser);
			Display();

		
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

/* This is an addition to the insertion sort in order for it to work. 
   This is just for creatin a new node.*/
list1* createNode(int currKey){

	list1 * n = new list1;
	n -> key = currKey;
	n ->next = NULL;

	return n;
}

// This will work for when we start inserting our keys into our arrays. 
void insertionSort (list1 **head, list1* new_node){
	list1 *current;
	//case for the head end.
	if (*head == NULL || (*head) -> key >= new_node -> key){
		new_node -> next = *head;
		*head = new_node;
	}
	else {
		//look for the node before insertion
		current = *head;
		while(current ->next != NULL && current -> next -> key < new_node -> key){
			current = current -> next;
		}
		new_node -> next = current -> next;
		current -> next = new_node;
	}
}
/* instead of doing all these comands in the main function 
I made a helper function to insert the keys*/
void inserting(){

	list1 *new_node = createNode(3);
	insertionSort(&h1, new_node);

	new_node = createNode(1);			
	insertionSort(&h1, new_node);

	new_node = createNode(4);
	insertionSort(&h2, new_node);

	new_node = createNode(5);
	insertionSort(&h1, new_node);

	new_node = createNode(2);
	insertionSort(&h2, new_node);

	new_node = createNode(9);
	insertionSort(&h2, new_node);

	new_node = createNode(8);
	insertionSort(&h2, new_node);

	new_node = createNode(4);
	insertionSort(&h1, new_node);
}
/* In this function we can select and delete a specific node */
void Delete(list1* head , int n){

	list1 *p = head;

	for (int i = 0; i < n-1 && p != NULL; i++){
		p = p ->next;
	}

	if (p != NULL){
		list1* del = p -> next;
		p -> next = p -> next -> next;
		delete del;
	}
	else 
		delete p;
}
/* This is the garbage collection. In here i will implement the mark and sweep function. */ 
void GarbageCollector(list1* head1, list1* head2, list1* freeL){
this will iterate 
	// through the nodes 
	list1* iter;

	/* in this next part is the mark function
	this will search for h1, h2, and the freelist, 
	and it will mark them with 1 at the begining */
	for (iter = head1; iter != NULL; iter = iter -> next){
		iter -> listInfo -> mark = 1;
	}

	for (iter = head2; iter != NULL; iter = iter -> next){
		iter -> listInfo -> mark = 1;
	}

	for (iter = freeL; iter != NULL; iter = iter -> next){
		iter -> listInfo -> mark = 1;
	}

	/* I could not finish implementing the garbage collector */
}


void Display(){
	cout << "h1 = " << h1 -> listInfo -> key +1 << endl;
}

/* Due to a csci 144 midterm that i have on monday I could not finih
this lab. I did as much as I could :( */