#include <iostream>
#pragma once

namespace CGJM{

class Mat2{
private:
	//Internally row-major
	float mat[2][2];
public:
	Mat2();
	Mat2(const float& d);	
	Mat2(const float& a11, const float& a12,
			const float& a21, const float&a22);
	Mat2(const Mat2& other);

	std::ostream& write(std::ostream& os) const;

	const float* operator[](std::size_t i) const;
	const Mat2 operator=(const Mat2& other);
	bool operator==(const Mat2& other);
	bool operator!=(const Mat2& other);
	const Mat2 operator+(const Mat2& right) const;
	const Mat2 operator-(const Mat2& right) const;
	const Mat2 operator*(const Mat2& right) const;
	const Vec2 operator*(const Vec2& v) const;
};
}



using namespace CGJM;
std::ostream& operator<<(std::ostream &os, Mat2& v);
const Mat2 operator*(const Mat2& A, float scalar);
const Mat2 operator*(float scalar, const Mat2& A);
