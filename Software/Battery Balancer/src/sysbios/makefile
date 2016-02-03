
XOPTS = -I"C:/ti/xdctools_3_31_01_33_core/packages/" -Dxdc_target_types__=C:/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages/ti/targets/std.h -Dxdc_target_name__=C28_large

vpath % C:/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages/ti/sysbios/
vpath %.c C:/ti/xdctools_3_31_01_33_core/packages/

CCOPTS    = -v28 -DLARGE_MODEL=1 -ml -q -mo  --program_level_compile -o3 -g --optimize_with_debug  -Dti_sysbios_family_c28_Hwi_zeroLatencyIERMask__D=0U

XDC_ROOT  = C:/ti/xdctools_3_31_01_33_core/packages/

BIOS_ROOT = C:/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages/ti/sysbios/

BIOS_DEFS =  -Dti_sysbios_BIOS_swiEnabled__D=TRUE -Dti_sysbios_BIOS_taskEnabled__D=TRUE -Dti_sysbios_BIOS_clockEnabled__D=TRUE -Dti_sysbios_BIOS_runtimeCreatesEnabled__D=TRUE -Dti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS -Dti_sysbios_knl_Swi_DISABLE_ALL_HOOKS -Dti_sysbios_knl_Task_DISABLE_ALL_HOOKS -Dti_sysbios_BIOS_smpEnabled__D=FALSE -Dti_sysbios_Build_useHwiMacros -Dti_sysbios_knl_Swi_numPriorities__D=16 -Dti_sysbios_knl_Task_deleteTerminatedTasks__D=FALSE -Dti_sysbios_knl_Task_numPriorities__D=16 -Dti_sysbios_knl_Task_checkStackFlag__D=TRUE -Dti_sysbios_knl_Clock_TICK_SOURCE=ti_sysbios_knl_Clock_TickSource_TIMER -Dti_sysbios_knl_Clock_TICK_MODE=ti_sysbios_knl_Clock_TickMode_PERIODIC -Dti_sysbios_hal_Core_delegate_getId=ti_sysbios_hal_CoreNull_getId__E -Dti_sysbios_hal_Core_delegate_interruptCore=ti_sysbios_hal_CoreNull_interruptCore__E -Dti_sysbios_hal_Core_delegate_lock=ti_sysbios_hal_CoreNull_lock__E -Dti_sysbios_hal_Core_delegate_unlock=ti_sysbios_hal_CoreNull_unlock__E -Dti_sysbios_hal_Core_numCores__D=1 -Dti_sysbios_hal_CoreNull_numCores__D=1 -Dti_sysbios_utils_Load_taskEnabled__D=TRUE -Dti_sysbios_utils_Load_swiEnabled__D=FALSE -Dti_sysbios_family_c28_Hwi_dispatcherSwiSupport__D=TRUE -Dti_sysbios_family_c28_Hwi_dispatcherTaskSupport__D=TRUE -Dti_sysbios_family_c28_Hwi_dispatcherAutoNestingSupport__D=FALSE -Dti_sysbios_family_c28_Hwi_dispatcherIrpTrackingSupport__D=TRUE -Dti_sysbios_knl_Semaphore_supportsEvents__D=FALSE -Dti_sysbios_knl_Semaphore_supportsPriority__D=TRUE

BIOS_INC  = -I"C:/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages/"

TARGET_INC = -I"C:/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages/"

INCS = $(BIOS_INC) $(TARGET_INC)  

CC = C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.9/bin/cl2000 -c $(CCOPTS) -I C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.9/include
ASM = C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.9/bin/cl2000 -c $(CCOPTS) -I C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.9/include
AR = C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.9/bin/ar2000 rq

DEL = cmd.exe /q /c del /f
CP = cmd.exe /q /c copy /Y

define RM
	$(if $(wildcard $1),$(DEL) $1,:)
endef

define ASSEMBLE
	@echo asm28L $< ...
	@$(ASM) $(BIOS_DEFS) $(XOPTS) $(INCS) $<
endef

all: sysbios.a28L

c28_Hwi_asm.obj: family/c28/Hwi_asm.s28 makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=c28_Hwi_asm.obj

c28_Hwi_disp_asm.obj: family/c28/Hwi_disp_asm.s28 makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=c28_Hwi_disp_asm.obj

c28_IntrinsicsSupport_asm.obj: family/c28/IntrinsicsSupport_asm.s28 makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=c28_IntrinsicsSupport_asm.obj

c28_TaskSupport_asm.obj: family/c28/TaskSupport_asm.s28 makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=c28_TaskSupport_asm.obj


BIOS.obj: BIOS.c family/c28/TimestampProvider.c family/c28/Hwi.c family/c28/Timer.c family/c28/TaskSupport.c knl/Clock.c knl/Idle.c knl/Intrinsics.c knl/Event.c knl/Queue.c knl/Semaphore.c knl/Swi.c knl/Swi_andn.c knl/Task.c hal/Hwi.c hal/Hwi_stack.c hal/Hwi_startup.c gates/GateHwi.c gates/GateMutex.c heaps/HeapMem.c makefile
	@-$(call RM, $@)
	@echo cl28L $< ...
	@$(CC) $(BIOS_DEFS) $(XOPTS) $(INCS) \
	$(BIOS_ROOT)BIOS.c \
	$(BIOS_ROOT)family/c28/TimestampProvider.c \
	$(BIOS_ROOT)family/c28/Hwi.c \
	$(BIOS_ROOT)family/c28/Timer.c \
	$(BIOS_ROOT)family/c28/TaskSupport.c \
	$(BIOS_ROOT)knl/Clock.c \
	$(BIOS_ROOT)knl/Idle.c \
	$(BIOS_ROOT)knl/Intrinsics.c \
	$(BIOS_ROOT)knl/Event.c \
	$(BIOS_ROOT)knl/Queue.c \
	$(BIOS_ROOT)knl/Semaphore.c \
	$(BIOS_ROOT)knl/Swi.c \
	$(BIOS_ROOT)knl/Swi_andn.c \
	$(BIOS_ROOT)knl/Task.c \
	$(BIOS_ROOT)hal/Hwi.c \
	$(BIOS_ROOT)hal/Hwi_stack.c \
	$(BIOS_ROOT)hal/Hwi_startup.c \
	$(BIOS_ROOT)gates/GateHwi.c \
	$(BIOS_ROOT)gates/GateMutex.c \
	$(BIOS_ROOT)heaps/HeapMem.c \

sysbios.a28L: BIOS.obj c28_Hwi_asm.obj c28_Hwi_disp_asm.obj c28_IntrinsicsSupport_asm.obj c28_TaskSupport_asm.obj
	@-$(call RM, $@)
	@echo ar28L $^ ...
	@$(AR) $@ $^


clean:
	@-$(call RM, sysbios.a28L)
	@-$(call RM, BIOS.obj)
	@-$(call RM, c28_Hwi_asm.obj)
	@-$(call RM, c28_Hwi_disp_asm.obj)
	@-$(call RM, c28_IntrinsicsSupport_asm.obj)
	@-$(call RM, c28_TaskSupport_asm.obj)
