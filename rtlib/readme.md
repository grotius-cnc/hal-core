Compiled source files from /src/hal/components/* are copied to this directory /rtlib/*

Hal-core system component:
	
	hal_lib.so
    
Test component:

	test.so 
	
Thread component:	

	threads.so (module that creates thread's, for example to update a module function every 1ms)
    
Motion component:
   
   	dofs.so (s-curve motion planner module)
	libdofs.so (shared library, needed by dofs.so) 

Ethercat component:
	
	lcec.so
	lcec_conf (ethercat .xml config reader used by lcec.so)
