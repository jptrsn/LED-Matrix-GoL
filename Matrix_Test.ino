#include "LedControl.h"

#define Width 16
#define Height 16

/*
 * pin 12 is connected to the DATA
 * pin 11 is connected to the CLK 
 * pin 10 is connected to CS 
 */

LedControl lc=LedControl(12,11,10,4);
byte t1[16][16]  =  {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                     {0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0},
                     {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                     {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
                     {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                     {0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0},
                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                     {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0},
                     {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0},
                     {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0},
                     {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0},
                     {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                     {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

unsigned long delayTime=500;
//byte t1[16][16];                      
byte t2[16][16];
byte last[16][16];
int idx=0;
int noOfGeneration = 0;
byte A[8][8];
/*
byte A[8][8] = {{1, 0, 0, 0, 0, 0, 0, 1},
         {0, 0, 0, 0, 0, 0, 0, 1},
         {0, 0, 0, 0, 0, 0, 1, 1},
         {0, 0, 0, 0, 0, 0, 1, 1},
         {0, 0, 0, 0, 0, 0, 1, 1},
         {0, 0, 0, 0, 0, 1, 1, 0},
         {0, 0, 1, 0, 0, 1, 1, 0},
         {0, 0, 0, 1, 1, 0, 0, 0}};
*/
void setup() {
  Serial.begin(9600);
  //we have already set the number of devices when we created the LedControl
  int devices=lc.getDeviceCount();
  //we have to init all devices in a loop
  for(int address=0;address<devices;address++) {
    delay(10);
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,1);
    /* and clear the display */
    lc.clearDisplay(address);
  }
  //randomize(t1);
}

void loop() {
  display(t1);
  shiftUp();
  //fadeDisplay();
  waitForKeyPress();
  delay(delayTime);
}

void shiftUp() {
    for (int i=0; i<16;i++) {
    for (int j=0; j<16; j++) {
      t2[i][j]=t1[i][j];
      Serial.print(t2[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  for (int i=0; i<16; i++) {
    t1[15][i]=t2[0][i];
  }
  for (int i=0; i<15;i++) {
    for (int j=0; j<16; j++) {
      t1[i][j]=t2[i+1][j];
    }
  }

}

void fadeDisplay() {
  unsigned char delayInterval = 50;
  int devices=lc.getDeviceCount();
  for (int brightness = 0; brightness<16; brightness++) {
    for(int address=0;address<devices;address++) {
      lc.setIntensity(address,brightness);
      delay(delayInterval);
    }
    delay(delayInterval);
  }
  for (int brightness = 15; brightness>=0; brightness--) {
    for(int address=0;address<devices;address++) {
      lc.setIntensity(address,brightness);
      delay(delayInterval);
      
    }
    delay(delayInterval);
  }
  for (int address=0; address<4; address++) {
    lc.clearDisplay(address);
    delay(delayInterval);
  }
}

void display(byte t1[16][16]) {
  for(unsigned char matrixCount = 0; matrixCount < 4; matrixCount++) {
    lc.clearDisplay(matrixCount);
    for (unsigned int i=0; i<8;i++) {
      for (unsigned int j=0; j<8; j++) {
        switch (matrixCount) {
          case 1: {
            A[i][j] = t1[i][j];
          /*  Serial.print(i);
            Serial.print(" ");
            Serial.print(j);
            Serial.print(" ");
            Serial.println(A[i][j]);
         */   break;
          }
          case 0: {
            A[i][j] = t1[i][j+8];
         /*   Serial.print(i);
            Serial.print(" ");
            Serial.print(j+8);
            Serial.print(" ");
            Serial.println(A[i][j]);
         */   break;
          }
          case 2: {
            A[i][j] = t1[i+8][j];
         /*   Serial.print(i+8);
            Serial.print(" ");
            Serial.print(j);
            Serial.print(" ");
            Serial.println(A[i][j]);
         */   break;
          }
          case 3: {
            A[i][j] = t1[i+8][j+8];
        /*    Serial.print(i+8);
            Serial.print(" ");
            Serial.print(j+8);
            Serial.print(" ");
            Serial.println(A[i][j]);
          */  break;
          }
        }
        Serial.print(A[i][j]);
        Serial.print(" ");
      }
      
      Serial.println();
    }
    Serial.print("matrixCount: ");
    Serial.println(matrixCount);
    //waitForKeyPress();
    //delay(delayTime*5);
   
         
    updateDisplay(matrixCount);
  }
}

void waitForKeyPress() {
  Serial.println("Press enter");
  while (1) {
    int serialData = Serial.read();
    if (serialData == '\r' ) {
      break;
    }
  }
}

void updateDisplay(unsigned char address) {
  if (address <2 ) {
    for (int i=0; i<8; i++) {
      lc.setRow(address,i,rowValue(address,i));
      //delay(100);
    }
  } else {
    for (int i=0; i<8; i++) {
      lc.setRow(address,i,rowValue(address,7-i));
      //delay(100);
    }
  }
 
}

int rowValue(unsigned char address, byte i) {
  int result;
  switch (address) {
      case 0: {
        result = (A[i][0]*128) + (A[i][1]*64) + (A[i][2]*32) + (A[i][3]*16) + (A[i][4]*8) + (A[i][5]*4) + (A[i][6]*2) + (A[i][7]*1);
        break;
      }
      case 1: {
        result = (A[i][0]*128) + (A[i][1]*64) + (A[i][2]*32) + (A[i][3]*16) + (A[i][4]*8) + (A[i][5]*4) + (A[i][6]*2) + (A[i][7]*1);
        break;
      }
      case 2: {
        result = (A[i][7]*128) + (A[i][6]*64) + (A[i][5]*32) + (A[i][4]*16) + (A[i][3]*8) + (A[i][2]*4) + (A[i][1]*2) + (A[i][0]*1);
        break;
      }
      case 3: {
        result = (A[i][7]*128) + (A[i][6]*64) + (A[i][5]*32) + (A[i][4]*16) + (A[i][3]*8) + (A[i][2]*4) + (A[i][1]*2) + (A[i][0]*1);
        break;
      }
    }
  //Serial.println();
  //Serial.print(result);
  //Serial.println();
  //Serial.print(" ");
  //Serial.print(i);
  //Serial.print(" ");
  //Serial.println(result);
  
  return result;
}
