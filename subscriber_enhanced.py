import paho.mqtt.client as mqtt
import sys
# This is the Subscriber
broker_address = "141.60.191.41"

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
        client.subscribe("/lab/temp")
        client.subscribe("/lab/press")
        client.subscribe("/lab/hum")

#this function is executed everytime a message is received for the subscribed topics
def on_message(client, userdata, msg):
    print(msg.payload)

#authentication via password and username
client.username_pw_set("thangz","hello")

#set last will message
client.will_set('/lab/status','Last will message', 1, True)

#assign on_connect to client.on_connect to fulfill the meachnism of the paho library
client.on_connect = on_connect

#assign on_message to client.on_message to fulfill the meachnism of the paho library
client.on_message = on_message

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

#wait for a keyboard action to exit the program
raw_input("Press enter to exit...\n\n")
client.disconnect()


