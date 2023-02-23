#ifndef DISTANCETEST_H
#define DISTANCETEST_H
static const int SUB_SAMPLE_SIZE =  49;
static const long int MAXDIST=  50000;//in micrometers ;)
static const long int MAXDISTSQ = 2500000000;
static const char FILENAME[] = "positions.xyz";
struct Point {
    long int x, y, z;
};

struct args{
	struct Point* points;
	int levels, current_level, len;
};

struct border_args{
	struct Point* points1;
	struct Point* points2;
	int len1, len2;
};
int parse_file(const char* filename, struct Point* points);
int count_lines(const char *filename);
int is_neighbour(struct Point p1,struct Point p2);
int count_neighbours(struct Point* points, int len);
int count_border_neighbours(void* arg);
int divide_conquer(void* arg);
int *get_indices(int N, int k);
struct Point get_median(struct Point* points, int len);
long int get_median_scalar(struct Point* points, int len, int current_level);
int compare_longs(const void* a, const void* b);
struct Point* get_subset(struct Point* points, int* indices, int sub_sample_size);
#endif
