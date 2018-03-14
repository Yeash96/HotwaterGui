//Temperature sensor libraries
#include "OneWire.h"
#include "DS18B20.h"
#include <DallasTemperature.h>

//Timer
//#include <SimpleTimer.h>
//SimpleTimer timer; //may not work

//Touch screen libraries + header files
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
uint8_t addr  = 0x38;
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

//Pin for temperature sensors
#define ONE_WIRE_BUS_PIN 31//<---------------------------pin for sensor signal

//Pin for touch screen
#define RA8875_INT 4
#define RA8875_CS 10
#define RA8875_RESET 9
#define FT5206_WAKE 6
#define FT5206_INT   7
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

//Touch screen functions + variables
enum {
  eNORMAL = 0,
  eTEST   = 0x04,
  eSYSTEM = 0x01
};

struct TouchLocation {
  uint16_t x;
  uint16_t y;
};

TouchLocation touchLocations[5];

uint8_t readFT5206TouchRegister( uint8_t reg );
uint8_t readFT5206TouchLocation( TouchLocation * pLoc, uint8_t num );
uint8_t readFT5206TouchAddr( uint8_t regAddr, uint8_t * pBuf, uint8_t len );

uint8_t buf[30];

uint8_t readFT5206TouchRegister( uint8_t reg ) {
  Wire.beginTransmission(addr);
  Wire.write( reg );  // register 0
  uint8_t retVal = Wire.endTransmission();

  uint8_t returned = Wire.requestFrom(addr, uint8_t(1) );    // request 6 uint8_ts from slave device #2

  if (Wire.available()) {
    retVal = Wire.read();
  }

  return retVal;
}

uint8_t readFT5206TouchAddr( uint8_t regAddr, uint8_t * pBuf, uint8_t len ) {
  Wire.beginTransmission(addr);
  Wire.write( regAddr );  // register 0
  uint8_t retVal = Wire.endTransmission();

  uint8_t returned = Wire.requestFrom(addr, len);    // request 1 bytes from slave device #2

  uint8_t i;
  for (i = 0; (i < len) && Wire.available(); i++) {
    pBuf[i] = Wire.read();
  }

  return i;
}

uint8_t readFT5206TouchLocation( TouchLocation * pLoc, uint8_t num ) {
  uint8_t retVal = 0;
  uint8_t i;
  uint8_t k;

  do
  {
    if (!pLoc) break; // must have a buffer
    if (!num)  break; // must be able to take at least one

    uint8_t status = readFT5206TouchRegister(2);

    static uint8_t tbuf[40];

    if ((status & 0x0f) == 0) break; // no points detected

    uint8_t hitPoints = status & 0x0f;

    Serial.print("number of hit points = ");
    Serial.println( hitPoints );

    readFT5206TouchAddr( 0x03, tbuf, hitPoints*6);

    for (k=0,i = 0; (i < hitPoints*6)&&(k < num); k++, i += 6)
    {
      pLoc[k].x = (tbuf[i+0] & 0x0f) << 8 | tbuf[i+1];
      pLoc[k].y = (tbuf[i+2] & 0x0f) << 8 | tbuf[i+3];
    }

    retVal = k;

  } while (0);

  return retVal;
}

void writeFT5206TouchRegister( uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(addr);
  Wire.write( reg );  // register 0
  Wire.write( val );  // value

  uint8_t retVal = Wire.endTransmission();
}

void readOriginValues(void)
{
  writeFT5206TouchRegister(0, eTEST);
  delay(1);
  //uint8_t originLength = readFT5206TouchAddr(0x08, buf, 8 );
  uint8_t originXH = readFT5206TouchRegister(0x08);
  uint8_t originXL = readFT5206TouchRegister(0x09);
  uint8_t originYH = readFT5206TouchRegister(0x0a);
  uint8_t originYL = readFT5206TouchRegister(0x0b);

  uint8_t widthXH  = readFT5206TouchRegister(0x0c);
  uint8_t widthXL  = readFT5206TouchRegister(0x0d);
  uint8_t widthYH  = readFT5206TouchRegister(0x0e);
  uint8_t widthYL  = readFT5206TouchRegister(0x0f);
}

//Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS_PIN);

//Pass our oneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);

//Declare variables for each temperature sensor
//Add or delete these lines depending on how many temperature probes you have
//See the tutorial on how to obtain these addresses:
//http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html
DeviceAddress Probe01 = { 0x28, 0xFF, 0x75, 0x1F, 0x60, 0x17, 0x05, 0x4D};//<---------------comment this out use the on below this one
//DeviceAddress Probe01 = { 0x28, 0xFF, 0x1A, 0x61, 0xB1, 0x17, 0x05, 0x3C};
//DeviceAddress Probe02 = { 0x28, 0xFF, 0xCA, 0xB5, 0x02, 0x17, 0x04, 0x6B};
//DeviceAddress Probe03 = { 0x28, 0xFF, 0x41, 0xC6, 0x02, 0x17, 0x04, 0xF0};
//DeviceAddress Probe04 = { 0x28, 0xFF, 0x64, 0x56, 0x02, 0x17, 0x05, 0x65};
//DeviceAddress Probe05 = { 0x28, 0xFF, 0xCA, 0xB5, 0x02, 0x17, 0x04, 0x6B};
//DeviceAddress Probe06 = { 0x28, 0xFF, 0x41, 0xC6, 0x02, 0x17, 0x04, 0xF0};
//DeviceAddress Probe07 = { 0x28, 0xFF, 0x64, 0x56, 0x02, 0x17, 0x05, 0x65};
//DeviceAddress Probe08 = { 0x28, 0xFF, 0xCA, 0xB5, 0x02, 0x17, 0x04, 0x6B};
//DeviceAddress Probe09 = { 0x28, 0xFF, 0x41, 0xC6, 0x02, 0x17, 0x04, 0xF0};
//DeviceAddress Probe10 = { 0x28, 0xFF, 0x64, 0x56, 0x02, 0x17, 0x05, 0x65};
//DeviceAddress Probe11 = { 0x28, 0xFF, 0xCA, 0xB5, 0x02, 0x17, 0x04, 0x6B};

//Declare variables for motor pins (forward and reverse)
int forwardMotorPin = 23;//<------------------------------------------------------------------------------Pin Numbers
int reverseMotorPin = 25;

//Declare variable for relay pin
int relayPin = 27;

//Declare variable for submerged pressure pin
int pressurePin = A10;

//Declare variables for final temperatrue and current average temperature
int finalTemperature = 114;//<-----------------------must be greater then coldtemp
float averageTemperature;
float tolerance= 2;//<----------------------------------------------------------------------------tolerance for temp differance
int coldtemp = 113;//<------------will be less then this
//Declare variables for "time to delay motor" and "time to step motor"
int delayTime = 0;
int stepTime = 0;
int cdelayTime = 0;
int cstepTime = 0;
//Declare variable for voltage of pressure pin
float voltage = 0.0;

int displayTime = 5;
int currentTime = 0;
float waterLevel = 0.0;
bool opModeFlag = 0;
//

void setup(){
  //Start serial port to show results
  Serial.begin(9600);
  Serial.println("RA8875 start");
  Wire.begin();

  readOriginValues();
  pinMode     (FT5206_WAKE, INPUT);
  digitalWrite(FT5206_WAKE, HIGH );
  writeFT5206TouchRegister(0, eNORMAL);

  while (!tft.begin(RA8875_800x480))
  {
    Serial.println("RA8875 Not Found!");
    delay(100);
  }

  Serial.println("Found RA8875");

  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);


  pinMode     (FT5206_INT, INPUT);
  //digitalWrite(FT5206_INT, HIGH );

  tft.touchEnable(false);

  randomSeed(analogRead(0));

//  timer.setInterval(1000, getTime);

  //Initialize the temperature measurement library
  sensors.begin();

  //Set the resolution to 12 bit (Can be 9 to 12 bits... lower is faster)
  //Add or delete these lines depending on how many temperature probes you have
  sensors.setResolution(Probe01, 12);
//  sensors.setResolution(Probe02, 12);
//  sensors.setResolution(Probe03, 12);
//  sensors.setResolution(Probe04, 12);
//  sensors.setResolution(Probe05, 12);
//  sensors.setResolution(Probe06, 12);
//  sensors.setResolution(Probe07, 12);
//  sensors.setResolution(Probe08, 12);
//  sensors.setResolution(Probe09, 12);
//  sensors.setResolution(Probe10, 12);
//  sensors.setResolution(Probe11, 12);

  //Set all the motor control pins to outputs
  pinMode(relayPin, OUTPUT);
  pinMode(forwardMotorPin, OUTPUT);
  pinMode(reverseMotorPin, OUTPUT);

//  digitalWrite(relayPin, HIGH);
}

void loop(){

   Serial.print("from the top");
    tft.fillScreen(RA8875_WHITE);//<-- clears screen and creates white backround
    drawDisplay();//<---draws buttons
  while(opModeFlag == 0) {
// this is all hunters stuff I have not touched it for the most part. since it seems to work
    static uint16_t w = tft.width();
    static uint16_t h = tft.height();

    float xScale = 1024.0F/w;
    float yScale = 1024.0F/h;

    uint8_t attention = digitalRead(FT5206_INT);
    static uint8_t oldAttention = 1;

    uint8_t i=0;
      /* Wait around for touch events */
    if (!attention && oldAttention )
    {
      uint8_t count = readFT5206TouchLocation( touchLocations, 5 );

      //static uint8_t lastCount = count;

     if (count)
      {
        static TouchLocation lastTouch = touchLocations[0];

        lastTouch = touchLocations[0];

        if (contains(lastTouch, 510, 55, 80, 80) && finalTemperature < 160) {
          finalTemperature++;
          delay(250);
          drawDisplay();
        }
        else if (contains(lastTouch, 510, 155, 80, 80) && finalTemperature > 113) {//<------------if you change finalTemperature  or coldtemp must change x in >x
          finalTemperature--;
          delay(250);
          drawDisplay();
        }
        else if (contains(lastTouch, 510, 270, 80, 80) && displayTime < 30) {
          displayTime++;
          delay(250);
          drawDisplay();
        }
        else if (contains(lastTouch, 510, 370, 80, 80) && displayTime > 1) {
          displayTime--;
          delay(250);
          drawDisplay();
        }
        else if (contains(lastTouch, 620, 410, 130, 50)) {
          //START SYSTEM
          opModeFlag = 1;
          //digitalWrite(relayPin, HIGH);
        }
      }
    }

    else {}
  }

  while (opModeFlag == 1) {
    delay(500);
    digitalWrite(relayPin, HIGH)
   //Serial.print("Number of Devices found on bus = ");
   //Serial.println(sensors.getDeviceCount());
   //Serial.print("Getting temperatures... ");
   //Serial.println();

   //Command all devices on bus to read temperature
   //sensors.requestTemperatures();
    //popup();
    //Serial.print("Number of Devices found on bus = ");
    //Serial.println(sensors.getDeviceCount());
    //Serial.print("Getting temperatures... ");
    //Serial.println();

    //Command all devices on bus to read temperature
    sensors.requestTemperatures();

    //Print all temperatures
    Serial.print("Probe 01 temperature is:   ");
    //tempcurrent =printTemperature(Probe01);
    printTemperature(Probe01);
    Serial.println();
//    Serial.print("Probe 02 temperature is:   ");
//    printTemperature(Probe02);
//    Serial.println();
//
//    Serial.print("Probe 03 temperature is:   ");
//    printTemperature(Probe03);
//    Serial.println();
//
//    Serial.print("Probe 04 temperature is:   ");
//    printTemperature(Probe04);
//    Serial.println();
//
//    Serial.print("Probe 05 temperature is:   ");
//    printTemperature(Probe05);
//    Serial.println();
//
//    Serial.print("Probe 06 temperature is:   ");
//    printTemperature(Probe06);
//    Serial.println();
//
//    Serial.print("Probe 07 temperature is:   ");
//    printTemperature(Probe07);
//    Serial.println();
//
//    Serial.print("Probe 08 temperature is:   ");
//    printTemperature(Probe08);
//    Serial.println();
//
//    Serial.print("Probe 09 temperature is:   ");
//    printTemperature(Probe09);
//    Serial.println();
//
//    Serial.print("Probe 10 temperature is:   ");
//    printTemperature(Probe10);
//    Serial.println();
//
//    Serial.print("Probe 11 temperature is:   ");
//    printTemperature(Probe11);
//    Serial.println();

//    printAverageTemperature();
  //  printTemperature(Probe01);
    //timer.run();
//    float temperatureDifference = finalTemperature - averageTemperature;
    float tempcurrent = sensors.getTempF(Probe01);
    popup(tempcurrent);
    float temperatureDifference = finalTemperature - tempcurrent;


    if (temperatureDifference > 63.0) {
      //------------------------------------------Zach you are going to have to play with these time vaules, for stepTime, delay time
      //---------------------------------------------in each IF statement case.
      stepTime = 3000;//<-------------------------------- turns motor on for this long
      delayTime = 4000;//<------------------------ time for it to exit if statement and restart loop/ system delay time
    }

    else if (temperatureDifference < 63.0 && temperatureDifference > 20.0) {
      stepTime = 2000;
      delayTime = 4000;
    }

    else if (temperatureDifference < 20.0 && temperatureDifference > 10.0) {
      stepTime = 1000;
      delayTime = 4000;
    }

    else if (temperatureDifference < 10.0 && temperatureDifference > 1.0) {
      stepTime = 500;
      delayTime = 1000;
    }

//    printAverageTemperature();
  averageTemperature = tempcurrent;
  // printTemperature(Probe01);

    if (averageTemperature < finalTemperature) {
      stepHot(stepTime);
      stepOff();
      delay(delayTime);
    }
    if (averageTemperature > finalTemperature) {
      stepCold(stepTime);
      stepOff();
      delay(delayTime);
    }

//    while(currentTime < displayTime * 60) {
//      Serial.print(
//    }
//
//    if (currentTime >= displayTime * 60) {
//      for (int i = 0; i < 10; i++) {
//        stepCold();
//        }
//      digitalWrite(relayPin, LOW);
//      while(1) {}
//    }

//    if (Serial.available() > 3) {
//      for (int i = 0; i < 10; i++) {
//        stepCold();
//        }
//      digitalWrite(relayPin, LOW);
//      while(1) {}
//      }
      Serial.print("Avgtemp>");
      Serial.println(finalTemperature + tolerance);
      Serial.print("Avgtemp<");
      Serial.println(finalTemperature - tolerance);
      Serial.println(averageTemperature);
  if((averageTemperature < (finalTemperature + tolerance)) && (averageTemperature > (finalTemperature - tolerance)) ){
    //start the timer
    timerguistart();//<display red line that counter has started
   Serial.println(displayTime*60);
   int zep = displayTime;//<- time user inputs is now called zep
      for( int i=0; i<=zep*60;i++){
        Serial.println(i);
        delay(1000);//<-- becuase int is too small to hold total time in miliseconds this loop delays for every second for zep*60 amount
        }
        while(averageTemperature > coldtemp){
          //so while teh average temp is > then cold temp fiddle with mixing valve motors
          averageTemperature = sensors.getTempF(Probe01);
          //update averageTemperature
          float temperatureDifference = averageTemperature - coldtemp;
          //the diffrence in averageTemperature and coldtemp
          if (temperatureDifference > 63.0) {
            //if the diffrence is large turn alot and wait for reaction
            cstepTime = 3000;
            cdelayTime = 4000;
          }

          else if (temperatureDifference < 63.0 && temperatureDifference > 20.0) {
            cstepTime = 2000;
            cdelayTime = 4000;
          }

          else if (temperatureDifference < 20.0 && temperatureDifference > 10.0) {
            cstepTime = 1000;
            cdelayTime = 4000;
          }

          else if (temperatureDifference < 10.0 && temperatureDifference > 5.0) {
            cstepTime = 500;
            cdelayTime = 1000;
          }
          // use step cold function to change mixer valve in colder direction
            stepCold(cstepTime);
            stepOff();//<---stop turning the mixer valve
            delay(cdelayTime);//<---time for system to react
        }
        for( int z=0; z<=5*60;z++){
          //when at coldtemp or lower wait five minutes
          Serial.println(z);
          delay(1000);//<-- becuase int is too small to hold total time in miliseconds this loop delays for every second for zep*60 amount
          }

    digitalWrite(relayPin, LOW);//<-----turn off pumps
    opModeFlag = 0;//<--restarts the void loop and first while loop

  }
    }
}

//Increase water temperature
void stepHot(int stepTime) {
  digitalWrite(forwardMotorPin, HIGH);
  delay(stepTime);
//digitalWrite(forwardMotorPin, LOW);
}

//Decrease water temperature
void stepCold(int stepTime) {
  digitalWrite(reverseMotorPin, HIGH);

  delay(stepTime);
  //digitalWrite(reverseMotorPin, LOW);
}

//Turn off motor changing temperature
void stepOff() {
  digitalWrite(forwardMotorPin, LOW);
  digitalWrite(reverseMotorPin, LOW);
}

float checkWaterLevel() {
  voltage = (analogRead(pressurePin) * (5.0 / 1023.0));

  waterLevel = ((voltage - 1.2) / 2.21) * 100;

  Serial.print("Water level is: ");
  Serial.print(waterLevel);
  Serial.print("%");
  Serial.println();
}

//void printAverageTemperature() {
//  Serial.print("Average temperature is:  ");
//  averageTemperature = (sensors.getTempF(Probe01) + sensors.getTempF(Probe02) + sensors.getTempF(Probe03) + sensors.getTempF(Probe04) + sensors.getTempF(Probe05)
//                         + sensors.getTempF(Probe06) + sensors.getTempF(Probe07) + sensors.getTempF(Probe08) + sensors.getTempF(Probe09) + sensors.getTempF(Probe10) + sensors.getTempF(Probe11))/11;
//  Serial.print(averageTemperature);
//  Serial.println();
//}

void printTemperature(DeviceAddress deviceAddress) {

  float tempC = sensors.getTempC(deviceAddress);

   if (tempC == -127.00) {
   Serial.print("Error getting temperature  ");
   }

   else {
   Serial.print("C: ");
   Serial.print(tempC);
   Serial.print(" F: ");
   Serial.print(DallasTemperature::toFahrenheit(tempC));
   }

}

bool contains (TouchLocation lastTouch, int x, int y, int width, int height) {
  if (lastTouch.x > x && lastTouch.x < x + width && lastTouch.y > y && lastTouch.y < y + height) {
    return true;
  }
  else {
    return false;
  }
}

void drawDisplay() {
  //Complete UI Text
  tft.textMode();
  tft.textSetCursor(125, 25);
  tft.textColor(RA8875_BLUE, RA8875_WHITE);
  tft.textEnlarge(1);
  tft.textWrite("Hot Water:", 10);
  tft.textSetCursor(125, 60);
  checkWaterLevel();
  String temp1 = "Level: " + String(waterLevel) + "%";
  int size1 = temp1.length();
  char char_array1[size1 + 1];
  strcpy(char_array1, temp1.c_str());
  tft.textWrite(char_array1, 11);
  tft.textSetCursor(165, 140);
  String temp2 = "Temperature: " + String(finalTemperature) + " F";
  int size2 = temp2.length();
  char char_array2[size2 + 1];
  strcpy(char_array2, temp2.c_str());
  tft.textWrite(char_array2, 18);
  tft.textSetCursor(125, 340);
  String temp3 = "Treatment time: " + String(displayTime) + " min";
  int size3 = temp3.length();
  char char_array3[size3 + 1];
  strcpy(char_array3, temp3.c_str());
  tft.textWrite(char_array3, 22);
  tft.textSetCursor(650, 25);
//  tft.textWrite("12:00 PM", 8);

  tft.graphicsMode();
  //Water tank
  tft.drawRect(20, 20, 100, 120, RA8875_BLUE);
  tft.fillRect(20, 50, 100, 90, RA8875_BLUE);

  //Go arrow
  tft.fillRect(620, 420, 100, 30, RA8875_GREEN);
  tft.fillTriangle(720, 410, 720, 460, 750, 435, RA8875_GREEN);

  //Temperature arrows
  tft.fillRect(510, 55, 80, 80, RA8875_BLACK);
  tft.fillRect(510, 155, 80, 80, RA8875_BLACK);
  tft.fillTriangle(550, 65, 520, 125, 580, 125, RA8875_YELLOW);
  tft.fillTriangle(550, 225, 520, 165, 580, 165, RA8875_YELLOW);

  //Time arrows
  tft.fillRect(510, 270, 80, 80, RA8875_BLACK);
  tft.fillRect(510, 370, 80, 80, RA8875_BLACK);
  tft.fillTriangle(550, 280, 520, 340, 580, 340, RA8875_YELLOW);
  tft.fillTriangle(550, 440, 520, 380, 580, 380, RA8875_YELLOW);
}

void getTime() {
  int h, m, s;
  currentTime = millis() / 1000;
  m = s / 60;
  h = s / 3600;
  s = s - m * 60;
  m = m - h * 60;
  Serial.print(h);
  printDigits(m);
  printDigits(s);
  Serial.println();
}

void printDigits(int digits) {
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
//we can make a another void loop similar to drawDisplay for the pop up when opModeFlag =1
void popup(float tempcurrent){
  tft.graphicsMode();
  tft.fillRect(20,20,750,950, RA8875_BLUE);

  tft.textMode();
  tft.textSetCursor(165,140);
  tft.textColor(RA8875_WHITE, RA8875_BLUE);
  tft.textEnlarge(1);
  String dtemp = "Desired temp:"+ String(finalTemperature)+ " F";
  int sizeftemp = dtemp.length();
  char char_arrayftemp[sizeftemp + 1];
  strcpy(char_arrayftemp, dtemp.c_str());
  tft.textWrite(char_arrayftemp, 17);
  tft.textSetCursor(165,170);
  String cTemp = "Current Temp:"+ String(tempcurrent) + " F";
  int sizet = cTemp.length();
  char char_arrayt[sizet + 1];
  strcpy(char_arrayt, cTemp.c_str());
  tft.textWrite(char_arrayt, 20);
}

void timerguistart(){
  tft.textMode();
  tft.textSetCursor(165,200);
  tft.textColor(RA8875_WHITE, RA8875_RED);
  tft.textEnlarge(1);
  String countdown = "Timer has started";
  int sizeftemp = countdown.length();
  char char_arrayftemp[sizeftemp + 1];
  strcpy(char_arrayftemp, countdown.c_str());
  tft.textWrite(char_arrayftemp, 17);
}
