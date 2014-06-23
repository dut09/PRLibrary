/*
Tao Du
taodu@stanford.edu
May 7, 2014
*/

//	here is the class to represent the intersection point
//	in our simple ray tracing program
//	the intersection struct contains
//		an intersection point
//		a nomral(we require the normal to be normalized in intersection)
//		a parameter t

#ifndef _LIBPR_PRINTERSECTION_H_
#define _LIBPR_PRINTERSECTION_H_

#include "PRVector3.h"

struct PRIntersection
{
public:
	//	an empty constructor
	//	the value is undecided
	PRIntersection(){}
	PRIntersection(const PRVector3 &p, const PRVector3 &n, double d)
	{
		point = p;
		normal = n;
		t = d;
	}

	PRVector3 point;
	PRVector3 normal;
	double t;
};

#endif