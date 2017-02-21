/*
 * Generic Stepper Motor Driver Driver
 * Indexer mode only.

 * Copyright (C)2015 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include "BasicStepperDriver.h"

/*
 * Basic connection: only DIR, STEP are connected.
 * Microstepping controls should be hardwired.
 */
BasicStepperDriver::BasicStepperDriver(int steps, int dir_pin, int step_pin)
:motor_steps(steps), dir_pin(dir_pin), step_pin(step_pin)
{
    init();
}

BasicStepperDriver::BasicStepperDriver(int steps, int dir_pin, int step_pin, int enable_pin)
:motor_steps(steps), dir_pin(dir_pin), step_pin(step_pin), enable_pin(enable_pin)
{
    init();
}

void BasicStepperDriver::init(void){
    pinMode(dir_pin, OUTPUT);
    digitalWrite(dir_pin, HIGH);

    pinMode(step_pin, OUTPUT);
    digitalWrite(step_pin, LOW);

    if IS_CONNECTED(enable_pin){
        pinMode(enable_pin, OUTPUT);
        digitalWrite(enable_pin, HIGH); // disable
    }

    setMicrostep(1);
    setRPM(60); // 60 rpm is a reasonable default

    enable();
}

void BasicStepperDriver::calcStepPulse(int rpm) {
	stepPulse = (60 * 1000000L / motor_steps / microsteps / rpm);
}

void BasicStepperDriver::setRPM(unsigned rpm) {
	calcStepPulse(rpm);
	this->rpm = rpm;
}

/*
 * Set target motor RPM (1-200 is a reasonable range)
 */
signed BasicStepperDriver::getRPM() {
	return rpm;
}

/*
 * Set stepping mode (1:microsteps)
 * Allowed ranges for BasicStepperDriver are 1:1 to 1:128
 */
unsigned BasicStepperDriver::setMicrostep(unsigned microsteps){
    for (unsigned ms=1; ms <= this->getMaxMicrostep(); ms<<=1){
        if (microsteps == ms){
            this->microsteps = microsteps;
            break;
        }
    }
    calcStepPulse(rpm);
    return this->microsteps;
}

/*
 * DIR: forward HIGH, reverse LOW
 */
void BasicStepperDriver::setDirection(int direction){
    digitalWrite(dir_pin, (direction<0) ? LOW : HIGH);
}

signed int BasicStepperDriver::getDirection() {
	return ( (digitalRead(dir_pin)==LOW) ? -1 : 1);
}

/*
 * Move the motor a given number of steps.
 * positive to move forward, negative to reverse
 */
void BasicStepperDriver::move(long steps){
    if (steps >= 0){
        setDirection(1);
    } else {
        setDirection(-1);
        steps = -steps;
    }
    /*
     * We currently try to do a 50% duty cycle so it's easy to see.
     * Other option is step_high_min, pulse_duration-step_high_min.
     */
    unsigned long pulse_duration = stepPulse /2;
    while (steps--){
        digitalWrite(step_pin, HIGH);
        unsigned long next_edge = micros() + pulse_duration;
        microWaitUntil(next_edge);
        digitalWrite(step_pin, LOW);
        microWaitUntil(next_edge + pulse_duration);
		updateLoc(getDirection());
    }
}

void BasicStepperDriver::moveAcc(long steps,int MAX_SPD, long stepsAcc){
	yield();
    if (steps >= 0){
        setDirection(1);
    } else {
        setDirection(-1);
        steps = -steps;
    }
    /*
     * We currently try to do a 50% duty cycle so it's easy to see.
     * Other option is step_high_min, pulse_duration-step_high_min.
     */
    unsigned long pulse_duration = stepPulse /2;
	int cmdSpd = MAX_SPD;
	while (steps--){
        digitalWrite(step_pin, HIGH);
        unsigned long next_edge = micros() + pulse_duration;
        microWaitUntil(next_edge);
        digitalWrite(step_pin, LOW);
		if (steps < stepsAcc){
			cmdSpd = steps * MAX_SPD / stepsAcc;
			if (cmdSpd < 10) {
				cmdSpd = 10;
			}			
		}		
		setRPM(cmdSpd);
		pulse_duration = stepPulse / 2;
        microWaitUntil(next_edge + pulse_duration);
    }
}

/*
 * Move the motor a given number of degrees (1-360)
 */
void BasicStepperDriver::rotate(long deg){
    long steps = deg * motor_steps * (long)microsteps / 360;
    move(steps);
}


void BasicStepperDriver::rotateAcc(long deg, int MAX_SPD, long angleAcc){
	/*
	*	Rotate with smooth acceleration and decelaration
	*	Specified max speed, and angle of acc/dec
	*/
	yield();
	long steps = deg * motor_steps * (long)microsteps / 360;
	long stepsAcc = angleAcc * motor_steps * (long)microsteps / 360;
    moveAcc(steps,MAX_SPD, stepsAcc);
}

/*
 * Move the motor with sub-degree precision.
 * Note that using this function even once will add 1K to your program size
 * due to inclusion of float support.
 */
void BasicStepperDriver::rotate(double deg){
    long steps = deg * motor_steps * microsteps / 360;
    move(steps);
}

/*
 * Enable/Disable the motor by setting a digital flag
 */
void BasicStepperDriver::enable(void){
    if IS_CONNECTED(enable_pin){
        digitalWrite(enable_pin, LOW);
    }
}

void BasicStepperDriver::disable(void){
    if IS_CONNECTED(enable_pin){
        digitalWrite(enable_pin, HIGH);
    }
}

unsigned BasicStepperDriver::getMaxMicrostep(){
    return BasicStepperDriver::MAX_MICROSTEP;
}

signed int BasicStepperDriver::getSteps() {
	return stepCount;
}

signed int BasicStepperDriver::getRevCount() {
	return revCount;
}

void BasicStepperDriver::updateLoc(int steps) {
	int dir = getDirection();
	steps = dir * steps;

	stepCount = stepCount + steps;
	if (stepCount > motor_steps) {
		revCount++;
		stepCount = stepCount - motor_steps;
	}
	else if (stepCount <= 0) {
		revCount--;
		stepCount = stepCount + motor_steps;
	}
}