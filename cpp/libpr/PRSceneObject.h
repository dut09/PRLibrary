/*
Tao Du
taodu@stanford.edu
May 8, 2014
*/

//	PRSceneObject is a classical class defined
//	for objects with non spatially varying reflectance
//	properties. A pointer to material will help handle
//	color information

#ifndef _LIBPR_PRSCENEOBJECT_H_
#define _LIBPR_PRSCENEOBJECT_H_

#include "PRShape.h"
#include "PRMaterial.h"

class PRSceneObject
{
public:
	PRSceneObject(PRShape* shape, const PRTransform& t, PRMaterial* material);
	bool doesIntersect(const PRLine& line);
	PRIntersection getIntersection(const PRLine& line);
	double getIntersectionT(const PRLine& line);
	PRMaterial* getMaterial() {return m_material;}

private:
	//	shape
	PRShape *m_shape;
	//	transformation
	//	convention:
	//	for a point p in SceneObject's coordinates:
	//	m_transform * p will transform it into the camera space
	//	m_inverse * q will transform a point in camera space into
	//	the object space
	//	line.transform(m_inverse) will transform a line from the camera
	//	space into the object space
	//	m_inverseTranspose:
	//	m_inverseTranspose will transform normal from object space
	//	into camera space
	//	m_inverseTranspose * n - m_inverseTranspose * 0 will give the normal
	//	in camera space
	PRTransform m_tranform, m_inverse, m_inverseTranspose;

	//	material
	PRMaterial *m_material;
};

#endif