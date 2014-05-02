#
# OMNeT++/OMNEST Makefile for SkyNet
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I../mixim-sommer/src/modules/analogueModel -I../mixim-sommer/src/modules/phy -I../mixim-sommer/src/modules/mac/ieee80211p -I../mixim-sommer/src/modules/mobility -I../mixim-sommer/src/modules/messages -I../mixim-sommer/src/base/utils -I../mixim-sommer/src/base/phyLayer -I../mixim-sommer/src/modules/utility -I../mixim-sommer/src/modules -I../mixim-sommer/src/base/modules -I../mixim-sommer/src/base/messages -I../mixim-sommer/src/base/connectionManager -I../mixim-sommer/src/modules/obstacle -L../mixim-sommer/out/$$\(CONFIGNAME\)/src/modules -L../mixim-sommer/out/$$\(CONFIGNAME\)/src/base -L../mixim-sommer/out/$$\(CONFIGNAME\)/tests/testUtils -lmiximmodules -lmiximbase -lmiximtestUtils -KMIXIM_SOMMER_PROJ=../mixim-sommer
#

# Name of target to be created (-o option)
TARGET = SkyNet$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../mixim-sommer/src/modules/analogueModel \
    -I../mixim-sommer/src/modules/phy \
    -I../mixim-sommer/src/modules/mac/ieee80211p \
    -I../mixim-sommer/src/modules/mobility \
    -I../mixim-sommer/src/modules/messages \
    -I../mixim-sommer/src/base/utils \
    -I../mixim-sommer/src/base/phyLayer \
    -I../mixim-sommer/src/modules/utility \
    -I../mixim-sommer/src/modules \
    -I../mixim-sommer/src/base/modules \
    -I../mixim-sommer/src/base/messages \
    -I../mixim-sommer/src/base/connectionManager \
    -I../mixim-sommer/src/modules/obstacle \
    -I. \
    -ILogFiles \
    -ILogFiles/20131104 \
    -ILogFiles/2013BassanoTag1 \
    -Iradio \
    -Iresults

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../mixim-sommer/out/$(CONFIGNAME)/src/modules -L../mixim-sommer/out/$(CONFIGNAME)/src/base -L../mixim-sommer/out/$(CONFIGNAME)/tests/testUtils  -lmiximmodules -lmiximbase -lmiximtestUtils
LIBS += -Wl,-rpath,`abspath ../mixim-sommer/out/$(CONFIGNAME)/src/modules` -Wl,-rpath,`abspath ../mixim-sommer/out/$(CONFIGNAME)/src/base` -Wl,-rpath,`abspath ../mixim-sommer/out/$(CONFIGNAME)/tests/testUtils`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/GliderApplLayer.o \
    $O/GliderMobility.o \
    $O/Position.o \
    $O/PositionMessage.o \
    $O/Transmission.o \
    $O/radio/PgPhyLayer.o \
    $O/radio/PgRadioModel.o

# Message files
MSGFILES =

# Other makefile variables (-K)
MIXIM_SOMMER_PROJ=../mixim-sommer

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f SkyNet SkyNet.exe libSkyNet.so libSkyNet.a libSkyNet.dll libSkyNet.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f LogFiles/*_m.cc LogFiles/*_m.h
	$(Q)-rm -f LogFiles/20131104/*_m.cc LogFiles/20131104/*_m.h
	$(Q)-rm -f LogFiles/2013BassanoTag1/*_m.cc LogFiles/2013BassanoTag1/*_m.h
	$(Q)-rm -f radio/*_m.cc radio/*_m.h
	$(Q)-rm -f results/*_m.cc results/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc LogFiles/*.cc LogFiles/20131104/*.cc LogFiles/2013BassanoTag1/*.cc radio/*.cc results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/GliderApplLayer.o: GliderApplLayer.cc \
	GliderApplLayer.h \
	Transmission.h \
	$(MIXIM_SOMMER_PROJ)/src/base/messages/ApplPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseApplLayer.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseLayer.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/MiXiMDefs.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/NetwToMacControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/PassedMessage.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/SimpleAddress.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/miximkerneldefs.h
$O/GliderMobility.o: GliderMobility.cc \
	GliderMobility.h \
	Position.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseMobility.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/MiXiMDefs.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/Move.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/miximkerneldefs.h \
	$(MIXIM_SOMMER_PROJ)/src/modules/mobility/LineSegmentsMobilityBase.h
$O/Position.o: Position.cc \
	Position.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/MiXiMDefs.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/miximkerneldefs.h
$O/PositionMessage.o: PositionMessage.cc \
	Position.h \
	PositionMessage.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/MiXiMDefs.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/miximkerneldefs.h
$O/Transmission.o: Transmission.cc \
	Transmission.h
$O/radio/PgPhyLayer.o: radio/PgPhyLayer.cc \
	radio/PgPhyLayer.h \
	radio/PgRadioModel.h \
	$(MIXIM_SOMMER_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(MIXIM_SOMMER_PROJ)/src/base/connectionManager/ChannelAccess.h \
	$(MIXIM_SOMMER_PROJ)/src/base/connectionManager/NicEntry.h \
	$(MIXIM_SOMMER_PROJ)/src/base/messages/AirFrame_m.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseMobility.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/AnalogueModel.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/BasePhyLayer.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/ChannelInfo.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/ChannelState.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/DeciderToPhyInterface.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/Interpolation.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/MacToPhyInterface.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/Mapping.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/MappingBase.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/MappingUtils.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/PhyUtils.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/Signal_.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/FindModule.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/MiXiMDefs.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/Move.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/miximkerneldefs.h \
	$(MIXIM_SOMMER_PROJ)/src/modules/phy/PhyLayer.h
$O/radio/PgRadioModel.o: radio/PgRadioModel.cc \
	GliderMobility.h \
	Position.h \
	radio/PgPhyLayer.h \
	radio/PgRadioModel.h \
	$(MIXIM_SOMMER_PROJ)/src/base/connectionManager/ChannelAccess.h \
	$(MIXIM_SOMMER_PROJ)/src/base/messages/AirFrame_m.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseMobility.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(MIXIM_SOMMER_PROJ)/src/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/AnalogueModel.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/BasePhyLayer.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/ChannelInfo.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/ChannelState.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/DeciderToPhyInterface.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/Interpolation.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/MacToPhyInterface.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/Mapping.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/MappingBase.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/MappingUtils.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/PhyUtils.h \
	$(MIXIM_SOMMER_PROJ)/src/base/phyLayer/Signal_.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/FindModule.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/MiXiMDefs.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/Move.h \
	$(MIXIM_SOMMER_PROJ)/src/base/utils/miximkerneldefs.h \
	$(MIXIM_SOMMER_PROJ)/src/modules/mobility/LineSegmentsMobilityBase.h \
	$(MIXIM_SOMMER_PROJ)/src/modules/phy/PhyLayer.h

