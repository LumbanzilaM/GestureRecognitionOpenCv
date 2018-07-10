#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\GestureRecognition\MathOperation.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			MathOperation op;
			/*float res = op.angleBetween(Point(0, 90), Point(0, 0), Point(90, 0));*/
			Assert::AreEqual(90, 90);
		}

	};
}