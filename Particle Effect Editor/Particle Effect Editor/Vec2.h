#pragma once

#include "Units.h"
#include <string>

//2D Vector class. All angles are in degrees, unless suffix Rad is present.
class Vec2 {
public:
	double X, Y;

	/* INITIALISATION */
	Vec2(double x=0, double y=0) { X = x; Y = y; }
	Vec2(const Vec2 &v) { X = v.X; Y = v.Y; }
	Vec2* operator=(const Vec2 &v) { X = v.X; Y = v.Y; return this; }


	/* METHODS */
	//Mutators
	void Normalise();
	void Rotate(double a) { RotateRad(a * DEGTORAD); }
	void RotateRad(double a);
	void Reflect(Vec2 v);
	void Invert() { *this *= -1; }

	//Immutable versions (useful for chaining)
	Vec2 Normalised() const { Vec2 v(*this); v.Normalise(); return v; }
	Vec2 Rotated(double a) const { Vec2 v(*this); v.Rotate(a); return v; }
	Vec2 RotatedRad(double a) const { Vec2 v(*this); v.RotateRad(a); return v; }
	Vec2 Reflected(const Vec2 &v) const { Vec2 v2(*this); v2.Reflect(v); return v2; }
	Vec2 Inverted() const { return *this * -1; }

	//Operations between vectors
	double Dot(const Vec2 &v) const { return X * v.X + Y * v.Y; } //project one vector onto another: http://www.mvps.org/directx/articles/math/dot/
	double Cross(const Vec2 &v) const { return X * v.Y - Y * v.X; } //calculate winding: http://stackoverflow.com/a/243984
	double DistanceTo(const Vec2 &v) const { return (v - *this).Length(); }
	double AngleTo(const Vec2 &v) const { return Angle() - v.Angle(); }


	/* PROPERTIES */
	std::string str();
	double Angle() const { return AngleRad() * RADTODEG; }
	double AngleRad() const { return atan2(Y, X); }
	void Angle(double a) { AngleRad(a * DEGTORAD); } 
	void AngleRad(double a);
	double Length() const { return sqrt(X*X + Y*Y); }
	void Length(double f) { Normalise(); *this *= f; }


	/* OPERATORS */
	//Vec2 prefix
	Vec2 operator++() { Length(Length()+1); return *this; } 
	Vec2 operator--() { Length(Length()-1); return *this; } 
	
	//Vec2 infix Vec2
	Vec2 operator+(const Vec2 &v) const { return Vec2(X + v.X, Y + v.Y); }
	Vec2 operator-(const Vec2 &v) const { return Vec2(X - v.X, Y - v.Y); }
	Vec2 operator*(const Vec2 &v) const { return Vec2(X * v.X, Y * v.Y); }
	Vec2 operator/(const Vec2 &v) const { return Vec2(X / v.X, Y / v.Y); }

	void operator+=(const Vec2 &v) { X += v.X; Y += v.Y; }
	void operator-=(const Vec2 &v) { X -= v.X; Y -= v.Y; }
	void operator*=(const Vec2 &v) { X *= v.X; Y *= v.Y; }
	void operator/=(const Vec2 &v) { X /= v.X; Y /= v.Y; }

	bool operator==(const Vec2 &v) const { return X == v.X && Y == v.Y; }
	bool operator<(const Vec2 &v) const { return X < v.X && Y < v.Y; }
	bool operator>(const Vec2 &v) const { return X > v.X && Y > v.Y;  }
	bool operator<=(const Vec2 &v) const { return X <= v.X && Y <= v.Y; }
	bool operator>=(const Vec2 &v) const { return X >= v.X && Y >= v.Y; }

	//Vec2 infix double (scalar)
	Vec2 operator+(double f) const { return Vec2(X + f, Y + f); }	
	Vec2 operator-(double f) const { return Vec2(X - f, Y - f); }	
	Vec2 operator*(double f) const { return Vec2(X * f, Y * f); }	
	Vec2 operator/(double f) const { return Vec2(X / f, Y / f); }

	void operator+=(double f) { X += f; Y += f; }
	void operator-=(double f) { X -= f; Y -= f; }
	void operator*=(double f) { X *= f; Y *= f; }
	void operator/=(double f) { X /= f; Y /= f; }

	//Vec2 postfix
	double operator[](int i) { return i == 0 ? X : Y; }
	Vec2 operator++(int) { Vec2 temp = *this; ++(*this); return temp; } 
	Vec2 operator--(int) { Vec2 temp = *this; --(*this); return temp; }
};