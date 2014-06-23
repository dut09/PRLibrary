#include "PhongMaterial.h"

PhongMaterial::PhongMaterial()
{
	m_ambient = PRVector3();
	m_diffuse = PRVector3();
	m_specular = PRVector3();
	m_alpha = 0.0;
}

PhongMaterial::PhongMaterial(const PRVector3 &ambient,
	const PRVector3 &diffuse,
	const PRVector3 &specular,
	double alpha)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_alpha = alpha;
}

void PhongMaterial::ReadFromFile(const std::string fileName)
{
	//	the definition of the phong file
	std::ifstream fin(fileName, std::ios::in);
	double x, y, z;
	//	ambient: x, y, z
	fin >> x >> y >> z;
	m_ambient = PRVector3(x, y, z);
	//	diffuse
	fin >> x >> y >> z;
	m_diffuse = PRVector3(x, y, z);
	//	specular
	fin >> x >> y >> z;
	m_specular = PRVector3(x, y, z);
	//	alpha
	fin >> m_alpha;
	fin.close();
}

void PhongMaterial::WriteIntoFile(const std::string fileName)
{
	std::ofstream fout(fileName, std::ios::out);
	fout << m_ambient.x << "\t" << m_ambient.y << "\t" << m_ambient.z << std::endl;
	fout << m_diffuse.x << "\t" << m_diffuse.y << "\t" << m_diffuse.z << std::endl;
	fout << m_specular.x << "\t" << m_specular.y << "\t" << m_specular.z << std::endl;
	fout << m_alpha << std::endl;
	fout.close();
}

PRVector3 PhongMaterial::shade(const PRVector3 &lightColor,
	const PRVector3 &lightIn,
	const PRVector3 &lightOut,
	const PRVector3 &point,
	const PRVector3 &normal)
{
	PRVector3 color;
	PRVector3 lin = lightIn;
	lin /= lin.length();
	PRVector3 lout = lightOut;
	lout /= lout.length();
	PRVector3 n = normal;
	n /= n.length();
	//	ambient
	color += lightColor * m_ambient;
	//	diffuse
	double cosine = PRVector3::dot(lin, n);
	color += m_diffuse * (lightColor * cosine);
	//	specular
	//	perfect specular angle
	PRVector3 perfOut = n * 2 * cosine - lin;
	//	viewer: lout
	double cosine2 = PRVector3::dot(perfOut, lout);
	cosine2 = cosine2 > 0 ? cosine2 : 0.0;
	color += m_specular * lightColor
		* PRVector3(1.0, 1.0, 1.0) * pow(cosine2, m_alpha);
	return color;
}