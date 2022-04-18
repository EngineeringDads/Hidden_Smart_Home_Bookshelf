// Adafruit IO IFTTT Example - Gmailbox
// Tutorial Link: https://learn.adafruit.com/gmailbox
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Brent Rubell for Adafruit Industries
// Copyright (c) 2018 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// Pin Declaration used to control the Relay
#define Open_Relay 12
#define Close_Relay 4
#define LED_Relay 5

// Set standard status of box to closed, assumed its natural state if power is interuppted 
int closed = 1;

// set up the 'adafruit Io' feed
AdafruitIO_Feed *gmail_feed = io.feed("Name of Io Feed");

void setup() {

  // Sets up the Relay Pins
  pinMode(Open_Relay,OUTPUT);
  pinMode(Close_Relay,OUTPUT);
  pinMode(LED_Relay,OUTPUT);

  //Ensures Door is fully closed if power is cut. If door is open and power is closed can flip power a few times to shut the door
  digitalWrite(Close_Relay,HIGH);
  delay(5000);
  digitalWrite(Close_Relay,LOW);
  delay(500);

  //Quick Open and close to signify that the initial door procedure is complete
  digitalWrite(Open_Relay,HIGH);
  delay(500);
  digitalWrite(Open_Relay,LOW);
  digitalWrite(Close_Relay,HIGH);
  delay(1000);
  digitalWrite(Close_Relay,LOW);

  
  // start the serial connection
  Serial.begin(115200);
  

  // wait for serial monitor to open
  //while(! Serial);

  
  Serial.println("IFTTT Open/Close");

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'servo' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
 gmail_feed->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  gmail_feed->get();



}



void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

}

// this function is called whenever a 'gmail' message
// is received from Adafruit IO. it was attached to
// the gmail feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  
  Serial.println("Order Received");
  Serial.println(data->toPinLevel());


  // If statement to open linear actuator, delays is calibrated for full length of opening
  // Please ensure delay is set approriately to avoid overextension of linear actuator
  // LED light turns on as the door is almost fully open
  if(data->toPinLevel() == HIGH && closed == 1){
     digitalWrite(Open_Relay,HIGH);
     delay(20000);
     digitalWrite(LED_Relay,HIGH);
     delay(4000);
     digitalWrite(Open_Relay,LOW);
     Serial.println("Opening Bookshelf");
     closed = 0;


   // If Statement to close linear actuator
   // Delay for closing door is set to > delay for opening to ensure full closure
  }else if(data->toPinLevel() == LOW && closed == 0){
    digitalWrite(Close_Relay,HIGH);
    delay(5000);
    digitalWrite(LED_Relay,LOW);
    delay(22500);
    digitalWrite(Close_Relay,LOW);
    Serial.println("Closing Bookshelf");
    closed = 1;   
   
  }
}
