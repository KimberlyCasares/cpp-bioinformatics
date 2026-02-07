/*
Find GC content and length of a DNA sequence in a FASTA file -- Alternative 1

Using a different way to count the nucleotides to avoid the complex if statements

Input:
A file with a single DNA sequence in FASTA format (https://en.wikipedia.org/wiki/FASTA_format)

Output:
Length of the sequence (number of nucleotides)
GC content (% G-C base pairs) 


Learning objetive: 
1. ASCII code
2. Practice using arrays

Code written by Jan Mrazek, mrazek@uga.edu

*/



#include <iostream>
#include <fstream>  // container that includes the class ifstream
#include <string>  // container that includes the class string

using namespace std;

int main(int argc, char **argv) {

	if (argc<2)  {
		cout << "Use as:  " << argv[0] << " <FASTA_file_name>\n";
		cout << "!!! The file can contain only one sequence\n";
		cout << "Example: " << argv[0] << " Ecoli.fasta\n";
		return 0;
	}
	
	ifstream InFile(argv[1]);
	if (!InFile.is_open())  {
		cout << "Cannot open file \"" << argv[1] << "\"\n";
		return 1;
	}
	
	char Z=InFile.get();
	if (Z!='>')  {
		cout << "The file does not appear to be in FASTA format\n";
		return 1;
	}
	while (Z!='\n')  Z=InFile.get();
	

// Up to here, everything is the same as in the previous version of the code
// What I am planning to do next is count all possible different characters, how many times
// each appears in the file (after skipping the first line. For that, I am going to reference
// the ASCII table: https://www.asciitable.com/
// There are 128 standard characters, each represented by a numerical code between 0 and 127.
// A variable of type char stores a number that represents the character.
// For example, a statement char A='C' is the same as char A=67 because 67 is ASCII code for C.
// I am going to take advantage of that and set an array with 128 elements of type int:
	int Counts[128]={};
// The ={} is a simple way to initialize all values of the array to 0. I could also run a for loop
// to assign 0 to each element of the array.

	while (InFile.get(Z))  {
		++Counts[Z];
// Now instead of all the ifs, I simply increment the count with index Z
	}
	
	InFile.close();

// At this point, the value Counts[i] contains the number of times the character with ASCII code i appears
// in the file (after skipping the first line)
// This simplifies the loop, which is desirable because the loop runs millions of times, so to make the code
// efficient, you want to make it as simple as possible. However, you now have to do some work to add up the
// correct numbers:
	int AT=Counts['A']+Counts['T']+Counts['a']+Counts['t']+Counts['U']+Counts['u']+Counts['W']+Counts['w'];
	int GC=Counts['G']+Counts['C']+Counts['g']+Counts['c']+Counts['S']+Counts['s'];
	int Length=AT+GC+Counts['R']+Counts['Y']+Counts['M']+Counts['K']+Counts['B']+Counts['D']+Counts['H']+Counts['V']+Counts['N']
	                 +Counts['r']+Counts['y']+Counts['m']+Counts['k']+Counts['b']+Counts['d']+Counts['h']+Counts['v']+Counts['n'];
	
	
// Now just print the results:
	cout << "Sequence length: " << Length << " nucleotides\n";
	cout << "GC content: " << 100.0*GC/(GC+AT) << "%\n";

	
  return 0;

}


