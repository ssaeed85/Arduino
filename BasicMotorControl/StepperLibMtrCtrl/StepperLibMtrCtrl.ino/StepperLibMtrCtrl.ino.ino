#include "A4988.h"
//#include <PlatformControl.h>
int X1stepPin = 3;
int X1dirPin  = 4;
int X2stepPin = 9;
int X2dirPin  = 8;
int YstepPin = 6;
int YdirPin  = 7;
int motorSteps = 200;

int stepsPer = 40;      //steps between adjascent fills
int numberPerSide = 5;  //number of containers per side. 5 -> 5x5

A4988 X1(motorSteps,X1dirPin,X1stepPin);
A4988 X2(motorSteps,X2dirPin,X2stepPin);
A4988 Y(motorSteps,YdirPin,YstepPin);
//PlatformControl p(X1,X2,Y);
int maxSpeed = 60;

void setup() {
    X1.setMicrostep(1);
    X2.setMicrostep(1);
    Y.setMicrostep(1);
}

void loop() {
    X1.moveAcc(360,60,60);
    X2.moveAcc(360,90,60);
    Y.moveAcc(360,120,60);
    
    delay(3000);
    //platform.moveX(360);
    
    fillOp();  
    delay(500);  
}

void fillOp(){
    for(int i=0; i<numberPerSide; i++){
      for(int j=0; j<numberPerSide; j++){
        moveY(stepsPer);
        delay(1000);
      }
      moveX(stepsPer);
      moveY(-stepsPer * numberPerSide);
      delay(1000);
    }
}

void moveY(int steps){
    if (steps >= 0){
        digitalWrite(YdirPin,HIGH);;
    } else {
        digitalWrite(YdirPin,LOW);
        steps = -steps;
    }     
    while (steps--){
        digitalWrite(YstepPin, HIGH);
        digitalWrite(YstepPin, LOW);
        delay(10);
    }
}

void moveX(int steps){
    //yield();
    if (steps >= 0){
        digitalWrite(X1dirPin,HIGH);
        digitalWrite(X2dirPin,LOW);
    } else {
        digitalWrite(X1dirPin,LOW);
        digitalWrite(X2dirPin,HIGH);
        steps = -steps;
    }     
    while (steps--){
        digitalWrite(X1stepPin, HIGH);
        digitalWrite(X2stepPin, HIGH);
        digitalWrite(X1stepPin, LOW);
        digitalWrite(X2stepPin, LOW);
        delay(10);
    }
}


