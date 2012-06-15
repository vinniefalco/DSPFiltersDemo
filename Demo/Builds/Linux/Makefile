# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(TARGET_ARCH),)
  TARGET_ARCH := -march=native
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_8068DBC5=1" -I /usr/include -I /usr/include/freetype2 -I ../Jucer/JuceLibraryCode -I ../../../DSPFilters -I ../../../JUCEAmalgam/include
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_8068DBC5=1" -I /usr/include -I /usr/include/freetype2 -I ../Jucer/JuceLibraryCode -I ../../../DSPFilters -I ../../../JUCEAmalgam/include
  TARGET := DSPFiltersDemo
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_8068DBC5=1" -I /usr/include -I /usr/include/freetype2 -I ../Jucer/JuceLibraryCode -I ../../../DSPFilters -I ../../../JUCEAmalgam/include
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_8068DBC5=1" -I /usr/include -I /usr/include/freetype2 -I ../Jucer/JuceLibraryCode -I ../../../DSPFilters -I ../../../JUCEAmalgam/include
  TARGET := DSPFiltersDemo
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/demo_core_d23b1af4.o \
  $(OBJDIR)/demo_gui_5ba1611c.o \
  $(OBJDIR)/dsp_filters_5334acb2.o \
  $(OBJDIR)/juce_audio_basics_amalgam_878967fc.o \
  $(OBJDIR)/juce_audio_devices_amalgam_8f6e72aa.o \
  $(OBJDIR)/juce_audio_formats_amalgam_a45e06a9.o \
  $(OBJDIR)/juce_audio_processors_amalgam_d7b3f798.o \
  $(OBJDIR)/juce_audio_utils_amalgam_576e8b3e.o \
  $(OBJDIR)/juce_core_amalgam_ad1deb8d.o \
  $(OBJDIR)/juce_data_structures_amalgam_2540afcb.o \
  $(OBJDIR)/juce_events_amalgam_3b85cf07.o \
  $(OBJDIR)/juce_graphics_amalgam_6feed679.o \
  $(OBJDIR)/juce_gui_basics_amalgam_4c508d77.o \
  $(OBJDIR)/juce_gui_extra_amalgam_1ad2b742.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking DSPFiltersDemo
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning DSPFiltersDemo
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

$(OBJDIR)/demo_core_d23b1af4.o: ../../modules/demo_core/demo_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling demo_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/demo_gui_5ba1611c.o: ../../modules/demo_gui/demo_gui.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling demo_gui.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dsp_filters_5334acb2.o: ../../../DSPFilters/modules/dsp_filters/dsp_filters.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dsp_filters.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_amalgam_878967fc.o: ../../../JUCEAmalgam/include/juce_audio_basics_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_amalgam_8f6e72aa.o: ../../../JUCEAmalgam/include/juce_audio_devices_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_amalgam_a45e06a9.o: ../../../JUCEAmalgam/include/juce_audio_formats_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_amalgam_d7b3f798.o: ../../../JUCEAmalgam/include/juce_audio_processors_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_amalgam_576e8b3e.o: ../../../JUCEAmalgam/include/juce_audio_utils_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_amalgam_ad1deb8d.o: ../../../JUCEAmalgam/include/juce_core_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_amalgam_2540afcb.o: ../../../JUCEAmalgam/include/juce_data_structures_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_amalgam_3b85cf07.o: ../../../JUCEAmalgam/include/juce_events_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_amalgam_6feed679.o: ../../../JUCEAmalgam/include/juce_graphics_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_amalgam_4c508d77.o: ../../../JUCEAmalgam/include/juce_gui_basics_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_amalgam_1ad2b742.o: ../../../JUCEAmalgam/include/juce_gui_extra_amalgam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra_amalgam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
