/*
Tao Du
taodu@stanford.edu
April 10, 2014
*/

#ifndef _LIBPR_PRCAMERA_H_
#define _LIBPR_PRCAMERA_H_

#include "../base/PRMatrix2D.h"
#include "../shape/PRTriangle.h"

//	this class represents the camera in our project, it includes

//	***	all the intrinsic parameters	***
//	height, width: image height and width
//	fc:	focal length, a 2x1 vector
//	cc: center point, a 2x1 vector	by default, cc is (heigth-1)/2 and (width-1)/2
//	kc: distortion parameters, a 5x1 vector
//	alpha_c: skew coefficient, a scalar

//	***	normal vector	***
//	this is a martrix storing the normalized vector pointing to each pixel
//	normalized vector: the z-value = 1

//	for detailed infomation about the parameters, see
//	http://vision.caltech.edu/bouguetj/calib_doc/htmls/parameters.html

//	this camera is more focused on geometric information
//	it provides methods to do transformation between 2d image and 3d camera space

class PRCamera
{
public:
	//	the method to construct the camera is through a camera config file
	PRCamera(const std::string filename);
	//	assign fc and cc directly
	//	used for pinhole camera
	PRCamera(int height, int width, double fc[], double cc[]);
	//	we don't support assignment, or copy constructor
	//	there is no need to support them!

	//	overload operator
	//	this is a tricky implementation:
	//	returns the normalized vector in height and width
	//	note that the return type is value, not reference
	//	we don't allow other functions to modify the matrix
	PRVector3 operator()(int height, int width);
	
	//	transformation
	//	transform a point in the camera position into the image plane
	//	the results are rounded to the nearest pixel
	//	the coordinate in PRPixel is [height, width]
	PRPixel camera2pixel(const PRVector3 &v) const;
	
	//	normalize function
	//	this function is used to transform a pixel into the camera frame
	//	the resulting vector satisfies v.z = 1
	//	the pixel p is [height, width]
	PRVector3 pixel2camera(const PRPixel &p) const;
	
	//	write camera parameters into prc file
	void writeIntoFile(std::string filename) const;

	//	rasterization function
	//	the input matrix:
	//		a PRVector3 matrix representing the point cloud in the camera frame
	//	the output matrix:
	//		a double matrix2D representing the rasterization results:
	//		rasterization step gives a depth value for each pixel in the camera image plane
	PRMatrix2D<double> rasterize(PRMatrix2D<PRVector3>& points);

	//	get function
	//	height and width
	int getHeight() const {return m_height;}
	int getWidth() const {return m_width;}
	//	get fc
	double getFC(int i) const;
	//	get cc
	double getCC(int i) const;
	//	get kc
	double getKC(int i) const;
	//	get alpha_c
	double getAlphaC() const {return m_alpha_c;}

	//	PRCamera provides a static method to help check
	//	whether a triangle from the point cloud is a valid
	//	patch
	//	this method is not general, actually it is specifically
	//	designed for pinhole camera system used in our setting
	static bool isValidTrianglePatch(PRTriangle& triangle);

private:
	//	private function
	//	used in rasterization
	void rasterizeTriangle(PRTriangle& triangle, PRMatrix2D<double> &depthImage);


	//	data member
	//	focal length
	double m_fc[2];
	//	center point
	double m_cc[2];
	//	distortion parameters:
	double m_kc[5];
	//	skew coefficient
	double m_alpha_c;

	//	height and width
	int m_height;
	int m_width;

	//	normalized vector
	PRMatrix2D<PRVector3> m_normalizedVector;
};



#endif