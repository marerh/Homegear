# GNU Make project makefile autogenerated by Premake
ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifndef CC
  CC = gcc
endif

ifndef CXX
  CXX = g++
endif

ifndef AR
  AR = ar
endif

ifndef RESCOMP
  ifdef WINDRES
    RESCOMP = $(WINDRES)
  else
    RESCOMP = windres
  endif
endif

ifeq ($(config),debug)
  OBJDIR     = obj/Debug/types
  TARGETDIR  = lib/Debug
  TARGET     = $(TARGETDIR)/libtypes.a
  DEFINES   += -DFORTIFY_SOURCE=2 -DDEBUG
  INCLUDES  += -IARM\ headers
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -g -std=c++11
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -LARM\ libraries -l crypto
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LIBS      += 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release)
  OBJDIR     = obj/Release/types
  TARGETDIR  = lib/Release
  TARGET     = $(TARGETDIR)/libtypes.a
  DEFINES   += -DFORTIFY_SOURCE=2 -DNDEBUG
  INCLUDES  += -IARM\ headers
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O2 -std=c++11
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -LARM\ libraries -s -l crypto
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LIBS      += 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),profiling)
  OBJDIR     = obj/Profiling/types
  TARGETDIR  = lib/Profiling
  TARGET     = $(TARGETDIR)/libtypes.a
  DEFINES   += -DFORTIFY_SOURCE=2 -DNDEBUG
  INCLUDES  += -IARM\ headers
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O2 -g -std=c++11 -pg
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -LARM\ libraries -l crypto -pg
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LIBS      += 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug_rpi)
  CC         = arm-linux-gnueabihf-gcc
  CXX        = arm-linux-gnueabihf-g++
  OBJDIR     = obj/rpi/Debug/types
  TARGETDIR  = lib/Debug
  TARGET     = $(TARGETDIR)/libtypes.a
  DEFINES   += -DFORTIFY_SOURCE=2 -DDEBUG
  INCLUDES  += -IARM\ headers
  CPPFLAGS  += -MMD -D_GLIBCXX_USE_NANOSLEEP -D_FORTIFY_SOURCE=2 -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -g -std=c++11
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -LARM\ libraries -l crypto
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LIBS      += 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release_rpi)
  CC         = arm-linux-gnueabihf-gcc
  CXX        = arm-linux-gnueabihf-g++
  OBJDIR     = obj/rpi/Release/types
  TARGETDIR  = lib/Release
  TARGET     = $(TARGETDIR)/libtypes.a
  DEFINES   += -DFORTIFY_SOURCE=2 -DNDEBUG
  INCLUDES  += -IARM\ headers
  CPPFLAGS  += -MMD -D_GLIBCXX_USE_NANOSLEEP -D_FORTIFY_SOURCE=2 -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O2 -std=c++11
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -LARM\ libraries -s -l crypto
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LIBS      += 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),profiling_rpi)
  CC         = arm-linux-gnueabihf-gcc
  CXX        = arm-linux-gnueabihf-g++
  OBJDIR     = obj/rpi/Profiling/types
  TARGETDIR  = lib/Profiling
  TARGET     = $(TARGETDIR)/libtypes.a
  DEFINES   += -DFORTIFY_SOURCE=2 -DNDEBUG
  INCLUDES  += -IARM\ headers
  CPPFLAGS  += -MMD -D_GLIBCXX_USE_NANOSLEEP -D_FORTIFY_SOURCE=2 -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O2 -g -std=c++11 -pg
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -LARM\ libraries -l crypto -pg
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LIBS      += 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJECTS := \
	$(OBJDIR)/RPCVariable.o \
	$(OBJDIR)/Packet.o \

RESOURCES := \

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

.PHONY: clean prebuild prelink

all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking types
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning types
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(GCH): $(PCH)
	@echo $(notdir $<)
ifeq (posix,$(SHELLTYPE))
	-$(SILENT) cp $< $(OBJDIR)
else
	$(SILENT) xcopy /D /Y /Q "$(subst /,\,$<)" "$(subst /,\,$(OBJDIR))" 1>nul
endif
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
endif

$(OBJDIR)/RPCVariable.o: Libraries/Types/RPCVariable.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/Packet.o: Libraries/Types/Packet.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"

-include $(OBJECTS:%.o=%.d)
