#include "stdafx.h"
#include "ValidateGesture.h"


ValidateGesture::ValidateGesture() : AGesture()
{
	initGesture();
}


ValidateGesture::~ValidateGesture()
{
}

void ValidateGesture::initGesture()
{
	isThumb = true;
	nbFingers = 1;
	name = "Validate";
}

bool ValidateGesture::isGesture(Hand hand)
{
	if (hand.NbFingerUp == nbFingers && hand.ThumbIdx != -1)
	{
		if( hand.RotationAngle > -15 && hand.RotationAngle < 15 )
			return true;
	}
		
	return false;
}

