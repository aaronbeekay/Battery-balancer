#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.C28_large{1,0,6.4,9
#
ifeq (,$(MK_NOGENDEPS))
-include package/cfg/app_p28L.o28L.dep
package/cfg/app_p28L.o28L.dep: ;
endif

package/cfg/app_p28L.o28L: | .interfaces
package/cfg/app_p28L.o28L: package/cfg/app_p28L.c package/cfg/app_p28L.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl28L $< ...
	$(ti.targets.C28_large.rootDir)/bin/cl2000 -c  -g --optimize_with_debug -qq -pdsw225 -Dfar=  -mo -v28 -DLARGE_MODEL=1 -ml -eo.o28L -ea.s28L  --embed_inline_assembly  -Dxdc_cfg__xheader__='"xconfig_app/package/cfg/app_p28L.h"'  -Dxdc_target_name__=C28_large -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_6_4_9 -O2  $(XDCINCS) -I$(ti.targets.C28_large.rootDir)/include -fs=./package/cfg -fr=./package/cfg -fc $<
	$(MKDEP) -a $@.dep -p package/cfg -s o28L $< -C   -g --optimize_with_debug -qq -pdsw225 -Dfar=  -mo -v28 -DLARGE_MODEL=1 -ml -eo.o28L -ea.s28L  --embed_inline_assembly  -Dxdc_cfg__xheader__='"xconfig_app/package/cfg/app_p28L.h"'  -Dxdc_target_name__=C28_large -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_6_4_9 -O2  $(XDCINCS) -I$(ti.targets.C28_large.rootDir)/include -fs=./package/cfg -fr=./package/cfg
	-@$(FIXDEP) $@.dep $@.dep
	
package/cfg/app_p28L.o28L: export C_DIR=
package/cfg/app_p28L.o28L: PATH:=$(ti.targets.C28_large.rootDir)/bin/;$(PATH)
package/cfg/app_p28L.o28L: Path:=$(ti.targets.C28_large.rootDir)/bin/;$(PATH)

package/cfg/app_p28L.s28L: | .interfaces
package/cfg/app_p28L.s28L: package/cfg/app_p28L.c package/cfg/app_p28L.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl28L -n $< ...
	$(ti.targets.C28_large.rootDir)/bin/cl2000 -c -n -s --symdebug:none -g --optimize_with_debug -qq -pdsw225 -Dfar=  -v28 -DLARGE_MODEL=1 -ml -eo.o28L -ea.s28L   -Dxdc_cfg__xheader__='"xconfig_app/package/cfg/app_p28L.h"'  -Dxdc_target_name__=C28_large -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_6_4_9 -O2  $(XDCINCS) -I$(ti.targets.C28_large.rootDir)/include -fs=./package/cfg -fr=./package/cfg -fc $<
	$(MKDEP) -a $@.dep -p package/cfg -s o28L $< -C  -n -s --symdebug:none -g --optimize_with_debug -qq -pdsw225 -Dfar=  -v28 -DLARGE_MODEL=1 -ml -eo.o28L -ea.s28L   -Dxdc_cfg__xheader__='"xconfig_app/package/cfg/app_p28L.h"'  -Dxdc_target_name__=C28_large -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_6_4_9 -O2  $(XDCINCS) -I$(ti.targets.C28_large.rootDir)/include -fs=./package/cfg -fr=./package/cfg
	-@$(FIXDEP) $@.dep $@.dep
	
package/cfg/app_p28L.s28L: export C_DIR=
package/cfg/app_p28L.s28L: PATH:=$(ti.targets.C28_large.rootDir)/bin/;$(PATH)
package/cfg/app_p28L.s28L: Path:=$(ti.targets.C28_large.rootDir)/bin/;$(PATH)

clean,28L ::
	-$(RM) package/cfg/app_p28L.o28L
	-$(RM) package/cfg/app_p28L.s28L

app.p28L: package/cfg/app_p28L.o28L package/cfg/app_p28L.mak

clean::
	-$(RM) package/cfg/app_p28L.mak
