#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "distanceTest.h"
#include <threads.h>
int main(int argc, char **argv) {
    	int tree_levels;
	if (argc == 1){
		tree_levels = 4; //default value
    	}
    	else{
    		tree_levels = (int) atoi(&argv[1][1]);//we assume the argument is on the form "-5"
		printf("you chose %d as number of tree levels\n", atoi(&argv[1][1]));
    	}
	int pairs = 0;//This variable holds the result of the program's work
	//We read the file, and store the coordinates in a list of struct Point.
	int num_lines = count_lines(FILENAME);
	printf("There are %d particles in the file %s\n",num_lines, FILENAME);
	struct Point* points = malloc(num_lines*sizeof(struct Point));	
	int parsed = parse_file(FILENAME, points);
	struct args arguments;
	arguments.points = points;
	arguments.len = num_lines;
	arguments.levels = tree_levels;
	arguments.current_level = 1;
	pairs = divide_conquer((void*) &arguments);
	free(points);
	printf("We have a total of %d pairs of particles within the distance of 0.05 meters from each other\n", pairs);
	return 0;
}
