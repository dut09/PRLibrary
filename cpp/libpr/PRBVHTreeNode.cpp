/*
Tao Du
taodu@stanford.edu
May 7, 2014
*/
#include "PRBVHTreeNode.h"

bool compareX(PRTriangle* t1, PRTriangle* t2)
{
	PRVector3 c1 = t1->getBBox().getCenter();
	PRVector3 c2 = t2->getBBox().getCenter();
	return c1.x < c2.x;
}

bool compareY(PRTriangle* t1, PRTriangle* t2)
{
	PRVector3 c1 = t1->getBBox().getCenter();
	PRVector3 c2 = t2->getBBox().getCenter();
	return c1.y < c2.y;
}

bool compareZ(PRTriangle* t1, PRTriangle* t2)
{
	PRVector3 c1 = t1->getBBox().getCenter();
	PRVector3 c2 = t2->getBBox().getCenter();
	return c1.z < c2.z;
}

//	init a bvh tree leaf node
PRBVHTreeNode::PRBVHTreeNode(PRTriangle *triangle)
{
	//	build a leaf node
	this->bbox = triangle->getBBox();
	this->triangle = triangle;
	this->left = this->right = NULL;
}

//	init a bvh tree
PRBVHTreeNode::PRBVHTreeNode(std::vector<PRTriangle *> &objs)
{
	int num = (int)objs.size();
	if (num == 0)
	{
		this->triangle = NULL;
		this->left = this->right = NULL;
	}
	else if (num == 1)
	{
		this->triangle = objs.at(0);
		this->left = this->right = NULL;
		this->bbox = this->triangle->getBBox();
	}
	else if (num == 2)
	{
		this->triangle = NULL;
		this->left = new PRBVHTreeNode(objs[0]);
		this->right = new PRBVHTreeNode(objs[1]);
		this->bbox = PRBBox::combine(this->left->bbox, this->right->bbox);
	}
	else
	{
		PRBBox bound;
		for (unsigned int i = 0; i < objs.size(); i++)
		{
			bound = PRBBox::combine(bound, objs[i]->getBBox());
		}
		int id = bound.getMaxExtentId();
		switch (id)
		{
		case 0:
			std::sort(objs.begin(), objs.end(), compareX);
			break;
		case 1:
			std::sort(objs.begin(), objs.end(), compareY);
			break;
		case 2:
		default:
			//	use case 2
			std::sort(objs.begin(), objs.end(), compareZ);
			break;
		}
		std::vector<PRTriangle *> lobjs;
		std::vector<PRTriangle *> robjs;
		for (int i = 0; i < num / 2; i++)
			lobjs.push_back(objs[i]);
		for (int i = num / 2; i < num; i++)
			robjs.push_back(objs[i]);

		this->triangle = NULL;
		this->left = new PRBVHTreeNode(lobjs);
		this->right = new PRBVHTreeNode(robjs);
		this->bbox = PRBBox::combine(this->left->bbox, this->right->bbox);
	}
}

//	delete a bvh tree node
PRBVHTreeNode::~PRBVHTreeNode()
{
	if (left)
		delete left;
	if (right)
		delete right;
}

PRIntersection PRBVHTreeNode::getIntersection(const PRLine &l)
{
	if (triangle)
	{
		//	leaf node
		return triangle->getIntersection(l);
	}
	else
	{
		//	interior node
		if (this->bbox.doesIntersect(l))
		{
			//	test left and right
			PRIntersection iLeft = left->getIntersection(l);
			PRIntersection iRight = right->getIntersection(l);
			//	decide which one is closer
			if (iLeft.t == DBL_MAX)
				return iRight;
			else if (iRight.t == DBL_MAX)
				return iLeft;
			else
				return iLeft.t < iRight.t ? iLeft : iRight;
		}
		else
		{
			//	no intersection
			return PRIntersection(
					PRVector3(DBL_MAX, DBL_MAX, DBL_MAX),
					PRVector3(DBL_MAX, DBL_MAX, DBL_MAX),
					DBL_MAX
				);
		}
	}
}

bool PRBVHTreeNode::doesIntersect(const PRLine &l)
{
	return getIntersectionT(l) != DBL_MAX;
}

double PRBVHTreeNode::getIntersectionT(const PRLine &l)
{
	if (triangle)
	{
		//	leaf node
		return triangle->getIntersectionT(l);
	}
	else
	{
		//	interior node
		if (this->bbox.doesIntersect(l))
		{
			double tLeft = this->left->getIntersectionT(l);
			double tRight = this->right->getIntersectionT(l);
			if (tLeft == DBL_MAX)
				return tRight;
			else if (tRight == DBL_MAX)
				return tLeft;
			else
				return tLeft < tRight ? tLeft : tRight;
		}
		else
		{
			return DBL_MAX;
		}
	}
}

//	add for bvhtree specifically
PRIntersection PRBVHTreeNode::getIntersectionWithTriangle(const PRLine &l, PRTriangle &t)
{
	if (triangle)
	{
		//	leaf node
		PRIntersection inter = triangle->getIntersection(l);
		//	if it is a valid intersection
		if (inter.t != DBL_MAX)
			t = *triangle;
		return inter;
	}
	else
	{
		//	interior node
		if (this->bbox.doesIntersect(l))
		{
			//	test left and right
			PRTriangle t1, t2;
			PRIntersection iLeft = left->getIntersectionWithTriangle(l, t1);
			PRIntersection iRight = right->getIntersectionWithTriangle(l, t2);
			//	decide which one is closer
			if (iLeft.t == DBL_MAX && iRight.t == DBL_MAX)
				return iLeft;
			else if (iRight.t == DBL_MAX)
			{	
				t = t1;
				return iLeft;
			}
			else
			{
				t = iLeft.t < iRight.t ? t1 : t2;
				return iLeft.t < iRight.t ? iLeft : iRight;
			}
		}
		else
		{
			//	no intersection
			return PRIntersection(
					PRVector3(DBL_MAX, DBL_MAX, DBL_MAX),
					PRVector3(DBL_MAX, DBL_MAX, DBL_MAX),
					DBL_MAX
				);
		}
	}
}