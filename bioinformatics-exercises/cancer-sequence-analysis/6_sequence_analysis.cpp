/* homework assignment 6 
written by Kimberly Casares */ 

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// First define struct
struct ProteinInfo {
    double hydrophobicPercent;
    string name;
    long length;
};

// Create a function to calculate hydrophobic percentage
double calculateHydrophobic(const string& sequence) {
    int hydrophobicCount = 0;
    for(int i = 0; i < sequence.length(); i++) {
        char aa = sequence[i];
        if(aa == 'L' || aa == 'I' || aa == 'V' || aa == 'F' || aa == 'M') {
            hydrophobicCount++;
        }
    }
    return (100.0 * hydrophobicCount) / sequence.length();
}

// Create a function to handle sorting
void insertIntoTop(ProteinInfo topProteins[], const ProteinInfo& newProtein) {
    topProteins[15] = newProtein;
    // Here's where sorting loop goes
    for(int i = 15; i > 0; i--) {
        if(topProteins[i].hydrophobicPercent > topProteins[i-1].hydrophobicPercent) {
            ProteinInfo temp = topProteins[i];
            topProteins[i] = topProteins[i-1];
            topProteins[i-1] = temp;
        }
    }
}

// Main function where the program starts
int main(int argc, char **argv) {
    // Check command line arguments
    if (argc < 2) {
        cout << "Use as: " << argv[0] << " <FASTA_file_name>\n";
        return 0;
    }
    
    // Open input file
    ifstream InFile(argv[1]);
    if (!InFile.is_open()) {
        cout << "Cannot open file \"" << argv[1] << "\"\n";
        return 1;
    }

    // Initialize array of top proteins
    ProteinInfo topProteins[16];
    // Here's where initialization loop goes
    for(int i = 0; i < 16; i++) {
        topProteins[i].hydrophobicPercent = -1;
    }

    string header, sequence, line;
    
    // Here's where file reading loop goes
    while(getline(InFile, line)) {
        if(line[0] == '>') {  // Found a header
            if(!sequence.empty()) {  // Process previous sequence if exists
                if(sequence.length() >= 100) {
                    ProteinInfo protein;
                    protein.name = header;
                    protein.length = sequence.length();
                    protein.hydrophobicPercent = calculateHydrophobic(sequence);
                    insertIntoTop(topProteins, protein);
                }
            }
            header = line;
            sequence.clear();
        }
        else {
            sequence += line;  // Add to current sequence
        }
    }

    // Process the last sequence
    if(!sequence.empty() && sequence.length() >= 100) {
        ProteinInfo protein;
        protein.name = header;
        protein.length = sequence.length();
        protein.hydrophobicPercent = calculateHydrophobic(sequence);
        insertIntoTop(topProteins, protein);
    }

    // Print results
    cout << "Rank\t%Hydrophobic\tLength\tProtein\n";
    for(int i = 0; i < 15; i++) {
        if(topProteins[i].hydrophobicPercent >= 0) {
            cout << i + 1 << "\t"
                 << topProteins[i].hydrophobicPercent << "\t"
                 << topProteins[i].length << "\t"
                 << topProteins[i].name.substr(1) << "\n";
        }
    }

    return 0;
}