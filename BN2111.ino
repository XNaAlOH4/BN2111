#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int pulsePin = 0,
    blinkPin = 13,
    tooHighPin = 6;

volatile int BPM = 0;
volatile int Signal;
volatile int IBI = 600;
volatile boolean Pulse = false;
volatile boolean QS = false;
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM

volatile byte graph_cell;
int fade;

void setup() {
  Serial.begin(9600);
  pinMode(blinkPin, OUTPUT);
  pinMode(tooHighPin, OUTPUT);
  digitalWrite(tooHighPin, LOW);

  lcd.begin(20, 4);

  // These variables won't be used apart from now I think, so don't waste Arduino memory storing them
  //byte heart[8] = {0b00000, 0b01010, 0b11111, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000};

  byte heart_vals[8][8] = {
                  {0,0xa,0x1f,0x1f,0x1f,0xe,4,0},
                  {0,0xa,0x1f,0x1f,0x1f,0xe,4,0},
                  {0,0,0x1f,0x1f,0x1f,0xe,4,0},
                  {0,0,0,0x1f,0x1f,0xe,4,0},
                  {0,0,0,0,0x1f,0xe,4,0},
                  {0,0,0,0,0,0xe,4,0},
                  {0,0,0,0,0,0,4,0},
                  {0,0,0,0,0,0,0,0},
  };

  for(int i = 0; i < 8; i++)lcd.createChar(i, heart_vals[i]);
  lcd.clear();

  interruptSetup();
}

void loop() {
  // LCD should be first cleared in the TIMER2 ISR

  if(QS == true) {
    lcd.clear();
    lcd.setCursor(0, 2);

    // Print BPM
    lcd.print("BPM :");
    lcd.print(BPM);
    lcd.write(byte(0));
    lcd.print(" ");

    if(BPM > 100) {
      lcd.print("TOO HIGH");
    }else {
      digitalWrite(tooHighPin, LOW);
      lcd.print("         ");
    }
    
    QS = false;
  }

  if(firstBeat == true) {
    lcd.clear();
    lcd.setCursor(0, 2);
    lcd.print("Please Place Finger");
    //lcd.setCursor(0, 3);
    //lcd.print(" ");
  }else {
    // Draw the beat
    lcd.setCursor(0, 3);
    lcd.write(byte(graph_cell));
    if(fade & 1) {// If the first beat of flip is set
      if(graph_cell > 0) graph_cell--;
    }
    fade = (fade + 1) % 4;
    //fade ^= 1;// XOR operator, flips fade between 0 and 1
  }

  delay(20);
}
