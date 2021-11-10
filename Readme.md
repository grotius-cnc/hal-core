Hal_core is a lightweight hal environment.

The installed size is approx 6.8Mib

To get the latest source code just download the latest release tar.gz archive.
After the installation as discribed below, you can overwrite the /opt/hal-core directory with latest archive files and eventually you
can perform a recompile fron the /opt/hal-core top dir, requesting the make file.

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

![cyberdyne](https://user-images.githubusercontent.com/44880102/140981140-ab475f91-21ff-497b-9f54-ab2f5540c380.jpg)

Full realtime kinematic model of boston dynamics robot dog running in a kernel module.
1us base thread, 1ms servo thread.
4 Seperate kinematic chains, each representing a dog's leg.
Dynamic motion back-end by ruckigs dofs s-curve profile.
Can eventually perform an ai fann library implementation.
On the dog's upper side, robot's can be attached as a dog's node. 

![boston_cyberdyne](https://user-images.githubusercontent.com/44880102/141102184-1dfd6f2e-4bbb-4cf5-b5ba-cbbbbd562764.jpg)
