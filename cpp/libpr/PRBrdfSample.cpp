/*
Tao Du
taodu@stanford.edu
May 14, 2014
*/

#include "prStd.h"
#include "PRBrdfSample.h"

PRBrdfSample::PRBrdfSample(const PRVector3 &w, const PRVector3 &normal, const PRVector3 &radiance)
{
	double lengthw = w.length();
	assert(lengthw > DBL_EPSILON);
	double lengthn = normal.length();
	assert(lengthn > DBL_EPSILON);
	m_cosTheta = PRVector3::dot(w, normal) / (lengthw * lengthn);
	assert(m_cosTheta >= 0.0);
	m_theta = acos(m_cosTheta);
	m_radiance = radiance;
	//	set imgId to be -1
	m_imgId = -1;
}

PRBrdfSample::PRBrdfSample(const PRVector3 &w,
		const PRVector3 &normal,
		const PRVector3 &radiance,
		int imgId,
		const PRPixel &pixel)
{
	double lengthw = w.length();
	assert(lengthw > DBL_EPSILON);
	double lengthn = normal.length();
	assert(lengthn > DBL_EPSILON);
	m_cosTheta = PRVector3::dot(w, normal) / (lengthw * lengthn);
	assert(m_cosTheta >= 0.0);
	m_theta = acos(m_cosTheta);
	m_radiance = radiance;
	m_imgId = imgId;
	m_pixel = pixel;
}

double PRBrdfSample::operator()(int channel)
{
	assert(channel >= 0 && channel < 3);
	return m_radiance[channel];
}

double PRBrdfSample::getRadiance(int channel)
{
	assert(channel >= 0 && channel < 3);
	return m_radiance[channel];
}