#ifndef STUDENT_H_
#define STUDENT_H_
#include <iostream>
using namespace std;

class Student {
public:
	
	Student();
	Student(int tA, int tW);
	~Student();

	int windowTime;
	int arriveTime;
	int timeLeaveLine;
	int windowNumber;
	int waitTime;

};

#endif /* STUDENT_H_ */
