#!/usr/bin/env python

import sys
import serial
import pyautogui

#serial input consists of lines containing two characters: name, action
# letter can be the name of an entry in the keymap
#  or, if it's not present, the name of the key itself
# action is "p" for press, or anything else to release

keymap = { 'b' : 'backspace' }

if len(sys.argv)<2:
  print("usage: {} ttydev".format(sys.argv[0]))
  quit()
dev = sys.argv[1]

ser = serial.Serial(dev, 9600)
ser.flushInput()
ser.flushOutput()

while True:
  data_raw = ser.readline()
  string_raw = data_raw.decode('utf-8')
  name = string_raw[0]
  if name in keymap:
    key = keymap[name]
  else:
    key = name

  if (string_raw[1] == 'p'):
    pyautogui.keyDown(key)
  else:
    pyautogui.keyUp(key)
