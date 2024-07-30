//########## THIS FILE CONTAINS GYRO-RELATED DATA AND FUNCTIONS #############
//**** INCLUDES *************************************************************
#include<Wire.h>

//**** CONSTANTS ************************************************************
static const int MPU_addr = 0x69;                 // 0x68 0r 0x69 I2C address of the MPU-6050
static const float CalibrationFilterCoeff = 0.05; // Low-pass filter coefficient
static const float GyroRate = 250;                // Gyro full scale in Degrees per second
static const float MaxDegrees = 35;               // "35" means: +/-35° rotation is reported as +/-100%
static const float MovementDetectionThreshold = 700;// Threshold for detecting movement in raw data
static const int MillisecondsToCalibrate = 2500;  // Calibrating will be done, if there is no movement for more than this time

//**** GLOBAL VARIABLES *****************************************************
static int ledCalibrate;   // Shows that the board is in calibration mode now

//**** 'gyro_initialize()' prepares the Gyro for being used *****************
void gyro_initialize(int led) {
  ledCalibrate = led;
  
  // Configure MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     //   => set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1A);  // Config Low Pass Filter
  Wire.write(0x06);  //   => set to 5 Hz, Delay 13,4 ms
  Wire.endTransmission(true);
}
  
/**** 'gyro_getRotation()' senses, if the sensor is in movement. Then it either
 ****  calibrates (no movement), or delivers the rotation in % (scale +/-100)
 ****/
float* gyro_getRotation() {
  static int timeOfLastMovement; // time in ms where movement was last detected
  static const float CalibrationFactor = SamplingTime/float(1000) * GyroRate * 100.0 / MaxDegrees / float(0x7FFF);
  short gyroRaw[3];           // Raw values {X, Y, Z} read directly from gyro sensor
  static float gyroBias[3];   // Offset Errors {X, Y, Z}
  float diffRotation[3];      // Array of relative angles in Degrees {X, Y, Z}
  static float absoluteRotation[3] = {0, 0, 0};
                              // Return Value: Array of cumulative angles in Degrees {X, Y, Z}
  
  // Get raw Values from Sensor
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43);  // starting with register 0x43 (GYRO_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_addr, (size_t)6, true);  // request a total of 6 registers
  gyroRaw[0]=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gyroRaw[1]=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gyroRaw[2]=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  int totalMovement = abs(gyroRaw[0]) + abs(gyroRaw[1]) + abs(gyroRaw[2]);

  // Movement?
  if(totalMovement < MovementDetectionThreshold)
    Serial.println("No movement detected: totalMovement=" + String(totalMovement));
  else
    timeOfLastMovement = millis();
    
  // Calibration?
  if(millis()-timeOfLastMovement > MillisecondsToCalibrate) {
    // Do a simple Low-Pass Filtering to get the biases
    Serial.print("CALIBRATION! ");
    digitalWrite(ledCalibrate, true);
    for(int i=0; i<3; i++)
      gyroBias[i] = (1.0-CalibrationFilterCoeff)*gyroBias[i] + CalibrationFilterCoeff*gyroRaw[i];      
  } else {
    digitalWrite(ledCalibrate, false);

    // Un-bias raw values and integrate the angular speed to get the angle
    for(int i=0; i<3; i++) {
      diffRotation[i] = (gyroRaw[i] - gyroBias[i]) * CalibrationFactor;
      absoluteRotation[i] = constrain(absoluteRotation[i] + diffRotation[i], -100, 100);
    }
  }

  return absoluteRotation;
}
