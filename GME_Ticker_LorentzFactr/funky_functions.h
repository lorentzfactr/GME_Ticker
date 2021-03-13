//This is a high-speed string parser cuz Arduino built serial read functions are trash.
//This is high-speed, zingin, winnin, bang another doot fast life ticker action people. NO FUCKIN TIME TO WASTE!
//But really matrix draw commands are annoying when working with Serial, so we're left with this fuckin beast.
//Behold, a string parser that I copied, pasted and then spent hours trying to implement. Again, ape brain 'coder'.


const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

                                  // variables to hold the parsed data
int mode_ = 0;
int delta = 0;
char stockName[numChars] = {0};
char quote [numChars] = {0};

boolean newData = false;

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while(Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        //Serial.println("Arduino Saw: ");
        //Serial.println(rc);
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}


void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    mode_ = atoi(strtokIndx);               // copy it to mode_

    strtokIndx = strtok(NULL,",");           // get the first part - the string
    delta = atoi(strtokIndx);               // copy it to mode_

    strtokIndx = strtok(NULL,",");           // get the first part - the string
    strcpy(stockName, strtokIndx);           // copy it to mode_

    strtokIndx = strtok(NULL, ",");
    strcpy(quote, strtokIndx);               // convert this part to a float

    strcat(stockName,quote);
}

//this is just for testing, doesn't ever get called in the real thing.
void showParsedData() {
    Serial.print("Mode: ");
    Serial.println(mode_);
    Serial.print("Change: ");
    Serial.println(delta);
    Serial.print("Stock Name: ");
    Serial.println(stockName);
    Serial.print("Price: ");
    Serial.println(quote);
}
