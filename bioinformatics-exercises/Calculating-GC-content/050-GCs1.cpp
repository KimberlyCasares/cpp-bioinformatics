/*
Find GC content and length of a DNA sequence in a FASTA file

Input:
A file with a single DNA sequence in FASTA format (https://en.wikipedia.org/wiki/FASTA_format)

Output:
Length of the sequence (number of nucleotides)
GC content (% G-C base pairs) 


Learning objective: 
1. reading data from a file
2. basic understanding of the ifstream class and some of its most useful member functions
3. caveats of integer division

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
// This creates input stream InFile and associates it with file argv[1] (the first command line argument).
// It opens the file for access.
// In object oriented terms: this statement constructs an object InFile of the class ifstream, associated with the file argv[1].
// You can find a complete reference to ifstream class and all its functions here https://cplusplus.com/reference/fstream/ifstream/
// but do not worry if you cannot decipher what the website says. It is not that complex once you get the hang of it, but it is running
// way ahead of where we are now.

// The first thing to do is check if the file was opened successfully
// For example, the command would fail if the user mistyped the file name
	if (!InFile.is_open())  {
// ! is a negation, so this condition is true if the file failed to open
// InFile.is_open() tells the computer to apply the member function is_open() (member of the class ifstream) to the object InFile.
// The parentheses specify that is_open is a function but are empty because we are not passing any arguments to the funciton
// The is_open function returns true if the stream is open and false if not. See https://cplusplus.com/reference/fstream/ifstream/is_open/
		cout << "Cannot open file \"" << argv[1] << "\"\n";
// If you want to use " in a string literal, you have to type \" because " alone has a special meaning
		return 1;
// If the file failed to open, I print a message and quit
	}
	
// Now we know that the file is open and we can start reading from it.

	char Z;
	InFile.get(Z);
// get() is another member function of the class ifstream. It reads a single character from the stream
// I stored the character as Z.

// I like to include simple checks whether the input is what I expect it to be when possible. For example, a FASTA file should 
// start with >, which should now be in the variable Z. So let's check if it's true:
	if (Z!='>')  {
// '>' is a character constant. The difference between ">" and '>' is that ">" is a string that contains a single character '>'
		cout << "The file does not appear to be in FASTA format\n";
		return 1;
	}
// if the first character in the file is not > I print a message and quit.

// The first line (starting with >) is the sequence header, which we want to ignore (the sequence starts on the next line)
// Without knowing any other functions than get(), we can simply read characters until we find the end of the line, denoted by \n:
	while (Z!='\n')  InFile.get(Z);
	
// Now we are at the start of the second line in the file, so the next character we read should be the first nucleotide of the DNA sequence
// But we do not want to read just one nucleotide -- we want to read the whole sequence and count the number of 
// nucleotides and GC and AT base pairs. Let's prepare for the counting and set the counters to 0:
	int AT=0;
	int GC=0;
	int Length=0;


	string OtherCodes="RYMKBDHVNrymkbdhvn";
// I set up a string OtherCodes that contains text RYMKBDHVNrymkbdhvn
// If you are not familiar with the IUPAC code, see, for example https://www.bioinformatics.org/sms/iupac.html
// Don't think about it too much now -- it will come into play a few lines below.

	while (InFile.get(Z))  {
// This is the easiest way to read characters until the end of the file. InFile.get(Z) reads the next character and stores it 
// in Z, and checks if the operation was successful. If it was unsuccessful (e.g., because there are no more characters in the file),
// the condition is false, and the loop will end.

// Now one needs to think about different characters that can be in the file and their meaning:
// CGcgSs -- should count towards G+C
// ATatUuWw -- should count towards A+T
// The ambiguous codes RYMKBDHVNrymkbdhvn mean nucleotides but we do not know if they are G-C or A-T base pairs.
// I am going to count them in the length of the sequence but not towards G+C or A+T
		if (Z=='C' || Z=='G' || Z=='c' || Z=='g' || Z=='S' || Z=='s')  {
// || is the "logical or" operator (https://cplusplus.com/doc/tutorial/operators/). The logical expression 
// Z=='C' || Z=='G' || Z=='c' || Z=='g' || Z=='S' || Z=='s') is true if at least one of the conditions is satisfied.
// Analogously, && is a "logical and" oprator.
			++GC;
			++Length;
		}
		else if (Z=='A' || Z=='T' || Z=='a' || Z=='t' || Z=='U' || Z=='u' || Z=='W' || Z=='w')  {
			++AT;
			++Length;
		}
		else if (OtherCodes.find(Z)!=std::string::npos)  {
// There would be way to many conditions in this test if I used || as I did above, so I did something different
// OtherCodes is an object of the class string that I set up earlier (just think of it as a string)
// For reference, see https://cplusplus.com/reference/string/string/
// find() is a member function of the class string. OtherCodes.find(Z) searches for the first occurrence of the
// character Z in the string OtherCodes. If it finds it, it returns the position of the character in the string,
// and if not, it returns std::string::npos -- a system constant that signifies the maximum number of characters
// that can be in a string (it is a large number). Consequently the expression OtherCodes.find(Z)!=std::string::npos
// is true if Z is somewhere in the string OtherCodes.
// For reference, see https://cplusplus.com/reference/string/string/find/
			++Length;
		}
	}
	
// Now I finished reading the file and counted the G-C base pairs, A-T base pairs, and all characters that signify
// nucleotides, while all other characters (e.g., the new line characters, '\n' were ignored.
// I won't need the file anymore, so I can close it:
	InFile.close();
	
// I can print the results now. For length, it's easy:
	cout << "Sequence length: " << Length << " nucleotides\n";
	
// For the GC content, I want to calculate GC/(GC+AT). However, if write this in the code the result will be 0.
// That is because GC and (GC+AT) are both integers and a result of arithmetic operation is of the type that is
// the same as the most precise type of any of the operands. And because I am dividing two integers, the result is
// also an integer, which means that the decimal part is lost. And because the ratio is less then 1, removing the 
// decimal part will convert the result to 0. To avoid that, I have to cast (change the variable type) at least one
// of the operands as double (or float). This can be done by a function double that converts teh argument to type double:
	cout << "GC content: " << double(GC)/(GC+AT)*100.0 << "%\n";
// In the expression above, GC is of the type int whereas double(GC) is of the type double.
// Dividing double by int yields a double due to implicit conversion of (GC+AT) in the expression,
// so I do not need to (but can) write double(GC)/double(GC+AT)*100.0.

// Alternatively, I could use this (I commented it out but you can remove the // to see that it works):
//	cout << "GC content: " << 100.0*GC/(GC+AT) << "%\n";
// 100.0 is a constant of the type double. When it is multiplied by GC it yields a double, and when it is further
// divided by (GC+AT) the result is again double. Note that this would not work if you type 100 instead of 100.0
// because 100 is an integer constant whereas 100.0 is a floating point constant. That is why I usually add .0 to
// numbers when they are supposed to be a floating point type (double or float).  

	
  return 0;

}


