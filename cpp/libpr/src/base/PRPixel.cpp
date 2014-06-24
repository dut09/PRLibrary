/*
Tao Du
taodu@stanford.edu
April 10, 2014
*/

#include "../../include/prStd.h"
#include "../../include/base/PRPixel.h"

//	constructor
PRPixel::PRPixel()
{
	//	init with zero
	height = width = 0;
}

PRPixel::PRPixel(double h, double w)
{
	this->height = h;
	this->width = w;
}

//	copy constructor
PRPixel::PRPixel(const PRPixel& p)
{
	height = p.height;
	width = p.width;
}

//	overload operator
//	assignment
PRPixel& PRPixel::operator=(const PRPixel &p)
{
	this->height = p.height;
	this->width = p.width;
	return (*this);
}

//	comparison
bool PRPixel::operator==(const PRPixel& p) const
{
	return (height == p.height) && (width == p.width);
}

//	comparison
bool PRPixel::operator!=(const PRPixel& p) const
{
	return (height != p.height) || (width != p.width);
}

//	[] subscription
double& PRPixel::operator[](int i)
{
	//	the index should be in [0, 2)
	assert(i >= 0 && i < 2);
	if (i == 0)
		return height;
	else
		return width;	//	i == 1
}

//	neighborhood
PRPixel PRPixel::getNeighbor(PR_PIXEL_DIR dir, int step) const
{
	//	we don't accept negative step
	assert(step >= 0);
	//	set the neighbor to be the pixel itself
	PRPixel neighbor(*this);
	switch (dir)
	{
	case PR_UP:
		neighbor.height -= step;
		break;
	case PR_UP_RIGHT:
		neighbor.height -= step;
		neighbor.width += step;
		break;
	case PR_RIGHT:
		neighbor.width += step;
		break;
	case PR_DOWN_RIGHT:
		neighbor.height += step;
		neighbor.width += step;
		break;
	case PR_DOWN:
		neighbor.height += step;
		break;
	case PR_DOWN_LEFT:
		neighbor.height += step;
		neighbor.width -= step;
		break;
	case PR_LEFT:
		neighbor.width -= step;
		break;
	default:	//	PR_UP_LEFT
		neighbor.height -= step;
		neighbor.width -= step;
		break;
	}
	return neighbor;
}