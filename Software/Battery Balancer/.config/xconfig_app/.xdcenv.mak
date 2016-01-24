#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/tirtos_c2000_2_12_01_33/packages;C:/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages;C:/ti/tirtos_c2000_2_12_01_33/products/ipc_3_30_01_12/packages;C:/ti/tirtos_c2000_2_12_01_33/products/ndk_2_24_02_31/packages;C:/ti/tirtos_c2000_2_12_01_33/products/uia_2_00_02_39/packages;C:/ti/ccsv6/ccs_base;C:/Users/SEANHA~1/DOCUME~1/BUCKEY~1/CCS/BATTER~1/Software/BATTER~1/.config
override XDCROOT = C:/ti/xdctools_3_31_01_33_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/tirtos_c2000_2_12_01_33/packages;C:/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages;C:/ti/tirtos_c2000_2_12_01_33/products/ipc_3_30_01_12/packages;C:/ti/tirtos_c2000_2_12_01_33/products/ndk_2_24_02_31/packages;C:/ti/tirtos_c2000_2_12_01_33/products/uia_2_00_02_39/packages;C:/ti/ccsv6/ccs_base;C:/Users/SEANHA~1/DOCUME~1/BUCKEY~1/CCS/BATTER~1/Software/BATTER~1/.config;C:/ti/xdctools_3_31_01_33_core/packages;..
HOSTOS = Windows
endif
