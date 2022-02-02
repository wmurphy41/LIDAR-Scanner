#ifndef _LIDARSCANNER_HPP_
#define _LIDARSCANNER_HPP_

#include <Servo.h>
#include <LIDARLite.h>
#include "vector.hpp"

// ------------------------------
// LidarScanner Class Header
// ------------------------------
// Geometry notes:
// - The lidar is positioned using servos that operate
//   in degrees from 0 to 180.  (Each has stops that are between 
//   those extremes.
// - Horizontal/Vertical/Distance
//   - Can be queried individually and are returned in 
//     degree/degree/cm
//   - Any position that is H(orizontal) or V(ertical) will
//     be in deg
// - Cartesian points
//   used to return the radar return results in x, y, z format
// - Spherical coordinates
//   - Used internally for calculating positioning needed for the
//     spiral scan.
//   - Angles are referred to as phi and theta and are in radians
class LidarScanner {
public:

  enum directions_t {left, right, up, down} ;
  enum scan_t {manual, horizontal, vertical, spiral} ;

private:
	Servo servoH ;
	Servo servoV ;
	LIDARLite lidar ;

	// Server physical stops in deg and Pin #s
	int minPosH, maxPosH ;
	int minPosV, maxPosV ;
  int hPin, vPin ;

  scan_t scanMode ;
  int resetPosH, resetPosV ; 

  // auto scan window
  int scanRadius ;
  int minScanH, maxScanH ;
  int minScanV, maxScanV ;
  int maxScanDepth ;
  int maxScanRadius ;

  // autoscan temp vars (horiz and vert)
	short scanDir = 1 ;
	int scanIncrement = 1 ;
	int stepCount = 0 ;

  // auto temp vars (spiral scan)
  float offset, rotationTheta, maxOffset ;
  float rotationIncrement = 5.0 * Vector::deg2rad ;
  Vector axisVector ;

  public:
  // Constructors and setup
	LidarScanner(int, int, int, int, int, int) ;
	void setup() ;

  // Reference methods
  scan_t getScanMode() ;
  int getReturnDistance() ;
  int getHorizontalPos() ;
  int getVerticalPos() ;
  Point getReturnPoint() ;
  Vector getVector() ;
  void setResetPos(int, int) ;

  // Scanner positioning methods
  void resetPosition() ;
  void moveServosRel(directions_t, int) ;
  void moveServosAbs(int, int) ;

  // Autoscanning methods
  void startAutoScan(scan_t, int, int) ;
  void stopAutoScan() ;
  bool autoScanStep() ;
  private: 
  bool autoScanStep_H() ;
  bool autoScanStep_V() ;
  bool autoScanStep_S() ;
} ;

#endif  // _LIDARSCANNER_HPP_
