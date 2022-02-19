#include "LidarScanner.hpp"
#include <Arduino.h>



// ***************************************
// Constructors and initialization routines
// ***************************************
// -------------------------------
// LidarScanner class definition
// -------------------------------
LidarScanner::LidarScanner(int h, int v, int minh, int maxh, int minv, int maxv) {
	hPin = h ;
	vPin = v ;

  // Set the min/max stops
	minPosH = minh ;
	maxPosH = maxh ;
	minPosV = minv ;
	maxPosV = maxv ;
  maxScanDepth = INT16_MAX ;
  
  scanMode = manual ;

  setResetPos((minPosH + maxPosH)/2, (minPosV + maxPosV)/2) ;
}

// Lidar setup
// initialize lidar, attach servo pins, and move to start position
void LidarScanner::setup() {

	lidar.begin(0, true) ;
	lidar.configure(0) ;

	servoH.attach(hPin) ;
	servoV.attach(vPin) ;

  resetPosition() ; // center scanner
}

// ***************************************
// Reference methods
// ***************************************
// getScanMode:
LidarScanner::scan_t LidarScanner::getScanMode() {
   return scanMode ;
}
// getHorizontalPos:
int LidarScanner::getHorizontalPos() {
   return servoH.read() ;
}
// getVerticalPos:
int LidarScanner::getVerticalPos() {
   return servoV.read() ;
}

// getReturnDistance() ;
// Record lidar reading
// Calibrate every 100 steps
int LidarScanner::getReturnDistance() {

  stepCount += 1;
  bool calibrate = (stepCount % 100 == 0) ;
  int d = lidar.distance(calibrate) ;
  return std::min(d, maxScanDepth) ;
}

// getVector() 
Vector LidarScanner::getVector() {
  int posH = servoH.read() ;
  int posV = servoV.read();
  Vector v(posH, posV) ;
  return  v ;
}
// getReturnPoint() 
Point LidarScanner::getReturnPoint() {
  int d = getReturnDistance() ;
  Vector v = getVector() ;
  
  return v.getPoint((float)d) ;
}

void LidarScanner::setResetPos(int h, int v) {
  resetPosH = h ;
  resetPosV = v ;
}


// ***************************************
// Positioning the Scanner
// ***************************************
// resetPosition
// repositions to starting positions
void LidarScanner::resetPosition() {
  moveServosAbs(resetPosH, resetPosV) ;
}

// moveServosRel:
// Calculate new position from direction and increment
// Signal servo to move to new position and pause to let it move
void LidarScanner::moveServosRel(directions_t dir, int inc) {

    int newPosH = getHorizontalPos() ;
    int newPosV = getVerticalPos() ;

    switch (dir) {
      case left :
        newPosH -= inc ;
        break ;
      case right : 
        newPosH += inc ;
        break ;
      case down :
        newPosV -= inc ;
        break ;
      case up : 
        newPosV += inc ;
        break ;
    }

    moveServosAbs(newPosH, newPosV) ;

    return ;
}
// moveServosAbs:
// Make sure new position is not out of bounds
// Signal servo to move to new position and pause to let it move
void LidarScanner::moveServosAbs(int newPosH, int newPosV) {

    newPosH = constrain(newPosH, minPosH, maxPosH) ;
    newPosV = constrain(newPosV, minPosV, maxPosV) ;

    servoH.write(newPosH);   
    servoV.write(newPosV);
    
    delay(30);
}

// ***************************************
// Auto Scanning Methods
// ***************************************
// General methods for scanning:  
// These routines are meant to be run from the Arduino loop
// so the scan is broken into steps, and then control returned
// to the loop.
//
// On start...
// - Set up control parameters
// - Move to first scan position
// - Return to loop
//
// In the loop:
// - check distance, then call next step
//
// For each step:
// - Calculate next scan position
// - If out of boundaries, we're done
// - Move to next scan position


// startAutoScan:
// Sets the autoscan window based on current position 
// and size and moves to the starting point.
void LidarScanner::startAutoScan(scan_t scanType, int maxScanDiam, int windowDepth) {

  int posH = getHorizontalPos() ;
  int posV = getVerticalPos() ;
  setResetPos(posH, posV) ;
  maxScanRadius = maxScanDiam/2 ;
  scanMode = scanType ;
  maxScanDepth = windowDepth ;
  scanDir = 1 ;

  if(scanMode == horizontal || scanMode == vertical) { 
      minScanH = constrain (posH - maxScanRadius, minPosH, maxPosH) ;
      maxScanH = constrain (posH + maxScanRadius, minPosH, maxPosH) ;
      minScanV = constrain (posV - maxScanRadius, minPosV, maxPosV) ;
      maxScanV = constrain (posV + maxScanRadius, minPosV, maxPosV) ;
      moveServosAbs(minScanH, minScanV) ;
  }
  
  else { // spiral
    offset = 0 ;
    rotationTheta = 2 * PI ;
    maxOffset = maxScanRadius * Vector::deg2rad ;
    axisVector = getVector() ;
    Serial << " axisVector rad: ( " << axisVector.theta << ", " << axisVector.phi << " )\n" ;
    Serial << " axisVector deg: ( " << axisVector.getHorizontal() << ", " << axisVector.getVertical() << " )\n" ;
    // No need to move... center is the first scan position
 }
}

// stopAutoScan:
// resests the autoscan window and resets the scan flag
// and size and moves to the starting point.
void LidarScanner::stopAutoScan() {

    maxScanDepth = INT16_MAX ;
    scanMode = manual ;
    
    resetPosition() ; // move back to reset position
}

// AutoScanStep:
// take one step in the direction determined by the
// scanMode 
bool LidarScanner::autoScanStep () {

  bool results = false ;
  
  switch (scanMode) {
    case horizontal:
      results = autoScanStep_H() ;
      break ;
    case vertical:
      results = autoScanStep_V() ;
      break ;
    case spiral:
      results = autoScanStep_S() ;
      break ;
    default:
      break ;
  }
  return results ;
}

// AutoScanStep_H:  
// If we're done, return without moving
// Update position vector one increment
// At horiz lim, reverse and move position up
// Move servo to new position
// Returns true if it moved, false if done
bool LidarScanner::autoScanStep_H () {

  // Calculate next scan position
  int newPosH, newPosV ;
  newPosV = getVerticalPos() ;
  newPosH = getHorizontalPos() + (scanDir * scanIncrement) ;

  if (newPosH > maxScanH || newPosH < minScanH) {
    scanDir = -1 * scanDir ;
    newPosH += scanDir * scanIncrement ;
    newPosV += scanIncrement ;
  }

  // if we're past the vertical limit, we're done
  if (newPosV > maxScanV) return false ;

  // Move to next position
  moveServosAbs(newPosH, newPosV) ;
  return true ;
 
 }

// AutoScanStep_V:
// If we're done, return without moving
// Update position vector one increment
// At VERTICAL lim, reverse and move position LEFT
// Move servo to new position
// Returns true if it moved, false if done
bool LidarScanner::autoScanStep_V () {

  // Calculate next position
  int newPosH, newPosV ;
  newPosH = getHorizontalPos() ;
  newPosV = getVerticalPos() + (scanDir * scanIncrement) ;
  if (newPosV > maxScanV || newPosV < minScanV) {
    scanDir = -1 * scanDir ;
    newPosV += scanDir * scanIncrement ;
    newPosH += scanIncrement ;
  }

  // Return if we're done
  if (newPosH > maxScanH) return false ; 

  // Move the servo
  moveServosAbs(newPosH, newPosV) ;
  return true ;
}

// AutoScanStep_S:
// calculate base vector (axis + horizontal offset)
// If we're done, then return
// Otherwise move the scanner
bool LidarScanner::autoScanStep_S () {

  // * Calculate next scan offset and rotationTheta 
  rotationTheta += 1 * Vector::deg2rad ;
  if (rotationTheta >= 2*PI) {
    rotationTheta = 0 ;
    offset += rotationIncrement ;
  }

  // * Return if done
  if (offset > maxOffset) return false ;

  // * Calculate rotation and move servo 
  Vector vector(axisVector.theta + offset, axisVector.phi) ;
  Vector v_rot = axisVector.rodriguesRotate(vector, rotationTheta) ;
  moveServosAbs(v_rot.getHorizontal(), v_rot.getVertical()) ;

/*
  Serial << "offset, rotationTheta: " << offset << ", " << rotationTheta << "\n" ;
  Serial << " vector rad: ( " << vector.theta << ", " << vector.phi << " )\n" ;
  Serial << " vector deg: ( " << vector.getHorizontal() << ", " << vector.getVertical() << " )\n" ;
  Serial << " v_rot rad: ( " << v_rot.theta << ", " << v_rot.phi << " )\n" ;
  Serial << " v_rot deg: ( " << v_rot.getVertical() << ", " << v_rot.getHorizontal() << " )\n" ;
*/

  return true ;
}
