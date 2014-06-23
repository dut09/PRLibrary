/*
Tao Du
taodu@stanford.edu
April 10, 2014
*/
#ifndef _LIBPR_PRPIXEL_H_
#define _LIBPR_PRPIXEL_H_

enum PR_PIXEL_DIR {PR_UP = 0, PR_UP_RIGHT, PR_RIGHT, PR_DOWN_RIGHT, PR_DOWN, PR_DOWN_LEFT, PR_LEFT, PR_UP_LEFT};

//	this is PRPixel class, we use it to represent the pixel in the image
//	note that we don't check the range of pixels, it only stores two ints

//	the coordinates for the pixel:
//	consider a h x w image, the range of pixel is [0, h - 1] x [0, w - 1]
//	the origin is in the upper left corner, i.e.,
//	[0][0] [0][1] [0][2] ... [0][w - 1]
//	[1][0] [1][1] [1][2] ... [1][w - 1]
//	etc

class PRPixel
{
public:
	//	constructor
	PRPixel();
	PRPixel(double h, double w);
	//	copy constructor
	PRPixel(const PRPixel& p);

	//	overload operator
	//	assignment
	PRPixel& operator=(const PRPixel &p);

	//	comparison
	bool operator==(const PRPixel& p) const;
	bool operator!=(const PRPixel& p) const;
	//	[] subscription
	double& operator[](int i);

	//	neighborhood
	//	step controls the distance between the neighbor and the pixel
	//	step = 0 returns the pixel itself
	PRPixel getNeighbor(PR_PIXEL_DIR dir, int step) const;

	//	data member
	double height, width;
};


#endif