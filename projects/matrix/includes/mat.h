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

	const Mat2 transpose() const;
	const Mat2 inverse() const;
	float determinant() const;

	const float* operator[](std::size_t i) const;
	const Mat2 operator=(const Mat2& other);
	bool operator==(const Mat2& other)const;
	bool operator!=(const Mat2& other)const;
	const Mat2 operator+(const Mat2& right) const;
	const Mat2 operator-(const Mat2& right) const;
	const Mat2 operator*(const Mat2& right) const;
	const Vec2 operator*(const Vec2& v) const;
};
class Mat3{
private:
	//Internally row-major
	float mat[3][3];
public:
	Mat3();
	Mat3(const float& d);	
	Mat3(const float& a11, const float& a12, const float& a13,
			const float& a21, const float& a22, const float& a23,
			const float& a31, const float& a32, const float& a33);
	Mat3(const Mat3& other);

	std::ostream& write(std::ostream& os) const;

	const Mat3 transpose() const;
	const Mat3 inverse() const;
	float determinant() const;

	const float* operator[](std::size_t i) const;
	const Mat3 operator=(const Mat3& other);
	bool operator==(const Mat3& other)const;
	bool operator!=(const Mat3& other)const;
	const Mat3 operator+(const Mat3& right) const;
	const Mat3 operator-(const Mat3& right) const;
	const Mat3 operator*(const Mat3& right) const;
	const Vec3 operator*(const Vec3& v) const;
};
}



using namespace CGJM;
std::ostream& operator<<(std::ostream &os, const Mat2& v);
std::ostream& operator<<(std::ostream &os, const Mat3& v);
const Mat2 operator*(const Mat2& A, float scalar);
const Mat2 operator*(float scalar, const Mat2& A);
const Mat3 operator*(const Mat3& A, float scalar);
const Mat3 operator*(float scalar, const Mat3& A);
