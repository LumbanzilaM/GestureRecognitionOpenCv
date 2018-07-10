#include "stdafx.h"
#include "MathOperation.h"

# define M_PI           3.14159265358979323846 

MathOperation::MathOperation()
{

}


MathOperation::~MathOperation()
{
}

float MathOperation::distanceP2P(Point a, Point b)
{
	float d = sqrt(fabs(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
	return d;
}

float MathOperation::angleBetween(const Point &s, const Point & f, const Point & e)
{
	float l1 = distanceP2P(f, s);
	float l2 = distanceP2P(f, e);
	float dot = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float angle = acos(dot / (l1*l2));
	angle = angle * 180 / M_PI;
	return angle;
}
