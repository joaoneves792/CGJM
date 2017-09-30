#include <iostream>
#pragma once

namespace CGJM{

class Vec2{
private:
	float vec[2];
public:
	Vec2();
	Vec2(const float& x, const float& y);
	Vec2(const Vec2& other);

	std::ostream& write(std::ostream& os) const;
	std::istream& read(std::istream& is);

	float& operator[](std::size_t i);
	float operator[](std::size_t i) const;
	const Vec2 operator=(const Vec2& other);
	const Vec2 operator+(const Vec2& right) const;
	const Vec2 operator-(const Vec2& right) const;
};

}

using namespace CGJM;
std::ostream& operator<<(std::ostream &os, Vec2& v);
std::istream& operator>>(std::istream &is, Vec2& v);
const Vec2 operator*(const Vec2& v, float scalar);
const Vec2 operator*(float scalar,const Vec2& v);
