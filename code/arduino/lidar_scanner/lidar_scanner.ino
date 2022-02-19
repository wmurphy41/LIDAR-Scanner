// Lidar Scanner Sketch
// 
// Let's you manually position the scanner to a start position and
// then kick off an automatic scan around that start point.

#include "LidarScanner.hpp"
#include <Geometry.h>
#include <ArduinoSTL.h>
#include <map>


// Pin settings
#define SERVO_HORIZ_PIN 2
#define SERVO_VERT_PIN 3

// Physical configuration of servo
#define MIN_POS_H  20        
#define MAX_POS_H 160      
#define MIN_POS_V  -20        
#define MAX_POS_V 125      
#define SCAN_WINDOW_DEG   30 // Degrees
#define MAX_WINDOW_DEPTH 100 // CM
#define SCAN_STEP_DEG 5

// Global Variables

// Mapping for the loop commands
std::map<char, LidarScanner::directions_t> dir_cmds ;
std::map<char, LidarScanner::scan_t> scan_cmds ;

LidarScanner scanner(SERVO_HORIZ_PIN, SERVO_VERT_PIN,
					            MIN_POS_H, MAX_POS_H,
					            MIN_POS_V, MAX_POS_V) ; 
                 

// -------------------------------
// Setup code
// -------------------------------
void setup() {
	Serial.begin(9600);

  dir_cmds.insert(std::pair<char, LidarScanner::directions_t>('U', LidarScanner::up)) ;
  dir_cmds.insert(std::pair<char, LidarScanner::directions_t>('D', LidarScanner::down)) ;
  dir_cmds.insert(std::pair<char, LidarScanner::directions_t>('L', LidarScanner::left)) ;
  dir_cmds.insert(std::pair<char, LidarScanner::directions_t>('R', LidarScanner::right)) ;

  scan_cmds.insert(std::pair<char, LidarScanner::scan_t>('H', LidarScanner::horizontal)) ;
  scan_cmds.insert(std::pair<char, LidarScanner::scan_t>('V', LidarScanner::vertical)) ;
  scan_cmds.insert(std::pair<char, LidarScanner::scan_t>('S', LidarScanner::spiral)) ;

  Serial << "Moving scanner to initial position...\n" ;
  scanner.setup() ;  // Initialize and move to start pos

 }


// -------------------------------
// Loop Logic
// -------------------------------
void loop() {

  int incomingByte = 0 ;
  if (Serial.available() > 0)
    incomingByte = Serial.read() ;

  if (scanner.getScanMode() == LidarScanner::manual) {

    if (! incomingByte) return ;

    if (dir_cmds.find(incomingByte) != dir_cmds.end()) {
      scanner.moveServosRel(dir_cmds[incomingByte], SCAN_STEP_DEG) ;
      
      Serial << "Manual step " << (char) incomingByte << "\n" ;
      Serial << "New position: h = " << scanner.getHorizontalPos() ;
      Serial <<             ", v = " << scanner.getVerticalPos() << "\n" ;
      Serial << "Return: distance = " << scanner.getReturnDistance() << " cm \n" ;
    }
      
    // Initiate a scan
    else if (scan_cmds.find(incomingByte) != scan_cmds.end()) {
    scanner.startAutoScan(scan_cmds[incomingByte], SCAN_WINDOW_DEG, MAX_WINDOW_DEPTH) ;
    Serial << "Starting auto scan type: " << (char)incomingByte << "\n" ;
    }
    
    else 
     Serial << "Unknown command " << (char)incomingByte << "\n";

    return ;
  }
 
  // Autoscan mode: 
  // 1.) check if user is breaking; 
  // 2.) print current position
  // 3.) Step to next postion, cancel autoscan, if done 
  if (incomingByte == 'B') {
      Serial << "Breaking autoscan. \n" ;
      scanner.stopAutoScan() ;           
      return ;
  }
  
  Point p = scanner.getReturnPoint() ;
  Serial.println(String(-p.X(), 5) + " " + String(p.Y(), 5) + " " + String(-p.Z(), 5) ) ;
  
  bool continue_scan = scanner.autoScanStep() ;
  
  if (! continue_scan) {
      Serial << "Autoscan completed. \n " ;
      scanner.stopAutoScan() ;
  }

  return ;
}
