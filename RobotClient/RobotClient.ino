//**** CONSTANTS ************************************************************
static const int SamplingTime = 15;  // SamplingTime in ms

//**** GLOBAL VARIABLES *****************************************************
static bool clientId;      // User can choose a second Board-Identity,
                           // by holding down 'ButtonLeft' during reset!
static bool ledOn;         // Blinking/Flickering blue LED shows prog state!
  
//**** PIN ASSIGNMENT OF OUR SHIELD AND ADDRESSES ***************************
static const int ButtonLeft = D7;
static const int ButtonRight = D6;
static const int LedBlue = D5;
static const int LedRed = D3;
static const int Ldr = A0;
static const int Buzzer = D0;

//**** SETUP ****************************************************************
void setup(){
  Serial.begin(9600); // Configure Serial Port

  // Pin operation mode
  pinMode(ButtonLeft, INPUT_PULLUP);  // Function: Reset all angles to center
  pinMode(ButtonRight, INPUT_PULLUP); // Function: Movement modifies angles
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LedBlue, OUTPUT);
  pinMode(LedRed, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  // Determine Client ID of this Board (there are two different IDs)
  clientId = !digitalRead(ButtonLeft);// Hold during reset for 2nd identity
  digitalWrite(LedBlue, clientId);    // Blue LED indicates 2nd Board
  Serial.println("\nBoard-ID: " + String(clientId?"2nd Board":"1st Board"));

  comm_wifiConnect();                 // Set up WiFi connection
  gyro_initialize(LedRed);            // Setup Gyros
  
  // Indicate Ready state acoustically
  if(clientId) {
    tone(Buzzer, 1200, 60);
    delay(100);
    tone(Buzzer, 900, 60);
  } else {
    tone(Buzzer, 900, 60);
    delay(100);
    tone(Buzzer, 1200, 60);
  }
}

//**** LOOP ****************************************************************
void loop(){
  float* rotations;                   // Absolute Angles in Degrees

  rotations = gyro_getRotation();
  comm_sendData(rotations);

  digitalWrite(LED_BUILTIN, ledOn = !ledOn);  // Blue LED Flickers
  delay(SamplingTime-1);  // Assume 1ms of fixed calculation time per round
}
