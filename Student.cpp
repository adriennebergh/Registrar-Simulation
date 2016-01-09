#include <iostream>
#include "student.h"

using namespace std;

Student::Student(int tA, int tW) {
	arriveTime = tA;
	windowTime = tW;
	waitTime = 0;
}
