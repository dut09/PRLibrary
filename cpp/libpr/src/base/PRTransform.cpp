/*
Tao Du
taodu@stanford.edu
May 8, 2014
*/


#include "../../include/base/PRTransform.h"

PRTransform::PRTransform()
{
	m_mat = PRMatrix2D<double>(4, 4);
	//	init as identity matrix
	m_mat.reset(0.0);
	for (int i = 0; i < 4; i++)
		m_mat(i, i) = 1.0;
}

PRTransform::PRTransform(const PRTransform& t)
{
	m_mat = t.m_mat;
}

//	overload operator
PRTransform& PRTransform::operator=(const PRTransform& t)
{
	m_mat = t.m_mat;
	return *this;
}

//	access variables
double& PRTransform::operator()(int i, int j)
{
	assert(i >= 0 && i < 4);
	assert(j >= 0 && j < 4);
	return m_mat(i, j);
}

//	algebra
PRTransform& PRTransform::operator+=(const PRTransform& t)
{
	PRTransform t2 = t;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_mat(i, j) += t2(i, j);
	return *this;
}

PRTransform& PRTransform::operator-=(const PRTransform& t)
{
	PRTransform t2 = t;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_mat(i, j) -= t2(i, j);
	return *this;
}

PRTransform& PRTransform::operator*=(const PRTransform& t)
{
	PRTransform result;
	PRTransform t2 = t;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			result(i, j) = 0.0;
			for (int k = 0; k < 4; k++)
				result(i, j) += (m_mat(i, k) * t2(k, j));
		}
	*this = result;
	return *this;
}

PRTransform& PRTransform::operator*=(double d)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_mat(i, j) *= d;
	return *this;
}

PRTransform& PRTransform::operator/=(double d)
{
	assert(abs(d) > DBL_EPSILON);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_mat(i, j) /= d;
	return *this;
}

//	transpose
PRTransform PRTransform::transpose()
{
	PRTransform t;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			t.m_mat(i, j) = m_mat(j, i);
	return t;
}
	
//	inverse
PRTransform PRTransform::inverse()
{
	//
	// Inversion by Cramer's rule.  Code taken from an Intel publication
	// (adapted by Matt Fisher)
	//
	double Result[4][4];
	double tmp[12]; /* temp array for pairs */
	double src[16]; /* array of transpose source matrix */
	double det; /* determinant */
	/* transpose matrix */
	for (int i = 0; i < 4; i++)
	{
		src[i + 0] = (*this)(i, 0);
		src[i + 4] = (*this)(i, 1);
		src[i + 8] = (*this)(i, 2);
		src[i + 12] = (*this)(i, 3);
	}
	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	/* calculate first 8 elements (cofactors) */
	Result[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	Result[0][0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	Result[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	Result[0][1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	Result[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	Result[0][2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	Result[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	Result[0][3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	Result[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	Result[1][0] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	Result[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	Result[1][1] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	Result[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	Result[1][2] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	Result[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	Result[1][3] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = src[2]*src[7];
	tmp[1] = src[3]*src[6];
	tmp[2] = src[1]*src[7];
	tmp[3] = src[3]*src[5];
	tmp[4] = src[1]*src[6];
	tmp[5] = src[2]*src[5];

	tmp[6] = src[0]*src[7];
	tmp[7] = src[3]*src[4];
	tmp[8] = src[0]*src[6];
	tmp[9] = src[2]*src[4];
	tmp[10] = src[0]*src[5];
	tmp[11] = src[1]*src[4];
	/* calculate second 8 elements (cofactors) */
	Result[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	Result[2][0] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	Result[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	Result[2][1] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	Result[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	Result[2][2] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	Result[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	Result[2][3] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	Result[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	Result[3][0] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	Result[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	Result[3][1] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	Result[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	Result[3][2] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	Result[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	Result[3][3] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
	/* calculate determinant */
	det =
		src[0] * Result[0][0] + src[1] * Result[0][1]
		+ src[2] * Result[0][2] + src[3] * Result[0][3];
	/* calculate matrix inverse */
	det = 1.0f / det;

	PRTransform r;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			r(i, j) = Result[i][j] * det;
		}
	}
	return r;
}

//	static function for specific transformations
PRTransform PRTransform::identity()
{
	return PRTransform();
}

PRTransform PRTransform::translate(double x, double y, double z)
{
	PRTransform t;
	t(0, 3) = x;
	t(1, 3) = y;
	t(2, 3) = z;
	return t;
}

PRTransform PRTransform::rotate(double x, double y, double z)
{
	PRTransform rotx, roty, rotz;
	double cosrx, sinrx, cosry, sinry, cosrz, sinrz;
	cosrx = cos(x); sinrx = sin(x);
	cosry = cos(y); sinry = sin(y);
	cosrz = cos(z); sinrz = sin(z);

	rotx(1, 1) = cosrx; rotx(1, 2) = -sinrx;
	rotx(2, 1) = sinrx; rotx(2, 2) = cosrx;

	roty(0, 0) = cosry; roty(2, 0) = -sinry;
	roty(0, 2) = sinry; roty(2, 2) = cosry;

	rotz(0, 0) = cosrz; rotz(0, 1) = -sinrz;
	rotz(1, 0) = sinrz; rotz(1, 1) = cosrz;
	return rotx * roty * rotz;
}

PRTransform PRTransform::scale(double x, double y, double z)
{
	PRTransform t;
	t(0, 0) = x;
	t(1, 1) = y;
	t(2, 2) = z;
	return t;
}

PRTransform operator+(const PRTransform& left, const PRTransform& right)
{
	PRTransform l = left;
	l += right;
	return l;
}

PRTransform operator+(const PRTransform& left, double d)
{
	PRTransform l = left;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			l(i, j) += d;
	return l;
}

PRTransform operator+(double d, const PRTransform& right)
{
	return right + d;
}

PRTransform operator-(const PRTransform& left, const PRTransform& right)
{
	PRTransform l = left;
	l -= right;
	return l;
}

PRTransform operator-(const PRTransform& left, double d)
{
	return left + (-d);
}

PRTransform operator-(double d, const PRTransform& right)
{
	PRTransform r = right;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			r(i, j) = d - r(i, j);
	return r;
}

PRTransform operator*(const PRTransform& left, const PRTransform& right)
{
	PRTransform l = left;
	l *= right;
	return l;
}

PRTransform operator*(const PRTransform& left, double d)
{
	PRTransform l = left;
	l *= d;
	return l;
}

PRTransform operator*(double d, const PRTransform& right)
{
	return right * d;
}

PRVector3 operator*(const PRTransform& left, const PRVector3& right)
{
	PRVector3 result;
	PRVector3 r = right;
	PRTransform l = left;
	for (int i = 0; i < 3; i++)
	{
		result[i] = 0.0;
		for (int j = 0; j < 3; j++)
			result[i] += (l(i, j) * r[j]);
		result[i] += l(i, 3);
	}
	return result;
}

PRTransform operator/(const PRTransform& left, double d)
{
	assert(abs(d) >= DBL_EPSILON);
	return left * (1 / d);
}
