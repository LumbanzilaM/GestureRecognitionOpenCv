#include "stdafx.h"
#include "GestureRecognition.h"


GestureRecognition::GestureRecognition(MyCamImage *camImg) : actualCamImg(camImg)
{
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
	threshImg = new MyCamImage("TreshImage");
	while(1)
	{
		//actualCamImg->readImage();
		//actualCamImg->showImage();
		flip(actualCamImg->capture, actualCamImg->capture, 1);
		prepareHandExtraction();
		performHandExtraction();
		actualCamImg->showImage();
		if (waitKey(30) == 27) {
			break;
		}
	}
}

void GestureRecognition::prepareHandExtraction()
{
	Mat resImg;
	actualCamImg->capture = resImg;
	actualCamImg->readImage();
	threshImg->capture = registrationHandler.FindHand(actualCamImg->capture);

	contours;
	vector<Vec4i>hierarchy;
	findContours(threshImg->capture, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
	threshImg->showImage();

}

void GestureRecognition::performHandExtraction()
{
	int count = 0;
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
		Point2f rect_point[4];
		vector<RotatedRect>minRect(contours.size());
		vector<Rect> boundRect(contours.size());
		for (size_t i = 0;i < contours.size();i++) {
			if (contourArea(contours[i]) > 4000) {
				convexHull(contours[i], hull[i], false);
				convexityDefects(contours[i], hull[i], defects[i]);
				if (indexOfBiggestContour == i /*|| indexOfSecondContour == i*/) {
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
						if (defects[i][k][3] > 13 * 256) {
							/*   int p_start=defects[i][k][0];   */
							int p_end = defects[i][k][1];
							int p_far = defects[i][k][2];
							int p_start = defects[i][k][0];
							
							/*if (abs(p_end - p_start) < 0.4 * tmp.height)
							{*/
								//defectPoint[i].push_back(contours[i][p_far]);
								circle(actualCamImg->capture, contours[i][p_end], 3, Scalar(0, 255, 0), 2);
								/*circle(actualCamImg->capture, contours[i][p_far], 3, Scalar(255, 0, 0), 2);
								circle(actualCamImg->capture, contours[i][p_start], 3, Scalar(0, 0, 255), 2);*/
								count++;
							//}
						}

					}

					if (count == 2)
						strcpy_s(a, "One");
					else if (count == 3)
						strcpy_s(a, "Two");
					else if (count == 4)
						strcpy_s(a, "Three");
					else if (count == 5)
						strcpy_s(a, "Four");
					else if (count == 6)
						strcpy_s(a, "Five");
					else
						strcpy_s(a, "Welcome !!");

					putText(actualCamImg->capture, a, Point(70, 70), CV_FONT_HERSHEY_SIMPLEX, 3, Scalar(255, 0, 0), 2, 8, false);
					/*drawContours(threshImg->capture, contours, i, Scalar(255, 255, 0), 2, 8, vector<Vec4i>(), 0, Point());
					drawContours(threshImg->capture, hullPoint, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());
					drawContours(actualCamImg->capture, hullPoint, i, Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point());*/
					
					minRect[i].points(rect_point);
					for (size_t k = 0;k < 4;k++) {
						line(actualCamImg->capture, rect_point[k], rect_point[(k + 1) % 4], Scalar(0, 255, 0), 2, 8);
					}
				}
			}
		}
	}
}

void GestureRecognition::writeGesture(String gesture)
{
}
