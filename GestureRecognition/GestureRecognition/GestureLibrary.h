#pragma once
#include "AGesture.h"
#include "ValidateGesture.h"
#include "BackGesture.h"
#include "NextRightGesture.h"
#include "NextLeftGesture.h"
#include "LockGesture.h"

class GestureLibrary
{
public:
	GestureLibrary();
	~GestureLibrary();
	string FindGesture(Hand hand);


private :
	list<AGesture*> gestures;

	void initLibrary();


	

};

