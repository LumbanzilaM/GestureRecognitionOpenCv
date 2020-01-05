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
		if (hand.RotationAngle > 60 && hand.RotationAngle < 80)
			return true;
	}
	return false;
}