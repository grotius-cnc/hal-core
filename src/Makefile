
# see http://www.cmcrossroads.com/ask-mr-make/6535-tracing-rule-execution-in-gnu-make
# to trace make execution of make in more detail:
# make VV=1
ifeq ("$(origin VV)", "command line")
    OLD_SHELL := $(SHELL)
    SHELL = $(warning Building $@$(if $<, (from $<))$(if $?, ($? newer)))$(OLD_SHELL)
endif

# Delete the default suffix rules
.SUFFIXES:
.PHONY: default userspace modules clean modclean depclean install python pythonclean cscope cscopeclean

# A "trivial build" is one which should not include dependency information
# either because it should be usable before dependency information can be
# generated or when it is invalid (clean, docclean) or when running as root
# when the user must guarantee in advance that everything is built
# (setuid, install)
ifeq ($(MAKECMDGOALS),)
TRIVIAL_BUILD=no
else
ifeq ($(filter-out docclean clean setuid install tags swish,$(MAKECMDGOALS)),)
TRIVIAL_BUILD=yes
else
TRIVIAL_BUILD=no
endif
endif


# Beautify output
# ---------------------------------------------------------------------------
#
# A simple variant is to prefix commands with $(Q) - that's useful
# for commands that shall be hidden in non-verbose mode.
#
#	$(Q)ln $@ :<
#
# If BUILD_VERBOSE equals 0 then the above command will be hidden.
# If BUILD_VERBOSE equals 1 then the above command is displayed.

ifeq ("$(origin V)", "command line")
  BUILD_VERBOSE = $(V)
endif
ifndef BUILD_VERBOSE
  BUILD_VERBOSE = 1
endif

ifeq ($(BUILD_VERBOSE),1)
  Q =
else
  Q = @
endif

ifeq "$(findstring s,$(filter-out --%, $(MAKEFLAGS)))" ""
ECHO=@echo
VECHO=echo
else
ECHO=@true
VECHO=true
endif

ifeq ($(BASEPWD),)
BASEPWD := $(shell pwd)
export BASEPWD
include Makefile.inc
ifeq ($(origin PYTHONPATH),undefined)
PYTHONPATH:=$(EMC2_HOME)/lib/python
else
PYTHONPATH:=$(EMC2_HOME)/lib/python:$(PYTHONPATH)
endif
export PYTHONPATH
else
include $(BASEPWD)/Makefile.inc
endif
ifeq ($(RTPREFIX),)
$(error Makefile.inc must specify RTPREFIX and other variables)
endif

cc-option = $(shell if $(CC) $(CFLAGS) $(1) -S -o /dev/null -xc /dev/null \
	     > /dev/null 2>&1; then echo "$(1)"; fi ;)
cxx-option = $(shell if $(CXX) $(CXXFLAGS) $(1) -S -o /dev/null -xc++ /dev/null \
            > /dev/null 2>&1; then echo "$(1)"; else echo "$(2)"; fi ;)

ifeq ($(origin KERNELRELEASE),undefined)
# When KERNELRELEASE is not defined, this is the userspace build.
# The "modules" target is the gateway to the kernel module build.
default: userspace modules
ifeq ($(RUN_IN_PLACE),yes)
ifneq ($(BUILD_SYS),uspace)
	@if [ -f ../bin/module_helper ]; then if ! [ `id -u` = 0 -a -O ../bin/module_helper -a -u ../bin/module_helper ]; then $(VECHO) "You now need to run 'sudo make setuid' in order to run in place."; fi; fi
else
	@if [ -f ../bin/rtapi_app ]; then if ! [ `id -u` = 0 -a -O  ../bin/rtapi_app -a -u ../bin/rtapi_app ]; then $(VECHO) "You now need to run 'sudo make setuid' in order to run in place with access to hardware."; fi; fi
endif
endif


# Print 'entering' all the time
MAKEFLAGS += w

# Create the variables with := so that subsequent += alterations keep it
# as a "substitute at assignment time" variable
TARGETS :=
PYTARGETS :=
GENERATED_MANPAGES :=

# Submakefiles from each of these directories will be included if they exist

######################## Here you can easy remove items from compiling :
SUBDIRS := \
    rtapi \
    hal/utils \
    hal \
    module_helper \

ULAPISRCS := rtapi/$(RTPREFIX)_ulapi.c

# Each item in INCLUDES is transformed into a -I directive later on
# The top directory is always included
INCLUDES := .

USERSRCS :=
PROGRAMS :=

# When used like $(call TOxxx, ...) these turn a list of source files
# into the corresponding list of object files, dependency files,
# or both.  When a source file has to be compiled with special flags,
# TOOBJSDEPS is used.  Confusingly, TOOBJSDEPS includes preprocessed source
# file names, but this is what allows 'make src.i' to produce proper
# preprocessed source when src.c needs a customized compile flag.
# See Submakefile.skel for an example.
TOOBJS = $(patsubst %.cc,objects/%.o,$(patsubst %.c,objects/%.o,$(1)))
TODEPS = $(patsubst %.cc,objects/%.d,$(patsubst %.c,objects/%.d,$(1)))
TOOBJSDEPS = $(call TOOBJS,$(1)) $(call TODEPS, $(1)) $(patsubst %.cc,%.ii,$(patsubst %.c,%.i,$(1)))

SUBMAKEFILES := $(patsubst %,%/Submakefile,$(SUBDIRS))
-include $(wildcard $(SUBMAKEFILES))

# This checks that all the things listed in USERSRCS are either C files
# or C++ files
ASSERT_EMPTY = $(if $(1), $(error "Should be empty but is not: $(1)"))
$(call ASSERT_EMPTY,$(filter-out %.c %.cc, $(USERSRCS)))

$(call TOOBJS,$(PYSRCS)) : EXTRAFLAGS += -fPIC -fno-strict-aliasing
USERSRCS += $(PYSRCS)

# Find the list of object files for each type of source file
CUSERSRCS := $(filter %.c,$(USERSRCS))
CXXUSERSRCS := $(filter %.cc,$(USERSRCS))
CUSEROBJS := $(call TOOBJS,$(CUSERSRCS))
CXXUSEROBJS += $(call TOOBJS,$(CXXUSERSRCS))

ifeq ($(TRIVIAL_BUILD),no)
# Find the dependency filenames, then include them all
DEPS := $(sort $(patsubst %.o,%.d,$(CUSEROBJS) $(CXXUSEROBJS)))
READ_DEPS = $(wildcard $(DEPS))
$(shell $(VECHO) 1>&2 Reading $(words $(READ_DEPS))/$(words $(DEPS)) dependency files)
-include $(READ_DEPS)
UNREAD_DEPS = $(filter-out $(READ_DEPS), $(DEPS))
$(shell $(VECHO) 1>&2 Done reading dependencies)
endif

# Each directory in $(INCLUDES) is passed as a -I directory when compiling.
INCLUDE := $(patsubst %,-I%, $(INCLUDES)) -I$(RTDIR)/include
INCLUDE += -I$(INCLUDEPY)
INCLUDE += $(LIBTIRPC_CFLAGS)

# Compilation options.	Perhaps some of these should come from Makefile.inc? (CXXFLAGS now does)
INTEGER_OVERFLOW_FLAGS := -fwrapv
OPT := -Os $(INTEGER_OVERFLOW_FLAGS)
DEBUG := $(DEBUG) -g -Wall
CFLAGS := $(INCLUDE) $(OPT) $(DEBUG) $(EXTRA_DEBUG) -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration $(CFLAGS) $(CPPFLAGS)
CXXFLAGS := $(INCLUDE) $(EXTRA_DEBUG) -DULAPI $(DEBUG) $(OPT) -Woverloaded-virtual $(CXXFLAGS) $(CPPFLAGS)
CXXFLAGS += $(call cxx-option, -Wno-psabi)
CXXFLAGS += $(call cxx-option, -std=gnu++11, -std=gnu++0x)

CFLAGS   += $(TOOL_NML_FLAG)
CXXFLAGS += $(TOOL_NML_FLAG)

ifeq ($(RUN_IN_PLACE),yes)
LDFLAGS := -L$(LIB_DIR) -Wl,-rpath,$(LIB_DIR) $(LIBTIRPC_LIBS) $(LDFLAGS)
else
LDFLAGS := -Wl,-rpath-link,../lib $(LIBTIRPC_LIBS) $(LDFLAGS)
endif

# Rules to make .o (object) files
$(sort $(CUSEROBJS)) : objects/%.o: %.c
	$(ECHO) Compiling $<
	@mkdir -p $(dir $@)
	@rm -f $@
	$(Q)$(CC) -c $(CFLAGS) $(EXTRAFLAGS) \
		-MP -MD -MF "${@:.o=.d}" -MT "$@" \
		$< -o $@

$(sort $(CXXUSEROBJS)) : objects/%.o: %.cc
	$(ECHO) Compiling $<
	@mkdir -p $(dir $@)
	@rm -f $@
	$(Q)$(CXX) -c $(CXXFLAGS) $(EXTRAFLAGS) \
		-MP -MD -MF "${@:.o=.d}" -MT "$@" \
		$< -o $@

Makefile: config.h
config.h: config.h.in config.status
	@./config.status -q --header=$@

INFILES = \
    ../scripts/realtime \
	../scripts/rtapi.conf Makefile.inc Makefile.modinc \

$(INFILES): %: %.in config.status
	@./config.status --file=$@

default: $(INFILES)

# And make userspace depend on $(TARGETS)
userspace: $(TARGETS)

pythonclean:
	rm -f $(PYTARGETS)
	find ../lib/python -name '*.so' -exec rm {} +
python: $(PYTARGETS)
userspace: python
clean: docclean pythonclean cscopeclean

# This is the gateway into the crazy world of "kbuild", the linux 2.6 system
# for building kernel modules.	Other kernel module build styles need to be
# accomodated here.
ifeq ($(BUILD_SYS),kbuild)

modules:
	MAKEFLAGS="$(filter-out --warn-undefined-variables,$(MAKEFLAGS))" \
	$(PYTHON) modsilent.py $(MAKE) KBUILD_EXTRA_SYMBOLS=$(moduledir)/Module.symvers -C $(KERNELDIR) SUBDIRS=`pwd` CC=$(CC) V=$(BUILD_VERBOSE) modules
	-cp Module.symvers *$(MODULE_EXT) ../rtlib/
endif

# These rules clean things up.	'modclean' cleans files generated by 'modules'
# (except that it doesn't remove the modules that were copied to rtlib)
# 'clean' cleans everything but dependency files, and 'depclean' cleans them
# too.
modclean:
	find -name '.*.cmd' -or -name '*.ko' -or -name '*.mod.c' -or -name '*.mod.o' | xargs rm -f
	-rm -rf .tmp_versions
	find . -name .tmp_versions |xargs rm -rf
	-rm -f ../rtlib/*.ko
	-rm -f ../rtlib/*.so

depclean:
	-rm -rf depends

clean: depclean modclean
	find . -name '*.o' |xargs rm -f
	-rm -rf objects
	-rm -f $(TARGETS)
	-rm -f $(GENERATED_MANPAGES)
	-rm -f ../rtlib/*.$(MODULE_EXT)
	-rm -f hal/components/conv_*.comp

pycheck-python-files:
	@echo 'Checking *.py files for python2 and python3 compatibility...'
	@for d in ./hal; do \
		filelist=$(shell mktemp) && \
		find $$d -name '*.py' -type f > $$filelist && \
		cat $$filelist | xargs /usr/bin/env python2 -m py_compile && \
		cat $$filelist | xargs /usr/bin/env python3 -m py_compile && \
		rm -f $$filelist; \
	done

pycheck-python-script:
	@echo 'Checking "text/x-python" (by MIME type) files for python2 and python3 compatibility...'
	@for d in ../tests; do \
		filelist=$(shell mktemp) && \
		find $$d -type f -exec file -F ':' -i {} \; | grep 'text/x-python' | awk -F ':' '{print $$1}' > $$filelist && \
		cat $$filelist | xargs /usr/bin/env python2 -m py_compile && \
		cat $$filelist | xargs /usr/bin/env python3 -m py_compile && \
		rm -f $$filelist; \
	done

pycheck: pycheck-python-files pycheck-python-script

# So that nothing is built as root, this rule does not depend on the touched
# files (Note that files in depends/ might be rebuilt, and there's little that
# can be done about it)
setuid:
ifeq ($(BUILD_SYS),uspace)
	chown root ../bin/rtapi_app
	chmod 4750 ../bin/rtapi_app
	chown root ../bin/module_helper
	chmod 4750 ../bin/module_helper
endif

# These rules allows a header file from this directory to be installed into
# ../include.  A pair of rules like these will exist in the Submakefile
# of each file that contains headers.

CONF=../configs
COMMON=$(CONF)/common
CONFILES=$(addsuffix /$(1), $(filter-out $(COMMON) $(CONF),\
                                         ${shell find ${CONF} -type d -print}))

endif # userspace

# KERNELRELEASE is nonempty, therefore we are building modules using the
# "kbuild" system.  $(BASEPWD) is used here, instead of relative paths, because
# that's what kbuild seems to require

EXTRA_CFLAGS := $(filter-out -ffast-math,$(RTFLAGS)) -D__MODULE__ -I$(BASEPWD)/../include -I$(BASEPWD) -I$(BASEPWD)/libnml/linklist \
	-I$(BASEPWD)/libnml/cms -I$(BASEPWD)/libnml/rcs -I$(BASEPWD)/libnml/inifile \
	-I$(BASEPWD)/libnml/os_intf -I$(BASEPWD)/libnml/nml -I$(BASEPWD)/libnml/buffer \
	-I$(BASEPWD)/libnml/posemath -I$(BASEPWD)/rtapi -I$(BASEPWD)/hal \
	-I$(BASEPWD)/emc/nml_intf -I$(BASEPWD)/emc/kinematics -I$(BASEPWD)/emc/tp -I$(BASEPWD)/emc/motion \
	-DSEQUENTIAL_SUPPORT -DHAL_SUPPORT -DDYNAMIC_PLCSIZE -DRT_SUPPORT -DOLD_TIMERS_MONOS_SUPPORT -DMODBUS_IO_MASTER \
	-fno-fast-math -fno-unsafe-math-optimizations \
	-Wframe-larger-than=2560 -Wno-declaration-after-statement \
	$(INTEGER_OVERFLOW_FLAGS)
ifneq ($(KERNELRELEASE),)
ifeq ($(RTARCH):$(RTAI):$(filter $(RTFLAGS),-msse),x86_64:3:)
EXTRA_CFLAGS += -msse
EXTRA_CFLAGS += -g
endif
endif
EXTRA_CFLAGS += -fno-builtin-sin -fno-builtin-cos -fno-builtin-sincos

ifeq "$(USE_STUBS)" "1"
MATHSTUB := rtapi/mathstubs.o
endif

ifdef SEQUENTIAL_SUPPORT
EXTRA_CFLAGS += -DSEQUENTIAL_SUPPORT
endif

# For each module, there's an addition to obj-m or obj-$(CONFIG_foo)
# plus a definition of foo-objs, which contains the full path to the
# object file(s) that the module contains.  Unfortunately, this setup pollutes
# the source directory with object files and other temporaries, but I can't
# find a way around it.

# Subdirectory:  rtapi
ifneq ($(BUILD_SYS),uspace)
obj-$(CONFIG_RTAPI) += rtapi.o
rtapi-objs := rtapi/$(RTPREFIX)_rtapi.o
endif

obj-m += hal_lib.o
hal_lib-objs := hal/hal_lib.o $(MATHSTUB)

TORTOBJS = $(foreach file,$($(patsubst %.o,%,$(1))-objs), objects/rt$(file))
ifeq ($(BUILD_SYS),uspace)
EXTRA_CFLAGS += -fPIC -Os
RTOBJS := $(sort $(foreach mod,$(obj-m),$(call TORTOBJS,$(mod))))

RTDEPS := $(sort $(RTOBJS:.o=.d))

modules: $(patsubst %.o,../rtlib/%.so,$(obj-m))
../rtlib/%.so:
	$(ECHO) Linking $@
	$(Q)ld -d -r -o objects/$*.tmp $^
	$(Q)objcopy -j .rtapi_export -O binary objects/$*.tmp objects/$*.sym
	$(Q)(echo '{ global : '; tr -s '\0' < objects/$*.sym | xargs -r0 printf '%s;\n' | grep .; echo 'local : * ; };') > objects/$*.ver
	$(Q)$(CC) -shared -Bsymbolic $(LDFLAGS) -Wl,--version-script,objects/$*.ver -o $@ $^ -lm

RTFLAGS += -fno-strict-aliasing -fwrapv

# Rules to make .o (object) files
$(sort $(RTOBJS)) : objects/rt%.o : %.c
	$(ECHO) Compiling realtime $<
	@rm -f $@
	@mkdir -p $(dir $@)
	$(Q)$(CC) -c $(OPT) $(DEBUG) $(EXTRA_DEBUG) -DRTAPI \
		$(EXTRA_CFLAGS) \
		-MP -MD -MF "${@:.o=.d}" -MT "$@" \
		$< -o $@
endif

ifeq ($(BUILD_SYS),normal)
modules: $(patsubst %,../rtlib/%,$(obj-m))
RTOBJS := $(sort $(foreach mod,$(obj-m),$(call TORTOBJS,$(mod))))
RTDEPS := $(sort $(patsubst objects/%.o,depends/%.d, $(RTOBJS)))

# Rules to make .o (object) files
$(sort $(RTOBJS)) : objects/rt%.o : %.c
	$(ECHO) Compiling realtime $<
	@rm -f $@
	@mkdir -p $(dir $@)
	$(Q)$(CC) -c -DRTAPI -nostdinc -isystem $(shell $(CC) -print-file-name=include) -I$(KERNELDIR)/include $(EXTRA_CFLAGS) \
		-MP -MD -MF "${@:.o=.d}" -MT "$@" \
		$< -o $@
endif

ifneq "$(filter normal uspace,$(BUILD_SYS))" ""
ifneq "$(BUILD_SYS)" "uspace"
../rtlib/rtapi$(MODULE_EXT): $(addprefix objects/rt,$(rtapi-objs))
endif
../rtlib/scope_rt$(MODULE_EXT): $(addprefix objects/rt,$(scope_rt-objs))
../rtlib/hal_lib$(MODULE_EXT): $(addprefix objects/rt,$(hal_lib-objs))

ifeq ($(TRIVIAL_BUILD),no)
READ_RTDEPS = $(wildcard $(RTDEPS))
$(shell $(VECHO) 1>&2 Reading $(words $(READ_RTDEPS))/$(words $(RTDEPS)) realtime dependency files)
-include $(READ_RTDEPS)
$(shell $(VECHO) 1>&2 Done reading realtime dependencies)
endif
endif

# Phony so that it is always rebuilt when requested, not because it
# shouldn't exist as a file
.PHONY: tags
tags:
	ctags-exuberant \
		--extra=+fq \
		--exclude=depends --exclude=objects --exclude=.mod.c \
		'--langmap=make:+(Submakefile),make:+(Makefile.inc),c:+.comp' \
		-I EXPORT_SYMBOL+,RTAPI_MP_INT+,RTAPI_MP_LONG+,RTAPI_MP_STRING+ \
		-I RTAPI_MP_ARRAY_INT+,RTAPI_MP_ARRAY_LONG+,RTAPI_MP_ARRAY_STRING+ \
		-I MODULE_AUTHOR+,MODULE_DESCRIPTION+,MODULE_LICENSE+ \
		-R . ../tcl ../scripts ../share/axis/tcl
	rm -f TAGS
	find . -type f -name '*.[ch]' -printf '%P\0' |xargs -0 etags -l c --append
	find . -type f -name '*.cc' -printf '%P\0' |xargs -0 etags -l c++ --append
	find . -type f -name '*.hh' -printf '%P\0' |xargs -0 etags -l c++ --append

etags:
	etags	--extra=+fq \
		--exclude=depends --exclude=objects --exclude=.mod.c \
		'--langmap=make:+(Submakefile),make:+(Makefile.inc),c:+.comp' \
		-I EXPORT_SYMBOL+,RTAPI_MP_INT+,RTAPI_MP_LONG+,RTAPI_MP_STRING+ \
		-I RTAPI_MP_ARRAY_INT+,RTAPI_MP_ARRAY_LONG+,RTAPI_MP_ARRAY_STRING+ \
		-I MODULE_AUTHOR+,MODULE_DESCRIPTION+,MODULE_LICENSE+ \
		-R . ../tcl ../scripts ../share/axis/tcl
	find . -type f -name '*.[ch]' |xargs etags --language-force=C --append
	find . -type f -name '*.cc' |xargs etags --language-force=C++ --append
	find . -type f -name '*.hh' |xargs etags --language-force=C++ --append

# When you depend on objects/var-ZZZ you are depending on the contents of the
# variable ZZZ, which is assumed to depend on a Makefile, a Submakefile, or
# Makefile.inc
objects/var-%: Makefile $(wildcard $(SUBMAKEFILES)) Makefile.inc
	@mkdir -p $(dir $@)
	@echo $($*) > $@.tmp
	@sh move-if-change $@.tmp $@

../lib/%.so: ../lib/%.so.0
	ln -sf $(notdir $<) $@

cscope:
	cscope -Rb

cscopeclean:
	bash -c 'for f in `find ./ -name "cscope.*out"`;do rm $$f;done'
# vim:ts=8:sts=8:sw=8:noet:
