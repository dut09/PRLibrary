/*
Tao Du
taodu@stanford.edu
May 14, 2014
*/

#ifndef _LIBPR_PRLUMITEXEL_H_
#define _LIBPR_PRLUMITEXEL_H_

//	lumitexel is responsible for collecting
//	brdf information from photos
//	PRLumitexel is closed related with a vertex
//	in the triangle
//	each vertex has a pointer to a PRLumitexel
//	object

#include "prStd.h"
#include "PRBrdfSample.h"

class PRLumitexel
{
public:
	PRLumitexel();
	~PRLumitexel();

	void addBrdfSample(PRBrdfSample *sample);
	int getBrdfSampleNum();
	//	get cosine theta
	double getTheta(int i);
	double getCosTheta(int i);
	//	get radiance
	PRVector3 getRadiance(int i);
	double getRadiance(int i, int channel);
	//	get median radiance
	//	PRVector3 getMedianRadiance();
	//	get albedo component in the material
	PRVector3 getAlbedo();


private:
	std::vector<PRBrdfSample *> m_brdfSamples;
};

#endif