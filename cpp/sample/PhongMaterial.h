/*
Tao Du
taodu@stanford.edu
Jun 22, 2014
*/

#ifndef _SAMPLE_PHONG_MATERIAL_
#define _SAMPLE_PHONG_MATERIAL_

#include "pr.h"

class PhongMaterial : public PRMaterial
{
public:
	PhongMaterial();
	PhongMaterial(const PRVector3 &ambient,
		const PRVector3 &diffuse,
		const PRVector3 &specular,
		double alpha);

	void ReadFromFile(const std::string fileName);
	void WriteIntoFile(const std::string fileName);
	PRVector3 shade(const PRVector3 &lightColor,
		const PRVector3 &lightIn,
		const PRVector3 &lightOut,
		const PRVector3 &point,
		const PRVector3 &normal);
private:
	PRVector3 m_ambient;
	PRVector3 m_diffuse;
	PRVector3 m_specular;
	double m_alpha;
};

#endif