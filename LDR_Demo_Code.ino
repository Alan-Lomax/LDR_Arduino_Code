// #define Debug                 // choose debug or calibrate (or neither)
// #define Calibrate             // uncomment this line and use serial plotter. (Trend lines will show offset to apply lines 59-61)
// when offsets are correct all three lines should be within a 10 count of of each other.
//
#include "DblDelay.h"         // Use this to debounce the LDR signal after it is made into a boolean
// #############################################################
// ##                                                         ##
// ##  Testing LDR tuning                                     ##
// ##                                                         ##
// #############################################################
#define LDR1 A2   // LDR1 input pin. LDR always goes between signal pin and ground.
#define LDR2 A3
#define LDR3 A4
//
#define blueLED   A5      // Adjust to suit your LED outputs one for each LDR
#define yellowLED A0
#define redLED    A1
//
#define p    0.04        // smoothing coefficient (typically 0.01 < p < 0.1)
//
int value1;              // will hold the count from the LDR analog value
int value2;
int value3;

#define offset1  35      // Offsets to be applied to individual inputs to align them
#define offset2  0
#define offset3  20

int smoothVal = 300;     // Although this will be calculated providing a starting value allows it to converge quicker
#define ldrThreshold  35 // A constant to be added to the smoothed value to calculate an upper limit.
int upperLimit;          // This will be the calculated upper limit

DblDelay LDR1_TD( 100, 200, false); // time delay 'debounce' object with 100 ms on delay and 200 ms off delay
DblDelay LDR2_TD( 100, 200, false);
DblDelay LDR3_TD( 100, 200, false);

void setup() {
  // Setup serial monitor
  Serial.begin(115200);
  Serial.print(" ");
  Serial.println("Tuning LDR.");
  Serial.println(__FILE__);
  delay(20);   // As previous line could be quite long ... must allow buffer to clear

  // initialize output pins
  pinMode (blueLED, OUTPUT);
  pinMode (yellowLED, OUTPUT);
  pinMode (redLED, OUTPUT);

  // initialize input pins
  // LDR's are analog but need the pullup resistor enabled to work.
  pinMode(LDR1, INPUT_PULLUP);  // sets pull-up on analog pin
  pinMode(LDR2, INPUT_PULLUP);
  pinMode(LDR3, INPUT_PULLUP);
#ifdef Debug
  Serial.println("Value1, Value2, Value3");
#endif
#ifdef Calibrate
  Serial.println("Offset1, Offset2, Offset3");
#endif
}

void loop() {
  // 1: update LDR inputs.
  // Read three LDR's as analog and convert to three booleans if they differ from the average.
  // LDRS's are physically connected between input pin and ground. Using the internal pullup resistor this forms a divider circuit.
  // The three analogs are only read in once per loop (as analog conversion takes a relatively long time).
  // If one ldr is significantly different than the smoothed average it's corresponding boolean is activated.
  // It is expected that using the average value wil take care of normal ambient light variations
  value1 = analogRead(LDR1) - offset1;                // note fixed offsets to account for piece to piece variations AFTER installation
  value2 = analogRead(LDR2) - offset2;                // start with no offset (offset2 = 0). Use the serial plotter to determine values.
  value3 = analogRead(LDR3) - offset3;                // not all readings may need an offset - just make the three plots line up
  int avgValue = (value1 + value2 + value3) / 3;      // calculate an average of all 3 LDR's each time through.
  smoothVal = p * avgValue + (1 - p) * smoothVal;     // apply 'exponential smoothing' to the average
  upperLimit = smoothVal + ldrThreshold;              // calculate a new upper limit

#ifdef Debug
  Serial.print(value1);
  Serial.print(",");
  Serial.print(value2);
  Serial.print(",");
  Serial.print(value3);
  Serial.print(",");
  Serial.println(upperLimit);
  delay (20);
#endif

#ifdef Calibrate
  Serial.print(value1 - smoothVal);
  Serial.print(",");
  Serial.print(value2 - smoothVal);
  Serial.print(",");
  Serial.println(value3 - smoothVal);
  delay (20);
#endif

  // pass each LDR true / false state to the debounce function first
  LDR1_TD.input((value1 >= upperLimit) ? true : false);
  LDR2_TD.input((value2 >= upperLimit) ? true : false);
  LDR3_TD.input((value3 >= upperLimit) ? true : false);

  // check each LDR vs debounced upperLimit and set LED accordingly.
  digitalWrite (blueLED, LDR1_TD.output());
  digitalWrite (redLED, LDR2_TD.output());
  digitalWrite (yellowLED, LDR3_TD.output());;

  // 4: update the individual objects
  LDR1_TD.update();
  LDR2_TD.update();
  LDR3_TD.update();
}
