// -----------------------
// Blink an LED, act as a TCP server and
// ------------
#include "Particle.h"
#include "BlinkClass.h"
#include "version.h"
/*-------------

We've heavily commented this code for you. If you're a pro, feel free to ignore it.

Comments start with two slashes or are blocked off by a slash and a star.
You can read them, but your device can't.
It's like a secret message just for you.

Every program based on Wiring (programming language used by Arduino, and Particle devices) has two essential parts:
setup - runs once at the beginning of your program
loop - runs continuously over and over

You'll see how we use these in a second.

This program will blink an led on and off every second.
It blinks the D7 LED on your Particle device. If you have an LED wired to D0, it will blink that LED as well.

// access_token = 2a0bf3f1fa3d75cdc51e8c945f974e6ccaffded0

-------------*/
String Version = "LedBlink ver 1.02";

int BlinkTheLed = TRUE;

int LEDStatus;
int LoopCounter = 0;

class Version V;
class Blink B(D7);

// First, we're going to make some variables.
// This is our "shorthand" that we'll use throughout the program:

int led1 = D0; // Instead of writing D0 over and over again, we'll write led1
// You'll need to wire an LED to this one to see it blink.

int led2 = D7; // Instead of writing D7 over and over again, we'll write led2
// This one is the little blue LED on your board. On the Photon it is next to D7, and on the Core it is next to the USB jack.

// Having declared these variables, let's move on to the setup function.
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

  // It's important you do this here, inside the setup() function rather than outside it or in the loop function
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Setup the version/compilation date
  Version += " "; Version += V.getDate().c_str();
  Version += " "; Version += V.getTime().c_str();

  // Publish the variables and functions
  Particle.function("Led",ledCommand);
  Particle.variable("LEDStatus", LEDStatus);
  Particle.variable("LoopCounter", LoopCounter);
  Particle.variable("Vers", Version);
  Particle.variable("Blink", BlinkTheLed);

  // Wait for something to happen in the serial input
  Serial.begin(9600);
  while(!Serial.available()) Particle.process();
  while(Serial.read() != -1);

  // Start TCP server and print the WiFi data
  server.begin();
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.SSID());

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

  LEDStatus = BlinkTheLed;
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
