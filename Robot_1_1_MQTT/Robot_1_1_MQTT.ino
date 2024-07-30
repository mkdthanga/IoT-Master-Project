//**** INCLUDES *************************************************************
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//**** PIN ASSIGNMENT OF OUR SHIELD AND ADDRESSES ***************************
static const int Servos[] = { 0, D0, D3, D5, D6, D7 };                   // { Rotation, InnerArm, OuterArm, Hand, Gripper }

//**** PROTOTYPES ***********************************************************
void robot_initialize();                       
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

//**** GLOBALS **************************************************************
static const IPAddress ServerIpAddr = {192,168,10,1};                 // IP address of the MQTT Server/Broker
static bool ledOn;                                                    // Blinking/Flickering blue LED shows prog state!
WiFiClient WIFIclient;                                                // Use WiFiClient class to create TCP connections
PubSubClient MQTTclient(ServerIpAddr, 1883, callback, WIFIclient);    // Initialize the PubSubClient library


//**** SETUP ****************************************************************
void setup() {
  #ifdef DEBUG 
  Serial.begin(9600);
  #endif DEBUG
  pinMode(LED_BUILTIN, OUTPUT);     // Pin operation mode

  comm_wifiSetup();                 // Sets up the WiFi Access Point
  robot_initialize();               // Setup Servos
}

//**** LOOP ****************************************************************
void loop() {

  if (!MQTTclient.connected()) 
    {
      reconnect();
    }
  MQTTclient.loop();
}
