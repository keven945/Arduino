// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <dht.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

dht DHT;
#define DHT11_PIN 3

RTC_DS1307 rtc;
int addr = 0;

String DateAndTime;
String TempAndHumi;
byte value;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int buttonPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;  
int buttonValue = 0;   // variable to store the value coming from the sensor

void setup () {
  pinMode(ledPin, OUTPUT);
  while (!Serial); // for Leonardo/Micro/Zero
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2017, 3, 19,11, 54, 0));
  }
  
}

void loop () {
      //lcd.clear();
      DateAndTime = String("");
      DateTime now = rtc.now();
      
      DateAndTime += now.year(),
      DateAndTime += "-";

      DateAndTime += (getDigits(now.month()));
      DateAndTime += "-";
      DateAndTime += (getDigits(now.day()));
      DateAndTime += " ";
      DateAndTime += (getDigits(now.hour()));
      DateAndTime += ':';
      DateAndTime += (getDigits(now.minute()));
      
      
      TempAndHumi = String("T:");
     
      int chk = DHT.read11(DHT11_PIN);
 
      TempAndHumi += DHT.temperature;
      TempAndHumi += "  H:";
      TempAndHumi += DHT.humidity;
  
      //char cbuff[32];
      //stringData.toCharArray(cbuff, 32);
      
      
      lcd.setCursor(0, 0);
      lcd.print(DateAndTime);
      
       /*for (int index = 0 ; index < sizeof(cbuff); index++) {
        Serial.print("Input");
        Serial.print(index);
        Serial.print("   \t");
        Serial.println(cbuff[index]);
     
      }
      save_string_to_eeprom(cbuff);
  
      for (int index = 0 ; index < 100 ; index++) { //EEPROM.length()*
        value = EEPROM.read(index);
        Serial.print(index);
        Serial.print("   \t");
        Serial.println(value, DEC);
      }
  
      addr = addr + 1;
      if (addr == EEPROM.length()) {
       addr = 0;
      }*/

    
       // set the cursor to column 0, line 1
       // (note: line 1 is the second row, since counting begins with 0):
       lcd.setCursor(0, 1);
       // print the number of seconds since reset:
       lcd.print(TempAndHumi);


        // read the value from the sensor:
      buttonValue = analogRead(buttonPin);
      lcd.print(buttonValue);
      digitalWrite(ledPin, HIGH);
      // stop the program for <sensorValue> milliseconds:
      delay(buttonValue);
      // turn the ledPin off:
      digitalWrite(ledPin, LOW);
      // stop the program for for <sensorValue> milliseconds:
      delay(buttonValue);

      //analogWrite(ledPin, brightness);
    
      delay(5000);
}

String getDigits(byte digits){
    // utility function for digital clock display: prints colon and leading 0
    String digitStr;
    if(digits < 10)
      digitStr += '0';
    digitStr += digits;   
    return digitStr;
}

void save_string_to_eeprom(char* stringIn){
  
  for(int i = 0; i <strlen(stringIn); i++){
      EEPROM.update(i, stringIn[i]);
  }

}
