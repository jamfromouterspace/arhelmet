#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <string>
// For global variables
#include "Global.h"
// IMU (Inertial Measurement Unit) library for the BNO055 sensor
#include "RTIMULib.h"
// This header file includes simplified functions for data retrieval
// Namely, ax(), ay(), gx(), gy(), ... , euler angles phi(), theta() ...
// displayBNOData("accel"), etc.
#include "BNO.h"
// Intel RealSense SDk
#include <librealsense/rs.hpp>
#include <librealsense/rsutil.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>
// GLFW -- OpenGL library for creating windows
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
// Miscellaneous functions
#include "Tools.h"
#include "Menus.h"

using namespace cv;
using namespace std;


// Function prototypes
void cleanup();

// Global variables
RTIMU_DATA BNOData;
RTIMU * bno;
RTIMUSettings * settings;
const float MAX_DEPTH = 4.0;

double yaw, pitch, lastX, lastY; int mleft;
static void on_mouse_button(GLFWwindow * win, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT) mleft = action == GLFW_PRESS;
}
static double clamp(double val, double lo, double hi) { return val < lo ? lo : val > hi ? hi : val; }
static void on_cursor_pos(GLFWwindow * win, double x, double y)
{
    if(mleft)
    {
        yaw = clamp(yaw - (x - lastX), -120, 120);
        pitch = clamp(pitch + (y - lastY), -80, 80);
    }
    lastX = x;
    lastY = y;
}


int main()
{
	char ch;
	// Get screen resolution for fullscreen 
	int disp_w, disp_h;
	getScreenRes(disp_w, disp_h);
    BNOInit();
    
    while (true){

        cout << "\n\n---------------------------------\n";
        cout << "----WELCOME TO AR HEADSET 2.0----\n";
        cout << "---------------------------------\n\n";
        
        cout << "(Main Menu)\n";
        cout << "'b' - Display BNO data\n";
        cout << "'v' - Test computer vision\n";
        cout << "'x' - Exit program\n\n";

		ch = getchar();

        if (ch == 'b'){
            BNOMenu();
        }

        if (ch == 'v'){
			// RealSenseMenu();

			// Enable logging to console			
			rs::log_to_console(rs::log_severity::warn);
			// Instantiate RealSense "context" object, 
			// which connects all RS devices
			rs::context ctx;
			cout << "\nThere are " << ctx.get_device_count() << " connects RealSense devices\n";
			if(ctx.get_device_count() == 0) return EXIT_FAILURE;

			// Create R200 device pointer
			rs::device * dev = ctx.get_device(0);
	    	printf("\nUsing device 0, an %s\n", dev->get_name());
    		printf("    Serial number: %s\n", dev->get_serial());
   			printf("    Firmware version: %s\n", dev->get_firmware_version());		

			dev->enable_stream(rs::stream::depth, rs::preset::best_quality);
			dev->enable_stream(rs::stream::color, rs::preset::best_quality);
			dev->start();

			while(true){

			cout << "\nRealSense depth stream started. OPTIONS:\n\n";
			cout << "[a] Simple depth readout\n[b] Pointcloud demo\n[c] Streamlines\n[x] BACK\n";


			ch = getchar();
			if (ch == 'x') { 
				ch = 'z';
				break;
			}

			if (ch == 'b'){
				glfwInit();
				GLFWwindow * win = glfwCreateWindow(1280, 960, "librealsense tutorial #3", nullptr, nullptr);
				glfwSetCursorPosCallback(win, on_cursor_pos);
				glfwSetMouseButtonCallback(win, on_mouse_button);
				glfwMakeContextCurrent(win);

				while(!glfwWindowShouldClose(win))
				{
					// Wait for new frame data
					glfwPollEvents();
					dev->wait_for_frames();

					// Retrieve our images
					const uint16_t * depth_image = (const uint16_t *)dev->get_frame_data(rs::stream::depth);
					const uint8_t * color_image = (const uint8_t *)dev->get_frame_data(rs::stream::color);

					// Retrieve camera parameters for mapping between depth and color
					rs::intrinsics depth_intrin = dev->get_stream_intrinsics(rs::stream::depth);
					rs::extrinsics depth_to_color = dev->get_extrinsics(rs::stream::depth, rs::stream::color);
					rs::intrinsics color_intrin = dev->get_stream_intrinsics(rs::stream::color);
					float scale = dev->get_depth_scale();

					// Set up a perspective transform in a space that we can rotate by clicking and dragging the mouse
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					gluPerspective(60, (float)1280/960, 0.01f, 20.0f);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					gluLookAt(0,0,0, 0,0,1, 0,-1,0);
					glTranslatef(0,0,+0.5f);
					glRotated(pitch, 1, 0, 0);
					glRotated(yaw, 0, 1, 0);
					glTranslatef(0,0,-0.5f);

					// We will render our depth data as a set of points in 3D space
					glPointSize(2);
					glEnable(GL_DEPTH_TEST);
					glBegin(GL_POINTS);

					for(int dy=0; dy<depth_intrin.height; ++dy)
					{
						for(int dx=0; dx<depth_intrin.width; ++dx)
						{
							// Retrieve the 16-bit depth value and map it into a depth in meters
							uint16_t depth_value = depth_image[dy * depth_intrin.width + dx];
							float depth_in_meters = depth_value * scale;

							// Skip over pixels with a depth value of zero, which is used to indicate no data
							if(depth_value == 0) continue;

							// Map from pixel coordinates in the depth image to pixel coordinates in the color image
							rs::float2 depth_pixel = {(float)dx, (float)dy};
							rs::float3 depth_point = depth_intrin.deproject(depth_pixel, depth_in_meters);
							rs::float3 color_point = depth_to_color.transform(depth_point);
							rs::float2 color_pixel = color_intrin.project(color_point);

							// Use the color from the nearest color pixel, or pure white if this point falls outside the color image
							const int cx = (int)std::round(color_pixel.x), cy = (int)std::round(color_pixel.y);
							if(cx < 0 || cy < 0 || cx >= color_intrin.width || cy >= color_intrin.height)
							{
							    glColor3ub(255, 255, 255);
							}
							else
							{
							    glColor3ubv(color_image + (cy * color_intrin.width + cx) * 3);
							}

							// Emit a vertex at the 3D location of this depth pixel
							//if (depth_point.z < 1.0)
							glVertex3f(depth_point.x, depth_point.y, depth_point.z);
						
						}
					}
					glEnd();

					glfwSwapBuffers(win);
				}			

			}
			
			if (ch == 'a') {
				// Pixel area to get depth data from
				// e.g. 10*10
				int range = 10;
				// Max red dot size
				int dmax = 40;
				// Min red dot size
				int dmin = 5;
				int dscale = (dmax - dmin);

				// Initalize OpenCV fullscreen drawing
				Mat drawing;
				drawing = Mat::zeros(Size(disp_w,disp_h),CV_8UC3);
				cvNamedWindow("Depth Dot", CV_WINDOW_NORMAL);
   				cvSetWindowProperty("Depth Dot", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
				//imshow("Name", drawing);
				//waitKey(10);

				
				while (true) {
					// Get depth frame (640x480 for the R200)
					dev->wait_for_frames();
					const uint16_t * depth_image = (const uint16_t *)dev->get_frame_data(rs::stream::depth);
					rs::intrinsics depth_intrin = dev->get_stream_intrinsics(rs::stream::depth);

					float scale = dev->get_depth_scale();

					// For computing average depth across area of pixels
					float sum = 0;
					int N_pixels = 0;
					float avg = 0;
					int jmin = (depth_intrin.height/2 - range/2 - 1);
					int jmax = (depth_intrin.height/2 + range/2);
					int imin = (depth_intrin.width/2 - range/2 - 1);
					int imax = (depth_intrin.width/2 + range/2); // (THE ULTIMATE AUGMENTED REALITY IMAX EXPERIENCE)

					for(int j = jmin; j < jmax ; j++){
						for(int i = imin ; i < imax ; i++){

							uint16_t depth_val = depth_image[j * imax + i];
							// Depth in meters
							float depth = depth_val * scale;
							if (depth == 0) continue;
							sum += depth;
							N_pixels++;
				
						}
					}

					

					if (N_pixels > 0) 
						avg = sum/( (float) N_pixels );

					float radius = dmax - dscale*(avg/MAX_DEPTH);
		
					if (radius < dmin) radius = dmin; 
					

					cout << avg << endl;

					circle(drawing, Point( disp_w/2, disp_h/2 ), dmax ,Scalar(0,0,0),-1,8);
					circle(drawing, Point( disp_w/2, disp_h/2 ), radius ,Scalar(0,0,255),-1,8);
					imshow("Depth Dot", drawing);
					waitKey(3);

				};		

			}
			

			}

        }

        if (ch == 'x') break;
        
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
