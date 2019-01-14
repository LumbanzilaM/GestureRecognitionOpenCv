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
	Point RotatePoint(const cv::Point2f & p, float rad);
	Point RotatePoint(const cv::Point2f & cen_pt, const cv::Point2f & p, float rad);
	struct str {
		bool operator() (Point2f a, Point2f b) {
			if (a.y != b.y)
				return a.y < b.y;
			return a.x <= b.x;
		}
	} comp;
};

  