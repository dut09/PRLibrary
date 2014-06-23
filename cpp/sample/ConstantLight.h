/*
Tao Du
taodu@stanford.edu
Jun 22, 2014
*/

#ifndef _SAMPLE_CONSTANT_LIGHT_
#define _SAMPLE_CONSTANT_LIGHT_

#include "pr.h"

class ConstantLight : public PRLightModel
{
public:
	ConstantLight(const PRVector3 &lightPos);
	PRVector3 getLightRadiance(const PRVector3 &point);
private:
};

#endif