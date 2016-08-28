/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 2 - Robot Arm
*/

#include "Vec3.h"

const float Pi = 3.14;

Vec3::Vec3(float x, float y, float z)
	:x(x), y(y), z(z)
{
}

float Vec3::dot(const Vec3 &other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const
{
	return Vec3(y * other.z - other.y * z, z * other.x - other.z * x, x * other.y - other.x * y);
}

Vec3 Vec3::operator+ (const Vec3 &other) const
{
	return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator- (const Vec3 &other) const
{
	return *this + other.flip();
}

Vec3 Vec3::operator* (float scalar) const
{
	return Vec3(scalar * x, scalar * y, scalar * z);
}

Vec3 Vec3::flip() const
{
	return Vec3(-x, -y, -z);
}


std::ostream& operator<<(std::ostream &out, const Vec3 &vec)
{
	out << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">";
	return out;
}

float Vec3::magnitude() const
{
	return sqrt(x * x + y * y + z * z);
}

float Vec3::xyAngle() const
{
	return atan(y / x);
}

float Vec3::yzAngle() const
{
	return atan(y / z);
}

float Vec3::xzAngle() const
{
	float angle = atan(z / x) * 180 / Pi;
	if (x < 0)
		angle += 180;
	if (angle < 0)
		angle += 360;
	return angle;
}

void Vec3::rotate(float angle, float x, float y, float z)
{
	

}

void Vec3::rotatex(float angle)
{
	Vec3 temp = *this;
	float radAngle = angle * Pi / 180;
	y = temp.y * cos(radAngle) - temp.z * sin(radAngle);
	z = temp.y * sin(radAngle) + temp.z * cos(radAngle);
}

void Vec3::rotatey(float angle)
{
	Vec3 temp = *this;
	float radAngle = angle * Pi / 180;
	x = temp.x * cos(radAngle) + temp.z * sin(radAngle);
	z = -temp.x * sin(radAngle) + temp.z * cos(radAngle);
}

void Vec3::rotatez(float angle)
{
	Vec3 temp = *this;
	float radAngle = angle * Pi / 180;
	x = temp.x * cos(radAngle) - temp.y * sin(radAngle);
	y = temp.x * sin(radAngle) + temp.y * cos(radAngle);
}

void Vec3::normalize()
{
	float mag = magnitude();

	if (mag == 0)
		return;

	x /= mag;
	y /= mag;
	z /= mag;
}