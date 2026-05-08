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


## T2.2 Register Allocation with Web Spilling


## T2.3 Register Allocation with Web Spilling


## T2.4 Register Allocation of Your Own


# Documentation

The HTML documentation generated with Doxygen can be found in the html directory, in the index.html

## Contributions

This project was developed by:
- Daniela Sousa up202406448
- João Costa up202403747
- Rodrigo Correia up202405057

Class 12 - Group 5