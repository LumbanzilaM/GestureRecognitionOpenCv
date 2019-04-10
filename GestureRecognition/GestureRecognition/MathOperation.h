#pragma once

#include <math.h>

#include<opencv2/opencv.hpp>

using namespace std;

class MathOperation
{
public:
	MathOperation();
	~MathOperation();
	float distanceP2P(cv::Point a, cv::Point b);
	float angleBetween(const cv::Point &v1, const cv::Point &origin, const cv::Point &v2);
	cv::Point RotatePoint(const cv::Point2f & p, float rad);
	cv::Point RotatePoint(const cv::Point2f & cen_pt, const cv::Point2f & p, float rad);
	double diffclock(clock_t clock1, clock_t clock2);
	struct str {
		bool operator() (cv::Point2f a, cv::Point2f b) {
			if (a.y != b.y)
				return a.y < b.y;
			return a.x <= b.x;
		}
	} comp;
};

  