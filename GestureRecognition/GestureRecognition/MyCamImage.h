#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<opencv2/opencv.hpp>

using namespace cv;

class MyCamImage
{
public:
	MyCamImage(String windowname);
	~MyCamImage();

	bool readImage(bool doesFlip = true);
	bool readImage(Mat *img, bool doesFlip = true);
	void showImage();
	void showImage(Mat *img);
	void setWindowName(String newWindowName);
	String getWindowName();

	Mat capture;
	VideoCapture cam;


private:
	String windowName;
	
};

