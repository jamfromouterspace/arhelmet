# AR HELMET v0.2 #

This version of the Concordia Augmented Reality Headset allows for basic functionalities in a C++ environment (v0.1 included basic demos in Python). In other words, this code does the following:

* Sensor integration with C++ libraries
* Automatic installation of important libraries
* IMU data readout
* AR depth demo
* Pointcloud demo
* Rudimentary 3D scanning using PCL

## Important Notes:

* If you're starting from scratch (fresh Ubuntu installation), setup might take all day.
* [Ubuntu 16.04 with kernel version 4 for ODROID XU4](https://wiki.odroid.com/odroid-xu4/os_images/linux/ubuntu_4.9/ubuntu_4.9) is required. Librealsense legaacy does not support 18.04.
* Make sure you clone this repo into a folder called `arhelmet` (or just blindly follow the code below)
* Libraries are installed to `/usr/local/share/lib`
* Header files are installed to `/usr/local/share/include`
* BNO055 calibration was done using RTIMULib. It should already be calibrated, but if you're having issues follow the instructions in `rtimulib/Calibration.pdf`. Essentially, you have to build the calibration program and follow the steps, and it'll output a calibration file. **You may need to install octave**. And I suggest you use the terminal version, not the GUI.

## Setup:
The `deps.sh` script will install system dependencies and the following libraries: 

* [Intel RealSense (legacy)](https://github.com/IntelRealSense/librealsense/tree/v1.12.1)
* [RTIMULib2](https://github.com/RTIMULib/RTIMULib2)
* [ncurses](https://www.gnu.org/software/ncurses/)
* [OpenGL](https://www.opengl.org/)
* [OpenCV](https://opencv.org/)
* [PointCloud Library (PCL)](http://www.pointclouds.org)

PCL has several [dependencies](http://www.pointclouds.org/documentation/tutorials/compiling_pcl_posix.php#dependencies), so the script will also install:

* Boost
* Eigen (linear algebra library)
* FLANN
* VTK (for pcl visualization)

**This takes a long time (1-3 hours).**

*NOTE: The AR2 code has only been tested on an Odroid XU4.*

To install, run the following commands in the terminal:

```
sudo apt-get update
sudo apt-get install git
git clone https://github.com/closetothe/arhelmet.git ~/Desktop/arhelmet
cd AR2
sudo -s
chmod +x deps.sh
chmod 755 deps.sh
./deps.sh
```


This version of AR Headset allows for basic functionalities in a C++ environment
(V1 included basic demos in Python)

## Hardware:
* ODROID XU4
* BNO055 IMU sensor 
* Intel RealSense R200
* 720p Multitouch display

## Software: 
[Ubuntu 16.04 MATE with kernel v4.9 for ODROID XU4](https://wiki.odroid.com/odroid-xu4/os_images/linux/ubuntu_4.9/ubuntu_4.9).
 

## Currently working:
      - Pre-calibrated BNO055  
      - Simple menu system to display data stream from BNO055  
        (including acceleration, gyro, euler, etc.)  
      - Pointcloud demo
      - Depth demo


## Planned:  
      - SLAM integration
      - 3D object detection  
      - Irrotational flow calculation on scanned 2D object  
      - Streamlines in AR  
