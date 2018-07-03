#include "stdafx.h"
//#include "opencv2/core/utility.hpp"
//#include "opencv2/core/ocl.hpp"
//#include "opencv2/video/tracking.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/videoio.hpp"
//#include "opencv2/highgui.hpp"
//
//#include <iostream>
//#include <cctype>
//
//static cv::UMat image;
//static bool backprojMode = false;
//static bool selectObject = false;
//static int trackObject = 0;
//static bool showHist = true;
//static cv::Rect selection;
//static int vmin = 10, vmax = 256, smin = 30;
//
//static void onMouse(int event, int x, int y, int, void*)
//{
//	static cv::Point origin;
//
//	if (selectObject)
//	{
//		selection.x = std::min(x, origin.x);
//		selection.y = std::min(y, origin.y);
//		selection.width = std::abs(x - origin.x);
//		selection.height = std::abs(y - origin.y);
//
//		selection &= cv::Rect(0, 0, image.cols, image.rows);
//	}
//
//	switch (event)
//	{
//	case cv::EVENT_LBUTTONDOWN:
//		origin = cv::Point(x, y);
//		selection = cv::Rect(x, y, 0, 0);
//		selectObject = true;
//		break;
//	case cv::EVENT_LBUTTONUP:
//		selectObject = false;
//		if (selection.width > 0 && selection.height > 0)
//			trackObject = -1;
//		break;
//	default:
//		break;
//	}
//}
//
//static void help()
//{
//	std::cout << "\nThis is a demo that shows mean-shift based tracking using Transparent API\n"
//		"You select a color objects such as your face and it tracks it.\n"
//		"This reads from video camera (0 by default, or the camera number the user enters\n"
//		"Usage: \n"
//		"   ./camshiftdemo [camera number]\n";
//
//	std::cout << "\n\nHot keys: \n"
//		"\tESC - quit the program\n"
//		"\ts - stop the tracking\n"
//		"\tb - switch to/from backprojection view\n"
//		"\th - show/hide object histogram\n"
//		"\tp - pause video\n"
//		"\tc - use OpenCL or not\n"
//		"To initialize tracking, select the object with mouse\n";
//}
//
//int main(int argc, const char ** argv)
//{
//	help();
//
//	cv::VideoCapture cap;
//	cv::Rect trackWindow;
//	int hsize = 16;
//	float hranges[2] = { 0, 180 };
//
//	const char * const keys = { "{@camera_number| 0 | camera number}" };
//	cv::CommandLineParser parser(argc, argv, keys);
//	int camNum = parser.get<int>(0);
//
//	cap.open(camNum);
//
//	if (!cap.isOpened())
//	{
//		help();
//
//		std::cout << "***Could not initialize capturing...***\n";
//		std::cout << "Current parameter's value: \n";
//		parser.printMessage();
//
//		return EXIT_FAILURE;
//	}
//
//	cv::namedWindow("Histogram", cv::WINDOW_NORMAL);
//	cv::namedWindow("CamShift Demo", cv::WINDOW_NORMAL);
//	cv::setMouseCallback("CamShift Demo", onMouse);
//	cv::createTrackbar("Vmin", "CamShift Demo", &vmin, 256);
//	cv::createTrackbar("Vmax", "CamShift Demo", &vmax, 256);
//	cv::createTrackbar("Smin", "CamShift Demo", &smin, 256);
//
//	cv::Mat frame, histimg(200, 320, CV_8UC3, cv::Scalar::all(0));
//	cv::UMat hsv, hist, hue, mask, backproj;
//	bool paused = false;
//
//	for (; ; )
//	{
//		if (!paused)
//		{
//			cap >> frame;
//			if (frame.empty())
//				break;
//		}
//
//		frame.copyTo(image);
//
//		if (!paused)
//		{
//			cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
//
//			if (trackObject)
//			{
//				int _vmin = vmin, _vmax = vmax;
//
//				cv::inRange(hsv, cv::Scalar(0, smin, std::min(_vmin, _vmax)),
//					cv::Scalar(180, 256, std::max(_vmin, _vmax)), mask);
//
//				int fromTo[2] = { 0,0 };
//				hue.create(hsv.size(), hsv.depth());
//				cv::mixChannels(std::vector<cv::UMat>(1, hsv), std::vector<cv::UMat>(1, hue), fromTo, 1);
//
//				if (trackObject < 0)
//				{
//					cv::UMat roi(hue, selection), maskroi(mask, selection);
//					cv::calcHist(std::vector<cv::Mat>(1, roi.getMat(cv::ACCESS_READ)), std::vector<int>(1, 0),
//						maskroi, hist, std::vector<int>(1, hsize), std::vector<float>(hranges, hranges + 2));
//					cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);
//
//					trackWindow = selection;
//					trackObject = 1;
//
//					histimg = cv::Scalar::all(0);
//					int binW = histimg.cols / hsize;
//					cv::Mat buf(1, hsize, CV_8UC3);
//					for (int i = 0; i < hsize; i++)
//						buf.at<cv::Vec3b>(i) = cv::Vec3b(cv::saturate_cast<uchar>(i*180. / hsize), 255, 255);
//					cv::cvtColor(buf, buf, cv::COLOR_HSV2BGR);
//
//					{
//						cv::Mat _hist = hist.getMat(cv::ACCESS_READ);
//						for (int i = 0; i < hsize; i++)
//						{
//							int val = cv::saturate_cast<int>(_hist.at<float>(i)*histimg.rows / 255);
//							cv::rectangle(histimg, cv::Point(i*binW, histimg.rows),
//								cv::Point((i + 1)*binW, histimg.rows - val),
//								cv::Scalar(buf.at<cv::Vec3b>(i)), -1, 8);
//						}
//					}
//				}
//
//				cv::calcBackProject(std::vector<cv::UMat>(1, hue), std::vector<int>(1, 0), hist, backproj,
//					std::vector<float>(hranges, hranges + 2), 1.0);
//				cv::bitwise_and(backproj, mask, backproj);
//
//				cv::RotatedRect trackBox = cv::CamShift(backproj, trackWindow,
//					cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1));
//				if (trackWindow.area() <= 1)
//				{
//					int cols = backproj.cols, rows = backproj.rows, r = (std::min(cols, rows) + 5) / 6;
//					trackWindow = cv::Rect(trackWindow.x - r, trackWindow.y - r,
//						trackWindow.x + r, trackWindow.y + r) &
//						cv::Rect(0, 0, cols, rows);
//				}
//
//				if (backprojMode)
//					cv::cvtColor(backproj, image, cv::COLOR_GRAY2BGR);
//
//				{
//					cv::Mat _image = image.getMat(cv::ACCESS_RW);
//					cv::ellipse(_image, trackBox, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
//				}
//			}
//		}
//		else if (trackObject < 0)
//			paused = false;
//
//		if (selectObject && selection.width > 0 && selection.height > 0)
//		{
//			cv::UMat roi(image, selection);
//			cv::bitwise_not(roi, roi);
//		}
//
//		cv::imshow("CamShift Demo", image);
//		if (showHist)
//			cv::imshow("Histogram", histimg);
//
//		char c = (char)cv::waitKey(10);
//		if (c == 27)
//			break;
//
//		switch (c)
//		{
//		case 'b':
//			backprojMode = !backprojMode;
//			break;
//		case 't':
//			trackObject = 0;
//			histimg = cv::Scalar::all(0);
//			break;
//		case 'h':
//			showHist = !showHist;
//			if (!showHist)
//				cv::destroyWindow("Histogram");
//			else
//				cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
//			break;
//		case 'p':
//			paused = !paused;
//			break;
//		case 'c':
//			cv::ocl::setUseOpenCL(!cv::ocl::useOpenCL());
//		default:
//			break;
//		}
//	}
//
//	return EXIT_SUCCESS;
//}

#include "HandRegistrationHandler.h"
#include "GestureRecognition.h"
#include "MyCamImage.h"
//opencv

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<opencv2/opencv.hpp>


//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
using namespace cv;
using namespace std;
// Global variables
Mat frame; //current frame
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
int keyboard; //input from keyboard
void processVideo();
int SimpleHandDetection();
void BaseCamLoop();

int main(int argc, char* argv[])
{
	//SimpleHandDetection();
	//BaseCamLoop();
	MyCamImage output("OutputImage");
	GestureRecognition recognition(&output);
	recognition.launchHandRegistration();
	recognition.launchGestureRecognition();

	return 0;
}

void BaseCamLoop()
{
	VideoCapture cam(0);
	//cam.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	//cam.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	HandRegistrationHandler handRegistrationHandler;
	MyCamImage camImg("OutputImage");
	handRegistrationHandler.InitHandRegistration(&camImg);
	handRegistrationHandler.RegisterHand();
	while (1)
	{
		camImg.readImage();
		camImg.showImage();
		if (waitKey(30) == 27) {
			break;
		}
	}
}
//
//int SimpleHandDetection()
//{
//	VideoCapture cam(0);
//	HandRegistrationHandler handRegistrationHandler;
//	if (!cam.isOpened()) {
//		cout << "ERROR not opened " << endl;
//		return -1;
//	}
//	Mat img;
//	Mat img_threshold;
//	Mat img_gray;
//	Mat img_roi;
//	pMOG2 = createBackgroundSubtractorMOG2();
//	namedWindow("Original_image", CV_WINDOW_AUTOSIZE);
//	namedWindow("Gray_image", CV_WINDOW_AUTOSIZE);
//	namedWindow("Thresholded_image", CV_WINDOW_AUTOSIZE);
//	namedWindow("ROI", CV_WINDOW_AUTOSIZE);
//	char a[40];
//	int count = 0;
//	while (1) {
//		bool b = cam.read(img);
//		if (!b) {
//			cout << "ERROR : cannot read" << endl;
//			return -1;
//		}
//
//		//Init the background extraction
//		Rect roi(340, 100, 270, 270);
//		img_roi = img(roi);
//		//pMOG2->apply(img, img_gray);
//		cvtColor(img_roi, img_gray, CV_RGB2GRAY);
//
//		GaussianBlur(img_gray, img_gray, Size(19, 19), 0.0, 0);
//		threshold(img_gray, img_threshold, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);
//		//bitwise_not(img_threshold, img_threshold);
//		vector<vector<Point> >contours;
//		vector<Vec4i>hierarchy;
//		findContours(img_threshold, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
//		// ---------------------------------------------------- end
//
//		//Find de biggest contour to locate the hand
//		if (contours.size() > 0) {
//			size_t indexOfBiggestContour = -1;
//			size_t sizeOfBiggestContour = 0;
//
//			for (size_t i = 0; i < contours.size(); i++) {
//				if (contours[i].size() > sizeOfBiggestContour) {
//					sizeOfBiggestContour = contours[i].size();
//					indexOfBiggestContour = i;
//				}
//			}
//			// ---------------------------------------------------- end
//
//
//			vector<vector<int> >hull(contours.size());
//			vector<vector<Point> >hullPoint(contours.size());
//			vector<vector<Vec4i> >defects(contours.size());
//			vector<vector<Point> >defectPoint(contours.size());
//			vector<vector<Point> >contours_poly(contours.size());
//			Point2f rect_point[4];
//			vector<RotatedRect>minRect(contours.size());
//			vector<Rect> boundRect(contours.size());
//			for (size_t i = 0;i < contours.size();i++) {
//				if (contourArea(contours[i]) > 5000) {
//					convexHull(contours[i], hull[i], false);
//					convexityDefects(contours[i], hull[i], defects[i]);
//					if (indexOfBiggestContour == i) {
//						minRect[i] = minAreaRect(contours[i]);
//						for (size_t k = 0;k < hull[i].size();k++) {
//							int ind = hull[i][k];
//							hullPoint[i].push_back(contours[i][ind]);
//						}
//						count = 0;
//
//						for (size_t k = 0;k < defects[i].size();k++) {
//							if (defects[i][k][3] > 13 * 256) {
//								/*   int p_start=defects[i][k][0];   */
//								int p_end = defects[i][k][1];
//								int p_far = defects[i][k][2];
//								defectPoint[i].push_back(contours[i][p_far]);
//								circle(img_roi, contours[i][p_end], 3, Scalar(0, 255, 0), 2);
//								count++;
//							}
//
//						}
//
//						if (count == 1)
//							strcpy_s(a, "One");
//						else if (count == 2)
//							strcpy_s(a, "Two");
//						else if (count == 3)
//							strcpy_s(a, "Three");
//						else if (count == 4)
//							strcpy_s(a, "Four");
//						else if (count == 5)
//							strcpy_s(a, "Five");
//						else
//							strcpy_s(a, "Welcome !!");
//
//						putText(img, a, Point(70, 70), CV_FONT_HERSHEY_SIMPLEX, 3, Scalar(255, 0, 0), 2, 8, false);
//						drawContours(img_threshold, contours, i, Scalar(255, 255, 0), 2, 8, vector<Vec4i>(), 0, Point());
//						drawContours(img_threshold, hullPoint, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());
//						drawContours(img_roi, hullPoint, i, Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point());
//						approxPolyDP(contours[i], contours_poly[i], 3, false);
//						boundRect[i] = boundingRect(contours_poly[i]);
//						rectangle(img_roi, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
//						minRect[i].points(rect_point);
//						for (size_t k = 0;k < 4;k++) {
//							line(img_roi, rect_point[k], rect_point[(k + 1) % 4], Scalar(0, 255, 0), 2, 8);
//						}
//
//					}
//				}
//			}
//			imshow("Original_image", img);
//			imshow("Gray_image", img_gray);
//			imshow("Thresholded_image", img_threshold);
//			imshow("ROI", img_roi);
//			if (waitKey(30) == 27) {
//				break;
//			}
//
//		}
//	}
//}



//#include "opencv2/imgproc.hpp"
//#include "opencv2/highgui.hpp"
//#include <iostream>
//using namespace cv;
//const int max_value_H = 360 / 2;
//const int max_value = 255;
//const String window_capture_name = "Video Capture";
//const String window_detection_name = "Object Detection";
//int low_H = 0, low_S = 0, low_V = 0;
//int high_H = max_value_H, high_S = max_value, high_V = max_value;
//static void on_low_H_thresh_trackbar(int, void *)
//{
//	low_H = min(high_H - 1, low_H);
//	setTrackbarPos("Low H", window_detection_name, low_H);
//}
//static void on_high_H_thresh_trackbar(int, void *)
//{
//	high_H = max(high_H, low_H + 1);
//	setTrackbarPos("High H", window_detection_name, high_H);
//}
//static void on_low_S_thresh_trackbar(int, void *)
//{
//	low_S = min(high_S - 1, low_S);
//	setTrackbarPos("Low S", window_detection_name, low_S);
//}
//static void on_high_S_thresh_trackbar(int, void *)
//{
//	high_S = max(high_S, low_S + 1);
//	setTrackbarPos("High S", window_detection_name, high_S);
//}
//static void on_low_V_thresh_trackbar(int, void *)
//{
//	low_V = min(high_V - 1, low_V);
//	setTrackbarPos("Low V", window_detection_name, low_V);
//}
//static void on_high_V_thresh_trackbar(int, void *)
//{
//	high_V = max(high_V, low_V + 1);
//	setTrackbarPos("High V", window_detection_name, high_V);
//}
//int main(int argc, char* argv[])
//{
//	VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
//	namedWindow(window_capture_name);
//	namedWindow(window_detection_name);
//	// Trackbars to set thresholds for HSV values
//	createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
//	createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
//	createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
//	createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
//	createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
//	createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
//	Mat frame, frame_HSV, frame_threshold;
//	while (true) {
//		cap >> frame;
//		if (frame.empty())
//		{
//			break;
//		}
//		// Convert from BGR to HSV colorspace
//		cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
//		// Detect the object based on HSV Range Values
//		inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
//		// Show the frames
//		imshow(window_capture_name, frame);
//		imshow(window_detection_name, frame_threshold);
//		char key = (char)waitKey(30);
//		if (key == 'q' || key == 27)
//		{
//			break;
//		}
//	}
//	return 0;
//}

