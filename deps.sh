#!/bin/bash

DOWNLOADS="/home/odroid/Downloads"
DESKTOP="/home/odroid/Desktop"
RS_DIR="/usr/local/include/librealsense"
GLFW_DIR="/usr/local/include/GLFW"
RSLIB_PATH="/usr/local/lib/librealsense*"
GLFWLIB_PATH="/usr/local/lib/libglfw*"
CV_DIR="/usr/local/include/opencv*"
CVLIB_PATH="/usr/local/lib/libopencv*"
EIGEN_DIR="/usr/local/include/eigen3"
BOOST_PATH="/usr/local/lib/libboost*"
BOOST_DIR="/usr/local/include/boost*"
FLANN_PATH="/usr/local/lib/libflann*"
FLANN_DIR="/usr/local/include/flann*"
VTK_PATH="/usr/local/lib/libvtk*"
VTK_DIR="/usr/local/include/vtk*"
PCL_PATH=""
clear


printf "\nINSTALLING EVERYTHING\n\n"
printf "Updating system...\n\n"
apt-get --yes update && apt-get --yes upgrade

printf "\nInstalling basic dependencies...\n\n"
apt-get --yes install cmake i2c-tools python-smbus dconf-tools doxygen pkg-config git build-essential qtcreator wget curl unzip vim

printf "\nInstalling Odroid Utility\n\n"
wget -O /usr/local/bin/odroid-utility.sh https://raw.githubusercontent.com/mdrjr/odroid-utility/master/odroid-utility.sh
chmod +x /usr/local/bin/odroid-utility.sh

printf "\nInstalling ncurses library\n\n"
apt-get --yes install libncurses5-dev libncursesw5-dev

printf "\nInstalling Python\n\n"
apt-get --yes install python-dev python-pip python3-dev python3-pip

# RealSense Library
if [ ! -d $RS_DIR ] || [ ! -e $RSLIB_PATH ]
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
	printf "\nPatching uvc video drivers...\n\n"
	./$DESKTOP/librealsense/scripts/patch-uvcvideo-16.04.simple.sh
	modprobe uvcvideo
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
apt-get --yes --force-yes install checkinstall yasm gfortran libjpeg8-dev libjasper-dev libpng12-dev libtiff4-dev libtiff5-dev libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libxine2-dev libv4l-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev qt5-default libgtk2.0-dev libtbb-dev libatlas-base-dev libfaac-dev libmp3lame-dev libtheora-dev libvorbis-dev libxvidcore-dev libopencore-amrnb-dev libopencore-amrwb-dev x264 v4l-utils  libprotobuf-dev protobuf-compiler libgoogle-glog-dev libgflags-dev libgphoto2-dev libeigen3-dev libhdf5-dev libgtk2.0-dev libcairo2-doc libgtk2.0-doc libpango1.0-doc pkg-config

if [ ! -e $CVLIB_PATH ] || [ ! -d $CV_DIR ]
then
	printf "\nInstalling OpenCV...\n\n"
	
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

printf "\nChecking PCL dependencies...\n\n"

if [ ! -d $BOOST_DIR ] || [ ! -e $BOOST_PATH ]
then
	printf "\nInstalling Boost v1.66.0..."

	if [ -d $DOWNLOADS/boost* ] || [ -e $DOWNLOADS/boost* ]
	then
		rm -rf $DOWNLOADS/boost*
	fi

	wget -O $DOWNLOADS/boost.tar.bz2 https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.bz2
	tar --bzip2 -xf $DOWNLOADS/boost.tar.bz2

	if [ -d $DESKTOP/boost ]
	then
		rm -rf $DESKTOP/boost
	fi

	mv $DOWNLOADS/boost $DESKTOP/boost
	cd $DESKTOP/boost/
	./bootstrap.sh
	./b2
	sudo cp -a $DESKTOP/boost /usr/local/include
	sudo cp -a $DESKTOP/boost/stage/lib/. /usr/local/lib
	rm -rf $DOWNLOADS/boost*
else
	printf "\nBoost is already installed.\n\n"
fi

if [ ! -d $EIGEN_DIR ]
then 
	printf "\nInstalling Eigen...\n\n"
	if [ -d $DESKTOP/eigen* ] || [ -e $DESKTOP/eigen* ]
	then 
		rm -rf $DOWNLOADS/eigen*
	fi
	git clone https://github.com/eigenteam/eigen-git-mirror.git $DESKTOP/eigen
	mkdir $DESKTOP/eigen/build
	cd $DESKTOP/eigen/build
	cmake ..
	sudo make install
else
	printf "\nEigen is already installed."
fi

if [ ! -e $FLANN_PATH ] || [ ! -d $FLANN_DIR ]
then
	printf "\nInstalling FLANN...\n\n"
	if [ -d $DESKTOP/flann ]
	then
		rm -rf $DESKTOP/flann
	fi

	git clone https://github.com/mariusmuja/flann.git $DESKTOP/flann

	mkdir $DESKTOP/flann/build
	cd $DESKTOP/flann/build
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make -j4
	sudo make -j4 install
else
	printf "\nFLANN is already installed."
fi



if [ ! -e $VTK_PATH ] || [ ! -d $VTK_DIR ]
then
	printf "\nInstalling VTK...\n\n"
	if [ -d $DESKTOP/vtk ]
	then
		rm -rf $DESKTOP/vtk
	fi

	git clone https://gitlab.kitware.com/vtk/vtk.git $DESKTOP/vtk

	mkdir $DESKTOP/vtk/build
	cd $DESKTOP/vtk/build
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make -j4
	sudo make -j4 install
else
	printf "\nVTK is already installed."
fi


if [ ! -e $PCL_PATH ] || [ ! -d $PCL_DIR ]
then
	printf "\nInstalling PCL v1.8.1...\n\n"

	if [ -d $DOWNLOADS/pcl* ] || [ -e $DOWNLOADS/pcl* ]
	then
		rm -rf $DOWNLOADS/pcl*
	fi

	if [ -d $DESKTOP/pcl ]
	then
		rm -rf $DESKTOP/pcl
	fi
	wget -O $DOWNLOADS/pcl.zip https://github.com/PointCloudLibrary/pcl/archive/pcl-1.8.1.zip
	unzip $DOWNLOADS/pcl.zip -d $DOWNLOADS/pcl
	mv $DOWNLOADS/pcl/* $DESKTOP/pcl
	rm -rf $DOWNLOADS/pcl*
	mkdir $DESKTOP/pcl/build
	cd $DESKTOP/pcl/build
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make -j4
	make -j4 install

else
	printf "\nPCL already installed."

fi

printf "\n\n----DONE!----\N\N"