#include "A4988.h"
#include "PlatformControl.h"

int motorSteps = 200;
int maxSpeed = 30;

int X1dirPin = 4;
int X2dirPin = 8;
int YdirPin = 7;
int X1stepPin = 3;
int X2stepPin = 9;
int YstepPin = 6;
int startBtnPin = 12;

int stepsPer = 40;      //steps between adjascent fills
int numberPerSide = 5;  //number of containers per side. 5 -> 5x5

A4988 X1(motorSteps,X1dirPin,X1stepPin);
A4988 X2(motorSteps,X2dirPin,X2stepPin);
A4988 Y(motorSteps,YdirPin,YstepPin);

PlatformControl p(X1, X2, Y);

void setup() {
  Serial.begin(9600);
}

void loop() {    
    Serial.println(digitalRead(startBtnPin));
    delay(1000);
    if(digitalRead(startBtnPin)){
      fillOp();
    }  
}

void fillOp(){  
    for(int i=0; i<numberPerSide; i++){
      for(int j=0; j<numberPerSide; j++){
        //Fill op in a linear path = number perSide
        p.moveY(stepsPer,maxSpeed);
        delay(500);
      }
      //move filler to start position and shift to next row
      p.moveX(stepsPer,maxSpeed);
      p.moveY(-stepsPer * numberPerSide,maxSpeed);
      delay(500);
    }
    delay(10000);
}


