#include "stdafx.h"
#include "MathOperation.h"

# define M_PI           3.14159265358979323846 

MathOperation::MathOperation()
{

}


MathOperation::~MathOperation()
{
}

float MathOperation::distanceP2P(cv::Point a, cv::Point b)
{
	float d = sqrt(fabs(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
	return d;
}

float MathOperation::angleBetween(const cv::Point &s, const cv::Point & f, const cv::Point & e)
{
	float l1 = distanceP2P(f, s);
	float l2 = distanceP2P(f, e);
	float dot = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float angle = acos(dot / (l1*l2));
	angle = angle * 180 / M_PI;
	return angle;
}


cv::Point MathOperation::RotatePoint(const cv::Point2f& p, float rad)
{
	const float x = std::cos(rad) * p.x - std::sin(rad) * p.y;
	const float y = std::sin(rad) * p.x + std::cos(rad) * p.y;

	const cv::Point2f rot_p(x, y);
	return rot_p;
}

cv::Point MathOperation::RotatePoint(const cv::Point2f& cen_pt, const cv::Point2f& p, float angle)
{
	float rad = (angle * M_PI) / 180;
	const cv::Point2f trans_pt = p - cen_pt;
	const cv::Point2f rot_pt = RotatePoint(trans_pt, rad);
	const cv::Point2f fin_pt = rot_pt + cen_pt;

	return fin_pt;
}
