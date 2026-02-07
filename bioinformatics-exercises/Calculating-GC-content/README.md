# Calculating GC Content (C++)

## Overview
This folder contains a series of C++ programs developed to compute GC content from biological sequence data. The exercises progress in complexity, demonstrating iterative refinement of algorithms for sequence analysis.

GC content is a fundamental metric in genomics, commonly used for genome characterization, quality control, and comparative sequence analysis.

## Programs
- `050-GCs1.cpp` — introductory GC content calculation for a single sequence
- `051-GCs2.cpp` — extended GC calculation with improved input handling and validation
- `052-GCs3.cpp` — advanced GC analysis with support for multiple sequences and/or file-based input

## Concepts Demonstrated
- String traversal and character counting
- Conditional logic and loops
- File I/O for biological data
- Incremental algorithm refinement
- Applying C++ fundamentals to bioinformatics problems

## Build & Run
Each program can be compiled using:
```bash
g++ -std=c++17 filename.cpp -o gc
./gc

