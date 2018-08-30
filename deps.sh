#!/bin/bash

DOWNLOADS="/home/odroid/Downloads"
DESKTOP="/home/odroid/Desktop"
RS_DIR="/usr/local/include/librealsense"
GLFW_DIR="/usr/local/include/GLFW"
RSLIB_PATH="/usr/local/lib/librealsense*"
GLFWLIB_PATH="/usr/local/lib/libglfw*"
CV_DIR="/usr/local/include/opencv*"
CVLIB_PATH="/usr/local/lib/libopencv*"

clear

: '
printf "\nINSTALLING EVERYTHING\n\n"
printf "Updating system...\n\n"
apt-get --yes update && apt-get --yes upgrade

printf "\nInstalling basic dependencies...\n\n"
apt-get --yes install cmake i2c-tools dconf-tools doxygen pkg-config git build-essential qtcreator wget curl unzip vim

printf "\nInstalling Odroid Utility\n\n"
wget -O /usr/local/bin/odroid-utility.sh https://raw.githubusercontent.com/mdrjr/odroid-utility/master/odroid-utility.sh
chmod +x /usr/local/bin/odroid-utility.sh

printf "\nInstalling ncurses library\n\n"
apt-get --yes libncurses5-dev libncursesw5-dev

printf "\nInstalling Python\n\n"
apt-get --yes install python-dev python-pip python3-dev python3-pip
'
# RealSense Library
if [ ! -d $RS_DIR ] || [ ! -d $GLFW_DIR ] || [ ! -e $RSLIB_PATH ] || [ ! -e $GLFWLIB_PATH ]
then
	printf "\nRealSense library not found. Installing now...\n\n"
	apt-get install libusb-1.0-0-dev libglfw3-dev
	
	if [ -d $DOWNLOADS/librealsense* ] || [ -e $DOWNLOADS/librealsense* ]
	then
		rm -rf $DOWNLOADS/librealsense*
	fi

	wget -O $DOWNLOADS/librealsense.zip https://github.com/IntelRealSense/librealsense/archive/v1.12.1.zip
	
	if [ -d $DESKTOP/librealsense ]
	then
		rm -rf $DESKTOP/librealsense
	fi
	unzip $DOWNLOADS/librealsense.zip -d $DOWNLOADS/librealsense
	mv $DOWNLOADS/librealsense/librealsense-1* $DESKTOP/librealsense
	rm -rf $DOWNLOADS/librealsense	
	mkdir $DESKTOP/librealsense/build
	cd $DESKTOP/librealsense/build
	cmake .. -DBUILD_EXAMPLES:BOOL=true
	make && make install
	printf "\nRS example programs have been installed to /usr/local/bin\n\n"
else
	printf "\nRealSense library is already installed.\n\n"
fi

# RTIMULib
if [ ! -d /home/odroid/Desktop/rtimulib ] 
then
	printf "\nDownloading RTIMULib to desktop...\n\n"
	git clone https://github.com/richardstechnotes/RTIMULib2.git $DESKTOP/rtimulib
else 
	printf "\nRTIMULib already found.\n\n"
fi

# OpenGl
printf "\nInstalling OpenGL\n\n"
apt-get --yes install libx11-dev xorg-dev libglu1-mesa-dev freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev


# OpenCV
printf "\nInstalling OpenCV dependencies...\n\n"
apt-get --yes --force-yes install checkinstall yasm gfortran libjpeg8-dev libjasper-dev libpng12-dev libtiff4-dev libtiff5-dev libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libxine2-dev libv4l-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev qt5-default libgtk2.0-dev libtbb-dev libatlas-base-dev libfaac-dev libmp3lame-dev libtheora-dev libvorbis-dev libxvidcore-dev libopencore-amrnb-dev libopencore-amrwb-dev x264 v4l-utils  libprotobuf-dev protobuf-compiler libgoogle-glog-dev libgflags-dev libgphoto2-dev libeigen3-dev libhdf5-dev 

if [ ! -e $CVLIB_PATH ] || [ ! -d $CV_DIR ]
then
	printf "\nInstalling OpenCV ...\n\n"
	
	if [ -d $DESKTOP/opencv ]
	then
		rm -rf $DESKTOP/opencv
	fi

	git clone https://github.com/opencv/opencv.git $DESKTOP/opencv

	if [ -d $DESKTOP/opencv/build ]
	then
		rm -rf $DESKTOP/opencv/build 
	fi

	mkdir $DESKTOP/opencv/build

	cd $DESKTOP/opencv/build
	cmake -D WITH_TBB=ON -D WITH_OPENMP=ON -D WITH_IPP=ON -D CMAKE_BUILD_TYPE=RELEASE -D BUILD_EXAMPLES=OFF -D WITH_NVCUVID=ON -D WITH_CUDA=ON -D BUILD_DOCS=OFF -D BUILD_PERF_TESTS=OFF -D BUILD_TESTS=OFF -D WITH_CSTRIPES=ON -D WITH_OPENCL=ON -D WITH_OPENGL=ON -D CMAKE_INSTALL_PREFIX=/usr/local/ ..
	make -j4 && make install

else
	printf "\nOpenCV is already installed\n\n"
fi

printf "\n\nDONE. Consider rebooting.\n\n"

