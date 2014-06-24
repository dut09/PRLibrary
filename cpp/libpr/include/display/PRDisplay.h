/*
Tao Du
taodu@stanford.edu
Jun 23, 2014
*/

//	use opencv to display PRMatrix2D<PRVector3> images

#ifndef _LIBPR_PRDISPLAY_H_
#define _LIBPR_PRDISPLAY_H_

#include "../base/PRVector3.h"
#include "../base/PRMatrix2D.h"

void displayImage(const PRMatrix2D<PRVector3> &image);

#endif