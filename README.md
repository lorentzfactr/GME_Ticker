# GME_Ticker
A smooth brained ticker for GME ape's.

Overview:
Pulls real-time GME quotes from yfinance using a python script.
Sends that quote over Serial, Arduino listens for the Serial string and 
stores the ticker as a string and the quote as a float. Arduino compares
the last quote from the current quote and decides if text is green and 
"Stonks go brrrr" or if text is red and you must "Buy tendie dip".

Arduino and Matrix:
It requires a serial port connection and the GME_ticker.py script to display
real-time stock data for GME or whatever else you want to put on there. You'll
likely have to update the COM PORT in the Python script to the one your Arduino is
connected to but that's it.

For use with a 16x16 LED Matrix, controlled by a ESP8266 NodeMCU (though,
the board type probably doesn't matter as long as your LED Pin is matched
the #DEFINE PIN in line 29.

My matrix uses WS2812 LED's starting with LED0 at bottom left. 
Alternating pattern (zigzag pattern) 16 columns wide.
Your shit might be different. Use at your own risk ¯\_(ツ)_/¯ 

Disclaimer: I'm a smoothed brained ape 'coder'. It might break. It might not. 
LMK if it you need help here:
https://discord.gg/PYwjqWxMM3


twitch.tv/lorentzfactr
