#include <stdio.h>
#include "distanceTest.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
int *get_indices(int N, int k) {
  int *indices = malloc(k * sizeof(int));
  srand(time(NULL));

  for (int i = 0; i < k; i++) {
    int index = rand() % N;
    for (int j = 0; j < i; j++) {
      if (indices[j] == index) {
        index = rand() % N;
        j = -1;
      }
    }
    indices[i] = index;
  }
  return indices;
}

struct Point* get_subset(struct Point* points, int * indices, int sub_sample_size){
	struct Point* subset = malloc(sub_sample_size*sizeof(struct Point));
	for(int i = 0; i< sub_sample_size; ++i){
		memcpy(&subset[i], &points[indices[i]],sizeof(struct Point));
	}
	return subset;
}

long int get_median_scalar(struct Point *points, int len, int current_level){
	long int *values =(long int*) malloc(len * sizeof(long int));
	long int med;
	if(current_level % 3 == 1){
		for (int i = 0; i < len; i++) {
			memcpy(&values[i],  &points[i].x,sizeof(long int));
		}
	}
	else if(current_level % 3 == 2){
		for (int i = 0; i < len; i++) {
			memcpy(&values[i], &points[i].y,sizeof(long int));
		}
	}
	else { //we consider z-axis
		for (int i = 0; i < len; i++) {
			memcpy(&values[i], &points[i].z, sizeof(long int));
		}
	}
	qsort(values, len, sizeof(long int), compare_longs);
	if (len % 2 == 0) {
  		med =  (values[len / 2 - 1] + values[len / 2]) / 2;
	}
	else {
		med = values[len / 2];
  	}	
	free(values);
	return med;
}

int compare_longs(const void *a, const void *b) {
  const long int *da = (const long int *)a;
  const long int *db = (const long int *)b;
  return (*da > *db) - (*da < *db);
}
