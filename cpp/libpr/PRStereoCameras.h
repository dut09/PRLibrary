/*
Tao Du
taodu@stanford.edu
April 29, 2014
*/

#ifndef _LIBPR_PRSTEREOCAMERAS_H_
#define _LIBPR_PRSTEREOCAMERAS_H_

#include "PRCamera.h"

//	we define a .prsc file format(P_honeR_eflectometryS_tereoC_amera)
//	to represent the rotation and translation between two camereas
//	file format:
//	m_R(0, 0) m_R(0, 1) m_R(0, 2) 
//	m_R(1, 0) m_R(1, 1) m_R(1, 2) 
//	m_R(2, 0) m_R(2, 1) m_R(2, 2) 
//	m_T(0) m_T(1) m_T(2) 

class PRStereoCameras
{
public:
	//	initialize depth camera, rgb camera and stereo system
	PRStereoCameras(const std::string depthCameraParas, 
		const std::string rgbCameraParas,
		const std::string stereoParas);
	~PRStereoCameras();
	
	//	registrate the depth image to the rgb image
	PRMatrix2D<double> registrateDepth2RGB(PRMatrix2D<double> &depth);

	PRCamera* getDepthCamera() {return m_depthCamera;}
	PRCamera* getRGBCamera() {return m_rgbCamera;}
	PRMatrix2D<double> getR() const {return m_R;}
	PRVector3 getT() const {return m_T;}

	PRVector3 depth2rgb(const PRVector3& point);
	PRVector3 rgb2depth(const PRVector3& point);
	

private:
	//	depth camera
	//	by default it is the 'left' camera
	PRCamera* m_depthCamera;
	//	rgb camera
	//	by default it is the 'right' camera
	PRCamera* m_rgbCamera;
	//	the definition of m_R and m_T
	//	X_R = R * X_L + T
	//	where X_R is a 3d point in rgb camera
	//	and X_L is a 3d point in depth camera
	PRMatrix2D<double> m_R;
	PRVector3 m_T;
};

#endif