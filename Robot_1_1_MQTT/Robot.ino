//########## THIS FILE CONTAINS ROBOT-RELATED DATA AND FUNCTIONS #############
//**** INCLUDES *************************************************************
#include <Servo.h> 

//**** CONSTANTS ************************************************************
static const int ServoLimits[][2] = {  // Hard Mechanical Servo Limits - in us
  { 700, 1000}, // Dummy Value
  { 650, 2300}, // Azimuth  {right, left}
  { 900, 1800}, // InnerArm {out, in}
  {1060, 1780}, // OuterArm {up, down}
  { 650, 2350}, // Hand     {left, right}
  {1400, 2030}  // Gripper  {close, open}
};  // CAUTION: Changing these limits might result in driving the servo against
    // their mechanical limit with high currents, leading to Thermal Destruction!

//**** GLOBALS **************************************************************
Servo myServos[6];

//**** 'robot_initialize()' prepares the Servos for being used **************
void robot_initialize() {
  // Attach Servos
  for(int i=1; i<6; i++)
    myServos[i].attach(Servos[i], ServoLimits[i][0], ServoLimits[i][1]);

  // Put Servos to their Default rotations
  float defaults[] = {0,0,0,0,0,0};
  robot_controlServos(defaults);
}

//**** 'robot_controlServos()' maps the rotation +/-100% to the servo positions ***
void robot_controlServos(float *rot) {
  
  float servoPos[6];      // { Azimuth, InnerArm, OuterArm, Hand, Gripper }

  //  If servo gets detached (error with azimuth axis) attach again - but this doesn´t work
  //  for (int i = 0; i < 5; i++)
  //  {
  //    if(myServos[i].attached()==0)
  //      robot_initialize();
  //  }
  
  //    Just for data check before we calculate and write out the values
  //    for(int i=0;i<6;i++)
  //    {
  //       Serial.print(rot[i]);
  //       Serial.print("   ");
  //    }

  // MAIN PART OF THIS MODULE: Coordinate Transformation - Map 6 Axis -> 5 Servos, range +/-100%
  //Serial.println(servoPos[2] = 1.00*rot[2]);    // FOR ADJUSTMENT! COMMENT NEXT 3 LINES OUT!
  servoPos[1] = 0.8*rot[2];       // Azimuth
  servoPos[2] = -0.90*rot[0] - 0.65*rot[3];  // InnerArm
  servoPos[3] = -0.80*rot[0] + 0.65*rot[3];  // OuterArm
  servoPos[4] = -1.0*rot[5];      // Hand
  servoPos[5] = rot[4];           // Gripper
  // Note: servoPos[5] does not exist; rot[1] is not used.
  //Serial.print(servoPos[0]);
  //Serial.println();

  // Control Servos
  for(int axis=1; axis<6; axis++) {
      
    float pulseWidth = (servoPos[axis]/200+0.5)*(ServoLimits[axis][1]-ServoLimits[axis][0]) + ServoLimits[axis][0];
    myServos[axis].write(constrain(pulseWidth, ServoLimits[axis][0], ServoLimits[axis][1]));
    // I tried to reattach the servos here, but this doesn´t work either
    //if(myServos[0].attached()==0)
    //{
    //  myServos[0].attach(Servos[0], ServoLimits[0][0], ServoLimits[0][1]);
    //  Serial.print("Att");
    //}
  }

}
