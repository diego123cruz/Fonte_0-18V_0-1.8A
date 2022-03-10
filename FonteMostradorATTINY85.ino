/* Mostrador Fonte
   Attiny85 + Oled 128x32

  Fuse Low: 0xE2
  Fuse High: 0x5F
  EFuse: 0xFF

          ----_----
  V -----|         |--- VCC(5V)
  I -----| ATTINY  |--- SCL
  Imax --|   85    |---
  GND ---|_________|--- SDA


   
*/

#include <Tiny4kOLED.h>

#define pinV A0         // pin 1
#define pinA A3         // pin 2
#define pinImax A2      // pin 3

// OLED SCL             // pin 7
// OLED SDA             // pin 5

#define offset_V 0.0230
#define offset_A 0.00488
#define offset_Imax 0.01

void setup() {
  oled.begin();
  oled.clear();
  oled.on();
  oled.switchRenderFrame();
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

  // Clear the non-displayed half of the memory to all black
  // (The previous clear only cleared the other half of RAM)
  oled.clear();

  // The characters in the 8x16 font are 8 pixels wide and 16 pixels tall
  // 2 lines of 16 characters exactly fills 128x32
  oled.setFont(FONT8X16);


  oled.setCursor(0, 0);
  oled.print(F("V"));
  oled.setCursor(16, 0);
  oled.print(v_in * offset_V);
  

  oled.setCursor(70, 0);
  oled.print(F("I"));
  oled.setCursor(86, 0);
  oled.print(a_in * offset_A);


  oled.setCursor(0, 2);
  oled.print(F("I max:"));
  oled.setCursor(70, 2);
  oled.print(imax);

  // Swap which half of RAM is being written to, and which half is being displayed
  oled.switchFrame();
  delay(100);
}
