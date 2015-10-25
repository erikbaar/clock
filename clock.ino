// erik baar - 10/25
// building an ardunio clock based on the TFTLCD Library.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library


// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

unsigned long timer = millis();
unsigned long prevMillis = 0;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  tft.reset();

  uint16_t identifier = tft.readID();

  Serial.print("TFT ID IS: "); Serial.println(identifier);
  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  // Erik Baar - 10/25
  // force the chipset - this is a hack for the current adafruit board I ordered on
  // Amazon
  identifier = 0x9341;
  tft.begin(identifier);    
  tft.setRotation(1);

  timer = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  //tft.fillScreen(BLACK);
  
  prevMillis = timer;
  
  unsigned long perf = drawClock(timer);

  Serial.print("Perf: "); Serial.println(perf);
  
  timer = millis();

  unsigned long diff = timer - prevMillis;

  Serial.println(diff); 

  if (diff < 1000) {
    delay(1000 - (diff));
  } else {
    delay(1000);
  }

}

unsigned long drawClock(unsigned long lTimer) {
  //always return performance data
  unsigned long start = micros();

//  tft.fillRect(0, 0, 240, 320, BLACK);
  
  tft.setCursor(5,5);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(6);

  String clockText;

  unsigned long seconds = millis() / 1000;

  unsigned long hour = seconds / 3600;
  unsigned long minute = (seconds - hour * 3600)/60 ;
  unsigned long second = seconds - (hour * 3600 + minute * 60);

  // string concatenation is better performant than multiple print statements
  // writing to the monitor.  also, zero pad the string if the hour 
  // is less than what is expected.
  if (hour < 10) {
    clockText += "0";
  }
  clockText.concat(hour);
  clockText += ":";
  if (minute < 10) {
    clockText += "0";
  }
  clockText.concat(minute);
  clockText += ":";
  if (second < 10) {
    clockText += "0";
  }
  clockText.concat(second);

  tft.print(clockText);

  return micros() - start; // function performance in microseconds.
}

unsigned long drawLines() {
  

}

