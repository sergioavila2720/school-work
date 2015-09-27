/*

Sergio Avila
Program 3

Now that we have a interpreter that deals with precedence, we are ready to include a
small program that will be able to deal not only with syntax errors but with semantic errors as well.



*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <stdlib.h>
#include <math.h>


using namespace std;
// here are the function prototypes that we need expression, terminal, and factor
int Exp(), Term(), Exp2(int), Term2(int), Fact(), Pwr();
int Declarations(), Declaration(), Statements(), Statement(int);
int input_st();
int output_st();
int assignment_st(int);
void Programa1();

// This declaration is for when we want to get data from the data file where our example operation is
ifstream fin("myfile1.txt");

int main(){
	 ifstream fin("myfile1.txt");
	 string word;

	 /*
	 -- made this liitle function to test that i can search 
	 -- for words in different lines from a file 
	 if (fin >> word)
		if (word == "program")
			Programa1();
	fin.close();
	*/
	

	if (fin >> word){
		if (word == "Program"){
			cout << Declarations();
			cout << Statements();
		}
	}

	
	


return(0);
}

// this is the first Expression this will be called recursebly
// this will serve as the E--> TE'
int Exp(){

	return( Exp2(Term()) );
}

// this is the recursive (T') from T--> FT'
int Term(){

	return (Term2(Pwr()) );
}

// this is already inside the loop E'--> +TE'|-TE'| e
int Exp2(int input){
	
	int result = input;
	char a;
	if (!fin.eof() ){
		fin.get(a);
	//if( (a = fin.get()) != fin.eof()){ // just another way i could have written this. works for my notes.
		if (a == '+')
			result = Exp2(result + Term() );
		else if(a == '-')
			result = Exp2(result - Term() );
		else if(a == ')')
			fin.putback(a);
	}
	return result;
}

// T'--> *FT'| /FT'| e
int Term2(int input){
	
	int result = input;
	char a;

	//if( (a = fin.get()) != fin.eof() ){ // just another way i could have written this. works for my notes. 
	if (!fin.eof() ){
		fin.get(a);
		if (a == '*')
			result = Term2(result * Pwr() );
		else if (a == '/')
			result = Term2(result / Pwr() );
		else if (a == '+' || a == '-' || a == ')') // i added the ')' to close the parenthesis . it does this because when it finds it a gets put back so it knows what operation to do 
			fin.putback(a);
	}
	return result;
}

// This takes care of the precedence because everytime the program sees
// something that is not a "^" that symbol will be put back and start again
int Pwr(){

	int p = Fact();
	char a;

	if (!fin.eof() ){
		fin.get(a);

		if (a == '^')
			p = pow(p, Pwr());
		if (a == '+' || a == '-' || a == '*' || a == '/' || a == ')')
			fin.putback(a);

	}

	return p;

}

// in this part it just tells the program that when the program sees '(' 
//it jumps back to the top to see what operation is going to do first
int Fact(){
	
	char a = fin.get();
	
	// in this part it just tells the program that when the program sees '(' this jump back to the top to see what operation is going to do first
	if ( a == '('){
		int ExpTemp = Exp();
		fin.get(a);
		if(a == ')'){
			return ExpTemp;
		}
	}

	return (atoi(&a) );
	
}

int Declarations(){

	int countword = 0;
	string word;
	if (fin >> word){
		if (word == "begin"){
			exit(1);
		}
		else if (word == "var"){
			return Declaration();
		}
		return Declarations();
	}
}

int Declaration(){


}

int Statements(){


}

int Statement(int word){


}

int input_st(){


}

int output_st(){


}

int assignment_st(int word){


}

/*
--made this liitle function to test that i can search 
--for words in different lines from a file 
void Programa1(){

	string word;

	int countword = 0;
	while (getline(fin, word)){
		if (countword == 1){
			if (word == "begin"){
				cout << "this worked" << endl;
			}
			else if (word == "var")
				cout << " this did not work" << endl;
		}

		countword = countword +1;
	}
}

*/
	
