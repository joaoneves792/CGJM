#include <string>
#include <sstream>
#include <cmath>
#include <assert.h>
#include "vec.h"
#include "mat.h"

using namespace CGJM;

Mat2::Mat2(){
	mat[0][0]=0;
	mat[0][1]=0;

	mat[1][0]=0;
	mat[1][1]=0;
}

Mat2::Mat2(const float& d){
	mat[0][0]=d;
	mat[0][1]=0;

	mat[1][0]=0;
	mat[1][1]=d;
}

Mat2::Mat2(const float& a11, const float& a12,
		const float& a21, const float&a22){
	mat[0][0]=a11;
	mat[0][1]=a12;

	mat[1][0]=a21;
	mat[1][1]=a22;
}


Mat2::Mat2(const Mat2& other){
	mat[0][0]=other[0][0];
	mat[0][1]=other[0][1];

	mat[1][0]=other[1][0];
	mat[1][1]=other[1][1];
}


const float* Mat2::operator[](std::size_t i) const{
	assert(i<2);
	return (mat)[i];
}

std::ostream& Mat2::write(std::ostream& os) const{
	return os << "| " << mat[0][0] << " " << mat[0][1] << " |" << std::endl
		<< "| " << mat[1][0] << " " << mat[1][1] << " |" << std::endl;
}


std::ostream& operator<<(std::ostream& os, Mat2& v){
	return v.write(os);
}

const Mat2 Mat2::operator=(const Mat2& other){
	mat[0][0] = other[0][0];
	mat[0][1] = other[0][1];
	mat[1][0] = other[1][0];
	mat[1][1] = other[1][1];
	return *this;
}

bool Mat2::operator==(const Mat2& other){
#ifndef EPSILON
#define EPSILON 0.001
#endif
	return ((std::abs(mat[0][0]-other[0][0]) < EPSILON) &&
		(std::abs(mat[0][1]-other[0][1]) < EPSILON) &&
		(std::abs(mat[1][0]-other[1][0]) < EPSILON) &&
		(std::abs(mat[1][1]-other[1][1]) < EPSILON) );


}

bool Mat2::operator!=(const Mat2& other){
	return !(*this==other);
}


const Mat2 Mat2::operator+(const Mat2& right) const{
	return Mat2(mat[0][0]+right[0][0], mat[0][1]+right[0][1],
			mat[1][0]+right[1][0], mat[1][1]+right[1][1]);
}

const Mat2 Mat2::operator-(const Mat2& right) const{
	return Mat2(mat[0][0]-right[0][0], mat[0][1]-right[0][1],
			mat[1][0]-right[1][0], mat[1][1]-right[1][1]);
}

const Mat2 operator*(const Mat2& A, float scalar){
	return Mat2(A[0][0]*scalar, A[0][1]*scalar,
			A[1][0]*scalar, A[1][1]*scalar);
}

const Mat2 operator*(float scalar, const Mat2& A){
	return Mat2(A[0][0]*scalar, A[0][1]*scalar,
			A[1][0]*scalar, A[1][1]*scalar);
}

const Mat2 Mat2::operator*(const Mat2& right) const{
	return Mat2(mat[0][0]*right[0][0]+mat[0][1]*right[1][0],
		mat[0][0]*right[0][1]+mat[0][1]*right[1][1],
		mat[1][0]*right[0][0]+mat[1][1]*right[1][0],
		mat[1][0]*right[0][1]+mat[1][1]*right[1][1]);

}

const Vec2 Mat2::operator*(const Vec2& v) const{
	return Vec2(mat[0][0]*v[0]+mat[0][1]*v[1],
			mat[1][0]*v[0]+mat[1][1]*v[1]);
}
