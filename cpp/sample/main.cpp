#include "PhongMaterial.h"
#include "ConstantLight.h"

int main()
{
	//	read camera configuration file
	PRCamera camera("camera.prc");
	//	read the triangle mesh
	PRTriangleMesh bunnyMesh("bunny.obj");
	//	transformation
	PRTransform transform = PRTransform::translate(0, 0, 325) * PRTransform::scale(1.0, -1.0, 1.0);
	//	material
	PhongMaterial material(PRVector3(0.1, 0.1, 0.1), 
		PRVector3(0.3, 0.1, 0.5), 
		PRVector3(0.7, 0.2, 0.9), 
		15.0);
	//	object
	PRSceneObject bunny(&bunnyMesh, transform, &material);
	//	light
	ConstantLight light(PRVector3(15, 0, 0));

	//	imaging
	int height = 480;
	int width = 640;
	PRMatrix2D<PRVector3> image(height, width);
	image.reset(PRVector3());
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			PRVector3 dir = camera(h, w);
			PRLine line(PRVector3(), dir);
			PRIntersection inter = bunny.getIntersection(line);
			if (inter.t != DBL_MAX)
			{
				PRVector3 point = inter.point;
				image(h, w) = bunny.getMaterial()->shade(
					light.getLightRadiance(point),
					light.getLightPos() - point,
					-point, point, inter.normal);
			}
		}
	}
	image.writeIntoFile("image.prb");
	return 0;
}