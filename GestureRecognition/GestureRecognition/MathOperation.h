#pragma once

#include <math.h>

#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MathOperation
{
public:
	MathOperation();
	~MathOperation();
	float distanceP2P(Point a, Point b);
	float angleBetween(const Point &v1, const Point &origin, const Point &v2);
};

  