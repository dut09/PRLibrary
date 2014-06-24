#include "../../include/light/PRLightModel.h"

PRLightModel::PRLightModel(const PRVector3 &lightPos, const PRVector3 &lightDir)
{
	m_lightPos = lightPos;
	m_lightDir = lightDir;
	//	normalized lightDir
	m_lightDir = m_lightDir / m_lightDir.length();
}

double PRLightModel::getCosine(const PRVector3 &point, const PRVector3 &normal)
{
	PRVector3 lightVector = m_lightPos - point;
	lightVector /= lightVector.length();
	PRVector3 n = normal;
	n /= n.length();
	return PRVector3::dot(n, lightVector);
}