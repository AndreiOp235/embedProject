#include "temp.h"
#include "sdcard.h"
#include "servoPOT.h"
#include "ceas.h"

void setup()
{
    Serial.begin(9600);
    lcd.init();                      // initialize the  lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setBacklight(HIGH);

  delay(500);

  initSD();

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
  startMillisSERVO = millis();  //initial start time

  pinMode(hs, INPUT_PULLUP);  // avoid external Pullup resistors for Button 1
  pinMode(ms, INPUT_PULLUP);  // and Button 2
  nowRTC = millis();  // read RTC initial value
  dht.begin();

}

void  loop()
{
    ceas();
    TEMPPOT();
    if(temperature>35)
    notaSUS();
    else if (temperature<15)
    notaJOS();
    else
    noTone(10);
    servoPOT();
}

void ceas()
{
    // every second
  // Read temperature and humidity from DHT11 sensor
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  // Update LCD Display
  // Print TIME in Hour, Min, Sec + AM/PM

  lcd.setCursor(0, 0);
  lcd.print("T");
  lcd.print(temperature);
  lcd.print("C ");

  if (h < 10)
    lcd.print("0");  // always 2 digits
  lcd.print(h);
  lcd.print(":");
  if (m < 10)
    lcd.print("0");
  lcd.print(m);
  lcd.print(":");
  if (s < 10)
    lcd.print("0");
  lcd.print(s);

  lcd.setCursor(0, 1);  // for Line 2



  lcd.print("H");
  lcd.print(humidity);
  lcd.print("% ");

  if (d < 10)
    lcd.print("0");
  lcd.print(d);
  lcd.print(".");
  if (l < 10)
    lcd.print("0");
  lcd.print(l);
  lcd.print(".");
  lcd.print(an);


  // improved replacement of delay(1000)
  // Much better accuracy, no more dependent on loop execution time
  for (int i = 0; i < 5; i++)  // make 5 times 200ms loop for faster Button response
  {
    while ((nowRTC - last_timeRTC) < 200)  // delay 200ms
    {
      nowRTC = millis();
    }
    // inner 200ms loop
    last_timeRTC = nowRTC;  // prepare for the next loop

    // read Setting Buttons
    button1 = digitalRead(hs);  // Read Buttons
    button2 = digitalRead(ms);

    // Process Button 1 or Button 2 when hit while Backlight on
    if (button1 == 0) 
    {
      h = h + 1;
    }

    if (button2 == 0) 
    {
      s = 0;
      m = m + 1;
    }

    if(button1==0 && button2 == 0)
    {
      d++;
    }

    /* ---- manage seconds, minutes, hours am/pm overflow ----*/
    if (s == 60) 
    {
      s = 0;
      m = m + 1;
    }
    if (m == 60)
    {
      m = 0;
      h = h + 1;
    }
    if (h == 24)
    {
      h=0;
      d++;
    }

    if (d==32)
    {
      d=1;
      l++;
    }
  }  // end for

  // outer 1000ms loop
  s = s + 1;  // increment sec. counting

  // ---- manage seconds, minutes, hours am/pm overflow ----
  if (s == 60) {
    s = 0;
    m = m + 1;
  }
  if (m == 60)
  {
    m = 0;
    h = h + 1;
  }
  if (h == 24)
    h = 0;

  // Loop end
}


void TEMPPOT()
{
  currentMillisTEMP = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillisTEMP - startMillisTEMP >= periodTEMP)  //test whether the period has elapsed
  {

    Serial.print("Umiditate:");
    Serial.print(humidity);
    Serial.println("%");
    Serial.print("Temperat: "); 
    Serial.print(temperature);
    Serial.println("C    ");


    scrieSD();
  startMillisTEMP = currentMillisTEMP;  //IMPORTANT to save the start time of the current LED state.
  }

}

void initSD()
{
  // Initialize the SD Card
  if (success)
  {
    if (!SD.begin(4))
    {
      Serial.println("SD Card initialization failed!");
      success = false;
    }
    Serial.println("SD Card initialization done.");
  }

  // Create a default file (if the file does not already exist)
  if (success)
  {
    if (!SD.exists(FILE_NAME))
    {
      // Create a default file
      Serial.println("Creating default file...");
      
      myFile = SD.open(FILE_NAME, FILE_WRITE);
      if (myFile)
      {
        myFile.print("0");
        myFile.close();
      }
      else 
      {
        Serial.println("Failed to create default file");
        success = false;
      }
    }
    else
    {
      Serial.println("File exists.");
    }
    
  }
}

void scrieSD()
{
  // Write the new reset value in the file
  if (success)
  {
    myFile = SD.open(FILE_NAME, FILE_WRITE);
    if (myFile)
    {
      myFile.print(String(d));
      myFile.print(String("."));
      myFile.print(String(l));
      myFile.print(String("."));
      myFile.print(String(an));
      myFile.print(String("  "));
      myFile.print(String(h));
      myFile.print(String(":"));
      myFile.print(String(m));
      myFile.print(String(":"));
      myFile.print(String(s));
      myFile.print(String('\n'));

      myFile.print(String("Temp= "));
      myFile.print(String(temperature));
      myFile.print(String('\n'));

      myFile.print(String("HUM= "));
      myFile.print(String(humidity));
      myFile.print(String('\n'));
      myFile.print(String('\n'));
      
      myFile.close();
    }
    else 
    {
      Serial.println("Failed to update the file");
      success = false;
    }
  }

  if (success)
  {
    Serial.println("S-a scris ");
  }

  Serial.println();
}

void notaSUS()
{
  tone(10, 4500);  //sus
}

void notaJOS()
{
  tone(10, 500);  //sus
}

void servoPOT()
{
  currentMillisSERVO = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillisSERVO - startMillisSERVO >= periodSERVO)  //test whether the period has elapsed
  {
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 120);     // scale it for use with the servo (value between 0 and 180)
  Serial.println(val);
  x=120-val;
  Serial.println(x);

  myservo.write(x);
  delay(60000);
  myservo.write(0);
  startMillisSERVO = currentMillisSERVO;  //IMPORTANT to save the start time of the current LED state.
  }

}