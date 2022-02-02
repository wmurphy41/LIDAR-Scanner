#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <ArduinoSTL.h>
#include <Geometry.h>

// ------------------------------
// pointSpherical, pointCartesian  Headers
// ------------------------------
class Vector {
public:
  static const float pi = PI ;
  static const float deg2rad = PI / 180.0 ;
  static const float rad2deg = 180.0 / PI ;

  float theta ;   // angle in radian from x toward yz plane
  float phi ;     // angle in radian from z toward xy plane

  Vector() ;
  Vector(float, float) ;  // Constructs from phi/theta in radians
  Vector(int, int) ;      // Constructs vector from horiz/vert in degrees
  Vector(Point) ;
  set(float, float) ;

  Vector rodriguesRotate(Vector, float) ;

  Point getPoint(float distance = 1) ;
  int getHorizontal() ;
  int getVertical() ;
} ;


#endif  // _VECTOR_HPP_
