/*
Tao Du
taodu@stanford.edu
May 5, 2014
*/

//	here we implement PRBVHTree class
//	including the corresponding PRBVHTreeNode class
#ifndef _LIBPR_PRBVHTREENODE_H_
#define _LIBPR_PRBVHTREENODE_H_

#include "../prStd.h"
#include "../shape/PRShape.h"

//	PRBVHTreeNode should implements
//	the virtual class PRShape
struct PRBVHTreeNode : public PRShape
{
	PRBVHTreeNode(PRShape *shape);
	PRBVHTreeNode(std::vector<PRShape *> &objs);
	~PRBVHTreeNode();

	PRIntersection getIntersection(const PRLine &l);
	bool doesIntersect(const PRLine &l);
	double getIntersectionT(const PRLine &l);

	//	get bounding box
	PRBBox getBBox() {return bbox;}

	//	interior node:
	//		shape = NULL
	//	leaf node:
	//		shape != NULL
	//		left = right = NULL
	PRBBox bbox;
	PRShape *shape;
	PRBVHTreeNode *left;
	PRBVHTreeNode *right;
};

#endif