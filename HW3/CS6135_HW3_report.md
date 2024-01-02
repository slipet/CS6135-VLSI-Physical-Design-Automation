# CS6135 HW3 Report

## Compilation and Execution
- **Directory**: `HW3/src/`
- **Usage**: `../bin/hw3 <*.hardblocks> <*.nets> <*.pl> <*.floorplan> <dead_space_ratio>`
- **Example**: `$ ../bin/hw3 ../testcase/n100.hardblocks ../testcase/n100.nets ../testcase/n100.pl ../output/n100.floorplan 0.1`

## Performance Metrics
- Wirelength and runtime for test cases with dead space ratios 0.1 and 0.15.
- Includes I/O time, initial floorplanning time, and computation parts.
- Example: `Case1: IO time 0s, B*tree init 1.08s, Simulated annealing time 1037.48s, Runtime 1038.48s, Wire length 293900`

## Dead Space Ratio Analysis
- Smallest dead space ratio for legal results in 20 minutes.
- Example: `Case1: 0.0983`

## Algorithm Details
- Utilized B* tree and fixed-outline cost function methodologies.
- Flow chart and pseudo-code used to elaborate the algorithm.

## Enhancements and Comparison
- Tricks for speeding up the program or enhancing solution quality.
- Comparison with top students' results from the previous year.

## Learning and Challenges
- Insights and problems encountered during the assignment.
- Focus on algorithm design and experimental parameter testing.

(Note: This is a summarized version, focusing on key elements of the report)

