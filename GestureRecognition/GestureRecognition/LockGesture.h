#pragma once
#include "AGesture.h"
class LockGesture :
	public AGesture
{
public:
	LockGesture();
	~LockGesture();
	void initGesture();
	bool isGesture(Hand hand);
};

