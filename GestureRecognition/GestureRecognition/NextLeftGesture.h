#pragma once
#include "AGesture.h"
class NextLeftGesture :
	public AGesture
{
public:
	NextLeftGesture();
	~NextLeftGesture();
	void initGesture();
	bool isGesture(Hand hand);
};

