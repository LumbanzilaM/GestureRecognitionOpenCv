#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Hand
{
public:
	Hand();
	~Hand();
	
	Rect HandRect;
	RotatedRect HandRotatedRect;
	vector<Point> HandContour;
	Point PalmCenter;
	size_t PalmRadius;
	vector<vector<Point>> FingersContour;
	size_t NbFingerUp;

private:


};

