#include "stdafx.h"
#include "CppUnitTest.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<opencv2/opencv.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace cv;
namespace HandRegistrationTest
{		
	TEST_CLASS(HandRegistrationHandler)
	{
	public:
		
		TEST_METHOD(TestangleBetween)
		{
			/*MathOperation op;
			float res = op.angleBetween(Point(0, 90), Point(0, 0), Point(90, 0));*/
			Assert::AreEqual(90, 90, 0);
			// TODO: Ici, votre code de test
		}

	};
}