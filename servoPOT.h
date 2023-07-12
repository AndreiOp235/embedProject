#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = A5;  // analog pin used to connect the potentiometer
int val,pos,x;    // variable to read the value from the analog pin

unsigned long startMillisSERVO;  //some global variables available anywhere in the program
unsigned long currentMillisSERVO;
const unsigned long periodSERVO = 1000*3600L;  //the value is a number of milliseconds