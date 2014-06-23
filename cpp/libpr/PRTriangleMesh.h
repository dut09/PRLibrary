/*
Tao Du
taodu@stanford.edu
May 5, 2014
*/

//	triangle mesh definition
//	we have to support the following:
//		read obj files
//		compute normal for each vertex
//		triangle intersection
#ifndef _LIBPR_PRTRIANGLEMESH_H_
#define _LIBPR_PRTRIANGLEMESH_H_

#include "prStd.h"
#include "PRShape.h"
#include "PRBVHTreeNode.h"
#include "PRMaterial.h"

//	face: used in triangle mesh
struct PRFace;	

//	vertex: used in triangle mesh only!

//	PRVertex contains a point, a normal
//	a pointer to a PRFace
struct PRVertex
{
	PRVertex(double x, double y, double z)
	{
		point = PRVector3(x, y, z);
		normal = PRVector3(0.0, 0.0, 0.0);
		face = NULL;
	}

	PRVertex(const PRVector3& p)
	{
		point = p;
		normal = PRVector3(0.0, 0.0, 0.0);
		face = NULL;
	}

	PRVector3 point;
	PRVector3 normal;
	PRFace *face;
};

//	PRFace contains three vertex
//	and three adjecent faces
//	as well as a normal
struct PRFace
{
	PRFace(PRVertex* v0, PRVertex* v1, PRVertex* v2)
	{
		vertex[0] = v0;
		vertex[1] = v1;
		vertex[2] = v2;
	}

	PRVertex *vertex[3];
	PRFace *adjFace[3];
	PRVector3 normal;
};

//	the code below are modified from CS 148 sample code
//	I think the author is Wenlong Lu?
//	triangle mesh should implement the interface
//	PRShape
class PRTriangleMesh : public PRShape
{
public:
	//	constructor
	//	the objFileName is the filename of the .obj file
	//	of the triangle mesh
	PRTriangleMesh(const std::string objFileName);
	//	deconstructor
	~PRTriangleMesh();

	PRIntersection getIntersection(const PRLine &l);
	bool doesIntersect(const PRLine &l);
	double getIntersectionT(const PRLine &l);
	
	//	get bounding box
	PRBBox getBBox() {return m_bbox;}

	//	get vertex number
	int getVertexNumber() {return (int)m_vertices.size();}
	PRVector3 getVertex(int id);
	PRVector3 getVertexNormal(int id);

	//	face
	int getFaceNumber() {return (int)m_faces.size();}
	PRFace* getFace(int id);

private:
	//	these functions are from CS148 source code
	//	provided by the TAs
	void readObjFile(const std::string objFileName);
	void build();
	void buildTopology();
	void computeNormal();

	PRFace* nextAdjFace(PRVertex *v, PRFace *f);
	PRFace* nextAdjFaceReverse(PRVertex *v, PRFace *f);

	//	data member
	std::vector<PRVertex*> m_vertices;
	std::vector<PRFace*> m_faces;
	//	build trinagles from the faces
	std::vector<PRShape *> m_triangles;

	//	BVH tree
	PRBVHTreeNode *m_bvhTree;

	//	bounding box
	PRBBox m_bbox;
};

#endif