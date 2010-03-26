#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>

struct Vec2 {
	Vec2() {}
	Vec2(double x, double y): x(x), y(y) {}
	double x,y;

	Vec2& operator+=(const Vec2& v) {
		x+=v.x, y+=v.y;
		return *this;
	}
	Vec2& operator-=(const Vec2& v) {
		x-=v.x, y-=v.y;
		return *this;
	}
	Vec2& operator*=(double a) {
		x*=a, y*=a;
		return *this;
	}
	Vec2& operator/=(double a) {
		x/=a, y/=a;
		return *this;
	}
};
Vec2 operator+(Vec2 a, const Vec2& b)
{
	return a+=b;
}
Vec2 operator-(Vec2 a, const Vec2& b)
{
	return a-=b;
}
Vec2 operator*(double x, Vec2 v)
{
	return v*=x;
}
Vec2 operator*(Vec2 v, double x)
{
	return v*=x;
}
Vec2 operator/(Vec2 v, double x)
{
	return v/=x;
}

double length2(const Vec2& v)
{
	return v.x*v.x + v.y*v.y;
}
double length(const Vec2& v)
{
	return sqrt(length2(v));
}
Vec2 normalize(Vec2 v)
{
	return v/=length(v);
}

#endif
