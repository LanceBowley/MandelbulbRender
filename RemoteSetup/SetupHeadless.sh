#!/bin/bash

# Install and initialize VNC server

REMOTESUDO='ubuntu'
LOCALDESKDIR='/home/lance/RemoteDesktop'
REMOTEDESKDIR='/home/ubuntu/RemoteDesktop'

cd ~
mkdir $REMOTEDESKDIR
cd $REMOTEDESKDIR
mkdir LeftImages
mkdir RightImages
mkdir NewImages
mkdir TempImages
mkdir Images
mkdir LeftKeys
mkdir RightKeys
mkdir OutboundKeys
cd ~
chmod -R 777 $REMOTEDESKDIR
mv MandelbulberSettings $REMOTEDESKDIR
cd ~

# Set up drivers for OpenCL

echo yes | apt-get update
echo yes | apt-get install gcc
# sudo -u $REMOTESUDO wget http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1404/x86_64/cuda-repo-ubuntu1404_7.5-18_amd64.deb
# dpkg -i cuda-repo-ubuntu1404_7.5-18_amd64.deb
# apt-get update
# echo yes | apt-get install cuda OLD ORDER
# sudo -u $REMOTESUDO export PATH=/usr/local/cuda/bin:$PATH
# sudo -u $REMOTESUDO export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
# https://github.com/brotchie/torch-ubuntu-gpu-ec2-install
wget http://developer.download.nvidia.com/compute/cuda/7_0/Prod/local_installers/rpmdeb/cuda-repo-ubuntu1404-7-0-local_7.0-28_amd64.deb
sudo dpkg -i cuda-repo-ubuntu1404-7-0-local_7.0-28_amd64.deb
sudo apt-get update
sudo apt-get install -y linux-image-extra-virtual
sudo apt-get install -y linux-source linux-headers-3.13.0-53-generic
sudo apt-get install -y cuda
sudo reboot # Closes it down :(

# cat >> ~/.bashrc <<END
# export PATH=/usr/local/cuda-7.0/bin/:\$PATH
# export LD_LIBRARY_PATH=/usr/local/cuda-7.0/lib64/:\$LD_LIBRARY_PATH
# END
# source ~/.bashrc
# echo yes | apt-get install opencl-headers python-pip python-dev python-numpy python-mako
# sudo -u $REMOTESUDO wget https://pypi.python.org/packages/source/p/pyopencl/pyopencl-2013.1.tar.gz#md5=c506e4ec5bc56ad85bf005ec40d4783b
# sudo -u tar -vxzf pyopencl-2013.1.tar.gz
# cd pyopencl-2013.1
# echo yes | python setup.py install
# echo yes | apt-get update # This and down may be superfluous
# apt-get upgrade -y
# echo yes | apt-get install -y opencl-headers build-essential protobuf-compiler \
#     libprotoc-dev libboost-all-dev libleveldb-dev hdf5-tools libhdf5-serial-dev \
#     libopencv-core-dev  libopencv-highgui-dev libsnappy-dev libsnappy1 \
#     libatlas-base-dev cmake libstdc++6-4.8-dbg libgoogle-glog0 libgoogle-glog-dev \
#     libgflags-dev liblmdb-dev git python-pip gfortran
# echo yes | apt-get clean
# apt-get install -y linux-image-extra-`uname -r` linux-headers-`uname -r` linux-image-`uname -r`
#
# # Install Mandelbulber in the current directory
#
# echo yes | apt-get install libgtk2.0-dev libjpeg8-dev ocl-icd-opencl-dev ocl-icd-libopencl1
# echo yes | apt-get install -f
# echo yes | apt-get install git
# cd $REMOTEDESKDIR
# git clone https://github.com/LanceBowley/mandelbulber-opencl-1.21-1.patch.git
# cd mandelbulber-opencl-1.21-1.patch/makefiles
# make all
# cd ..
# ./install
# cd src
# chmod 777 Exec.sh