You need to build the ruckig lib first:

    cd /opt/hal-core/src/hal/components/skynet/cpp_interface/libruckig/
    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make

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
