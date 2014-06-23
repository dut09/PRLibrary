#include "ConstantLight.h"

ConstantLight::ConstantLight(const PRVector3 &lightPos)
	: PRLightModel(lightPos, PRVector3())
{

}

PRVector3 ConstantLight::getLightRadiance(const PRVector3 &point)
{
	return PRVector3(1.0, 1.0, 1.0);
}