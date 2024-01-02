# CS6135 HW3 Report

## Compilation and Execution
- **Directory**: `HW3/src/`
- **Usage**: `../bin/hw3 <*.hardblocks> <*.nets> <*.pl> <*.floorplan> <dead_space_ratio>`
- **Example**: `$ ../bin/hw3 ../testcase/n100.hardblocks ../testcase/n100.nets ../testcase/n100.pl ../output/n100.floorplan 0.1`

## Performance Metrics
In my approach, I did not specifically account for the dead space ratio; instead, I focused on optimizing the layout to fit within the fixed outline constraints. Initially, I conducted experiments using a dead space ratio of 0.1, leading to identical results for both 0.1 and 0.15 ratios. The variance in runtime for Case 1 and others stems from the use of distinct parameters in each scenario, influencing the time it takes to conform to the fixed outline. Consequently, there isn't a direct correlation between the execution time and the number of modules."

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
