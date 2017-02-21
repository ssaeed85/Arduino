/*
 * 
 */
#include "PlatformControl.h"

/*constructor*/
PlatformControl::PlatformControl(A4988 X1, A4988 X2, A4988 Y){
	this->X1 = X1;
	this->X2 = X2;
	this->Y = Y;
}

void PlatformControl::moveX(int steps) {
	int dir = 0;
	if (steps > 0)
		dir = 1;
	else {
	dir = -1;
	steps = -steps;
	}
	
	while (steps--) {
		X1.move(dir);
		X2.move(-dir);
	}
}
void PlatformControl::moveX(int steps, int maxSPD, int stepsAcc) {

}
void PlatformControl::moveY(int steps) {

}
void PlatformControl::move(int stepsX, int stepsY) {

}
void PlatformControl::printLoc() {

}

