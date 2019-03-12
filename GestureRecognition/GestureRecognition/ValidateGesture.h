#pragma once
#include "AGesture.h"
class ValidateGesture :
	public AGesture
{
public:
	ValidateGesture();
	~ValidateGesture();
	void initGesture();
	bool isGesture(Hand hand);
};

