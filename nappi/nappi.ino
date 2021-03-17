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
}
 
//two states
#define WAITING 0
#define PRESSED 1
void loop(){
   static char state = WAITING; //state machine state
   static char debounce = 0; //counter for debounce
 
   //run state machine
   switch(state){
      case WAITING:
         //wait for button to be pressed, keep debounce cleared
         if (digitalRead(BUTTON) == HIGH){
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
