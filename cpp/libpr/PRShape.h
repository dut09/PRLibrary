/*
Tao Du
taodu@stanford.edu
May 7, 2014
*/

#ifndef _LIBPR_PRSHAPE_H_
#define _LIBPR_PRSHAPE_H_

//	here we define a shape interface for ray tracing
//	all the geometries: triangle, box, triangle mesh
//	should inherit from this class
//	and implement the following interfaces:
//		doesIntersect
//		getIntersection
//		getIntersectionT
//	the convention of the three virutal functions
//	getIntersection:
//		return DBL_MAX point, DBL_MAX normal and DBL_MAX t if the shape
//		and l do not intersect for any reason
//	doesIntersect
//		return false if l and the shape do not intersect for any reason
//	getIntersectionT
//		return DBL_MAX if l and the shape do not intersect for any reason

#include "PRIntersection.h"
#include "PRLine.h"
#include "PRBBox.h"

class PRShape
{
public:
	virtual PRIntersection getIntersection(const PRLine &l) = 0;
	virtual bool doesIntersect(const PRLine &l) = 0;
	virtual double getIntersectionT(const PRLine &l) = 0;
	virtual PRBBox getBBox() = 0;
};

#endif 