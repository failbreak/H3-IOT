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
int totalItems = 1;
bool menuState;
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
// button2.begin();
// button2.onPressed();
button.onPressed(onPressed);
if (button.supportsInterrupt())
  {
    button.enableInterrupt(buttonISR);
    Serial.println("Button will be used through interrupts");
  }
// MenuSelect to make sure it gets called atleast once
}

void loop() {
// delay(2000);
  // Serial.print(F("Humidity: "));
  // Serial.print(t);
  Serial.println(" ");
  menuSelect();

DateTime now = rtc.now();
  // put your main code here, to run repeatedly:
  // char final[13] = {""};
  // appened(&final);
	display.clearDisplay();
	display.setTextColor(WHITE); // Draw white text
  switch(){
case 0:
	display.setCursor(1,1); // Start at top-left corner
  display.print("Clock");
	display.setCursor(1,21); // Start at top-left corner
  display.print("Humidity");
  break;
  case 1:

  break;
  
  }
	display.display();
	// delay(2000);
}


void menuSelect(){
  if(menuState){
   if (item_sel > totalItems) {
   item_sel = 0;
   }
   if (item_sel != 0) {
      display.drawRoundRect(0, 19, 140, 18, 2, WHITE);
      display.drawRoundRect(0, 0, 140, 18, 2, BLACK);
   }
   else {
      display.drawRoundRect(0, 0, 140, 18, 2, WHITE);
      display.drawRoundRect(0, 19, 140, 18, 2, BLACK);
   }
  }
  else{
    return;
  }
}
void clock(){
  DateTime now = rtc.now();
  display.print(now.hour() , DEC);
  display.print(":");
  display.print(now.minute() , DEC);
  display.print(":");
  display.print(now.second() , DEC);
  display.println("");
}
void temp()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
}
// Easy button functions

void onPressed() {
    item_sel++;
}
void buttonISR()
{
  button.read();
}