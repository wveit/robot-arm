/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#pragma once

#include <iostream>

class Vec3
{
public:
	float x, y, z;
	Vec3(float x = 0, float y = 0, float z = 0);

	float dot(const Vec3 &other) const;
	Vec3 cross(const Vec3 &other) const;
	Vec3 operator+ (const Vec3 &other) const;
	Vec3 operator- (const Vec3 &other) const;
	Vec3 operator* (float scalar) const;
	Vec3 flip() const;

	float magnitude() const;
	void normalize();

	void rotatex(float angle);
	void rotatey(float angle);
	void rotatez(float angle);

	// not implemented yet
	void rotate(float angle, float x, float y, float z);

	// test these angle functions before using
	float xyAngle() const;
	float yzAngle() const;
	float xzAngle() const;

	
};

std::ostream& operator<<(std::ostream &out, const Vec3 &vec);