#include "stdafx.h"
#include "MyCamImage.h"


MyCamImage::MyCamImage(String windowname) : windowName(windowname)
{
	cam = VideoCapture(0);
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

bool MyCamImage::readImage(Mat *img, bool doesFlip)
{
	
	bool succes = cam.read(*img);
	if (flip)
	{
		flip(*img, *img, 1);
	}
	return succes;
}

void MyCamImage::showImage()
{
	imshow(windowName, capture);
}

void MyCamImage::showImage(Mat *img)
{
	imshow(windowName, *img);
}

void MyCamImage::setWindowName(String newWindowName)
{
	windowName = newWindowName;
}

String MyCamImage::getWindowName()
{
	return windowName;
}
