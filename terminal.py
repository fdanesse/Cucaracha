#!/usr/bin/python3
# -*- coding: utf-8 -*-

#import os
import serial
#import sys

arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=0.1)
arduino.flushInput()

while True:
    arduino.write(raw_input("Comando: "))
    arduino.flush()
    print arduino.readline().strip().replace("\n", "")
