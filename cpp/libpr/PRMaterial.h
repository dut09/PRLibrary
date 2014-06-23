/*
Tao Du
taodu@stanford.edu
May 8, 2014
*/

//	the interface of materials, define a function interface
//	to represent the color from given light vector, given
//	input light, given output light, given point and given normal

#ifndef _LIBPR_PRMATERIAL_H_
#define _LIBPR_PRMATERIAL_H_

#include "PRVector3.h"

class PRMaterial
{
public:
	virtual void ReadFromFile(const std::string fileName) = 0;
	virtual void WriteIntoFile(const std::string fileName) = 0;
	virtual PRVector3 shade(const PRVector3 &lightColor, 
		const PRVector3 &lightIn,
		const PRVector3 &lightOut,
		const PRVector3 &point,
		const PRVector3 &normal) = 0;
};

#endif