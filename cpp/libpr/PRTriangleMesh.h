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
#include "PRLumitexel.h"
#include "PRMaterial.h"

//	face: used in triangle mesh
struct PRFace;	

//	vertex: used in triangle mesh only!

//	PRVertex contains a point, a normal
//	a pointer to a PRFace and a pointer
//	to lumitexel
//	it is a really loose struct data structure
//	used to store information only
struct PRVertex
{
	PRVertex(double x, double y, double z)
	{
		point = PRVector3(x, y, z);
		normal = PRVector3(0.0, 0.0, 0.0);
		color = PRVector3(0.0, 0.0, 0.0);
		face = NULL;
		//	lumitexel information
		lumitexel = new PRLumitexel();
		material = NULL;
	}

	PRVertex(const PRVector3& p)
	{
		point = p;
		normal = PRVector3(0.0, 0.0, 0.0);
		color = PRVector3(0.0, 0.0, 0.0);
		face = NULL;
		lumitexel = new PRLumitexel();
		material = NULL;
	}

	PRVertex(const PRVector3& p, const PRVector3& c)
	{
		point = p;
		normal = PRVector3(0.0, 0.0, 0.0);
		color = c;
		face = NULL;
		lumitexel = new PRLumitexel();
		material = NULL;
	}

	PRVertex(double x, double y, double z, double r, double g, double b)
	{
		point = PRVector3(x, y, z);
		normal = PRVector3(0.0, 0.0, 0.0);
		color = PRVector3(r, g, b);
		face = NULL;
		lumitexel = new PRLumitexel();
		material = NULL;
	}

	~PRVertex()
	{
		//	delete lumitexel information
		delete lumitexel;
		if (material)
		{
			delete material;
			material = NULL;
		}
	}

	PRVector3 point;
	PRVector3 normal;
	PRVector3 color;
	PRFace *face;
	//	lumitexel information
	PRLumitexel *lumitexel;
	PRMaterial *material;
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
	
	//	add for triangle specifically
	PRIntersection getIntersectionWithTriangle(const PRLine &l, PRTriangle &triangle);
	
	//	get bounding box
	PRBBox getBBox() {return m_bbox;}

	//	get vertex number
	int getVertexNumber() {return (int)m_vertices.size();}
	//	get vertex
	//	get the point the vertex
	//	this function is dangerous because
	//	it fully exposes the vertex to the caller
	//	but it could allow the caller to add brdf samples
	//	into the lumitexel into the vertex
	PRVertex* getWholeVertex(int id);
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

	//	manually delete the triangles in m_bvhTree
	void deleteTriangleInTreeNode(PRBVHTreeNode *root);

	//	data member
	std::vector<PRVertex*> m_vertices;
	std::vector<PRFace*> m_faces;

	//	BVH tree
	PRBVHTreeNode *m_bvhTree;

	//	bounding box
	PRBBox m_bbox;
};

#endif