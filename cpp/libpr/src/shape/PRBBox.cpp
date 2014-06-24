/*
Tao Du
taodu@stanford.edu
May 6, 2014
*/

#include "../../include/pr.h"

//	default constructor
//	will init pMin = +Infty
//	and init pMax = -Infty
PRBBox::PRBBox()
{
	m_pMin.x = m_pMin.y = m_pMin.z = DBL_MAX;
	m_pMax.x = m_pMax.y = m_pMax.z = -DBL_MAX;
	m_center.x = m_center.y = m_center.z = 0.0;
}

//	use pMin and pMax to init the box
PRBBox::PRBBox(PRVector3 pMin, PRVector3 pMax)
{
	m_pMin = pMin;
	m_pMax = pMax;
	m_center = (m_pMin + m_pMax) / 2;
}

//	use 6 double numbers to init the box
PRBBox::PRBBox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
{
	assert(xmax >= xmin);
	assert(ymax >= ymin);
	assert(zmax >= zmin);
	m_pMin.x = xmin;
	m_pMax.x = xmax;
	m_pMin.y = ymin;
	m_pMax.y = ymax;
	m_pMin.z = zmin;
	m_pMax.z = zmax;
	m_center = (m_pMax + m_pMin) / 2.0;
}

//	the convention of doesIntersect can
//	be found in PRShape.h
bool PRBBox::doesIntersect(const PRLine &l)
{
	return getIntersectionT(l) != DBL_MAX;
}

//	fill an intersection object
//	then return it
//	the convention can be found in PRShape.h
PRIntersection PRBBox::getIntersection(const PRLine &l)
{
	PRIntersection inter;
	inter.normal = PRVector3(DBL_MAX, DBL_MAX, DBL_MAX);
	inter.point = PRVector3(DBL_MAX, DBL_MAX, DBL_MAX);
	inter.t = DBL_MAX;

	PRVector3 d = l.getDirection();
	PRVector3 o = l.getOrigin();
	//	if o is inside the bbox
	//	return DBL_MAX
	if (isInside(o) || (d.x == 0.0 && d.y == 0.0 && d.z == 0.0))
		return inter;

	//	find the t parameter of three pairs of slabs
	PRVector3 tnear, tfar;
	for (int i = 0; i < 3; i++)
	{
		if (d[i] == 0.0)
		{
			if (o[i] > m_pMin[i] && o[i] < m_pMax[i])
			{
				tnear[i] = -DBL_MAX;
				tfar[i] = DBL_MAX;
			}
			else
				return inter;
		}
		else
		{
			double a = 1.0 / d[i];
			if (a >= 0.0)
			{
				tnear[i] = a * (m_pMin[i] - o[i]);
				tfar[i] = a * (m_pMax[i] - o[i]);
			}
			else
			{
				tnear[i] = a * (m_pMax[i] - o[i]);
				tfar[i] = a * (m_pMin[i] - o[i]);
			}
		}
	}
	double tmin = -DBL_MAX;
	double tmax = DBL_MAX;
	int tminId = -1;
	for (int i = 0; i < 3; i++)
	{
		if (tnear[i] > tmin)
		{
			tmin = tnear[i];
			tminId = i;
		}
		if (tfar[i] < tmax)
			tmax = tfar[i];
	}
	if (tmin <= tmax && tmin > DBL_EPSILON)
	{
		//	fill the data in inter
		//	find the normal is the hardest part!
		PRVector3 normal(0.0, 0.0, 0.0);
		normal[tminId] = o[tminId] < m_pMin[tminId] ? -1.0 : 1.0;
		inter.normal = normal;
		inter.point = l(tmin);
		inter.t = tmin;
	}
	return inter;
}

//	the convention for get intersectionT
//	if we can find an intersection t\in[0, +infty)
//	return it
//	in all the other cases return DBL_MAX
double PRBBox::getIntersectionT(const PRLine &l)
{
	PRVector3 d = l.getDirection();
	PRVector3 o = l.getOrigin();
	//	if o is inside the bbox
	//	return DBL_MAX
	if (isInside(o) || (d.x == 0.0 && d.y == 0.0 && d.z == 0.0))
		return DBL_MAX;

	//	find the t parameter of three pairs of slabs
	PRVector3 tnear, tfar;
	for (int i = 0; i < 3; i++)
	{
		if (d[i] == 0.0)
		{
			if (o[i] > m_pMin[i] && o[i] < m_pMax[i])
			{
				tnear[i] = -DBL_MAX;
				tfar[i] = DBL_MAX;
			}
			else
				return DBL_MAX;
		}
		else
		{
			double a = 1.0 / d[i];
			if (a >= 0.0)
			{
				tnear[i] = a * (m_pMin[i] - o[i]);
				tfar[i] = a * (m_pMax[i] - o[i]);
			}
			else
			{
				tnear[i] = a * (m_pMax[i] - o[i]);
				tfar[i] = a * (m_pMin[i] - o[i]);
			}
		}
	}
	double tmin = -DBL_MAX;
	double tmax = DBL_MAX;
	for (int i = 0; i < 3; i++)
	{
		if (tnear[i] > tmin)
			tmin = tnear[i];
		if (tfar[i] < tmax)
			tmax = tfar[i];
	}
	double inter = DBL_MAX;
	if (tmin <= tmax && tmin > DBL_EPSILON)
	{
		inter = tmin;
	}
	return inter;
}

//	find the longest edge in the box
//	and return its id
//	0 -> x, 1 -> y, 2 -> z
int PRBBox::getMaxExtentId()
{
	int maxId = 0;
	PRVector3 edgeLength = m_pMax - m_pMin;
	double maxValue = edgeLength.x;
	if (edgeLength.y > maxValue)
	{
		maxId = 1;
		maxValue = edgeLength.y;
	}
	if (edgeLength.z > maxValue)
	{
		maxId = 2;
		maxValue = edgeLength.z;
	}
	return maxId;
}

//	find the shortest edge in the box
//	and return its id
int PRBBox::getMinExtentId()
{
	int minId = 0;
	PRVector3 edgeLength = m_pMax - m_pMin;
	double minValue = edgeLength.x;
	if (edgeLength.y < minValue)
	{
		minId = 1;
		minValue = edgeLength.y;
	}
	if (edgeLength.z < minValue)
	{
		minId = 2;
		minValue = edgeLength.z;
	}
	return minId;
}

//	decide whether a point is inside
//	the bounding box
bool PRBBox::isInside(const PRVector3 &p)
{
	return p.x >= m_pMin.x && p.x <= m_pMax.x
		&& p.y >= m_pMin.y && p.y <= m_pMax.y
		&& p.z >= m_pMin.z && p.z <= m_pMax.z;
}

//	combine two bounding box together
PRBBox PRBBox::combine(const PRBBox &b1, const PRBBox &b2)
{
	PRBBox b;
	b.m_pMin = PRVector3::min(b1.m_pMin, b2.m_pMin);
	b.m_pMax = PRVector3::max(b1.m_pMax, b2.m_pMax);
	//	update the center
	b.m_center = (b.m_pMax + b.m_pMin) / 2;
	return b;
}

//	combine a bounding box and a point together
PRBBox PRBBox::combine(const PRBBox &b, const PRVector3 &p)
{
	PRBBox b2 = b;
	b2.m_pMin = PRVector3::min(p, b.m_pMin);
	b2.m_pMax = PRVector3::max(p, b.m_pMax);
	//	update the center
	b2.m_center = (b2.m_pMax + b2.m_pMin) / 2;
	return b2;
}

//	combine two bounding box together
PRBBox* PRBBox::combine(const PRBBox *b1, const PRBBox *b2)
{
	PRVector3 pMin = PRVector3::min(b1->m_pMin, b2->m_pMin);
	PRVector3 pMax = PRVector3::max(b1->m_pMax, b2->m_pMax);
	PRBBox *b = new PRBBox(pMin, pMax);
	return b;
}