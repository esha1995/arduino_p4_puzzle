// Uduino Default Board
#include<Uduino.h>
Uduino uduino("uduinoBoard"); // Declare and name your object

// variables for two pixel rings
#include <Adafruit_NeoPixel.h> // import rgb circle library thing
int ring1Pin = 7; // pin number for ring 
int ring2Pin = 6;
#define NUMPIXELS 20 // define max number of leds
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ring1Pin, NEO_GRB + NEO_KHZ800); // create our circular light as a object
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS, ring2Pin, NEO_GRB + NEO_KHZ800); // create our circular light as a object
int pixelI = 10; // intensity of color which is changes by unity later 


// variables 
int index1 = 0;
int index2 = 0;

// variables for first encoder
static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

// variables for first encoder
static int pinC = 4; // Our first hardware interrupt pin is digital pin 2
static int pinD = 5; // Our second hardware interrupt pin is digital pin 3
volatile byte cFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte dFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte reading2 = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

//variables for buttons
int button1 = 8;
int button2 = 9;

int buttonState1 = 0;
int buttonState2 = 0;

int oldButtonState1 = 0;
int oldButtonState2 = 0;



void setup()
{
  Serial.begin(9600);
  // first encoder
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(2, PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(3, PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  // second enconder
  pinMode(pinC, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinD, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(4, PinC,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(5, PinD,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  // buttons
  pinMode(button1, INPUT); // taking the input from buttons
  pinMode(button2, INPUT); 


  // LED rings 
  pixels.begin();
  pixels2.begin();


  // commands added to uduino which can be called from Unity. The commands triggers the corresponding function in this arduino code. 
  uduino.addCommand("turnOn1", turnOnPixel1); 
  uduino.addCommand("turnOn2", turnOnPixel2);
  uduino.addCommand("changeIntensity", changeIntensity);
  

  
}

void loop()
{
  uduino.update();

  // reading the state of the buttons
  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
  
  // checking if the state of the button has change
  if(buttonState1 != oldButtonState1){
    if (buttonState1 == HIGH){ // if state is high send code - 3
      Serial.println(3);
    }
    if (buttonState1 == LOW){ // if low send 4
      Serial.println(4);
    }
    oldButtonState1 = buttonState1;
  }


  if(buttonState2 != oldButtonState2){ // same as with the other button
    if (buttonState2 == HIGH){
      Serial.println(5);
    }
    if (buttonState2 == LOW){
      Serial.println(6);
    }
    oldButtonState2 = buttonState2;
  }

   
  // for loop lighting up each LED on both rings, depending on the value index1 and index2. 
  for(int i=0;i<NUMPIXELS;i++)
      {
        if (i < index1){ // if the LED is under the index(encoder) value, then the LEDS will light
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0,pixelI,0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        }
        else{ // else it will not light
        pixels.setPixelColor(i, 0); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        }
        if (i < index2){ // if the LED is under the index(encoder) value, then the LEDS will light
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels2.setPixelColor(i, pixels2.Color(0,pixelI,0)); // Moderately bright green color.
        pixels2.show(); // This sends the updated pixel color to the hardware.
        }
        else{ // else it will not light
        pixels2.setPixelColor(i, 0); // Moderately bright green color.
        pixels2.show(); // This sends the updated pixel color to the hardware.
        }
    }
}

// functions that takes an argument from unity when called. 
void turnOnPixel1(){
  index1 = atoi(uduino.getParameter(0)); // index1 (int controlling number of pixels which should light up) becomes the first argument sent from unity when function is called
}

// functions that takes an argument from unity when called.
void turnOnPixel2(){
  index2 = atoi(uduino.getParameter(0));// index1 (int controlling number of pixels which should light up) becomes the first argument sent from unity when function is called
} 

// functions that takes an argument from unity when called.
void changeIntensity(){
  pixelI = atoi(uduino.getParameter(0));
}



// interrupt functions for all encoders: 
void PinA(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 12; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == 12 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(1);
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == 4) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 12; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == 12 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(2);//increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == 4) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinC(){
  cli(); //stop interrupts happening before we read pin values
  reading2 = PIND & 60; // read all eight pin values then strip away all but pinC and pinD's values
  if(reading2 == 60 && cFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(7);
    dFlag = 0; //reset flags for the next turn
    cFlag = 0; //reset flags for the next turn
  }
  else if (reading2 == 44) dFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinD(){
  cli(); //stop interrupts happening before we read pin values
  reading2 = PIND & 60; //read all eight pin values then strip away all but pinC and pinD's values
  if (reading2 == 60 && dFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(8);//increment the encoder's position count
    dFlag = 0; //reset flags for the next turn
    cFlag = 0; //reset flags for the next turn
  }
  else if (reading2 == 44) cFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}
