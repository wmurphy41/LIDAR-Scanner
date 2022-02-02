# 3D Lidar Scanner
 3D Printed 3D Lidar Scanner

 ## Project Overview

 #### References and Attributions



 #### Background Information


## Files
Content of the project folders:
- **code** - Arduino and processing code
- **physical** - STL Files
- **circuits** - electronic circuit design
- **media** - pictures and video of project

## Bill of Materials
- 1x Arduino nano


## Installation
Instructions for building the printed components, electronics, software, and assembly of the 3D Scanner

#### Printed Components
Print all components in PLA.

| Component  | Count | Color |
| ---------- | ----: | ----- |
| xxxx       |     1 | black |


#### Electronics
1. Hack the stepper motors.  The 28BYJ-48 stepper motors are unipolar and the
A4988 driver only works with bipolar motors.  Take the red wire out of the connector,
cut it near the motor and discard.  Reorder the wires in the connector so that
are in order orange, yellow, pink, blue, reading left to right when looking at
the flat side of the connector.  More info on this step [here](https://www.instructables.com/28BYJ-48-5V-Stepper-Motor-and-A4988-Driver/)  I ignored adjusting
the current potentiometer.
2. Build prototype board based on circuit diagrams.  Pins.h matches the pin
configuration of the prototype circuit diagram.  Breadboard diagram used different pins.

#### Code

## Assembly

## Usage
Operating instructions

## Ideas for Future Enhancements


## Contributing
- Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
- Please make sure to test updates as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
