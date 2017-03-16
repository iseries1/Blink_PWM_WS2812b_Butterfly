/*
  Blink_ws2812b_butterfly
  Runs through the full 255 color spectrum for a WS2812B rgb led 
 
  This example code is in the public domain.
 */

#include "ws2812b.h"

// Set up the ws2812b led 
int led1R = 13;
int led1G = 38;
int led1B = 26;
int ledWS = 20;
ws2812b ledws;

uint8_t ledArray[3] = {led1R, led1G, led1B};
uint32_t ledwsArray[3];

boolean invert = true; // set true if common anode, false if common cathode

uint8_t color = 0;         // a value from 0 to 255 representing the hue
uint8_t R, G, B;           // the Red Green and Blue color components
uint8_t brightness = 255;  // 255 is maximum brightness, but can be changed

// the setup routine runs once when you press reset:
void setup() 
{
  delay(5000);
  analogWriteResolution(8); // default is 8 which means 256 colors

  ledws.setPin(ledWS);
  
 for(int i=0; i<3; i++) {
  analogWrite(ledArray[i], 255);  // test high output of all leds in sequence
  ledwsArray[i] = ledws.ColorValue(255,255,255);  //test one led at high output
  ledws.setLEDs(ledwsArray, i);
  delay(2000);
  analogWrite(ledArray[i], 0);
  ledwsArray[i] = ledws.ColorValue(0,0,0);
  ledws.setLEDs(ledwsArray, i);
 }
 
}
// void loop runs over and over again
void loop() 
{

 for (color = 0; color < 255; color++) { // Slew through the color spectrum

  hueToRGB(color, brightness);  // call function to convert hue to RGB

  // write the RGB values to the pins
  analogWrite(led1B, B);
  analogWrite(led1R, R);
  analogWrite(led1G, G);

  invert = false;
  hueToRGB(color, 16);
  ledwsArray[0] = ledws.ColorValue(R, G, B);
  ledwsArray[1] = ledws.ColorValue(R, G, B);
  ledwsArray[2] = ledws.ColorValue(R, G, B);
  ledws.setLEDs(ledwsArray, 3);
  invert = true;
  delay(100);
 }
 
}

// Courtesy http://www.instructables.com/id/How-to-Use-an-RGB-LED/?ALLSTEPS
// function to convert a color to its Red, Green, and Blue components.

void hueToRGB(uint8_t hue, uint8_t brightness)
{
    uint16_t scaledHue = (hue * 6);
    uint8_t segment = scaledHue / 256; // segment 0 to 5 around the
                                            // color wheel
    uint16_t segmentOffset =
      scaledHue - (segment * 256); // position within the segment

    uint8_t complement = 0;
    uint16_t prev = (brightness * ( 255 -  segmentOffset)) / 256;
    uint16_t next = (brightness *  segmentOffset) / 256;

    if(invert)
    {
      brightness = 255 - brightness;
      complement = 255;
      prev = 255 - prev;
      next = 255 - next;
    }

    switch(segment ) {
    case 0:      // red
        R = brightness;
        G = next;
        B = complement;
    break;
    case 1:     // yellow
        R = prev;
        G = brightness;
        B = complement;
    break;
    case 2:     // green
        R = complement;
        G = brightness;
        B = next;
    break;
    case 3:    // cyan
        R = complement;
        G = prev;
        B = brightness;
    break;
    case 4:    // blue
        R = next;
        G = complement;
        B = brightness;
    break;
   case 5:      // magenta
    default:
        R = brightness;
        G = complement;
        B = prev;
    break;
    }
}
