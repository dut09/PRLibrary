/*
Tao Du
taodu@stanford.edu
May 5, 2014
*/

#include "PRBilinearInterpolater.h"

PRBilinearInterpolater::PRBilinearInterpolater()
{
	m_x.clear();
	m_y.clear();
}

void PRBilinearInterpolater::set(const std::vector<double> &x,
		const std::vector<double> &y,
		const PRMatrix2D<double> &z)
{
	//	TODO
	//	add sorting algorithm here!
	//	now we assume x and y has to be sorted!
	int lengthX = (int)x.size();
	int lengthY = (int)y.size();
	for (int i = 0; i < lengthX; i++)
		m_x.push_back(x.at(i));
	for (int i = 0; i < lengthY; i++)
		m_y.push_back(y.at(i));
	assert(lengthX == z.getHeight());
	assert(lengthY == z.getWidth());
	m_z = z;
}

double PRBilinearInterpolater::operator()(double x, double y)
{
	//	TODO: it's very hacky
	//	out of bound cases
	x = (x <= m_x.front()) ? m_x.front() : x;
	x = (x >= m_x.back()) ? m_x.back() - 0.001 : x;
	y = (y <= m_y.front()) ? m_y.front() : y;
	y = (y >= m_y.back()) ? m_y.back() - 0.001 : y;
	
	double x0, x1, y0, y1;
	//	search 
	int xi = binarySearchInX(x);
	x0 = m_x.at(xi);
	x1 = m_x.at(xi + 1);

	int yi = binarySearchInY(y);
	y0 = m_y.at(yi);
	y1 = m_y.at(yi + 1);

	//	now interpolate between x and y to get z
	double tx = (x - x0) / (x1 - x0);
	double ty = (y - y0) / (y1 - y0);
	double z00 = m_z(xi, yi);
	double z01 = m_z(xi, yi + 1);
	double z10 = m_z(xi + 1, yi);
	double z11 = m_z(xi + 1, yi + 1);
	double z = (1 - tx) * (1 - ty) * z00 
		+ (1 - tx) * ty * z01 
		+ tx * (1 - ty) * z10 
		+ tx * ty * z11;
	return z;
}

int PRBilinearInterpolater::binarySearchInX(double x) const
{
	//	binary search in x
	if (x <= m_x.front())
		return 0;
	if (x >= m_x.back())
		return (int)m_x.size() - 1;
	int p = 0, q = (int)m_x.size() - 1;
	//	find index r such that
	//	m_x[r] < x && m_x[r + 1] >= x
	while (p < q)
	{
		int r = (p + q) / 2 + 1;
		if (m_x.at(r) < x)
			p = r;
		else
			q = r - 1;
	}
	assert(p == q);
	return p;
}

int PRBilinearInterpolater::binarySearchInY(double y) const
{
	//	binary search in y
	if (y <= m_y.front())
		return 0;
	if (y >= m_y.back())
		return (int)m_y.size() - 1;
	int p = 0, q = (int)m_y.size() - 1;
	//	find index r such that
	//	m_y[r] < y && m_y[r + 1] >= y
	while (p < q)
	{
		int r = (p + q) / 2 + 1;
		if (m_y.at(r) < y)
			p = r;
		else
			q = r - 1;
	}
	assert(p == q);
	return p;
}