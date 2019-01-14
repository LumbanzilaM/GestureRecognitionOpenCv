#include "stdafx.h"
#include "HandRegistrationHandler.h"

# define M_PI           3.14159265358979323846  /* pi */


HandRegistrationHandler::HandRegistrationHandler()
{
	squareSize = 20;
	bool lol;
	lol = face_cascade.load(face_cascade_name);
	TrackBars = new MyCamImage("TrackBar");
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
	createTrackbar("Hmin", TrackBars->getWindowName(), &hmin, 256, 0);
	createTrackbar("Hmax", TrackBars->getWindowName(), &hmax, 256, 0);
	createTrackbar("Vmin", TrackBars->getWindowName(), &vmin, 256, 0);
	createTrackbar("Vmax", TrackBars->getWindowName(), &vmax, 256, 0);
	createTrackbar("Smin", TrackBars->getWindowName(), &smin, 256, 0);
	createTrackbar("Smaw", TrackBars->getWindowName(), &smax, 256, 0);

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
	TrackBars->readImage();
	TrackBars->showImage();
	int i = 0;
	for each (Rect rect in rois)
	{
		Mat registrationImgHSV;
		cvtColor(registrationImg(rect), registrationImgHSV, CV_RGB2HSV);
		Scalar median = mean(registrationImgHSV);
		medians.push_back(median);
		double a = 255;
		double b = 94;
		double c = 37;
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
	/*erode(result, result, element, Point(1, 1), 3);*/
	//dilate(result, result, element, Point(1, 1), 2);
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
	//dilate(drawing, drawing, element, Point(1, 1), 3);
	/*imshow("Contours", drawing);*/
	cvtColor(drawing, result, CV_RGB2GRAY);
	GaussianBlur(result, result, Size(3, 3), 1);
	medianBlur(result, result, 7);
	// Get hand contours and draw them on a new black mat to erase noise arround it
	vector<vector<Point>> handContours = FindTwoBiggestContoursBBox(result);
	Mat ret = Mat(result.rows, result.cols, CV_8U, Scalar(0, 0, 0));
	drawContours(ret, handContours, 0, Scalar(255), -1);
	imshow("ret", ret);
	return ret;
}

vector<Mat> HandRegistrationHandler::FindHands(Mat src)
{
	return vector<Mat>();
}

void HandRegistrationHandler::FindPalmCenter(Mat src, Hand *hand, bool draw)
{
	Mat dist;
	Point minLoc, maxLoc;
	int MaxRadiusReachedNb = 0;
	double min, max;
	distanceTransform(src, dist, DIST_L2, 5);
	normalize(dist, dist, 0, 1, NORM_MINMAX);
	minMaxLoc(dist, &min, &max, &minLoc, &maxLoc);
	hand->PalmCenter = maxLoc;
	hand->PalmContour.clear();
	if (draw)
	{
		circle(dist, maxLoc, 10, Scalar(255, 0, 0), -2);
		/*imshow("dist", dist);*/
	}
	size_t circleRadius = 0;
	vector<Point> circleContour;
	try
	{
		while (MaxRadiusReachedNb != 1)
		{
			ellipse2Poly(maxLoc, Size(circleRadius, circleRadius), 0, 0, 360, 1, circleContour);
			if (MaxRadiusReachedNb == 0)
			{
				for each (Point pt in circleContour)
				{
					if (pt.x > 0 && pt.y > 0 && pt.x < src.rows && pt.y < src.cols)
					{
						unsigned char color = src.at<unsigned char>(pt);

						if (color != 255)
						{
							MaxRadiusReachedNb += 1;
							circleRadius = circleRadius * 1.2;
							ellipse2Poly(maxLoc, Size(circleRadius, circleRadius), 0, 0, 360, 1, circleContour);
							break;
						}
					}
				}
			}
			hand->PalmRadius = circleRadius;
			hand->PalmCircle = circleContour;
			circleRadius += 10;
			circleContour.clear();
		}
		FindBoundaries(src, hand);
		FindWristPoints(hand);
		ExtractPalm(src, hand);
		ExtractFingers(src, hand);
	}
	catch (Exception e)
	{
		cout << e.msg;
	}
}


void HandRegistrationHandler::FindBoundaries(Mat src, Hand * hand)
{
	vector<Point> boundPixels;
	Rect roi;
	Mat tmpMat;

	for each (Point pt in hand->PalmCircle)
	{
		//roi = Rect(pt.x - 1, pt.y - 1, 3, 3);
		//tmpMat = src(roi);
		////Check if the pixel nieghboures contain black AND whith pixel
		//int nbBlackPixel = countNonZero(tmpMat);
		//if (nbBlackPixel > 0 && nbBlackPixel < 9)
		//{
		//	hand->PalmContour.push_back(pt);
		//}
		FindClosestBondary(src, hand, pt);
	}
}

Mat HandRegistrationHandler::ExtractPalm(Mat src, Hand* hand)
{
	Mat palm = Mat(src.rows, src.cols, CV_8U, Scalar(0, 0, 0));
	std::vector<std::vector<Point>> contours;
	int lol = hand->PalmContour.size();
	contours.push_back(hand->PalmContour);

	drawContours(palm, contours, 0, Scalar(255), -1);
	imshow("Palm", palm);
	return palm;
}

Mat HandRegistrationHandler::RotateHand(Mat src, Hand *hand)
{
	MathOperation op;
	float angle = 0 - (90 - op.angleBetween(hand->WristPoints[2], hand->PalmCenter, Point(src.rows, hand->PalmCenter.y)));
	hand->RotationAngle = angle;
	Mat matrix = getRotationMatrix2D(hand->PalmCenter, angle, 1);
	Mat rotated;
	warpAffine(src, rotated, matrix, src.size());
	Rect myROI(0, hand[0].WristPoints[2].y, src.rows, src.cols);
	rectangle(rotated, myROI, Scalar(0), -1);
	imshow("rotated hand", rotated);
	return rotated;
}

Mat HandRegistrationHandler::ExtractFingers(Mat src, Hand * hand)
{
	MathOperation op;
	Mat fonly = Mat(src.rows, src.cols, CV_8UC3, Scalar(0));
	vector<vector<Point>> contours;
	Mat fingers = src.clone();
	std::vector<std::vector<Point>> palmContours;
	palmContours.push_back(hand->PalmContour);

	drawContours(fingers, palmContours, 0, Scalar(0), -1);
	
	fingers = RotateHand(fingers, hand);
	
	/*Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1));
	erode(fingers, fingers, element, Point(1, 1), 2);*/
	vector<Vec4i>hierarchy;
	//Find fingers bounding box
	findContours(fingers, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
	hand->FingersDefectsTop.clear();
	hand->FingersDefectsBot.clear();
	hand->FingersContour.clear();
	hand->FingersCenter.clear();
	for (size_t i = 0;i < contours.size();i++)
	{
		if (contours[i].size() > 30)
		{
			Rect bounding = boundingRect(contours[i]);
			RotatedRect rect = minAreaRect(contours[i]);
			hand->FingersRotatedRect.push_back(rect);
			hand->FingersRect.push_back(bounding);
			hand->FingersContour.push_back(contours[i]);

			Point2f rp[4]; 
			rect.points(rp);
			sort(rp, rp + 4, op.comp);
			
			hand->FingersDefectsTop.push_back(Point((rp[0].x + rp[1].x) / 2, (rp[0].y + rp[1].y) / 2));
			hand->FingersDefectsBot.push_back(Point((rp[2].x + rp[3].x) / 2, (rp[2].y + rp[3].y) / 2));
			hand->FingersCenter.push_back(rect.center);
		}
	}

	//Draw finger image
	
	circle(fonly, hand->PalmCenter, 5, Scalar(255,0,0), 3);
	for (int i = 0; i < hand->FingersDefectsBot.size(); i++)
	{
		circle(fonly, hand->FingersDefectsTop[i], 5, Scalar(0,255,0), -2);
		circle(fonly, hand->FingersDefectsTop[i], 5, Scalar(0, 0, 255), -2);
		line(fonly, hand->FingersDefectsBot[i], hand->FingersCenter[i], Scalar(0, 255, 0));
		line(fonly, hand->FingersDefectsBot[i], hand->PalmCenter, Scalar(0, 255, 0));
	}
	drawContours(fonly, hand->FingersContour, -1, Scalar(255), 2);
	imshow("fingers", fonly);
	return fingers;
}

void HandRegistrationHandler::FindClosestBondary(Mat src, Hand* hand, Point centerPoint)
{
	bool MaxRadiusReached = false;
	int circleRadius = 1;
	Mat tmpMat;
	int angle = 1;
	vector<Point> circleContour;
	//imshow("SrcBoundary", src);
	while (angle < 360)
	{
		int x = (cos((angle * M_PI) / 180)) * circleRadius + centerPoint.x;
		int y = (sin((angle * M_PI) / 180)) * circleRadius + centerPoint.y;
		Point pt = Point(x, y);
		if (x > 0 && y > 0 && x < src.rows && y < src.cols)
		{
			unsigned char color = src.at<unsigned char>(pt);

			if (color == 0)
			{
				Rect roi = Rect(pt.x - 1, pt.y - 1, 3, 3);
				tmpMat = src(roi);
				//Check if the pixel nieghboures contain black AND whith pixel
				int nbBlackPixel = countNonZero(tmpMat);
				if (nbBlackPixel > 0 && nbBlackPixel < 9)
				{
					MaxRadiusReached = true;
					hand->PalmContour.push_back(pt);
					break;
				}
			}
		}
		angle += 1;
		if (angle == 360)
		{
			circleRadius += 5;
			angle = 1;
		}
	}
}


void HandRegistrationHandler::FindWristPoints(Hand * hand)
{
	Point wristPoint1;
	Point wristPoint2;
	hand->WristPoints.clear();
	MathOperation helper;
	float maxDist = 0;

	for (int i = hand->PalmContour.size() - 1; i >= 1; i--)
	{
		float distP2P = helper.distanceP2P(hand->PalmContour[i], hand->PalmContour[i - 1]);
		float angle = helper.angleBetween(hand->PalmContour[i], hand->PalmCenter, hand->PalmContour[i - 1]);
		if (distP2P > maxDist && angle > 30 && angle < 60)
		{
			maxDist = distP2P;
			wristPoint1 = hand->PalmContour[i];
			wristPoint2 = hand->PalmContour[i - 1];
		}
	}
	hand->WristPoints.push_back(wristPoint1);
	hand->WristPoints.push_back(wristPoint2);
	//Get the midle point of the wrist line
	hand->WristPoints.push_back((wristPoint1 + wristPoint2)*0.5);
}

void HandRegistrationHandler::CreatePalmCircle(Hand *hand)
{
	vector<Point> palmMask;
	for (int i = 0; i < 360; i++)
	{

		int x = hand->PalmRadius * (cos((i * M_PI) / 180)) + hand->PalmCenter.x;
		int y = hand->PalmRadius * (sin((i * M_PI) / 180)) + hand->PalmCenter.y;
		palmMask.push_back(Point(x, y));
	}
	hand->PalmCircle = palmMask;
}

Mat HandRegistrationHandler::getRegistrationImg()
{
	return registrationImg;
}


void HandRegistrationHandler::AddSquareRegistration()
{
	rois.push_back(Rect(registrationImg.cols - (registrationImg.cols / 5), registrationImg.rows / 5, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols - registrationImg.cols / 7, registrationImg.rows / 3, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols - registrationImg.cols / 4, registrationImg.rows / 3, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols - registrationImg.cols / 5, registrationImg.rows / 2.5, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols - registrationImg.cols / 4.5, registrationImg.rows / 2, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols - registrationImg.cols / 6, registrationImg.rows / 2, squareSize, squareSize));
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
	/*imshow("hsv", srcHSV);*/
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

vector<vector<Point>> HandRegistrationHandler::FindTwoBiggestContoursBBox(Mat src)
{
	vector<vector<Point>> contours;
	vector<Vec4i>hierarchy;
	vector<vector<Point>> ret;
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
	size_t indexOfBiggestContour = -1;
	size_t sizeOfBiggestContour = 0;
	size_t indexOfSecondContour = -1;
	size_t sizeOfSecondContour = 0;

	for (size_t i = 0; i < contours.size(); i++) {

		if (contours[i].size() > sizeOfBiggestContour) {
			sizeOfSecondContour = sizeOfBiggestContour;
			indexOfSecondContour = indexOfBiggestContour;
			sizeOfBiggestContour = contours[i].size();
			indexOfBiggestContour = i;
		}
	}
	if (indexOfBiggestContour != -1)
	ret.push_back(contours[indexOfBiggestContour]);

	//Manage only one hand for now
	/*if (indexOfSecondContour != -1)
	ret.push_back(contours[indexOfSecondContour]);*/
	return ret;
}
