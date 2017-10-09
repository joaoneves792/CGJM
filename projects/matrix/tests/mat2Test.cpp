#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define _USE_MATH_DEFINES
#include <math.h>

#include "vec.h"
#include "mat.h"

/*Sample Test
void mat2Test(){
	std::cout << __func__ << " ";
	std::cout << "PASSED!" << std::endl;
}
*/


float randomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

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


/*Mat2 Tests
 * ------------------------------------------------
 */

void mat2accessEmptyConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat2 A;
	assertFloat(A[0][0], 0);
	assertFloat(A[0][1], 0);
	assertFloat(A[1][0], 0);
	assertFloat(A[1][1], 0);

	std::cout << "PASSED!" << std::endl;
}

void mat2accessFloatConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat2 A(4.5, 3.3, 2.2, 1.1);
	assertFloat(A[0][0], 4.5);
	assertFloat(A[0][1], 3.3);
	assertFloat(A[1][0], 2.2);
	assertFloat(A[1][1], 1.1);
	
	std::cout << "PASSED!" << std::endl;
}

void mat2DiagonalConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat2 A(1);

	assertFloat(A[0][0], 1);
	assertFloat(A[0][1], 0);
	assertFloat(A[1][0], 0);
	assertFloat(A[1][1], 1);

	std::cout << "PASSED!" << std::endl;
}

void mat2outputTest(){
	std::cout << __func__ << " ";
	CGJM::Mat2 A(1);
	std::ostringstream ss;
	ss << A;
	std::ostringstream expected;
	expected << "| 1 0 |" << std::endl << "| 0 1 |" << std::endl;
	assertString(ss.str(), expected.str());
	
	std::cout << "PASSED!" << std::endl;
}

void mat2MatCopyConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat2 A(0, 1, 2, 3);
	CGJM::Mat2 B(A);

	assertFloat(B[0][0], 0);
	assertFloat(B[0][1], 1);
	assertFloat(B[1][0], 2);
	assertFloat(B[1][1], 3);

	std::cout << "PASSED!" << std::endl;
}

void mat2AssignmentTest(){
	std::cout << __func__ << " ";

	CGJM::Mat2 A(0, 1, 2, 3);
	CGJM::Mat2 B;

	B = A;

	assertFloat(B[0][0], 0);
	assertFloat(B[0][1], 1);
	assertFloat(B[1][0], 2);
	assertFloat(B[1][1], 3);


	std::cout << "PASSED!" << std::endl;
}

void mat2ComparisonTest(){
	std::cout << __func__ << " ";

	CGJM::Mat2 A(0, 1, 2, 3);
	CGJM::Mat2 B(3);
	CGJM::Mat2 C(0);
	CGJM::Mat2 D;

	assertTrue(A==A);
	assertTrue(A!=B);
	assertTrue(A!=C);

	D = A;

	assertTrue(A==D);

	std::cout << "PASSED!" << std::endl;
}


void mat2AddSubTest(){
	std::cout << __func__ << " ";

	CGJM::Mat2 A(0, 1, 2, 3);
	CGJM::Mat2 B(7, 6, 5, 4);

	CGJM::Mat2 result;

	result = A + B;

	CGJM::Mat2 Expected(7, 7, 7, 7);

	assertTrue(result==Expected);

	result = result - B;

	assertTrue(result == A);

	std::cout << "PASSED!" << std::endl;
}

void mat2ScalarProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat2 A(0, 1, 2, 3);
	CGJM::Mat2 result;

	result = 3*A;

	CGJM::Mat2 Expected(3*0, 3*1, 3*2, 3*3);

	assertTrue(result==Expected);

	result = CGJM::Mat2(1);
	result = A*3;
	
	assertTrue(result==Expected);
	
	
	std::cout << "PASSED!" << std::endl;
}

void mat2mat2ProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat2 A(1, 2, 3, 4);
	CGJM::Mat2 B(2, 0, 1, 2);
	CGJM::Mat2 Expected1(4, 4, 10, 8);
	CGJM::Mat2 Expected2(2, 4, 7, 10);

	CGJM::Mat2 result;

	result = A*B;

	assertTrue(result==Expected1);

	result = B*A;

	assertTrue(result==Expected2);

	std::cout << "PASSED!" << std::endl;
}

void mat2VectorProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat2 A(2, 0, 0, 1); //x scale by 2
	CGJM::Vec2 v(1, 0);

	CGJM::Vec2 result = A*v;

	assertFloat(result[0], 2);
	assertFloat(result[1], 0);


	CGJM::Mat2 B(std::cos(M_PI/2), -std::cos(M_PI/2), std::sin(M_PI/2), std::cos(M_PI/2));// rotate 90 degrees

	result = B*v;

	assertFloat(result[0], 0);
	assertFloat(result[1], 1);


	std::cout << "PASSED!" << std::endl;
}

void mat2TransposeTest(){
	std::cout << __func__ << " ";
	CGJM::Mat2 A(1, 2, 3, 4);
	CGJM::Mat2 E(1, 3, 2, 4);

	assertTrue(A.transpose()==E);

	std::cout << "PASSED!" << std::endl;
}

void mat2InverseTest(){
	std::cout << __func__ << " ";
	CGJM::Mat2 A(4.3, -5.2, 100, -43.3);
	CGJM::Mat2 I(1);

	assertTrue(A*A.inverse()==I);

	std::cout << "PASSED!" << std::endl;
}


int main(){
	
	mat2accessEmptyConstructorTest();
	mat2accessFloatConstructorTest();
	mat2DiagonalConstructorTest();
	mat2outputTest();
	mat2MatCopyConstructorTest();
	mat2AssignmentTest();
	mat2ComparisonTest();
	mat2AddSubTest();
	mat2ScalarProductTest();
	mat2mat2ProductTest();
	mat2VectorProductTest();
	mat2TransposeTest();
	mat2InverseTest();

	std::cout << "All tests passed!! Press Enter to close..." << std::endl;


	std::string str;
	std::getline(std::cin, str);

	return 0;
}


