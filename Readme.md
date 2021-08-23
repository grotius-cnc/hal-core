
To install from a git clone :

	Install hal-core:
	$ git clone https://github.com/grotius-cnc/hal_core.git hal-core
	$ cp -rf hal-core /opt
	$ cd /opt/hal-core/src
	$ chmod +x configure
	$ ./make
	$ cd /opt/hal-core/
	$ chmod +x runtest
	$ ./runtest
	
	Install ethercat component:
	$ cd /opt/hal-core/src/hal/components/ethercat/
	$ chmod +x make 
	$ chmod +x runtest
	$ ./make
	$ ./runtest
	
	Install threads component:
	$ cd /opt/hal-core/src/hal/components/threads/
	$ chmod +x make 
	$ ./make
	
	Install dofs component:
	$ cd /opt/hal-core/src/hal/components/dofs/hal
	$ chmod +x make
	$ ./make

Installation and runtest preview:

https://user-images.githubusercontent.com/44880102/129791198-ab705999-23ca-4004-a5f5-f0bd3357b47e.mp4

Hal_core is a stripped down hal environment.
The installed size is approx 6.8Mib

This repository is compiled (amd64) in the following dir : /opt/hal_core/
Download the hal-core.deb release here : https://github.com/grotius-cnc/hal_core/releases/tag/1.0

A hal environment can be used as platform to run realtime applications like 

	motion controllers 
	robots
	cnc-machines 
	parport, ethercat applications
	research and development 
	scientific projects

Install guide:

https://github.com/grotius-cnc/hal_core/tree/main/src

To startup the hal environment:

    $ cd scripts
    $ . ./rip-environment
    
To test if hal is working:

    $ cd bin 
    $ ./halcmd show
    
![hal_working](https://user-images.githubusercontent.com/44880102/129553575-bca46124-055e-47be-980e-ba4062991ffc.jpg)


To compile and install custom components. (realtime kernel modules) visit : 

https://github.com/grotius-cnc/hal_core/tree/main/src/hal/components

A runtest to load hal and insert components, visit :

https://github.com/grotius-cnc/hal_core/blob/main/start_hal.sh


    
