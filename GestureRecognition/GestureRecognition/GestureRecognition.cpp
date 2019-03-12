#include "stdafx.h"
#include "GestureRecognition.h"

# define M_PI           3.14159265358979323846 

GestureRecognition::GestureRecognition(MyCamImage *camImg) : actualCamImg(camImg)
{
	hands.push_back(Hand());
	hands.push_back(Hand());
}


GestureRecognition::~GestureRecognition()
{
}

void GestureRecognition::setActualImage(MyCamImage *camImg)
{
	actualCamImg = camImg;
}

void GestureRecognition::launchHandRegistration()
{

	registrationHandler.InitHandRegistration(actualCamImg);
	registrationHandler.RegisterHand();
}

void GestureRecognition::launchGestureRecognition()
{
	bool reset = false;
	threshImg = new MyCamImage("TreshImage");
	while (1)
	{
		prepareHandExtraction();
		FindGesture();
		Draw();
		actualCamImg->showImage();
		if (waitKey(30) == 27) {
			break;
		}
		if (waitKey(30) == 'r')
		{
			reset = true;
			break;
		}
	}
	if (reset)
	{
		launchHandRegistration();
	}
}

void GestureRecognition::prepareHandExtraction()
{
	Mat resImg;
	actualCamImg->capture = resImg;
	actualCamImg->readImage();
	registrationHandler.FindAndHideFace(actualCamImg->capture);
	threshImg->capture = registrationHandler.FilterHand(actualCamImg->capture);

	contours;
	vector<Vec4i>hierarchy;
	findContours(threshImg->capture, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
	registrationHandler.FindPalmCenter(threshImg->capture, &hands[0], true);
	threshImg->showImage();

}

void GestureRecognition::FindGesture()
{
	string gesture = gestureLib.FindGesture(hands[0]);
	putText(actualCamImg->capture, gesture, Point(100,100), CV_FONT_HERSHEY_SIMPLEX, 3, Scalar(255, 0, 255), 2, 8, false);
}

float GestureRecognition::distanceP2P(Point a, Point b)
{
	float d = sqrt(fabs(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
	return d;
}

float GestureRecognition::angleBetween(const Point &s, const Point &f, const Point &e)
{
	float l1 = distanceP2P(f, s);
	float l2 = distanceP2P(f, e);
	float dot = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float angle = acos(dot / (l1*l2));
	angle = angle * 180 / M_PI;
	return angle;
}

void GestureRecognition::Draw()
{
	for each(Point pt in hands[0].PalmCircle)
	{
		circle(actualCamImg->capture, pt, 1, Scalar(255, 0, 255), -2);
	}
	polylines(actualCamImg->capture, hands[0].PalmContour, true, Scalar(0, 0, 255), 1);
	hands[0].FingerTips.clear();
	circle(actualCamImg->capture, hands[0].PalmCenter, 5, Scalar(255, 0, 0), -2);
	if (hands[0].WristPoints.size() > 1)
	{
		line(actualCamImg->capture, hands[0].WristPoints[0], hands[0].WristPoints[1], Scalar(0, 255, 255), 10, 8);
	}
	int a = hands[0].RotationAngle;
	stringstream ss;
	ss << a;
	string str = ss.str();
	putText(actualCamImg->capture, str, hands[0].PalmCenter, CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2, 8, false);
}

//Deprecated
void GestureRecognition::performHandExtraction()
{
	int count = 0;
	vector<vector<Point>> selectedContours;
	if (contours.size() > 0) {
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
		// ---------------------------------------------------- end


		vector<vector<int> >hull(contours.size());
		vector<vector<Point> >hullPoint(contours.size());
		vector<vector<Vec4i> >defects(contours.size());
		vector<vector<Point> >defectPoint(contours.size());
		vector<vector<Point> >contours_poly(contours.size());
		vector<Point> fingerMaskUp;
		vector<Point> fingerMaskDown;
		vector<Point> fingerMask;
		Point2f rect_point[4];
		vector<RotatedRect>minRect(contours.size());
		vector<Rect> boundRect(contours.size());
		for (size_t i = 0;i < contours.size();i++) {
			if (contourArea(contours[i]) > 8000) {
				convexHull(contours[i], hull[i], false);
				convexityDefects(contours[i], hull[i], defects[i]);
				if (indexOfBiggestContour == i || indexOfSecondContour == i) {
					/*selectedContours.push_back(contours[i]);*/
					//drawContours(actualCamImg->capture, contours, i, Scalar(255, 255, 255), 1, 8);
					//drawContours(actualCamImg->capture, contours[i], i, Scalar(0, 0, 255), 1, 8);
					minRect[i] = minAreaRect(contours[i]);
					for (size_t k = 0;k < hull[i].size();k++) {
						int ind = hull[i][k];
						hullPoint[i].push_back(contours[i][ind]);
					}
					count = 0;
					approxPolyDP(contours[i], contours_poly[i], 3, false);
					boundRect[i] = boundingRect(contours_poly[i]);
					rectangle(actualCamImg->capture, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
					Rect tmp = boundRect[i];
					int heigt = tmp.height;
					int width = tmp.width;
					for (size_t k = 0;k < defects[i].size();k++) {

						/*   int p_start=defects[i][k][0];   */
						int p_end = defects[i][k][1];
						int p_far = defects[i][k][2];
						int p_start = defects[i][k][0];


						int angle = angleBetween(contours[i][p_start], contours[i][p_far], contours[i][p_end]);
						/*circle(actualCamImg->capture, contours[i][p_end], 3, Scalar(0, 255, 0), 2);
						circle(actualCamImg->capture, contours[i][p_start], 3, Scalar(0, 0, 255), 2);*/
						if (angle < 80 && defects[i][k][3] > 13 * 256)
						{
							/*int p_endlast = p_end;
							int p_farlast = p_far;
							int p_startlast = p_start;
							if (k > 0)
							{
								p_endlast = defects[i][k - 1][1];
								p_farlast = defects[i][k - 1][2];
								p_startlast = defects[i][k - 1][0];
							}*/
							/*if (hands[0].FingerTips.size() == 0)
							{
								hands[0].FingerTips.push_back(contours[i][p_end]);
							}
							else
							{*/
							hands[0].FingerTips.push_back(contours[i][p_start]);
							hands[0].FingerTips.push_back(contours[i][p_end]);
							/*fingerMaskUp.push_back(contours[i][p_startlast]);
							fingerMaskUp.push_back(contours[i][p_endlast]);*/
							fingerMaskUp.push_back(contours[i][p_start]);
							fingerMaskUp.push_back(contours[i][p_end]);
							//fingerMaskUp.push_back(contours[i][p_s]);
							fingerMaskDown.push_back(contours[i][p_far]);
							//fingerMaskDown.push_back(contours[i][p_farlast]);
							//}
							/*if (abs(p_end - p_start) < 0.4 * tmp.height)
							{*/
							//defectPoint[i].push_back(contours[i][p_far]);

							//putText(actualCamImg->capture, std::to_string(k), contours[i][p_end], CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2, 8, false);
							/*circle(actualCamImg->capture, contours[i][p_far], 3, Scalar(255, 0, 0), 2);
							circle(actualCamImg->capture, contours[i][p_start], 3, Scalar(0, 0, 255), 2);*/
							if (count < 4)
							{
								count++;
							}
							//}
						}

					}
					reverse(fingerMaskDown.begin(), fingerMaskDown.end());
					fingerMask.reserve(fingerMaskDown.size() + fingerMaskUp.size());
					fingerMask.insert(fingerMask.end(), fingerMaskUp.begin(), fingerMaskUp.end());
					fingerMask.insert(fingerMask.end(), fingerMaskDown.begin(), fingerMaskDown.end());
					//polylines(actualCamImg->capture, fingerMask, true, Scalar(0, 0, 255), 5);
					putText(actualCamImg->capture, std::to_string(count + 1), Point((boundRect[i].height + boundRect[i].x) / 2, (boundRect[i].width + boundRect[i].y) / 2), CV_FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 0, 255), 2, 8, false);
					if (count == 2)
						strcpy_s(a, "One");
					else if (count == 3)
						strcpy_s(a, "Two");
					else if (count == 4)
						strcpy_s(a, "Three");
					else if (count == 5)
						strcpy_s(a, "Four");
					else if (count >= 6)
						strcpy_s(a, "Five");


					//putText(actualCamImg->capture, a, Point(70, 70), CV_FONT_HERSHEY_SIMPLEX, 3, Scalar(255, 0, 0), 2, 8, false);
					/*drawContours(threshImg->capture, contours, i, Scalar(255, 255, 0), 2, 8, vector<Vec4i>(), 0, Point());
					drawContours(threshImg->capture, hullPoint, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());
					drawContours(actualCamImg->capture, hullPoint, i, Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point());*/
					/*minRect[i].points(rect_point);*/


					/*for (size_t k = 0;k < 4;k++) {
						line(actualCamImg->capture, rect_point[k], rect_point[(k + 1) % 4], Scalar(0, 255, 0), 2, 8);
					}*/
					//for each (Point pt in hands[0].FingerTips)
					//{
					//	circle(actualCamImg->capture, pt, 5, Scalar(255, 0, 0), -2);
					//	/*line(actualCamImg->capture, hands[0].PalmCenter, pt, Scalar(0, 255, 0), 2, 8);*/
					//}
					for each(Point pt in hands[0].PalmCircle)
					{
						circle(actualCamImg->capture, pt, 1, Scalar(255, 0, 255), -2);
					}
					/*for each(Point pt in hands[0].PalmContour)
					{
						circle(actualCamImg->capture, pt, 1, Scalar(0, 255, 0), -2);
					}*/
					
					polylines(actualCamImg->capture, hands[0].PalmContour, true, Scalar(0, 0, 255), 1);
					hands[0].FingerTips.clear();
					circle(actualCamImg->capture, hands[0].PalmCenter, 5, Scalar(255, 0, 0), -2);
					if (hands[0].WristPoints.size() > 1)
					{
						line(actualCamImg->capture, hands[0].WristPoints[0], hands[0].WristPoints[1], Scalar(0, 255, 255), 10, 8);
					}
					int a = hands[0].RotationAngle;
					stringstream ss;
					ss << a;
					string str = ss.str();
					putText(actualCamImg->capture, str, hands[0].PalmCenter, CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2, 8, false);
					//circle(actualCamImg->capture, hands[0].PalmCenter, hands[0].PalmRadius, Scalar(0, 255, 0), 2);
				}
			}
		}
	}
}

void GestureRecognition::writeGesture(String gesture)
{
}
