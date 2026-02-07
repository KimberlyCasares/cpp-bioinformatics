/*
Find GC content and length of a DNA sequence in a FASTA file -- Alternative 2

Using getline() instead of get() to read the file

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
		return 1;
	}
	
	ifstream InFile(argv[1]);
	if (!InFile.is_open())  {
		cout << "Cannot open file \"" << argv[1] << "\"\n";
		return 1;
	}
	
// opening the file is the same

	string Line;
// I created a string called Line without assigning any value to it.
	
	getline(InFile,Line);
// This reads a line (until a '\n' character or end of file) from the stream InFile and stores it in the string Line.
// getline returns true if the line is read successfully, false upon failure, but in this case we do not use the return value.
	
	if (Line[0]!='>' || Line.empty())  {
// Line[0] refers to the first character of the string Line. 
// Line.empty() is true if Line is an empty string (has length 0)
		cout << "The file does not appear to be in FASTA format\n";
		return 1;
	}
	
	int Counts[128]={};

	while (getline(InFile,Line))  {
// reads the next line from the file into the string Line and stops the loop when reading fails (e.g., when the end of the file is reached.
// Note that this time I am using the return value of getline to determine if I reached the end of the file or not.
// The prior content of Line is lost but I don't need it anymore.
		for (int i=0;i<Line.size();++i)  ++Counts[Line[i]];
// Line.size() returns the size (number of characters) of the string Line. Line.length() does the same thing and can also be used.
// The for loop runs through all characters Line[i] in the string Line and increments the counter with index Line[i]
	}
	
// Note: you can combine get() and getline() (and also other forms of input) when reading from a stream.
// For example, you could use getline() to read the first line, and then use get() in the loop to read 
// the sequence as we did in the previous versions of this program. The next read always starts from where the
// previous ended, even if you combine diffeernt functions.
	
	InFile.close();

	int AT=Counts['A']+Counts['T']+Counts['a']+Counts['t']+Counts['U']+Counts['u']+Counts['W']+Counts['w'];
	int GC=Counts['G']+Counts['C']+Counts['g']+Counts['c']+Counts['S']+Counts['s'];
	int Length=AT+GC+Counts['R']+Counts['Y']+Counts['M']+Counts['K']+Counts['B']+Counts['D']+Counts['H']+Counts['V']+Counts['N']
	                 +Counts['r']+Counts['y']+Counts['m']+Counts['k']+Counts['b']+Counts['d']+Counts['h']+Counts['v']+Counts['n'];
	
	cout << "Sequence length: " << Length << " nucleotides\n";
	cout << "GC content: " << 100.0*GC/(GC+AT) << "%\n";

  return 0;

}


