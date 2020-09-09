#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PRECISION1 32768
#define PRECISION2 16384
/*#define PI 3.1415926535897932*/
#ifndef MIN
#define MIN(x,y) ( (x) < (y) ? (x) : (y) )
#endif

#ifndef MAX
#define MAX(x,y) ((x)>(y)?(x):(y))
#endif

#define SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#define MAXINT 2147483647
#define ASCII_TEXT_BORDER_WIDTH 4
#define MAXHIST 100
#define STEP0 0.01
#define FORWARD 1
#define BACKWARD -1
#define PROJ_DIM 5
#define True 1
#define False 0


typedef struct {
	float x, y, z;
} fcoords;

typedef struct {
	long x, y, z;
} lcoords;

typedef struct {
	int x, y, z;
} icoords;

typedef struct {
	float min, max;
} lims;


/* grand tour history */
typedef struct hist_rec {
	struct hist_rec *prev, *next;
	float *basis[3];
	int pos;
} hist_rec;

static float PYTHAG(float a, float b);
int dsvd(float **a, int m, int n, float *w, float **v);
static float eps(float x);
void pinv(float *a, int m, int n, float *X);