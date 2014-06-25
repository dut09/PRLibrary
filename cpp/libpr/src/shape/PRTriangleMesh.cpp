/*
Tao Du
taodu@stanford.edu
May 6, 2014
*/

//	this code is mainly from the CS 148 sample code
//	provided by Wenlong Lu and Bo Zhu
#include "../../include/shape/PRTriangle.h"
#include "../../include/shape/PRTriangleMesh.h"

PRTriangleMesh::PRTriangleMesh(const std::string objFileName)
{
	readObjFile(objFileName);
	build();
	//	build BVHTree and the bounding box
	m_triangles.reserve(m_faces.size());
	for (unsigned int i = 0; i < m_faces.size(); i++)
	{
		PRFace *f = m_faces.at(i);
		PRTriangle *p = new PRTriangle(f->vertex[0]->point,
			f->vertex[1]->point,
			f->vertex[2]->point);
		m_triangles.push_back(p);
		m_bbox = PRBBox::combine(m_bbox, p->getBBox());
	}
	m_bvhTree = new PRBVHTreeNode(m_triangles);
}

PRTriangleMesh::~PRTriangleMesh()
{
	//	deconstructor
	//	release all the memory from m_vertices and m_faces
	for (unsigned int i = 0; i < m_vertices.size(); i++)
		delete m_vertices[i];
	m_vertices.clear();
	for (unsigned int i = 0; i < m_faces.size(); i++)
		delete m_faces[i];
	for (unsigned int i = 0; i < m_triangles.size(); i++)
		delete m_triangles[i];
	m_faces.clear();
	//	delete all the triangles in bvhTree
	//	manually delete the triangles in the bvhTree
	if (m_bvhTree)
	{
		//	delete the bvhTree
		delete m_bvhTree;
	}
}

PRIntersection PRTriangleMesh::getIntersection(const PRLine &l)
{
	if (m_bvhTree)
		return m_bvhTree->getIntersection(l);
	else
		return PRIntersection(
			PRVector3(DBL_MAX, DBL_MAX, DBL_MAX), 
			PRVector3(DBL_MAX, DBL_MAX, DBL_MAX), 
			DBL_MAX
		);
}

bool PRTriangleMesh::doesIntersect(const PRLine &l)
{
	return getIntersectionT(l) != DBL_MAX;
}

double PRTriangleMesh::getIntersectionT(const PRLine &l)
{
	if (m_bvhTree)
		return m_bvhTree->getIntersectionT(l);
	else
		return DBL_MAX;
}

PRVector3 PRTriangleMesh::getVertex(int id)
{
	int num = getVertexNumber();
	assert(id >= 0 && id < num);
	return m_vertices.at(id)->point;
}

PRVector3 PRTriangleMesh::getVertexNormal(int id)
{
	int num = getVertexNumber();
	assert(id >= 0 && id < num);
	return m_vertices.at(id)->normal;
}

PRFace* PRTriangleMesh::getFace(int id)
{
	int num = getFaceNumber();
	assert(id >= 0 && id < num);
	return m_faces.at(id);
}

//	read .obj file	
void PRTriangleMesh::readObjFile(const std::string objFileName)
{
	std::ifstream fin;
	fin.open(objFileName, std::ios::in);
	//	clear current memory
	for (unsigned int i = 0; i < m_vertices.size(); i++)
		delete m_vertices[i];
	m_vertices.clear();
	for (unsigned int i = 0; i < m_vertices.size(); i++)
		delete m_faces[i];
	m_faces.clear();

	char comments[256];
	char token[128];
	double x, y, z;
	double nx, ny, nz;
	int v1, v2, v3;
	int n1, n2, n3;

	std::vector<PRVector3> normals;
	
	while (fin >> token)
	{
		if (strcmp(token, "#") == 0)
		{
			//	comments
			//	skip them
			fin.getline(comments, 256);
		}
		else if (strcmp(token, "v") == 0)
		{
			//	vertex
			fin >> x >> y >> z;
			m_vertices.push_back(new PRVertex(x, y, z));
		}
		else if (strcmp(token, "vn") == 0)
		{
			//	normal
			fin >> nx >> ny >> nz;
			normals.push_back(PRVector3(nx, ny, nz));
		}
		else if (strcmp(token, "f") == 0)
		{
			//	face
			//	we only consider the simple case
			//	f	v1//n1 v2//n2 v3//n3
			//	we assume the normal for vertex is consistent
			char slash;
			fin >> v1 >> slash >> slash >> n1 
				>> v2 >> slash >> slash >> n2 
				>> v3 >> slash >> slash >> n3;
			PRFace *f = new PRFace(m_vertices[v1 - 1], m_vertices[v2 - 1], m_vertices[v3 - 1]);
			f->vertex[0]->normal = normals[n1 - 1];
			f->vertex[1]->normal = normals[n2 - 1];
			f->vertex[2]->normal = normals[n3 - 1];
			m_faces.push_back(f);
		}
	}
}

void PRTriangleMesh::build()
{
	buildTopology();
	computeNormal();
}

void PRTriangleMesh::buildTopology()
{
	std::map<std::pair<PRVertex*, PRVertex*>, PRFace*> he2f;
	for (unsigned int i = 0; i < m_faces.size(); i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			m_faces[i]->vertex[j]->face = m_faces[i];
			he2f.insert(std::pair<std::pair<PRVertex*, PRVertex*>, PRFace*>
				(
					std::pair<PRVertex*, PRVertex*>(
						m_faces[i]->vertex[j], 
						m_faces[i]->vertex[(j + 1) % 3]), 
					m_faces[i]
				)
			);
		}
	}
	for (unsigned int i = 0; i < m_faces.size(); i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			std::map<std::pair<PRVertex*, PRVertex*>, PRFace*>::iterator itr
				= he2f.find(std::pair<PRVertex*, PRVertex*>(
							m_faces[i]->vertex[(j + 2) % 3], 
							m_faces[i]->vertex[(j + 1) % 3]
						)
					);
			if(itr != he2f.end())
				m_faces[i]->adjFace[j] = itr->second;
			else
				m_faces[i]->adjFace[j] = NULL;
		}
	}
}

void PRTriangleMesh::computeNormal()
{
	for (unsigned int i = 0; i < m_faces.size(); i++)
	{
		PRFace* face = m_faces[i];
		face->normal = PRVector3::cross(face->vertex[0]->point - face->vertex[1]->point,
			face->vertex[0]->point - face->vertex[2]->point);
	}
	for (unsigned int i = 0; i < m_faces.size(); i++)
	{
		PRVector3 n = m_faces[i]->normal;
		m_faces[i]->normal = n / n.length();
	}

	for (unsigned int i = 0; i < m_vertices.size(); i++)
	{
		PRVector3 n = m_vertices[i]->normal;
		m_vertices[i]->normal = n / n.length();
	}
}

PRFace* PRTriangleMesh::nextAdjFace(PRVertex *v, PRFace *f)
{
	if (v == f->vertex[0])
		return f->adjFace[1];
	else if (v == f->vertex[1])
		return f->adjFace[2];
	else if (v == f->vertex[2])
		return f->adjFace[0];
	else
		return NULL;
}

PRFace* PRTriangleMesh::nextAdjFaceReverse(PRVertex *v, PRFace *f)
{
	if (v == f->vertex[0])
		return f->adjFace[2];
	else if (v == f->vertex[1])
		return f->adjFace[0];
	else if (v == f->vertex[2])
		return f->adjFace[1];
	else
		return NULL;
}