#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>

#include "MyCamImage.h"
#include "Hand.h"
#include "MathOperation.h"
using namespace std;

class HandRegistrationHandler
{
public:
	HandRegistrationHandler();
	~HandRegistrationHandler();
	void InitHandRegistration(MyCamImage *camImg);
	void SetBackground(cv::Rect handRect);
	void RegisterHand();
	void FindAndHideFace(cv::Mat &src);
	void FindBoundaries(cv::Mat src, Hand *hand);
	void FindClosestBondary(cv::Mat src, Hand * hand, cv::Point pt);
	void FindWristPoints(Hand *hand);
	cv::Mat FilterHand(cv::Mat src);
	vector<cv::Mat> FindHands(cv::Mat src);
	void FindPalmCenter(cv::Mat src, Hand *hand, bool draw = true);
	cv::Mat getRegistrationImg();
	cv::Rect Face;
	cv::Mat ExtractPalm(cv::Mat src, Hand* hand);
	cv::Mat RotateHand(cv::Mat src, Hand * hand);
	cv::Mat ExtractFingers(cv::Mat src, Hand* hand);



private : 
	void AddSquareRegistration();
	void InitFaceDetection();
	void CreatePalmCircle(Hand *hand);
	cv::Mat Filtering(cv::Mat src);
	vector<vector<cv::Point>> FindTwoBiggestContoursBBox(cv::Mat src);

	MyCamImage *TrackBars;
	cv::Mat hist;
	cv::Mat backProject;
	vector<cv::Rect> rois;
	vector<cv::Scalar>  medians;
	vector<cv::Mat> thresholds;
	cv::Mat registrationImg;
	int squareSize;
	cv::CascadeClassifier face_cascade;
	cv::String face_cascade_name = "haarcascade_frontalface_alt2.xml";
	int hmin, smin, vmin, hmax, smax, vmax = 0;
	int adaptedVmin, adaptedVmax = 0;
	list<int> boundPixelX;
	list<int> boundPixelY;
};

