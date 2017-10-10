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

void assertMatrix(Mat4 output, Mat4 expected, int line){
	if(output!=expected){
		std::cout << "Assertion failed! at line " << line << "\n" << "Expected:" << std::endl << expected << "Got: " << std::endl << output << std::endl;
		exit(1);
	}

}
void assertVector(Vec3 output, Vec3 expected){
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

	assertMatrix(result, Expected, __LINE__);

	result = result - B;

	assertMatrix(result, A, __LINE__);

	std::cout << "PASSED!" << std::endl;
}

void mat4ScalarProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CGJM::Mat4 result;

	result = 3*A;

	CGJM::Mat4 Expected(3*1, 3*2, 3*3, 3*4, 3*5, 3*6, 3*7, 3*8, 3*9, 3*10, 3*11, 3*12, 3*13, 3*14, 3*15, 3*16);

	assertMatrix(result, Expected, __LINE__);

	result = CGJM::Mat4(1);
	result = A*3;
	
	assertMatrix(result, Expected, __LINE__);
	
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
	
	assertMatrix(result, Expected1, __LINE__);
	result = B*A;

	
	assertMatrix(result, Expected2, __LINE__);

	result = A*CGJM::Mat4(1);
	
	assertMatrix(result, A, __LINE__);

	std::cout << "PASSED!" << std::endl;
}

void mat4VectorProductTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 S = CGJM::scale(2, 0, 0);
	CGJM::Vec3 v(1, 0, 0);
	CGJM::Vec3 result = S*v;

	assertFloat(result[0], 2);
	assertFloat(result[1], 0);
	assertFloat(result[2], 0);


	CGJM::Mat4 R = CGJM::rotate(Vec3(0, 1, 0), M_PI/2);
	result = R*v;

	assertFloat(result[0], 0);
	assertFloat(result[1], 0);
	assertFloat(result[2], -1);


	CGJM::Mat4 T = CGJM::translate(2, 0, 0);
	result = T*v;

	assertFloat(result[0], 3);
	assertFloat(result[1], 0);
	assertFloat(result[2], 0);

	std::cout << "PASSED!" << std::endl;
}

void mat4TransposeTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CGJM::Mat4 E(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16);


	assertMatrix(A.transpose(), E, __LINE__);

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

		assertMatrix(A*A.inverse(), I, __LINE__);
	}
	std::cout << "PASSED!" << std::endl;
}

void mat4TransformsTest(){
	std::cout << __func__ << " ";
	CGJM::Vec3 v1(1, 0, 0);
	CGJM::Vec3 v2(0, 1, 0);
	CGJM::Vec3 v3(0, 0, 1);

	CGJM::Mat4 S = CGJM::scale(2, 2, 2);
	CGJM::Mat4 R = CGJM::rotate(Vec3(0, 1, 0), M_PI/2);
	CGJM::Mat4 T = CGJM::translate(2, 0, 0);

	CGJM::Vec3 result = S*T*R*v1;
	assertVector(result, Vec3(4, 0, -2));
	
	result = S*T*R*v2;
	assertVector(result, Vec3(4, 2, 0));
	
	result = S*T*R*v2;
	assertVector(result, Vec3(4, 2, 0));

	result = R*T*v1;
	assertVector(result, Vec3(0, 0, -3));

	std::cout << "PASSED!" << std::endl;
}

void mat4AlgebraicPropertiesTest(){
	std::cout << __func__ << " ";
	CGJM::Mat4 A(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CGJM::Mat4 B(16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	CGJM::Mat4 C(34, 12, 23, 15, 42, 14, 56, 7, 1, 3, 5, 2, 4, 1, 3, 7);


	assertMatrix(A+B, B+A, __LINE__);
	assertMatrix((A+B)+C, A+(B+C), __LINE__);
	assertMatrix(A+CGJM::Mat4(0), A, __LINE__);

	assertMatrix((A*B)*C, A*(B*C), __LINE__);
	assertMatrix(4*(3*A), (4*3)*A, __LINE__);
	assertMatrix(4*(A*B), (4*A)*B, __LINE__);
	assertMatrix((4*A)*B, A*(4*B), __LINE__);
	assertMatrix(A*CGJM::Mat4(0), CGJM::Mat4(0), __LINE__);

	assertMatrix((A+B)*C, A*C+B*C, __LINE__);
	assertMatrix(A*(B+C), A*B+A*C, __LINE__);

	assertMatrix(4*(A+B), 4*A+4*B, __LINE__);
	assertMatrix((4+3)*A, 4*A+3*A, __LINE__);

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
	mat4VectorProductTest();
	mat4TransposeTest();
	mat4InverseTest();

	mat4TransformsTest();
	mat4AlgebraicPropertiesTest();


	std::cout << "All tests passed!! Press Enter to close..." << std::endl;


	std::string str;
	std::getline(std::cin, str);

	return 0;
}


