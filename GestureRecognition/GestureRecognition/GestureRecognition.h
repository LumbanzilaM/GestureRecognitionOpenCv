#pragma once
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include "HandRegistrationHandler.h"
#include "GestureLibrary.h"
#include "Communication.h"


class GestureRecognition
{
public:
	GestureRecognition(MyCamImage *camImg);
	~GestureRecognition();
	void setActualImage(MyCamImage *camImg);
	void launchHandRegistration();
	void launchGestureRecognition();

private:
	void prepareHandExtraction();
	void FindGesture();
	float distanceP2P(cv::Point a, cv::Point b);
	float angleBetween(const cv::Point &v1, const cv::Point &origin, const cv::Point &v2);
	void Draw();
	void performHandExtraction();
	void writeGesture(std::string gesture);

	Communication *comManager;
	GestureLibrary gestureLib;
	HandRegistrationHandler registrationHandler;
	MyCamImage *actualCamImg;
	MyCamImage *threshImg;
	vector<vector<cv::Point>> contours;
	vector<Hand> hands;
	clock_t cstart;
	std::chrono::time_point<std::chrono::high_resolution_clock> t_start;
	char a[40];

};

