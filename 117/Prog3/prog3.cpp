/*

Sergio Avila
Program 3

Now that we have a interpreter that deals with precedence, we are ready to include a
small program that will be able to deal not only with syntax errors but with semantic errors as well.



*/
#include <iostream>
#include <string>
#include <cstring>
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
node symbol[4];

// here are the function prototypes that we need expression, terminal, and factor
int Exp(), Term(), Exp2(int), Term2(int), Fact(), Pwr();
void Statement(string);
void Declarations(), Declaration(), input_st(), output_st(),assignment_st(string);
void Statements();
void Programa1();
bool lettercheck(char);

string letters = "abcdefghijklmnopqrstuvwxyz";

// This declaration is for when we want to get data from the data file where our example operation is
ifstream fin("myfile1.txt");

int main(){
	 //ifstream fin("myfile1.txt");
	 string word;


	
	fin >> word;
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
			return 0;
		
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

	string word;

		
		fin >> word;
		if (word == "begin")
			return;
		else if (word == "var") // when you see a variable go to declaration so that it can read the actual variables
			Declaration();
		else
			return;
			//cout << "Error" << endl;
		Declarations();

		
}

void Declaration(){
	char word;

	fin >> word;
	for (int i = 0; i< letters.size(); i++){
		//if (word == letters[i]) // if word matches a letter store it in symbol id
		if ((word >= 'A' && word <='Z') || (word >= 'a' && word <= 'z'))
			symbol[i].id = word;
		else if (word == ',') // when ever you se a comma it has to read another variable so call itself again
			Declaration();
		else if (word == ';') // when you see a semicolon go back to declarations
			return;

	}
	Declarations();
}


void Statements(){
	string word;
	
	fin >> word;
		if (word == "end")
			return;
		else if (word == "input" || word == "output" || (word.length() == 1 || strchr(word.c_str(), '=') ))
			Statement(word);
		Statements();
}

void Statement(string word){

	int countword = 0;
	//fin >> word;
		if (word == "input")
			input_st();
		else if (word == "output")
			output_st();
		else {
			assignment_st(word);
		}
	}

void input_st(){

	char word;
	int variable;

	fin >> word;
		for (int i = 0; i < 1; i++){
			//for (int j = 0; j < letters.size(); j++){
			if ((word >= 'A' && word <='Z') || (word >= 'a' && word <= 'z')) {
				//symbol[i].id == word;
					if(symbol[i].id == word){
						cout << "enter value "; 
						cin >> variable;
						symbol[i].value = variable;

						}					
					else 
						cout << "Semaniinonbtic Error" << endl;
				}
				else
				cout << "Syntax Error1" << endl;
		}
}
void output_st(){

	char word;
	fin >> word;
		for (int i = 0; i < 4; i++){
			//for (int j = 0; j < letters.size(); j++){
				//if (symbol[i].id == letters[i]){
			if ((word >= 'A' && word <='Z') || (word >= 'a' && word <= 'z')){
				//symbol[i].id = letters[i];
				if (word == symbol[i].id)
					cout << "the value for a is " << symbol[i].value << endl;
				else 
					cout << "Semantic Error1"<< endl;
			}
			else {
				fin.putback(word);
				cout << Exp() << endl;
			}
		}

	


}

void assignment_st(string word){

		fin >> word;
		if ((word.length() == 1 || strchr(word.c_str(), '=') )){
			int Temp = Exp();
			for (int i = 0; i < 4; i++){
				//for (int j =0; j< letters.size(); j++){
				if ((word[i] >= 'A' && word[i] <='Z') || (word[i] >= 'a' && word[i] <= 'z')){
					//symbol[i].id == letters[i];
					if (symbol[i].id == word[i])
							symbol[i].value = Temp;
					else 
							cout << "Semantic Error2" << endl;
				}
			}
		}
		else
			cout << "Syntax Error2" << endl;
	

}
bool lettercheck(char letter){

	if ((letter >= 'A' && letter <='Z') || (letter >= 'a' && letter <= 'z'))
		return true;
	else 
		return false;
}