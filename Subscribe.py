import paho.mqtt.client as mqtt

# This is the Subscriber

def on_connect(client, userdata, flags, rc):
  print("Connected with result code "+str(rc))
  client.subscribe("/lab/temp")
  client.subscribe("/lab/hum")
  client.subscribe("/lab/press")

def on_message(client, userdata, msg):
    print(msg.payload.decode())
    client.disconnect()
    
client = mqtt.Client()
client.connect("141.60.191.108",1883,60)

client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()