#include "../../include/camera/PRStereoCameras.h"

PRStereoCameras::PRStereoCameras(const std::string depthCameraParas, 
		const std::string rgbCameraParas,
		const std::string stereoParas)
{
	m_depthCamera = new PRCamera(depthCameraParas);
	m_rgbCamera = new PRCamera(rgbCameraParas);
	//	read prsc files
	std::ifstream fin(stereoParas, std::ios::in | std::ios::binary);
	assert(!fin.fail());
	//	read m_R
	m_R = PRMatrix2D<double>(3, 3);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			double d;
			fin.read((char *)&d, sizeof(double));
			m_R(i, j) = d;
		}
	}
	//	read m_T
	for (int i = 0; i < 3; i++)
	{
		double d;
		fin.read((char *)&d, sizeof(double));
		m_T[i] = d;
	}
	//	close the file
	fin.close();
}
	
//	destruct the cameras
PRStereoCameras::~PRStereoCameras()
{
	delete m_depthCamera;
	delete m_rgbCamera;
}

//	registrate the depth image to the rgb image
PRMatrix2D<double> PRStereoCameras::registrateDepth2RGB(PRMatrix2D<double> &depth)
{
	//	store the point in depth camera frame
	PRVector3 point;
	int height = depth.getHeight();
	int width = depth.getWidth();
	//	store the points in rgb camera frame
	PRMatrix2D<PRVector3> points(height, width);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			PRPixel p;
			p.height = i;
			p.width = j;
			point = m_depthCamera->pixel2camera(p) * depth(i, j);
			points(i, j) = depth2rgb(point);
		}
	return m_rgbCamera->rasterize(points);
}

PRVector3 PRStereoCameras::depth2rgb(const PRVector3& point) 
{
	//	the definition of m_R and m_T
	//	X_R = R * X_L + T
	//	where X_R is a 3d point in rgb camera
	//	and X_L is a 3d point in depth camera
	//	so we should return R * point + T
	PRVector3 X_r;
	PRVector3 X_L = point;
	for (int i = 0; i < 3; i++)
	{
		X_r[i] = 0.0;
		for (int j = 0; j < 3; j++)
		{
			X_r[i] += m_R(i, j) * X_L[j];
		}
		X_r[i] += m_T[i];
	}
	return X_r;
}

PRVector3 PRStereoCameras::rgb2depth(const PRVector3& point)
{
	//	the definition of m_R and m_T
	//	X_R = R * X_L + T
	//	where X_R is a 3d point in rgb camera
	//	and X_L is a 3d point in depth camera
	//	so R' * (X_R - T) = X_L
	PRVector3 X_r = point;
	PRVector3 X_l;
	X_r -= m_T;
	for (int i = 0; i < 3; i++)
	{
		X_l[i] = 0.0;
		for (int j = 0; j < 3; j++)
		{
			X_l[i] += m_R(j, i) * X_r[j];
		}
	}
	return X_l;
}