#include "stdafx.h"
#include "NextLeftGesture.h"


NextLeftGesture::NextLeftGesture()
{
	initGesture();
}


NextLeftGesture::~NextLeftGesture()
{
	initGesture();
}

void NextLeftGesture::initGesture()
{
	isThumb = true;
	nbFingers = 1;
	name = "Next Left";
}

bool NextLeftGesture::isGesture(Hand hand)
{
	if (hand.NbFingerUp >= nbFingers)
	{
		if (hand.RotationAngle < -60 && hand.RotationAngle > -90)
			return true;
	}
	return false;
}