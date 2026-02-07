/*
Kimberly Casares
This program creates a correlation matrix for samples represented by numerical variables.
Reads a tab delimited matrix and outputs a N*M matrix with pearson correlation 
coefficients between pairs of samples. 

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

int main(int argc, char **argv) {
    if (argc < 3) {
        cout << "Use as: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }
    
    ifstream inFile(argv[1]);
    if (!inFile.is_open()) {
        cout << "Cannot open file \"" << argv[1] << "\"\n";
        return 1;
    }
    
    // Read header line to get column names
    string line;
    getline(inFile, line);
    
    // Parse header to get variable names
    vector<string> columnNames;
    string token = "";
    bool firstColumn = true;
    
    for (int i = 0; i <= line.length(); i++) {
        if (i == line.length() || line[i] == '\t') {
            if (!firstColumn && !token.empty()) {
                columnNames.push_back(token);
            }
            token = "";
            firstColumn = false;
        } else {
            token += line[i];
        }
    }
    
    // Read data rows
    vector<string> sampleNames;
    vector<vector<double> > data;
    
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        
        vector<double> row;
        token = "";
        firstColumn = true;
        
        for (int i = 0; i <= line.length(); i++) {
            if (i == line.length() || line[i] == '\t') {
                if (firstColumn) {
                    sampleNames.push_back(token);
                    firstColumn = false;
                } else if (!token.empty()) {
                    row.push_back(stod(token));
                }
                token = "";
            } else {
                token += line[i];
            }
        }
        
        data.push_back(row);
    }
    inFile.close();
    
    int N = sampleNames.size();
    vector<vector<double> > corr(N, vector<double>(N, 0.0));
    
    // Calculate correlations
    for (int i = 0; i < N; i++) {
        corr[i][i] = 1.0;  // Self-correlation is always 1
        
        for (int j = i + 1; j < N; j++) {
            int M = data[i].size();
            
            // Calculate means
            double meanI = 0.0, meanJ = 0.0;
            for (int k = 0; k < M; k++) {
                meanI += data[i][k];
                meanJ += data[j][k];
            }
            meanI /= M;
            meanJ /= M;
            
            // Calculate correlation
            double num = 0.0, denI = 0.0, denJ = 0.0;
            for (int k = 0; k < M; k++) {
                double dI = data[i][k] - meanI;
                double dJ = data[j][k] - meanJ;
                num += dI * dJ;
                denI += dI * dI;
                denJ += dJ * dJ;
            }
            
            double r = (denI > 0 && denJ > 0) ? num / sqrt(denI * denJ) : 0.0;
            corr[i][j] = corr[j][i] = r;  // Store in both positions since matrix is symmetric
        }
    }
    
    // Write output file
    ofstream outFile(argv[2]);
    if (!outFile.is_open()) {
        cout << "Cannot open output file \"" << argv[2] << "\"\n";
        return 1;
    }
    
    // Write header row with sample names
    outFile << "\t";
    for (int i = 0; i < N; i++) {
        outFile << sampleNames[i];
        if (i < N - 1) {
            outFile << "\t";
        }
    }
    outFile << "\n";
    
    // Write correlation matrix (only upper triangle)
    for (int i = 0; i < N; i++) {
        outFile << sampleNames[i];
        for (int j = 0; j < N; j++) {
            outFile << "\t";
            if (i == j) {
                outFile << "*";
            } else if (j > i) {
                outFile << corr[i][j];
            } // Leave lower triangle empty
        }
        outFile << "\n";
    }
    
    outFile.close();
    cout << "Correlation matrix created successfully.\n";
    return 0;
}