#include "stdafx.h"
#include "GestureLibrary.h"


GestureLibrary::GestureLibrary()
{
	initLibrary();
}


GestureLibrary::~GestureLibrary()
{
}

string GestureLibrary::FindGesture(Hand hand)
{ 
	for each(AGesture* gesture in gestures)
	{
		if (gesture->isGesture(hand))
			return gesture->getName();
	}
	return "NG";
}

void GestureLibrary::initLibrary()
{
	gestures.push_back(new ValidateGesture());
	gestures.push_back(new BackGesture());
	gestures.push_back(new NextRightGesture());

}
