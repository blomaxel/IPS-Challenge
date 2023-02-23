#include "distanceTest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

int count_neighbours(struct Point* points, int len){
	int count = 0;
	for(int i = 0; i < (len-1); ++i){
		for(int j = (i+1); j < len; ++j){
			count += is_neighbour(points[i], points[j]);
		}
	}
	return count;
}

int count_border_neighbours(void* arg){
	struct border_args* arguments = arg;
	struct Point* points1 = arguments->points1;
	struct Point* points2 = arguments->points2;
	int len1 = arguments->len1;
	int len2 = arguments->len2;
	int count = 0;
	for(int i = 0; i < len1; ++i){
		for(int j = 0; j < len2; ++j){
			count += is_neighbour(points1[i], points2[j]);
		}
	}
	return count;
}


int is_neighbour(struct Point p1,struct Point p2){
    return ((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)+ (p1.z-p2.z)*(p1.z-p2.z) <= MAXDISTSQ);
}

/*Divide_conquer is a recursive function. In the leaves of the tree, it simply calls count_neighbours,
which in turn returns the number of neighbours within the given subset, here denoted by "arguments->points".
If divide_conquer acts on the root or an intermidiate node, it finds the median along either x, y, or z,
divides points into two approximately equally large subsets based upon the median point, then calls divide_conquer 
for each of the two subsets. Divide conquer also check which points that are close to where points is split,
and calls count_border_neighbours.*/
int divide_conquer(void* arg){
	struct args* arguments = arg;
	struct Point* points = arguments->points;
	int len = arguments->len;
	int levels = arguments->levels;
	int current_level = arguments->current_level;
	if(levels == current_level){
		return count_neighbours(points, len);
	}//else	
	double median;
	if (len > SUB_SAMPLE_SIZE){
		int *indices = get_indices(len, SUB_SAMPLE_SIZE);
		struct Point* subset = get_subset(points, indices, SUB_SAMPLE_SIZE);
		free(indices);
		median = get_median_scalar(subset, SUB_SAMPLE_SIZE, current_level);
		free(subset);
	}
	else{
		return count_neighbours(points,len);
	}

	//We allocate memory for lefts and rights, respectively. 
	//these sets are yet of unknown size, which is why we declare counters to keep track of the number fo values.
	struct Point* lefts = (struct Point*)malloc(sizeof(struct Point)*len);
	struct Point* rights = (struct Point*) malloc(sizeof(struct Point)*(len));
	int l_count = 0;
	int r_count = 0;
	//We allocate memory for the sets of close-to-border points. 
	//these sets are yet of unknown size, which is why we declare counters to keep track of the number fo values.
	struct Point* l_border =(struct Point*) malloc(sizeof(struct Point)*(len));
	struct Point* r_border =(struct Point*) malloc(sizeof(struct Point)*(len));
	int r_border_count = 0;
	int l_border_count = 0;
	if(current_level % 3 == 1){//modulu 1 corresponds to x-split
		for(int i = 0; i < len; i++){
			if(points[i].x <= median){
				memcpy(&lefts[l_count], &points[i], sizeof(struct Point));
				l_count++;
				if(points[i].x >= median - MAXDIST){
					memcpy(&l_border[l_border_count],&points[i],sizeof(struct Point));
					l_border_count++;
				}
			}
			else{// if(points[i].x > median)
				memcpy(&rights[r_count], &points[i], sizeof(struct Point));
				r_count++;
				if(points[i].x <= median + MAXDIST){
					memcpy(&r_border[r_border_count], &points[i], sizeof(struct Point));
					r_border_count++;
				}
			}
		}
	}
	else if (current_level % 3 == 2){//modulu 2 corresponds to y-split
		for(int i = 0; i < len; i++){
			if(points[i].y <= median){
				memcpy(&lefts[l_count], &points[i], sizeof(struct Point));
				l_count++;
				if(points[i].y >= median - MAXDIST){
					memcpy(&l_border[l_border_count],&points[i],sizeof(struct Point));
					l_border_count++;
				}
			}
			else { //if(points[i].y > median)
				memcpy(&rights[r_count], &points[i], sizeof(struct Point));
				r_count++;
				if(points[i].y <= median + MAXDIST){
					memcpy(&r_border[r_border_count], &points[i], sizeof(struct Point));
					r_border_count++;
				}
			}
		}
	}
	
	else if (current_level % 3 == 0){//modulu 0 corresponds to z-split
		for(int i = 0; i < len; i++){
			if(points[i].z <= median){
				memcpy(&lefts[l_count], &points[i], sizeof(struct Point));
				l_count++;
				if(points[i].z >= median - MAXDIST){
					memcpy(&l_border[l_border_count],&points[i],sizeof(struct Point));
					l_border_count++;
				}
			}
			else {// if(points[i].z > median)
				memcpy(&rights[r_count], &points[i], sizeof(struct Point));
				r_count++;
				if(points[i].z <= median + MAXDIST){
					memcpy(&r_border[r_border_count], &points[i], sizeof(struct Point));
					r_border_count++;
				}
			}
		}
	}
	// create threads
	thrd_t thread_border, thread_left, thread_right;
	//free(points);
	//printf("checkpoint 1. Rights has length %d and lefts has length %d\n", r_count, l_count);
	struct border_args b_args;
	b_args.points1 = l_border;
	b_args.points2 = r_border;
	b_args.len1 = l_border_count;
	b_args.len2 = r_border_count;
	int r = thrd_create(&thread_border,count_border_neighbours, (void *) &b_args);
	if ( r != thrd_success ) {
     		 fprintf(stderr, "failed to create thread\n");
      		exit(1);
    	}
	struct args l_args;
	l_args.points = lefts;
	l_args.len = l_count;
	l_args.levels = levels;
	l_args.current_level = current_level +1;
	r = thrd_create(&thread_left, divide_conquer, &l_args);
	if ( r != thrd_success ) {
     		 fprintf(stderr, "failed to create thread\n");
      		exit(1);
    	}
	struct args r_args;
	r_args.points = rights;
	r_args.len = r_count;
	r_args.levels = levels;
	r_args.current_level = current_level +1;
	r = thrd_create(&thread_right, divide_conquer, &r_args);
	if ( r != thrd_success ) {
     		 fprintf(stderr, "failed to create thread\n");
      		exit(1);
    	}
	int l_neighbours, r_neighbours, b_neighbours;
	thrd_join(thread_border, &b_neighbours);
	thrd_join(thread_left, &l_neighbours);
	thrd_join(thread_right, &r_neighbours);
	free(rights);
	free(r_border);
	free(l_border);
	free(lefts);
	return b_neighbours + l_neighbours +r_neighbours;
}
