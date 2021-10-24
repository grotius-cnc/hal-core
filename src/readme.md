Install by Makefile:

    $ ./clean
    $ ./configure
    $ make -j2
    $ sudo make setuid
    
Install by script file, this is a very transparant way, the compile steps are show'n by order:
    
    $ ./clean
    $ ./make
    
To clean install:

    $ ./clean
    
If app refuses to insert kernel module try:
- This is actually the command "sudo make setuid".

        $ sudo chown $USER -R rtapi_app
        $ sudo chown $USER -R module_helper
        $ sudo chmod 777 rtapi_app
        $ sudo chmod 777 module_helper    
    
Script install example tested on the linux-pro edition (https://sourceforge.net/projects/linux-11-pro/)

Filename : script.sh
Flag : executable
Usage : sudo ./script.sh
File :

    #!/bin/bash
    
    # Update sources to get dependencies:
    cat <<EOF > /etc/apt/sources.list
    deb http://ftp.de.debian.org/debian bullseye main contrib non-free
    deb-src http://ftp.de.debian.org/debian bullseye main contrib non-free
    deb http://security.debian.org/debian-security/ bullseye-security main
    deb-src http://security.debian.org/debian-security/ bullseye-security main
    deb http://ftp.de.debian.org/debian sid main
    EOF
    apt-get update

    apt-get install -y locales
    export LC_ALL=C
    export LANGUAGE=en
    
    apt-get install git
    chmod 777 /opt

    # Install ethercat (optional for hal-core) 
    wget https://github.com/grotius-cnc/Linux-Pro/releases/download/1.0.0/ethercat-master.deb 
    dpkg -i ethercat-master.deb
    /opt/ethercat/script/init.d/ethercat restart

    # Install hal-core dependencies :
    apt-get install -y build-essential 
    apt-get install -y libudev-dev
    apt-get install -y libboost-all-dev
    apt-get install -y libreadline-dev 
    
    # Download hal-core
    git clone https://github.com/grotius-cnc/hal_core.git /opt/hal-core
    
    # Compile and install hal-core
    /opt/hal-core/./make
    
    


