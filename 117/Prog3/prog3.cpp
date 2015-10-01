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

struct node
{
	char id; 
	int value;
};
node symbol[20];

// here are the function prototypes that we need expression, terminal, and factor
int Exp(), Term(), Exp2(int), Term2(int), Fact(), Pwr();
int Statement(string);
void Declarations(), Declaration(), input_st(), output_st(),assignment_st(char);
int Statements();
void Programa1();

string letters = "abcdefghijklmnopqrstuvwxyz";

// This declaration is for when we want to get data from the data file where our example operation is
ifstream fin("myfile1.txt");

int main(){
	 //ifstream fin("myfile1.txt");
	 string word;

	 /*
	// -- made this liitle function to test that i can search 
	//-- for words in different lines from a file 
	 if (fin >> word)
		if (word == "program" || word == "Program")
			Programa1();
	fin.close();
	*/
	
	if (fin >> word){
		if (word == "Program" || word == "program"){
			Declarations();
			Statements();

			if (word == "end"){
				cout << "Program is done" << endl;
				return 0;
			}

		}
		else{
			cout << "Error (Lexical/Syntax) " << endl;
			exit(1);
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

void Declarations(){

	int countword = 0;
	string word;
/*
	while (getline(fin, word)){
		if (countword == 1 || countword == 2){
			if (word == "begin"){
				exit(1);
			}
			else if (word == "var"){
				Declaration();
			}
			return Declarations();
		}
	} 
	*/
		
		while (fin >> word){
			if (word == "begin")
				exit(1);
			else if (word == "var")
				Declaration();
			Declarations();

		}
		
}

void Declaration(){
	char word;
	//string word;
	int countword = 0;
	

	while(getline(fin, letters)){
		if (countword == 1 || countword == 2){
			while(word != ';'){
				for (int i = 0; i< letters.size(); i++){
					if(word == letters[i])
						symbol[i].id = word;
				}
			}
		}
	}

}

int Statements(){
	int countword = 0;
	string word;
	/*
	while (fin >> word){
		if (word == "end")
			exit(1);
		else 
			return Statement(word);
		return Statements();
	} */

	while(getline (fin, word)){
		if (countword == 11){
			if (word == "end")
				exit(1);
			else 
				return Statement(word);
			
		}
		countword = countword +1;
	} return Statements();

}

int Statement(string word){

	int countword = 0;
	while (getline (fin, word)){
		if (countword == 4){
			if (word == "input")
				input_st();
		}
		if (countword == 6 || countword == 8 || countword == 9 || countword == 10){
			if (word == "output")
				output_st();
		}
		else{

			for (int i = 0; i < word.size(); i++){
				assignment_st(word[i]);
			}
		}
	}


}

void input_st(){

	char word;

	while (fin >> word ){
		for (int i = 0; i < 20; i++){
			for (int j = 0; j < letters.size(); j++){
				if (symbol[i].id == letters[i]){
					if(symbol[i].id == word){
						cin >> symbol[i].value;
					}
					else 
						cout << "Semantic Error" << endl;
				}
				else
					cout << "Syntax Error" << endl;

			}
		}
	}



}

void output_st(){

	char word;
	while (fin >> word){
		for (int i = 0; i < 20; i++){
			for (int j = 0; j < letters.size(); j++){
				if (symbol[i].id == letters[i]){
					if (symbol[i].id == word)
						cout << symbol[i].value << endl;
					else 
						cout << "Semantic Error" << endl;
				}
				else {
					fin.putback(word);
					cout << Exp() << endl;
				}
				
			}
		}

	}


}

void assignment_st(char word){

	while (fin >> word){
		if (word == '='){
			int Temp = Exp();
			for (int i = 0; i < 20; i++){
				for (int j =0; j< letters.size(); j++){
					if (symbol[i].id == letters[i]){
						if (symbol[i].id == word)
							symbol[i].value = Temp;
						else 
							cout << "Semantic Error" << endl;
					}
					else 
						cout << "Syntax Error" << endl;
				}
			}
		}
	}

}

/*
//made this liitle function to test that i can search 
//for words in different lines from a file 
void Programa1(){

	string word;

	int countword = 0;
	while (getline(fin, word)){
		if (countword == 2){
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
/*
void Programa1(){

	string word;
	while(fin >> word){
		if (word == "begin")
			cout << "this worked"<< endl;
		else if (word == "var")
			cout << "didnt work" << endl;
	}
}
*/
	
