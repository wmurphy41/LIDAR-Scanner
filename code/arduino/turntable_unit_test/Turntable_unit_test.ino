
/*
 *  Turntable Unit Test
 *
 *  Exercises the methods of the Turntable class.
 */

#include "Turntable.h"
#include "pins.h"         // pin assignments


// Logic constants
const int LOOP_TIMER = 5000 ;     // Time between switching modes (5s)
enum TestModes { turn_f, turn_b, pause_1, turn_s1, turn_s2, turn_s3, pause } ;

// Globals
Turntable turntable(STEP_PIN, DIR_PIN, ENABLE_PIN) ;


/* Setup()
 *
 * Start serial port
 */
void setup () {
    Serial.begin(57600);
    Serial.println("Starting setup...") ;

    Serial.println("...completed setup.") ;
}

/*
 * loop()
 * - Every 15 seconds cycle to the next test mode.
 * - Call the turntable run funcion to step the turntable
 */
void loop () {
  static TestModes test_mode = pause ;
  static long timeLastClick = -LOOP_TIMER ;  // Start testing on first pass

  // Every 15 seconds cycle through to next test mode
  if ( millis() - timeLastClick > LOOP_TIMER ) {
    timeLastClick = millis() ;
    test_mode = nextTest(test_mode) ;

    switch(test_mode) {
      case turn_f :
        Serial.println("Turning forward.") ;
        turntable.turnForward() ;
        break ;
      case turn_b :
        Serial.println("Turning backward.") ;
        turntable.turnBackward() ;
        break ;
      case turn_s1 :
      case turn_s2 :
      case turn_s3 :
       Serial.println("Turning one slice.") ;
        turntable.turnOneSlice() ;
        break ;
      default :
        Serial.println("Stopping.") ;
        turntable.stopTurn() ;
    }
  }

  // Step the turntable.
  turntable.run() ;
}

// Helper function for cycling through the test modes
int nextTest(int tm) {
  if (tm == pause)
    tm = turn_f ;
  else
    tm++ ;
  return tm ;
} 
