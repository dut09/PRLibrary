/*
Tao Du
taodu@stanford.edu
April 8, 2014
*/

#ifndef _LIBPR_PR_H_
#define _LIBPR_PR_H_

//	forward header
#include "prForward.h"

//	include all the header files in libpr

//	basic class: vector, pixel and matrix
#include "PRVector3.h"
#include "PRPixel.h"
#include "PRMatrix2D.h"

//	geometry class in ray tracing
#include "PRBBox.h"				//	bounding box
#include "PRTriangleMesh.h"		//	triangle mesh
#include "PRBVHTreeNode.h"			//	bvh tree: acceleration structure
#include "PRIntersection.h"		//	intersectio point
#include "PRLine.h"				//	line: used to represent the ray
#include "PRShape.h"			//	virtual class, define the interfaces for all the shapes
#include "PRTriangle.h"			//	triangle
#include "PRTransform.h"		//	transformation
#include "PRSceneObject.h"
#include "PRMaterial.h"

//	rasterization class
#include "PRCamera.h"				//	camera with intrinsic/extrinsic parameters
#include "PRLightModel.h"			//	light model
#include "PRStereoCameras.h"		//	stereo camera systems

#endif