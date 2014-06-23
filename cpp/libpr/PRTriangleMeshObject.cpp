#include "PRTriangleMeshObject.h"

PRTriangleMeshObject::PRTriangleMeshObject(PRTriangleMesh* mesh, const PRTransform& t)
{
	m_mesh = mesh;
	m_transform = t;
	m_inverse = m_transform.inverse();
	m_inverseTranspose = m_inverse.transpose();
}

bool PRTriangleMeshObject::doesIntersect(const PRLine& line)
{
	return m_mesh->doesIntersect(line.transform(m_inverse));
}

PRIntersection PRTriangleMeshObject::getIntersection(const PRLine& line)
{
	PRIntersection inter = m_mesh->getIntersection(line.transform(m_inverse));
	//	transform it back!
	if (inter.t == DBL_MAX)
		return inter;
	inter.point = m_transform * inter.point;
	//	transform the normal
	inter.normal = m_inverseTranspose * inter.normal 
		- m_inverseTranspose * PRVector3(0.0, 0.0, 0.0);
	return inter;
}

double PRTriangleMeshObject::getIntersectionT(const PRLine& line)
{
	return m_mesh->getIntersectionT(line.transform(m_inverse));
}

//	get vertex in the world frame
PRVector3 PRTriangleMeshObject::getVertex(int id)
{
	PRVector3 p = m_mesh->getVertex(id);
	return m_transform * p;
}

//	get vertex normal in the world frame
PRVector3 PRTriangleMeshObject::getVertexNormal(int id)
{
	PRVector3 n = m_mesh->getVertexNormal(id);
	return m_inverseTranspose * n - m_inverseTranspose * PRVector3(0.0, 0.0, 0.0);
}

PRIntersection PRTriangleMeshObject::getIntersectionWithTriangle(const PRLine& line, PRTriangle& t)
{
	PRIntersection inter = m_mesh->getIntersectionWithTriangle(line.transform(m_inverse), t);
	//	transform it back!
	if (inter.t == DBL_MAX)
		return inter;
	inter.point = m_transform * inter.point;
	//	transform the normal
	inter.normal = m_inverseTranspose * inter.normal 
		- m_inverseTranspose * PRVector3(0.0, 0.0, 0.0);
	//	transform the vertex in triangle
	t = t.transform(m_transform);
	return inter;
}

PRIntersection PRTriangleMeshObject::getIntersectionWithColor(const PRLine& line, PRVector3& c)
{
	PRTriangle t;
	PRIntersection inter = m_mesh->getIntersectionWithTriangle(line.transform(m_inverse), t);
	if (inter.t == DBL_MAX)
		return inter;
	//	get the color
	c = t.getColor(inter.point);
	//	transform the intersection into camera space
	inter.point = m_transform * inter.point;
	//	transform the normal
	inter.normal = m_inverseTranspose * inter.normal 
		- m_inverseTranspose * PRVector3(0.0, 0.0, 0.0);
	return inter;
} 