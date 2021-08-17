#                                                     -*-makefile-gmake-*-
#
# Makefile.inc.  Generated from Makefile.inc.in by configure. 
# on Tue Aug 17 13:49:06 EDT 2021
#


# Directories
#############

prefix=/opt/hal-core
exec_prefix=${prefix}

EMC2_HOME=/opt/hal-core
LIB_DIR=/opt/hal-core/lib

#used for install stuff
#but have them here as reference
#build system only uses EMC2_RTLIB_DIR when creating rtapi.conf
EMC2_BIN_DIR=/opt/hal-core/bin
EMC2_TCL_DIR=/opt/hal-core/tcl
EMC2_HELP_DIR=/opt/hal-core/docs/help
EMC2_RTLIB_DIR=/opt/hal-core/rtlib
EMC2_CONFIG_DIR=@EMC2_CONFIG_DIR@
EMC2_USER_CONFIG_DIR=~/emc2/configs
EMC2_SYSTEM_CONFIG_DIR=/usr/local/etc/emc2/configs
EMC2_NCFILES_DIR=/opt/hal-core/nc_files
REALTIME=/opt/hal-core/scripts/realtime
EMC2_IMAGEDIR=/opt/hal-core


MODULE_EXT=.so # module extension, used when insmod'ing

# used for building
RTDIR     = /opt/hal-core/src
RTARCH    = x86_64
ULAPI_CFLAGS = 
ULAPI_LDFLAGS = 

# Standard configure directories
# do we really need these?
bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${datarootdir}
datarootdir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${datarootdir}/info
mandir = ${datarootdir}/man
includedir = ${prefix}/include
moduledir = /opt/hal-core/lib/linuxcnc/modules
rip_moduledir = /opt/hal-core/rtlib
kernelvers = 

# i18n variables:

package = linuxcnc
localedir = /usr/share/locale
LANGUAGES = 

#still needs discussion
# do we really need these?
initd_dir = /etc/init.d
docsdir = ${prefix}/share/doc/linuxcnc
sampleconfsdir = ${prefix}/share/doc/linuxcnc/examples/sample-configs
ncfilesdir = ${prefix}/share/linuxcnc/ncfiles
tcldir = ${prefix}/lib/tcltk/linuxcnc


# /Standard configure directories

RUN_IN_PLACE = yes
RTPREFIX = uspace
RTAI = 
RTFLAGS = -DUSPACE 
KERNELDIR = 
RTFLAGS := -I. -I/opt/hal-core/src/include $(RTFLAGS) -DRTAPI -D_GNU_SOURCE -Drealtime
USE_RTLIBM = @USE_RTLIBM@
USE_LIBM = @USE_LIBM@
USE_STUBS = @USE_STUBS@
MATHINC = @MATHINC@
MATHLIB = @MATHLIB@
ULFLAGS = -Wall -g -I. -I/opt/hal-core/src/include -DULAPI -D_GNU_SOURCE -Os -DLOCALE_DIR=\"$(localedir)\" -DPACKAGE=\"$(package)\"
MODULE_EXT = .so
BUILD_SYS = uspace
CC = gcc
MANDB = /usr/bin/mandb
HIDRAW_H_USABLE = yes
TOOL_NML = no

# readline support for halcmd
READLINE_LIBS =  -lreadline

# flags for libudev
LIBUDEV_CFLAGS = 
LIBUDEV_LIBS = -ludev

# flags for glib
GLIB_CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
GLIB_LIBS = -lglib-2.0

# local flags for GTK include
GTK_VERSION = 2.24.33
GTK_CFLAGS = -pthread -I/usr/include/gtk-2.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/fribidi -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16
GTK_LIBS = -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lharfbuzz -lfontconfig -lfreetype
HAVE_GNOMEPRINT = 

# tcl tk cflags, includes and libs
TCL_DBGX=
TK_DBGX=
TCL_CFLAGS=-I/usr/include/tcl8.6 -DUSE_TCL_STUBS -I/usr/include/tcl8.6 
TCL_LIBS=-L/usr/lib/x86_64-linux-gnu -ltclstub8.6 -lX11 -lXss -lXext -lXft -lfontconfig -lfreetype -lfontconfig  -lpthread -ldl -lz  -lpthread -lm -L/usr/lib/x86_64-linux-gnu -ltk8.6 -lGL  -lXinerama 
HAVE_WORKING_BLT=@HAVE_WORKING_BLT@

AR = /usr/bin/ar
ARFLAGS = cr
CXX = g++
ifeq ($(BUILD_SYS),uspace)
CPPFLAGS = 
CXXFLAGS = -g -O2
CFLAGS = -g -O2
LDFLAGS = 
endif
RANLIB = ranlib
RANLIBFLAGS = 
MSGFMT = /usr/bin/msgfmt
XGETTEXT = /usr/bin/xgettext
LIBDL = -ldl 
YAPPS = /usr/bin/yapps2

BUILD_DOCS = no
BUILD_DOCS_PDF = 
BUILD_DOCS_HTML = 
LYX = @LYX@
PYTHON = /usr/bin/python3
TCLSH = /usr/bin/tclsh8.6
EMC2_TCL_LIB_DIR = /opt/hal-core/tcl
A2X_LATEX_ENCODING = 

HAVE_LIBMODBUS3   = yes
LIBMODBUS_LIBS    = -lmodbus
LIBMODBUS_CFLAGS  = -I/usr/include/modbus

HAVE_LIBUSB10   = yes
LIBUSB10_LIBS   = -lusb-1.0
LIBUSB10_CFLAGS = -I/usr/include/libusb-1.0

YAPPS = /usr/bin/yapps2


#
# i18n
#


#### BEGIN CONFIG ####

# To disable compilation of any particular module,
# edit the following lines replacing "m" with "n".
#
# In time, we may have some fancy dohicky to set
# these options at compile time.

# usually you need rtapi,
# so you probably don't want to change this
CONFIG_RTAPI=m

# rtapi examples
CONFIG_RTAPI_EXAMPLES_EXTINT=m
CONFIG_RTAPI_EXAMPLES_FIFO=m
CONFIG_RTAPI_EXAMPLES_SEM=m
CONFIG_RTAPI_EXAMPLES_SHMEM=m
CONFIG_RTAPI_EXAMPLES_TIMER=m

# emc motion module
CONFIG_MOTMOD=m

# HAL components
CONFIG_BLOCKS=m
CONFIG_BOSS_PLC=m
CONFIG_DEBOUNCE=m
CONFIG_ENCODER=m
CONFIG_COUNTER=m
CONFIG_ENCODER_RATIO=m
CONFIG_STEPGEN=m
CONFIG_LCD=m
CONFIG_MUX_GENERIC=m
CONFIG_MATRIX_KB=m
CONFIG_FREQGEN=m
CONFIG_PWMGEN=m
CONFIG_SIGGEN=m
CONFIG_AT_PID=m
CONFIG_PID=m
CONFIG_SUPPLY=m
CONFIG_CLASSICLADDER_RT=m
CONFIG_TIMEDELAY=m
CONFIG_SIM_ENCODER=m
CONFIG_WEIGHTED_SUM=m
CONFIG_WATCHDOG=m
CONFIG_MODMATH=m
CONFIG_STREAMER=m
CONFIG_SAMPLER=m

# HAL drivers
CONFIG_UPARPORT=m
CONFIG_HAL_PARPORT=m
CONFIG_PROBE_PARPORT=m
CONFIG_HAL_TIRO=m
CONFIG_HAL_EVOREG=m
CONFIG_HAL_MOTENC=m
CONFIG_HAL_SKELETON=m
CONFIG_HAL_SPEAKER=m
CONFIG_HAL_STG=m
CONFIG_HAL_VTI=m
CONFIG_HAL_AX521H=m
CONFIG_HAL_PPMC=m
CONFIG_PCI_8255=m
CONFIG_HOSTMOT2=m
CONFIG_OPTO_AC5=m
CONFIG_HAL_GM=m
CONFIG_HAL_BB_GPIO=m
CONFIG_HAL_PI_GPIO=m

RTAPI_APP_CFLAGS=
CONFIG_USPACE_RTAI=n
RTAI_LXRT_CFLAGS=
RTAI_LXRT_LDFLAGS=

CONFIG_USPACE_XENOMAI=n
XENOMAI_CFLAGS=
XENOMAI_LDFLAGS=

LIBTIRPC_CFLAGS=
LIBTIRPC_LIBS=
INCLUDEPY=/usr/include/python3.9
LIBPYTHON=python3.9
BOOST_PYTHON_LIBS=-lboost_python39
PYTHON_CPPFLAGS=-I/usr/include/python3.9
PYTHON_LIBS=-lcrypt -lpthread -ldl  -lutil -lm
SITEPY=/usr/lib/python3/dist-packages
YAPPS=/usr/bin/yapps2
#### END CONFIG ####

