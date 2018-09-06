#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<opencv2/opencv.hpp>

#include "MyCamImage.h"
using namespace cv;
using namespace std;

class HandRegistrationHandler
{
public:
	HandRegistrationHandler();
	~HandRegistrationHandler();
	void InitHandRegistration(MyCamImage *camImg);
	void SetBackground(Rect handRect);
	void RegisterHand();
	void FindAndHideFace(Mat &src);
	Mat FindHand(Mat src);
	Mat getRegistrationImg();
	Rect Face;

private : 
	void AddSquareRegistration();
	void InitFaceDetection();
	Mat Filtering(Mat src);

	Mat hist;
	Mat backProject;
	vector<cv::Rect> rois;
	vector<Scalar>  medians;
	vector<Mat> thresholds;
	Mat registrationImg;
	int squareSize;
	CascadeClassifier face_cascade;
	String face_cascade_name = "haarcascade_frontalface_alt2.xml";
	int hmin, smin, vmin, hmax, smax, vmax = 0;
};

