# CS6135 HW2 Report

## Compilation and Execution
* Compile: make
* Execute: ../bin/hw2 <path/*.nets> <path/*.cells> <path/*.out>
* Example: ../bin/hw2 ../testcases/p2-3.nets ../testcases/p2-3.cells ../output/p2-3.out

## Performance Metrics

* The final cut size and the runtime of each testcase 

| Case | Cut Size | Run Time (s) |
|------|----------|--------------|
| 1    | 30       | 0.007        |
| 2    | 765      | 3.2          |
| 3    | 16893    | 31.34        |
| 4    | 51809    | 45.38        |
| 5    | 144533   | 114          |

## Run Time Analysis
Runtime  =  $T_{IO}$ + $T_{computation}$

Use clock() to evaluate the time for FM. Storing cells using map can solve the problem of discontinuous cells and eliminates the need for using a hash function.

| Case  | Total Run Time | I/O Time | Computation Time |
|-------|----------------|----------|------------------|
| Case1 | 0.007s         | 0.00s    | 0.07s            |
| Case2 | 3.2s           | 0.04s    | 3.16s            |
| Case3 | 31.34s         | 0.65s    | 30.69s           |
| Case4 | 45.38s         | 1.15s    | 44.23s           |
| Case5 | 114s           | 2.27s    | 111.73s          |

## Implementation Details

### I. Algorithm Comparison
- **Question**: Difference between the implemented algorithm and the FM Algorithm described in class.
- **Answer**: The design is the same as taught in class.

### II. Bucket List Data Structure
- **Question**: Implementation of the bucket list data structure and its conformity with the class slides.
- **Answer**: Implemented using `map<int, list>` to store the bucket list, allowing the use of negative numbers for indexing without additional overload.

### III. Finding Maximum Partial Sum and Restoration
- **Question**: Method for finding the maximum partial sum and restoring the result.
- **Answer**: Recorded each partial sum, located the maximum step, and used two arrays to move cells and restore their original positions.

### IV. Comparison with Top Students
- **Question**: Comparison of results with top students from the previous year, focusing on runtime and solution quality.
- **Answer**: Improved I/O time significantly in the second iteration. The bottleneck is suspected to be in the initial gain calculation, requiring a change in the data structure. For poor solution quality, considering using a random partition at the start for a better initial cut.

### V. Enhancements to Solution Quality or Speed
- **Question**: Additional efforts to enhance solution quality or speed up the program.
- **Answer**: Initially attempted to use a buffer for I/O to speed up reading but shifted to using a map for storing cells, which reduced the need for a hash function and significantly improved speed.

### VI. Learning Outcomes and Challenges
- **Question**: Learnings and problems encountered during the homework.
- **Answer**: 
  1. Importance of deciding on parameters and the overall structure at the beginning to avoid errors and difficulty in debugging.
  2. The necessity of analyzing input files thoroughly, as initial tests with continuous cell numbers in cases 1 and 5 did not reveal issues with discontinuous cell numbers, which were later addressed using a map.


