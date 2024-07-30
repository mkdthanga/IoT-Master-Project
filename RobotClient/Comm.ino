//####### THIS FILE CONTAINS COMMUNICATION-RELATED DATA AND FUNCTIONS #######
//**** INCLUDES *************************************************************
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


//**** CONSTANTS ************************************************************
//#define LAPTOP_VIA_HOMEACCESSPOINT
#ifdef LAPTOP_VIA_HOMEACCESSPOINT
static const char* AccessPointSsid = "-Fill-Ssid-Here-";
static const char* AccessPointPassword = "-Fill-Password-Here-";
static const char* ServerHost = "192.168.178.38";
#else
static const char* AccessPointSsid = "Robot";
static const char* AccessPointPassword = "CommSysLab";
static const char* ServerHost = "192.168.0.1";
#endif

static const unsigned short ServerPort = 40000;
static const unsigned short LocalPort = 12345;
static const int MsgLength = 4; // 1(ID) + 3 x 1 (Angle)

//**** GLOBAL VARIABLES *****************************************************
WiFiUDP udp;               // UDP instance
static byte packetSendBuffer[MsgLength];

//**** 'comm_wifiConnect()' sets up the WiFi connection *********************
void comm_wifiConnect() {
  // Setup Wifi
  Serial.print("Connecting WLAN to AP \"" + String(AccessPointSsid) + "\"");
  WiFi.mode(WIFI_STA);
  WiFi.setOutputPower(20);
  WiFi.begin(AccessPointSsid, AccessPointPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, ledOn = !ledOn); // Blue LED blinks slowly
  }
  Serial.println(" Done!");

  // Create UDP instance
  udp.begin(12345);
}

//**** 'comm_sendData()' sends the angle array to the robot *****************
void comm_sendData(float *rotations) {
  udp.beginPacket(ServerHost, ServerPort);

  packetSendBuffer[0] = clientId;
  for(int i=0; i<3; i++)
    packetSendBuffer[i+1] = byte(rotations[i]+100);
  udp.write(packetSendBuffer, MsgLength);
  udp.endPacket();
}
