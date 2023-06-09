#include "RTClib.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <EasyButton.h>


// Easy button
#define button_pin 9
#define button_pin2 8
EasyButton button(button_pin);
EasyButton button2(button_pin2);
// Oled display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// RTC
#define DHTPIN 3
#define DHTTYPE DHT11
RTC_DS3231 rtc;
// DHT
DHT dht(DHTPIN, DHTTYPE);


// global variables
int item_sel = 0;
int totalItems = 2;
int menuSelected = 0;
bool menuState = true;
//

void setup() {
Serial.begin(9600); 
// RTC
rtc.begin();
rtc.adjust(DateTime(2023,6,8,9,20,40));
// dht
dht.begin();
// dispaly
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
	{ // Address for 128x64
		Serial.println(F("SSD1306 allocation failed"));
		for(;;); // Don't proceed, loop forever
	}
	display.setTextSize(2); // Normal 1:1 pixel scale
// easy button
button.begin();
button2.begin();
button.onPressed(menuSelectFunc);
button2.onPressed(menuActivator);
if (button.supportsInterrupt())
  {
    button.enableInterrupt(buttonISR);
    Serial.println("Button will be used through interrupts btn1");
  }
  if (button2.supportsInterrupt())
  {
    button2.enableInterrupt(buttonISR2);
    Serial.println("Button will be used through interrupts btn2");
  }
// MenuSelect to make sure it gets called atleast once
}

void loop() {
// delay(2000);
  // Serial.println(" ");

DateTime now = rtc.now();
  // put your main code here, to run repeatedly:
  // char final[13] = {""};
  // appened(&final);
	display.clearDisplay();
	display.setTextColor(WHITE); // Draw white text
  if(menuState == true){
  menuSelect();
	display.setCursor(1,1); // Start at top-left corner
  display.print("Clock");
	display.setCursor(1,21); // Start at top-left corner
  display.print("Humidity");
  display.setCursor(1,38); // Start at top-left corner
  display.print("Temp");
	display.display();
  }
  else {
  switch(item_sel){
case 0:
	display.setCursor(1,1); // Start at top-left corner
  Serial.println("case 0");
  display.clearDisplay();
  clock();
  display.display();
  break;
  case 1:
	display.setCursor(1,1); // Start at top-left corner
  Serial.println("case 1");
 display.clearDisplay();
  hum();
  display.display();
  break;
    case 2:
	display.setCursor(1,1); // Start at top-left corner
  Serial.println("case 2");
 display.clearDisplay();
  temp();
  display.display();
  break;

  }
	// delay(2000);
}
}
void menuSelect(){
  // if(menuState){
   if (item_sel > totalItems) {
   item_sel = 0;
   }
   switch (item_sel) {
   case 0:
         display.drawRoundRect(0, 0, 140, 18, 2, WHITE);
      display.drawRoundRect(0, 19, 140, 18, 2, BLACK);
      display.drawRoundRect(0, 30, 140, 18, 2, BLACK);
   break;
   case 1:
         display.drawRoundRect(0, 19, 140, 18, 2, WHITE);
      display.drawRoundRect(0, 0, 140, 18, 2, BLACK);
      display.drawRoundRect(0, 30, 140, 18, 2, BLACK);
   break;
   case 2:
      display.drawRoundRect(0, 37, 140, 18, 2, WHITE);
       display.drawRoundRect(0, 19, 140, 18, 2, BLACK);
      display.drawRoundRect(0, 0, 140, 18, 2, BLACK);
   break;
  }
}

  
  // }
  // else{
  //   return;
  // }
// CLock function
void clock(){
  DateTime now = rtc.now();
  display.print(now.hour() , DEC);
  display.print(":");
  display.print(now.minute() , DEC);
  display.print(":");
  display.print(now.second() , DEC);
  display.println("");
}
// temperature
void temp()
{
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  display.print(F("Temperature: c "));
  display.print(t);
  delay(2000);
}
void hum(){
  float h = dht.readHumidity();
if (isnan(h)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
}
display.print(F("Humidity: "));
  display.print(h);
  delay(2000);
}
// Easy button functions

void menuSelectFunc() {
    item_sel++;
}
void menuActivator(){
  menuState = !menuState;
  Serial.println(menuState);
}
void buttonISR()
{
  button.read();
}
void buttonISR2()
{
  button2.read();
}
