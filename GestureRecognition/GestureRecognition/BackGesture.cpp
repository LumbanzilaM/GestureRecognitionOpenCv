#include "stdafx.h"
#include "BackGesture.h"


BackGesture::BackGesture()
{
	initGesture();
}


BackGesture::~BackGesture()
{
}

void BackGesture::initGesture()
{
	isThumb = true;
	nbFingers = 1;
	name = "Back";
}

bool BackGesture::isGesture(Hand hand)
{
	if (hand.NbFingerUp == 0)
	{
		if (hand.RotationAngle > -15 && hand.RotationAngle < 20)
			return true;
	}

	return false;
}