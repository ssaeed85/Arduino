/*
 * 
 */

#ifndef PlatformControl_H
#define PlatformControl_H
#include <Arduino.h>
#include "A4988.h"

class PlatformControl {
protected:

private:
	A4988 X1;
	A4988 X2;
	A4988 Y;

public:
    /*
     * Initializing the platform and the connecting the different motors used to move it
     */
	PlatformControl(A4988 x1, A4988 x2, A4988 y);
	//PlatformControl(int motorSteps, int stepPins[3], int dirPins[3]);
    
	void moveX(int steps);
	void moveX(int steps, int rpm);
	void moveY(int steps);
	void moveY(int steps, int rpm);
	void move(int stepsX, int stepsY);
	void getLoc();
};
#endif // PlatformControl_H
