#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = D:/ti/ccsv6/ccs_base;D:/ti/tirtos_simplelink_2_01_00_03/packages;D:/ti/tirtos_simplelink_2_01_00_03/products/bios_6_40_03_39/packages;D:/ti/tirtos_simplelink_2_01_00_03/products/uia_2_00_01_34/packages
override XDCROOT = D:/ti/xdctools_3_30_03_47_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = D:/ti/ccsv6/ccs_base;D:/ti/tirtos_simplelink_2_01_00_03/packages;D:/ti/tirtos_simplelink_2_01_00_03/products/bios_6_40_03_39/packages;D:/ti/tirtos_simplelink_2_01_00_03/products/uia_2_00_01_34/packages;D:/ti/xdctools_3_30_03_47_core/packages;..
HOSTOS = Windows
endif
