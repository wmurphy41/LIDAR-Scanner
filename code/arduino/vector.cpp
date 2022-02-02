#include "vector.hpp"
#include <Arduino.h>



// ***************************************
// Vector
// ***************************************
// Class:  Vector
Vector::Vector() { theta = phi = 0.0 ; } 
Vector::Vector(float t, float p) {
  set(t, p) ;
}
Vector::Vector(Point p) {
  theta = atan2( p.Y() , p.X() ) ;
  phi = PI + atan( sqrt( sq(p.X()) + sq(p.Y()) ) / p.Z() ) ; 
}
Vector::Vector(int posH, int posV) {
   phi = (180 - posV) * deg2rad, 
   theta = (90 - posH) * deg2rad ;
}
Vector::set(float t, float p) {
  theta = t ;
  phi = p ;
}

int Vector::getHorizontal() {
  return (int) (90.0 - theta*rad2deg) ;
}

int Vector::getVertical() {
  return (int) (180.0 - phi*rad2deg) ;
}

// getPoint
// Converts vector + distance into cartesian point
// representing the point radius units along the vector
// if radius is omitted, it returns the unit vector
Point Vector::getPoint(float radius) {
      Point p ;
      p.X() = radius * sin(phi) * cos(theta) ;
      p.Y() = radius * sin(phi) * sin(theta) ;
      p.Z() = radius * cos(phi) ;

      return p ;
}

// Rodrigues' rotation formula
// Rotates v_vector theta degrees around this axis 
Vector Vector::rodriguesRotate(Vector v_vector, float theta) {

  Point k = getPoint() ;
  Point v = v_vector.getPoint() ;

  Point k_cross_v = k.CrossProduct(v) ;
  Point k_cross_k_cross_v = k.CrossProduct(k_cross_v) ;

  Point k_v1 = k_cross_v * sin(theta) ;
  Point k_k_v1 = k_cross_k_cross_v * (1 - cos(theta)) ;

  Point v_rot1 = v + k_v1 ;
  Point v_rot = v_rot1 + k_k_v1 ;

  return Vector(v_rot) ;

}
