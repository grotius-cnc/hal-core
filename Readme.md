
Hal_core is a lightweight hal environment.
The installed size is approx 6.8Mib

A hal environment can be used as platform to run realtime applications like:

	motion controllers 
	robots
	cnc-machines 
	parport, ethercat applications
	research and development 
	scientific projects

Packages needed for the installation (at the bottom):

https://github.com/grotius-cnc/hal_core/tree/main/src#readme

To install:

	Install hal-core:
	$ apt-get install git
        $ chmod 777 /opt
	$ git clone https://github.com/grotius-cnc/hal_core.git /opt/hal-core
	$ /opt/hal-core/./prepare
	$ /opt/hal-core/./make
	
To run:

	$ /opt/hal-core/./runtest

https://user-images.githubusercontent.com/44880102/129791198-ab705999-23ca-4004-a5f5-f0bd3357b47e.mp4
