# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

ifeq ($(origin CC), default)
  CC = clang
endif
ifeq ($(origin CXX), default)
  CXX = clang++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
INCLUDES += -I../Libraries/Cute -I../Libraries/Catch -I../Libraries -I../Projects/Entropy
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug)
TARGETDIR = ../Build/Bin/UnitTests/Debug
TARGET = $(TARGETDIR)/UnitTests
OBJDIR = ../Build/Obj/UnitTests/Debug
DEFINES += -DPROFILE -DNDEBUG -DCATCH_CPP11_OR_GREATER
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g -Wdeprecated-declarations -ftime-report
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g -std=c++17 -Wdeprecated-declarations -ftime-report
LIBS += ../Build/Bin/Entropy/Debug/libEntropy.dylib -lglfw
LDDEPS += ../Build/Bin/Entropy/Debug/libEntropy.dylib
ALL_LDFLAGS += $(LDFLAGS) -L../Libraries/GLFW/Lib -Wl,-rpath,'@loader_path/../../Entropy/Debug' -m64

else ifeq ($(config),release)
TARGETDIR = ../Build/Bin/UnitTests/Release
TARGET = $(TARGETDIR)/UnitTests
OBJDIR = ../Build/Obj/UnitTests/Release
DEFINES += -DPROFILE -DCATCH_CPP11_OR_GREATER
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -std=c++17
LIBS += ../Build/Bin/Entropy/Release/libEntropy.dylib -lglfw
LDDEPS += ../Build/Bin/Entropy/Release/libEntropy.dylib
ALL_LDFLAGS += $(LDFLAGS) -L../Libraries/GLFW/Lib -Wl,-rpath,'@loader_path/../../Entropy/Release' -m64

else
  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

OBJECTS :=

OBJECTS += $(OBJDIR)/main.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking UnitTests
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
	@echo Cleaning UnitTests
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/main.o: ../Projects/EntropyUnitTests/main.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif