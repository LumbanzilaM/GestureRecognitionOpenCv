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
	void FindBoundaries(Mat src, Hand *hand);
	void FindWristPoints(Hand *hand);
	Mat FilterHand(Mat src);
	vector<Mat> FindHands(Mat src);
	void FindPalmCenter(Mat src, Hand *hand, bool draw = true);
	Mat getRegistrationImg();
	Rect Face;

private : 
	void AddSquareRegistration();
	void InitFaceDetection();
	void CreatePalmCircle(Hand *hand);
	Mat Filtering(Mat src);

	MyCamImage *TrackBars;
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
	int adaptedVmin, adaptedVmax = 0;
	list<int> boundPixelX;
	list<int> boundPixelY;
};

