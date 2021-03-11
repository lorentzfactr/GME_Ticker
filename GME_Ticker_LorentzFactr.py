#import some stuff
import time
from serial import Serial

#Data Source
import yfinance as yf

#You can add more tickers if you want. But the hyper advanced Arduino algorithm might need to be fixed.
#Give it a shot and lmk
StockShares = ['GME']

#you prolly need to update your COM PORT
ser = Serial('COM10',9600)  

#just builds the string data, encodes and sends it over serial. This is Python 3 so things are different.
def printShare(share,name):
    buildStr = str(name) + ": $" + str(share)
    ser.write(buildStr.encode())
    print(name + ': $' + str(share))
    time.sleep(.1)

#hits up yahoo and asks for dat pricing
def get_current_price(symbol):
    
    ticker_yahoo = yf.Ticker(symbol)
    data = ticker_yahoo.history(rounding=True)
    last_quote = (data.tail(1)['Close'].iloc[0])
    return last_quote


#Main just lives a life of looping every 9.5 seconds...
def mainProgram():
    for i in StockShares:
        name = i
        s = get_current_price(i)
        printShare(s,name)
        #s.refresh()
        time.sleep(9.5)
    mainProgram()
mainProgram()