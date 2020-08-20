ifeq (${KERNELRELEASE},)
# Not yet called from the kernel build system

BUILD_DIR ?= /lib/modules/`uname -r`/build

EXTRA_CFLAGS += -I$(CURDIR)
EXTRA_CFLAGS += -I$(CURDIR)/include


.PHONY: __default
__default:
	$(MAKE) -C $(BUILD_DIR) M=$(CURDIR) EXTRA_CFLAGS="$(EXTRA_CFLAGS)"

.PHONY: $(MAKECMDGOALS)
$(MAKECMDGOALS):
	$(MAKE) -C $(BUILD_DIR) M=$(CURDIR) EXTRA_CFLAGS="$(EXTRA_CFLAGS)" $@

else
# Called from the kernel build system

MODULE = mbgclock

# object files required to build the kernel module
MODULE_OBJS = mbgclock_main.o

# object files from mbglib
MBGLIB_OBJS_COMMON += pcpsdrvr.o
MBGLIB_OBJS_COMMON += mbgioctl.o
MBGLIB_OBJS_COMMON += mbgsystm.o
MBGLIB_OBJS_COMMON += identdec.o
MBGLIB_OBJS_COMMON += mbgddmsg.o
MBGLIB_OBJS_COMMON += mbgerror.o

MBGLIB_OBJS_LINUX += rsrc_lx.o

obj-m := $(MODULE).o
$(MODULE)-objs := $(MODULE_OBJS) $(MBGLIB_OBJS_COMMON) $(MBGLIB_OBJS_LINUX)

endif

# This is a hack that prevents the kernel build system from
# being called to 'install' the kernel.
.PHONY: install
install:
	@echo "Calling 'make modules_install' to install"
	$(MAKE) -C $(BUILD_DIR) M=$(CURDIR) EXTRA_CFLAGS="$(EXTRA_CFLAGS)" modules_install
