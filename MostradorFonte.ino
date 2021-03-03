// Mostrador para fonte
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define pinV A0
#define pinA A1
#define pinImax A2

#define offset_V 0.0230
#define offset_A 0.00488
#define offset_Imax 0.01

void setup() {
  Serial.begin(9600);

  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
}

void loop() {
  double v_in = 0;
  double a_in = 0;
  double i_max = 0;

  for (int i = 0; i < 30; i++) {
    v_in += analogRead(pinV);
    a_in += analogRead(pinA);
    i_max += analogRead(pinImax);
  }

  v_in = v_in / 30;
  a_in = a_in / 30;
  i_max = i_max / 30;

  double imax = (i_max * offset_A) - offset_Imax;
  if (imax < 0) imax = 0;
  
  Serial.print("V: ");
  Serial.print(v_in * offset_V);
  Serial.print("  ");
  Serial.print("I: ");
  Serial.print(a_in * offset_A);
  Serial.print("  ");
  Serial.print("I max: ");
  Serial.print(imax);
  Serial.println();


  // ------------------------- MOSTRA NO DISPLAY -------------------------
  display.clearDisplay();
  display.setTextSize(2);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0);             
  display.println(F("V"));

  display.setTextSize(2);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(45,0);             
  display.println(v_in * offset_V);
  
  display.setTextSize(2);          
  display.setTextColor(SSD1306_WHITE);     
  display.setCursor(0,16);   
  display.println(F("I"));

  display.setTextSize(2);        
  display.setTextColor(SSD1306_WHITE);   
  display.setCursor(45,16);  
  display.println(a_in * offset_A);


  // ------- Corrente maxima
  display.setTextSize(2);            
  display.setTextColor(SSD1306_WHITE);    
  display.setCursor(0,48);            
  display.println(F("I"));

  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(16,48);             
  display.println(F("max"));

  display.setTextSize(2);            
  display.setTextColor(SSD1306_WHITE);   
  display.setCursor(45,48); 
  display.println(imax);

  display.display();
  
  delay(100);
}
