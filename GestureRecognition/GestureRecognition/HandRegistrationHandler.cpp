#include "stdafx.h"
#include "HandRegistrationHandler.h"


HandRegistrationHandler::HandRegistrationHandler()
{
	squareSize = 20;
	bool lol;
	lol = face_cascade.load(face_cascade_name);

}


HandRegistrationHandler::~HandRegistrationHandler()
{
}

void HandRegistrationHandler::InitHandRegistration(MyCamImage *camImg)
{
cv:String name = camImg->getWindowName();
	hmin = 16;
	hmax = 16;
	vmin = 0;
	vmax = 255;
	smin = 26;
	smax = 26;
	createTrackbar("Hmin", name, &hmin, 256, 0);
	createTrackbar("Hmax", name, &hmax, 256, 0);
	createTrackbar("Vmin", name, &vmin, 256, 0);
	createTrackbar("Vmax", name, &vmax, 256, 0);
	createTrackbar("Smin", name, &smin, 256, 0);
	createTrackbar("Smaw", name, &smax, 256, 0);

	camImg->readImage(&registrationImg);
	AddSquareRegistration();
	while (1) {

		camImg->readImage(&registrationImg);

		for each (Rect rect in rois)
		{
			rectangle(registrationImg, rect, Scalar(255, 0, 0), 2);
		}

		camImg->showImage(&registrationImg);

		if (waitKey(30) == 'n')
		{
			camImg->readImage(&registrationImg);
			break;
		}
	}
}

void HandRegistrationHandler::SetBackground(Rect handRect)
{
}


void HandRegistrationHandler::RegisterHand()
{
	int i = 0;
	for each (Rect rect in rois)
	{
		Mat registrationImgHSV;
		cvtColor(registrationImg(rect), registrationImgHSV, CV_RGB2HSV);
		Scalar median = mean(registrationImgHSV);
		medians.push_back(median);
		double a = median[0];
		double b = median[1];
		double c = median[2];
		i++;
	}
	//FindAndHideFace(registrationImg);
	//FindHand(registrationImg);
}

void HandRegistrationHandler::FindAndHideFace(Mat &src)
{
	Mat srcGray;
	std::vector<Rect> faces;
	cvtColor(src, srcGray, CV_RGB2GRAY);
	face_cascade.detectMultiScale(srcGray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(100, 100));
	size_t lol = faces.size();
	for each (Rect face in faces)
	{
		rectangle(src, face, Scalar(0), -2);
		Face = face;
	}
	//rectangle(src, Face, Scalar(0), -2);
}

Mat HandRegistrationHandler::FilterHand(Mat src)
{
	Mat result;
	Mat srcHSV;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1));

	//Erase noise with erode, and find contours
	result = Filtering(src);
	erode(result, result, element, Point(1, 1), 3);
	findContours(result, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	//Draw filled contours
	Mat drawing = Mat::zeros(result.size(), CV_8UC3);
	vector<vector<Point>> hull(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 255);
		convexHull(contours[i], hull[i]);
		drawContours(drawing, contours, i, color, FILLED, 8, hierarchy, 0, Point());
		//drawContours(drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	}
	//Dilate result for counterbalance the previous erode
	dilate(drawing, drawing, element, Point(1, 1), 8);
	imshow("Contours", drawing);
	cvtColor(drawing, result, CV_RGB2GRAY);
	GaussianBlur(result, result, Size(3, 3), 1);
	medianBlur(result, result, 7);
	//imshow("result", result);
	return result;
}

vector<Mat> HandRegistrationHandler::FindHands(Mat src)
{
	return vector<Mat>();
}

void HandRegistrationHandler::FindPalmCenter(Mat src, Hand *hand, bool draw)
{
	Mat dist;
	Point minLoc, maxLoc;
	bool isMaxRadiusReached = false;
	double min, max;
	distanceTransform(src, dist, DIST_L2, 5);
	normalize(dist, dist, 0, 1, NORM_MINMAX);
	minMaxLoc(dist, &min, &max, &minLoc, &maxLoc);
	hand->PalmCenter = maxLoc;
	if (draw)
	{
		circle(dist, maxLoc, 10, Scalar(255, 0, 0), -2);
		imshow("dist", dist);
	}
	size_t circleRadius = 0;
	vector<Point> circleContour;
	try
	{


		while (!isMaxRadiusReached)
		{
			ellipse2Poly(maxLoc, Size(circleRadius, circleRadius), 0, 0, 360, 1, circleContour);
			for each (Point pt in circleContour)
			{
				if (pt.x > 0 && pt.y > 0 && pt.x < src.rows && pt.y < src.cols)
				{
					unsigned char color = src.at<unsigned char>(pt);
					
					if (color != 255)
					{
						isMaxRadiusReached = true;
						hand->PalmRadius = circleRadius;
						break;
					}
				}
			}
			circleRadius += 5;
			circleContour.clear();
		}
	}
	catch (Exception e)
	{
		cout << e.msg;
	}
}

Mat HandRegistrationHandler::getRegistrationImg()
{
	return registrationImg;
}

void HandRegistrationHandler::AddSquareRegistration()
{
	rois.push_back(Rect(registrationImg.cols / 5, registrationImg.rows / 5, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 7, registrationImg.rows / 3, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 4, registrationImg.rows / 3, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 5, registrationImg.rows / 2.5, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 4.5, registrationImg.rows / 2, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 6, registrationImg.rows / 2, squareSize, squareSize));
	;
}

void HandRegistrationHandler::InitFaceDetection()
{

}


//Use the hand hsv median values to perform tresholds and add them
Mat HandRegistrationHandler::Filtering(Mat src)
{
	Mat srcHSV;
	vector<Mat> thresholds;
	Mat out;
	cvtColor(src, srcHSV, CV_RGB2HSV);
	imshow("hsv", srcHSV);
	for each (Scalar median in medians)
	{
		Scalar lower(median[0] - hmin, median[1] - smin, median[2] - vmin);
		Scalar upper(median[0] + hmax, median[1] + smax, median[2] + vmax);
		Mat thresholdImg;
		inRange(srcHSV, lower, upper, thresholdImg);
		thresholds.push_back(thresholdImg);
	}

	out = thresholds[0];
	for each (Mat thresholdImg in thresholds)
	{
		out += thresholdImg;
	}
	imshow("result before manipulation", out);
	thresholds.clear();
	return out;
}
