/*
Tao Du
taodu@stanford.edu
May 5, 2014
*/

#ifndef _LIBPR_PRBILINEARINTERPOLATER_H_
#define _LIBPR_PRBILINEARINTERPOLATER_H_

#include "prStd.h"
#include "PRMatrix2D.h"

class PRBilinearInterpolater
{
public:
	PRBilinearInterpolater();
	void set(const std::vector<double> &x,
		const std::vector<double> &y,
		const PRMatrix2D<double> &z);

	double operator()(double x, double y);

private:
	int binarySearchInX(double x) const;
	int binarySearchInY(double y) const;

	std::vector<double> m_x;
	std::vector<double> m_y;
	PRMatrix2D<double> m_z;
};

#endif