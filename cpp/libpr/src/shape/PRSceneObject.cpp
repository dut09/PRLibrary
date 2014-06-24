/*
Tao Du
taodu@stanford.edu
May 8, 2014
*/

#include "../../include/shape/PRSceneObject.h"

PRSceneObject::PRSceneObject(PRShape* shape, const PRTransform& t, PRMaterial* material)
{
	m_shape = shape;
	m_tranform = t;
	m_inverse = m_tranform.inverse();
	m_inverseTranspose = m_inverse.transpose();
	m_material = material;
}

bool PRSceneObject::doesIntersect(const PRLine& line)
{
	return m_shape->doesIntersect(line.transform(m_inverse));
}

PRIntersection PRSceneObject::getIntersection(const PRLine& line)
{
	PRIntersection inter = m_shape->getIntersection(line.transform(m_inverse));
	//	transform it back!
	if (inter.t == DBL_MAX)
		return inter;
	inter.point = m_tranform * inter.point;
	//	transform the normal
	inter.normal = m_inverseTranspose * inter.normal 
		- m_inverseTranspose * PRVector3(0.0, 0.0, 0.0);
	return inter;
}

double PRSceneObject::getIntersectionT(const PRLine& line)
{
	return m_shape->getIntersectionT(line.transform(m_inverse));
}