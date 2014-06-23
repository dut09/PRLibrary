/*
Tao Du
taodu@stanford.edu
May 5, 2014
*/

//	here we implement PRBVHTree class
//	including the corresponding PRBVHTreeNode class
//	the BVHTree is specifically designed for triangle meshes
//	because we have a slightly different problem:
//	the triangle has its own color
//	so we cannot assign a material for the whole triangle mesh
//	the triangle will provide an interface to get the color from
//	a specific point
#ifndef _LIBPR_PRBVHTREENODE_H_
#define _LIBPR_PRBVHTREENODE_H_

#include "prStd.h"
#include "PRTriangle.h"

//	PRBVHTreeNode should implements
//	the virtual class PRShape
struct PRBVHTreeNode : public PRShape
{
	PRBVHTreeNode(PRTriangle *triangle);
	PRBVHTreeNode(std::vector<PRTriangle *> &objs);
	~PRBVHTreeNode();

	PRIntersection getIntersection(const PRLine &l);
	bool doesIntersect(const PRLine &l);
	double getIntersectionT(const PRLine &l);

	//	add for bvhtree specifically
	PRIntersection getIntersectionWithTriangle(const PRLine &l, PRTriangle &t);

	//	get bounding box
	PRBBox getBBox() {return bbox;}

	//	interior node:
	//		triangle = NULL
	//	leaf node:
	//		triangle != NULL
	//		left = right = NULL
	PRBBox bbox;
	PRTriangle *triangle;
	PRBVHTreeNode *left;
	PRBVHTreeNode *right;
};

#endif