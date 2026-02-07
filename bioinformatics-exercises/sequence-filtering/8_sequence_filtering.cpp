/* Written by Kim Casares
Write a program to test statistical significance of Pearson product-moment correlation 
coefficient by permutation test, using random
sampling from all possible permutations
we are testing whether a correlation between two variables is statistically significant
using a permutation test. 
reads two columns of data from a file
calculates the pearson correlation test to determine the p-value 
reports both the correlation coefficient and the p-value
 */ 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <cmath>

using namespace std;

// Function to calculate the Pearson correlation coefficient
double calculateCorrelation(const vector<double>& X, const vector<double>& Y, int rows) {
    // Calculate means
    double sumX = 0.0, sumY = 0.0;
    for (int i = 0; i < rows; ++i) {
        sumX += X[i];
        sumY += Y[i];
    }
    double meanX = sumX / rows;
    double meanY = sumY / rows;
    
    // Calculate correlation coefficient
    double numerator = 0.0;
    double denomX = 0.0;
    double denomY = 0.0;
    
    for (int i = 0; i < rows; ++i) {
        double xDiff = X[i] - meanX;
        double yDiff = Y[i] - meanY;
        numerator += xDiff * yDiff;
        denomX += xDiff * xDiff;
        denomY += yDiff * yDiff;
    }
    
    if (denomX <= 0.0 || denomY <= 0.0) {
        return 0.0; // To avoid division by zero
    }
    
    return numerator / sqrt(denomX * denomY);
}

// Fisher-Yates shuffle algorithm for permuting an array
void fisherYatesShuffle(vector<double>& arr, mt19937& gen, int n) {
    for (int i = n - 1; i > 0; --i) {
        uniform_int_distribution<> dist(0, i);
        int j = dist(gen);
        // Swap elements
        double temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Function to perform the permutation test
double performPermutationTest(const vector<double>& X, vector<double> Y, int rows, 
                           int numPermutations, double originalCorr, mt19937& gen) {
    int countEqual = 0;
    
    for (int i = 0; i < numPermutations; ++i) {
        // Shuffle Y array while keeping X fixed
        fisherYatesShuffle(Y, gen, rows);
        
        // Calculate correlation after shuffling
        double shuffledCorr = calculateCorrelation(X, Y, rows);
        
        // Count permutations with correlation at least as extreme as the original
        if (abs(shuffledCorr) >= abs(originalCorr)) {
            ++countEqual;
        }
    }
    
    // Return two-tailed p-value
    return static_cast<double>(countEqual) / numPermutations;
}

int main(int argc, char **argv) {
    // Check if theres sufficient arguments 
    if (argc < 3) {
        cout << "Use as:  " << argv[0] << " <Tab-delimited text file> <Number of permutations>\n";
        cout << "Example: " << argv[0] << " Table.txt 10000\n";
        return 0;
    }
    
    // Parse the number of permutations
    int numPermutations;
    try {
        numPermutations = stoi(argv[2]);
        if (numPermutations <= 0) {
            cerr << "Number of permutations must be positive\n";
            return 1;
        }
    } catch (...) {
        cerr << "Invalid number of permutations\n";
        return 1;
    }
    
    // Open input file
    ifstream InFile(argv[1]);
    if (!InFile.is_open()) {
        cerr << "Cannot open file \"" << argv[1] << "\"\n";
        return 1;
    }
    
    // Count number of lines
    int rows = 0;
    char Z;
    while (InFile.get(Z)) {
        if (Z == '\n') ++rows;
    }
    --rows; // Adjust for the last line if needed
    cout << "Number of rows in the file: " << rows << "\n";
    
    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());
    
    // Return to the start of the file
    InFile.clear();
    InFile.seekg(0, InFile.beg);
    
    // Use vectors instead of fixed arrays for flexibility
    vector<double> X(rows), Y(rows);
    
    // Read data from file
    string Str;
    for (int i = 0; i < rows; ++i) {
        getline(InFile, Str, '\t');
        X[i] = stof(Str);
        getline(InFile, Str);
        Y[i] = stof(Str);
    }
    
    // Check if there is any text after the last '\n'
    if (getline(InFile, Str)) {
        cerr << "WARNING: there is text after the last end-of-line: \"" << Str << "\"\n";
    }
    
    // Print the data
    cout << "Data read from the file:\nX\tY\n";
    for (int i = 0; i < rows; ++i) {
        cout << X[i] << "\t" << Y[i] << "\n";
    }
    
    // Calculate the original correlation coefficient
    double originalCorr = calculateCorrelation(X, Y, rows);
    cout << "Original correlation coefficient: " << originalCorr << "\n";
    
    // Perform permutation test using random sampling
    cout << "Using " << numPermutations << " random permutations...\n";
    double pValue = performPermutationTest(X, Y, rows, numPermutations, originalCorr, gen);
    
    // Output results
    cout << "Correlation coefficient: " << originalCorr << "\n";
    cout << "Two-tailed p-value: " << pValue << "\n";
    
    return 0;
}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
