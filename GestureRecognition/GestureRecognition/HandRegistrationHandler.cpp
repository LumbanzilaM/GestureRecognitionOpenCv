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
	hmin = 16;
	hmax = 16;
	vmin = 0;
	vmax = 255;
	smin = 26;
	smax = 26;
	cv::createTrackbar("Hmin", TrackBars->getWindowName(), &hmin, 256, 0);
	cv::createTrackbar("Hmax", TrackBars->getWindowName(), &hmax, 256, 0);
	cv::createTrackbar("Vmin", TrackBars->getWindowName(), &vmin, 256, 0);
	cv::createTrackbar("Vmax", TrackBars->getWindowName(), &vmax, 256, 0);
	cv::createTrackbar("Smin", TrackBars->getWindowName(), &smin, 256, 0);
	cv::createTrackbar("Smaw", TrackBars->getWindowName(), &smax, 256, 0);

	camImg->readImage(&registrationImg);
	AddSquareRegistration();
	while (1) {

		camImg->readImage(&registrationImg);

		for each (cv::Rect rect in rois)
		{
			rectangle(registrationImg, rect, cv::Scalar(255, 0, 0), 2);
		}

		camImg->showImage(&registrationImg);

		if (cv::waitKey(30) == 'n')
		{
			camImg->readImage(&registrationImg);
			break;
		}
	}
}

void HandRegistrationHandler::SetBackground(cv::Rect handRect)
{
}


void HandRegistrationHandler::RegisterHand()
{
	TrackBars->readImage();
	TrackBars->showImage();
	int i = 0;
	for each (cv::Rect rect in rois)
	{
		cv::Mat registrationImgHSV;
		cv::cvtColor(registrationImg(rect), registrationImgHSV, CV_RGB2HSV);
		cv::Scalar median = mean(registrationImgHSV);
		medians.push_back(median);
		double a = 255;
		double b = 94;
		double c = 37;
		i++;
	}
	//FindAndHideFace(registrationImg);
	//FindHand(registrationImg);
}

void HandRegistrationHandler::FindAndHideFace(cv::Mat &src)
{
	cv::Mat srcGray;
	std::vector<cv::Rect> faces;
	cvtColor(src, srcGray, CV_RGB2GRAY);
	face_cascade.detectMultiScale(srcGray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(100, 100));
	size_t lol = faces.size();
	for each (cv::Rect face in faces)
	{
		rectangle(src, face, cv::Scalar(0), -2);
		Face = face;
	}
	//rectangle(src, Face, Scalar(0), -2);
}


cv::Mat HandRegistrationHandler::FilterHand(cv::Mat src)
{
	cv::Mat result;
	cv::Mat srcHSV;
	cv::Mat canny;
	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3), cv::Point(-1, -1));

	//Erase noise with erode, and find contours
	result = Filtering(src);
	erode(result, result, element, cv::Point(1, 1), 3);
	Canny(result, canny, 0, 255);
	dilate(canny, canny, element, cv::Point(1, 1), 5);
	imshow("Afeter canny", canny);
	dilate(result, result, element, cv::Point(1, 1), 5);
	imshow("Afeter dilate", result);
	findContours(result, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	//Draw filled contours
	cv::Mat drawing = cv::Mat::zeros(result.size(), CV_8UC3);
	vector<vector<cv::Point>> hull(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(255, 255, 255);
		convexHull(contours[i], hull[i]);
		drawContours(drawing, contours, i, color, cv::FILLED, 8, hierarchy, 0, cv::Point());
		//drawContours(drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	}
	//Dilate result for counterbalance the previous erode
	//dilate(drawing, drawing, element, Point(1, 1), 3);
	/*imshow("Contours", drawing);*/
	cvtColor(drawing, result, CV_RGB2GRAY);
	GaussianBlur(result, result, cv::Size(3, 3), 1);
	medianBlur(result, result, 7);
	// Get hand contours and draw them on a new black mat to erase noise arround it
	vector<vector<cv::Point>> handContours = FindTwoBiggestContoursBBox(result);
	cv::Mat ret = cv::Mat(result.rows, result.cols, CV_8U, cv::Scalar(0, 0, 0));
	drawContours(ret, handContours, 0, cv::Scalar(255), -1);
	imshow("ret", ret);
	return ret;
}

vector<cv::Mat> HandRegistrationHandler::FindHands(cv::Mat src)
{
	return vector<cv::Mat>();
}

void HandRegistrationHandler::FindPalmCenter(cv::Mat src, Hand *hand, bool draw)
{
	cv::Mat dist;
	cv::Point minLoc, maxLoc;
	int MaxRadiusReachedNb = 0;
	double min, max;
	distanceTransform(src, dist, cv::DIST_L2, 5);
	normalize(dist, dist, 0, 1, cv::NORM_MINMAX);
	minMaxLoc(dist, &min, &max, &minLoc, &maxLoc);
	hand->PalmCenter = maxLoc;
	hand->PalmContour.clear();
	if (draw)
	{
		circle(dist, maxLoc, 10, cv::Scalar(255, 0, 0), -2);
		/*imshow("dist", dist);*/
	}
	size_t circleRadius = 0;
	vector<cv::Point> circleContour;
	try
	{
		while (MaxRadiusReachedNb != 1)
		{
			cv::ellipse2Poly(maxLoc, cv::Size(circleRadius, circleRadius), 0, 0, 360, 1, circleContour);
			if (MaxRadiusReachedNb == 0)
			{
				for each (cv::Point pt in circleContour)
				{
					if (pt.x > 0 && pt.y > 0 && pt.x < src.rows && pt.y < src.cols)
					{
						unsigned char color = src.at<unsigned char>(pt);

						if (color != 255)
						{
							MaxRadiusReachedNb += 1;
							circleRadius = circleRadius * 1.2;
							ellipse2Poly(maxLoc, cv::Size(circleRadius, circleRadius), 0, 0, 360, 1, circleContour);
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
	catch (cv::Exception e)
	{
		cout << e.msg;
	}
}


void HandRegistrationHandler::FindBoundaries(cv::Mat src, Hand * hand)
{
	vector<cv::Point> boundPixels;
	cv::Rect roi;
	cv::Mat tmpMat;

	for each (cv::Point pt in hand->PalmCircle)
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

cv::Mat HandRegistrationHandler::ExtractPalm(cv::Mat src, Hand* hand)
{
	cv::Mat palm = cv::Mat(src.rows, src.cols, CV_8U, cv::Scalar(0, 0, 0));
	std::vector<std::vector<cv::Point>> contours;
	int lol = hand->PalmContour.size();
	contours.push_back(hand->PalmContour);

	drawContours(palm, contours, 0, cv::Scalar(255), -1);
	imshow("Palm", palm);
	return palm;
}

cv::Mat HandRegistrationHandler::RotateHand(cv::Mat src, Hand *hand)
{
	MathOperation op;
	float angle = 0 - (90 - op.angleBetween(hand->WristPoints[2], hand->PalmCenter, cv::Point(src.rows, hand->PalmCenter.y)));
	hand->RotationAngle = angle;
	cv::Mat matrix = getRotationMatrix2D(hand->PalmCenter, angle, 1);
	cv::Mat rotated;
	warpAffine(src, rotated, matrix, src.size());
	cv::Rect myROI(0, hand[0].WristPoints[2].y, src.rows, src.cols);
	rectangle(rotated, myROI, cv::Scalar(0), -1);
	imshow("rotated hand", rotated);
	return rotated;
}

cv::Mat HandRegistrationHandler::ExtractFingers(cv::Mat src, Hand * hand)
{
	MathOperation op;
	cv::Mat fonly = cv::Mat(src.rows, src.cols, CV_8UC3, cv::Scalar(0));
	vector<vector<cv::Point>> contours;
	cv::Mat fingers = src.clone();
	std::vector<std::vector<cv::Point>> palmContours;
	palmContours.push_back(hand->PalmContour);

	drawContours(fingers, palmContours, 0, cv::Scalar(0), -1);
	
	fingers = RotateHand(fingers, hand);
	
	/*Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1));
	erode(fingers, fingers, element, Point(1, 1), 2);*/
	vector<cv::Vec4i>hierarchy;
	//Find fingers bounding box
	findContours(fingers, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point());
	hand->FingersDefectsTop.clear();
	hand->FingersDefectsBot.clear();
	hand->FingersContour.clear();
	hand->FingersCenter.clear();
	for (size_t i = 0;i < contours.size();i++)
	{
		if (contours[i].size() > 50)
		{
			cv::Rect bounding = boundingRect(contours[i]);
			cv::RotatedRect rect = minAreaRect(contours[i]);
			hand->FingersRotatedRect.push_back(rect);
			hand->FingersRect.push_back(bounding);
			hand->FingersContour.push_back(contours[i]);

			cv::Point2f rp[4];
			rect.points(rp);
			sort(rp, rp + 4, op.comp);
			
			hand->FingersDefectsTop.push_back(cv::Point((rp[0].x + rp[1].x) / 2, (rp[0].y + rp[1].y) / 2));
			hand->FingersDefectsBot.push_back(cv::Point((rp[2].x + rp[3].x) / 2, (rp[2].y + rp[3].y) / 2));
			hand->FingersCenter.push_back(rect.center);
		}
	}

	if (hand->FingersDefectsBot.size() == 1)
	{
		hand->NbFingerUp = 1;
		hand->ThumbIdx = 0;
	}
	else
	{
		hand->NbFingerUp = hand->FingersDefectsBot.size();
	}

	//Draw finger image
	
	circle(fonly, hand->PalmCenter, 5, cv::Scalar(255,0,0), 3);
	for (int i = 0; i < hand->FingersDefectsBot.size(); i++)
	{
		circle(fonly, hand->FingersDefectsTop[i], 5, cv::Scalar(0,255,0), -2);
		circle(fonly, hand->FingersDefectsTop[i], 5, cv::Scalar(0, 0, 255), -2);
		line(fonly, hand->FingersDefectsBot[i], hand->FingersCenter[i], cv::Scalar(0, 255, 0));
		line(fonly, hand->FingersDefectsBot[i], hand->PalmCenter, cv::Scalar(0, 255, 0));
	}
	drawContours(fonly, hand->FingersContour, -1, cv::Scalar(255), 2);
	imshow("fingers", fonly);
	return fingers;
}

void HandRegistrationHandler::FindClosestBondary(cv::Mat src, Hand* hand, cv::Point centerPoint)
{
	bool MaxRadiusReached = false;
	int circleRadius = 1;
	cv::Mat tmpMat;
	int angle = 1;
	vector<cv::Point> circleContour;
	//imshow("SrcBoundary", src);
	while (angle < 360)
	{
		int x = (cos((angle * M_PI) / 180)) * circleRadius + centerPoint.x;
		int y = (sin((angle * M_PI) / 180)) * circleRadius + centerPoint.y;
		cv::Point pt = cv::Point(x, y);
		if (x > 0 && y > 0 && x < src.rows && y < src.cols)
		{
			unsigned char color = src.at<unsigned char>(pt);

			if (color == 0)
			{
				cv::Rect roi = cv::Rect(pt.x - 1, pt.y - 1, 3, 3);
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
	cv::Point wristPoint1;
	cv::Point wristPoint2;
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
	vector<cv::Point> palmMask;
	for (int i = 0; i < 360; i++)
	{

		int x = hand->PalmRadius * (cos((i * M_PI) / 180)) + hand->PalmCenter.x;
		int y = hand->PalmRadius * (sin((i * M_PI) / 180)) + hand->PalmCenter.y;
		palmMask.push_back(cv::Point(x, y));
	}
	hand->PalmCircle = palmMask;
}

cv::Mat HandRegistrationHandler::getRegistrationImg()
{
	return registrationImg;
}


void HandRegistrationHandler::AddSquareRegistration()
{
	rois.push_back(cv::Rect(registrationImg.cols - (registrationImg.cols / 5), registrationImg.rows / 5, squareSize, squareSize));
	rois.push_back(cv::Rect(registrationImg.cols - registrationImg.cols / 7, registrationImg.rows / 3, squareSize, squareSize));
	rois.push_back(cv::Rect(registrationImg.cols - registrationImg.cols / 4, registrationImg.rows / 3, squareSize, squareSize));
	rois.push_back(cv::Rect(registrationImg.cols - registrationImg.cols / 5, registrationImg.rows / 2.5, squareSize, squareSize));
	rois.push_back(cv::Rect(registrationImg.cols - registrationImg.cols / 4.5, registrationImg.rows / 2, squareSize, squareSize));
	rois.push_back(cv::Rect(registrationImg.cols - registrationImg.cols / 6, registrationImg.rows / 2, squareSize, squareSize));
	;
}

void HandRegistrationHandler::InitFaceDetection()
{

}


//Use the hand hsv median values to perform tresholds and add them
cv::Mat HandRegistrationHandler::Filtering(cv::Mat src)
{
	cv::Mat srcHSV;
	vector<cv::Mat> thresholds;
	cv::Mat out;
	cvtColor(src, srcHSV, CV_RGB2HSV);
	/*imshow("hsv", srcHSV);*/
	for each (cv::Scalar median in medians)
	{
		cv::Scalar lower(median[0] - hmin, median[1] - smin, median[2] - vmin);
		cv::Scalar upper(median[0] + hmax, median[1] + smax, median[2] + vmax);
		cv::Mat thresholdImg;
		inRange(srcHSV, lower, upper, thresholdImg);
		cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5), cv::Point(-1, -1));
		morphologyEx(thresholdImg, thresholdImg, cv::MORPH_OPEN, element);
		thresholds.push_back(thresholdImg);
	}

	out = thresholds[0];
	for each (cv::Mat thresholdImg in thresholds)
	{
		out += thresholdImg;
	}
	imshow("result before manipulation", out);
	thresholds.clear();
	return out;
}

vector<vector<cv::Point>> HandRegistrationHandler::FindTwoBiggestContoursBBox(cv::Mat src)
{
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i>hierarchy;
	vector<vector<cv::Point>> ret;
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point());
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
