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
#include "funky_functions.h"


#define PIN 2                         //Yours pin might be different...
 
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

uint16_t color = 0;
const uint16_t red = matrix.Color(150, 0, 0);
const uint16_t green = matrix.Color(0, 150, 0);
uint16_t black = matrix.Color(0, 0, 0);
String fromPython;
String yay = "Stonk go brrrr";
String nay = "Buy tendie dip";
String txt ;



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
 
int x = matrix.width();                               


 
void loop() 
{
    recvWithStartEndMarkers();                        //See if there's a new string, then get it.
    if (newData == true) {                            //If there's a new string, parse it.
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        //showParsedData();
        newData = false;
    }

 
    if (mode_ == 1){                                        //executes JustArrow mode
          //Serial.println("Delta1: ");
          //Serial.println(delta);
          if (delta == 1){                                  //compare current close price to last close price
            matrix.fillScreen(0);
            color = green;
            matrix.drawRect(0, 0, 16, 16,color);          
            drawUpDoot();                       
                        
          }
          if (delta == 0){
            matrix.fillScreen(0);
            color = red;
            matrix.drawRect(0, 0, 16, 16,color); 
            drawDownDoot();
          }
          else{}
          
    }
    
    if (mode_ == 2){                                    //executes Ticker mode
      if (delta == 1){                                  //compare current close price to last close price
        color = green;
        txt = yay; 
      }
      if (delta == 0){
        color = red;
        txt = nay;
      }
        matrix.fillScreen(0);                             //draw scrolling ticker of GME and price.
        matrix.setCursor(x, 0);                           //top line displays ticker info
        matrix.setTextSize(1);
        matrix.setTextColor(color);
        matrix.print(stockName);
        matrix.setCursor(x, 8);                           //bottom line tells you to buy or HODL,  I dont know how to sell
        matrix.print(txt);
        if(--x < -150) {
          x = matrix.width();
        }                      
    }
    
    if (mode_ == 3){                                    //executes Color mode AKA "I only have LED Strips"
      matrix.fillScreen(0);
      if (delta == 1){                                 //compare current close price to last close price    
            color = green;                                       
          }
          if (delta == 0){
            color = red;                         
          }
          else{} 
            uint16_t x_0 = 0;
            
            for (int i = 0 ; i<=17 ; i++){
                matrix.drawLine(x_0,0,x_0,16,color); 
                x_0++;
                matrix.show();
                delay(50);
            }
    }
  
  matrix.show();
  delay(40);
  
}
