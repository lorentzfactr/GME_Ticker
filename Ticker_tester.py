#I only use this code after trading is closed
#to test new arduino code. Otherwise, I'll typically
#test with the live market data (GME_ticker_Lorentzfactr.py)

#import some stuff
import time
from serial import Serial
from numpy import random

#Choose how you want to display the info:
JustArrow = False           #Just show me green up or red down arrow. I don't want to know the price.
TickerPrice = False          #Show me the fuckin money!

#Help! I only have an WS2812 LED strip. What mode is for me?
#Set both JustArrow & TickerPrice to False and only colors will be displayed.
#That's it for Python...you don't really need to touch anything else in here.
#After you've uploaded Arduino firmware just run this script. If it's not displaying
#within a few seconds, kill the script and run it again. Sometimes Serial is funky.


#Data Source
#Ok I lied... In theory, you could add more stonks for your ticker
#in the StockShares list. But the colors/arrows won't work properly.
#If you REALLLY want it, hit up my Discord and if there's enough want for it, I'll add it.
#Alternatively, feel free to fork my Github and implement it yourself!

StockShares = ['GME']

#global def for saving the last quote
last_s = 0.0

#you prolly need to update your COM PORT
ser = Serial('COM11',115200)  

#checks which mode you selected. If in invalid default to ticker mode.
#Previously hardcoded in Arduino, but why rebuild firmware everytime you want to see it differently?
def modeLogic():
    global JustArrow
    global TickerPrice
    
    if JustArrow == True and TickerPrice == False:
        return 1
    if JustArrow == False and TickerPrice == True:
        return 2
    if JustArrow == False and TickerPrice == False:
        return 3
    else:
        return 2

#Checks the change in current price vs last price
#Previously done in Arduino, but why take up memory on there when we have tons here?
def change(delta):
    if delta >= 0:
        return 1
    else:
        return 0

#just builds the string data, encodes and sends it over serial. This is Python 3 so things are different.
def printShare(share,name,delta):
    
    diff = change(delta)
    mode = modeLogic()
    buildStr = "<" + str(mode) + "," + str(diff) + "," + str(name) + ": $," + str(share) + ">"
    ser.write(buildStr.encode())
    time.sleep(.1)


#Generates random pricing between $0-$100, make it $10000 to simulate real thing.
def get_current_price(): 
    x = random.rand()
    x = round(x*100,2)
    return x


#Main just lives a life of looping every 9.5 seconds..."What is my purpose?" "You pass the ̶b̶u̶t̶t̶e̶r̶  tendies."
def mainProgram():
    global last_s
    for i in StockShares:
        name = i
        s = get_current_price()
        delta = s - last_s
        last_s = s
        printShare(s,name, delta)
        time.sleep(10)
    mainProgram()
mainProgram()