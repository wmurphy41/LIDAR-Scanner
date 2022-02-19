#ifndef TURNTABLE_H
#define TURNTABLE_H

#include <AccelStepper.h>


// Turntable class
// Built around the AccelStepper classs
//
//

class Turntable {
private:
  // constants
  const int STEPS_PER_REV = 2048 ;
  const int TEETH_PER_SMALL_GEAR = 12 ;
  const int TEETH_PER_LARGE_GEAR = 84 ;
  const long SLICES_PER_REV = 8 ;          // 45 deg or 7.5 minutes
  const long STEPS_PER_SLICE = 1792 ;
// STEPS_PER_REV * TEETH_PER_LARGE_GEAR / TEETH_PER_SMALL_GEAR / SLICES_PER_REV ;
  const float MAX_SPEED = 1000 ;      // steps per sec. 4k is max for arduino
  const float CONSTANT_SPEED = 500 ;
  const float ACCELERATION = 30 ;
  const long LARGE_NUMBER = 100000 ;
  const long SMALL_NUMBER = 0 ;

  bool is_enabled ;                     // State of enable pin

  AccelStepper stepper ;
  int enable_pin ;              // Arduino pin for enabling stepper
  enum Mode {turn_fwd, turn_bwd, turn_slice, stopping, stopped} ;
  Mode mode ;

// Methods
private:
  // private methods
  void enableStepper() ;
  void disableStepper() ;
  void hardStop() ;

public:
  Turntable(int step_pin, int dir_pin, int en_pin) ;

  void turnOneSlice() ;
  void turnForward() ;
  void turnBackward() ;
  void stopTurn() ;
  bool run() ;
};



#endif //TURNTABLE_H
