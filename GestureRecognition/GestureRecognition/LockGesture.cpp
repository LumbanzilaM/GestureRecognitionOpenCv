#include "stdafx.h"
#include "LockGesture.h"


LockGesture::LockGesture()
{
	initGesture();
}


LockGesture::~LockGesture()
{
}

void LockGesture::initGesture()
{
	isThumb = true;
	nbFingers = 2;
	name = "Lock";
}

bool LockGesture::isGesture(Hand hand)
{
	if (hand.NbFingerUp == nbFingers)
	{
		if (hand.RotationAngle > -15 && hand.RotationAngle < 15)
			return true;
	}

	return false;
}