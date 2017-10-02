// algorithm2_SJF.c
// To compile this, you need to have ncurses installed
// to compile, do this: edit the "make" file and then type "make" in the console

/*
	Author: Salvador Orozco Villalever - A07104218
	Version: 10/02/2017
*/

#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "schedulingAlgorithmAnimation.h"

#define DELAY 30000
#define TOT_PROCESSES 4

int *array;

int compare(const void *a, const void *b){
    
    int ia = *(int *)a;
    int ib = *(int *)b;
    
    return (array[ia] < array[ib]) ? -1 : array[ia] > array[ib];
}

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
	arrHiY[3] = 10;
	arrHiX[3] = arrLoX[3] + 2*(arrHiY[3] - arrLoY[3]);
	arrNext_x[3] = 0;
	arrDirection[3] = 1;

	// Assigning coordinate values and direction to the box with index 2
	arrLoY[2] = 8;
	arrLoX[2] = 8;
	arrHiY[2] = 14;
	arrHiX[2] = arrLoX[2] + 2*(arrHiY[2] - arrLoY[2]);
	arrNext_x[2] = 0;
	arrDirection[2] = 1;

	// Assigning coordinate values and direction to the box with index 1
	arrLoY[1] = 8;
	arrLoX[1] = 24;
	arrHiY[1] = 11;
	arrHiX[1] = arrLoX[1] + 2*(arrHiY[1] - arrLoY[1]);
	arrNext_x[1] = 0;
	arrDirection[1] = 1;

	// Assigning coordinate values and direction to the box with index 0
	arrLoY[0] = 8;
	arrLoX[0] = 34;
	arrHiY[0] = 12;
	arrHiX[0] = arrLoX[0] + 2*(arrHiY[0] - arrLoY[0]);
	arrNext_x[0] = 0;
	arrDirection[0] = 1;

	//Start screen
	initscr();
	noecho();
	curs_set(FALSE);

	////////// TO MAKE THE SHORTEST PROCESSES GO FIRST //////////////

	// Get the order of the processes; short processes go first

	// Array to store the index of each process in an ordered way
	int orderedProcessesIndices[TOT_PROCESSES];//use malloc to large size array

    for(int i = 0; i < TOT_PROCESSES; i++)
        orderedProcessesIndices[i] = i;
   
   	// Assign to global variable "array"
    array = arrProcessTotalTime;

    // Sort it by using the built-in quicksort implementation
    qsort(orderedProcessesIndices, TOT_PROCESSES, sizeof(*orderedProcessesIndices), compare);

	////////////////////////////////////

	// Global var `stdscr` is created by the call to `initscr()`
	getmaxyx(stdscr, max_y, max_x);

	//Initialize colors
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	
	// Counter to determine the color to be used for painting the boxes
	int iterationCounter = 0;

	// Counter to determine how many processes have finished
	int processCounter = 0;

	// While there still are remaining processes
	while(remainingProcessesCounter > 0) {

		clear();

		// Draw the CPU; it has fixed coordinates
		drawCPU(cpuLoY, cpuLoX, cpuHiY, cpuHiX);

		// Draw the title of the algorithm
		mvprintw(0, 0, "Shortest-job-first algorithm");

		// Draw the number of remaining processes
		drawRemainingProcessesCounter(2, 20, remainingProcessesCounter);

		// Draw the total waiting time
		drawTotalWaitingTime(3, 12, totalWaitingTime);

		char intToStr[10];

		// The index of the next process to be selected (from lower to greater required CPU time)
		int currProcessIndex = orderedProcessesIndices[processCounter];

		for(int i = 0; i < TOT_PROCESSES; i++){

			// If the process has already finished, don't paint its box
			if(processIsFinished[i] == 1) 
				continue;

			// Draw the corresponding box of the process
			draw(arrLoY[i], arrLoX[i], arrHiY[i], arrHiX[i], toDraw, iterationCounter, arrProcessesNames[i], arrProcessesBurstTimes[i], currProcessIndex == i);

			// If it is not the turn of this process (i.e. the previous process has not finished), don't move the process
			if(i > 0 && !isItsTurn(i, processIsFinished))
				continue;
	 			
	 		// If the process hasn't entered the CPU, move it forward
			if(arrLoX[currProcessIndex] < cpuLoX + (cpuHiX - cpuLoX)/2 - 4){

	 			advanceCoordinates(&arrNext_x[currProcessIndex], &arrLoX[currProcessIndex], &arrHiX[currProcessIndex], &arrDirection[currProcessIndex], max_x);
			}

	 		else{ // If the process has already entered the CPU

	 			// If the process has already consumed all its required CPU time
	 			if(!processIsFinished[currProcessIndex] && arrProcessCurrentTime[currProcessIndex] == arrProcessTotalTime[currProcessIndex]){

	 				remainingProcessesCounter--; // Decrement the amount of remaining processes
	 				processIsFinished[currProcessIndex] = 1; // Mark this process as completed
	 				processCounter++; // Add one to the finished processes counter
	 			}

	 			else{

	 				if(!processIsFinished[currProcessIndex]){

		 				totalWaitingTime++; //Add to the CPU time counter
		 				arrProcessCurrentTime[currProcessIndex]++; //Add to the time this process has consumed
		 			}
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