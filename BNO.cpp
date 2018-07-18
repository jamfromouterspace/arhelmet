#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <string>
#include "RTIMULib.h"
#include "BNO.h"
#include "Global.h"

using namespace std;

// Global variables defined in Main.cpp: 
// RTIMU_DATA BNOData;
// RTIMU * bno;
// RTIMUSettings * settings;

// RTIMU_DATA is a struct found in RTIMULibDefs.h
// It contains variables of type RTVector3 and RTQuaternion
// These classes are defined in RTMath.h and RTMath.cpp


#define ERROR_CODE 1;


// NOTE:    There are many more useful functions in the RTVector3 class
//          (dotProduct, crossProduct, normalize, conversion between Euler and Quat, etc)
//          The following functions are for quick and simplified access to the most important data.


void BNOInit(){

    // Calibration and settings information retrieved from RTIMUCal.ini

    // --ASIDE-- C++ creates the string literal "RTIMUCal" as a const char *, but it allows
    // --------- assignment to a regular string for backwards compatibility with C,
    // --------- which does NOT create it as a constant. But it's not "proper",
    // --------- so to avoid the compiler warning we cast it to a char *

    char * settingsFile = (char *)"RTIMUCal";
    RTIMUSettings * settings = new RTIMUSettings(settingsFile);


    bno = RTIMU::createIMU(settings);

    if ((bno == NULL) || (bno->IMUType() == RTIMU_TYPE_NULL)) {
        printf("BNO055 not detected.\n");
        exit(1);
    }

    bno->IMUInit();
    bno->setCalibrationData();
}

// Read data and update data object
bool update(){
   if (bno->IMURead()){
      BNOData = bno->getIMUData();
      return true;
   }
   else {
      cout << "Error: could not read IMU data" << endl;
      return false;
   }
}

// The following functions are very simple: update the data, then read from the RTIMU_DATA struct
// For custom use, simply update() and access what you need, for example, BNOData.accel.normalize()


// Acceleration data in multiples of g
RTFLOAT ax(){
   if (update()) {

      if( BNOData.accelValid )
        return BNOData.accel.x();
      else {
        cout << "Accel data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}


RTFLOAT ay(){
   if (update()) {

      if( BNOData.accelValid )
        return BNOData.accel.y();
      else {
        cout << "Accel data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}

RTFLOAT az(){
   if (update()) {

      if( BNOData.accelValid )
        return BNOData.accel.z();
      else {
        cout << "Accel data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}



// Gyro data in rad/s
RTFLOAT gx(){
   if (update()) {

      if( BNOData.gyroValid )
        return BNOData.gyro.x();
      else {
        cout << "Gyro data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}

RTFLOAT gy(){
   if (update()) {

      if( BNOData.gyroValid )
        return BNOData.gyro.y();
      else {
        cout << "Gyro data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}

RTFLOAT gz(){
   if (update()) {

      if( BNOData.gyroValid )
        return BNOData.gyro.z();
      else {
        cout << "Gyro data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}



// Compass data in uT
RTFLOAT cx(){
   if (update()) {

      if( BNOData.compassValid )
        return BNOData.compass.x();
      else {
        cout << "Compass data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}

RTFLOAT cy(){
   if (update()) {

      if( BNOData.compassValid )
        return BNOData.compass.y();
      else {
        cout << "Compass data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}

RTFLOAT cz(){
   if (update()) {

      if( BNOData.compassValid )
        return BNOData.compass.z();
      else {
        cout << "Compass data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}



// Euler angles (angular orientation with respect to North)

// TO VISUALIZE:
// Rotation about the axis that goes through your ears
// (i.e. looking up to the sky)
RTFLOAT phi(){
   if (update()) {

      if( BNOData.fusionPoseValid )
        return BNOData.fusionPose.x();
      else {
        cout << "Euler fusion data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
    }
    else return ERROR_CODE;
}

// Rotation about the direction your eyes point 
// (i.e. ears towards shoulders while looking straight ahead)
RTFLOAT theta() {
   if (update()) {

      if( BNOData.fusionPoseValid )
        return BNOData.fusionPose.y();
      else {
        cout << "Euler fusion data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}

// Rotation about the "vertical" axis that goes through your body
// from foot to head
// (i.e. rotate head while keeping neck straight)
RTFLOAT psi() {
   if (update()) {

      if( BNOData.fusionPoseValid )
        return BNOData.fusionPose.z();
      else {
        cout << "Euler fusion data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}

// Quaternion scalar
RTFLOAT qw() {
   if (update()) {

      if( BNOData.fusionQPoseValid )
        return BNOData.fusionQPose.scalar();
      else {
        cout << "Quaternion fusion data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}

// Quaternion x,y,z
RTFLOAT qx() {
   if (update()) {

      if( BNOData.fusionQPoseValid )
        return BNOData.fusionQPose.x();
      else {
        cout << "Quaternion fusion data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}

RTFLOAT qy() {
   if (update()) {

      if( BNOData.fusionQPoseValid )
        return BNOData.fusionQPose.y();
      else {
        cout << "Quaternion fusion data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}

RTFLOAT qz() {
   if (update()) {

      if( BNOData.fusionQPoseValid )
        return BNOData.fusionQPose.z();
      else {
        cout << "Quaternion fusion data is marked as invalid by RTIMULib" << endl;
        return ERROR_CODE;
      }
   }
   else return ERROR_CODE;
}



int p = 5;

void displayBNOData(string type){

    if (type == "accel"){
        cout << "\nAccel (x,y,z in gs)\n  " << setprecision(p) << ax();
	    cout << "\n  " << setprecision(p) << ay(); 
	    cout << "\n  " << setprecision(p) << az() << "\n\n";
    }

    if (type == "gyro"){
	    cout << "\nGyro (x,y,z in rad/s)\n  " << setprecision(p) << gx(); 
	    cout << "\n  " << setprecision(p) << gy(); 
	    cout << "\n  " << setprecision(p) <<  gz() << "\n\n";
    }

    if (type == "compass"){
	    cout << "\nCompass (x,y,z in uT)\n  " << setprecision(p) << cx(); 
	    cout << "\n  " << setprecision(p) << cy(); 
	    cout << "\n  " << setprecision(p) << cz() << "\n\n";
    }

    if (type == "euler"){
	    cout << "\nEuler (phi, theta, psi in rad)\n  " << setprecision(p) << phi(); 
	    cout << "\n  " << setprecision(p) << theta(); 
	    cout << "\n  " << setprecision(p) << psi() << "\n\n";
    }
    if (type == "quat" || type == "quaternion"){
	    cout << "\nQuaternion (scalar, x, y, z )\n  " << setprecision(p) << qw(); 
	    cout << "\n  " << setprecision(p) << qx(); 
        cout << "\n  " << setprecision(p) << qy(); 
	    cout << "\n  " << setprecision(p) << qz() << "\n\n";
    }
}










