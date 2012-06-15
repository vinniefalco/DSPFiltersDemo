/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#define JUCE_AMALGAMATED_INCLUDE 1
#if !defined(__APPLE__) || defined(__OBJC__)

/*** Start of inlined file: juce_audio_utils.cpp ***/
#if defined (__JUCE_AUDIO_UTILS_JUCEHEADER__) && ! JUCE_AMALGAMATED_INCLUDE
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
	already included any other headers - just put it inside a file on its own, possibly with your config
	flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
	header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.
#include "AppConfig.h"


/*** Start of inlined file: juce_BasicNativeHeaders.h ***/
#ifndef __JUCE_BASICNATIVEHEADERS_JUCEHEADER__
#define __JUCE_BASICNATIVEHEADERS_JUCEHEADER__


/*** Start of inlined file: juce_TargetPlatform.h ***/
#ifndef __JUCE_TARGETPLATFORM_JUCEHEADER__
#define __JUCE_TARGETPLATFORM_JUCEHEADER__

/*  This file figures out which platform is being built, and defines some macros
	that the rest of the code can use for OS-specific compilation.

	Macros that will be set here are:

	- One of JUCE_WINDOWS, JUCE_MAC JUCE_LINUX, JUCE_IOS, JUCE_ANDROID, etc.
	- Either JUCE_32BIT or JUCE_64BIT, depending on the architecture.
	- Either JUCE_LITTLE_ENDIAN or JUCE_BIG_ENDIAN.
	- Either JUCE_INTEL or JUCE_PPC
	- Either JUCE_GCC or JUCE_MSVC
*/

#if (defined (_WIN32) || defined (_WIN64))
  #define       JUCE_WIN32 1
  #define       JUCE_WINDOWS 1
#elif defined (JUCE_ANDROID)
  #undef        JUCE_ANDROID
  #define       JUCE_ANDROID 1
#elif defined (LINUX) || defined (__linux__)
  #define     JUCE_LINUX 1
#elif defined (__APPLE_CPP__) || defined(__APPLE_CC__)
  #define Point CarbonDummyPointName // (workaround to avoid definition of "Point" by old Carbon headers)
  #define Component CarbonDummyCompName
  #include <CoreFoundation/CoreFoundation.h> // (needed to find out what platform we're using)
  #undef Point
  #undef Component

  #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
	#define     JUCE_IPHONE 1
	#define     JUCE_IOS 1
  #else
	#define     JUCE_MAC 1
  #endif
#else
  #error "Unknown platform!"
#endif

#if JUCE_WINDOWS
  #ifdef _MSC_VER
	#ifdef _WIN64
	  #define JUCE_64BIT 1
	#else
	  #define JUCE_32BIT 1
	#endif
  #endif

  #ifdef _DEBUG
	#define JUCE_DEBUG 1
  #endif

  #ifdef __MINGW32__
	#define JUCE_MINGW 1
  #endif

  /** If defined, this indicates that the processor is little-endian. */
  #define JUCE_LITTLE_ENDIAN 1

  #define JUCE_INTEL 1
#endif

#if JUCE_MAC || JUCE_IOS

  #if defined (DEBUG) || defined (_DEBUG) || ! (defined (NDEBUG) || defined (_NDEBUG))
	#define JUCE_DEBUG 1
  #endif

  #if ! (defined (DEBUG) || defined (_DEBUG) || defined (NDEBUG) || defined (_NDEBUG))
	#warning "Neither NDEBUG or DEBUG has been defined - you should set one of these to make it clear whether this is a release build,"
  #endif

  #ifdef __LITTLE_ENDIAN__
	#define JUCE_LITTLE_ENDIAN 1
  #else
	#define JUCE_BIG_ENDIAN 1
  #endif
#endif

#if JUCE_MAC

  #if defined (__ppc__) || defined (__ppc64__)
	#define JUCE_PPC 1
  #else
	#define JUCE_INTEL 1
  #endif

  #ifdef __LP64__
	#define JUCE_64BIT 1
  #else
	#define JUCE_32BIT 1
  #endif

  #if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_4
	#error "Building for OSX 10.3 is no longer supported!"
  #endif

  #ifndef MAC_OS_X_VERSION_10_5
	#error "To build with 10.4 compatibility, use a 10.5 or 10.6 SDK and set the deployment target to 10.4"
  #endif

#endif

#if JUCE_LINUX || JUCE_ANDROID

  #ifdef _DEBUG
	#define JUCE_DEBUG 1
  #endif

  // Allow override for big-endian Linux platforms
  #if defined (__LITTLE_ENDIAN__) || ! defined (JUCE_BIG_ENDIAN)
	#define JUCE_LITTLE_ENDIAN 1
	#undef JUCE_BIG_ENDIAN
  #else
	#undef JUCE_LITTLE_ENDIAN
	#define JUCE_BIG_ENDIAN 1
  #endif

  #if defined (__LP64__) || defined (_LP64)
	#define JUCE_64BIT 1
  #else
	#define JUCE_32BIT 1
  #endif

  #if __MMX__ || __SSE__ || __amd64__
	#define JUCE_INTEL 1
  #endif
#endif

// Compiler type macros.

#ifdef __GNUC__
  #define JUCE_GCC 1
#elif defined (_MSC_VER)
  #define JUCE_MSVC 1

  #if _MSC_VER < 1500
	#define JUCE_VC8_OR_EARLIER 1

	#if _MSC_VER < 1400
	  #define JUCE_VC7_OR_EARLIER 1

	  #if _MSC_VER < 1300
		#warning "MSVC 6.0 is no longer supported!"
	  #endif
	#endif
  #endif

  #if JUCE_64BIT || ! JUCE_VC7_OR_EARLIER
	#define JUCE_USE_INTRINSICS 1
  #endif
#else
  #error unknown compiler
#endif

#endif   // __JUCE_TARGETPLATFORM_JUCEHEADER__

/*** End of inlined file: juce_TargetPlatform.h ***/

#undef T

#if JUCE_MAC || JUCE_IOS

 #if JUCE_IOS
  #import <Foundation/Foundation.h>
  #import <UIKit/UIKit.h>
  #import <CoreData/CoreData.h>
  #import <MobileCoreServices/MobileCoreServices.h>
  #include <sys/fcntl.h>
 #else
  #define Point CarbonDummyPointName
  #define Component CarbonDummyCompName
  #import <Cocoa/Cocoa.h>
  #import <CoreAudio/HostTime.h>
  #undef Point
  #undef Component
  #include <sys/dir.h>
 #endif

 #include <sys/socket.h>
 #include <sys/sysctl.h>
 #include <sys/stat.h>
 #include <sys/param.h>
 #include <sys/mount.h>
 #include <sys/utsname.h>
 #include <sys/mman.h>
 #include <fnmatch.h>
 #include <utime.h>
 #include <dlfcn.h>
 #include <ifaddrs.h>
 #include <net/if_dl.h>
 #include <mach/mach_time.h>
 #include <mach-o/dyld.h>

#elif JUCE_WINDOWS
 #if JUCE_MSVC
  #ifndef _CPPRTTI
   #error "You're compiling without RTTI enabled! This is needed for a lot of JUCE classes, please update your compiler settings!"
  #endif

  #ifndef _CPPUNWIND
   #error "You're compiling without exceptions enabled! This is needed for a lot of JUCE classes, please update your compiler settings!"
  #endif

  #pragma warning (push)
  #pragma warning (disable : 4100 4201 4514 4312 4995)
 #endif

 #define STRICT 1
 #define WIN32_LEAN_AND_MEAN 1
 #define _WIN32_WINNT 0x0600
 #define _UNICODE 1
 #define UNICODE 1
 #ifndef _WIN32_IE
  #define _WIN32_IE 0x0400
 #endif

 #include <windows.h>
 #include <shellapi.h>
 #include <tchar.h>
 #include <stddef.h>
 #include <ctime>
 #include <wininet.h>
 #include <nb30.h>
 #include <iphlpapi.h>
 #include <mapi.h>
 #include <float.h>
 #include <process.h>
 #include <shlobj.h>
 #include <shlwapi.h>
 #include <mmsystem.h>

 #if JUCE_MINGW
  #include <basetyps.h>
 #else
  #include <crtdbg.h>
  #include <comutil.h>
 #endif

 #undef PACKED

 #if JUCE_MSVC
  #pragma warning (pop)
  #pragma warning (4: 4511 4512 4100 /*4365*/)  // (enable some warnings that are turned off in VC8)
 #endif

 #if JUCE_MSVC && ! JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment (lib, "kernel32.lib")
  #pragma comment (lib, "user32.lib")
  #pragma comment (lib, "shell32.lib")
  #pragma comment (lib, "wininet.lib")
  #pragma comment (lib, "advapi32.lib")
  #pragma comment (lib, "ws2_32.lib")
  #pragma comment (lib, "version.lib")
  #pragma comment (lib, "shlwapi.lib")
  #pragma comment (lib, "winmm.lib")

  #ifdef _NATIVE_WCHAR_T_DEFINED
   #ifdef _DEBUG
	#pragma comment (lib, "comsuppwd.lib")
   #else
	#pragma comment (lib, "comsuppw.lib")
   #endif
  #else
   #ifdef _DEBUG
	#pragma comment (lib, "comsuppd.lib")
   #else
	#pragma comment (lib, "comsupp.lib")
   #endif
  #endif
 #endif

 /* Used with DynamicLibrary to simplify importing functions

	functionName: function to import
	localFunctionName: name you want to use to actually call it (must be different)
	returnType: the return type
	object: the DynamicLibrary to use
	params: list of params (bracketed)
 */
 #define JUCE_DLL_FUNCTION(functionName, localFunctionName, returnType, object, params) \
	typedef returnType (WINAPI *type##localFunctionName) params; \
	type##localFunctionName localFunctionName = (type##localFunctionName)object.getFunction (#functionName);

#elif JUCE_LINUX
 #include <sched.h>
 #include <pthread.h>
 #include <sys/time.h>
 #include <errno.h>
 #include <sys/stat.h>
 #include <sys/dir.h>
 #include <sys/ptrace.h>
 #include <sys/vfs.h>
 #include <sys/wait.h>
 #include <sys/mman.h>
 #include <fnmatch.h>
 #include <utime.h>
 #include <pwd.h>
 #include <fcntl.h>
 #include <dlfcn.h>
 #include <netdb.h>
 #include <arpa/inet.h>
 #include <netinet/in.h>
 #include <sys/types.h>
 #include <sys/ioctl.h>
 #include <sys/socket.h>
 #include <net/if.h>
 #include <sys/sysinfo.h>
 #include <sys/file.h>
 #include <sys/prctl.h>
 #include <signal.h>
 #include <stddef.h>

#elif JUCE_ANDROID
 #include <jni.h>
 #include <pthread.h>
 #include <sched.h>
 #include <sys/time.h>
 #include <utime.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <dlfcn.h>
 #include <sys/stat.h>
 #include <sys/statfs.h>
 #include <sys/ptrace.h>
 #include <sys/sysinfo.h>
 #include <sys/mman.h>
 #include <pwd.h>
 #include <dirent.h>
 #include <fnmatch.h>
 #include <sys/wait.h>
#endif

// Need to clear various moronic redefinitions made by system headers..
#undef max
#undef min
#undef direct
#undef check

#endif   // __JUCE_BASICNATIVEHEADERS_JUCEHEADER__

/*** End of inlined file: juce_BasicNativeHeaders.h ***/

#include "juce_audio_utils_amalgam.h"

namespace juce
{

// START_AUTOINCLUDE gui/*.cpp, players/*.cpp

/*** Start of inlined file: juce_AudioDeviceSelectorComponent.cpp ***/
class SimpleDeviceManagerInputLevelMeter  : public Component,
											public Timer
{
public:
	SimpleDeviceManagerInputLevelMeter (AudioDeviceManager* const manager_)
		: manager (manager_),
		  level (0)
	{
		startTimer (50);
		manager->enableInputLevelMeasurement (true);
	}

	~SimpleDeviceManagerInputLevelMeter()
	{
		manager->enableInputLevelMeasurement (false);
	}

	void timerCallback()
	{
		const float newLevel = (float) manager->getCurrentInputLevel();

		if (std::abs (level - newLevel) > 0.005f)
		{
			level = newLevel;
			repaint();
		}
	}

	void paint (Graphics& g)
	{
		getLookAndFeel().drawLevelMeter (g, getWidth(), getHeight(),
										 (float) exp (log (level) / 3.0)); // (add a bit of a skew to make the level more obvious)
	}

private:
	AudioDeviceManager* const manager;
	float level;

	JUCE_DECLARE_NON_COPYABLE (SimpleDeviceManagerInputLevelMeter);
};

class AudioDeviceSelectorComponent::MidiInputSelectorComponentListBox  : public ListBox,
																		 public ListBoxModel
{
public:

	MidiInputSelectorComponentListBox (AudioDeviceManager& deviceManager_,
									   const String& noItemsMessage_,
									   const int minNumber_,
									   const int maxNumber_)
		: ListBox (String::empty, nullptr),
		  deviceManager (deviceManager_),
		  noItemsMessage (noItemsMessage_),
		  minNumber (minNumber_),
		  maxNumber (maxNumber_)
	{
		items = MidiInput::getDevices();

		setModel (this);
		setOutlineThickness (1);
	}

	int getNumRows()
	{
		return items.size();
	}

	void paintListBoxItem (int row,
						   Graphics& g,
						   int width, int height,
						   bool rowIsSelected)
	{
		if (isPositiveAndBelow (row, items.size()))
		{
			if (rowIsSelected)
				g.fillAll (findColour (TextEditor::highlightColourId)
							   .withMultipliedAlpha (0.3f));

			const String item (items [row]);
			bool enabled = deviceManager.isMidiInputEnabled (item);

			const int x = getTickX();
			const float tickW = height * 0.75f;

			getLookAndFeel().drawTickBox (g, *this, x - tickW, (height - tickW) / 2, tickW, tickW,
										  enabled, true, true, false);

			g.setFont (height * 0.6f);
			g.setColour (findColour (ListBox::textColourId, true).withMultipliedAlpha (enabled ? 1.0f : 0.6f));
			g.drawText (item, x, 0, width - x - 2, height, Justification::centredLeft, true);
		}
	}

	void listBoxItemClicked (int row, const MouseEvent& e)
	{
		selectRow (row);

		if (e.x < getTickX())
			flipEnablement (row);
	}

	void listBoxItemDoubleClicked (int row, const MouseEvent&)
	{
		flipEnablement (row);
	}

	void returnKeyPressed (int row)
	{
		flipEnablement (row);
	}

	void paint (Graphics& g)
	{
		ListBox::paint (g);

		if (items.size() == 0)
		{
			g.setColour (Colours::grey);
			g.setFont (13.0f);
			g.drawText (noItemsMessage,
						0, 0, getWidth(), getHeight() / 2,
						Justification::centred, true);
		}
	}

	int getBestHeight (const int preferredHeight)
	{
		const int extra = getOutlineThickness() * 2;

		return jmax (getRowHeight() * 2 + extra,
					 jmin (getRowHeight() * getNumRows() + extra,
						   preferredHeight));
	}

private:

	AudioDeviceManager& deviceManager;
	const String noItemsMessage;
	StringArray items;
	int minNumber, maxNumber;

	void flipEnablement (const int row)
	{
		if (isPositiveAndBelow (row, items.size()))
		{
			const String item (items [row]);
			deviceManager.setMidiInputEnabled (item, ! deviceManager.isMidiInputEnabled (item));
		}
	}

	int getTickX() const
	{
		return getRowHeight() + 5;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiInputSelectorComponentListBox);
};

struct AudioDeviceSetupDetails
{
	AudioDeviceManager* manager;
	int minNumInputChannels, maxNumInputChannels;
	int minNumOutputChannels, maxNumOutputChannels;
	bool useStereoPairs;
};

class AudioDeviceSettingsPanel : public Component,
								 public ChangeListener,
								 public ComboBoxListener,  // (can't use ComboBox::Listener due to idiotic VC2005 bug)
								 public ButtonListener
{
public:
	AudioDeviceSettingsPanel (AudioIODeviceType* type_,
							  AudioDeviceSetupDetails& setup_,
							  const bool hideAdvancedOptionsWithButton)
		: type (type_),
		  setup (setup_)
	{
		if (hideAdvancedOptionsWithButton)
		{
			addAndMakeVisible (showAdvancedSettingsButton = new TextButton (TRANS("Show advanced settings...")));
			showAdvancedSettingsButton->addListener (this);
		}

		type->scanForDevices();

		setup.manager->addChangeListener (this);
		updateAllControls();
	}

	~AudioDeviceSettingsPanel()
	{
		setup.manager->removeChangeListener (this);
	}

	void resized()
	{
		const int lx = proportionOfWidth (0.35f);
		const int w = proportionOfWidth (0.4f);
		const int h = 24;
		const int space = 6;
		const int dh = h + space;
		int y = 0;

		if (outputDeviceDropDown != nullptr)
		{
			outputDeviceDropDown->setBounds (lx, y, w, h);

			if (testButton != nullptr)
				testButton->setBounds (proportionOfWidth (0.77f),
									   outputDeviceDropDown->getY(),
									   proportionOfWidth (0.18f),
									   h);
			y += dh;
		}

		if (inputDeviceDropDown != nullptr)
		{
			inputDeviceDropDown->setBounds (lx, y, w, h);

			inputLevelMeter->setBounds (proportionOfWidth (0.77f),
										inputDeviceDropDown->getY(),
										proportionOfWidth (0.18f),
										h);
			y += dh;
		}

		const int maxBoxHeight = 100;//(getHeight() - y - dh * 2) / numBoxes;

		if (outputChanList != nullptr)
		{
			const int bh = outputChanList->getBestHeight (maxBoxHeight);
			outputChanList->setBounds (lx, y, proportionOfWidth (0.55f), bh);
			y += bh + space;
		}

		if (inputChanList != nullptr)
		{
			const int bh = inputChanList->getBestHeight (maxBoxHeight);
			inputChanList->setBounds (lx, y, proportionOfWidth (0.55f), bh);
			y += bh + space;
		}

		y += space * 2;

		if (showAdvancedSettingsButton != nullptr)
		{
			showAdvancedSettingsButton->changeWidthToFitText (h);
			showAdvancedSettingsButton->setTopLeftPosition (lx, y);
		}

		if (sampleRateDropDown != nullptr)
		{
			sampleRateDropDown->setVisible (showAdvancedSettingsButton == nullptr
											 || ! showAdvancedSettingsButton->isVisible());

			sampleRateDropDown->setBounds (lx, y, w, h);
			y += dh;
		}

		if (bufferSizeDropDown != nullptr)
		{
			bufferSizeDropDown->setVisible (showAdvancedSettingsButton == nullptr
											 || ! showAdvancedSettingsButton->isVisible());
			bufferSizeDropDown->setBounds (lx, y, w, h);
			y += dh;
		}

		if (showUIButton != nullptr)
		{
			showUIButton->setVisible (showAdvancedSettingsButton == nullptr
										|| ! showAdvancedSettingsButton->isVisible());
			showUIButton->changeWidthToFitText (h);
			showUIButton->setTopLeftPosition (lx, y);
		}
	}

	void comboBoxChanged (ComboBox* comboBoxThatHasChanged)
	{
		if (comboBoxThatHasChanged == nullptr)
			return;

		AudioDeviceManager::AudioDeviceSetup config;
		setup.manager->getAudioDeviceSetup (config);
		String error;

		if (comboBoxThatHasChanged == outputDeviceDropDown
			  || comboBoxThatHasChanged == inputDeviceDropDown)
		{
			if (outputDeviceDropDown != nullptr)
				config.outputDeviceName = outputDeviceDropDown->getSelectedId() < 0 ? String::empty
																					: outputDeviceDropDown->getText();

			if (inputDeviceDropDown != nullptr)
				config.inputDeviceName = inputDeviceDropDown->getSelectedId() < 0 ? String::empty
																				  : inputDeviceDropDown->getText();

			if (! type->hasSeparateInputsAndOutputs())
				config.inputDeviceName = config.outputDeviceName;

			if (comboBoxThatHasChanged == inputDeviceDropDown)
				config.useDefaultInputChannels = true;
			else
				config.useDefaultOutputChannels = true;

			error = setup.manager->setAudioDeviceSetup (config, true);

			showCorrectDeviceName (inputDeviceDropDown, true);
			showCorrectDeviceName (outputDeviceDropDown, false);

			updateControlPanelButton();
			resized();
		}
		else if (comboBoxThatHasChanged == sampleRateDropDown)
		{
			if (sampleRateDropDown->getSelectedId() > 0)
			{
				config.sampleRate = sampleRateDropDown->getSelectedId();
				error = setup.manager->setAudioDeviceSetup (config, true);
			}
		}
		else if (comboBoxThatHasChanged == bufferSizeDropDown)
		{
			if (bufferSizeDropDown->getSelectedId() > 0)
			{
				config.bufferSize = bufferSizeDropDown->getSelectedId();
				error = setup.manager->setAudioDeviceSetup (config, true);
			}
		}

		if (error.isNotEmpty())
		{
			AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
											  "Error when trying to open audio device!",
											  error);
		}
	}

	bool showDeviceControlPanel()
	{
		AudioIODevice* const device = setup.manager->getCurrentAudioDevice();

		if (device == nullptr)
			return false;

		Component modalWindow (String::empty);
		modalWindow.setOpaque (true);
		modalWindow.addToDesktop (0);
		modalWindow.enterModalState();

		return device->showControlPanel();
	}

	void buttonClicked (Button* button)
	{
		if (button == showAdvancedSettingsButton)
		{
			showAdvancedSettingsButton->setVisible (false);
			resized();
		}
		else if (button == showUIButton)
		{
			if (showDeviceControlPanel())
			{
				setup.manager->closeAudioDevice();
				setup.manager->restartLastAudioDevice();
				getTopLevelComponent()->toFront (true);
			}
		}
		else if (button == testButton && testButton != nullptr)
		{
			setup.manager->playTestSound();
		}
	}

	void updateAllControls()
	{
		updateOutputsComboBox();
		updateInputsComboBox();

		updateControlPanelButton();

		AudioIODevice* const currentDevice = setup.manager->getCurrentAudioDevice();

		if (currentDevice != nullptr)
		{
			if (setup.maxNumOutputChannels > 0
				 && setup.minNumOutputChannels < setup.manager->getCurrentAudioDevice()->getOutputChannelNames().size())
			{
				if (outputChanList == nullptr)
				{
					addAndMakeVisible (outputChanList
						= new ChannelSelectorListBox (setup, ChannelSelectorListBox::audioOutputType,
													  TRANS ("(no audio output channels found)")));
					outputChanLabel = new Label (String::empty, TRANS ("active output channels:"));
					outputChanLabel->attachToComponent (outputChanList, true);
				}

				outputChanList->refresh();
			}
			else
			{
				outputChanLabel = nullptr;
				outputChanList = nullptr;
			}

			if (setup.maxNumInputChannels > 0
				 && setup.minNumInputChannels < setup.manager->getCurrentAudioDevice()->getInputChannelNames().size())
			{
				if (inputChanList == nullptr)
				{
					addAndMakeVisible (inputChanList
						= new ChannelSelectorListBox (setup, ChannelSelectorListBox::audioInputType,
													  TRANS ("(no audio input channels found)")));
					inputChanLabel = new Label (String::empty, TRANS ("active input channels:"));
					inputChanLabel->attachToComponent (inputChanList, true);
				}

				inputChanList->refresh();
			}
			else
			{
				inputChanLabel = nullptr;
				inputChanList = nullptr;
			}

			updateSampleRateComboBox (currentDevice);
			updateBufferSizeComboBox (currentDevice);
		}
		else
		{
			jassert (setup.manager->getCurrentAudioDevice() == nullptr); // not the correct device type!

			sampleRateLabel = nullptr;
			bufferSizeLabel = nullptr;
			sampleRateDropDown = nullptr;
			bufferSizeDropDown = nullptr;

			if (outputDeviceDropDown != nullptr)
				outputDeviceDropDown->setSelectedId (-1, true);

			if (inputDeviceDropDown != nullptr)
				inputDeviceDropDown->setSelectedId (-1, true);
		}

		resized();
		setSize (getWidth(), getLowestY() + 4);
	}

	void changeListenerCallback (ChangeBroadcaster*)
	{
		updateAllControls();
	}

private:
	AudioIODeviceType* const type;
	const AudioDeviceSetupDetails setup;

	ScopedPointer<ComboBox> outputDeviceDropDown, inputDeviceDropDown, sampleRateDropDown, bufferSizeDropDown;
	ScopedPointer<Label> outputDeviceLabel, inputDeviceLabel, sampleRateLabel, bufferSizeLabel, inputChanLabel, outputChanLabel;
	ScopedPointer<TextButton> testButton;
	ScopedPointer<Component> inputLevelMeter;
	ScopedPointer<TextButton> showUIButton, showAdvancedSettingsButton;

	void showCorrectDeviceName (ComboBox* const box, const bool isInput)
	{
		if (box != nullptr)
		{
			AudioIODevice* const currentDevice = setup.manager->getCurrentAudioDevice();

			const int index = type->getIndexOfDevice (currentDevice, isInput);

			box->setSelectedId (index + 1, true);

			if (testButton != nullptr && ! isInput)
				testButton->setEnabled (index >= 0);
		}
	}

	void addNamesToDeviceBox (ComboBox& combo, bool isInputs)
	{
		const StringArray devs (type->getDeviceNames (isInputs));

		combo.clear (true);

		for (int i = 0; i < devs.size(); ++i)
			combo.addItem (devs[i], i + 1);

		combo.addItem (TRANS("<< none >>"), -1);
		combo.setSelectedId (-1, true);
	}

	int getLowestY() const
	{
		int y = 0;

		for (int i = getNumChildComponents(); --i >= 0;)
			y = jmax (y, getChildComponent (i)->getBottom());

		return y;
	}

	void updateControlPanelButton()
	{
		AudioIODevice* const currentDevice = setup.manager->getCurrentAudioDevice();
		showUIButton = nullptr;

		if (currentDevice != nullptr && currentDevice->hasControlPanel())
		{
			addAndMakeVisible (showUIButton = new TextButton (TRANS ("show this device's control panel"),
															  TRANS ("opens the device's own control panel")));
			showUIButton->addListener (this);
		}

		resized();
	}

	void updateOutputsComboBox()
	{
		if (setup.maxNumOutputChannels > 0 || ! type->hasSeparateInputsAndOutputs())
		{
			if (outputDeviceDropDown == nullptr)
			{
				outputDeviceDropDown = new ComboBox (String::empty);
				outputDeviceDropDown->addListener (this);
				addAndMakeVisible (outputDeviceDropDown);

				outputDeviceLabel = new Label (String::empty,
											   type->hasSeparateInputsAndOutputs() ? TRANS ("output:")
																				   : TRANS ("device:"));
				outputDeviceLabel->attachToComponent (outputDeviceDropDown, true);

				if (setup.maxNumOutputChannels > 0)
				{
					addAndMakeVisible (testButton = new TextButton (TRANS ("Test")));
					testButton->addListener (this);
				}
			}

			addNamesToDeviceBox (*outputDeviceDropDown, false);
		}

		showCorrectDeviceName (outputDeviceDropDown, false);
	}

	void updateInputsComboBox()
	{
		if (setup.maxNumInputChannels > 0 && type->hasSeparateInputsAndOutputs())
		{
			if (inputDeviceDropDown == nullptr)
			{
				inputDeviceDropDown = new ComboBox (String::empty);
				inputDeviceDropDown->addListener (this);
				addAndMakeVisible (inputDeviceDropDown);

				inputDeviceLabel = new Label (String::empty, TRANS ("input:"));
				inputDeviceLabel->attachToComponent (inputDeviceDropDown, true);

				addAndMakeVisible (inputLevelMeter
					= new SimpleDeviceManagerInputLevelMeter (setup.manager));
			}

			addNamesToDeviceBox (*inputDeviceDropDown, true);
		}

		showCorrectDeviceName (inputDeviceDropDown, true);
	}

	void updateSampleRateComboBox (AudioIODevice* currentDevice)
	{
		if (sampleRateDropDown == nullptr)
		{
			addAndMakeVisible (sampleRateDropDown = new ComboBox (String::empty));

			sampleRateLabel = new Label (String::empty, TRANS ("sample rate:"));
			sampleRateLabel->attachToComponent (sampleRateDropDown, true);
		}
		else
		{
			sampleRateDropDown->clear();
			sampleRateDropDown->removeListener (this);
		}

		const int numRates = currentDevice->getNumSampleRates();

		for (int i = 0; i < numRates; ++i)
		{
			const int rate = roundToInt (currentDevice->getSampleRate (i));
			sampleRateDropDown->addItem (String (rate) + " Hz", rate);
		}

		sampleRateDropDown->setSelectedId (roundToInt (currentDevice->getCurrentSampleRate()), true);
		sampleRateDropDown->addListener (this);
	}

	void updateBufferSizeComboBox (AudioIODevice* currentDevice)
	{
		if (bufferSizeDropDown == nullptr)
		{
			addAndMakeVisible (bufferSizeDropDown = new ComboBox (String::empty));

			bufferSizeLabel = new Label (String::empty, TRANS ("audio buffer size:"));
			bufferSizeLabel->attachToComponent (bufferSizeDropDown, true);
		}
		else
		{
			bufferSizeDropDown->clear();
			bufferSizeDropDown->removeListener (this);
		}

		const int numBufferSizes = currentDevice->getNumBufferSizesAvailable();
		double currentRate = currentDevice->getCurrentSampleRate();
		if (currentRate == 0)
			currentRate = 48000.0;

		for (int i = 0; i < numBufferSizes; ++i)
		{
			const int bs = currentDevice->getBufferSizeSamples (i);
			bufferSizeDropDown->addItem (String (bs)
										  + " samples ("
										  + String (bs * 1000.0 / currentRate, 1)
										  + " ms)",
										 bs);
		}

		bufferSizeDropDown->setSelectedId (currentDevice->getCurrentBufferSizeSamples(), true);
		bufferSizeDropDown->addListener (this);
	}

public:

	class ChannelSelectorListBox  : public ListBox,
									public ListBoxModel
	{
	public:
		enum BoxType
		{
			audioInputType,
			audioOutputType
		};

		ChannelSelectorListBox (const AudioDeviceSetupDetails& setup_,
								const BoxType type_,
								const String& noItemsMessage_)
			: ListBox (String::empty, nullptr),
			  setup (setup_),
			  type (type_),
			  noItemsMessage (noItemsMessage_)
		{
			refresh();
			setModel (this);
			setOutlineThickness (1);
		}

		void refresh()
		{
			items.clear();

			AudioIODevice* const currentDevice = setup.manager->getCurrentAudioDevice();

			if (currentDevice != nullptr)
			{
				if (type == audioInputType)
					items = currentDevice->getInputChannelNames();
				else if (type == audioOutputType)
					items = currentDevice->getOutputChannelNames();

				if (setup.useStereoPairs)
				{
					StringArray pairs;

					for (int i = 0; i < items.size(); i += 2)
					{
						const String& name = items[i];

						if (i + 1 >= items.size())
							pairs.add (name.trim());
						else
							pairs.add (getNameForChannelPair (name, items[i + 1]));
					}

					items = pairs;
				}
			}

			updateContent();
			repaint();
		}

		int getNumRows()
		{
			return items.size();
		}

		void paintListBoxItem (int row, Graphics& g, int width, int height, bool rowIsSelected)
		{
			if (isPositiveAndBelow (row, items.size()))
			{
				if (rowIsSelected)
					g.fillAll (findColour (TextEditor::highlightColourId)
								   .withMultipliedAlpha (0.3f));

				const String item (items [row]);
				bool enabled = false;

				AudioDeviceManager::AudioDeviceSetup config;
				setup.manager->getAudioDeviceSetup (config);

				if (setup.useStereoPairs)
				{
					if (type == audioInputType)
						enabled = config.inputChannels [row * 2] || config.inputChannels [row * 2 + 1];
					else if (type == audioOutputType)
						enabled = config.outputChannels [row * 2] || config.outputChannels [row * 2 + 1];
				}
				else
				{
					if (type == audioInputType)
						enabled = config.inputChannels [row];
					else if (type == audioOutputType)
						enabled = config.outputChannels [row];
				}

				const int x = getTickX();
				const float tickW = height * 0.75f;

				getLookAndFeel().drawTickBox (g, *this, x - tickW, (height - tickW) / 2, tickW, tickW,
											  enabled, true, true, false);

				g.setFont (height * 0.6f);
				g.setColour (findColour (ListBox::textColourId, true).withMultipliedAlpha (enabled ? 1.0f : 0.6f));
				g.drawText (item, x, 0, width - x - 2, height, Justification::centredLeft, true);
			}
		}

		void listBoxItemClicked (int row, const MouseEvent& e)
		{
			selectRow (row);

			if (e.x < getTickX())
				flipEnablement (row);
		}

		void listBoxItemDoubleClicked (int row, const MouseEvent&)
		{
			flipEnablement (row);
		}

		void returnKeyPressed (int row)
		{
			flipEnablement (row);
		}

		void paint (Graphics& g)
		{
			ListBox::paint (g);

			if (items.size() == 0)
			{
				g.setColour (Colours::grey);
				g.setFont (13.0f);
				g.drawText (noItemsMessage,
							0, 0, getWidth(), getHeight() / 2,
							Justification::centred, true);
			}
		}

		int getBestHeight (int maxHeight)
		{
			return getRowHeight() * jlimit (2, jmax (2, maxHeight / getRowHeight()),
											getNumRows())
					   + getOutlineThickness() * 2;
		}

	private:

		const AudioDeviceSetupDetails setup;
		const BoxType type;
		const String noItemsMessage;
		StringArray items;

		static String getNameForChannelPair (const String& name1, const String& name2)
		{
			String commonBit;

			for (int j = 0; j < name1.length(); ++j)
				if (name1.substring (0, j).equalsIgnoreCase (name2.substring (0, j)))
					commonBit = name1.substring (0, j);

			// Make sure we only split the name at a space, because otherwise, things
			// like "input 11" + "input 12" would become "input 11 + 2"
			while (commonBit.isNotEmpty() && ! CharacterFunctions::isWhitespace (commonBit.getLastCharacter()))
				commonBit = commonBit.dropLastCharacters (1);

			return name1.trim() + " + " + name2.substring (commonBit.length()).trim();
		}

		void flipEnablement (const int row)
		{
			jassert (type == audioInputType || type == audioOutputType);

			if (isPositiveAndBelow (row, items.size()))
			{
				AudioDeviceManager::AudioDeviceSetup config;
				setup.manager->getAudioDeviceSetup (config);

				if (setup.useStereoPairs)
				{
					BigInteger bits;
					BigInteger& original = (type == audioInputType ? config.inputChannels
																   : config.outputChannels);

					int i;
					for (i = 0; i < 256; i += 2)
						bits.setBit (i / 2, original [i] || original [i + 1]);

					if (type == audioInputType)
					{
						config.useDefaultInputChannels = false;
						flipBit (bits, row, setup.minNumInputChannels / 2, setup.maxNumInputChannels / 2);
					}
					else
					{
						config.useDefaultOutputChannels = false;
						flipBit (bits, row, setup.minNumOutputChannels / 2, setup.maxNumOutputChannels / 2);
					}

					for (i = 0; i < 256; ++i)
						original.setBit (i, bits [i / 2]);
				}
				else
				{
					if (type == audioInputType)
					{
						config.useDefaultInputChannels = false;
						flipBit (config.inputChannels, row, setup.minNumInputChannels, setup.maxNumInputChannels);
					}
					else
					{
						config.useDefaultOutputChannels = false;
						flipBit (config.outputChannels, row, setup.minNumOutputChannels, setup.maxNumOutputChannels);
					}
				}

				String error (setup.manager->setAudioDeviceSetup (config, true));

				if (! error.isEmpty())
				{
					//xxx
				}
			}
		}

		static void flipBit (BigInteger& chans, int index, int minNumber, int maxNumber)
		{
			const int numActive = chans.countNumberOfSetBits();

			if (chans [index])
			{
				if (numActive > minNumber)
					chans.setBit (index, false);
			}
			else
			{
				if (numActive >= maxNumber)
				{
					const int firstActiveChan = chans.findNextSetBit();

					chans.setBit (index > firstActiveChan
									 ? firstActiveChan : chans.getHighestBit(),
								  false);
				}

				chans.setBit (index, true);
			}
		}

		int getTickX() const
		{
			return getRowHeight() + 5;
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelSelectorListBox);
	};

private:
	ScopedPointer<ChannelSelectorListBox> inputChanList, outputChanList;

	JUCE_DECLARE_NON_COPYABLE (AudioDeviceSettingsPanel);
};

AudioDeviceSelectorComponent::AudioDeviceSelectorComponent (AudioDeviceManager& deviceManager_,
															const int minInputChannels_,
															const int maxInputChannels_,
															const int minOutputChannels_,
															const int maxOutputChannels_,
															const bool showMidiInputOptions,
															const bool showMidiOutputSelector,
															const bool showChannelsAsStereoPairs_,
															const bool hideAdvancedOptionsWithButton_)
	: deviceManager (deviceManager_),
	  minOutputChannels (minOutputChannels_),
	  maxOutputChannels (maxOutputChannels_),
	  minInputChannels (minInputChannels_),
	  maxInputChannels (maxInputChannels_),
	  showChannelsAsStereoPairs (showChannelsAsStereoPairs_),
	  hideAdvancedOptionsWithButton (hideAdvancedOptionsWithButton_)
{
	jassert (minOutputChannels >= 0 && minOutputChannels <= maxOutputChannels);
	jassert (minInputChannels >= 0 && minInputChannels <= maxInputChannels);

	const OwnedArray<AudioIODeviceType>& types = deviceManager_.getAvailableDeviceTypes();

	if (types.size() > 1)
	{
		deviceTypeDropDown = new ComboBox (String::empty);

		for (int i = 0; i < types.size(); ++i)
			deviceTypeDropDown->addItem (types.getUnchecked(i)->getTypeName(), i + 1);

		addAndMakeVisible (deviceTypeDropDown);
		deviceTypeDropDown->addListener (this);

		deviceTypeDropDownLabel = new Label (String::empty, TRANS ("audio device type:"));
		deviceTypeDropDownLabel->setJustificationType (Justification::centredRight);
		deviceTypeDropDownLabel->attachToComponent (deviceTypeDropDown, true);
	}

	if (showMidiInputOptions)
	{
		addAndMakeVisible (midiInputsList
							= new MidiInputSelectorComponentListBox (deviceManager,
																	 TRANS("(no midi inputs available)"),
																	 0, 0));

		midiInputsLabel = new Label (String::empty, TRANS ("active midi inputs:"));
		midiInputsLabel->setJustificationType (Justification::topRight);
		midiInputsLabel->attachToComponent (midiInputsList, true);
	}
	else
	{
		midiInputsList = nullptr;
		midiInputsLabel = nullptr;
	}

	if (showMidiOutputSelector)
	{
		addAndMakeVisible (midiOutputSelector = new ComboBox (String::empty));
		midiOutputSelector->addListener (this);

		midiOutputLabel = new Label ("lm", TRANS("Midi Output:"));
		midiOutputLabel->attachToComponent (midiOutputSelector, true);
	}
	else
	{
		midiOutputSelector = nullptr;
		midiOutputLabel = nullptr;
	}

	deviceManager_.addChangeListener (this);
	updateAllControls();
}

AudioDeviceSelectorComponent::~AudioDeviceSelectorComponent()
{
	deviceManager.removeChangeListener (this);
}

void AudioDeviceSelectorComponent::resized()
{
	const int lx = proportionOfWidth (0.35f);
	const int w = proportionOfWidth (0.4f);
	const int h = 24;
	const int space = 6;
	const int dh = h + space;
	int y = 15;

	if (deviceTypeDropDown != nullptr)
	{
		deviceTypeDropDown->setBounds (lx, y, proportionOfWidth (0.3f), h);
		y += dh + space * 2;
	}

	if (audioDeviceSettingsComp != nullptr)
	{
		audioDeviceSettingsComp->setBounds (0, y, getWidth(), audioDeviceSettingsComp->getHeight());
		y += audioDeviceSettingsComp->getHeight() + space;
	}

	if (midiInputsList != nullptr)
	{
		const int bh = midiInputsList->getBestHeight (jmin (h * 8, getHeight() - y - space - h));
		midiInputsList->setBounds (lx, y, w, bh);
		y += bh + space;
	}

	if (midiOutputSelector != nullptr)
		midiOutputSelector->setBounds (lx, y, w, h);
}

void AudioDeviceSelectorComponent::childBoundsChanged (Component* child)
{
	if (child == audioDeviceSettingsComp)
		resized();
}

void AudioDeviceSelectorComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == deviceTypeDropDown)
	{
		AudioIODeviceType* const type = deviceManager.getAvailableDeviceTypes() [deviceTypeDropDown->getSelectedId() - 1];

		if (type != nullptr)
		{
			audioDeviceSettingsComp = nullptr;

			deviceManager.setCurrentAudioDeviceType (type->getTypeName(), true);

			updateAllControls(); // needed in case the type hasn't actally changed
		}
	}
	else if (comboBoxThatHasChanged == midiOutputSelector)
	{
		deviceManager.setDefaultMidiOutput (midiOutputSelector->getText());
	}
}

void AudioDeviceSelectorComponent::changeListenerCallback (ChangeBroadcaster*)
{
	updateAllControls();
}

void AudioDeviceSelectorComponent::updateAllControls()
{
	if (deviceTypeDropDown != nullptr)
		deviceTypeDropDown->setText (deviceManager.getCurrentAudioDeviceType(), false);

	if (audioDeviceSettingsComp == nullptr
		 || audioDeviceSettingsCompType != deviceManager.getCurrentAudioDeviceType())
	{
		audioDeviceSettingsCompType = deviceManager.getCurrentAudioDeviceType();
		audioDeviceSettingsComp = nullptr;

		AudioIODeviceType* const type
			= deviceManager.getAvailableDeviceTypes() [deviceTypeDropDown == nullptr
														? 0 : deviceTypeDropDown->getSelectedId() - 1];

		if (type != nullptr)
		{
			AudioDeviceSetupDetails details;
			details.manager = &deviceManager;
			details.minNumInputChannels = minInputChannels;
			details.maxNumInputChannels = maxInputChannels;
			details.minNumOutputChannels = minOutputChannels;
			details.maxNumOutputChannels = maxOutputChannels;
			details.useStereoPairs = showChannelsAsStereoPairs;

			audioDeviceSettingsComp = new AudioDeviceSettingsPanel (type, details, hideAdvancedOptionsWithButton);

			if (audioDeviceSettingsComp != nullptr)
			{
				addAndMakeVisible (audioDeviceSettingsComp);
				audioDeviceSettingsComp->resized();
			}
		}
	}

	if (midiInputsList != nullptr)
	{
		midiInputsList->updateContent();
		midiInputsList->repaint();
	}

	if (midiOutputSelector != nullptr)
	{
		midiOutputSelector->clear();

		const StringArray midiOuts (MidiOutput::getDevices());

		midiOutputSelector->addItem (TRANS("<< none >>"), -1);
		midiOutputSelector->addSeparator();

		for (int i = 0; i < midiOuts.size(); ++i)
			midiOutputSelector->addItem (midiOuts[i], i + 1);

		int current = -1;

		if (deviceManager.getDefaultMidiOutput() != nullptr)
			current = 1 + midiOuts.indexOf (deviceManager.getDefaultMidiOutputName());

		midiOutputSelector->setSelectedId (current, true);
	}

	resized();
}

/*** End of inlined file: juce_AudioDeviceSelectorComponent.cpp ***/



/*** Start of inlined file: juce_AudioThumbnail.cpp ***/
struct AudioThumbnail::MinMaxValue
{
	MinMaxValue() noexcept
	{
		values[0] = 0;
		values[1] = 0;
	}

	inline void set (const char newMin, const char newMax) noexcept
	{
		values[0] = newMin;
		values[1] = newMax;
	}

	inline char getMinValue() const noexcept        { return values[0]; }
	inline char getMaxValue() const noexcept        { return values[1]; }

	inline void setFloat (const float newMin, const float newMax) noexcept
	{
		values[0] = (char) jlimit (-128, 127, roundFloatToInt (newMin * 127.0f));
		values[1] = (char) jlimit (-128, 127, roundFloatToInt (newMax * 127.0f));

		if (values[0] == values[1])
		{
			if (values[1] == 127)
				values[0]--;
			else
				values[1]++;
		}
	}

	inline bool isNonZero() const noexcept
	{
		return values[1] > values[0];
	}

	inline int getPeak() const noexcept
	{
		return jmax (std::abs ((int) values[0]),
					 std::abs ((int) values[1]));
	}

	inline void read (InputStream& input)      { input.read (values, 2); }
	inline void write (OutputStream& output)   { output.write (values, 2); }

private:
	char values[2];
};

class AudioThumbnail::LevelDataSource   : public TimeSliceClient
{
public:
	LevelDataSource (AudioThumbnail& owner_, AudioFormatReader* newReader, int64 hash)
		: lengthInSamples (0), numSamplesFinished (0), sampleRate (0), numChannels (0),
		  hashCode (hash), owner (owner_), reader (newReader)
	{
	}

	LevelDataSource (AudioThumbnail& owner_, InputSource* source_)
		: lengthInSamples (0), numSamplesFinished (0), sampleRate (0), numChannels (0),
		  hashCode (source_->hashCode()), owner (owner_), source (source_)
	{
	}

	~LevelDataSource()
	{
		owner.cache.removeTimeSliceClient (this);
	}

	enum { timeBeforeDeletingReader = 3000 };

	void initialise (int64 numSamplesFinished_)
	{
		const ScopedLock sl (readerLock);

		numSamplesFinished = numSamplesFinished_;

		createReader();

		if (reader != nullptr)
		{
			lengthInSamples = reader->lengthInSamples;
			numChannels = reader->numChannels;
			sampleRate = reader->sampleRate;

			if (lengthInSamples <= 0 || isFullyLoaded())
				reader = nullptr;
			else
				owner.cache.addTimeSliceClient (this);
		}
	}

	void getLevels (int64 startSample, int numSamples, Array<float>& levels)
	{
		const ScopedLock sl (readerLock);

		if (reader == nullptr)
		{
			createReader();

			if (reader != nullptr)
			{
				lastReaderUseTime = Time::getMillisecondCounter();
				owner.cache.addTimeSliceClient (this);
			}
		}

		if (reader != nullptr)
		{
			float l[4] = { 0 };
			reader->readMaxLevels (startSample, numSamples, l[0], l[1], l[2], l[3]);

			levels.clearQuick();
			levels.addArray ((const float*) l, 4);

			lastReaderUseTime = Time::getMillisecondCounter();
		}
	}

	void releaseResources()
	{
		const ScopedLock sl (readerLock);
		reader = nullptr;
	}

	int useTimeSlice()
	{
		if (isFullyLoaded())
		{
			if (reader != nullptr && source != nullptr)
			{
				if (Time::getMillisecondCounter() > lastReaderUseTime + timeBeforeDeletingReader)
					releaseResources();
				else
					return 200;
			}

			return -1;
		}

		bool justFinished = false;

		{
			const ScopedLock sl (readerLock);

			createReader();

			if (reader != nullptr)
			{
				if (! readNextBlock())
					return 0;

				justFinished = true;
			}
		}

		if (justFinished)
			owner.cache.storeThumb (owner, hashCode);

		return 200;
	}

	bool isFullyLoaded() const noexcept
	{
		return numSamplesFinished >= lengthInSamples;
	}

	inline int sampleToThumbSample (const int64 originalSample) const noexcept
	{
		return (int) (originalSample / owner.samplesPerThumbSample);
	}

	int64 lengthInSamples, numSamplesFinished;
	double sampleRate;
	unsigned int numChannels;
	int64 hashCode;

private:
	AudioThumbnail& owner;
	ScopedPointer <InputSource> source;
	ScopedPointer <AudioFormatReader> reader;
	CriticalSection readerLock;
	uint32 lastReaderUseTime;

	void createReader()
	{
		if (reader == nullptr && source != nullptr)
		{
			InputStream* audioFileStream = source->createInputStream();

			if (audioFileStream != nullptr)
				reader = owner.formatManagerToUse.createReaderFor (audioFileStream);
		}
	}

	bool readNextBlock()
	{
		jassert (reader != nullptr);

		if (! isFullyLoaded())
		{
			const int numToDo = (int) jmin (256 * (int64) owner.samplesPerThumbSample, lengthInSamples - numSamplesFinished);

			if (numToDo > 0)
			{
				int64 startSample = numSamplesFinished;

				const int firstThumbIndex = sampleToThumbSample (startSample);
				const int lastThumbIndex  = sampleToThumbSample (startSample + numToDo);
				const int numThumbSamps = lastThumbIndex - firstThumbIndex;

				HeapBlock<MinMaxValue> levelData ((size_t) numThumbSamps * 2);
				MinMaxValue* levels[2] = { levelData, levelData + numThumbSamps };

				for (int i = 0; i < numThumbSamps; ++i)
				{
					float lowestLeft, highestLeft, lowestRight, highestRight;

					reader->readMaxLevels ((firstThumbIndex + i) * owner.samplesPerThumbSample, owner.samplesPerThumbSample,
										   lowestLeft, highestLeft, lowestRight, highestRight);

					levels[0][i].setFloat (lowestLeft, highestLeft);
					levels[1][i].setFloat (lowestRight, highestRight);
				}

				{
					const ScopedUnlock su (readerLock);
					owner.setLevels (levels, firstThumbIndex, 2, numThumbSamps);
				}

				numSamplesFinished += numToDo;
				lastReaderUseTime = Time::getMillisecondCounter();
			}
		}

		return isFullyLoaded();
	}
};

class AudioThumbnail::ThumbData
{
public:
	ThumbData (const int numThumbSamples)
		: peakLevel (-1)
	{
		ensureSize (numThumbSamples);
	}

	inline MinMaxValue* getData (const int thumbSampleIndex) noexcept
	{
		jassert (thumbSampleIndex < data.size());
		return data.getRawDataPointer() + thumbSampleIndex;
	}

	int getSize() const noexcept
	{
		return data.size();
	}

	void getMinMax (int startSample, int endSample, MinMaxValue& result) const noexcept
	{
		if (startSample >= 0)
		{
			endSample = jmin (endSample, data.size() - 1);

			char mx = -128;
			char mn = 127;

			while (startSample <= endSample)
			{
				const MinMaxValue& v = data.getReference (startSample);

				if (v.getMinValue() < mn)  mn = v.getMinValue();
				if (v.getMaxValue() > mx)  mx = v.getMaxValue();

				++startSample;
			}

			if (mn <= mx)
			{
				result.set (mn, mx);
				return;
			}
		}

		result.set (1, 0);
	}

	void write (const MinMaxValue* const source, const int startIndex, const int numValues)
	{
		resetPeak();

		if (startIndex + numValues > data.size())
			ensureSize (startIndex + numValues);

		MinMaxValue* const dest = getData (startIndex);

		for (int i = 0; i < numValues; ++i)
			dest[i] = source[i];
	}

	void resetPeak() noexcept
	{
		peakLevel = -1;
	}

	int getPeak() noexcept
	{
		if (peakLevel < 0)
		{
			for (int i = 0; i < data.size(); ++i)
			{
				const int peak = data[i].getPeak();
				if (peak > peakLevel)
					peakLevel = peak;
			}
		}

		return peakLevel;
	}

private:
	Array <MinMaxValue> data;
	int peakLevel;

	void ensureSize (const int thumbSamples)
	{
		const int extraNeeded = thumbSamples - data.size();
		if (extraNeeded > 0)
			data.insertMultiple (-1, MinMaxValue(), extraNeeded);
	}
};

class AudioThumbnail::CachedWindow
{
public:
	CachedWindow()
		: cachedStart (0), cachedTimePerPixel (0),
		  numChannelsCached (0), numSamplesCached (0),
		  cacheNeedsRefilling (true)
	{
	}

	void invalidate()
	{
		cacheNeedsRefilling = true;
	}

	void drawChannel (Graphics& g, const Rectangle<int>& area,
					  const double startTime, const double endTime,
					  const int channelNum, const float verticalZoomFactor,
					  const double sampleRate, const int numChannels, const int samplesPerThumbSample,
					  LevelDataSource* levelData, const OwnedArray<ThumbData>& channels)
	{
		refillCache (area.getWidth(), startTime, endTime, sampleRate,
					 numChannels, samplesPerThumbSample, levelData, channels);

		if (isPositiveAndBelow (channelNum, numChannelsCached))
		{
			const Rectangle<int> clip (g.getClipBounds().getIntersection (area.withWidth (jmin (numSamplesCached, area.getWidth()))));

			if (! clip.isEmpty())
			{
				const float topY = (float) area.getY();
				const float bottomY = (float) area.getBottom();
				const float midY = (topY + bottomY) * 0.5f;
				const float vscale = verticalZoomFactor * (bottomY - topY) / 256.0f;

				const MinMaxValue* cacheData = getData (channelNum, clip.getX() - area.getX());

				int x = clip.getX();
				for (int w = clip.getWidth(); --w >= 0;)
				{
					if (cacheData->isNonZero())
						g.drawVerticalLine (x, jmax (midY - cacheData->getMaxValue() * vscale - 0.3f, topY),
											   jmin (midY - cacheData->getMinValue() * vscale + 0.3f, bottomY));

					++x;
					++cacheData;
				}
			}
		}
	}

private:
	Array <MinMaxValue> data;
	double cachedStart, cachedTimePerPixel;
	int numChannelsCached, numSamplesCached;
	bool cacheNeedsRefilling;

	void refillCache (const int numSamples, double startTime, const double endTime,
					  const double sampleRate, const int numChannels, const int samplesPerThumbSample,
					  LevelDataSource* levelData, const OwnedArray<ThumbData>& channels)
	{
		const double timePerPixel = (endTime - startTime) / numSamples;

		if (numSamples <= 0 || timePerPixel <= 0.0 || sampleRate <= 0)
		{
			invalidate();
			return;
		}

		if (numSamples == numSamplesCached
			 && numChannelsCached == numChannels
			 && startTime == cachedStart
			 && timePerPixel == cachedTimePerPixel
			 && ! cacheNeedsRefilling)
		{
			return;
		}

		numSamplesCached = numSamples;
		numChannelsCached = numChannels;
		cachedStart = startTime;
		cachedTimePerPixel = timePerPixel;
		cacheNeedsRefilling = false;

		ensureSize (numSamples);

		if (timePerPixel * sampleRate <= samplesPerThumbSample && levelData != nullptr)
		{
			int sample = roundToInt (startTime * sampleRate);
			Array<float> levels;

			int i;
			for (i = 0; i < numSamples; ++i)
			{
				const int nextSample = roundToInt ((startTime + timePerPixel) * sampleRate);

				if (sample >= 0)
				{
					if (sample >= levelData->lengthInSamples)
						break;

					levelData->getLevels (sample, jmax (1, nextSample - sample), levels);

					const int numChans = jmin (levels.size() / 2, numChannelsCached);

					for (int chan = 0; chan < numChans; ++chan)
						getData (chan, i)->setFloat (levels.getUnchecked (chan * 2),
													 levels.getUnchecked (chan * 2 + 1));
				}

				startTime += timePerPixel;
				sample = nextSample;
			}

			numSamplesCached = i;
		}
		else
		{
			jassert (channels.size() == numChannelsCached);

			for (int channelNum = 0; channelNum < numChannelsCached; ++channelNum)
			{
				ThumbData* channelData = channels.getUnchecked (channelNum);
				MinMaxValue* cacheData = getData (channelNum, 0);

				const double timeToThumbSampleFactor = sampleRate / (double) samplesPerThumbSample;

				startTime = cachedStart;
				int sample = roundToInt (startTime * timeToThumbSampleFactor);

				for (int i = numSamples; --i >= 0;)
				{
					const int nextSample = roundToInt ((startTime + timePerPixel) * timeToThumbSampleFactor);

					channelData->getMinMax (sample, nextSample, *cacheData);

					++cacheData;
					startTime += timePerPixel;
					sample = nextSample;
				}
			}
		}
	}

	MinMaxValue* getData (const int channelNum, const int cacheIndex) noexcept
	{
		jassert (isPositiveAndBelow (channelNum, numChannelsCached) && isPositiveAndBelow (cacheIndex, data.size()));

		return data.getRawDataPointer() + channelNum * numSamplesCached
										+ cacheIndex;
	}

	void ensureSize (const int numSamples)
	{
		const int itemsRequired = numSamples * numChannelsCached;

		if (data.size() < itemsRequired)
			data.insertMultiple (-1, MinMaxValue(), itemsRequired - data.size());
	}
};

AudioThumbnail::AudioThumbnail (const int originalSamplesPerThumbnailSample,
								AudioFormatManager& formatManagerToUse_,
								AudioThumbnailCache& cacheToUse)
	: formatManagerToUse (formatManagerToUse_),
	  cache (cacheToUse),
	  window (new CachedWindow()),
	  samplesPerThumbSample (originalSamplesPerThumbnailSample),
	  totalSamples (0),
	  numChannels (0),
	  sampleRate (0)
{
}

AudioThumbnail::~AudioThumbnail()
{
	clear();
}

void AudioThumbnail::clear()
{
	source = nullptr;
	const ScopedLock sl (lock);
	clearChannelData();
}

void AudioThumbnail::clearChannelData()
{
	window->invalidate();
	channels.clear();
	totalSamples = numSamplesFinished = 0;
	numChannels = 0;
	sampleRate = 0;

	sendChangeMessage();
}

void AudioThumbnail::reset (int newNumChannels, double newSampleRate, int64 totalSamplesInSource)
{
	const ScopedLock sl (lock);
	clear();

	numChannels = newNumChannels;
	sampleRate = newSampleRate;
	totalSamples = totalSamplesInSource;

	createChannels (1 + (int) (totalSamplesInSource / samplesPerThumbSample));
}

void AudioThumbnail::createChannels (const int length)
{
	while (channels.size() < numChannels)
		channels.add (new ThumbData (length));
}

void AudioThumbnail::loadFrom (InputStream& rawInput)
{
	BufferedInputStream input (rawInput, 4096);

	if (input.readByte() != 'j' || input.readByte() != 'a' || input.readByte() != 't' || input.readByte() != 'm')
		return;

	const ScopedLock sl (lock);
	clearChannelData();

	samplesPerThumbSample = input.readInt();
	totalSamples = input.readInt64();             // Total number of source samples.
	numSamplesFinished = input.readInt64();       // Number of valid source samples that have been read into the thumbnail.
	int32 numThumbnailSamples = input.readInt();  // Number of samples in the thumbnail data.
	numChannels = input.readInt();                // Number of audio channels.
	sampleRate = input.readInt();                 // Source sample rate.
	input.skipNextBytes (16);                     // (reserved)

	createChannels (numThumbnailSamples);

	for (int i = 0; i < numThumbnailSamples; ++i)
		for (int chan = 0; chan < numChannels; ++chan)
			channels.getUnchecked(chan)->getData(i)->read (input);
}

void AudioThumbnail::saveTo (OutputStream& output) const
{
	const ScopedLock sl (lock);

	const int numThumbnailSamples = channels.size() == 0 ? 0 : channels.getUnchecked(0)->getSize();

	output.write ("jatm", 4);
	output.writeInt (samplesPerThumbSample);
	output.writeInt64 (totalSamples);
	output.writeInt64 (numSamplesFinished);
	output.writeInt (numThumbnailSamples);
	output.writeInt (numChannels);
	output.writeInt ((int) sampleRate);
	output.writeInt64 (0);
	output.writeInt64 (0);

	for (int i = 0; i < numThumbnailSamples; ++i)
		for (int chan = 0; chan < numChannels; ++chan)
			channels.getUnchecked(chan)->getData(i)->write (output);
}

bool AudioThumbnail::setDataSource (LevelDataSource* newSource)
{
	jassert (MessageManager::getInstance()->currentThreadHasLockedMessageManager());

	numSamplesFinished = 0;

	if (cache.loadThumb (*this, newSource->hashCode) && isFullyLoaded())
	{
		source = newSource; // (make sure this isn't done before loadThumb is called)

		source->lengthInSamples = totalSamples;
		source->sampleRate = sampleRate;
		source->numChannels = (unsigned int) numChannels;
		source->numSamplesFinished = numSamplesFinished;
	}
	else
	{
		source = newSource; // (make sure this isn't done before loadThumb is called)

		const ScopedLock sl (lock);
		source->initialise (numSamplesFinished);

		totalSamples = source->lengthInSamples;
		sampleRate = source->sampleRate;
		numChannels = (int32) source->numChannels;

		createChannels (1 + (int) (totalSamples / samplesPerThumbSample));
	}

	return sampleRate > 0 && totalSamples > 0;
}

bool AudioThumbnail::setSource (InputSource* const newSource)
{
	clear();

	return newSource != nullptr && setDataSource (new LevelDataSource (*this, newSource));
}

void AudioThumbnail::setReader (AudioFormatReader* newReader, int64 hash)
{
	clear();

	if (newReader != nullptr)
		setDataSource (new LevelDataSource (*this, newReader, hash));
}

int64 AudioThumbnail::getHashCode() const
{
	return source == nullptr ? 0 : source->hashCode;
}

void AudioThumbnail::addBlock (const int64 startSample, const AudioSampleBuffer& incoming,
							   int startOffsetInBuffer, int numSamples)
{
	jassert (startSample >= 0);

	const int firstThumbIndex = (int) (startSample / samplesPerThumbSample);
	const int lastThumbIndex  = (int) ((startSample + numSamples + (samplesPerThumbSample - 1)) / samplesPerThumbSample);
	const int numToDo = lastThumbIndex - firstThumbIndex;

	if (numToDo > 0)
	{
		const int numChans = jmin (channels.size(), incoming.getNumChannels());

		const HeapBlock<MinMaxValue> thumbData ((size_t) (numToDo * numChans));
		const HeapBlock<MinMaxValue*> thumbChannels ((size_t) numChans);

		for (int chan = 0; chan < numChans; ++chan)
		{
			const float* const sourceData = incoming.getSampleData (chan, startOffsetInBuffer);
			MinMaxValue* const dest = thumbData + numToDo * chan;
			thumbChannels [chan] = dest;

			for (int i = 0; i < numToDo; ++i)
			{
				float low, high;
				const int start = i * samplesPerThumbSample;
				findMinAndMax (sourceData + start, jmin (samplesPerThumbSample, numSamples - start), low, high);
				dest[i].setFloat (low, high);
			}
		}

		setLevels (thumbChannels, firstThumbIndex, numChans, numToDo);
	}
}

void AudioThumbnail::setLevels (const MinMaxValue* const* values, int thumbIndex, int numChans, int numValues)
{
	const ScopedLock sl (lock);

	for (int i = jmin (numChans, channels.size()); --i >= 0;)
		channels.getUnchecked(i)->write (values[i], thumbIndex, numValues);

	const int64 start = thumbIndex * (int64) samplesPerThumbSample;
	const int64 end = (thumbIndex + numValues) * (int64) samplesPerThumbSample;

	if (numSamplesFinished >= start && end > numSamplesFinished)
		numSamplesFinished = end;

	totalSamples = jmax (numSamplesFinished, totalSamples);
	window->invalidate();
	sendChangeMessage();
}

int AudioThumbnail::getNumChannels() const noexcept
{
	return numChannels;
}

double AudioThumbnail::getTotalLength() const noexcept
{
	return sampleRate > 0 ? (totalSamples / sampleRate) : 0;
}

bool AudioThumbnail::isFullyLoaded() const noexcept
{
	return numSamplesFinished >= totalSamples - samplesPerThumbSample;
}

int64 AudioThumbnail::getNumSamplesFinished() const noexcept
{
	return numSamplesFinished;
}

float AudioThumbnail::getApproximatePeak() const
{
	const ScopedLock sl (lock);
	int peak = 0;

	for (int i = channels.size(); --i >= 0;)
		peak = jmax (peak, channels.getUnchecked(i)->getPeak());

	return jlimit (0, 127, peak) / 127.0f;
}

void AudioThumbnail::getApproximateMinMax (const double startTime, const double endTime, const int channelIndex,
										   float& minValue, float& maxValue) const noexcept
{
	const ScopedLock sl (lock);
	MinMaxValue result;
	const ThumbData* const data = channels [channelIndex];

	if (data != nullptr && sampleRate > 0)
	{
		const int firstThumbIndex = (int) ((startTime * sampleRate) / samplesPerThumbSample);
		const int lastThumbIndex  = (int) (((endTime * sampleRate) + samplesPerThumbSample - 1) / samplesPerThumbSample);

		data->getMinMax (jmax (0, firstThumbIndex), lastThumbIndex, result);
	}

	minValue = result.getMinValue() / 128.0f;
	maxValue = result.getMaxValue() / 128.0f;
}

void AudioThumbnail::drawChannel (Graphics& g, const Rectangle<int>& area, double startTime,
								  double endTime, int channelNum, float verticalZoomFactor)
{
	const ScopedLock sl (lock);

	window->drawChannel (g, area, startTime, endTime, channelNum, verticalZoomFactor,
						 sampleRate, numChannels, samplesPerThumbSample, source, channels);
}

void AudioThumbnail::drawChannels (Graphics& g, const Rectangle<int>& area, double startTimeSeconds,
								   double endTimeSeconds, float verticalZoomFactor)
{
	for (int i = 0; i < numChannels; ++i)
	{
		const int y1 = roundToInt ((i * area.getHeight()) / numChannels);
		const int y2 = roundToInt (((i + 1) * area.getHeight()) / numChannels);

		drawChannel (g, Rectangle<int> (area.getX(), area.getY() + y1, area.getWidth(), y2 - y1),
					 startTimeSeconds, endTimeSeconds, i, verticalZoomFactor);
	}
}

/*** End of inlined file: juce_AudioThumbnail.cpp ***/


/*** Start of inlined file: juce_AudioThumbnailCache.cpp ***/
class AudioThumbnailCache::ThumbnailCacheEntry
{
public:
	ThumbnailCacheEntry (const int64 hash_)
		: hash (hash_),
		  lastUsed (Time::getMillisecondCounter())
	{
	}

	ThumbnailCacheEntry (InputStream& in)
		: lastUsed (0)
	{
		hash = in.readInt64();
		const int64 len = in.readInt64();
		in.readIntoMemoryBlock (data, (ssize_t) len);
	}

	void write (OutputStream& out)
	{
		out.writeInt64 (hash);
		out.writeInt64 ((int64) data.getSize());
		out << data;
	}

	int64 hash;
	uint32 lastUsed;
	MemoryBlock data;

private:
	JUCE_LEAK_DETECTOR (ThumbnailCacheEntry);
};

AudioThumbnailCache::AudioThumbnailCache (const int maxNumThumbsToStore_)
	: TimeSliceThread ("thumb cache"),
	  maxNumThumbsToStore (maxNumThumbsToStore_)
{
	jassert (maxNumThumbsToStore > 0);
	startThread (2);
}

AudioThumbnailCache::~AudioThumbnailCache()
{
}

AudioThumbnailCache::ThumbnailCacheEntry* AudioThumbnailCache::findThumbFor (const int64 hash) const
{
	for (int i = thumbs.size(); --i >= 0;)
		if (thumbs.getUnchecked(i)->hash == hash)
			return thumbs.getUnchecked(i);

	return nullptr;
}

int AudioThumbnailCache::findOldestThumb() const
{
	int oldest = 0;
	uint32 oldestTime = Time::getMillisecondCounter() + 1;

	for (int i = thumbs.size(); --i >= 0;)
	{
		const ThumbnailCacheEntry* const te = thumbs.getUnchecked(i);

		if (te->lastUsed < oldestTime)
		{
			oldest = i;
			oldestTime = te->lastUsed;
		}
	}

	return oldest;
}

bool AudioThumbnailCache::loadThumb (AudioThumbnailBase& thumb, const int64 hashCode)
{
	const ScopedLock sl (lock);
	ThumbnailCacheEntry* te = findThumbFor (hashCode);

	if (te != nullptr)
	{
		te->lastUsed = Time::getMillisecondCounter();

		MemoryInputStream in (te->data, false);
		thumb.loadFrom (in);
		return true;
	}

	return false;
}

void AudioThumbnailCache::storeThumb (const AudioThumbnailBase& thumb,
									  const int64 hashCode)
{
	const ScopedLock sl (lock);
	ThumbnailCacheEntry* te = findThumbFor (hashCode);

	if (te == nullptr)
	{
		te = new ThumbnailCacheEntry (hashCode);

		if (thumbs.size() < maxNumThumbsToStore)
			thumbs.add (te);
		else
			thumbs.set (findOldestThumb(), te);
	}

	MemoryOutputStream out (te->data, false);
	thumb.saveTo (out);
}

void AudioThumbnailCache::clear()
{
	const ScopedLock sl (lock);
	thumbs.clear();
}

static inline int getThumbnailCacheFileMagicHeader() noexcept
{
	return (int) ByteOrder::littleEndianInt ("ThmC");
}

bool AudioThumbnailCache::readFromStream (InputStream& source)
{
	if (source.readInt() != getThumbnailCacheFileMagicHeader())
		return false;

	const ScopedLock sl (lock);
	clear();
	int numThumbnails = jmin (maxNumThumbsToStore, source.readInt());

	while (--numThumbnails >= 0 && ! source.isExhausted())
		thumbs.add (new ThumbnailCacheEntry (source));

	return true;
}

void AudioThumbnailCache::writeToStream (OutputStream& out)
{
	const ScopedLock sl (lock);

	out.writeInt (getThumbnailCacheFileMagicHeader());
	out.writeInt (thumbs.size());

	for (int i = 0; i < thumbs.size(); ++i)
		thumbs.getUnchecked(i)->write (out);
}

/*** End of inlined file: juce_AudioThumbnailCache.cpp ***/


/*** Start of inlined file: juce_MidiKeyboardComponent.cpp ***/
class MidiKeyboardUpDownButton  : public Button
{
public:
	MidiKeyboardUpDownButton (MidiKeyboardComponent& owner_, const int delta_)
		: Button (String::empty),
		  owner (owner_),
		  delta (delta_)
	{
		setOpaque (true);
	}

	void clicked()
	{
		int note = owner.getLowestVisibleKey();

		if (delta < 0)
			note = (note - 1) / 12;
		else
			note = note / 12 + 1;

		owner.setLowestVisibleKey (note * 12);
	}

	void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
	{
		owner.drawUpDownButton (g, getWidth(), getHeight(),
								isMouseOverButton, isButtonDown,
								delta > 0);
	}

private:
	MidiKeyboardComponent& owner;
	const int delta;

	JUCE_DECLARE_NON_COPYABLE (MidiKeyboardUpDownButton);
};

MidiKeyboardComponent::MidiKeyboardComponent (MidiKeyboardState& state_,
											  const Orientation orientation_)
	: state (state_),
	  xOffset (0),
	  blackNoteLength (1),
	  keyWidth (16.0f),
	  orientation (orientation_),
	  midiChannel (1),
	  midiInChannelMask (0xffff),
	  velocity (1.0f),
	  shouldCheckState (false),
	  rangeStart (0),
	  rangeEnd (127),
	  firstKey (12 * 4.0f),
	  canScroll (true),
	  useMousePositionForVelocity (true),
	  shouldCheckMousePos (false),
	  keyMappingOctave (6),
	  octaveNumForMiddleC (3)
{
	addChildComponent (scrollDown = new MidiKeyboardUpDownButton (*this, -1));
	addChildComponent (scrollUp   = new MidiKeyboardUpDownButton (*this, 1));

	// initialise with a default set of querty key-mappings..
	const char* const keymap = "awsedftgyhujkolp;";

	for (int i = 0; keymap[i] != 0; ++i)
		setKeyPressForNote (KeyPress (keymap[i], 0, 0), i);

	const int numSources = Desktop::getInstance().getNumMouseSources();
	mouseOverNotes.insertMultiple (0, -1, numSources);
	mouseDownNotes.insertMultiple (0, -1, numSources);

	setOpaque (true);
	setWantsKeyboardFocus (true);

	state.addListener (this);

	startTimer (1000 / 20);
}

MidiKeyboardComponent::~MidiKeyboardComponent()
{
	state.removeListener (this);
}

void MidiKeyboardComponent::setKeyWidth (const float widthInPixels)
{
	keyWidth = widthInPixels;
	resized();
}

void MidiKeyboardComponent::setOrientation (const Orientation newOrientation)
{
	if (orientation != newOrientation)
	{
		orientation = newOrientation;
		resized();
	}
}

void MidiKeyboardComponent::setAvailableRange (const int lowestNote,
											   const int highestNote)
{
	jassert (lowestNote >= 0 && lowestNote <= 127);
	jassert (highestNote >= 0 && highestNote <= 127);
	jassert (lowestNote <= highestNote);

	if (rangeStart != lowestNote || rangeEnd != highestNote)
	{
		rangeStart = jlimit (0, 127, lowestNote);
		rangeEnd = jlimit (0, 127, highestNote);
		firstKey = jlimit ((float) rangeStart, (float) rangeEnd, firstKey);
		resized();
	}
}

void MidiKeyboardComponent::setLowestVisibleKey (int noteNumber)
{
	setLowestVisibleKeyFloat ((float) noteNumber);
}

void MidiKeyboardComponent::setLowestVisibleKeyFloat (float noteNumber)
{
	noteNumber = jlimit ((float) rangeStart, (float) rangeEnd, noteNumber);

	if (noteNumber != firstKey)
	{
		const bool hasMoved = (((int) firstKey) != (int) noteNumber);
		firstKey = noteNumber;

		if (hasMoved)
		{
			sendChangeMessage();
			resized();
		}
	}
}

void MidiKeyboardComponent::setScrollButtonsVisible (const bool canScroll_)
{
	if (canScroll != canScroll_)
	{
		canScroll = canScroll_;
		resized();
	}
}

void MidiKeyboardComponent::colourChanged()
{
	repaint();
}

void MidiKeyboardComponent::setMidiChannel (const int midiChannelNumber)
{
	jassert (midiChannelNumber > 0 && midiChannelNumber <= 16);

	if (midiChannel != midiChannelNumber)
	{
		resetAnyKeysInUse();
		midiChannel = jlimit (1, 16, midiChannelNumber);
	}
}

void MidiKeyboardComponent::setMidiChannelsToDisplay (const int midiChannelMask)
{
	midiInChannelMask = midiChannelMask;
	shouldCheckState = true;
}

void MidiKeyboardComponent::setVelocity (const float velocity_, const bool useMousePositionForVelocity_)
{
	velocity = jlimit (0.0f, 1.0f, velocity_);
	useMousePositionForVelocity = useMousePositionForVelocity_;
}

void MidiKeyboardComponent::getKeyPosition (int midiNoteNumber, const float keyWidth_, int& x, int& w) const
{
	jassert (midiNoteNumber >= 0 && midiNoteNumber < 128);

	static const float blackNoteWidth = 0.7f;

	static const float notePos[] = { 0.0f, 1 - blackNoteWidth * 0.6f,
									 1.0f, 2 - blackNoteWidth * 0.4f,
									 2.0f,
									 3.0f, 4 - blackNoteWidth * 0.7f,
									 4.0f, 5 - blackNoteWidth * 0.5f,
									 5.0f, 6 - blackNoteWidth * 0.3f,
									 6.0f };

	static const float widths[] = { 1.0f, blackNoteWidth,
									1.0f, blackNoteWidth,
									1.0f,
									1.0f, blackNoteWidth,
									1.0f, blackNoteWidth,
									1.0f, blackNoteWidth,
									1.0f };

	const int octave = midiNoteNumber / 12;
	const int note = midiNoteNumber % 12;

	x = roundToInt (octave * 7.0f * keyWidth_ + notePos [note] * keyWidth_);
	w = roundToInt (widths [note] * keyWidth_);
}

void MidiKeyboardComponent::getKeyPos (int midiNoteNumber, int& x, int& w) const
{
	getKeyPosition (midiNoteNumber, keyWidth, x, w);

	int rx, rw;
	getKeyPosition (rangeStart, keyWidth, rx, rw);

	x -= xOffset + rx;
}

Rectangle<int> MidiKeyboardComponent::getWhiteNotePos (int noteNum) const
{
	int x, w;
	getKeyPos (noteNum, x, w);
	Rectangle<int> pos;

	switch (orientation)
	{
		case horizontalKeyboard:            pos.setBounds (x, 0, w, getHeight()); break;
		case verticalKeyboardFacingLeft:    pos.setBounds (0, x, getWidth(), w); break;
		case verticalKeyboardFacingRight:   pos.setBounds (0, getHeight() - x - w, getWidth(), w); break;
		default: break;
	}

	return pos;
}

int MidiKeyboardComponent::getKeyStartPosition (const int midiNoteNumber) const
{
	int x, y;
	getKeyPos (midiNoteNumber, x, y);
	return x;
}

const uint8 MidiKeyboardComponent::whiteNotes[] = { 0, 2, 4, 5, 7, 9, 11 };
const uint8 MidiKeyboardComponent::blackNotes[] = { 1, 3, 6, 8, 10 };

int MidiKeyboardComponent::xyToNote (const Point<int>& pos, float& mousePositionVelocity)
{
	if (! reallyContains (pos, false))
		return -1;

	Point<int> p (pos);

	if (orientation != horizontalKeyboard)
	{
		p = Point<int> (p.y, p.x);

		if (orientation == verticalKeyboardFacingLeft)
			p = Point<int> (p.x, getWidth() - p.y);
		else
			p = Point<int> (getHeight() - p.x, p.y);
	}

	return remappedXYToNote (p + Point<int> (xOffset, 0), mousePositionVelocity);
}

int MidiKeyboardComponent::remappedXYToNote (const Point<int>& pos, float& mousePositionVelocity) const
{
	if (pos.getY() < blackNoteLength)
	{
		for (int octaveStart = 12 * (rangeStart / 12); octaveStart <= rangeEnd; octaveStart += 12)
		{
			for (int i = 0; i < 5; ++i)
			{
				const int note = octaveStart + blackNotes [i];

				if (note >= rangeStart && note <= rangeEnd)
				{
					int kx, kw;
					getKeyPos (note, kx, kw);
					kx += xOffset;

					if (pos.x >= kx && pos.x < kx + kw)
					{
						mousePositionVelocity = pos.y / (float) blackNoteLength;
						return note;
					}
				}
			}
		}
	}

	for (int octaveStart = 12 * (rangeStart / 12); octaveStart <= rangeEnd; octaveStart += 12)
	{
		for (int i = 0; i < 7; ++i)
		{
			const int note = octaveStart + whiteNotes [i];

			if (note >= rangeStart && note <= rangeEnd)
			{
				int kx, kw;
				getKeyPos (note, kx, kw);
				kx += xOffset;

				if (pos.x >= kx && pos.x < kx + kw)
				{
					const int whiteNoteLength = (orientation == horizontalKeyboard) ? getHeight() : getWidth();
					mousePositionVelocity = pos.y / (float) whiteNoteLength;
					return note;
				}
			}
		}
	}

	mousePositionVelocity = 0;
	return -1;
}

void MidiKeyboardComponent::repaintNote (const int noteNum)
{
	if (noteNum >= rangeStart && noteNum <= rangeEnd)
		repaint (getWhiteNotePos (noteNum));
}

void MidiKeyboardComponent::paint (Graphics& g)
{
	g.fillAll (Colours::white.overlaidWith (findColour (whiteNoteColourId)));

	const Colour lineColour (findColour (keySeparatorLineColourId));
	const Colour textColour (findColour (textLabelColourId));

	int x, w, octave;

	for (octave = 0; octave < 128; octave += 12)
	{
		for (int white = 0; white < 7; ++white)
		{
			const int noteNum = octave + whiteNotes [white];

			if (noteNum >= rangeStart && noteNum <= rangeEnd)
			{
				const Rectangle<int> pos (getWhiteNotePos (noteNum));

				drawWhiteNote (noteNum, g, pos.getX(), pos.getY(), pos.getWidth(), pos.getHeight(),
							   state.isNoteOnForChannels (midiInChannelMask, noteNum),
							   mouseOverNotes.contains (noteNum), lineColour, textColour);
			}
		}
	}

	float x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;

	if (orientation == verticalKeyboardFacingLeft)
	{
		x1 = getWidth() - 1.0f;
		x2 = getWidth() - 5.0f;
	}
	else if (orientation == verticalKeyboardFacingRight)
		x2 = 5.0f;
	else
		y2 = 5.0f;

	g.setGradientFill (ColourGradient (Colours::black.withAlpha (0.3f), x1, y1,
									   Colours::transparentBlack, x2, y2, false));

	getKeyPos (rangeEnd, x, w);
	x += w;

	switch (orientation)
	{
		case horizontalKeyboard:            g.fillRect (0, 0, x, 5); break;
		case verticalKeyboardFacingLeft:    g.fillRect (getWidth() - 5, 0, 5, x); break;
		case verticalKeyboardFacingRight:   g.fillRect (0, 0, 5, x); break;
		default: break;
	}

	g.setColour (lineColour);

	switch (orientation)
	{
		case horizontalKeyboard:            g.fillRect (0, getHeight() - 1, x, 1); break;
		case verticalKeyboardFacingLeft:    g.fillRect (0, 0, 1, x); break;
		case verticalKeyboardFacingRight:   g.fillRect (getWidth() - 1, 0, 1, x); break;
		default: break;
	}

	const Colour blackNoteColour (findColour (blackNoteColourId));

	for (octave = 0; octave < 128; octave += 12)
	{
		for (int black = 0; black < 5; ++black)
		{
			const int noteNum = octave + blackNotes [black];

			if (noteNum >= rangeStart && noteNum <= rangeEnd)
			{
				getKeyPos (noteNum, x, w);
				Rectangle<int> pos;

				switch (orientation)
				{
					case horizontalKeyboard:            pos.setBounds (x, 0, w, blackNoteLength); break;
					case verticalKeyboardFacingLeft:    pos.setBounds (getWidth() - blackNoteLength, x, blackNoteLength, w); break;
					case verticalKeyboardFacingRight:   pos.setBounds (0, getHeight() - x - w, blackNoteLength, w); break;
					default: break;
				}

				drawBlackNote (noteNum, g, pos.getX(), pos.getY(), pos.getWidth(), pos.getHeight(),
							   state.isNoteOnForChannels (midiInChannelMask, noteNum),
							   mouseOverNotes.contains (noteNum), blackNoteColour);
			}
		}
	}
}

void MidiKeyboardComponent::drawWhiteNote (int midiNoteNumber,
										   Graphics& g, int x, int y, int w, int h,
										   bool isDown, bool isOver,
										   const Colour& lineColour,
										   const Colour& textColour)
{
	Colour c (Colours::transparentWhite);

	if (isDown)
		c = findColour (keyDownOverlayColourId);

	if (isOver)
		c = c.overlaidWith (findColour (mouseOverKeyOverlayColourId));

	g.setColour (c);
	g.fillRect (x, y, w, h);

	const String text (getWhiteNoteText (midiNoteNumber));

	if (! text.isEmpty())
	{
		g.setColour (textColour);
		g.setFont (Font (jmin (12.0f, keyWidth * 0.9f)).withHorizontalScale (0.8f));

		Justification justification (Justification::centredBottom);

		if (orientation == verticalKeyboardFacingLeft)
			justification = Justification::centredLeft;
		else if (orientation == verticalKeyboardFacingRight)
			justification = Justification::centredRight;

		g.drawFittedText (text, x + 2, y + 2, w - 4, h - 4, justification, 1);
	}

	g.setColour (lineColour);

	switch (orientation)
	{
		case horizontalKeyboard:            g.fillRect (x, y, 1, h); break;
		case verticalKeyboardFacingLeft:    g.fillRect (x, y, w, 1); break;
		case verticalKeyboardFacingRight:   g.fillRect (x, y + h - 1, w, 1); break;
		default: break;
	}

	if (midiNoteNumber == rangeEnd)
	{
		switch (orientation)
		{
			case horizontalKeyboard:            g.fillRect (x + w, y, 1, h); break;
			case verticalKeyboardFacingLeft:    g.fillRect (x, y + h, w, 1); break;
			case verticalKeyboardFacingRight:   g.fillRect (x, y - 1, w, 1); break;
			default: break;
		}
	}
}

void MidiKeyboardComponent::drawBlackNote (int /*midiNoteNumber*/,
										   Graphics& g, int x, int y, int w, int h,
										   bool isDown, bool isOver,
										   const Colour& noteFillColour)
{
	Colour c (noteFillColour);

	if (isDown)
		c = c.overlaidWith (findColour (keyDownOverlayColourId));

	if (isOver)
		c = c.overlaidWith (findColour (mouseOverKeyOverlayColourId));

	g.setColour (c);
	g.fillRect (x, y, w, h);

	if (isDown)
	{
		g.setColour (noteFillColour);
		g.drawRect (x, y, w, h);
	}
	else
	{
		g.setColour (c.brighter());
		const int xIndent = jmax (1, jmin (w, h) / 8);

		switch (orientation)
		{
			case horizontalKeyboard:            g.fillRect (x + xIndent, y, w - xIndent * 2, 7 * h / 8); break;
			case verticalKeyboardFacingLeft:    g.fillRect (x + w / 8, y + xIndent, w - w / 8, h - xIndent * 2); break;
			case verticalKeyboardFacingRight:   g.fillRect (x, y + xIndent, 7 * w / 8, h - xIndent * 2); break;
			default: break;
		}
	}
}

void MidiKeyboardComponent::setOctaveForMiddleC (const int octaveNumForMiddleC_)
{
	octaveNumForMiddleC = octaveNumForMiddleC_;
	repaint();
}

String MidiKeyboardComponent::getWhiteNoteText (const int midiNoteNumber)
{
	if (keyWidth > 14.0f && midiNoteNumber % 12 == 0)
		return MidiMessage::getMidiNoteName (midiNoteNumber, true, true, octaveNumForMiddleC);

	return String::empty;
}

void MidiKeyboardComponent::drawUpDownButton (Graphics& g, int w, int h,
											  const bool isMouseOver_,
											  const bool isButtonDown,
											  const bool movesOctavesUp)
{
	g.fillAll (findColour (upDownButtonBackgroundColourId));

	float angle;

	switch (orientation)
	{
		case horizontalKeyboard:            angle = movesOctavesUp ? 0.0f  : 0.5f;  break;
		case verticalKeyboardFacingLeft:    angle = movesOctavesUp ? 0.25f : 0.75f; break;
		case verticalKeyboardFacingRight:   angle = movesOctavesUp ? 0.75f : 0.25f; break;
		default:                            jassertfalse; angle = 0; break;
	}

	Path path;
	path.addTriangle (0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f);
	path.applyTransform (AffineTransform::rotation (float_Pi * 2.0f * angle, 0.5f, 0.5f));

	g.setColour (findColour (upDownButtonArrowColourId)
				  .withAlpha (isButtonDown ? 1.0f : (isMouseOver_ ? 0.6f : 0.4f)));

	g.fillPath (path, path.getTransformToScaleToFit (1.0f, 1.0f, w - 2.0f, h - 2.0f, true));
}

void MidiKeyboardComponent::resized()
{
	int w = getWidth();
	int h = getHeight();

	if (w > 0 && h > 0)
	{
		if (orientation != horizontalKeyboard)
			std::swap (w, h);

		blackNoteLength = roundToInt (h * 0.7f);

		int kx2, kw2;
		getKeyPos (rangeEnd, kx2, kw2);

		kx2 += kw2;

		if ((int) firstKey != rangeStart)
		{
			int kx1, kw1;
			getKeyPos (rangeStart, kx1, kw1);

			if (kx2 - kx1 <= w)
			{
				firstKey = (float) rangeStart;
				sendChangeMessage();
				repaint();
			}
		}

		const bool showScrollButtons = canScroll && (((int) firstKey) > rangeStart || kx2 > w + xOffset * 2);

		scrollDown->setVisible (showScrollButtons);
		scrollUp->setVisible (showScrollButtons);

		xOffset = 0;

		if (showScrollButtons)
		{
			const int scrollButtonW = jmin (12, w / 2);

			if (orientation == horizontalKeyboard)
			{
				scrollDown->setBounds (0, 0, scrollButtonW, getHeight());
				scrollUp->setBounds (getWidth() - scrollButtonW, 0, scrollButtonW, getHeight());
			}
			else if (orientation == verticalKeyboardFacingLeft)
			{
				scrollDown->setBounds (0, 0, getWidth(), scrollButtonW);
				scrollUp->setBounds (0, getHeight() - scrollButtonW, getWidth(), scrollButtonW);
			}
			else
			{
				scrollDown->setBounds (0, getHeight() - scrollButtonW, getWidth(), scrollButtonW);
				scrollUp->setBounds (0, 0, getWidth(), scrollButtonW);
			}

			int endOfLastKey, kw;
			getKeyPos (rangeEnd, endOfLastKey, kw);
			endOfLastKey += kw;

			float mousePositionVelocity;
			const int spaceAvailable = w - scrollButtonW * 2;
			const int lastStartKey = remappedXYToNote (Point<int> (endOfLastKey - spaceAvailable, 0), mousePositionVelocity) + 1;

			if (lastStartKey >= 0 && ((int) firstKey) > lastStartKey)
			{
				firstKey = (float) jlimit (rangeStart, rangeEnd, lastStartKey);
				sendChangeMessage();
			}

			int newOffset = 0;
			getKeyPos (((int) firstKey), newOffset, kw);
			xOffset = newOffset - scrollButtonW;
		}
		else
		{
			firstKey = (float) rangeStart;
		}

		repaint();
	}
}

void MidiKeyboardComponent::handleNoteOn (MidiKeyboardState*, int /*midiChannel*/, int /*midiNoteNumber*/, float /*velocity*/)
{
	shouldCheckState = true; // (probably being called from the audio thread, so avoid blocking in here)
}

void MidiKeyboardComponent::handleNoteOff (MidiKeyboardState*, int /*midiChannel*/, int /*midiNoteNumber*/)
{
	shouldCheckState = true; // (probably being called from the audio thread, so avoid blocking in here)
}

void MidiKeyboardComponent::resetAnyKeysInUse()
{
	if (keysPressed.countNumberOfSetBits() > 0 || mouseDownNotes.size() > 0)
	{
		state.allNotesOff (midiChannel);
		keysPressed.clear();

		for (int i = mouseDownNotes.size(); --i >= 0;)
		{
			mouseDownNotes.set (i, -1);
			mouseOverNotes.set (i, -1);
		}
	}
}

void MidiKeyboardComponent::updateNoteUnderMouse (const MouseEvent& e, bool isDown)
{
	updateNoteUnderMouse (e.getPosition(), isDown, e.source.getIndex());
}

void MidiKeyboardComponent::updateNoteUnderMouse (const Point<int>& pos, bool isDown, int fingerNum)
{
	float mousePositionVelocity = 0.0f;
	const int newNote = xyToNote (pos, mousePositionVelocity);
	const int oldNote = mouseOverNotes.getUnchecked (fingerNum);

	if (oldNote != newNote)
	{
		repaintNote (oldNote);
		repaintNote (newNote);
		mouseOverNotes.set (fingerNum, newNote);
	}

	int oldNoteDown = mouseDownNotes.getUnchecked (fingerNum);

	if (isDown)
	{
		if (newNote != oldNoteDown)
		{
			if (oldNoteDown >= 0)
			{
				mouseDownNotes.set (fingerNum, -1);

				if (! mouseDownNotes.contains (oldNoteDown))
					state.noteOff (midiChannel, oldNoteDown);
			}

			if (newNote >= 0)
			{
				if (! useMousePositionForVelocity)
					mousePositionVelocity = 1.0f;

				state.noteOn (midiChannel, newNote, mousePositionVelocity * velocity);
				mouseDownNotes.set (fingerNum, newNote);
			}
		}
	}
	else if (oldNoteDown >= 0)
	{
		mouseDownNotes.set (fingerNum, -1);

		if (! mouseDownNotes.contains (oldNoteDown))
			state.noteOff (midiChannel, oldNoteDown);
	}
}

void MidiKeyboardComponent::mouseMove (const MouseEvent& e)
{
	updateNoteUnderMouse (e, false);
	shouldCheckMousePos = false;
}

void MidiKeyboardComponent::mouseDrag (const MouseEvent& e)
{
	float mousePositionVelocity;
	const int newNote = xyToNote (e.getPosition(), mousePositionVelocity);

	if (newNote >= 0)
		mouseDraggedToKey (newNote, e);

	updateNoteUnderMouse (e, true);
}

bool MidiKeyboardComponent::mouseDownOnKey (int /*midiNoteNumber*/, const MouseEvent&)
{
	return true;
}

void MidiKeyboardComponent::mouseDraggedToKey (int /*midiNoteNumber*/, const MouseEvent&)
{
}

void MidiKeyboardComponent::mouseDown (const MouseEvent& e)
{
	float mousePositionVelocity;
	const int newNote = xyToNote (e.getPosition(), mousePositionVelocity);

	if (newNote >= 0 && mouseDownOnKey (newNote, e))
	{
		updateNoteUnderMouse (e, true);
		shouldCheckMousePos = true;
	}
}

void MidiKeyboardComponent::mouseUp (const MouseEvent& e)
{
	updateNoteUnderMouse (e, false);
	shouldCheckMousePos = false;
}

void MidiKeyboardComponent::mouseEnter (const MouseEvent& e)
{
	updateNoteUnderMouse (e, false);
}

void MidiKeyboardComponent::mouseExit (const MouseEvent& e)
{
	updateNoteUnderMouse (e, false);
}

void MidiKeyboardComponent::mouseWheelMove (const MouseEvent&, float ix, float iy)
{
	const float amount = (orientation == horizontalKeyboard && ix != 0)
							? ix : (orientation == verticalKeyboardFacingLeft ? iy : -iy);

	setLowestVisibleKeyFloat (firstKey - amount * keyWidth);
}

void MidiKeyboardComponent::timerCallback()
{
	if (shouldCheckState)
	{
		shouldCheckState = false;

		for (int i = rangeStart; i <= rangeEnd; ++i)
		{
			if (keysCurrentlyDrawnDown[i] != state.isNoteOnForChannels (midiInChannelMask, i))
			{
				keysCurrentlyDrawnDown.setBit (i, state.isNoteOnForChannels (midiInChannelMask, i));
				repaintNote (i);
			}
		}
	}

	if (shouldCheckMousePos)
	{
		Desktop& desktop = Desktop::getInstance();

		for (int i = desktop.getNumMouseSources(); --i >= 0;)
		{
			MouseInputSource* source = desktop.getMouseSource (i);
			jassert (source != nullptr);
			updateNoteUnderMouse (getLocalPoint (nullptr, source->getScreenPosition()),
								  source->isDragging(), source->getIndex());
		}
	}
}

void MidiKeyboardComponent::clearKeyMappings()
{
	resetAnyKeysInUse();
	keyPressNotes.clear();
	keyPresses.clear();
}

void MidiKeyboardComponent::setKeyPressForNote (const KeyPress& key,
												const int midiNoteOffsetFromC)
{
	removeKeyPressForNote (midiNoteOffsetFromC);

	keyPressNotes.add (midiNoteOffsetFromC);
	keyPresses.add (key);
}

void MidiKeyboardComponent::removeKeyPressForNote (const int midiNoteOffsetFromC)
{
	for (int i = keyPressNotes.size(); --i >= 0;)
	{
		if (keyPressNotes.getUnchecked (i) == midiNoteOffsetFromC)
		{
			keyPressNotes.remove (i);
			keyPresses.remove (i);
		}
	}
}

void MidiKeyboardComponent::setKeyPressBaseOctave (const int newOctaveNumber)
{
	jassert (newOctaveNumber >= 0 && newOctaveNumber <= 10);

	keyMappingOctave = newOctaveNumber;
}

bool MidiKeyboardComponent::keyStateChanged (const bool /*isKeyDown*/)
{
	bool keyPressUsed = false;

	for (int i = keyPresses.size(); --i >= 0;)
	{
		const int note = 12 * keyMappingOctave + keyPressNotes.getUnchecked (i);

		if (keyPresses.getReference(i).isCurrentlyDown())
		{
			if (! keysPressed [note])
			{
				keysPressed.setBit (note);
				state.noteOn (midiChannel, note, velocity);
				keyPressUsed = true;
			}
		}
		else
		{
			if (keysPressed [note])
			{
				keysPressed.clearBit (note);
				state.noteOff (midiChannel, note);
				keyPressUsed = true;
			}
		}
	}

	return keyPressUsed;
}

void MidiKeyboardComponent::focusLost (FocusChangeType)
{
	resetAnyKeysInUse();
}

/*** End of inlined file: juce_MidiKeyboardComponent.cpp ***/


/*** Start of inlined file: juce_AudioProcessorPlayer.cpp ***/
AudioProcessorPlayer::AudioProcessorPlayer()
	: processor (nullptr),
	  sampleRate (0),
	  blockSize (0),
	  isPrepared (false),
	  numInputChans (0),
	  numOutputChans (0),
	  tempBuffer (1, 1)
{
}

AudioProcessorPlayer::~AudioProcessorPlayer()
{
	setProcessor (nullptr);
}

void AudioProcessorPlayer::setProcessor (AudioProcessor* const processorToPlay)
{
	if (processor != processorToPlay)
	{
		if (processorToPlay != nullptr && sampleRate > 0 && blockSize > 0)
		{
			processorToPlay->setPlayConfigDetails (numInputChans, numOutputChans,
												   sampleRate, blockSize);

			processorToPlay->prepareToPlay (sampleRate, blockSize);
		}

		AudioProcessor* oldOne;

		{
			const ScopedLock sl (lock);
			oldOne = isPrepared ? processor : nullptr;
			processor = processorToPlay;
			isPrepared = true;
		}

		if (oldOne != nullptr)
			oldOne->releaseResources();
	}
}

void AudioProcessorPlayer::audioDeviceIOCallback (const float** const inputChannelData,
												  const int numInputChannels,
												  float** const outputChannelData,
												  const int numOutputChannels,
												  const int numSamples)
{
	// these should have been prepared by audioDeviceAboutToStart()...
	jassert (sampleRate > 0 && blockSize > 0);

	incomingMidi.clear();
	messageCollector.removeNextBlockOfMessages (incomingMidi, numSamples);
	int i, totalNumChans = 0;

	if (numInputChannels > numOutputChannels)
	{
		// if there aren't enough output channels for the number of
		// inputs, we need to create some temporary extra ones (can't
		// use the input data in case it gets written to)
		tempBuffer.setSize (numInputChannels - numOutputChannels, numSamples,
							false, false, true);

		for (i = 0; i < numOutputChannels; ++i)
		{
			channels[totalNumChans] = outputChannelData[i];
			memcpy (channels[totalNumChans], inputChannelData[i], sizeof (float) * (size_t) numSamples);
			++totalNumChans;
		}

		for (i = numOutputChannels; i < numInputChannels; ++i)
		{
			channels[totalNumChans] = tempBuffer.getSampleData (i - numOutputChannels, 0);
			memcpy (channels[totalNumChans], inputChannelData[i], sizeof (float) * (size_t) numSamples);
			++totalNumChans;
		}
	}
	else
	{
		for (i = 0; i < numInputChannels; ++i)
		{
			channels[totalNumChans] = outputChannelData[i];
			memcpy (channels[totalNumChans], inputChannelData[i], sizeof (float) * (size_t) numSamples);
			++totalNumChans;
		}

		for (i = numInputChannels; i < numOutputChannels; ++i)
		{
			channels[totalNumChans] = outputChannelData[i];
			zeromem (channels[totalNumChans], sizeof (float) * (size_t) numSamples);
			++totalNumChans;
		}
	}

	AudioSampleBuffer buffer (channels, totalNumChans, numSamples);

	const ScopedLock sl (lock);

	if (processor != nullptr)
	{
		const ScopedLock sl2 (processor->getCallbackLock());

		if (processor->isSuspended())
		{
			for (i = 0; i < numOutputChannels; ++i)
				zeromem (outputChannelData[i], sizeof (float) * (size_t) numSamples);
		}
		else
		{
			processor->processBlock (buffer, incomingMidi);
		}
	}
}

void AudioProcessorPlayer::audioDeviceAboutToStart (AudioIODevice* device)
{
	const ScopedLock sl (lock);

	sampleRate = device->getCurrentSampleRate();
	blockSize = device->getCurrentBufferSizeSamples();
	numInputChans = device->getActiveInputChannels().countNumberOfSetBits();
	numOutputChans = device->getActiveOutputChannels().countNumberOfSetBits();

	messageCollector.reset (sampleRate);
	zeromem (channels, sizeof (channels));

	if (processor != nullptr)
	{
		if (isPrepared)
			processor->releaseResources();

		AudioProcessor* const oldProcessor = processor;
		setProcessor (nullptr);
		setProcessor (oldProcessor);
	}
}

void AudioProcessorPlayer::audioDeviceStopped()
{
	const ScopedLock sl (lock);

	if (processor != nullptr && isPrepared)
		processor->releaseResources();

	sampleRate = 0.0;
	blockSize = 0;
	isPrepared = false;
	tempBuffer.setSize (1, 1);
}

void AudioProcessorPlayer::handleIncomingMidiMessage (MidiInput*, const MidiMessage& message)
{
	messageCollector.addMessageToQueue (message);
}

/*** End of inlined file: juce_AudioProcessorPlayer.cpp ***/

// END_AUTOINCLUDE

}

/*** End of inlined file: juce_audio_utils.cpp ***/


#endif

