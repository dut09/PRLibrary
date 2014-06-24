/*
Tao Du
taodu@stanford.edu
May 4, 2014
*/

//	this class represents a light model in our project
//	it includes the following data member:
//		lightPos: the light position in the current frame
//		lightDir: the light direction in the current frame
//		
#ifndef _LIBPR_PRLIGHTMODEL_H_
#define _LIBPR_PRLIGHTMODEL_H_

#include "../prStd.h"
#include "../base/PRVector3.h"

class PRLightModel
{
public:
	PRLightModel(const PRVector3 &lightPos, const PRVector3 &lightDir);

	PRVector3 getLightPos() const {return m_lightPos;}
	PRVector3 setLightDir() const {return m_lightDir;}

	double getCosine(const PRVector3 &point, const PRVector3 &normal);

	virtual PRVector3 getLightRadiance(const PRVector3 &point) { return PRVector3(); }
private:
	PRVector3 m_lightPos;
	PRVector3 m_lightDir;
};

#endif