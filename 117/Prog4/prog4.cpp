# include <iostream>
# include <unordered_map>
# include <vector>
/* In this program we will make use of unordered_map library. we will be building a hash table to see 
that a hash table could be very usefull when we're dealing with a lot of information because
it has a running time O(1). 
*/
# include <string>
# include <fstream>


using namespace std;
//i need to read the data from a file as well as writing the new data to another file
ifstream fin("Prog4-data.txt");
ofstream finout("map2.txt");

int main (){
	//her are the types I will be using for the key and value in the hash table
	string key, value;

	// my map declaration it'll take two strings. (val and type)
	unordered_map <string, string> map;
	while(!fin.eof()){
		fin >> key;			// reading a word set to key
		fin >> value;		// reading another word for value
		map[key] = value;	// storing content of data file into unordered_map
		
		}
		unordered_map <string, vector<string>> map2;	// make another unordered_map to store the new content from map1

		// need to loop through map to be able to store 
		// contents of map to map2 in reverse. that way
		//it will store the sequence as keys and values that have the same sequence
		for (auto i: map){ 
			map2[i.second].push_back(i.first);
		}

		//this loop is to have the values/label and keys from map2
		//written into a new file. 
		for (auto j : map2){
			for (int k =0; k<j.second.size();k++){
				finout << j.second[k] << ",";
			}
			finout << endl;
			finout<< j.first << endl;
		}
		return 0;
	}
