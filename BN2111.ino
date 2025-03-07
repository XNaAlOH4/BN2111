#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int pulsePin = 0,
    blinkPin = 13;

volatile int BPM,
         Signal,
         IBI = 600;
volatile boolean Pulse = false,
         QS = false;

//Function ptr used to hopefully optimise speed of Arduino
void (*signalToHeight)(int,byte**,char*);

volatile byte graph[20*8][8] = {0},
         **graph_cell,
         graph_pixel;

void signalToHeight_NotEnd(int sig, byte ***cell, char *pixel) {
  // The height of each cell is only 8, so we need to change a range of [0,1024] to [0,7] so divide by 1024 and multiply by 7,
  // To do it faster, 2^10 = 1024, so shift down by 10 or >> 10;
  int height = 7-(sig >> 10)*7;
  byte set = (1 << *pixel);// the pixel to set

  for(int i = 7; i >= height; i--) {// Since 0 is the top of the cell and 7 is the bottom, start from 7, then go down till height
    (*cell)[i] |= set;
  }

  if((*pixel)-- == 0) {
    // If the pixel is the last pixel in the cell, reset pixel and move cell forward
    *pixel = 4;
    *cell++;
    if(*cell == graph+19) {
      signalToHeight = signalToHeight_AtEnd;
      // Once last cell of the graph is reached, use the other function
      return;
    }
  }
}

void signalToHeight_AtEnd(int sig, byte ***cell, char *pixel) {
  int height = 7-(sig >> 10)*7;
  byte set = (1 << *pixel);// the pixel to set
  // If we have reached the last cell, shift all data points left and then copy data into the buffer
  byte** ptr = graph;
  for(int j = 0; j < 8; j++) ptr[0][j] <<= 1;
  for(int i = 1; i < 19; i++) {
    for(int j = 0; j < 8; j++) {
      ptr[i-1][j] |= ( (ptr[i][j] & 0x10) > 0);
      ptr[i][j] <<= 1;
    }
  }

  for(int i = 7; i >= height; i--) (*cell)[i] |= set;

  if((*pixel)-- == 0) *pixel = 4;  
}

void setup() {
  lcd.begin(20, 4);

  graph_cell = graph;
  graph_cell = 4;// the 4th bit is the first pixel, and the 0th bit is the last pixel

  // These variables won't be used apart from now I think, so don't waste Arduino memory storing them
  byte heart[8] = {0b00000, 0b01010, 0b11111, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000};

  byte SpO2[4][8] = {
                  {0xe,0x11,0x10,0xe,0x1,0x11,0xe},
                  {0,0,0xc,0x12,0x12,0x1c,0x1,0x1},
                  {0xe,0x11,0x11,0x11,0xe,0,0,0},
                  {0,0,0x6,0x9,0x1,0x6,0xf,0}
  };

  lcd.createChar(0, heart);
  for(int i = 0; i < 4; i++)lcd.createChar(i+1, SpO2[i]);
  lcd.clear();
}

void loop() {
  if(QS == true) {
    lcd.setCursor(0, 0);

    // Print BPM
    lcd.print("BPM :");
    lcd.print(BPM);
    lcd.write(byte(0));
    // Print SpO2
    lcd.setCursor(0, 1);
    for(int i = 1; i < 5; i++) lcd.write(byte(i));
    lcd.print(":");
    lcd.print(Signal);

    // Draw out the graph
    lcd.setCursor(0, 3);
    for(byte** ptr = graph; ptr != cell+1; ptr++) {
      lcd.write(*ptr);
    }
    
    QS = false;
  }

  if(firstBeat == true) {
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("No HeartBeat detected");
    // Clear the graph
    for(int i = 0; i < 20; i++) memset(graph[i], 0, sizeof(byte)*8);
    // Reset our functions
    signalToHeight = signalToHeight_NotEnd;
  }

  delay(20);
}
