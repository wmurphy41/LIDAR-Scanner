#include "Turntable.h"


// Constructor
// Initalizes the stepper driver with pins and constants
Turntable::Turntable(int step_pin, int dir_pin, int en_pin) :
  stepper(AccelStepper(AccelStepper::DRIVER, step_pin, dir_pin)) ,
  enable_pin(en_pin) ,
  mode(stopped) {

    stepper.setSpeed(CONSTANT_SPEED) ;
    stepper.setMaxSpeed(MAX_SPEED) ;
    pinMode(en_pin, OUTPUT) ;
    disableStepper() ;
}

// run
// If we are in turnForward/Backward mode, then bump out the
// set position so we never stop.
// Have the stepper run one step.
// If we're no longer running, disable the stepper
// Return true if the stepper is running.
bool Turntable::run() {

  // How do we keep running? 
  
  stepper.runSpeedToPosition() ;

  bool is_running = stepper.isRunning() ;
  if (! is_running) {
    disableStepper() ;
    mode = stopped ;
  }
  return (is_running) ;
}

// turnForward
// Set the run position to a large positive number
// enable the stepper adn set the mode
void Turntable::turnForward() {
  hardStop() ;                 // Hard stop if it's currently running
  stepper.move(LARGE_NUMBER) ; 
  stepper.setSpeed(CONSTANT_SPEED) ;
  enableStepper() ;
  mode = turn_fwd ;
}

// turnBackward
// Set the run position to a large negative number
// enable the stepper adn set the mode
void Turntable::turnBackward() {
  hardStop() ;                 // Hard stop if it's currently running
  stepper.move(-LARGE_NUMBER) ;
  stepper.setSpeed(CONSTANT_SPEED) ;
  enableStepper() ;
  mode = turn_bwd ;
}

// turnOneSlice
// Set the run position to the number of steps forward that equals one slice
// Enable the stepper motor and set the mode
void Turntable::turnOneSlice() {
  hardStop() ;                 // Hard stop if it's currently running
  stepper.move(STEPS_PER_SLICE) ;
  stepper.setSpeed(CONSTANT_SPEED) ;
  enableStepper() ;
  mode = turn_slice ;
}

// stopTurn
void Turntable::stopTurn() {
  hardStop() ;
  return ;  
}


// enableSteppers - sets pin low (enabled)
void Turntable::enableStepper() {
    digitalWrite (enable_pin, LOW);
    is_enabled = true ;
}

// disableStepper - sets pin high (disabled)
void Turntable::disableStepper() {
    digitalWrite (enable_pin, HIGH);
    is_enabled = false ;
}

// hardStop - stops the turntable immediately 
// blocking while stopping
// Leaves stepper disabled when done
void Turntable::hardStop() {
    enableStepper() ;
    stepper.stop() ;
    stepper.setSpeed(CONSTANT_SPEED) ;
    stepper.runSpeedToPosition() ;
    disableStepper() ;
}
