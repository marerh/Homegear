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
  OBJDIR     = obj/Debug/cli
  TARGETDIR  = lib/Debug
  TARGET     = $(TARGETDIR)/libcli.a
  DEFINES   += -DFORTIFY_SOURCE=2 -DGCRYPT_NO_DEPRECATED -DDEBUG
  INCLUDES  += -I/usr/include/php5 -I/usr/include/php5/main -I/usr/include/php5/sapi -I/usr/include/php5/TSRM -I/usr/include/php5/Zend
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -g -Wall -std=c++11
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -Wl,-rpath=/lib/homegear -Wl,-rpath=/usr/lib/homegear
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
  OBJDIR     = obj/Release/cli
  TARGETDIR  = lib/Release
  TARGET     = $(TARGETDIR)/libcli.a
  DEFINES   += -DFORTIFY_SOURCE=2 -DGCRYPT_NO_DEPRECATED -DNDEBUG
  INCLUDES  += -I/usr/include/php5 -I/usr/include/php5/main -I/usr/include/php5/sapi -I/usr/include/php5/TSRM -I/usr/include/php5/Zend
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O2 -Wall -std=c++11
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s -Wl,-rpath=/lib/homegear -Wl,-rpath=/usr/lib/homegear
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
  OBJDIR     = obj/Profiling/cli
  TARGETDIR  = lib/Profiling
  TARGET     = $(TARGETDIR)/libcli.a
  DEFINES   += -DFORTIFY_SOURCE=2 -DGCRYPT_NO_DEPRECATED -DNDEBUG
  INCLUDES  += -I/usr/include/php5 -I/usr/include/php5/main -I/usr/include/php5/sapi -I/usr/include/php5/TSRM -I/usr/include/php5/Zend
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O2 -g -Wall -std=c++11 -pg
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -Wl,-rpath=/lib/homegear -Wl,-rpath=/usr/lib/homegear -pg
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
	$(OBJDIR)/CLIServer.o \
	$(OBJDIR)/CLIClient.o \

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
	@echo Linking cli
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
	@echo Cleaning cli
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

$(OBJDIR)/CLIServer.o: Libraries/CLI/CLIServer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/CLIClient.o: Libraries/CLI/CLIClient.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"

-include $(OBJECTS:%.o=%.d)
