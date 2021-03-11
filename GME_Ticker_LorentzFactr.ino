/*
 * LorentzFactr's hyper advanced GME stock ticker that completely bites the Adafruit
 * scrolling text example. It requires a serial port connection and the GME_ticker.py script to display
 * real-time stock data for GME or whatever else you want to put on there. You'll
 * likely have to update the COM PORT in the Python script to the one your Arduino is
 * connected to but that's it.
 * 
 * For use with a 16x16 LED Matrix, controlled by a ESP8266 NodeMCU (though,
 * the board type probably doesn't matter as long as your LED Pin is matched
 * the #DEFINE PIN in line 29.
 * 
 * My matrix uses WS2812 LED's starting with LED0 at bottom left. 
 * Alternating pattern (zigzag pattern) 16 columns wide.
 * Your shit might be different. Use at your own risk ¯\_(ツ)_/¯ 
 * 
 * Disclaimer: I'm a smoothed brained ape 'coder'. It might break. It might not.
 * 
 * Ask for help here:
 * https://discord.gg/PYwjqWxMM3
 * 
 * twitch.tv/lorentzfactr 
 * 
 */
//----------------------------------------------------------------------------------



#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 2                         //Your pin might be different...
 
// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
 
//Your Matrix definition might be different. See above instructions. 
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, PIN,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);



//some variable shit
float c;
float last_c = 0.00;
String s; 
String yay = "Stonk go brrrr";
String nay = "Buy tendie dip";
String txt ;
const uint16_t RED = matrix.Color(255, 0, 0);
const uint16_t GREEN = matrix.Color(0, 255, 0);

//set that shit up 
void setup() 
{
  
  Serial.begin(9600);
  Serial.flush();                   //probably doesn't do shit cuz Arduino
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(255);
  matrix.setTextColor(GREEN);
}
 
int x = matrix.width();             //i dunno prolly coulda defined this at the top.
 
void loop() 
{
  if(Serial.available()>0)          // Check for availablity of data at Serial Port
  {                                 // and do math stuff before trying to call matrix
    s = Serial.readStringUntil('$');
    c = Serial.parseFloat();
  
    if (c <= last_c){               //compare current close price to last close price
      matrix.setTextColor(GREEN);   //AKA hyper advanced algorithm to tell you to either buy or HODL
      txt = yay;
    }
    else{
      matrix.setTextColor(RED);
      txt = nay;
    }
  }
  matrix.fillScreen(0);             //draw that shit on the screen
  matrix.setCursor(x, 0);           //top line displays ticker info
  matrix.setTextSize(1);
  matrix.print(s + '$'+ c);
  matrix.setCursor(x, 8);           //bottom line tells you to buy or HODL,  we dont know how to sell
  matrix.print(txt);
  if(--x < -150) {
    x = matrix.width();
  }
  matrix.show();
  last_c = c;
  Serial.flush();                   //probably doesn't do shit cuz Arduino
  delay(40);
}
