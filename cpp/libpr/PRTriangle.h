/*
Tao Du
taodu@stanford.edu
April 16, 2014
*/

//	a triangle class
#ifndef _LIBPR_PRTRIANGLE_H_
#define _LIBPR_PRTRIANGLE_H_

#include "PRShape.h"
#include "PRBBox.h"

//	this class should implement
//	the virtual class PRShape
class PRTriangle : public PRShape
{
public:
	PRTriangle();
	PRTriangle(const PRVector3& v1, const PRVector3& v2, const PRVector3& v3);
	PRTriangle(const PRVector3& v1, const PRVector3& v2, const PRVector3& v3,
		const PRVector3& c1, const PRVector3& c2, const PRVector3& c3);
	~PRTriangle();
	//	overload operator
	//	it is not allowed to modify the data
	//	so we return by value
	PRVector3 operator[](int i) const;
	
	//	get normal
	//	the normal is defined on right hand rule
	//	and the normal is guaranteed to be normalized
	PRVector3 getNormal() const {return m_normal;}
	//	compute the barycentric weight of a given p

	//	the convention of barycentric weight:
	//	if p contains DBL_MAX or p is not in the plane
	//	return (0.0, 0.0, 0.0)
	//	otherwise return the barycentric weight
	//	it might contain negative numbers!
	PRVector3 getBarycentricWeight (const PRVector3 &p) const;
	
	//	get the area of the triangle
	double getArea() const {return m_area;}

	PRTriangle transform(const PRTransform &t);

	//	the convention of getIntersection and getIntersectionT
	//	if the line and the triangle are parallel
	//	return DBL_MAX and (DBL_MAX, DBL_MAX, DBL_MAX)

	//	virtual functions
	PRIntersection getIntersection(const PRLine &l);
	bool doesIntersect(const PRLine &l);
	double getIntersectionT(const PRLine &l);
	//	get bounding box
	PRBBox getBBox() {return m_bbox;}

	PRVector3 getColor(const PRVector3& p);

private:
	//	data member
	PRVector3 m_vertices[3];
	//	m_color stores the color 
	//	from the obj file
	//	this field is not mandatory
	PRVector3 m_color[3];
	//	m_normal stores the normal of the triangle
	PRVector3 m_normal;
	double m_area;
	//	bounding box
	PRBBox m_bbox;
};

#endif