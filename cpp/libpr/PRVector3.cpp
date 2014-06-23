/*
Tao Du
taodu@stanford.edu
April 8, 2014
*/
#include "prStd.h"
#include "PRVector3.h"

PRVector3::PRVector3()
{
	x = y = z = 0.0;
}

PRVector3::PRVector3(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

PRVector3::PRVector3(double s)
{
	x = y = z = s;
}

PRVector3::PRVector3(const PRVector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

//	assignment
PRVector3& PRVector3::operator=(const PRVector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return (*this);
}

//	[] subscription
double& PRVector3::operator[](int i)
{
	//	the index should be in the range [0, 3)
	assert(i >= 0 && i < 3);
	switch (i)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:	//	i == 2
		return z;
	}
}

//	- prefix
PRVector3 PRVector3::operator-() const
{
	PRVector3 v;
	v.x = -x;
	v.y = -y;
	v.z = -z;
	return v;
}


PRVector3& PRVector3::operator+=(double d)
{
	x += d;
	y += d;
	z += d;
	return (*this);
}

PRVector3& PRVector3::operator+=(const PRVector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return (*this);
}

PRVector3& PRVector3::operator-=(double d)
{
	x -= d;
	y -= d;
	z -= d;
	return (*this);
}

PRVector3& PRVector3::operator-=(const PRVector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return (*this);
}

PRVector3& PRVector3::operator*=(double d)
{
	x *= d;
	y *= d;
	z *= d;
	return (*this);
}

//	component wise product
PRVector3& PRVector3::operator*=(const PRVector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return (*this);
}

PRVector3& PRVector3::operator/=(double d)
{
	//	report assertion failed if d is too small
	assert(abs(d) > DBL_EPSILON);
	double invd = 1.0 / d;
	x *= invd;
	y *= invd;
	z *= invd;
	return (*this);
}

//	component wise division
PRVector3& PRVector3::operator/=(const PRVector3& v)
{
	//	report assertion failed if v is too small
	assert(abs(v.x) > DBL_EPSILON && abs(v.y) > DBL_EPSILON && abs(v.z) > DBL_EPSILON);
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return (*this);
}

//	length
double PRVector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

//	lengthSq
double PRVector3::lengthSq() const
{
	return x * x + y * y + z * z;
}

//	cross product
PRVector3 PRVector3::cross(const PRVector3 &left, const PRVector3 &right)
{
	PRVector3 v;
	v.x = left.y * right.z - left.z * right.y;
	v.y = left.z * right.x - left.x * right.z;
	v.z = left.x * right.y - left.y * right.x;
	return v;
}

//	dot product
double PRVector3::dot(const PRVector3 &left, const PRVector3 &right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

//	component-wise min and max
PRVector3 PRVector3::min(const PRVector3 &left, const PRVector3 &right)
{
	PRVector3 p;
	p.x = left.x > right.x ? right.x : left.x;
	p.y = left.y > right.y ? right.y : left.y;
	p.z = left.z > right.z ? right.z : left.z;
	return p;
}

PRVector3 PRVector3::max(const PRVector3 &left, const PRVector3 &right)
{
	PRVector3 p;
	p.x = left.x < right.x ? right.x : left.x;
	p.y = left.y < right.y ? right.y : left.y;
	p.z = left.z < right.z ? right.z : left.z;
	return p;
}

PRVector3 PRVector3::inf()
{
	return PRVector3(DBL_MAX, DBL_MAX, DBL_MAX);
}

bool PRVector3::isInf(const PRVector3 &vec)
{
	return vec == inf();
}

//	binary operators that won't change left/right operand
PRVector3 operator+(const PRVector3& left, const PRVector3& right)
{
	PRVector3 v;
	v.x = left.x + right.x;
	v.y = left.y + right.y;
	v.z = left.z + right.z;
	return v;
}

PRVector3 operator+(const PRVector3& left, double right)
{
	PRVector3 v;
	v.x = left.x + right;
	v.y = left.y + right;
	v.z = left.z + right;
	return v;
}

PRVector3 operator+(double left, const PRVector3& right)
{
	PRVector3 v;
	v.x = left + right.x;
	v.y = left + right.y;
	v.z = left + right.z;
	return v;
}

//	minus
PRVector3 operator-(const PRVector3& left, const PRVector3& right)
{
	PRVector3 v;
	v.x = left.x - right.x;
	v.y = left.y - right.y;
	v.z = left.z - right.z;
	return v;
}

PRVector3 operator-(const PRVector3& left, double right)
{
	PRVector3 v;
	v.x = left.x - right;
	v.y = left.y - right;
	v.z = left.z - right;
	return v;
}

PRVector3 operator-(double left, const PRVector3& right)
{
	PRVector3 v;
	v.x = left - right.x;
	v.y = left - right.y;
	v.z = left - right.z;
	return v;
}

//	product
//	component wise product
PRVector3 operator*(const PRVector3& left, const PRVector3& right)
{
	PRVector3 v;
	v.x = left.x * right.x;
	v.y = left.y * right.y;
	v.z = left.z * right.z;
	return v;
}

PRVector3 operator*(const PRVector3& left, double right)
{
	PRVector3 v;
	v.x = left.x * right;
	v.y = left.y * right;
	v.z = left.z * right;
	return v;
}

PRVector3 operator*(double left, const PRVector3& right)
{
	PRVector3 v;
	v.x = left * right.x;
	v.y = left * right.y;
	v.z = left * right.z;
	return v;
}

//	division
//	component wise division
PRVector3 operator/(const PRVector3& left, const PRVector3& right)
{
	assert(abs(right.x) > DBL_EPSILON && abs(right.y) > DBL_EPSILON && abs(right.z) > DBL_EPSILON);
	PRVector3 v;
	v.x = left.x / right.x;
	v.y = left.y / right.y;
	v.z = left.z / right.z;
	return v;
}

PRVector3 operator/(const PRVector3& left, double right)
{
	assert(abs(right) > DBL_EPSILON);
	double invd = 1.0 / right;
	PRVector3 v;
	v.x = left.x * invd;
	v.y = left.y * invd;
	v.z = left.z * invd;
	return v;
}

PRVector3 operator/(double left, const PRVector3& right)
{
	assert(abs(right.x) > DBL_EPSILON && abs(right.y) > DBL_EPSILON && abs(right.z) > DBL_EPSILON);
	PRVector3 v;
	v.x = left / right.x;
	v.y = left / right.y;
	v.z = left / right.z;
	return v;
}

//	equal
bool operator==(const PRVector3& left, const PRVector3& right)
{
	return left.x == right.x
		&& left.y == right.y
		&& left.z == right.z;
}