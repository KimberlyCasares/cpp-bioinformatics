# FASTA Sequence Analysis Tool (C++ + R)

## Overview
This exercise implements a C++ command-line tool for processing biological sequence data from FASTA files and generating ranked sequence metrics. The output is designed for downstream statistical analysis and visualization in R.

## Features
- Parses multi-sequence FASTA input files
- Filters sequences based on length thresholds
- Computes per-sequence metrics (e.g., hydrophobic residue percentage)
- Ranks sequences by computed values
- Outputs structured results for statistical analysis

## File Structure
- `6_sequence_analysis.cpp` — main C++ program
- `6_SampleInput.fasta` — example FASTA input
- `6_SampleOutput.txt` — example program output
- `6_explanation_documentation.docx` — exercise write-up and results

## How to Compile and Run
```bash
g++ -std=c++17 6_sequence_analysis.cpp -o hw6
./hw6 "Analysis 6_SampleInput.fasta" > 6_SampleOutput.txt

