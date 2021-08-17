In this area libhalcore.so is present.

Source files of libhalcore.so :

	hal_core/src/hal/hal_lib.c
	hal_core/src/hal/hal.h
	hal_core/src/hal/hal_priv.h

libhalcore.so is used by the gcc make command, for example linking the library:

    -lhalcore
    
    
    
A gcc implementation example can be found :
https://github.com/grotius-cnc/hal_core/blob/main/src/hal/components/test/compile_test.sh
    
When writing hal components in c or c++ user can use functions from the halcore library.
More info at 
	
	hal_core/src/hal/hal_lib.c
