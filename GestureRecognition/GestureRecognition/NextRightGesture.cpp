#include "stdafx.h"
#include "NextRightGesture.h"


NextRightGesture::NextRightGesture()
{
	initGesture();
}


NextRightGesture::~NextRightGesture()
{
}

void NextRightGesture::initGesture()
{
	isThumb = true;
	nbFingers = 1;
	name = "Next Right";
}

bool NextRightGesture::isGesture(Hand hand)
{
	if (hand.NbFingerUp >= nbFingers)
	{
		if (hand.RotationAngle > 30 && hand.RotationAngle < 70)
			return true;
	}
	return false;
}