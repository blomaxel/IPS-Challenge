#include <stdio.h>
#include "distanceTest.h"
#include <stdlib.h>
#include <time.h>
#include <limits.h>
int count_lines(const char *filename) {
  int count = 0;
  char buffer[1024];
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Error opening file");
    return -1;
  }

  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    count++;
  }

  if (fclose(fp) != 0) {
    perror("Error closing file");
    return -1;
  }
  return count;
}

int parse_file(const char *filename, struct Point* points){
	FILE *fp = fopen(filename, "r");
	int num_points = 0;
	double x,y,z;
	if (fp == NULL) {
		perror("Error opening file");
		return -1;
 	}
	while(fscanf(fp, "%lf %lf %lf", &x,&y,&z) == 3){
		points[num_points].x = (long int) (x*1e6 - INT_MIN);
		points[num_points].y = (long int) (y*1e6 - INT_MIN);
		points[num_points].z = (long int) (z*1e6 - INT_MIN);
		num_points++;
	}
	if (fclose(fp) != 0) {
   		perror("Error closing file");
    		return -1;
  	}
	return 0;
}
