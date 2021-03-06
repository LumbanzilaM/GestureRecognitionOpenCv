#include "stdafx.h"


#include "HandRegistrationHandler.h"
#include "GestureRecognition.h"
#include "MyCamImage.h"
//opencv

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<opencv2/opencv.hpp>


//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
using namespace std;
// Global variables

int main(int argc, char* argv[])
{
	//SimpleHandDetection();
	//BaseCamLoop();
	MyCamImage output("OutputImage");
	GestureRecognition recognition(&output);
	recognition.launchHandRegistration();
	recognition.launchGestureRecognition();

	return 0;
}
