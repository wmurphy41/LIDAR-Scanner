/**
 * Vector 
 * by Daniel Shiffman.  
 * 
 * Demonstration some basic vector math: subtraction, normalization, scaling
 * Normalizing a vector sets its length to 1.
 */

  // Vector being drawn
  
  float azimuth_deg = 135 ;
  float elevation_deg = 45 ;
  float phi = (90 * PI/180) - (elevation_deg * PI/180) ;
  float theta = (azimuth_deg * PI/180) - (90 * PI/180) ;
  float radius = 200.0 ;
  float rotation = 45 * PI / 180 ;

  PVector x_axis = new PVector(500*sin(PI/2)*cos(0), 0, 0) ;
  PVector y_axis = new PVector(0, 500*sin(PI/2)*sin(PI/2), 0) ; 
  PVector z_axis = new PVector(0, 0, 500*cos(0)) ; 
  PVector k_vector ;
  PVector k_vector1 ;
  PVector v_vector ;

  float xmag, ymag = 0;
  float newXmag, newYmag = 0; 
  
  void setup() {
    size(1000, 1000, P3D);
    colorMode(RGB, 255, 255, 255);
    noSmooth();    
  
    k_vector = new PVector(
                      1 * sin(phi) * cos(theta), 
                      1 * sin(phi) * sin(theta), 
                      1 * cos(phi)) ;
                      
    k_vector1 = new PVector(
                      radius * sin(phi) * cos( theta), 
                      radius * sin(phi) * sin(theta), 
                      radius * cos(phi)) ;
    v_vector = new PVector() ;        
  }
  
  void draw() {
  

    background(0.5);
    
    pushMatrix(); 
    translate(width/2, height/2, -30); 
    
    newXmag = mouseX/float(width) * TWO_PI;
    newYmag = mouseY/float(height) * TWO_PI;
    
    float diff = xmag-newXmag;
    if (abs(diff) >  0.01) { 
      xmag -= diff/4.0; 
    }
    
    diff = ymag-newYmag;
    if (abs(diff) >  0.01) { 
      ymag -= diff/4.0; 
    }
    
    rotateX(-ymag); 
    rotateY(-xmag); 
    
    // scale(90);
  
    
    // Draw the axes
    stroke(126);
    strokeWeight(4);
    line(0, 0, 0, x_axis.x, x_axis.y, x_axis.z);
    line(0, 0, 0, y_axis.x, y_axis.y, y_axis.z);
    line(0, 0, 0, z_axis.x, z_axis.y, z_axis.z);
    
    // Draw the rotation vector
    stroke(0, 0, 255);
    strokeWeight(4);
    line(0, 0, 0, k_vector1.x, k_vector1.y, k_vector1.z);
 
    // Draw the rotated vector
    for (float offset = 20*PI/180; offset < 85*PI/180; offset += 10*PI/180) {
      
      v_vector.set( radius * sin(phi+offset) * cos( theta), 
                    radius * sin(phi+offset) * sin(theta), 
                    radius * cos(phi+offset)) ;
      stroke(0, 255, 0);
      strokeWeight(4);
      line(0, 0, 0, v_vector.x, v_vector.y, v_vector.z);
    
      for (float r_theta = PI/45; r_theta < (TWO_PI-PI/45) ; r_theta += PI/45) {
   
        PVector v_rot = rodriguesRotate(k_vector, v_vector, r_theta) ;

        stroke(255, 0, 0);
        strokeWeight(4);
        line(0, 0, 0, v_rot.x, v_rot.y, v_rot.z);
      }
    }
    
     popMatrix(); 
  
  }

 // Rodrigues' rotation formula
 // Rotates v_vector theta degrees around unit axis k_vector
  PVector rodriguesRotate(PVector k_vector, PVector v_vector, float theta) {
  
    PVector k_cross_v = k_vector.cross(v_vector) ;
    PVector k_cross_k_cross_v = k_vector.cross(k_cross_v) ;
    
    PVector k_v1 = PVector.mult(k_cross_v, sin(theta)) ;
    PVector k_k_v1 = PVector.mult(k_cross_k_cross_v, (1 - cos(theta))) ;
    
    PVector v_rot1 = PVector.add(v_vector, k_v1) ;
    PVector v_rot = PVector.add(v_rot1, k_k_v1) ;
  
    return v_rot ;
  }
