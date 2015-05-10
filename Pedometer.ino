#include <Wire.h>
#include <SeeedOLED.h>
#include <ADXL345.h>
#include "pedometer.h"
#include <SPI.h>
#include <WiFi.h>
#include <TimerOne.h>
#include "Seeed_QTouch.h"


unsigned long curtime = 0;
unsigned long cursteps = 0;

int isWalking = 0;        // if walking,isWalking=1.  
int ipprinted = 0;        // if printing local ip, ipprinted = 1.

int notificationSteps = 0;
int intervalSteps = 15;

Pedometer pedometer;
int stepIndex = 0;

void setup() {
  Serial.begin(9600);           // set baudrate = 9600bps
  // put your setup code here, to run once:

  pedometer.init();
  vibratorInit(); 
  Wire.begin();
  SeeedOled.init();                  //initialze SEEED OLED display
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode  
  
  QTouch.reset();
  QTouch.calibrate();
  QTouch.setNTHRForKey(6, 0);
  QTouch.setNTHRForKey(6, 1);
  QTouch.setNTHRForKey(6, 2);
  
  curtime = millis();              // get the current time
  cursteps = pedometer.stepCount;  // get the current steps
}

void loop() {
  // update pedometer steps
  int tn = QTouch.touchNum();
  
  StepsUpdate();
  SeeedOled.setTextXY(0,0);
  SeeedOled.putString("Steps Taken: ");
  SeeedOled.putNumber(cursteps);
  SeeedOled.setTextXY(1,0);
  SeeedOled.putString("Interval: ");
  SeeedOled.putNumber(intervalSteps);
  
  Serial.println(notificationSteps);
  
  if (notificationSteps == intervalSteps)
  {
      vibrate();
      notificationSteps = 0;
  }
  
  if (QTouch.isTouch(0))
  {
     if (intervalSteps > 10)
     {
         intervalSteps--;
     }
     Serial.println(intervalSteps);
     notificationSteps = 0;
     delay(500);
  }  else if (QTouch.isTouch(1))
  {
     clear();
  }  else if (QTouch.isTouch(2))
  {
     intervalSteps++;
     notificationSteps = 0;
     Serial.println(intervalSteps);
     delay(500);
  } 
}
void clear()
{
  notificationSteps = 0;
  cursteps = 0;
  pedometer.init();
  Serial.println(cursteps);
  Serial.println(pedometer.stepCount);
  SeeedOled.setTextXY(0,0);
  SeeedOled.putString("                                   ");
  delay(500);
}

// update steps
void StepsUpdate(){
  static unsigned int stepCount = pedometer.stepCount;
  pedometer.stepCalc();
  stepCount = pedometer.stepCount;
  if (cursteps != stepCount) {notificationSteps++;}
  cursteps = stepCount;
}

void vibratorInit() {
  pinMode(10, OUTPUT);      //10, 11 are pins is vibrator 
  pinMode(11, OUTPUT);
}
void vibrate() {
  digitalWrite(10, HIGH);  //turn on vibrator
  digitalWrite(11, HIGH);
  
  delay(200);
  
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}
