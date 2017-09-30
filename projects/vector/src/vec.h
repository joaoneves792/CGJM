#include <iostream>
#pragma once

namespace CGJM{

class Vec2{
private:
	float vec[2];
public:
	Vec2();
	Vec2(float x, float y);
	Vec2(Vec2& other);

	std::ostream& write(std::ostream& os);
	std::istream& read(std::istream& is);

	float operator[](std::size_t i);
	Vec2 operator=(Vec2& other);
	Vec2* operator+(Vec2& right);
	Vec2* operator-(Vec2& right);
};

}

using namespace CGJM;
std::ostream& operator<<(std::ostream &os, Vec2& v);
std::istream& operator>>(std::istream &is, Vec2& v);
Vec2* operator*(Vec2& v, float scalar);
Vec2* operator*(float scalar, Vec2& v);
