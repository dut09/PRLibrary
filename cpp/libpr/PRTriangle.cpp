/*
Tao Du
taodu@stanford.edu
April 16, 2014
*/
#include "prStd.h"
#include "PRTriangle.h"

PRTriangle::PRTriangle()
{
	m_vertices[0] = m_vertices[1] = m_vertices[2] = PRVector3();
	m_normal = PRVector3();
	m_area = 0.0;
	m_bbox = PRBBox(PRVector3(), PRVector3());
}

PRTriangle::PRTriangle(const PRVector3& v1, const PRVector3& v2, const PRVector3& v3)
{
	m_vertices[0] = v1;
	m_vertices[1] = v2;
	m_vertices[2] = v3;
	m_normal = PRVector3::cross(v2 - v1, v3 - v2);

	//	compute the area of the triangle
	m_area = 0.5 * m_normal.length();
	//	normalize the normal!
	if (m_area < DBL_EPSILON)
		m_normal = PRVector3(0.0, 0.0, 0.0);
	else
		m_normal = m_normal / (2 * m_area);

	//	compute the bounding box
	PRVector3 pMin = PRVector3::min(v1, PRVector3::min(v2, v3));
	PRVector3 pMax = PRVector3::max(v1, PRVector3::max(v2, v3));
	m_bbox = PRBBox(pMin, pMax);
}

PRTriangle::~PRTriangle()
{
	//	empty deconstructor function
}

//	overload operator
//	it is not allowed to modify the data
//	so we return by value
PRVector3 PRTriangle::operator[](int i) const
{
	//	i can only be 0, 1 or 2
	assert(i >= 0 && i < 3);
	return m_vertices[i];
}
	
//	compute the barycentric weight of a given p
//	if p is not in the plane, return DBL_MAX, DBL_MAX, DBL_MAX
PRVector3 PRTriangle::getBarycentricWeight(const PRVector3 &p) const
{
	//	if p is (DBL_MAX || DBL_MAX || DBL_MAX)
	//	return (0.0, 0.0, 0.0)
	if (p.x == DBL_MAX 
		|| p.y == DBL_MAX
		|| p.z == DBL_MAX)
	{
		return PRVector3(DBL_MAX, DBL_MAX, DBL_MAX);
	}
	//	compute the area of the three triangles
	PRVector3 v[3];
	//	v = triangle vertices - p
	for (int i = 0; i < 3; i++)
	{
		v[i] = m_vertices[i] - p;
	}
	//	use cross product to compute the area of the triangles
	PRVector3 vArea[3];
	vArea[0] = PRVector3::cross(v[1], v[2]);
	vArea[1] = PRVector3::cross(v[2], v[0]);
	vArea[2] = PRVector3::cross(v[0], v[1]);
	//	compute the signed area of each of the three triangles
	double area[3];
	//	if the dot product between vArea and m_normal is positive
	//	then the corresponding triangle is positive
	//	otherwise it is negative
	double areaSum = 0.0;
	for (int i = 0; i < 3; i++)
	{
		area[i] = vArea[i].length() * 0.5 
			* (PRVector3::dot(vArea[i], m_normal) > 0.0 ? 1.0 : -1.0);
		areaSum += area[i];
	}
	//	if the sum of areas are not equal to m_area
	//	then the point is not in the triangle plane
	//	here we use a relaxed error bound
	if (abs(areaSum - m_area) > FLT_EPSILON)
	{
		return PRVector3(DBL_MAX, DBL_MAX, DBL_MAX);
	}
	//	return the barycentric weight
	return PRVector3(area[0], area[1], area[2]) / m_area;
}

PRTriangle PRTriangle::transform(const PRTransform &t)
{
	m_vertices[0] = t * m_vertices[0];
	m_vertices[1] = t * m_vertices[1];
	m_vertices[2] = t * m_vertices[2];
	return PRTriangle(m_vertices[0], m_vertices[1], m_vertices[2]);
}

//	get the intersection point
//	be careful to test the normal
PRIntersection PRTriangle::getIntersection(const PRLine& l)
{
	PRIntersection inter;
	inter.normal = PRVector3(DBL_MAX, DBL_MAX, DBL_MAX);
	inter.point = PRVector3(DBL_MAX, DBL_MAX, DBL_MAX);
	inter.t = DBL_MAX;
	double t = getIntersectionT(l);
	if (t == DBL_MAX)
		return inter;
	inter.t = t;
	inter.point = l(t);
	inter.normal = m_normal;
	return inter;
}

bool PRTriangle::doesIntersect(const PRLine &l)
{
	return getIntersectionT(l) != DBL_MAX;
}

//	get the parameter of the intersection point in line
//	if the line and the triangle are parallel
//	return DBL_MAX
//	modification:
//	for any other cases where the line does not intersect
//	with the triangle, we will return DBL_MAX
double PRTriangle::getIntersectionT(const PRLine &l)
{
	//	test whether the line and the plane are parallel
	PRVector3 o = l.getOrigin();
	PRVector3 d = l.getDirection();
	//	if they are parallel, return DBL_MAX
	double c = PRVector3::dot(d, m_normal);
	if (abs(c) < FLT_EPSILON)
		return DBL_MAX;

	//	compute the solution
	//	line equation: o + t*d = p
	//	where p is the intersection point
	//	triangle plane equation
	//	(p - v[0]) * m_normal = 0
	//	solve:
	//	(o + t*d - v[0]) * normal = 0
	//	or equally:
	//	t*(d * normal) + (o - v[0]) * normal = 0
	//	t*c + (o - v[0]) * normal = 0
	double t = PRVector3::dot(o - m_vertices[0], m_normal) / -c;
	if (t <= 0.0)
		return DBL_MAX;
	PRVector3 w = getBarycentricWeight(l(t));
	if ((w.x == DBL_MAX && w.y == DBL_MAX && w.z == DBL_MAX)
		|| w.x < 0.0 || w.y < 0.0 || w.z < 0.0)
		return DBL_MAX;
	return t;
}
