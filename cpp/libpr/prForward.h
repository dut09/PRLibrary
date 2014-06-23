/*
Tao Du
taodu@stanford.edu
April 8, 2014
*/

#ifndef _LIBPR_PRFORWARD_H_
#define _LIBPR_PRFORWARD_H_
//	this header file includes all the class/struct definition in the libpr
//	you can find the descriptions for each class in their header file
//	or in the header file pr.h

//	basic class: vector, pixel and matrix
class PRVector3;
class PRPixel;

//	geometry class in ray tracing
struct PRIntersection;
struct PRBVHTreeNode;

class PRBBox;
class PRTriangleMesh;
class PRLine;
class PRShape;
class PRTriangle;
class PRTransform;
class PRSceneObject;
class PRTriangleMeshObject;
class PRMaterial;
class PRBrdfSample;
class PRLumitexel;

//	rasterization class
class PRBilinearInterpolater;
class PRCamera;
class PRLightModel;
class PRStereoCameras;

#endif