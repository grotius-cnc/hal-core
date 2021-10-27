
Hal_core is a lightweight hal environment.
The installed size is approx 6.8Mib

A hal environment can be used as platform to run realtime applications like:

	motion controllers 
	robots
	cnc-machines 
	parport, ethercat applications
	research and development 
	scientific projects
	   
Hal-core can interact with high end software packages like qt, kdl kinematics, opencascade cad.
Therefore consider to install these software packages before installing hal-core. 

To install:

	$ sudo apt-get install git
	
	# Set user permissions to the /opt dir.
        $ sudo chmod 777 /opt
	
	# If you don't have installed qt, kdl kinematics, opencascade cad :
	$ wget https://github.com/grotius-cnc/Linux-Pro/releases/download/1.0.0/qt-creator.deb
	$ wget https://github.com/grotius-cnc/Linux-Pro/releases/download/1.0.0/kdl-kinematics.deb
	$ wget https://github.com/grotius-cnc/Linux-Pro/releases/download/1.0.0/opencascade.deb
	# Todo. create a updated opencascade.deb installing the freeimage drivers to preview pictures with opencascade.
	$ sudo dpkg -i qt-creator.deb kdl-kinematics.deb opencascade.deb
	
	# Install hal-core
	$ git clone https://github.com/grotius-cnc/hal_core.git /opt/hal-core
	$ sudo chmod +x /opt/hal-core/prepare
	$ sudo /opt/hal-core/./prepare
	$ sudo /opt/hal-core/./make
	
To run hal-core:

	$ /opt/hal-core/./runtest

https://user-images.githubusercontent.com/44880102/129791198-ab705999-23ca-4004-a5f5-f0bd3357b47e.mp4
