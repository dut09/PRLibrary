/*
Tao Du
taodu@stanford.edu
May 8, 2014
*/

//	transformation 4x4 matrix
#ifndef _LIBPR_PRTRANSFORM_H_
#define _LIBPR_PRTRANSFORM_H_

#include "PRMatrix2D.h"
#include "PRVector3.h"

class PRTransform
{
public:
	PRTransform();						//	identity transform
	PRTransform(const PRTransform& t);

	//	overload operator
	PRTransform& operator=(const PRTransform& t);
	//	access variables
	double& operator()(int i, int j);

	//	algebra
	PRTransform& operator+=(const PRTransform& t);
	PRTransform& operator-=(const PRTransform& t);
	PRTransform& operator*=(const PRTransform& t);
	PRTransform& operator*=(double d);
	PRTransform& operator/=(double d);

	//	transpose
	PRTransform transpose();
	
	//	inverse
	PRTransform inverse();

	//	static function for specific transformations
	static PRTransform identity();
	static PRTransform translate(double x, double y, double z);
	static PRTransform rotate(double x, double y, double z);
	static PRTransform scale(double x, double y, double z);

private:
	PRMatrix2D<double> m_mat;	//	4x4 matrix
};

PRTransform operator+(const PRTransform& left, const PRTransform& right);
PRTransform operator+(const PRTransform& left, double d);
PRTransform operator+(double d, const PRTransform& right);
PRTransform operator-(const PRTransform& left, const PRTransform& right);
PRTransform operator-(const PRTransform& left, double d);
PRTransform operator-(double d, const PRTransform& right);
PRTransform operator*(const PRTransform& left, const PRTransform& right);
PRTransform operator*(const PRTransform& left, double d);
PRTransform operator*(double d, const PRTransform& right);
PRVector3 operator*(const PRTransform& left, const PRVector3& right);
PRTransform operator/(const PRTransform& left, double d);

#endif