/*
Tao Du
taodu@stanford.edu
April 8, 2014
*/
#ifndef _LIBPR_PRSTD_H_
#define _LIBPR_PRSTD_H_
//	this header file includes all the .h file from C++

//	assertion
#include <assert.h>

//	math
#define _USE_MATH_DEFINES
#include <math.h>
//	usage from math.h
//	M_PI	pi
//	M_PI_2	pi/2
//	M_PI_4	pi/4
//	M_1_PI	1/pi
//	M_2_PI	2/pi

//	epsilon
#include <float.h>
//	usage from float.h
//	DBL_EPSILON		epsilon between 1 and 1+\varepsilon that is representable
//	this epsilon is not (exactly) correct to judge whether a small number is zero
//	the actual marco is DBL_MIN, but for robustness we use DBL_EPSILON

//	string
#include <string.h>

//	file stream
#include <fstream>

//	vector
#include <vector>

//	map
//	used in loading mesh objective files
#include <map>

//	sorting
//	used in building BVHTree
#include <algorithm>

//	marco definition
//	define the bounding box size / depth image
//	used to cull invalid triangle in rasterization
#define PRCAMERA_BBOX_RATIO			0.02
//	define the closest distance to the origin
//	in camera frame
//	used to cull invalid triangle in rasterization
#define PRCAMERA_MIN_DEPTH	450.0
#define PRCAMERA_MAX_DEPTH	1500.0


#endif