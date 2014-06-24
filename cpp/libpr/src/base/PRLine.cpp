/*
Tao Du
taodu@stanford.edu
April 16, 2014
*/

#include "../../include/base/PRLine.h"

PRLine::PRLine(const PRVector3 &origin, const PRVector3 &direction)
	: m_origin(origin), m_direction(direction)
{

}

//	overload operator
PRVector3 PRLine::operator()(double t) const
{
	return m_origin + m_direction * t;
}

//	transform the line

PRLine PRLine::transform(const PRTransform& t) const
{
	PRVector3 o2, d2;
	o2 = t * m_origin;
	d2 = t * (m_origin + m_direction);
	return PRLine(o2, d2 - o2);
}