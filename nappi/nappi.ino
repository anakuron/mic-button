#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 3 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 1 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

#include "Keyboard.h"
 
#define PTT 240
#define BUTTON 2
#define DEBOUNCE 1 //multiples of tick rate
#define TICK 100 //tick rate
 
void setup(){
   //setup button
   pinMode(BUTTON, INPUT);
   digitalWrite(BUTTON, HIGH);
 
   //setup keyboard
   Keyboard.begin();

   pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}
 
//two states
#define WAITING 0
#define PRESSED 1
void loop(){
   pixels.clear(); // Set all pixel colors to 'off'
   static char state = WAITING; //state machine state
   static char debounce = 0; //counter for debounce
 
   //run state machine
   switch(state){
      case WAITING:
         //wait for button to be pressed, keep debounce cleared
         if (digitalRead(BUTTON) == HIGH){
            pixels.setPixelColor(0, pixels.Color(0, 255, 0));
            pixels.show();   // Send the updated pixel colors to the hardware.
            debounce = 0;
            break;
         }
 
         //wait for button to stop bouncing
         debounce ++;
         if ( debounce < DEBOUNCE)
            break;
 
         //press key
         Keyboard.press(PTT);
        
         //change state
         state = PRESSED;
         pixels.setPixelColor(0, pixels.Color(255, 0, 0));
         pixels.show();
         break;
 
      case PRESSED:
         //wait for button to be released, keep debounce cleared
         if(digitalRead(BUTTON) == LOW){
            debounce = 0;
            break;
         }
 
         //wait for button to stop bouncing
         debounce ++;
         if ( debounce < DEBOUNCE)
            break;
 
         //release key
         Keyboard.release(PTT);
 
         //change state
         state = WAITING;
         break;
   }
 
   //wait for next tick
   delay(1000/TICK);
}
