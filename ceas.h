#include <DHT.h>

#define DHTPIN 8
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Initial Time display is 12:59:45 PM
int h = 12;
int m = 59;
int s = 45;

int d=20;
int l=5;
int an=23;


// Time Set Buttons
int button1;
int button2;

float temperature=25;
float humidity=25;

// Pins definition for Time Set Buttons
int hs = 2;  // pin A0 for Hours Setting
int ms = 7;  // pin A1 for Minutes Setting

// For accurate Time reading, use Arduino Real-Time Clock and not just delay()
static uint32_t last_timeRTC, nowRTC = 0;  // RTC