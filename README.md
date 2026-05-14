# Programming Project II

This project develops a compiler register allocation tool based on graph coloring. The goal is to assign live webs to a limited number of physical registers while ensuring that interfering webs are never assigned to the same register.
The live ranges provided in the input files are merged into webs and these webs are used to build an interference graph. In this graph, each node represents a web and each edge represents an interference between two webs.
Register allocation is then performed using graph coloring heuristics, where each color represents a physical register. When allocation is not possible with the available registers, the project also supports web spilling, web splitting and a custom free allocation strategy.

The only changes made to `graph.h` were the addition of Doxygen comments.


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

The web splitting algorithm is used only when it is selected in the `registers.txt` file and the basic register allocation fails.

First, the program attempts to color the original interference graph using the basic graph coloring algorithm. If this succeeds, no splitting is performed.
If the allocation fails, the program tries to split webs in order to reduce interference in the graph and make coloring possible. The algorithm starts by splitting only one web. If no valid allocation is found, it then tries combinations of two webs, and so on, up to the maximum number of splits allowed by the input parameter.

The implementation uses an exhaustive search strategy within the allowed limit. Instead of selecting a single web using a fixed heuristic, the algorithm tests different combinations of webs to split. For each selected web, all possible split points between the beginning and the end of the web are tested.

When a split point is chosen, the original web is replaced by two smaller webs. Although this increases the number of webs, each new web has a shorter live range and may interfere with fewer webs. This can reduce the number of edges in the interference graph, making graph coloring easier.

After each splitting attempt, the basic graph coloring algorithm is executed again. If a valid coloring is found, the resulting webs are assigned to registers. Otherwise, the algorithm continues testing other splitting possibilities until the maximum allowed number of splits is reached.

## T2.4 Register Allocation of Your Own

Our own algorithm implements a custom register allocation strategy based on the DSATUR graph coloring heuristic.

First, the program builds the interference graph, where each web is represented as a node and each interference between two webs is represented as an edge.

The DSATUR heuristic chooses the next web to color according to its saturation degree, meaning the number of different register colors already used by its neighbouring webs. The idea is to prioritize the webs with fewer register choices available. If there is a tie, the algorithm chooses the web with the highest degree in the interference graph, meaning the web that interferes with the largest number of other webs.
If the graph can be successfully colored using the available registers, the allocation is completed.
If coloring fails, the algorithm removes the web with the highest degree from the graph, since removing the web with the largest number of interferences is more likely to simplify the graph. The removed web is considered spilled to memory. The algorithm then rebuilds the reduced graph and tries to color it again.
This process is repeated until a valid coloring is found or until no solution is possible.

This approach respects the main restriction of the project: two webs that interfere are never assigned the same register.

# Documentation

The HTML documentation generated with Doxygen can be found in the html directory, in the index.html

## Contributions

This project was developed by:
- Daniela Sousa up202406448
- João Costa up202403747
- Rodrigo Correia up202405057

Class 12 - Group 5