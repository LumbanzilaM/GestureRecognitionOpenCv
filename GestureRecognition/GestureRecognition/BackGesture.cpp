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
	nbFingers = 0;
	name = "Back";
}

bool BackGesture::isGesture(Hand hand)
{
	if (hand.NbFingerUp == nbFingers)
	{
		if (hand.RotationAngle > -15 && hand.RotationAngle < 15)
			return true;
	}

	return false;
}