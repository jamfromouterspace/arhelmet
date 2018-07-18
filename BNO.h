#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <string>
#include "RTIMULib.h"

using namespace std;


void BNOInit();
// Get simplified 9-axis BNO055 data
bool update();
// Accel units: multiples of g
RTFLOAT ax();
RTFLOAT ay();
RTFLOAT az();
// Gyro units: rad/s
RTFLOAT gx();
RTFLOAT gy();
RTFLOAT gz();
// Compass units: uT
RTFLOAT cx();
RTFLOAT cy();
RTFLOAT cz();
// Euler angles
RTFLOAT phi();
RTFLOAT theta();
RTFLOAT psi();
// Quaternion
RTFLOAT qw();
RTFLOAT qx();
RTFLOAT qy();
RTFLOAT qz();


void displayBNOData(string type);
