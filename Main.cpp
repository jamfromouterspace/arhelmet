#include <iostream>
#include <stdio.h>
#include <string>
// For global variables
#include "Global.h"
// IMU (Inertial Measurement Unit) library for the BNO055 sensor
#include "RTIMULib.h"
// This header file includes simplified functions for data retrieval
// Namely, ax(), ay(), gx(), gy(), ... , euler angles phi(), theta() ...
// displayBNOData("accel"), etc.
#include "BNO.h"
<<<<<<< HEAD
// Intel RealSense SDk
#include <librealsense/rs.hpp>
#include <librealsense/rsutil.h>
// GLFW -- OpenGL library for basic windows
#include <GLFW/glfw3.h>
=======
>>>>>>> b8137ae73a6b56a39a326b8c044a2463a2366edc
// Miscellaneous functions
#include "Tools.h"
#include "Menus.h"
using namespace std;


// Function prototypes
void cleanup();

// Global variables
RTIMU_DATA BNOData;
RTIMU * bno;
RTIMUSettings * settings;


int main()
{

    BNOInit();
    
    while (true){

        cout << "\n\n---------------------------------\n";
        cout << "----WELCOME TO AR HEADSET 2.0----\n";
        cout << "---------------------------------\n\n";
        
        cout << "(Main Menu)\n";
        cout << "'b' - Display BNO data\n";
        cout << "'v' - Test computer vision\n";
        cout << "'x' - Exit program\n\n";

        if (getchar() == 'b'){
            BNOMenu();
        }

        if (getchar() == 'v'){
           // Do Intel RealSense stuff
        }

        if (getchar() == 'x') break;
        
        // Clear screen
        if (system("clear")) break;
    }

    cleanup();

    return 0;
}



void cleanup(){
// Delete global memory allocations
    delete settings;
}
