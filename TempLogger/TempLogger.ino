// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
//#include <Wire.h>
#include <RTClib.h>
#include <dht.h>
//#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHT11_PIN 
#define ONE_WIRE_BUS 3

String DateAndTime;
String TempAndHumi;

int addr = 0;
byte value;

int buttonPin = A0;
int ledPin = 13;
int buttonValue = 0;

const int ButtonUp = 1;
const int ButtonDown = 2;
const int ButtonLeft = 3;
const int ButtonRight = 4;
const int ButtonEnter = 5;

const int BUTTONULOW = 150;
const int BUTTONUHIGH = 250;
const int BUTTONDLOW = 300;
const int BUTTONDHIGH = 400;
const int BUTTONLLOW = 500;
const int BUTTONLHIGH = 600;
const int BUTTONRLOW = 0;
const int BUTTONRHIGH = 100;
const int BUTTONELOW = 700;
const int BUTTONEHIGH = 800;

dht DHT;
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
RTC_DS1307 rtc;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonState; 
int lastButtonState = LOW;

void setup () {

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  while (!Serial);
  Serial.begin(9600);

  lcd.begin(16, 2);
  sensors.begin(); 

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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
  
  sensors.requestTemperatures();
  TempAndHumi = String("T:");
  TempAndHumi += sensors.getTempCByIndex(0);

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
  //buttonValue = digitalRead(buttonPin);
  //lcd.print(buttonValue);
  //digitalWrite(ledPin, HIGH);
  // stop the program for <sensorValue> milliseconds:
  //delay(buttonValue);
  // turn the ledPin off:
  //digitalWrite(ledPin, LOW);
  // stop the program for for <sensorValue> milliseconds:
  //Serial.println(buttonValue);

  //analogWrite(ledPin, brightness);

  int tmpButtonState = LOW; 

  if (buttonValue > BUTTONULOW && buttonValue < BUTTONUHIGH) {
    tmpButtonState = ButtonUp;
  } else if (buttonValue > BUTTONDLOW && buttonValue < BUTTONDHIGH) {
    tmpButtonState = ButtonDown;
  } else if (buttonValue > BUTTONLLOW && buttonValue < BUTTONLHIGH) {
    tmpButtonState = ButtonLeft;
  } else if (buttonValue > BUTTONRLOW && buttonValue < BUTTONRHIGH) {
    tmpButtonState = ButtonRight;
  } else if (buttonValue > BUTTONELOW && buttonValue < BUTTONEHIGH) {
    tmpButtonState = ButtonEnter;
  }

   // If the switch changed, due to noise or pressing:
   if (tmpButtonState != lastButtonState) {
     // reset the debouncing timer
     lastDebounceTime = millis();
   }

   if ((millis() - lastDebounceTime) > debounceDelay && tmpButtonState > 0 ) {
     // whatever the reading is at, it's been there for longer
     // than the debounce delay, so take it as the actual current state:
     buttonState = tmpButtonState;
     Serial.println(buttonState);
     delay(500);
   }

   // save the reading.  Next time through the loop,
   // it'll be the lastButtonState:
   lastButtonState = tmpButtonState;
   delay(1000);
}

String getDigits(byte digits) {
  // utility function for digital clock display: prints colon and leading 0
  String digitStr;
  if (digits < 10)
    digitStr += '0';
  digitStr += digits;
  return digitStr;
}

void goToSleep(byte sleepSeconds) {
  /*
  Wire.beginTransmission(RTCADDR);
  Wire.write(RTCSEC);
  Wire.write(0x80);//back to zero, but start counting
  Wire.endTransmission();

  Wire.beginTransmission(RTCADDR);//set the alarm time
  Wire.write(ALM0SEC);
  Wire.write(sleepSeconds);//seconds only
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.write(0x00);//clears flag
  Wire.endTransmission();
  */

  //BOD DISABLE - this must be called right before the __asm__ sleep instruction
  MCUCR |= (3 << 5); //set both BODS and BODSE at the same time
  MCUCR = (MCUCR & ~(1 << 5)) | (1 << 6); //then set the BODS bit and clear the BODSE bit at the same time
  __asm__  __volatile__("sleep");//in line assembler to go to sleep
}
/*
void save_string_to_eeprom(char* stringIn) {
  for (int i = 0; i < strlen(stringIn); i++) {
    EEPROM.update(i, stringIn[i]);
  }
}
*/
