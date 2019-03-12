#pragma once
#include <stdio.h>
#include "Hand.h"

class AGesture
{
public:
	AGesture();
	~AGesture();
	virtual void initGesture() = 0;
	virtual bool isGesture(Hand hand) = 0;
	string getName();


protected:

	float handAngle[2];
	int nbFingers;
	bool isThumb;
	vector<int> fingersIdx;
	string name;



};

