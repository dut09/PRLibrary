/*
Tao Du
taodu@stanford.edu
May 9, 2014
*/

//	PRTriangleMeshObject is a class specifically designed
//	for our project
//	it wraps the class PRTriangleMesh, and provides interface
//	to compute the intersecction and get the color for the object
//	m_mesh stores the geometric data for the whole triangle mesh
//	each triangle contains color information for each vertex
//	and we can use getIntersectionWithTriangle and getIntersectionWithColor
//	to help get the color
//	the logic behind PRTriangleMeshObject:
//	PRTriangleMesh: contains the vertex and face of the triangle mesh
//	each triangle contains color information from the obj file only for reference
//	they are used when aligning depth images
//	to implement spatially varying brdf materials, we have to implement an interface
//	for triangle mesh object


#ifndef _LIBPR_PRTRIANGLEMESHOBJECT_H_
#define _LIBPR_PRTRIANGLEMESHOBJECT_H_

#include "PRTriangleMesh.h"

class PRTriangleMeshObject
{
public:
	PRTriangleMeshObject(PRTriangleMesh* mesh, const PRTransform& t);
	bool doesIntersect(const PRLine& line);
	PRIntersection getIntersection(const PRLine& line);
	double getIntersectionT(const PRLine& line);
	
	//	get vertex information from triangle mesh
	//	they are all in the world frame
	//	get vertex number
	int getVertexNumber() {return m_mesh->getVertexNumber();}
	//	get vertex in the world frame
	PRVector3 getVertex(int id);
	//	get vertex normal in the world frame
	PRVector3 getVertexNormal(int id);

	PRIntersection getIntersectionWithTriangle(const PRLine& line, PRTriangle& t);
	//	this function is used only in the case of aligning different depth images
	//	to use this function correctly, please guarantee your obj file contains
	//	color information for each vertex!
	PRIntersection getIntersectionWithColor(const PRLine& line, PRVector3& c);

	//	we need an interface to represent the spatially varying brdf

private:
	//	shape
	PRTriangleMesh *m_mesh;
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
	PRTransform m_transform, m_inverse, m_inverseTranspose;
};

#endif