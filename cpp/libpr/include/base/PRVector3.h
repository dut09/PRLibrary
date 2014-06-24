/*
Tao Du
taodu@stanford.edu
April 8, 2014
*/

#ifndef _LIBPR_PRVECTOR3_H_
#define _LIBPR_PRVECTOR3_H_

//	we do not differentiate between vector3, point3 and color3

class PRVector3
{
public:
	//	initialization
	PRVector3();
	PRVector3(double x, double y, double z);
	PRVector3(double s);
	PRVector3(const PRVector3& v);

	//	overload operator
	//	assignment
	PRVector3& operator=(const PRVector3& v);
	//	[] subscription
	double& operator[](int i);

	//	- prefix
	PRVector3 operator-() const;

	//	binary operator that treat left/right operand differently
	PRVector3& operator+=(double d);
	PRVector3& operator+=(const PRVector3& v);
	PRVector3& operator-=(double d);
	PRVector3& operator-=(const PRVector3& v);
	PRVector3& operator*=(double d);
	//	component wise product
	PRVector3& operator*=(const PRVector3& v);
	PRVector3& operator/=(double d);
	//	component wise division
	PRVector3& operator/=(const PRVector3& v);

	//	length
	double length() const;
	//	lengthSq
	double lengthSq() const;

	//	static method:
	//	cross product
	static PRVector3 cross(const PRVector3 &left, const PRVector3 &right);
	//	dot product
	static double dot(const PRVector3 &left, const PRVector3 &right);

	//	component-wise min and max
	static PRVector3 min(const PRVector3 &left, const PRVector3 &right);
	static PRVector3 max(const PRVector3 &left, const PRVector3 &right);

	//	infinite PRVector3
	static PRVector3 inf(); 
	static bool isInf(const PRVector3 &vec);

	//	data member in PRVector3c
	//	it is acceptable to make them public
	double x, y, z;
};

//	binary operator that won't change left/right operand
//	plus
PRVector3 operator+(const PRVector3& left, const PRVector3& right);
PRVector3 operator+(const PRVector3& left, double right);
PRVector3 operator+(double left, const PRVector3& right);
//	minus
PRVector3 operator-(const PRVector3& left, const PRVector3& right);
PRVector3 operator-(const PRVector3& left, double right);
PRVector3 operator-(double left, const PRVector3& right);
//	product
//	component wise product
PRVector3 operator*(const PRVector3& left, const PRVector3& right);
PRVector3 operator*(const PRVector3& left, double right);
PRVector3 operator*(double left, const PRVector3& right);
//	division
//	component wise division
PRVector3 operator/(const PRVector3& left, const PRVector3& right);
PRVector3 operator/(const PRVector3& left, double right);
PRVector3 operator/(double left, const PRVector3& right);
//	equal
bool operator==(const PRVector3& left, const PRVector3& right);

#endif