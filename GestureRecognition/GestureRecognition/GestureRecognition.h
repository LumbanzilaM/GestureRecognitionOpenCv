#pragma once
#include <iostream>
#include <stdio.h>

#include "HandRegistrationHandler.h"


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
	void performHandExtraction();
	void writeGesture(String gesture);

	HandRegistrationHandler registrationHandler;
	MyCamImage *actualCamImg;
	MyCamImage *threshImg;
	vector<vector<Point>> contours;
	char a[40];

};

