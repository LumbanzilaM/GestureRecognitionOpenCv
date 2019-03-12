#pragma once
#include "AGesture.h"
class BackGesture :
	public AGesture
{
public:
	BackGesture();
	~BackGesture();
	void initGesture();
	bool isGesture(Hand hand);
};

