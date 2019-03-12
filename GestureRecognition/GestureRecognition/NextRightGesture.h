#pragma once
#include "AGesture.h"
class NextRightGesture :
	public AGesture
{
public:
	NextRightGesture();
	~NextRightGesture();
	void initGesture();
	bool isGesture(Hand hand);
};

