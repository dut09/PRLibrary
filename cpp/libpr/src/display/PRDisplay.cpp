/*
Tao Du
taodu@stanford.edu
Jun 23, 2014
*/

#include "../../include/display/PRDisplay.h"

#include "../../include/3rdparty/opencv2/core/core.hpp"
#include "../../include/3rdparty/opencv2/highgui/highgui.hpp"

void displayImage(const PRMatrix2D<PRVector3> &mat)
{
	//	get the height and width of the images
	//	assume depthFrame and colorFrame has the same size
	int height = mat.getHeight();
	int width = mat.getWidth();

	//	concatenate depth and color images together
	cv::Mat image = cv::Mat::zeros(height, width, CV_8UC3);
	//	draw the depth image first
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			PRVector3 color = PRVector3::max(mat.getData(i, j) * 255, PRVector3());
			color = PRVector3::min(color, PRVector3(255.0, 255.0, 255.0));
			image.at<cv::Vec3b>(i, j)[0] = (unsigned char)color.z;	//	b
			image.at<cv::Vec3b>(i, j)[1] = (unsigned char)color.y;	//	g
			image.at<cv::Vec3b>(i, j)[2] = (unsigned char)color.x;	//	r
		}
	}

	//	show the image
	cv::imshow("image", image);
	cv::waitKey();
	return;
}