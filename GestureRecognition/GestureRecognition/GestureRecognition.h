#pragma once
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "HandRegistrationHandler.h"
#include "GestureLibrary.h"


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
	float distanceP2P(Point a, Point b);
	float angleBetween(const Point &v1, const Point &origin, const Point &v2);
	void Draw();
	void performHandExtraction();
	void writeGesture(String gesture);

	GestureLibrary gestureLib;
	HandRegistrationHandler registrationHandler;
	MyCamImage *actualCamImg;
	MyCamImage *threshImg;
	vector<vector<Point>> contours;
	vector<Hand> hands;
	char a[40];

};

