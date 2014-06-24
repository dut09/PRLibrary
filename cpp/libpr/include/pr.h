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
#include "base/PRVector3.h"
#include "base/PRPixel.h"
#include "base/PRMatrix2D.h"

//	geometry class in ray tracing
#include "shape/PRBBox.h"				//	bounding box
#include "shape/PRTriangleMesh.h"		//	triangle mesh
#include "accel/PRBVHTreeNode.h"			//	bvh tree: acceleration structure
#include "base/PRIntersection.h"		//	intersectio point
#include "base/PRLine.h"				//	line: used to represent the ray
#include "shape/PRShape.h"			//	virtual class, define the interfaces for all the shapes
#include "shape/PRTriangle.h"			//	triangle
#include "base/PRTransform.h"		//	transformation
#include "shape/PRSceneObject.h"
#include "material/PRMaterial.h"

//	rasterization class
#include "camera/PRCamera.h"				//	camera with intrinsic/extrinsic parameters
#include "light/PRLightModel.h"			//	light model
#include "camera/PRStereoCameras.h"		//	stereo camera systems

#endif