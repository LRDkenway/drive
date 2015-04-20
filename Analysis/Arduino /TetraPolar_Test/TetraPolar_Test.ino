
#include "Wire.h"
#include "Math.h"
#include "AD5933.h" //Library for AD5933 functions (must be installed)

// ================================================================
// Constants
// ================================================================

#define TWI_FREQ 400000L      // Set TWI/I2C Frequency to 400MHz.

const int CYCLES_BASE = 15;       // Cycles to ignore before a measurement is taken. Max is 511.

const int CYCLES_MULTIPLIER = 1;    // Multiple for CYCLES_BASE. Can be 1, 2, or 4.

const int CAL_RESISTANCE = 353;  // Calibration resistance for the gain factor. 

const int CAL_SAMPLES = 10;         // Number of measurements to take of the calibration resistance.

const int LED7_R = 3;

const int LED7_G = 4;

const int LED7_B = 5;

const int LED5 = 13;

const int LED6 = 12;

const int LED8 = 11;

const int PUSH1 = 27; // Push button 1:  Switch 2

const int PUSH2 = 28; // Push button 2:  Switch 3

const int BOOST = 22; // 5V On

const int BI_TETRA = 29; // Selects between Bi-Polar & Tetra-Polar modes

const int SW_VI = 23; // Voltage current multiplexer

const int DELAY = 10; // Delay between toggling the multiplexer

int leds[6] = {
  LED5, LED6, LED7_R, LED7_G, LED7_B, LED8};

// Define bit clearing and setting variables

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// ================================================================
// Dynamic variables
// ================================================================

int ctrReg = 0; // Initialize control register variable.

int incomingByte = 0; // Bytes read from the serial monitor.

int firstByte = 0; // First byte read from the serial monitor. Used to set AD5933 mode.

int bytesAvailable = 0; // Bytes available to read.

int startFreq = 50; // AC start frequency in kilohertz.

int stepSize = 0;  // Step size of AC frequency sweep in kilohertz.

int numOfIncrements = 0;  // Number of increments in frequency sweep.

double GAIN_FACTOR = 0;

double VOLTAGE_PHASE = 0;

double CURRENT_PHASE = 0;

double z_mag = 0;

double phaseAngle = 0;

double temp = 0;

void setup() {

  // ================================================================
  // For AD5933
  // ================================================================

  Serial.println();
  Serial.println();

  // put your setup code here, to run once:
  //Set IOs mode 

  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);  
  pinMode(LED7_R, OUTPUT);
  pinMode(LED7_G, OUTPUT);
  pinMode(LED7_B, OUTPUT);
  pinMode(LED8, OUTPUT);

  pinMode(BI_TETRA, OUTPUT);
  pinMode(SW_VI, OUTPUT);


  pinMode(BOOST, OUTPUT);

  pinMode(PUSH1, INPUT);
  pinMode(PUSH2, INPUT);

  pinMode(25, INPUT);
  pinMode(24, INPUT);

  //All leds off
  for(int i=0;i<6;i++) {
    digitalWrite(leds[i],HIGH);
  }

  digitalWrite(BOOST, HIGH);
  digitalWrite(BI_TETRA, LOW); // Set AD5933 to tetra-polar mode

  Wire.begin(); // Start Arduino I2C library
  Serial.begin(38400); // Open serial port
  Serial.println();

  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1); // Clear bits in port

  AD5933.setExtClock(false); 
  AD5933.resetAD5933(); 
  AD5933.setSettlingCycles(CYCLES_BASE, CYCLES_MULTIPLIER); 
  AD5933.setStartFreq(50000);
  AD5933.setRange(1);
  AD5933.setPGA(1);
  temp = AD5933.getTemperature(); 

  Serial.println(AD5933.getGainFactorTetra(CAL_RESISTANCE, CAL_SAMPLES, GAIN_FACTOR, VOLTAGE_PHASE, CURRENT_PHASE, false));

  Serial.print("Current phase is: ");
  Serial.println(CURRENT_PHASE);
  Serial.print("Voltage phase is: ");
  Serial.println(VOLTAGE_PHASE);
  Serial.print("Gain factor is: ");
  Serial.println(GAIN_FACTOR); 
  Serial.println();  
}

void loop() {

  if (Serial.available()) {
    delay(15);
    uint8_t ch = Serial.read();
    uint8_t status;

    if (ch == '1') {
      Serial.println(AD5933.getComplexTetra(DELAY, GAIN_FACTOR, VOLTAGE_PHASE, CURRENT_PHASE, z_mag, phaseAngle));

      Serial.print(z_mag);
      Serial.print(",");
      Serial.print(phaseAngle); 

      Serial.println();
      Serial.println();      
    }

  } // end if serial available

}






























