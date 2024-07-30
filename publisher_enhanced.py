#Import required libraries
from Adafruit_BME280 import *
import paho.mqtt.client as mqtt
import time
import sys

#define the mqtt broker adress (destination)
broker_address = "141.60.191.41"

#create an interface for the BME280 sensor
sensor = BME280(t_mode=BME280_OSAMPLE_8, p_mode=BME280_OSAMPLE_8, h_mode=BME280_OSAMPLE_8)

#create a client with the paho mqtt library
client = mqtt.Client()

#this function is executed everytime the client tries to connect to the MQTT broker
def on_connect(client, userdata, flags, rc):
    if rc > 0:
        print("\nConnection failed:\n\n")
        print("Check user and password configuration")
        print("Check MQTT broker settings")
    else:
        print("\nConnected successfully\n")
        print("Publishing data\n")

#authentication via password and username
client.username_pw_set("thangz","hello")

#set last will message
client.will_set('/lab/status', 'Last will', 1, True)

#assign on_connect to client.on_connect to fulfill the meachnism of the paho library
client.on_connect = on_connect

#start mqtt client loop enables the client to use its loopback function / otherwise there is no response from the library on_xxx functions
client.loop_start()

#try to connect the client to the mqtt broker
try:
    client.connect(broker_address,1883)
#if the connection is not successful, print info and end the program    
except Exception: 
    print("\nConnection failed:\n\n")
    print("Check network connection")
    print("Check MQTT broker (IP-Address)\n")
    sys.exit()
    
    
while True:
    #send message every ten seconds
    time.sleep(10)
    
    #read in and calculate all values
    degrees = sensor.read_temperature()
    pascals = sensor.read_pressure()
    hectopascals = pascals / 100
    humidity = sensor.read_humidity()
    
    #publish the messages in different messages
    client.publish("/lab/hum", humidity, 0, 1)
    client.publish("/lab/temp", degrees, 0, 1)
    client.publish("/lab/press", hectopascals, 0, 1)