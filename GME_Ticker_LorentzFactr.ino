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
 * My matrix uses WS2812 LED's starting with LED0 at bottom left. For some reason with
 * this library, text displays correctly but the drawing commands are upside down. Not sure why but that forced 
 * me to draw everything upside down (so down arrows are up and up arrows are down)...Hey! It's sort of like the
 * real stock market! So, if you're trying to draw your own custom shapes instead of my arrows, take that into 
 * consideration. Last, my matrix is an alternating pattern (zigzag) 16 columns wide.
 * Your shit might be different ¯\_(ツ)_/¯ 
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


bool JustArrow   = false;         //Set JustColors = true and TickerPrice = false, if you dont want to see the price or if you only have LED strips.
bool TickerPrice  = false;        //Set JustColors = false and TickerPrice = True, to enable hyper advanced Ticker mode
                                  //Set both = false or both = true: defaults to ticker mode.
                                  //(EVENTUALLY) Set both = false or both = true: to just fill with color (pro tip: do this if you only have WS2812 LED strips AKA no matrix)
                                  //Will work with a 16 x 16 LED Matrix made of WS2812 LEDs. 
                                  //How you define the Matrix matters, see above for details. 
                                  //If you set both to true or both to false it will just revert to ticker.



//some variable shit
float c;
float last_c = 0.00;
String s; 
String yay = "Stonk go brrrr";
String nay = "Buy tendie dip";
String txt ;
uint16_t color = 0;
const uint16_t red = matrix.Color(255, 0, 0);
const uint16_t green = matrix.Color(0, 255, 0);
uint16_t black = matrix.Color(0, 0, 0);


//Updoot arrow :)
//Remember: the draw commands are upside down.
//Think of it like listening to CNBC. If they say buy, you sell. If they say sell, you buy.

void drawUpDoot(){
    matrix.drawLine(7,13,7,7,color);
    matrix.drawLine(8,13,8,7,color);
    matrix.drawLine(4,6,11,6,color);
    matrix.drawLine(5,5,10,5,color);
    matrix.drawLine(6,4,9,4,color);
    matrix.drawLine(7,3,8,3,color);
    //matrix.show();
    //delay(25);
}

//Downdoot arrow :(
void drawDownDoot(){
    matrix.drawLine(7,2,7,8,color);
    matrix.drawLine(8,2,8,8,color);
    matrix.drawLine(4,9,11,9,color);
    matrix.drawLine(5,10,10,10,color);
    matrix.drawLine(6,11,9,11,color);
    matrix.drawLine(7,12,8,12,color);
    //matrix.show();
    //delay(25);
}




//set that shit up 
void setup() 
{
  
  Serial.begin(115200);
  Serial.flush();                                     //probably doesn't do shit cuz Arduino
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(255);
  matrix.setTextColor(green);
}
 
int x = matrix.width();                               //i dunno prolly coulda defined this at the top.


 
void loop() 
{
  if(Serial.available()>0)                            // Check for availablity of data at Serial Port
  {                                                   
    s = Serial.readStringUntil('$');
    c = Serial.parseFloat(); 
  
    if (c >= last_c){                                 //compare current close price to last close price
      matrix.fillScreen(0);
      matrix.setTextColor(green);                     //AKA hyper advanced algorithm to tell you to either buy or HODL
      txt = yay;
      color = green;
      drawUpDoot();                                   
    }
    else{
      matrix.fillScreen(0);
      matrix.setTextColor(red);
      txt = nay;
      color = red;
      drawDownDoot();
    }
  }
  
  //Only executes in TickerPrice mode
  
  if (TickerPrice == true && JustArrow == false){
    matrix.fillScreen(0);                             //draw scrolling ticker of GME and price.
    matrix.setCursor(x, 0);                           //top line displays ticker info
    matrix.setTextSize(1);
    matrix.print(s + '$'+ c);
    matrix.setCursor(x, 8);                           //bottom line tells you to buy or HODL,  I dont know how to sell
    matrix.print(txt);
    if(--x < -150) {
      x = matrix.width();
    }  
  }
  
  //Only executes in JustArrow mode.
  
  if (TickerPrice == false && JustArrow == true){    //draw the up or down arrows only.
    matrix.drawRect(0, 0, 16, 16,color);
    matrix.show();
  }

  //Only executes when both modes are off (or both on as a fail safe). 
  
  if (TickerPrice == false && JustArrow == false || TickerPrice == true && JustArrow == true){
    TickerPrice = true;
    JustArrow = false;
  }
  matrix.show();
  last_c = c;
  Serial.flush();                                   //probably doesn't do shit cuz Arduino
  delay(40);
}
