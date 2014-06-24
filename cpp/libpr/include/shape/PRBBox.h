/*
Tao Du
taodu@stanford.edu
May 6, 2014
*/

//	the axis aligned bounding box class
//	used in BVH acceleration structure
//	it is inherited from the virtual class PRShape

#ifndef _LIBPR_PRBBOX_H_
#define _LIBPR_PRBBOX_H_

#include "PRShape.h"

class PRBBox
{
public:
	//	default constructor
	//	will init pMin = +Infty
	//	and init pMax = -Infty
	PRBBox();
	//	use pMin and pMax to init the box
	PRBBox(PRVector3 pMin, PRVector3 pMax);
	//	use 6 double numbers to init the box
	PRBBox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);

	//	virtual function interfaces
	bool doesIntersect(const PRLine &l);
	PRIntersection getIntersection(const PRLine &l);
	double getIntersectionT(const PRLine &l);

	//	get pMin
	PRVector3 getMin() {return m_pMin;}
	//	get pMax
	PRVector3 getMax() {return m_pMax;}
	//	get the center of the box
	PRVector3 getCenter() {return m_center;}
	
	//	find the longest edge in the box
	//	and return its id
	//	0 -> x, 1 -> y, 2 -> z
	int getMaxExtentId();
	//	find the shortest edge in the box
	//	and return its id
	//	0 -> x, 1 -> y, 2 -> z
	int getMinExtentId();
	
	//	decide whether a point is inside
	bool isInside(const PRVector3 &p);

	//	static functions
	//	combine two bounding box together
	static PRBBox combine(const PRBBox &b1, const PRBBox &b2);
	//	combine a bounding box and a point together
	static PRBBox combine(const PRBBox &b, const PRVector3 &p);
	//	combine two bounding box together
	//	by their pointers
	static PRBBox* combine(const PRBBox *b1, const PRBBox *b2);

private:
	//	data member
	PRVector3 m_center;			//	the center of the bounding box
	PRVector3 m_pMin, m_pMax;	//	the min and max vertex of the bounding box
};

#endif