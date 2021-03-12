#I only use this code after trading is closed
#to test new arduino code. Otherwise, I'll typically
#test with the live market data (GME_ticker_Lorentzfactr.py)

#import some stuff
import time
from serial import Serial
from numpy import random

#Data Source
StockShares = ['GME']

#you prolly need to update your COM PORT
ser = Serial('COM11',115200)  

#just builds the string data, encodes and sends it over serial. This is Python 3 so things are different.
def printShare(share,name):
    buildStr = str(name) + ": $" + str(share)
    ser.write(buildStr.encode())
    print(name + ': $' + str(share))
    time.sleep(.1)


#Generates random pricing between $0-$100, make it $10000 to simulate real thing.
def get_current_price(): 
    x = random.rand()
    x = round(x*100,2)
    return x


#Main just lives a life of looping every 9.5 seconds..."What is my purpose?" "You pass the ̶b̶u̶t̶t̶e̶r̶  tendies."
def mainProgram():
    for i in StockShares:
        name = i
        s = get_current_price()
        printShare(s,name)
        time.sleep(10)
    mainProgram()
mainProgram()