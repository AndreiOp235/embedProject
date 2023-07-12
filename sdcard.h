/* This example shows how to create a file that stores the number of the board resets.
   The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK  - pin 13
 ** CS   - pin 4
*/
#include <SPI.h>
#include <SD.h>

/* Set the file name */
#define FILE_NAME "contor.txt"
bool success   = true;
File myFile;
char buf[100]  = {0};
