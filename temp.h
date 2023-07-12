//#include  <dht.h> // sensor library using lib from https://www.ardumotive.com/how-to-use-dht-22-sensor-en.html
#include  <LiquidCrystal_I2C.h> // LCD library using from  https://www.ardumotive.com/i2clcden.html  for the i2c LCD library 
#include <Wire.h> 

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 after finding it from serial monitor (see comment  above) for a 16 chars and 2 line display


unsigned long startMillisTEMP;  //some global variables available anywhere in the program
unsigned long currentMillisTEMP;
const unsigned long periodTEMP = 1000*2L;  //the value is a number of milliseconds
