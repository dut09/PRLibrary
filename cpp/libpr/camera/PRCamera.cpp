/*
Tao Du
taodu@stanford.edu
April 11, 2014
*/
#include "./PRCamera.h"
#include "../prStd.h"

PRCamera::PRCamera(const std::string filename)
{
	//	the only way to init a camera is to use a config file
	//	the file format of the camera file
	//	the file extension is .prc(P_honeR_eflectometry C_amera)
	//	Byte 0 - 3:		height
	//	Byte 4 - 7:		width
	//	Byte 8 - 23:	fc
	//	Byte 24 - 39:	cc
	//	Byte 40 - 79:	kc
	//	Byte 80 - 87:	alpha_c
	std::ifstream fin(filename, std::ios::in | std::ios::binary);
	assert(!fin.fail());
	//	read the height
	fin.read((char *)&m_height, sizeof(m_height));
	//	read the width
	fin.read((char *)&m_width, sizeof(m_width));
	//	read fc
	fin.read((char *)m_fc, sizeof(m_fc));
	//	read cc
	fin.read((char *)m_cc, sizeof(m_cc));
	//	read kc
	fin.read((char *)m_kc, sizeof(m_kc));
	//	read alpha_c
	fin.read((char *)&m_alpha_c, sizeof(m_alpha_c));
	//	close the file
	fin.close();

	//	build the normalizedVector
	PRMatrix2D<PRVector3> vectors(m_height, m_width);
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			//	compute the normalizedVector for each pixel
			//	given pixel (j, i), compute the 3d vector in camera space
			//	this code is implemented based on normalize.m in the calib toolbox
			PRPixel p(i, j);
			vectors(i, j) = pixel2camera(p);
		}
	m_normalizedVector = vectors;
}

PRCamera::PRCamera(int height, int width, double fc[], double cc[])
{
	m_height = height;
	m_width = width;
	m_fc[0] = fc[0];
	m_fc[1] = fc[1];
	m_cc[0] = cc[0];
	m_cc[1] = cc[1];
	for (int i = 0; i < 5; i++)
		m_kc[i] = 0.0;
	m_alpha_c = 0.0;
		
	//	build the normalizedVector
	PRMatrix2D<PRVector3> vectors(m_height, m_width);
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			//	compute the normalizedVector for each pixel
			//	given pixel (j, i), compute the 3d vector in camera space
			//	this code is implemented based on normalize.m in the calib toolbox
			PRPixel p(i, j);
			vectors(i, j) = pixel2camera(p);
		}
	m_normalizedVector = vectors;
}

//	overload operator
//	returns the normalized vector in height and width
//	note that the return type is value, not reference
//	we don't allow other functions to modify the matrix
PRVector3 PRCamera::operator()(int height, int width)
{
	//	check the range of height and width
	assert(height >= 0 && height < m_height);
	assert(width >= 0 && width < m_width);
	//	return the vector in the matrix
	return m_normalizedVector(height, width);
}
	
//	transformation
//	transform a point in the camera position into the image plane
//	the results are rounded to the nearest pixel
//	to test this function, use the function in the matlab toolbox
//	project_points2.m
PRPixel PRCamera::camera2pixel(const PRVector3 &v) const
{
	//	the implementation is based on the website:
	//	http://vision.caltech.edu/bouguetj/calib_doc/htmls/parameters.html

	//	in order for reader to read the code easily, we use the name in that website
	double Xc = v.x, Yc = v.y, Zc = v.z;
	double x = Xc / Zc, y = Yc / Zc;
	double xn[2] = {x, y};
	double r2 = x * x + y * y;
	double dx[2];
	//	note that the index in the website starts from 1 to 5
	//	but in our implementation it starts from 0 to 4
	dx[0] = 2 * m_kc[2] * x * y + m_kc[3] * (r2 + 2 * x * x);
	dx[1] = m_kc[2] * (r2 + 2 * y * y) + 2 * m_kc[3] * x * y;

	double xd[2];
	double xnScale = 1 + m_kc[0] * r2 + m_kc[1] * pow(r2, 2) + m_kc[4] * pow(r2, 3);
	xd[0] = xnScale * xn[0] + dx[0];
	xd[1] = xnScale * xn[1] + dx[1];

	double xp, yp;
	xp = m_fc[0] * (xd[0] + m_alpha_c * xd[1]) + m_cc[0];
	yp = m_fc[1] * xd[1] + m_cc[1];

	PRPixel p(yp, xp);
	return p;
}

//	normalize function
//	this function is used to transform a pixel into the camera frame
//	the resulting vector satisfies v.z = 1
PRVector3 PRCamera::pixel2camera(const PRPixel &p) const
{
	//	first note that we have to 'flip' p's data member:
	//	we follow the name convention in the normalize.m in the toolbox
	//	it is a C++ implementation of the matlab function normalize.m
	double x_kk[2];
	x_kk[0] = p.width;
	x_kk[1] = p.height;

	double x_distort[2];
	x_distort[0] = (x_kk[0] - m_cc[0]) / m_fc[0];
	x_distort[1] = (x_kk[1] - m_cc[1]) / m_fc[1];

	x_distort[0] -= (m_alpha_c * x_distort[1]);

	double norm = 0;
	//	compute the norm of kc
	for (int i = 0; i < 5; i++)
		norm += (m_kc[i] * m_kc[i]);

	//	the result vector
	PRVector3 v;
	v.z = 1.0;
	//	strictly zero test
	if (norm == 0.0)
	{
		v.x = x_distort[0];
		v.y = x_distort[1];
	}
	else
	{
		double k1 = m_kc[0];
		double k2 = m_kc[1];
		double k3 = m_kc[4];
		double p1 = m_kc[2];
		double p2 = m_kc[3];
		
		double xd[2] = {x_distort[0], x_distort[1]};
		double x[2] = {x_distort[0], x_distort[1]};
		for (int kk = 1; kk <= 20; kk++)
		{
			double xZeroSq = x[0] * x[0];
			double xOneSq = x[1] * x[1];
			double x01 = x[0] * x[1];
			double r_2 = xZeroSq + xOneSq;
			double k_radial = 1 + k1 * r_2 + k2 * r_2 * r_2 + k3 * pow(r_2, 3);
			double delta_x[2];
			delta_x[0] = 2 * p1 * x01 + p2 * (r_2 + 2 * xZeroSq);
			delta_x[1] = p1 * (r_2 + 2 * xOneSq) + 2 * p2 * x01;
			x[0] = (xd[0] - delta_x[0]) / k_radial;
			x[1] = (xd[1] - delta_x[1]) / k_radial;
		}
		v.x = x[0];
		v.y = x[1];
	}
	return v;
}

//	write camera parameters into prc file
void PRCamera::writeIntoFile(std::string filename) const
{
	//	the file format of prc file
	//	Byte 0 - 3:		height
	//	Byte 4 - 7:		width
	//	Byte 8 - 23:	fc
	//	Byte 24 - 39:	cc
	//	Byte 40 - 79:	kc
	//	Byte 80 - 87:	alpha_c
	std::ofstream fout(filename, std::ios::out | std::ios::binary);
	//	write the height
	fout.write((char *)&m_height, sizeof(m_height));
	//	write the width
	fout.write((char *)&m_width, sizeof(m_width));
	//	write fc
	fout.write((char *)m_fc, sizeof(m_fc));
	//	write cc
	fout.write((char *)m_cc, sizeof(m_cc));
	//	write kc
	fout.write((char *)m_kc, sizeof(m_kc));
	//	write alpha_c
	fout.write((char *)&m_alpha_c, sizeof(m_alpha_c));
	//	close the file
	fout.close();
}

//	rasterization function
//	the input matrix:
//		a PRVector3 matrix representing the point cloud in the camera frame
//	the output matrix:
//		a double matrix2D representing the rasterization results:
//		rasterization step gives a depth value for each pixel in the camera image plane
PRMatrix2D<double> PRCamera::rasterize(PRMatrix2D<PRVector3>& points)
{
	PRMatrix2D<double> depthImage(m_height, m_width);
	//	initialize the depthImage
	depthImage.reset(DBL_MAX);
	int height = points.getHeight();
	int width = points.getWidth();
	//	scan all the triangles
	for (int i = 0; i < height - 1; i++)
		for (int j = 0; j < width - 1; j++)
		{
			//	(i, j), (i + 1, j), (i + 1, j + 1)
			PRTriangle triangle1(points(i, j), points(i + 1, j), points(i + 1, j + 1));
			//	test whether it is a valid patch
			if (isValidTrianglePatch(triangle1))
				rasterizeTriangle(triangle1, depthImage);
			//	(i, j), (i + 1, j + 1), (i, j + 1)
			PRTriangle triangle2(points(i, j), points(i + 1, j + 1), points(i, j + 1));
			if (isValidTrianglePatch(triangle2))
				rasterizeTriangle(triangle2, depthImage);
		}
	//	reset DBL_MAX to zero
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (depthImage(i, j) == DBL_MAX)
				depthImage(i, j) = 0.0;
	//	sadly here we have deep copy
	return depthImage;
}

//	get fc
double PRCamera::getFC(int i) const
{
	//	i could only be 0 or 1
	assert(i >= 0 && i < 2);
	return m_fc[i];
}

//	get cc
double PRCamera::getCC(int i) const
{
	//	i could only be 0 or 1
	assert(i >= 0 && i < 2);
	return m_cc[i];
}

//	get kc
double PRCamera::getKC(int i) const
{
	//	i could only be in 0 to 5
	assert(i >= 0 && i < 5);
	return m_kc[i];
}

void PRCamera::rasterizeTriangle(PRTriangle& triangle, PRMatrix2D<double> &depthImage)
{
	//	rasterize a single triangle and update the depth image when necessary
	//	project the three vertices
	PRPixel p[3];
	for (int i = 0; i < 3; i++)
	{
		p[i] = camera2pixel(triangle[i]);
	}
	//	find the bounding box in the image plane
	double hMin = p[0].height, hMax = hMin;
	double wMin = p[0].width, wMax = wMin;
	for (int i = 0; i < 3; i++)
	{
		hMin = std::min(hMin, p[i].height);
		hMax = std::max(hMax, p[i].height);
		wMin = std::min(wMin, p[i].width);
		wMax = std::max(wMax, p[i].width);
	}
	//	crop it into the imgae plane
	int ihMin = (int)std::max(0.0, hMin);
	int iwMin = (int)std::max(0.0, wMin);
	int ihMax = (int)std::min(m_height - 1.0, hMax);
	int iwMax = (int)std::min(m_width - 1.0, wMax);

	//	scan all the pixels in the bounding box
	for (int i = ihMin; i <= ihMax; i++)
		for (int j = iwMin; j <= iwMax; j++)
		{
			//	ray tracing from this pixel
			PRVector3 d = pixel2camera(PRPixel(i, j));
			//	build a line
			PRLine l(PRVector3(0.0, 0.0, 0.0), d);
			//	compute the intersection point
			PRIntersection inter = triangle.getIntersection(l);
			if (inter.t == DBL_MAX)
				continue;
			double depth = inter.point.z;
			if (depth < depthImage(i, j))
				depthImage(i, j) = depth;
		}
}

//	this function is used to test whether
//	a triangle patch is valid during the rasterization
//	a triangle is invalid if it is too close to the screen
//	or its bound box is too large
bool PRCamera::isValidTrianglePatch(PRTriangle& triangle)
{
	//	test whether the triangle depth is too close
	if (triangle[0].z <  PRCAMERA_MIN_DEPTH || triangle[0].z > PRCAMERA_MAX_DEPTH
		|| triangle[1].z < PRCAMERA_MIN_DEPTH || triangle[1].z > PRCAMERA_MAX_DEPTH
		|| triangle[2].z < PRCAMERA_MIN_DEPTH || triangle[2].z > PRCAMERA_MAX_DEPTH)
		return false;
	//	compute the bounding box of triangle to test whether it is valid
	PRVector3 bmin, bmax;
	//	compute min and max in xyz coords
	for (int i = 0; i < 3; i++)
	{
		bmin[i] = std::min(triangle[0][i], std::min(triangle[1][i], triangle[2][i]));
		bmax[i] = std::max(triangle[0][i], std::max(triangle[1][i], triangle[2][i]));
	}
	//	compute the average z
	double aveDepth = (triangle[0].z + triangle[1].z + triangle[2].z) / 3.0;
	double bboxMaxSize = aveDepth * PRCAMERA_BBOX_RATIO;
	//	test the bounding box
	PRVector3 bbox = bmax - bmin;
	if (bbox.x > bboxMaxSize
		|| bbox.y > bboxMaxSize
		|| bbox.z > bboxMaxSize)
		return false;
	return true;
}