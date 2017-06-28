# tesis-source
Source code of the PSOC uController for the thesis project.

### Update 28 June, 2017
Commenting the global macros 
```C
//#define MANUAL_CONTROL
//#define NOT_MOVE_MOTOR
```
the motor follows the position reference (in turns) and stop when it reach the reference (with an error that can be fix moving the PID parameters).

If you only comment
```C
#define MANUAL_CONTROL
//#define NOT_MOVE_MOTOR
```
the motor follows a speed reference given by the potentiometer.
