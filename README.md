# Programming Project II

This project aims to develop a compiler register allocation tool using graph coloring. The goal is to assign live webs to a limited number of physical registers, ensuring that interfering webs are not assigned to the same register.
The live ranges from the input are merged into webs, which are then used to build an interference graph. Register allocation is solved by applying a greedy graph coloring heuristic, where each color represents a physical register.
When allocation is not possible with the available registers, the project also supports web spilling and web splitting strategies.


## Compilation and Execution of the Program

### Compile using Makefile
- make clean  #optional
- make
### Run in Interactive Mode
- ./myProg

### Run in Batch Mode
- ./myProg -b ranges.txt registers.txt allocation.txt

where `ranges.txt` is the input file containing the live ranges, `registers.txt` is the input file containing the register settings and `allocation.txt` is the output file where the results will be written. If the files are located in folders, the full path must be provided.


## T2.1 Basic Register Allocation

The basic register allocation was implemented using graph coloring.
First, the program live ranges are merged into webs. Each web represents a value that may need a register. Then, an interference graph is created, where each node is a web and each edge means that two webs are alive at the same time. Therefore, connected webs cannot use the same register.
The algorithm tries to color the graph with the smallest possible number of registers. It starts with 1 register and increases the number until the maximum allowed value.

For each number of registers K, the algorithm:
1. Removes nodes with degree smaller than K and stores them in a stack.
2. Colors the nodes in reverse order, choosing the first available register not used by their neighbors.

If the graph can be colored, the allocation succeeds and each web receives a register. If not, the allocation fails and the webs are assigned to memory (M).


## T2.2 Register Allocation with Web Spilling

The web spilling algorithm is used only when it is selected in the `registers.txt` file and the basic register allocation fails.

First, the program tries to color the original interference graph using the basic graph coloring algorithm. If this succeeds, no spilling is performed.
If the basic algorithm fails, the program tries to remove some webs from the graph, up to the limit given in the input parameter. It starts by trying to spill only one web. If no valid allocation is found, it then tries combinations of two webs, and so on, until the maximum number of allowed spilled webs is reached.

For each attempt, the selected webs are removed from the interference graph and assigned to memory. The remaining webs are then allocated using the basic graph coloring algorithm.
The rationale used for choosing the webs to spill is exhaustive search within the allowed limit. Instead of choosing only one web using a simple heuristic, the implementation tests different combinations of webs. This increases the chance of finding a valid allocation while still respecting the maximum value provided in the input file.

If a valid coloring is found after spilling, the non-spilled webs are assigned to registers and the spilled webs are assigned to memory. If no valid allocation is possible within the allowed number of spills, the allocation fails.


## T2.3 Register Allocation with Web Splitting


## T2.4 Register Allocation of Your Own


# Documentation

The HTML documentation generated with Doxygen can be found in the html directory, in the index.html

## Contributions

This project was developed by:
- Daniela Sousa up202406448
- João Costa up202403747
- Rodrigo Correia up202405057

Class 12 - Group 5