#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>

struct Vec2 {
	Vec2() {}
	Vec2(double x, double y): x(x), y(y) {}
	double x,y;
};
#define OP(op,ope)\
	inline Vec2& operator ope(Vec2& a, const Vec2& b) {a.x ope b.x; a.y ope b.y; return a;}\
	inline Vec2 operator op(Vec2 a, const Vec2& b) {return a ope b;}
OP(+,+=)
OP(-,-=)
#undef OP
#define OP(op,ope)\
	inline Vec2& operator ope(Vec2& a, double x) {a.x ope x; a.y ope x; return a;}\
	inline Vec2 operator op(Vec2 a, double x) {return a ope x;}
OP(*,*=)
OP(/,/=)
#undef OP
inline Vec2 operator-(Vec2 a)
{
    Vec2 z(0,0);
	return z-a;
}
inline Vec2 operator*(double x, Vec2 v)
{
	return v*=x;
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

struct Vec3 {
	Vec3() {}
	Vec3(double x, double y, double z): x(x), y(y), z(z) {}
	Vec3(const Vec2 v, double z): x(v.x), y(v.y), z(z) {}
	double x,y,z;
};
#define OP(op,ope)\
	inline Vec3& operator ope(Vec3& a, const Vec3& b) {a.x ope b.x; a.y ope b.y; a.z ope b.z; return a;}\
	inline Vec3 operator op(Vec3 a, const Vec3& b) {return a ope b;}
OP(+,+=)
OP(-,-=)
#undef OP
#define OP(op,ope)\
	inline Vec3& operator ope(Vec3& a, double x) {a.x ope x; a.y ope x; a.z ope x; return a;}\
	inline Vec3 operator op(Vec3 a, double x) {return a ope x;}
OP(*,*=)
OP(/,/=)
#undef OP
inline Vec3 operator*(double x, Vec3 v) {return v*=x;}

inline double length2(const Vec3& v)
{
	return v.x*v.x + v.y*v.y + v.z*v.z;
}
inline double length(const Vec3& v)
{
	return sqrt(length2(v));
}

#include <ostream>
inline std::ostream& operator<<(std::ostream& o, const Vec2& v)
{
	return o<<'('<<v.x<<','<<v.y<<')';
}

#endif
