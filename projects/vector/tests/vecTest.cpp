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


/*Vec2 Tests
 * ------------------------------------------------
 */

void vec2accessEmptyConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v;
	assertFloat(v[0], 0);
	assertFloat(v[1], 0);

	std::cout << "PASSED!" << std::endl;
}

void vec2accessFloatConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v(4.5, 3.3);
	assertFloat(v[0], 4.5);
	assertFloat(v[1], 3.3);
	
	std::cout << "PASSED!" << std::endl;
}

void vec2accessVecConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v1(7, 8);
	CGJM::Vec2 v2(v1);

	assertFloat(v2[0], 7);
	assertFloat(v2[0], v1[0]);
	
	assertFloat(v2[1], 8);
	assertFloat(v2[1], v1[1]);

	std::cout << "PASSED!" << std::endl;
}

void vec2directAssignmentTest(){
	std::cout << __func__ << " ";

	CGJM::Vec2 v(1, 2);
	v[0] = 3;
	v[1] = 4;

	assertFloat(v[0], 3);
	assertFloat(v[1], 4);

	std::cout << "PASSED!" << std::endl;
}

void vec2outputTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v(3.4, 7.3);
	std::ostringstream ss;
	ss << v;
	assertString(ss.str(), "[ 3.4 7.3 ]");
	
	std::cout << "PASSED!" << std::endl;
}

void vec2assignmentTest(){
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


void vec2addTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 sum;
	CGJM::Vec2 v1(1, 2);
	CGJM::Vec2 v2(3, 4);

	sum = v1 + v2;

	assertFloat(sum[0], 4);
	assertFloat(sum[1], 6);

	std::cout << "PASSED!" << std::endl;
}

void vec2subtractionTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 sub;
	CGJM::Vec2 v1(1, 2);
	CGJM::Vec2 v2(3, 5);

	sub = v1 - v2;

	assertFloat(sub[0], -2);
	assertFloat(sub[1], -3);

	std::cout << "PASSED!" << std::endl;
}

void vec2multiplicationTest(){
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


void vec2inputStreamTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v;

	std::istringstream* iss = new std::istringstream("[ 3 4 ]\n"); 
	(*iss) >> v;

	assertFloat(v[0], 3);
	assertFloat(v[1], 4);
	delete iss;


	iss = new std::istringstream(" 5 6 \n"); 
	(*iss) >> v;

	assertFloat(v[0], 5);
	assertFloat(v[1], 6);
	delete iss;
	
	iss = new std::istringstream("7 8\n"); 
	(*iss) >> v;

	assertFloat(v[0], 7);
	assertFloat(v[1], 8);
	delete iss;
	std::cout << "PASSED!" << std::endl;
}

void vec2algebraicPropertiesTest(){
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

void vec2quadranceTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v1(1, 1);
	CGJM::Vec2 v2(2, 2);

	assertTrue((v1.quadrance() < v2.quadrance()));

	std::cout << "PASSED!" << std::endl;
}


void vec2normTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v1(0, 1);
	CGJM::Vec2 v2(2, 0);

	CGJM::Vec2 v3(3, 3);

	assertFloat(v1.norm(), 1);
	assertFloat(v2.norm(), 2);

	assertFloat(v3.norm(), std::sqrt(18));

	std::cout << "PASSED!" << std::endl;
}


void vec2normalizeTest(){
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

void vec2dotTest(){
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

void vec2crossTest(){
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

/*Vec3 Tests
 * ------------------------------------------------
 */

void vec3accessEmptyConstructorTest(){
	std::cout << __func__ << " ";

	CGJM::Vec3 v;
	assertFloat(v[0], 0);
	assertFloat(v[1], 0);
	assertFloat(v[2], 0);
	std::cout << "PASSED!" << std::endl;
}

void vec3accessFloatConstructorTest(){
	std::cout << __func__ << " ";
	
	CGJM::Vec3 v(4.5, 3.3, 8.8);
	assertFloat(v[0], 4.5);
	assertFloat(v[1], 3.3);
	assertFloat(v[2], 8.8);
	std::cout << "PASSED!" << std::endl;
}

void vec3accessVecConstructorTest(){
	std::cout << __func__ << " ";

	CGJM::Vec3 v1(7, 8, 9);
	CGJM::Vec3 v2(v1);

	assertFloat(v2[0], 7);
	assertFloat(v2[0], v1[0]);
	
	assertFloat(v2[1], 8);
	assertFloat(v2[1], v1[1]);
	
	assertFloat(v2[2], 9);
	assertFloat(v2[2], v1[2]);


	std::cout << "PASSED!" << std::endl;
}

void vec3directAssignmentTest(){
	std::cout << __func__ << " ";

	CGJM::Vec3 v(1, 2, 3);
	v[0] = 4;
	v[1] = 5;
	v[2] = 6;

	assertFloat(v[0], 4);
	assertFloat(v[1], 5);
	assertFloat(v[2], 6);

	std::cout << "PASSED!" << std::endl;
}

void vec3outputTest(){
	std::cout << __func__ << " ";
	
	CGJM::Vec3 v(3.4, 7.3, 6.6);
	std::ostringstream ss;
	ss << v;
	assertString(ss.str(), "[ 3.4 7.3 6.6 ]");
	std::cout << "PASSED!" << std::endl;
}

void vec3assignmentTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 v1(7, 8, 9);
	CGJM::Vec3 v2(10, 11, 12);

	v1=v2;

	assertFloat(v1[0], 10);
	assertFloat(v1[0], v2[0]);

	assertFloat(v1[1], 11);
	assertFloat(v1[1], v2[1]);
	
	assertFloat(v1[2], 12);
	assertFloat(v1[2], v2[2]);
	std::cout << "PASSED!" << std::endl;
}


void vec3addTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 sum;
	CGJM::Vec3 v1(1, 2, 3);
	CGJM::Vec3 v2(3, 4, 5);

	sum = v1 + v2;

	assertFloat(sum[0], 4);
	assertFloat(sum[1], 6);
	assertFloat(sum[2], 8);

	std::cout << "PASSED!" << std::endl;
}

void vec3subtractionTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 sub;
	CGJM::Vec3 v1(1, 2, 3);
	CGJM::Vec3 v2(3, 5, 6);

	sub = v1 - v2;

	assertFloat(sub[0], -2);
	assertFloat(sub[1], -3);
	assertFloat(sub[2], -3);

	std::cout << "PASSED!" << std::endl;
}

void vec3multiplicationTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 product;
	CGJM::Vec3 v(3, 2, 1);

	product = v * 3;

	assertFloat(product[0], 9);
	assertFloat(product[1], 6);
	assertFloat(product[2], 3);

	product = 2 * v;
	
	assertFloat(product[0], 6);
	assertFloat(product[1], 4);
	assertFloat(product[2], 2);

	std::cout << "PASSED!" << std::endl;
}


void vec3inputStreamTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 v;

	std::istringstream* iss = new std::istringstream("[ 3 4 5 ]\n");
	(*iss) >> v;

	assertFloat(v[0], 3);
	assertFloat(v[1], 4);
	assertFloat(v[2], 5);
	delete iss;


	iss = new std::istringstream(" 5 6 7 \n"); 
	(*iss) >> v;

	assertFloat(v[0], 5);
	assertFloat(v[1], 6);
	assertFloat(v[2], 7);
	delete iss;
	
	iss = new std::istringstream("7 8 9\n"); 
	(*iss) >> v;

	assertFloat(v[0], 7);
	assertFloat(v[1], 8);
	assertFloat(v[2], 9);
	delete iss;

	std::cout << "PASSED!" << std::endl;
}

void vec3algebraicPropertiesTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 v1(1, 2, 3);
	CGJM::Vec3 v2(3, 4, 5);
	CGJM::Vec3 v3(5, 6, 7);


	/*Commutative*/
	CGJM::Vec3 s1;
	CGJM::Vec3 s2;
	
	s1 = v1 + v2;
	s2 = v2 + v1;
	assertFloat(s1[0], s2[0]);
	assertFloat(s1[1], s2[1]);
	assertFloat(s1[2], s2[2]);

	/*Associative*/
	s1 = (v1 + v2) + v3;
	s2 = v1 + (v2 + v3);
	assertFloat(s1[0], s2[0]);
	assertFloat(s1[1], s2[1]);
	assertFloat(s1[2], s2[2]);


	/*Distributive (vector)*/
	s1 = 3 * (v1 + v2);
	s2 = 3 * v1 + 3 * v2;
	assertFloat(s1[0], s2[0]);
	assertFloat(s1[1], s2[1]);
	assertFloat(s1[2], s2[2]);

	/*Distributive (scalar)*/
	s1 = (4 + 5) * v1;
	s2 = 4 * v1 + 5* v1;
	assertFloat(s1[0], s2[0]);
	assertFloat(s1[1], s2[1]);
	assertFloat(s1[2], s2[2]);

	/*Associative (scalar)*/
	s1 = 6 * (7 * v1);
	s2 = (6 * 7) * v1;
	assertFloat(s1[0], s2[0]);
	assertFloat(s1[1], s2[1]);
	assertFloat(s1[2], s2[2]);

	std::cout << "PASSED!" << std::endl;
}

void vec3quadranceTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 v1(1, 1, 1);
	CGJM::Vec3 v2(2, 2, 2);

	assertTrue((v1.quadrance() < v2.quadrance()));

	std::cout << "PASSED!" << std::endl;
}


void vec3normTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 v1(0, 1, 0);
	CGJM::Vec3 v2(2, 0, 0);

	CGJM::Vec3 v3(3, 3, 3);

	assertFloat(v1.norm(), 1);
	assertFloat(v2.norm(), 2);

	assertFloat(v3.norm(), std::sqrt(27));

	std::cout << "PASSED!" << std::endl;
}


void vec3normalizeTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 v1(0, 1, 0);
	CGJM::Vec3 v2(2, 0, 0);
	CGJM::Vec3 v3(3, 3, 45);

	v1.normalize();
	v2.normalize();
	v3.normalize();

	assertFloat(v1.norm(), 1);
	assertFloat(v2.norm(), 1);
	assertFloat(v3.norm(), 1);

	std::cout << "PASSED!" << std::endl;
}

void vec3dotTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 v1(0, 1, 0);
	CGJM::Vec3 v2(2, 0, 0);

	CGJM::Vec3 v3(0, 1, 0);
	CGJM::Vec3 v4(0, 2, 0);


	CGJM::Vec3 v5(0, 1, 0);
	CGJM::Vec3 v6(0, -1, 0);

	CGJM::Vec3 v7(1, 1, 0);

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

void vec3crossTest(){
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
int main(){
	
	vec2accessEmptyConstructorTest();
	vec2accessFloatConstructorTest();
	vec2accessVecConstructorTest();
	vec2directAssignmentTest();
	vec2outputTest();
	vec2assignmentTest();
	vec2addTest();
	vec2subtractionTest();
	vec2multiplicationTest();
	vec2inputStreamTest();
	vec2algebraicPropertiesTest();
	vec2quadranceTest();
	vec2normTest();
	vec2normalizeTest();
	vec2dotTest();

	vec3accessEmptyConstructorTest();
	vec3accessFloatConstructorTest();
	vec3accessVecConstructorTest();
	vec3directAssignmentTest();
	vec3outputTest();
	vec3assignmentTest();
	vec3addTest();
	vec3subtractionTest();
	vec3multiplicationTest();
	vec3inputStreamTest();
	vec3algebraicPropertiesTest();
	vec3quadranceTest();
	vec3normTest();
	vec3normalizeTest();
	vec3dotTest();
	vec3crossTest();
	std::cout << "All tests passed!! Press Enter to close..." << std::endl;

	std::string str;
	std::getline(std::cin, str);

	return 0;
}


