// algorithm1_FCFS.c
// To compile this, you need to have ncurses installed
// to compile, do this: edit the "make" file and then type "make" in the console

#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include "schedulingAlgorithmAnimation.h"

#define DELAY 30000
#define TOT_PROCESSES 4

int main() {

	//Variables for the max coords of the terminal window
	int max_y = 0, max_x = 0;

	// Character to draw for the boxes of the processes
	char* toDraw = "*";

	// Total CPU time used
	int totalWaitingTime = 0;

	// Amount of remaining processes
	int remainingProcessesCounter = TOT_PROCESSES;

	// Array that holds the lower y coordinate of each box
	int arrLoY[TOT_PROCESSES];

	// Array that holds the lower x coordinate of each box
	int arrLoX[TOT_PROCESSES];

	// Array that holds the higher y coordinate of each box
	int arrHiY[TOT_PROCESSES];

	// Array that holds the higher x coordinate of each box
	int arrHiX[TOT_PROCESSES];

	// Array that holds the next x coordinate of each box
	int arrNext_x[TOT_PROCESSES];

	// Array that holds the direction of each box
	int arrDirection[TOT_PROCESSES];

	// lower y coordiante of the CPU
	int cpuLoY = 0;

	// lower x coordiante of the CPU
	int cpuLoX = 70;

	// higher y coordiante of the CPU
	int cpuHiY = 20;

	// higher x coordiante of the CPU
	int cpuHiX = 90;

	// Array to store the names of each of the TOT_PROCESSES processes
	char arrProcessesNames[TOT_PROCESSES][10];

	// Assigning names to the processes
	strcpy(arrProcessesNames[0], "A");
	strcpy(arrProcessesNames[1], "B");
	strcpy(arrProcessesNames[2], "C");
	strcpy(arrProcessesNames[3], "D");

	// Array to store the time each process requires
	int arrProcessTotalTime[TOT_PROCESSES] = {4, 3, 7, 1};

	// Array to store the time each process has consumed
	int arrProcessCurrentTime[TOT_PROCESSES] = {0, 0, 0, 0};

	// Auxiliary string to be used to convert an int to a string
	char intToStr[10];

	// Array to store the strings equivalent to the time each process requires
	char arrProcessesBurstTimes[TOT_PROCESSES][10];
	
	// Copying the string to the array
	sprintf(intToStr, "%d", arrProcessTotalTime[0]);
	strcpy(arrProcessesBurstTimes[0], intToStr);
	
	// Copying the string to the array
	sprintf(intToStr, "%d", arrProcessTotalTime[1]);
	strcpy(arrProcessesBurstTimes[1], intToStr);
	
	// Copying the string to the array	
	sprintf(intToStr, "%d", arrProcessTotalTime[2]);
	strcpy(arrProcessesBurstTimes[2], intToStr);
		
	// Copying the string to the array
	sprintf(intToStr, "%d", arrProcessTotalTime[3]);
	strcpy(arrProcessesBurstTimes[3], intToStr);

	// Array to store whether a process has already finished
	int processIsFinished[TOT_PROCESSES] = {0, 0, 0, 0};


	// Assigning coordinate values and direction to the box with index 3
	arrLoY[3] = 8;
	arrLoX[3] = 0;
	arrHiY[3] = 12;
	arrHiX[3] = 8;
	arrNext_x[3] = 0;
	arrDirection[3] = 1;

	// Assigning coordinate values and direction to the box with index 2
	arrLoY[2] = 8;
	arrLoX[2] = 12;
	arrHiY[2] = 12;
	arrHiX[2] = 20;
	arrNext_x[2] = 0;
	arrDirection[2] = 1;

	// Assigning coordinate values and direction to the box with index 1
	arrLoY[1] = 8;
	arrLoX[1] = 24;
	arrHiY[1] = 12;
	arrHiX[1] = 32;
	arrNext_x[1] = 0;
	arrDirection[1] = 1;

	// Assigning coordinate values and direction to the box with index 0
	arrLoY[0] = 8;
	arrLoX[0] = 36;
	arrHiY[0] = 12;
	arrHiX[0] = 44;
	arrNext_x[0] = 0;
	arrDirection[0] = 1;

	//Start screen
	initscr();
	noecho();
	curs_set(FALSE);

	// Global var `stdscr` is created by the call to `initscr()`
	getmaxyx(stdscr, max_y, max_x);

	//Initialize colors
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	
	// Counter to determine the color to be used for painting the boxes
	int iterationCounter = 0;

	while(remainingProcessesCounter > 0) {

		clear();

		// Draw the CPU; it has fixed coordinates
		drawCPU(cpuLoY, cpuLoX, cpuHiY, cpuHiX);

		// Draw the title of the algorithm
		mvprintw(0, 0, "First-come-first-serve algorithm");

		// Draw the number of remaining processes
		drawRemainingProcessesCounter(2, 20, remainingProcessesCounter);

		// Draw the total waiting time
		drawTotalWaitingTime(3, 12, totalWaitingTime);

		for(int i = 0; i < TOT_PROCESSES; i++){

			// If the process has already finished, don't paint its box
			if(processIsFinished[i] == 1) 
				continue;

			// Draw the corresponding box of the process
			draw(arrLoY[i], arrLoX[i], arrHiY[i], arrHiX[i], toDraw, iterationCounter, arrProcessesNames[i], arrProcessesBurstTimes[i]);

			// If it is not the turn of this process (i.e. the previous process has not finished), don't move the process
			if(i > 0 && !isItsTurn(i, processIsFinished))
				continue;
	 		
			// If the process hasn't entered the CPU, move it forward
			if(arrLoX[i] < cpuLoX + (cpuHiX - cpuLoX)/2 - 4){
	 			advanceCoordinates(&arrNext_x[i], &arrLoX[i], &arrHiX[i], &arrDirection[i], max_x);
			}

			else{ // If the process has already entered the CPU

				// If the process has already consumed all its required CPU time
				if(arrProcessCurrentTime[i] == arrProcessTotalTime[i]){

	 				remainingProcessesCounter--; // Decrement the amount of remaining processes
	 				processIsFinished[i] = 1; // Mark this process as completed
	 			}

	 			else{ //If the process has not consumed all its required CPU time yet

	 				totalWaitingTime++; //Add to the CPU time counter
	 				arrProcessCurrentTime[i]++; //Add to the time this process has consumed
	 			}
			}
	 	}

	 	refresh(); 

	 	// Sleep 
	 	usleep(10*DELAY);

	 	// Increment the counter for the color
	 	iterationCounter++;
	}

	// Sleep before exiting
	usleep(10*DELAY);

	// End the window
	endwin();

	return 0;
}