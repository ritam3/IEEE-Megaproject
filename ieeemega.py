# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import serial
import time
import matplotlib.pyplot as plt
import numpy

ser = 0

def init_serial():
    COMNUM = "COM4"      
    global ser          
    ser = serial.Serial('COM4', 115200, timeout=0)
    if ser.isOpen():
        print 'Open: ' + ser.portstr
import turtle
turtle = turtle.Turtle()
size = 500
turtle.speed(8)
turtle.pensize(5)
def drawPoints(dist1, pos):
    turtle.goto(0,0)
    turtle.setheading(pos)
    turtle.pendown()
    turtle.forward(dist1)
    turtle.penup()
    turtle.backward(dist1)

def resetBackgroud(pos):
    if pos <= 2:
        turtle.clear() 
    if pos>=178:
        turtle.clear()
        
def drawBackground():
    turtle.goto(0,0)
    turtle.penup()
    turtle.sety(turtle.ycor() - size)
    turtle.pendown()
    turtle.begin_fill()
    turtle.circle(size)
    turtle.end_fill()
    turtle.sety(turtle.ycor() + size)
    
turtle.pencolor("#ff0000")        

#Call the Serial Initilization Function, Main Program Starts from here
init_serial()

while 1: 
    if(ser.inWaiting()>0):
      bytes = ser.readline()  
      data = bytes.split()
      if(len(data)==2):
         th = int(data[0])
         r = int(data[1])
         resetBackgroud(th)
         drawPoints(r,th)
       
    
