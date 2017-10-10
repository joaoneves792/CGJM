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
void mat4Test(){
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

void assertMatrix(Mat4 output, Mat4 expected){
	if(output!=expected){
		std::cout << "Assertion failed!\n" << "Expected:" << std::endl << expected << "Got: " << std::endl << output << std::endl;
		exit(1);
	}

}


/*Mat4 Tests
 * ------------------------------------------------
 */

void mat4accessEmptyConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A;
	assertFloat(A[0][0], 0);
	assertFloat(A[0][1], 0);
	assertFloat(A[0][2], 0);
	assertFloat(A[0][3], 0);
	assertFloat(A[1][0], 0);
	assertFloat(A[1][1], 0);
	assertFloat(A[1][2], 0);
	assertFloat(A[1][3], 0);
	assertFloat(A[2][0], 0);
	assertFloat(A[2][1], 0);
	assertFloat(A[2][2], 0);
	assertFloat(A[2][3], 0);
	assertFloat(A[3][0], 0);
	assertFloat(A[3][1], 0);
	assertFloat(A[3][2], 0);
	assertFloat(A[3][3], 0);

	std::cout << "PASSED!" << std::endl;
}

void mat4accessFloatConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

	int a = 1;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			assertFloat(A[i][j], a++);
		}
	}
	
	std::cout << "PASSED!" << std::endl;
}

void mat4DiagonalConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(1);

	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(i==j)
				assertFloat(A[i][j], 1);
			else
				assertFloat(A[i][j], 0);
		}
	}

	std::cout << "PASSED!" << std::endl;
}

void mat4outputTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(1);
	std::ostringstream ss;
	ss << A;
	std::ostringstream expected;
	expected << "| 1 0 0 0 |" << std::endl << "| 0 1 0 0 |" << std::endl << "| 0 0 1 0 |" <<std::endl << "| 0 0 0 1 |" << std::endl;
	assertString(ss.str(), expected.str());
	
	std::cout << "PASSED!" << std::endl;
}

void mat4MatCopyConstructorTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CGJM::Mat4 B(A);

	int a = 1;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			assertFloat(A[i][j], a++);
		}
	}

	std::cout << "PASSED!" << std::endl;
}

void mat4AssignmentTest(){
	std::cout << __func__ << " ";

	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CGJM::Mat4 B;

	B = A;

	int a = 1;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			assertFloat(B[i][j], a++);
		}
	}

	std::cout << "PASSED!" << std::endl;
}

void mat4ComparisonTest(){
	std::cout << __func__ << " ";

	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CGJM::Mat4 B(3);
	CGJM::Mat4 C(0);
	CGJM::Mat4 D;
	CGJM::Mat4 E(1, 2, 3, 4, 5, 4, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

	assertTrue(A==A);
	assertTrue(A!=B);
	assertTrue(A!=C);
	assertTrue(A!=E);

	D = A;

	assertTrue(A==D);

	std::cout << "PASSED!" << std::endl;
}


void mat4AddSubTest(){
	std::cout << __func__ << " ";

	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CGJM::Mat4 B(16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);

	CGJM::Mat4 result;

	result = A + B;

	CGJM::Mat4 Expected(17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17);

	assertMatrix(result, Expected);

	result = result - B;

	assertMatrix(result, A);

	std::cout << "PASSED!" << std::endl;
}

void mat4ScalarProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CGJM::Mat4 result;

	result = 3*A;

	CGJM::Mat4 Expected(3*1, 3*2, 3*3, 3*4, 3*5, 3*6, 3*7, 3*8, 3*9, 3*10, 3*11, 3*12, 3*13, 3*14, 3*15, 3*16);

	assertMatrix(result, Expected);

	result = CGJM::Mat4(1);
	result = A*3;
	
	assertMatrix(result, Expected);
	
	std::cout << "PASSED!" << std::endl;
}

void mat4mat4ProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CGJM::Mat4 B(16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	CGJM::Mat4 Expected1(80, 70, 60, 50, 240, 214, 188, 162, 400, 358, 316, 274, 560, 502, 444, 386);
	CGJM::Mat4 Expected2(386, 444, 502, 560, 274, 316, 358, 400, 162, 188, 214, 240, 50, 60, 70, 80);

	CGJM::Mat4 result;

	result = A*B;
	
	assertMatrix(result, Expected1);
	result = B*A;

	
	assertMatrix(result, Expected2);

	result = A*CGJM::Mat4(1);
	
	assertMatrix(result, A);

	std::cout << "PASSED!" << std::endl;
}
/*
void mat4VectorProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(2, 0, 0,
		     0, 1, 0,
		     0, 0, 1); //x scale by 2
	CGJM::Vec3 v(1, 0, 0);

	CGJM::Vec3 result = A*v;

	assertFloat(result[0], 2);
	assertFloat(result[1], 0);
	assertFloat(result[2], 0);


	CGJM::Vec3 k(0, 1, 0); //Rotation axis (y)
	CGJM::Mat4 K(0, -k[2], k[1], k[2], 0, -k[0], -k[1], k[0], 0); //Dual Matrix

	CGJM::Mat4 R = CGJM::Mat4(1)+(std::sin(M_PI/2))*K+(1-std::cos(M_PI/2))*(K*K); //Rodrigues Rotation matrix for 90 degrees around x
	result = R*v;

	assertFloat(result[0], 0);
	assertFloat(result[1], 0);
	assertFloat(result[2], -1);


	CGJM::Vec3 v2(1, 0, 1); //2d vector homogeneous
	CGJM::Mat4 T(1, 0, 2,  //2D transformation
		     0, 1, 0,
		     0, 0, 1);

	result = T*v2;

	assertFloat(result[0], 3);
	assertFloat(result[1], 0);
	assertFloat(result[2], 1);

	std::cout << "PASSED!" << std::endl;
}
*/
void mat4TransposeTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CGJM::Mat4 E(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16);


	assertMatrix(A.transpose(), E);

	std::cout << "PASSED!" << std::endl;
}

void mat4InverseTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 I(1);
	for(int i=0; i<30; i++){

		CGJM::Mat4 A(randomNumber(-10, 10), randomNumber(-10, 10), randomNumber(-10, 10), randomNumber(-10, 10),
			     randomNumber(-10, 10), randomNumber(-10, 10), randomNumber(-10, 10), randomNumber(-10, 10),
			     randomNumber(-10, 10), randomNumber(-10, 10), randomNumber(-10, 10), randomNumber(-10, 10),
			     randomNumber(-10, 10), randomNumber(-10, 10), randomNumber(-10, 10), randomNumber(-10, 10)
			    );

		assertMatrix(A*A.inverse(), I);
	}
	std::cout << "PASSED!" << std::endl;
}


int main(){
	srand(static_cast <unsigned> (time(0)));
	
	mat4accessEmptyConstructorTest();
	mat4accessFloatConstructorTest();
	mat4DiagonalConstructorTest();
	mat4outputTest();
	mat4MatCopyConstructorTest();
	mat4AssignmentTest();
	mat4ComparisonTest();
	mat4AddSubTest();
	mat4ScalarProductTest();
	mat4mat4ProductTest();
	//mat4VectorProductTest();
	mat4TransposeTest();
	mat4InverseTest();

	std::cout << "All tests passed!! Press Enter to close..." << std::endl;


	std::string str;
	std::getline(std::cin, str);

	return 0;
}


