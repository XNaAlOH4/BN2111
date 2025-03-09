#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int pulsePin = 0,
    blinkPin = 13;

volatile int BPM,
         Signal,
         IBI = 600;
volatile boolean Pulse = false,
         QS = false;
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM

//Function ptr used to hopefully optimise speed of Arduino
void (*signalToHeight)(int,byte**);

volatile byte graph[20] = {0},
         *graph_cell;

void signalToHeight_NotEnd(int sig, byte **cell) {
  // The height of each cell is only 8, so we need to change a range of [0,1024] to [0,7] so divide by 1024 and multiply by 7,
  // To do it faster, 2^10 = 1024, so shift down by 10 or >> 10;
  int height = 7-(sig >> 10)*7;

  (*cell) = height;

  (*cell)++;
  if(*cell == graph+19) {
    signalToHeight = signalToHeight_AtEnd;
    // Once last cell of the graph is reached, use the other function
    return;
  }
}

void signalToHeight_AtEnd(int sig, byte **cell) {
  int height = 7-(sig >> 10)*7;
  
  // If we have reached the last cell, shift all data points left and then copy data into the buffer
  memmove(*cell, (*cell)+1, sizeof(byte)*7);

  (*cell) = height;
}

void setup() {
  pinMode(blinkPin, OUTPUT);
  interruptSetup();

  lcd.begin(20, 4);

  graph_cell = graph;

  // These variables won't be used apart from now I think, so don't waste Arduino memory storing them
  byte heart[8] = {0b00000, 0b01010, 0b11111, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000};

/*  byte SpO2[4][8] = {
                  {0xe,0x11,0x10,0xe,0x1,0x11,0xe},
                  {0,0,0xc,0x12,0x12,0x1c,0x1,0x1},
                  {0xe,0x11,0x11,0x11,0xe,0,0,0},
                  {0,0,0x6,0x9,0x1,0x6,0xf,0}
  };*/

  byte graph_vals[7][8] = {
                  {0,0,0,0,0,0,0x1f,0x1f},
                  {0,0,0,0,0,0x1f,0x1f,0x1f},
                  {0,0,0,0,0x1f,0x1f,0x1f,0x1f},
                  {0,0,0,0x1f,0x1f,0x1f,0x1f,0x1f},
                  {0,0,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f},
                  {0,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f},
                  {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f},
  };

  lcd.createChar(0, heart);
  //for(int i = 0; i < 4; i++)lcd.createChar(i+1, SpO2[i]);
  for(int i = 0; i < 7; i++)lcd.createChar(i+1, graph_vals[i]);
  lcd.clear();
}

void loop() {
  // LCD should be first cleared in the TIMER2 ISR

  if(QS == true) {
    lcd.setCursor(0, 0);

    // Print BPM
    lcd.print("BPM :");
    lcd.print(BPM);
    lcd.write(byte(0));
    
    // Print SpO2
    lcd.setCursor(0, 1);
    //for(int i = 1; i < 5; i++) lcd.write(byte(i));
    lcd.print("SpO2 :");
    lcd.print(Signal);

    // Draw out the graph
    lcd.setCursor(0, 3);
    for(byte* ptr = graph; ptr != graph_cell+1; ptr++) {
      switch(*ptr) {
        case 0:
          lcd.write(" ");
          break;
        case 1:
          lcd.write("_");
          break;
        default:
          lcd.write(byte(*ptr-1));
      }
    }
    
    QS = false;
  }

  if(firstBeat == true) {
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("No HeartBeat detected");
    // Clear the graph
    memset(graph, 0, sizeof(byte)*20);
    // Reset our functions
    signalToHeight = signalToHeight_NotEnd;
  }

  delay(20);
}
