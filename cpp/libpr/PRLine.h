/*
Tao Du
taodu@stanford.edu
April 16, 2014
*/

#ifndef _LIBPR_PRLINE_H_
#define _LIBPR_PRLINE_H_

#include "PRTransform.h"

class PRLine
{
public:
	PRLine(const PRVector3 &origin, const PRVector3 &direction);
	PRVector3 getDirection() const {return m_direction;}
	PRVector3 getOrigin() const {return m_origin;}

	//	overload operator
	PRVector3 operator()(double t) const;

	//	transformation
	PRLine transform(const PRTransform& t) const;

private:
	//	data member
	//	origin
	PRVector3 m_origin;
	//	it is not required to normalized the direction
	PRVector3 m_direction;
};

#endif