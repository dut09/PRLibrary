#include "PRLightModel.h"

PRLightModel::PRLightModel(const PRVector3 &lightPos, const PRVector3 &lightDir,
		const std::string rFile, const std::string gFile, const std::string bFile,
		const std::vector<double> &theta, const std::vector<double> &distance)
{
	m_lightPos = lightPos;
	m_lightDir = lightDir;
	//	normalized lightDir
	m_lightDir = m_lightDir / m_lightDir.length();

	PRMatrix2D<double> meshR(rFile);
	PRMatrix2D<double> meshG(gFile);
	PRMatrix2D<double> meshB(bFile);

	m_R.set(theta, distance, meshR);
	m_G.set(theta, distance, meshG);
	m_B.set(theta, distance, meshB);
}

PRVector3 PRLightModel::operator()(const PRVector3 &point)
{
	PRVector3 lightVector = point - m_lightPos;
	double dist = lightVector.length();
	double angle = abs(acos(PRVector3::dot(lightVector, m_lightDir) / dist));
	PRVector3 color;
	color.x = m_R(angle, dist);
	color.y = m_G(angle, dist);
	color.z = m_B(angle, dist);
	return color;
}

void PRLightModel::setLightPos(const PRVector3 lightPos)
{
	m_lightPos = lightPos;
}

void PRLightModel::setLightDir(const PRVector3 lightDir)
{
	m_lightDir = lightDir;
}

double PRLightModel::getCosine(const PRVector3 &point, const PRVector3 &normal)
{
	PRVector3 lightVector = m_lightPos - point;
	lightVector /= lightVector.length();
	PRVector3 n = normal;
	n /= n.length();
	return PRVector3::dot(n, lightVector);
}