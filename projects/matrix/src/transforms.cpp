#include <cmath>
#include "vec.h"
#include "mat.h"

using namespace CGJM;

const Mat4 CGJM::scale(const float& x, const float& y, const float& z){
	return Mat4(x, 0, 0, 0,
		    0, y, 0, 0,
		    0, 0, z, 0,
		    0, 0, 0, 1);
}

const Mat4 CGJM::rotate(const Vec3& axis, const float& angle){
#define __COS std::cos(angle)
#define __1_COS (1-std::cos(angle))
#define __SIN std::sin(angle)
#define __l axis[0]
#define __m axis[1]
#define __n axis[2]
	return Mat4( __l*__l*__1_COS+__COS, __m*__l*__1_COS-__n*__SIN, __n*__l*__1_COS+__m*__SIN, 0,
		     __l*__m*__1_COS+__n*__SIN, __m*__m*__1_COS+__COS, __n*__m*__1_COS-__l*__SIN, 0,
		     __l*__n*__1_COS-__m*__SIN, __m*__n*__1_COS+__l*__SIN, __n*__n*__1_COS+__COS, 0,
		     0, 0, 0, 1);
}

const Mat4 CGJM::translate(const float& x, const float& y, const float& z){
	return Mat4(1, 0, 0, x,
		    0, 1, 0, y,
		    0, 0, 1, z,
		    0, 0, 0, 1);
}