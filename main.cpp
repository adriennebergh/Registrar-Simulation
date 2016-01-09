#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "listnode.h"
#include "queue.h"
#include "student.h"

using namespace std;

bool windowsEmpty(int *a, int size) {
	for(int i=0; i<size; i++) {
		if(a[i] != 0) {
			return false;
		}
	}
	return true;
}
//function to sort array of wait times to find median
int compare(const void * a, const void * b) {
	return ( *(int*)a - *(int*)b );
}

int main(int argc, char **argv) {

	string fileName;
	fileName = argv[1];
	ifstream file;
	file.open(fileName);
	//test to make sure file has opened. If not, re-enter file name.
	while (!file.is_open()) {
		cout << "Error opening file." << endl;
		cout << "Re-type file name: " << endl;
		getline(cin, fileName);
		file.open(fileName);
	}
	//create a queue of students to represent the line in registrar's office
	GenQueue<Student*> *queue = new GenQueue<Student*>();
	
	int numberWindows;
	string line;
	getline(file, line);
	numberWindows = stoi(line);

	//create an array to represent the available windows in registrar's office
	int *windowArray = new int[numberWindows];
	for(int i=0; i<numberWindows; i++) {
		windowArray[i] = 0;
	}

	int totalNumberStudents = 0;
	//create an object for each student accounted for in the file
	while(getline(file, line)) {
		int clockTick = stoi(line);

		getline(file, line);
		int numStudentsArrived = stoi(line);
		totalNumberStudents += numStudentsArrived;

		for(int i=0; i<numStudentsArrived; i++) {
			getline(file, line);
			int windowTime = stoi(line);
			//each student initialized with their time of arrival and the time they'll need at a window
			Student *s = new Student(clockTick, windowTime);

			queue->insert(s);
		}
	}

	//create a list to hold students who have exited the line
	DLinkedList<Student*> *studentsHelped = new DLinkedList<Student*>();

	int currentTime = 0;
	int sumIdleTimes = 0;
	bool allWindowsFull;
	int numWindowsFull;

	//run the simulation until BOTH the line is empty and all of the windows are empty
	while(!(queue->isEmpty()) || (numWindowsFull != 0)) { 
		numWindowsFull = 0;
		allWindowsFull = false;

		for(int i=0; i<numberWindows; i++) {
			if(windowArray[i] != 0) {
				windowArray[i] -= 1; //for each clock tick, decrement time remaining at window for windows that are full 
				if(windowArray[i] != 0) {
					numWindowsFull++; //if the window still full, increment counter
				}
			}
		}

		if(numWindowsFull == numberWindows) {
			allWindowsFull = true;
		}

		//if there are still students in line
		if(!(queue->isEmpty())) {
			//while there are students in line and the current time is the arrival time of the next student in line
			//and there is still an available window, send a student to that window
			while(!(queue->isEmpty()) && (currentTime >= queue->peek()->arriveTime) && !allWindowsFull) { 
				
				for(int i=0; i<numberWindows; i++) {
					if(windowArray[i] == 0) {
						windowArray[i] = queue->peek()->windowTime;
						queue->peek()->timeLeaveLine = currentTime;
						//keep track of window number that student went to for metrics calculations later
						queue->peek()->windowNumber = i;
						numWindowsFull++; //once a student is sent to a window, fill a window
						break;
					}
				}
				//remove student from line
				studentsHelped->insertBack(queue->remove());
				//check if all windows are full now in case the next student in line has same arrival time as the one
				//that just went to a window (will affect the waitTime of the student still in line)
				if(numWindowsFull == numberWindows) {
					allWindowsFull = true;
				}
			}
			//increment wait time of student still in line
			if((currentTime >= queue->peek()->arriveTime) && allWindowsFull) {
				queue->peek()->waitTime += 1;
			}
		}

		//if any of the windows are empty, increment the idle time counter
		for(int i=0; i<numberWindows; i++) {
			if(windowArray[i] == 0) {
				sumIdleTimes++;
			}
		}
		currentTime++;
	}
	//END SIMULATION WHEN QUEUE IS EMPTY **AND** WINDOW ARRAY IS EMPTY (ALL INDEXES = 0)

	//CALCULATE METRICS

	//mean window idle time
	double meanWindowIdleTime = (double)sumIdleTimes/numberWindows;

	int *idleTimeArray = new int[numberWindows];

	//initialize idle time of all windows to be total time of simulation
	for(int i=0; i<numberWindows; ++i) {
		idleTimeArray[i] = currentTime;
	}

	//initialize wait time of each student to be 0
	int *waitTimeArray = new int[totalNumberStudents];
	for(int i=0; i<totalNumberStudents; i++) {
		waitTimeArray[i] = 0;
	}

	Student *student;
	while(!studentsHelped->isEmpty()) {

		//pull data from student to calculate metrics
		student = studentsHelped->removeFront();

		//fill idleTime array with idle times of each window
		int studentWindowNumber = student->windowNumber;
		for(int i=0; i<numberWindows; ++i) {
			if(studentWindowNumber == i) {
				//idle time of each window = total time of simulation - time that a student was at that window
				idleTimeArray[i] -= student->windowTime;
			}
		}

		//fill waitTime array with the wait times of each student
		for(int i=0; i<totalNumberStudents; ++i) {
			//studentsHelped->getSize() "numbers" each student since students are removed from the list as their data is farmed
			if(i == (studentsHelped->getSize())) {
				waitTimeArray[i] = student->waitTime;
			}
		}
	}

	//CALCULATE Window Idle Time metrics
	int longestWindowIdleTime = 0;
	int heldBy;
	int numberWindowsIdleOverFiveMinutes = 0;

	for(int i=0; i<numberWindows; ++i) {
		if(idleTimeArray[i] > longestWindowIdleTime) {
			longestWindowIdleTime = idleTimeArray[i];
			heldBy = i+1;
		}
		if(idleTimeArray[i] > 5) {
			numberWindowsIdleOverFiveMinutes++;
		}
	}

	//CALCULATE Student Wait Time metrics

	//mean student wait time
	int sumWaitTimes = 0;
	int longestWaitTime = 0;
	int numberStudentsWaitingOverTenMin = 0;
	for(int i=0; i<totalNumberStudents; i++) {
		sumWaitTimes += waitTimeArray[i];
		if(waitTimeArray[i] > longestWaitTime) {
			longestWaitTime = waitTimeArray[i];
		}
		if(waitTimeArray[i] > 10) {
			numberStudentsWaitingOverTenMin++;
		}
	}

	double meanStudentWaitTime = (double)sumWaitTimes/totalNumberStudents;

	//median student wait time
	//sort array of wait times
	qsort(waitTimeArray, totalNumberStudents, sizeof(int), compare);

	//find middle index (where median will occur)
	int middleIndex = (totalNumberStudents/2);
	int secondIndex = 0;
	//if the array has an even number of elements, find a second index (median = middle two / 2)
	if(middleIndex%2 == 0) {
		secondIndex = middleIndex-1;
	}
	double medianWaitTime;
	if(secondIndex == 0) {
		medianWaitTime = waitTimeArray[middleIndex];
	}
	else {
		medianWaitTime = (double)(waitTimeArray[middleIndex] + waitTimeArray[secondIndex])/2;
	}

	//Print metrics
	cout << "The mean student wait time is: " << meanStudentWaitTime << endl;
	cout << "The median student wait time is: " << medianWaitTime << endl;
	cout << "The longest student wait time is: " << longestWaitTime << endl;
	cout << "The number of students who waited over 10 minutes is: " << numberStudentsWaitingOverTenMin << endl;
	cout << "The mean window idle time is: " << meanWindowIdleTime << endl;
	cout << "The longest window idle time is: " << longestWindowIdleTime << endl;
	cout << "The number of windows idle over 5 minutes is: " << numberWindowsIdleOverFiveMinutes << endl;

	file.close();

	return 0;
}
