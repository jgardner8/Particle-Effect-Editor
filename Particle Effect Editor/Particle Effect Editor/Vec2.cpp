#include "Vec2.h"
#include <sstream>
 
using namespace std;

void Vec2::Normalise() {
	double length = Length();
	if (length != 0) {
		X /= length;
		Y /= length;
	} 
}

void Vec2::RotateRad(double a) {
	double angle = AngleRad();
	double length = Length();
	X = cos(angle + a) * length; 
	Y = sin(angle + a) * length;
}
 
//Reflecting by a vector more than 90 degrees away may not produce the results you expect.
//  (the result will be as if the vector to reflect by is inverted before the calculations)
void Vec2::Reflect(Vec2 v) {
	v.Normalise();
	*this -= v * (2 * Dot(v));
	this->Invert();
}

void Vec2::AngleRad(double a) {
	double length = Length();
	X = cos(a) * length;
	Y = sin(a) * length;
}
 
string Vec2::str() {
	stringstream ss;
	ss << '[' << X << ", " << Y << ']';
	return ss.str();
}