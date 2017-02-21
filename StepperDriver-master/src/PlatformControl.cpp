/*
 * 
 */
#include "PlatformControl.h"

/*constructor*/
PlatformControl::PlatformControl(A4988 x1, A4988 x2, A4988 y)
:X1(x1),X2(x2),Y(y){
	X1.setMicrostep(1);
	X2.setMicrostep(1);
	Y.setMicrostep(1);
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
void PlatformControl::moveX(int steps, int rpm) {
	//Calc factor of 2 included since alternately controlling two different motors
	X1.setRPM(rpm * 2);
	X2.setRPM(rpm * 2);
	moveX(steps);
}
void PlatformControl::moveY(int steps) {
	int dir = 0;
	if (steps > 0)
		dir = 1;
	else {
		dir = -1;
		steps = -steps;
	}
	while (steps--) {
		Y.move(dir);
	}
}
void PlatformControl::moveY(int steps, int rpm) {
	Y.setRPM(rpm);
	moveY(steps);
}

void PlatformControl::move(int stepsX, int stepsY) {

}
void PlatformControl::getLoc() {

}

