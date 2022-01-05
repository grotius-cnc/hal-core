[![grotius-cnc - hal-core](https://img.shields.io/static/v1?label=grotius-cnc&message=hal-core&color=blue&logo=github)](https://github.com/grotius-cnc/hal-core "Go to GitHub repo")
[![stars - hal-core](https://img.shields.io/github/stars/grotius-cnc/hal-core?style=social)](https://github.com/grotius-cnc/hal-core)
[![forks - hal-core](https://img.shields.io/github/forks/grotius-cnc/hal-core?style=social)](https://github.com/grotius-cnc/hal-core)
[![Github All Releases](https://img.shields.io/github/downloads/grotius-cnc/hal-core/total.svg)]()

[![GitHub release](https://img.shields.io/github/release/grotius-cnc/hal-core?include_prereleases=&sort=semver&color=blue)](https://github.com/grotius-cnc/hal-core/releases/)
[![License](https://img.shields.io/badge/License-MIT-blue)](#license)

![Your Repository's Stats](https://github-readme-stats.vercel.app/api?username=grotius-cnc&show_icons=true)

Hal_core is a lightweight cnc and robot controller in progress. It uses mature kernel technologies based on hal.
It makes advanched usage of high end c++ libraries to do advanched calculations in realtime.

Hal-core is written as a universal program where all kind of kinematic models are welcome.

Multiple machines can be controlled at the same time. Multiple gcode's can be applied to a certain machine id.

My latest work can be found in the /component/matrix/ dir.

Matrix project examples, now including parts of the gcode runner:

a 3d gcode. The machine in the back is doing a complex task !

https://user-images.githubusercontent.com/44880102/144768106-1d59e470-8c0b-46f1-806d-56ce2cb4c4f6.mp4

a pocket gcode 2000+ gcodelines

https://user-images.githubusercontent.com/44880102/144765887-f340a338-94d0-4e9b-b308-38a88b8f102a.mp4

https://user-images.githubusercontent.com/44880102/144764463-0cdf39f8-ac25-4901-a94b-2b6bf52af1aa.mp4

Skynet project examples:
- skynet cyberdyne 6 axis gantry cnc (5 axis xyzab + 1 c axis tool drill rotation)
![skynet_cyberdyne_5axiscnc](https://user-images.githubusercontent.com/44880102/141767852-eba6eab5-252b-49d7-9844-b5945201f2b1.jpg)
- novabot
![novabot](https://user-images.githubusercontent.com/44880102/141680735-aaa408bb-2462-430c-8513-e02252ae7fe7.jpg)
- eseries_ur16e
![eseries_ur16e](https://user-images.githubusercontent.com/44880102/141760557-11c9ba86-6a8c-487b-86e5-1551f3207175.jpg)
- dmu 54 fd monoblock 5 axis cnc
![5_axis_cnc](https://user-images.githubusercontent.com/44880102/141680728-e012b434-fdae-41c6-82e7-2b57b09c69e3.jpg)
- nachi mz07
![nachi](https://user-images.githubusercontent.com/44880102/141680733-f8fd7094-5430-4181-9b76-a427854e4f8f.jpg)
- kuka kr6 1- agilus
- ![kuka](https://user-images.githubusercontent.com/44880102/141680726-2ecc4cad-3c7b-43b9-bafd-078d19d8ea02.jpg)
- staubli tx90l hb
![staubli](https://user-images.githubusercontent.com/44880102/141680737-f5ba2ccb-8d7c-4e44-8ddb-1aabddfe3ee2.jpg)
- mitsubishi rv 6sdl
![mitsubishi](https://user-images.githubusercontent.com/44880102/141680731-5bf6d756-7e3f-4673-a7df-7430611fc17b.jpg)
- fanuc arcmate 100i
![fanuc](https://user-images.githubusercontent.com/44880102/141680721-7c114b41-52c8-4191-9415-7ed65770aa4c.jpg)


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
		






