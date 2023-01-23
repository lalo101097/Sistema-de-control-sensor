#include <AccelStepper.h>

#include <Wire.h>
#include "Adafruit_TCS34725.h"

int sensx = 6;
int sensy = 11;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_16X);

AccelStepper stpx(AccelStepper::FULL4WIRE, 5, 3, 4, 2);
AccelStepper stpy(AccelStepper::FULL4WIRE, 10, 8, 9, 7);

bool sen = false, calib = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensx, INPUT_PULLUP);
  pinMode(sensy, INPUT_PULLUP);
  sen = tcs.begin();
  tcs.setInterrupt(true);
  stpx.setMaxSpeed(800.0);
  stpx.setSpeed(800.0);
  stpx.setAcceleration(200.0);
  stpy.setMaxSpeed(800.0);
  stpy.setSpeed(800.0);
  stpy.setAcceleration(200.0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    String str = Serial.readString();
    String strs[20];
    int StringCount = 0;
  
    // Split the string into substrings
    while (str.length() > 0)
    {
      int index = str.indexOf(',');
      if (index == -1) // No space found
      {
        strs[StringCount++] = str;
        break;
      }
      else
      {
        strs[StringCount++] = str.substring(0, index);
        str = str.substring(index+1);
      }
    }

    if(strs[0] == "a")
    {
        uint16_t r = 0, g = 0, b = 0, c = 0;
        for(int e = 0;e < 2;e++)
        {
          uint16_t r1, g1, b1, c1;
          tcs.getRawData(&r1, &g1, &b1, &c1);
          r = r + r1;
          g = g + g1;
          b = b + b1;
          c = c + c1;
        }
        r = r/2;
        g = g/2;
        b = b/2;
        c = c/2;
        Serial.print(r, DEC);
        Serial.print(',');
        Serial.print(g, DEC);
        Serial.print(',');
        Serial.print(b, DEC);
        Serial.print(',');
        Serial.println(c, DEC);
    }

    if(strs[0] == "b")
    {
      stpx.setMaxSpeed(800.0);
      stpx.setSpeed(800.0);
      stpx.setAcceleration(200.0);
      stpy.setMaxSpeed(800.0);
      stpy.setSpeed(800.0);
      stpy.setAcceleration(200.0);
      if(calib == true)
      {
        stpx.moveTo(strs[1].toDouble() * 10);
        while(stpx.distanceToGo() != 0)
        {
          stpx.run();
        }
        stpx.disableOutputs();
  
        stpy.moveTo(strs[2].toDouble() * 10);
        while(stpy.distanceToGo() != 0)
        {
          stpy.run();
        }
        stpy.disableOutputs();
      }
      Serial.println("t");
    }
    
    if(strs[0] == "p")
    {
        Serial.print("l");
        Serial.println(sen);
    }
    if(strs[0] == "c")
    {
      stpx.setMaxSpeed(500.0);
      stpx.setSpeed(500.0);
      stpy.setMaxSpeed(500.0);
      stpy.setSpeed(500.0);

      stpx.setCurrentPosition(0);
      stpy.setCurrentPosition(0);

      stpx.moveTo(-100000);
      stpy.moveTo(-100000);
      
      while (digitalRead(sensx) == HIGH)
      {
        stpx.run();
      }
      stpx.setCurrentPosition(0);
      stpx.disableOutputs();
      
      while (digitalRead(sensy) == HIGH)
      {
        stpy.run();
      }
      stpy.setCurrentPosition(0);
      stpy.disableOutputs();
      
      calib = true;
      Serial.println("t");
    }
    if(strs[0] == "l")
    {
      if(strs[1] == "s")
      {
        tcs.setInterrupt(false);
      }
      else if(strs[1] == "n")
      {
        tcs.setInterrupt(true);
      }
    }
  }
}
