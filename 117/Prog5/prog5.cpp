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

void Insertion_sort(int [],int );



int main (){


int *array;
array = new int[10];

list1 *h1,*h2,*free_list;
h1 = NULL;
h2 = NULL;
free_list = NULL;

	return 0;


}

void Insertion_sort(int a[],int n){

	 int key,i,j;
	 for(j=2;j<=n;j++){
	 	key=a[j];
	 	i=j-1;
	 	while((i>0)&&(a[i]>key)){
	 		a[i+1]=a[i];
	 		i=i-1;
	 	}

	 	a[i+1]=key;
	}
}