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

/*** Start of inlined file: juce_audio_devices.cpp ***/
#if defined (__JUCE_AUDIO_DEVICES_JUCEHEADER__) && ! JUCE_AMALGAMATED_INCLUDE
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

#include "juce_audio_devices_amalgam.h"

#if JUCE_MAC
 #define Point CarbonDummyPointName
 #define Component CarbonDummyCompName
 #import <CoreAudio/AudioHardware.h>
 #import <CoreMIDI/MIDIServices.h>
 #import <DiscRecording/DiscRecording.h>
 #undef Point
 #undef Component

#elif JUCE_IOS
 #import <AudioToolbox/AudioToolbox.h>
 #import <AVFoundation/AVFoundation.h>
 #import <CoreMIDI/MIDIServices.h>

#elif JUCE_WINDOWS
 #if JUCE_WASAPI
  #pragma warning (push)
  #pragma warning (disable: 4201)
  #include <MMReg.h>
  #include <mmdeviceapi.h>
  #include <Audioclient.h>
  #include <Audiopolicy.h>
  #include <Avrt.h>
  #include <functiondiscoverykeys.h>
  #pragma warning (pop)
 #endif

 #if JUCE_ASIO
  /* This is very frustrating - we only need to use a handful of definitions from
	 a couple of the header files in Steinberg's ASIO SDK, and it'd be easy to copy
	 about 30 lines of code into this cpp file to create a fully stand-alone ASIO
	 implementation...

	 ..unfortunately that would break Steinberg's license agreement for use of
	 their SDK, so I'm not allowed to do this.

	 This means that anyone who wants to use JUCE's ASIO abilities will have to:

	 1) Agree to Steinberg's licensing terms and download the ASIO SDK
		 (see www.steinberg.net/Steinberg/Developers.asp).

	 2) Enable this code with a global definition #define JUCE_ASIO 1.

	 3) Make sure that your header search path contains the iasiodrv.h file that
		comes with the SDK. (Only about a handful of the SDK header files are actually
		needed - so to simplify things, you could just copy these into your JUCE directory).
  */
  #include <iasiodrv.h>
 #endif

 #if JUCE_USE_CDBURNER
  /* You'll need the Platform SDK for these headers - if you don't have it and don't
	 need to use CD-burning, then you might just want to set the JUCE_USE_CDBURNER flag
	 to 0, to avoid these includes.
  */
  #include <imapi.h>
  #include <imapierror.h>
 #endif

#elif JUCE_LINUX
 #if JUCE_ALSA
  /* Got an include error here? If so, you've either not got ALSA installed, or you've
	 not got your paths set up correctly to find its header files.

	 The package you need to install to get ASLA support is "libasound2-dev".

	 If you don't have the ALSA library and don't want to build Juce with audio support,
	 just set the JUCE_ALSA flag to 0.
  */
  #include <alsa/asoundlib.h>
 #endif

 #if JUCE_JACK
  /* Got an include error here? If so, you've either not got jack-audio-connection-kit
	 installed, or you've not got your paths set up correctly to find its header files.

	 The package you need to install to get JACK support is "libjack-dev".

	 If you don't have the jack-audio-connection-kit library and don't want to build
	 Juce with low latency audio support, just set the JUCE_JACK flag to 0.
  */
  #include <jack/jack.h>
 #endif
 #undef SIZEOF

#elif JUCE_ANDROID

 #if JUCE_USE_ANDROID_OPENSLES
  #include <SLES/OpenSLES.h>
  #include <SLES/OpenSLES_Android.h>
 #endif

#endif

namespace juce
{

// START_AUTOINCLUDE audio_io/*.cpp, midi_io/*.cpp, audio_cd/*.cpp, sources/*.cpp

/*** Start of inlined file: juce_AudioDeviceManager.cpp ***/
AudioDeviceManager::AudioDeviceSetup::AudioDeviceSetup()
	: sampleRate (0),
	  bufferSize (0),
	  useDefaultInputChannels (true),
	  useDefaultOutputChannels (true)
{
}

bool AudioDeviceManager::AudioDeviceSetup::operator== (const AudioDeviceManager::AudioDeviceSetup& other) const
{
	return outputDeviceName == other.outputDeviceName
			&& inputDeviceName == other.inputDeviceName
			&& sampleRate == other.sampleRate
			&& bufferSize == other.bufferSize
			&& inputChannels == other.inputChannels
			&& useDefaultInputChannels == other.useDefaultInputChannels
			&& outputChannels == other.outputChannels
			&& useDefaultOutputChannels == other.useDefaultOutputChannels;
}

AudioDeviceManager::AudioDeviceManager()
	: numInputChansNeeded (0),
	  numOutputChansNeeded (2),
	  listNeedsScanning (true),
	  useInputNames (false),
	  inputLevelMeasurementEnabledCount (0),
	  inputLevel (0),
	  tempBuffer (2, 2),
	  cpuUsageMs (0),
	  timeToCpuScale (0)
{
	callbackHandler.owner = this;
}

AudioDeviceManager::~AudioDeviceManager()
{
	currentAudioDevice = nullptr;
	defaultMidiOutput = nullptr;
}

void AudioDeviceManager::createDeviceTypesIfNeeded()
{
	if (availableDeviceTypes.size() == 0)
	{
		createAudioDeviceTypes (availableDeviceTypes);

		while (lastDeviceTypeConfigs.size() < availableDeviceTypes.size())
			lastDeviceTypeConfigs.add (new AudioDeviceSetup());

		if (availableDeviceTypes.size() > 0)
			currentDeviceType = availableDeviceTypes.getUnchecked(0)->getTypeName();

		for (int i = 0; i < availableDeviceTypes.size(); ++i)
			availableDeviceTypes.getUnchecked(i)->addListener (&callbackHandler);
	}
}

const OwnedArray <AudioIODeviceType>& AudioDeviceManager::getAvailableDeviceTypes()
{
	scanDevicesIfNeeded();
	return availableDeviceTypes;
}

void AudioDeviceManager::audioDeviceListChanged()
{
	sendChangeMessage();
}

static void addIfNotNull (OwnedArray <AudioIODeviceType>& list, AudioIODeviceType* const device)
{
	if (device != nullptr)
		list.add (device);
}

void AudioDeviceManager::createAudioDeviceTypes (OwnedArray <AudioIODeviceType>& list)
{
	addIfNotNull (list, AudioIODeviceType::createAudioIODeviceType_WASAPI());
	addIfNotNull (list, AudioIODeviceType::createAudioIODeviceType_DirectSound());
	addIfNotNull (list, AudioIODeviceType::createAudioIODeviceType_ASIO());
	addIfNotNull (list, AudioIODeviceType::createAudioIODeviceType_CoreAudio());
	addIfNotNull (list, AudioIODeviceType::createAudioIODeviceType_iOSAudio());
	addIfNotNull (list, AudioIODeviceType::createAudioIODeviceType_ALSA());
	addIfNotNull (list, AudioIODeviceType::createAudioIODeviceType_JACK());
	addIfNotNull (list, AudioIODeviceType::createAudioIODeviceType_OpenSLES());
	addIfNotNull (list, AudioIODeviceType::createAudioIODeviceType_Android());
}

String AudioDeviceManager::initialise (const int numInputChannelsNeeded,
									   const int numOutputChannelsNeeded,
									   const XmlElement* const e,
									   const bool selectDefaultDeviceOnFailure,
									   const String& preferredDefaultDeviceName,
									   const AudioDeviceSetup* preferredSetupOptions)
{
	scanDevicesIfNeeded();

	numInputChansNeeded = numInputChannelsNeeded;
	numOutputChansNeeded = numOutputChannelsNeeded;

	if (e != nullptr && e->hasTagName ("DEVICESETUP"))
	{
		lastExplicitSettings = new XmlElement (*e);

		String error;
		AudioDeviceSetup setup;

		if (preferredSetupOptions != nullptr)
			setup = *preferredSetupOptions;

		if (e->getStringAttribute ("audioDeviceName").isNotEmpty())
		{
			setup.inputDeviceName = setup.outputDeviceName
				= e->getStringAttribute ("audioDeviceName");
		}
		else
		{
			setup.inputDeviceName = e->getStringAttribute ("audioInputDeviceName");
			setup.outputDeviceName = e->getStringAttribute ("audioOutputDeviceName");
		}

		currentDeviceType = e->getStringAttribute ("deviceType");

		if (findType (currentDeviceType) == nullptr)
		{
			AudioIODeviceType* const type = findType (setup.inputDeviceName, setup.outputDeviceName);

			if (type != nullptr)
				currentDeviceType = type->getTypeName();
			else if (availableDeviceTypes.size() > 0)
				currentDeviceType = availableDeviceTypes.getUnchecked(0)->getTypeName();
		}

		setup.bufferSize = e->getIntAttribute ("audioDeviceBufferSize");
		setup.sampleRate = e->getDoubleAttribute ("audioDeviceRate");

		setup.inputChannels .parseString (e->getStringAttribute ("audioDeviceInChans",  "11"), 2);
		setup.outputChannels.parseString (e->getStringAttribute ("audioDeviceOutChans", "11"), 2);

		setup.useDefaultInputChannels  = ! e->hasAttribute ("audioDeviceInChans");
		setup.useDefaultOutputChannels = ! e->hasAttribute ("audioDeviceOutChans");

		error = setAudioDeviceSetup (setup, true);

		midiInsFromXml.clear();
		forEachXmlChildElementWithTagName (*e, c, "MIDIINPUT")
			midiInsFromXml.add (c->getStringAttribute ("name"));

		const StringArray allMidiIns (MidiInput::getDevices());

		for (int i = allMidiIns.size(); --i >= 0;)
			setMidiInputEnabled (allMidiIns[i], midiInsFromXml.contains (allMidiIns[i]));

		if (error.isNotEmpty() && selectDefaultDeviceOnFailure)
			error = initialise (numInputChannelsNeeded, numOutputChannelsNeeded, 0,
								false, preferredDefaultDeviceName);

		setDefaultMidiOutput (e->getStringAttribute ("defaultMidiOutput"));

		return error;
	}
	else
	{
		AudioDeviceSetup setup;

		if (preferredSetupOptions != nullptr)
		{
			setup = *preferredSetupOptions;
		}
		else if (preferredDefaultDeviceName.isNotEmpty())
		{
			for (int j = availableDeviceTypes.size(); --j >= 0;)
			{
				AudioIODeviceType* const type = availableDeviceTypes.getUnchecked(j);

				StringArray outs (type->getDeviceNames (false));

				int i;
				for (i = 0; i < outs.size(); ++i)
				{
					if (outs[i].matchesWildcard (preferredDefaultDeviceName, true))
					{
						setup.outputDeviceName = outs[i];
						break;
					}
				}

				StringArray ins (type->getDeviceNames (true));

				for (i = 0; i < ins.size(); ++i)
				{
					if (ins[i].matchesWildcard (preferredDefaultDeviceName, true))
					{
						setup.inputDeviceName = ins[i];
						break;
					}
				}
			}
		}

		insertDefaultDeviceNames (setup);
		return setAudioDeviceSetup (setup, false);
	}
}

void AudioDeviceManager::insertDefaultDeviceNames (AudioDeviceSetup& setup) const
{
	AudioIODeviceType* type = getCurrentDeviceTypeObject();
	if (type != nullptr)
	{
		if (setup.outputDeviceName.isEmpty())
			setup.outputDeviceName = type->getDeviceNames (false) [type->getDefaultDeviceIndex (false)];

		if (setup.inputDeviceName.isEmpty())
			setup.inputDeviceName = type->getDeviceNames (true) [type->getDefaultDeviceIndex (true)];
	}
}

XmlElement* AudioDeviceManager::createStateXml() const
{
	return lastExplicitSettings.createCopy();
}

void AudioDeviceManager::scanDevicesIfNeeded()
{
	if (listNeedsScanning)
	{
		listNeedsScanning = false;

		createDeviceTypesIfNeeded();

		for (int i = availableDeviceTypes.size(); --i >= 0;)
			availableDeviceTypes.getUnchecked(i)->scanForDevices();
	}
}

AudioIODeviceType* AudioDeviceManager::findType (const String& typeName)
{
	scanDevicesIfNeeded();

	for (int i = availableDeviceTypes.size(); --i >= 0;)
		if (availableDeviceTypes.getUnchecked(i)->getTypeName() == typeName)
			return availableDeviceTypes.getUnchecked(i);

	return nullptr;
}

AudioIODeviceType* AudioDeviceManager::findType (const String& inputName, const String& outputName)
{
	scanDevicesIfNeeded();

	for (int i = availableDeviceTypes.size(); --i >= 0;)
	{
		AudioIODeviceType* const type = availableDeviceTypes.getUnchecked(i);

		if ((inputName.isNotEmpty() && type->getDeviceNames (true).contains (inputName, true))
			|| (outputName.isNotEmpty() && type->getDeviceNames (false).contains (outputName, true)))
		{
			return type;
		}
	}

	return nullptr;
}

void AudioDeviceManager::getAudioDeviceSetup (AudioDeviceSetup& setup)
{
	setup = currentSetup;
}

void AudioDeviceManager::deleteCurrentDevice()
{
	currentAudioDevice = nullptr;
	currentSetup.inputDeviceName = String::empty;
	currentSetup.outputDeviceName = String::empty;
}

void AudioDeviceManager::setCurrentAudioDeviceType (const String& type,
													const bool treatAsChosenDevice)
{
	for (int i = 0; i < availableDeviceTypes.size(); ++i)
	{
		if (availableDeviceTypes.getUnchecked(i)->getTypeName() == type
			 && currentDeviceType != type)
		{
			currentDeviceType = type;

			AudioDeviceSetup s (*lastDeviceTypeConfigs.getUnchecked(i));
			insertDefaultDeviceNames (s);

			setAudioDeviceSetup (s, treatAsChosenDevice);

			sendChangeMessage();
			break;
		}
	}
}

AudioIODeviceType* AudioDeviceManager::getCurrentDeviceTypeObject() const
{
	for (int i = 0; i < availableDeviceTypes.size(); ++i)
		if (availableDeviceTypes[i]->getTypeName() == currentDeviceType)
			return availableDeviceTypes[i];

	return availableDeviceTypes[0];
}

String AudioDeviceManager::setAudioDeviceSetup (const AudioDeviceSetup& newSetup,
												const bool treatAsChosenDevice)
{
	jassert (&newSetup != &currentSetup);    // this will have no effect

	if (newSetup == currentSetup && currentAudioDevice != nullptr)
		return String::empty;

	if (! (newSetup == currentSetup))
		sendChangeMessage();

	stopDevice();

	const String newInputDeviceName (numInputChansNeeded == 0 ? String::empty : newSetup.inputDeviceName);
	const String newOutputDeviceName (numOutputChansNeeded == 0 ? String::empty : newSetup.outputDeviceName);

	String error;
	AudioIODeviceType* type = getCurrentDeviceTypeObject();

	if (type == nullptr || (newInputDeviceName.isEmpty() && newOutputDeviceName.isEmpty()))
	{
		deleteCurrentDevice();

		if (treatAsChosenDevice)
			updateXml();

		return String::empty;
	}

	if (currentSetup.inputDeviceName != newInputDeviceName
		 || currentSetup.outputDeviceName != newOutputDeviceName
		 || currentAudioDevice == nullptr)
	{
		deleteCurrentDevice();
		scanDevicesIfNeeded();

		if (newOutputDeviceName.isNotEmpty()
			 && ! type->getDeviceNames (false).contains (newOutputDeviceName))
		{
			return "No such device: " + newOutputDeviceName;
		}

		if (newInputDeviceName.isNotEmpty()
			 && ! type->getDeviceNames (true).contains (newInputDeviceName))
		{
			return "No such device: " + newInputDeviceName;
		}

		currentAudioDevice = type->createDevice (newOutputDeviceName, newInputDeviceName);

		if (currentAudioDevice == nullptr)
			error = "Can't open the audio device!\n\nThis may be because another application is currently using the same device - if so, you should close any other applications and try again!";
		else
			error = currentAudioDevice->getLastError();

		if (error.isNotEmpty())
		{
			deleteCurrentDevice();
			return error;
		}

		if (newSetup.useDefaultInputChannels)
		{
			inputChannels.clear();
			inputChannels.setRange (0, numInputChansNeeded, true);
		}

		if (newSetup.useDefaultOutputChannels)
		{
			outputChannels.clear();
			outputChannels.setRange (0, numOutputChansNeeded, true);
		}

		if (newInputDeviceName.isEmpty())
			inputChannels.clear();

		if (newOutputDeviceName.isEmpty())
			outputChannels.clear();
	}

	if (! newSetup.useDefaultInputChannels)
		inputChannels = newSetup.inputChannels;

	if (! newSetup.useDefaultOutputChannels)
		outputChannels = newSetup.outputChannels;

	currentSetup = newSetup;

	currentSetup.sampleRate = chooseBestSampleRate (newSetup.sampleRate);
	currentSetup.bufferSize = chooseBestBufferSize (newSetup.bufferSize);

	error = currentAudioDevice->open (inputChannels,
									  outputChannels,
									  currentSetup.sampleRate,
									  currentSetup.bufferSize);

	if (error.isEmpty())
	{
		currentDeviceType = currentAudioDevice->getTypeName();

		currentAudioDevice->start (&callbackHandler);

		currentSetup.sampleRate = currentAudioDevice->getCurrentSampleRate();
		currentSetup.bufferSize = currentAudioDevice->getCurrentBufferSizeSamples();
		currentSetup.inputChannels = currentAudioDevice->getActiveInputChannels();
		currentSetup.outputChannels = currentAudioDevice->getActiveOutputChannels();

		for (int i = 0; i < availableDeviceTypes.size(); ++i)
			if (availableDeviceTypes.getUnchecked (i)->getTypeName() == currentDeviceType)
				*(lastDeviceTypeConfigs.getUnchecked (i)) = currentSetup;

		if (treatAsChosenDevice)
			updateXml();
	}
	else
	{
		deleteCurrentDevice();
	}

	return error;
}

double AudioDeviceManager::chooseBestSampleRate (double rate) const
{
	jassert (currentAudioDevice != nullptr);

	if (rate > 0)
		for (int i = currentAudioDevice->getNumSampleRates(); --i >= 0;)
			if (currentAudioDevice->getSampleRate (i) == rate)
				return rate;

	double lowestAbove44 = 0.0;

	for (int i = currentAudioDevice->getNumSampleRates(); --i >= 0;)
	{
		const double sr = currentAudioDevice->getSampleRate (i);

		if (sr >= 44100.0 && (lowestAbove44 < 1.0 || sr < lowestAbove44))
			lowestAbove44 = sr;
	}

	if (lowestAbove44 > 0.0)
		return lowestAbove44;

	return currentAudioDevice->getSampleRate (0);
}

int AudioDeviceManager::chooseBestBufferSize (int bufferSize) const
{
	jassert (currentAudioDevice != nullptr);

	if (bufferSize > 0)
		for (int i = currentAudioDevice->getNumBufferSizesAvailable(); --i >= 0;)
			if (currentAudioDevice->getBufferSizeSamples(i) == bufferSize)
				return bufferSize;

	return currentAudioDevice->getDefaultBufferSize();
}

void AudioDeviceManager::stopDevice()
{
	if (currentAudioDevice != nullptr)
		currentAudioDevice->stop();

	testSound = nullptr;
}

void AudioDeviceManager::closeAudioDevice()
{
	stopDevice();
	currentAudioDevice = nullptr;
}

void AudioDeviceManager::restartLastAudioDevice()
{
	if (currentAudioDevice == nullptr)
	{
		if (currentSetup.inputDeviceName.isEmpty()
			  && currentSetup.outputDeviceName.isEmpty())
		{
			// This method will only reload the last device that was running
			// before closeAudioDevice() was called - you need to actually open
			// one first, with setAudioDevice().
			jassertfalse;
			return;
		}

		AudioDeviceSetup s (currentSetup);
		setAudioDeviceSetup (s, false);
	}
}

void AudioDeviceManager::updateXml()
{
	lastExplicitSettings = new XmlElement ("DEVICESETUP");

	lastExplicitSettings->setAttribute ("deviceType", currentDeviceType);
	lastExplicitSettings->setAttribute ("audioOutputDeviceName", currentSetup.outputDeviceName);
	lastExplicitSettings->setAttribute ("audioInputDeviceName", currentSetup.inputDeviceName);

	if (currentAudioDevice != nullptr)
	{
		lastExplicitSettings->setAttribute ("audioDeviceRate", currentAudioDevice->getCurrentSampleRate());

		if (currentAudioDevice->getDefaultBufferSize() != currentAudioDevice->getCurrentBufferSizeSamples())
			lastExplicitSettings->setAttribute ("audioDeviceBufferSize", currentAudioDevice->getCurrentBufferSizeSamples());

		if (! currentSetup.useDefaultInputChannels)
			lastExplicitSettings->setAttribute ("audioDeviceInChans", currentSetup.inputChannels.toString (2));

		if (! currentSetup.useDefaultOutputChannels)
			lastExplicitSettings->setAttribute ("audioDeviceOutChans", currentSetup.outputChannels.toString (2));
	}

	for (int i = 0; i < enabledMidiInputs.size(); ++i)
	{
		XmlElement* const m = lastExplicitSettings->createNewChildElement ("MIDIINPUT");
		m->setAttribute ("name", enabledMidiInputs[i]->getName());
	}

	if (midiInsFromXml.size() > 0)
	{
		// Add any midi devices that have been enabled before, but which aren't currently
		// open because the device has been disconnected.
		const StringArray availableMidiDevices (MidiInput::getDevices());

		for (int i = 0; i < midiInsFromXml.size(); ++i)
		{
			if (! availableMidiDevices.contains (midiInsFromXml[i], true))
			{
				XmlElement* const m = lastExplicitSettings->createNewChildElement ("MIDIINPUT");
				m->setAttribute ("name", midiInsFromXml[i]);
			}
		}
	}

	if (defaultMidiOutputName.isNotEmpty())
		lastExplicitSettings->setAttribute ("defaultMidiOutput", defaultMidiOutputName);
}

void AudioDeviceManager::addAudioCallback (AudioIODeviceCallback* newCallback)
{
	{
		const ScopedLock sl (audioCallbackLock);
		if (callbacks.contains (newCallback))
			return;
	}

	if (currentAudioDevice != nullptr && newCallback != nullptr)
		newCallback->audioDeviceAboutToStart (currentAudioDevice);

	const ScopedLock sl (audioCallbackLock);
	callbacks.add (newCallback);
}

void AudioDeviceManager::removeAudioCallback (AudioIODeviceCallback* callbackToRemove)
{
	if (callbackToRemove != nullptr)
	{
		bool needsDeinitialising = currentAudioDevice != nullptr;

		{
			const ScopedLock sl (audioCallbackLock);

			needsDeinitialising = needsDeinitialising && callbacks.contains (callbackToRemove);
			callbacks.removeValue (callbackToRemove);
		}

		if (needsDeinitialising)
			callbackToRemove->audioDeviceStopped();
	}
}

void AudioDeviceManager::audioDeviceIOCallbackInt (const float** inputChannelData,
												   int numInputChannels,
												   float** outputChannelData,
												   int numOutputChannels,
												   int numSamples)
{
	const ScopedLock sl (audioCallbackLock);

	if (inputLevelMeasurementEnabledCount > 0 && numInputChannels > 0)
	{
		for (int j = 0; j < numSamples; ++j)
		{
			float s = 0;

			for (int i = 0; i < numInputChannels; ++i)
				s += std::abs (inputChannelData[i][j]);

			s /= numInputChannels;

			const double decayFactor = 0.99992;

			if (s > inputLevel)
				inputLevel = s;
			else if (inputLevel > 0.001f)
				inputLevel *= decayFactor;
			else
				inputLevel = 0;
		}
	}
	else
	{
		inputLevel = 0;
	}

	if (callbacks.size() > 0)
	{
		const double callbackStartTime = Time::getMillisecondCounterHiRes();

		tempBuffer.setSize (jmax (1, numOutputChannels), jmax (1, numSamples), false, false, true);

		callbacks.getUnchecked(0)->audioDeviceIOCallback (inputChannelData, numInputChannels,
														  outputChannelData, numOutputChannels, numSamples);

		float** const tempChans = tempBuffer.getArrayOfChannels();

		for (int i = callbacks.size(); --i > 0;)
		{
			callbacks.getUnchecked(i)->audioDeviceIOCallback (inputChannelData, numInputChannels,
															  tempChans, numOutputChannels, numSamples);

			for (int chan = 0; chan < numOutputChannels; ++chan)
			{
				const float* const src = tempChans [chan];
				float* const dst = outputChannelData [chan];

				if (src != nullptr && dst != nullptr)
					for (int j = 0; j < numSamples; ++j)
						dst[j] += src[j];
			}
		}

		const double msTaken = Time::getMillisecondCounterHiRes() - callbackStartTime;
		const double filterAmount = 0.2;
		cpuUsageMs += filterAmount * (msTaken - cpuUsageMs);
	}
	else
	{
		for (int i = 0; i < numOutputChannels; ++i)
			zeromem (outputChannelData[i], sizeof (float) * (size_t) numSamples);
	}

	if (testSound != nullptr)
	{
		const int numSamps = jmin (numSamples, testSound->getNumSamples() - testSoundPosition);
		const float* const src = testSound->getSampleData (0, testSoundPosition);

		for (int i = 0; i < numOutputChannels; ++i)
			for (int j = 0; j < numSamps; ++j)
				outputChannelData [i][j] += src[j];

		testSoundPosition += numSamps;
		if (testSoundPosition >= testSound->getNumSamples())
			testSound = nullptr;
	}
}

void AudioDeviceManager::audioDeviceAboutToStartInt (AudioIODevice* const device)
{
	cpuUsageMs = 0;

	const double sampleRate = device->getCurrentSampleRate();
	const int blockSize = device->getCurrentBufferSizeSamples();

	if (sampleRate > 0.0 && blockSize > 0)
	{
		const double msPerBlock = 1000.0 * blockSize / sampleRate;
		timeToCpuScale = (msPerBlock > 0.0) ? (1.0 / msPerBlock) : 0.0;
	}

	{
		const ScopedLock sl (audioCallbackLock);
		for (int i = callbacks.size(); --i >= 0;)
			callbacks.getUnchecked(i)->audioDeviceAboutToStart (device);
	}

	sendChangeMessage();
}

void AudioDeviceManager::audioDeviceStoppedInt()
{
	cpuUsageMs = 0;
	timeToCpuScale = 0;
	sendChangeMessage();

	const ScopedLock sl (audioCallbackLock);
	for (int i = callbacks.size(); --i >= 0;)
		callbacks.getUnchecked(i)->audioDeviceStopped();
}

double AudioDeviceManager::getCpuUsage() const
{
	return jlimit (0.0, 1.0, timeToCpuScale * cpuUsageMs);
}

void AudioDeviceManager::setMidiInputEnabled (const String& name,
											  const bool enabled)
{
	if (enabled != isMidiInputEnabled (name))
	{
		if (enabled)
		{
			const int index = MidiInput::getDevices().indexOf (name);

			if (index >= 0)
			{
				MidiInput* const midiIn = MidiInput::openDevice (index, &callbackHandler);

				if (midiIn != nullptr)
				{
					enabledMidiInputs.add (midiIn);
					midiIn->start();
				}
			}
		}
		else
		{
			for (int i = enabledMidiInputs.size(); --i >= 0;)
				if (enabledMidiInputs[i]->getName() == name)
					enabledMidiInputs.remove (i);
		}

		updateXml();
		sendChangeMessage();
	}
}

bool AudioDeviceManager::isMidiInputEnabled (const String& name) const
{
	for (int i = enabledMidiInputs.size(); --i >= 0;)
		if (enabledMidiInputs[i]->getName() == name)
			return true;

	return false;
}

void AudioDeviceManager::addMidiInputCallback (const String& name,
											   MidiInputCallback* callbackToAdd)
{
	removeMidiInputCallback (name, callbackToAdd);

	if (name.isEmpty() || isMidiInputEnabled (name))
	{
		const ScopedLock sl (midiCallbackLock);
		midiCallbacks.add (callbackToAdd);
		midiCallbackDevices.add (name);
	}
}

void AudioDeviceManager::removeMidiInputCallback (const String& name, MidiInputCallback* callbackToRemove)
{
	for (int i = midiCallbacks.size(); --i >= 0;)
	{
		if (midiCallbackDevices[i] == name && midiCallbacks.getUnchecked(i) == callbackToRemove)
		{
			const ScopedLock sl (midiCallbackLock);
			midiCallbacks.remove (i);
			midiCallbackDevices.remove (i);
		}
	}
}

void AudioDeviceManager::handleIncomingMidiMessageInt (MidiInput* source,
													   const MidiMessage& message)
{
	if (! message.isActiveSense())
	{
		const bool isDefaultSource = (source == nullptr || source == enabledMidiInputs.getFirst());

		const ScopedLock sl (midiCallbackLock);

		for (int i = midiCallbackDevices.size(); --i >= 0;)
		{
			const String name (midiCallbackDevices[i]);

			if ((isDefaultSource && name.isEmpty()) || (name.isNotEmpty() && name == source->getName()))
				midiCallbacks.getUnchecked(i)->handleIncomingMidiMessage (source, message);
		}
	}
}

void AudioDeviceManager::setDefaultMidiOutput (const String& deviceName)
{
	if (defaultMidiOutputName != deviceName)
	{
		Array <AudioIODeviceCallback*> oldCallbacks;

		{
			const ScopedLock sl (audioCallbackLock);
			oldCallbacks = callbacks;
			callbacks.clear();
		}

		if (currentAudioDevice != nullptr)
			for (int i = oldCallbacks.size(); --i >= 0;)
				oldCallbacks.getUnchecked(i)->audioDeviceStopped();

		defaultMidiOutput = nullptr;
		defaultMidiOutputName = deviceName;

		if (deviceName.isNotEmpty())
			defaultMidiOutput = MidiOutput::openDevice (MidiOutput::getDevices().indexOf (deviceName));

		if (currentAudioDevice != nullptr)
			for (int i = oldCallbacks.size(); --i >= 0;)
				oldCallbacks.getUnchecked(i)->audioDeviceAboutToStart (currentAudioDevice);

		{
			const ScopedLock sl (audioCallbackLock);
			callbacks = oldCallbacks;
		}

		updateXml();
		sendChangeMessage();
	}
}

void AudioDeviceManager::CallbackHandler::audioDeviceIOCallback (const float** inputChannelData,
																 int numInputChannels,
																 float** outputChannelData,
																 int numOutputChannels,
																 int numSamples)
{
	owner->audioDeviceIOCallbackInt (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
}

void AudioDeviceManager::CallbackHandler::audioDeviceAboutToStart (AudioIODevice* device)
{
	owner->audioDeviceAboutToStartInt (device);
}

void AudioDeviceManager::CallbackHandler::audioDeviceStopped()
{
	owner->audioDeviceStoppedInt();
}

void AudioDeviceManager::CallbackHandler::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
	owner->handleIncomingMidiMessageInt (source, message);
}

void AudioDeviceManager::CallbackHandler::audioDeviceListChanged()
{
	owner->audioDeviceListChanged();
}

void AudioDeviceManager::playTestSound()
{
	{ // cunningly nested to swap, unlock and delete in that order.
		ScopedPointer <AudioSampleBuffer> oldSound;

		{
			const ScopedLock sl (audioCallbackLock);
			oldSound = testSound;
		}
	}

	testSoundPosition = 0;

	if (currentAudioDevice != nullptr)
	{
		const double sampleRate = currentAudioDevice->getCurrentSampleRate();
		const int soundLength = (int) sampleRate;

		AudioSampleBuffer* const newSound = new AudioSampleBuffer (1, soundLength);
		float* samples = newSound->getSampleData (0);

		const double frequency = MidiMessage::getMidiNoteInHertz (80);
		const float amplitude = 0.5f;

		const double phasePerSample = double_Pi * 2.0 / (sampleRate / frequency);

		for (int i = 0; i < soundLength; ++i)
			samples[i] = amplitude * (float) std::sin (i * phasePerSample);

		newSound->applyGainRamp (0, 0, soundLength / 10, 0.0f, 1.0f);
		newSound->applyGainRamp (0, soundLength - soundLength / 4, soundLength / 4, 1.0f, 0.0f);

		const ScopedLock sl (audioCallbackLock);
		testSound = newSound;
	}
}

void AudioDeviceManager::enableInputLevelMeasurement (const bool enableMeasurement)
{
	const ScopedLock sl (audioCallbackLock);

	if (enableMeasurement)
		++inputLevelMeasurementEnabledCount;
	else
		--inputLevelMeasurementEnabledCount;

	inputLevel = 0;
}

double AudioDeviceManager::getCurrentInputLevel() const
{
	jassert (inputLevelMeasurementEnabledCount > 0); // you need to call enableInputLevelMeasurement() before using this!
	return inputLevel;
}

/*** End of inlined file: juce_AudioDeviceManager.cpp ***/



/*** Start of inlined file: juce_AudioIODevice.cpp ***/
AudioIODevice::AudioIODevice (const String& deviceName, const String& typeName_)
	: name (deviceName),
	  typeName (typeName_)
{
}

AudioIODevice::~AudioIODevice()
{
}

bool AudioIODevice::hasControlPanel() const
{
	return false;
}

bool AudioIODevice::showControlPanel()
{
	jassertfalse;    // this should only be called for devices which return true from
					 // their hasControlPanel() method.
	return false;
}

void AudioIODeviceCallback::audioDeviceError (const String&) {}

/*** End of inlined file: juce_AudioIODevice.cpp ***/


/*** Start of inlined file: juce_AudioIODeviceType.cpp ***/
AudioIODeviceType::AudioIODeviceType (const String& name)
	: typeName (name)
{
}

AudioIODeviceType::~AudioIODeviceType()
{
}

void AudioIODeviceType::addListener (Listener* l)      { listeners.add (l); }
void AudioIODeviceType::removeListener (Listener* l)   { listeners.remove (l); }

void AudioIODeviceType::callDeviceChangeListeners()
{
	listeners.call (&AudioIODeviceType::Listener::audioDeviceListChanged);
}

#if ! JUCE_MAC
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_CoreAudio()       { return nullptr; }
#endif

#if ! JUCE_IOS
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_iOSAudio()        { return nullptr; }
#endif

#if ! (JUCE_WINDOWS && JUCE_WASAPI)
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_WASAPI()          { return nullptr; }
#endif

#if ! (JUCE_WINDOWS && JUCE_DIRECTSOUND)
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_DirectSound()     { return nullptr; }
#endif

#if ! (JUCE_WINDOWS && JUCE_ASIO)
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_ASIO()            { return nullptr; }
#endif

#if ! (JUCE_LINUX && JUCE_ALSA)
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_ALSA()            { return nullptr; }
#endif

#if ! (JUCE_LINUX && JUCE_JACK)
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_JACK()            { return nullptr; }
#endif

#if ! JUCE_ANDROID
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_Android()         { return nullptr; }
#endif

#if ! (JUCE_ANDROID && JUCE_USE_ANDROID_OPENSLES)
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_OpenSLES()        { return nullptr; }
#endif

/*** End of inlined file: juce_AudioIODeviceType.cpp ***/


/*** Start of inlined file: juce_MidiMessageCollector.cpp ***/
MidiMessageCollector::MidiMessageCollector()
	: lastCallbackTime (0),
	  sampleRate (44100.0001)
{
}

MidiMessageCollector::~MidiMessageCollector()
{
}

void MidiMessageCollector::reset (const double sampleRate_)
{
	jassert (sampleRate_ > 0);

	const ScopedLock sl (midiCallbackLock);
	sampleRate = sampleRate_;
	incomingMessages.clear();
	lastCallbackTime = Time::getMillisecondCounterHiRes();
}

void MidiMessageCollector::addMessageToQueue (const MidiMessage& message)
{
	// you need to call reset() to set the correct sample rate before using this object
	jassert (sampleRate != 44100.0001);

	// the messages that come in here need to be time-stamped correctly - see MidiInput
	// for details of what the number should be.
	jassert (message.getTimeStamp() != 0);

	const ScopedLock sl (midiCallbackLock);

	const int sampleNumber
		= (int) ((message.getTimeStamp() - 0.001 * lastCallbackTime) * sampleRate);

	incomingMessages.addEvent (message, sampleNumber);

	// if the messages don't get used for over a second, we'd better
	// get rid of any old ones to avoid the queue getting too big
	if (sampleNumber > sampleRate)
		incomingMessages.clear (0, sampleNumber - (int) sampleRate);
}

void MidiMessageCollector::removeNextBlockOfMessages (MidiBuffer& destBuffer,
													  const int numSamples)
{
	// you need to call reset() to set the correct sample rate before using this object
	jassert (sampleRate != 44100.0001);

	const double timeNow = Time::getMillisecondCounterHiRes();
	const double msElapsed = timeNow - lastCallbackTime;

	const ScopedLock sl (midiCallbackLock);
	lastCallbackTime = timeNow;

	if (! incomingMessages.isEmpty())
	{
		int numSourceSamples = jmax (1, roundToInt (msElapsed * 0.001 * sampleRate));

		int startSample = 0;
		int scale = 1 << 16;

		const uint8* midiData;
		int numBytes, samplePosition;

		MidiBuffer::Iterator iter (incomingMessages);

		if (numSourceSamples > numSamples)
		{
			// if our list of events is longer than the buffer we're being
			// asked for, scale them down to squeeze them all in..
			const int maxBlockLengthToUse = numSamples << 5;

			if (numSourceSamples > maxBlockLengthToUse)
			{
				startSample = numSourceSamples - maxBlockLengthToUse;
				numSourceSamples = maxBlockLengthToUse;
				iter.setNextSamplePosition (startSample);
			}

			scale = (numSamples << 10) / numSourceSamples;

			while (iter.getNextEvent (midiData, numBytes, samplePosition))
			{
				samplePosition = ((samplePosition - startSample) * scale) >> 10;

				destBuffer.addEvent (midiData, numBytes,
									 jlimit (0, numSamples - 1, samplePosition));
			}
		}
		else
		{
			// if our event list is shorter than the number we need, put them
			// towards the end of the buffer
			startSample = numSamples - numSourceSamples;

			while (iter.getNextEvent (midiData, numBytes, samplePosition))
			{
				destBuffer.addEvent (midiData, numBytes,
									 jlimit (0, numSamples - 1, samplePosition + startSample));
			}
		}

		incomingMessages.clear();
	}
}

void MidiMessageCollector::handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
	MidiMessage m (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
	m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);

	addMessageToQueue (m);
}

void MidiMessageCollector::handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber)
{
	MidiMessage m (MidiMessage::noteOff (midiChannel, midiNoteNumber));
	m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);

	addMessageToQueue (m);
}

void MidiMessageCollector::handleIncomingMidiMessage (MidiInput*, const MidiMessage& message)
{
	addMessageToQueue (message);
}

/*** End of inlined file: juce_MidiMessageCollector.cpp ***/


/*** Start of inlined file: juce_MidiOutput.cpp ***/
struct MidiOutput::PendingMessage
{
	PendingMessage (const void* const data, const int len, const double timeStamp)
		: message (data, len, timeStamp)
	{}

	MidiMessage message;
	PendingMessage* next;
};

MidiOutput::MidiOutput()
	: Thread ("midi out"),
	  internal (nullptr),
	  firstMessage (nullptr)
{
}

void MidiOutput::sendBlockOfMessages (const MidiBuffer& buffer,
									  const double millisecondCounterToStartAt,
									  double samplesPerSecondForBuffer)
{
	// You've got to call startBackgroundThread() for this to actually work..
	jassert (isThreadRunning());

	// this needs to be a value in the future - RTFM for this method!
	jassert (millisecondCounterToStartAt > 0);

	const double timeScaleFactor = 1000.0 / samplesPerSecondForBuffer;

	MidiBuffer::Iterator i (buffer);

	const uint8* data;
	int len, time;

	while (i.getNextEvent (data, len, time))
	{
		const double eventTime = millisecondCounterToStartAt + timeScaleFactor * time;

		PendingMessage* const m = new PendingMessage (data, len, eventTime);

		const ScopedLock sl (lock);

		if (firstMessage == nullptr || firstMessage->message.getTimeStamp() > eventTime)
		{
			m->next = firstMessage;
			firstMessage = m;
		}
		else
		{
			PendingMessage* mm = firstMessage;

			while (mm->next != nullptr && mm->next->message.getTimeStamp() <= eventTime)
				mm = mm->next;

			m->next = mm->next;
			mm->next = m;
		}
	}

	notify();
}

void MidiOutput::clearAllPendingMessages()
{
	const ScopedLock sl (lock);

	while (firstMessage != nullptr)
	{
		PendingMessage* const m = firstMessage;
		firstMessage = firstMessage->next;
		delete m;
	}
}

void MidiOutput::startBackgroundThread()
{
	startThread (9);
}

void MidiOutput::stopBackgroundThread()
{
	stopThread (5000);
}

void MidiOutput::run()
{
	while (! threadShouldExit())
	{
		uint32 now = Time::getMillisecondCounter();
		uint32 eventTime = 0;
		uint32 timeToWait = 500;

		PendingMessage* message;

		{
			const ScopedLock sl (lock);
			message = firstMessage;

			if (message != nullptr)
			{
				eventTime = (uint32) roundToInt (message->message.getTimeStamp());

				if (eventTime > now + 20)
				{
					timeToWait = eventTime - (now + 20);
					message = nullptr;
				}
				else
				{
					firstMessage = message->next;
				}
			}
		}

		if (message != nullptr)
		{
			const ScopedPointer<PendingMessage> messageDeleter (message);

			if (eventTime > now)
			{
				Time::waitForMillisecondCounter (eventTime);

				if (threadShouldExit())
					break;
			}

			if (eventTime > now - 200)
				sendMessageNow (message->message);
		}
		else
		{
			jassert (timeToWait < 1000 * 30);
			wait ((int) timeToWait);
		}
	}

	clearAllPendingMessages();
}

/*** End of inlined file: juce_MidiOutput.cpp ***/


/*** Start of inlined file: juce_AudioCDReader.cpp ***/
#if JUCE_USE_CDREADER

int AudioCDReader::getNumTracks() const
{
	return trackStartSamples.size() - 1;
}

int AudioCDReader::getPositionOfTrackStart (int trackNum) const
{
	return trackStartSamples [trackNum];
}

const Array<int>& AudioCDReader::getTrackOffsets() const
{
	return trackStartSamples;
}

int AudioCDReader::getCDDBId()
{
	int checksum = 0;
	const int numTracks = getNumTracks();

	for (int i = 0; i < numTracks; ++i)
		for (int offset = (trackStartSamples.getUnchecked(i) + 88200) / 44100; offset > 0; offset /= 10)
			checksum += offset % 10;

	const int length = (trackStartSamples.getLast() - trackStartSamples.getFirst()) / 44100;

	// CCLLLLTT: checksum, length, tracks
	return ((checksum & 0xff) << 24) | (length << 8) | numTracks;
}

#endif

/*** End of inlined file: juce_AudioCDReader.cpp ***/


/*** Start of inlined file: juce_AudioSourcePlayer.cpp ***/
AudioSourcePlayer::AudioSourcePlayer()
	: source (nullptr),
	  sampleRate (0),
	  bufferSize (0),
	  tempBuffer (2, 8),
	  lastGain (1.0f),
	  gain (1.0f)
{
}

AudioSourcePlayer::~AudioSourcePlayer()
{
	setSource (nullptr);
}

void AudioSourcePlayer::setSource (AudioSource* newSource)
{
	if (source != newSource)
	{
		AudioSource* const oldSource = source;

		if (newSource != nullptr && bufferSize > 0 && sampleRate > 0)
			newSource->prepareToPlay (bufferSize, sampleRate);

		{
			const ScopedLock sl (readLock);
			source = newSource;
		}

		if (oldSource != nullptr)
			oldSource->releaseResources();
	}
}

void AudioSourcePlayer::setGain (const float newGain) noexcept
{
	gain = newGain;
}

void AudioSourcePlayer::audioDeviceIOCallback (const float** inputChannelData,
											   int totalNumInputChannels,
											   float** outputChannelData,
											   int totalNumOutputChannels,
											   int numSamples)
{
	// these should have been prepared by audioDeviceAboutToStart()...
	jassert (sampleRate > 0 && bufferSize > 0);

	const ScopedLock sl (readLock);

	if (source != nullptr)
	{
		int i, numActiveChans = 0, numInputs = 0, numOutputs = 0;

		// messy stuff needed to compact the channels down into an array
		// of non-zero pointers..
		for (i = 0; i < totalNumInputChannels; ++i)
		{
			if (inputChannelData[i] != nullptr)
			{
				inputChans [numInputs++] = inputChannelData[i];
				if (numInputs >= numElementsInArray (inputChans))
					break;
			}
		}

		for (i = 0; i < totalNumOutputChannels; ++i)
		{
			if (outputChannelData[i] != nullptr)
			{
				outputChans [numOutputs++] = outputChannelData[i];
				if (numOutputs >= numElementsInArray (outputChans))
					break;
			}
		}

		if (numInputs > numOutputs)
		{
			// if there aren't enough output channels for the number of
			// inputs, we need to create some temporary extra ones (can't
			// use the input data in case it gets written to)
			tempBuffer.setSize (numInputs - numOutputs, numSamples,
								false, false, true);

			for (i = 0; i < numOutputs; ++i)
			{
				channels[numActiveChans] = outputChans[i];
				memcpy (channels[numActiveChans], inputChans[i], sizeof (float) * (size_t) numSamples);
				++numActiveChans;
			}

			for (i = numOutputs; i < numInputs; ++i)
			{
				channels[numActiveChans] = tempBuffer.getSampleData (i - numOutputs, 0);
				memcpy (channels[numActiveChans], inputChans[i], sizeof (float) * (size_t) numSamples);
				++numActiveChans;
			}
		}
		else
		{
			for (i = 0; i < numInputs; ++i)
			{
				channels[numActiveChans] = outputChans[i];
				memcpy (channels[numActiveChans], inputChans[i], sizeof (float) * (size_t) numSamples);
				++numActiveChans;
			}

			for (i = numInputs; i < numOutputs; ++i)
			{
				channels[numActiveChans] = outputChans[i];
				zeromem (channels[numActiveChans], sizeof (float) * (size_t) numSamples);
				++numActiveChans;
			}
		}

		AudioSampleBuffer buffer (channels, numActiveChans, numSamples);

		AudioSourceChannelInfo info (&buffer, 0, numSamples);
		source->getNextAudioBlock (info);

		for (i = info.buffer->getNumChannels(); --i >= 0;)
			buffer.applyGainRamp (i, info.startSample, info.numSamples, lastGain, gain);

		lastGain = gain;
	}
	else
	{
		for (int i = 0; i < totalNumOutputChannels; ++i)
			if (outputChannelData[i] != nullptr)
				zeromem (outputChannelData[i], sizeof (float) * (size_t) numSamples);
	}
}

void AudioSourcePlayer::audioDeviceAboutToStart (AudioIODevice* device)
{
	sampleRate = device->getCurrentSampleRate();
	bufferSize = device->getCurrentBufferSizeSamples();
	zeromem (channels, sizeof (channels));

	if (source != nullptr)
		source->prepareToPlay (bufferSize, sampleRate);
}

void AudioSourcePlayer::audioDeviceStopped()
{
	if (source != nullptr)
		source->releaseResources();

	sampleRate = 0.0;
	bufferSize = 0;

	tempBuffer.setSize (2, 8);
}

/*** End of inlined file: juce_AudioSourcePlayer.cpp ***/


/*** Start of inlined file: juce_AudioTransportSource.cpp ***/
AudioTransportSource::AudioTransportSource()
	: source (nullptr),
	  resamplerSource (nullptr),
	  bufferingSource (nullptr),
	  positionableSource (nullptr),
	  masterSource (nullptr),
	  gain (1.0f),
	  lastGain (1.0f),
	  playing (false),
	  stopped (true),
	  sampleRate (44100.0),
	  sourceSampleRate (0.0),
	  blockSize (128),
	  readAheadBufferSize (0),
	  isPrepared (false),
	  inputStreamEOF (false)
{
}

AudioTransportSource::~AudioTransportSource()
{
	setSource (nullptr);

	releaseMasterResources();
}

void AudioTransportSource::setSource (PositionableAudioSource* const newSource,
									  int readAheadBufferSize_,
									  TimeSliceThread* readAheadThread,
									  double sourceSampleRateToCorrectFor,
									  int maxNumChannels)
{
	if (source == newSource)
	{
		if (source == nullptr)
			return;

		setSource (nullptr, 0, nullptr); // deselect and reselect to avoid releasing resources wrongly
	}

	readAheadBufferSize = readAheadBufferSize_;
	sourceSampleRate = sourceSampleRateToCorrectFor;

	ResamplingAudioSource* newResamplerSource = nullptr;
	BufferingAudioSource* newBufferingSource = nullptr;
	PositionableAudioSource* newPositionableSource = nullptr;
	AudioSource* newMasterSource = nullptr;

	ScopedPointer <ResamplingAudioSource> oldResamplerSource (resamplerSource);
	ScopedPointer <BufferingAudioSource> oldBufferingSource (bufferingSource);
	AudioSource* oldMasterSource = masterSource;

	if (newSource != nullptr)
	{
		newPositionableSource = newSource;

		if (readAheadBufferSize_ > 0)
		{
			// If you want to use a read-ahead buffer, you must also provide a TimeSliceThread
			// for it to use!
			jassert (readAheadThread != nullptr);

			newPositionableSource = newBufferingSource
				= new BufferingAudioSource (newPositionableSource, *readAheadThread,
											false, readAheadBufferSize_, maxNumChannels);
		}

		newPositionableSource->setNextReadPosition (0);

		if (sourceSampleRateToCorrectFor > 0)
			newMasterSource = newResamplerSource
				= new ResamplingAudioSource (newPositionableSource, false, maxNumChannels);
		else
			newMasterSource = newPositionableSource;

		if (isPrepared)
		{
			if (newResamplerSource != nullptr && sourceSampleRate > 0 && sampleRate > 0)
				newResamplerSource->setResamplingRatio (sourceSampleRate / sampleRate);

			newMasterSource->prepareToPlay (blockSize, sampleRate);
		}
	}

	{
		const ScopedLock sl (callbackLock);

		source = newSource;
		resamplerSource = newResamplerSource;
		bufferingSource = newBufferingSource;
		masterSource = newMasterSource;
		positionableSource = newPositionableSource;

		playing = false;
	}

	if (oldMasterSource != nullptr)
		oldMasterSource->releaseResources();
}

void AudioTransportSource::start()
{
	if ((! playing) && masterSource != nullptr)
	{
		{
			const ScopedLock sl (callbackLock);
			playing = true;
			stopped = false;
			inputStreamEOF = false;
		}

		sendChangeMessage();
	}
}

void AudioTransportSource::stop()
{
	if (playing)
	{
		{
			const ScopedLock sl (callbackLock);
			playing = false;
		}

		int n = 500;
		while (--n >= 0 && ! stopped)
			Thread::sleep (2);

		sendChangeMessage();
	}
}

void AudioTransportSource::setPosition (double newPosition)
{
	if (sampleRate > 0.0)
		setNextReadPosition ((int64) (newPosition * sampleRate));
}

double AudioTransportSource::getCurrentPosition() const
{
	if (sampleRate > 0.0)
		return getNextReadPosition() / sampleRate;
	else
		return 0.0;
}

double AudioTransportSource::getLengthInSeconds() const
{
	return getTotalLength() / sampleRate;
}

void AudioTransportSource::setNextReadPosition (int64 newPosition)
{
	if (positionableSource != nullptr)
	{
		if (sampleRate > 0 && sourceSampleRate > 0)
			newPosition = (int64) (newPosition * sourceSampleRate / sampleRate);

		positionableSource->setNextReadPosition (newPosition);
	}
}

int64 AudioTransportSource::getNextReadPosition() const
{
	if (positionableSource != nullptr)
	{
		const double ratio = (sampleRate > 0 && sourceSampleRate > 0) ? sampleRate / sourceSampleRate : 1.0;

		return (int64) (positionableSource->getNextReadPosition() * ratio);
	}

	return 0;
}

int64 AudioTransportSource::getTotalLength() const
{
	const ScopedLock sl (callbackLock);

	if (positionableSource != nullptr)
	{
		const double ratio = (sampleRate > 0 && sourceSampleRate > 0) ? sampleRate / sourceSampleRate : 1.0;

		return (int64) (positionableSource->getTotalLength() * ratio);
	}

	return 0;
}

bool AudioTransportSource::isLooping() const
{
	const ScopedLock sl (callbackLock);

	return positionableSource != nullptr
			&& positionableSource->isLooping();
}

void AudioTransportSource::setGain (const float newGain) noexcept
{
	gain = newGain;
}

void AudioTransportSource::prepareToPlay (int samplesPerBlockExpected,
										  double sampleRate_)
{
	const ScopedLock sl (callbackLock);

	sampleRate = sampleRate_;
	blockSize = samplesPerBlockExpected;

	if (masterSource != nullptr)
		masterSource->prepareToPlay (samplesPerBlockExpected, sampleRate);

	if (resamplerSource != nullptr && sourceSampleRate > 0)
		resamplerSource->setResamplingRatio (sourceSampleRate / sampleRate);

	isPrepared = true;
}

void AudioTransportSource::releaseMasterResources()
{
	const ScopedLock sl (callbackLock);

	if (masterSource != nullptr)
		masterSource->releaseResources();

	isPrepared = false;
}

void AudioTransportSource::releaseResources()
{
	releaseMasterResources();
}

void AudioTransportSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
	const ScopedLock sl (callbackLock);

	inputStreamEOF = false;

	if (masterSource != nullptr && ! stopped)
	{
		masterSource->getNextAudioBlock (info);

		if (! playing)
		{
			// just stopped playing, so fade out the last block..
			for (int i = info.buffer->getNumChannels(); --i >= 0;)
				info.buffer->applyGainRamp (i, info.startSample, jmin (256, info.numSamples), 1.0f, 0.0f);

			if (info.numSamples > 256)
				info.buffer->clear (info.startSample + 256, info.numSamples - 256);
		}

		if (positionableSource->getNextReadPosition() > positionableSource->getTotalLength() + 1
			 && ! positionableSource->isLooping())
		{
			playing = false;
			inputStreamEOF = true;
			sendChangeMessage();
		}

		stopped = ! playing;

		for (int i = info.buffer->getNumChannels(); --i >= 0;)
		{
			info.buffer->applyGainRamp (i, info.startSample, info.numSamples,
										lastGain, gain);
		}
	}
	else
	{
		info.clearActiveBufferRegion();
		stopped = true;
	}

	lastGain = gain;
}

/*** End of inlined file: juce_AudioTransportSource.cpp ***/

// END_AUTOINCLUDE

}

using namespace juce;

namespace juce
{

/*** Start of inlined file: juce_MidiDataConcatenator.h ***/
#ifndef __JUCE_MIDIDATACONCATENATOR_JUCEHEADER__
#define __JUCE_MIDIDATACONCATENATOR_JUCEHEADER__

/**
	Helper class that takes chunks of incoming midi bytes, packages them into
	messages, and dispatches them to a midi callback.
*/
class MidiDataConcatenator
{
public:

	MidiDataConcatenator (const int initialBufferSize)
		: pendingData ((size_t) initialBufferSize),
		  pendingBytes (0), pendingDataTime (0)
	{
	}

	void reset()
	{
		pendingBytes = 0;
		pendingDataTime = 0;
	}

	void pushMidiData (const void* data, int numBytes, double time,
					   MidiInput* input, MidiInputCallback& callback)
	{
		const uint8* d = static_cast <const uint8*> (data);

		while (numBytes > 0)
		{
			if (pendingBytes > 0 || d[0] == 0xf0)
			{
				processSysex (d, numBytes, time, input, callback);
			}
			else
			{
				int used = 0;
				const MidiMessage m (d, numBytes, used, 0, time);

				if (used <= 0)
					break; // malformed message..

				callback.handleIncomingMidiMessage (input, m);
				numBytes -= used;
				d += used;
			}
		}
	}

private:
	void processSysex (const uint8*& d, int& numBytes, double time,
					   MidiInput* input, MidiInputCallback& callback)
	{
		if (*d == 0xf0)
		{
			pendingBytes = 0;
			pendingDataTime = time;
		}

		pendingData.ensureSize ((size_t) (pendingBytes + numBytes), false);
		uint8* totalMessage = static_cast<uint8*> (pendingData.getData());
		uint8* dest = totalMessage + pendingBytes;

		do
		{
			if (pendingBytes > 0 && *d >= 0x80)
			{
				if (*d >= 0xfa || *d == 0xf8)
				{
					callback.handleIncomingMidiMessage (input, MidiMessage (*d, time));
					++d;
					--numBytes;
				}
				else
				{
					if (*d == 0xf7)
					{
						*dest++ = *d++;
						pendingBytes++;
						--numBytes;
					}

					break;
				}
			}
			else
			{
				*dest++ = *d++;
				pendingBytes++;
				--numBytes;
			}
		}
		while (numBytes > 0);

		if (pendingBytes > 0)
		{
			if (totalMessage [pendingBytes - 1] == 0xf7)
			{
				callback.handleIncomingMidiMessage (input, MidiMessage (totalMessage, pendingBytes, pendingDataTime));
				pendingBytes = 0;
			}
			else
			{
				callback.handlePartialSysexMessage (input, totalMessage, pendingBytes, pendingDataTime);
			}
		}
	}

	MemoryBlock pendingData;
	int pendingBytes;
	double pendingDataTime;

	JUCE_DECLARE_NON_COPYABLE (MidiDataConcatenator);
};

#endif   // __JUCE_MIDIDATACONCATENATOR_JUCEHEADER__

/*** End of inlined file: juce_MidiDataConcatenator.h ***/


#if JUCE_MAC

/*** Start of inlined file: juce_osx_ObjCHelpers.h ***/
#ifndef __JUCE_OSX_OBJCHELPERS_JUCEHEADER__
#define __JUCE_OSX_OBJCHELPERS_JUCEHEADER__

/* This file contains a few helper functions that are used internally but which
   need to be kept away from the public headers because they use obj-C symbols.
*/
namespace
{

	String nsStringToJuce (NSString* s)
	{
		return CharPointer_UTF8 ([s UTF8String]);
	}

	NSString* juceStringToNS (const String& s)
	{
		return [NSString stringWithUTF8String: s.toUTF8()];
	}

	NSString* nsStringLiteral (const char* const s) noexcept
	{
		return [NSString stringWithUTF8String: s];
	}

	NSString* nsEmptyString() noexcept
	{
		return [NSString string];
	}
}

#endif   // __JUCE_OSX_OBJCHELPERS_JUCEHEADER__

/*** End of inlined file: juce_osx_ObjCHelpers.h ***/



/*** Start of inlined file: juce_mac_ObjCSuffix.h ***/
#ifndef __JUCE_MAC_OBJCSUFFIX_JUCEHEADER__
#define __JUCE_MAC_OBJCSUFFIX_JUCEHEADER__

/** This suffix is used for naming all Obj-C classes that are used inside juce.

	Because of the flat naming structure used by Obj-C, you can get horrible situations where
	two DLLs are loaded into a host, each of which uses classes with the same names, and these get
	cross-linked so that when you make a call to a class that you thought was private, it ends up
	actually calling into a similarly named class in the other module's address space.

	By changing this macro to a unique value, you ensure that all the obj-C classes in your app
	have unique names, and should avoid this problem.

	If you're using the amalgamated version, you can just set this macro to something unique before
	you include juce_amalgamated.cpp.
*/
#ifndef JUCE_ObjCExtraSuffix
 #define JUCE_ObjCExtraSuffix 3
#endif

#ifndef DOXYGEN
 #define appendMacro1(a, b, c, d, e) a ## _ ## b ## _ ## c ## _ ## d ## _ ## e
 #define appendMacro2(a, b, c, d, e) appendMacro1(a, b, c, d, e)
 #define MakeObjCClassName(rootName) appendMacro2 (rootName, JUCE_MAJOR_VERSION, JUCE_MINOR_VERSION, JUCE_BUILDNUMBER, JUCE_ObjCExtraSuffix)
#endif

#endif   // __JUCE_MAC_OBJCSUFFIX_JUCEHEADER__

/*** End of inlined file: juce_mac_ObjCSuffix.h ***/


/*** Start of inlined file: juce_mac_CoreAudio.cpp ***/
#if JUCE_COREAUDIO_LOGGING_ENABLED
 #define JUCE_COREAUDIOLOG(a) Logger::writeToLog (a)
#else
 #define JUCE_COREAUDIOLOG(a)
#endif

class CoreAudioInternal  : public Timer
{
public:

	CoreAudioInternal (AudioDeviceID id)
	   : inputLatency (0),
		 outputLatency (0),
		 callback (nullptr),
		#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
		 audioProcID (0),
		#endif
		 isSlaveDevice (false),
		 deviceID (id),
		 started (false),
		 sampleRate (0),
		 bufferSize (512),
		 numInputChans (0),
		 numOutputChans (0),
		 callbacksAllowed (true),
		 numInputChannelInfos (0),
		 numOutputChannelInfos (0)
	{
		jassert (deviceID != 0);

		updateDetailsFromDevice();

		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioObjectPropertySelectorWildcard;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementWildcard;

		AudioObjectAddPropertyListener (deviceID, &pa, deviceListenerProc, this);
	}

	~CoreAudioInternal()
	{
		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioObjectPropertySelectorWildcard;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementWildcard;

		AudioObjectRemovePropertyListener (deviceID, &pa, deviceListenerProc, this);

		stop (false);
	}

	void allocateTempBuffers()
	{
		const int tempBufSize = bufferSize + 4;
		audioBuffer.calloc ((size_t) ((numInputChans + numOutputChans) * tempBufSize));

		tempInputBuffers.calloc ((size_t) numInputChans + 2);
		tempOutputBuffers.calloc ((size_t) numOutputChans + 2);

		int i, count = 0;
		for (i = 0; i < numInputChans; ++i)
			tempInputBuffers[i] = audioBuffer + count++ * tempBufSize;

		for (i = 0; i < numOutputChans; ++i)
			tempOutputBuffers[i] = audioBuffer + count++ * tempBufSize;
	}

	// returns the number of actual available channels
	void fillInChannelInfo (const bool input)
	{
		int chanNum = 0;
		UInt32 size;

		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioDevicePropertyStreamConfiguration;
		pa.mScope = input ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;
		pa.mElement = kAudioObjectPropertyElementMaster;

		if (OK (AudioObjectGetPropertyDataSize (deviceID, &pa, 0, 0, &size)))
		{
			HeapBlock <AudioBufferList> bufList;
			bufList.calloc (size, 1);

			if (OK (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, bufList)))
			{
				const int numStreams = (int) bufList->mNumberBuffers;

				for (int i = 0; i < numStreams; ++i)
				{
					const AudioBuffer& b = bufList->mBuffers[i];

					for (unsigned int j = 0; j < b.mNumberChannels; ++j)
					{
						String name;

						{
							char channelName [256] = { 0 };
							UInt32 nameSize = sizeof (channelName);
							UInt32 channelNum = (UInt32) chanNum + 1;
							pa.mSelector = kAudioDevicePropertyChannelName;

							if (AudioObjectGetPropertyData (deviceID, &pa, sizeof (channelNum), &channelNum, &nameSize, channelName) == noErr)
								name = String::fromUTF8 (channelName, (int) nameSize);
						}

						if (input)
						{
							if (activeInputChans[chanNum])
							{
								inputChannelInfo [numInputChannelInfos].streamNum = i;
								inputChannelInfo [numInputChannelInfos].dataOffsetSamples = (int) j;
								inputChannelInfo [numInputChannelInfos].dataStrideSamples = (int) b.mNumberChannels;
								++numInputChannelInfos;
							}

							if (name.isEmpty())
								name << "Input " << (chanNum + 1);

							inChanNames.add (name);
						}
						else
						{
							if (activeOutputChans[chanNum])
							{
								outputChannelInfo [numOutputChannelInfos].streamNum = i;
								outputChannelInfo [numOutputChannelInfos].dataOffsetSamples = (int) j;
								outputChannelInfo [numOutputChannelInfos].dataStrideSamples = (int) b.mNumberChannels;
								++numOutputChannelInfos;
							}

							if (name.isEmpty())
								name << "Output " << (chanNum + 1);

							outChanNames.add (name);
						}

						++chanNum;
					}
				}
			}
		}
	}

	void updateDetailsFromDevice()
	{
		stopTimer();

		if (deviceID == 0)
			return;

		const ScopedLock sl (callbackLock);

		AudioObjectPropertyAddress pa;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementMaster;

		UInt32 isAlive;
		UInt32 size = sizeof (isAlive);
		pa.mSelector = kAudioDevicePropertyDeviceIsAlive;
		if (OK (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, &isAlive))
			 && isAlive == 0)
			return;

		Float64 sr;
		size = sizeof (sr);
		pa.mSelector = kAudioDevicePropertyNominalSampleRate;
		if (OK (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, &sr)))
			sampleRate = sr;

		UInt32 framesPerBuf;
		size = sizeof (framesPerBuf);
		pa.mSelector = kAudioDevicePropertyBufferFrameSize;
		if (OK (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, &framesPerBuf)))
		{
			bufferSize = (int) framesPerBuf;
			allocateTempBuffers();
		}

		bufferSizes.clear();

		pa.mSelector = kAudioDevicePropertyBufferFrameSizeRange;

		if (OK (AudioObjectGetPropertyDataSize (deviceID, &pa, 0, 0, &size)))
		{
			HeapBlock <AudioValueRange> ranges;
			ranges.calloc (size, 1);

			if (OK (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, ranges)))
			{
				bufferSizes.add ((int) ranges[0].mMinimum);

				for (int i = 32; i < 2048; i += 32)
				{
					for (int j = size / (int) sizeof (AudioValueRange); --j >= 0;)
					{
						if (i >= ranges[j].mMinimum && i <= ranges[j].mMaximum)
						{
							bufferSizes.addIfNotAlreadyThere (i);
							break;
						}
					}
				}

				if (bufferSize > 0)
					bufferSizes.addIfNotAlreadyThere (bufferSize);
			}
		}

		if (bufferSizes.size() == 0 && bufferSize > 0)
			bufferSizes.add (bufferSize);

		sampleRates.clear();
		const double possibleRates[] = { 44100.0, 48000.0, 88200.0, 96000.0, 176400.0, 192000.0 };
		String rates;

		pa.mSelector = kAudioDevicePropertyAvailableNominalSampleRates;

		if (OK (AudioObjectGetPropertyDataSize (deviceID, &pa, 0, 0, &size)))
		{
			HeapBlock <AudioValueRange> ranges;
			ranges.calloc (size, 1);

			if (OK (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, ranges)))
			{
				for (int i = 0; i < numElementsInArray (possibleRates); ++i)
				{
					bool ok = false;

					for (int j = size / (int) sizeof (AudioValueRange); --j >= 0;)
						if (possibleRates[i] >= ranges[j].mMinimum - 2 && possibleRates[i] <= ranges[j].mMaximum + 2)
							ok = true;

					if (ok)
					{
						sampleRates.add (possibleRates[i]);
						rates << possibleRates[i] << ' ';
					}
				}
			}
		}

		if (sampleRates.size() == 0 && sampleRate > 0)
		{
			sampleRates.add (sampleRate);
			rates << sampleRate;
		}

		JUCE_COREAUDIOLOG ("sr: " + rates);

		inputLatency = 0;
		outputLatency = 0;
		UInt32 lat;
		size = sizeof (lat);
		pa.mSelector = kAudioDevicePropertyLatency;
		pa.mScope = kAudioDevicePropertyScopeInput;
		if (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, &lat) == noErr)
			inputLatency = (int) lat;

		pa.mScope = kAudioDevicePropertyScopeOutput;
		size = sizeof (lat);

		if (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, &lat) == noErr)
			outputLatency = (int) lat;

		JUCE_COREAUDIOLOG ("lat: " + String (inputLatency) + " " + String (outputLatency));

		inChanNames.clear();
		outChanNames.clear();

		inputChannelInfo.calloc ((size_t) numInputChans + 2);
		numInputChannelInfos = 0;

		outputChannelInfo.calloc ((size_t) numOutputChans + 2);
		numOutputChannelInfos = 0;

		fillInChannelInfo (true);
		fillInChannelInfo (false);
	}

	StringArray getSources (bool input)
	{
		StringArray s;
		HeapBlock <OSType> types;
		const int num = getAllDataSourcesForDevice (deviceID, types);

		for (int i = 0; i < num; ++i)
		{
			AudioValueTranslation avt;
			char buffer[256];

			avt.mInputData = &(types[i]);
			avt.mInputDataSize = sizeof (UInt32);
			avt.mOutputData = buffer;
			avt.mOutputDataSize = 256;

			UInt32 transSize = sizeof (avt);

			AudioObjectPropertyAddress pa;
			pa.mSelector = kAudioDevicePropertyDataSourceNameForID;
			pa.mScope = input ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;
			pa.mElement = kAudioObjectPropertyElementMaster;

			if (OK (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &transSize, &avt)))
			{
				DBG (buffer);
				s.add (buffer);
			}
		}

		return s;
	}

	int getCurrentSourceIndex (bool input) const
	{
		OSType currentSourceID = 0;
		UInt32 size = sizeof (currentSourceID);
		int result = -1;

		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioDevicePropertyDataSource;
		pa.mScope = input ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;
		pa.mElement = kAudioObjectPropertyElementMaster;

		if (deviceID != 0)
		{
			if (OK (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, &currentSourceID)))
			{
				HeapBlock <OSType> types;
				const int num = getAllDataSourcesForDevice (deviceID, types);

				for (int i = 0; i < num; ++i)
				{
					if (types[num] == currentSourceID)
					{
						result = i;
						break;
					}
				}
			}
		}

		return result;
	}

	void setCurrentSourceIndex (int index, bool input)
	{
		if (deviceID != 0)
		{
			HeapBlock <OSType> types;
			const int num = getAllDataSourcesForDevice (deviceID, types);

			if (isPositiveAndBelow (index, num))
			{
				AudioObjectPropertyAddress pa;
				pa.mSelector = kAudioDevicePropertyDataSource;
				pa.mScope = input ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;
				pa.mElement = kAudioObjectPropertyElementMaster;

				OSType typeId = types[index];

				OK (AudioObjectSetPropertyData (deviceID, &pa, 0, 0, sizeof (typeId), &typeId));
			}
		}
	}

	String reopen (const BigInteger& inputChannels,
				   const BigInteger& outputChannels,
				   double newSampleRate,
				   int bufferSizeSamples)
	{
		String error;
		JUCE_COREAUDIOLOG ("CoreAudio reopen");
		callbacksAllowed = false;
		stopTimer();

		stop (false);

		activeInputChans = inputChannels;
		activeInputChans.setRange (inChanNames.size(),
								   activeInputChans.getHighestBit() + 1 - inChanNames.size(),
								   false);

		activeOutputChans = outputChannels;
		activeOutputChans.setRange (outChanNames.size(),
									activeOutputChans.getHighestBit() + 1 - outChanNames.size(),
									false);

		numInputChans = activeInputChans.countNumberOfSetBits();
		numOutputChans = activeOutputChans.countNumberOfSetBits();

		// set sample rate
		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioDevicePropertyNominalSampleRate;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementMaster;
		Float64 sr = newSampleRate;

		if (! OK (AudioObjectSetPropertyData (deviceID, &pa, 0, 0, sizeof (sr), &sr)))
		{
			error = "Couldn't change sample rate";
		}
		else
		{
			// change buffer size
			UInt32 framesPerBuf = (UInt32) bufferSizeSamples;
			pa.mSelector = kAudioDevicePropertyBufferFrameSize;

			if (! OK (AudioObjectSetPropertyData (deviceID, &pa, 0, 0, sizeof (framesPerBuf), &framesPerBuf)))
			{
				error = "Couldn't change buffer size";
			}
			else
			{
				// Annoyingly, after changing the rate and buffer size, some devices fail to
				// correctly report their new settings until some random time in the future, so
				// after calling updateDetailsFromDevice, we need to manually bodge these values
				// to make sure we're using the correct numbers..
				updateDetailsFromDevice();
				sampleRate = newSampleRate;
				bufferSize = bufferSizeSamples;

				if (sampleRates.size() == 0)
					error = "Device has no available sample-rates";
				else if (bufferSizes.size() == 0)
					error = "Device has no available buffer-sizes";
				else if (inputDevice != 0)
					error = inputDevice->reopen (inputChannels,
												 outputChannels,
												 newSampleRate,
												 bufferSizeSamples);
			}
		}

		callbacksAllowed = true;
		return error;
	}

	bool start (AudioIODeviceCallback* cb)
	{
		if (! started)
		{
			callback = nullptr;

			if (deviceID != 0)
			{
			   #if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
				if (OK (AudioDeviceAddIOProc (deviceID, audioIOProc, this)))
			   #else
				if (OK (AudioDeviceCreateIOProcID (deviceID, audioIOProc, this, &audioProcID)))
			   #endif
				{
					if (OK (AudioDeviceStart (deviceID, audioIOProc)))
					{
						started = true;
					}
					else
					{
					   #if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
						OK (AudioDeviceRemoveIOProc (deviceID, audioIOProc));
					   #else
						OK (AudioDeviceDestroyIOProcID (deviceID, audioProcID));
						audioProcID = 0;
					   #endif
					}
				}
			}
		}

		if (started)
		{
			const ScopedLock sl (callbackLock);
			callback = cb;
		}

		return started && (inputDevice == nullptr || inputDevice->start (cb));
	}

	void stop (bool leaveInterruptRunning)
	{
		{
			const ScopedLock sl (callbackLock);
			callback = nullptr;
		}

		if (started
			 && (deviceID != 0)
			 && ! leaveInterruptRunning)
		{
			OK (AudioDeviceStop (deviceID, audioIOProc));

		   #if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
			OK (AudioDeviceRemoveIOProc (deviceID, audioIOProc));
		   #else
			OK (AudioDeviceDestroyIOProcID (deviceID, audioProcID));
			audioProcID = 0;
		   #endif

			started = false;

			{ const ScopedLock sl (callbackLock); }

			// wait until it's definately stopped calling back..
			for (int i = 40; --i >= 0;)
			{
				Thread::sleep (50);

				UInt32 running = 0;
				UInt32 size = sizeof (running);

				AudioObjectPropertyAddress pa;
				pa.mSelector = kAudioDevicePropertyDeviceIsRunning;
				pa.mScope = kAudioObjectPropertyScopeWildcard;
				pa.mElement = kAudioObjectPropertyElementMaster;

				OK (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, &running));

				if (running == 0)
					break;
			}

			const ScopedLock sl (callbackLock);
		}

		if (inputDevice != nullptr)
			inputDevice->stop (leaveInterruptRunning);
	}

	double getSampleRate() const  { return sampleRate; }
	int getBufferSize() const     { return bufferSize; }

	void audioCallback (const AudioBufferList* inInputData,
						AudioBufferList* outOutputData)
	{
		int i;
		const ScopedLock sl (callbackLock);

		if (callback != nullptr)
		{
			if (inputDevice == 0)
			{
				for (i = numInputChans; --i >= 0;)
				{
					const CallbackDetailsForChannel& info = inputChannelInfo[i];
					float* dest = tempInputBuffers [i];
					const float* src = ((const float*) inInputData->mBuffers[info.streamNum].mData)
										+ info.dataOffsetSamples;
					const int stride = info.dataStrideSamples;

					if (stride != 0) // if this is zero, info is invalid
					{
						for (int j = bufferSize; --j >= 0;)
						{
							*dest++ = *src;
							src += stride;
						}
					}
				}
			}

			if (! isSlaveDevice)
			{
				if (inputDevice == 0)
				{
					callback->audioDeviceIOCallback (const_cast<const float**> (tempInputBuffers.getData()),
													 numInputChans,
													 tempOutputBuffers,
													 numOutputChans,
													 bufferSize);
				}
				else
				{
					jassert (inputDevice->bufferSize == bufferSize);

					// Sometimes the two linked devices seem to get their callbacks in
					// parallel, so we need to lock both devices to stop the input data being
					// changed while inside our callback..
					const ScopedLock sl2 (inputDevice->callbackLock);

					callback->audioDeviceIOCallback (const_cast<const float**> (inputDevice->tempInputBuffers.getData()),
													 inputDevice->numInputChans,
													 tempOutputBuffers,
													 numOutputChans,
													 bufferSize);
				}

				for (i = numOutputChans; --i >= 0;)
				{
					const CallbackDetailsForChannel& info = outputChannelInfo[i];
					const float* src = tempOutputBuffers [i];
					float* dest = ((float*) outOutputData->mBuffers[info.streamNum].mData)
									+ info.dataOffsetSamples;
					const int stride = info.dataStrideSamples;

					if (stride != 0) // if this is zero, info is invalid
					{
						for (int j = bufferSize; --j >= 0;)
						{
							*dest = *src++;
							dest += stride;
						}
					}
				}
			}
		}
		else
		{
			for (i = jmin (numOutputChans, numOutputChannelInfos); --i >= 0;)
			{
				const CallbackDetailsForChannel& info = outputChannelInfo[i];
				float* dest = ((float*) outOutputData->mBuffers[info.streamNum].mData)
								+ info.dataOffsetSamples;
				const int stride = info.dataStrideSamples;

				if (stride != 0) // if this is zero, info is invalid
				{
					for (int j = bufferSize; --j >= 0;)
					{
						*dest = 0.0f;
						dest += stride;
					}
				}
			}
		}
	}

	// called by callbacks
	void deviceDetailsChanged()
	{
		if (callbacksAllowed)
			startTimer (100);
	}

	void timerCallback()
	{
		stopTimer();
		JUCE_COREAUDIOLOG ("CoreAudio device changed callback");

		const double oldSampleRate = sampleRate;
		const int oldBufferSize = bufferSize;
		updateDetailsFromDevice();

		if (oldBufferSize != bufferSize || oldSampleRate != sampleRate)
		{
			callbacksAllowed = false;
			stop (false);
			updateDetailsFromDevice();
			callbacksAllowed = true;
		}
	}

	CoreAudioInternal* getRelatedDevice() const
	{
		UInt32 size = 0;
		ScopedPointer <CoreAudioInternal> result;

		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioDevicePropertyRelatedDevices;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementMaster;

		if (deviceID != 0
			 && AudioObjectGetPropertyDataSize (deviceID, &pa, 0, 0, &size) == noErr
			 && size > 0)
		{
			HeapBlock <AudioDeviceID> devs;
			devs.calloc (size, 1);

			if (OK (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, devs)))
			{
				for (unsigned int i = 0; i < size / sizeof (AudioDeviceID); ++i)
				{
					if (devs[i] != deviceID && devs[i] != 0)
					{
						result = new CoreAudioInternal (devs[i]);

						const bool thisIsInput = inChanNames.size() > 0 && outChanNames.size() == 0;
						const bool otherIsInput = result->inChanNames.size() > 0 && result->outChanNames.size() == 0;

						if (thisIsInput != otherIsInput
							 || (inChanNames.size() + outChanNames.size() == 0)
							 || (result->inChanNames.size() + result->outChanNames.size()) == 0)
							break;

						result = 0;
					}
				}
			}
		}

		return result.release();
	}

	int inputLatency, outputLatency;
	BigInteger activeInputChans, activeOutputChans;
	StringArray inChanNames, outChanNames;
	Array <double> sampleRates;
	Array <int> bufferSizes;
	AudioIODeviceCallback* callback;
   #if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
	AudioDeviceIOProcID audioProcID;
   #endif

	ScopedPointer<CoreAudioInternal> inputDevice;
	bool isSlaveDevice;

private:
	CriticalSection callbackLock;
	AudioDeviceID deviceID;
	bool started;
	double sampleRate;
	int bufferSize;
	HeapBlock <float> audioBuffer;
	int numInputChans, numOutputChans;
	bool callbacksAllowed;

	struct CallbackDetailsForChannel
	{
		int streamNum;
		int dataOffsetSamples;
		int dataStrideSamples;
	};

	int numInputChannelInfos, numOutputChannelInfos;
	HeapBlock <CallbackDetailsForChannel> inputChannelInfo, outputChannelInfo;
	HeapBlock <float*> tempInputBuffers, tempOutputBuffers;

	static OSStatus audioIOProc (AudioDeviceID /*inDevice*/,
								 const AudioTimeStamp* /*inNow*/,
								 const AudioBufferList* inInputData,
								 const AudioTimeStamp* /*inInputTime*/,
								 AudioBufferList* outOutputData,
								 const AudioTimeStamp* /*inOutputTime*/,
								 void* device)
	{
		static_cast <CoreAudioInternal*> (device)->audioCallback (inInputData, outOutputData);
		return noErr;
	}

	static OSStatus deviceListenerProc (AudioDeviceID /*inDevice*/, UInt32 /*inLine*/, const AudioObjectPropertyAddress* pa, void* inClientData)
	{
		CoreAudioInternal* const intern = static_cast <CoreAudioInternal*> (inClientData);

		switch (pa->mSelector)
		{
			case kAudioDevicePropertyBufferSize:
			case kAudioDevicePropertyBufferFrameSize:
			case kAudioDevicePropertyNominalSampleRate:
			case kAudioDevicePropertyStreamFormat:
			case kAudioDevicePropertyDeviceIsAlive:
			case kAudioStreamPropertyPhysicalFormat:
				intern->deviceDetailsChanged();
				break;

			case kAudioDevicePropertyBufferSizeRange:
			case kAudioDevicePropertyVolumeScalar:
			case kAudioDevicePropertyMute:
			case kAudioDevicePropertyPlayThru:
			case kAudioDevicePropertyDataSource:
			case kAudioDevicePropertyDeviceIsRunning:
				break;
		}

		return noErr;
	}

	static int getAllDataSourcesForDevice (AudioDeviceID deviceID, HeapBlock <OSType>& types)
	{
		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioDevicePropertyDataSources;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementMaster;
		UInt32 size = 0;

		if (deviceID != 0
			 && AudioObjectGetPropertyDataSize (deviceID, &pa, 0, 0, &size) == noErr)
		{
			types.calloc (size, 1);

			if (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, types) == noErr)
				return size / (int) sizeof (OSType);
		}

		return 0;
	}

	bool OK (const OSStatus errorCode) const
	{
		if (errorCode == noErr)
			return true;

		const String errorMessage ("CoreAudio error: " + String::toHexString ((int) errorCode));
		JUCE_COREAUDIOLOG (errorMessage);

		if (callback != nullptr)
			callback->audioDeviceError (errorMessage);

		return false;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoreAudioInternal);
};

class CoreAudioIODevice   : public AudioIODevice
{
public:
	CoreAudioIODevice (const String& deviceName,
					   AudioDeviceID inputDeviceId,
					   const int inputIndex_,
					   AudioDeviceID outputDeviceId,
					   const int outputIndex_)
		: AudioIODevice (deviceName, "CoreAudio"),
		  inputIndex (inputIndex_),
		  outputIndex (outputIndex_),
		  isOpen_ (false),
		  isStarted (false)
	{
		CoreAudioInternal* device = nullptr;

		if (outputDeviceId == 0 || outputDeviceId == inputDeviceId)
		{
			jassert (inputDeviceId != 0);

			device = new CoreAudioInternal (inputDeviceId);
		}
		else
		{
			device = new CoreAudioInternal (outputDeviceId);

			if (inputDeviceId != 0)
			{
				CoreAudioInternal* secondDevice = new CoreAudioInternal (inputDeviceId);

				device->inputDevice = secondDevice;
				secondDevice->isSlaveDevice = true;
			}
		}

		internal = device;
		jassert (device != nullptr);

		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioObjectPropertySelectorWildcard;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementWildcard;

		AudioObjectAddPropertyListener (kAudioObjectSystemObject, &pa, hardwareListenerProc, internal);
	}

	~CoreAudioIODevice()
	{
		close();

		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioObjectPropertySelectorWildcard;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementWildcard;

		AudioObjectRemovePropertyListener (kAudioObjectSystemObject, &pa, hardwareListenerProc, internal);
	}

	StringArray getOutputChannelNames()
	{
		return internal->outChanNames;
	}

	StringArray getInputChannelNames()
	{
		if (internal->inputDevice != 0)
			return internal->inputDevice->inChanNames;
		else
			return internal->inChanNames;
	}

	bool isOpen()    { return isOpen_; }

	int getNumSampleRates()              { return internal->sampleRates.size(); }
	double getSampleRate (int index)     { return internal->sampleRates [index]; }
	double getCurrentSampleRate()        { return internal->getSampleRate(); }

	int getCurrentBitDepth()             { return 32; }  // no way to find out, so just assume it's high..

	int getNumBufferSizesAvailable()     { return internal->bufferSizes.size(); }
	int getBufferSizeSamples (int index) { return internal->bufferSizes [index]; }
	int getCurrentBufferSizeSamples()    { return internal->getBufferSize(); }

	int getDefaultBufferSize()
	{
		int best = 0;

		for (int i = 0; best < 512 && i < getNumBufferSizesAvailable(); ++i)
			best = getBufferSizeSamples(i);

		if (best == 0)
			best = 512;

		return best;
	}

	String open (const BigInteger& inputChannels,
				 const BigInteger& outputChannels,
				 double sampleRate,
				 int bufferSizeSamples)
	{
		isOpen_ = true;

		if (bufferSizeSamples <= 0)
			bufferSizeSamples = getDefaultBufferSize();

		lastError = internal->reopen (inputChannels, outputChannels, sampleRate, bufferSizeSamples);
		isOpen_ = lastError.isEmpty();
		return lastError;
	}

	void close()
	{
		isOpen_ = false;
		internal->stop (false);
	}

	BigInteger getActiveOutputChannels() const
	{
		return internal->activeOutputChans;
	}

	BigInteger getActiveInputChannels() const
	{
		BigInteger chans (internal->activeInputChans);

		if (internal->inputDevice != nullptr)
			chans |= internal->inputDevice->activeInputChans;

		return chans;
	}

	int getOutputLatencyInSamples()
	{
		// this seems like a good guess at getting the latency right - comparing
		// this with a round-trip measurement, it gets it to within a few millisecs
		// for the built-in mac soundcard
		return internal->outputLatency + internal->getBufferSize() * 2;
	}

	int getInputLatencyInSamples()
	{
		return internal->inputLatency + internal->getBufferSize() * 2;
	}

	void start (AudioIODeviceCallback* callback)
	{
		if (! isStarted)
		{
			if (callback != nullptr)
				callback->audioDeviceAboutToStart (this);

			isStarted = true;
			internal->start (callback);
		}
	}

	void stop()
	{
		if (isStarted)
		{
			AudioIODeviceCallback* const lastCallback = internal->callback;

			isStarted = false;
			internal->stop (true);

			if (lastCallback != nullptr)
				lastCallback->audioDeviceStopped();
		}
	}

	bool isPlaying()
	{
		if (internal->callback == nullptr)
			isStarted = false;

		return isStarted;
	}

	String getLastError()
	{
		return lastError;
	}

	int inputIndex, outputIndex;

private:
	ScopedPointer<CoreAudioInternal> internal;
	bool isOpen_, isStarted;
	String lastError;

	static OSStatus hardwareListenerProc (AudioDeviceID /*inDevice*/, UInt32 /*inLine*/, const AudioObjectPropertyAddress* pa, void* inClientData)
	{
		CoreAudioInternal* const intern = static_cast <CoreAudioInternal*> (inClientData);

		switch (pa->mSelector)
		{
			case kAudioHardwarePropertyDevices:
				intern->deviceDetailsChanged();
				break;

			case kAudioHardwarePropertyDefaultOutputDevice:
			case kAudioHardwarePropertyDefaultInputDevice:
			case kAudioHardwarePropertyDefaultSystemOutputDevice:
				break;
		}

		return noErr;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoreAudioIODevice);
};

class CoreAudioIODeviceType  : public AudioIODeviceType
{
public:

	CoreAudioIODeviceType()
		: AudioIODeviceType ("CoreAudio"),
		  hasScanned (false)
	{
		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioHardwarePropertyDevices;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementWildcard;

		AudioObjectAddPropertyListener (kAudioObjectSystemObject, &pa, hardwareListenerProc, this);
	}

	~CoreAudioIODeviceType()
	{
		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioHardwarePropertyDevices;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementWildcard;

		AudioObjectRemovePropertyListener (kAudioObjectSystemObject, &pa, hardwareListenerProc, this);
	}

	void scanForDevices()
	{
		hasScanned = true;

		inputDeviceNames.clear();
		outputDeviceNames.clear();
		inputIds.clear();
		outputIds.clear();

		UInt32 size;

		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioHardwarePropertyDevices;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementMaster;

		if (AudioObjectGetPropertyDataSize (kAudioObjectSystemObject, &pa, 0, 0, &size) == noErr)
		{
			HeapBlock <AudioDeviceID> devs;
			devs.calloc (size, 1);

			if (AudioObjectGetPropertyData (kAudioObjectSystemObject, &pa, 0, 0, &size, devs) == noErr)
			{
				const int num = size / (int) sizeof (AudioDeviceID);
				for (int i = 0; i < num; ++i)
				{
					char name [1024];
					size = sizeof (name);
					pa.mSelector = kAudioDevicePropertyDeviceName;

					if (AudioObjectGetPropertyData (devs[i], &pa, 0, 0, &size, name) == noErr)
					{
						const String nameString (String::fromUTF8 (name, (int) strlen (name)));
						const int numIns = getNumChannels (devs[i], true);
						const int numOuts = getNumChannels (devs[i], false);

						if (numIns > 0)
						{
							inputDeviceNames.add (nameString);
							inputIds.add (devs[i]);
						}

						if (numOuts > 0)
						{
							outputDeviceNames.add (nameString);
							outputIds.add (devs[i]);
						}
					}
				}
			}
		}

		inputDeviceNames.appendNumbersToDuplicates (false, true);
		outputDeviceNames.appendNumbersToDuplicates (false, true);
	}

	StringArray getDeviceNames (bool wantInputNames) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		return wantInputNames ? inputDeviceNames
							  : outputDeviceNames;
	}

	int getDefaultDeviceIndex (bool forInput) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		AudioDeviceID deviceID;
		UInt32 size = sizeof (deviceID);

		// if they're asking for any input channels at all, use the default input, so we
		// get the built-in mic rather than the built-in output with no inputs..

		AudioObjectPropertyAddress pa;
		pa.mSelector = forInput ? kAudioHardwarePropertyDefaultInputDevice : kAudioHardwarePropertyDefaultOutputDevice;
		pa.mScope = kAudioObjectPropertyScopeWildcard;
		pa.mElement = kAudioObjectPropertyElementMaster;

		if (AudioObjectGetPropertyData (kAudioObjectSystemObject, &pa, 0, 0, &size, &deviceID) == noErr)
		{
			if (forInput)
			{
				for (int i = inputIds.size(); --i >= 0;)
					if (inputIds[i] == deviceID)
						return i;
			}
			else
			{
				for (int i = outputIds.size(); --i >= 0;)
					if (outputIds[i] == deviceID)
						return i;
			}
		}

		return 0;
	}

	int getIndexOfDevice (AudioIODevice* device, bool asInput) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		CoreAudioIODevice* const d = dynamic_cast <CoreAudioIODevice*> (device);
		if (d == nullptr)
			return -1;

		return asInput ? d->inputIndex
					   : d->outputIndex;
	}

	bool hasSeparateInputsAndOutputs() const    { return true; }

	AudioIODevice* createDevice (const String& outputDeviceName,
								 const String& inputDeviceName)
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		const int inputIndex = inputDeviceNames.indexOf (inputDeviceName);
		const int outputIndex = outputDeviceNames.indexOf (outputDeviceName);

		String deviceName (outputDeviceName);
		if (deviceName.isEmpty())
			deviceName = inputDeviceName;

		if (index >= 0)
			return new CoreAudioIODevice (deviceName,
										  inputIds [inputIndex],
										  inputIndex,
										  outputIds [outputIndex],
										  outputIndex);

		return nullptr;
	}

private:
	StringArray inputDeviceNames, outputDeviceNames;
	Array <AudioDeviceID> inputIds, outputIds;

	bool hasScanned;

	static int getNumChannels (AudioDeviceID deviceID, bool input)
	{
		int total = 0;
		UInt32 size;

		AudioObjectPropertyAddress pa;
		pa.mSelector = kAudioDevicePropertyStreamConfiguration;
		pa.mScope = input ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;
		pa.mElement = kAudioObjectPropertyElementMaster;

		if (AudioObjectGetPropertyDataSize (deviceID, &pa, 0, 0, &size) == noErr)
		{
			HeapBlock <AudioBufferList> bufList;
			bufList.calloc (size, 1);

			if (AudioObjectGetPropertyData (deviceID, &pa, 0, 0, &size, bufList) == noErr)
			{
				const int numStreams = (int) bufList->mNumberBuffers;

				for (int i = 0; i < numStreams; ++i)
				{
					const AudioBuffer& b = bufList->mBuffers[i];
					total += b.mNumberChannels;
				}
			}
		}

		return total;
	}

	void audioDeviceListChanged()
	{
		scanForDevices();
		callDeviceChangeListeners();
	}

	static OSStatus hardwareListenerProc (AudioDeviceID, UInt32, const AudioObjectPropertyAddress*, void* clientData)
	{
		static_cast <CoreAudioIODeviceType*> (clientData)->audioDeviceListChanged();
		return noErr;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoreAudioIODeviceType);
};

AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_CoreAudio()
{
	return new CoreAudioIODeviceType();
}

#undef JUCE_COREAUDIOLOG

/*** End of inlined file: juce_mac_CoreAudio.cpp ***/


/*** Start of inlined file: juce_mac_CoreMidi.cpp ***/
namespace CoreMidiHelpers
{
	static bool logError (const OSStatus err, const int lineNum)
	{
		if (err == noErr)
			return true;

		Logger::writeToLog ("CoreMidi error: " + String (lineNum) + " - " + String::toHexString ((int) err));
		jassertfalse;
		return false;
	}

	#undef CHECK_ERROR
	#define CHECK_ERROR(a) CoreMidiHelpers::logError (a, __LINE__)

	static String getEndpointName (MIDIEndpointRef endpoint, bool isExternal)
	{
		String result;
		CFStringRef str = 0;

		MIDIObjectGetStringProperty (endpoint, kMIDIPropertyName, &str);

		if (str != 0)
		{
			result = String::fromCFString (str);
			CFRelease (str);
			str = 0;
		}

		MIDIEntityRef entity = 0;
		MIDIEndpointGetEntity (endpoint, &entity);

		if (entity == 0)
			return result; // probably virtual

		if (result.isEmpty())
		{
			// endpoint name has zero length - try the entity
			MIDIObjectGetStringProperty (entity, kMIDIPropertyName, &str);

			if (str != 0)
			{
				result += String::fromCFString (str);
				CFRelease (str);
				str = 0;
			}
		}

		// now consider the device's name
		MIDIDeviceRef device = 0;
		MIDIEntityGetDevice (entity, &device);
		if (device == 0)
			return result;

		MIDIObjectGetStringProperty (device, kMIDIPropertyName, &str);

		if (str != 0)
		{
			const String s (String::fromCFString (str));
			CFRelease (str);

			// if an external device has only one entity, throw away
			// the endpoint name and just use the device name
			if (isExternal && MIDIDeviceGetNumberOfEntities (device) < 2)
			{
				result = s;
			}
			else if (! result.startsWithIgnoreCase (s))
			{
				// prepend the device name to the entity name
				result = (s + " " + result).trimEnd();
			}
		}

		return result;
	}

	static String getConnectedEndpointName (MIDIEndpointRef endpoint)
	{
		String result;

		// Does the endpoint have connections?
		CFDataRef connections = 0;
		int numConnections = 0;

		MIDIObjectGetDataProperty (endpoint, kMIDIPropertyConnectionUniqueID, &connections);

		if (connections != 0)
		{
			numConnections = ((int) CFDataGetLength (connections)) / (int) sizeof (MIDIUniqueID);

			if (numConnections > 0)
			{
				const SInt32* pid = reinterpret_cast <const SInt32*> (CFDataGetBytePtr (connections));

				for (int i = 0; i < numConnections; ++i, ++pid)
				{
					MIDIUniqueID uid = (MIDIUniqueID) ByteOrder::swapIfLittleEndian ((uint32) *pid);
					MIDIObjectRef connObject;
					MIDIObjectType connObjectType;
					OSStatus err = MIDIObjectFindByUniqueID (uid, &connObject, &connObjectType);

					if (err == noErr)
					{
						String s;

						if (connObjectType == kMIDIObjectType_ExternalSource
							 || connObjectType == kMIDIObjectType_ExternalDestination)
						{
							// Connected to an external device's endpoint (10.3 and later).
							s = getEndpointName (static_cast <MIDIEndpointRef> (connObject), true);
						}
						else
						{
							// Connected to an external device (10.2) (or something else, catch-all)
							CFStringRef str = 0;
							MIDIObjectGetStringProperty (connObject, kMIDIPropertyName, &str);

							if (str != 0)
							{
								s = String::fromCFString (str);
								CFRelease (str);
							}
						}

						if (s.isNotEmpty())
						{
							if (result.isNotEmpty())
								result += ", ";

							result += s;
						}
					}
				}
			}

			CFRelease (connections);
		}

		if (result.isNotEmpty())
			return result;

		// Here, either the endpoint had no connections, or we failed to obtain names for any of them.
		return getEndpointName (endpoint, false);
	}

	static MIDIClientRef getGlobalMidiClient()
	{
		static MIDIClientRef globalMidiClient = 0;

		if (globalMidiClient == 0)
		{
			String name ("JUCE");

			if (JUCEApplicationBase::getInstance() != nullptr)
				name = JUCEApplicationBase::getInstance()->getApplicationName();

			CFStringRef appName = name.toCFString();
			CHECK_ERROR (MIDIClientCreate (appName, 0, 0, &globalMidiClient));
			CFRelease (appName);
		}

		return globalMidiClient;
	}

	class MidiPortAndEndpoint
	{
	public:
		MidiPortAndEndpoint (MIDIPortRef port_, MIDIEndpointRef endPoint_)
			: port (port_), endPoint (endPoint_)
		{
		}

		~MidiPortAndEndpoint()
		{
			if (port != 0)
				MIDIPortDispose (port);

			if (port == 0 && endPoint != 0) // if port == 0, it means we created the endpoint, so it's safe to delete it
				MIDIEndpointDispose (endPoint);
		}

		void send (const MIDIPacketList* const packets)
		{
			if (port != 0)
				MIDISend (port, endPoint, packets);
			else
				MIDIReceived (endPoint, packets);
		}

		MIDIPortRef port;
		MIDIEndpointRef endPoint;
	};

	class MidiPortAndCallback;
	CriticalSection callbackLock;
	Array<MidiPortAndCallback*> activeCallbacks;

	class MidiPortAndCallback
	{
	public:
		MidiPortAndCallback (MidiInputCallback& callback_)
			: input (nullptr), active (false), callback (callback_), concatenator (2048)
		{
		}

		~MidiPortAndCallback()
		{
			active = false;

			{
				const ScopedLock sl (callbackLock);
				activeCallbacks.removeValue (this);
			}

			if (portAndEndpoint != nullptr && portAndEndpoint->port != 0)
				CHECK_ERROR (MIDIPortDisconnectSource (portAndEndpoint->port, portAndEndpoint->endPoint));
		}

		void handlePackets (const MIDIPacketList* const pktlist)
		{
			const double time = Time::getMillisecondCounterHiRes() * 0.001;

			const ScopedLock sl (callbackLock);
			if (activeCallbacks.contains (this) && active)
			{
				const MIDIPacket* packet = &pktlist->packet[0];

				for (unsigned int i = 0; i < pktlist->numPackets; ++i)
				{
					concatenator.pushMidiData (packet->data, (int) packet->length, time,
											   input, callback);

					packet = MIDIPacketNext (packet);
				}
			}
		}

		MidiInput* input;
		ScopedPointer<MidiPortAndEndpoint> portAndEndpoint;
		volatile bool active;

	private:
		MidiInputCallback& callback;
		MidiDataConcatenator concatenator;
	};

	static void midiInputProc (const MIDIPacketList* pktlist, void* readProcRefCon, void* /*srcConnRefCon*/)
	{
		static_cast <MidiPortAndCallback*> (readProcRefCon)->handlePackets (pktlist);
	}
}

StringArray MidiOutput::getDevices()
{
	StringArray s;

	const ItemCount num = MIDIGetNumberOfDestinations();
	for (ItemCount i = 0; i < num; ++i)
	{
		MIDIEndpointRef dest = MIDIGetDestination (i);
		String name;

		if (dest != 0)
			name = CoreMidiHelpers::getConnectedEndpointName (dest);

		if (name.isEmpty())
			name = "<error>";

		s.add (name);
	}

	return s;
}

int MidiOutput::getDefaultDeviceIndex()
{
	return 0;
}

MidiOutput* MidiOutput::openDevice (int index)
{
	MidiOutput* mo = nullptr;

	if (isPositiveAndBelow (index, (int) MIDIGetNumberOfDestinations()))
	{
		MIDIEndpointRef endPoint = MIDIGetDestination ((ItemCount) index);

		CFStringRef pname;
		if (CHECK_ERROR (MIDIObjectGetStringProperty (endPoint, kMIDIPropertyName, &pname)))
		{
			MIDIClientRef client = CoreMidiHelpers::getGlobalMidiClient();
			MIDIPortRef port;

			if (client != 0 && CHECK_ERROR (MIDIOutputPortCreate (client, pname, &port)))
			{
				mo = new MidiOutput();
				mo->internal = new CoreMidiHelpers::MidiPortAndEndpoint (port, endPoint);
			}

			CFRelease (pname);
		}
	}

	return mo;
}

MidiOutput* MidiOutput::createNewDevice (const String& deviceName)
{
	MidiOutput* mo = nullptr;
	MIDIClientRef client = CoreMidiHelpers::getGlobalMidiClient();

	MIDIEndpointRef endPoint;
	CFStringRef name = deviceName.toCFString();

	if (client != 0 && CHECK_ERROR (MIDISourceCreate (client, name, &endPoint)))
	{
		mo = new MidiOutput();
		mo->internal = new CoreMidiHelpers::MidiPortAndEndpoint (0, endPoint);
	}

	CFRelease (name);
	return mo;
}

MidiOutput::~MidiOutput()
{
	delete static_cast<CoreMidiHelpers::MidiPortAndEndpoint*> (internal);
}

void MidiOutput::sendMessageNow (const MidiMessage& message)
{
	CoreMidiHelpers::MidiPortAndEndpoint* const mpe = static_cast<CoreMidiHelpers::MidiPortAndEndpoint*> (internal);

   #if JUCE_IOS
	const MIDITimeStamp timeStamp = mach_absolute_time();
   #else
	const MIDITimeStamp timeStamp = AudioGetCurrentHostTime();
   #endif

	if (message.isSysEx())
	{
		const int maxPacketSize = 256;
		int pos = 0, bytesLeft = message.getRawDataSize();
		const int numPackets = (bytesLeft + maxPacketSize - 1) / maxPacketSize;
		HeapBlock <MIDIPacketList> packets;
		packets.malloc ((size_t) (32 * numPackets + message.getRawDataSize()), 1);
		packets->numPackets = (UInt32) numPackets;

		MIDIPacket* p = packets->packet;

		for (int i = 0; i < numPackets; ++i)
		{
			p->timeStamp = timeStamp;
			p->length = (UInt16) jmin (maxPacketSize, bytesLeft);
			memcpy (p->data, message.getRawData() + pos, p->length);
			pos += p->length;
			bytesLeft -= p->length;
			p = MIDIPacketNext (p);
		}

		mpe->send (packets);
	}
	else
	{
		MIDIPacketList packets;
		packets.numPackets = 1;
		packets.packet[0].timeStamp = timeStamp;
		packets.packet[0].length = (UInt16) message.getRawDataSize();
		*(int*) (packets.packet[0].data) = *(const int*) message.getRawData();

		mpe->send (&packets);
	}
}

StringArray MidiInput::getDevices()
{
	StringArray s;

	const ItemCount num = MIDIGetNumberOfSources();
	for (ItemCount i = 0; i < num; ++i)
	{
		MIDIEndpointRef source = MIDIGetSource (i);
		String name;

		if (source != 0)
			name = CoreMidiHelpers::getConnectedEndpointName (source);

		if (name.isEmpty())
			name = "<error>";

		s.add (name);
	}

	return s;
}

int MidiInput::getDefaultDeviceIndex()
{
	return 0;
}

MidiInput* MidiInput::openDevice (int index, MidiInputCallback* callback)
{
	jassert (callback != 0);

	using namespace CoreMidiHelpers;
	MidiInput* newInput = nullptr;

	if (isPositiveAndBelow (index, (int) MIDIGetNumberOfSources()))
	{
		MIDIEndpointRef endPoint = MIDIGetSource ((ItemCount) index);

		if (endPoint != 0)
		{
			CFStringRef name;

			if (CHECK_ERROR (MIDIObjectGetStringProperty (endPoint, kMIDIPropertyName, &name)))
			{
				MIDIClientRef client = getGlobalMidiClient();

				if (client != 0)
				{
					MIDIPortRef port;
					ScopedPointer <MidiPortAndCallback> mpc (new MidiPortAndCallback (*callback));

					if (CHECK_ERROR (MIDIInputPortCreate (client, name, midiInputProc, mpc, &port)))
					{
						if (CHECK_ERROR (MIDIPortConnectSource (port, endPoint, 0)))
						{
							mpc->portAndEndpoint = new MidiPortAndEndpoint (port, endPoint);

							newInput = new MidiInput (getDevices() [index]);
							mpc->input = newInput;
							newInput->internal = mpc;

							const ScopedLock sl (callbackLock);
							activeCallbacks.add (mpc.release());
						}
						else
						{
							CHECK_ERROR (MIDIPortDispose (port));
						}
					}
				}
			}

			CFRelease (name);
		}
	}

	return newInput;
}

MidiInput* MidiInput::createNewDevice (const String& deviceName, MidiInputCallback* callback)
{
	jassert (callback != nullptr);

	using namespace CoreMidiHelpers;
	MidiInput* mi = nullptr;
	MIDIClientRef client = getGlobalMidiClient();

	if (client != 0)
	{
		ScopedPointer <MidiPortAndCallback> mpc (new MidiPortAndCallback (*callback));
		mpc->active = false;

		MIDIEndpointRef endPoint;
		CFStringRef name = deviceName.toCFString();

		if (CHECK_ERROR (MIDIDestinationCreate (client, name, midiInputProc, mpc, &endPoint)))
		{
			mpc->portAndEndpoint = new MidiPortAndEndpoint (0, endPoint);

			mi = new MidiInput (deviceName);
			mpc->input = mi;
			mi->internal = mpc;

			const ScopedLock sl (callbackLock);
			activeCallbacks.add (mpc.release());
		}

		CFRelease (name);
	}

	return mi;
}

MidiInput::MidiInput (const String& name_)
	: name (name_)
{
}

MidiInput::~MidiInput()
{
	delete static_cast<CoreMidiHelpers::MidiPortAndCallback*> (internal);
}

void MidiInput::start()
{
	const ScopedLock sl (CoreMidiHelpers::callbackLock);
	static_cast<CoreMidiHelpers::MidiPortAndCallback*> (internal)->active = true;
}

void MidiInput::stop()
{
	const ScopedLock sl (CoreMidiHelpers::callbackLock);
	static_cast<CoreMidiHelpers::MidiPortAndCallback*> (internal)->active = false;
}

#undef CHECK_ERROR

/*** End of inlined file: juce_mac_CoreMidi.cpp ***/

 #if JUCE_USE_CDREADER

/*** Start of inlined file: juce_mac_AudioCDReader.mm ***/
namespace CDReaderHelpers
{
	inline const XmlElement* getElementForKey (const XmlElement& xml, const String& key)
	{
		forEachXmlChildElementWithTagName (xml, child, "key")
			if (child->getAllSubText().trim() == key)
				return child->getNextElement();

		return nullptr;
	}

	static int getIntValueForKey (const XmlElement& xml, const String& key, int defaultValue = -1)
	{
		const XmlElement* const block = getElementForKey (xml, key);
		return block != nullptr ? block->getAllSubText().trim().getIntValue() : defaultValue;
	}

	// Get the track offsets for a CD given an XmlElement representing its TOC.Plist.
	// Returns NULL on success, otherwise a const char* representing an error.
	static const char* getTrackOffsets (XmlDocument& xmlDocument, Array<int>& offsets)
	{
		const ScopedPointer<XmlElement> xml (xmlDocument.getDocumentElement());
		if (xml == nullptr)
			return "Couldn't parse XML in file";

		const XmlElement* const dict = xml->getChildByName ("dict");
		if (dict == nullptr)
			return "Couldn't get top level dictionary";

		const XmlElement* const sessions = getElementForKey (*dict, "Sessions");
		if (sessions == nullptr)
			return "Couldn't find sessions key";

		const XmlElement* const session = sessions->getFirstChildElement();
		if (session == nullptr)
			return "Couldn't find first session";

		const int leadOut = getIntValueForKey (*session, "Leadout Block");
		if (leadOut < 0)
			return "Couldn't find Leadout Block";

		const XmlElement* const trackArray = getElementForKey (*session, "Track Array");
		if (trackArray == nullptr)
			return "Couldn't find Track Array";

		forEachXmlChildElement (*trackArray, track)
		{
			const int trackValue = getIntValueForKey (*track, "Start Block");
			if (trackValue < 0)
				return "Couldn't find Start Block in the track";

			offsets.add (trackValue * AudioCDReader::samplesPerFrame - 88200);
		}

		offsets.add (leadOut * AudioCDReader::samplesPerFrame - 88200);
		return nullptr;
	}

	static void findDevices (Array<File>& cds)
	{
		File volumes ("/Volumes");
		volumes.findChildFiles (cds, File::findDirectories, false);

		for (int i = cds.size(); --i >= 0;)
			if (! cds.getReference(i).getChildFile (".TOC.plist").exists())
				cds.remove (i);
	}

	struct TrackSorter
	{
		static int getCDTrackNumber (const File& file)
		{
			return file.getFileName().initialSectionContainingOnly ("0123456789").getIntValue();
		}

		static int compareElements (const File& first, const File& second)
		{
			const int firstTrack  = getCDTrackNumber (first);
			const int secondTrack = getCDTrackNumber (second);

			jassert (firstTrack > 0 && secondTrack > 0);

			return firstTrack - secondTrack;
		}
	};
}

StringArray AudioCDReader::getAvailableCDNames()
{
	Array<File> cds;
	CDReaderHelpers::findDevices (cds);

	StringArray names;

	for (int i = 0; i < cds.size(); ++i)
		names.add (cds.getReference(i).getFileName());

	return names;
}

AudioCDReader* AudioCDReader::createReaderForCD (const int index)
{
	Array<File> cds;
	CDReaderHelpers::findDevices (cds);

	if (cds[index].exists())
		return new AudioCDReader (cds[index]);

	return nullptr;
}

AudioCDReader::AudioCDReader (const File& volume)
   : AudioFormatReader (0, "CD Audio"),
	 volumeDir (volume),
	 currentReaderTrack (-1),
	 reader (0)
{
	 sampleRate = 44100.0;
	 bitsPerSample = 16;
	 numChannels = 2;
	 usesFloatingPointData = false;

	 refreshTrackLengths();
}

AudioCDReader::~AudioCDReader()
{
}

void AudioCDReader::refreshTrackLengths()
{
	tracks.clear();
	trackStartSamples.clear();
	lengthInSamples = 0;

	volumeDir.findChildFiles (tracks, File::findFiles | File::ignoreHiddenFiles, false, "*.aiff");

	CDReaderHelpers::TrackSorter sorter;
	tracks.sort (sorter);

	const File toc (volumeDir.getChildFile (".TOC.plist"));

	if (toc.exists())
	{
		XmlDocument doc (toc);
		const char* error = CDReaderHelpers::getTrackOffsets (doc, trackStartSamples);
		(void) error; // could be logged..

		lengthInSamples = trackStartSamples.getLast() - trackStartSamples.getFirst();
	}
}

bool AudioCDReader::readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
								 int64 startSampleInFile, int numSamples)
{
	while (numSamples > 0)
	{
		int track = -1;

		for (int i = 0; i < trackStartSamples.size() - 1; ++i)
		{
			if (startSampleInFile < trackStartSamples.getUnchecked (i + 1))
			{
				track = i;
				break;
			}
		}

		if (track < 0)
			return false;

		if (track != currentReaderTrack)
		{
			reader = nullptr;

			FileInputStream* const in = tracks [track].createInputStream();

			if (in != nullptr)
			{
				BufferedInputStream* const bin = new BufferedInputStream (in, 65536, true);

				AiffAudioFormat format;
				reader = format.createReaderFor (bin, true);

				if (reader == nullptr)
					currentReaderTrack = -1;
				else
					currentReaderTrack = track;
			}
		}

		if (reader == nullptr)
			return false;

		const int startPos = (int) (startSampleInFile - trackStartSamples.getUnchecked (track));
		const int numAvailable = (int) jmin ((int64) numSamples, reader->lengthInSamples - startPos);

		reader->readSamples (destSamples, numDestChannels, startOffsetInDestBuffer, startPos, numAvailable);

		numSamples -= numAvailable;
		startSampleInFile += numAvailable;
	}

	return true;
}

bool AudioCDReader::isCDStillPresent() const
{
	return volumeDir.exists();
}

void AudioCDReader::ejectDisk()
{
	JUCE_AUTORELEASEPOOL
	[[NSWorkspace sharedWorkspace] unmountAndEjectDeviceAtPath: juceStringToNS (volumeDir.getFullPathName())];
}

bool AudioCDReader::isTrackAudio (int trackNum) const
{
	return tracks [trackNum].hasFileExtension (".aiff");
}

void AudioCDReader::enableIndexScanning (bool)
{
	// any way to do this on a Mac??
}

int AudioCDReader::getLastIndex() const
{
	return 0;
}

Array<int> AudioCDReader::findIndexesInTrack (const int /*trackNumber*/)
{
	return Array<int>();
}

/*** End of inlined file: juce_mac_AudioCDReader.mm ***/


 #endif

 #if JUCE_USE_CDBURNER

/*** Start of inlined file: juce_mac_AudioCDBurner.mm ***/
const int kilobytesPerSecond1x = 176;

} // (juce namespace)

#define OpenDiskDevice MakeObjCClassName(OpenDiskDevice)

@interface OpenDiskDevice   : NSObject
{
@public
	DRDevice* device;
	NSMutableArray* tracks;
	bool underrunProtection;
}

- (OpenDiskDevice*) initWithDRDevice: (DRDevice*) device;
- (void) dealloc;
- (void) addSourceTrack: (juce::AudioSource*) source numSamples: (int) numSamples_;
- (void) burn: (juce::AudioCDBurner::BurnProgressListener*) listener  errorString: (juce::String*) error
		 ejectAfterwards: (bool) shouldEject isFake: (bool) peformFakeBurnForTesting speed: (int) burnSpeed;
@end

#define AudioTrackProducer MakeObjCClassName(AudioTrackProducer)

@interface AudioTrackProducer   : NSObject
{
	juce::AudioSource* source;
	int readPosition, lengthInFrames;
}

- (AudioTrackProducer*) init: (int) lengthInFrames;
- (AudioTrackProducer*) initWithAudioSource: (juce::AudioSource*) source numSamples: (int) lengthInSamples;
- (void) dealloc;
- (void) setupTrackProperties: (DRTrack*) track;

- (void) cleanupTrackAfterBurn: (DRTrack*) track;
- (BOOL) cleanupTrackAfterVerification:(DRTrack*)track;
- (uint64_t) estimateLengthOfTrack:(DRTrack*)track;
- (BOOL) prepareTrack:(DRTrack*)track forBurn:(DRBurn*)burn
		 toMedia:(NSDictionary*)mediaInfo;
- (BOOL) prepareTrackForVerification:(DRTrack*)track;
- (uint32_t) produceDataForTrack:(DRTrack*)track intoBuffer:(char*)buffer
		length:(uint32_t)bufferLength atAddress:(uint64_t)address
		blockSize:(uint32_t)blockSize ioFlags:(uint32_t*)flags;
- (uint32_t) producePreGapForTrack:(DRTrack*)track
			 intoBuffer:(char*)buffer length:(uint32_t)bufferLength
			 atAddress:(uint64_t)address blockSize:(uint32_t)blockSize
			 ioFlags:(uint32_t*)flags;
- (BOOL) verifyDataForTrack:(DRTrack*)track inBuffer:(const char*)buffer
		 length:(uint32_t)bufferLength atAddress:(uint64_t)address
		 blockSize:(uint32_t)blockSize ioFlags:(uint32_t*)flags;
- (uint32_t) producePreGapForTrack:(DRTrack*)track
		intoBuffer:(char*)buffer length:(uint32_t)bufferLength
		atAddress:(uint64_t)address blockSize:(uint32_t)blockSize
		ioFlags:(uint32_t*)flags;
@end

@implementation OpenDiskDevice

- (OpenDiskDevice*) initWithDRDevice: (DRDevice*) device_
{
	[super init];

	device = device_;
	tracks = [[NSMutableArray alloc] init];
	underrunProtection = true;
	return self;
}

- (void) dealloc
{
	[tracks release];
	[super dealloc];
}

- (void) addSourceTrack: (juce::AudioSource*) source_ numSamples: (int) numSamples_
{
	AudioTrackProducer* p = [[AudioTrackProducer alloc] initWithAudioSource: source_ numSamples: numSamples_];
	DRTrack* t = [[DRTrack alloc] initWithProducer: p];
	[p setupTrackProperties: t];

	[tracks addObject: t];

	[t release];
	[p release];
}

- (void) burn: (juce::AudioCDBurner::BurnProgressListener*) listener errorString: (juce::String*) error
		 ejectAfterwards: (bool) shouldEject isFake: (bool) peformFakeBurnForTesting speed: (int) burnSpeed
{
	DRBurn* burn = [DRBurn burnForDevice: device];

	if (! [device acquireExclusiveAccess])
	{
		*error = "Couldn't open or write to the CD device";
		return;
	}

	[device acquireMediaReservation];

	NSMutableDictionary* d = [[burn properties] mutableCopy];
	[d autorelease];
	[d setObject: [NSNumber numberWithBool: peformFakeBurnForTesting] forKey: DRBurnTestingKey];
	[d setObject: [NSNumber numberWithBool: false] forKey: DRBurnVerifyDiscKey];
	[d setObject: (shouldEject ? DRBurnCompletionActionEject : DRBurnCompletionActionMount) forKey: DRBurnCompletionActionKey];

	if (burnSpeed > 0)
		[d setObject: [NSNumber numberWithFloat: burnSpeed * juce::kilobytesPerSecond1x] forKey: DRBurnRequestedSpeedKey];

	if (! underrunProtection)
		[d setObject: [NSNumber numberWithBool: false] forKey: DRBurnUnderrunProtectionKey];

	[burn setProperties: d];

	[burn writeLayout: tracks];

	for (;;)
	{
		juce::Thread::sleep (300);
		float progress = [[[burn status] objectForKey: DRStatusPercentCompleteKey] floatValue];

		if (listener != nullptr && listener->audioCDBurnProgress (progress))
		{
			[burn abort];
			*error = "User cancelled the write operation";
			break;
		}

		if ([[[burn status] objectForKey: DRStatusStateKey] isEqualTo: DRStatusStateFailed])
		{
			*error = "Write operation failed";
			break;
		}
		else if ([[[burn status] objectForKey: DRStatusStateKey] isEqualTo: DRStatusStateDone])
		{
			break;
		}

		NSString* err = (NSString*) [[[burn status] objectForKey: DRErrorStatusKey]
													objectForKey: DRErrorStatusErrorStringKey];

		if ([err length] > 0)
		{
			*error = juce::CharPointer_UTF8 ([err UTF8String]);
			break;
		}
	}

	[device releaseMediaReservation];
	[device releaseExclusiveAccess];
}
@end

@implementation AudioTrackProducer

- (AudioTrackProducer*) init: (int) lengthInFrames_
{
	lengthInFrames = lengthInFrames_;
	readPosition = 0;
	return self;
}

- (void) setupTrackProperties: (DRTrack*) track
{
	NSMutableDictionary*  p = [[track properties] mutableCopy];
	[p setObject:[DRMSF msfWithFrames: lengthInFrames] forKey: DRTrackLengthKey];
	[p setObject:[NSNumber numberWithUnsignedShort:2352] forKey: DRBlockSizeKey];
	[p setObject:[NSNumber numberWithInt:0] forKey: DRDataFormKey];
	[p setObject:[NSNumber numberWithInt:0] forKey: DRBlockTypeKey];
	[p setObject:[NSNumber numberWithInt:0] forKey: DRTrackModeKey];
	[p setObject:[NSNumber numberWithInt:0] forKey: DRSessionFormatKey];

	[track setProperties: p];
	[p release];
}

- (AudioTrackProducer*) initWithAudioSource: (juce::AudioSource*) source_ numSamples: (int) lengthInSamples
{
	AudioTrackProducer* s = [self init: (lengthInSamples + 587) / 588];

	if (s != nil)
		s->source = source_;

	return s;
}

- (void) dealloc
{
	if (source != nullptr)
	{
		source->releaseResources();
		delete source;
	}

	[super dealloc];
}

- (void) cleanupTrackAfterBurn: (DRTrack*) track
{
	(void) track;
}

- (BOOL) cleanupTrackAfterVerification: (DRTrack*) track
{
	(void) track;
	return true;
}

- (uint64_t) estimateLengthOfTrack: (DRTrack*) track
{
	(void) track;
	return lengthInFrames;
}

- (BOOL) prepareTrack: (DRTrack*) track forBurn: (DRBurn*) burn
		 toMedia: (NSDictionary*) mediaInfo
{
	(void) track; (void) burn; (void) mediaInfo;

	if (source != nullptr)
		source->prepareToPlay (44100 / 75, 44100);

	readPosition = 0;
	return true;
}

- (BOOL) prepareTrackForVerification: (DRTrack*) track
{
	(void) track;
	if (source != nullptr)
		source->prepareToPlay (44100 / 75, 44100);

	return true;
}

- (uint32_t) produceDataForTrack: (DRTrack*) track intoBuffer: (char*) buffer
		length: (uint32_t) bufferLength atAddress: (uint64_t) address
		blockSize: (uint32_t) blockSize ioFlags: (uint32_t*) flags
{
	(void) track; (void) address; (void) blockSize; (void) flags;

	if (source != nullptr)
	{
		const int numSamples = juce::jmin ((int) bufferLength / 4, (lengthInFrames * (44100 / 75)) - readPosition);

		if (numSamples > 0)
		{
			juce::AudioSampleBuffer tempBuffer (2, numSamples);
			juce::AudioSourceChannelInfo info (tempBuffer);

			source->getNextAudioBlock (info);

			typedef juce::AudioData::Pointer <juce::AudioData::Int16,
											  juce::AudioData::LittleEndian,
											  juce::AudioData::Interleaved,
											  juce::AudioData::NonConst> CDSampleFormat;

			typedef juce::AudioData::Pointer <juce::AudioData::Float32,
											  juce::AudioData::NativeEndian,
											  juce::AudioData::NonInterleaved,
											  juce::AudioData::Const> SourceSampleFormat;
			CDSampleFormat left (buffer, 2);
			left.convertSamples (SourceSampleFormat (tempBuffer.getSampleData (0)), numSamples);
			CDSampleFormat right (buffer + 2, 2);
			right.convertSamples (SourceSampleFormat (tempBuffer.getSampleData (1)), numSamples);

			readPosition += numSamples;
		}

		return numSamples * 4;
	}

	return 0;
}

- (uint32_t) producePreGapForTrack: (DRTrack*) track
		intoBuffer: (char*) buffer length: (uint32_t) bufferLength
		atAddress: (uint64_t) address blockSize: (uint32_t) blockSize
		ioFlags: (uint32_t*) flags
{
	(void) track; (void) address; (void) blockSize; (void) flags;
	zeromem (buffer, bufferLength);
	return bufferLength;
}

- (BOOL) verifyDataForTrack: (DRTrack*) track inBuffer: (const char*) buffer
		 length: (uint32_t) bufferLength atAddress: (uint64_t) address
		 blockSize: (uint32_t) blockSize ioFlags: (uint32_t*) flags
{
	(void) track; (void) buffer; (void) bufferLength; (void) address; (void) blockSize; (void) flags;
	return true;
}

@end

namespace juce
{

class AudioCDBurner::Pimpl  : public Timer
{
public:
	Pimpl (AudioCDBurner& owner_, const int deviceIndex)
		: device (0), owner (owner_)
	{
		DRDevice* dev = [[DRDevice devices] objectAtIndex: deviceIndex];
		if (dev != nil)
		{
			device = [[OpenDiskDevice alloc] initWithDRDevice: dev];
			lastState = getDiskState();
			startTimer (1000);
		}
	}

	~Pimpl()
	{
		stopTimer();
		[device release];
	}

	void timerCallback()
	{
		const DiskState state = getDiskState();

		if (state != lastState)
		{
			lastState = state;
			owner.sendChangeMessage();
		}
	}

	DiskState getDiskState() const
	{
		if ([device->device isValid])
		{
			NSDictionary* status = [device->device status];

			NSString* state = [status objectForKey: DRDeviceMediaStateKey];

			if ([state isEqualTo: DRDeviceMediaStateNone])
			{
				if ([[status objectForKey: DRDeviceIsTrayOpenKey] boolValue])
					return trayOpen;

				return noDisc;
			}

			if ([state isEqualTo: DRDeviceMediaStateMediaPresent])
			{
				if ([[[status objectForKey: DRDeviceMediaInfoKey] objectForKey: DRDeviceMediaBlocksFreeKey] intValue] > 0)
					return writableDiskPresent;
				else
					return readOnlyDiskPresent;
			}
		}

		return unknown;
	}

	bool openTray() { return [device->device isValid] && [device->device ejectMedia]; }

	Array<int> getAvailableWriteSpeeds() const
	{
		Array<int> results;

		if ([device->device isValid])
		{
			NSArray* speeds = [[[device->device status] objectForKey: DRDeviceMediaInfoKey] objectForKey: DRDeviceBurnSpeedsKey];
			for (unsigned int i = 0; i < [speeds count]; ++i)
			{
				const int kbPerSec = [[speeds objectAtIndex: i] intValue];
				results.add (kbPerSec / kilobytesPerSecond1x);
			}
		}

		return results;
	}

	bool setBufferUnderrunProtection (const bool shouldBeEnabled)
	{
		if ([device->device isValid])
		{
			device->underrunProtection = shouldBeEnabled;
			return shouldBeEnabled && [[[device->device status] objectForKey: DRDeviceCanUnderrunProtectCDKey] boolValue];
		}

		return false;
	}

	int getNumAvailableAudioBlocks() const
	{
		return [[[[device->device status] objectForKey: DRDeviceMediaInfoKey]
										  objectForKey: DRDeviceMediaBlocksFreeKey] intValue];
	}

	OpenDiskDevice* device;

private:
	DiskState lastState;
	AudioCDBurner& owner;
};

AudioCDBurner::AudioCDBurner (const int deviceIndex)
{
	pimpl = new Pimpl (*this, deviceIndex);
}

AudioCDBurner::~AudioCDBurner()
{
}

AudioCDBurner* AudioCDBurner::openDevice (const int deviceIndex)
{
	ScopedPointer <AudioCDBurner> b (new AudioCDBurner (deviceIndex));

	if (b->pimpl->device == nil)
		b = 0;

	return b.release();
}

namespace
{
	NSArray* findDiskBurnerDevices()
	{
		NSMutableArray* results = [NSMutableArray array];
		NSArray* devs = [DRDevice devices];

		for (int i = 0; i < [devs count]; ++i)
		{
			NSDictionary* dic = [[devs objectAtIndex: i] info];
			NSString* name = [dic valueForKey: DRDeviceProductNameKey];
			if (name != nil)
				[results addObject: name];
		}

		return results;
	}
}

StringArray AudioCDBurner::findAvailableDevices()
{
	NSArray* names = findDiskBurnerDevices();
	StringArray s;

	for (unsigned int i = 0; i < [names count]; ++i)
		s.add (CharPointer_UTF8 ([[names objectAtIndex: i] UTF8String]));

	return s;
}

AudioCDBurner::DiskState AudioCDBurner::getDiskState() const
{
	return pimpl->getDiskState();
}

bool AudioCDBurner::isDiskPresent() const
{
	return getDiskState() == writableDiskPresent;
}

bool AudioCDBurner::openTray()
{
	return pimpl->openTray();
}

AudioCDBurner::DiskState AudioCDBurner::waitUntilStateChange (int timeOutMilliseconds)
{
	const int64 timeout = Time::currentTimeMillis() + timeOutMilliseconds;
	DiskState oldState = getDiskState();
	DiskState newState = oldState;

	while (newState == oldState && Time::currentTimeMillis() < timeout)
	{
		newState = getDiskState();
		Thread::sleep (100);
	}

	return newState;
}

Array<int> AudioCDBurner::getAvailableWriteSpeeds() const
{
	return pimpl->getAvailableWriteSpeeds();
}

bool AudioCDBurner::setBufferUnderrunProtection (const bool shouldBeEnabled)
{
	return pimpl->setBufferUnderrunProtection (shouldBeEnabled);
}

int AudioCDBurner::getNumAvailableAudioBlocks() const
{
	return pimpl->getNumAvailableAudioBlocks();
}

bool AudioCDBurner::addAudioTrack (AudioSource* source, int numSamps)
{
	if ([pimpl->device->device isValid])
	{
		[pimpl->device addSourceTrack: source numSamples: numSamps];
		return true;
	}

	return false;
}

String AudioCDBurner::burn (juce::AudioCDBurner::BurnProgressListener* listener,
							bool ejectDiscAfterwards,
							bool performFakeBurnForTesting,
							int writeSpeed)
{
	String error ("Couldn't open or write to the CD device");

	if ([pimpl->device->device isValid])
	{
		error = String::empty;

		[pimpl->device  burn: listener
				 errorString: &error
			 ejectAfterwards: ejectDiscAfterwards
					  isFake: performFakeBurnForTesting
					   speed: writeSpeed];
	}

	return error;
}

/*** End of inlined file: juce_mac_AudioCDBurner.mm ***/


 #endif

#elif JUCE_IOS

/*** Start of inlined file: juce_ios_Audio.cpp ***/
class IPhoneAudioIODevice  : public AudioIODevice
{
public:
	IPhoneAudioIODevice (const String& deviceName)
		: AudioIODevice (deviceName, "Audio"),
		  actualBufferSize (0),
		  isRunning (false),
		  audioUnit (0),
		  callback (nullptr),
		  floatData (1, 2)
	{
		getSessionHolder().activeDevices.add (this);

		numInputChannels = 2;
		numOutputChannels = 2;
		preferredBufferSize = 0;

		updateDeviceInfo();
	}

	~IPhoneAudioIODevice()
	{
		getSessionHolder().activeDevices.removeValue (this);
		close();
	}

	StringArray getOutputChannelNames()
	{
		StringArray s;
		s.add ("Left");
		s.add ("Right");
		return s;
	}

	StringArray getInputChannelNames()
	{
		StringArray s;
		if (audioInputIsAvailable)
		{
			s.add ("Left");
			s.add ("Right");
		}
		return s;
	}

	int getNumSampleRates()                 { return 1; }
	double getSampleRate (int index)        { return sampleRate; }

	int getNumBufferSizesAvailable()        { return 6; }
	int getBufferSizeSamples (int index)    { return 1 << (jlimit (0, 5, index) + 6); }
	int getDefaultBufferSize()              { return 1024; }

	String open (const BigInteger& inputChannels,
				 const BigInteger& outputChannels,
				 double sampleRate,
				 int bufferSize)
	{
		close();

		lastError = String::empty;
		preferredBufferSize = (bufferSize <= 0) ? getDefaultBufferSize() : bufferSize;

		//  xxx set up channel mapping

		activeOutputChans = outputChannels;
		activeOutputChans.setRange (2, activeOutputChans.getHighestBit(), false);
		numOutputChannels = activeOutputChans.countNumberOfSetBits();
		monoOutputChannelNumber = activeOutputChans.findNextSetBit (0);

		activeInputChans = inputChannels;
		activeInputChans.setRange (2, activeInputChans.getHighestBit(), false);
		numInputChannels = activeInputChans.countNumberOfSetBits();
		monoInputChannelNumber = activeInputChans.findNextSetBit (0);

		AudioSessionSetActive (true);

		UInt32 audioCategory = audioInputIsAvailable ? kAudioSessionCategory_PlayAndRecord
													 : kAudioSessionCategory_MediaPlayback;

		AudioSessionSetProperty (kAudioSessionProperty_AudioCategory, sizeof (audioCategory), &audioCategory);
		AudioSessionAddPropertyListener (kAudioSessionProperty_AudioRouteChange, routingChangedStatic, this);

		fixAudioRouteIfSetToReceiver();
		updateDeviceInfo();

		Float32 bufferDuration = preferredBufferSize / sampleRate;
		AudioSessionSetProperty (kAudioSessionProperty_PreferredHardwareIOBufferDuration, sizeof (bufferDuration), &bufferDuration);
		actualBufferSize = preferredBufferSize;

		prepareFloatBuffers();

		isRunning = true;
		routingChanged (nullptr);  // creates and starts the AU

		lastError = audioUnit != 0 ? "" : "Couldn't open the device";
		return lastError;
	}

	void close()
	{
		if (isRunning)
		{
			isRunning = false;
			AudioSessionRemovePropertyListenerWithUserData (kAudioSessionProperty_AudioRouteChange, routingChangedStatic, this);
			AudioSessionSetActive (false);

			if (audioUnit != 0)
			{
				AudioComponentInstanceDispose (audioUnit);
				audioUnit = 0;
			}
		}
	}

	bool isOpen()                       { return isRunning; }

	int getCurrentBufferSizeSamples()   { return actualBufferSize; }
	double getCurrentSampleRate()       { return sampleRate; }
	int getCurrentBitDepth()            { return 16; }

	BigInteger getActiveOutputChannels() const    { return activeOutputChans; }
	BigInteger getActiveInputChannels() const     { return activeInputChans; }

	int getOutputLatencyInSamples()               { return 0; } //xxx
	int getInputLatencyInSamples()                { return 0; } //xxx

	void start (AudioIODeviceCallback* callback_)
	{
		if (isRunning && callback != callback_)
		{
			if (callback_ != nullptr)
				callback_->audioDeviceAboutToStart (this);

			const ScopedLock sl (callbackLock);
			callback = callback_;
		}
	}

	void stop()
	{
		if (isRunning)
		{
			AudioIODeviceCallback* lastCallback;

			{
				const ScopedLock sl (callbackLock);
				lastCallback = callback;
				callback = nullptr;
			}

			if (lastCallback != nullptr)
				lastCallback->audioDeviceStopped();
		}
	}

	bool isPlaying()            { return isRunning && callback != nullptr; }
	String getLastError()       { return lastError; }

private:

	CriticalSection callbackLock;
	Float64 sampleRate;
	int numInputChannels, numOutputChannels;
	int preferredBufferSize, actualBufferSize;
	bool isRunning;
	String lastError;

	AudioStreamBasicDescription format;
	AudioUnit audioUnit;
	UInt32 audioInputIsAvailable;
	AudioIODeviceCallback* callback;
	BigInteger activeOutputChans, activeInputChans;

	AudioSampleBuffer floatData;
	float* inputChannels[3];
	float* outputChannels[3];
	bool monoInputChannelNumber, monoOutputChannelNumber;

	void prepareFloatBuffers()
	{
		floatData.setSize (numInputChannels + numOutputChannels, actualBufferSize);
		zeromem (inputChannels, sizeof (inputChannels));
		zeromem (outputChannels, sizeof (outputChannels));

		for (int i = 0; i < numInputChannels; ++i)
			inputChannels[i] = floatData.getSampleData (i);

		for (int i = 0; i < numOutputChannels; ++i)
			outputChannels[i] = floatData.getSampleData (i + numInputChannels);
	}

	OSStatus process (AudioUnitRenderActionFlags* flags, const AudioTimeStamp* time,
					  const UInt32 numFrames, AudioBufferList* data)
	{
		OSStatus err = noErr;

		if (audioInputIsAvailable && numInputChannels > 0)
			err = AudioUnitRender (audioUnit, flags, time, 1, numFrames, data);

		const ScopedLock sl (callbackLock);

		if (callback != nullptr)
		{
			if (audioInputIsAvailable && numInputChannels > 0)
			{
				short* shortData = (short*) data->mBuffers[0].mData;

				if (numInputChannels >= 2)
				{
					for (UInt32 i = 0; i < numFrames; ++i)
					{
						inputChannels[0][i] = *shortData++ * (1.0f / 32768.0f);
						inputChannels[1][i] = *shortData++ * (1.0f / 32768.0f);
					}
				}
				else
				{
					if (monoInputChannelNumber > 0)
						++shortData;

					for (UInt32 i = 0; i < numFrames; ++i)
					{
						inputChannels[0][i] = *shortData++ * (1.0f / 32768.0f);
						++shortData;
					}
				}
			}
			else
			{
				for (int i = numInputChannels; --i >= 0;)
					zeromem (inputChannels[i], sizeof (float) * numFrames);
			}

			callback->audioDeviceIOCallback ((const float**) inputChannels, numInputChannels,
											 outputChannels, numOutputChannels, (int) numFrames);

			short* shortData = (short*) data->mBuffers[0].mData;
			int n = 0;

			if (numOutputChannels >= 2)
			{
				for (UInt32 i = 0; i < numFrames; ++i)
				{
					shortData [n++] = (short) (outputChannels[0][i] * 32767.0f);
					shortData [n++] = (short) (outputChannels[1][i] * 32767.0f);
				}
			}
			else if (numOutputChannels == 1)
			{
				for (UInt32 i = 0; i < numFrames; ++i)
				{
					const short s = (short) (outputChannels[monoOutputChannelNumber][i] * 32767.0f);
					shortData [n++] = s;
					shortData [n++] = s;
				}
			}
			else
			{
				zeromem (data->mBuffers[0].mData, 2 * sizeof (short) * numFrames);
			}
		}
		else
		{
			zeromem (data->mBuffers[0].mData, 2 * sizeof (short) * numFrames);
		}

		return err;
	}

	void updateDeviceInfo()
	{
		UInt32 size = sizeof (sampleRate);
		AudioSessionGetProperty (kAudioSessionProperty_CurrentHardwareSampleRate, &size, &sampleRate);

		size = sizeof (audioInputIsAvailable);
		AudioSessionGetProperty (kAudioSessionProperty_AudioInputAvailable, &size, &audioInputIsAvailable);
	}

	void routingChanged (const void* propertyValue)
	{
		if (! isRunning)
			return;

		if (propertyValue != nullptr)
		{
			CFDictionaryRef routeChangeDictionary = (CFDictionaryRef) propertyValue;
			CFNumberRef routeChangeReasonRef = (CFNumberRef) CFDictionaryGetValue (routeChangeDictionary,
																				   CFSTR (kAudioSession_AudioRouteChangeKey_Reason));

			SInt32 routeChangeReason;
			CFNumberGetValue (routeChangeReasonRef, kCFNumberSInt32Type, &routeChangeReason);

			if (routeChangeReason == kAudioSessionRouteChangeReason_OldDeviceUnavailable)
				fixAudioRouteIfSetToReceiver();
		}

		updateDeviceInfo();
		createAudioUnit();

		AudioSessionSetActive (true);

		if (audioUnit != 0)
		{
			UInt32 formatSize = sizeof (format);
			AudioUnitGetProperty (audioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &format, &formatSize);

			Float32 bufferDuration = preferredBufferSize / sampleRate;
			UInt32 bufferDurationSize = sizeof (bufferDuration);
			AudioSessionGetProperty (kAudioSessionProperty_CurrentHardwareIOBufferDuration, &bufferDurationSize, &bufferDurationSize);
			actualBufferSize = (int) (sampleRate * bufferDuration + 0.5);

			AudioOutputUnitStart (audioUnit);
		}
	}

	struct AudioSessionHolder
	{
		AudioSessionHolder()
		{
			AudioSessionInitialize (0, 0, interruptionListenerCallback, this);
		}

		static void interruptionListenerCallback (void* client, UInt32 interruptionType)
		{
			const Array <IPhoneAudioIODevice*>& activeDevices = static_cast <AudioSessionHolder*> (client)->activeDevices;

			for (int i = activeDevices.size(); --i >= 0;)
				activeDevices.getUnchecked(i)->interruptionListener (interruptionType);
		}

		Array <IPhoneAudioIODevice*> activeDevices;
	};

	static AudioSessionHolder& getSessionHolder()
	{
		static AudioSessionHolder audioSessionHolder;
		return audioSessionHolder;
	}

	void interruptionListener (const UInt32 interruptionType)
	{
		/*if (interruptionType == kAudioSessionBeginInterruption)
		{
			isRunning = false;
			AudioOutputUnitStop (audioUnit);

			if (juce_iPhoneShowModalAlert ("Audio Interrupted",
										   "This could have been interrupted by another application or by unplugging a headset",
										   @"Resume",
										   @"Cancel"))
			{
				isRunning = true;
				routingChanged (nullptr);
			}
		}*/

		if (interruptionType == kAudioSessionEndInterruption)
		{
			isRunning = true;
			AudioSessionSetActive (true);
			AudioOutputUnitStart (audioUnit);
		}
	}

	static OSStatus processStatic (void* client, AudioUnitRenderActionFlags* flags, const AudioTimeStamp* time,
								   UInt32 /*busNumber*/, UInt32 numFrames, AudioBufferList* data)
	{
		return static_cast <IPhoneAudioIODevice*> (client)->process (flags, time, numFrames, data);
	}

	static void routingChangedStatic (void* client, AudioSessionPropertyID, UInt32 /*inDataSize*/, const void* propertyValue)
	{
		static_cast <IPhoneAudioIODevice*> (client)->routingChanged (propertyValue);
	}

	void resetFormat (const int numChannels) noexcept
	{
		zerostruct (format);
		format.mFormatID = kAudioFormatLinearPCM;
		format.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked | kAudioFormatFlagsNativeEndian;
		format.mBitsPerChannel = 8 * sizeof (short);
		format.mChannelsPerFrame = numChannels;
		format.mFramesPerPacket = 1;
		format.mBytesPerFrame = format.mBytesPerPacket = numChannels * sizeof (short);
	}

	bool createAudioUnit()
	{
		if (audioUnit != 0)
		{
			AudioComponentInstanceDispose (audioUnit);
			audioUnit = 0;
		}

		resetFormat (2);

		AudioComponentDescription desc;
		desc.componentType = kAudioUnitType_Output;
		desc.componentSubType = kAudioUnitSubType_RemoteIO;
		desc.componentManufacturer = kAudioUnitManufacturer_Apple;
		desc.componentFlags = 0;
		desc.componentFlagsMask = 0;

		AudioComponent comp = AudioComponentFindNext (0, &desc);
		AudioComponentInstanceNew (comp, &audioUnit);

		if (audioUnit == 0)
			return false;

		if (numInputChannels > 0)
		{
			const UInt32 one = 1;
			AudioUnitSetProperty (audioUnit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, 1, &one, sizeof (one));
		}

		{
			AudioChannelLayout layout;
			layout.mChannelBitmap = 0;
			layout.mNumberChannelDescriptions = 0;
			layout.mChannelLayoutTag = kAudioChannelLayoutTag_Stereo;
			AudioUnitSetProperty (audioUnit, kAudioUnitProperty_AudioChannelLayout, kAudioUnitScope_Input,  0, &layout, sizeof (layout));
			AudioUnitSetProperty (audioUnit, kAudioUnitProperty_AudioChannelLayout, kAudioUnitScope_Output, 0, &layout, sizeof (layout));
		}

		{
			AURenderCallbackStruct inputProc;
			inputProc.inputProc = processStatic;
			inputProc.inputProcRefCon = this;
			AudioUnitSetProperty (audioUnit, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0, &inputProc, sizeof (inputProc));
		}

		AudioUnitSetProperty (audioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input,  0, &format, sizeof (format));
		AudioUnitSetProperty (audioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &format, sizeof (format));

		AudioUnitInitialize (audioUnit);
		return true;
	}

	// If the routing is set to go through the receiver (i.e. the speaker, but quiet), this re-routes it
	// to make it loud. Needed because by default when using an input + output, the output is kept quiet.
	static void fixAudioRouteIfSetToReceiver()
	{
		CFStringRef audioRoute = 0;
		UInt32 propertySize = sizeof (audioRoute);
		if (AudioSessionGetProperty (kAudioSessionProperty_AudioRoute, &propertySize, &audioRoute) == noErr)
		{
			NSString* route = (NSString*) audioRoute;

			//DBG ("audio route: " + nsStringToJuce (route));

			if ([route hasPrefix: @"Receiver"])
			{
				UInt32 audioRouteOverride = kAudioSessionOverrideAudioRoute_Speaker;
				AudioSessionSetProperty (kAudioSessionProperty_OverrideAudioRoute, sizeof (audioRouteOverride), &audioRouteOverride);
			}

			CFRelease (audioRoute);
		}
	}

	JUCE_DECLARE_NON_COPYABLE (IPhoneAudioIODevice);
};

class IPhoneAudioIODeviceType  : public AudioIODeviceType
{
public:

	IPhoneAudioIODeviceType()
		: AudioIODeviceType ("iPhone Audio")
	{
	}

	void scanForDevices() {}

	StringArray getDeviceNames (bool wantInputNames) const
	{
		return StringArray ("iPhone Audio");
	}

	int getDefaultDeviceIndex (bool forInput) const
	{
		return 0;
	}

	int getIndexOfDevice (AudioIODevice* device, bool asInput) const
	{
		return device != nullptr ? 0 : -1;
	}

	bool hasSeparateInputsAndOutputs() const    { return false; }

	AudioIODevice* createDevice (const String& outputDeviceName,
								 const String& inputDeviceName)
	{
		if (outputDeviceName.isNotEmpty() || inputDeviceName.isNotEmpty())
			return new IPhoneAudioIODevice (outputDeviceName.isNotEmpty() ? outputDeviceName
																		  : inputDeviceName);

		return nullptr;
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IPhoneAudioIODeviceType);
};

AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_iOSAudio()
{
	return new IPhoneAudioIODeviceType();
}

/*** End of inlined file: juce_ios_Audio.cpp ***/



/*** Start of inlined file: juce_mac_CoreMidi.cpp ***/
namespace CoreMidiHelpers
{
	static bool logError (const OSStatus err, const int lineNum)
	{
		if (err == noErr)
			return true;

		Logger::writeToLog ("CoreMidi error: " + String (lineNum) + " - " + String::toHexString ((int) err));
		jassertfalse;
		return false;
	}

	#undef CHECK_ERROR
	#define CHECK_ERROR(a) CoreMidiHelpers::logError (a, __LINE__)

	static String getEndpointName (MIDIEndpointRef endpoint, bool isExternal)
	{
		String result;
		CFStringRef str = 0;

		MIDIObjectGetStringProperty (endpoint, kMIDIPropertyName, &str);

		if (str != 0)
		{
			result = String::fromCFString (str);
			CFRelease (str);
			str = 0;
		}

		MIDIEntityRef entity = 0;
		MIDIEndpointGetEntity (endpoint, &entity);

		if (entity == 0)
			return result; // probably virtual

		if (result.isEmpty())
		{
			// endpoint name has zero length - try the entity
			MIDIObjectGetStringProperty (entity, kMIDIPropertyName, &str);

			if (str != 0)
			{
				result += String::fromCFString (str);
				CFRelease (str);
				str = 0;
			}
		}

		// now consider the device's name
		MIDIDeviceRef device = 0;
		MIDIEntityGetDevice (entity, &device);
		if (device == 0)
			return result;

		MIDIObjectGetStringProperty (device, kMIDIPropertyName, &str);

		if (str != 0)
		{
			const String s (String::fromCFString (str));
			CFRelease (str);

			// if an external device has only one entity, throw away
			// the endpoint name and just use the device name
			if (isExternal && MIDIDeviceGetNumberOfEntities (device) < 2)
			{
				result = s;
			}
			else if (! result.startsWithIgnoreCase (s))
			{
				// prepend the device name to the entity name
				result = (s + " " + result).trimEnd();
			}
		}

		return result;
	}

	static String getConnectedEndpointName (MIDIEndpointRef endpoint)
	{
		String result;

		// Does the endpoint have connections?
		CFDataRef connections = 0;
		int numConnections = 0;

		MIDIObjectGetDataProperty (endpoint, kMIDIPropertyConnectionUniqueID, &connections);

		if (connections != 0)
		{
			numConnections = ((int) CFDataGetLength (connections)) / (int) sizeof (MIDIUniqueID);

			if (numConnections > 0)
			{
				const SInt32* pid = reinterpret_cast <const SInt32*> (CFDataGetBytePtr (connections));

				for (int i = 0; i < numConnections; ++i, ++pid)
				{
					MIDIUniqueID uid = (MIDIUniqueID) ByteOrder::swapIfLittleEndian ((uint32) *pid);
					MIDIObjectRef connObject;
					MIDIObjectType connObjectType;
					OSStatus err = MIDIObjectFindByUniqueID (uid, &connObject, &connObjectType);

					if (err == noErr)
					{
						String s;

						if (connObjectType == kMIDIObjectType_ExternalSource
							 || connObjectType == kMIDIObjectType_ExternalDestination)
						{
							// Connected to an external device's endpoint (10.3 and later).
							s = getEndpointName (static_cast <MIDIEndpointRef> (connObject), true);
						}
						else
						{
							// Connected to an external device (10.2) (or something else, catch-all)
							CFStringRef str = 0;
							MIDIObjectGetStringProperty (connObject, kMIDIPropertyName, &str);

							if (str != 0)
							{
								s = String::fromCFString (str);
								CFRelease (str);
							}
						}

						if (s.isNotEmpty())
						{
							if (result.isNotEmpty())
								result += ", ";

							result += s;
						}
					}
				}
			}

			CFRelease (connections);
		}

		if (result.isNotEmpty())
			return result;

		// Here, either the endpoint had no connections, or we failed to obtain names for any of them.
		return getEndpointName (endpoint, false);
	}

	static MIDIClientRef getGlobalMidiClient()
	{
		static MIDIClientRef globalMidiClient = 0;

		if (globalMidiClient == 0)
		{
			String name ("JUCE");

			if (JUCEApplicationBase::getInstance() != nullptr)
				name = JUCEApplicationBase::getInstance()->getApplicationName();

			CFStringRef appName = name.toCFString();
			CHECK_ERROR (MIDIClientCreate (appName, 0, 0, &globalMidiClient));
			CFRelease (appName);
		}

		return globalMidiClient;
	}

	class MidiPortAndEndpoint
	{
	public:
		MidiPortAndEndpoint (MIDIPortRef port_, MIDIEndpointRef endPoint_)
			: port (port_), endPoint (endPoint_)
		{
		}

		~MidiPortAndEndpoint()
		{
			if (port != 0)
				MIDIPortDispose (port);

			if (port == 0 && endPoint != 0) // if port == 0, it means we created the endpoint, so it's safe to delete it
				MIDIEndpointDispose (endPoint);
		}

		void send (const MIDIPacketList* const packets)
		{
			if (port != 0)
				MIDISend (port, endPoint, packets);
			else
				MIDIReceived (endPoint, packets);
		}

		MIDIPortRef port;
		MIDIEndpointRef endPoint;
	};

	class MidiPortAndCallback;
	CriticalSection callbackLock;
	Array<MidiPortAndCallback*> activeCallbacks;

	class MidiPortAndCallback
	{
	public:
		MidiPortAndCallback (MidiInputCallback& callback_)
			: input (nullptr), active (false), callback (callback_), concatenator (2048)
		{
		}

		~MidiPortAndCallback()
		{
			active = false;

			{
				const ScopedLock sl (callbackLock);
				activeCallbacks.removeValue (this);
			}

			if (portAndEndpoint != nullptr && portAndEndpoint->port != 0)
				CHECK_ERROR (MIDIPortDisconnectSource (portAndEndpoint->port, portAndEndpoint->endPoint));
		}

		void handlePackets (const MIDIPacketList* const pktlist)
		{
			const double time = Time::getMillisecondCounterHiRes() * 0.001;

			const ScopedLock sl (callbackLock);
			if (activeCallbacks.contains (this) && active)
			{
				const MIDIPacket* packet = &pktlist->packet[0];

				for (unsigned int i = 0; i < pktlist->numPackets; ++i)
				{
					concatenator.pushMidiData (packet->data, (int) packet->length, time,
											   input, callback);

					packet = MIDIPacketNext (packet);
				}
			}
		}

		MidiInput* input;
		ScopedPointer<MidiPortAndEndpoint> portAndEndpoint;
		volatile bool active;

	private:
		MidiInputCallback& callback;
		MidiDataConcatenator concatenator;
	};

	static void midiInputProc (const MIDIPacketList* pktlist, void* readProcRefCon, void* /*srcConnRefCon*/)
	{
		static_cast <MidiPortAndCallback*> (readProcRefCon)->handlePackets (pktlist);
	}
}

StringArray MidiOutput::getDevices()
{
	StringArray s;

	const ItemCount num = MIDIGetNumberOfDestinations();
	for (ItemCount i = 0; i < num; ++i)
	{
		MIDIEndpointRef dest = MIDIGetDestination (i);
		String name;

		if (dest != 0)
			name = CoreMidiHelpers::getConnectedEndpointName (dest);

		if (name.isEmpty())
			name = "<error>";

		s.add (name);
	}

	return s;
}

int MidiOutput::getDefaultDeviceIndex()
{
	return 0;
}

MidiOutput* MidiOutput::openDevice (int index)
{
	MidiOutput* mo = nullptr;

	if (isPositiveAndBelow (index, (int) MIDIGetNumberOfDestinations()))
	{
		MIDIEndpointRef endPoint = MIDIGetDestination ((ItemCount) index);

		CFStringRef pname;
		if (CHECK_ERROR (MIDIObjectGetStringProperty (endPoint, kMIDIPropertyName, &pname)))
		{
			MIDIClientRef client = CoreMidiHelpers::getGlobalMidiClient();
			MIDIPortRef port;

			if (client != 0 && CHECK_ERROR (MIDIOutputPortCreate (client, pname, &port)))
			{
				mo = new MidiOutput();
				mo->internal = new CoreMidiHelpers::MidiPortAndEndpoint (port, endPoint);
			}

			CFRelease (pname);
		}
	}

	return mo;
}

MidiOutput* MidiOutput::createNewDevice (const String& deviceName)
{
	MidiOutput* mo = nullptr;
	MIDIClientRef client = CoreMidiHelpers::getGlobalMidiClient();

	MIDIEndpointRef endPoint;
	CFStringRef name = deviceName.toCFString();

	if (client != 0 && CHECK_ERROR (MIDISourceCreate (client, name, &endPoint)))
	{
		mo = new MidiOutput();
		mo->internal = new CoreMidiHelpers::MidiPortAndEndpoint (0, endPoint);
	}

	CFRelease (name);
	return mo;
}

MidiOutput::~MidiOutput()
{
	delete static_cast<CoreMidiHelpers::MidiPortAndEndpoint*> (internal);
}

void MidiOutput::sendMessageNow (const MidiMessage& message)
{
	CoreMidiHelpers::MidiPortAndEndpoint* const mpe = static_cast<CoreMidiHelpers::MidiPortAndEndpoint*> (internal);

   #if JUCE_IOS
	const MIDITimeStamp timeStamp = mach_absolute_time();
   #else
	const MIDITimeStamp timeStamp = AudioGetCurrentHostTime();
   #endif

	if (message.isSysEx())
	{
		const int maxPacketSize = 256;
		int pos = 0, bytesLeft = message.getRawDataSize();
		const int numPackets = (bytesLeft + maxPacketSize - 1) / maxPacketSize;
		HeapBlock <MIDIPacketList> packets;
		packets.malloc ((size_t) (32 * numPackets + message.getRawDataSize()), 1);
		packets->numPackets = (UInt32) numPackets;

		MIDIPacket* p = packets->packet;

		for (int i = 0; i < numPackets; ++i)
		{
			p->timeStamp = timeStamp;
			p->length = (UInt16) jmin (maxPacketSize, bytesLeft);
			memcpy (p->data, message.getRawData() + pos, p->length);
			pos += p->length;
			bytesLeft -= p->length;
			p = MIDIPacketNext (p);
		}

		mpe->send (packets);
	}
	else
	{
		MIDIPacketList packets;
		packets.numPackets = 1;
		packets.packet[0].timeStamp = timeStamp;
		packets.packet[0].length = (UInt16) message.getRawDataSize();
		*(int*) (packets.packet[0].data) = *(const int*) message.getRawData();

		mpe->send (&packets);
	}
}

StringArray MidiInput::getDevices()
{
	StringArray s;

	const ItemCount num = MIDIGetNumberOfSources();
	for (ItemCount i = 0; i < num; ++i)
	{
		MIDIEndpointRef source = MIDIGetSource (i);
		String name;

		if (source != 0)
			name = CoreMidiHelpers::getConnectedEndpointName (source);

		if (name.isEmpty())
			name = "<error>";

		s.add (name);
	}

	return s;
}

int MidiInput::getDefaultDeviceIndex()
{
	return 0;
}

MidiInput* MidiInput::openDevice (int index, MidiInputCallback* callback)
{
	jassert (callback != 0);

	using namespace CoreMidiHelpers;
	MidiInput* newInput = nullptr;

	if (isPositiveAndBelow (index, (int) MIDIGetNumberOfSources()))
	{
		MIDIEndpointRef endPoint = MIDIGetSource ((ItemCount) index);

		if (endPoint != 0)
		{
			CFStringRef name;

			if (CHECK_ERROR (MIDIObjectGetStringProperty (endPoint, kMIDIPropertyName, &name)))
			{
				MIDIClientRef client = getGlobalMidiClient();

				if (client != 0)
				{
					MIDIPortRef port;
					ScopedPointer <MidiPortAndCallback> mpc (new MidiPortAndCallback (*callback));

					if (CHECK_ERROR (MIDIInputPortCreate (client, name, midiInputProc, mpc, &port)))
					{
						if (CHECK_ERROR (MIDIPortConnectSource (port, endPoint, 0)))
						{
							mpc->portAndEndpoint = new MidiPortAndEndpoint (port, endPoint);

							newInput = new MidiInput (getDevices() [index]);
							mpc->input = newInput;
							newInput->internal = mpc;

							const ScopedLock sl (callbackLock);
							activeCallbacks.add (mpc.release());
						}
						else
						{
							CHECK_ERROR (MIDIPortDispose (port));
						}
					}
				}
			}

			CFRelease (name);
		}
	}

	return newInput;
}

MidiInput* MidiInput::createNewDevice (const String& deviceName, MidiInputCallback* callback)
{
	jassert (callback != nullptr);

	using namespace CoreMidiHelpers;
	MidiInput* mi = nullptr;
	MIDIClientRef client = getGlobalMidiClient();

	if (client != 0)
	{
		ScopedPointer <MidiPortAndCallback> mpc (new MidiPortAndCallback (*callback));
		mpc->active = false;

		MIDIEndpointRef endPoint;
		CFStringRef name = deviceName.toCFString();

		if (CHECK_ERROR (MIDIDestinationCreate (client, name, midiInputProc, mpc, &endPoint)))
		{
			mpc->portAndEndpoint = new MidiPortAndEndpoint (0, endPoint);

			mi = new MidiInput (deviceName);
			mpc->input = mi;
			mi->internal = mpc;

			const ScopedLock sl (callbackLock);
			activeCallbacks.add (mpc.release());
		}

		CFRelease (name);
	}

	return mi;
}

MidiInput::MidiInput (const String& name_)
	: name (name_)
{
}

MidiInput::~MidiInput()
{
	delete static_cast<CoreMidiHelpers::MidiPortAndCallback*> (internal);
}

void MidiInput::start()
{
	const ScopedLock sl (CoreMidiHelpers::callbackLock);
	static_cast<CoreMidiHelpers::MidiPortAndCallback*> (internal)->active = true;
}

void MidiInput::stop()
{
	const ScopedLock sl (CoreMidiHelpers::callbackLock);
	static_cast<CoreMidiHelpers::MidiPortAndCallback*> (internal)->active = false;
}

#undef CHECK_ERROR

/*** End of inlined file: juce_mac_CoreMidi.cpp ***/

#elif JUCE_WINDOWS

/*** Start of inlined file: juce_win32_ComSmartPtr.h ***/
#ifndef __JUCE_WIN32_COMSMARTPTR_JUCEHEADER__
#define __JUCE_WIN32_COMSMARTPTR_JUCEHEADER__

/** A simple COM smart pointer.
*/
template <class ComClass>
class ComSmartPtr
{
public:
	ComSmartPtr() throw() : p (0)                               {}
	ComSmartPtr (ComClass* const p_) : p (p_)                   { if (p_ != 0) p_->AddRef(); }
	ComSmartPtr (const ComSmartPtr<ComClass>& p_) : p (p_.p)    { if (p  != 0) p ->AddRef(); }
	~ComSmartPtr()                                              { release(); }

	operator ComClass*() const throw()     { return p; }
	ComClass& operator*() const throw()    { return *p; }
	ComClass* operator->() const throw()   { return p; }

	ComSmartPtr& operator= (ComClass* const newP)
	{
		if (newP != 0)  newP->AddRef();
		release();
		p = newP;
		return *this;
	}

	ComSmartPtr& operator= (const ComSmartPtr<ComClass>& newP)  { return operator= (newP.p); }

	// Releases and nullifies this pointer and returns its address
	ComClass** resetAndGetPointerAddress()
	{
		release();
		p = 0;
		return &p;
	}

	HRESULT CoCreateInstance (REFCLSID classUUID, DWORD dwClsContext = CLSCTX_INPROC_SERVER)
	{
	   #if ! JUCE_MINGW
		return ::CoCreateInstance (classUUID, 0, dwClsContext, __uuidof (ComClass), (void**) resetAndGetPointerAddress());
	   #else
		jassertfalse; // need to find a mingw equivalent of __uuidof to make this possible
		return E_NOTIMPL;
	   #endif
	}

	template <class OtherComClass>
	HRESULT QueryInterface (REFCLSID classUUID, ComSmartPtr<OtherComClass>& destObject) const
	{
		if (p == 0)
			return E_POINTER;

		return p->QueryInterface (classUUID, (void**) destObject.resetAndGetPointerAddress());
	}

	template <class OtherComClass>
	HRESULT QueryInterface (ComSmartPtr<OtherComClass>& destObject) const
	{
	   #if ! JUCE_MINGW
		return this->QueryInterface (__uuidof (OtherComClass), destObject);
	   #else
		jassertfalse; // need to find a mingw equivalent of __uuidof to make this possible
		return E_NOTIMPL;
	   #endif
	}

private:
	ComClass* p;

	void release()  { if (p != 0) p->Release(); }

	ComClass** operator&() throw(); // private to avoid it being used accidentally
};

#define JUCE_COMRESULT  HRESULT __stdcall

template <class ComClass>
class ComBaseClassHelperBase   : public ComClass
{
public:
	ComBaseClassHelperBase()  : refCount (1) {}
	virtual ~ComBaseClassHelperBase() {}

	ULONG __stdcall AddRef()    { return ++refCount; }
	ULONG __stdcall Release()   { const ULONG r = --refCount; if (r == 0) delete this; return r; }

	void resetReferenceCount() noexcept     { refCount = 0; }

protected:
	ULONG refCount;
};

/** Handy base class for writing COM objects, providing ref-counting and a basic QueryInterface method.
*/
template <class ComClass>
class ComBaseClassHelper   : public ComBaseClassHelperBase <ComClass>
{
public:
	ComBaseClassHelper() {}
	~ComBaseClassHelper() {}

	JUCE_COMRESULT QueryInterface (REFIID refId, void** result)
	{
	   #if ! JUCE_MINGW
		if (refId == __uuidof (ComClass))   { AddRef(); *result = dynamic_cast <ComClass*> (this); return S_OK; }
	   #else
		jassertfalse; // need to find a mingw equivalent of __uuidof to make this possible
	   #endif

		if (refId == IID_IUnknown)          { AddRef(); *result = dynamic_cast <IUnknown*> (this); return S_OK; }

		*result = 0;
		return E_NOINTERFACE;
	}
};

#endif   // __JUCE_WIN32_COMSMARTPTR_JUCEHEADER__

/*** End of inlined file: juce_win32_ComSmartPtr.h ***/



/*** Start of inlined file: juce_win32_HiddenMessageWindow.h ***/
#ifndef __JUCE_WIN32_HIDDENMESSAGEWINDOW_JUCEHEADER__
#define __JUCE_WIN32_HIDDENMESSAGEWINDOW_JUCEHEADER__

class HiddenMessageWindow
{
public:
	HiddenMessageWindow (const TCHAR* const messageWindowName, WNDPROC wndProc)
	{
		String className ("JUCE_");
		className << String::toHexString (Time::getHighResolutionTicks());

		HMODULE moduleHandle = (HMODULE) Process::getCurrentModuleInstanceHandle();

		WNDCLASSEX wc = { 0 };
		wc.cbSize         = sizeof (wc);
		wc.lpfnWndProc    = wndProc;
		wc.cbWndExtra     = 4;
		wc.hInstance      = moduleHandle;
		wc.lpszClassName  = className.toWideCharPointer();

		atom = RegisterClassEx (&wc);
		jassert (atom != 0);

		hwnd = CreateWindow (getClassNameFromAtom(), messageWindowName,
							 0, 0, 0, 0, 0, 0, 0, moduleHandle, 0);
		jassert (hwnd != 0);
	}

	~HiddenMessageWindow()
	{
		DestroyWindow (hwnd);
		UnregisterClass (getClassNameFromAtom(), 0);
	}

	inline HWND getHWND() const noexcept     { return hwnd; }

private:
	ATOM atom;
	HWND hwnd;

	LPCTSTR getClassNameFromAtom() noexcept  { return (LPCTSTR) MAKELONG (atom, 0); }
};

class JuceWindowIdentifier
{
public:
	static bool isJUCEWindow (HWND hwnd) noexcept
	{
		return GetWindowLongPtr (hwnd, GWLP_USERDATA) == getImprobableWindowNumber();
	}

	static void setAsJUCEWindow (HWND hwnd, bool isJuceWindow) noexcept
	{
		SetWindowLongPtr (hwnd, GWLP_USERDATA, isJuceWindow ? getImprobableWindowNumber() : 0);
	}

private:
	static LONG_PTR getImprobableWindowNumber() noexcept
	{
		static LONG_PTR number = (LONG_PTR) Random::getSystemRandom().nextInt64();
		return number;
	}
};

class DeviceChangeDetector  : private Timer
{
public:
	DeviceChangeDetector (const wchar_t* const name)
		: messageWindow (name, (WNDPROC) deviceChangeEventCallback)
	{
		SetWindowLongPtr (messageWindow.getHWND(), GWLP_USERDATA, (LONG_PTR) this);
	}

	virtual ~DeviceChangeDetector() {}

protected:
	virtual void systemDeviceChanged() = 0;

private:
	HiddenMessageWindow messageWindow;

	static LRESULT CALLBACK deviceChangeEventCallback (HWND h, const UINT message,
													   const WPARAM wParam, const LPARAM lParam)
	{
		if (message == WM_DEVICECHANGE
			 && (wParam == 0x8000 /*DBT_DEVICEARRIVAL*/
				  || wParam == 0x8004 /*DBT_DEVICEREMOVECOMPLETE*/
				  || wParam == 0x0007 /*DBT_DEVNODES_CHANGED*/))
		{
			// We'll pause before sending a message, because on device removal, the OS hasn't always updated
			// its device lists correctly at this point. This also helps avoid repeated callbacks.
			((DeviceChangeDetector*) GetWindowLongPtr (h, GWLP_USERDATA))->startTimer (500);
		}

		return DefWindowProc (h, message, wParam, lParam);
	}

	void timerCallback()
	{
		systemDeviceChanged();
	}
};

#endif   // __JUCE_WIN32_HIDDENMESSAGEWINDOW_JUCEHEADER__

/*** End of inlined file: juce_win32_HiddenMessageWindow.h ***/

 #if JUCE_WASAPI

/*** Start of inlined file: juce_win32_WASAPI.cpp ***/
#ifndef WASAPI_ENABLE_LOGGING
 #define WASAPI_ENABLE_LOGGING 0
#endif

namespace WasapiClasses
{

void logFailure (HRESULT hr)
{
	(void) hr;

  #if WASAPI_ENABLE_LOGGING
	if (FAILED (hr))
	{
		String e;
		e << Time::getCurrentTime().toString (true, true, true, true)
		  << " -- WASAPI error: ";

		switch (hr)
		{
			case E_POINTER:                                 e << "E_POINTER"; break;
			case E_INVALIDARG:                              e << "E_INVALIDARG"; break;
			case AUDCLNT_E_NOT_INITIALIZED:                 e << "AUDCLNT_E_NOT_INITIALIZED"; break;
			case AUDCLNT_E_ALREADY_INITIALIZED:             e << "AUDCLNT_E_ALREADY_INITIALIZED"; break;
			case AUDCLNT_E_WRONG_ENDPOINT_TYPE:             e << "AUDCLNT_E_WRONG_ENDPOINT_TYPE"; break;
			case AUDCLNT_E_DEVICE_INVALIDATED:              e << "AUDCLNT_E_DEVICE_INVALIDATED"; break;
			case AUDCLNT_E_NOT_STOPPED:                     e << "AUDCLNT_E_NOT_STOPPED"; break;
			case AUDCLNT_E_BUFFER_TOO_LARGE:                e << "AUDCLNT_E_BUFFER_TOO_LARGE"; break;
			case AUDCLNT_E_OUT_OF_ORDER:                    e << "AUDCLNT_E_OUT_OF_ORDER"; break;
			case AUDCLNT_E_UNSUPPORTED_FORMAT:              e << "AUDCLNT_E_UNSUPPORTED_FORMAT"; break;
			case AUDCLNT_E_INVALID_SIZE:                    e << "AUDCLNT_E_INVALID_SIZE"; break;
			case AUDCLNT_E_DEVICE_IN_USE:                   e << "AUDCLNT_E_DEVICE_IN_USE"; break;
			case AUDCLNT_E_BUFFER_OPERATION_PENDING:        e << "AUDCLNT_E_BUFFER_OPERATION_PENDING"; break;
			case AUDCLNT_E_THREAD_NOT_REGISTERED:           e << "AUDCLNT_E_THREAD_NOT_REGISTERED"; break;
			case AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED:      e << "AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED"; break;
			case AUDCLNT_E_ENDPOINT_CREATE_FAILED:          e << "AUDCLNT_E_ENDPOINT_CREATE_FAILED"; break;
			case AUDCLNT_E_SERVICE_NOT_RUNNING:             e << "AUDCLNT_E_SERVICE_NOT_RUNNING"; break;
			case AUDCLNT_E_EVENTHANDLE_NOT_EXPECTED:        e << "AUDCLNT_E_EVENTHANDLE_NOT_EXPECTED"; break;
			case AUDCLNT_E_EXCLUSIVE_MODE_ONLY:             e << "AUDCLNT_E_EXCLUSIVE_MODE_ONLY"; break;
			case AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL:    e << "AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL"; break;
			case AUDCLNT_E_EVENTHANDLE_NOT_SET:             e << "AUDCLNT_E_EVENTHANDLE_NOT_SET"; break;
			case AUDCLNT_E_INCORRECT_BUFFER_SIZE:           e << "AUDCLNT_E_INCORRECT_BUFFER_SIZE"; break;
			case AUDCLNT_E_BUFFER_SIZE_ERROR:               e << "AUDCLNT_E_BUFFER_SIZE_ERROR"; break;
			case AUDCLNT_S_BUFFER_EMPTY:                    e << "AUDCLNT_S_BUFFER_EMPTY"; break;
			case AUDCLNT_S_THREAD_ALREADY_REGISTERED:       e << "AUDCLNT_S_THREAD_ALREADY_REGISTERED"; break;
			default:                                        e << String::toHexString ((int) hr); break;
		}

		DBG (e);
		jassertfalse;
	}
  #endif
}

#undef check

bool check (HRESULT hr)
{
	logFailure (hr);
	return SUCCEEDED (hr);
}

String getDeviceID (IMMDevice* const device)
{
	String s;
	WCHAR* deviceId = nullptr;

	if (check (device->GetId (&deviceId)))
	{
		s = String (deviceId);
		CoTaskMemFree (deviceId);
	}

	return s;
}

EDataFlow getDataFlow (const ComSmartPtr<IMMDevice>& device)
{
	EDataFlow flow = eRender;
	ComSmartPtr <IMMEndpoint> endPoint;
	if (check (device.QueryInterface (endPoint)))
		(void) check (endPoint->GetDataFlow (&flow));

	return flow;
}

int refTimeToSamples (const REFERENCE_TIME& t, const double sampleRate) noexcept
{
	return roundDoubleToInt (sampleRate * ((double) t) * 0.0000001);
}

void copyWavFormat (WAVEFORMATEXTENSIBLE& dest, const WAVEFORMATEX* const src) noexcept
{
	memcpy (&dest, src, src->wFormatTag == WAVE_FORMAT_EXTENSIBLE ? sizeof (WAVEFORMATEXTENSIBLE)
																  : sizeof (WAVEFORMATEX));
}

class WASAPIDeviceBase
{
public:
	WASAPIDeviceBase (const ComSmartPtr <IMMDevice>& device_, const bool useExclusiveMode_)
		: device (device_),
		  sampleRate (0),
		  defaultSampleRate (0),
		  numChannels (0),
		  actualNumChannels (0),
		  minBufferSize (0),
		  defaultBufferSize (0),
		  latencySamples (0),
		  useExclusiveMode (useExclusiveMode_),
		  sampleRateHasChanged (false)
	{
		clientEvent = CreateEvent (0, false, false, _T("JuceWASAPI"));

		ComSmartPtr <IAudioClient> tempClient (createClient());
		if (tempClient == nullptr)
			return;

		REFERENCE_TIME defaultPeriod, minPeriod;
		if (! check (tempClient->GetDevicePeriod (&defaultPeriod, &minPeriod)))
			return;

		WAVEFORMATEX* mixFormat = nullptr;
		if (! check (tempClient->GetMixFormat (&mixFormat)))
			return;

		WAVEFORMATEXTENSIBLE format;
		copyWavFormat (format, mixFormat);
		CoTaskMemFree (mixFormat);

		actualNumChannels = numChannels = format.Format.nChannels;
		defaultSampleRate = format.Format.nSamplesPerSec;
		minBufferSize = refTimeToSamples (minPeriod, defaultSampleRate);
		defaultBufferSize = refTimeToSamples (defaultPeriod, defaultSampleRate);

		rates.addUsingDefaultSort (defaultSampleRate);

		static const double ratesToTest[] = { 44100.0, 48000.0, 88200.0, 96000.0 };

		for (int i = 0; i < numElementsInArray (ratesToTest); ++i)
		{
			if (ratesToTest[i] == defaultSampleRate)
				continue;

			format.Format.nSamplesPerSec = (DWORD) roundDoubleToInt (ratesToTest[i]);

			if (SUCCEEDED (tempClient->IsFormatSupported (useExclusiveMode ? AUDCLNT_SHAREMODE_EXCLUSIVE : AUDCLNT_SHAREMODE_SHARED,
														  (WAVEFORMATEX*) &format, 0)))
				if (! rates.contains (ratesToTest[i]))
					rates.addUsingDefaultSort (ratesToTest[i]);
		}
	}

	~WASAPIDeviceBase()
	{
		device = nullptr;
		CloseHandle (clientEvent);
	}

	bool isOk() const noexcept    { return defaultBufferSize > 0 && defaultSampleRate > 0; }

	bool openClient (const double newSampleRate, const BigInteger& newChannels)
	{
		sampleRate = newSampleRate;
		channels = newChannels;
		channels.setRange (actualNumChannels, channels.getHighestBit() + 1 - actualNumChannels, false);
		numChannels = channels.getHighestBit() + 1;

		if (numChannels == 0)
			return true;

		client = createClient();

		if (client != nullptr
			 && (tryInitialisingWithFormat (true, 4) || tryInitialisingWithFormat (false, 4)
				  || tryInitialisingWithFormat (false, 3) || tryInitialisingWithFormat (false, 2)))
		{
			sampleRateHasChanged = false;

			channelMaps.clear();
			for (int i = 0; i <= channels.getHighestBit(); ++i)
				if (channels[i])
					channelMaps.add (i);

			REFERENCE_TIME latency;
			if (check (client->GetStreamLatency (&latency)))
				latencySamples = refTimeToSamples (latency, sampleRate);

			(void) check (client->GetBufferSize (&actualBufferSize));

			createSessionEventCallback();

			return check (client->SetEventHandle (clientEvent));
		}

		return false;
	}

	void closeClient()
	{
		if (client != nullptr)
			client->Stop();

		deleteSessionEventCallback();
		client = nullptr;
		ResetEvent (clientEvent);
	}

	void deviceSampleRateChanged()
	{
		sampleRateHasChanged = true;
	}

	ComSmartPtr <IMMDevice> device;
	ComSmartPtr <IAudioClient> client;
	double sampleRate, defaultSampleRate;
	int numChannels, actualNumChannels;
	int minBufferSize, defaultBufferSize, latencySamples;
	const bool useExclusiveMode;
	Array <double> rates;
	HANDLE clientEvent;
	BigInteger channels;
	Array <int> channelMaps;
	UINT32 actualBufferSize;
	int bytesPerSample;
	bool sampleRateHasChanged;

	virtual void updateFormat (bool isFloat) = 0;

private:

	class SessionEventCallback  : public ComBaseClassHelper <IAudioSessionEvents>
	{
	public:
		SessionEventCallback (WASAPIDeviceBase& owner_) : owner (owner_) {}

		JUCE_COMRESULT OnDisplayNameChanged (LPCWSTR, LPCGUID)                 { return S_OK; }
		JUCE_COMRESULT OnIconPathChanged (LPCWSTR, LPCGUID)                    { return S_OK; }
		JUCE_COMRESULT OnSimpleVolumeChanged (float, BOOL, LPCGUID)            { return S_OK; }
		JUCE_COMRESULT OnChannelVolumeChanged (DWORD, float*, DWORD, LPCGUID)  { return S_OK; }
		JUCE_COMRESULT OnGroupingParamChanged (LPCGUID, LPCGUID)               { return S_OK; }
		JUCE_COMRESULT OnStateChanged (AudioSessionState)                      { return S_OK; }

		JUCE_COMRESULT OnSessionDisconnected (AudioSessionDisconnectReason reason)
		{
			if (reason == DisconnectReasonFormatChanged)
				owner.deviceSampleRateChanged();

			return S_OK;
		}

	private:
		WASAPIDeviceBase& owner;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SessionEventCallback);
	};

	ComSmartPtr <IAudioSessionControl> audioSessionControl;
	ComSmartPtr <SessionEventCallback> sessionEventCallback;

	void createSessionEventCallback()
	{
		deleteSessionEventCallback();
		client->GetService (__uuidof (IAudioSessionControl),
							(void**) audioSessionControl.resetAndGetPointerAddress());

		if (audioSessionControl != nullptr)
		{
			sessionEventCallback = new SessionEventCallback (*this);
			audioSessionControl->RegisterAudioSessionNotification (sessionEventCallback);
			sessionEventCallback->Release(); // (required because ComBaseClassHelper objects are constructed with a ref count of 1)
		}
	}

	void deleteSessionEventCallback()
	{
		if (audioSessionControl != nullptr && sessionEventCallback != nullptr)
			audioSessionControl->UnregisterAudioSessionNotification (sessionEventCallback);

		audioSessionControl = nullptr;
		sessionEventCallback = nullptr;
	}

	const ComSmartPtr <IAudioClient> createClient()
	{
		ComSmartPtr <IAudioClient> client;

		if (device != nullptr)
		{
			HRESULT hr = device->Activate (__uuidof (IAudioClient), CLSCTX_INPROC_SERVER, 0, (void**) client.resetAndGetPointerAddress());
			logFailure (hr);
		}

		return client;
	}

	bool tryInitialisingWithFormat (const bool useFloat, const int bytesPerSampleToTry)
	{
		WAVEFORMATEXTENSIBLE format = { 0 };

		if (numChannels <= 2 && bytesPerSampleToTry <= 2)
		{
			format.Format.wFormatTag = WAVE_FORMAT_PCM;
		}
		else
		{
			format.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
			format.Format.cbSize = sizeof (WAVEFORMATEXTENSIBLE) - sizeof (WAVEFORMATEX);
		}

		format.Format.nSamplesPerSec = (DWORD) roundDoubleToInt (sampleRate);
		format.Format.nChannels = (WORD) numChannels;
		format.Format.wBitsPerSample = (WORD) (8 * bytesPerSampleToTry);
		format.Format.nAvgBytesPerSec = (DWORD) (format.Format.nSamplesPerSec * numChannels * bytesPerSampleToTry);
		format.Format.nBlockAlign = (WORD) (numChannels * bytesPerSampleToTry);
		format.SubFormat = useFloat ? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT : KSDATAFORMAT_SUBTYPE_PCM;
		format.Samples.wValidBitsPerSample = format.Format.wBitsPerSample;

		switch (numChannels)
		{
			case 1:     format.dwChannelMask = SPEAKER_FRONT_CENTER; break;
			case 2:     format.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT; break;
			case 4:     format.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT; break;
			case 6:     format.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT; break;
			case 8:     format.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_FRONT_LEFT_OF_CENTER | SPEAKER_FRONT_RIGHT_OF_CENTER; break;
			default:    break;
		}

		WAVEFORMATEXTENSIBLE* nearestFormat = nullptr;

		HRESULT hr = client->IsFormatSupported (useExclusiveMode ? AUDCLNT_SHAREMODE_EXCLUSIVE : AUDCLNT_SHAREMODE_SHARED,
												(WAVEFORMATEX*) &format, useExclusiveMode ? nullptr : (WAVEFORMATEX**) &nearestFormat);
		logFailure (hr);

		if (hr == S_FALSE && format.Format.nSamplesPerSec == nearestFormat->Format.nSamplesPerSec)
		{
			copyWavFormat (format, (WAVEFORMATEX*) nearestFormat);
			hr = S_OK;
		}

		CoTaskMemFree (nearestFormat);

		REFERENCE_TIME defaultPeriod = 0, minPeriod = 0;
		if (useExclusiveMode)
			check (client->GetDevicePeriod (&defaultPeriod, &minPeriod));

		GUID session;
		if (hr == S_OK
			 && check (client->Initialize (useExclusiveMode ? AUDCLNT_SHAREMODE_EXCLUSIVE : AUDCLNT_SHAREMODE_SHARED,
										   AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
										   defaultPeriod, defaultPeriod, (WAVEFORMATEX*) &format, &session)))
		{
			actualNumChannels = format.Format.nChannels;
			const bool isFloat = format.Format.wFormatTag == WAVE_FORMAT_EXTENSIBLE && format.SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
			bytesPerSample = format.Format.wBitsPerSample / 8;

			updateFormat (isFloat);
			return true;
		}

		return false;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WASAPIDeviceBase);
};

class WASAPIInputDevice  : public WASAPIDeviceBase
{
public:
	WASAPIInputDevice (const ComSmartPtr <IMMDevice>& device_, const bool useExclusiveMode_)
		: WASAPIDeviceBase (device_, useExclusiveMode_),
		  reservoir (1, 1)
	{
	}

	~WASAPIInputDevice()
	{
		close();
	}

	bool open (const double newSampleRate, const BigInteger& newChannels)
	{
		reservoirSize = 0;
		reservoirCapacity = 16384;
		reservoir.setSize (actualNumChannels * reservoirCapacity * sizeof (float));
		return openClient (newSampleRate, newChannels)
				&& (numChannels == 0 || check (client->GetService (__uuidof (IAudioCaptureClient),
																   (void**) captureClient.resetAndGetPointerAddress())));
	}

	void close()
	{
		closeClient();
		captureClient = nullptr;
		reservoir.setSize (0);
	}

	template <class SourceType>
	void updateFormatWithType (SourceType*)
	{
		typedef AudioData::Pointer <AudioData::Float32, AudioData::NativeEndian, AudioData::NonInterleaved, AudioData::NonConst> NativeType;
		converter = new AudioData::ConverterInstance <AudioData::Pointer <SourceType, AudioData::LittleEndian, AudioData::Interleaved, AudioData::Const>, NativeType> (actualNumChannels, 1);
	}

	void updateFormat (bool isFloat)
	{
		if (isFloat)                    updateFormatWithType ((AudioData::Float32*) 0);
		else if (bytesPerSample == 4)   updateFormatWithType ((AudioData::Int32*) 0);
		else if (bytesPerSample == 3)   updateFormatWithType ((AudioData::Int24*) 0);
		else                            updateFormatWithType ((AudioData::Int16*) 0);
	}

	void copyBuffers (float** destBuffers, int numDestBuffers, int bufferSize, Thread& thread)
	{
		if (numChannels <= 0)
			return;

		int offset = 0;

		while (bufferSize > 0)
		{
			if (reservoirSize > 0)  // There's stuff in the reservoir, so use that...
			{
				const int samplesToDo = jmin (bufferSize, (int) reservoirSize);

				for (int i = 0; i < numDestBuffers; ++i)
					converter->convertSamples (destBuffers[i] + offset, 0, reservoir.getData(), channelMaps.getUnchecked(i), samplesToDo);

				bufferSize -= samplesToDo;
				offset += samplesToDo;
				reservoirSize = 0;
			}
			else
			{
				UINT32 packetLength = 0;
				if (! check (captureClient->GetNextPacketSize (&packetLength)))
					break;

				if (packetLength == 0)
				{
					if (thread.threadShouldExit()
						 || WaitForSingleObject (clientEvent, 1000) == WAIT_TIMEOUT)
						break;

					continue;
				}

				uint8* inputData;
				UINT32 numSamplesAvailable;
				DWORD flags;

				if (check (captureClient->GetBuffer (&inputData, &numSamplesAvailable, &flags, 0, 0)))
				{
					const int samplesToDo = jmin (bufferSize, (int) numSamplesAvailable);

					for (int i = 0; i < numDestBuffers; ++i)
						converter->convertSamples (destBuffers[i] + offset, 0, inputData, channelMaps.getUnchecked(i), samplesToDo);

					bufferSize -= samplesToDo;
					offset += samplesToDo;

					if (samplesToDo < (int) numSamplesAvailable)
					{
						reservoirSize = jmin ((int) (numSamplesAvailable - samplesToDo), reservoirCapacity);
						memcpy ((uint8*) reservoir.getData(), inputData + bytesPerSample * actualNumChannels * samplesToDo,
								(size_t) (bytesPerSample * actualNumChannels * reservoirSize));
					}

					captureClient->ReleaseBuffer (numSamplesAvailable);
				}
			}
		}
	}

	ComSmartPtr <IAudioCaptureClient> captureClient;
	MemoryBlock reservoir;
	int reservoirSize, reservoirCapacity;
	ScopedPointer <AudioData::Converter> converter;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WASAPIInputDevice);
};

class WASAPIOutputDevice  : public WASAPIDeviceBase
{
public:
	WASAPIOutputDevice (const ComSmartPtr <IMMDevice>& device_, const bool useExclusiveMode_)
		: WASAPIDeviceBase (device_, useExclusiveMode_)
	{
	}

	~WASAPIOutputDevice()
	{
		close();
	}

	bool open (const double newSampleRate, const BigInteger& newChannels)
	{
		return openClient (newSampleRate, newChannels)
			&& (numChannels == 0 || check (client->GetService (__uuidof (IAudioRenderClient), (void**) renderClient.resetAndGetPointerAddress())));
	}

	void close()
	{
		closeClient();
		renderClient = nullptr;
	}

	template <class DestType>
	void updateFormatWithType (DestType*)
	{
		typedef AudioData::Pointer <AudioData::Float32, AudioData::NativeEndian, AudioData::NonInterleaved, AudioData::Const> NativeType;
		converter = new AudioData::ConverterInstance <NativeType, AudioData::Pointer <DestType, AudioData::LittleEndian, AudioData::Interleaved, AudioData::NonConst> > (1, actualNumChannels);
	}

	void updateFormat (bool isFloat)
	{
		if (isFloat)                    updateFormatWithType ((AudioData::Float32*) 0);
		else if (bytesPerSample == 4)   updateFormatWithType ((AudioData::Int32*) 0);
		else if (bytesPerSample == 3)   updateFormatWithType ((AudioData::Int24*) 0);
		else                            updateFormatWithType ((AudioData::Int16*) 0);
	}

	void copyBuffers (const float** const srcBuffers, const int numSrcBuffers, int bufferSize, Thread& thread)
	{
		if (numChannels <= 0)
			return;

		int offset = 0;

		while (bufferSize > 0)
		{
			UINT32 padding = 0;
			if (! check (client->GetCurrentPadding (&padding)))
				return;

			int samplesToDo = useExclusiveMode ? bufferSize
											   : jmin ((int) (actualBufferSize - padding), bufferSize);

			if (samplesToDo <= 0)
			{
				if (thread.threadShouldExit()
					 || WaitForSingleObject (clientEvent, 1000) == WAIT_TIMEOUT)
					break;

				continue;
			}

			uint8* outputData = nullptr;
			if (check (renderClient->GetBuffer ((UINT32) samplesToDo, &outputData)))
			{
				for (int i = 0; i < numSrcBuffers; ++i)
					converter->convertSamples (outputData, channelMaps.getUnchecked(i), srcBuffers[i] + offset, 0, samplesToDo);

				renderClient->ReleaseBuffer ((UINT32) samplesToDo, 0);

				offset += samplesToDo;
				bufferSize -= samplesToDo;
			}
		}
	}

	ComSmartPtr <IAudioRenderClient> renderClient;
	ScopedPointer <AudioData::Converter> converter;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WASAPIOutputDevice);
};

class WASAPIAudioIODevice  : public AudioIODevice,
							 public Thread
{
public:
	WASAPIAudioIODevice (const String& deviceName,
						 const String& outputDeviceId_,
						 const String& inputDeviceId_,
						 const bool useExclusiveMode_)
		: AudioIODevice (deviceName, "Windows Audio"),
		  Thread ("Juce WASAPI"),
		  outputDeviceId (outputDeviceId_),
		  inputDeviceId (inputDeviceId_),
		  useExclusiveMode (useExclusiveMode_),
		  isOpen_ (false),
		  isStarted (false),
		  currentBufferSizeSamples (0),
		  currentSampleRate (0),
		  callback (nullptr)
	{
	}

	~WASAPIAudioIODevice()
	{
		close();
	}

	bool initialise()
	{
		latencyIn = latencyOut = 0;
		Array <double> ratesIn, ratesOut;

		if (createDevices())
		{
			jassert (inputDevice != nullptr || outputDevice != nullptr);

			if (inputDevice != nullptr && outputDevice != nullptr)
			{
				defaultSampleRate = jmin (inputDevice->defaultSampleRate, outputDevice->defaultSampleRate);
				minBufferSize = jmin (inputDevice->minBufferSize, outputDevice->minBufferSize);
				defaultBufferSize = jmax (inputDevice->defaultBufferSize, outputDevice->defaultBufferSize);
				sampleRates = inputDevice->rates;
				sampleRates.removeValuesNotIn (outputDevice->rates);
			}
			else
			{
				WASAPIDeviceBase* d = inputDevice != nullptr ? static_cast<WASAPIDeviceBase*> (inputDevice)
															 : static_cast<WASAPIDeviceBase*> (outputDevice);
				defaultSampleRate = d->defaultSampleRate;
				minBufferSize = d->minBufferSize;
				defaultBufferSize = d->defaultBufferSize;
				sampleRates = d->rates;
			}

			bufferSizes.addUsingDefaultSort (defaultBufferSize);
			if (minBufferSize != defaultBufferSize)
				bufferSizes.addUsingDefaultSort (minBufferSize);

			int n = 64;
			for (int i = 0; i < 40; ++i)
			{
				if (n >= minBufferSize && n <= 2048 && ! bufferSizes.contains (n))
					bufferSizes.addUsingDefaultSort (n);

				n += (n < 512) ? 32 : (n < 1024 ? 64 : 128);
			}

			return true;
		}

		return false;
	}

	StringArray getOutputChannelNames()
	{
		StringArray outChannels;

		if (outputDevice != nullptr)
			for (int i = 1; i <= outputDevice->actualNumChannels; ++i)
				outChannels.add ("Output channel " + String (i));

		return outChannels;
	}

	StringArray getInputChannelNames()
	{
		StringArray inChannels;

		if (inputDevice != nullptr)
			for (int i = 1; i <= inputDevice->actualNumChannels; ++i)
				inChannels.add ("Input channel " + String (i));

		return inChannels;
	}

	int getNumSampleRates()                             { return sampleRates.size(); }
	double getSampleRate (int index)                    { return sampleRates [index]; }
	int getNumBufferSizesAvailable()                    { return bufferSizes.size(); }
	int getBufferSizeSamples (int index)                { return bufferSizes [index]; }
	int getDefaultBufferSize()                          { return defaultBufferSize; }

	int getCurrentBufferSizeSamples()                   { return currentBufferSizeSamples; }
	double getCurrentSampleRate()                       { return currentSampleRate; }
	int getCurrentBitDepth()                            { return 32; }
	int getOutputLatencyInSamples()                     { return latencyOut; }
	int getInputLatencyInSamples()                      { return latencyIn; }
	BigInteger getActiveOutputChannels() const          { return outputDevice != nullptr ? outputDevice->channels : BigInteger(); }
	BigInteger getActiveInputChannels() const           { return inputDevice != nullptr ? inputDevice->channels : BigInteger(); }
	String getLastError()                               { return lastError; }

	String open (const BigInteger& inputChannels, const BigInteger& outputChannels,
				 double sampleRate, int bufferSizeSamples)
	{
		close();
		lastError = String::empty;

		if (sampleRates.size() == 0 && inputDevice != nullptr && outputDevice != nullptr)
		{
			lastError = "The input and output devices don't share a common sample rate!";
			return lastError;
		}

		currentBufferSizeSamples = bufferSizeSamples <= 0 ? defaultBufferSize : jmax (bufferSizeSamples, minBufferSize);
		currentSampleRate = sampleRate > 0 ? sampleRate : defaultSampleRate;

		if (inputDevice != nullptr && ! inputDevice->open (currentSampleRate, inputChannels))
		{
			lastError = "Couldn't open the input device!";
			return lastError;
		}

		if (outputDevice != nullptr && ! outputDevice->open (currentSampleRate, outputChannels))
		{
			close();
			lastError = "Couldn't open the output device!";
			return lastError;
		}

		if (inputDevice != nullptr)   ResetEvent (inputDevice->clientEvent);
		if (outputDevice != nullptr)  ResetEvent (outputDevice->clientEvent);

		startThread (8);
		Thread::sleep (5);

		if (inputDevice != nullptr && inputDevice->client != nullptr)
		{
			latencyIn = (int) (inputDevice->latencySamples + inputDevice->actualBufferSize + inputDevice->minBufferSize);

			if (! check (inputDevice->client->Start()))
			{
				close();
				lastError = "Couldn't start the input device!";
				return lastError;
			}
		}

		if (outputDevice != nullptr && outputDevice->client != nullptr)
		{
			latencyOut = (int) (outputDevice->latencySamples + outputDevice->actualBufferSize + outputDevice->minBufferSize);

			if (! check (outputDevice->client->Start()))
			{
				close();
				lastError = "Couldn't start the output device!";
				return lastError;
			}
		}

		isOpen_ = true;
		return lastError;
	}

	void close()
	{
		stop();
		signalThreadShouldExit();

		if (inputDevice != nullptr)   SetEvent (inputDevice->clientEvent);
		if (outputDevice != nullptr)  SetEvent (outputDevice->clientEvent);

		stopThread (5000);

		if (inputDevice != nullptr)   inputDevice->close();
		if (outputDevice != nullptr)  outputDevice->close();

		isOpen_ = false;
	}

	bool isOpen()       { return isOpen_ && isThreadRunning(); }
	bool isPlaying()    { return isStarted && isOpen_ && isThreadRunning(); }

	void start (AudioIODeviceCallback* call)
	{
		if (isOpen_ && call != nullptr && ! isStarted)
		{
			if (! isThreadRunning())
			{
				// something's gone wrong and the thread's stopped..
				isOpen_ = false;
				return;
			}

			call->audioDeviceAboutToStart (this);

			const ScopedLock sl (startStopLock);
			callback = call;
			isStarted = true;
		}
	}

	void stop()
	{
		if (isStarted)
		{
			AudioIODeviceCallback* const callbackLocal = callback;

			{
				const ScopedLock sl (startStopLock);
				isStarted = false;
			}

			if (callbackLocal != nullptr)
				callbackLocal->audioDeviceStopped();
		}
	}

	void setMMThreadPriority()
	{
		DynamicLibrary dll ("avrt.dll");
		JUCE_DLL_FUNCTION (AvSetMmThreadCharacteristicsW, avSetMmThreadCharacteristics, HANDLE, dll, (LPCWSTR, LPDWORD))
		JUCE_DLL_FUNCTION (AvSetMmThreadPriority, avSetMmThreadPriority, HANDLE, dll, (HANDLE, AVRT_PRIORITY))

		if (avSetMmThreadCharacteristics != 0 && avSetMmThreadPriority != 0)
		{
			DWORD dummy = 0;
			HANDLE h = avSetMmThreadCharacteristics (L"Pro Audio", &dummy);

			if (h != 0)
				avSetMmThreadPriority (h, AVRT_PRIORITY_NORMAL);
		}
	}

	void run()
	{
		setMMThreadPriority();

		const int bufferSize = currentBufferSizeSamples;
		const int numInputBuffers = getActiveInputChannels().countNumberOfSetBits();
		const int numOutputBuffers = getActiveOutputChannels().countNumberOfSetBits();
		bool sampleRateChanged = false;

		AudioSampleBuffer ins (jmax (1, numInputBuffers), bufferSize + 32);
		AudioSampleBuffer outs (jmax (1, numOutputBuffers), bufferSize + 32);
		float** const inputBuffers = ins.getArrayOfChannels();
		float** const outputBuffers = outs.getArrayOfChannels();
		ins.clear();

		while (! threadShouldExit())
		{
			if (inputDevice != nullptr)
			{
				inputDevice->copyBuffers (inputBuffers, numInputBuffers, bufferSize, *this);

				if (threadShouldExit())
					break;

				if (inputDevice->sampleRateHasChanged)
					sampleRateChanged = true;
			}

			JUCE_TRY
			{
				const ScopedLock sl (startStopLock);

				if (isStarted)
					callback->audioDeviceIOCallback (const_cast <const float**> (inputBuffers), numInputBuffers,
													 outputBuffers, numOutputBuffers, bufferSize);
				else
					outs.clear();
			}
			JUCE_CATCH_EXCEPTION

			if (outputDevice != nullptr)
			{
				outputDevice->copyBuffers (const_cast <const float**> (outputBuffers), numOutputBuffers, bufferSize, *this);

				if (outputDevice->sampleRateHasChanged)
					sampleRateChanged = true;
			}

			if (sampleRateChanged)
			{
				// xxx one of the devices has had its sample rate changed externally.. not 100% sure how
				// to handle this..
			}
		}
	}

	String outputDeviceId, inputDeviceId;
	String lastError;

private:
	// Device stats...
	ScopedPointer<WASAPIInputDevice> inputDevice;
	ScopedPointer<WASAPIOutputDevice> outputDevice;
	const bool useExclusiveMode;
	double defaultSampleRate;
	int minBufferSize, defaultBufferSize;
	int latencyIn, latencyOut;
	Array <double> sampleRates;
	Array <int> bufferSizes;

	// Active state...
	bool isOpen_, isStarted;
	int currentBufferSizeSamples;
	double currentSampleRate;

	AudioIODeviceCallback* callback;
	CriticalSection startStopLock;

	bool createDevices()
	{
		ComSmartPtr <IMMDeviceEnumerator> enumerator;
		if (! check (enumerator.CoCreateInstance (__uuidof (MMDeviceEnumerator))))
			return false;

		ComSmartPtr <IMMDeviceCollection> deviceCollection;
		if (! check (enumerator->EnumAudioEndpoints (eAll, DEVICE_STATE_ACTIVE, deviceCollection.resetAndGetPointerAddress())))
			return false;

		UINT32 numDevices = 0;
		if (! check (deviceCollection->GetCount (&numDevices)))
			return false;

		for (UINT32 i = 0; i < numDevices; ++i)
		{
			ComSmartPtr <IMMDevice> device;
			if (! check (deviceCollection->Item (i, device.resetAndGetPointerAddress())))
				continue;

			const String deviceId (getDeviceID (device));
			if (deviceId.isEmpty())
				continue;

			const EDataFlow flow = getDataFlow (device);

			if (deviceId == inputDeviceId && flow == eCapture)
				inputDevice = new WASAPIInputDevice (device, useExclusiveMode);
			else if (deviceId == outputDeviceId && flow == eRender)
				outputDevice = new WASAPIOutputDevice (device, useExclusiveMode);
		}

		return (outputDeviceId.isEmpty() || (outputDevice != nullptr && outputDevice->isOk()))
			&& (inputDeviceId.isEmpty() || (inputDevice != nullptr && inputDevice->isOk()));
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WASAPIAudioIODevice);
};

class WASAPIAudioIODeviceType  : public AudioIODeviceType,
								 private DeviceChangeDetector
{
public:
	WASAPIAudioIODeviceType()
		: AudioIODeviceType ("Windows Audio"),
		  DeviceChangeDetector (L"Windows Audio"),
		  hasScanned (false)
	{
	}

	void scanForDevices()
	{
		hasScanned = true;

		outputDeviceNames.clear();
		inputDeviceNames.clear();
		outputDeviceIds.clear();
		inputDeviceIds.clear();

		scan (outputDeviceNames, inputDeviceNames,
			  outputDeviceIds, inputDeviceIds);
	}

	StringArray getDeviceNames (bool wantInputNames) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		return wantInputNames ? inputDeviceNames
							  : outputDeviceNames;
	}

	int getDefaultDeviceIndex (bool /*forInput*/) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this
		return 0;
	}

	int getIndexOfDevice (AudioIODevice* device, bool asInput) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this
		WASAPIAudioIODevice* const d = dynamic_cast <WASAPIAudioIODevice*> (device);
		return d == nullptr ? -1 : (asInput ? inputDeviceIds.indexOf (d->inputDeviceId)
											: outputDeviceIds.indexOf (d->outputDeviceId));
	}

	bool hasSeparateInputsAndOutputs() const    { return true; }

	AudioIODevice* createDevice (const String& outputDeviceName,
								 const String& inputDeviceName)
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		const bool useExclusiveMode = false;
		ScopedPointer<WASAPIAudioIODevice> device;

		const int outputIndex = outputDeviceNames.indexOf (outputDeviceName);
		const int inputIndex = inputDeviceNames.indexOf (inputDeviceName);

		if (outputIndex >= 0 || inputIndex >= 0)
		{
			device = new WASAPIAudioIODevice (outputDeviceName.isNotEmpty() ? outputDeviceName
																			: inputDeviceName,
											  outputDeviceIds [outputIndex],
											  inputDeviceIds [inputIndex],
											  useExclusiveMode);

			if (! device->initialise())
				device = nullptr;
		}

		return device.release();
	}

	StringArray outputDeviceNames, outputDeviceIds;
	StringArray inputDeviceNames, inputDeviceIds;

private:
	bool hasScanned;

	static String getDefaultEndpoint (IMMDeviceEnumerator* const enumerator, const bool forCapture)
	{
		String s;
		IMMDevice* dev = nullptr;
		if (check (enumerator->GetDefaultAudioEndpoint (forCapture ? eCapture : eRender,
														eMultimedia, &dev)))
		{
			WCHAR* deviceId = nullptr;
			if (check (dev->GetId (&deviceId)))
			{
				s = deviceId;
				CoTaskMemFree (deviceId);
			}

			dev->Release();
		}

		return s;
	}

	void scan (StringArray& outputDeviceNames,
			   StringArray& inputDeviceNames,
			   StringArray& outputDeviceIds,
			   StringArray& inputDeviceIds)
	{
		ComSmartPtr <IMMDeviceEnumerator> enumerator;
		if (! check (enumerator.CoCreateInstance (__uuidof (MMDeviceEnumerator))))
			return;

		const String defaultRenderer (getDefaultEndpoint (enumerator, false));
		const String defaultCapture (getDefaultEndpoint (enumerator, true));

		ComSmartPtr <IMMDeviceCollection> deviceCollection;
		UINT32 numDevices = 0;

		if (! (check (enumerator->EnumAudioEndpoints (eAll, DEVICE_STATE_ACTIVE, deviceCollection.resetAndGetPointerAddress()))
				&& check (deviceCollection->GetCount (&numDevices))))
			return;

		for (UINT32 i = 0; i < numDevices; ++i)
		{
			ComSmartPtr <IMMDevice> device;
			if (! check (deviceCollection->Item (i, device.resetAndGetPointerAddress())))
				continue;

			DWORD state = 0;
			if (! (check (device->GetState (&state)) && state == DEVICE_STATE_ACTIVE))
				continue;

			const String deviceId (getDeviceID (device));
			String name;

			{
				ComSmartPtr <IPropertyStore> properties;
				if (! check (device->OpenPropertyStore (STGM_READ, properties.resetAndGetPointerAddress())))
					continue;

				PROPVARIANT value;
				PropVariantInit (&value);
				if (check (properties->GetValue (PKEY_Device_FriendlyName, &value)))
					name = value.pwszVal;

				PropVariantClear (&value);
			}

			const EDataFlow flow = getDataFlow (device);

			if (flow == eRender)
			{
				const int index = (deviceId == defaultRenderer) ? 0 : -1;
				outputDeviceIds.insert (index, deviceId);
				outputDeviceNames.insert (index, name);
			}
			else if (flow == eCapture)
			{
				const int index = (deviceId == defaultCapture) ? 0 : -1;
				inputDeviceIds.insert (index, deviceId);
				inputDeviceNames.insert (index, name);
			}
		}

		inputDeviceNames.appendNumbersToDuplicates (false, false);
		outputDeviceNames.appendNumbersToDuplicates (false, false);
	}

	void systemDeviceChanged()
	{
		StringArray newOutNames, newInNames, newOutIds, newInIds;
		scan (newOutNames, newInNames, newOutIds, newInIds);

		if (newOutNames != outputDeviceNames
			 || newInNames != inputDeviceNames
			 || newOutIds != outputDeviceIds
			 || newInIds != inputDeviceIds)
		{
			hasScanned = true;
			outputDeviceNames = newOutNames;
			inputDeviceNames = newInNames;
			outputDeviceIds = newOutIds;
			inputDeviceIds = newInIds;

			callDeviceChangeListeners();
		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WASAPIAudioIODeviceType);
};

}

AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_WASAPI()
{
	if (SystemStats::getOperatingSystemType() >= SystemStats::WinVista)
		return new WasapiClasses::WASAPIAudioIODeviceType();

	return nullptr;
}

/*** End of inlined file: juce_win32_WASAPI.cpp ***/


 #endif

 #if JUCE_DIRECTSOUND

/*** Start of inlined file: juce_win32_DirectSound.cpp ***/
} // (juce namespace)

extern "C"
{
	// Declare just the minimum number of interfaces for the DSound objects that we need..
	typedef struct typeDSBUFFERDESC
	{
		DWORD dwSize;
		DWORD dwFlags;
		DWORD dwBufferBytes;
		DWORD dwReserved;
		LPWAVEFORMATEX lpwfxFormat;
		GUID guid3DAlgorithm;
	} DSBUFFERDESC;

	struct IDirectSoundBuffer;

	#undef INTERFACE
	#define INTERFACE IDirectSound
	DECLARE_INTERFACE_(IDirectSound, IUnknown)
	{
		STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID*) PURE;
		STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
		STDMETHOD_(ULONG,Release)       (THIS) PURE;
		STDMETHOD(CreateSoundBuffer)    (THIS_ DSBUFFERDESC*, IDirectSoundBuffer**, LPUNKNOWN) PURE;
		STDMETHOD(GetCaps)              (THIS_ void*) PURE;
		STDMETHOD(DuplicateSoundBuffer) (THIS_ IDirectSoundBuffer*, IDirectSoundBuffer**) PURE;
		STDMETHOD(SetCooperativeLevel)  (THIS_ HWND, DWORD) PURE;
		STDMETHOD(Compact)              (THIS) PURE;
		STDMETHOD(GetSpeakerConfig)     (THIS_ LPDWORD) PURE;
		STDMETHOD(SetSpeakerConfig)     (THIS_ DWORD) PURE;
		STDMETHOD(Initialize)           (THIS_ const GUID*) PURE;
	};

	#undef INTERFACE
	#define INTERFACE IDirectSoundBuffer
	DECLARE_INTERFACE_(IDirectSoundBuffer, IUnknown)
	{
		STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID*) PURE;
		STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
		STDMETHOD_(ULONG,Release)       (THIS) PURE;
		STDMETHOD(GetCaps)              (THIS_ void*) PURE;
		STDMETHOD(GetCurrentPosition)   (THIS_ LPDWORD, LPDWORD) PURE;
		STDMETHOD(GetFormat)            (THIS_ LPWAVEFORMATEX, DWORD, LPDWORD) PURE;
		STDMETHOD(GetVolume)            (THIS_ LPLONG) PURE;
		STDMETHOD(GetPan)               (THIS_ LPLONG) PURE;
		STDMETHOD(GetFrequency)         (THIS_ LPDWORD) PURE;
		STDMETHOD(GetStatus)            (THIS_ LPDWORD) PURE;
		STDMETHOD(Initialize)           (THIS_ IDirectSound*, DSBUFFERDESC*) PURE;
		STDMETHOD(Lock)                 (THIS_ DWORD, DWORD, LPVOID*, LPDWORD, LPVOID*, LPDWORD, DWORD) PURE;
		STDMETHOD(Play)                 (THIS_ DWORD, DWORD, DWORD) PURE;
		STDMETHOD(SetCurrentPosition)   (THIS_ DWORD) PURE;
		STDMETHOD(SetFormat)            (THIS_ const WAVEFORMATEX*) PURE;
		STDMETHOD(SetVolume)            (THIS_ LONG) PURE;
		STDMETHOD(SetPan)               (THIS_ LONG) PURE;
		STDMETHOD(SetFrequency)         (THIS_ DWORD) PURE;
		STDMETHOD(Stop)                 (THIS) PURE;
		STDMETHOD(Unlock)               (THIS_ LPVOID, DWORD, LPVOID, DWORD) PURE;
		STDMETHOD(Restore)              (THIS) PURE;
	};

	typedef struct typeDSCBUFFERDESC
	{
		DWORD dwSize;
		DWORD dwFlags;
		DWORD dwBufferBytes;
		DWORD dwReserved;
		LPWAVEFORMATEX lpwfxFormat;
	} DSCBUFFERDESC;

	struct IDirectSoundCaptureBuffer;

	#undef INTERFACE
	#define INTERFACE IDirectSoundCapture
	DECLARE_INTERFACE_(IDirectSoundCapture, IUnknown)
	{
		STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID*) PURE;
		STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
		STDMETHOD_(ULONG,Release)       (THIS) PURE;
		STDMETHOD(CreateCaptureBuffer)  (THIS_ DSCBUFFERDESC*, IDirectSoundCaptureBuffer**, LPUNKNOWN) PURE;
		STDMETHOD(GetCaps)              (THIS_ void*) PURE;
		STDMETHOD(Initialize)           (THIS_ const GUID*) PURE;
	};

	#undef INTERFACE
	#define INTERFACE IDirectSoundCaptureBuffer
	DECLARE_INTERFACE_(IDirectSoundCaptureBuffer, IUnknown)
	{
		STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID*) PURE;
		STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
		STDMETHOD_(ULONG,Release)       (THIS) PURE;
		STDMETHOD(GetCaps)              (THIS_ void*) PURE;
		STDMETHOD(GetCurrentPosition)   (THIS_ LPDWORD, LPDWORD) PURE;
		STDMETHOD(GetFormat)            (THIS_ LPWAVEFORMATEX, DWORD, LPDWORD) PURE;
		STDMETHOD(GetStatus)            (THIS_ LPDWORD) PURE;
		STDMETHOD(Initialize)           (THIS_ IDirectSoundCapture*, DSCBUFFERDESC*) PURE;
		STDMETHOD(Lock)                 (THIS_ DWORD, DWORD, LPVOID*, LPDWORD, LPVOID*, LPDWORD, DWORD) PURE;
		STDMETHOD(Start)                (THIS_ DWORD) PURE;
		STDMETHOD(Stop)                 (THIS) PURE;
		STDMETHOD(Unlock)               (THIS_ LPVOID, DWORD, LPVOID, DWORD) PURE;
	};
}

namespace juce
{

namespace
{
	String getDSErrorMessage (HRESULT hr)
	{
		const char* result = nullptr;

		switch (hr)
		{
			case MAKE_HRESULT(1, 0x878, 10):    result = "Device already allocated"; break;
			case MAKE_HRESULT(1, 0x878, 30):    result = "Control unavailable"; break;
			case E_INVALIDARG:                  result = "Invalid parameter"; break;
			case MAKE_HRESULT(1, 0x878, 50):    result = "Invalid call"; break;
			case E_FAIL:                        result = "Generic error"; break;
			case MAKE_HRESULT(1, 0x878, 70):    result = "Priority level error"; break;
			case E_OUTOFMEMORY:                 result = "Out of memory"; break;
			case MAKE_HRESULT(1, 0x878, 100):   result = "Bad format"; break;
			case E_NOTIMPL:                     result = "Unsupported function"; break;
			case MAKE_HRESULT(1, 0x878, 120):   result = "No driver"; break;
			case MAKE_HRESULT(1, 0x878, 130):   result = "Already initialised"; break;
			case CLASS_E_NOAGGREGATION:         result = "No aggregation"; break;
			case MAKE_HRESULT(1, 0x878, 150):   result = "Buffer lost"; break;
			case MAKE_HRESULT(1, 0x878, 160):   result = "Another app has priority"; break;
			case MAKE_HRESULT(1, 0x878, 170):   result = "Uninitialised"; break;
			case E_NOINTERFACE:                 result = "No interface"; break;
			case S_OK:                          result = "No error"; break;
			default:                            return "Unknown error: " + String ((int) hr);
		}

		return result;
	}

	#define DS_DEBUGGING 1

	#ifdef DS_DEBUGGING
		#define CATCH JUCE_CATCH_EXCEPTION
		#undef log
		#define log(a) Logger::writeToLog(a);
		#undef logError
		#define logError(a) logDSError(a, __LINE__);

		static void logDSError (HRESULT hr, int lineNum)
		{
			if (hr != S_OK)
			{
				String error ("DS error at line ");
				error << lineNum << " - " << getDSErrorMessage (hr);
				log (error);
			}
		}
	#else
		#define CATCH JUCE_CATCH_ALL
		#define log(a)
		#define logError(a)
	#endif

	#define DSOUND_FUNCTION(functionName, params) \
		typedef HRESULT (WINAPI *type##functionName) params; \
		static type##functionName ds##functionName = 0;

	#define DSOUND_FUNCTION_LOAD(functionName) \
		ds##functionName = (type##functionName) GetProcAddress (h, #functionName);  \
		jassert (ds##functionName != 0);

	typedef BOOL (CALLBACK *LPDSENUMCALLBACKW) (LPGUID, LPCWSTR, LPCWSTR, LPVOID);
	typedef BOOL (CALLBACK *LPDSENUMCALLBACKA) (LPGUID, LPCSTR, LPCSTR, LPVOID);

	DSOUND_FUNCTION (DirectSoundCreate, (const GUID*, IDirectSound**, LPUNKNOWN))
	DSOUND_FUNCTION (DirectSoundCaptureCreate, (const GUID*, IDirectSoundCapture**, LPUNKNOWN))
	DSOUND_FUNCTION (DirectSoundEnumerateW, (LPDSENUMCALLBACKW, LPVOID))
	DSOUND_FUNCTION (DirectSoundCaptureEnumerateW, (LPDSENUMCALLBACKW, LPVOID))

	void initialiseDSoundFunctions()
	{
		if (dsDirectSoundCreate == 0)
		{
			HMODULE h = LoadLibraryA ("dsound.dll");

			DSOUND_FUNCTION_LOAD (DirectSoundCreate)
			DSOUND_FUNCTION_LOAD (DirectSoundCaptureCreate)
			DSOUND_FUNCTION_LOAD (DirectSoundEnumerateW)
			DSOUND_FUNCTION_LOAD (DirectSoundCaptureEnumerateW)
		}
	}
}

class DSoundInternalOutChannel
{
public:
	DSoundInternalOutChannel (const String& name_, const GUID& guid_, int rate,
							  int bufferSize, float* left, float* right)
		: bitDepth (16), name (name_), guid (guid_), sampleRate (rate),
		  bufferSizeSamples (bufferSize), leftBuffer (left), rightBuffer (right),
		  pDirectSound (nullptr), pOutputBuffer (nullptr)
	{
	}

	~DSoundInternalOutChannel()
	{
		close();
	}

	void close()
	{
		HRESULT hr;

		if (pOutputBuffer != nullptr)
		{
			log ("closing dsound out: " + name);
			hr = pOutputBuffer->Stop();
			logError (hr);

			pOutputBuffer->Release();
			pOutputBuffer = nullptr;
		}

		if (pDirectSound != nullptr)
		{
			pDirectSound->Release();
			pDirectSound = nullptr;
		}
	}

	String open()
	{
		log ("opening dsound out device: " + name + "  rate=" + String (sampleRate)
			  + " bits=" + String (bitDepth) + " buf=" + String (bufferSizeSamples));

		pDirectSound = nullptr;
		pOutputBuffer = nullptr;
		writeOffset = 0;

		String error;
		HRESULT hr = E_NOINTERFACE;

		if (dsDirectSoundCreate != 0)
			hr = dsDirectSoundCreate (&guid, &pDirectSound, 0);

		if (hr == S_OK)
		{
			bytesPerBuffer = (bufferSizeSamples * (bitDepth >> 2)) & ~15;
			totalBytesPerBuffer = (3 * bytesPerBuffer) & ~15;
			const int numChannels = 2;

			hr = pDirectSound->SetCooperativeLevel (GetDesktopWindow(), 2 /* DSSCL_PRIORITY  */);
			logError (hr);

			if (hr == S_OK)
			{
				IDirectSoundBuffer* pPrimaryBuffer;

				DSBUFFERDESC primaryDesc = { 0 };
				primaryDesc.dwSize = sizeof (DSBUFFERDESC);
				primaryDesc.dwFlags = 1 /* DSBCAPS_PRIMARYBUFFER */;
				primaryDesc.dwBufferBytes = 0;
				primaryDesc.lpwfxFormat = 0;

				log ("opening dsound out step 2");
				hr = pDirectSound->CreateSoundBuffer (&primaryDesc, &pPrimaryBuffer, 0);
				logError (hr);

				if (hr == S_OK)
				{
					WAVEFORMATEX wfFormat;
					wfFormat.wFormatTag = WAVE_FORMAT_PCM;
					wfFormat.nChannels = (unsigned short) numChannels;
					wfFormat.nSamplesPerSec = (DWORD) sampleRate;
					wfFormat.wBitsPerSample = (unsigned short) bitDepth;
					wfFormat.nBlockAlign = (unsigned short) (wfFormat.nChannels * wfFormat.wBitsPerSample / 8);
					wfFormat.nAvgBytesPerSec = wfFormat.nSamplesPerSec * wfFormat.nBlockAlign;
					wfFormat.cbSize = 0;

					hr = pPrimaryBuffer->SetFormat (&wfFormat);
					logError (hr);

					if (hr == S_OK)
					{
						DSBUFFERDESC secondaryDesc = { 0 };
						secondaryDesc.dwSize = sizeof (DSBUFFERDESC);
						secondaryDesc.dwFlags =  0x8000 /* DSBCAPS_GLOBALFOCUS */
												  | 0x10000 /* DSBCAPS_GETCURRENTPOSITION2 */;
						secondaryDesc.dwBufferBytes = (DWORD) totalBytesPerBuffer;
						secondaryDesc.lpwfxFormat = &wfFormat;

						hr = pDirectSound->CreateSoundBuffer (&secondaryDesc, &pOutputBuffer, 0);
						logError (hr);

						if (hr == S_OK)
						{
							log ("opening dsound out step 3");

							DWORD dwDataLen;
							unsigned char* pDSBuffData;

							hr = pOutputBuffer->Lock (0, (DWORD) totalBytesPerBuffer,
													  (LPVOID*) &pDSBuffData, &dwDataLen, 0, 0, 0);
							logError (hr);

							if (hr == S_OK)
							{
								zeromem (pDSBuffData, dwDataLen);

								hr = pOutputBuffer->Unlock (pDSBuffData, dwDataLen, 0, 0);

								if (hr == S_OK)
								{
									hr = pOutputBuffer->SetCurrentPosition (0);

									if (hr == S_OK)
									{
										hr = pOutputBuffer->Play (0, 0, 1 /* DSBPLAY_LOOPING */);

										if (hr == S_OK)
											return String::empty;
									}
								}
							}
						}
					}
				}
			}
		}

		error = getDSErrorMessage (hr);
		close();
		return error;
	}

	void synchronisePosition()
	{
		if (pOutputBuffer != nullptr)
		{
			DWORD playCursor;
			pOutputBuffer->GetCurrentPosition (&playCursor, &writeOffset);
		}
	}

	bool service()
	{
		if (pOutputBuffer == 0)
			return true;

		DWORD playCursor, writeCursor;

		for (;;)
		{
			HRESULT hr = pOutputBuffer->GetCurrentPosition (&playCursor, &writeCursor);

			if (hr == MAKE_HRESULT (1, 0x878, 150)) // DSERR_BUFFERLOST
			{
				pOutputBuffer->Restore();
				continue;
			}

			if (hr == S_OK)
				break;

			logError (hr);
			jassertfalse;
			return true;
		}

		int playWriteGap = (int) (writeCursor - playCursor);
		if (playWriteGap < 0)
			playWriteGap += totalBytesPerBuffer;

		int bytesEmpty = (int) (playCursor - writeOffset);

		if (bytesEmpty < 0)
			bytesEmpty += totalBytesPerBuffer;

		if (bytesEmpty > (totalBytesPerBuffer - playWriteGap))
		{
			writeOffset = writeCursor;
			bytesEmpty = totalBytesPerBuffer - playWriteGap;
		}

		if (bytesEmpty >= bytesPerBuffer)
		{
			void* lpbuf1 = nullptr;
			void* lpbuf2 = nullptr;
			DWORD dwSize1 = 0;
			DWORD dwSize2 = 0;

			HRESULT hr = pOutputBuffer->Lock ((DWORD) writeOffset, (DWORD) bytesPerBuffer,
											  &lpbuf1, &dwSize1,
											  &lpbuf2, &dwSize2, 0);

			if (hr == MAKE_HRESULT (1, 0x878, 150)) // DSERR_BUFFERLOST
			{
				pOutputBuffer->Restore();

				hr = pOutputBuffer->Lock ((DWORD) writeOffset, (DWORD) bytesPerBuffer,
										  &lpbuf1, &dwSize1,
										  &lpbuf2, &dwSize2, 0);
			}

			if (hr == S_OK)
			{
				if (bitDepth == 16)
				{
					int* dest = static_cast<int*> (lpbuf1);
					const float* left = leftBuffer;
					const float* right = rightBuffer;
					int samples1 = (int) (dwSize1 >> 2);
					int samples2 = (int) (dwSize2 >> 2);

					if (left == 0)
					{
						while (--samples1 >= 0)
							*dest++ = (convertInputValue (*right++) << 16);

						dest = static_cast<int*> (lpbuf2);

						while (--samples2 >= 0)
							*dest++ = (convertInputValue (*right++) << 16);
					}
					else if (right == 0)
					{
						while (--samples1 >= 0)
							*dest++ = (0xffff & convertInputValue (*left++));

						dest = static_cast<int*> (lpbuf2);

						while (--samples2 >= 0)
							*dest++ = (0xffff & convertInputValue (*left++));
					}
					else
					{
						while (--samples1 >= 0)
						{
							const int l = convertInputValue (*left++);
							const int r = convertInputValue (*right++);
							*dest++ = (r << 16) | (0xffff & l);
						}

						dest = static_cast<int*> (lpbuf2);

						while (--samples2 >= 0)
						{
							const int l = convertInputValue (*left++);
							const int r = convertInputValue (*right++);
							*dest++ = (r << 16) | (0xffff & l);
						}
					}
				}
				else
				{
					jassertfalse;
				}

				writeOffset = (writeOffset + dwSize1 + dwSize2) % totalBytesPerBuffer;

				pOutputBuffer->Unlock (lpbuf1, dwSize1, lpbuf2, dwSize2);
			}
			else
			{
				jassertfalse;
				logError (hr);
			}

			bytesEmpty -= bytesPerBuffer;
			return true;
		}
		else
		{
			return false;
		}
	}

	int bitDepth;
	bool doneFlag;

private:
	String name;
	GUID guid;
	int sampleRate, bufferSizeSamples;
	float* leftBuffer;
	float* rightBuffer;

	IDirectSound* pDirectSound;
	IDirectSoundBuffer* pOutputBuffer;
	DWORD writeOffset;
	int totalBytesPerBuffer, bytesPerBuffer;
	unsigned int lastPlayCursor;

	static inline int convertInputValue (const float v) noexcept
	{
		return jlimit (-32768, 32767, roundToInt (32767.0f * v));
	}

	JUCE_DECLARE_NON_COPYABLE (DSoundInternalOutChannel);
};

struct DSoundInternalInChannel
{
public:
	DSoundInternalInChannel (const String& name_, const GUID& guid_, int rate,
							 int bufferSize, float* left, float* right)
		: bitDepth (16), name (name_), guid (guid_), sampleRate (rate),
		  bufferSizeSamples (bufferSize), leftBuffer (left), rightBuffer (right),
		  pDirectSound (nullptr), pDirectSoundCapture (nullptr), pInputBuffer (nullptr)
	{
	}

	~DSoundInternalInChannel()
	{
		close();
	}

	void close()
	{
		HRESULT hr;

		if (pInputBuffer != nullptr)
		{
			log ("closing dsound in: " + name);
			hr = pInputBuffer->Stop();
			logError (hr);

			pInputBuffer->Release();
			pInputBuffer = nullptr;
		}

		if (pDirectSoundCapture != nullptr)
		{
			pDirectSoundCapture->Release();
			pDirectSoundCapture = nullptr;
		}

		if (pDirectSound != nullptr)
		{
			pDirectSound->Release();
			pDirectSound = nullptr;
		}
	}

	String open()
	{
		log ("opening dsound in device: " + name
			  + "  rate=" + String (sampleRate) + " bits=" + String (bitDepth) + " buf=" + String (bufferSizeSamples));

		pDirectSound = nullptr;
		pDirectSoundCapture = nullptr;
		pInputBuffer = nullptr;
		readOffset = 0;
		totalBytesPerBuffer = 0;

		String error;
		HRESULT hr = E_NOINTERFACE;

		if (dsDirectSoundCaptureCreate != 0)
			hr = dsDirectSoundCaptureCreate (&guid, &pDirectSoundCapture, 0);

		logError (hr);

		if (hr == S_OK)
		{
			const int numChannels = 2;
			bytesPerBuffer = (bufferSizeSamples * (bitDepth >> 2)) & ~15;
			totalBytesPerBuffer = (3 * bytesPerBuffer) & ~15;

			WAVEFORMATEX wfFormat;
			wfFormat.wFormatTag = WAVE_FORMAT_PCM;
			wfFormat.nChannels = (unsigned short)numChannels;
			wfFormat.nSamplesPerSec = (DWORD) sampleRate;
			wfFormat.wBitsPerSample = (unsigned short)bitDepth;
			wfFormat.nBlockAlign = (unsigned short)(wfFormat.nChannels * (wfFormat.wBitsPerSample / 8));
			wfFormat.nAvgBytesPerSec = wfFormat.nSamplesPerSec * wfFormat.nBlockAlign;
			wfFormat.cbSize = 0;

			DSCBUFFERDESC captureDesc = { 0 };
			captureDesc.dwSize = sizeof (DSCBUFFERDESC);
			captureDesc.dwFlags = 0;
			captureDesc.dwBufferBytes = (DWORD) totalBytesPerBuffer;
			captureDesc.lpwfxFormat = &wfFormat;

			log ("opening dsound in step 2");
			hr = pDirectSoundCapture->CreateCaptureBuffer (&captureDesc, &pInputBuffer, 0);

			logError (hr);

			if (hr == S_OK)
			{
				hr = pInputBuffer->Start (1 /* DSCBSTART_LOOPING */);
				logError (hr);

				if (hr == S_OK)
					return String::empty;
			}
		}

		error = getDSErrorMessage (hr);
		close();

		return error;
	}

	void synchronisePosition()
	{
		if (pInputBuffer != nullptr)
		{
			DWORD capturePos;
			pInputBuffer->GetCurrentPosition (&capturePos, (DWORD*)&readOffset);
		}
	}

	bool service()
	{
		if (pInputBuffer == 0)
			return true;

		DWORD capturePos, readPos;
		HRESULT hr = pInputBuffer->GetCurrentPosition (&capturePos, &readPos);
		logError (hr);

		if (hr != S_OK)
			return true;

		int bytesFilled = (int) (readPos - readOffset);
		if (bytesFilled < 0)
			bytesFilled += totalBytesPerBuffer;

		if (bytesFilled >= bytesPerBuffer)
		{
			LPBYTE lpbuf1 = nullptr;
			LPBYTE lpbuf2 = nullptr;
			DWORD dwsize1 = 0;
			DWORD dwsize2 = 0;

			HRESULT hr = pInputBuffer->Lock ((DWORD) readOffset, (DWORD) bytesPerBuffer,
											 (void**) &lpbuf1, &dwsize1,
											 (void**) &lpbuf2, &dwsize2, 0);

			if (hr == S_OK)
			{
				if (bitDepth == 16)
				{
					const float g = 1.0f / 32768.0f;

					float* destL = leftBuffer;
					float* destR = rightBuffer;
					int samples1 = (int) (dwsize1 >> 2);
					int samples2 = (int) (dwsize2 >> 2);

					const short* src = (const short*)lpbuf1;

					if (destL == 0)
					{
						while (--samples1 >= 0)
						{
							++src;
							*destR++ = *src++ * g;
						}

						src = (const short*)lpbuf2;

						while (--samples2 >= 0)
						{
							++src;
							*destR++ = *src++ * g;
						}
					}
					else if (destR == 0)
					{
						while (--samples1 >= 0)
						{
							*destL++ = *src++ * g;
							++src;
						}

						src = (const short*)lpbuf2;

						while (--samples2 >= 0)
						{
							*destL++ = *src++ * g;
							++src;
						}
					}
					else
					{
						while (--samples1 >= 0)
						{
							*destL++ = *src++ * g;
							*destR++ = *src++ * g;
						}

						src = (const short*)lpbuf2;

						while (--samples2 >= 0)
						{
							*destL++ = *src++ * g;
							*destR++ = *src++ * g;
						}
					}
				}
				else
				{
					jassertfalse;
				}

				readOffset = (readOffset + dwsize1 + dwsize2) % totalBytesPerBuffer;

				pInputBuffer->Unlock (lpbuf1, dwsize1, lpbuf2, dwsize2);
			}
			else
			{
				logError (hr);
				jassertfalse;
			}

			bytesFilled -= bytesPerBuffer;

			return true;
		}
		else
		{
			return false;
		}
	}

	unsigned int readOffset;
	int bytesPerBuffer, totalBytesPerBuffer;
	int bitDepth;
	bool doneFlag;

private:
	String name;
	GUID guid;
	int sampleRate, bufferSizeSamples;
	float* leftBuffer;
	float* rightBuffer;

	IDirectSound* pDirectSound;
	IDirectSoundCapture* pDirectSoundCapture;
	IDirectSoundCaptureBuffer* pInputBuffer;

	JUCE_DECLARE_NON_COPYABLE (DSoundInternalInChannel);
};

class DSoundAudioIODevice  : public AudioIODevice,
							 public Thread
{
public:
	DSoundAudioIODevice (const String& deviceName,
						 const int outputDeviceIndex_,
						 const int inputDeviceIndex_)
		: AudioIODevice (deviceName, "DirectSound"),
		  Thread ("Juce DSound"),
		  outputDeviceIndex (outputDeviceIndex_),
		  inputDeviceIndex (inputDeviceIndex_),
		  isOpen_ (false),
		  isStarted (false),
		  bufferSizeSamples (0),
		  totalSamplesOut (0),
		  sampleRate (0.0),
		  inputBuffers (1, 1),
		  outputBuffers (1, 1),
		  callback (nullptr)
	{
		if (outputDeviceIndex_ >= 0)
		{
			outChannels.add (TRANS("Left"));
			outChannels.add (TRANS("Right"));
		}

		if (inputDeviceIndex_ >= 0)
		{
			inChannels.add (TRANS("Left"));
			inChannels.add (TRANS("Right"));
		}
	}

	~DSoundAudioIODevice()
	{
		close();
	}

	String open (const BigInteger& inputChannels,
				 const BigInteger& outputChannels,
				 double sampleRate, int bufferSizeSamples)
	{
		lastError = openDevice (inputChannels, outputChannels, sampleRate, bufferSizeSamples);
		isOpen_ = lastError.isEmpty();

		return lastError;
	}

	void close()
	{
		stop();

		if (isOpen_)
		{
			closeDevice();
			isOpen_ = false;
		}
	}

	bool isOpen()                                       { return isOpen_ && isThreadRunning(); }
	int getCurrentBufferSizeSamples()                   { return bufferSizeSamples; }
	double getCurrentSampleRate()                       { return sampleRate; }
	BigInteger getActiveOutputChannels() const          { return enabledOutputs; }
	BigInteger getActiveInputChannels() const           { return enabledInputs; }
	int getOutputLatencyInSamples()                     { return (int) (getCurrentBufferSizeSamples() * 1.5); }
	int getInputLatencyInSamples()                      { return getOutputLatencyInSamples(); }
	StringArray getOutputChannelNames()                 { return outChannels; }
	StringArray getInputChannelNames()                  { return inChannels; }

	int getNumSampleRates()                             { return 4; }
	int getDefaultBufferSize()                          { return 2560; }
	int getNumBufferSizesAvailable()                    { return 50; }

	double getSampleRate (int index)
	{
		const double samps[] = { 44100.0, 48000.0, 88200.0, 96000.0 };
		return samps [jlimit (0, 3, index)];
	}

	int getBufferSizeSamples (int index)
	{
		int n = 64;
		for (int i = 0; i < index; ++i)
			n += (n < 512) ? 32
						   : ((n < 1024) ? 64
										 : ((n < 2048) ? 128 : 256));

		return n;
	}

	int getCurrentBitDepth()
	{
		int i, bits = 256;

		for (i = inChans.size(); --i >= 0;)
			bits = jmin (bits, inChans[i]->bitDepth);

		for (i = outChans.size(); --i >= 0;)
			bits = jmin (bits, outChans[i]->bitDepth);

		if (bits > 32)
			bits = 16;

		return bits;
	}

	void start (AudioIODeviceCallback* call)
	{
		if (isOpen_ && call != nullptr && ! isStarted)
		{
			if (! isThreadRunning())
			{
				// something gone wrong and the thread's stopped..
				isOpen_ = false;
				return;
			}

			call->audioDeviceAboutToStart (this);

			const ScopedLock sl (startStopLock);
			callback = call;
			isStarted = true;
		}
	}

	void stop()
	{
		if (isStarted)
		{
			AudioIODeviceCallback* const callbackLocal = callback;

			{
				const ScopedLock sl (startStopLock);
				isStarted = false;
			}

			if (callbackLocal != nullptr)
				callbackLocal->audioDeviceStopped();
		}
	}

	bool isPlaying()                { return isStarted && isOpen_ && isThreadRunning(); }
	String getLastError()           { return lastError; }

	StringArray inChannels, outChannels;
	int outputDeviceIndex, inputDeviceIndex;

private:
	bool isOpen_;
	bool isStarted;
	String lastError;

	OwnedArray <DSoundInternalInChannel> inChans;
	OwnedArray <DSoundInternalOutChannel> outChans;
	WaitableEvent startEvent;

	int bufferSizeSamples;
	int volatile totalSamplesOut;
	int64 volatile lastBlockTime;
	double sampleRate;
	BigInteger enabledInputs, enabledOutputs;
	AudioSampleBuffer inputBuffers, outputBuffers;

	AudioIODeviceCallback* callback;
	CriticalSection startStopLock;

	String openDevice (const BigInteger& inputChannels,
					   const BigInteger& outputChannels,
					   double sampleRate_, int bufferSizeSamples_);

	void closeDevice()
	{
		isStarted = false;
		stopThread (5000);

		inChans.clear();
		outChans.clear();
		inputBuffers.setSize (1, 1);
		outputBuffers.setSize (1, 1);
	}

	void resync()
	{
		if (! threadShouldExit())
		{
			sleep (5);

			int i;
			for (i = 0; i < outChans.size(); ++i)
				outChans.getUnchecked(i)->synchronisePosition();

			for (i = 0; i < inChans.size(); ++i)
				inChans.getUnchecked(i)->synchronisePosition();
		}
	}

public:
	void run()
	{
		while (! threadShouldExit())
		{
			if (wait (100))
				break;
		}

		const int latencyMs = (int) (bufferSizeSamples * 1000.0 / sampleRate);
		const int maxTimeMS = jmax (5, 3 * latencyMs);

		while (! threadShouldExit())
		{
			int numToDo = 0;
			uint32 startTime = Time::getMillisecondCounter();

			int i;
			for (i = inChans.size(); --i >= 0;)
			{
				inChans.getUnchecked(i)->doneFlag = false;
				++numToDo;
			}

			for (i = outChans.size(); --i >= 0;)
			{
				outChans.getUnchecked(i)->doneFlag = false;
				++numToDo;
			}

			if (numToDo > 0)
			{
				const int maxCount = 3;
				int count = maxCount;

				for (;;)
				{
					for (i = inChans.size(); --i >= 0;)
					{
						DSoundInternalInChannel* const in = inChans.getUnchecked(i);

						if ((! in->doneFlag) && in->service())
						{
							in->doneFlag = true;
							--numToDo;
						}
					}

					for (i = outChans.size(); --i >= 0;)
					{
						DSoundInternalOutChannel* const out = outChans.getUnchecked(i);

						if ((! out->doneFlag) && out->service())
						{
							out->doneFlag = true;
							--numToDo;
						}
					}

					if (numToDo <= 0)
						break;

					if (Time::getMillisecondCounter() > startTime + maxTimeMS)
					{
						resync();
						break;
					}

					if (--count <= 0)
					{
						Sleep (1);
						count = maxCount;
					}

					if (threadShouldExit())
						return;
				}
			}
			else
			{
				sleep (1);
			}

			const ScopedLock sl (startStopLock);

			if (isStarted)
			{
				JUCE_TRY
				{
					callback->audioDeviceIOCallback (const_cast <const float**> (inputBuffers.getArrayOfChannels()),
													 inputBuffers.getNumChannels(),
													 outputBuffers.getArrayOfChannels(),
													 outputBuffers.getNumChannels(),
													 bufferSizeSamples);
				}
				JUCE_CATCH_EXCEPTION

				totalSamplesOut += bufferSizeSamples;
			}
			else
			{
				outputBuffers.clear();
				totalSamplesOut = 0;
				sleep (1);
			}
		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSoundAudioIODevice);
};

struct DSoundDeviceList
{
	StringArray outputDeviceNames, inputDeviceNames;
	Array<GUID> outputGuids, inputGuids;

	void scan()
	{
		outputDeviceNames.clear();
		inputDeviceNames.clear();
		outputGuids.clear();
		inputGuids.clear();

		if (dsDirectSoundEnumerateW != 0)
		{
			dsDirectSoundEnumerateW (outputEnumProcW, this);
			dsDirectSoundCaptureEnumerateW (inputEnumProcW, this);
		}
	}

	bool operator!= (const DSoundDeviceList& other) const noexcept
	{
		return outputDeviceNames != other.outputDeviceNames
			|| inputDeviceNames != other.inputDeviceNames
			|| outputGuids != other.outputGuids
			|| inputGuids != other.inputGuids;
	}

private:
	static BOOL enumProc (LPGUID lpGUID, String desc, StringArray& names, Array<GUID>& guids)
	{
		desc = desc.trim();

		if (desc.isNotEmpty())
		{
			const String origDesc (desc);

			int n = 2;
			while (names.contains (desc))
				desc = origDesc + " (" + String (n++) + ")";

			names.add (desc);
			guids.add (lpGUID != nullptr ? *lpGUID : GUID());
		}

		return TRUE;
	}

	BOOL outputEnumProc (LPGUID guid, LPCWSTR desc)  { return enumProc (guid, desc, outputDeviceNames, outputGuids); }
	BOOL inputEnumProc (LPGUID guid, LPCWSTR desc)   { return enumProc (guid, desc, inputDeviceNames, inputGuids); }

	static BOOL CALLBACK outputEnumProcW (LPGUID lpGUID, LPCWSTR description, LPCWSTR, LPVOID object)
	{
		return static_cast<DSoundDeviceList*> (object)->outputEnumProc (lpGUID, description);
	}

	static BOOL CALLBACK inputEnumProcW (LPGUID lpGUID, LPCWSTR description, LPCWSTR, LPVOID object)
	{
		return static_cast<DSoundDeviceList*> (object)->inputEnumProc (lpGUID, description);
	}
};

String DSoundAudioIODevice::openDevice (const BigInteger& inputChannels,
										const BigInteger& outputChannels,
										double sampleRate_, int bufferSizeSamples_)
{
	closeDevice();
	totalSamplesOut = 0;

	sampleRate = sampleRate_;

	if (bufferSizeSamples_ <= 0)
		bufferSizeSamples_ = 960; // use as a default size if none is set.

	bufferSizeSamples = bufferSizeSamples_ & ~7;

	DSoundDeviceList dlh;
	dlh.scan();

	enabledInputs = inputChannels;
	enabledInputs.setRange (inChannels.size(),
							enabledInputs.getHighestBit() + 1 - inChannels.size(),
							false);

	inputBuffers.setSize (jmax (1, enabledInputs.countNumberOfSetBits()), bufferSizeSamples);
	inputBuffers.clear();
	int i, numIns = 0;

	for (i = 0; i <= enabledInputs.getHighestBit(); i += 2)
	{
		float* left = nullptr;
		if (enabledInputs[i])
			left = inputBuffers.getSampleData (numIns++);

		float* right = nullptr;
		if (enabledInputs[i + 1])
			right = inputBuffers.getSampleData (numIns++);

		if (left != nullptr || right != nullptr)
			inChans.add (new DSoundInternalInChannel (dlh.inputDeviceNames [inputDeviceIndex],
													  dlh.inputGuids [inputDeviceIndex],
													  (int) sampleRate, bufferSizeSamples,
													  left, right));
	}

	enabledOutputs = outputChannels;
	enabledOutputs.setRange (outChannels.size(),
							 enabledOutputs.getHighestBit() + 1 - outChannels.size(),
							 false);

	outputBuffers.setSize (jmax (1, enabledOutputs.countNumberOfSetBits()), bufferSizeSamples);
	outputBuffers.clear();
	int numOuts = 0;

	for (i = 0; i <= enabledOutputs.getHighestBit(); i += 2)
	{
		float* left = nullptr;
		if (enabledOutputs[i])
			left = outputBuffers.getSampleData (numOuts++);

		float* right = nullptr;
		if (enabledOutputs[i + 1])
			right = outputBuffers.getSampleData (numOuts++);

		if (left != nullptr || right != nullptr)
			outChans.add (new DSoundInternalOutChannel (dlh.outputDeviceNames[outputDeviceIndex],
														dlh.outputGuids [outputDeviceIndex],
														(int) sampleRate, bufferSizeSamples,
														left, right));
	}

	String error;

	// boost our priority while opening the devices to try to get better sync between them
	const int oldThreadPri = GetThreadPriority (GetCurrentThread());
	const DWORD oldProcPri = GetPriorityClass (GetCurrentProcess());
	SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	SetPriorityClass (GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	for (i = 0; i < outChans.size(); ++i)
	{
		error = outChans[i]->open();

		if (error.isNotEmpty())
		{
			error = "Error opening " + dlh.outputDeviceNames[i] + ": \"" + error + "\"";
			break;
		}
	}

	if (error.isEmpty())
	{
		for (i = 0; i < inChans.size(); ++i)
		{
			error = inChans[i]->open();

			if (error.isNotEmpty())
			{
				error = "Error opening " + dlh.inputDeviceNames[i] + ": \"" + error + "\"";
				break;
			}
		}
	}

	if (error.isEmpty())
	{
		totalSamplesOut = 0;

		for (i = 0; i < outChans.size(); ++i)
			outChans.getUnchecked(i)->synchronisePosition();

		for (i = 0; i < inChans.size(); ++i)
			inChans.getUnchecked(i)->synchronisePosition();

		startThread (9);
		sleep (10);

		notify();
	}
	else
	{
		log (error);
	}

	SetThreadPriority (GetCurrentThread(), oldThreadPri);
	SetPriorityClass (GetCurrentProcess(), oldProcPri);

	return error;
}

class DSoundAudioIODeviceType  : public AudioIODeviceType,
								 private DeviceChangeDetector
{
public:
	DSoundAudioIODeviceType()
		: AudioIODeviceType ("DirectSound"),
		  DeviceChangeDetector (L"DirectSound"),
		  hasScanned (false)
	{
		initialiseDSoundFunctions();
	}

	void scanForDevices()
	{
		hasScanned = true;
		deviceList.scan();
	}

	StringArray getDeviceNames (bool wantInputNames) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		return wantInputNames ? deviceList.inputDeviceNames
							  : deviceList.outputDeviceNames;
	}

	int getDefaultDeviceIndex (bool /*forInput*/) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this
		return 0;
	}

	int getIndexOfDevice (AudioIODevice* device, bool asInput) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		DSoundAudioIODevice* const d = dynamic_cast <DSoundAudioIODevice*> (device);
		if (d == 0)
			return -1;

		return asInput ? d->inputDeviceIndex
					   : d->outputDeviceIndex;
	}

	bool hasSeparateInputsAndOutputs() const    { return true; }

	AudioIODevice* createDevice (const String& outputDeviceName,
								 const String& inputDeviceName)
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		const int outputIndex = deviceList.outputDeviceNames.indexOf (outputDeviceName);
		const int inputIndex = deviceList.inputDeviceNames.indexOf (inputDeviceName);

		if (outputIndex >= 0 || inputIndex >= 0)
			return new DSoundAudioIODevice (outputDeviceName.isNotEmpty() ? outputDeviceName
																		  : inputDeviceName,
											outputIndex, inputIndex);

		return nullptr;
	}

private:

	DSoundDeviceList deviceList;
	bool hasScanned;

	void systemDeviceChanged()
	{
		DSoundDeviceList newList;
		newList.scan();

		if (newList != deviceList)
		{
			deviceList = newList;
			callDeviceChangeListeners();
		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSoundAudioIODeviceType);
};

AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_DirectSound()
{
	return new DSoundAudioIODeviceType();
}

#undef log
#undef logError

/*** End of inlined file: juce_win32_DirectSound.cpp ***/


 #endif


/*** Start of inlined file: juce_win32_Midi.cpp ***/
class MidiInCollector
{
public:

	MidiInCollector (MidiInput* const input_,
					 MidiInputCallback& callback_)
		: deviceHandle (0),
		  input (input_),
		  callback (callback_),
		  concatenator (4096),
		  isStarted (false),
		  startTime (0)
	{
	}

	~MidiInCollector()
	{
		stop();

		if (deviceHandle != 0)
		{
			int count = 5;
			while (--count >= 0)
			{
				if (midiInClose (deviceHandle) == MMSYSERR_NOERROR)
					break;

				Sleep (20);
			}
		}
	}

	void handleMessage (const uint32 message, const uint32 timeStamp)
	{
		if ((message & 0xff) >= 0x80 && isStarted)
		{
			concatenator.pushMidiData (&message, 3, convertTimeStamp (timeStamp), input, callback);
			writeFinishedBlocks();
		}
	}

	void handleSysEx (MIDIHDR* const hdr, const uint32 timeStamp)
	{
		if (isStarted && hdr->dwBytesRecorded > 0)
		{
			concatenator.pushMidiData (hdr->lpData, (int) hdr->dwBytesRecorded, convertTimeStamp (timeStamp), input, callback);
			writeFinishedBlocks();
		}
	}

	void start()
	{
		jassert (deviceHandle != 0);
		if (deviceHandle != 0 && ! isStarted)
		{
			activeMidiCollectors.addIfNotAlreadyThere (this);

			for (int i = 0; i < (int) numHeaders; ++i)
				headers[i].write (deviceHandle);

			startTime = Time::getMillisecondCounter();
			MMRESULT res = midiInStart (deviceHandle);

			if (res == MMSYSERR_NOERROR)
			{
				concatenator.reset();
				isStarted = true;
			}
			else
			{
				unprepareAllHeaders();
			}
		}
	}

	void stop()
	{
		if (isStarted)
		{
			isStarted = false;
			midiInReset (deviceHandle);
			midiInStop (deviceHandle);
			activeMidiCollectors.removeValue (this);
			unprepareAllHeaders();
			concatenator.reset();
		}
	}

	static void CALLBACK midiInCallback (HMIDIIN, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR midiMessage, DWORD_PTR timeStamp)
	{
		MidiInCollector* const collector = reinterpret_cast <MidiInCollector*> (dwInstance);

		if (activeMidiCollectors.contains (collector))
		{
			if (uMsg == MIM_DATA)
				collector->handleMessage ((uint32) midiMessage, (uint32) timeStamp);
			else if (uMsg == MIM_LONGDATA)
				collector->handleSysEx ((MIDIHDR*) midiMessage, (uint32) timeStamp);
		}
	}

	HMIDIIN deviceHandle;

private:
	static Array <MidiInCollector*, CriticalSection> activeMidiCollectors;

	MidiInput* input;
	MidiInputCallback& callback;
	MidiDataConcatenator concatenator;
	bool volatile isStarted;
	uint32 startTime;

	class MidiHeader
	{
	public:
		MidiHeader()
		{
			zerostruct (hdr);
			hdr.lpData = data;
			hdr.dwBufferLength = (DWORD) numElementsInArray (data);
		}

		void write (HMIDIIN deviceHandle)
		{
			hdr.dwBytesRecorded = 0;
			MMRESULT res = midiInPrepareHeader (deviceHandle, &hdr, sizeof (hdr));
			res = midiInAddBuffer (deviceHandle, &hdr, sizeof (hdr));
		}

		void writeIfFinished (HMIDIIN deviceHandle)
		{
			if ((hdr.dwFlags & WHDR_DONE) != 0)
			{
				MMRESULT res = midiInUnprepareHeader (deviceHandle, &hdr, sizeof (hdr));
				(void) res;
				write (deviceHandle);
			}
		}

		void unprepare (HMIDIIN deviceHandle)
		{
			if ((hdr.dwFlags & WHDR_DONE) != 0)
			{
				int c = 10;
				while (--c >= 0 && midiInUnprepareHeader (deviceHandle, &hdr, sizeof (hdr)) == MIDIERR_STILLPLAYING)
					Thread::sleep (20);

				jassert (c >= 0);
			}
		}

	private:
		MIDIHDR hdr;
		char data [256];

		JUCE_DECLARE_NON_COPYABLE (MidiHeader);
	};

	enum { numHeaders = 32 };
	MidiHeader headers [numHeaders];

	void writeFinishedBlocks()
	{
		for (int i = 0; i < (int) numHeaders; ++i)
			headers[i].writeIfFinished (deviceHandle);
	}

	void unprepareAllHeaders()
	{
		for (int i = 0; i < (int) numHeaders; ++i)
			headers[i].unprepare (deviceHandle);
	}

	double convertTimeStamp (uint32 timeStamp)
	{
		timeStamp += startTime;

		const uint32 now = Time::getMillisecondCounter();
		if (timeStamp > now)
		{
			if (timeStamp > now + 2)
				--startTime;

			timeStamp = now;
		}

		return timeStamp * 0.001;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiInCollector);
};

Array <MidiInCollector*, CriticalSection> MidiInCollector::activeMidiCollectors;

StringArray MidiInput::getDevices()
{
	StringArray s;
	const UINT num = midiInGetNumDevs();

	for (UINT i = 0; i < num; ++i)
	{
		MIDIINCAPS mc = { 0 };

		if (midiInGetDevCaps (i, &mc, sizeof (mc)) == MMSYSERR_NOERROR)
			s.add (String (mc.szPname, sizeof (mc.szPname)));
	}

	return s;
}

int MidiInput::getDefaultDeviceIndex()
{
	return 0;
}

MidiInput* MidiInput::openDevice (const int index, MidiInputCallback* const callback)
{
	if (callback == nullptr)
		return nullptr;

	UINT deviceId = MIDI_MAPPER;
	int n = 0;
	String name;

	const UINT num = midiInGetNumDevs();

	for (UINT i = 0; i < num; ++i)
	{
		MIDIINCAPS mc = { 0 };

		if (midiInGetDevCaps (i, &mc, sizeof (mc)) == MMSYSERR_NOERROR)
		{
			if (index == n)
			{
				deviceId = i;
				name = String (mc.szPname, (size_t) numElementsInArray (mc.szPname));
				break;
			}

			++n;
		}
	}

	ScopedPointer <MidiInput> in (new MidiInput (name));
	ScopedPointer <MidiInCollector> collector (new MidiInCollector (in, *callback));

	HMIDIIN h;
	MMRESULT err = midiInOpen (&h, deviceId,
							   (DWORD_PTR) &MidiInCollector::midiInCallback,
							   (DWORD_PTR) (MidiInCollector*) collector,
							   CALLBACK_FUNCTION);

	if (err == MMSYSERR_NOERROR)
	{
		collector->deviceHandle = h;
		in->internal = collector.release();
		return in.release();
	}

	return nullptr;
}

MidiInput::MidiInput (const String& name_)
	: name (name_),
	  internal (0)
{
}

MidiInput::~MidiInput()
{
	delete static_cast <MidiInCollector*> (internal);
}

void MidiInput::start()
{
	static_cast <MidiInCollector*> (internal)->start();
}

void MidiInput::stop()
{
	static_cast <MidiInCollector*> (internal)->stop();
}

struct MidiOutHandle
{
	int refCount;
	UINT deviceId;
	HMIDIOUT handle;

	static Array<MidiOutHandle*> activeHandles;

private:
	JUCE_LEAK_DETECTOR (MidiOutHandle);
};

Array<MidiOutHandle*> MidiOutHandle::activeHandles;

StringArray MidiOutput::getDevices()
{
	StringArray s;
	const UINT num = midiOutGetNumDevs();

	for (UINT i = 0; i < num; ++i)
	{
		MIDIOUTCAPS mc = { 0 };

		if (midiOutGetDevCaps (i, &mc, sizeof (mc)) == MMSYSERR_NOERROR)
			s.add (String (mc.szPname, sizeof (mc.szPname)));
	}

	return s;
}

int MidiOutput::getDefaultDeviceIndex()
{
	const UINT num = midiOutGetNumDevs();
	int n = 0;

	for (UINT i = 0; i < num; ++i)
	{
		MIDIOUTCAPS mc = { 0 };

		if (midiOutGetDevCaps (i, &mc, sizeof (mc)) == MMSYSERR_NOERROR)
		{
			if ((mc.wTechnology & MOD_MAPPER) != 0)
				return n;

			 ++n;
		}
	}

	return 0;
}

MidiOutput* MidiOutput::openDevice (int index)
{
	UINT deviceId = MIDI_MAPPER;
	const UINT num = midiOutGetNumDevs();
	int n = 0;

	{
		for (UINT i = 0; i < num; ++i)
		{
			MIDIOUTCAPS mc = { 0 };

			if (midiOutGetDevCaps (i, &mc, sizeof (mc)) == MMSYSERR_NOERROR)
			{
				// use the microsoft sw synth as a default - best not to allow deviceId
				// to be MIDI_MAPPER, or else device sharing breaks
				if (String (mc.szPname, sizeof (mc.szPname)).containsIgnoreCase ("microsoft"))
					deviceId = i;

				if (index == n)
				{
					deviceId = i;
					break;
				}

				++n;
			}
		}
	}

	int i;
	for (i = MidiOutHandle::activeHandles.size(); --i >= 0;)
	{
		MidiOutHandle* const han = MidiOutHandle::activeHandles.getUnchecked(i);

		if (han->deviceId == deviceId)
		{
			han->refCount++;

			MidiOutput* const out = new MidiOutput();
			out->internal = han;
			return out;
		}
	}

	for (i = 4; --i >= 0;)
	{
		HMIDIOUT h = 0;
		MMRESULT res = midiOutOpen (&h, deviceId, 0, 0, CALLBACK_NULL);

		if (res == MMSYSERR_NOERROR)
		{
			MidiOutHandle* const han = new MidiOutHandle();
			han->deviceId = deviceId;
			han->refCount = 1;
			han->handle = h;
			MidiOutHandle::activeHandles.add (han);

			MidiOutput* const out = new MidiOutput();
			out->internal = han;
			return out;
		}
		else if (res == MMSYSERR_ALLOCATED)
		{
			Sleep (100);
		}
		else
		{
			break;
		}
	}

	return nullptr;
}

MidiOutput::~MidiOutput()
{
	stopBackgroundThread();

	MidiOutHandle* const h = static_cast <MidiOutHandle*> (internal);

	if (MidiOutHandle::activeHandles.contains (h) && --(h->refCount) == 0)
	{
		midiOutClose (h->handle);
		MidiOutHandle::activeHandles.removeValue (h);
		delete h;
	}
}

void MidiOutput::sendMessageNow (const MidiMessage& message)
{
	const MidiOutHandle* const handle = static_cast <const MidiOutHandle*> (internal);

	if (message.getRawDataSize() > 3
		 || message.isSysEx())
	{
		MIDIHDR h = { 0 };

		h.lpData = (char*) message.getRawData();
		h.dwBufferLength = (DWORD) message.getRawDataSize();
		h.dwBytesRecorded = (DWORD) message.getRawDataSize();

		if (midiOutPrepareHeader (handle->handle, &h, sizeof (MIDIHDR)) == MMSYSERR_NOERROR)
		{
			MMRESULT res = midiOutLongMsg (handle->handle, &h, sizeof (MIDIHDR));

			if (res == MMSYSERR_NOERROR)
			{
				while ((h.dwFlags & MHDR_DONE) == 0)
					Sleep (1);

				int count = 500; // 1 sec timeout

				while (--count >= 0)
				{
					res = midiOutUnprepareHeader (handle->handle, &h, sizeof (MIDIHDR));

					if (res == MIDIERR_STILLPLAYING)
						Sleep (2);
					else
						break;
				}
			}
		}
	}
	else
	{
		midiOutShortMsg (handle->handle,
						 *(unsigned int*) message.getRawData());
	}
}

/*** End of inlined file: juce_win32_Midi.cpp ***/

 #if JUCE_ASIO

/*** Start of inlined file: juce_win32_ASIO.cpp ***/
#undef WINDOWS
#undef log

// #define ASIO_DEBUGGING 1

#if ASIO_DEBUGGING
 #define log(a) { Logger::writeToLog (a); DBG (a) }
#else
 #define log(a) {}
#endif

/* The ASIO SDK *should* declare its callback functions as being __cdecl, but different versions seem
   to be pretty random about whether or not they do this. If you hit an error using these functions
   it'll be because you're trying to build using __stdcall, in which case you'd need to either get hold of
   an ASIO SDK which correctly specifies __cdecl, or add the __cdecl keyword to its functions yourself.
*/
#define JUCE_ASIOCALLBACK __cdecl

namespace ASIODebugging
{
  #if ASIO_DEBUGGING
	static void log (const String& context, long error)
	{
		const char* err = "unknown error";

		if (error == ASE_NotPresent)            err = "Not Present";
		else if (error == ASE_HWMalfunction)    err = "Hardware Malfunction";
		else if (error == ASE_InvalidParameter) err = "Invalid Parameter";
		else if (error == ASE_InvalidMode)      err = "Invalid Mode";
		else if (error == ASE_SPNotAdvancing)   err = "Sample position not advancing";
		else if (error == ASE_NoClock)          err = "No Clock";
		else if (error == ASE_NoMemory)         err = "Out of memory";

		log ("!!error: " + context + " - " + err);
	}

	#define logError(a, b) ASIODebugging::log ((a), (b))
  #else
	#define logError(a, b) {}
  #endif
}

struct ASIOSampleFormat
{
	ASIOSampleFormat() noexcept {}

	ASIOSampleFormat (const long type) noexcept
		: bitDepth (24),
		  littleEndian (true),
		  formatIsFloat (false),
		  byteStride (4)
	{
		switch (type)
		{
			case ASIOSTInt16MSB:    byteStride = 2; littleEndian = false; bitDepth = 16; break;
			case ASIOSTInt24MSB:    byteStride = 3; littleEndian = false; break;
			case ASIOSTInt32MSB:    bitDepth = 32; littleEndian = false; break;
			case ASIOSTFloat32MSB:  bitDepth = 32; littleEndian = false; formatIsFloat = true; break;
			case ASIOSTFloat64MSB:  bitDepth = 64; byteStride = 8; littleEndian = false; break;
			case ASIOSTInt32MSB16:  bitDepth = 16; littleEndian = false; break;
			case ASIOSTInt32MSB18:  littleEndian = false; break;
			case ASIOSTInt32MSB20:  littleEndian = false; break;
			case ASIOSTInt32MSB24:  littleEndian = false; break;
			case ASIOSTInt16LSB:    byteStride = 2; bitDepth = 16; break;
			case ASIOSTInt24LSB:    byteStride = 3; break;
			case ASIOSTInt32LSB:    bitDepth = 32; break;
			case ASIOSTFloat32LSB:  bitDepth = 32; formatIsFloat = true; break;
			case ASIOSTFloat64LSB:  bitDepth = 64; byteStride = 8; break;
			case ASIOSTInt32LSB16:  bitDepth = 16; break;
			case ASIOSTInt32LSB18:  break; // (unhandled)
			case ASIOSTInt32LSB20:  break; // (unhandled)
			case ASIOSTInt32LSB24:  break;

			case ASIOSTDSDInt8LSB1: break; // (unhandled)
			case ASIOSTDSDInt8MSB1: break; // (unhandled)
			case ASIOSTDSDInt8NER8: break; // (unhandled)

			default:
				jassertfalse;  // (not a valid format code..)
				break;
		}
	}

	void convertToFloat (const void* const src, float* const dst, const int samps) const noexcept
	{
		if (formatIsFloat)
		{
			memcpy (dst, src, samps * sizeof (float));
		}
		else
		{
			switch (bitDepth)
			{
				case 16: convertInt16ToFloat (static_cast <const char*> (src), dst, byteStride, samps, littleEndian); break;
				case 24: convertInt24ToFloat (static_cast <const char*> (src), dst, byteStride, samps, littleEndian); break;
				case 32: convertInt32ToFloat (static_cast <const char*> (src), dst, byteStride, samps, littleEndian); break;
				default: jassertfalse; break;
			}
		}
	}

	void convertFromFloat (const float* const src, void* const dst, const int samps) const noexcept
	{
		if (formatIsFloat)
		{
			memcpy (dst, src, samps * sizeof (float));
		}
		else
		{
			switch (bitDepth)
			{
				case 16: convertFloatToInt16 (src, static_cast <char*> (dst), byteStride, samps, littleEndian); break;
				case 24: convertFloatToInt24 (src, static_cast <char*> (dst), byteStride, samps, littleEndian); break;
				case 32: convertFloatToInt32 (src, static_cast <char*> (dst), byteStride, samps, littleEndian); break;
				default: jassertfalse; break;
			}
		}
	}

	void clear (void* dst, const int numSamps) noexcept
	{
		if (dst != nullptr)
			zeromem (dst, numSamps * byteStride);
	}

	int bitDepth, byteStride;
	bool formatIsFloat, littleEndian;

private:
	static void convertInt16ToFloat (const char* src, float* dest, const int srcStrideBytes,
									 int numSamples, const bool littleEndian) noexcept
	{
		const double g = 1.0 / 32768.0;

		if (littleEndian)
		{
			while (--numSamples >= 0)
			{
				*dest++ = (float) (g * (short) ByteOrder::littleEndianShort (src));
				src += srcStrideBytes;
			}
		}
		else
		{
			while (--numSamples >= 0)
			{
				*dest++ = (float) (g * (short) ByteOrder::bigEndianShort (src));
				src += srcStrideBytes;
			}
		}
	}

	static void convertFloatToInt16 (const float* src, char* dest, const int dstStrideBytes,
									 int numSamples, const bool littleEndian) noexcept
	{
		const double maxVal = (double) 0x7fff;

		if (littleEndian)
		{
			while (--numSamples >= 0)
			{
				*(uint16*) dest = ByteOrder::swapIfBigEndian ((uint16) (short) roundDoubleToInt (jlimit (-maxVal, maxVal, maxVal * *src++)));
				dest += dstStrideBytes;
			}
		}
		else
		{
			while (--numSamples >= 0)
			{
				*(uint16*) dest = ByteOrder::swapIfLittleEndian ((uint16) (short) roundDoubleToInt (jlimit (-maxVal, maxVal, maxVal * *src++)));
				dest += dstStrideBytes;
			}
		}
	}

	static void convertInt24ToFloat (const char* src, float* dest, const int srcStrideBytes,
									 int numSamples, const bool littleEndian) noexcept
	{
		const double g = 1.0 / 0x7fffff;

		if (littleEndian)
		{
			while (--numSamples >= 0)
			{
				*dest++ = (float) (g * ByteOrder::littleEndian24Bit (src));
				src += srcStrideBytes;
			}
		}
		else
		{
			while (--numSamples >= 0)
			{
				*dest++ = (float) (g * ByteOrder::bigEndian24Bit (src));
				src += srcStrideBytes;
			}
		}
	}

	static void convertFloatToInt24 (const float* src, char* dest, const int dstStrideBytes,
									 int numSamples, const bool littleEndian) noexcept
	{
		const double maxVal = (double) 0x7fffff;

		if (littleEndian)
		{
			while (--numSamples >= 0)
			{
				ByteOrder::littleEndian24BitToChars ((uint32) roundDoubleToInt (jlimit (-maxVal, maxVal, maxVal * *src++)), dest);
				dest += dstStrideBytes;
			}
		}
		else
		{
			while (--numSamples >= 0)
			{
				ByteOrder::bigEndian24BitToChars ((uint32) roundDoubleToInt (jlimit (-maxVal, maxVal, maxVal * *src++)), dest);
				dest += dstStrideBytes;
			}
		}
	}

	static void convertInt32ToFloat (const char* src, float* dest, const int srcStrideBytes,
									 int numSamples, const bool littleEndian) noexcept
	{
		const double g = 1.0 / 0x7fffffff;

		if (littleEndian)
		{
			while (--numSamples >= 0)
			{
				*dest++ = (float) (g * (int) ByteOrder::littleEndianInt (src));
				src += srcStrideBytes;
			}
		}
		else
		{
			while (--numSamples >= 0)
			{
				*dest++ = (float) (g * (int) ByteOrder::bigEndianInt (src));
				src += srcStrideBytes;
			}
		}
	}

	static void convertFloatToInt32 (const float* src, char* dest, const int dstStrideBytes,
									 int numSamples, const bool littleEndian) noexcept
	{
		const double maxVal = (double) 0x7fffffff;

		if (littleEndian)
		{
			while (--numSamples >= 0)
			{
				*(uint32*) dest = ByteOrder::swapIfBigEndian ((uint32) roundDoubleToInt (jlimit (-maxVal, maxVal, maxVal * *src++)));
				dest += dstStrideBytes;
			}
		}
		else
		{
			while (--numSamples >= 0)
			{
				*(uint32*) dest = ByteOrder::swapIfLittleEndian ((uint32) roundDoubleToInt (jlimit (-maxVal, maxVal, maxVal * *src++)));
				dest += dstStrideBytes;
			}
		}
	}
};

class ASIOAudioIODevice;
static ASIOAudioIODevice* volatile currentASIODev[3] = { 0 };

extern HWND juce_messageWindowHandle;

class ASIOAudioIODevice  : public AudioIODevice,
						   private Timer
{
public:
	ASIOAudioIODevice (const String& name_, const CLSID classId_, const int slotNumber,
					   const String& optionalDllForDirectLoading_)
	   : AudioIODevice (name_, "ASIO"),
		 asioObject (nullptr),
		 classId (classId_),
		 optionalDllForDirectLoading (optionalDllForDirectLoading_),
		 currentBitDepth (16),
		 currentSampleRate (0),
		 deviceIsOpen (false),
		 isStarted (false),
		 buffersCreated (false),
		 postOutput (true),
		 insideControlPanelModalLoop (false),
		 shouldUsePreferredSize (false)
	{
		name = name_;

		jassert (currentASIODev [slotNumber] == nullptr);
		currentASIODev [slotNumber] = this;

		openDevice();
	}

	~ASIOAudioIODevice()
	{
		for (int i = 0; i < numElementsInArray (currentASIODev); ++i)
			if (currentASIODev[i] == this)
				currentASIODev[i] = nullptr;

		close();
		log ("ASIO - exiting");
		removeCurrentDriver();
	}

	void updateSampleRates()
	{
		// find a list of sample rates..
		const double possibleSampleRates[] = { 44100.0, 48000.0, 88200.0, 96000.0, 176400.0, 192000.0 };
		sampleRates.clear();

		if (asioObject != nullptr)
		{
			for (int index = 0; index < numElementsInArray (possibleSampleRates); ++index)
			{
				const long err = asioObject->canSampleRate (possibleSampleRates[index]);

				if (err == 0)
				{
					sampleRates.add ((int) possibleSampleRates[index]);
					log ("rate: " + String ((int) possibleSampleRates[index]));
				}
				else if (err != ASE_NoClock)
				{
					logError ("CanSampleRate", err);
				}
			}

			if (sampleRates.size() == 0)
			{
				double cr = 0;
				const long err = asioObject->getSampleRate (&cr);
				log ("No sample rates supported - current rate: " + String ((int) cr));

				if (err == 0)
					sampleRates.add ((int) cr);
			}
		}
	}

	StringArray getOutputChannelNames()         { return outputChannelNames; }
	StringArray getInputChannelNames()          { return inputChannelNames; }

	int getNumSampleRates()                     { return sampleRates.size(); }
	double getSampleRate (int index)            { return sampleRates [index]; }

	int getNumBufferSizesAvailable()            { return bufferSizes.size(); }
	int getBufferSizeSamples (int index)        { return bufferSizes [index]; }
	int getDefaultBufferSize()                  { return preferredSize; }

	String open (const BigInteger& inputChannels,
				 const BigInteger& outputChannels,
				 double sr,
				 int bufferSizeSamples)
	{
		close();
		currentCallback = nullptr;

		if (bufferSizeSamples <= 0)
			shouldUsePreferredSize = true;

		if (asioObject == nullptr || ! isASIOOpen)
		{
			log ("Warning: device not open");
			const String err (openDevice());

			if (asioObject == nullptr || ! isASIOOpen)
				return err;
		}

		isStarted = false;
		bufferIndex = -1;
		long err = 0;
		long newPreferredSize = 0;
		minSize = 0;
		maxSize = 0;
		granularity = 0;

		if (asioObject->getBufferSize (&minSize, &maxSize, &newPreferredSize, &granularity) == 0)
		{
			if (preferredSize != 0 && newPreferredSize != 0 && newPreferredSize != preferredSize)
				shouldUsePreferredSize = true;

			preferredSize = newPreferredSize;
		}

		// unfortunate workaround for certain manufacturers whose drivers crash horribly if you make
		// dynamic changes to the buffer size...
		shouldUsePreferredSize = shouldUsePreferredSize
								   || getName().containsIgnoreCase ("Digidesign");

		if (shouldUsePreferredSize)
		{
			log ("Using preferred size for buffer..");

			if ((err = asioObject->getBufferSize (&minSize, &maxSize, &preferredSize, &granularity)) == 0)
			{
				bufferSizeSamples = preferredSize;
			}
			else
			{
				bufferSizeSamples = 1024;
				logError ("GetBufferSize1", err);
			}

			shouldUsePreferredSize = false;
		}

		int sampleRate = roundDoubleToInt (sr);
		currentSampleRate = sampleRate;
		currentBlockSizeSamples = bufferSizeSamples;
		currentChansOut.clear();
		currentChansIn.clear();
		zeromem (inBuffers, sizeof (inBuffers));
		zeromem (outBuffers, sizeof (outBuffers));

		updateSampleRates();

		if (sampleRate == 0 || (sampleRates.size() > 0 && ! sampleRates.contains (sampleRate)))
			sampleRate = sampleRates[0];

		jassert (sampleRate != 0);
		if (sampleRate == 0)
			sampleRate = 44100;

		ASIOClockSource clocks[32] = { 0 };
		long numSources = numElementsInArray (clocks);
		asioObject->getClockSources (clocks, &numSources);
		bool isSourceSet = false;

		// careful not to remove this loop because it does more than just logging!
		int i;
		for (i = 0; i < numSources; ++i)
		{
			String s ("clock: ");
			s += clocks[i].name;

			if (clocks[i].isCurrentSource)
			{
				isSourceSet = true;
				s << " (cur)";
			}

			log (s);
		}

		if (numSources > 1 && ! isSourceSet)
		{
			log ("setting clock source");
			asioObject->setClockSource (clocks[0].index);
			Thread::sleep (20);
		}
		else
		{
			if (numSources == 0)
			{
				log ("ASIO - no clock sources!");
			}
		}

		double cr = 0;
		err = asioObject->getSampleRate (&cr);
		if (err == 0)
		{
			currentSampleRate = cr;
		}
		else
		{
			logError ("GetSampleRate", err);
			currentSampleRate = 0;
		}

		error = String::empty;
		needToReset = false;
		isReSync = false;
		err = 0;
		buffersCreated = false;

		if (currentSampleRate != sampleRate)
		{
			log ("ASIO samplerate: " + String (currentSampleRate) + " to " + String (sampleRate));
			err = asioObject->setSampleRate (sampleRate);

			if (err == ASE_NoClock && numSources > 0)
			{
				log ("trying to set a clock source..");
				Thread::sleep (10);
				err = asioObject->setClockSource (clocks[0].index);
				if (err != 0)
				{
					logError ("SetClock", err);
				}

				Thread::sleep (10);
				err = asioObject->setSampleRate (sampleRate);
			}
		}

		if (err == 0)
		{
			currentSampleRate = sampleRate;

			if (needToReset)
			{
				if (isReSync)
				{
					log ("Resync request");
				}

				log ("! Resetting ASIO after sample rate change");
				removeCurrentDriver();

				loadDriver();
				const String error (initDriver());

				if (error.isNotEmpty())
				{
					log ("ASIOInit: " + error);
				}

				needToReset = false;
				isReSync = false;
			}

			numActiveInputChans = 0;
			numActiveOutputChans = 0;

			ASIOBufferInfo* info = bufferInfos;
			int i;
			for (i = 0; i < totalNumInputChans; ++i)
			{
				if (inputChannels[i])
				{
					currentChansIn.setBit (i);
					info->isInput = 1;
					info->channelNum = i;
					info->buffers[0] = info->buffers[1] = nullptr;
					++info;
					++numActiveInputChans;
				}
			}

			for (i = 0; i < totalNumOutputChans; ++i)
			{
				if (outputChannels[i])
				{
					currentChansOut.setBit (i);
					info->isInput = 0;
					info->channelNum = i;
					info->buffers[0] = info->buffers[1] = nullptr;
					++info;
					++numActiveOutputChans;
				}
			}

			const int totalBuffers = numActiveInputChans + numActiveOutputChans;

			setCallbackFunctions();

			log ("disposing buffers");
			err = asioObject->disposeBuffers();

			log ("creating buffers: " + String (totalBuffers) + ", " + String (currentBlockSizeSamples));
			err = asioObject->createBuffers (bufferInfos,
											 totalBuffers,
											 currentBlockSizeSamples,
											 &callbacks);

			if (err != 0)
			{
				currentBlockSizeSamples = preferredSize;
				logError ("create buffers 2", err);

				asioObject->disposeBuffers();
				err = asioObject->createBuffers (bufferInfos,
												 totalBuffers,
												 currentBlockSizeSamples,
												 &callbacks);
			}

			if (err == 0)
			{
				buffersCreated = true;

				tempBuffer.calloc (totalBuffers * currentBlockSizeSamples + 32);

				int n = 0;
				Array <int> types;
				currentBitDepth = 16;

				for (i = 0; i < jmin ((int) totalNumInputChans, (int) maxASIOChannels); ++i)
				{
					if (inputChannels[i])
					{
						inBuffers[n] = tempBuffer + (currentBlockSizeSamples * n);

						ASIOChannelInfo channelInfo = { 0 };
						channelInfo.channel = i;
						channelInfo.isInput = 1;
						asioObject->getChannelInfo (&channelInfo);

						types.addIfNotAlreadyThere (channelInfo.type);
						inputFormat[n] = ASIOSampleFormat (channelInfo.type);

						currentBitDepth = jmax (currentBitDepth, inputFormat[n].bitDepth);
						++n;
					}
				}

				jassert (numActiveInputChans == n);
				n = 0;

				for (i = 0; i < jmin ((int) totalNumOutputChans, (int) maxASIOChannels); ++i)
				{
					if (outputChannels[i])
					{
						outBuffers[n] = tempBuffer + (currentBlockSizeSamples * (numActiveInputChans + n));

						ASIOChannelInfo channelInfo = { 0 };
						channelInfo.channel = i;
						channelInfo.isInput = 0;
						asioObject->getChannelInfo (&channelInfo);

						types.addIfNotAlreadyThere (channelInfo.type);
						outputFormat[n] = ASIOSampleFormat (channelInfo.type);

						currentBitDepth = jmax (currentBitDepth, outputFormat[n].bitDepth);
						++n;
					}
				}

				jassert (numActiveOutputChans == n);

				for (i = types.size(); --i >= 0;)
				{
					log ("channel format: " + String (types[i]));
				}

				jassert (n <= totalBuffers);

				for (i = 0; i < numActiveOutputChans; ++i)
				{
					outputFormat[i].clear (bufferInfos [numActiveInputChans + i].buffers[0], currentBlockSizeSamples);
					outputFormat[i].clear (bufferInfos [numActiveInputChans + i].buffers[1], currentBlockSizeSamples);
				}

				inputLatency = outputLatency = 0;

				if (asioObject->getLatencies (&inputLatency, &outputLatency) != 0)
				{
					log ("ASIO - no latencies");
				}
				else
				{
					log ("ASIO latencies: " + String ((int) outputLatency) + ", " + String ((int) inputLatency));
				}

				deviceIsOpen = true;

				log ("starting ASIO");
				calledback = false;
				err = asioObject->start();

				if (err != 0)
				{
					deviceIsOpen = false;
					log ("ASIO - stop on failure");
					Thread::sleep (10);
					asioObject->stop();
					error = "Can't start device";
					Thread::sleep (10);
				}
				else
				{
					int count = 300;
					while (--count > 0 && ! calledback)
						Thread::sleep (10);

					isStarted = true;

					if (! calledback)
					{
						error = "Device didn't start correctly";
						log ("ASIO didn't callback - stopping..");
						asioObject->stop();
					}
				}
			}
			else
			{
				error = "Can't create i/o buffers";
			}
		}
		else
		{
			error = "Can't set sample rate: ";
			error << sampleRate;
		}

		if (error.isNotEmpty())
		{
			logError (error, err);
			disposeBuffers();

			Thread::sleep (20);
			isStarted = false;
			deviceIsOpen = false;

			const String errorCopy (error);
			close(); // (this resets the error string)
			error = errorCopy;
		}

		needToReset = false;
		isReSync = false;

		return error;
	}

	void close()
	{
		error = String::empty;
		stopTimer();
		stop();

		if (isASIOOpen && deviceIsOpen)
		{
			const ScopedLock sl (callbackLock);

			deviceIsOpen = false;
			isStarted = false;
			needToReset = false;
			isReSync = false;

			log ("ASIO - stopping");

			if (asioObject != nullptr)
			{
				Thread::sleep (20);
				asioObject->stop();
				Thread::sleep (10);
				disposeBuffers();
			}

			Thread::sleep (10);
		}
	}

	bool isOpen()                       { return deviceIsOpen || insideControlPanelModalLoop; }
	bool isPlaying()                    { return isASIOOpen && (currentCallback != nullptr); }

	int getCurrentBufferSizeSamples()   { return currentBlockSizeSamples; }
	double getCurrentSampleRate()       { return currentSampleRate; }
	int getCurrentBitDepth()            { return currentBitDepth; }

	BigInteger getActiveOutputChannels() const    { return currentChansOut; }
	BigInteger getActiveInputChannels() const     { return currentChansIn; }

	int getOutputLatencyInSamples()     { return outputLatency + currentBlockSizeSamples / 4; }
	int getInputLatencyInSamples()      { return inputLatency + currentBlockSizeSamples / 4; }

	void start (AudioIODeviceCallback* callback)
	{
		if (callback != nullptr)
		{
			callback->audioDeviceAboutToStart (this);

			const ScopedLock sl (callbackLock);
			currentCallback = callback;
		}
	}

	void stop()
	{
		AudioIODeviceCallback* const lastCallback = currentCallback;

		{
			const ScopedLock sl (callbackLock);
			currentCallback = nullptr;
		}

		if (lastCallback != nullptr)
			lastCallback->audioDeviceStopped();
	}

	String getLastError()           { return error; }
	bool hasControlPanel() const    { return true; }

	bool showControlPanel()
	{
		log ("ASIO - showing control panel");

		bool done = false;

		JUCE_TRY
		{
			// are there are devices that need to be closed before showing their control panel?
			// close();
			insideControlPanelModalLoop = true;

			const uint32 started = Time::getMillisecondCounter();

			if (asioObject != nullptr)
			{
				asioObject->controlPanel();

				const int spent = (int) Time::getMillisecondCounter() - (int) started;

				log ("spent: " + String (spent));

				if (spent > 300)
				{
					shouldUsePreferredSize = true;
					done = true;
				}
			}
		}
		JUCE_CATCH_ALL

		insideControlPanelModalLoop = false;
		return done;
	}

	void resetRequest() noexcept
	{
		needToReset = true;
	}

	void resyncRequest() noexcept
	{
		needToReset = true;
		isReSync = true;
	}

	void timerCallback()
	{
		if (! insideControlPanelModalLoop)
		{
			stopTimer();

			// used to cause a reset
			log ("! ASIO restart request!");

			if (deviceIsOpen)
			{
				AudioIODeviceCallback* const oldCallback = currentCallback;

				close();
				open (BigInteger (currentChansIn), BigInteger (currentChansOut),
					  currentSampleRate, currentBlockSizeSamples);

				if (oldCallback != nullptr)
					start (oldCallback);
			}
		}
		else
		{
			startTimer (100);
		}
	}

private:

	IASIO* volatile asioObject;
	ASIOCallbacks callbacks;

	CLSID classId;
	const String optionalDllForDirectLoading;
	String error;

	long totalNumInputChans, totalNumOutputChans;
	StringArray inputChannelNames, outputChannelNames;

	Array<int> sampleRates, bufferSizes;
	long inputLatency, outputLatency;
	long minSize, maxSize, preferredSize, granularity;

	int volatile currentBlockSizeSamples;
	int volatile currentBitDepth;
	double volatile currentSampleRate;
	BigInteger currentChansOut, currentChansIn;
	AudioIODeviceCallback* volatile currentCallback;
	CriticalSection callbackLock;

	enum { maxASIOChannels = 160 };

	ASIOBufferInfo bufferInfos [maxASIOChannels];
	float* inBuffers [maxASIOChannels];
	float* outBuffers [maxASIOChannels];

	ASIOSampleFormat inputFormat [maxASIOChannels];
	ASIOSampleFormat outputFormat [maxASIOChannels];

	WaitableEvent event1;
	HeapBlock <float> tempBuffer;
	int volatile bufferIndex, numActiveInputChans, numActiveOutputChans;

	bool deviceIsOpen, isStarted, buffersCreated;
	bool volatile isASIOOpen;
	bool volatile calledback;
	bool volatile littleEndian, postOutput, needToReset, isReSync;
	bool volatile insideControlPanelModalLoop;
	bool volatile shouldUsePreferredSize;

	void removeCurrentDriver()
	{
		if (asioObject != nullptr)
		{
			asioObject->Release();
			asioObject = nullptr;
		}
	}

	bool loadDriver()
	{
		removeCurrentDriver();

		JUCE_TRY
		{
			if (CoCreateInstance (classId, 0, CLSCTX_INPROC_SERVER,
								  classId, (void**) &asioObject) == S_OK)
			{
				return true;
			}

			// If a class isn't registered but we have a path for it, we can fallback to
			// doing a direct load of the COM object (only available via the juce_createASIOAudioIODeviceForGUID function).
			if (optionalDllForDirectLoading.isNotEmpty())
			{
				HMODULE h = LoadLibrary (optionalDllForDirectLoading.toWideCharPointer());

				if (h != 0)
				{
					typedef HRESULT (CALLBACK* DllGetClassObjectFunc) (REFCLSID clsid, REFIID iid, LPVOID* ppv);
					DllGetClassObjectFunc dllGetClassObject = (DllGetClassObjectFunc) GetProcAddress (h, "DllGetClassObject");

					if (dllGetClassObject != 0)
					{
						IClassFactory* classFactory = nullptr;
						HRESULT hr = dllGetClassObject (classId, IID_IClassFactory, (void**) &classFactory);

						if (classFactory != nullptr)
						{
							hr = classFactory->CreateInstance (0, classId, (void**) &asioObject);
							classFactory->Release();
						}

						return asioObject != nullptr;
					}
				}
			}
		}
		JUCE_CATCH_ALL

		asioObject = nullptr;
		return false;
	}

	String initDriver()
	{
		if (asioObject != nullptr)
		{
			char buffer [256] = { 0 };

			if (! asioObject->init (juce_messageWindowHandle))
			{
				asioObject->getErrorMessage (buffer);
				return String (buffer, sizeof (buffer) - 1);
			}

			// just in case any daft drivers expect this to be called..
			asioObject->getDriverName (buffer);

			return String::empty;
		}

		return "No Driver";
	}

	String openDevice()
	{
		// open the device and get its info..
		log ("opening ASIO device: " + getName());

		needToReset = false;
		isReSync = false;
		outputChannelNames.clear();
		inputChannelNames.clear();
		bufferSizes.clear();
		sampleRates.clear();
		isASIOOpen = false;
		deviceIsOpen = false;
		totalNumInputChans = 0;
		totalNumOutputChans = 0;
		numActiveInputChans = 0;
		numActiveOutputChans = 0;
		currentCallback = nullptr;

		error = String::empty;

		if (getName().isEmpty())
			return error;

		long err = 0;

		if (loadDriver())
		{
			if ((error = initDriver()).isEmpty())
			{
				numActiveInputChans = 0;
				numActiveOutputChans = 0;
				totalNumInputChans = 0;
				totalNumOutputChans = 0;

				if (asioObject != nullptr
					 && (err = asioObject->getChannels (&totalNumInputChans, &totalNumOutputChans)) == 0)
				{
					log (String ((int) totalNumInputChans) + " in, " + String ((int) totalNumOutputChans) + " out");

					if ((err = asioObject->getBufferSize (&minSize, &maxSize, &preferredSize, &granularity)) == 0)
					{
						// find a list of buffer sizes..
						log (String ((int) minSize) + " " + String ((int) maxSize) + " " + String ((int) preferredSize) + " " + String ((int) granularity));

						if (granularity >= 0)
						{
							granularity = jmax (1, (int) granularity);

							for (int i = jmax ((int) minSize, (int) granularity); i < jmin (6400, (int) maxSize); i += granularity)
								bufferSizes.addIfNotAlreadyThere (granularity * (i / granularity));
						}
						else if (granularity < 0)
						{
							for (int i = 0; i < 18; ++i)
							{
								const int s = (1 << i);

								if (s >= minSize && s <= maxSize)
									bufferSizes.add (s);
							}
						}

						if (! bufferSizes.contains (preferredSize))
							bufferSizes.insert (0, preferredSize);

						double currentRate = 0;
						asioObject->getSampleRate (&currentRate);

						if (currentRate <= 0.0 || currentRate > 192001.0)
						{
							log ("setting sample rate");
							err = asioObject->setSampleRate (44100.0);
							if (err != 0)
							{
								logError ("setting sample rate", err);
							}

							asioObject->getSampleRate (&currentRate);
						}

						currentSampleRate = currentRate;

						postOutput = (asioObject->outputReady() == 0);
						if (postOutput)
						{
							log ("ASIO outputReady = ok");
						}

						updateSampleRates();

						// ..because cubase does it at this point
						inputLatency = outputLatency = 0;
						if (asioObject->getLatencies (&inputLatency, &outputLatency) != 0)
						{
							log ("ASIO - no latencies");
						}

						log ("latencies: " + String ((int) inputLatency) + ", " + String ((int) outputLatency));

						// create some dummy buffers now.. because cubase does..
						numActiveInputChans = 0;
						numActiveOutputChans = 0;

						ASIOBufferInfo* info = bufferInfos;
						int i, numChans = 0;
						for (i = 0; i < jmin (2, (int) totalNumInputChans); ++i)
						{
							info->isInput = 1;
							info->channelNum = i;
							info->buffers[0] = info->buffers[1] = nullptr;
							++info;
							++numChans;
						}

						const int outputBufferIndex = numChans;

						for (i = 0; i < jmin (2, (int) totalNumOutputChans); ++i)
						{
							info->isInput = 0;
							info->channelNum = i;
							info->buffers[0] = info->buffers[1] = nullptr;
							++info;
							++numChans;
						}

						setCallbackFunctions();

						log ("creating buffers (dummy): " + String (numChans) + ", " + String ((int) preferredSize));

						if (preferredSize > 0)
						{
							err = asioObject->createBuffers (bufferInfos, numChans, preferredSize, &callbacks);
							if (err != 0)
							{
								logError ("dummy buffers", err);
							}
						}

						long newInps = 0, newOuts = 0;
						asioObject->getChannels (&newInps, &newOuts);

						if (totalNumInputChans != newInps || totalNumOutputChans != newOuts)
						{
							totalNumInputChans = newInps;
							totalNumOutputChans = newOuts;

							log (String ((int) totalNumInputChans) + " in; " + String ((int) totalNumOutputChans) + " out");
						}

						updateSampleRates();

						for (i = 0; i < totalNumInputChans; ++i)
						{
							ASIOChannelInfo channelInfo = { 0 };
							channelInfo.channel = i;
							channelInfo.isInput = 1;
							asioObject->getChannelInfo (&channelInfo);

							inputChannelNames.add (String (CharPointer_UTF8 (channelInfo.name)));
						}

						for (i = 0; i < totalNumOutputChans; ++i)
						{
							ASIOChannelInfo channelInfo = { 0 };
							channelInfo.channel = i;
							channelInfo.isInput = 0;
							asioObject->getChannelInfo (&channelInfo);

							outputChannelNames.add (String (CharPointer_UTF8 (channelInfo.name)));
							outputFormat[i] = ASIOSampleFormat (channelInfo.type);

							if (i < 2)
							{
								// clear the channels that are used with the dummy stuff
								outputFormat[i].clear (bufferInfos [outputBufferIndex + i].buffers[0], preferredSize);
								outputFormat[i].clear (bufferInfos [outputBufferIndex + i].buffers[1], preferredSize);
							}
						}

						outputChannelNames.trim();
						inputChannelNames.trim();
						outputChannelNames.appendNumbersToDuplicates (false, true);
						inputChannelNames.appendNumbersToDuplicates (false, true);

						// start and stop because cubase does it..
						asioObject->getLatencies (&inputLatency, &outputLatency);

						if ((err = asioObject->start()) != 0)
						{
							// ignore an error here, as it might start later after setting other stuff up
							logError ("ASIO start", err);
						}

						Thread::sleep (100);
						asioObject->stop();
					}
					else
					{
						error = "Can't detect buffer sizes";
					}
				}
				else
				{
					error = "Can't detect asio channels";
				}
			}
		}
		else
		{
			error = "No such device";
		}

		if (error.isNotEmpty())
		{
			logError (error, err);
			disposeBuffers();
			removeCurrentDriver();
			isASIOOpen = false;
		}
		else
		{
			isASIOOpen = true;
			log ("ASIO device open");
		}

		deviceIsOpen = false;
		needToReset = false;
		isReSync = false;

		return error;
	}

	void disposeBuffers()
	{
		if (asioObject != nullptr && buffersCreated)
		{
			buffersCreated = false;
			asioObject->disposeBuffers();
		}
	}

	void JUCE_ASIOCALLBACK callback (const long index)
	{
		if (isStarted)
		{
			bufferIndex = index;
			processBuffer();
		}
		else
		{
			if (postOutput && (asioObject != nullptr))
				asioObject->outputReady();
		}

		calledback = true;
	}

	void processBuffer()
	{
		const ASIOBufferInfo* const infos = bufferInfos;
		const int bi = bufferIndex;

		const ScopedLock sl (callbackLock);

		if (needToReset)
		{
			needToReset = false;

			if (isReSync)
			{
				log ("! ASIO resync");
				isReSync = false;
			}
			else
			{
				startTimer (20);
			}
		}

		if (bi >= 0)
		{
			const int samps = currentBlockSizeSamples;

			if (currentCallback != nullptr)
			{
				int i;
				for (i = 0; i < numActiveInputChans; ++i)
				{
					jassert (inBuffers[i]!= nullptr);
					inputFormat[i].convertToFloat (infos[i].buffers[bi], inBuffers[i], samps);
				}

				currentCallback->audioDeviceIOCallback ((const float**) inBuffers, numActiveInputChans,
														outBuffers, numActiveOutputChans, samps);

				for (i = 0; i < numActiveOutputChans; ++i)
				{
					jassert (outBuffers[i] != nullptr);
					outputFormat[i].convertFromFloat (outBuffers[i], infos [numActiveInputChans + i].buffers[bi], samps);
				}
			}
			else
			{
				for (int i = 0; i < numActiveOutputChans; ++i)
					 outputFormat[i].clear (infos[numActiveInputChans + i].buffers[bi], samps);
			}
		}

		if (postOutput)
			asioObject->outputReady();
	}

	template <int deviceIndex>
	struct ASIOCallbackFunctions
	{
		static ASIOTime* JUCE_ASIOCALLBACK bufferSwitchTimeInfoCallback (ASIOTime*, long index, long)
		{
			if (currentASIODev[deviceIndex] != nullptr)
				currentASIODev[deviceIndex]->callback (index);

			return nullptr;
		}

		static void JUCE_ASIOCALLBACK bufferSwitchCallback (long index, long)
		{
			if (currentASIODev[deviceIndex] != nullptr)
				currentASIODev[deviceIndex]->callback (index);
		}

		static long JUCE_ASIOCALLBACK asioMessagesCallback (long selector, long value, void*, double*)
		{
			return ASIOAudioIODevice::asioMessagesCallback (selector, value, deviceIndex);
		}

		static void setCallbacks (ASIOCallbacks& callbacks)
		{
			callbacks.bufferSwitch = &bufferSwitchCallback;
			callbacks.asioMessage = &asioMessagesCallback;
			callbacks.bufferSwitchTimeInfo = &bufferSwitchTimeInfoCallback;
		}
	};

	void setCallbackFunctions()
	{
		callbacks.sampleRateDidChange = &sampleRateChangedCallback;

		if      (currentASIODev[0] == this)  ASIOCallbackFunctions<0>::setCallbacks (callbacks);
		else if (currentASIODev[1] == this)  ASIOCallbackFunctions<1>::setCallbacks (callbacks);
		else if (currentASIODev[2] == this)  ASIOCallbackFunctions<2>::setCallbacks (callbacks);
		else                                 jassertfalse;
	}

	static long asioMessagesCallback (long selector, long value, const int deviceIndex)
	{
		switch (selector)
		{
		case kAsioSelectorSupported:
			if (value == kAsioResetRequest
				|| value == kAsioEngineVersion
				|| value == kAsioResyncRequest
				|| value == kAsioLatenciesChanged
				|| value == kAsioSupportsInputMonitor)
				return 1;
			break;

		case kAsioBufferSizeChange:
			break;

		case kAsioResetRequest:
			if (currentASIODev[deviceIndex] != nullptr)
				currentASIODev[deviceIndex]->resetRequest();

			return 1;

		case kAsioResyncRequest:
			if (currentASIODev[deviceIndex] != nullptr)
				currentASIODev[deviceIndex]->resyncRequest();

			return 1;

		case kAsioLatenciesChanged:
			return 1;

		case kAsioEngineVersion:
			return 2;

		case kAsioSupportsTimeInfo:
		case kAsioSupportsTimeCode:
			return 0;
		}

		return 0;
	}

	static void JUCE_ASIOCALLBACK sampleRateChangedCallback (ASIOSampleRate)
	{
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ASIOAudioIODevice);
};

class ASIOAudioIODeviceType  : public AudioIODeviceType
{
public:
	ASIOAudioIODeviceType()
		: AudioIODeviceType ("ASIO"),
		  hasScanned (false)
	{
		CoInitialize (0);
	}

	void scanForDevices()
	{
		hasScanned = true;

		deviceNames.clear();
		classIds.clear();

		HKEY hk = 0;
		int index = 0;

		if (RegOpenKey (HKEY_LOCAL_MACHINE, _T("software\\asio"), &hk) == ERROR_SUCCESS)
		{
			TCHAR name [256];

			while (RegEnumKey (hk, index++, name, numElementsInArray (name)) == ERROR_SUCCESS)
				addDriverInfo (name, hk);

			RegCloseKey (hk);
		}
	}

	StringArray getDeviceNames (bool /*wantInputNames*/) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		return deviceNames;
	}

	int getDefaultDeviceIndex (bool) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		for (int i = deviceNames.size(); --i >= 0;)
			if (deviceNames[i].containsIgnoreCase ("asio4all"))
				return i; // asio4all is a safe choice for a default..

	   #if JUCE_DEBUG
		if (deviceNames.size() > 1 && deviceNames[0].containsIgnoreCase ("digidesign"))
			return 1; // (the digi m-box driver crashes the app when you run
					  // it in the debugger, which can be a bit annoying)
	   #endif

		return 0;
	}

	static int findFreeSlot()
	{
		for (int i = 0; i < numElementsInArray (currentASIODev); ++i)
			if (currentASIODev[i] == 0)
				return i;

		jassertfalse;  // unfortunately you can only have a finite number
					   // of ASIO devices open at the same time..
		return -1;
	}

	int getIndexOfDevice (AudioIODevice* d, bool /*asInput*/) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		return d == nullptr ? -1 : deviceNames.indexOf (d->getName());
	}

	bool hasSeparateInputsAndOutputs() const    { return false; }

	AudioIODevice* createDevice (const String& outputDeviceName,
								 const String& inputDeviceName)
	{
		// ASIO can't open two different devices for input and output - they must be the same one.
		jassert (inputDeviceName == outputDeviceName || outputDeviceName.isEmpty() || inputDeviceName.isEmpty());
		jassert (hasScanned); // need to call scanForDevices() before doing this

		const int index = deviceNames.indexOf (outputDeviceName.isNotEmpty() ? outputDeviceName
																			 : inputDeviceName);

		if (index >= 0)
		{
			const int freeSlot = findFreeSlot();

			if (freeSlot >= 0)
				return new ASIOAudioIODevice (outputDeviceName, *(classIds [index]), freeSlot, String::empty);
		}

		return nullptr;
	}

private:
	StringArray deviceNames;
	OwnedArray <CLSID> classIds;

	bool hasScanned;

	static bool checkClassIsOk (const String& classId)
	{
		HKEY hk = 0;
		bool ok = false;

		if (RegOpenKey (HKEY_CLASSES_ROOT, _T("clsid"), &hk) == ERROR_SUCCESS)
		{
			int index = 0;
			TCHAR name [512];

			while (RegEnumKey (hk, index++, name, numElementsInArray (name)) == ERROR_SUCCESS)
			{
				if (classId.equalsIgnoreCase (name))
				{
					HKEY subKey, pathKey;

					if (RegOpenKeyEx (hk, name, 0, KEY_READ, &subKey) == ERROR_SUCCESS)
					{
						if (RegOpenKeyEx (subKey, _T("InprocServer32"), 0, KEY_READ, &pathKey) == ERROR_SUCCESS)
						{
							TCHAR pathName [1024] = { 0 };
							DWORD dtype = REG_SZ;
							DWORD dsize = sizeof (pathName);

							if (RegQueryValueEx (pathKey, 0, 0, &dtype, (LPBYTE) pathName, &dsize) == ERROR_SUCCESS)
								// In older code, this used to check for the existance of the file, but there are situations
								// where our process doesn't have access to it, but where the driver still loads ok..
								ok = (pathName[0] != 0);

							RegCloseKey (pathKey);
						}

						RegCloseKey (subKey);
					}

					break;
				}
			}

			RegCloseKey (hk);
		}

		return ok;
	}

	void addDriverInfo (const String& keyName, HKEY hk)
	{
		HKEY subKey;

		if (RegOpenKeyEx (hk, keyName.toWideCharPointer(), 0, KEY_READ, &subKey) == ERROR_SUCCESS)
		{
			TCHAR buf [256] = { 0 };
			DWORD dtype = REG_SZ;
			DWORD dsize = sizeof (buf);

			if (RegQueryValueEx (subKey, _T("clsid"), 0, &dtype, (LPBYTE) buf, &dsize) == ERROR_SUCCESS)
			{
				if (dsize > 0 && checkClassIsOk (buf))
				{
					CLSID classId;
					if (CLSIDFromString ((LPOLESTR) buf, &classId) == S_OK)
					{
						dtype = REG_SZ;
						dsize = sizeof (buf);
						String deviceName;

						if (RegQueryValueEx (subKey, _T("description"), 0, &dtype, (LPBYTE) buf, &dsize) == ERROR_SUCCESS)
							deviceName = buf;
						else
							deviceName = keyName;

						log ("found " + deviceName);
						deviceNames.add (deviceName);
						classIds.add (new CLSID (classId));
					}
				}

				RegCloseKey (subKey);
			}
		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ASIOAudioIODeviceType);
};

AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_ASIO()
{
	return new ASIOAudioIODeviceType();
}

AudioIODevice* juce_createASIOAudioIODeviceForGUID (const String& name,
													void* guid,
													const String& optionalDllForDirectLoading)
{
	const int freeSlot = ASIOAudioIODeviceType::findFreeSlot();

	if (freeSlot < 0)
		return nullptr;

	return new ASIOAudioIODevice (name, *(CLSID*) guid, freeSlot, optionalDllForDirectLoading);
}

#undef logError
#undef log

/*** End of inlined file: juce_win32_ASIO.cpp ***/


 #endif

 #if JUCE_USE_CDREADER

/*** Start of inlined file: juce_win32_AudioCDReader.cpp ***/
namespace CDReaderHelpers
{

#define FILE_ANY_ACCESS 0
#ifndef FILE_READ_ACCESS
 #define FILE_READ_ACCESS 1
#endif
#ifndef FILE_WRITE_ACCESS
 #define FILE_WRITE_ACCESS 2
#endif

#define METHOD_BUFFERED 0
#define IOCTL_SCSI_BASE 4
#define SCSI_IOCTL_DATA_OUT          0
#define SCSI_IOCTL_DATA_IN           1
#define SCSI_IOCTL_DATA_UNSPECIFIED  2

#define CTL_CODE2(DevType, Function, Method, Access) (((DevType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define IOCTL_SCSI_PASS_THROUGH_DIRECT  CTL_CODE2( IOCTL_SCSI_BASE, 0x0405, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS )
#define IOCTL_SCSI_GET_ADDRESS          CTL_CODE2( IOCTL_SCSI_BASE, 0x0406, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define SENSE_LEN         14
#define SRB_ENABLE_RESIDUAL_COUNT 0x04
#define SRB_DIR_IN        0x08
#define SRB_DIR_OUT       0x10
#define SRB_EVENT_NOTIFY  0x40
#define SC_HA_INQUIRY     0x00
#define SC_GET_DEV_TYPE   0x01
#define SC_EXEC_SCSI_CMD  0x02
#define SS_PENDING        0x00
#define SS_COMP           0x01
#define SS_ERR            0x04

enum
{
	READTYPE_ANY = 0,
	READTYPE_ATAPI1 = 1,
	READTYPE_ATAPI2 = 2,
	READTYPE_READ6 = 3,
	READTYPE_READ10 = 4,
	READTYPE_READ_D8 = 5,
	READTYPE_READ_D4 = 6,
	READTYPE_READ_D4_1 = 7,
	READTYPE_READ10_2 = 8
};

struct SCSI_PASS_THROUGH
{
	USHORT Length;
	UCHAR ScsiStatus;
	UCHAR PathId;
	UCHAR TargetId;
	UCHAR Lun;
	UCHAR CdbLength;
	UCHAR SenseInfoLength;
	UCHAR DataIn;
	ULONG DataTransferLength;
	ULONG TimeOutValue;
	ULONG DataBufferOffset;
	ULONG SenseInfoOffset;
	UCHAR Cdb[16];
};

struct SCSI_PASS_THROUGH_DIRECT
{
	USHORT Length;
	UCHAR ScsiStatus;
	UCHAR PathId;
	UCHAR TargetId;
	UCHAR Lun;
	UCHAR CdbLength;
	UCHAR SenseInfoLength;
	UCHAR DataIn;
	ULONG DataTransferLength;
	ULONG TimeOutValue;
	PVOID DataBuffer;
	ULONG SenseInfoOffset;
	UCHAR Cdb[16];
};

struct SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER
{
	SCSI_PASS_THROUGH_DIRECT spt;
	ULONG Filler;
	UCHAR ucSenseBuf[32];
};

struct SCSI_ADDRESS
{
	ULONG Length;
	UCHAR PortNumber;
	UCHAR PathId;
	UCHAR TargetId;
	UCHAR Lun;
};

#pragma pack(1)

struct SRB_GDEVBlock
{
	BYTE SRB_Cmd;
	BYTE SRB_Status;
	BYTE SRB_HaID;
	BYTE SRB_Flags;
	DWORD SRB_Hdr_Rsvd;
	BYTE SRB_Target;
	BYTE SRB_Lun;
	BYTE SRB_DeviceType;
	BYTE SRB_Rsvd1;
	BYTE pad[68];
};

struct SRB_ExecSCSICmd
{
	BYTE SRB_Cmd;
	BYTE SRB_Status;
	BYTE SRB_HaID;
	BYTE SRB_Flags;
	DWORD SRB_Hdr_Rsvd;
	BYTE SRB_Target;
	BYTE SRB_Lun;
	WORD SRB_Rsvd1;
	DWORD SRB_BufLen;
	BYTE *SRB_BufPointer;
	BYTE SRB_SenseLen;
	BYTE SRB_CDBLen;
	BYTE SRB_HaStat;
	BYTE SRB_TargStat;
	VOID *SRB_PostProc;
	BYTE SRB_Rsvd2[20];
	BYTE CDBByte[16];
	BYTE SenseArea[SENSE_LEN + 2];
};

struct SRB
{
	BYTE SRB_Cmd;
	BYTE SRB_Status;
	BYTE SRB_HaId;
	BYTE SRB_Flags;
	DWORD SRB_Hdr_Rsvd;
};

struct TOCTRACK
{
	BYTE rsvd;
	BYTE ADR;
	BYTE trackNumber;
	BYTE rsvd2;
	BYTE addr[4];
};

struct TOC
{
	WORD tocLen;
	BYTE firstTrack;
	BYTE lastTrack;
	TOCTRACK tracks[100];
};

#pragma pack()

struct CDDeviceDescription
{
	CDDeviceDescription()  : ha (0), tgt (0), lun (0), scsiDriveLetter (0)
	{
	}

	void createDescription (const char* data)
	{
		description << String (data + 8, 8).trim() // vendor
					<< ' ' << String (data + 16, 16).trim() // product id
					<< ' ' << String (data + 32, 4).trim(); // rev
	}

	String description;
	BYTE ha, tgt, lun;
	char scsiDriveLetter; // will be 0 if not using scsi
};

class CDReadBuffer
{
public:
	CDReadBuffer (const int numberOfFrames)
		: startFrame (0), numFrames (0), dataStartOffset (0),
		  dataLength (0), bufferSize (2352 * numberOfFrames), index (0),
		  buffer (bufferSize), wantsIndex (false)
	{
	}

	bool isZero() const noexcept
	{
		for (int i = 0; i < dataLength; ++i)
			if (buffer [dataStartOffset + i] != 0)
				return false;

		return true;
	}

	int startFrame, numFrames, dataStartOffset;
	int dataLength, bufferSize, index;
	HeapBlock<BYTE> buffer;
	bool wantsIndex;
};

class CDDeviceHandle;

class CDController
{
public:
	CDController() : initialised (false) {}
	virtual ~CDController() {}

	virtual bool read (CDReadBuffer&) = 0;
	virtual void shutDown() {}

	bool readAudio (CDReadBuffer& rb, CDReadBuffer* overlapBuffer = 0);
	int getLastIndex();

public:
	CDDeviceHandle* deviceInfo;
	int framesToCheck, framesOverlap;
	bool initialised;

	void prepare (SRB_ExecSCSICmd& s);
	void perform (SRB_ExecSCSICmd& s);
	void setPaused (bool paused);
};

class CDDeviceHandle
{
public:
	CDDeviceHandle (const CDDeviceDescription& device, HANDLE scsiHandle_)
		: info (device), scsiHandle (scsiHandle_), readType (READTYPE_ANY)
	{
	}

	~CDDeviceHandle()
	{
		if (controller != nullptr)
		{
			controller->shutDown();
			controller = 0;
		}

		if (scsiHandle != 0)
			CloseHandle (scsiHandle);
	}

	bool readTOC (TOC* lpToc);
	bool readAudio (CDReadBuffer& buffer, CDReadBuffer* overlapBuffer = 0);
	void openDrawer (bool shouldBeOpen);
	void performScsiCommand (HANDLE event, SRB_ExecSCSICmd& s);

	CDDeviceDescription info;
	HANDLE scsiHandle;
	BYTE readType;

private:
	ScopedPointer<CDController> controller;

	bool testController (int readType, CDController* newController, CDReadBuffer& bufferToUse);
};

HANDLE createSCSIDeviceHandle (const char driveLetter)
{
	TCHAR devicePath[] = { '\\', '\\', '.', '\\', driveLetter, ':', 0, 0 };
	DWORD flags = GENERIC_READ | GENERIC_WRITE;
	HANDLE h = CreateFile (devicePath, flags, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (h == INVALID_HANDLE_VALUE)
	{
		flags ^= GENERIC_WRITE;
		h = CreateFile (devicePath, flags, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	}

	return h;
}

void findCDDevices (Array<CDDeviceDescription>& list)
{
	for (char driveLetter = 'b'; driveLetter <= 'z'; ++driveLetter)
	{
		TCHAR drivePath[] = { driveLetter, ':', '\\', 0, 0 };

		if (GetDriveType (drivePath) == DRIVE_CDROM)
		{
			HANDLE h = createSCSIDeviceHandle (driveLetter);

			if (h != INVALID_HANDLE_VALUE)
			{
				char buffer[100] = { 0 };

				SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER p = { 0 };
				p.spt.Length             = sizeof (SCSI_PASS_THROUGH);
				p.spt.CdbLength          = 6;
				p.spt.SenseInfoLength    = 24;
				p.spt.DataIn             = SCSI_IOCTL_DATA_IN;
				p.spt.DataTransferLength = sizeof (buffer);
				p.spt.TimeOutValue       = 2;
				p.spt.DataBuffer         = buffer;
				p.spt.SenseInfoOffset    = offsetof (SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, ucSenseBuf);
				p.spt.Cdb[0]             = 0x12;
				p.spt.Cdb[4]             = 100;

				DWORD bytesReturned = 0;

				if (DeviceIoControl (h, IOCTL_SCSI_PASS_THROUGH_DIRECT,
									 &p, sizeof (p), &p, sizeof (p),
									 &bytesReturned, 0) != 0)
				{
					CDDeviceDescription dev;
					dev.scsiDriveLetter = driveLetter;
					dev.createDescription (buffer);

					SCSI_ADDRESS scsiAddr = { 0 };
					scsiAddr.Length = sizeof (scsiAddr);

					if (DeviceIoControl (h, IOCTL_SCSI_GET_ADDRESS,
										 0, 0, &scsiAddr, sizeof (scsiAddr),
										 &bytesReturned, 0) != 0)
					{
						dev.ha = scsiAddr.PortNumber;
						dev.tgt = scsiAddr.TargetId;
						dev.lun = scsiAddr.Lun;
						list.add (dev);
					}
				}

				CloseHandle (h);
			}
		}
	}
}

DWORD performScsiPassThroughCommand (SRB_ExecSCSICmd* const srb, const char driveLetter,
									 HANDLE& deviceHandle, const bool retryOnFailure)
{
	SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER s = { 0 };
	s.spt.Length = sizeof (SCSI_PASS_THROUGH);
	s.spt.CdbLength = srb->SRB_CDBLen;

	s.spt.DataIn = (BYTE) ((srb->SRB_Flags & SRB_DIR_IN)
							? SCSI_IOCTL_DATA_IN
							: ((srb->SRB_Flags & SRB_DIR_OUT)
								? SCSI_IOCTL_DATA_OUT
								: SCSI_IOCTL_DATA_UNSPECIFIED));

	s.spt.DataTransferLength = srb->SRB_BufLen;
	s.spt.TimeOutValue = 5;
	s.spt.DataBuffer = srb->SRB_BufPointer;
	s.spt.SenseInfoOffset = offsetof (SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, ucSenseBuf);

	memcpy (s.spt.Cdb, srb->CDBByte, srb->SRB_CDBLen);

	srb->SRB_Status = SS_ERR;
	srb->SRB_TargStat = 0x0004;

	DWORD bytesReturned = 0;

	if (DeviceIoControl (deviceHandle, IOCTL_SCSI_PASS_THROUGH_DIRECT,
						 &s, sizeof (s), &s, sizeof (s), &bytesReturned, 0) != 0)
	{
		srb->SRB_Status = SS_COMP;
	}
	else if (retryOnFailure)
	{
		const DWORD error = GetLastError();

		if ((error == ERROR_MEDIA_CHANGED) || (error == ERROR_INVALID_HANDLE))
		{
			if (error != ERROR_INVALID_HANDLE)
				CloseHandle (deviceHandle);

			deviceHandle = createSCSIDeviceHandle (driveLetter);

			return performScsiPassThroughCommand (srb, driveLetter, deviceHandle, false);
		}
	}

	return srb->SRB_Status;
}

// Controller types..

class ControllerType1  : public CDController
{
public:
	ControllerType1() {}

	bool read (CDReadBuffer& rb)
	{
		if (rb.numFrames * 2352 > rb.bufferSize)
			return false;

		SRB_ExecSCSICmd s;
		prepare (s);
		s.SRB_Flags = SRB_DIR_IN | SRB_EVENT_NOTIFY;
		s.SRB_BufLen = rb.bufferSize;
		s.SRB_BufPointer = rb.buffer;
		s.SRB_CDBLen = 12;
		s.CDBByte[0] = 0xBE;
		s.CDBByte[3] = (BYTE) ((rb.startFrame >> 16) & 0xFF);
		s.CDBByte[4] = (BYTE) ((rb.startFrame >> 8) & 0xFF);
		s.CDBByte[5] = (BYTE) (rb.startFrame & 0xFF);
		s.CDBByte[8] = (BYTE) (rb.numFrames & 0xFF);
		s.CDBByte[9] = (BYTE) (deviceInfo->readType == READTYPE_ATAPI1 ? 0x10 : 0xF0);
		perform (s);

		if (s.SRB_Status != SS_COMP)
			return false;

		rb.dataLength = rb.numFrames * 2352;
		rb.dataStartOffset = 0;
		return true;
	}
};

class ControllerType2  : public CDController
{
public:
	ControllerType2() {}

	void shutDown()
	{
		if (initialised)
		{
			BYTE bufPointer[] = { 0, 0, 0, 8, 83, 0, 0, 0, 0, 0, 8, 0 };

			SRB_ExecSCSICmd s;
			prepare (s);
			s.SRB_Flags = SRB_EVENT_NOTIFY | SRB_ENABLE_RESIDUAL_COUNT;
			s.SRB_BufLen = 0x0C;
			s.SRB_BufPointer = bufPointer;
			s.SRB_CDBLen = 6;
			s.CDBByte[0] = 0x15;
			s.CDBByte[4] = 0x0C;
			perform (s);
		}
	}

	bool init()
	{
		SRB_ExecSCSICmd s;
		s.SRB_Status = SS_ERR;

		if (deviceInfo->readType == READTYPE_READ10_2)
		{
			BYTE bufPointer1[] = { 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 9, 48, 35, 6, 0, 0, 0, 0, 0, 128 };
			BYTE bufPointer2[] = { 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 9, 48, 1, 6, 32, 7, 0, 0, 0, 0 };

			for (int i = 0; i < 2; ++i)
			{
				prepare (s);
				s.SRB_Flags = SRB_EVENT_NOTIFY;
				s.SRB_BufLen = 0x14;
				s.SRB_BufPointer = (i == 0) ? bufPointer1 : bufPointer2;
				s.SRB_CDBLen = 6;
				s.CDBByte[0] = 0x15;
				s.CDBByte[1] = 0x10;
				s.CDBByte[4] = 0x14;
				perform (s);

				if (s.SRB_Status != SS_COMP)
					return false;
			}
		}
		else
		{
			BYTE bufPointer[] = { 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 9, 48 };

			prepare (s);
			s.SRB_Flags = SRB_EVENT_NOTIFY;
			s.SRB_BufLen = 0x0C;
			s.SRB_BufPointer = bufPointer;
			s.SRB_CDBLen = 6;
			s.CDBByte[0] = 0x15;
			s.CDBByte[4] = 0x0C;
			perform (s);
		}

		return s.SRB_Status == SS_COMP;
	}

	bool read (CDReadBuffer& rb)
	{
		if (rb.numFrames * 2352 > rb.bufferSize)
			return false;

		if (! initialised)
		{
			initialised = init();

			if (! initialised)
				return false;
		}

		SRB_ExecSCSICmd s;
		prepare (s);
		s.SRB_Flags = SRB_DIR_IN | SRB_EVENT_NOTIFY;
		s.SRB_BufLen = rb.bufferSize;
		s.SRB_BufPointer = rb.buffer;
		s.SRB_CDBLen = 10;
		s.CDBByte[0] = 0x28;
		s.CDBByte[1] = (BYTE) (deviceInfo->info.lun << 5);
		s.CDBByte[3] = (BYTE) ((rb.startFrame >> 16) & 0xFF);
		s.CDBByte[4] = (BYTE) ((rb.startFrame >> 8) & 0xFF);
		s.CDBByte[5] = (BYTE) (rb.startFrame & 0xFF);
		s.CDBByte[8] = (BYTE) (rb.numFrames & 0xFF);
		perform (s);

		if (s.SRB_Status != SS_COMP)
			return false;

		rb.dataLength = rb.numFrames * 2352;
		rb.dataStartOffset = 0;
		return true;
	}
};

class ControllerType3  : public CDController
{
public:
	ControllerType3() {}

	bool read (CDReadBuffer& rb)
	{
		if (rb.numFrames * 2352 > rb.bufferSize)
			return false;

		if (! initialised)
		{
			setPaused (false);
			initialised = true;
		}

		SRB_ExecSCSICmd s;
		prepare (s);
		s.SRB_Flags = SRB_DIR_IN | SRB_EVENT_NOTIFY;
		s.SRB_BufLen = rb.numFrames * 2352;
		s.SRB_BufPointer = rb.buffer;
		s.SRB_CDBLen = 12;
		s.CDBByte[0] = 0xD8;
		s.CDBByte[3] = (BYTE) ((rb.startFrame >> 16) & 0xFF);
		s.CDBByte[4] = (BYTE) ((rb.startFrame >> 8) & 0xFF);
		s.CDBByte[5] = (BYTE) (rb.startFrame & 0xFF);
		s.CDBByte[9] = (BYTE) (rb.numFrames & 0xFF);
		perform (s);

		if (s.SRB_Status != SS_COMP)
			return false;

		rb.dataLength = rb.numFrames * 2352;
		rb.dataStartOffset = 0;
		return true;
	}
};

class ControllerType4  : public CDController
{
public:
	ControllerType4() {}

	bool selectD4Mode()
	{
		BYTE bufPointer[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 48 };

		SRB_ExecSCSICmd s;
		prepare (s);
		s.SRB_Flags = SRB_EVENT_NOTIFY;
		s.SRB_CDBLen = 6;
		s.SRB_BufLen = 12;
		s.SRB_BufPointer = bufPointer;
		s.CDBByte[0] = 0x15;
		s.CDBByte[1] = 0x10;
		s.CDBByte[4] = 0x08;
		perform (s);

		return s.SRB_Status == SS_COMP;
	}

	bool read (CDReadBuffer& rb)
	{
		if (rb.numFrames * 2352 > rb.bufferSize)
			return false;

		if (! initialised)
		{
			setPaused (true);

			if (deviceInfo->readType == READTYPE_READ_D4_1)
				selectD4Mode();

			initialised = true;
		}

		SRB_ExecSCSICmd s;
		prepare (s);
		s.SRB_Flags = SRB_DIR_IN | SRB_EVENT_NOTIFY;
		s.SRB_BufLen = rb.bufferSize;
		s.SRB_BufPointer = rb.buffer;
		s.SRB_CDBLen = 10;
		s.CDBByte[0] = 0xD4;
		s.CDBByte[3] = (BYTE) ((rb.startFrame >> 16) & 0xFF);
		s.CDBByte[4] = (BYTE) ((rb.startFrame >> 8) & 0xFF);
		s.CDBByte[5] = (BYTE) (rb.startFrame & 0xFF);
		s.CDBByte[8] = (BYTE) (rb.numFrames & 0xFF);
		perform (s);

		if (s.SRB_Status != SS_COMP)
			return false;

		rb.dataLength = rb.numFrames * 2352;
		rb.dataStartOffset = 0;
		return true;
	}
};

void CDController::prepare (SRB_ExecSCSICmd& s)
{
	zerostruct (s);
	s.SRB_Cmd = SC_EXEC_SCSI_CMD;
	s.SRB_HaID = deviceInfo->info.ha;
	s.SRB_Target = deviceInfo->info.tgt;
	s.SRB_Lun = deviceInfo->info.lun;
	s.SRB_SenseLen = SENSE_LEN;
}

void CDController::perform (SRB_ExecSCSICmd& s)
{
	s.SRB_PostProc = CreateEvent (0, TRUE, FALSE, 0);

	deviceInfo->performScsiCommand (s.SRB_PostProc, s);
}

void CDController::setPaused (bool paused)
{
	SRB_ExecSCSICmd s;
	prepare (s);
	s.SRB_Flags = SRB_EVENT_NOTIFY;
	s.SRB_CDBLen = 10;
	s.CDBByte[0] = 0x4B;
	s.CDBByte[8] = (BYTE) (paused ? 0 : 1);
	perform (s);
}

bool CDController::readAudio (CDReadBuffer& rb, CDReadBuffer* overlapBuffer)
{
	if (overlapBuffer != nullptr)
	{
		const bool canDoJitter = (overlapBuffer->bufferSize >= 2352 * framesToCheck);
		const bool doJitter = canDoJitter && ! overlapBuffer->isZero();

		if (doJitter
			 && overlapBuffer->startFrame > 0
			 && overlapBuffer->numFrames > 0
			 && overlapBuffer->dataLength > 0)
		{
			const int numFrames = rb.numFrames;

			if (overlapBuffer->startFrame == (rb.startFrame - framesToCheck))
			{
				rb.startFrame -= framesOverlap;

				if (framesToCheck < framesOverlap
					 && numFrames + framesOverlap <= rb.bufferSize / 2352)
					rb.numFrames += framesOverlap;
			}
			else
			{
				overlapBuffer->dataLength = 0;
				overlapBuffer->startFrame = 0;
				overlapBuffer->numFrames = 0;
			}
		}

		if (! read (rb))
			return false;

		if (doJitter)
		{
			const int checkLen = framesToCheck * 2352;
			const int maxToCheck = rb.dataLength - checkLen;

			if (overlapBuffer->dataLength == 0 || overlapBuffer->isZero())
				return true;

			BYTE* const p = overlapBuffer->buffer + overlapBuffer->dataStartOffset;
			bool found = false;

			for (int i = 0; i < maxToCheck; ++i)
			{
				if (memcmp (p, rb.buffer + i, checkLen) == 0)
				{
					i += checkLen;
					rb.dataStartOffset = i;
					rb.dataLength -= i;
					rb.startFrame = overlapBuffer->startFrame + framesToCheck;
					found = true;
					break;
				}
			}

			rb.numFrames = rb.dataLength / 2352;
			rb.dataLength = 2352 * rb.numFrames;

			if (! found)
				return false;
		}

		if (canDoJitter)
		{
			memcpy (overlapBuffer->buffer,
					rb.buffer + rb.dataStartOffset + 2352 * (rb.numFrames - framesToCheck),
					2352 * framesToCheck);

			overlapBuffer->startFrame = rb.startFrame + rb.numFrames - framesToCheck;
			overlapBuffer->numFrames = framesToCheck;
			overlapBuffer->dataLength = 2352 * framesToCheck;
			overlapBuffer->dataStartOffset = 0;
		}
		else
		{
			overlapBuffer->startFrame = 0;
			overlapBuffer->numFrames = 0;
			overlapBuffer->dataLength = 0;
		}

		return true;
	}

	return read (rb);
}

int CDController::getLastIndex()
{
	char qdata[100];

	SRB_ExecSCSICmd s;
	prepare (s);
	s.SRB_Flags = SRB_DIR_IN | SRB_EVENT_NOTIFY;
	s.SRB_BufLen = sizeof (qdata);
	s.SRB_BufPointer = (BYTE*) qdata;
	s.SRB_CDBLen = 12;
	s.CDBByte[0] = 0x42;
	s.CDBByte[1] = (BYTE) (deviceInfo->info.lun << 5);
	s.CDBByte[2] = 64;
	s.CDBByte[3] = 1; // get current position
	s.CDBByte[7] = 0;
	s.CDBByte[8] = (BYTE) sizeof (qdata);
	perform (s);

	return s.SRB_Status == SS_COMP ? qdata[7] : 0;
}

bool CDDeviceHandle::readTOC (TOC* lpToc)
{
	SRB_ExecSCSICmd s = { 0 };
	s.SRB_Cmd = SC_EXEC_SCSI_CMD;
	s.SRB_HaID = info.ha;
	s.SRB_Target = info.tgt;
	s.SRB_Lun = info.lun;
	s.SRB_Flags = SRB_DIR_IN | SRB_EVENT_NOTIFY;
	s.SRB_BufLen = 0x324;
	s.SRB_BufPointer = (BYTE*) lpToc;
	s.SRB_SenseLen = 0x0E;
	s.SRB_CDBLen = 0x0A;
	s.SRB_PostProc = CreateEvent (0, TRUE, FALSE, 0);
	s.CDBByte[0] = 0x43;
	s.CDBByte[1] = 0x00;
	s.CDBByte[7] = 0x03;
	s.CDBByte[8] = 0x24;

	performScsiCommand (s.SRB_PostProc, s);
	return (s.SRB_Status == SS_COMP);
}

void CDDeviceHandle::performScsiCommand (HANDLE event, SRB_ExecSCSICmd& s)
{
	ResetEvent (event);
	DWORD status = performScsiPassThroughCommand ((SRB_ExecSCSICmd*) &s, info.scsiDriveLetter, scsiHandle, true);

	if (status == SS_PENDING)
		WaitForSingleObject (event, 4000);

	CloseHandle (event);
}

bool CDDeviceHandle::readAudio (CDReadBuffer& buffer, CDReadBuffer* overlapBuffer)
{
	if (controller == 0)
	{
		   testController (READTYPE_ATAPI2,    new ControllerType1(), buffer)
		|| testController (READTYPE_ATAPI1,    new ControllerType1(), buffer)
		|| testController (READTYPE_READ10_2,  new ControllerType2(), buffer)
		|| testController (READTYPE_READ10,    new ControllerType2(), buffer)
		|| testController (READTYPE_READ_D8,   new ControllerType3(), buffer)
		|| testController (READTYPE_READ_D4,   new ControllerType4(), buffer)
		|| testController (READTYPE_READ_D4_1, new ControllerType4(), buffer);
	}

	buffer.index = 0;

	if (controller != nullptr && controller->readAudio (buffer, overlapBuffer))
	{
		if (buffer.wantsIndex)
			buffer.index = controller->getLastIndex();

		return true;
	}

	return false;
}

void CDDeviceHandle::openDrawer (bool shouldBeOpen)
{
	if (shouldBeOpen)
	{
		if (controller != nullptr)
		{
			controller->shutDown();
			controller = nullptr;
		}

		if (scsiHandle != 0)
		{
			CloseHandle (scsiHandle);
			scsiHandle = 0;
		}
	}

	SRB_ExecSCSICmd s = { 0 };
	s.SRB_Cmd = SC_EXEC_SCSI_CMD;
	s.SRB_HaID = info.ha;
	s.SRB_Target = info.tgt;
	s.SRB_Lun = info.lun;
	s.SRB_SenseLen = SENSE_LEN;
	s.SRB_Flags = SRB_DIR_IN | SRB_EVENT_NOTIFY;
	s.SRB_BufLen = 0;
	s.SRB_BufPointer = 0;
	s.SRB_CDBLen = 12;
	s.CDBByte[0] = 0x1b;
	s.CDBByte[1] = (BYTE) (info.lun << 5);
	s.CDBByte[4] = (BYTE) (shouldBeOpen ? 2 : 3);
	s.SRB_PostProc = CreateEvent (0, TRUE, FALSE, 0);

	performScsiCommand (s.SRB_PostProc, s);
}

bool CDDeviceHandle::testController (const int type, CDController* const newController, CDReadBuffer& rb)
{
	controller = newController;
	readType = (BYTE) type;

	controller->deviceInfo = this;
	controller->framesToCheck = 1;
	controller->framesOverlap = 3;

	bool passed = false;
	memset (rb.buffer, 0xcd, rb.bufferSize);

	if (controller->read (rb))
	{
		passed = true;
		int* p = (int*) (rb.buffer + rb.dataStartOffset);
		int wrong = 0;

		for (int i = rb.dataLength / 4; --i >= 0;)
		{
			if (*p++ == (int) 0xcdcdcdcd)
			{
				if (++wrong == 4)
				{
					passed = false;
					break;
				}
			}
			else
			{
				wrong = 0;
			}
		}
	}

	if (! passed)
	{
		controller->shutDown();
		controller = nullptr;
	}

	return passed;
}

struct CDDeviceWrapper
{
	CDDeviceWrapper (const CDDeviceDescription& device, HANDLE scsiHandle)
		: deviceHandle (device, scsiHandle), overlapBuffer (3), jitter (false)
	{
		// xxx jitter never seemed to actually be enabled (??)
	}

	CDDeviceHandle deviceHandle;
	CDReadBuffer overlapBuffer;
	bool jitter;
};

int getAddressOfTrack (const TOCTRACK& t) noexcept
{
	return (((DWORD) t.addr[0]) << 24) + (((DWORD) t.addr[1]) << 16)
			+ (((DWORD) t.addr[2]) << 8) + ((DWORD) t.addr[3]);
}

const int samplesPerFrame = 44100 / 75;
const int bytesPerFrame = samplesPerFrame * 4;
const int framesPerIndexRead = 4;

}

StringArray AudioCDReader::getAvailableCDNames()
{
	using namespace CDReaderHelpers;
	StringArray results;

	Array<CDDeviceDescription> list;
	findCDDevices (list);

	for (int i = 0; i < list.size(); ++i)
	{
		String s;
		if (list[i].scsiDriveLetter > 0)
			s << String::charToString (list[i].scsiDriveLetter).toUpperCase() << ": ";

		s << list[i].description;
		results.add (s);
	}

	return results;
}

AudioCDReader* AudioCDReader::createReaderForCD (const int deviceIndex)
{
	using namespace CDReaderHelpers;

	Array<CDDeviceDescription> list;
	findCDDevices (list);

	if (isPositiveAndBelow (deviceIndex, list.size()))
	{
		HANDLE h = createSCSIDeviceHandle (list [deviceIndex].scsiDriveLetter);

		if (h != INVALID_HANDLE_VALUE)
		{
			ScopedPointer<AudioCDReader> cd (new AudioCDReader (new CDDeviceWrapper (list [deviceIndex], h)));

			if (cd->lengthInSamples > 0)
				return cd.release();
		}
	}

	return nullptr;
}

AudioCDReader::AudioCDReader (void* handle_)
	: AudioFormatReader (0, "CD Audio"),
	  handle (handle_),
	  indexingEnabled (false),
	  lastIndex (0),
	  firstFrameInBuffer (0),
	  samplesInBuffer (0)
{
	using namespace CDReaderHelpers;
	jassert (handle_ != nullptr);

	refreshTrackLengths();

	sampleRate = 44100.0;
	bitsPerSample = 16;
	numChannels = 2;
	usesFloatingPointData = false;

	buffer.setSize (4 * bytesPerFrame, true);
}

AudioCDReader::~AudioCDReader()
{
	using namespace CDReaderHelpers;
	CDDeviceWrapper* const device = static_cast <CDDeviceWrapper*> (handle);
	delete device;
}

bool AudioCDReader::readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
								 int64 startSampleInFile, int numSamples)
{
	using namespace CDReaderHelpers;
	CDDeviceWrapper* const device = static_cast <CDDeviceWrapper*> (handle);

	bool ok = true;

	while (numSamples > 0)
	{
		const int bufferStartSample = firstFrameInBuffer * samplesPerFrame;
		const int bufferEndSample = bufferStartSample + samplesInBuffer;

		if (startSampleInFile >= bufferStartSample
			 && startSampleInFile < bufferEndSample)
		{
			const int toDo = (int) jmin ((int64) numSamples, bufferEndSample - startSampleInFile);

			int* const l = destSamples[0] + startOffsetInDestBuffer;
			int* const r = numDestChannels > 1 ? (destSamples[1] + startOffsetInDestBuffer) : nullptr;
			const short* src = (const short*) buffer.getData();
			src += 2 * (startSampleInFile - bufferStartSample);

			for (int i = 0; i < toDo; ++i)
			{
				l[i] = src [i << 1] << 16;

				if (r != nullptr)
					r[i] = src [(i << 1) + 1] << 16;
			}

			startOffsetInDestBuffer += toDo;
			startSampleInFile += toDo;
			numSamples -= toDo;
		}
		else
		{
			const int framesInBuffer = buffer.getSize() / bytesPerFrame;
			const int frameNeeded = (int) (startSampleInFile / samplesPerFrame);

			if (firstFrameInBuffer + framesInBuffer != frameNeeded)
			{
				device->overlapBuffer.dataLength = 0;
				device->overlapBuffer.startFrame = 0;
				device->overlapBuffer.numFrames = 0;
				device->jitter = false;
			}

			firstFrameInBuffer = frameNeeded;
			lastIndex = 0;

			CDReadBuffer readBuffer (framesInBuffer + 4);
			readBuffer.wantsIndex = indexingEnabled;

			int i;
			for (i = 5; --i >= 0;)
			{
				readBuffer.startFrame = frameNeeded;
				readBuffer.numFrames = framesInBuffer;

				if (device->deviceHandle.readAudio (readBuffer, device->jitter ? &device->overlapBuffer : 0))
					break;
				else
					device->overlapBuffer.dataLength = 0;
			}

			if (i >= 0)
			{
				buffer.copyFrom (readBuffer.buffer + readBuffer.dataStartOffset, 0, readBuffer.dataLength);
				samplesInBuffer = readBuffer.dataLength >> 2;
				lastIndex = readBuffer.index;
			}
			else
			{
				int* l = destSamples[0] + startOffsetInDestBuffer;
				int* r = numDestChannels > 1 ? (destSamples[1] + startOffsetInDestBuffer) : nullptr;

				while (--numSamples >= 0)
				{
					*l++ = 0;

					if (r != nullptr)
						*r++ = 0;
				}

				// sometimes the read fails for just the very last couple of blocks, so
				// we'll ignore and errors in the last half-second of the disk..
				ok = startSampleInFile > (trackStartSamples [getNumTracks()] - 20000);
				break;
			}
		}
	}

	return ok;
}

bool AudioCDReader::isCDStillPresent() const
{
	using namespace CDReaderHelpers;
	TOC toc = { 0 };
	return static_cast <CDDeviceWrapper*> (handle)->deviceHandle.readTOC (&toc);
}

void AudioCDReader::refreshTrackLengths()
{
	using namespace CDReaderHelpers;
	trackStartSamples.clear();
	zeromem (audioTracks, sizeof (audioTracks));

	TOC toc = { 0 };

	if (static_cast <CDDeviceWrapper*> (handle)->deviceHandle.readTOC (&toc))
	{
		int numTracks = 1 + toc.lastTrack - toc.firstTrack;

		for (int i = 0; i <= numTracks; ++i)
		{
			trackStartSamples.add (samplesPerFrame * getAddressOfTrack (toc.tracks [i]));
			audioTracks [i] = ((toc.tracks[i].ADR & 4) == 0);
		}
	}

	lengthInSamples = getPositionOfTrackStart (getNumTracks());
}

bool AudioCDReader::isTrackAudio (int trackNum) const
{
	return trackNum >= 0 && trackNum < getNumTracks() && audioTracks [trackNum];
}

void AudioCDReader::enableIndexScanning (bool b)
{
	indexingEnabled = b;
}

int AudioCDReader::getLastIndex() const
{
	return lastIndex;
}

int AudioCDReader::getIndexAt (int samplePos)
{
	using namespace CDReaderHelpers;
	CDDeviceWrapper* const device = static_cast <CDDeviceWrapper*> (handle);

	const int frameNeeded = samplePos / samplesPerFrame;

	device->overlapBuffer.dataLength = 0;
	device->overlapBuffer.startFrame = 0;
	device->overlapBuffer.numFrames = 0;
	device->jitter = false;

	firstFrameInBuffer = 0;
	lastIndex = 0;

	CDReadBuffer readBuffer (4 + framesPerIndexRead);
	readBuffer.wantsIndex = true;

	int i;
	for (i = 5; --i >= 0;)
	{
		readBuffer.startFrame = frameNeeded;
		readBuffer.numFrames = framesPerIndexRead;

		if (device->deviceHandle.readAudio (readBuffer))
			break;
	}

	if (i >= 0)
		return readBuffer.index;

	return -1;
}

Array<int> AudioCDReader::findIndexesInTrack (const int trackNumber)
{
	using namespace CDReaderHelpers;
	Array <int> indexes;

	const int trackStart = getPositionOfTrackStart (trackNumber);
	const int trackEnd = getPositionOfTrackStart (trackNumber + 1);

	bool needToScan = true;

	if (trackEnd - trackStart > 20 * 44100)
	{
		// check the end of the track for indexes before scanning the whole thing
		needToScan = false;
		int pos = jmax (trackStart, trackEnd - 44100 * 5);
		bool seenAnIndex = false;

		while (pos <= trackEnd - samplesPerFrame)
		{
			const int index = getIndexAt (pos);

			if (index == 0)
			{
				// lead-out, so skip back a bit if we've not found any indexes yet..
				if (seenAnIndex)
					break;

				pos -= 44100 * 5;

				if (pos < trackStart)
					break;
			}
			else
			{
				if (index > 0)
					seenAnIndex = true;

				if (index > 1)
				{
					needToScan = true;
					break;
				}

				pos += samplesPerFrame * framesPerIndexRead;
			}
		}
	}

	if (needToScan)
	{
		CDDeviceWrapper* const device = static_cast <CDDeviceWrapper*> (handle);

		int pos = trackStart;
		int last = -1;

		while (pos < trackEnd - samplesPerFrame * 10)
		{
			const int frameNeeded = pos / samplesPerFrame;

			device->overlapBuffer.dataLength = 0;
			device->overlapBuffer.startFrame = 0;
			device->overlapBuffer.numFrames = 0;
			device->jitter = false;

			firstFrameInBuffer = 0;

			CDReadBuffer readBuffer (4);
			readBuffer.wantsIndex = true;

			int i;
			for (i = 5; --i >= 0;)
			{
				readBuffer.startFrame = frameNeeded;
				readBuffer.numFrames = framesPerIndexRead;

				if (device->deviceHandle.readAudio (readBuffer))
					break;
			}

			if (i < 0)
				break;

			if (readBuffer.index > last && readBuffer.index > 1)
			{
				last = readBuffer.index;
				indexes.add (pos);
			}

			pos += samplesPerFrame * framesPerIndexRead;
		}

		indexes.removeValue (trackStart);
	}

	return indexes;
}

void AudioCDReader::ejectDisk()
{
	using namespace CDReaderHelpers;
	static_cast <CDDeviceWrapper*> (handle)->deviceHandle.openDrawer (true);
}

/*** End of inlined file: juce_win32_AudioCDReader.cpp ***/


 #endif

 #if JUCE_USE_CDBURNER

/*** Start of inlined file: juce_win32_AudioCDBurner.cpp ***/
namespace CDBurnerHelpers
{
	IDiscRecorder* enumCDBurners (StringArray* list, int indexToOpen, IDiscMaster** master)
	{
		CoInitialize (0);

		IDiscMaster* dm;
		IDiscRecorder* result = nullptr;

		if (SUCCEEDED (CoCreateInstance (CLSID_MSDiscMasterObj, 0,
										 CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,
										 IID_IDiscMaster,
										 (void**) &dm)))
		{
			if (SUCCEEDED (dm->Open()))
			{
				IEnumDiscRecorders* drEnum = nullptr;

				if (SUCCEEDED (dm->EnumDiscRecorders (&drEnum)))
				{
					IDiscRecorder* dr = nullptr;
					DWORD dummy;
					int index = 0;

					while (drEnum->Next (1, &dr, &dummy) == S_OK)
					{
						if (indexToOpen == index)
						{
							result = dr;
							break;
						}
						else if (list != nullptr)
						{
							BSTR path;

							if (SUCCEEDED (dr->GetPath (&path)))
								list->add ((const WCHAR*) path);
						}

						++index;
						dr->Release();
					}

					drEnum->Release();
				}

				if (master == 0)
					dm->Close();
			}

			if (master != nullptr)
				*master = dm;
			else
				dm->Release();
		}

		return result;
	}
}

class AudioCDBurner::Pimpl  : public ComBaseClassHelper <IDiscMasterProgressEvents>,
							  public Timer
{
public:
	Pimpl (AudioCDBurner& owner_, IDiscMaster* discMaster_, IDiscRecorder* discRecorder_)
	  : owner (owner_), discMaster (discMaster_), discRecorder (discRecorder_), redbook (0),
		listener (0), progress (0), shouldCancel (false)
	{
		HRESULT hr = discMaster->SetActiveDiscMasterFormat (IID_IRedbookDiscMaster, (void**) &redbook);
		jassert (SUCCEEDED (hr));
		hr = discMaster->SetActiveDiscRecorder (discRecorder);
		//jassert (SUCCEEDED (hr));

		lastState = getDiskState();
		startTimer (2000);
	}

	~Pimpl()  {}

	void releaseObjects()
	{
		discRecorder->Close();
		if (redbook != nullptr)
			redbook->Release();
		discRecorder->Release();
		discMaster->Release();
		Release();
	}

	JUCE_COMRESULT QueryCancel (boolean* pbCancel)
	{
		if (listener != nullptr && ! shouldCancel)
			shouldCancel = listener->audioCDBurnProgress (progress);

		*pbCancel = shouldCancel;

		return S_OK;
	}

	JUCE_COMRESULT NotifyBlockProgress (long nCompleted, long nTotal)
	{
		progress = nCompleted / (float) nTotal;
		shouldCancel = listener != nullptr && listener->audioCDBurnProgress (progress);

		return E_NOTIMPL;
	}

	JUCE_COMRESULT NotifyPnPActivity (void)                              { return E_NOTIMPL; }
	JUCE_COMRESULT NotifyAddProgress (long /*nCompletedSteps*/, long /*nTotalSteps*/)    { return E_NOTIMPL; }
	JUCE_COMRESULT NotifyTrackProgress (long /*nCurrentTrack*/, long /*nTotalTracks*/)   { return E_NOTIMPL; }
	JUCE_COMRESULT NotifyPreparingBurn (long /*nEstimatedSeconds*/)      { return E_NOTIMPL; }
	JUCE_COMRESULT NotifyClosingDisc (long /*nEstimatedSeconds*/)        { return E_NOTIMPL; }
	JUCE_COMRESULT NotifyBurnComplete (HRESULT /*status*/)               { return E_NOTIMPL; }
	JUCE_COMRESULT NotifyEraseComplete (HRESULT /*status*/)              { return E_NOTIMPL; }

	class ScopedDiscOpener
	{
	public:
		ScopedDiscOpener (Pimpl& p) : pimpl (p) { pimpl.discRecorder->OpenExclusive(); }
		~ScopedDiscOpener()                     { pimpl.discRecorder->Close(); }

	private:
		Pimpl& pimpl;

		JUCE_DECLARE_NON_COPYABLE (ScopedDiscOpener);
	};

	DiskState getDiskState()
	{
		const ScopedDiscOpener opener (*this);

		long type, flags;
		HRESULT hr = discRecorder->QueryMediaType (&type, &flags);

		if (FAILED (hr))
			return unknown;

		if (type != 0 && (flags & MEDIA_WRITABLE) != 0)
			return writableDiskPresent;

		if (type == 0)
			return noDisc;
		else
			return readOnlyDiskPresent;
	}

	int getIntProperty (const LPOLESTR name, const int defaultReturn) const
	{
		ComSmartPtr<IPropertyStorage> prop;
		if (FAILED (discRecorder->GetRecorderProperties (prop.resetAndGetPointerAddress())))
			return defaultReturn;

		PROPSPEC iPropSpec;
		iPropSpec.ulKind = PRSPEC_LPWSTR;
		iPropSpec.lpwstr = name;

		PROPVARIANT iPropVariant;
		return FAILED (prop->ReadMultiple (1, &iPropSpec, &iPropVariant))
				   ? defaultReturn : (int) iPropVariant.lVal;
	}

	bool setIntProperty (const LPOLESTR name, const int value) const
	{
		ComSmartPtr<IPropertyStorage> prop;
		if (FAILED (discRecorder->GetRecorderProperties (prop.resetAndGetPointerAddress())))
			return false;

		PROPSPEC iPropSpec;
		iPropSpec.ulKind = PRSPEC_LPWSTR;
		iPropSpec.lpwstr = name;

		PROPVARIANT iPropVariant;
		if (FAILED (prop->ReadMultiple (1, &iPropSpec, &iPropVariant)))
			return false;

		iPropVariant.lVal = (long) value;
		return SUCCEEDED (prop->WriteMultiple (1, &iPropSpec, &iPropVariant, iPropVariant.vt))
				&& SUCCEEDED (discRecorder->SetRecorderProperties (prop));
	}

	void timerCallback()
	{
		const DiskState state = getDiskState();

		if (state != lastState)
		{
			lastState = state;
			owner.sendChangeMessage();
		}
	}

	AudioCDBurner& owner;
	DiskState lastState;
	IDiscMaster* discMaster;
	IDiscRecorder* discRecorder;
	IRedbookDiscMaster* redbook;
	AudioCDBurner::BurnProgressListener* listener;
	float progress;
	bool shouldCancel;
};

AudioCDBurner::AudioCDBurner (const int deviceIndex)
{
	IDiscMaster* discMaster = nullptr;
	IDiscRecorder* discRecorder = CDBurnerHelpers::enumCDBurners (0, deviceIndex, &discMaster);

	if (discRecorder != nullptr)
		pimpl = new Pimpl (*this, discMaster, discRecorder);
}

AudioCDBurner::~AudioCDBurner()
{
	if (pimpl != nullptr)
		pimpl.release()->releaseObjects();
}

StringArray AudioCDBurner::findAvailableDevices()
{
	StringArray devs;
	CDBurnerHelpers::enumCDBurners (&devs, -1, 0);
	return devs;
}

AudioCDBurner* AudioCDBurner::openDevice (const int deviceIndex)
{
	ScopedPointer<AudioCDBurner> b (new AudioCDBurner (deviceIndex));

	if (b->pimpl == 0)
		b = nullptr;

	return b.release();
}

AudioCDBurner::DiskState AudioCDBurner::getDiskState() const
{
	return pimpl->getDiskState();
}

bool AudioCDBurner::isDiskPresent() const
{
	return getDiskState() == writableDiskPresent;
}

bool AudioCDBurner::openTray()
{
	const Pimpl::ScopedDiscOpener opener (*pimpl);
	return SUCCEEDED (pimpl->discRecorder->Eject());
}

AudioCDBurner::DiskState AudioCDBurner::waitUntilStateChange (int timeOutMilliseconds)
{
	const int64 timeout = Time::currentTimeMillis() + timeOutMilliseconds;
	DiskState oldState = getDiskState();
	DiskState newState = oldState;

	while (newState == oldState && Time::currentTimeMillis() < timeout)
	{
		newState = getDiskState();
		Thread::sleep (jmin (250, (int) (timeout - Time::currentTimeMillis())));
	}

	return newState;
}

Array<int> AudioCDBurner::getAvailableWriteSpeeds() const
{
	Array<int> results;
	const int maxSpeed = pimpl->getIntProperty (L"MaxWriteSpeed", 1);
	const int speeds[] = { 1, 2, 4, 8, 12, 16, 20, 24, 32, 40, 64, 80 };

	for (int i = 0; i < numElementsInArray (speeds); ++i)
		if (speeds[i] <= maxSpeed)
			results.add (speeds[i]);

	results.addIfNotAlreadyThere (maxSpeed);
	return results;
}

bool AudioCDBurner::setBufferUnderrunProtection (const bool shouldBeEnabled)
{
	if (pimpl->getIntProperty (L"BufferUnderrunFreeCapable", 0) == 0)
		return false;

	pimpl->setIntProperty (L"EnableBufferUnderrunFree", shouldBeEnabled ? -1 : 0);
	return pimpl->getIntProperty (L"EnableBufferUnderrunFree", 0) != 0;
}

int AudioCDBurner::getNumAvailableAudioBlocks() const
{
	long blocksFree = 0;
	pimpl->redbook->GetAvailableAudioTrackBlocks (&blocksFree);
	return blocksFree;
}

String AudioCDBurner::burn (AudioCDBurner::BurnProgressListener* listener, bool ejectDiscAfterwards,
							bool performFakeBurnForTesting, int writeSpeed)
{
	pimpl->setIntProperty (L"WriteSpeed", writeSpeed > 0 ? writeSpeed : -1);

	pimpl->listener = listener;
	pimpl->progress = 0;
	pimpl->shouldCancel = false;

	UINT_PTR cookie;
	HRESULT hr = pimpl->discMaster->ProgressAdvise ((AudioCDBurner::Pimpl*) pimpl, &cookie);

	hr = pimpl->discMaster->RecordDisc (performFakeBurnForTesting,
										ejectDiscAfterwards);

	String error;
	if (hr != S_OK)
	{
		const char* e = "Couldn't open or write to the CD device";

		if (hr == IMAPI_E_USERABORT)
			e = "User cancelled the write operation";
		else if (hr == IMAPI_E_MEDIUM_NOTPRESENT || hr == IMAPI_E_TRACKOPEN)
			e = "No Disk present";

		error = e;
	}

	pimpl->discMaster->ProgressUnadvise (cookie);
	pimpl->listener = 0;

	return error;
}

bool AudioCDBurner::addAudioTrack (AudioSource* audioSource, int numSamples)
{
	if (audioSource == 0)
		return false;

	ScopedPointer<AudioSource> source (audioSource);

	long bytesPerBlock;
	HRESULT hr = pimpl->redbook->GetAudioBlockSize (&bytesPerBlock);

	const int samplesPerBlock = bytesPerBlock / 4;
	bool ok = true;

	hr = pimpl->redbook->CreateAudioTrack ((long) numSamples / (bytesPerBlock * 4));

	HeapBlock <byte> buffer (bytesPerBlock);
	AudioSampleBuffer sourceBuffer (2, samplesPerBlock);
	int samplesDone = 0;

	source->prepareToPlay (samplesPerBlock, 44100.0);

	while (ok)
	{
		{
			AudioSourceChannelInfo info (&sourceBuffer, 0, samplesPerBlock);
			sourceBuffer.clear();

			source->getNextAudioBlock (info);
		}

		buffer.clear (bytesPerBlock);

		typedef AudioData::Pointer <AudioData::Int16, AudioData::LittleEndian,
									AudioData::Interleaved, AudioData::NonConst> CDSampleFormat;

		typedef AudioData::Pointer <AudioData::Float32, AudioData::NativeEndian,
									AudioData::NonInterleaved, AudioData::Const> SourceSampleFormat;

		CDSampleFormat left (buffer, 2);
		left.convertSamples (SourceSampleFormat (sourceBuffer.getSampleData (0)), samplesPerBlock);
		CDSampleFormat right (buffer + 2, 2);
		right.convertSamples (SourceSampleFormat (sourceBuffer.getSampleData (1)), samplesPerBlock);

		hr = pimpl->redbook->AddAudioTrackBlocks (buffer, bytesPerBlock);

		if (FAILED (hr))
			ok = false;

		samplesDone += samplesPerBlock;

		if (samplesDone >= numSamples)
			break;
	}

	hr = pimpl->redbook->CloseAudioTrack();
	return ok && hr == S_OK;
}

/*** End of inlined file: juce_win32_AudioCDBurner.cpp ***/


 #endif

#elif JUCE_LINUX
 #if JUCE_ALSA

/*** Start of inlined file: juce_linux_ALSA.cpp ***/
namespace
{
	void getDeviceSampleRates (snd_pcm_t* handle, Array <int>& rates)
	{
		const int ratesToTry[] = { 22050, 32000, 44100, 48000, 88200, 96000, 176400, 192000, 0 };

		snd_pcm_hw_params_t* hwParams;
		snd_pcm_hw_params_alloca (&hwParams);

		for (int i = 0; ratesToTry[i] != 0; ++i)
		{
			if (snd_pcm_hw_params_any (handle, hwParams) >= 0
				 && snd_pcm_hw_params_test_rate (handle, hwParams, ratesToTry[i], 0) == 0)
			{
				rates.addIfNotAlreadyThere (ratesToTry[i]);
			}
		}
	}

	void getDeviceNumChannels (snd_pcm_t* handle, unsigned int* minChans, unsigned int* maxChans)
	{
		snd_pcm_hw_params_t *params;
		snd_pcm_hw_params_alloca (&params);

		if (snd_pcm_hw_params_any (handle, params) >= 0)
		{
			snd_pcm_hw_params_get_channels_min (params, minChans);
			snd_pcm_hw_params_get_channels_max (params, maxChans);
		}
	}

	void getDeviceProperties (const String& deviceID,
							  unsigned int& minChansOut,
							  unsigned int& maxChansOut,
							  unsigned int& minChansIn,
							  unsigned int& maxChansIn,
							  Array <int>& rates)
	{
		if (deviceID.isEmpty())
			return;

		snd_ctl_t* handle;

		if (snd_ctl_open (&handle, deviceID.upToLastOccurrenceOf (",", false, false).toUTF8(), SND_CTL_NONBLOCK) >= 0)
		{
			snd_pcm_info_t* info;
			snd_pcm_info_alloca (&info);

			snd_pcm_info_set_stream (info, SND_PCM_STREAM_PLAYBACK);
			snd_pcm_info_set_device (info, deviceID.fromLastOccurrenceOf (",", false, false).getIntValue());
			snd_pcm_info_set_subdevice (info, 0);

			if (snd_ctl_pcm_info (handle, info) >= 0)
			{
				snd_pcm_t* pcmHandle;
				if (snd_pcm_open (&pcmHandle, deviceID.toUTF8(), SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC | SND_PCM_NONBLOCK) >= 0)
				{
					getDeviceNumChannels (pcmHandle, &minChansOut, &maxChansOut);
					getDeviceSampleRates (pcmHandle, rates);

					snd_pcm_close (pcmHandle);
				}
			}

			snd_pcm_info_set_stream (info, SND_PCM_STREAM_CAPTURE);

			if (snd_ctl_pcm_info (handle, info) >= 0)
			{
				snd_pcm_t* pcmHandle;
				if (snd_pcm_open (&pcmHandle, deviceID.toUTF8(), SND_PCM_STREAM_CAPTURE, SND_PCM_ASYNC | SND_PCM_NONBLOCK) >= 0)
				{
					getDeviceNumChannels (pcmHandle, &minChansIn, &maxChansIn);

					if (rates.size() == 0)
						getDeviceSampleRates (pcmHandle, rates);

					snd_pcm_close (pcmHandle);
				}
			}

			snd_ctl_close (handle);
		}
	}
}

class ALSADevice
{
public:
	ALSADevice (const String& deviceID, bool forInput)
		: handle (0),
		  bitDepth (16),
		  numChannelsRunning (0),
		  latency (0),
		  isInput (forInput),
		  isInterleaved (true)
	{
		failed (snd_pcm_open (&handle, deviceID.toUTF8(),
							  forInput ? SND_PCM_STREAM_CAPTURE : SND_PCM_STREAM_PLAYBACK,
							  SND_PCM_ASYNC));
	}

	~ALSADevice()
	{
		if (handle != 0)
			snd_pcm_close (handle);
	}

	bool setParameters (unsigned int sampleRate, int numChannels, int bufferSize)
	{
		if (handle == 0)
			return false;

		snd_pcm_hw_params_t* hwParams;
		snd_pcm_hw_params_alloca (&hwParams);

		if (failed (snd_pcm_hw_params_any (handle, hwParams)))
			return false;

		if (snd_pcm_hw_params_set_access (handle, hwParams, SND_PCM_ACCESS_RW_NONINTERLEAVED) >= 0)
			isInterleaved = false;
		else if (snd_pcm_hw_params_set_access (handle, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED) >= 0)
			isInterleaved = true;
		else
		{
			jassertfalse;
			return false;
		}

		enum { isFloatBit = 1 << 16, isLittleEndianBit = 1 << 17 };

		const int formatsToTry[] = { SND_PCM_FORMAT_FLOAT_LE,   32 | isFloatBit | isLittleEndianBit,
									 SND_PCM_FORMAT_FLOAT_BE,   32 | isFloatBit,
									 SND_PCM_FORMAT_S32_LE,     32 | isLittleEndianBit,
									 SND_PCM_FORMAT_S32_BE,     32,
									 SND_PCM_FORMAT_S24_3LE,    24 | isLittleEndianBit,
									 SND_PCM_FORMAT_S24_3BE,    24,
									 SND_PCM_FORMAT_S16_LE,     16 | isLittleEndianBit,
									 SND_PCM_FORMAT_S16_BE,     16 };
		bitDepth = 0;

		for (int i = 0; i < numElementsInArray (formatsToTry); i += 2)
		{
			if (snd_pcm_hw_params_set_format (handle, hwParams, (_snd_pcm_format) formatsToTry [i]) >= 0)
			{
				bitDepth = formatsToTry [i + 1] & 255;
				const bool isFloat = (formatsToTry [i + 1] & isFloatBit) != 0;
				const bool isLittleEndian = (formatsToTry [i + 1] & isLittleEndianBit) != 0;
				converter = createConverter (isInput, bitDepth, isFloat, isLittleEndian, numChannels);
				break;
			}
		}

		if (bitDepth == 0)
		{
			error = "device doesn't support a compatible PCM format";
			DBG ("ALSA error: " + error + "\n");
			return false;
		}

		int dir = 0;
		unsigned int periods = 4;
		snd_pcm_uframes_t samplesPerPeriod = bufferSize;

		if (failed (snd_pcm_hw_params_set_rate_near (handle, hwParams, &sampleRate, 0))
			|| failed (snd_pcm_hw_params_set_channels (handle, hwParams, numChannels))
			|| failed (snd_pcm_hw_params_set_periods_near (handle, hwParams, &periods, &dir))
			|| failed (snd_pcm_hw_params_set_period_size_near (handle, hwParams, &samplesPerPeriod, &dir))
			|| failed (snd_pcm_hw_params (handle, hwParams)))
		{
			return false;
		}

		snd_pcm_uframes_t frames = 0;

		if (failed (snd_pcm_hw_params_get_period_size (hwParams, &frames, &dir))
			 || failed (snd_pcm_hw_params_get_periods (hwParams, &periods, &dir)))
			latency = 0;
		else
			latency = frames * (periods - 1); // (this is the method JACK uses to guess the latency..)

		snd_pcm_sw_params_t* swParams;
		snd_pcm_sw_params_alloca (&swParams);
		snd_pcm_uframes_t boundary;

		if (failed (snd_pcm_sw_params_current (handle, swParams))
			|| failed (snd_pcm_sw_params_get_boundary (swParams, &boundary))
			|| failed (snd_pcm_sw_params_set_silence_threshold (handle, swParams, 0))
			|| failed (snd_pcm_sw_params_set_silence_size (handle, swParams, boundary))
			|| failed (snd_pcm_sw_params_set_start_threshold (handle, swParams, samplesPerPeriod))
			|| failed (snd_pcm_sw_params_set_stop_threshold (handle, swParams, boundary))
			|| failed (snd_pcm_sw_params (handle, swParams)))
		{
			return false;
		}

	  #if 0
		// enable this to dump the config of the devices that get opened
		snd_output_t* out;
		snd_output_stdio_attach (&out, stderr, 0);
		snd_pcm_hw_params_dump (hwParams, out);
		snd_pcm_sw_params_dump (swParams, out);
	  #endif

		numChannelsRunning = numChannels;

		return true;
	}

	bool writeToOutputDevice (AudioSampleBuffer& outputChannelBuffer, const int numSamples)
	{
		jassert (numChannelsRunning <= outputChannelBuffer.getNumChannels());
		float** const data = outputChannelBuffer.getArrayOfChannels();
		snd_pcm_sframes_t numDone = 0;

		if (isInterleaved)
		{
			scratch.ensureSize (sizeof (float) * numSamples * numChannelsRunning, false);

			for (int i = 0; i < numChannelsRunning; ++i)
				converter->convertSamples (scratch.getData(), i, data[i], 0, numSamples);

			numDone = snd_pcm_writei (handle, scratch.getData(), numSamples);
		}
		else
		{
			for (int i = 0; i < numChannelsRunning; ++i)
				converter->convertSamples (data[i], data[i], numSamples);

			numDone = snd_pcm_writen (handle, (void**) data, numSamples);
		}

		if (failed (numDone))
		{
			if (numDone == -EPIPE)
			{
				if (failed (snd_pcm_prepare (handle)))
					return false;
			}
			else if (numDone != -ESTRPIPE)
				return false;
		}

		return true;
	}

	bool readFromInputDevice (AudioSampleBuffer& inputChannelBuffer, const int numSamples)
	{
		jassert (numChannelsRunning <= inputChannelBuffer.getNumChannels());
		float** const data = inputChannelBuffer.getArrayOfChannels();

		if (isInterleaved)
		{
			scratch.ensureSize (sizeof (float) * numSamples * numChannelsRunning, false);
			scratch.fillWith (0); // (not clearing this data causes warnings in valgrind)

			snd_pcm_sframes_t num = snd_pcm_readi (handle, scratch.getData(), numSamples);

			if (failed (num))
			{
				if (num == -EPIPE)
				{
					if (failed (snd_pcm_prepare (handle)))
						return false;
				}
				else if (num != -ESTRPIPE)
					return false;
			}

			for (int i = 0; i < numChannelsRunning; ++i)
				converter->convertSamples (data[i], 0, scratch.getData(), i, numSamples);
		}
		else
		{
			snd_pcm_sframes_t num = snd_pcm_readn (handle, (void**) data, numSamples);

			if (failed (num) && num != -EPIPE && num != -ESTRPIPE)
				return false;

			for (int i = 0; i < numChannelsRunning; ++i)
				converter->convertSamples (data[i], data[i], numSamples);
		}

		return true;
	}

	snd_pcm_t* handle;
	String error;
	int bitDepth, numChannelsRunning, latency;

private:
	const bool isInput;
	bool isInterleaved;
	MemoryBlock scratch;
	ScopedPointer<AudioData::Converter> converter;

	template <class SampleType>
	struct ConverterHelper
	{
		static AudioData::Converter* createConverter (const bool forInput, const bool isLittleEndian, const int numInterleavedChannels)
		{
			if (forInput)
			{
				typedef AudioData::Pointer <AudioData::Float32, AudioData::NativeEndian, AudioData::NonInterleaved, AudioData::NonConst> DestType;

				if (isLittleEndian)
					return new AudioData::ConverterInstance <AudioData::Pointer <SampleType, AudioData::LittleEndian, AudioData::Interleaved, AudioData::Const>, DestType> (numInterleavedChannels, 1);
				else
					return new AudioData::ConverterInstance <AudioData::Pointer <SampleType, AudioData::BigEndian, AudioData::Interleaved, AudioData::Const>, DestType> (numInterleavedChannels, 1);
			}
			else
			{
				typedef AudioData::Pointer <AudioData::Float32, AudioData::NativeEndian, AudioData::NonInterleaved, AudioData::Const> SourceType;

				if (isLittleEndian)
					return new AudioData::ConverterInstance <SourceType, AudioData::Pointer <SampleType, AudioData::LittleEndian, AudioData::Interleaved, AudioData::NonConst> > (1, numInterleavedChannels);
				else
					return new AudioData::ConverterInstance <SourceType, AudioData::Pointer <SampleType, AudioData::BigEndian, AudioData::Interleaved, AudioData::NonConst> > (1, numInterleavedChannels);
			}
		}
	};

	static AudioData::Converter* createConverter (const bool forInput, const int bitDepth, const bool isFloat, const bool isLittleEndian, const int numInterleavedChannels)
	{
		switch (bitDepth)
		{
			case 16:    return ConverterHelper <AudioData::Int16>::createConverter (forInput, isLittleEndian,  numInterleavedChannels);
			case 24:    return ConverterHelper <AudioData::Int24>::createConverter (forInput, isLittleEndian,  numInterleavedChannels);
			case 32:    return isFloat ? ConverterHelper <AudioData::Float32>::createConverter (forInput, isLittleEndian,  numInterleavedChannels)
									   : ConverterHelper <AudioData::Int32>::createConverter (forInput, isLittleEndian,  numInterleavedChannels);
			default:    jassertfalse; break; // unsupported format!
		}

		return nullptr;
	}

	bool failed (const int errorNum)
	{
		if (errorNum >= 0)
			return false;

		error = snd_strerror (errorNum);
		DBG ("ALSA error: " + error + "\n");
		return true;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ALSADevice);
};

class ALSAThread  : public Thread
{
public:
	ALSAThread (const String& inputId_,
				const String& outputId_)
		: Thread ("Juce ALSA"),
		  sampleRate (0),
		  bufferSize (0),
		  outputLatency (0),
		  inputLatency (0),
		  callback (0),
		  inputId (inputId_),
		  outputId (outputId_),
		  numCallbacks (0),
		  inputChannelBuffer (1, 1),
		  outputChannelBuffer (1, 1)
	{
		initialiseRatesAndChannels();
	}

	~ALSAThread()
	{
		close();
	}

	void open (BigInteger inputChannels,
			   BigInteger outputChannels,
			   const double sampleRate_,
			   const int bufferSize_)
	{
		close();

		error = String::empty;
		sampleRate = sampleRate_;
		bufferSize = bufferSize_;

		inputChannelBuffer.setSize (jmax ((int) minChansIn, inputChannels.getHighestBit()) + 1, bufferSize);
		inputChannelBuffer.clear();
		inputChannelDataForCallback.clear();
		currentInputChans.clear();

		if (inputChannels.getHighestBit() >= 0)
		{
			for (int i = 0; i <= jmax (inputChannels.getHighestBit(), (int) minChansIn); ++i)
			{
				if (inputChannels[i])
				{
					inputChannelDataForCallback.add (inputChannelBuffer.getSampleData (i));
					currentInputChans.setBit (i);
				}
			}
		}

		outputChannelBuffer.setSize (jmax ((int) minChansOut, outputChannels.getHighestBit()) + 1, bufferSize);
		outputChannelBuffer.clear();
		outputChannelDataForCallback.clear();
		currentOutputChans.clear();

		if (outputChannels.getHighestBit() >= 0)
		{
			for (int i = 0; i <= jmax (outputChannels.getHighestBit(), (int) minChansOut); ++i)
			{
				if (outputChannels[i])
				{
					outputChannelDataForCallback.add (outputChannelBuffer.getSampleData (i));
					currentOutputChans.setBit (i);
				}
			}
		}

		if (outputChannelDataForCallback.size() > 0 && outputId.isNotEmpty())
		{
			outputDevice = new ALSADevice (outputId, false);

			if (outputDevice->error.isNotEmpty())
			{
				error = outputDevice->error;
				outputDevice = nullptr;
				return;
			}

			currentOutputChans.setRange (0, minChansOut, true);

			if (! outputDevice->setParameters ((unsigned int) sampleRate,
											   jlimit ((int) minChansOut, (int) maxChansOut, currentOutputChans.getHighestBit() + 1),
											   bufferSize))
			{
				error = outputDevice->error;
				outputDevice = nullptr;
				return;
			}

			outputLatency = outputDevice->latency;
		}

		if (inputChannelDataForCallback.size() > 0 && inputId.isNotEmpty())
		{
			inputDevice = new ALSADevice (inputId, true);

			if (inputDevice->error.isNotEmpty())
			{
				error = inputDevice->error;
				inputDevice = nullptr;
				return;
			}

			currentInputChans.setRange (0, minChansIn, true);

			if (! inputDevice->setParameters ((unsigned int) sampleRate,
											  jlimit ((int) minChansIn, (int) maxChansIn, currentInputChans.getHighestBit() + 1),
											  bufferSize))
			{
				error = inputDevice->error;
				inputDevice = nullptr;
				return;
			}

			inputLatency = inputDevice->latency;
		}

		if (outputDevice == nullptr && inputDevice == nullptr)
		{
			error = "no channels";
			return;
		}

		if (outputDevice != nullptr && inputDevice != nullptr)
		{
			snd_pcm_link (outputDevice->handle, inputDevice->handle);
		}

		if (inputDevice != nullptr && failed (snd_pcm_prepare (inputDevice->handle)))
			return;

		if (outputDevice != nullptr && failed (snd_pcm_prepare (outputDevice->handle)))
			return;

		startThread (9);

		int count = 1000;

		while (numCallbacks == 0)
		{
			sleep (5);

			if (--count < 0 || ! isThreadRunning())
			{
				error = "device didn't start";
				break;
			}
		}
	}

	void close()
	{
		stopThread (6000);

		inputDevice = nullptr;
		outputDevice = nullptr;

		inputChannelBuffer.setSize (1, 1);
		outputChannelBuffer.setSize (1, 1);

		numCallbacks = 0;
	}

	void setCallback (AudioIODeviceCallback* const newCallback) noexcept
	{
		const ScopedLock sl (callbackLock);
		callback = newCallback;
	}

	void run()
	{
		while (! threadShouldExit())
		{
			if (inputDevice != nullptr)
			{
				if (! inputDevice->readFromInputDevice (inputChannelBuffer, bufferSize))
				{
					DBG ("ALSA: read failure");
					break;
				}
			}

			if (threadShouldExit())
				break;

			{
				const ScopedLock sl (callbackLock);
				++numCallbacks;

				if (callback != nullptr)
				{
					callback->audioDeviceIOCallback ((const float**) inputChannelDataForCallback.getRawDataPointer(),
													 inputChannelDataForCallback.size(),
													 outputChannelDataForCallback.getRawDataPointer(),
													 outputChannelDataForCallback.size(),
													 bufferSize);
				}
				else
				{
					for (int i = 0; i < outputChannelDataForCallback.size(); ++i)
						zeromem (outputChannelDataForCallback[i], sizeof (float) * bufferSize);
				}
			}

			if (outputDevice != nullptr)
			{
				failed (snd_pcm_wait (outputDevice->handle, 2000));

				if (threadShouldExit())
					break;

				failed (snd_pcm_avail_update (outputDevice->handle));

				if (! outputDevice->writeToOutputDevice (outputChannelBuffer, bufferSize))
				{
					DBG ("ALSA: write failure");
					break;
				}
			}
		}
	}

	int getBitDepth() const noexcept
	{
		if (outputDevice != nullptr)
			return outputDevice->bitDepth;

		if (inputDevice != nullptr)
			return inputDevice->bitDepth;

		return 16;
	}

	String error;
	double sampleRate;
	int bufferSize, outputLatency, inputLatency;
	BigInteger currentInputChans, currentOutputChans;

	Array <int> sampleRates;
	StringArray channelNamesOut, channelNamesIn;
	AudioIODeviceCallback* callback;

private:

	const String inputId, outputId;
	ScopedPointer<ALSADevice> outputDevice, inputDevice;
	int numCallbacks;

	CriticalSection callbackLock;

	AudioSampleBuffer inputChannelBuffer, outputChannelBuffer;
	Array<float*> inputChannelDataForCallback, outputChannelDataForCallback;

	unsigned int minChansOut, maxChansOut;
	unsigned int minChansIn, maxChansIn;

	bool failed (const int errorNum)
	{
		if (errorNum >= 0)
			return false;

		error = snd_strerror (errorNum);
		DBG ("ALSA error: " + error + "\n");
		return true;
	}

	void initialiseRatesAndChannels()
	{
		sampleRates.clear();
		channelNamesOut.clear();
		channelNamesIn.clear();
		minChansOut = 0;
		maxChansOut = 0;
		minChansIn = 0;
		maxChansIn = 0;
		unsigned int dummy = 0;

		getDeviceProperties (inputId, dummy, dummy, minChansIn, maxChansIn, sampleRates);
		getDeviceProperties (outputId, minChansOut, maxChansOut, dummy, dummy, sampleRates);

		unsigned int i;
		for (i = 0; i < maxChansOut; ++i)
			channelNamesOut.add ("channel " + String ((int) i + 1));

		for (i = 0; i < maxChansIn; ++i)
			channelNamesIn.add ("channel " + String ((int) i + 1));
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ALSAThread);
};

class ALSAAudioIODevice   : public AudioIODevice
{
public:
	ALSAAudioIODevice (const String& deviceName,
					   const String& inputId_,
					   const String& outputId_)
		: AudioIODevice (deviceName, "ALSA"),
		  inputId (inputId_),
		  outputId (outputId_),
		  isOpen_ (false),
		  isStarted (false),
		  internal (inputId_, outputId_)
	{
	}

	~ALSAAudioIODevice()
	{
		close();
	}

	StringArray getOutputChannelNames()             { return internal.channelNamesOut; }
	StringArray getInputChannelNames()              { return internal.channelNamesIn; }

	int getNumSampleRates()                         { return internal.sampleRates.size(); }
	double getSampleRate (int index)                { return internal.sampleRates [index]; }

	int getDefaultBufferSize()                      { return 512; }
	int getNumBufferSizesAvailable()                { return 50; }

	int getBufferSizeSamples (int index)
	{
		int n = 16;
		for (int i = 0; i < index; ++i)
			n += n < 64 ? 16
						: (n < 512 ? 32
								   : (n < 1024 ? 64
											   : (n < 2048 ? 128 : 256)));

		return n;
	}

	String open (const BigInteger& inputChannels,
				 const BigInteger& outputChannels,
				 double sampleRate,
				 int bufferSizeSamples)
	{
		close();

		if (bufferSizeSamples <= 0)
			bufferSizeSamples = getDefaultBufferSize();

		if (sampleRate <= 0)
		{
			for (int i = 0; i < getNumSampleRates(); ++i)
			{
				if (getSampleRate (i) >= 44100)
				{
					sampleRate = getSampleRate (i);
					break;
				}
			}
		}

		internal.open (inputChannels, outputChannels,
					   sampleRate, bufferSizeSamples);

		isOpen_ = internal.error.isEmpty();
		return internal.error;
	}

	void close()
	{
		stop();
		internal.close();
		isOpen_ = false;
	}

	bool isOpen()                           { return isOpen_; }
	bool isPlaying()                        { return isStarted && internal.error.isEmpty(); }
	String getLastError()                   { return internal.error; }

	int getCurrentBufferSizeSamples()       { return internal.bufferSize; }
	double getCurrentSampleRate()           { return internal.sampleRate; }
	int getCurrentBitDepth()                { return internal.getBitDepth(); }

	BigInteger getActiveOutputChannels() const    { return internal.currentOutputChans; }
	BigInteger getActiveInputChannels() const     { return internal.currentInputChans; }

	int getOutputLatencyInSamples()         { return internal.outputLatency; }
	int getInputLatencyInSamples()          { return internal.inputLatency; }

	void start (AudioIODeviceCallback* callback)
	{
		if (! isOpen_)
			callback = nullptr;

		if (callback != nullptr)
			callback->audioDeviceAboutToStart (this);

		internal.setCallback (callback);

		isStarted = (callback != nullptr);
	}

	void stop()
	{
		AudioIODeviceCallback* const oldCallback = internal.callback;

		start (0);

		if (oldCallback != nullptr)
			oldCallback->audioDeviceStopped();
	}

	String inputId, outputId;

private:
	bool isOpen_, isStarted;
	ALSAThread internal;
};

class ALSAAudioIODeviceType  : public AudioIODeviceType
{
public:

	ALSAAudioIODeviceType()
		: AudioIODeviceType ("ALSA"),
		  hasScanned (false)
	{
	}

	~ALSAAudioIODeviceType()
	{
	}

	void scanForDevices()
	{
		if (hasScanned)
			return;

		hasScanned = true;
		inputNames.clear();
		inputIds.clear();
		outputNames.clear();
		outputIds.clear();

/*        void** hints = 0;
		if (snd_device_name_hint (-1, "pcm", &hints) >= 0)
		{
			for (void** hint = hints; *hint != 0; ++hint)
			{
				const String name (getHint (*hint, "NAME"));

				if (name.isNotEmpty())
				{
					const String ioid (getHint (*hint, "IOID"));

					String desc (getHint (*hint, "DESC"));
					if (desc.isEmpty())
						desc = name;

					desc = desc.replaceCharacters ("\n\r", "  ");

					DBG ("name: " << name << "\ndesc: " << desc << "\nIO: " << ioid);

					if (ioid.isEmpty() || ioid == "Input")
					{
						inputNames.add (desc);
						inputIds.add (name);
					}

					if (ioid.isEmpty() || ioid == "Output")
					{
						outputNames.add (desc);
						outputIds.add (name);
					}
				}
			}

			snd_device_name_free_hint (hints);
		}
*/
		snd_ctl_t* handle = nullptr;
		snd_ctl_card_info_t* info = nullptr;
		snd_ctl_card_info_alloca (&info);

		int cardNum = -1;

		while (outputIds.size() + inputIds.size() <= 32)
		{
			snd_card_next (&cardNum);

			if (cardNum < 0)
				break;

			if (snd_ctl_open (&handle, ("hw:" + String (cardNum)).toUTF8(), SND_CTL_NONBLOCK) >= 0)
			{
				if (snd_ctl_card_info (handle, info) >= 0)
				{
					String cardId (snd_ctl_card_info_get_id (info));

					if (cardId.removeCharacters ("0123456789").isEmpty())
						cardId = String (cardNum);

					int device = -1;

					for (;;)
					{
						if (snd_ctl_pcm_next_device (handle, &device) < 0 || device < 0)
							break;

						String id, name;
						id << "hw:" << cardId << ',' << device;

						bool isInput, isOutput;

						if (testDevice (id, isInput, isOutput))
						{
							name << snd_ctl_card_info_get_name (info);

							if (name.isEmpty())
								name = id;

							if (isInput)
							{
								inputNames.add (name);
								inputIds.add (id);
							}

							if (isOutput)
							{
								outputNames.add (name);
								outputIds.add (id);
							}
						}
					}
				}

				snd_ctl_close (handle);
			}
		}

		inputNames.appendNumbersToDuplicates (false, true);
		outputNames.appendNumbersToDuplicates (false, true);
	}

	StringArray getDeviceNames (bool wantInputNames) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		return wantInputNames ? inputNames : outputNames;
	}

	int getDefaultDeviceIndex (bool forInput) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this
		return 0;
	}

	bool hasSeparateInputsAndOutputs() const    { return true; }

	int getIndexOfDevice (AudioIODevice* device, bool asInput) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		ALSAAudioIODevice* d = dynamic_cast <ALSAAudioIODevice*> (device);
		if (d == nullptr)
			return -1;

		return asInput ? inputIds.indexOf (d->inputId)
					   : outputIds.indexOf (d->outputId);
	}

	AudioIODevice* createDevice (const String& outputDeviceName,
								 const String& inputDeviceName)
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		const int inputIndex = inputNames.indexOf (inputDeviceName);
		const int outputIndex = outputNames.indexOf (outputDeviceName);

		String deviceName (outputIndex >= 0 ? outputDeviceName
											: inputDeviceName);

		if (inputIndex >= 0 || outputIndex >= 0)
			return new ALSAAudioIODevice (deviceName,
										  inputIds [inputIndex],
										  outputIds [outputIndex]);

		return nullptr;
	}

private:
	StringArray inputNames, outputNames, inputIds, outputIds;
	bool hasScanned;

	static bool testDevice (const String& id, bool& isInput, bool& isOutput)
	{
		unsigned int minChansOut = 0, maxChansOut = 0;
		unsigned int minChansIn = 0, maxChansIn = 0;
		Array <int> rates;

		getDeviceProperties (id, minChansOut, maxChansOut, minChansIn, maxChansIn, rates);

		DBG ("ALSA device: " + id
			  + " outs=" + String ((int) minChansOut) + "-" + String ((int) maxChansOut)
			  + " ins=" + String ((int) minChansIn) + "-" + String ((int) maxChansIn)
			  + " rates=" + String (rates.size()));

		isInput = maxChansIn > 0;
		isOutput = maxChansOut > 0;

		return (isInput || isOutput) && rates.size() > 0;
	}

	/*static String getHint (void* hint, const char* type)
	{
		char* const n = snd_device_name_get_hint (hint, type);
		const String s ((const char*) n);
		free (n);
		return s;
	}*/

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ALSAAudioIODeviceType);
};

AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_ALSA()
{
	return new ALSAAudioIODeviceType();
}

/*** End of inlined file: juce_linux_ALSA.cpp ***/


 #endif


/*** Start of inlined file: juce_linux_Midi.cpp ***/
#if JUCE_ALSA

// You can define these strings in your app if you want to override the default names:
#ifndef JUCE_ALSA_MIDI_INPUT_NAME
 #define JUCE_ALSA_MIDI_INPUT_NAME  "Juce Midi Input"
#endif

#ifndef JUCE_ALSA_MIDI_OUTPUT_NAME
 #define JUCE_ALSA_MIDI_OUTPUT_NAME "Juce Midi Output"
#endif

namespace
{
	snd_seq_t* iterateMidiClient (snd_seq_t* seqHandle,
								  snd_seq_client_info_t* clientInfo,
								  const bool forInput,
								  StringArray& deviceNamesFound,
								  const int deviceIndexToOpen)
	{
		snd_seq_t* returnedHandle = nullptr;

		snd_seq_port_info_t* portInfo;
		if (snd_seq_port_info_malloc (&portInfo) == 0)
		{
			int numPorts = snd_seq_client_info_get_num_ports (clientInfo);
			const int client = snd_seq_client_info_get_client (clientInfo);

			snd_seq_port_info_set_client (portInfo, client);
			snd_seq_port_info_set_port (portInfo, -1);

			while (--numPorts >= 0)
			{
				if (snd_seq_query_next_port (seqHandle, portInfo) == 0
					 && (snd_seq_port_info_get_capability (portInfo)
						   & (forInput ? SND_SEQ_PORT_CAP_READ
									   : SND_SEQ_PORT_CAP_WRITE)) != 0)
				{
					deviceNamesFound.add (snd_seq_client_info_get_name (clientInfo));

					if (deviceNamesFound.size() == deviceIndexToOpen + 1)
					{
						const int sourcePort = snd_seq_port_info_get_port (portInfo);
						const int sourceClient = snd_seq_client_info_get_client (clientInfo);

						if (sourcePort != -1)
						{
							if (forInput)
							{
								snd_seq_set_client_name (seqHandle, JUCE_ALSA_MIDI_INPUT_NAME);

								const int portId = snd_seq_create_simple_port (seqHandle, "Juce Midi In Port",
																			   SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
																			   SND_SEQ_PORT_TYPE_MIDI_GENERIC);

								snd_seq_connect_from (seqHandle, portId, sourceClient, sourcePort);
							}
							else
							{
								snd_seq_set_client_name (seqHandle, JUCE_ALSA_MIDI_OUTPUT_NAME);

								const int portId = snd_seq_create_simple_port (seqHandle, "Juce Midi Out Port",
																			   SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
																			   SND_SEQ_PORT_TYPE_MIDI_GENERIC);

								snd_seq_connect_to (seqHandle, portId, sourceClient, sourcePort);
							}

							returnedHandle = seqHandle;
						}
					}
				}
			}

			snd_seq_port_info_free (portInfo);
		}

		return returnedHandle;
	}

	snd_seq_t* iterateMidiDevices (const bool forInput,
								   StringArray& deviceNamesFound,
								   const int deviceIndexToOpen)
	{
		snd_seq_t* returnedHandle = nullptr;
		snd_seq_t* seqHandle = nullptr;

		if (snd_seq_open (&seqHandle, "default", forInput ? SND_SEQ_OPEN_INPUT
														  : SND_SEQ_OPEN_OUTPUT, 0) == 0)
		{
			snd_seq_system_info_t* systemInfo = nullptr;
			snd_seq_client_info_t* clientInfo = nullptr;

			if (snd_seq_system_info_malloc (&systemInfo) == 0)
			{
				if (snd_seq_system_info (seqHandle, systemInfo) == 0
					 && snd_seq_client_info_malloc (&clientInfo) == 0)
				{
					int numClients = snd_seq_system_info_get_cur_clients (systemInfo);

					while (--numClients >= 0 && returnedHandle == 0)
						if (snd_seq_query_next_client (seqHandle, clientInfo) == 0)
							returnedHandle = iterateMidiClient (seqHandle, clientInfo,
																forInput, deviceNamesFound,
																deviceIndexToOpen);

					snd_seq_client_info_free (clientInfo);
				}

				snd_seq_system_info_free (systemInfo);
			}

			if (returnedHandle == 0)
				snd_seq_close (seqHandle);
		}

		deviceNamesFound.appendNumbersToDuplicates (true, true);

		return returnedHandle;
	}

	snd_seq_t* createMidiDevice (const bool forInput, const String& deviceNameToOpen)
	{
		snd_seq_t* seqHandle = nullptr;

		if (snd_seq_open (&seqHandle, "default", forInput ? SND_SEQ_OPEN_INPUT
														  : SND_SEQ_OPEN_OUTPUT, 0) == 0)
		{
			snd_seq_set_client_name (seqHandle,
									 (deviceNameToOpen + (forInput ? " Input" : " Output")).toUTF8());

			const int portId
				= snd_seq_create_simple_port (seqHandle,
											  forInput ? "in"
													   : "out",
											  forInput ? (SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE)
													   : (SND_SEQ_PORT_CAP_READ  | SND_SEQ_PORT_CAP_SUBS_READ),
											  forInput ? SND_SEQ_PORT_TYPE_APPLICATION
													   : SND_SEQ_PORT_TYPE_MIDI_GENERIC);

			if (portId < 0)
			{
				snd_seq_close (seqHandle);
				seqHandle = nullptr;
			}
		}

		return seqHandle;
	}
}

class MidiOutputDevice
{
public:
	MidiOutputDevice (MidiOutput* const midiOutput_,
					  snd_seq_t* const seqHandle_)
		:
		  midiOutput (midiOutput_),
		  seqHandle (seqHandle_),
		  maxEventSize (16 * 1024)
	{
		jassert (seqHandle != 0 && midiOutput != 0);
		snd_midi_event_new (maxEventSize, &midiParser);
	}

	~MidiOutputDevice()
	{
		snd_midi_event_free (midiParser);
		snd_seq_close (seqHandle);
	}

	void sendMessageNow (const MidiMessage& message)
	{
		if (message.getRawDataSize() > maxEventSize)
		{
			maxEventSize = message.getRawDataSize();
			snd_midi_event_free (midiParser);
			snd_midi_event_new (maxEventSize, &midiParser);
		}

		snd_seq_event_t event;
		snd_seq_ev_clear (&event);

		snd_midi_event_encode (midiParser,
							   message.getRawData(),
							   message.getRawDataSize(),
							   &event);

		snd_midi_event_reset_encode (midiParser);

		snd_seq_ev_set_source (&event, 0);
		snd_seq_ev_set_subs (&event);
		snd_seq_ev_set_direct (&event);

		snd_seq_event_output (seqHandle, &event);
		snd_seq_drain_output (seqHandle);
	}

private:
	MidiOutput* const midiOutput;
	snd_seq_t* const seqHandle;
	snd_midi_event_t* midiParser;
	int maxEventSize;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiOutputDevice);
};

StringArray MidiOutput::getDevices()
{
	StringArray devices;
	iterateMidiDevices (false, devices, -1);
	return devices;
}

int MidiOutput::getDefaultDeviceIndex()
{
	return 0;
}

MidiOutput* MidiOutput::openDevice (int deviceIndex)
{
	MidiOutput* newDevice = nullptr;

	StringArray devices;
	snd_seq_t* const handle = iterateMidiDevices (false, devices, deviceIndex);

	if (handle != 0)
	{
		newDevice = new MidiOutput();
		newDevice->internal = new MidiOutputDevice (newDevice, handle);
	}

	return newDevice;
}

MidiOutput* MidiOutput::createNewDevice (const String& deviceName)
{
	MidiOutput* newDevice = nullptr;

	snd_seq_t* const handle = createMidiDevice (false, deviceName);

	if (handle != 0)
	{
		newDevice = new MidiOutput();
		newDevice->internal = new MidiOutputDevice (newDevice, handle);
	}

	return newDevice;
}

MidiOutput::~MidiOutput()
{
	delete static_cast <MidiOutputDevice*> (internal);
}

void MidiOutput::sendMessageNow (const MidiMessage& message)
{
	static_cast <MidiOutputDevice*> (internal)->sendMessageNow (message);
}

class MidiInputThread   : public Thread
{
public:
	MidiInputThread (MidiInput* const midiInput_,
					 snd_seq_t* const seqHandle_,
					 MidiInputCallback* const callback_)
		: Thread ("Juce MIDI Input"),
		  midiInput (midiInput_),
		  seqHandle (seqHandle_),
		  callback (callback_)
	{
		jassert (seqHandle != 0 && callback != 0 && midiInput != 0);
	}

	~MidiInputThread()
	{
		snd_seq_close (seqHandle);
	}

	void run()
	{
		const int maxEventSize = 16 * 1024;
		snd_midi_event_t* midiParser;

		if (snd_midi_event_new (maxEventSize, &midiParser) >= 0)
		{
			HeapBlock <uint8> buffer (maxEventSize);

			const int numPfds = snd_seq_poll_descriptors_count (seqHandle, POLLIN);
			struct pollfd* const pfd = (struct pollfd*) alloca (numPfds * sizeof (struct pollfd));

			snd_seq_poll_descriptors (seqHandle, pfd, numPfds, POLLIN);

			while (! threadShouldExit())
			{
				if (poll (pfd, numPfds, 500) > 0)
				{
					snd_seq_event_t* inputEvent = nullptr;

					snd_seq_nonblock (seqHandle, 1);

					do
					{
						if (snd_seq_event_input (seqHandle, &inputEvent) >= 0)
						{
							// xxx what about SYSEXes that are too big for the buffer?
							const int numBytes = snd_midi_event_decode (midiParser, buffer, maxEventSize, inputEvent);

							snd_midi_event_reset_decode (midiParser);

							if (numBytes > 0)
							{
								const MidiMessage message ((const uint8*) buffer,
														   numBytes,
														   Time::getMillisecondCounter() * 0.001);

								callback->handleIncomingMidiMessage (midiInput, message);
							}

							snd_seq_free_event (inputEvent);
						}
					}
					while (snd_seq_event_input_pending (seqHandle, 0) > 0);

					snd_seq_free_event (inputEvent);
				}
			}

			snd_midi_event_free (midiParser);
		}
	};

private:
	MidiInput* const midiInput;
	snd_seq_t* const seqHandle;
	MidiInputCallback* const callback;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiInputThread);
};

MidiInput::MidiInput (const String& name_)
	: name (name_),
	  internal (0)
{
}

MidiInput::~MidiInput()
{
	stop();
	delete static_cast <MidiInputThread*> (internal);
}

void MidiInput::start()
{
	static_cast <MidiInputThread*> (internal)->startThread();
}

void MidiInput::stop()
{
	static_cast <MidiInputThread*> (internal)->stopThread (3000);
}

int MidiInput::getDefaultDeviceIndex()
{
	return 0;
}

StringArray MidiInput::getDevices()
{
	StringArray devices;
	iterateMidiDevices (true, devices, -1);
	return devices;
}

MidiInput* MidiInput::openDevice (int deviceIndex, MidiInputCallback* callback)
{
	MidiInput* newDevice = nullptr;

	StringArray devices;
	snd_seq_t* const handle = iterateMidiDevices (true, devices, deviceIndex);

	if (handle != 0)
	{
		newDevice = new MidiInput (devices [deviceIndex]);
		newDevice->internal = new MidiInputThread (newDevice, handle, callback);
	}

	return newDevice;
}

MidiInput* MidiInput::createNewDevice (const String& deviceName, MidiInputCallback* callback)
{
	MidiInput* newDevice = nullptr;

	snd_seq_t* const handle = createMidiDevice (true, deviceName);

	if (handle != 0)
	{
		newDevice = new MidiInput (deviceName);
		newDevice->internal = new MidiInputThread (newDevice, handle, callback);
	}

	return newDevice;
}

#else

// (These are just stub functions if ALSA is unavailable...)

StringArray MidiOutput::getDevices()                                { return StringArray(); }
int MidiOutput::getDefaultDeviceIndex()                             { return 0; }
MidiOutput* MidiOutput::openDevice (int)                            { return nullptr; }
MidiOutput* MidiOutput::createNewDevice (const String&)             { return nullptr; }
MidiOutput::~MidiOutput()   {}
void MidiOutput::sendMessageNow (const MidiMessage&)    {}

MidiInput::MidiInput (const String& name_) : name (name_), internal (0)  {}
MidiInput::~MidiInput() {}
void MidiInput::start() {}
void MidiInput::stop()  {}
int MidiInput::getDefaultDeviceIndex()      { return 0; }
StringArray MidiInput::getDevices()         { return StringArray(); }
MidiInput* MidiInput::openDevice (int, MidiInputCallback*)                  { return nullptr; }
MidiInput* MidiInput::createNewDevice (const String&, MidiInputCallback*)   { return nullptr; }

#endif

/*** End of inlined file: juce_linux_Midi.cpp ***/

 #if JUCE_JACK

/*** Start of inlined file: juce_linux_JackAudio.cpp ***/
static void* juce_libjackHandle = nullptr;

static void* juce_loadJackFunction (const char* const name)
{
	if (juce_libjackHandle == nullptr)
		return nullptr;

	return dlsym (juce_libjackHandle, name);
}

#define JUCE_DECL_JACK_FUNCTION(return_type, fn_name, argument_types, arguments)  \
  typedef return_type (*fn_name##_ptr_t)argument_types;                           \
  return_type fn_name argument_types {                                            \
	static fn_name##_ptr_t fn = nullptr;                                          \
	if (fn == nullptr) { fn = (fn_name##_ptr_t)juce_loadJackFunction(#fn_name); } \
	if (fn) return (*fn)arguments;                                                \
	else return nullptr;                                                          \
  }

#define JUCE_DECL_VOID_JACK_FUNCTION(fn_name, argument_types, arguments)          \
  typedef void (*fn_name##_ptr_t)argument_types;                                  \
  void fn_name argument_types {                                                   \
	static fn_name##_ptr_t fn = nullptr;                                          \
	if (fn == nullptr) { fn = (fn_name##_ptr_t)juce_loadJackFunction(#fn_name); } \
	if (fn) (*fn)arguments;                                                       \
  }

JUCE_DECL_JACK_FUNCTION (jack_client_t*, jack_client_open, (const char* client_name, jack_options_t options, jack_status_t* status, ...), (client_name, options, status));
JUCE_DECL_JACK_FUNCTION (int, jack_client_close, (jack_client_t *client), (client));
JUCE_DECL_JACK_FUNCTION (int, jack_activate, (jack_client_t* client), (client));
JUCE_DECL_JACK_FUNCTION (int, jack_deactivate, (jack_client_t* client), (client));
JUCE_DECL_JACK_FUNCTION (jack_nframes_t, jack_get_buffer_size, (jack_client_t* client), (client));
JUCE_DECL_JACK_FUNCTION (jack_nframes_t, jack_get_sample_rate, (jack_client_t* client), (client));
JUCE_DECL_VOID_JACK_FUNCTION (jack_on_shutdown, (jack_client_t* client, void (*function)(void* arg), void* arg), (client, function, arg));
JUCE_DECL_JACK_FUNCTION (void* , jack_port_get_buffer, (jack_port_t* port, jack_nframes_t nframes), (port, nframes));
JUCE_DECL_JACK_FUNCTION (jack_nframes_t, jack_port_get_total_latency, (jack_client_t* client, jack_port_t* port), (client, port));
JUCE_DECL_JACK_FUNCTION (jack_port_t* , jack_port_register, (jack_client_t* client, const char* port_name, const char* port_type, unsigned long flags, unsigned long buffer_size), (client, port_name, port_type, flags, buffer_size));
JUCE_DECL_VOID_JACK_FUNCTION (jack_set_error_function, (void (*func)(const char*)), (func));
JUCE_DECL_JACK_FUNCTION (int, jack_set_process_callback, (jack_client_t* client, JackProcessCallback process_callback, void* arg), (client, process_callback, arg));
JUCE_DECL_JACK_FUNCTION (const char**, jack_get_ports, (jack_client_t* client, const char* port_name_pattern, const char* type_name_pattern, unsigned long flags), (client, port_name_pattern, type_name_pattern, flags));
JUCE_DECL_JACK_FUNCTION (int, jack_connect, (jack_client_t* client, const char* source_port, const char* destination_port), (client, source_port, destination_port));
JUCE_DECL_JACK_FUNCTION (const char*, jack_port_name, (const jack_port_t* port), (port));
JUCE_DECL_JACK_FUNCTION (int, jack_set_port_connect_callback, (jack_client_t* client, JackPortConnectCallback connect_callback, void* arg), (client, connect_callback, arg));
JUCE_DECL_JACK_FUNCTION (jack_port_t* , jack_port_by_id, (jack_client_t* client, jack_port_id_t port_id), (client, port_id));
JUCE_DECL_JACK_FUNCTION (int, jack_port_connected, (const jack_port_t* port), (port));
JUCE_DECL_JACK_FUNCTION (int, jack_port_connected_to, (const jack_port_t* port, const char* port_name), (port, port_name));

#if JUCE_DEBUG
  #define JACK_LOGGING_ENABLED 1
#endif

#if JACK_LOGGING_ENABLED
namespace
{
	void jack_Log (const String& s)
	{
		std::cerr << s << std::endl;
	}

	void dumpJackErrorMessage (const jack_status_t status)
	{
		if (status & JackServerFailed || status & JackServerError)  jack_Log ("Unable to connect to JACK server");
		if (status & JackVersionError)      jack_Log ("Client's protocol version does not match");
		if (status & JackInvalidOption)     jack_Log ("The operation contained an invalid or unsupported option");
		if (status & JackNameNotUnique)     jack_Log ("The desired client name was not unique");
		if (status & JackNoSuchClient)      jack_Log ("Requested client does not exist");
		if (status & JackInitFailure)       jack_Log ("Unable to initialize client");
	}
}
#else
  #define dumpJackErrorMessage(a) {}
  #define jack_Log(...) {}
#endif

#ifndef JUCE_JACK_CLIENT_NAME
  #define JUCE_JACK_CLIENT_NAME "JuceJack"
#endif

class JackAudioIODevice   : public AudioIODevice
{
public:
	JackAudioIODevice (const String& deviceName,
					   const String& inputId_,
					   const String& outputId_)
		: AudioIODevice (deviceName, "JACK"),
		  inputId (inputId_),
		  outputId (outputId_),
		  isOpen_ (false),
		  callback (nullptr),
		  totalNumberOfInputChannels (0),
		  totalNumberOfOutputChannels (0)
	{
		jassert (deviceName.isNotEmpty());

		jack_status_t status;
		client = juce::jack_client_open (JUCE_JACK_CLIENT_NAME, JackNoStartServer, &status);

		if (client == 0)
		{
			dumpJackErrorMessage (status);
		}
		else
		{
			juce::jack_set_error_function (errorCallback);

			// open input ports
			const StringArray inputChannels (getInputChannelNames());
			for (int i = 0; i < inputChannels.size(); i++)
			{
				String inputName;
				inputName << "in_" << ++totalNumberOfInputChannels;

				inputPorts.add (juce::jack_port_register (client, inputName.toUTF8(),
														  JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
			}

			// open output ports
			const StringArray outputChannels (getOutputChannelNames());
			for (int i = 0; i < outputChannels.size (); i++)
			{
				String outputName;
				outputName << "out_" << ++totalNumberOfOutputChannels;

				outputPorts.add (juce::jack_port_register (client, outputName.toUTF8(),
														   JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0));
			}

			inChans.calloc (totalNumberOfInputChannels + 2);
			outChans.calloc (totalNumberOfOutputChannels + 2);
		}
	}

	~JackAudioIODevice()
	{
		close();
		if (client != 0)
		{
			juce::jack_client_close (client);
			client = 0;
		}
	}

	StringArray getChannelNames (bool forInput) const
	{
		StringArray names;
		const char** const ports = juce::jack_get_ports (client, 0, 0, /* JackPortIsPhysical | */
														 forInput ? JackPortIsInput : JackPortIsOutput);

		if (ports != 0)
		{
			int j = 0;
			while (ports[j] != 0)
			{
				const String portName (ports [j++]);

				if (portName.upToFirstOccurrenceOf (":", false, false) == getName())
					names.add (portName.fromFirstOccurrenceOf (":", false, false));
			}

			free (ports);
		}

		return names;
	}

	StringArray getOutputChannelNames()         { return getChannelNames (false); }
	StringArray getInputChannelNames()          { return getChannelNames (true); }
	int getNumSampleRates()                     { return client != 0 ? 1 : 0; }
	double getSampleRate (int index)            { return client != 0 ? juce::jack_get_sample_rate (client) : 0; }
	int getNumBufferSizesAvailable()            { return client != 0 ? 1 : 0; }
	int getBufferSizeSamples (int index)        { return getDefaultBufferSize(); }
	int getDefaultBufferSize()                  { return client != 0 ? juce::jack_get_buffer_size (client) : 0; }

	String open (const BigInteger& inputChannels, const BigInteger& outputChannels,
				 double sampleRate, int bufferSizeSamples)
	{
		if (client == 0)
		{
			lastError = "No JACK client running";
			return lastError;
		}

		lastError = String::empty;
		close();

		juce::jack_set_process_callback (client, processCallback, this);
		juce::jack_on_shutdown (client, shutdownCallback, this);
		juce::jack_activate (client);
		isOpen_ = true;

		if (! inputChannels.isZero())
		{
			const char** const ports = juce::jack_get_ports (client, 0, 0, /* JackPortIsPhysical | */ JackPortIsOutput);

			if (ports != 0)
			{
				const int numInputChannels = inputChannels.getHighestBit() + 1;

				for (int i = 0; i < numInputChannels; ++i)
				{
					const String portName (ports[i]);

					if (inputChannels[i] && portName.upToFirstOccurrenceOf (":", false, false) == getName())
					{
						int error = juce::jack_connect (client, ports[i], juce::jack_port_name ((jack_port_t*) inputPorts[i]));
						if (error != 0)
							jack_Log ("Cannot connect input port " + String (i) + " (" + String (ports[i]) + "), error " + String (error));
					}
				}

				free (ports);
			}
		}

		if (! outputChannels.isZero())
		{
			const char** const ports = juce::jack_get_ports (client, 0, 0, /* JackPortIsPhysical | */ JackPortIsInput);

			if (ports != 0)
			{
				const int numOutputChannels = outputChannels.getHighestBit() + 1;

				for (int i = 0; i < numOutputChannels; ++i)
				{
					const String portName (ports[i]);

					if (outputChannels[i] && portName.upToFirstOccurrenceOf (":", false, false) == getName())
					{
						int error = juce::jack_connect (client, juce::jack_port_name ((jack_port_t*) outputPorts[i]), ports[i]);
						if (error != 0)
							jack_Log ("Cannot connect output port " + String (i) + " (" + String (ports[i]) + "), error " + String (error));
					}
				}

				free (ports);
			}
		}

		return lastError;
	}

	void close()
	{
		stop();

		if (client != 0)
		{
			juce::jack_deactivate (client);
			juce::jack_set_process_callback (client, processCallback, 0);
			juce::jack_on_shutdown (client, shutdownCallback, 0);
		}

		isOpen_ = false;
	}

	void start (AudioIODeviceCallback* newCallback)
	{
		if (isOpen_ && newCallback != callback)
		{
			if (newCallback != nullptr)
				newCallback->audioDeviceAboutToStart (this);

			AudioIODeviceCallback* const oldCallback = callback;

			{
				const ScopedLock sl (callbackLock);
				callback = newCallback;
			}

			if (oldCallback != nullptr)
				oldCallback->audioDeviceStopped();
		}
	}

	void stop()
	{
		start (0);
	}

	bool isOpen()                           { return isOpen_; }
	bool isPlaying()                        { return callback != nullptr; }
	int getCurrentBufferSizeSamples()       { return getBufferSizeSamples (0); }
	double getCurrentSampleRate()           { return getSampleRate (0); }
	int getCurrentBitDepth()                { return 32; }
	String getLastError()                   { return lastError; }

	BigInteger getActiveOutputChannels() const
	{
		BigInteger outputBits;

		for (int i = 0; i < outputPorts.size(); i++)
			if (juce::jack_port_connected ((jack_port_t*) outputPorts [i]))
				outputBits.setBit (i);

		return outputBits;
	}

	BigInteger getActiveInputChannels() const
	{
		BigInteger inputBits;

		for (int i = 0; i < inputPorts.size(); i++)
			if (juce::jack_port_connected ((jack_port_t*) inputPorts [i]))
				inputBits.setBit (i);

		return inputBits;
	}

	int getOutputLatencyInSamples()
	{
		int latency = 0;

		for (int i = 0; i < outputPorts.size(); i++)
			latency = jmax (latency, (int) juce::jack_port_get_total_latency (client, (jack_port_t*) outputPorts [i]));

		return latency;
	}

	int getInputLatencyInSamples()
	{
		int latency = 0;

		for (int i = 0; i < inputPorts.size(); i++)
			latency = jmax (latency, (int) juce::jack_port_get_total_latency (client, (jack_port_t*) inputPorts [i]));

		return latency;
	}

	String inputId, outputId;

private:
	void process (const int numSamples)
	{
		int i, numActiveInChans = 0, numActiveOutChans = 0;

		for (i = 0; i < totalNumberOfInputChannels; ++i)
		{
			jack_default_audio_sample_t* in
				= (jack_default_audio_sample_t*) juce::jack_port_get_buffer ((jack_port_t*) inputPorts.getUnchecked(i), numSamples);

			if (in != nullptr)
				inChans [numActiveInChans++] = (float*) in;
		}

		for (i = 0; i < totalNumberOfOutputChannels; ++i)
		{
			jack_default_audio_sample_t* out
				= (jack_default_audio_sample_t*) juce::jack_port_get_buffer ((jack_port_t*) outputPorts.getUnchecked(i), numSamples);

			if (out != nullptr)
				outChans [numActiveOutChans++] = (float*) out;
		}

		const ScopedLock sl (callbackLock);

		if (callback != nullptr)
		{
			callback->audioDeviceIOCallback (const_cast <const float**> (inChans.getData()), numActiveInChans,
											 outChans, numActiveOutChans, numSamples);
		}
		else
		{
			for (i = 0; i < numActiveOutChans; ++i)
				zeromem (outChans[i], sizeof (float) * numSamples);
		}
	}

	static int processCallback (jack_nframes_t nframes, void* callbackArgument)
	{
		if (callbackArgument != 0)
			((JackAudioIODevice*) callbackArgument)->process (nframes);

		return 0;
	}

	static void threadInitCallback (void* callbackArgument)
	{
		jack_Log ("JackAudioIODevice::initialise");
	}

	static void shutdownCallback (void* callbackArgument)
	{
		jack_Log ("JackAudioIODevice::shutdown");

		JackAudioIODevice* device = (JackAudioIODevice*) callbackArgument;

		if (device != nullptr)
		{
			device->client = 0;
			device->close();
		}
	}

	static void errorCallback (const char* msg)
	{
		jack_Log ("JackAudioIODevice::errorCallback " + String (msg));
	}

	bool isOpen_;
	jack_client_t* client;
	String lastError;
	AudioIODeviceCallback* callback;
	CriticalSection callbackLock;

	HeapBlock <float*> inChans, outChans;
	int totalNumberOfInputChannels;
	int totalNumberOfOutputChannels;
	Array<void*> inputPorts, outputPorts;
};

class JackAudioIODeviceType  : public AudioIODeviceType
{
public:

	JackAudioIODeviceType()
		: AudioIODeviceType ("JACK"),
		  hasScanned (false)
	{
	}

	void scanForDevices()
	{
		hasScanned = true;
		inputNames.clear();
		inputIds.clear();
		outputNames.clear();
		outputIds.clear();

		if (juce_libjackHandle == nullptr)
		{
			juce_libjackHandle = dlopen ("libjack.so", RTLD_LAZY);

			if (juce_libjackHandle == nullptr)
				return;
		}

		// open a dummy client
		jack_status_t status;
		jack_client_t* client = juce::jack_client_open ("JuceJackDummy", JackNoStartServer, &status);

		if (client == 0)
		{
			dumpJackErrorMessage (status);
		}
		else
		{
			// scan for output devices
			const char** ports = juce::jack_get_ports (client, 0, 0, /* JackPortIsPhysical | */ JackPortIsOutput);

			if (ports != 0)
			{
				int j = 0;
				while (ports[j] != 0)
				{
					String clientName (ports[j]);
					clientName = clientName.upToFirstOccurrenceOf (":", false, false);

					if (clientName != String (JUCE_JACK_CLIENT_NAME)
						 && ! inputNames.contains (clientName))
					{
						inputNames.add (clientName);
						inputIds.add (ports [j]);
					}

					++j;
				}

				free (ports);
			}

			// scan for input devices
			ports = juce::jack_get_ports (client, 0, 0, /* JackPortIsPhysical | */ JackPortIsInput);

			if (ports != 0)
			{
				int j = 0;
				while (ports[j] != 0)
				{
					String clientName (ports[j]);
					clientName = clientName.upToFirstOccurrenceOf (":", false, false);

					if (clientName != String (JUCE_JACK_CLIENT_NAME)
						 && ! outputNames.contains (clientName))
					{
						outputNames.add (clientName);
						outputIds.add (ports [j]);
					}

					++j;
				}

				free (ports);
			}

			juce::jack_client_close (client);
		}
	}

	StringArray getDeviceNames (bool wantInputNames) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this
		return wantInputNames ? inputNames : outputNames;
	}

	int getDefaultDeviceIndex (bool forInput) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this
		return 0;
	}

	bool hasSeparateInputsAndOutputs() const    { return true; }

	int getIndexOfDevice (AudioIODevice* device, bool asInput) const
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		JackAudioIODevice* d = dynamic_cast <JackAudioIODevice*> (device);
		if (d == 0)
			return -1;

		return asInput ? inputIds.indexOf (d->inputId)
					   : outputIds.indexOf (d->outputId);
	}

	AudioIODevice* createDevice (const String& outputDeviceName,
								 const String& inputDeviceName)
	{
		jassert (hasScanned); // need to call scanForDevices() before doing this

		const int inputIndex = inputNames.indexOf (inputDeviceName);
		const int outputIndex = outputNames.indexOf (outputDeviceName);

		if (inputIndex >= 0 || outputIndex >= 0)
			return new JackAudioIODevice (outputIndex >= 0 ? outputDeviceName
														   : inputDeviceName,
										  inputIds [inputIndex],
										  outputIds [outputIndex]);

		return nullptr;
	}

private:
	StringArray inputNames, outputNames, inputIds, outputIds;
	bool hasScanned;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JackAudioIODeviceType);
};

AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_JACK()
{
	return new JackAudioIODeviceType();
}

/*** End of inlined file: juce_linux_JackAudio.cpp ***/


 #endif

 #if JUCE_USE_CDREADER

/*** Start of inlined file: juce_linux_AudioCDReader.cpp ***/
AudioCDReader::AudioCDReader()
	: AudioFormatReader (0, "CD Audio")
{
}

StringArray AudioCDReader::getAvailableCDNames()
{
	StringArray names;
	return names;
}

AudioCDReader* AudioCDReader::createReaderForCD (const int index)
{
	return nullptr;
}

AudioCDReader::~AudioCDReader()
{
}

void AudioCDReader::refreshTrackLengths()
{
}

bool AudioCDReader::readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
								 int64 startSampleInFile, int numSamples)
{
	return false;
}

bool AudioCDReader::isCDStillPresent() const
{
	return false;
}

bool AudioCDReader::isTrackAudio (int trackNum) const
{
	return false;
}

void AudioCDReader::enableIndexScanning (bool b)
{
}

int AudioCDReader::getLastIndex() const
{
	return 0;
}

Array<int> AudioCDReader::findIndexesInTrack (const int trackNumber)
{
	return Array<int>();
}

/*** End of inlined file: juce_linux_AudioCDReader.cpp ***/


 #endif

#elif JUCE_ANDROID

/*** Start of inlined file: juce_android_JNIHelpers.h ***/
#ifndef __JUCE_ANDROID_JNIHELPERS_JUCEHEADER__
#define __JUCE_ANDROID_JNIHELPERS_JUCEHEADER__

#if ! (defined (JUCE_ANDROID_ACTIVITY_CLASSNAME) && defined (JUCE_ANDROID_ACTIVITY_CLASSPATH))
 #error "The JUCE_ANDROID_ACTIVITY_CLASSNAME and JUCE_ANDROID_ACTIVITY_CLASSPATH macros must be set!"
#endif

extern JNIEnv* getEnv() noexcept;

class GlobalRef
{
public:
	inline GlobalRef() noexcept                 : obj (0) {}
	inline explicit GlobalRef (jobject obj_)    : obj (retain (obj_)) {}
	inline GlobalRef (const GlobalRef& other)   : obj (retain (other.obj)) {}
	~GlobalRef()                                { clear(); }

	inline void clear()
	{
		if (obj != 0)
		{
			getEnv()->DeleteGlobalRef (obj);
			obj = 0;
		}
	}

	inline GlobalRef& operator= (const GlobalRef& other)
	{
		jobject newObj = retain (other.obj);
		clear();
		obj = newObj;
		return *this;
	}

	inline operator jobject() const noexcept    { return obj; }
	inline jobject get() const noexcept         { return obj; }

	#define DECLARE_CALL_TYPE_METHOD(returnType, typeName) \
		returnType call##typeName##Method (jmethodID methodID, ... ) const \
		{ \
			va_list args; \
			va_start (args, methodID); \
			returnType result = getEnv()->Call##typeName##MethodV (obj, methodID, args); \
			va_end (args); \
			return result; \
		}

	DECLARE_CALL_TYPE_METHOD (jobject, Object)
	DECLARE_CALL_TYPE_METHOD (jboolean, Boolean)
	DECLARE_CALL_TYPE_METHOD (jbyte, Byte)
	DECLARE_CALL_TYPE_METHOD (jchar, Char)
	DECLARE_CALL_TYPE_METHOD (jshort, Short)
	DECLARE_CALL_TYPE_METHOD (jint, Int)
	DECLARE_CALL_TYPE_METHOD (jlong, Long)
	DECLARE_CALL_TYPE_METHOD (jfloat, Float)
	DECLARE_CALL_TYPE_METHOD (jdouble, Double)
	#undef DECLARE_CALL_TYPE_METHOD

	void callVoidMethod (jmethodID methodID, ... ) const
	{
		va_list args;
		va_start (args, methodID);
		getEnv()->CallVoidMethodV (obj, methodID, args);
		va_end (args);
	}

private:

	jobject obj;

	static inline jobject retain (jobject obj_)
	{
		return obj_ == 0 ? 0 : getEnv()->NewGlobalRef (obj_);
	}
};

template <typename JavaType>
class LocalRef
{
public:
	explicit inline LocalRef (JavaType obj_) noexcept   : obj (obj_) {}
	inline LocalRef (const LocalRef& other) noexcept    : obj (retain (other.obj)) {}
	~LocalRef()                                         { clear(); }

	void clear()
	{
		if (obj != 0)
			getEnv()->DeleteLocalRef (obj);
	}

	LocalRef& operator= (const LocalRef& other)
	{
		jobject newObj = retain (other.obj);
		clear();
		obj = newObj;
		return *this;
	}

	inline operator JavaType() const noexcept   { return obj; }
	inline JavaType get() const noexcept        { return obj; }

private:
	JavaType obj;

	static JavaType retain (JavaType obj_)
	{
		return obj_ == 0 ? 0 : (JavaType) getEnv()->NewLocalRef (obj_);
	}
};

namespace
{
	String juceString (JNIEnv* env, jstring s)
	{
		const char* const utf8 = env->GetStringUTFChars (s, nullptr);
		CharPointer_UTF8 utf8CP (utf8);
		const String result (utf8CP);
		env->ReleaseStringUTFChars (s, utf8);
		return result;
	}

	String juceString (jstring s)
	{
		return juceString (getEnv(), s);
	}

	LocalRef<jstring> javaString (const String& s)
	{
		return LocalRef<jstring> (getEnv()->NewStringUTF (s.toUTF8()));
	}

	LocalRef<jstring> javaStringFromChar (const juce_wchar c)
	{
		char utf8[8] = { 0 };
		CharPointer_UTF8 (utf8).write (c);
		return LocalRef<jstring> (getEnv()->NewStringUTF (utf8));
	}
}

class JNIClassBase
{
public:
	explicit JNIClassBase (const char* classPath_);
	virtual ~JNIClassBase();

	inline operator jclass() const noexcept { return classRef; }

	static void initialiseAllClasses (JNIEnv*);
	static void releaseAllClasses (JNIEnv*);

protected:
	virtual void initialiseFields (JNIEnv*) = 0;

	jmethodID resolveMethod (JNIEnv*, const char* methodName, const char* params);
	jmethodID resolveStaticMethod (JNIEnv*, const char* methodName, const char* params);
	jfieldID resolveField (JNIEnv*, const char* fieldName, const char* signature);
	jfieldID resolveStaticField (JNIEnv*, const char* fieldName, const char* signature);

private:
	const char* const classPath;
	jclass classRef;

	static Array<JNIClassBase*>& getClasses();
	void initialise (JNIEnv*);
	void release (JNIEnv*);

	JUCE_DECLARE_NON_COPYABLE (JNIClassBase);
};

#define CREATE_JNI_METHOD(methodID, stringName, params)         methodID = resolveMethod (env, stringName, params);
#define CREATE_JNI_STATICMETHOD(methodID, stringName, params)   methodID = resolveStaticMethod (env, stringName, params);
#define CREATE_JNI_FIELD(fieldID, stringName, signature)        fieldID  = resolveField (env, stringName, signature);
#define CREATE_JNI_STATICFIELD(fieldID, stringName, signature)  fieldID  = resolveStaticField (env, stringName, signature);
#define DECLARE_JNI_METHOD(methodID, stringName, params)        jmethodID methodID;
#define DECLARE_JNI_FIELD(fieldID, stringName, signature)       jfieldID  fieldID;

#define DECLARE_JNI_CLASS(CppClassName, javaPath) \
	class CppClassName ## _Class   : public JNIClassBase \
	{ \
	public: \
		CppClassName ## _Class() : JNIClassBase (javaPath) {} \
	\
		void initialiseFields (JNIEnv* env) \
		{ \
			JNI_CLASS_MEMBERS (CREATE_JNI_METHOD, CREATE_JNI_STATICMETHOD, CREATE_JNI_FIELD, CREATE_JNI_STATICFIELD); \
		} \
	\
		JNI_CLASS_MEMBERS (DECLARE_JNI_METHOD, DECLARE_JNI_METHOD, DECLARE_JNI_FIELD, DECLARE_JNI_FIELD); \
	}; \
	static CppClassName ## _Class CppClassName;

#define JUCE_JNI_CALLBACK(className, methodName, returnType, params) \
  extern "C" __attribute__ ((visibility("default"))) returnType JUCE_JOIN_MACRO (JUCE_JOIN_MACRO (Java_, className), _ ## methodName) params

class AndroidSystem
{
public:
	AndroidSystem();

	void initialise (JNIEnv*, jobject activity, jstring appFile, jstring appDataDir);
	void shutdown (JNIEnv*);

	GlobalRef activity;
	String appFile, appDataDir;
	int screenWidth, screenHeight;
};

extern AndroidSystem android;

class ThreadLocalJNIEnvHolder
{
public:
	ThreadLocalJNIEnvHolder()
		: jvm (nullptr)
	{
		zeromem (threads, sizeof (threads));
		zeromem (envs, sizeof (envs));
	}

	void initialise (JNIEnv* env)
	{
		// NB: the DLL can be left loaded by the JVM, so the same static
		// objects can end up being reused by subsequent runs of the app
		zeromem (threads, sizeof (threads));
		zeromem (envs, sizeof (envs));

		env->GetJavaVM (&jvm);
		addEnv (env);
	}

	JNIEnv* attach()
	{
		JNIEnv* env = nullptr;
		jvm->AttachCurrentThread (&env, nullptr);

		if (env != nullptr)
			addEnv (env);

		return env;
	}

	void detach()
	{
		jvm->DetachCurrentThread();

		const pthread_t thisThread = pthread_self();

		SpinLock::ScopedLockType sl (addRemoveLock);
		for (int i = 0; i < maxThreads; ++i)
			if (threads[i] == thisThread)
				threads[i] = 0;
	}

	JNIEnv* getOrAttach() noexcept
	{
		JNIEnv* env = get();

		if (env == nullptr)
			env = attach();

		jassert (env != nullptr);
		return env;
	}

	JNIEnv* get() const noexcept
	{
		const pthread_t thisThread = pthread_self();

		for (int i = 0; i < maxThreads; ++i)
			if (threads[i] == thisThread)
				return envs[i];

		return nullptr;
	}

	enum { maxThreads = 32 };

private:
	JavaVM* jvm;
	pthread_t threads [maxThreads];
	JNIEnv* envs [maxThreads];
	SpinLock addRemoveLock;

	void addEnv (JNIEnv* env)
	{
		SpinLock::ScopedLockType sl (addRemoveLock);

		if (get() == nullptr)
		{
			const pthread_t thisThread = pthread_self();

			for (int i = 0; i < maxThreads; ++i)
			{
				if (threads[i] == 0)
				{
					envs[i] = env;
					threads[i] = thisThread;
					return;
				}
			}
		}

		jassertfalse; // too many threads!
	}
};

extern ThreadLocalJNIEnvHolder threadLocalJNIEnvHolder;

#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD) \
 METHOD (createNewView,          "createNewView",        "(Z)L" JUCE_ANDROID_ACTIVITY_CLASSPATH "$ComponentPeerView;") \
 METHOD (deleteView,             "deleteView",           "(L" JUCE_ANDROID_ACTIVITY_CLASSPATH "$ComponentPeerView;)V") \
 METHOD (postMessage,            "postMessage",          "(J)V") \
 METHOD (finish,                 "finish",               "()V") \
 METHOD (getClipboardContent,    "getClipboardContent",  "()Ljava/lang/String;") \
 METHOD (setClipboardContent,    "setClipboardContent",  "(Ljava/lang/String;)V") \
 METHOD (excludeClipRegion,      "excludeClipRegion",    "(Landroid/graphics/Canvas;FFFF)V") \
 METHOD (renderGlyph,            "renderGlyph",          "(CLandroid/graphics/Paint;Landroid/graphics/Matrix;Landroid/graphics/Rect;)[I") \
 STATICMETHOD (createHTTPStream, "createHTTPStream",     "(Ljava/lang/String;Z[BLjava/lang/String;ILjava/lang/StringBuffer;)L" JUCE_ANDROID_ACTIVITY_CLASSPATH "$HTTPStream;") \
 METHOD (launchURL,              "launchURL",            "(Ljava/lang/String;)V") \
 METHOD (showMessageBox,         "showMessageBox",       "(Ljava/lang/String;Ljava/lang/String;J)V") \
 METHOD (showOkCancelBox,        "showOkCancelBox",      "(Ljava/lang/String;Ljava/lang/String;J)V") \
 METHOD (showYesNoCancelBox,     "showYesNoCancelBox",   "(Ljava/lang/String;Ljava/lang/String;J)V") \

DECLARE_JNI_CLASS (JuceAppActivity, JUCE_ANDROID_ACTIVITY_CLASSPATH);
#undef JNI_CLASS_MEMBERS

#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD) \
 METHOD (constructor,   "<init>",           "(I)V") \
 METHOD (setColor,      "setColor",         "(I)V") \
 METHOD (setAlpha,      "setAlpha",         "(I)V") \
 METHOD (setTypeface,   "setTypeface",      "(Landroid/graphics/Typeface;)Landroid/graphics/Typeface;") \
 METHOD (ascent,        "ascent",           "()F") \
 METHOD (descent,       "descent",          "()F") \
 METHOD (setTextSize,   "setTextSize",      "(F)V") \
 METHOD (getTextWidths, "getTextWidths",    "(Ljava/lang/String;[F)I") \
 METHOD (setTextScaleX, "setTextScaleX",    "(F)V") \
 METHOD (getTextPath,   "getTextPath",      "(Ljava/lang/String;IIFFLandroid/graphics/Path;)V") \
 METHOD (setShader,     "setShader",        "(Landroid/graphics/Shader;)Landroid/graphics/Shader;") \

DECLARE_JNI_CLASS (Paint, "android/graphics/Paint");
#undef JNI_CLASS_MEMBERS

#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD) \
 METHOD (constructor,   "<init>",    "()V") \
 METHOD (setValues,     "setValues", "([F)V") \

DECLARE_JNI_CLASS (Matrix, "android/graphics/Matrix");
#undef JNI_CLASS_MEMBERS

#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD) \
 METHOD (constructor,   "<init>",   "(IIII)V") \
 FIELD (left,           "left",     "I") \
 FIELD (right,          "right",    "I") \
 FIELD (top,            "top",      "I") \
 FIELD (bottom,         "bottom",   "I") \

DECLARE_JNI_CLASS (RectClass, "android/graphics/Rect");
#undef JNI_CLASS_MEMBERS

#endif   // __JUCE_ANDROID_JNIHELPERS_JUCEHEADER__

/*** End of inlined file: juce_android_JNIHelpers.h ***/



/*** Start of inlined file: juce_android_Audio.cpp ***/
#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD) \
 STATICMETHOD (getMinBufferSize,            "getMinBufferSize",             "(III)I") \
 STATICMETHOD (getNativeOutputSampleRate,   "getNativeOutputSampleRate",    "(I)I") \
 METHOD (constructor,   "<init>",   "(IIIIII)V") \
 METHOD (getState,      "getState", "()I") \
 METHOD (play,          "play",     "()V") \
 METHOD (stop,          "stop",     "()V") \
 METHOD (release,       "release",  "()V") \
 METHOD (flush,         "flush",    "()V") \
 METHOD (write,         "write",    "([SII)I") \

DECLARE_JNI_CLASS (AudioTrack, "android/media/AudioTrack");
#undef JNI_CLASS_MEMBERS

#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD) \
 STATICMETHOD (getMinBufferSize, "getMinBufferSize", "(III)I") \
 METHOD (constructor,       "<init>",           "(IIIII)V") \
 METHOD (getState,          "getState",         "()I") \
 METHOD (startRecording,    "startRecording",   "()V") \
 METHOD (stop,              "stop",             "()V") \
 METHOD (read,              "read",             "([SII)I") \
 METHOD (release,           "release",          "()V") \

DECLARE_JNI_CLASS (AudioRecord, "android/media/AudioRecord");
#undef JNI_CLASS_MEMBERS

enum
{
	CHANNEL_OUT_STEREO  = 12,
	CHANNEL_IN_STEREO   = 12,
	CHANNEL_IN_MONO     = 16,
	ENCODING_PCM_16BIT  = 2,
	STREAM_MUSIC        = 3,
	MODE_STREAM         = 1,
	STATE_UNINITIALIZED = 0
};

const char* const javaAudioTypeName = "Android Audio";

class AndroidAudioIODevice  : public AudioIODevice,
							  public Thread
{
public:

	AndroidAudioIODevice (const String& deviceName)
		: AudioIODevice (deviceName, javaAudioTypeName),
		  Thread ("audio"),
		  minBufferSizeOut (0), minBufferSizeIn (0), callback (0), sampleRate (0),
		  numClientInputChannels (0), numDeviceInputChannels (0), numDeviceInputChannelsAvailable (2),
		  numClientOutputChannels (0), numDeviceOutputChannels (0),
		  actualBufferSize (0), isRunning (false),
		  outputChannelBuffer (1, 1),
		  inputChannelBuffer (1, 1)
	{
		JNIEnv* env = getEnv();
		sampleRate = env->CallStaticIntMethod (AudioTrack, AudioTrack.getNativeOutputSampleRate, MODE_STREAM);

		minBufferSizeOut = (int) env->CallStaticIntMethod (AudioTrack,  AudioTrack.getMinBufferSize,  sampleRate, CHANNEL_OUT_STEREO, ENCODING_PCM_16BIT);
		minBufferSizeIn  = (int) env->CallStaticIntMethod (AudioRecord, AudioRecord.getMinBufferSize, sampleRate, CHANNEL_IN_STEREO,  ENCODING_PCM_16BIT);

		if (minBufferSizeIn <= 0)
		{
			minBufferSizeIn = env->CallStaticIntMethod (AudioRecord, AudioRecord.getMinBufferSize, sampleRate, CHANNEL_IN_MONO, ENCODING_PCM_16BIT);

			if (minBufferSizeIn > 0)
				numDeviceInputChannelsAvailable = 1;
			else
				numDeviceInputChannelsAvailable = 0;
		}

		DBG ("Audio device - min buffers: " << minBufferSizeOut << ", " << minBufferSizeIn << "; "
			  << sampleRate << " Hz; input chans: " << numDeviceInputChannelsAvailable);
	}

	~AndroidAudioIODevice()
	{
		close();
	}

	StringArray getOutputChannelNames()
	{
		StringArray s;
		s.add ("Left");
		s.add ("Right");
		return s;
	}

	StringArray getInputChannelNames()
	{
		StringArray s;

		if (numDeviceInputChannelsAvailable == 2)
		{
			s.add ("Left");
			s.add ("Right");
		}
		else if (numDeviceInputChannelsAvailable == 1)
		{
			s.add ("Audio Input");
		}

		return s;
	}

	int getNumSampleRates()             { return 1;}
	double getSampleRate (int index)    { return sampleRate; }

	int getDefaultBufferSize()          { return 2048; }
	int getNumBufferSizesAvailable()    { return 50; }

	int getBufferSizeSamples (int index)
	{
		int n = 16;
		for (int i = 0; i < index; ++i)
			n += n < 64 ? 16
						: (n < 512 ? 32
								   : (n < 1024 ? 64
											   : (n < 2048 ? 128 : 256)));

		return n;
	}

	String open (const BigInteger& inputChannels,
				 const BigInteger& outputChannels,
				 double requestedSampleRate,
				 int bufferSize)
	{
		close();

		if (sampleRate != (int) requestedSampleRate)
			return "Sample rate not allowed";

		lastError = String::empty;
		int preferredBufferSize = (bufferSize <= 0) ? getDefaultBufferSize() : bufferSize;

		numDeviceInputChannels = 0;
		numDeviceOutputChannels = 0;

		activeOutputChans = outputChannels;
		activeOutputChans.setRange (2, activeOutputChans.getHighestBit(), false);
		numClientOutputChannels = activeOutputChans.countNumberOfSetBits();

		activeInputChans = inputChannels;
		activeInputChans.setRange (2, activeInputChans.getHighestBit(), false);
		numClientInputChannels = activeInputChans.countNumberOfSetBits();

		actualBufferSize = preferredBufferSize;
		inputChannelBuffer.setSize (2, actualBufferSize);
		inputChannelBuffer.clear();
		outputChannelBuffer.setSize (2, actualBufferSize);
		outputChannelBuffer.clear();

		JNIEnv* env = getEnv();

		if (numClientOutputChannels > 0)
		{
			numDeviceOutputChannels = 2;
			outputDevice = GlobalRef (env->NewObject (AudioTrack, AudioTrack.constructor,
													  STREAM_MUSIC, sampleRate, CHANNEL_OUT_STEREO, ENCODING_PCM_16BIT,
													  (jint) (minBufferSizeOut * numDeviceOutputChannels * sizeof (int16)), MODE_STREAM));

			if (env->CallIntMethod (outputDevice, AudioTrack.getState) != STATE_UNINITIALIZED)
				isRunning = true;
			else
				outputDevice.clear(); // failed to open the device
		}

		if (numClientInputChannels > 0 && numDeviceInputChannelsAvailable > 0)
		{
			numDeviceInputChannels = jmin (numClientInputChannels, numDeviceInputChannelsAvailable);
			inputDevice = GlobalRef (env->NewObject (AudioRecord, AudioRecord.constructor,
													 0 /* (default audio source) */, sampleRate,
													 numDeviceInputChannelsAvailable > 1 ? CHANNEL_IN_STEREO : CHANNEL_IN_MONO,
													 ENCODING_PCM_16BIT,
													 (jint) (minBufferSizeIn * numDeviceInputChannels * sizeof (int16))));

			if (env->CallIntMethod (inputDevice, AudioRecord.getState) != STATE_UNINITIALIZED)
				isRunning = true;
			else
				inputDevice.clear(); // failed to open the device
		}

		if (isRunning)
		{
			if (outputDevice != nullptr)
				env->CallVoidMethod (outputDevice, AudioTrack.play);

			if (inputDevice != nullptr)
				env->CallVoidMethod (inputDevice, AudioRecord.startRecording);

			startThread (8);
		}
		else
		{
			closeDevices();
		}

		return lastError;
	}

	void close()
	{
		if (isRunning)
		{
			stopThread (2000);
			isRunning = false;
			closeDevices();
		}
	}

	int getOutputLatencyInSamples()                     { return (minBufferSizeOut * 3) / 4; }
	int getInputLatencyInSamples()                      { return (minBufferSizeIn * 3) / 4; }
	bool isOpen()                                       { return isRunning; }
	int getCurrentBufferSizeSamples()                   { return actualBufferSize; }
	int getCurrentBitDepth()                            { return 16; }
	double getCurrentSampleRate()                       { return sampleRate; }
	BigInteger getActiveOutputChannels() const          { return activeOutputChans; }
	BigInteger getActiveInputChannels() const           { return activeInputChans; }
	String getLastError()                               { return lastError; }
	bool isPlaying()                                    { return isRunning && callback != 0; }

	void start (AudioIODeviceCallback* newCallback)
	{
		if (isRunning && callback != newCallback)
		{
			if (newCallback != nullptr)
				newCallback->audioDeviceAboutToStart (this);

			const ScopedLock sl (callbackLock);
			callback = newCallback;
		}
	}

	void stop()
	{
		if (isRunning)
		{
			AudioIODeviceCallback* lastCallback;

			{
				const ScopedLock sl (callbackLock);
				lastCallback = callback;
				callback = nullptr;
			}

			if (lastCallback != nullptr)
				lastCallback->audioDeviceStopped();
		}
	}

	void run()
	{
		JNIEnv* env = getEnv();
		jshortArray audioBuffer = env->NewShortArray (actualBufferSize * jmax (numDeviceOutputChannels, numDeviceInputChannels));

		while (! threadShouldExit())
		{
			if (inputDevice != nullptr)
			{
				jint numRead = env->CallIntMethod (inputDevice, AudioRecord.read, audioBuffer, 0, actualBufferSize * numDeviceInputChannels);

				if (numRead < actualBufferSize * numDeviceInputChannels)
				{
					DBG ("Audio read under-run! " << numRead);
				}

				jshort* const src = env->GetShortArrayElements (audioBuffer, 0);

				for (int chan = 0; chan < inputChannelBuffer.getNumChannels(); ++chan)
				{
					AudioData::Pointer <AudioData::Float32, AudioData::NativeEndian, AudioData::NonInterleaved, AudioData::NonConst> d (inputChannelBuffer.getSampleData (chan));

					if (chan < numDeviceInputChannels)
					{
						AudioData::Pointer <AudioData::Int16, AudioData::NativeEndian, AudioData::Interleaved, AudioData::Const> s (src + chan, numDeviceInputChannels);
						d.convertSamples (s, actualBufferSize);
					}
					else
					{
						d.clearSamples (actualBufferSize);
					}
				}

				env->ReleaseShortArrayElements (audioBuffer, src, 0);
			}

			if (threadShouldExit())
				break;

			{
				const ScopedLock sl (callbackLock);

				if (callback != nullptr)
				{
					callback->audioDeviceIOCallback ((const float**) inputChannelBuffer.getArrayOfChannels(), numClientInputChannels,
													 outputChannelBuffer.getArrayOfChannels(), numClientOutputChannels,
													 actualBufferSize);
				}
				else
				{
					outputChannelBuffer.clear();
				}
			}

			if (outputDevice != nullptr)
			{
				if (threadShouldExit())
					break;

				jshort* const dest = env->GetShortArrayElements (audioBuffer, 0);

				for (int chan = 0; chan < numDeviceOutputChannels; ++chan)
				{
					AudioData::Pointer <AudioData::Int16, AudioData::NativeEndian, AudioData::Interleaved, AudioData::NonConst> d (dest + chan, numDeviceOutputChannels);

					const float* const sourceChanData = outputChannelBuffer.getSampleData (jmin (chan, outputChannelBuffer.getNumChannels() - 1));
					AudioData::Pointer <AudioData::Float32, AudioData::NativeEndian, AudioData::NonInterleaved, AudioData::Const> s (sourceChanData);
					d.convertSamples (s, actualBufferSize);
				}

				env->ReleaseShortArrayElements (audioBuffer, dest, 0);
				jint numWritten = env->CallIntMethod (outputDevice, AudioTrack.write, audioBuffer, 0, actualBufferSize * numDeviceOutputChannels);

				if (numWritten < actualBufferSize * numDeviceOutputChannels)
				{
					DBG ("Audio write underrun! " << numWritten);
				}
			}
		}
	}

	int minBufferSizeOut, minBufferSizeIn;

private:

	CriticalSection callbackLock;
	AudioIODeviceCallback* callback;
	jint sampleRate;
	int numClientInputChannels, numDeviceInputChannels, numDeviceInputChannelsAvailable;
	int numClientOutputChannels, numDeviceOutputChannels;
	int actualBufferSize;
	bool isRunning;
	String lastError;
	BigInteger activeOutputChans, activeInputChans;
	GlobalRef outputDevice, inputDevice;
	AudioSampleBuffer inputChannelBuffer, outputChannelBuffer;

	void closeDevices()
	{
		if (outputDevice != nullptr)
		{
			outputDevice.callVoidMethod (AudioTrack.stop);
			outputDevice.callVoidMethod (AudioTrack.release);
			outputDevice.clear();
		}

		if (inputDevice != nullptr)
		{
			inputDevice.callVoidMethod (AudioRecord.stop);
			inputDevice.callVoidMethod (AudioRecord.release);
			inputDevice.clear();
		}
	}

	JUCE_DECLARE_NON_COPYABLE (AndroidAudioIODevice);
};

class AndroidAudioIODeviceType  : public AudioIODeviceType
{
public:
	AndroidAudioIODeviceType()  : AudioIODeviceType (javaAudioTypeName) {}

	void scanForDevices() {}
	StringArray getDeviceNames (bool wantInputNames) const              { return StringArray (javaAudioTypeName); }
	int getDefaultDeviceIndex (bool forInput) const                     { return 0; }
	int getIndexOfDevice (AudioIODevice* device, bool asInput) const    { return device != nullptr ? 0 : -1; }
	bool hasSeparateInputsAndOutputs() const                            { return false; }

	AudioIODevice* createDevice (const String& outputDeviceName,
								 const String& inputDeviceName)
	{
		ScopedPointer<AndroidAudioIODevice> dev;

		if (outputDeviceName.isNotEmpty() || inputDeviceName.isNotEmpty())
		{
			dev = new AndroidAudioIODevice (outputDeviceName.isNotEmpty() ? outputDeviceName
																		  : inputDeviceName);

			if (dev->getCurrentSampleRate() <= 0 || dev->getDefaultBufferSize() <= 0)
				dev = nullptr;
		}

		return dev.release();
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndroidAudioIODeviceType);
};

extern bool isOpenSLAvailable();

AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_Android()
{
   #if JUCE_USE_ANDROID_OPENSLES
	if (isOpenSLAvailable())
		return nullptr;
   #endif

	return new AndroidAudioIODeviceType();
}

/*** End of inlined file: juce_android_Audio.cpp ***/


/*** Start of inlined file: juce_android_Midi.cpp ***/
StringArray MidiOutput::getDevices()
{
	StringArray devices;

	return devices;
}

int MidiOutput::getDefaultDeviceIndex()
{
	return 0;
}

MidiOutput* MidiOutput::openDevice (int index)
{
	return nullptr;
}

MidiOutput::~MidiOutput()
{
}

void MidiOutput::sendMessageNow (const MidiMessage&)
{
}

MidiInput::MidiInput (const String& name_)
	: name (name_),
	  internal (0)
{
}

MidiInput::~MidiInput()
{
}

void MidiInput::start()
{
}

void MidiInput::stop()
{
}

int MidiInput::getDefaultDeviceIndex()
{
	return 0;
}

StringArray MidiInput::getDevices()
{
	StringArray devs;

	return devs;
}

MidiInput* MidiInput::openDevice (int index, MidiInputCallback* callback)
{
	return nullptr;
}

/*** End of inlined file: juce_android_Midi.cpp ***/

 #if JUCE_USE_ANDROID_OPENSLES

/*** Start of inlined file: juce_android_OpenSL.cpp ***/
const char* const openSLTypeName = "Android OpenSL";

bool isOpenSLAvailable()
{
	DynamicLibrary library;
	return library.open ("libOpenSLES.so");
}

const unsigned short openSLRates[]       = { 8000, 16000, 32000, 44100, 48000 };
const unsigned short openSLBufferSizes[] = { 256, 512, 768, 1024, 1280, 1600 }; // must all be multiples of the block size

class OpenSLAudioIODevice  : public AudioIODevice,
							 public Thread
{
public:
	OpenSLAudioIODevice (const String& deviceName)
		: AudioIODevice (deviceName, openSLTypeName),
		  Thread ("OpenSL"),
		  callback (nullptr), sampleRate (0), deviceOpen (false),
		  inputBuffer (2, 2), outputBuffer (2, 2)
	{
		// OpenSL has piss-poor support for determining latency, so the only way I can find to
		// get a number for this is by asking the AudioTrack/AudioRecord classes..
		AndroidAudioIODevice javaDevice (String::empty);

		// this is a total guess about how to calculate the latency, but seems to vaguely agree
		// with the devices I've tested.. YMMV
		inputLatency  = ((javaDevice.minBufferSizeIn  * 2) / 3);
		outputLatency = ((javaDevice.minBufferSizeOut * 2) / 3);

		const int longestLatency = jmax (inputLatency, outputLatency);
		const int totalLatency = inputLatency + outputLatency;
		inputLatency  = ((longestLatency * inputLatency)  / totalLatency) & ~15;
		outputLatency = ((longestLatency * outputLatency) / totalLatency) & ~15;
	}

	~OpenSLAudioIODevice()
	{
		close();
	}

	bool openedOk() const       { return engine.outputMixObject != nullptr; }

	StringArray getOutputChannelNames()
	{
		StringArray s;
		s.add ("Left");
		s.add ("Right");
		return s;
	}

	StringArray getInputChannelNames()
	{
		StringArray s;
		s.add ("Audio Input");
		return s;
	}

	int getNumSampleRates()                 { return numElementsInArray (openSLRates); }

	double getSampleRate (int index)
	{
		jassert (index >= 0 && index < getNumSampleRates());
		return (int) openSLRates [index];
	}

	int getDefaultBufferSize()              { return 1024; }
	int getNumBufferSizesAvailable()        { return numElementsInArray (openSLBufferSizes); }

	int getBufferSizeSamples (int index)
	{
		jassert (index >= 0 && index < getNumBufferSizesAvailable());
		return (int) openSLBufferSizes [index];
	}

	String open (const BigInteger& inputChannels,
				 const BigInteger& outputChannels,
				 double requestedSampleRate,
				 int bufferSize)
	{
		close();

		lastError = String::empty;
		sampleRate = (int) requestedSampleRate;

		int preferredBufferSize = (bufferSize <= 0) ? getDefaultBufferSize() : bufferSize;

		activeOutputChans = outputChannels;
		activeOutputChans.setRange (2, activeOutputChans.getHighestBit(), false);
		numOutputChannels = activeOutputChans.countNumberOfSetBits();

		activeInputChans = inputChannels;
		activeInputChans.setRange (1, activeInputChans.getHighestBit(), false);
		numInputChannels = activeInputChans.countNumberOfSetBits();

		actualBufferSize = preferredBufferSize;

		inputBuffer.setSize (jmax (1, numInputChannels), actualBufferSize);
		outputBuffer.setSize (jmax (1, numOutputChannels), actualBufferSize);

		recorder = engine.createRecorder (numInputChannels, sampleRate);
		player   = engine.createPlayer (numOutputChannels, sampleRate);

		startThread (8);

		deviceOpen = true;
		return lastError;
	}

	void close()
	{
		stop();
		stopThread (6000);
		deviceOpen = false;
		recorder = nullptr;
		player = nullptr;
	}

	int getOutputLatencyInSamples()                     { return outputLatency; }
	int getInputLatencyInSamples()                      { return inputLatency; }
	bool isOpen()                                       { return deviceOpen; }
	int getCurrentBufferSizeSamples()                   { return actualBufferSize; }
	int getCurrentBitDepth()                            { return 16; }
	double getCurrentSampleRate()                       { return sampleRate; }
	BigInteger getActiveOutputChannels() const          { return activeOutputChans; }
	BigInteger getActiveInputChannels() const           { return activeInputChans; }
	String getLastError()                               { return lastError; }
	bool isPlaying()                                    { return callback != nullptr; }

	void start (AudioIODeviceCallback* newCallback)
	{
		stop();

		if (deviceOpen && callback != newCallback)
		{
			if (newCallback != nullptr)
				newCallback->audioDeviceAboutToStart (this);

			setCallback (newCallback);
		}
	}

	void stop()
	{
		AudioIODeviceCallback* const oldCallback = setCallback (nullptr);

		if (oldCallback != nullptr)
			oldCallback->audioDeviceStopped();
	}

	void run()
	{
		if (recorder != nullptr)    recorder->start();
		if (player != nullptr)      player->start();

		while (! threadShouldExit())
		{
			if (player != nullptr && ! threadShouldExit())
				player->writeBuffer (outputBuffer, *this);

			if (recorder != nullptr)
				recorder->readNextBlock (inputBuffer, *this);

			invokeCallback();
		}
	}

	void invokeCallback()
	{
		const ScopedLock sl (callbackLock);

		if (callback != nullptr)
		{
			callback->audioDeviceIOCallback (numInputChannels > 0 ? (const float**) inputBuffer.getArrayOfChannels() : nullptr,
											 numInputChannels,
											 numOutputChannels > 0 ? outputBuffer.getArrayOfChannels() : nullptr,
											 numOutputChannels,
											 actualBufferSize);
		}
		else
		{
			outputBuffer.clear();
		}
	}

private:

	CriticalSection callbackLock;
	AudioIODeviceCallback* callback;
	int actualBufferSize, sampleRate;
	int inputLatency, outputLatency;
	bool deviceOpen;
	String lastError;
	BigInteger activeOutputChans, activeInputChans;
	int numInputChannels, numOutputChannels;
	AudioSampleBuffer inputBuffer, outputBuffer;
	struct Player;
	struct Recorder;

	AudioIODeviceCallback* setCallback (AudioIODeviceCallback* const newCallback)
	{
		const ScopedLock sl (callbackLock);
		AudioIODeviceCallback* const oldCallback = callback;
		callback = newCallback;
		return oldCallback;
	}

	struct Engine
	{
		Engine()
			: engineObject (nullptr), engineInterface (nullptr), outputMixObject (nullptr)
		{
			if (library.open ("libOpenSLES.so"))
			{
				typedef SLresult (*CreateEngineFunc) (SLObjectItf*, SLuint32, const SLEngineOption*, SLuint32, const SLInterfaceID*, const SLboolean*);
				CreateEngineFunc createEngine = (CreateEngineFunc) library.getFunction ("slCreateEngine");

				if (createEngine != nullptr)
				{
					check (createEngine (&engineObject, 0, nullptr, 0, nullptr, nullptr));

					SLInterfaceID* SL_IID_ENGINE    = (SLInterfaceID*) library.getFunction ("SL_IID_ENGINE");
					SL_IID_ANDROIDSIMPLEBUFFERQUEUE = (SLInterfaceID*) library.getFunction ("SL_IID_ANDROIDSIMPLEBUFFERQUEUE");
					SL_IID_PLAY                     = (SLInterfaceID*) library.getFunction ("SL_IID_PLAY");
					SL_IID_RECORD                   = (SLInterfaceID*) library.getFunction ("SL_IID_RECORD");

					check ((*engineObject)->Realize (engineObject, SL_BOOLEAN_FALSE));
					check ((*engineObject)->GetInterface (engineObject, *SL_IID_ENGINE, &engineInterface));

					check ((*engineInterface)->CreateOutputMix (engineInterface, &outputMixObject, 0, nullptr, nullptr));
					check ((*outputMixObject)->Realize (outputMixObject, SL_BOOLEAN_FALSE));
				}
			}
		}

		~Engine()
		{
			if (outputMixObject != nullptr) (*outputMixObject)->Destroy (outputMixObject);
			if (engineObject != nullptr)    (*engineObject)->Destroy (engineObject);
		}

		Player* createPlayer (const int numChannels, const int sampleRate)
		{
			if (numChannels <= 0)
				return nullptr;

			ScopedPointer<Player> player (new Player (numChannels, sampleRate, *this));
			return player->openedOk() ? player.release() : nullptr;
		}

		Recorder* createRecorder (const int numChannels, const int sampleRate)
		{
			if (numChannels <= 0)
				return nullptr;

			ScopedPointer<Recorder> recorder (new Recorder (numChannels, sampleRate, *this));
			return recorder->openedOk() ? recorder.release() : nullptr;
		}

		SLObjectItf engineObject;
		SLEngineItf engineInterface;
		SLObjectItf outputMixObject;

		SLInterfaceID* SL_IID_ANDROIDSIMPLEBUFFERQUEUE;
		SLInterfaceID* SL_IID_PLAY;
		SLInterfaceID* SL_IID_RECORD;

	private:
		DynamicLibrary library;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Engine);
	};

	struct BufferList
	{
		BufferList (const int numChannels_)
			: numChannels (numChannels_), bufferSpace (numChannels_ * numSamples * numBuffers), nextBlock (0)
		{
		}

		int16* waitForFreeBuffer (Thread& threadToCheck)
		{
			while (numBlocksOut.get() == numBuffers)
			{
				dataArrived.wait (1);

				if (threadToCheck.threadShouldExit())
					return nullptr;
			}

			return getNextBuffer();
		}

		int16* getNextBuffer()
		{
			if (++nextBlock == numBuffers)
				nextBlock = 0;

			return bufferSpace + nextBlock * numChannels * numSamples;
		}

		void bufferReturned()           { --numBlocksOut; dataArrived.signal(); }
		void bufferSent()               { ++numBlocksOut; dataArrived.signal(); }

		int getBufferSizeBytes() const  { return numChannels * numSamples * sizeof (int16); }

		const int numChannels;
		enum { numSamples = 256, numBuffers = 16 };

	private:
		HeapBlock<int16> bufferSpace;
		int nextBlock;
		Atomic<int> numBlocksOut;
		WaitableEvent dataArrived;
	};

	struct Player
	{
		Player (int numChannels, int sampleRate, Engine& engine)
			: playerObject (nullptr), playerPlay (nullptr), playerBufferQueue (nullptr),
			  bufferList (numChannels)
		{
			jassert (numChannels == 2);

			SLDataFormat_PCM pcmFormat =
			{
				SL_DATAFORMAT_PCM,
				numChannels,
				sampleRate * 1000, // (sample rate units are millihertz)
				SL_PCMSAMPLEFORMAT_FIXED_16,
				SL_PCMSAMPLEFORMAT_FIXED_16,
				SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
				SL_BYTEORDER_LITTLEENDIAN
			};

			SLDataLocator_AndroidSimpleBufferQueue bufferQueue = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, bufferList.numBuffers };
			SLDataSource audioSrc = { &bufferQueue, &pcmFormat };

			SLDataLocator_OutputMix outputMix = { SL_DATALOCATOR_OUTPUTMIX, engine.outputMixObject };
			SLDataSink audioSink = { &outputMix, nullptr };

			// (SL_IID_BUFFERQUEUE is not guaranteed to remain future-proof, so use SL_IID_ANDROIDSIMPLEBUFFERQUEUE)
			const SLInterfaceID interfaceIDs[] = { *engine.SL_IID_ANDROIDSIMPLEBUFFERQUEUE };
			const SLboolean flags[] = { SL_BOOLEAN_TRUE };

			check ((*engine.engineInterface)->CreateAudioPlayer (engine.engineInterface, &playerObject, &audioSrc, &audioSink,
																 1, interfaceIDs, flags));

			check ((*playerObject)->Realize (playerObject, SL_BOOLEAN_FALSE));
			check ((*playerObject)->GetInterface (playerObject, *engine.SL_IID_PLAY, &playerPlay));
			check ((*playerObject)->GetInterface (playerObject, *engine.SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &playerBufferQueue));
			check ((*playerBufferQueue)->RegisterCallback (playerBufferQueue, staticCallback, this));
		}

		~Player()
		{
			if (playerPlay != nullptr)
				check ((*playerPlay)->SetPlayState (playerPlay, SL_PLAYSTATE_STOPPED));

			if (playerBufferQueue != nullptr)
				check ((*playerBufferQueue)->Clear (playerBufferQueue));

			if (playerObject != nullptr)
				(*playerObject)->Destroy (playerObject);
		}

		bool openedOk() const noexcept      { return playerBufferQueue != nullptr; }

		void start()
		{
			jassert (openedOk());
			check ((*playerPlay)->SetPlayState (playerPlay, SL_PLAYSTATE_PLAYING));
		}

		void writeBuffer (const AudioSampleBuffer& buffer, Thread& thread)
		{
			jassert (buffer.getNumChannels() == bufferList.numChannels);
			jassert (buffer.getNumSamples() < bufferList.numSamples * bufferList.numBuffers);

			int offset = 0;
			int numSamples = buffer.getNumSamples();

			while (numSamples > 0)
			{
				int16* const destBuffer = bufferList.waitForFreeBuffer (thread);

				if (destBuffer == nullptr)
					break;

				for (int i = 0; i < bufferList.numChannels; ++i)
				{
					typedef AudioData::Pointer <AudioData::Int16,   AudioData::LittleEndian, AudioData::Interleaved, AudioData::NonConst> DstSampleType;
					typedef AudioData::Pointer <AudioData::Float32, AudioData::NativeEndian, AudioData::NonInterleaved, AudioData::Const> SrcSampleType;

					DstSampleType dstData (destBuffer + i, bufferList.numChannels);
					SrcSampleType srcData (buffer.getSampleData (i, offset));
					dstData.convertSamples (srcData, bufferList.numSamples);
				}

				check ((*playerBufferQueue)->Enqueue (playerBufferQueue, destBuffer, bufferList.getBufferSizeBytes()));
				bufferList.bufferSent();

				numSamples -= bufferList.numSamples;
				offset += bufferList.numSamples;
			}
		}

	private:
		SLObjectItf playerObject;
		SLPlayItf playerPlay;
		SLAndroidSimpleBufferQueueItf playerBufferQueue;

		BufferList bufferList;

		static void staticCallback (SLAndroidSimpleBufferQueueItf queue, void* context)
		{
			jassert (queue == static_cast <Player*> (context)->playerBufferQueue); (void) queue;
			static_cast <Player*> (context)->bufferList.bufferReturned();
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Player);
	};

	struct Recorder
	{
		Recorder (int numChannels, int sampleRate, Engine& engine)
			: recorderObject (nullptr), recorderRecord (nullptr), recorderBufferQueue (nullptr),
			  bufferList (numChannels)
		{
			jassert (numChannels == 1); // STEREO doesn't always work!!

			SLDataFormat_PCM pcmFormat =
			{
				SL_DATAFORMAT_PCM,
				numChannels,
				sampleRate * 1000, // (sample rate units are millihertz)
				SL_PCMSAMPLEFORMAT_FIXED_16,
				SL_PCMSAMPLEFORMAT_FIXED_16,
				(numChannels == 1) ? SL_SPEAKER_FRONT_CENTER : (SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT),
				SL_BYTEORDER_LITTLEENDIAN
			};

			SLDataLocator_IODevice ioDevice = { SL_DATALOCATOR_IODEVICE, SL_IODEVICE_AUDIOINPUT, SL_DEFAULTDEVICEID_AUDIOINPUT, nullptr };
			SLDataSource audioSrc = { &ioDevice, nullptr };

			SLDataLocator_AndroidSimpleBufferQueue bufferQueue = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, bufferList.numBuffers };
			SLDataSink audioSink = { &bufferQueue, &pcmFormat };

			const SLInterfaceID interfaceIDs[] = { *engine.SL_IID_ANDROIDSIMPLEBUFFERQUEUE };
			const SLboolean flags[] = { SL_BOOLEAN_TRUE };

			if (check ((*engine.engineInterface)->CreateAudioRecorder (engine.engineInterface, &recorderObject, &audioSrc,
																	   &audioSink, 1, interfaceIDs, flags)))
			{
				if (check ((*recorderObject)->Realize (recorderObject, SL_BOOLEAN_FALSE)))
				{
					check ((*recorderObject)->GetInterface (recorderObject, *engine.SL_IID_RECORD, &recorderRecord));
					check ((*recorderObject)->GetInterface (recorderObject, *engine.SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &recorderBufferQueue));
					check ((*recorderBufferQueue)->RegisterCallback (recorderBufferQueue, staticCallback, this));
					check ((*recorderRecord)->SetRecordState (recorderRecord, SL_RECORDSTATE_STOPPED));

					for (int i = bufferList.numBuffers; --i >= 0;)
					{
						int16* const buffer = bufferList.getNextBuffer();
						jassert (buffer != nullptr);
						enqueueBuffer (buffer);
					}
				}
			}
		}

		~Recorder()
		{
			if (recorderRecord != nullptr)
				check ((*recorderRecord)->SetRecordState (recorderRecord, SL_RECORDSTATE_STOPPED));

			if (recorderBufferQueue != nullptr)
				check ((*recorderBufferQueue)->Clear (recorderBufferQueue));

			if (recorderObject != nullptr)
				(*recorderObject)->Destroy (recorderObject);
		}

		bool openedOk() const noexcept      { return recorderBufferQueue != nullptr; }

		void start()
		{
			jassert (openedOk());
			check ((*recorderRecord)->SetRecordState (recorderRecord, SL_RECORDSTATE_RECORDING));
		}

		void readNextBlock (AudioSampleBuffer& buffer, Thread& thread)
		{
			jassert (buffer.getNumChannels() == bufferList.numChannels);
			jassert (buffer.getNumSamples() < bufferList.numSamples * bufferList.numBuffers);
			jassert ((buffer.getNumSamples() % bufferList.numSamples) == 0);

			int offset = 0;
			int numSamples = buffer.getNumSamples();

			while (numSamples > 0)
			{
				int16* const srcBuffer = bufferList.waitForFreeBuffer (thread);

				if (srcBuffer == nullptr)
					break;

				for (int i = 0; i < bufferList.numChannels; ++i)
				{
					typedef AudioData::Pointer <AudioData::Float32, AudioData::NativeEndian, AudioData::NonInterleaved, AudioData::NonConst> DstSampleType;
					typedef AudioData::Pointer <AudioData::Int16,   AudioData::LittleEndian, AudioData::Interleaved, AudioData::Const> SrcSampleType;

					DstSampleType dstData (buffer.getSampleData (i, offset));
					SrcSampleType srcData (srcBuffer + i, bufferList.numChannels);
					dstData.convertSamples (srcData, bufferList.numSamples);
				}

				enqueueBuffer (srcBuffer);

				numSamples -= bufferList.numSamples;
				offset += bufferList.numSamples;
			}
		}

	private:
		SLObjectItf recorderObject;
		SLRecordItf recorderRecord;
		SLAndroidSimpleBufferQueueItf recorderBufferQueue;

		BufferList bufferList;

		void enqueueBuffer (int16* buffer)
		{
			check ((*recorderBufferQueue)->Enqueue (recorderBufferQueue, buffer, bufferList.getBufferSizeBytes()));
			bufferList.bufferSent();
		}

		static void staticCallback (SLAndroidSimpleBufferQueueItf queue, void* context)
		{
			jassert (queue == static_cast <Recorder*> (context)->recorderBufferQueue); (void) queue;
			static_cast <Recorder*> (context)->bufferList.bufferReturned();
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Recorder);
	};

	Engine engine;

	ScopedPointer<Player> player;
	ScopedPointer<Recorder> recorder;

	static bool check (const SLresult result)
	{
		jassert (result == SL_RESULT_SUCCESS);
		return result == SL_RESULT_SUCCESS;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenSLAudioIODevice);
};

class OpenSLAudioDeviceType  : public AudioIODeviceType
{
public:
	OpenSLAudioDeviceType()  : AudioIODeviceType (openSLTypeName) {}

	void scanForDevices() {}
	StringArray getDeviceNames (bool wantInputNames) const              { return StringArray (openSLTypeName); }
	int getDefaultDeviceIndex (bool forInput) const                     { return 0; }
	int getIndexOfDevice (AudioIODevice* device, bool asInput) const    { return device != nullptr ? 0 : -1; }
	bool hasSeparateInputsAndOutputs() const                            { return false; }

	AudioIODevice* createDevice (const String& outputDeviceName,
								 const String& inputDeviceName)
	{
		ScopedPointer<OpenSLAudioIODevice> dev;

		if (outputDeviceName.isNotEmpty() || inputDeviceName.isNotEmpty())
		{
			dev = new OpenSLAudioIODevice (outputDeviceName.isNotEmpty() ? outputDeviceName
																		 : inputDeviceName);
			if (! dev->openedOk())
				dev = nullptr;
		}

		return dev.release();
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenSLAudioDeviceType);
};

AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_OpenSLES()
{
	return isOpenSLAvailable() ? new OpenSLAudioDeviceType() : nullptr;
}

/*** End of inlined file: juce_android_OpenSL.cpp ***/


 #endif

#endif

}

/*** End of inlined file: juce_audio_devices.cpp ***/


#endif

