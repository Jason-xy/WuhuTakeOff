#!/bin/bash -e
echo "GNU ARM Embedded Toolchain installation script for [AMD64] "
echo "Ensure that your environment is clean!"

mkdir -p ~/toolchain_tmp
rm -rf ~/toolchain_tmp

mkdir -p ~/toolchain_tmp
cd ~/toolchain_tmp

echo "Install GCC toolchain"
version="10.3-2021.10"
download_url="https://developer.arm.com/-/media/Files/downloads/gnu-rm/${version}/gcc-arm-none-eabi-${version}-x86_64-linux.tar.bz2"
wget ${download_url}
tar -jxf gcc-arm-none-eabi-${version}-x86_64-linux.tar.bz2
sudo mv gcc-arm-none-eabi-${version} /opt/arm
echo "PATH=$PATH:/opt/arm/bin" >> ~/.bashrc
source ~/.bashrc
/opt/arm/bin/arm-none-eabi-gcc --version

echo "Build ST Link"
sudo apt-get install gcc build-essential cmake libusb-1.0 libusb-1.0-0-dev libgtk-3-dev pandoc -y
git clone https://github.com/stlink-org/stlink.git
cd stlink
thread=`cat /proc/cpuinfo| grep "processor"| wc -l`
make clean
make release -j${thread}
sudo make install

echo "Grant Access to ST Link"
sudo cp -a config/udev/rules.d/* /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger

echo "Build OpenOCD For Debugging"
sudo apt-get install libtool pkg-config autoconf automake texinfo -y
git clone git://git.code.sf.net/p/openocd/code openocd
./bootstrap
./configure --prefix=/usr --enable-maintainer-mode --enable-stlink
make -j${thread}
sudo make install

echo Remove tmp file 
rm -rf ~/toolchain_tmp
