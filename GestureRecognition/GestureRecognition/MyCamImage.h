#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<opencv2/opencv.hpp>


class MyCamImage
{
public:
	MyCamImage(cv::String windowname);
	~MyCamImage();

	bool readImage(bool doesFlip = true);
	bool readImage(cv::Mat *img, bool doesFlip = true);
	void showImage();
	void showImage(cv::Mat *img);
	void setWindowName(cv::String newWindowName);
	cv::String getWindowName();

	cv::Mat capture;
	cv::VideoCapture cam;


private:
	cv::String windowName;
	
};

