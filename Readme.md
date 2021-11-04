Hal_core is a lightweight hal environment.

The installed size is approx 6.8Mib

A hal environment can be used as platform to run realtime applications like:

		motion controllers 
		robots
		cnc-machines 
		parport, ethercat applications
		research and development 
		scientific projects
		building control systems
		
Included hardware drivers:	

		beckhoff, rpi4 ethercat compatible
		mesa ethernet compatible
		parport compatible
	   
Hal-core can interact with high end software packages like "qt", "kdl kinematics", "opencascade cad", "ruckig trajectory", "hal view", "cam" etc.
Therefore consider to install these software packages before installing hal-core. 

Hal-Core install including subprogram "ethercat bus, qt, kdl, opencascade, halview, cam" :

		$ wget https://raw.githubusercontent.com/grotius-cnc/hal-core/main/install_guide
		$ chmod +x install_guide
		$ sudo ./install_guide
		
		# Set user permissions /opt dir.
		sudo chmod -R -f 777 /opt

		# Change owner /opt dir
		sudo chown -R $USER /opt

		# Change group /opt dir
		sudo chown -R $USER:$USER /opt
	
Run / test hal-core in terminal :

		$ /opt/hal-core/./runtest

Run / test hal-core with halview :

		$ /opt/hal-core/./runtest_halview
		
A example of the hal-core hal-opencascade component.
![fanuc_tig_welding](https://user-images.githubusercontent.com/44880102/140246982-4a3ec9e3-b9f8-4ef0-8b9e-9f2046071318.jpg)
