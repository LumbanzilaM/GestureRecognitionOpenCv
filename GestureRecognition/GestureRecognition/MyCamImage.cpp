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

bool MyCamImage::readImage()
{
	return cam.read(capture);
}

bool MyCamImage::readImage(Mat *img)
{
	return cam.read(*img);
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
