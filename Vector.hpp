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
inline Vec2 operator+(Vec2 a, const Vec2& b)
{
	return a+=b;
}
inline Vec2 operator-(Vec2 a, const Vec2& b)
{
	return a-=b;
}
inline Vec2 operator-(Vec2 a)
{
    Vec2 z(0,0);
	return z-a;
}
inline Vec2 operator*(double x, Vec2 v)
{
	return v*=x;
}
inline Vec2 operator*(Vec2 v, double x)
{
	return v*=x;
}
inline Vec2 operator/(Vec2 v, double x)
{
	return v/=x;
}

inline double length2(const Vec2& v)
{
	return v.x*v.x + v.y*v.y;
}
inline double length(const Vec2& v)
{
	return sqrt(length2(v));
}
inline Vec2 normalize(Vec2 v)
{
	return v/=length(v);
}
inline double dot(const Vec2& a, const Vec2& b)
{
	return a.x*b.x+a.y*b.y;
}
inline double cross(const Vec2& a, const Vec2& b)
{
	return a.x*b.y - a.y*b.x;
}

#include <ostream>
inline std::ostream& operator<<(std::ostream& o, const Vec2& v)
{
	return o<<'('<<v.x<<','<<v.y<<')';
}

#endif
