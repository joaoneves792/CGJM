#include <iostream>
#include <string>
#include <sstream>
#include "vec.h"

void assertString(std::string output, std::string expected){
	if(output.compare(expected)){
		std::cout << "Assertion failed!\n" << "Expected: " << expected << "\n" << "Got: " << output << "\n";
		exit(1);
	}
}

void assertFloat(float output, float expected){
#define EPSILON 0.001
	if((output - expected > EPSILON) && (expected - output) > EPSILON){
		std::cout << "Assertion failed!\n" << "Expected: " << expected << "\n" << "Got: " << output << "\n";
		exit(1);
	}
}


void accessEmptyConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v;
	assertFloat(v[0], 0);
	assertFloat(v[1], 0);
	std::cout << "PASSED!" << std::endl;
}

void accessFloatConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v(4.5, 3.3);
	assertFloat(v[0], 4.5);
	assertFloat(v[1], 3.3);
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
	std::cout << "PASSED!" << std::endl;
}

void outputTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2 v(3.4, 7.3);
	std::ostringstream ss;
	ss << v;
	assertString(ss.str(), "[ 3.4 7.3 ]");
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
	CGJM::Vec2* sum;
	CGJM::Vec2 v1(1, 2);
	CGJM::Vec2 v2(3, 4);

	sum = v1 + v2;

	assertFloat((*sum)[0], 4);
	assertFloat((*sum)[1], 6);

	delete sum;

	std::cout << "PASSED!" << std::endl;
}

void subtractionTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2* sub;
	CGJM::Vec2 v1(1, 2);
	CGJM::Vec2 v2(3, 5);

	sub = v1 - v2;

	assertFloat((*sub)[0], -2);
	assertFloat((*sub)[1], -3);

	delete sub;
	std::cout << "PASSED!" << std::endl;
}

void multiplicationTest(){
	std::cout << __func__ << " ";
	CGJM::Vec2* product;
	CGJM::Vec2 v(3, 2);

	product = v * 3;

	assertFloat((*product)[0], 9);
	assertFloat((*product)[1], 6);

	delete product;

	product = 2 * v;
	
	assertFloat((*product)[0], 6);
	assertFloat((*product)[1], 4);

	delete product;
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

int main(int argc, char** argv){
	
	accessEmptyConstructorTest();
	accessFloatConstructorTest();
	accessVecConstructorTest();

	outputTest();
	
	assignmentTest();

	addTest();
	subtractionTest();
	multiplicationTest();

	inputStreamTest();

	std::cout << "All tests passed!!\n";
	return 0;
}


