import paho.mqtt.client as mqtt
import time

# This is the Subscriber

def on_connect(client, userdata, flags, rc):
  print("Connected with result code "+str(rc))
  client.subscribe("/lab/temp")

def on_message(client, userdata, msg):
    print(msg.payload)
    

client = mqtt.Client()
client.connect("141.60.191.108",1883)

client.username_pw_set("thangz","hello")
client.on_connect = on_connect
client.on_message = on_message

client.loop_start()

raw_input("Press enter to exit...\n\n")
client.disconnect()



