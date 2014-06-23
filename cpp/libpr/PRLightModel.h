/*
Tao Du
taodu@stanford.edu
May 4, 2014
*/

//	this class represents a light model in our project
//	it includes the following data member:
//		lightPos: the light position in the current frame
//		lightDir: the light direction in the current frame
//		(in our experiment, lightPos = (7.0, 0.0, 0.0) and lightDir = (0.0, 0.0, 1.0))
//		m_R, m_G, m_B: bilinear interpolator
//		
#ifndef _LIBPR_PRLIGHTMODEL_H_
#define _LIBPR_PRLIGHTMODEL_H_

#include "prStd.h"
#include "PRBilinearInterpolater.h"
#include "PRVector3.h"

class PRLightModel
{
public:
	PRLightModel(const PRVector3 &lightPos, const PRVector3 &lightDir,
		const std::string rFile, const std::string gFile, const std::string bFile,
		const std::vector<double> &theta, const std::vector<double> &distance);
	PRVector3 operator()(const PRVector3 &point);
	void setLightPos(const PRVector3 lightPos);
	void setLightDir(const PRVector3 lightDir);

	PRVector3 getLightPos() const {return m_lightPos;}
	PRVector3 setLightDir() const {return m_lightDir;}

	double getCosine(const PRVector3 &point, const PRVector3 &normal);
private:
	PRVector3 m_lightPos;
	PRVector3 m_lightDir;
	PRBilinearInterpolater m_R, m_G, m_B;
};

#endif