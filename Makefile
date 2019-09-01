#DHCPSNP = DHCP SNOOPING
.PHONY: all clean

include ../LR/make.h
include ../LR/make.rule
include make.h

ifeq ("$(wildcard $(DHCPSNP_OBJ_DIR))","")
_DHCPSNP_OBJ_DIR	:= $(DHCPSNP_OBJ_DIR)
endif

$(DHCPSNP_OBJ_DIR)/FutureL2ds.${ObjSuffix}: $(DHCPSNP_OBJ_DIR) $(DHCPSNP_OBJ)
	$(LD) $(LD_FLAGS) -o $@  $(filter %.${ObjSuffix}, $^)

ifneq ($(MAKECMDGOALS), clean)
#sinclude $(DHCPSNP_DEP)
endif

$(DHCPSNP_DIRS):
	$(MKDIR) $(MKDIR_FLAGS) $@

$(DHCPSNP_OBJ_DIR)/%.${ObjSuffix}: $(DHCPSNP_SRC_DIR)/%.c
	$(CC) -o $@  $(DHCPSNP_CC_FLAGS) $(filter %.c, $^)

$(DHCPSNP_OBJ_DIR)/%.${ObjSuffix}.dep: $(_DHCPSNP_OBJ_DIR) $(DHCPSNP_SRC_DIR)/%.c
	@set -e ; \
	$(RM) -fr $@.tmp ; \
	$(CC_NO_ECHO) $(DHCPSNP_CC_FLAGS) -E -MM $(filter %.c, $^) > $@.tmp ; \
	sed 's,\(.*\)\.[oO][ :]*,$(DHCPSNP_OBJ_DIR)/\1.${ObjSuffix}: ,g' < $@.tmp > $@ ; \
	$(RM) -fr $@.tmp
clean:
	@echo "Cleaning dhcp snooping object files"
	$(RM) $(RM_FLAGS) $(DHCPSNP_RMS)

test:
	echo $(DHCPSNP_CC_FLAGS)
