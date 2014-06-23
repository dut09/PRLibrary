/*
Tao Du
taodu@stanford.edu
May 14, 2014
*/

#ifndef _LIBPR_PRBRDFSAMPLE_H_
#define _LIBPR_PRBRDFSAMPLE_H_

#include "PRVector3.h"
#include "PRPixel.h"

//	this class is defined to represent
//	the brdf samples specifically for our project
//	note that in our project wi and wo are the same 
//	vectors, so we only care about only single theta
class PRBrdfSample
{
public:
	//	this constructor accepts w, normal and radiance
	//	it will use w and normal to compute angles
	//	and stores the radiance
	//	by default the imgId will be reset to -1
	//	if no imgId is provided
	PRBrdfSample(const PRVector3 &w,
		const PRVector3 &normal,
		const PRVector3 &radiance);

	//	this constructor adds two new fields
	//	the image id and the pixel
	PRBrdfSample(const PRVector3 &w,
		const PRVector3 &normal,
		const PRVector3 &radiance,
		int imgId,
		const PRPixel &pixel);

	//	overload oeprator to get samples 
	//	in three different channels
	double operator()(int channel);

	//	get the angle in radiance
	double getTheta(){return m_theta;}
	double getCosTheta(){return m_cosTheta;}
	//	get the rgb value of the brdf
	PRVector3 getRadiance() {return m_radiance;}
	double getRadiance(int channel);

	//	get help information from the image
	int getImageId() {return m_imgId;}
	PRPixel getPixelPosition() {return m_pixel;}

private:
	double m_theta;
	double m_cosTheta;
	PRVector3 m_radiance;
	//	the image id and pixel position
	//	might be helpful for preprocessing
	//	or post process
	//	how to define the id is totally up to the caller
	int m_imgId;
	PRPixel m_pixel;
};

#endif
