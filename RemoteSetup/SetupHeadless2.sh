#!/bin/bash

REMOTESUDO='ubuntu'
LOCALDESKDIR='/home/lance/RemoteDesktop'
REMOTEDESKDIR='/home/ubuntu/RemoteDesktop'

chmod -R 777 $REMOTEDESKDIR

cat >> ~/.bashrc <<END
export PATH=/usr/local/cuda-7.0/bin/:\$PATH
export LD_LIBRARY_PATH=/usr/local/cuda-7.0/lib64/:\$LD_LIBRARY_PATH
END
source ~/.bashrc
echo yes | apt-get install opencl-headers python-pip python-dev python-numpy python-mako
sudo -u $REMOTESUDO wget https://pypi.python.org/packages/source/p/pyopencl/pyopencl-2013.1.tar.gz#md5=c506e4ec5bc56ad85bf005ec40d4783b
sudo -u tar -vxzf pyopencl-2013.1.tar.gz
cd pyopencl-2013.1
echo yes | python setup.py install
echo yes | apt-get update # This and down may be superfluous
apt-get upgrade -y
echo yes | apt-get install -y opencl-headers build-essential protobuf-compiler \
    libprotoc-dev libboost-all-dev libleveldb-dev hdf5-tools libhdf5-serial-dev \
    libopencv-core-dev  libopencv-highgui-dev libsnappy-dev libsnappy1 \
    libatlas-base-dev cmake libstdc++6-4.8-dbg libgoogle-glog0 libgoogle-glog-dev \
    libgflags-dev liblmdb-dev git python-pip gfortran
echo yes | apt-get clean
apt-get install -y linux-image-extra-`uname -r` linux-headers-`uname -r` linux-image-`uname -r`

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
cd src
chmod 777 Exec.sh