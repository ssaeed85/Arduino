/*
 * 
 */
#ifndef PlatformControl_H
#define PlatformControl_H
#include <PlatformControl.h>
#include "A4988.h"
#include "BasicStepperDriver.h"

class PlatformControl{
protected:
	A4988 X1;
	A4988 X2;
	A4988 Y;
private:
    

public:
    /*
     * Initializing the platform and the connecting the different motors used to move it
     */
    PControl(A4988 X1, A4988 X2, A4988 Y);
    
	void moveX(int steps);
	void moveX(int steps, int maxSPD, int stepsAcc); //acceleration and deceleration
	void moveY(int steps);
	void move(int stepsX, int stepsY);
	void printLoc();
};
#endif // A4988_H
