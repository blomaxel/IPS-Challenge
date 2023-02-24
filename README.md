# IPS-Challenge

This is my attempt to count the number of particle pairs within a distance of <=0.05 (meters) from each other.

The program is written in C.

The makefile uses GNU Compiler Collection (gcc). The command "make" compiles the program and creates an executable called "distanceTest".

When executing, there is an **optional argument** that determines the number of tree levels the program should have (see PROGRAM OUTLINE below). The default is four (4) levels. The argument is passed with a dash (-), example: ./distanceTest -3.

The **output** is simply the integer 0 if successful, but the program also prints the number of pairs of neighbours in the set.

The three-dimensional particle coordinates are available in the file "positions.xyz".

## PROGRAM OUTLINE:
**(1)** *We start at the root of the tree, which corresponds to level 1.

*It randomly select a subset of the particles and uses the median x-coordinate of the subset as estimate of the true median

*It splits the full set at the median

*It creates two threads that handle the newly created subsets in parallel

**(2)** *Each of the threads are now at level 2 of the tree. If the number of tree levels are greater than 2, we repeat (1) but with regards to the y-coordinates.

Next time we arrive at (2) we use the z-coordinates, then x again, then y, etc...

**(3)** If we arrive at the leaves, i.e. the current level equals the number of tree levels, we simply compare all pairs of points within the given set.

**Remark 1**: There is a constant value of the minimal subset size. If a node in the tree has a set of fewer particles 
than this minimal size, it starts comparing the pairs of particles, regardless of its level in the tree.

**Remark 2**: When the file "positions.xyz" is parsed, the program maps the decimal numbers (float or double float) to long unsigned integers. I just realised that this is unnecessary as a regular float generally has no larger size than a long int, so I will reverse this whenever I have time.

**Remark 3**: When splitting the set of particles into 2^(treelevels-1) subsets, there are neighbouring points close to the borders that are separated. This is accounted for in the program and they are all compared exactly once.


## Functionality and further improvements.

The program works especially well for particles that are not uniformly distributed, and it utilises parallel processing. It should in principle work well even for much larger sets than the one in the test file "positions.xyz".
It could be further improved by explicitly taking the hardware conditions into account. 
