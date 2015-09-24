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

// This declaration is for when we want to get data from the data file where our example operation is
ifstream fin("myfile.txt");

int main(){
		 
	
	if (fin.is_open())cout << "result = " << Exp() << endl;
	else cout << "could not open file" <<endl;
	fin.close();


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