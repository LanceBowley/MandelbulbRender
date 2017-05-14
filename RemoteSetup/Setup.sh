#!/bin/bash

# Install and initialize VNC server

REMOTESUDO="ubuntu"
REMOTEDESKDIR="/home/ubuntu/RemoteDesktop"

cd ~
mkdir $REMOTEDESKDIR
cd $REMOTEDESKDIR
mkdir NewImages
mkdir TempImages
mkdir Images
mkdir InboundKeys
mkdir OutboundKeys
cd ~
// chmod -R 777 /home/ubuntu
mv MandelbulberSettings $REMOTEDESKDIR
cd ~
apt-get update
cd ~
apt-get update
echo yes | apt-get install gnome-panel gnome-settings-daemon metacity nautilus gnome-terminal
echo yes | apt-get install ubuntu-desktop
echo yes | apt-get install tightvncserver
sudo -u $REMOTESUDO vncserver :1
sudo -u $REMOTESUDO echo "#!/bin/sh
export XKL_XMODMAP_DISABLE=1
unset SESSION_MANAGER
unset DBUS_SESSION_BUS_ADDRESS
[ -x /etc/vnc/xstartup ] && exec /etc/vnc/xstartup
[ -r $HOME/.Xresources ] && xrdb $HOME/.Xresources
xsetroot -solid grey
vncconfig -iconic &
gnome-panel &
gnome-settings-daemon &
metacity &
nautilus &
gnome-terminal &" > '/home/ubuntu/.vnc/xstartup'
sudo -u $REMOTESUDO vncserver -kill :1
sudo -u $REMOTESUDO vncserver :1 -geometry 1600x900

'
# Set up drivers for OpenCL

apt-get update
apt-get install gcc
sudo -u $REMOTESUDO wget http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1404/x86_64/cuda-repo-ubuntu1404_7.5-18_amd64.deb
dpkg -i cuda-repo-ubuntu1404_7.5-18_amd64.deb
apt-get update
echo yes | apt-get install cuda
sudo -u $REMOTESUDO export PATH=/usr/local/cuda/bin:$PATH
sudo -u $REMOTESUDO export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
echo yes | apt-get install opencl-headers python-pip python-dev python-numpy python-mako
sudo -u $REMOTESUDO wget https://pypi.python.org/packages/source/p/pyopencl/pyopencl-2013.1.tar.gz#md5=c506e4ec5bc56ad85bf005ec40d4783b 
sudo -u tar -vxzf pyopencl-2013.1.tar.gz 
cd pyopencl-2013.1 
echo yes | python setup.py install
apt-get update # This and down may be superfluous
apt-get upgrade -y
echo yes | apt-get install -y opencl-headers build-essential protobuf-compiler \
    libprotoc-dev libboost-all-dev libleveldb-dev hdf5-tools libhdf5-serial-dev \
    libopencv-core-dev  libopencv-highgui-dev libsnappy-dev libsnappy1 \
    libatlas-base-dev cmake libstdc++6-4.8-dbg libgoogle-glog0 libgoogle-glog-dev \
    libgflags-dev liblmdb-dev git python-pip gfortran
apt-get clean
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
'
