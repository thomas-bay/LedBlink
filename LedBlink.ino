// -----------------------
// Blink an LED, act as a TCP server and
// ------------
#include "Particle.h"
#include "BlinkClass.h"
#include "version.h"
/*-------------

This program will blink an led on and off in a user definable pattern.
The Blink class is used for defining the LED to blink and defining the
blink pattern.
It blinks the D7 LED on your Particle device. If you have an LED wired to D0, it will blink that LED as well.

-------------*/
String Version = "LedBlink ver 1.03";

int BlinkTheLed = TRUE;

int LEDStatusxxx;
int LoopCounter = 0;

class Version V;

#ifndef USE_SWD_JTAG
  class Blink B(D7);
#else
  class Blink B(D2);
#endif

// The setup function is a standard part of any microcontroller program.
// It runs only once when the device boots up or is reset.
int ledCommand(String command);

// telnet defaults to port 10000
TCPServer server = TCPServer(10000);
TCPClient client;
int RxCh;

void setup() {

  // We are going to tell our device that D0 and D7 (which we named led1 and led2 respectively) are going to be output
  // (That means that we will be sending voltage to them, rather than monitoring voltage that comes from them)

  // Setup the version/compilation date
  Version += " "; Version += V.getDate().c_str();
  Version += " "; Version += V.getTime().c_str();

  // Publish the variables and functions
  Particle.function("Led",ledCommand);
  Particle.variable("LEDStatus", LEDStatusxxx);
  Particle.variable("LoopCounter", LoopCounter);
  Particle.variable("Vers", Version);
  Particle.variable("Blink", BlinkTheLed);

  // Wait for something to happen in the serial input
  Serial.begin(9600);
//  while(!Serial.available()) Particle.process();
//  while(Serial.read() != -1);

  // Start TCP server and print the WiFi data
  server.begin();
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.SSID());

  // Define the blinking pattern
  int i;
  for (i = 1; i < 6; i++)
  {
    B.AddState(100*i,  HIGH);
    B.AddState(100*i,  LOW);
  }
  for (; i > 0; i--)
  {
    B.AddState(100*i,  HIGH);
    B.AddState(100*i,  LOW);
  }
  B.Print();
}

// Next we have the loop function, the other essential part of a microcontroller program.
// This routine gets repeated over and over, as quickly as possible and as many times as possible, after the setup function is called.
// Note: Code that blocks for too long (like more than 5 seconds), can make weird things happen (like dropping the network connection).  The built-in delay function shown below safely interleaves required background activity, so arbitrarily long delays can safely be done if you need them.

void loop() {

  if (BlinkTheLed == TRUE) {

    B.Action(); // This makes the LED blink without using delay().

    if ((LoopCounter++ % 5000) == 0) {

      Particle.publish("loopevent", "ged");
      Serial.printlnf("Loop = %i", LoopCounter);
      Serial.printlnf("Blink = %i", B.getCalls());

      // If more than 15 chars have been received then
      // echo them.

      // Loop the received data on serial back to terminal.
      while (Serial.available()) {
        Serial.write(Serial.read());
      }
    }
  }

  // If a client has connection, then loop all data
  if (client.connected()) {
    // echo all available bytes back to the client
    while (client.available()) {
      RxCh = client.read();
      server.write(RxCh);
      Serial.write(RxCh);
    }
  } else {
    // if no client is yet connected, check for a new connection
    client = server.available();
  }

  LEDStatusxxx = BlinkTheLed;
  // And repeat!
}

int ledCommand(String command) {

  if (command == "on")
  {
    BlinkTheLed = TRUE;
    return 1;
  }
  else
    BlinkTheLed = FALSE;

  return 0;
}
