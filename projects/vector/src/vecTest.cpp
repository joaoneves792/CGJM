#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

#include "vec.h"

void assertString(std::string output, std::string expected){
	if(output.compare(expected)){
		std::cout << "Assertion failed!\n" << "Expected: " << expected << "\n" << "Got: " << output << std::endl;
		exit(1);
	}
}

void assertFloat(float output, float expected){
#define EPSILON 0.001
	if(std::abs(output-expected) > EPSILON){
		std::cout << "Assertion failed!\n" << "Expected: " << expected << "\n" << "Got: " << output << std::endl;
		exit(1);
	}
}

void assertTrue(bool output){
	if(!output){
		std::cout << "Assertion failed!\n" << "Expected: True"  << "\n" << "Got: " << output << std::endl;
		exit(1);
	}
}


void accessEmptyConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v;
	assertFloat(v[0], 0);
	assertFloat(v[1], 0);

	CGJM::Vec3 v2;
	assertFloat(v2[0], 0);
	assertFloat(v2[1], 0);
	assertFloat(v2[2], 0);
	std::cout << "PASSED!" << std::endl;
}

void accessFloatConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v(4.5, 3.3);
	assertFloat(v[0], 4.5);
	assertFloat(v[1], 3.3);
	
	CGJM::Vec3 v2(4.5, 3.3, 8.8);
	assertFloat(v2[0], 4.5);
	assertFloat(v2[1], 3.3);
	assertFloat(v2[2], 8.8);
	std::cout << "PASSED!" << std::endl;
}

void accessVecConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v1(7, 8);
	CGJM::Vec2 v2(v1);

	assertFloat(v2[0], 7);
	assertFloat(v2[0], v1[0]);
	
	assertFloat(v2[1], 8);
	assertFloat(v2[1], v1[1]);


	CGJM::Vec3 v3(7, 8, 9);
	CGJM::Vec3 v4(v3);

	assertFloat(v4[0], 7);
	assertFloat(v4[0], v3[0]);
	
	assertFloat(v4[1], 8);
	assertFloat(v4[1], v3[1]);
	
	assertFloat(v4[2], 9);
	assertFloat(v4[2], v3[2]);


	std::cout << "PASSED!" << std::endl;
}

void directAssignmentTest(){
	std::cout << __func__ << " ";

	CGJM::Vec2 v(1, 2);
	v[0] = 3;
	v[1] = 4;

	assertFloat(v[0], 3);
	assertFloat(v[1], 4);

	CGJM::Vec3 v2(1, 2, 3);
	v2[0] = 4;
	v2[1] = 5;
	v2[2] = 6;

	assertFloat(v2[0], 4);
	assertFloat(v2[1], 5);
	assertFloat(v2[2], 6);

	std::cout << "PASSED!" << std::endl;
}

void outputTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v(3.4, 7.3);
	std::ostringstream ss;
	ss << v;
	assertString(ss.str(), "[ 3.4 7.3 ]");
	
	CGJM::Vec3 v2(3.4, 7.3, 6.6);
	ss << v2;
	assertString(ss.str(), "[ 3.4 7.3 6.6 ]");
	std::cout << "PASSED!" << std::endl;
}

void assignmentTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v1(7, 8);
	CGJM::Vec2 v2(9, 10);

	v1=v2;

	assertFloat(v1[0], 9);
	assertFloat(v1[0], v2[0]);

	assertFloat(v1[1], 10);
	assertFloat(v1[1], v2[1]);
	std::cout << "PASSED!" << std::endl;
}


void addTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 sum;
	CGJM::Vec2 v1(1, 2);
	CGJM::Vec2 v2(3, 4);

	sum = v1 + v2;

	assertFloat(sum[0], 4);
	assertFloat(sum[1], 6);

	std::cout << "PASSED!" << std::endl;
}

void subtractionTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 sub;
	CGJM::Vec2 v1(1, 2);
	CGJM::Vec2 v2(3, 5);

	sub = v1 - v2;

	assertFloat(sub[0], -2);
	assertFloat(sub[1], -3);

	std::cout << "PASSED!" << std::endl;
}

void multiplicationTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 product;
	CGJM::Vec2 v(3, 2);

	product = v * 3;

	assertFloat(product[0], 9);
	assertFloat(product[1], 6);

	product = 2 * v;
	
	assertFloat(product[0], 6);
	assertFloat(product[1], 4);

	std::cout << "PASSED!" << std::endl;
}


void inputStreamTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v;

	std::istringstream* iss = new std::istringstream("[ 3 4 ]\n"); 
	std::cin.rdbuf((*iss).rdbuf());
	std::cin >> v;

	assertFloat(v[0], 3);
	assertFloat(v[1], 4);
	delete iss;


	iss = new std::istringstream(" 5 6 \n"); 
	std::cin.rdbuf((*iss).rdbuf());
	std::cin >> v;

	assertFloat(v[0], 5);
	assertFloat(v[1], 6);
	delete iss;
	
	iss = new std::istringstream("7 8\n"); 
	std::cin.rdbuf((*iss).rdbuf());
	std::cin >> v;

	assertFloat(v[0], 7);
	assertFloat(v[1], 8);
	delete iss;
	std::cout << "PASSED!" << std::endl;
}

void algebraicPropertiesTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v1(1, 2);
	CGJM::Vec2 v2(3, 4);
	CGJM::Vec2 v3(5, 6);


	/*Commutative*/
	CGJM::Vec2 s1;
	CGJM::Vec2 s2;
	
	s1 = v1 + v2;
	s2 = v2 + v1;
	assertFloat(s1[0], s2[0]);
	assertFloat(s1[1], s2[1]);

	/*Associative*/
	s1 = (v1 + v2) + v3;
	s2 = v1 + (v2 + v3);
	assertFloat(s1[0], s2[0]);
	assertFloat(s1[1], s2[1]);


	/*Distributive (vector)*/
	s1 = 3 * (v1 + v2);
	s2 = 3 * v1 + 3 * v2;
	assertFloat(s1[0], s2[0]);
	assertFloat(s1[1], s2[1]);

	/*Distributive (scalar)*/
	s1 = (4 + 5) * v1;
	s2 = 4 * v1 + 5* v1;
	assertFloat(s1[0], s2[0]);
	assertFloat(s1[1], s2[1]);

	/*Associative (scalar)*/
	s1 = 6 * (7 * v1);
	s2 = (6 * 7) * v1;
	assertFloat(s1[0], s2[0]);
	assertFloat(s1[1], s2[1]);

	std::cout << "PASSED!" << std::endl;
}

void quadranceTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v1(1, 1);
	CGJM::Vec2 v2(2, 2);

	assertTrue((v1.quadrance() < v2.quadrance()));

	std::cout << "PASSED!" << std::endl;
}


void normTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v1(0, 1);
	CGJM::Vec2 v2(2, 0);

	CGJM::Vec2 v3(3, 3);

	assertFloat(v1.norm(), 1);
	assertFloat(v2.norm(), 2);

	assertFloat(v3.norm(), std::sqrt(18));

	std::cout << "PASSED!" << std::endl;
}


void normalizeTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v1(0, 1);
	CGJM::Vec2 v2(2, 0);
	CGJM::Vec2 v3(3, 3);

	v1.normalize();
	v2.normalize();
	v3.normalize();

	assertFloat(v1.norm(), 1);
	assertFloat(v2.norm(), 1);
	assertFloat(v3.norm(), 1);

	std::cout << "PASSED!" << std::endl;
}

void dotTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v1(0, 1);
	CGJM::Vec2 v2(2, 0);

	CGJM::Vec2 v3(0, 1);
	CGJM::Vec2 v4(0, 2);


	CGJM::Vec2 v5(0, 1);
	CGJM::Vec2 v6(0, -1);

	CGJM::Vec2 v7(1, 1);

	float dot_12 = v1.norm()*v2.norm()*std::cos(M_PI/2);
	assertFloat(CGJM::dot(v1, v2), dot_12);

	float dot_34 = v3.norm()*v4.norm()*std::cos(0);
	assertFloat(CGJM::dot(v3, v4), dot_34);


	float dot_56 = v5.norm()*v6.norm()*std::cos(M_PI);
	assertFloat(CGJM::dot(v5, v6), dot_56);


	float dot_17 = CGJM::dot(v1, v7);
	float angle = std::acos(dot_17/(v1.norm()*v7.norm()));
	assertFloat(angle, M_PI/4);

	std::cout << "PASSED!" << std::endl;
}

void crossTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 v1(1, 0, 0);
	CGJM::Vec3 v2(0, 1, 0);
	CGJM::Vec3 v3;

	v3 = CGJM::cross(v1, v2);

	assertFloat(v3[0], 0);
	assertFloat(v3[1], 0);
	assertFloat(v3[2], 1);

	std::cout << "PASSED!" << std::endl;
}

int main(int argc, char** argv){
	
	accessEmptyConstructorTest();
	accessFloatConstructorTest();
	accessVecConstructorTest();

	directAssignmentTest();

	outputTest();
	
	assignmentTest();

	addTest();
	subtractionTest();
	multiplicationTest();

	inputStreamTest();

	algebraicPropertiesTest();

	quadranceTest();
	normTest();
	normalizeTest();

	dotTest();

	crossTest();

	std::cout << "All tests passed!!" << std::endl;
	return 0;
}


