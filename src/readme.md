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
    
Pre requirments:

    sudo apt-get install git

Linuxcnc old libs from buster repository:

    wget https://github.com/grotius-cnc/Linux-Pro/releases/download/1.0.0/libreadline5_5.2+dfsg-3+b13_amd64.deb
    wget https://github.com/grotius-cnc/Linux-Pro/releases/download/1.0.0/libreadline-gplv2-dev_5.2+dfsg-3+b13_amd64.deb
    dpkg -i libreadline5_5.2+dfsg-3+b13_amd64.deb
    dpkg -i libreadline-gplv2-dev_5.2+dfsg-3+b13_amd64.deb

Update sources to get dependencies:

    cat <<EOF > /etc/apt/sources.list
    deb http://ftp.de.debian.org/debian bullseye main contrib non-free
    deb-src http://ftp.de.debian.org/debian bullseye main contrib non-free
    deb http://security.debian.org/debian-security/ bullseye-security main
    deb-src http://security.debian.org/debian-security/ bullseye-security main
    EOF
    apt-get update

Install dependencies from bullseye and sid repository, not all of this is needed anymore, we can filter out dep's in future.

    sudo apt-get -y install install libmodbus-dev libusb-1.0 libglib2.0-dev yapps2 libreadline-gplv2-dev tcl8.6-dev tclx8.4 tk8.6-dev libboost-python-dev



