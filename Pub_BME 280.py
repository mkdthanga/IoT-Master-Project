#!/usr/bin/env python
# -*- coding: utf-8 -*-from BME280 import *
from Adafruit_BME280 import *
import paho.mqtt.client as mqtt
import time

broker_address = "141.60.191.108"

sensor = BME280(t_mode=BME280_OSAMPLE_8, p_mode=BME280_OSAMPLE_8, h_mode=BME280_OSAMPLE_8)


#print 'Temp      = {0:0.3f} deg C'.format(degrees)
#print 'Pressure  = {0:0.2f} hPa'.format(hectopascals)
#print 'Humidity  = {0:0.2f} %'.format(humidity)


def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

client = mqtt.Client()
client.username_pw_set("thangz","hello")

client.on_connect = on_connect
client.connect(broker_address,1883,60)

client.loop_start()

while True:
    time.sleep(2)
    degrees = sensor.read_temperature()
    pascals = sensor.read_pressure()
    hectopascals = pascals / 100
    humidity = sensor.read_humidity()
    client.publish("/lab/hum", humidity, 1, 1)
    client.publish("/lab/temp", degrees, 1, 1)
    client.publish("/lab/press", hectopascals, 1, 1)
