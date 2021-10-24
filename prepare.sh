#!/bin/bash

apt-get install git
chmod 777 /opt

# Install ethercat (optional for hal-core) 
wget https://github.com/grotius-cnc/Linux-Pro/releases/download/1.0.0/ethercat-master.deb 
dpkg -i ethercat-master.deb
/opt/ethercat/script/init.d/ethercat restart

# Install hal-core :
apt-get install -y locales
export LC_ALL=C
export LANGUAGE=en

# Update sources to get dependencies:
cat <<EOF > /etc/apt/sources.list
deb http://ftp.de.debian.org/debian bullseye main contrib non-free
deb-src http://ftp.de.debian.org/debian bullseye main contrib non-free
deb http://security.debian.org/debian-security/ bullseye-security main
deb-src http://security.debian.org/debian-security/ bullseye-security main
deb http://ftp.de.debian.org/debian sid main
EOF
apt-get update

apt-get install -y build-essential 
apt-get install -y libudev-dev
apt-get install -y libboost-all-dev
apt-get install -y libreadline-dev 

# /opt/hal-core/./make
