// Example_11_LCD_Display.ino
/*
 Codes are modified from examples provided for LiquidCrystal Library.
 
 Demonstrates the use a LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Refer example code in the public domain:
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7)

// make some custom characters:
byte heart[8] = {0b00000, 0b01010, 0b11111, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000};
byte smiley[8] = {0b00000, 0b00000, 0b01010, 0b00000, 0b00000, 0b10001, 0b01110, 0b00000};
byte frownie[8] = {0b00000, 0b00000, 0b01010, 0b00000, 0b00000, 0b00000, 0b01110, 0b10001};
byte armsDown[8] = {0b00100, 0b01010, 0b00100, 0b00100, 0b01110, 0b10101, 0b00100, 0b01010};
byte armsUp[8] = {0b00100, 0b01010, 0b00100, 0b10101, 0b01110, 0b00100, 0b00100, 0b01010};

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  
  // create new characters
  // support up to 8 characters of 5x8 pixels
  // numbered as 0 to 7
  lcd.createChar(0, heart);
  lcd.createChar(1, smiley);
  lcd.createChar(2, frownie);
  lcd.createChar(3, armsDown);
  lcd.createChar(4, armsUp);
  
  // control contrast from Arduino PWM
  // use Timer 1, PWM at pin 9 or 10
  // connect LCD VO (pin 3) to Arduino pin 9
  //TCCR1B = (TCCR1B & 0b11111000) | 0b001;
  //analogWrite(9, 60);
}

void loop() 
{
  lcd.clear(); // clear screen 

  // lcd.rightToLeft(); // go right for the next letter
  // lcd.leftToRight(); // go left for the next letter (default)
 
  lcd.home(); // go to (0,0)
  
  // set the cursor to (col,row)=(0,0), counting begins with 0
  // lcd.setCursor(0, 0); 
  
  // print from 0 to 9:
  for (int thisNum = 0; thisNum < 10; thisNum++) 
  {
    lcd.print(thisNum);
    delay(500);
  }
   
  // set the display to automatically scroll
  lcd.autoscroll();
  lcd.setCursor(20, 3); // set the cursor to (col,row)
  
  // print from 0 to 9:
  for (int thisNum = 0; thisNum < 10; thisNum++) 
  {
    lcd.print(thisNum);
    delay(500);
  }
  lcd.noAutoscroll();  // turn off automatic scrolling

  lcd.clear();
  lcd.setCursor(0, 0);
  
  // Print a message to the LCD.
  lcd.print("hello, world!");
  for(int i=0; i<=4; i++)
  {
     lcd.scrollDisplayRight(); // scroll one position right
     delay(200);
  }
  for(int i=0; i<=4; i++) 
  {
     lcd.scrollDisplayLeft();  // scroll one position left
     delay(200);
  }
   
  for(int i=10; i<=20; i++)
  {
    // if (i >= 15) lcd.blink();     // Turn on the blinking cursor
    // if (i >= 15) lcd.cursor();    // Turn on the cursor
    // if (i >= 15) lcd.noDisplay(); // Turn off the display
    lcd.setCursor(16, 1);
    lcd.print(i);
    delay(300);
  }
  // lcd.noBlink();    // Turn off the blinking cursor
  // lcd.noCursor();   // Turn off the cursor
  // lcd.display();    // Turn on the display

  // Print a number on the lcd
  lcd.clear();
  float number = 78.12345678;
  lcd.setCursor(0, 0);
  lcd.print("number = ");
  lcd.print(number);
  lcd.setCursor(0, 1);
  lcd.print("no. = ");
  lcd.print(number, 5);
  lcd.setCursor(0, 2);
  lcd.print("01234567890123456789");
  lcd.setCursor(0, 3);
  char str[] = "20 characters / line";
  lcd.print(str);
  delay(3000);
  
// Print a message on the lcd
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("I ");
  lcd.write((byte) 0); // when calling lcd.write(data), data must be cast as a byte
  lcd.print(" BN2111 ");
  lcd.write((byte) 1); // or byte(1)
  lcd.print(" ! ");
  lcd.write((byte) 2);
  lcd.print(" ");
  lcd.write((byte) 3);
  lcd.print(" ");
  lcd.write((byte) 4);
  delay(3000);
}
