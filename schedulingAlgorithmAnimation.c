#include "schedulingAlgorithmAnimation.h"
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

void drawLine(int loIndex, int hiIndex, int verticalIsConstant, int constantIndex, char* string){

	// Draw from left to right
	for(int i = loIndex; i <= hiIndex; i++){

		if(verticalIsConstant == 1) //If the line has to be drawn vertically
			mvprintw(constantIndex, i, string);

		else // If the line has to be drawn horizontally
			mvprintw(i, constantIndex, string);
	}
}

void drawBox(int loY, int loX, int hiY, int hiX, char* charToDraw){

	// Draw the left vertical line
	drawLine(loY, hiY, 0, loX, charToDraw);

	// Draw the right vertical line
	drawLine(loY, hiY, 0, hiX, charToDraw);

	// Draw the upper horizontal line
	drawLine(loX, hiX, 1, loY, charToDraw);

	// Draw the lower horizontal line
	drawLine(loX, hiX, 1, hiY, charToDraw);
}

void drawInsideBox(int y, int x, char* numberToDraw){

	// Print numberToDraw at (y, x) coordinates
	mvprintw(y, x, numberToDraw);
}

void addColor(int counter, int mod){

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_RED);
	init_pair(2, COLOR_WHITE, COLOR_GREEN);

	// Choose the color depending on the counter
	attron(COLOR_PAIR((counter%mod)/(mod/2) +  1));
}

void draw(int loY, int loX, int hiY, int hiX, char* charToDraw, int counter, char* processName, char* processBurstTime){

	// Draw a box
	drawBox(loY, loX, hiY, hiX, charToDraw);

	// Draw the process name
	drawInsideBox(loY + (hiY - loY)/2, loX + (hiX - loX)/2, processName);

	// Draw the process burst time
	drawInsideBox(loY + 1 + (hiY - loY)/2, loX + (hiX - loX)/2, processBurstTime);

	// Add color
	addColor(counter, 10);
}

void draw2(int loY, int loX, int hiY, int hiX, char* charToDraw, int counter, char* processName, char* processBurstTime, char* processAriival){

	drawBox(loY, loX, hiY, hiX, charToDraw);
	drawInsideBox(loY - 1 +  (hiY - loY)/2, loX + (hiX - loX)/2, processName);
	drawInsideBox(loY + (hiY - loY)/2, loX + (hiX - loX)/2, processBurstTime);
    drawInsideBox(loY + 1 + (hiY - loY)/2, loX + (hiX - loX)/2, processAriival);
	addColor(counter, 10);
}

void drawCPU(int cpuLoY, int cpuLoX, int cpuHiY, int cpuHiX){

	// Draw the CPU bpx
	drawBox(cpuLoY, cpuLoX, cpuHiY, cpuHiX, "*");

	// Draw the name "CPU"
	drawInsideBox(cpuLoY + 2, cpuLoX - 1 + (cpuHiX - cpuLoX)/2, "CPU");

	// Add color
	addColor(10, 10);
}

void drawTotalWaitingTime(int y, int x, int time){

	char strTime[10];
	sprintf(strTime, "%d", time);
	mvprintw(y, x - 12, "Total CPU time: ");

	// Draw the amount of waiting time so far
	mvprintw(y, x + 4, strTime);
}

void drawRemainingProcessesCounter(int y, int x, int remainingProcessesCounter){

	char strRemainingProcessesCounter[10];
	sprintf(strRemainingProcessesCounter, "%d", remainingProcessesCounter);
	mvprintw(y, x - 20, "Remaining Processes: ");

	// Draw the remaining processes
	mvprintw(y, x + 1, strRemainingProcessesCounter);
}

void advanceCoordinates(int* ptrNext_x, int* ptrLoX, int* ptrHiX, int* ptrDirection, int max_x){

 	*ptrNext_x = *ptrHiX + *ptrDirection;

 	// If the box has reached the window's limit, make it move backwards by changing its direction
 	if(*ptrNext_x >= max_x || *ptrNext_x < 0){

 		*ptrDirection *= -1;
 	}

 	else{ // If the box can still move forward, move it forward

 		*ptrLoX += *ptrDirection;
 		*ptrHiX += *ptrDirection;
 	}
}

int isItsTurn(int processIndex, int processIsFinished[]){

	// If the previous process has been finished, return true
	return processIsFinished[processIndex - 1] == 1;
}

void backCoordinates(int* ptrNext_x, int* ptrLoX, int* ptrHiX, int* ptrDirection, int max_x){
 	
 	*ptrNext_x = *ptrHiX + *ptrDirection;

 	if(*ptrNext_x >= max_x || *ptrNext_x < 0){

 		*ptrDirection *= -1;
 	}

 	else{

 		*ptrLoX -= *ptrDirection;
 		*ptrHiX -= *ptrDirection;
 	}
}