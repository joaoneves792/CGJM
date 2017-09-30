#include <string>
#include <sstream>
#include "vec.h"

using namespace CGJM;

Vec2::Vec2(){
	vec[0]=0;
	vec[1]=0;
}

Vec2::Vec2(float x, float y){
	vec[0]=x;
	vec[1]=y;
}


Vec2::Vec2(Vec2& other){
	vec[0]=other[0];
	vec[1]=other[1];
}

float Vec2::operator[](std::size_t i){
	return this->vec[i];
	
}

std::ostream& Vec2::write(std::ostream& os){
	return os << "[ " << vec[0] << " " << vec[1] << " ]";
}


std::ostream& operator<<(std::ostream& os, Vec2& v){
	return v.write(os);
}

std::istream& Vec2::read(std::istream& is){
	std::string line;
	getline(is, line);
	for(int i=0;i<line.length();i++)
		if(line[i] == '[' || line[i] == ']')
			line[i] = ' ';
	std::istringstream iss(line);
	iss >> vec[0] >> vec[1];
	return is;
}

std::istream& operator>>(std::istream& is, Vec2& v){
	v.read(is);
}


Vec2 Vec2::operator=(Vec2& other){
	vec[0] = other[0];
	vec[1] = other[1];
	return *this;
}

Vec2* Vec2::operator+(Vec2& right){
	Vec2* newV = new Vec2(vec[0]+right[0], vec[1]+right[1]);
	return newV;
}

Vec2* Vec2::operator-(Vec2& right){
	Vec2* newV = new Vec2(vec[0]-right[0], vec[1]-right[1]);
	return newV;
}

Vec2* operator*(Vec2& v, float scalar){
	Vec2* newV = new Vec2(v[0]*scalar, v[1]*scalar);
	return newV;
}

Vec2* operator*(float scalar, Vec2& v){
	Vec2* newV = new Vec2(v[0]*scalar, v[1]*scalar);
	return newV;
}
