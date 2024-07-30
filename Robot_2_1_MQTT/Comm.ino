//####### THIS FILE CONTAINS COMMUNICATION-RELATED DATA AND FUNCTIONS #######


//**** CONSTANTS ************************************************************
static const char* AccessPointSsid = "iotlab";
static const char* AccessPointPassword = "masterproj";
static const unsigned short ServerPort = 1883;

//**** PROTOTYPES ************************************************************
void robot_controlServos(float *rot);


//**** 'comm_wifiSetup()' sets up the WiFi Access *********************
void comm_wifiSetup() {
   /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(AccessPointSsid, AccessPointPassword);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    #ifdef DEBUG
    Serial.print(".");
    #endif DEBUG
  }
  #ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  #endif DEBUG
    
  MQTTclient.setCallback(callback);

}
   

void callback(char* topic, byte* payload, unsigned int len) 
{
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  static bool GyroId=0;
  static byte rotations[6] = {0, 0, 0, 0, 0, 0};
                              // The rotation of 2 x 3 axis, range +/-100%
                              // { X0, Y0, Z0, X1, Y1, Z1}
  signed char * a;
  float translation [6] = {0,0,0,0,0,0};
  
  #ifdef DEBUG
  Serial.print(len);
  Serial.println();
  #endif
  
  GyroId = payload[0];
  a = (signed char*)&rotations[0];
  for (int i=1;i<len;i++)
    rotations[GyroId*3+i-1] = (payload[i]-100);

  #ifdef DEBUG
  Serial.println();
  #endif 
  
  for(int i=0;i<6;i++)
  {
  translation[i] = (float)a[i];
  }
  
  robot_controlServos(translation);
  #ifdef DEBUG
  Serial.println();
  Serial.println();
  #endif
  
  // Map 6 gyro axis to 5 Robot axis!
  digitalWrite(LED_BUILTIN, ledOn = !ledOn);  // Blue LED Flickers

}


void reconnect() {
  // Loop until we're reconnected
  while (!MQTTclient.connected()) 
  {
    #ifdef DEBUG
    Serial.print("Attempting MQTT connection...");
    #endif
    // Attempt to connect
    if (MQTTclient.connect("arduinoClient")) 
    {
      #ifdef DEBUG
      Serial.println("connected");
      #endif
      MQTTclient.subscribe("/lab/robot2/axgrp1");
      MQTTclient.subscribe("/lab/robot2/axgrp2");
    } 
    else 
    {
      #ifdef DEBUG
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" try again in 5 seconds");
      #endif
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



