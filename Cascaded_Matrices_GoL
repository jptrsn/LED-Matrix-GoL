/*  Code by James Petersen, Copyright 2015
 *  Published under a Creative Commons Share-Alike 4.0 International Licence
 *  
 *  For more code, visit http://www.opensourceteacher.ca
 *  
 *  Many thanks to other contributors for their hard work:
 *  e.fahle@wayoda.org for the LedControl library
 *  JolliFactory for all the Game of Life calculations - http://www.instructables.com/id/Arduino-based-Bi-color-LED-Matrix-Game-of-Life/
 *  
 *  Feel free to use and modify this code as you see fit, but
 *  please ensure that you do not remove the attributions listed above.
 * */
 
#include "LedControl.h"

#define Width 16
#define Height 16
#define NumCycles 100 // Number of iterations before the game is reset. Lower this number if your code runs into bugs
/*
 * pin 12 is connected to the DataIn 
 * pin 11 is connected to the CLK 
 * pin 10 is connected to LOAD 
 */
LedControl lc=LedControl(12,11,10,4);

// Use this defined matrix to test the alignment of your modules. You should see numbers 1 through 4, starting in the top-left and moving clockwise.
// See lower for what to change if your matrices are not in the same order.
// Do not worry about getting the numbers to face up, as this doesn't affect the game,
// but if they do not appear in this order, the game will be slightly out of whack.
/*
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
                     
*/

unsigned int delayTime=2000; // Time between each step

byte t1[16][16];                      
byte t2[16][16];
byte last[16][16];
int idx=0;
int noOfGeneration = 0;
int numLoops = 0;
int iCount = 0;
byte A[8][8];

void setup() {
  Serial.begin(9600);

  int devices=lc.getDeviceCount();
  
  for(int address=0;address<devices;address++) {
    lc.shutdown(address,false);
    lc.setIntensity(address,5);
    lc.clearDisplay(address);
  }
  
  randomize(t1); // Comment out this line when checking for matrix alignment
}

void loop() {

  // This check for the maximum number of iterations, and restarts if necessary
  if (idx++ > NumCycles) {
    randomize(t1);   
    noOfGeneration = 0;     
    idx=0;
    numLoops++;
    clearDisplays();
    Serial.print("Number of total loops: ");
    Serial.println(numLoops);
    delay(delayTime);
  }

  // Compute the iterations of the game
  compute_previous_generation(t1,t2);
  compute_neighbouring_cells(t1,t2);
  compute_next_generation(t1,t2);

  // Display the new generation
  display(t1);
  Serial.print("Generation: ");
  Serial.println(noOfGeneration);
  delay(delayTime);
  
}

/*
// This is legacy, and does not work properly. Left in as a warning to other lines of code, to prevent them from acting out.
void fadeDisplay() {
  unsigned char delayInterval = delayTime/32;
  int devices=lc.getDeviceCount();
  for (int brightness = 0; brightness<17; brightness++) {
    for(int address=0;address<devices;address++) {
      lc.setIntensity(address,brightness);
      //delay(delayInterval);
    }
    delay(delayInterval);
  }
  for (int brightness = 16; brightness>=0; brightness--) {
    for(int address=0;address<devices;address++) {
      lc.setIntensity(address,brightness);
      //delay(delayInterval);
      
    }
    delay(delayInterval);
  }
}
*/

// Clear all displays and reset their intensity
void clearDisplays() {
  for(unsigned char matrixCount = 0; matrixCount < 4; matrixCount++) {
    lc.shutdown(matrixCount,false);
    lc.setIntensity(matrixCount,5);
    lc.clearDisplay(matrixCount);
  }
}

// This function breaks the large 16x16 grid into four smaller grids, and sends them to the appropriate display
void display(byte t1[16][16]) {
  for(unsigned char matrixCount = 0; matrixCount < 4; matrixCount++) {
    for (unsigned int i=0; i<8;i++) {
      for (unsigned int j=0; j<8; j++) {
        switch (matrixCount) {
          case 1: { // Change these numbers to move your matrix display
            A[i][j] = t1[i][j];
            break;
          }
          case 0: { // Change these numbers to move your matrix display
            A[i][j] = t1[i][j+8];
            break;
          }
          case 2: { // Change these numbers to move your matrix display
            A[i][j] = t1[i+8][j];
            break;
          }
          case 3: { // Change these numbers to move your matrix display
            A[i][j] = t1[i+8][j+8];
            break;
          }
        }
        //Serial.print(A[i][j]);
        //Serial.print(" ");
      }
      
      //Serial.println();
    }
    Serial.print("matrixCount: ");
    Serial.println(matrixCount);
    updateDisplay(matrixCount);
  }
}

// Displays the given matrix address with the values from the appropriate 8x8 matrix stored in A[8][8]
void updateDisplay(unsigned char address) {
  if (address <2 ) {
    for (int i=0; i<8; i++) {
      lc.setRow(address,i,rowValue(address,i));
      // lc.setRow(address,i,rowValue(address,7-i)); // Use this line if numbers 1 or 2 appear upside down
      
    }
  } else {
    for (int i=0; i<8; i++) {
      // lc.setRow(address,i,rowValue(address,i)); // Use this line if numbers 3 or 4 appear upside down
      lc.setRow(address,i,rowValue(address,7-i));
    }
  }
}

// Compute the row value, given the address and the row index
int rowValue(unsigned char address, byte i) { //rowValue(address, whichRow)
  int result;
  switch (address) {
      case 0: {
        result = (A[i][0]*128) + (A[i][1]*64) + (A[i][2]*32) + (A[i][3]*16) + (A[i][4]*8) + (A[i][5]*4) + (A[i][6]*2) + (A[i][7]*1);
        // result = (A[i][7]*128) + (A[i][6]*64) + (A[i][5]*32) + (A[i][4]*16) + (A[i][3]*8) + (A[i][2]*4) + (A[i][1]*2) + (A[i][0]*1); // Use this line if number 1 appears mirrored
        break;
      }
      case 1: {
        result = (A[i][0]*128) + (A[i][1]*64) + (A[i][2]*32) + (A[i][3]*16) + (A[i][4]*8) + (A[i][5]*4) + (A[i][6]*2) + (A[i][7]*1);
        // result = (A[i][7]*128) + (A[i][6]*64) + (A[i][5]*32) + (A[i][4]*16) + (A[i][3]*8) + (A[i][2]*4) + (A[i][1]*2) + (A[i][0]*1); // Use this line if number 2 appears mirrored
        break;
      }
      case 2: {
        result = (A[i][7]*128) + (A[i][6]*64) + (A[i][5]*32) + (A[i][4]*16) + (A[i][3]*8) + (A[i][2]*4) + (A[i][1]*2) + (A[i][0]*1);
        // result = (A[i][0]*128) + (A[i][1]*64) + (A[i][2]*32) + (A[i][3]*16) + (A[i][4]*8) + (A[i][5]*4) + (A[i][6]*2) + (A[i][7]*1); // Use this line if number 3 appears mirrored
        break;
      }
      case 3: {
        result = (A[i][7]*128) + (A[i][6]*64) + (A[i][5]*32) + (A[i][4]*16) + (A[i][3]*8) + (A[i][2]*4) + (A[i][1]*2) + (A[i][0]*1);
        // result = (A[i][0]*128) + (A[i][1]*64) + (A[i][2]*32) + (A[i][3]*16) + (A[i][4]*8) + (A[i][5]*4) + (A[i][6]*2) + (A[i][7]*1); // Use this line if number 4 appears mirrored
        break;
      }
    }
  return result;
}

// This code is directly from the JolliFactory example. No promises from the author as to its accuracy or effecicacy.
void compute_previous_generation(byte t1[16][16],byte t2[16][16])
{
  byte i,j;

  for(i=0;i<Width;i++)
  {
    for(j=0;j<Height;j++)
    {
      t2[i][j]=t1[i][j];
      last[i][j]=t1[i][j];
    }
  }
}

// This code is directly from the JolliFactory example. No promises from the author as to its accuracy or effecicacy.
void compute_next_generation(byte t1[16][16],byte t2[16][16])
{
  byte i,j;

  for(i=0;i<Width;i++)
  {
    for(j=0;j<Height;j++)
    {
      t1[i][j]=t2[i][j];
    }
  }
  
  noOfGeneration++;
  //Serial.println(noOfGeneration);
}

// This code is directly from the JolliFactory example. No promises from the author as to its accuracy or effecicacy.
void compute_neighbouring_cells(byte t1[16][16],byte t2[16][16]) {
  byte i,j,a;

  for(i=0;i<Width;i++)
  {
    for(j=0;j<Height;j++)
    {
      if((i==0)&&(j==0))
      {
        a=t1[i][j+1]+t1[i+1][j]+t1[i+1][j+1]+t1[i][Height-1]+t1[i+1][Height-1]+t1[Width-1][j]+t1[Width-1][j+1]+t1[Width-1][Height-1];
      }

      if((i!=0)&&(j!=0)&&(i!=(Width-1))&&(j!=(Height-1)))
      {
        a=t1[i-1][j-1]+t1[i-1][j]+t1[i-1][j+1]+t1[i][j+1]+t1[i+1][j+1]+t1[i+1][j]+t1[i+1][j-1]+t1[i][j-1];
      }
      
      if((i==0)&&(j!=0)&&(j!=(Height-1)))
      {
        a=t1[i][j-1]+t1[i+1][j-1]+t1[i+1][j]+t1[i+1][j+1]+t1[i][j+1]+t1[Width-1][j-1]+t1[Width-1][j]+t1[Width-1][j+1];
      }

      if((i==0)&&(j==(Height-1)))
      {
        a=t1[i][j-1]+t1[i+1][j-1]+t1[i+1][j]+t1[i][0]+t1[i+1][0]+t1[Width-1][0]+t1[Width-1][j]+t1[Width-1][j-1];
      }
      
      if((i==(Width-1))&&(j==0))
      {
        a=t1[i-1][j]+t1[i-1][j+1]+t1[i][j+1]+t1[i][Height-1]+t1[i-1][Height-1]+t1[0][j]+t1[0][j+1]+t1[0][Height-1];
      }
      
      if((i==(Width-1))&&(j!=0)&&(j!=(Height-1)))
      {
        a=t1[i][j-1]+t1[i][j+1]+t1[i-1][j-1]+t1[i-1][j]+t1[i-1][j+1]+t1[0][j]+t1[0][j-1]+t1[0][j+1];
      }
      
      if((i==(Width-1))&&(j==(Height-1)))
      {
        a=t1[i][j-1]+t1[i-1][j-1]+t1[i-1][j]+t1[0][j]+t1[0][j-1]+t1[i][0]+t1[i-1][0]+t1[0][0];
      }

      if((i!=0)&&(i!=(Width-1))&&(j==0))
      {
        a=t1[i-1][j]+t1[i-1][j+1]+t1[i][j+1]+t1[i+1][j+1]+t1[i+1][j]+t1[i][Height-1]+t1[i-1][Height-1]+t1[i+1][Height-1];
      }

      if((i!=0)&&(i!=(Width-1))&&(j==(Height-1)))
      {
        a=t1[i-1][j]+t1[i-1][j-1]+t1[i][j-1]+t1[i+1][j-1]+t1[i+1][j]+t1[i][0]+t1[i-1][0]+t1[i+1][0];
      }

      if((t1[i][j]==0)&&(a==3)){t2[i][j]=1;}                   // populate if 3 neighours around it
      if((t1[i][j]==1)&&((a==2)||(a==3))){t2[i][j]=1;}         // stay alive if 2 or 3 neigbours around it
      if((t1[i][j]==1)&&((a==1)||(a==0)||(a>3))){t2[i][j]=0;}  // die if only one neighbour or over-crowding with 4 or more neighours
    }
  }
}

// This code is directly from the JolliFactory example. No promises from the author as to its accuracy or effecicacy.
void randomize(byte t1[16][16]) {
  byte i,j;
  randomSeed(millis());
  for(i=0;i<Width;i++)
  {
    for(j=0;j<Height;j++)
    {
      t1[i][j]=random(2);
    }
  }
}
