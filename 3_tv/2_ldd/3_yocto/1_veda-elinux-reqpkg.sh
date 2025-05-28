#!/bin/bash

sudo apt-get update

echo "Installing dev packages & build essentials"
sudo apt install -y libncurses5-dev vim bison flex texinfo zlibc zlib1g zlib1g-dev libglib2.0-dev gawk automake libtool cvs ctags cscope mtd-utils u-boot-tools minicom xinetd tftpd tftp nfs-kernel-server git g++ libssl-dev

sudo apt install wget diffstat unzip gcc build-essential chrpath socat cpio python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping python3-git python3-jinja2 libegl1-mesa libsdl1.2-dev pylint3 xterm python3-subunit mesa-common-dev zstd liblz4-tool

echo "Installing buildsystems in $HOME directory"
cd ~
git clone https://github.com/buildroot/buildroot.git
git clone http://git.yoctoproject.org/git/poky

