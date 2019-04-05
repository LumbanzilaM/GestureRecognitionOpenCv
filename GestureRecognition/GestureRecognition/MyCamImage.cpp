#include "stdafx.h"
#include "MyCamImage.h"


MyCamImage::MyCamImage(cv::String windowname) : windowName(windowname)
{
	cam = cv::VideoCapture(0);
	namedWindow(windowName, CV_WINDOW_KEEPRATIO);
}


MyCamImage::~MyCamImage()
{
}

bool MyCamImage::readImage(bool doesFlip)
{
	
	bool succes = cam.read(capture);
	if (doesFlip)
	{
		flip(capture, capture, 1);
	}
	return succes;
}

bool MyCamImage::readImage(cv::Mat *img, bool doesFlip)
{
	
	bool succes = cam.read(*img);
	if (cv::flip)
	{
		flip(*img, *img, 1);
	}
	return succes;
}

void MyCamImage::showImage()
{
	imshow(windowName, capture);
}

void MyCamImage::showImage(cv::Mat *img)
{
	imshow(windowName, *img);
}

void MyCamImage::setWindowName(cv::String newWindowName)
{
	windowName = newWindowName;
}

cv::String MyCamImage::getWindowName()
{
	return windowName;
}
