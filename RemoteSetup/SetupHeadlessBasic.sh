#!/bin/bash

# Install and initialize VNC server

REMOTESUDO='ubuntu'
LOCALDESKDIR='/home/lance/RemoteDesktop'
REMOTEDESKDIR='/home/ubuntu/RemoteDesktop'

cd ~
mkdir $REMOTEDESKDIR
cd $REMOTEDESKDIR
mkdir NewImages
mkdir TempImages
mkdir Images
mkdir InboundKeys
mkdir OutboundKeys
cd ~
chmod -R 757 $REMOTEDESKDIR
mv MandelbulberSettings $REMOTEDESKDIR
cd ~

# Install Mandelbulber in the current directory

echo yes | apt-get install libgtk2.0-dev libjpeg8-dev ocl-icd-opencl-dev ocl-icd-libopencl1
echo yes | apt-get install -f 
echo yes | apt-get install git
cd $REMOTEDESKDIR
git clone https://github.com/LanceBowley/mandelbulber-opencl-1.21-1.patch.git
cd mandelbulber-opencl-1.21-1.patch/makefiles
make all
cd ..
./install

