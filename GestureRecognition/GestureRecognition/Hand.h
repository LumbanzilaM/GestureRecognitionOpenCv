#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<opencv2/opencv.hpp>

using namespace std;

class Hand
{
public:
	Hand();
	~Hand();
	
	cv::Rect HandRect;
	cv::RotatedRect HandRotatedRect;
	vector<cv::Point> HandContour;
	vector<cv::Point> PalmCircle;
	vector<cv::Point> PalmContour;
	cv::Point PalmCenter;
	size_t PalmRadius;
	vector<vector<cv::Point>> FingersContour;
	size_t NbFingerUp;
	vector<cv::Point>  FingerTips;
	vector<cv::Point> WristPoints;
	vector<cv::Rect> FingersRect;
	vector<cv::RotatedRect> FingersRotatedRect;
	vector<cv::Point> FingersCenter;
	vector<cv::Point> FingersDefectsBot;
	vector<cv::Point> FingersDefectsTop;
	int ThumbIdx = -1;
	float RotationAngle;

private:



};

