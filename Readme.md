[![grotius-cnc - hal-core](https://img.shields.io/static/v1?label=grotius-cnc&message=hal-core&color=blue&logo=github)](https://github.com/grotius-cnc/hal-core "Go to GitHub repo")
[![stars - hal-core](https://img.shields.io/github/stars/grotius-cnc/hal-core?style=social)](https://github.com/grotius-cnc/hal-core)
[![forks - hal-core](https://img.shields.io/github/forks/grotius-cnc/hal-core?style=social)](https://github.com/grotius-cnc/hal-core)
[![Github All Releases](https://img.shields.io/github/downloads/grotius-cnc/hal-core/total.svg)]()

[![GitHub release](https://img.shields.io/github/release/grotius-cnc/hal-core?include_prereleases=&sort=semver&color=blue)](https://github.com/grotius-cnc/hal-core/releases/)
[![License](https://img.shields.io/badge/License-MIT-blue)](#license)

Hal_core is a lightweight cnc and robot controller in progress. It uses mature kernel technologies based on hal.
It makes advanched usage of high end c++ libraries to do advanched calculations in realtime.

Hal-core is written as a universal program where all kind of kinematic models are welcome.

Multiple machines can be controlled at the same time. Every machine may load a gcode file.

Matrix project examples: https://github.com/grotius-cnc/hal-core/blob/main/src/hal/components/matrix/readme.md

3d Printing a model of 30.000 gcode lines, used strategy opencascade linewire :

![new_gcode_parser](https://user-images.githubusercontent.com/44880102/150699178-48e660f0-0369-4772-9843-d500ff9c4fbc.jpg)

https://user-images.githubusercontent.com/44880102/150698778-b65a6f5a-653b-49ad-9368-a46931bd0c28.mp4

Skynet project examples: https://github.com/grotius-cnc/hal-core/blob/main/src/hal/components/skynet/readme.md
![novabot](https://user-images.githubusercontent.com/44880102/141680735-aaa408bb-2462-430c-8513-e02252ae7fe7.jpg)

To get the latest source code just download the latest release tar.gz archive.
This archives are pre-compiled for linux amd64.
To recompile yourself:
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
		





