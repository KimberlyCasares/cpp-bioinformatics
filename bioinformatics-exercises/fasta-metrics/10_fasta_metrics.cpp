//exercise 10; written by Kimberly Casares

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iomanip>

using namespace std;

const int MAX_AMINO_ACIDS = 21; 
int ASCII[128] = {0}; // Initialize all to zero

// Structure to hold protein information
struct ProteinInfo {
    string name;
    int length;
    double jaccardIndex;
};

// Structure to represent a tetrapeptide (4 amino acids)
struct Tetrapeptide {
    int a, b, c, d;
    
    // Constructor
    Tetrapeptide(int a_, int b_, int c_, int d_) : a(a_), b(b_), c(c_), d(d_) {}
    
    // Operator for comparison in set
    bool operator<(const Tetrapeptide& other) const {
        if (a != other.a) return a < other.a;
        if (b != other.b) return b < other.b;
        if (c != other.c) return c < other.c;
        return d < other.d;
    }
};

// Initialize ASCII array for amino acid conversion
void initializeASCII() {
    ASCII['A'] = 1;
    ASCII['R'] = 2;
    ASCII['N'] = 3;
    ASCII['D'] = 4;
    ASCII['C'] = 5;
    ASCII['Q'] = 6;
    ASCII['E'] = 7;
    ASCII['G'] = 8;
    ASCII['H'] = 9;
    ASCII['I'] = 10;
    ASCII['L'] = 11;
    ASCII['K'] = 12;
    ASCII['M'] = 13;
    ASCII['F'] = 14;
    ASCII['P'] = 15;
    ASCII['S'] = 16;
    ASCII['T'] = 17;
    ASCII['W'] = 18;
    ASCII['Y'] = 19;
    ASCII['V'] = 20;
    // Also handle lowercase amino acids
    ASCII['a'] = 1;
    ASCII['r'] = 2;
    ASCII['n'] = 3;
    ASCII['d'] = 4;
    ASCII['c'] = 5;
    ASCII['q'] = 6;
    ASCII['e'] = 7;
    ASCII['g'] = 8;
    ASCII['h'] = 9;
    ASCII['i'] = 10;
    ASCII['l'] = 11;
    ASCII['k'] = 12;
    ASCII['m'] = 13;
    ASCII['f'] = 14;
    ASCII['p'] = 15;
    ASCII['s'] = 16;
    ASCII['t'] = 17;
    ASCII['w'] = 18;
    ASCII['y'] = 19;
    ASCII['v'] = 20;
}

// Convert amino acid character to index (0-20)
int aminoAcidToIndex(char aa) {
    // Character is always less than 128 in standard ASCII
    return ASCII[(unsigned char)aa]; // Cast to unsigned char to prevent out of bounds
}

// Trim whitespace from a string
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == string::npos || end == string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

// Populate tetrapeptide array from a sequence
void populateTetramerArray(const string& sequence, bool tetramers[21][21][21][21]) {
    set<Tetrapeptide> seenTetramers; // Set to track unique tetramers using custom struct
    
    for (int i = 0; i + 3 < sequence.size(); i++) {
        int a = aminoAcidToIndex(sequence[i]);
        int b = aminoAcidToIndex(sequence[i + 1]);
        int c = aminoAcidToIndex(sequence[i + 2]);
        int d = aminoAcidToIndex(sequence[i + 3]);
        
        Tetrapeptide tetramer(a, b, c, d);
        
        if (seenTetramers.find(tetramer) == seenTetramers.end()) {
            tetramers[a][b][c][d] = true; 
            seenTetramers.insert(tetramer);
        }
    }
}

// Calculate Jaccard Index between two tetrapeptide arrays
double calculateJaccardIndex(bool queryTetramers[21][21][21][21], 
                             bool dbTetramers[21][21][21][21]) {
    int intersection = 0, unionCount = 0;
    
    for (int a = 0; a < MAX_AMINO_ACIDS; a++) {
        for (int b = 0; b < MAX_AMINO_ACIDS; b++) {
            for (int c = 0; c < MAX_AMINO_ACIDS; c++) {
                for (int d = 0; d < MAX_AMINO_ACIDS; d++) {
                    if (queryTetramers[a][b][c][d] || dbTetramers[a][b][c][d]) {
                        unionCount++;
                    }
                    
                    if (queryTetramers[a][b][c][d] && dbTetramers[a][b][c][d]) {
                        intersection++;
                    }
                }
            }
        }
    }

    if (unionCount == 0) return 0.0;

    return (double)(intersection) / unionCount;
}

// Pair struct to hold header and sequence together
struct FastaPair {
    string header;
    string sequence;
};

// Read a FASTA file and extract headers and sequences
string readFastaFile(const string& filename, vector<string>& headers, vector<string>& sequences) {
    ifstream file(filename.c_str()); // Use of .c_str() for older C++ compatibility
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return "";
    }
    
    string line, sequence = "";
    string header = "";
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        line = trim(line); 
        
        if (line[0] == '>') {
            if (!sequence.empty()) {
                sequences.push_back(sequence); 
            }
            headers.push_back(line); 
            sequence.clear();
        } else {
            sequence += line; 
        }
    }
    
    if (!sequence.empty()) {
        sequences.push_back(sequence); 
    }
    
    if (!sequences.empty()) {
        return sequences[0]; // Return the first sequence for the query
    }
    
    return "";
}

// Read a FASTA database
vector<FastaPair> readFastaDatabase(const string& filename) {
    ifstream file(filename.c_str()); 
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        vector<FastaPair> empty;
        return empty; 
    }
    
    string line, sequence = "";
    vector<FastaPair> database;
    string header = "";
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        line = trim(line); 
        if (line[0] == '>') {
            if (!sequence.empty()) {
                FastaPair pair;
                pair.header = header;
                pair.sequence = sequence;
                database.push_back(pair);
                sequence.clear();
            }
            header = line; 
        } else {
            sequence += line; 
        }
    }
    if (!sequence.empty()) {
        FastaPair pair;
        pair.header = header;
        pair.sequence = sequence;
        database.push_back(pair);
    }
    return database;
}

// Insert protein into top proteins array, maintaining sorted order
void insertIntoTop(vector<ProteinInfo>& topProteins, const ProteinInfo& protein) {
    // Find position to insert
    int pos = topProteins.size();
    for (int i = 0; i < topProteins.size(); i++) {
        if (protein.jaccardIndex > topProteins[i].jaccardIndex) {
            pos = i;
            break;
        }
    }
    
    // Insert at position
    if (pos < topProteins.size()) {
        topProteins.insert(topProteins.begin() + pos, protein);
        // Remove the last element if size exceeds 15
        if (topProteins.size() > 15) {
            topProteins.pop_back();
        }
    } else if (topProteins.size() < 15) {
        // If vector isn't full yet, add to the end
        topProteins.push_back(protein);
    }
}

// Main function
int main(int argc, char **argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <query_FASTA_file> <database_FASTA_file>\n";
        return 0;
    }

    // Initialize ASCII array for amino acid conversion
    initializeASCII();

    string queryFile = argv[1];
    string databaseFile = argv[2];

    // Read query file
    vector<string> queryHeaders;
    vector<string> querySequences;
    string querySequence = readFastaFile(queryFile, queryHeaders, querySequences);
    
    if (querySequence.empty()) {
        cerr << "Error: Query sequence is empty or file couldn't be read.\n";
        return 1;
    }
    
    // Create and populate query tetramers array
    bool queryTetramers[21][21][21][21] = {{{{false}}}}; // Initialize all elements to false
    populateTetramerArray(querySequence, queryTetramers);

    // Read database file
    vector<FastaPair> database = readFastaDatabase(databaseFile);
    if (database.empty()) {
        cerr << "Error: Database is empty or file couldn't be read.\n";
        return 1;
    }

    // Vector to store top proteins
    vector<ProteinInfo> topProteins;
    for (int i = 0; i < 15; i++) {
        ProteinInfo protein;
        protein.jaccardIndex = -1; // Initialize with invalid value
        topProteins.push_back(protein);
    }

    // Process each database sequence
    for (int i = 0; i < database.size(); i++) {
        string header = database[i].header;
        string dbSequence = database[i].sequence;
        
        // Only process sequences of sufficient length
        if (dbSequence.length() >= 100) {
            bool dbTetramers[21][21][21][21] = {{{{false}}}}; // Initialize all elements to false
            populateTetramerArray(dbSequence, dbTetramers);

            double jaccardIndex = calculateJaccardIndex(queryTetramers, dbTetramers);
            
            // Create protein info structure
            ProteinInfo protein;
            protein.name = header;
            protein.length = dbSequence.length();
            protein.jaccardIndex = jaccardIndex;
            
            // Insert into top proteins
            insertIntoTop(topProteins, protein);
        }
    }

    // Print results
    cout << "Query file: " << queryFile << endl;
    cout << "Database file: " << databaseFile << endl << endl;
    
    cout << "Top 5 matches by tetrapeptide Jaccard similarity:\n";
    cout << "Rank\tJaccard similarity\tLength\tProtein\n";
    
    for (int i = 0; i < 5; i++) {
        if (topProteins[i].jaccardIndex >= 0) {
            cout << i + 1 << "\t"
                 << fixed << setprecision(7) << topProteins[i].jaccardIndex << "\t"
                 << topProteins[i].length << "\t"
                 << topProteins[i].name << "\n";
        }
    }

    return 0;
}