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
void mat3Test(){
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


/*Mat3 Tests
 * ------------------------------------------------
 */

void mat3accessEmptyConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat3 A;
	assertFloat(A[0][0], 0);
	assertFloat(A[0][1], 0);
	assertFloat(A[0][2], 0);
	assertFloat(A[1][0], 0);
	assertFloat(A[1][1], 0);
	assertFloat(A[1][2], 0);
	assertFloat(A[2][0], 0);
	assertFloat(A[2][1], 0);
	assertFloat(A[2][2], 0);

	std::cout << "PASSED!" << std::endl;
}

void mat3accessFloatConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat3 A(1, 2, 3, 4, 5, 6, 7, 8, 9);
	
	assertFloat(A[0][0], 1);
	assertFloat(A[0][1], 2);
	assertFloat(A[0][2], 3);
	assertFloat(A[1][0], 4);
	assertFloat(A[1][1], 5);
	assertFloat(A[1][2], 6);
	assertFloat(A[2][0], 7);
	assertFloat(A[2][1], 8);
	assertFloat(A[2][2], 9);
	
	std::cout << "PASSED!" << std::endl;
}

void mat3DiagonalConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat3 A(1);

	assertFloat(A[0][0], 1);
	assertFloat(A[0][1], 0);
	assertFloat(A[0][2], 0);
	assertFloat(A[1][0], 0);
	assertFloat(A[1][1], 1);
	assertFloat(A[1][2], 0);
	assertFloat(A[2][0], 0);
	assertFloat(A[2][1], 0);
	assertFloat(A[2][2], 1);

	std::cout << "PASSED!" << std::endl;
}

void mat3outputTest(){
	std::cout << __func__ << " ";
	CGJM::Mat3 A(1);
	std::ostringstream ss;
	ss << A;
	std::ostringstream expected;
	expected << "| 1 0 0 |" << std::endl << "| 0 1 0 |" << std::endl << "| 0 0 1 |" <<std::endl;
	assertString(ss.str(), expected.str());
	
	std::cout << "PASSED!" << std::endl;
}

void mat3MatCopyConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat3 A(1, 2, 3, 4, 5, 6, 7, 8, 9);
	CGJM::Mat3 B(A);

	assertFloat(B[0][0], 1);
	assertFloat(B[0][1], 2);
	assertFloat(B[0][2], 3);
	assertFloat(B[1][0], 4);
	assertFloat(B[1][1], 5);
	assertFloat(B[1][2], 6);
	assertFloat(B[2][0], 7);
	assertFloat(B[2][1], 8);
	assertFloat(B[2][2], 9);

	std::cout << "PASSED!" << std::endl;
}

void mat3AssignmentTest(){
	std::cout << __func__ << " ";

	CGJM::Mat3 A(1, 2, 3, 4, 5, 6, 7, 8, 9);
	CGJM::Mat3 B;

	B = A;

	assertFloat(B[0][0], 1);
	assertFloat(B[0][1], 2);
	assertFloat(B[0][2], 3);
	assertFloat(B[1][0], 4);
	assertFloat(B[1][1], 5);
	assertFloat(B[1][2], 6);
	assertFloat(B[2][0], 7);
	assertFloat(B[2][1], 8);
	assertFloat(B[2][2], 9);


	std::cout << "PASSED!" << std::endl;
}

void mat3ComparisonTest(){
	std::cout << __func__ << " ";

	CGJM::Mat3 A(1, 2, 3, 4, 5, 6, 7, 8, 9);
	CGJM::Mat3 B(3);
	CGJM::Mat3 C(0);
	CGJM::Mat3 D;

	assertTrue(A==A);
	assertTrue(A!=B);
	assertTrue(A!=C);

	D = A;

	assertTrue(A==D);

	std::cout << "PASSED!" << std::endl;
}


void mat3AddSubTest(){
	std::cout << __func__ << " ";

	CGJM::Mat3 A(1, 2, 3, 4, 5, 6, 7, 8, 9);
	CGJM::Mat3 B(9, 8, 7, 6, 5, 4, 3, 2, 1);

	CGJM::Mat3 result;

	result = A + B;

	CGJM::Mat3 Expected(10, 10, 10, 10, 10, 10, 10, 10, 10);

	assertTrue(result==Expected);

	result = result - B;

	assertTrue(result == A);

	std::cout << "PASSED!" << std::endl;
}

void mat3ScalarProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat3 A(1, 2, 3, 4, 5, 6, 7, 8, 9);
	CGJM::Mat3 result;

	result = 3*A;

	CGJM::Mat3 Expected(3*1, 3*2, 3*3, 3*4, 3*5, 3*6, 3*7, 3*8, 3*9);

	assertTrue(result==Expected);

	result = CGJM::Mat3(1);
	result = A*3;
	
	assertTrue(result==Expected);
	
	
	std::cout << "PASSED!" << std::endl;
}

void mat3mat3ProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat3 A(1, 2, 3, 4, 5, 6, 7, 8, 9);
	CGJM::Mat3 B(9, 8, 7, 6, 5, 4, 3, 2, 1);
	CGJM::Mat3 Expected1(30, 24, 18, 84, 69, 54, 138, 114, 90);
	CGJM::Mat3 Expected2(90, 114, 138, 54, 69, 84, 18, 24, 30);

	CGJM::Mat3 result;

	result = A*B;

	assertTrue(result==Expected1);

	result = B*A;

	assertTrue(result==Expected2);

	result = A*CGJM::Mat3(1);

	assertTrue(result==A);

	std::cout << "PASSED!" << std::endl;
}

void mat3VectorProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat3 A(2, 0, 0,
		     0, 1, 0,
		     0, 0, 1); //x scale by 2
	CGJM::Vec3 v(1, 0, 0);

	CGJM::Vec3 result = A*v;

	assertFloat(result[0], 2);
	assertFloat(result[1], 0);
	assertFloat(result[2], 0);


	CGJM::Vec3 k(0, 1, 0); //Rotation axis (y)
	CGJM::Mat3 K(0, -k[2], k[1], k[2], 0, -k[0], -k[1], k[0], 0); //Dual Matrix

	CGJM::Mat3 R = CGJM::Mat3(1)+(std::sin(M_PI/2))*K+(1-std::cos(M_PI/2))*(K*K); //Rodrigues Rotation matrix for 90 degrees around x
	result = R*v;

	assertFloat(result[0], 0);
	assertFloat(result[1], 0);
	assertFloat(result[2], -1);


	CGJM::Vec3 v2(1, 0, 1); //2d vector homogeneous
	CGJM::Mat3 T(1, 0, 2,  //2D transformation
		     0, 1, 0,
		     0, 0, 1);

	result = T*v2;

	assertFloat(result[0], 3);
	assertFloat(result[1], 0);
	assertFloat(result[2], 1);

	std::cout << "PASSED!" << std::endl;
}

void mat3TransposeTest(){
	std::cout << __func__ << " ";
	CGJM::Mat3 A(1, 2, 3, 4, 5, 6, 7, 8, 9);
	CGJM::Mat3 E(1, 4, 7, 2, 5, 8, 3, 6, 9);

	assertTrue(A.transpose()==E);

	std::cout << "PASSED!" << std::endl;
}

void mat3InverseTest(){
	std::cout << __func__ << " ";
	CGJM::Mat3 A(4.3, -5.2, 100, -43.3, 654, 23, 62, 74.99, 2);
	CGJM::Mat3 I(1);

	assertTrue(A*A.inverse()==I);

	std::cout << "PASSED!" << std::endl;
}


int main(){
	
	mat3accessEmptyConstructorTest();
	mat3accessFloatConstructorTest();
	mat3DiagonalConstructorTest();
	mat3outputTest();
	mat3MatCopyConstructorTest();
	mat3AssignmentTest();
	mat3ComparisonTest();
	mat3AddSubTest();
	mat3ScalarProductTest();
	mat3mat3ProductTest();
	mat3VectorProductTest();
	mat3TransposeTest();
	mat3InverseTest();

	std::cout << "All tests passed!! Press Enter to close..." << std::endl;


	std::string str;
	std::getline(std::cin, str);

	return 0;
}


