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

/*** Start of inlined file: juce_audio_processors.cpp ***/
#if defined (__JUCE_AUDIO_PROCESSORS_JUCEHEADER__) && ! JUCE_AMALGAMATED_INCLUDE
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

#include "juce_audio_processors_amalgam.h"
#include "juce_gui_extra_amalgam.h"

#if JUCE_MAC
 #if (JUCE_PLUGINHOST_VST || JUCE_PLUGINHOST_AU) \
	   || ! (defined (MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_6)
  #define Point CarbonDummyPointName // (workaround to avoid definition of "Point" by old Carbon headers)
  #define Component CarbonDummyCompName
  #include <Carbon/Carbon.h>
  #undef Point
  #undef Component
 #endif
#endif

#if JUCE_PLUGINHOST_VST && JUCE_LINUX
 #include <X11/Xlib.h>
 #include <X11/Xutil.h>
#endif

namespace juce
{

// START_AUTOINCLUDE format/*.cpp, processors/*.cpp, format_types/*.cpp,
// format_types/*.mm, scanning/*.cpp

/*** Start of inlined file: juce_AudioPluginFormat.cpp ***/
AudioPluginFormat::AudioPluginFormat() noexcept {}
AudioPluginFormat::~AudioPluginFormat() {}

/*** End of inlined file: juce_AudioPluginFormat.cpp ***/



/*** Start of inlined file: juce_AudioPluginFormatManager.cpp ***/
AudioPluginFormatManager::AudioPluginFormatManager()
{
}

AudioPluginFormatManager::~AudioPluginFormatManager()
{
	clearSingletonInstance();
}

juce_ImplementSingleton_SingleThreaded (AudioPluginFormatManager);

void AudioPluginFormatManager::addDefaultFormats()
{
   #if JUCE_DEBUG
	// you should only call this method once!
	for (int i = formats.size(); --i >= 0;)
	{
	   #if JUCE_PLUGINHOST_VST && ! (JUCE_MAC && JUCE_64BIT)
		jassert (dynamic_cast <VSTPluginFormat*> (formats[i]) == nullptr);
	   #endif

	   #if JUCE_PLUGINHOST_AU && JUCE_MAC
		jassert (dynamic_cast <AudioUnitPluginFormat*> (formats[i]) == nullptr);
	   #endif

	   #if JUCE_PLUGINHOST_DX && JUCE_WINDOWS
		jassert (dynamic_cast <DirectXPluginFormat*> (formats[i]) == nullptr);
	   #endif

	   #if JUCE_PLUGINHOST_LADSPA && JUCE_LINUX
		jassert (dynamic_cast <LADSPAPluginFormat*> (formats[i]) == nullptr);
	   #endif
	}
   #endif

   #if JUCE_PLUGINHOST_AU && JUCE_MAC
	formats.add (new AudioUnitPluginFormat());
   #endif

   #if JUCE_PLUGINHOST_VST && ! (JUCE_MAC && JUCE_64BIT)
	formats.add (new VSTPluginFormat());
   #endif

   #if JUCE_PLUGINHOST_DX && JUCE_WINDOWS
	formats.add (new DirectXPluginFormat());
   #endif

   #if JUCE_PLUGINHOST_LADSPA && JUCE_LINUX
	formats.add (new LADSPAPluginFormat());
   #endif
}

int AudioPluginFormatManager::getNumFormats()
{
	return formats.size();
}

AudioPluginFormat* AudioPluginFormatManager::getFormat (const int index)
{
	return formats [index];
}

void AudioPluginFormatManager::addFormat (AudioPluginFormat* const format)
{
	formats.add (format);
}

AudioPluginInstance* AudioPluginFormatManager::createPluginInstance (const PluginDescription& description,
																	 String& errorMessage) const
{
	AudioPluginInstance* result = nullptr;

	for (int i = 0; i < formats.size(); ++i)
	{
		result = formats.getUnchecked(i)->createInstanceFromDescription (description);

		if (result != nullptr)
			break;
	}

	if (result == nullptr)
	{
		if (! doesPluginStillExist (description))
			errorMessage = TRANS ("This plug-in file no longer exists");
		else
			errorMessage = TRANS ("This plug-in failed to load correctly");
	}

	return result;
}

bool AudioPluginFormatManager::doesPluginStillExist (const PluginDescription& description) const
{
	for (int i = 0; i < formats.size(); ++i)
		if (formats.getUnchecked(i)->getName() == description.pluginFormatName)
			return formats.getUnchecked(i)->doesPluginStillExist (description);

	return false;
}

/*** End of inlined file: juce_AudioPluginFormatManager.cpp ***/


/*** Start of inlined file: juce_AudioProcessor.cpp ***/
AudioProcessor::AudioProcessor()
	: playHead (nullptr),
	  sampleRate (0),
	  blockSize (0),
	  numInputChannels (0),
	  numOutputChannels (0),
	  latencySamples (0),
	  suspended (false),
	  nonRealtime (false)
{
}

AudioProcessor::~AudioProcessor()
{
	// ooh, nasty - the editor should have been deleted before the filter
	// that it refers to is deleted..
	jassert (activeEditor == nullptr);

   #if JUCE_DEBUG
	// This will fail if you've called beginParameterChangeGesture() for one
	// or more parameters without having made a corresponding call to endParameterChangeGesture...
	jassert (changingParams.countNumberOfSetBits() == 0);
   #endif
}

void AudioProcessor::setPlayHead (AudioPlayHead* const newPlayHead) noexcept
{
	playHead = newPlayHead;
}

void AudioProcessor::addListener (AudioProcessorListener* const newListener)
{
	const ScopedLock sl (listenerLock);
	listeners.addIfNotAlreadyThere (newListener);
}

void AudioProcessor::removeListener (AudioProcessorListener* const listenerToRemove)
{
	const ScopedLock sl (listenerLock);
	listeners.removeValue (listenerToRemove);
}

void AudioProcessor::setPlayConfigDetails (const int numIns,
										   const int numOuts,
										   const double sampleRate_,
										   const int blockSize_) noexcept
{
	numInputChannels = numIns;
	numOutputChannels = numOuts;
	sampleRate = sampleRate_;
	blockSize = blockSize_;
}

void AudioProcessor::setSpeakerArrangement (const String& inputs, const String& outputs)
{
	inputSpeakerArrangement  = inputs;
	outputSpeakerArrangement = outputs;
}

void AudioProcessor::setNonRealtime (const bool nonRealtime_) noexcept
{
	nonRealtime = nonRealtime_;
}

void AudioProcessor::setLatencySamples (const int newLatency)
{
	if (latencySamples != newLatency)
	{
		latencySamples = newLatency;
		updateHostDisplay();
	}
}

void AudioProcessor::setParameterNotifyingHost (const int parameterIndex,
												const float newValue)
{
	setParameter (parameterIndex, newValue);
	sendParamChangeMessageToListeners (parameterIndex, newValue);
}

void AudioProcessor::sendParamChangeMessageToListeners (const int parameterIndex, const float newValue)
{
	jassert (isPositiveAndBelow (parameterIndex, getNumParameters()));

	for (int i = listeners.size(); --i >= 0;)
	{
		AudioProcessorListener* l;

		{
			const ScopedLock sl (listenerLock);
			l = listeners [i];
		}

		if (l != nullptr)
			l->audioProcessorParameterChanged (this, parameterIndex, newValue);
	}
}

void AudioProcessor::beginParameterChangeGesture (int parameterIndex)
{
	jassert (isPositiveAndBelow (parameterIndex, getNumParameters()));

   #if JUCE_DEBUG
	// This means you've called beginParameterChangeGesture twice in succession without a matching
	// call to endParameterChangeGesture. That might be fine in most hosts, but better to avoid doing it.
	jassert (! changingParams [parameterIndex]);
	changingParams.setBit (parameterIndex);
   #endif

	for (int i = listeners.size(); --i >= 0;)
	{
		AudioProcessorListener* l;

		{
			const ScopedLock sl (listenerLock);
			l = listeners [i];
		}

		if (l != nullptr)
			l->audioProcessorParameterChangeGestureBegin (this, parameterIndex);
	}
}

void AudioProcessor::endParameterChangeGesture (int parameterIndex)
{
	jassert (isPositiveAndBelow (parameterIndex, getNumParameters()));

   #if JUCE_DEBUG
	// This means you've called endParameterChangeGesture without having previously called
	// endParameterChangeGesture. That might be fine in most hosts, but better to keep the
	// calls matched correctly.
	jassert (changingParams [parameterIndex]);
	changingParams.clearBit (parameterIndex);
   #endif

	for (int i = listeners.size(); --i >= 0;)
	{
		AudioProcessorListener* l;

		{
			const ScopedLock sl (listenerLock);
			l = listeners [i];
		}

		if (l != nullptr)
			l->audioProcessorParameterChangeGestureEnd (this, parameterIndex);
	}
}

void AudioProcessor::updateHostDisplay()
{
	for (int i = listeners.size(); --i >= 0;)
	{
		AudioProcessorListener* l;

		{
			const ScopedLock sl (listenerLock);
			l = listeners [i];
		}

		if (l != nullptr)
			l->audioProcessorChanged (this);
	}
}

bool AudioProcessor::isParameterAutomatable (int /*parameterIndex*/) const
{
	return true;
}

bool AudioProcessor::isMetaParameter (int /*parameterIndex*/) const
{
	return false;
}

void AudioProcessor::suspendProcessing (const bool shouldBeSuspended)
{
	const ScopedLock sl (callbackLock);
	suspended = shouldBeSuspended;
}

void AudioProcessor::reset()
{
}

void AudioProcessor::editorBeingDeleted (AudioProcessorEditor* const editor) noexcept
{
	const ScopedLock sl (callbackLock);

	if (activeEditor == editor)
		activeEditor = nullptr;
}

AudioProcessorEditor* AudioProcessor::createEditorIfNeeded()
{
	if (activeEditor != nullptr)
		return activeEditor;

	AudioProcessorEditor* const ed = createEditor();

	// You must make your hasEditor() method return a consistent result!
	jassert (hasEditor() == (ed != nullptr));

	if (ed != nullptr)
	{
		// you must give your editor comp a size before returning it..
		jassert (ed->getWidth() > 0 && ed->getHeight() > 0);

		const ScopedLock sl (callbackLock);
		activeEditor = ed;
	}

	return ed;
}

void AudioProcessor::getCurrentProgramStateInformation (juce::MemoryBlock& destData)
{
	getStateInformation (destData);
}

void AudioProcessor::setCurrentProgramStateInformation (const void* data, int sizeInBytes)
{
	setStateInformation (data, sizeInBytes);
}

// magic number to identify memory blocks that we've stored as XML
const uint32 magicXmlNumber = 0x21324356;

void AudioProcessor::copyXmlToBinary (const XmlElement& xml, juce::MemoryBlock& destData)
{
	const String xmlString (xml.createDocument (String::empty, true, false));
	const int stringLength = xmlString.getNumBytesAsUTF8();

	destData.setSize ((size_t) stringLength + 10);

	char* const d = static_cast<char*> (destData.getData());
	*(uint32*) d = ByteOrder::swapIfBigEndian ((const uint32) magicXmlNumber);
	*(uint32*) (d + 4) = ByteOrder::swapIfBigEndian ((const uint32) stringLength);

	xmlString.copyToUTF8 (d + 8, stringLength + 1);
}

XmlElement* AudioProcessor::getXmlFromBinary (const void* data,
											  const int sizeInBytes)
{
	if (sizeInBytes > 8
		 && ByteOrder::littleEndianInt (data) == magicXmlNumber)
	{
		const int stringLength = (int) ByteOrder::littleEndianInt (addBytesToPointer (data, 4));

		if (stringLength > 0)
			return XmlDocument::parse (String::fromUTF8 (static_cast<const char*> (data) + 8,
														 jmin ((sizeInBytes - 8), stringLength)));
	}

	return nullptr;
}

void AudioProcessorListener::audioProcessorParameterChangeGestureBegin (AudioProcessor*, int) {}
void AudioProcessorListener::audioProcessorParameterChangeGestureEnd (AudioProcessor*, int) {}

bool AudioPlayHead::CurrentPositionInfo::operator== (const CurrentPositionInfo& other) const noexcept
{
	return timeInSeconds == other.timeInSeconds
		&& ppqPosition == other.ppqPosition
		&& editOriginTime == other.editOriginTime
		&& ppqPositionOfLastBarStart == other.ppqPositionOfLastBarStart
		&& frameRate == other.frameRate
		&& isPlaying == other.isPlaying
		&& isRecording == other.isRecording
		&& bpm == other.bpm
		&& timeSigNumerator == other.timeSigNumerator
		&& timeSigDenominator == other.timeSigDenominator;
}

bool AudioPlayHead::CurrentPositionInfo::operator!= (const CurrentPositionInfo& other) const noexcept
{
	return ! operator== (other);
}

void AudioPlayHead::CurrentPositionInfo::resetToDefault()
{
	zerostruct (*this);
	timeSigNumerator = 4;
	timeSigDenominator = 4;
	bpm = 120;
}

/*** End of inlined file: juce_AudioProcessor.cpp ***/


/*** Start of inlined file: juce_AudioProcessorEditor.cpp ***/
AudioProcessorEditor::AudioProcessorEditor (AudioProcessor* const owner_)
	: owner (owner_)
{
	// the filter must be valid..
	jassert (owner != nullptr);
}

AudioProcessorEditor::~AudioProcessorEditor()
{
	// if this fails, then the wrapper hasn't called editorBeingDeleted() on the
	// filter for some reason..
	jassert (owner->getActiveEditor() != this);
}

/*** End of inlined file: juce_AudioProcessorEditor.cpp ***/


/*** Start of inlined file: juce_AudioProcessorGraph.cpp ***/
const int AudioProcessorGraph::midiChannelIndex = 0x1000;

namespace GraphRenderingOps
{

class AudioGraphRenderingOp
{
public:
	AudioGraphRenderingOp() {}
	virtual ~AudioGraphRenderingOp()  {}

	virtual void perform (AudioSampleBuffer& sharedBufferChans,
						  const OwnedArray <MidiBuffer>& sharedMidiBuffers,
						  const int numSamples) = 0;

	JUCE_LEAK_DETECTOR (AudioGraphRenderingOp);
};

class ClearChannelOp : public AudioGraphRenderingOp
{
public:
	ClearChannelOp (const int channelNum_)
		: channelNum (channelNum_)
	{}

	void perform (AudioSampleBuffer& sharedBufferChans, const OwnedArray <MidiBuffer>&, const int numSamples)
	{
		sharedBufferChans.clear (channelNum, 0, numSamples);
	}

private:
	const int channelNum;

	JUCE_DECLARE_NON_COPYABLE (ClearChannelOp);
};

class CopyChannelOp : public AudioGraphRenderingOp
{
public:
	CopyChannelOp (const int srcChannelNum_, const int dstChannelNum_)
		: srcChannelNum (srcChannelNum_),
		  dstChannelNum (dstChannelNum_)
	{}

	void perform (AudioSampleBuffer& sharedBufferChans, const OwnedArray <MidiBuffer>&, const int numSamples)
	{
		sharedBufferChans.copyFrom (dstChannelNum, 0, sharedBufferChans, srcChannelNum, 0, numSamples);
	}

private:
	const int srcChannelNum, dstChannelNum;

	JUCE_DECLARE_NON_COPYABLE (CopyChannelOp);
};

class AddChannelOp : public AudioGraphRenderingOp
{
public:
	AddChannelOp (const int srcChannelNum_, const int dstChannelNum_)
		: srcChannelNum (srcChannelNum_),
		  dstChannelNum (dstChannelNum_)
	{}

	void perform (AudioSampleBuffer& sharedBufferChans, const OwnedArray <MidiBuffer>&, const int numSamples)
	{
		sharedBufferChans.addFrom (dstChannelNum, 0, sharedBufferChans, srcChannelNum, 0, numSamples);
	}

private:
	const int srcChannelNum, dstChannelNum;

	JUCE_DECLARE_NON_COPYABLE (AddChannelOp);
};

class ClearMidiBufferOp : public AudioGraphRenderingOp
{
public:
	ClearMidiBufferOp (const int bufferNum_)
		: bufferNum (bufferNum_)
	{}

	void perform (AudioSampleBuffer&, const OwnedArray <MidiBuffer>& sharedMidiBuffers, const int)
	{
		sharedMidiBuffers.getUnchecked (bufferNum)->clear();
	}

private:
	const int bufferNum;

	JUCE_DECLARE_NON_COPYABLE (ClearMidiBufferOp);
};

class CopyMidiBufferOp : public AudioGraphRenderingOp
{
public:
	CopyMidiBufferOp (const int srcBufferNum_, const int dstBufferNum_)
		: srcBufferNum (srcBufferNum_),
		  dstBufferNum (dstBufferNum_)
	{}

	void perform (AudioSampleBuffer&, const OwnedArray <MidiBuffer>& sharedMidiBuffers, const int)
	{
		*sharedMidiBuffers.getUnchecked (dstBufferNum) = *sharedMidiBuffers.getUnchecked (srcBufferNum);
	}

private:
	const int srcBufferNum, dstBufferNum;

	JUCE_DECLARE_NON_COPYABLE (CopyMidiBufferOp);
};

class AddMidiBufferOp : public AudioGraphRenderingOp
{
public:
	AddMidiBufferOp (const int srcBufferNum_, const int dstBufferNum_)
		: srcBufferNum (srcBufferNum_),
		  dstBufferNum (dstBufferNum_)
	{}

	void perform (AudioSampleBuffer&, const OwnedArray <MidiBuffer>& sharedMidiBuffers, const int numSamples)
	{
		sharedMidiBuffers.getUnchecked (dstBufferNum)
			->addEvents (*sharedMidiBuffers.getUnchecked (srcBufferNum), 0, numSamples, 0);
	}

private:
	const int srcBufferNum, dstBufferNum;

	JUCE_DECLARE_NON_COPYABLE (AddMidiBufferOp);
};

class DelayChannelOp : public AudioGraphRenderingOp
{
public:
	DelayChannelOp (const int channel_, const int numSamplesDelay_)
		: channel (channel_),
		  bufferSize (numSamplesDelay_ + 1),
		  readIndex (0), writeIndex (numSamplesDelay_)
	{
		buffer.calloc ((size_t) bufferSize);
	}

	void perform (AudioSampleBuffer& sharedBufferChans, const OwnedArray <MidiBuffer>&, const int numSamples)
	{
		float* data = sharedBufferChans.getSampleData (channel, 0);

		for (int i = numSamples; --i >= 0;)
		{
			buffer [writeIndex] = *data;
			*data++ = buffer [readIndex];

			if (++readIndex  >= bufferSize) readIndex = 0;
			if (++writeIndex >= bufferSize) writeIndex = 0;
		}
	}

private:
	HeapBlock<float> buffer;
	const int channel, bufferSize;
	int readIndex, writeIndex;

	JUCE_DECLARE_NON_COPYABLE (DelayChannelOp);
};

class ProcessBufferOp : public AudioGraphRenderingOp
{
public:
	ProcessBufferOp (const AudioProcessorGraph::Node::Ptr& node_,
					 const Array <int>& audioChannelsToUse_,
					 const int totalChans_,
					 const int midiBufferToUse_)
		: node (node_),
		  processor (node_->getProcessor()),
		  audioChannelsToUse (audioChannelsToUse_),
		  totalChans (jmax (1, totalChans_)),
		  midiBufferToUse (midiBufferToUse_)
	{
		channels.calloc ((size_t) totalChans);

		while (audioChannelsToUse.size() < totalChans)
			audioChannelsToUse.add (0);
	}

	void perform (AudioSampleBuffer& sharedBufferChans, const OwnedArray <MidiBuffer>& sharedMidiBuffers, const int numSamples)
	{
		for (int i = totalChans; --i >= 0;)
			channels[i] = sharedBufferChans.getSampleData (audioChannelsToUse.getUnchecked (i), 0);

		AudioSampleBuffer buffer (channels, totalChans, numSamples);

		processor->processBlock (buffer, *sharedMidiBuffers.getUnchecked (midiBufferToUse));
	}

	const AudioProcessorGraph::Node::Ptr node;
	AudioProcessor* const processor;

private:
	Array <int> audioChannelsToUse;
	HeapBlock <float*> channels;
	int totalChans;
	int midiBufferToUse;

	JUCE_DECLARE_NON_COPYABLE (ProcessBufferOp);
};

/** Used to calculate the correct sequence of rendering ops needed, based on
	the best re-use of shared buffers at each stage.
*/
class RenderingOpSequenceCalculator
{
public:

	RenderingOpSequenceCalculator (AudioProcessorGraph& graph_,
								   const Array<void*>& orderedNodes_,
								   Array<void*>& renderingOps)
		: graph (graph_),
		  orderedNodes (orderedNodes_),
		  totalLatency (0)
	{
		nodeIds.add ((uint32) zeroNodeID); // first buffer is read-only zeros
		channels.add (0);

		midiNodeIds.add ((uint32) zeroNodeID);

		for (int i = 0; i < orderedNodes.size(); ++i)
		{
			createRenderingOpsForNode ((AudioProcessorGraph::Node*) orderedNodes.getUnchecked(i),
									   renderingOps, i);

			markAnyUnusedBuffersAsFree (i);
		}

		graph.setLatencySamples (totalLatency);
	}

	int getNumBuffersNeeded() const         { return nodeIds.size(); }
	int getNumMidiBuffersNeeded() const     { return midiNodeIds.size(); }

private:

	AudioProcessorGraph& graph;
	const Array<void*>& orderedNodes;
	Array <int> channels;
	Array <uint32> nodeIds, midiNodeIds;

	enum { freeNodeID = 0xffffffff, zeroNodeID = 0xfffffffe };

	static bool isNodeBusy (uint32 nodeID) noexcept { return nodeID != freeNodeID && nodeID != zeroNodeID; }

	Array <uint32> nodeDelayIDs;
	Array <int> nodeDelays;
	int totalLatency;

	int getNodeDelay (const uint32 nodeID) const          { return nodeDelays [nodeDelayIDs.indexOf (nodeID)]; }

	void setNodeDelay (const uint32 nodeID, const int latency)
	{
		const int index = nodeDelayIDs.indexOf (nodeID);

		if (index >= 0)
		{
			nodeDelays.set (index, latency);
		}
		else
		{
			nodeDelayIDs.add (nodeID);
			nodeDelays.add (latency);
		}
	}

	int getInputLatencyForNode (const uint32 nodeID) const
	{
		int maxLatency = 0;

		for (int i = graph.getNumConnections(); --i >= 0;)
		{
			const AudioProcessorGraph::Connection* const c = graph.getConnection (i);

			if (c->destNodeId == nodeID)
				maxLatency = jmax (maxLatency, getNodeDelay (c->sourceNodeId));
		}

		return maxLatency;
	}

	void createRenderingOpsForNode (AudioProcessorGraph::Node* const node,
									Array<void*>& renderingOps,
									const int ourRenderingIndex)
	{
		const int numIns = node->getProcessor()->getNumInputChannels();
		const int numOuts = node->getProcessor()->getNumOutputChannels();
		const int totalChans = jmax (numIns, numOuts);

		Array <int> audioChannelsToUse;
		int midiBufferToUse = -1;

		int maxLatency = getInputLatencyForNode (node->nodeId);

		for (int inputChan = 0; inputChan < numIns; ++inputChan)
		{
			// get a list of all the inputs to this node
			Array <uint32> sourceNodes;
			Array<int> sourceOutputChans;

			for (int i = graph.getNumConnections(); --i >= 0;)
			{
				const AudioProcessorGraph::Connection* const c = graph.getConnection (i);

				if (c->destNodeId == node->nodeId && c->destChannelIndex == inputChan)
				{
					sourceNodes.add (c->sourceNodeId);
					sourceOutputChans.add (c->sourceChannelIndex);
				}
			}

			int bufIndex = -1;

			if (sourceNodes.size() == 0)
			{
				// unconnected input channel

				if (inputChan >= numOuts)
				{
					bufIndex = getReadOnlyEmptyBuffer();
					jassert (bufIndex >= 0);
				}
				else
				{
					bufIndex = getFreeBuffer (false);
					renderingOps.add (new ClearChannelOp (bufIndex));
				}
			}
			else if (sourceNodes.size() == 1)
			{
				// channel with a straightforward single input..
				const uint32 srcNode = sourceNodes.getUnchecked(0);
				const int srcChan = sourceOutputChans.getUnchecked(0);

				bufIndex = getBufferContaining (srcNode, srcChan);

				if (bufIndex < 0)
				{
					// if not found, this is probably a feedback loop
					bufIndex = getReadOnlyEmptyBuffer();
					jassert (bufIndex >= 0);
				}

				if (inputChan < numOuts
					 && isBufferNeededLater (ourRenderingIndex,
											 inputChan,
											 srcNode, srcChan))
				{
					// can't mess up this channel because it's needed later by another node, so we
					// need to use a copy of it..
					const int newFreeBuffer = getFreeBuffer (false);

					renderingOps.add (new CopyChannelOp (bufIndex, newFreeBuffer));

					bufIndex = newFreeBuffer;
				}

				const int nodeDelay = getNodeDelay (srcNode);

				if (nodeDelay < maxLatency)
					renderingOps.add (new DelayChannelOp (bufIndex, maxLatency - nodeDelay));
			}
			else
			{
				// channel with a mix of several inputs..

				// try to find a re-usable channel from our inputs..
				int reusableInputIndex = -1;

				for (int i = 0; i < sourceNodes.size(); ++i)
				{
					const int sourceBufIndex = getBufferContaining (sourceNodes.getUnchecked(i),
																	sourceOutputChans.getUnchecked(i));

					if (sourceBufIndex >= 0
						&& ! isBufferNeededLater (ourRenderingIndex,
												  inputChan,
												  sourceNodes.getUnchecked(i),
												  sourceOutputChans.getUnchecked(i)))
					{
						// we've found one of our input chans that can be re-used..
						reusableInputIndex = i;
						bufIndex = sourceBufIndex;

						const int nodeDelay = getNodeDelay (sourceNodes.getUnchecked (i));
						if (nodeDelay < maxLatency)
							renderingOps.add (new DelayChannelOp (sourceBufIndex, maxLatency - nodeDelay));

						break;
					}
				}

				if (reusableInputIndex < 0)
				{
					// can't re-use any of our input chans, so get a new one and copy everything into it..
					bufIndex = getFreeBuffer (false);
					jassert (bufIndex != 0);

					const int srcIndex = getBufferContaining (sourceNodes.getUnchecked (0),
															  sourceOutputChans.getUnchecked (0));
					if (srcIndex < 0)
					{
						// if not found, this is probably a feedback loop
						renderingOps.add (new ClearChannelOp (bufIndex));
					}
					else
					{
						renderingOps.add (new CopyChannelOp (srcIndex, bufIndex));
					}

					reusableInputIndex = 0;
					const int nodeDelay = getNodeDelay (sourceNodes.getFirst());

					if (nodeDelay < maxLatency)
						renderingOps.add (new DelayChannelOp (bufIndex, maxLatency - nodeDelay));
				}

				for (int j = 0; j < sourceNodes.size(); ++j)
				{
					if (j != reusableInputIndex)
					{
						int srcIndex = getBufferContaining (sourceNodes.getUnchecked(j),
															sourceOutputChans.getUnchecked(j));
						if (srcIndex >= 0)
						{
							const int nodeDelay = getNodeDelay (sourceNodes.getUnchecked (j));

							if (nodeDelay < maxLatency)
							{
								if (! isBufferNeededLater (ourRenderingIndex, inputChan,
														   sourceNodes.getUnchecked(j),
														   sourceOutputChans.getUnchecked(j)))
								{
									renderingOps.add (new DelayChannelOp (srcIndex, maxLatency - nodeDelay));
								}
								else // buffer is reused elsewhere, can't be delayed
								{
									const int bufferToDelay = getFreeBuffer (false);
									renderingOps.add (new CopyChannelOp (srcIndex, bufferToDelay));
									renderingOps.add (new DelayChannelOp (bufferToDelay, maxLatency - nodeDelay));
									srcIndex = bufferToDelay;
								}
							}

							renderingOps.add (new AddChannelOp (srcIndex, bufIndex));
						}
					}
				}
			}

			jassert (bufIndex >= 0);
			audioChannelsToUse.add (bufIndex);

			if (inputChan < numOuts)
				markBufferAsContaining (bufIndex, node->nodeId, inputChan);
		}

		for (int outputChan = numIns; outputChan < numOuts; ++outputChan)
		{
			const int bufIndex = getFreeBuffer (false);
			jassert (bufIndex != 0);
			audioChannelsToUse.add (bufIndex);

			markBufferAsContaining (bufIndex, node->nodeId, outputChan);
		}

		// Now the same thing for midi..
		Array <uint32> midiSourceNodes;

		for (int i = graph.getNumConnections(); --i >= 0;)
		{
			const AudioProcessorGraph::Connection* const c = graph.getConnection (i);

			if (c->destNodeId == node->nodeId && c->destChannelIndex == AudioProcessorGraph::midiChannelIndex)
				midiSourceNodes.add (c->sourceNodeId);
		}

		if (midiSourceNodes.size() == 0)
		{
			// No midi inputs..
			midiBufferToUse = getFreeBuffer (true); // need to pick a buffer even if the processor doesn't use midi

			if (node->getProcessor()->acceptsMidi() || node->getProcessor()->producesMidi())
				renderingOps.add (new ClearMidiBufferOp (midiBufferToUse));
		}
		else if (midiSourceNodes.size() == 1)
		{
			// One midi input..
			midiBufferToUse = getBufferContaining (midiSourceNodes.getUnchecked(0),
												   AudioProcessorGraph::midiChannelIndex);

			if (midiBufferToUse >= 0)
			{
				if (isBufferNeededLater (ourRenderingIndex,
										 AudioProcessorGraph::midiChannelIndex,
										 midiSourceNodes.getUnchecked(0),
										 AudioProcessorGraph::midiChannelIndex))
				{
					// can't mess up this channel because it's needed later by another node, so we
					// need to use a copy of it..
					const int newFreeBuffer = getFreeBuffer (true);
					renderingOps.add (new CopyMidiBufferOp (midiBufferToUse, newFreeBuffer));
					midiBufferToUse = newFreeBuffer;
				}
			}
			else
			{
				// probably a feedback loop, so just use an empty one..
				midiBufferToUse = getFreeBuffer (true); // need to pick a buffer even if the processor doesn't use midi
			}
		}
		else
		{
			// More than one midi input being mixed..
			int reusableInputIndex = -1;

			for (int i = 0; i < midiSourceNodes.size(); ++i)
			{
				const int sourceBufIndex = getBufferContaining (midiSourceNodes.getUnchecked(i),
																AudioProcessorGraph::midiChannelIndex);

				if (sourceBufIndex >= 0
					 && ! isBufferNeededLater (ourRenderingIndex,
											   AudioProcessorGraph::midiChannelIndex,
											   midiSourceNodes.getUnchecked(i),
											   AudioProcessorGraph::midiChannelIndex))
				{
					// we've found one of our input buffers that can be re-used..
					reusableInputIndex = i;
					midiBufferToUse = sourceBufIndex;
					break;
				}
			}

			if (reusableInputIndex < 0)
			{
				// can't re-use any of our input buffers, so get a new one and copy everything into it..
				midiBufferToUse = getFreeBuffer (true);
				jassert (midiBufferToUse >= 0);

				const int srcIndex = getBufferContaining (midiSourceNodes.getUnchecked(0),
														  AudioProcessorGraph::midiChannelIndex);
				if (srcIndex >= 0)
					renderingOps.add (new CopyMidiBufferOp (srcIndex, midiBufferToUse));
				else
					renderingOps.add (new ClearMidiBufferOp (midiBufferToUse));

				reusableInputIndex = 0;
			}

			for (int j = 0; j < midiSourceNodes.size(); ++j)
			{
				if (j != reusableInputIndex)
				{
					const int srcIndex = getBufferContaining (midiSourceNodes.getUnchecked(j),
															  AudioProcessorGraph::midiChannelIndex);
					if (srcIndex >= 0)
						renderingOps.add (new AddMidiBufferOp (srcIndex, midiBufferToUse));
				}
			}
		}

		if (node->getProcessor()->producesMidi())
			markBufferAsContaining (midiBufferToUse, node->nodeId,
									AudioProcessorGraph::midiChannelIndex);

		setNodeDelay (node->nodeId, maxLatency + node->getProcessor()->getLatencySamples());

		if (numOuts == 0)
			totalLatency = maxLatency;

		renderingOps.add (new ProcessBufferOp (node, audioChannelsToUse,
											   totalChans, midiBufferToUse));
	}

	int getFreeBuffer (const bool forMidi)
	{
		if (forMidi)
		{
			for (int i = 1; i < midiNodeIds.size(); ++i)
				if (midiNodeIds.getUnchecked(i) == freeNodeID)
					return i;

			midiNodeIds.add ((uint32) freeNodeID);
			return midiNodeIds.size() - 1;
		}
		else
		{
			for (int i = 1; i < nodeIds.size(); ++i)
				if (nodeIds.getUnchecked(i) == freeNodeID)
					return i;

			nodeIds.add ((uint32) freeNodeID);
			channels.add (0);
			return nodeIds.size() - 1;
		}
	}

	int getReadOnlyEmptyBuffer() const noexcept
	{
		return 0;
	}

	int getBufferContaining (const uint32 nodeId, const int outputChannel) const noexcept
	{
		if (outputChannel == AudioProcessorGraph::midiChannelIndex)
		{
			for (int i = midiNodeIds.size(); --i >= 0;)
				if (midiNodeIds.getUnchecked(i) == nodeId)
					return i;
		}
		else
		{
			for (int i = nodeIds.size(); --i >= 0;)
				if (nodeIds.getUnchecked(i) == nodeId
					 && channels.getUnchecked(i) == outputChannel)
					return i;
		}

		return -1;
	}

	void markAnyUnusedBuffersAsFree (const int stepIndex)
	{
		int i;
		for (i = 0; i < nodeIds.size(); ++i)
		{
			if (isNodeBusy (nodeIds.getUnchecked(i))
				 && ! isBufferNeededLater (stepIndex, -1,
										   nodeIds.getUnchecked(i),
										   channels.getUnchecked(i)))
			{
				nodeIds.set (i, (uint32) freeNodeID);
			}
		}

		for (i = 0; i < midiNodeIds.size(); ++i)
		{
			if (isNodeBusy (midiNodeIds.getUnchecked(i))
				 && ! isBufferNeededLater (stepIndex, -1,
										   midiNodeIds.getUnchecked(i),
										   AudioProcessorGraph::midiChannelIndex))
			{
				midiNodeIds.set (i, (uint32) freeNodeID);
			}
		}
	}

	bool isBufferNeededLater (int stepIndexToSearchFrom,
							  int inputChannelOfIndexToIgnore,
							  const uint32 nodeId,
							  const int outputChanIndex) const
	{
		while (stepIndexToSearchFrom < orderedNodes.size())
		{
			const AudioProcessorGraph::Node* const node = (const AudioProcessorGraph::Node*) orderedNodes.getUnchecked (stepIndexToSearchFrom);

			if (outputChanIndex == AudioProcessorGraph::midiChannelIndex)
			{
				if (inputChannelOfIndexToIgnore != AudioProcessorGraph::midiChannelIndex
					 && graph.getConnectionBetween (nodeId, AudioProcessorGraph::midiChannelIndex,
													node->nodeId, AudioProcessorGraph::midiChannelIndex) != nullptr)
					return true;
			}
			else
			{
				for (int i = 0; i < node->getProcessor()->getNumInputChannels(); ++i)
					if (i != inputChannelOfIndexToIgnore
						 && graph.getConnectionBetween (nodeId, outputChanIndex,
														node->nodeId, i) != nullptr)
						return true;
			}

			inputChannelOfIndexToIgnore = -1;
			++stepIndexToSearchFrom;
		}

		return false;
	}

	void markBufferAsContaining (int bufferNum, uint32 nodeId, int outputIndex)
	{
		if (outputIndex == AudioProcessorGraph::midiChannelIndex)
		{
			jassert (bufferNum > 0 && bufferNum < midiNodeIds.size());

			midiNodeIds.set (bufferNum, nodeId);
		}
		else
		{
			jassert (bufferNum >= 0 && bufferNum < nodeIds.size());

			nodeIds.set (bufferNum, nodeId);
			channels.set (bufferNum, outputIndex);
		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RenderingOpSequenceCalculator);
};

// Holds a fast lookup table for checking which nodes are inputs to others.
class ConnectionLookupTable
{
public:
	explicit ConnectionLookupTable (const OwnedArray<AudioProcessorGraph::Connection>& connections)
	{
		for (int i = 0; i < connections.size(); ++i)
		{
			const AudioProcessorGraph::Connection* const c = connections.getUnchecked(i);

			int index;
			Entry* entry = findEntry (c->destNodeId, index);

			if (entry == nullptr)
			{
				entry = new Entry (c->destNodeId);
				entries.insert (index, entry);
			}

			entry->srcNodes.add (c->sourceNodeId);
		}
	}

	bool isAnInputTo (const uint32 possibleInputId,
					  const uint32 possibleDestinationId) const noexcept
	{
		return isAnInputToRecursive (possibleInputId, possibleDestinationId, entries.size());
	}

private:

	struct Entry
	{
		explicit Entry (const uint32 destNodeId_) noexcept : destNodeId (destNodeId_) {}

		const uint32 destNodeId;
		SortedSet<uint32> srcNodes;

		JUCE_DECLARE_NON_COPYABLE (Entry);
	};

	OwnedArray<Entry> entries;

	bool isAnInputToRecursive (const uint32 possibleInputId,
							   const uint32 possibleDestinationId,
							   int recursionCheck) const noexcept
	{
		int index;
		const Entry* const entry = findEntry (possibleDestinationId, index);

		if (entry != nullptr)
		{
			const SortedSet<uint32>& srcNodes = entry->srcNodes;

			if (srcNodes.contains (possibleInputId))
				return true;

			if (--recursionCheck >= 0)
			{
				for (int i = 0; i < srcNodes.size(); ++i)
					if (isAnInputToRecursive (possibleInputId, srcNodes.getUnchecked(i), recursionCheck))
						return true;
			}
		}

		return false;
	}

	Entry* findEntry (const uint32 destNodeId, int& insertIndex) const noexcept
	{
		Entry* result = nullptr;

		int start = 0;
		int end = entries.size();

		for (;;)
		{
			if (start >= end)
			{
				break;
			}
			else if (destNodeId == entries.getUnchecked (start)->destNodeId)
			{
				result = entries.getUnchecked (start);
				break;
			}
			else
			{
				const int halfway = (start + end) / 2;

				if (halfway == start)
				{
					if (destNodeId >= entries.getUnchecked (halfway)->destNodeId)
						++start;

					break;
				}
				else if (destNodeId >= entries.getUnchecked (halfway)->destNodeId)
					start = halfway;
				else
					end = halfway;
			}
		}

		insertIndex = start;
		return result;
	}

	JUCE_DECLARE_NON_COPYABLE (ConnectionLookupTable);
};

struct ConnectionSorter
{
	static int compareElements (const AudioProcessorGraph::Connection* const first,
								const AudioProcessorGraph::Connection* const second) noexcept
	{
		if      (first->sourceNodeId < second->sourceNodeId)                return -1;
		else if (first->sourceNodeId > second->sourceNodeId)                return 1;
		else if (first->destNodeId < second->destNodeId)                    return -1;
		else if (first->destNodeId > second->destNodeId)                    return 1;
		else if (first->sourceChannelIndex < second->sourceChannelIndex)    return -1;
		else if (first->sourceChannelIndex > second->sourceChannelIndex)    return 1;
		else if (first->destChannelIndex < second->destChannelIndex)        return -1;
		else if (first->destChannelIndex > second->destChannelIndex)        return 1;

		return 0;
	}
};

}

AudioProcessorGraph::Connection::Connection (const uint32 sourceNodeId_, const int sourceChannelIndex_,
											 const uint32 destNodeId_, const int destChannelIndex_) noexcept
	: sourceNodeId (sourceNodeId_), sourceChannelIndex (sourceChannelIndex_),
	  destNodeId (destNodeId_), destChannelIndex (destChannelIndex_)
{
}

AudioProcessorGraph::Node::Node (const uint32 nodeId_, AudioProcessor* const processor_) noexcept
	: nodeId (nodeId_),
	  processor (processor_),
	  isPrepared (false)
{
	jassert (processor_ != nullptr);
}

void AudioProcessorGraph::Node::prepare (const double sampleRate, const int blockSize,
										 AudioProcessorGraph* const graph)
{
	if (! isPrepared)
	{
		isPrepared = true;
		setParentGraph (graph);

		processor->setPlayConfigDetails (processor->getNumInputChannels(),
										 processor->getNumOutputChannels(),
										 sampleRate, blockSize);

		processor->prepareToPlay (sampleRate, blockSize);
	}
}

void AudioProcessorGraph::Node::unprepare()
{
	if (isPrepared)
	{
		isPrepared = false;
		processor->releaseResources();
	}
}

void AudioProcessorGraph::Node::setParentGraph (AudioProcessorGraph* const graph) const
{
	AudioProcessorGraph::AudioGraphIOProcessor* const ioProc
		= dynamic_cast <AudioProcessorGraph::AudioGraphIOProcessor*> (processor.get());

	if (ioProc != nullptr)
		ioProc->setParentGraph (graph);
}

AudioProcessorGraph::AudioProcessorGraph()
	: lastNodeId (0),
	  renderingBuffers (1, 1),
	  currentAudioOutputBuffer (1, 1)
{
}

AudioProcessorGraph::~AudioProcessorGraph()
{
	clearRenderingSequence();
	clear();
}

const String AudioProcessorGraph::getName() const
{
	return "Audio Graph";
}

void AudioProcessorGraph::clear()
{
	nodes.clear();
	connections.clear();
	triggerAsyncUpdate();
}

AudioProcessorGraph::Node* AudioProcessorGraph::getNodeForId (const uint32 nodeId) const
{
	for (int i = nodes.size(); --i >= 0;)
		if (nodes.getUnchecked(i)->nodeId == nodeId)
			return nodes.getUnchecked(i);

	return nullptr;
}

AudioProcessorGraph::Node* AudioProcessorGraph::addNode (AudioProcessor* const newProcessor, uint32 nodeId)
{
	if (newProcessor == nullptr)
	{
		jassertfalse;
		return nullptr;
	}

	if (nodeId == 0)
	{
		nodeId = ++lastNodeId;
	}
	else
	{
		// you can't add a node with an id that already exists in the graph..
		jassert (getNodeForId (nodeId) == nullptr);
		removeNode (nodeId);

		if (nodeId > lastNodeId)
			lastNodeId = nodeId;
	}

	Node* const n = new Node (nodeId, newProcessor);
	nodes.add (n);
	triggerAsyncUpdate();

	n->setParentGraph (this);
	return n;
}

bool AudioProcessorGraph::removeNode (const uint32 nodeId)
{
	disconnectNode (nodeId);

	for (int i = nodes.size(); --i >= 0;)
	{
		if (nodes.getUnchecked(i)->nodeId == nodeId)
		{
			nodes.getUnchecked(i)->setParentGraph (nullptr);
			nodes.remove (i);
			triggerAsyncUpdate();

			return true;
		}
	}

	return false;
}

const AudioProcessorGraph::Connection* AudioProcessorGraph::getConnectionBetween (const uint32 sourceNodeId,
																				  const int sourceChannelIndex,
																				  const uint32 destNodeId,
																				  const int destChannelIndex) const
{
	const Connection c (sourceNodeId, sourceChannelIndex, destNodeId, destChannelIndex);
	GraphRenderingOps::ConnectionSorter sorter;
	return connections [connections.indexOfSorted (sorter, &c)];
}

bool AudioProcessorGraph::isConnected (const uint32 possibleSourceNodeId,
									   const uint32 possibleDestNodeId) const
{
	for (int i = connections.size(); --i >= 0;)
	{
		const Connection* const c = connections.getUnchecked(i);

		if (c->sourceNodeId == possibleSourceNodeId
			 && c->destNodeId == possibleDestNodeId)
		{
			return true;
		}
	}

	return false;
}

bool AudioProcessorGraph::canConnect (const uint32 sourceNodeId,
									  const int sourceChannelIndex,
									  const uint32 destNodeId,
									  const int destChannelIndex) const
{
	if (sourceChannelIndex < 0
		 || destChannelIndex < 0
		 || sourceNodeId == destNodeId
		 || (destChannelIndex == midiChannelIndex) != (sourceChannelIndex == midiChannelIndex))
		return false;

	const Node* const source = getNodeForId (sourceNodeId);

	if (source == nullptr
		 || (sourceChannelIndex != midiChannelIndex && sourceChannelIndex >= source->processor->getNumOutputChannels())
		 || (sourceChannelIndex == midiChannelIndex && ! source->processor->producesMidi()))
		return false;

	const Node* const dest = getNodeForId (destNodeId);

	if (dest == nullptr
		 || (destChannelIndex != midiChannelIndex && destChannelIndex >= dest->processor->getNumInputChannels())
		 || (destChannelIndex == midiChannelIndex && ! dest->processor->acceptsMidi()))
		return false;

	return getConnectionBetween (sourceNodeId, sourceChannelIndex,
								 destNodeId, destChannelIndex) == nullptr;
}

bool AudioProcessorGraph::addConnection (const uint32 sourceNodeId,
										 const int sourceChannelIndex,
										 const uint32 destNodeId,
										 const int destChannelIndex)
{
	if (! canConnect (sourceNodeId, sourceChannelIndex, destNodeId, destChannelIndex))
		return false;

	GraphRenderingOps::ConnectionSorter sorter;
	connections.addSorted (sorter, new Connection (sourceNodeId, sourceChannelIndex,
												   destNodeId, destChannelIndex));
	triggerAsyncUpdate();
	return true;
}

void AudioProcessorGraph::removeConnection (const int index)
{
	connections.remove (index);
	triggerAsyncUpdate();
}

bool AudioProcessorGraph::removeConnection (const uint32 sourceNodeId, const int sourceChannelIndex,
											const uint32 destNodeId, const int destChannelIndex)
{
	bool doneAnything = false;

	for (int i = connections.size(); --i >= 0;)
	{
		const Connection* const c = connections.getUnchecked(i);

		if (c->sourceNodeId == sourceNodeId
			 && c->destNodeId == destNodeId
			 && c->sourceChannelIndex == sourceChannelIndex
			 && c->destChannelIndex == destChannelIndex)
		{
			removeConnection (i);
			doneAnything = true;
		}
	}

	return doneAnything;
}

bool AudioProcessorGraph::disconnectNode (const uint32 nodeId)
{
	bool doneAnything = false;

	for (int i = connections.size(); --i >= 0;)
	{
		const Connection* const c = connections.getUnchecked(i);

		if (c->sourceNodeId == nodeId || c->destNodeId == nodeId)
		{
			removeConnection (i);
			doneAnything = true;
		}
	}

	return doneAnything;
}

bool AudioProcessorGraph::isConnectionLegal (const Connection* const c) const
{
	jassert (c != nullptr);

	const Node* const source = getNodeForId (c->sourceNodeId);
	const Node* const dest   = getNodeForId (c->destNodeId);

	return source != nullptr
		&& dest != nullptr
		&& (c->sourceChannelIndex != midiChannelIndex ? isPositiveAndBelow (c->sourceChannelIndex, source->processor->getNumOutputChannels())
													  : source->processor->producesMidi())
		&& (c->destChannelIndex   != midiChannelIndex ? isPositiveAndBelow (c->destChannelIndex, dest->processor->getNumInputChannels())
													  : dest->processor->acceptsMidi());
}

bool AudioProcessorGraph::removeIllegalConnections()
{
	bool doneAnything = false;

	for (int i = connections.size(); --i >= 0;)
	{
		if (! isConnectionLegal (connections.getUnchecked(i)))
		{
			removeConnection (i);
			doneAnything = true;
		}
	}

	return doneAnything;
}

static void deleteRenderOpArray (Array<void*>& ops)
{
	for (int i = ops.size(); --i >= 0;)
		delete static_cast<GraphRenderingOps::AudioGraphRenderingOp*> (ops.getUnchecked(i));
}

void AudioProcessorGraph::clearRenderingSequence()
{
	Array<void*> oldOps;

	{
		const ScopedLock sl (renderLock);
		renderingOps.swapWithArray (oldOps);
	}

	deleteRenderOpArray (oldOps);
}

bool AudioProcessorGraph::isAnInputTo (const uint32 possibleInputId,
									   const uint32 possibleDestinationId,
									   const int recursionCheck) const
{
	if (recursionCheck > 0)
	{
		for (int i = connections.size(); --i >= 0;)
		{
			const AudioProcessorGraph::Connection* const c = connections.getUnchecked (i);

			if (c->destNodeId == possibleDestinationId
				 && (c->sourceNodeId == possibleInputId
					  || isAnInputTo (possibleInputId, c->sourceNodeId, recursionCheck - 1)))
				return true;
		}
	}

	return false;
}

void AudioProcessorGraph::buildRenderingSequence()
{
	Array<void*> newRenderingOps;
	int numRenderingBuffersNeeded = 2;
	int numMidiBuffersNeeded = 1;

	{
		MessageManagerLock mml;

		Array<void*> orderedNodes;

		{
			const GraphRenderingOps::ConnectionLookupTable table (connections);

			for (int i = 0; i < nodes.size(); ++i)
			{
				Node* const node = nodes.getUnchecked(i);

				node->prepare (getSampleRate(), getBlockSize(), this);

				int j = 0;
				for (; j < orderedNodes.size(); ++j)
					if (table.isAnInputTo (node->nodeId, ((Node*) orderedNodes.getUnchecked(j))->nodeId))
					  break;

				orderedNodes.insert (j, node);
			}
		}

		GraphRenderingOps::RenderingOpSequenceCalculator calculator (*this, orderedNodes, newRenderingOps);

		numRenderingBuffersNeeded = calculator.getNumBuffersNeeded();
		numMidiBuffersNeeded = calculator.getNumMidiBuffersNeeded();
	}

	{
		// swap over to the new rendering sequence..
		const ScopedLock sl (renderLock);

		renderingBuffers.setSize (numRenderingBuffersNeeded, getBlockSize());
		renderingBuffers.clear();

		for (int i = midiBuffers.size(); --i >= 0;)
			midiBuffers.getUnchecked(i)->clear();

		while (midiBuffers.size() < numMidiBuffersNeeded)
			midiBuffers.add (new MidiBuffer());

		renderingOps.swapWithArray (newRenderingOps);
	}

	// delete the old ones..
	deleteRenderOpArray (newRenderingOps);
}

void AudioProcessorGraph::handleAsyncUpdate()
{
	buildRenderingSequence();
}

void AudioProcessorGraph::prepareToPlay (double /*sampleRate*/, int estimatedSamplesPerBlock)
{
	currentAudioInputBuffer = nullptr;
	currentAudioOutputBuffer.setSize (jmax (1, getNumOutputChannels()), estimatedSamplesPerBlock);
	currentMidiInputBuffer = nullptr;
	currentMidiOutputBuffer.clear();

	clearRenderingSequence();
	buildRenderingSequence();
}

void AudioProcessorGraph::releaseResources()
{
	for (int i = 0; i < nodes.size(); ++i)
		nodes.getUnchecked(i)->unprepare();

	renderingBuffers.setSize (1, 1);
	midiBuffers.clear();

	currentAudioInputBuffer = nullptr;
	currentAudioOutputBuffer.setSize (1, 1);
	currentMidiInputBuffer = nullptr;
	currentMidiOutputBuffer.clear();
}

void AudioProcessorGraph::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	const int numSamples = buffer.getNumSamples();

	const ScopedLock sl (renderLock);

	currentAudioInputBuffer = &buffer;
	currentAudioOutputBuffer.setSize (jmax (1, buffer.getNumChannels()), numSamples);
	currentAudioOutputBuffer.clear();
	currentMidiInputBuffer = &midiMessages;
	currentMidiOutputBuffer.clear();

	int i;
	for (i = 0; i < renderingOps.size(); ++i)
	{
		GraphRenderingOps::AudioGraphRenderingOp* const op
			= (GraphRenderingOps::AudioGraphRenderingOp*) renderingOps.getUnchecked(i);

		op->perform (renderingBuffers, midiBuffers, numSamples);
	}

	for (i = 0; i < buffer.getNumChannels(); ++i)
		buffer.copyFrom (i, 0, currentAudioOutputBuffer, i, 0, numSamples);

	midiMessages.clear();
	midiMessages.addEvents (currentMidiOutputBuffer, 0, buffer.getNumSamples(), 0);
}

const String AudioProcessorGraph::getInputChannelName (int channelIndex) const
{
	return "Input " + String (channelIndex + 1);
}

const String AudioProcessorGraph::getOutputChannelName (int channelIndex) const
{
	return "Output " + String (channelIndex + 1);
}

bool AudioProcessorGraph::isInputChannelStereoPair (int /*index*/) const    { return true; }
bool AudioProcessorGraph::isOutputChannelStereoPair (int /*index*/) const   { return true; }
bool AudioProcessorGraph::acceptsMidi() const   { return true; }
bool AudioProcessorGraph::producesMidi() const  { return true; }
void AudioProcessorGraph::getStateInformation (juce::MemoryBlock& /*destData*/)   {}
void AudioProcessorGraph::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)   {}

AudioProcessorGraph::AudioGraphIOProcessor::AudioGraphIOProcessor (const IODeviceType type_)
	: type (type_),
	  graph (nullptr)
{
}

AudioProcessorGraph::AudioGraphIOProcessor::~AudioGraphIOProcessor()
{
}

const String AudioProcessorGraph::AudioGraphIOProcessor::getName() const
{
	switch (type)
	{
		case audioOutputNode:   return "Audio Output";
		case audioInputNode:    return "Audio Input";
		case midiOutputNode:    return "Midi Output";
		case midiInputNode:     return "Midi Input";
		default:                break;
	}

	return String::empty;
}

void AudioProcessorGraph::AudioGraphIOProcessor::fillInPluginDescription (PluginDescription& d) const
{
	d.name = getName();
	d.uid = d.name.hashCode();
	d.category = "I/O devices";
	d.pluginFormatName = "Internal";
	d.manufacturerName = "Raw Material Software";
	d.version = "1.0";
	d.isInstrument = false;

	d.numInputChannels = getNumInputChannels();
	if (type == audioOutputNode && graph != nullptr)
		d.numInputChannels = graph->getNumInputChannels();

	d.numOutputChannels = getNumOutputChannels();
	if (type == audioInputNode && graph != nullptr)
		d.numOutputChannels = graph->getNumOutputChannels();
}

void AudioProcessorGraph::AudioGraphIOProcessor::prepareToPlay (double, int)
{
	jassert (graph != nullptr);
}

void AudioProcessorGraph::AudioGraphIOProcessor::releaseResources()
{
}

void AudioProcessorGraph::AudioGraphIOProcessor::processBlock (AudioSampleBuffer& buffer,
															   MidiBuffer& midiMessages)
{
	jassert (graph != nullptr);

	switch (type)
	{
		case audioOutputNode:
		{
			for (int i = jmin (graph->currentAudioOutputBuffer.getNumChannels(),
							   buffer.getNumChannels()); --i >= 0;)
			{
				graph->currentAudioOutputBuffer.addFrom (i, 0, buffer, i, 0, buffer.getNumSamples());
			}

			break;
		}

		case audioInputNode:
		{
			for (int i = jmin (graph->currentAudioInputBuffer->getNumChannels(),
							   buffer.getNumChannels()); --i >= 0;)
			{
				buffer.copyFrom (i, 0, *graph->currentAudioInputBuffer, i, 0, buffer.getNumSamples());
			}

			break;
		}

		case midiOutputNode:
			graph->currentMidiOutputBuffer.addEvents (midiMessages, 0, buffer.getNumSamples(), 0);
			break;

		case midiInputNode:
			midiMessages.addEvents (*graph->currentMidiInputBuffer, 0, buffer.getNumSamples(), 0);
			break;

		default:
			break;
	}
}

bool AudioProcessorGraph::AudioGraphIOProcessor::acceptsMidi() const
{
	return type == midiOutputNode;
}

bool AudioProcessorGraph::AudioGraphIOProcessor::producesMidi() const
{
	return type == midiInputNode;
}

const String AudioProcessorGraph::AudioGraphIOProcessor::getInputChannelName (int channelIndex) const
{
	switch (type)
	{
		case audioOutputNode:   return "Output " + String (channelIndex + 1);
		case midiOutputNode:    return "Midi Output";
		default:                break;
	}

	return String::empty;
}

const String AudioProcessorGraph::AudioGraphIOProcessor::getOutputChannelName (int channelIndex) const
{
	switch (type)
	{
		case audioInputNode:    return "Input " + String (channelIndex + 1);
		case midiInputNode:     return "Midi Input";
		default:                break;
	}

	return String::empty;
}

bool AudioProcessorGraph::AudioGraphIOProcessor::isInputChannelStereoPair (int /*index*/) const
{
	return type == audioInputNode || type == audioOutputNode;
}

bool AudioProcessorGraph::AudioGraphIOProcessor::isOutputChannelStereoPair (int index) const
{
	return isInputChannelStereoPair (index);
}

bool AudioProcessorGraph::AudioGraphIOProcessor::isInput() const   { return type == audioInputNode  || type == midiInputNode; }
bool AudioProcessorGraph::AudioGraphIOProcessor::isOutput() const  { return type == audioOutputNode || type == midiOutputNode; }

bool AudioProcessorGraph::AudioGraphIOProcessor::hasEditor() const                  { return false; }
AudioProcessorEditor* AudioProcessorGraph::AudioGraphIOProcessor::createEditor()    { return nullptr; }

int AudioProcessorGraph::AudioGraphIOProcessor::getNumParameters()                  { return 0; }
const String AudioProcessorGraph::AudioGraphIOProcessor::getParameterName (int)     { return String::empty; }

float AudioProcessorGraph::AudioGraphIOProcessor::getParameter (int)                { return 0.0f; }
const String AudioProcessorGraph::AudioGraphIOProcessor::getParameterText (int)     { return String::empty; }
void AudioProcessorGraph::AudioGraphIOProcessor::setParameter (int, float)          { }

int AudioProcessorGraph::AudioGraphIOProcessor::getNumPrograms()                    { return 0; }
int AudioProcessorGraph::AudioGraphIOProcessor::getCurrentProgram()                 { return 0; }
void AudioProcessorGraph::AudioGraphIOProcessor::setCurrentProgram (int)            { }

const String AudioProcessorGraph::AudioGraphIOProcessor::getProgramName (int)       { return String::empty; }
void AudioProcessorGraph::AudioGraphIOProcessor::changeProgramName (int, const String&) {}

void AudioProcessorGraph::AudioGraphIOProcessor::getStateInformation (juce::MemoryBlock&) {}
void AudioProcessorGraph::AudioGraphIOProcessor::setStateInformation (const void*, int) {}

void AudioProcessorGraph::AudioGraphIOProcessor::setParentGraph (AudioProcessorGraph* const newGraph)
{
	graph = newGraph;

	if (graph != nullptr)
	{
		setPlayConfigDetails (type == audioOutputNode ? graph->getNumOutputChannels() : 0,
							  type == audioInputNode ? graph->getNumInputChannels() : 0,
							  getSampleRate(),
							  getBlockSize());

		updateHostDisplay();
	}
}

/*** End of inlined file: juce_AudioProcessorGraph.cpp ***/


/*** Start of inlined file: juce_GenericAudioProcessorEditor.cpp ***/
class ProcessorParameterPropertyComp   : public PropertyComponent,
										 public AudioProcessorListener,
										 public Timer
{
public:
	ProcessorParameterPropertyComp (const String& name, AudioProcessor& owner_, const int index_)
		: PropertyComponent (name),
		  owner (owner_),
		  index (index_),
		  paramHasChanged (false),
		  slider (owner_, index_)
	{
		startTimer (100);
		addAndMakeVisible (&slider);
		owner_.addListener (this);
	}

	~ProcessorParameterPropertyComp()
	{
		owner.removeListener (this);
	}

	void refresh()
	{
		paramHasChanged = false;
		slider.setValue (owner.getParameter (index), false);
	}

	void audioProcessorChanged (AudioProcessor*)  {}

	void audioProcessorParameterChanged (AudioProcessor*, int parameterIndex, float)
	{
		if (parameterIndex == index)
			paramHasChanged = true;
	}

	void timerCallback()
	{
		if (paramHasChanged)
		{
			refresh();
			startTimer (1000 / 50);
		}
		else
		{
			startTimer (jmin (1000 / 4, getTimerInterval() + 10));
		}
	}

private:

	class ParamSlider  : public Slider
	{
	public:
		ParamSlider (AudioProcessor& owner_, const int index_)
			: owner (owner_),
			  index (index_)
		{
			setRange (0.0, 1.0, 0.0);
			setSliderStyle (Slider::LinearBar);
			setTextBoxIsEditable (false);
			setScrollWheelEnabled (false);
		}

		void valueChanged()
		{
			const float newVal = (float) getValue();

			if (owner.getParameter (index) != newVal)
				owner.setParameter (index, newVal);
		}

		String getTextFromValue (double /*value*/)
		{
			return owner.getParameterText (index);
		}

	private:

		AudioProcessor& owner;
		const int index;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamSlider);
	};

	AudioProcessor& owner;
	const int index;
	bool volatile paramHasChanged;
	ParamSlider slider;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorParameterPropertyComp);
};

GenericAudioProcessorEditor::GenericAudioProcessorEditor (AudioProcessor* const owner_)
	: AudioProcessorEditor (owner_)
{
	jassert (owner_ != nullptr);
	setOpaque (true);

	addAndMakeVisible (&panel);

	Array <PropertyComponent*> params;

	const int numParams = owner_->getNumParameters();
	int totalHeight = 0;

	for (int i = 0; i < numParams; ++i)
	{
		String name (owner_->getParameterName (i));
		if (name.trim().isEmpty())
			name = "Unnamed";

		ProcessorParameterPropertyComp* const pc = new ProcessorParameterPropertyComp (name, *owner_, i);
		params.add (pc);
		totalHeight += pc->getPreferredHeight();
	}

	panel.addProperties (params);

	setSize (400, jlimit (25, 400, totalHeight));
}

GenericAudioProcessorEditor::~GenericAudioProcessorEditor()
{
}

void GenericAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll (Colours::white);
}

void GenericAudioProcessorEditor::resized()
{
	panel.setBounds (getLocalBounds());
}

/*** End of inlined file: juce_GenericAudioProcessorEditor.cpp ***/


/*** Start of inlined file: juce_PluginDescription.cpp ***/
PluginDescription::PluginDescription()
	: uid (0),
	  isInstrument (false),
	  numInputChannels (0),
	  numOutputChannels (0)
{
}

PluginDescription::~PluginDescription()
{
}

PluginDescription::PluginDescription (const PluginDescription& other)
	: name (other.name),
	  descriptiveName (other.descriptiveName),
	  pluginFormatName (other.pluginFormatName),
	  category (other.category),
	  manufacturerName (other.manufacturerName),
	  version (other.version),
	  fileOrIdentifier (other.fileOrIdentifier),
	  lastFileModTime (other.lastFileModTime),
	  uid (other.uid),
	  isInstrument (other.isInstrument),
	  numInputChannels (other.numInputChannels),
	  numOutputChannels (other.numOutputChannels)
{
}

PluginDescription& PluginDescription::operator= (const PluginDescription& other)
{
	name = other.name;
	descriptiveName = other.descriptiveName;
	pluginFormatName = other.pluginFormatName;
	category = other.category;
	manufacturerName = other.manufacturerName;
	version = other.version;
	fileOrIdentifier = other.fileOrIdentifier;
	uid = other.uid;
	isInstrument = other.isInstrument;
	lastFileModTime = other.lastFileModTime;
	numInputChannels = other.numInputChannels;
	numOutputChannels = other.numOutputChannels;

	return *this;
}

bool PluginDescription::isDuplicateOf (const PluginDescription& other) const
{
	return fileOrIdentifier == other.fileOrIdentifier
			&& uid == other.uid;
}

String PluginDescription::createIdentifierString() const
{
	return pluginFormatName
			+ "-" + name
			+ "-" + String::toHexString (fileOrIdentifier.hashCode())
			+ "-" + String::toHexString (uid);
}

XmlElement* PluginDescription::createXml() const
{
	XmlElement* const e = new XmlElement ("PLUGIN");
	e->setAttribute ("name", name);
	if (descriptiveName != name)
		e->setAttribute ("descriptiveName", descriptiveName);

	e->setAttribute ("format", pluginFormatName);
	e->setAttribute ("category", category);
	e->setAttribute ("manufacturer", manufacturerName);
	e->setAttribute ("version", version);
	e->setAttribute ("file", fileOrIdentifier);
	e->setAttribute ("uid", String::toHexString (uid));
	e->setAttribute ("isInstrument", isInstrument);
	e->setAttribute ("fileTime", String::toHexString (lastFileModTime.toMilliseconds()));
	e->setAttribute ("numInputs", numInputChannels);
	e->setAttribute ("numOutputs", numOutputChannels);

	return e;
}

bool PluginDescription::loadFromXml (const XmlElement& xml)
{
	if (xml.hasTagName ("PLUGIN"))
	{
		name                = xml.getStringAttribute ("name");
		descriptiveName     = xml.getStringAttribute ("descriptiveName", name);
		pluginFormatName    = xml.getStringAttribute ("format");
		category            = xml.getStringAttribute ("category");
		manufacturerName    = xml.getStringAttribute ("manufacturer");
		version             = xml.getStringAttribute ("version");
		fileOrIdentifier    = xml.getStringAttribute ("file");
		uid                 = xml.getStringAttribute ("uid").getHexValue32();
		isInstrument        = xml.getBoolAttribute ("isInstrument", false);
		lastFileModTime     = Time (xml.getStringAttribute ("fileTime").getHexValue64());
		numInputChannels    = xml.getIntAttribute ("numInputs");
		numOutputChannels   = xml.getIntAttribute ("numOutputs");

		return true;
	}

	return false;
}

/*** End of inlined file: juce_PluginDescription.cpp ***/


/*** Start of inlined file: juce_VSTPluginFormat.cpp ***/
#if JUCE_PLUGINHOST_VST

#if ! (JUCE_MAC && JUCE_64BIT)

#if JUCE_MAC && JUCE_SUPPORT_CARBON

/*** Start of inlined file: juce_mac_CarbonViewWrapperComponent.h ***/
#ifndef __JUCE_MAC_CARBONVIEWWRAPPERCOMPONENT_JUCEHEADER__
#define __JUCE_MAC_CARBONVIEWWRAPPERCOMPONENT_JUCEHEADER__

/**
	Creates a floating carbon window that can be used to hold a carbon UI.

	This is a handy class that's designed to be inlined where needed, e.g.
	in the audio plugin hosting code.
*/
class CarbonViewWrapperComponent  : public Component,
									public ComponentMovementWatcher,
									public Timer
{
public:
	CarbonViewWrapperComponent()
		: ComponentMovementWatcher (this),
		  wrapperWindow (0),
		  carbonWindow (0),
		  embeddedView (0),
		  recursiveResize (false)
	{
	}

	virtual ~CarbonViewWrapperComponent()
	{
		jassert (embeddedView == 0); // must call deleteWindow() in the subclass's destructor!
	}

	virtual HIViewRef attachView (WindowRef windowRef, HIViewRef rootView) = 0;
	virtual void removeView (HIViewRef embeddedView) = 0;
	virtual void mouseDown (int, int) {}
	virtual void paint() {}

	virtual bool getEmbeddedViewSize (int& w, int& h)
	{
		if (embeddedView == 0)
			return false;

		HIRect bounds;
		HIViewGetBounds (embeddedView, &bounds);
		w = jmax (1, roundToInt (bounds.size.width));
		h = jmax (1, roundToInt (bounds.size.height));
		return true;
	}

	void createWindow()
	{
		if (wrapperWindow == 0)
		{
			Rect r;
			r.left   = getScreenX();
			r.top    = getScreenY();
			r.right  = r.left + getWidth();
			r.bottom = r.top + getHeight();

			CreateNewWindow (kDocumentWindowClass,
							 (WindowAttributes) (kWindowStandardHandlerAttribute | kWindowCompositingAttribute
												  | kWindowNoShadowAttribute | kWindowNoTitleBarAttribute),
							 &r, &wrapperWindow);

			jassert (wrapperWindow != 0);
			if (wrapperWindow == 0)
				return;

			carbonWindow = [[NSWindow alloc] initWithWindowRef: wrapperWindow];

			[getOwnerWindow() addChildWindow: carbonWindow
									 ordered: NSWindowAbove];

			embeddedView = attachView (wrapperWindow, HIViewGetRoot (wrapperWindow));

			// Check for the plugin creating its own floating window, and if there is one,
			// we need to reparent it to make it visible..
			NSWindow* floatingChildWindow = [[carbonWindow childWindows] objectAtIndex: 0];

			if (floatingChildWindow != nil)
				[getOwnerWindow() addChildWindow: floatingChildWindow
										 ordered: NSWindowAbove];

			EventTypeSpec windowEventTypes[] =
			{
				{ kEventClassWindow, kEventWindowGetClickActivation },
				{ kEventClassWindow, kEventWindowHandleDeactivate },
				{ kEventClassWindow, kEventWindowBoundsChanging },
				{ kEventClassMouse,  kEventMouseDown },
				{ kEventClassMouse,  kEventMouseMoved },
				{ kEventClassMouse,  kEventMouseDragged },
				{ kEventClassMouse,  kEventMouseUp},
				{ kEventClassWindow, kEventWindowDrawContent },
				{ kEventClassWindow, kEventWindowShown },
				{ kEventClassWindow, kEventWindowHidden }
			};

			EventHandlerUPP upp = NewEventHandlerUPP (carbonEventCallback);
			InstallWindowEventHandler (wrapperWindow, upp,
									   sizeof (windowEventTypes) / sizeof (EventTypeSpec),
									   windowEventTypes, this, &eventHandlerRef);

			setOurSizeToEmbeddedViewSize();
			setEmbeddedWindowToOurSize();

			creationTime = Time::getCurrentTime();
		}
	}

	void deleteWindow()
	{
		removeView (embeddedView);
		embeddedView = 0;

		if (wrapperWindow != 0)
		{
			NSWindow* ownerWindow = getOwnerWindow();

			if ([[ownerWindow childWindows] count] > 0)
			{
				[ownerWindow removeChildWindow: carbonWindow];
				[carbonWindow close];
			}

			RemoveEventHandler (eventHandlerRef);
			DisposeWindow (wrapperWindow);
			wrapperWindow = 0;
		}
	}

	void setOurSizeToEmbeddedViewSize()
	{
		int w, h;
		if (getEmbeddedViewSize (w, h))
		{
			if (w != getWidth() || h != getHeight())
			{
				startTimer (50);

				setSize (w, h);
				if (getParentComponent() != nullptr)
					getParentComponent()->setSize (w, h);
			}
			else
			{
				startTimer (jlimit (50, 500, getTimerInterval() + 20));
			}
		}
		else
		{
			stopTimer();
		}
	}

	void setEmbeddedWindowToOurSize()
	{
		if (! recursiveResize)
		{
			recursiveResize = true;

			if (embeddedView != 0)
			{
				HIRect r;
				r.origin.x = 0;
				r.origin.y = 0;
				r.size.width  = (float) getWidth();
				r.size.height = (float) getHeight();
				HIViewSetFrame (embeddedView, &r);
			}

			if (wrapperWindow != 0)
			{
				Rect wr;
				wr.left   = getScreenX();
				wr.top    = getScreenY();
				wr.right  = wr.left + getWidth();
				wr.bottom = wr.top + getHeight();

				SetWindowBounds (wrapperWindow, kWindowContentRgn, &wr);
				ShowWindow (wrapperWindow);
			}

			recursiveResize = false;
		}
	}

	void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/)
	{
		setEmbeddedWindowToOurSize();
	}

	void componentPeerChanged()
	{
		deleteWindow();
		createWindow();
	}

	void componentVisibilityChanged()
	{
		if (isShowing())
			createWindow();
		else
			deleteWindow();

		setEmbeddedWindowToOurSize();
	}

	static void recursiveHIViewRepaint (HIViewRef view)
	{
		HIViewSetNeedsDisplay (view, true);
		HIViewRef child = HIViewGetFirstSubview (view);

		while (child != 0)
		{
			recursiveHIViewRepaint (child);
			child = HIViewGetNextView (child);
		}
	}

	void timerCallback()
	{
		setOurSizeToEmbeddedViewSize();

		// To avoid strange overpainting problems when the UI is first opened, we'll
		// repaint it a few times during the first second that it's on-screen..
		if ((Time::getCurrentTime() - creationTime).inMilliseconds() < 1000)
			recursiveHIViewRepaint (HIViewGetRoot (wrapperWindow));
	}

	OSStatus carbonEventHandler (EventHandlerCallRef /*nextHandlerRef*/, EventRef event)
	{
		switch (GetEventKind (event))
		{
			case kEventWindowHandleDeactivate:
				ActivateWindow (wrapperWindow, TRUE);
				return noErr;

			case kEventWindowGetClickActivation:
			{
				getTopLevelComponent()->toFront (false);
				[carbonWindow makeKeyAndOrderFront: nil];

				ClickActivationResult howToHandleClick = kActivateAndHandleClick;

				SetEventParameter (event, kEventParamClickActivation, typeClickActivationResult,
								   sizeof (ClickActivationResult), &howToHandleClick);

				HIViewSetNeedsDisplay (embeddedView, true);
				return noErr;
			}
		}

		return eventNotHandledErr;
	}

	static pascal OSStatus carbonEventCallback (EventHandlerCallRef nextHandlerRef, EventRef event, void* userData)
	{
		return ((CarbonViewWrapperComponent*) userData)->carbonEventHandler (nextHandlerRef, event);
	}

protected:
	WindowRef wrapperWindow;
	NSWindow* carbonWindow;
	HIViewRef embeddedView;
	bool recursiveResize;
	Time creationTime;

	EventHandlerRef eventHandlerRef;

	NSWindow* getOwnerWindow() const    { return [((NSView*) getWindowHandle()) window]; }
};

#endif   // __JUCE_MAC_CARBONVIEWWRAPPERCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_mac_CarbonViewWrapperComponent.h ***/


#endif

#if JUCE_MAC
 static bool makeFSRefFromPath (FSRef* destFSRef, const String& path)
 {
	 return FSPathMakeRef (reinterpret_cast <const UInt8*> (path.toUTF8().getAddress()), destFSRef, 0) == noErr;
 }
#endif

#undef PRAGMA_ALIGN_SUPPORTED
#define VST_FORCE_DEPRECATED 0

#if JUCE_MSVC
 #pragma warning (push)
 #pragma warning (disable: 4996)
#else
 #define __cdecl
#endif

/*  Obviously you're going to need the Steinberg vstsdk2.4 folder in
	your include path if you want to add VST support.

	If you're not interested in VSTs, you can disable them by setting the
	JUCE_PLUGINHOST_VST flag to 0.
*/
#include <pluginterfaces/vst2.x/aeffectx.h>

#if JUCE_MSVC
 #pragma warning (pop)
 #pragma warning (disable: 4355) // ("this" used in initialiser list warning)
#endif

#if ! JUCE_WINDOWS
 static void _fpreset() {}
 static void _clearfp() {}
#endif

const int fxbVersionNum = 1;

struct fxProgram
{
	long chunkMagic;        // 'CcnK'
	long byteSize;          // of this chunk, excl. magic + byteSize
	long fxMagic;           // 'FxCk'
	long version;
	long fxID;              // fx unique id
	long fxVersion;
	long numParams;
	char prgName[28];
	float params[1];        // variable no. of parameters
};

struct fxSet
{
	long chunkMagic;        // 'CcnK'
	long byteSize;          // of this chunk, excl. magic + byteSize
	long fxMagic;           // 'FxBk'
	long version;
	long fxID;              // fx unique id
	long fxVersion;
	long numPrograms;
	char future[128];
	fxProgram programs[1];  // variable no. of programs
};

struct fxChunkSet
{
	long chunkMagic;        // 'CcnK'
	long byteSize;          // of this chunk, excl. magic + byteSize
	long fxMagic;           // 'FxCh', 'FPCh', or 'FBCh'
	long version;
	long fxID;              // fx unique id
	long fxVersion;
	long numPrograms;
	char future[128];
	long chunkSize;
	char chunk[8];          // variable
};

struct fxProgramSet
{
	long chunkMagic;        // 'CcnK'
	long byteSize;          // of this chunk, excl. magic + byteSize
	long fxMagic;           // 'FxCh', 'FPCh', or 'FBCh'
	long version;
	long fxID;              // fx unique id
	long fxVersion;
	long numPrograms;
	char name[28];
	long chunkSize;
	char chunk[8];          // variable
};

namespace
{
	long vst_swap (const long x) noexcept
	{
	  #ifdef JUCE_LITTLE_ENDIAN
		return (long) ByteOrder::swap ((uint32) x);
	  #else
		return x;
	  #endif
	}

	float vst_swapFloat (const float x) noexcept
	{
	  #ifdef JUCE_LITTLE_ENDIAN
		union { uint32 asInt; float asFloat; } n;
		n.asFloat = x;
		n.asInt = ByteOrder::swap (n.asInt);
		return n.asFloat;
	  #else
		return x;
	  #endif
	}

	double getVSTHostTimeNanoseconds()
	{
	  #if JUCE_WINDOWS
		return timeGetTime() * 1000000.0;
	  #elif JUCE_LINUX
		timeval micro;
		gettimeofday (&micro, 0);
		return micro.tv_usec * 1000.0;
	  #elif JUCE_MAC
		UnsignedWide micro;
		Microseconds (&micro);
		return micro.lo * 1000.0;
	  #endif
	}
}

typedef AEffect* (VSTCALLBACK *MainCall) (audioMasterCallback);

static VstIntPtr VSTCALLBACK audioMaster (AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);

static int shellUIDToCreate = 0;
static int insideVSTCallback = 0;

class IdleCallRecursionPreventer
{
public:
	IdleCallRecursionPreventer()
		: isMessageThread (MessageManager::getInstance()->isThisTheMessageThread())
	{
		if (isMessageThread)
			++insideVSTCallback;
	}

	~IdleCallRecursionPreventer()
	{
		if (isMessageThread)
			--insideVSTCallback;
	}

private:
	const bool isMessageThread;

	JUCE_DECLARE_NON_COPYABLE (IdleCallRecursionPreventer);
};

class VSTPluginWindow;

// Change this to disable logging of various VST activities
#ifndef VST_LOGGING
  #define VST_LOGGING 1
#endif

#if VST_LOGGING
 #define log(a) Logger::writeToLog(a);
#else
 #define log(a)
#endif

#if JUCE_MAC && JUCE_PPC
static void* NewCFMFromMachO (void* const machofp) noexcept
{
	void* result = (void*) new char[8];

	((void**) result)[0] = machofp;
	((void**) result)[1] = result;

	return result;
}
#endif

#if JUCE_LINUX

extern Display* display;
extern XContext windowHandleXContext;

typedef void (*EventProcPtr) (XEvent* ev);

static bool xErrorTriggered;

namespace
{
	int temporaryErrorHandler (Display*, XErrorEvent*)
	{
		xErrorTriggered = true;
		return 0;
	}

	int getPropertyFromXWindow (Window handle, Atom atom)
	{
		XErrorHandler oldErrorHandler = XSetErrorHandler (temporaryErrorHandler);
		xErrorTriggered = false;

		int userSize;
		unsigned long bytes, userCount;
		unsigned char* data;
		Atom userType;

		XGetWindowProperty (display, handle, atom, 0, 1, false, AnyPropertyType,
							&userType,  &userSize, &userCount, &bytes, &data);

		XSetErrorHandler (oldErrorHandler);

		return (userCount == 1 && ! xErrorTriggered) ? *reinterpret_cast<int*> (data)
													 : 0;
	}

	Window getChildWindow (Window windowToCheck)
	{
		Window rootWindow, parentWindow;
		Window* childWindows;
		unsigned int numChildren;

		XQueryTree (display,
					windowToCheck,
					&rootWindow,
					&parentWindow,
					&childWindows,
					&numChildren);

		if (numChildren > 0)
			return childWindows [0];

		return 0;
	}

	void translateJuceToXButtonModifiers (const MouseEvent& e, XEvent& ev) noexcept
	{
		if (e.mods.isLeftButtonDown())
		{
			ev.xbutton.button = Button1;
			ev.xbutton.state |= Button1Mask;
		}
		else if (e.mods.isRightButtonDown())
		{
			ev.xbutton.button = Button3;
			ev.xbutton.state |= Button3Mask;
		}
		else if (e.mods.isMiddleButtonDown())
		{
			ev.xbutton.button = Button2;
			ev.xbutton.state |= Button2Mask;
		}
	}

	void translateJuceToXMotionModifiers (const MouseEvent& e, XEvent& ev) noexcept
	{
		if (e.mods.isLeftButtonDown())          ev.xmotion.state |= Button1Mask;
		else if (e.mods.isRightButtonDown())    ev.xmotion.state |= Button3Mask;
		else if (e.mods.isMiddleButtonDown())   ev.xmotion.state |= Button2Mask;
	}

	void translateJuceToXCrossingModifiers (const MouseEvent& e, XEvent& ev) noexcept
	{
		if (e.mods.isLeftButtonDown())          ev.xcrossing.state |= Button1Mask;
		else if (e.mods.isRightButtonDown())    ev.xcrossing.state |= Button3Mask;
		else if (e.mods.isMiddleButtonDown())   ev.xcrossing.state |= Button2Mask;
	}

	void translateJuceToXMouseWheelModifiers (const MouseEvent& e, const float increment, XEvent& ev) noexcept
	{
		if (increment < 0)
		{
			ev.xbutton.button = Button5;
			ev.xbutton.state |= Button5Mask;
		}
		else if (increment > 0)
		{
			ev.xbutton.button = Button4;
			ev.xbutton.state |= Button4Mask;
		}
	}
}

#endif

class ModuleHandle    : public ReferenceCountedObject
{
public:

	File file;
	MainCall moduleMain;
	String pluginName;

	static Array <ModuleHandle*>& getActiveModules()
	{
		static Array <ModuleHandle*> activeModules;
		return activeModules;
	}

	static ModuleHandle* findOrCreateModule (const File& file)
	{
		for (int i = getActiveModules().size(); --i >= 0;)
		{
			ModuleHandle* const module = getActiveModules().getUnchecked(i);

			if (module->file == file)
				return module;
		}

		_fpreset(); // (doesn't do any harm)

		const IdleCallRecursionPreventer icrp;
		shellUIDToCreate = 0;

		log ("Attempting to load VST: " + file.getFullPathName());

		ScopedPointer <ModuleHandle> m (new ModuleHandle (file));

		if (! m->open())
			m = nullptr;

		_fpreset(); // (doesn't do any harm)

		return m.release();
	}

	ModuleHandle (const File& file_)
		: file (file_),
		  moduleMain (0)
		 #if JUCE_MAC
		  , fragId (0), resHandle (0), bundleRef (0), resFileId (0)
		 #endif
	{
		getActiveModules().add (this);

	   #if JUCE_WINDOWS || JUCE_LINUX
		fullParentDirectoryPathName = file_.getParentDirectory().getFullPathName();
	   #elif JUCE_MAC
		FSRef ref;
		makeFSRefFromPath (&ref, file_.getParentDirectory().getFullPathName());
		FSGetCatalogInfo (&ref, kFSCatInfoNone, 0, 0, &parentDirFSSpec, 0);
	   #endif
	}

	~ModuleHandle()
	{
		getActiveModules().removeValue (this);
		close();
	}

#if JUCE_WINDOWS || JUCE_LINUX
	DynamicLibrary module;
	String fullParentDirectoryPathName;

	bool open()
	{
	   #if JUCE_WINDOWS
		static bool timePeriodSet = false;

		if (! timePeriodSet)
		{
			timePeriodSet = true;
			timeBeginPeriod (2);
		}
	   #endif

		pluginName = file.getFileNameWithoutExtension();

		module.open (file.getFullPathName());

		moduleMain = (MainCall) module.getFunction ("VSTPluginMain");

		if (moduleMain == nullptr)
			moduleMain = (MainCall) module.getFunction ("main");

		return moduleMain != nullptr;
	}

	void close()
	{
		_fpreset(); // (doesn't do any harm)

		module.close();
	}

	void closeEffect (AEffect* eff)
	{
		eff->dispatcher (eff, effClose, 0, 0, 0, 0);
	}

#else
	CFragConnectionID fragId;
	Handle resHandle;
	CFBundleRef bundleRef;
	FSSpec parentDirFSSpec;
	short resFileId;

	bool open()
	{
		bool ok = false;
		const String filename (file.getFullPathName());

		if (file.hasFileExtension (".vst"))
		{
			const char* const utf8 = filename.toUTF8().getAddress();
			CFURLRef url = CFURLCreateFromFileSystemRepresentation (0, (const UInt8*) utf8,
																	strlen (utf8), file.isDirectory());

			if (url != 0)
			{
				bundleRef = CFBundleCreate (kCFAllocatorDefault, url);
				CFRelease (url);

				if (bundleRef != 0)
				{
					if (CFBundleLoadExecutable (bundleRef))
					{
						moduleMain = (MainCall) CFBundleGetFunctionPointerForName (bundleRef, CFSTR("main_macho"));

						if (moduleMain == 0)
							moduleMain = (MainCall) CFBundleGetFunctionPointerForName (bundleRef, CFSTR("VSTPluginMain"));

						if (moduleMain != 0)
						{
							CFTypeRef name = CFBundleGetValueForInfoDictionaryKey (bundleRef, CFSTR("CFBundleName"));

							if (name != 0)
							{
								if (CFGetTypeID (name) == CFStringGetTypeID())
								{
									char buffer[1024];

									if (CFStringGetCString ((CFStringRef) name, buffer, sizeof (buffer), CFStringGetSystemEncoding()))
										pluginName = buffer;
								}
							}

							if (pluginName.isEmpty())
								pluginName = file.getFileNameWithoutExtension();

							resFileId = CFBundleOpenBundleResourceMap (bundleRef);

							ok = true;
						}
					}

					if (! ok)
					{
						CFBundleUnloadExecutable (bundleRef);
						CFRelease (bundleRef);
						bundleRef = 0;
					}
				}
			}
		}
#if JUCE_PPC
		else
		{
			FSRef fn;

			if (FSPathMakeRef ((UInt8*) filename.toUTF8().getAddress(), &fn, 0) == noErr)
			{
				resFileId = FSOpenResFile (&fn, fsRdPerm);

				if (resFileId != -1)
				{
					const int numEffs = Count1Resources ('aEff');

					for (int i = 0; i < numEffs; ++i)
					{
						resHandle = Get1IndResource ('aEff', i + 1);

						if (resHandle != 0)
						{
							OSType type;
							Str255 name;
							SInt16 id;
							GetResInfo (resHandle, &id, &type, name);
							pluginName = String ((const char*) name + 1, name[0]);
							DetachResource (resHandle);
							HLock (resHandle);

							Ptr ptr;
							Str255 errorText;

							OSErr err = GetMemFragment (*resHandle, GetHandleSize (resHandle),
														name, kPrivateCFragCopy,
														&fragId, &ptr, errorText);

							if (err == noErr)
							{
								moduleMain = (MainCall) newMachOFromCFM (ptr);
								ok = true;
							}
							else
							{
								HUnlock (resHandle);
							}

							break;
						}
					}

					if (! ok)
						CloseResFile (resFileId);
				}
			}
		}
#endif

		return ok;
	}

	void close()
	{
	   #if JUCE_PPC
		if (fragId != 0)
		{
			if (moduleMain != 0)
				disposeMachOFromCFM ((void*) moduleMain);

			CloseConnection (&fragId);
			HUnlock (resHandle);

			if (resFileId != 0)
				CloseResFile (resFileId);
		}
		else
	   #endif
		if (bundleRef != 0)
		{
			CFBundleCloseBundleResourceMap (bundleRef, resFileId);

			if (CFGetRetainCount (bundleRef) == 1)
				CFBundleUnloadExecutable (bundleRef);

			if (CFGetRetainCount (bundleRef) > 0)
				CFRelease (bundleRef);
		}
	}

	void closeEffect (AEffect* eff)
	{
	   #if JUCE_PPC
		if (fragId != 0)
		{
			Array<void*> thingsToDelete;
			thingsToDelete.add ((void*) eff->dispatcher);
			thingsToDelete.add ((void*) eff->process);
			thingsToDelete.add ((void*) eff->setParameter);
			thingsToDelete.add ((void*) eff->getParameter);
			thingsToDelete.add ((void*) eff->processReplacing);

			eff->dispatcher (eff, effClose, 0, 0, 0, 0);

			for (int i = thingsToDelete.size(); --i >= 0;)
				disposeMachOFromCFM (thingsToDelete[i]);
		}
		else
	   #endif
		{
			eff->dispatcher (eff, effClose, 0, 0, 0, 0);
		}
	}

   #if JUCE_PPC
	static void* newMachOFromCFM (void* cfmfp)
	{
		if (cfmfp == 0)
			return nullptr;

		UInt32* const mfp = new UInt32[6];

		mfp[0] = 0x3d800000 | ((UInt32) cfmfp >> 16);
		mfp[1] = 0x618c0000 | ((UInt32) cfmfp & 0xffff);
		mfp[2] = 0x800c0000;
		mfp[3] = 0x804c0004;
		mfp[4] = 0x7c0903a6;
		mfp[5] = 0x4e800420;

		MakeDataExecutable (mfp, sizeof (UInt32) * 6);
		return mfp;
	}

	static void disposeMachOFromCFM (void* ptr)
	{
		delete[] static_cast <UInt32*> (ptr);
	}

	void coerceAEffectFunctionCalls (AEffect* eff)
	{
		if (fragId != 0)
		{
			eff->dispatcher = (AEffectDispatcherProc) newMachOFromCFM ((void*) eff->dispatcher);
			eff->process = (AEffectProcessProc) newMachOFromCFM ((void*) eff->process);
			eff->setParameter = (AEffectSetParameterProc) newMachOFromCFM ((void*) eff->setParameter);
			eff->getParameter = (AEffectGetParameterProc) newMachOFromCFM ((void*) eff->getParameter);
			eff->processReplacing = (AEffectProcessProc) newMachOFromCFM ((void*) eff->processReplacing);
		}
	}
   #endif

#endif

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleHandle);
};

/**
	An instance of a plugin, created by a VSTPluginFormat.

*/
class VSTPluginInstance     : public AudioPluginInstance,
							  private Timer,
							  private AsyncUpdater
{
public:

	~VSTPluginInstance();

	// AudioPluginInstance methods:

	void fillInPluginDescription (PluginDescription& desc) const
	{
		desc.name = name;

		{
			char buffer [512] = { 0 };
			dispatch (effGetEffectName, 0, 0, buffer, 0);

			desc.descriptiveName = String (buffer).trim();

			if (desc.descriptiveName.isEmpty())
				desc.descriptiveName = name;
		}

		desc.fileOrIdentifier = module->file.getFullPathName();
		desc.uid = getUID();
		desc.lastFileModTime = module->file.getLastModificationTime();
		desc.pluginFormatName = "VST";
		desc.category = getCategory();

		{
			char buffer [kVstMaxVendorStrLen + 8] = { 0 };
			dispatch (effGetVendorString, 0, 0, buffer, 0);
			desc.manufacturerName = buffer;
		}

		desc.version = getVersion();
		desc.numInputChannels = getNumInputChannels();
		desc.numOutputChannels = getNumOutputChannels();
		desc.isInstrument = (effect != nullptr && (effect->flags & effFlagsIsSynth) != 0);
	}

	void* getPlatformSpecificData()         { return effect; }
	const String getName() const            { return name; }
	int getUID() const;
	bool acceptsMidi() const                { return wantsMidiMessages; }
	bool producesMidi() const               { return dispatch (effCanDo, 0, 0, (void*) "sendVstMidiEvent", 0) > 0; }

	// AudioProcessor methods:

	void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock);
	void releaseResources();
	void processBlock (AudioSampleBuffer& buffer,
					   MidiBuffer& midiMessages);

	bool hasEditor() const                              { return effect != nullptr && (effect->flags & effFlagsHasEditor) != 0; }
	AudioProcessorEditor* createEditor();

	const String getInputChannelName (int index) const;
	bool isInputChannelStereoPair (int index) const;

	const String getOutputChannelName (int index) const;
	bool isOutputChannelStereoPair (int index) const;

	int getNumParameters()                              { return effect != nullptr ? effect->numParams : 0; }
	float getParameter (int index);
	void setParameter (int index, float newValue);
	const String getParameterName (int index);
	const String getParameterText (int index);
	bool isParameterAutomatable (int index) const;

	int getNumPrograms()                                { return effect != nullptr ? effect->numPrograms : 0; }
	int getCurrentProgram()                             { return dispatch (effGetProgram, 0, 0, 0, 0); }
	void setCurrentProgram (int index);
	const String getProgramName (int index);
	void changeProgramName (int index, const String& newName);

	void getStateInformation (MemoryBlock& destData);
	void getCurrentProgramStateInformation (MemoryBlock& destData);
	void setStateInformation (const void* data, int sizeInBytes);
	void setCurrentProgramStateInformation (const void* data, int sizeInBytes);

	void timerCallback();
	void handleAsyncUpdate();
	VstIntPtr handleCallback (VstInt32 opcode, VstInt32 index, VstInt32 value, void *ptr, float opt);

private:

	friend class VSTPluginWindow;
	friend class VSTPluginFormat;

	AEffect* effect;
	String name;
	CriticalSection lock;
	bool wantsMidiMessages, initialised, isPowerOn;
	mutable StringArray programNames;
	AudioSampleBuffer tempBuffer;
	CriticalSection midiInLock;
	MidiBuffer incomingMidi;
	VSTMidiEventList midiEventsToSend;
	VstTimeInfo vstHostTime;

	ReferenceCountedObjectPtr <ModuleHandle> module;

	int dispatch (const int opcode, const int index, const int value, void* const ptr, float opt) const;
	bool restoreProgramSettings (const fxProgram* const prog);
	const String getCurrentProgramName();
	void setParamsInProgramBlock (fxProgram* const prog);
	void updateStoredProgramNames();
	void initialise();
	void handleMidiFromPlugin (const VstEvents* const events);
	void createTempParameterStore (MemoryBlock& dest);
	void restoreFromTempParameterStore (const MemoryBlock& mb);
	const String getParameterLabel (int index) const;

	bool usesChunks() const noexcept        { return effect != nullptr && (effect->flags & effFlagsProgramChunks) != 0; }
	void getChunkData (MemoryBlock& mb, bool isPreset, int maxSizeMB) const;
	void setChunkData (const char* data, int size, bool isPreset);
	bool loadFromFXBFile (const void* data, int numBytes);
	bool saveToFXBFile (MemoryBlock& dest, bool isFXB, int maxSizeMB);

	int getVersionNumber() const noexcept   { return effect != nullptr ? effect->version : 0; }
	String getVersion() const;
	String getCategory() const;

	void setPower (const bool on);

	VSTPluginInstance (const ReferenceCountedObjectPtr <ModuleHandle>& module);
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VSTPluginInstance);
};

VSTPluginInstance::VSTPluginInstance (const ReferenceCountedObjectPtr <ModuleHandle>& module_)
	: effect (nullptr),
	  name (module_->pluginName),
	  wantsMidiMessages (false),
	  initialised (false),
	  isPowerOn (false),
	  tempBuffer (1, 1),
	  module (module_)
{
	try
	{
		const IdleCallRecursionPreventer icrp;
		_fpreset();

		log ("Creating VST instance: " + name);

	  #if JUCE_MAC
		if (module->resFileId != 0)
			UseResFile (module->resFileId);

	   #if JUCE_PPC
		if (module->fragId != 0)
		{
			static void* audioMasterCoerced = nullptr;
			if (audioMasterCoerced == nullptr)
				audioMasterCoerced = NewCFMFromMachO ((void*) &audioMaster);

			effect = module->moduleMain ((audioMasterCallback) audioMasterCoerced);
		}
		else
	   #endif
	  #endif
		{
			effect = module->moduleMain (&audioMaster);
		}

		if (effect != nullptr && effect->magic == kEffectMagic)
		{
		   #if JUCE_PPC
			module->coerceAEffectFunctionCalls (effect);
		   #endif

			jassert (effect->resvd2 == 0);
			jassert (effect->object != 0);

			_fpreset(); // some dodgy plugs fuck around with this
		}
		else
		{
			effect = nullptr;
		}
	}
	catch (...)
	{}
}

VSTPluginInstance::~VSTPluginInstance()
{
	const ScopedLock sl (lock);

	if (effect != nullptr && effect->magic == kEffectMagic)
	{
		try
		{
		   #if JUCE_MAC
			if (module->resFileId != 0)
				UseResFile (module->resFileId);
		   #endif

			// Must delete any editors before deleting the plugin instance!
			jassert (getActiveEditor() == 0);

			_fpreset(); // some dodgy plugs fuck around with this

			module->closeEffect (effect);
		}
		catch (...)
		{}
	}

	module = nullptr;
	effect = nullptr;
}

void VSTPluginInstance::initialise()
{
	if (initialised || effect == 0)
		return;

	log ("Initialising VST: " + module->pluginName);
	initialised = true;

	dispatch (effIdentify, 0, 0, 0, 0);

	if (getSampleRate() > 0)
		dispatch (effSetSampleRate, 0, 0, 0, (float) getSampleRate());

	if (getBlockSize() > 0)
		dispatch (effSetBlockSize, 0, jmax (32, getBlockSize()), 0, 0);

	dispatch (effOpen, 0, 0, 0, 0);

	setPlayConfigDetails (effect->numInputs, effect->numOutputs,
						  getSampleRate(), getBlockSize());

	if (getNumPrograms() > 1)
		setCurrentProgram (0);
	else
		dispatch (effSetProgram, 0, 0, 0, 0);

	int i;
	for (i = effect->numInputs; --i >= 0;)
		dispatch (effConnectInput, i, 1, 0, 0);

	for (i = effect->numOutputs; --i >= 0;)
		dispatch (effConnectOutput, i, 1, 0, 0);

	updateStoredProgramNames();

	wantsMidiMessages = dispatch (effCanDo, 0, 0, (void*) "receiveVstMidiEvent", 0) > 0;

	setLatencySamples (effect->initialDelay);
}

void VSTPluginInstance::prepareToPlay (double sampleRate_,
									   int samplesPerBlockExpected)
{
	setPlayConfigDetails (effect->numInputs, effect->numOutputs,
						  sampleRate_, samplesPerBlockExpected);

	setLatencySamples (effect->initialDelay);

	vstHostTime.tempo = 120.0;
	vstHostTime.timeSigNumerator = 4;
	vstHostTime.timeSigDenominator = 4;
	vstHostTime.sampleRate = sampleRate_;
	vstHostTime.samplePos = 0;
	vstHostTime.flags = kVstNanosValid;  /*| kVstTransportPlaying | kVstTempoValid | kVstTimeSigValid*/;

	initialise();

	if (initialised)
	{
		wantsMidiMessages = wantsMidiMessages
								|| (dispatch (effCanDo, 0, 0, (void*) "receiveVstMidiEvent", 0) > 0);

		if (wantsMidiMessages)
			midiEventsToSend.ensureSize (256);
		else
			midiEventsToSend.freeEvents();

		incomingMidi.clear();

		dispatch (effSetSampleRate, 0, 0, 0, (float) sampleRate_);
		dispatch (effSetBlockSize, 0, jmax (16, samplesPerBlockExpected), 0, 0);

		tempBuffer.setSize (jmax (1, effect->numOutputs), samplesPerBlockExpected);

		if (! isPowerOn)
			setPower (true);

		// dodgy hack to force some plugins to initialise the sample rate..
		if ((! hasEditor()) && getNumParameters() > 0)
		{
			const float old = getParameter (0);
			setParameter (0, (old < 0.5f) ? 1.0f : 0.0f);
			setParameter (0, old);
		}

		dispatch (effStartProcess, 0, 0, 0, 0);
	}
}

void VSTPluginInstance::releaseResources()
{
	if (initialised)
	{
		dispatch (effStopProcess, 0, 0, 0, 0);
		setPower (false);
	}

	tempBuffer.setSize (1, 1);
	incomingMidi.clear();

	midiEventsToSend.freeEvents();
}

void VSTPluginInstance::processBlock (AudioSampleBuffer& buffer,
									  MidiBuffer& midiMessages)
{
	const int numSamples = buffer.getNumSamples();

	if (initialised)
	{
		AudioPlayHead* playHead = getPlayHead();

		if (playHead != nullptr)
		{
			AudioPlayHead::CurrentPositionInfo position;
			playHead->getCurrentPosition (position);

			vstHostTime.tempo = position.bpm;
			vstHostTime.timeSigNumerator = position.timeSigNumerator;
			vstHostTime.timeSigDenominator = position.timeSigDenominator;
			vstHostTime.ppqPos = position.ppqPosition;
			vstHostTime.barStartPos = position.ppqPositionOfLastBarStart;
			vstHostTime.flags |= kVstTempoValid | kVstTimeSigValid | kVstPpqPosValid | kVstBarsValid;

			if (position.isPlaying)
				vstHostTime.flags |= kVstTransportPlaying;
			else
				vstHostTime.flags &= ~kVstTransportPlaying;
		}

		vstHostTime.nanoSeconds = getVSTHostTimeNanoseconds();

		if (wantsMidiMessages)
		{
			midiEventsToSend.clear();
			midiEventsToSend.ensureSize (1);

			MidiBuffer::Iterator iter (midiMessages);
			const uint8* midiData;
			int numBytesOfMidiData, samplePosition;

			while (iter.getNextEvent (midiData, numBytesOfMidiData, samplePosition))
			{
				midiEventsToSend.addEvent (midiData, numBytesOfMidiData,
										   jlimit (0, numSamples - 1, samplePosition));
			}

			try
			{
				effect->dispatcher (effect, effProcessEvents, 0, 0, midiEventsToSend.events, 0);
			}
			catch (...)
			{}
		}

		_clearfp();

		if ((effect->flags & effFlagsCanReplacing) != 0)
		{
			try
			{
				effect->processReplacing (effect, buffer.getArrayOfChannels(), buffer.getArrayOfChannels(), numSamples);
			}
			catch (...)
			{}
		}
		else
		{
			tempBuffer.setSize (effect->numOutputs, numSamples);
			tempBuffer.clear();

			try
			{
				effect->process (effect, buffer.getArrayOfChannels(), tempBuffer.getArrayOfChannels(), numSamples);
			}
			catch (...)
			{}

			for (int i = effect->numOutputs; --i >= 0;)
				buffer.copyFrom (i, 0, tempBuffer.getSampleData (i), numSamples);
		}
	}
	else
	{
		// Not initialised, so just bypass..
		for (int i = 0; i < getNumOutputChannels(); ++i)
			buffer.clear (i, 0, buffer.getNumSamples());
	}

	{
		// copy any incoming midi..
		const ScopedLock sl (midiInLock);

		midiMessages.swapWith (incomingMidi);
		incomingMidi.clear();
	}
}

void VSTPluginInstance::handleMidiFromPlugin (const VstEvents* const events)
{
	if (events != nullptr)
	{
		const ScopedLock sl (midiInLock);
		VSTMidiEventList::addEventsToMidiBuffer (events, incomingMidi);
	}
}

static Array <VSTPluginWindow*> activeVSTWindows;

class VSTPluginWindow   : public AudioProcessorEditor,
						 #if ! JUCE_MAC
						  public ComponentMovementWatcher,
						 #endif
						  public Timer
{
public:

	VSTPluginWindow (VSTPluginInstance& plugin_)
		: AudioProcessorEditor (&plugin_),
		 #if ! JUCE_MAC
		  ComponentMovementWatcher (this),
		 #endif
		  plugin (plugin_),
		  isOpen (false),
		  recursiveResize (false),
		  pluginWantsKeys (false),
		  pluginRefusesToResize (false),
		  alreadyInside (false)
	{
	   #if JUCE_WINDOWS
		sizeCheckCount = 0;
		pluginHWND = 0;
	   #elif JUCE_LINUX
		pluginWindow = None;
		pluginProc = None;
	   #else
		addAndMakeVisible (innerWrapper = new InnerWrapperComponent (*this));
	   #endif

		activeVSTWindows.add (this);

		setSize (1, 1);
		setOpaque (true);
		setVisible (true);
	}

	~VSTPluginWindow()
	{
	   #if JUCE_MAC
		innerWrapper = nullptr;
	   #else
		closePluginWindow();
	   #endif

		activeVSTWindows.removeValue (this);
		plugin.editorBeingDeleted (this);
	}

#if ! JUCE_MAC
	void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/)
	{
		if (recursiveResize)
			return;

		Component* const topComp = getTopLevelComponent();

		if (topComp->getPeer() != nullptr)
		{
			const Point<int> pos (topComp->getLocalPoint (this, Point<int>()));

			recursiveResize = true;

		   #if JUCE_WINDOWS
			if (pluginHWND != 0)
				MoveWindow (pluginHWND, pos.getX(), pos.getY(), getWidth(), getHeight(), TRUE);
		   #elif JUCE_LINUX
			if (pluginWindow != 0)
			{
				XResizeWindow (display, pluginWindow, getWidth(), getHeight());
				XMoveWindow (display, pluginWindow, pos.getX(), pos.getY());
				XMapRaised (display, pluginWindow);
			}
		   #endif

			recursiveResize = false;
		}
	}

	void componentVisibilityChanged()
	{
		if (isShowing())
			openPluginWindow();
		else
			closePluginWindow();

		componentMovedOrResized (true, true);
	}

	void componentPeerChanged()
	{
		closePluginWindow();
		openPluginWindow();
	}
#endif

	bool keyStateChanged (bool)
	{
		return pluginWantsKeys;
	}

	bool keyPressed (const juce::KeyPress&)
	{
		return pluginWantsKeys;
	}

#if JUCE_MAC
	void paint (Graphics& g)
	{
		g.fillAll (Colours::black);
	}
#else
	void paint (Graphics& g)
	{
		if (isOpen)
		{
			ComponentPeer* const peer = getPeer();

			if (peer != nullptr)
			{
				peer->addMaskedRegion (getScreenBounds() - peer->getScreenPosition());

			   #if JUCE_LINUX
				if (pluginWindow != 0)
				{
					const Rectangle<int> clip (g.getClipBounds());

					XEvent ev = { 0 };
					ev.xexpose.type = Expose;
					ev.xexpose.display = display;
					ev.xexpose.window = pluginWindow;
					ev.xexpose.x = clip.getX();
					ev.xexpose.y = clip.getY();
					ev.xexpose.width = clip.getWidth();
					ev.xexpose.height = clip.getHeight();

					sendEventToChild (&ev);
				}
			   #endif
			}
		}
		else
		{
			g.fillAll (Colours::black);
		}
	}
#endif

	void timerCallback()
	{
	   #if JUCE_WINDOWS
		if (--sizeCheckCount <= 0)
		{
			sizeCheckCount = 10;

			checkPluginWindowSize();
		}
	   #endif

		try
		{
			static bool reentrant = false;

			if (! reentrant)
			{
				reentrant = true;
				plugin.dispatch (effEditIdle, 0, 0, 0, 0);
				reentrant = false;
			}
		}
		catch (...)
		{}
	}

	void mouseDown (const MouseEvent& e)
	{
	   #if JUCE_LINUX
		if (pluginWindow == 0)
			return;

		toFront (true);

		XEvent ev = { 0 };
		ev.xbutton.display = display;
		ev.xbutton.type = ButtonPress;
		ev.xbutton.window = pluginWindow;
		ev.xbutton.root = RootWindow (display, DefaultScreen (display));
		ev.xbutton.time = CurrentTime;
		ev.xbutton.x = e.x;
		ev.xbutton.y = e.y;
		ev.xbutton.x_root = e.getScreenX();
		ev.xbutton.y_root = e.getScreenY();

		translateJuceToXButtonModifiers (e, ev);

		sendEventToChild (&ev);

	   #elif JUCE_WINDOWS
		(void) e;

		toFront (true);
	   #endif
	}

	void broughtToFront()
	{
		activeVSTWindows.removeValue (this);
		activeVSTWindows.add (this);

	   #if JUCE_MAC
		dispatch (effEditTop, 0, 0, 0, 0);
	   #endif
	}

private:
	VSTPluginInstance& plugin;
	bool isOpen, recursiveResize;
	bool pluginWantsKeys, pluginRefusesToResize, alreadyInside;

   #if JUCE_WINDOWS
	HWND pluginHWND;
	void* originalWndProc;
	int sizeCheckCount;
   #elif JUCE_LINUX
	Window pluginWindow;
	EventProcPtr pluginProc;
   #endif

#if JUCE_MAC
	void openPluginWindow (WindowRef parentWindow)
	{
		if (isOpen || parentWindow == 0)
			return;

		isOpen = true;

		ERect* rect = nullptr;
		dispatch (effEditGetRect, 0, 0, &rect, 0);
		dispatch (effEditOpen, 0, 0, parentWindow, 0);

		// do this before and after like in the steinberg example
		dispatch (effEditGetRect, 0, 0, &rect, 0);
		dispatch (effGetProgram, 0, 0, 0, 0); // also in steinberg code

		// Install keyboard hooks
		pluginWantsKeys = (dispatch (effKeysRequired, 0, 0, 0, 0) == 0);

		// double-check it's not too tiny
		int w = 250, h = 150;

		if (rect != nullptr)
		{
			w = rect->right - rect->left;
			h = rect->bottom - rect->top;

			if (w == 0 || h == 0)
			{
				w = 250;
				h = 150;
			}
		}

		w = jmax (w, 32);
		h = jmax (h, 32);

		setSize (w, h);

		startTimer (18 + juce::Random::getSystemRandom().nextInt (5));
		repaint();
	}

#else
	void openPluginWindow()
	{
		if (isOpen || getWindowHandle() == 0)
			return;

		log ("Opening VST UI: " + plugin.name);
		isOpen = true;

		ERect* rect = nullptr;
		dispatch (effEditGetRect, 0, 0, &rect, 0);
		dispatch (effEditOpen, 0, 0, getWindowHandle(), 0);

		// do this before and after like in the steinberg example
		dispatch (effEditGetRect, 0, 0, &rect, 0);
		dispatch (effGetProgram, 0, 0, 0, 0); // also in steinberg code

		// Install keyboard hooks
		pluginWantsKeys = (dispatch (effKeysRequired, 0, 0, 0, 0) == 0);

	   #if JUCE_WINDOWS
		originalWndProc = 0;
		pluginHWND = GetWindow ((HWND) getWindowHandle(), GW_CHILD);

		if (pluginHWND == 0)
		{
			isOpen = false;
			setSize (300, 150);
			return;
		}

		#pragma warning (push)
		#pragma warning (disable: 4244)

		originalWndProc = (void*) GetWindowLongPtr (pluginHWND, GWLP_WNDPROC);

		if (! pluginWantsKeys)
			SetWindowLongPtr (pluginHWND, GWLP_WNDPROC, (LONG_PTR) vstHookWndProc);

		#pragma warning (pop)

		int w, h;
		RECT r;
		GetWindowRect (pluginHWND, &r);
		w = r.right - r.left;
		h = r.bottom - r.top;

		if (rect != nullptr)
		{
			const int rw = rect->right - rect->left;
			const int rh = rect->bottom - rect->top;

			if ((rw > 50 && rh > 50 && rw < 2000 && rh < 2000 && rw != w && rh != h)
				|| ((w == 0 && rw > 0) || (h == 0 && rh > 0)))
			{
				// very dodgy logic to decide which size is right.
				if (abs (rw - w) > 350 || abs (rh - h) > 350)
				{
					SetWindowPos (pluginHWND, 0,
								  0, 0, rw, rh,
								  SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

					GetWindowRect (pluginHWND, &r);

					w = r.right - r.left;
					h = r.bottom - r.top;

					pluginRefusesToResize = (w != rw) || (h != rh);

					w = rw;
					h = rh;
				}
			}
		}

	   #elif JUCE_LINUX
		pluginWindow = getChildWindow ((Window) getWindowHandle());

		if (pluginWindow != 0)
			pluginProc = (EventProcPtr) getPropertyFromXWindow (pluginWindow,
																XInternAtom (display, "_XEventProc", False));

		int w = 250, h = 150;

		if (rect != nullptr)
		{
			w = rect->right - rect->left;
			h = rect->bottom - rect->top;

			if (w == 0 || h == 0)
			{
				w = 250;
				h = 150;
			}
		}

		if (pluginWindow != 0)
			XMapRaised (display, pluginWindow);
	   #endif

		// double-check it's not too tiny
		w = jmax (w, 32);
		h = jmax (h, 32);

		setSize (w, h);

	   #if JUCE_WINDOWS
		checkPluginWindowSize();
	   #endif

		startTimer (18 + juce::Random::getSystemRandom().nextInt (5));
		repaint();
	}
#endif

#if ! JUCE_MAC
	void closePluginWindow()
	{
		if (isOpen)
		{
			log ("Closing VST UI: " + plugin.getName());
			isOpen = false;

			dispatch (effEditClose, 0, 0, 0, 0);

		   #if JUCE_WINDOWS
			#pragma warning (push)
			#pragma warning (disable: 4244)

			if (pluginHWND != 0 && IsWindow (pluginHWND))
				SetWindowLongPtr (pluginHWND, GWLP_WNDPROC, (LONG_PTR) originalWndProc);

			#pragma warning (pop)

			stopTimer();

			if (pluginHWND != 0 && IsWindow (pluginHWND))
				DestroyWindow (pluginHWND);

			pluginHWND = 0;
		   #elif JUCE_LINUX
			stopTimer();
			pluginWindow = 0;
			pluginProc = 0;
		   #endif
		}
	}
#endif

	int dispatch (const int opcode, const int index, const int value, void* const ptr, float opt)
	{
		return plugin.dispatch (opcode, index, value, ptr, opt);
	}

#if JUCE_WINDOWS
	void checkPluginWindowSize()
	{
		RECT r;
		GetWindowRect (pluginHWND, &r);
		const int w = r.right - r.left;
		const int h = r.bottom - r.top;

		if (isShowing() && w > 0 && h > 0
			 && (w != getWidth() || h != getHeight())
			 && ! pluginRefusesToResize)
		{
			setSize (w, h);
			sizeCheckCount = 0;
		}
	}

	// hooks to get keyboard events from VST windows..
	static LRESULT CALLBACK vstHookWndProc (HWND hW, UINT message, WPARAM wParam, LPARAM lParam)
	{
		for (int i = activeVSTWindows.size(); --i >= 0;)
		{
			const VSTPluginWindow* const w = activeVSTWindows.getUnchecked (i);

			if (w->pluginHWND == hW)
			{
				if (message == WM_CHAR
					|| message == WM_KEYDOWN
					|| message == WM_SYSKEYDOWN
					|| message == WM_KEYUP
					|| message == WM_SYSKEYUP
					|| message == WM_APPCOMMAND)
				{
					SendMessage ((HWND) w->getTopLevelComponent()->getWindowHandle(),
								 message, wParam, lParam);
				}

				return CallWindowProc ((WNDPROC) (w->originalWndProc),
									   (HWND) w->pluginHWND,
									   message,
									   wParam,
									   lParam);
			}
		}

		return DefWindowProc (hW, message, wParam, lParam);
	}
#endif

#if JUCE_LINUX

	// overload mouse/keyboard events to forward them to the plugin's inner window..
	void sendEventToChild (XEvent* event)
	{
		if (pluginProc != 0)
		{
			// if the plugin publishes an event procedure, pass the event directly..
			pluginProc (event);
		}
		else if (pluginWindow != 0)
		{
			// if the plugin has a window, then send the event to the window so that
			// its message thread will pick it up..
			XSendEvent (display, pluginWindow, False, 0L, event);
			XFlush (display);
		}
	}

	void mouseEnter (const MouseEvent& e)
	{
		if (pluginWindow != 0)
		{
			XEvent ev = { 0 };
			ev.xcrossing.display = display;
			ev.xcrossing.type = EnterNotify;
			ev.xcrossing.window = pluginWindow;
			ev.xcrossing.root = RootWindow (display, DefaultScreen (display));
			ev.xcrossing.time = CurrentTime;
			ev.xcrossing.x = e.x;
			ev.xcrossing.y = e.y;
			ev.xcrossing.x_root = e.getScreenX();
			ev.xcrossing.y_root = e.getScreenY();
			ev.xcrossing.mode = NotifyNormal; // NotifyGrab, NotifyUngrab
			ev.xcrossing.detail = NotifyAncestor; // NotifyVirtual, NotifyInferior, NotifyNonlinear,NotifyNonlinearVirtual

			translateJuceToXCrossingModifiers (e, ev);

			sendEventToChild (&ev);
		}
	}

	void mouseExit (const MouseEvent& e)
	{
		if (pluginWindow != 0)
		{
			XEvent ev = { 0 };
			ev.xcrossing.display = display;
			ev.xcrossing.type = LeaveNotify;
			ev.xcrossing.window = pluginWindow;
			ev.xcrossing.root = RootWindow (display, DefaultScreen (display));
			ev.xcrossing.time = CurrentTime;
			ev.xcrossing.x = e.x;
			ev.xcrossing.y = e.y;
			ev.xcrossing.x_root = e.getScreenX();
			ev.xcrossing.y_root = e.getScreenY();
			ev.xcrossing.mode = NotifyNormal; // NotifyGrab, NotifyUngrab
			ev.xcrossing.detail = NotifyAncestor; // NotifyVirtual, NotifyInferior, NotifyNonlinear,NotifyNonlinearVirtual
			ev.xcrossing.focus = hasKeyboardFocus (true); // TODO - yes ?

			translateJuceToXCrossingModifiers (e, ev);

			sendEventToChild (&ev);
		}
	}

	void mouseMove (const MouseEvent& e)
	{
		if (pluginWindow != 0)
		{
			XEvent ev = { 0 };
			ev.xmotion.display = display;
			ev.xmotion.type = MotionNotify;
			ev.xmotion.window = pluginWindow;
			ev.xmotion.root = RootWindow (display, DefaultScreen (display));
			ev.xmotion.time = CurrentTime;
			ev.xmotion.is_hint = NotifyNormal;
			ev.xmotion.x = e.x;
			ev.xmotion.y = e.y;
			ev.xmotion.x_root = e.getScreenX();
			ev.xmotion.y_root = e.getScreenY();

			sendEventToChild (&ev);
		}
	}

	void mouseDrag (const MouseEvent& e)
	{
		if (pluginWindow != 0)
		{
			XEvent ev = { 0 };
			ev.xmotion.display = display;
			ev.xmotion.type = MotionNotify;
			ev.xmotion.window = pluginWindow;
			ev.xmotion.root = RootWindow (display, DefaultScreen (display));
			ev.xmotion.time = CurrentTime;
			ev.xmotion.x = e.x ;
			ev.xmotion.y = e.y;
			ev.xmotion.x_root = e.getScreenX();
			ev.xmotion.y_root = e.getScreenY();
			ev.xmotion.is_hint = NotifyNormal;

			translateJuceToXMotionModifiers (e, ev);
			sendEventToChild (&ev);
		}
	}

	void mouseUp (const MouseEvent& e)
	{
		if (pluginWindow != 0)
		{
			XEvent ev = { 0 };
			ev.xbutton.display = display;
			ev.xbutton.type = ButtonRelease;
			ev.xbutton.window = pluginWindow;
			ev.xbutton.root = RootWindow (display, DefaultScreen (display));
			ev.xbutton.time = CurrentTime;
			ev.xbutton.x = e.x;
			ev.xbutton.y = e.y;
			ev.xbutton.x_root = e.getScreenX();
			ev.xbutton.y_root = e.getScreenY();

			translateJuceToXButtonModifiers (e, ev);
			sendEventToChild (&ev);
		}
	}

	void mouseWheelMove (const MouseEvent& e,
						 float incrementX,
						 float incrementY)
	{
		if (pluginWindow != 0)
		{
			XEvent ev = { 0 };
			ev.xbutton.display = display;
			ev.xbutton.type = ButtonPress;
			ev.xbutton.window = pluginWindow;
			ev.xbutton.root = RootWindow (display, DefaultScreen (display));
			ev.xbutton.time = CurrentTime;
			ev.xbutton.x = e.x;
			ev.xbutton.y = e.y;
			ev.xbutton.x_root = e.getScreenX();
			ev.xbutton.y_root = e.getScreenY();

			translateJuceToXMouseWheelModifiers (e, incrementY, ev);
			sendEventToChild (&ev);

			// TODO - put a usleep here ?

			ev.xbutton.type = ButtonRelease;
			sendEventToChild (&ev);
		}
	}
#endif

#if JUCE_MAC

   #if ! JUCE_SUPPORT_CARBON
	#error "To build VSTs, you need to enable the JUCE_SUPPORT_CARBON flag in your config!"
   #endif

	class InnerWrapperComponent   : public CarbonViewWrapperComponent
	{
	public:
		InnerWrapperComponent (VSTPluginWindow& owner_)
			: owner (owner_),
			  alreadyInside (false)
		{
		}

		~InnerWrapperComponent()
		{
			deleteWindow();
		}

		HIViewRef attachView (WindowRef windowRef, HIViewRef rootView)
		{
			owner.openPluginWindow (windowRef);
			return 0;
		}

		void removeView (HIViewRef)
		{
			if (owner.isOpen)
			{
				owner.isOpen = false;
				owner.dispatch (effEditClose, 0, 0, 0, 0);
				owner.dispatch (effEditSleep, 0, 0, 0, 0);
			}
		}

		bool getEmbeddedViewSize (int& w, int& h)
		{
			ERect* rect = nullptr;
			owner.dispatch (effEditGetRect, 0, 0, &rect, 0);
			w = rect->right - rect->left;
			h = rect->bottom - rect->top;
			return true;
		}

		void mouseDown (int x, int y)
		{
			if (! alreadyInside)
			{
				alreadyInside = true;
				getTopLevelComponent()->toFront (true);
				owner.dispatch (effEditMouse, x, y, 0, 0);
				alreadyInside = false;
			}
			else
			{
				PostEvent (::mouseDown, 0);
			}
		}

		void paint()
		{
			ComponentPeer* const peer = getPeer();

			if (peer != nullptr)
			{
				const Point<int> pos (getScreenPosition() - peer->getScreenPosition());
				ERect r;
				r.left = pos.getX();
				r.right = r.left + getWidth();
				r.top = pos.getY();
				r.bottom = r.top + getHeight();

				owner.dispatch (effEditDraw, 0, 0, &r, 0);
			}
		}

	private:
		VSTPluginWindow& owner;
		bool alreadyInside;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InnerWrapperComponent);
	};

	friend class InnerWrapperComponent;
	ScopedPointer <InnerWrapperComponent> innerWrapper;

	void resized()
	{
		if (innerWrapper != nullptr)
			innerWrapper->setSize (getWidth(), getHeight());
	}
#endif

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VSTPluginWindow);
};

AudioProcessorEditor* VSTPluginInstance::createEditor()
{
	if (hasEditor())
		return new VSTPluginWindow (*this);

	return nullptr;
}

void VSTPluginInstance::handleAsyncUpdate()
{
	// indicates that something about the plugin has changed..
	updateHostDisplay();
}

bool VSTPluginInstance::restoreProgramSettings (const fxProgram* const prog)
{
	if (vst_swap (prog->chunkMagic) == 'CcnK' && vst_swap (prog->fxMagic) == 'FxCk')
	{
		changeProgramName (getCurrentProgram(), prog->prgName);

		for (int i = 0; i < vst_swap (prog->numParams); ++i)
			setParameter (i, vst_swapFloat (prog->params[i]));

		return true;
	}

	return false;
}

bool VSTPluginInstance::loadFromFXBFile (const void* const data,
										 const int dataSize)
{
	if (dataSize < 28)
		return false;

	const fxSet* const set = (const fxSet*) data;

	if ((vst_swap (set->chunkMagic) != 'CcnK' && vst_swap (set->chunkMagic) != 'KncC')
		 || vst_swap (set->version) > fxbVersionNum)
		return false;

	if (vst_swap (set->fxMagic) == 'FxBk')
	{
		// bank of programs
		if (vst_swap (set->numPrograms) >= 0)
		{
			const int oldProg = getCurrentProgram();
			const int numParams = vst_swap (((const fxProgram*) (set->programs))->numParams);
			const int progLen = sizeof (fxProgram) + (numParams - 1) * sizeof (float);

			for (int i = 0; i < vst_swap (set->numPrograms); ++i)
			{
				if (i != oldProg)
				{
					const fxProgram* const prog = (const fxProgram*) (((const char*) (set->programs)) + i * progLen);
					if (((const char*) prog) - ((const char*) set) >= dataSize)
						return false;

					if (vst_swap (set->numPrograms) > 0)
						setCurrentProgram (i);

					if (! restoreProgramSettings (prog))
						return false;
				}
			}

			if (vst_swap (set->numPrograms) > 0)
				setCurrentProgram (oldProg);

			const fxProgram* const prog = (const fxProgram*) (((const char*) (set->programs)) + oldProg * progLen);
			if (((const char*) prog) - ((const char*) set) >= dataSize)
				return false;

			if (! restoreProgramSettings (prog))
				return false;
		}
	}
	else if (vst_swap (set->fxMagic) == 'FxCk')
	{
		// single program
		const fxProgram* const prog = (const fxProgram*) data;

		if (vst_swap (prog->chunkMagic) != 'CcnK')
			return false;

		changeProgramName (getCurrentProgram(), prog->prgName);

		for (int i = 0; i < vst_swap (prog->numParams); ++i)
			setParameter (i, vst_swapFloat (prog->params[i]));
	}
	else if (vst_swap (set->fxMagic) == 'FBCh' || vst_swap (set->fxMagic) == 'hCBF')
	{
		// non-preset chunk
		const fxChunkSet* const cset = (const fxChunkSet*) data;

		if (vst_swap (cset->chunkSize) + sizeof (fxChunkSet) - 8 > (unsigned int) dataSize)
			return false;

		setChunkData (cset->chunk, vst_swap (cset->chunkSize), false);
	}
	else if (vst_swap (set->fxMagic) == 'FPCh' || vst_swap (set->fxMagic) == 'hCPF')
	{
		// preset chunk
		const fxProgramSet* const cset = (const fxProgramSet*) data;

		if (vst_swap (cset->chunkSize) + sizeof (fxProgramSet) - 8 > (unsigned int) dataSize)
			return false;

		setChunkData (cset->chunk, vst_swap (cset->chunkSize), true);

		changeProgramName (getCurrentProgram(), cset->name);
	}
	else
	{
		return false;
	}

	return true;
}

void VSTPluginInstance::setParamsInProgramBlock (fxProgram* const prog)
{
	const int numParams = getNumParameters();

	prog->chunkMagic = vst_swap ('CcnK');
	prog->byteSize = 0;
	prog->fxMagic = vst_swap ('FxCk');
	prog->version = vst_swap (fxbVersionNum);
	prog->fxID = vst_swap (getUID());
	prog->fxVersion = vst_swap (getVersionNumber());
	prog->numParams = vst_swap (numParams);

	getCurrentProgramName().copyToUTF8 (prog->prgName, sizeof (prog->prgName) - 1);

	for (int i = 0; i < numParams; ++i)
		prog->params[i] = vst_swapFloat (getParameter (i));
}

bool VSTPluginInstance::saveToFXBFile (MemoryBlock& dest, bool isFXB, int maxSizeMB)
{
	const int numPrograms = getNumPrograms();
	const int numParams = getNumParameters();

	if (usesChunks())
	{
		MemoryBlock chunk;
		getChunkData (chunk, ! isFXB, maxSizeMB);

		if (isFXB)
		{
			const size_t totalLen = sizeof (fxChunkSet) + chunk.getSize() - 8;
			dest.setSize (totalLen, true);

			fxChunkSet* const set = (fxChunkSet*) dest.getData();
			set->chunkMagic = vst_swap ('CcnK');
			set->byteSize = 0;
			set->fxMagic = vst_swap ('FBCh');
			set->version = vst_swap (fxbVersionNum);
			set->fxID = vst_swap (getUID());
			set->fxVersion = vst_swap (getVersionNumber());
			set->numPrograms = vst_swap (numPrograms);
			set->chunkSize = vst_swap ((long) chunk.getSize());

			chunk.copyTo (set->chunk, 0, chunk.getSize());
		}
		else
		{
			const size_t totalLen = sizeof (fxProgramSet) + chunk.getSize() - 8;
			dest.setSize (totalLen, true);

			fxProgramSet* const set = (fxProgramSet*) dest.getData();
			set->chunkMagic = vst_swap ('CcnK');
			set->byteSize = 0;
			set->fxMagic = vst_swap ('FPCh');
			set->version = vst_swap (fxbVersionNum);
			set->fxID = vst_swap (getUID());
			set->fxVersion = vst_swap (getVersionNumber());
			set->numPrograms = vst_swap (numPrograms);
			set->chunkSize = vst_swap ((long) chunk.getSize());

			getCurrentProgramName().copyToUTF8 (set->name, sizeof (set->name) - 1);
			chunk.copyTo (set->chunk, 0, chunk.getSize());
		}
	}
	else
	{
		if (isFXB)
		{
			const int progLen = sizeof (fxProgram) + (numParams - 1) * sizeof (float);
			const int len = (sizeof (fxSet) - sizeof (fxProgram)) + progLen * jmax (1, numPrograms);
			dest.setSize (len, true);

			fxSet* const set = (fxSet*) dest.getData();
			set->chunkMagic = vst_swap ('CcnK');
			set->byteSize = 0;
			set->fxMagic = vst_swap ('FxBk');
			set->version = vst_swap (fxbVersionNum);
			set->fxID = vst_swap (getUID());
			set->fxVersion = vst_swap (getVersionNumber());
			set->numPrograms = vst_swap (numPrograms);

			const int oldProgram = getCurrentProgram();
			MemoryBlock oldSettings;
			createTempParameterStore (oldSettings);

			setParamsInProgramBlock ((fxProgram*) (((char*) (set->programs)) + oldProgram * progLen));

			for (int i = 0; i < numPrograms; ++i)
			{
				if (i != oldProgram)
				{
					setCurrentProgram (i);
					setParamsInProgramBlock ((fxProgram*) (((char*) (set->programs)) + i * progLen));
				}
			}

			setCurrentProgram (oldProgram);
			restoreFromTempParameterStore (oldSettings);
		}
		else
		{
			const int totalLen = sizeof (fxProgram) + (numParams - 1) * sizeof (float);
			dest.setSize (totalLen, true);

			setParamsInProgramBlock ((fxProgram*) dest.getData());
		}
	}

	return true;
}

void VSTPluginInstance::getChunkData (MemoryBlock& mb, bool isPreset, int maxSizeMB) const
{
	if (usesChunks())
	{
		void* data = nullptr;
		const int bytes = dispatch (effGetChunk, isPreset ? 1 : 0, 0, &data, 0.0f);

		if (data != nullptr && bytes <= maxSizeMB * 1024 * 1024)
		{
			mb.setSize (bytes);
			mb.copyFrom (data, 0, bytes);
		}
	}
}

void VSTPluginInstance::setChunkData (const char* data, int size, bool isPreset)
{
	if (size > 0 && usesChunks())
	{
		dispatch (effSetChunk, isPreset ? 1 : 0, size, (void*) data, 0.0f);

		if (! isPreset)
			updateStoredProgramNames();
	}
}

void VSTPluginInstance::timerCallback()
{
	if (dispatch (effIdle, 0, 0, 0, 0) == 0)
		stopTimer();
}

int VSTPluginInstance::dispatch (const int opcode, const int index, const int value, void* const ptr, float opt) const
{
	int result = 0;

	if (effect != nullptr)
	{
		const ScopedLock sl (lock);
		const IdleCallRecursionPreventer icrp;

		try
		{
		   #if JUCE_MAC
			const int oldResFile = CurResFile();

			if (module->resFileId != 0)
				UseResFile (module->resFileId);
		   #endif

			result = effect->dispatcher (effect, opcode, index, value, ptr, opt);

		   #if JUCE_MAC
			const int newResFile = CurResFile();
			if (newResFile != oldResFile)  // avoid confusing the parent app's resource file with the plug-in's
			{
				module->resFileId = newResFile;
				UseResFile (oldResFile);
			}
		   #endif
		}
		catch (...)
		{}
	}

	return result;
}

namespace
{
	static const int defaultVSTSampleRateValue = 44100;
	static const int defaultVSTBlockSizeValue = 512;

	// handles non plugin-specific callbacks..
	VstIntPtr handleGeneralCallback (VstInt32 opcode, VstInt32 index, VstInt32 value, void *ptr, float opt)
	{
		(void) index;
		(void) value;
		(void) opt;

		switch (opcode)
		{
			case audioMasterCanDo:
			{
				static const char* canDos[] = { "supplyIdle",
												"sendVstEvents",
												"sendVstMidiEvent",
												"sendVstTimeInfo",
												"receiveVstEvents",
												"receiveVstMidiEvent",
												"supportShell",
												"shellCategory" };

				for (int i = 0; i < numElementsInArray (canDos); ++i)
					if (strcmp (canDos[i], (const char*) ptr) == 0)
						return 1;

				return 0;
			}

			case audioMasterVersion:                        return 0x2400;
			case audioMasterCurrentId:                      return shellUIDToCreate;
			case audioMasterGetNumAutomatableParameters:    return 0;
			case audioMasterGetAutomationState:             return 1;
			case audioMasterGetVendorVersion:               return 0x0101;

			case audioMasterGetVendorString:
			case audioMasterGetProductString:
			{
				String hostName ("Juce VST Host");

				if (JUCEApplication::getInstance() != nullptr)
					hostName = JUCEApplication::getInstance()->getApplicationName();

				hostName.copyToUTF8 ((char*) ptr, jmin (kVstMaxVendorStrLen, kVstMaxProductStrLen) - 1);
				break;
			}

			case audioMasterGetSampleRate:          return (VstIntPtr) defaultVSTSampleRateValue;
			case audioMasterGetBlockSize:           return (VstIntPtr) defaultVSTBlockSizeValue;
			case audioMasterSetOutputSampleRate:    return 0;

			default:
				DBG ("*** Unhandled VST Callback: " + String ((int) opcode));
				break;
		}

		return 0;
	}
}

// handles callbacks for a specific plugin
VstIntPtr VSTPluginInstance::handleCallback (VstInt32 opcode, VstInt32 index, VstInt32 value, void *ptr, float opt)
{
	switch (opcode)
	{
	case audioMasterAutomate:
		sendParamChangeMessageToListeners (index, opt);
		break;

	case audioMasterProcessEvents:
		handleMidiFromPlugin ((const VstEvents*) ptr);
		break;

	case audioMasterGetTime:
		#if JUCE_MSVC
		 #pragma warning (push)
		 #pragma warning (disable: 4311)
		#endif

		return (VstIntPtr) &vstHostTime;

		#if JUCE_MSVC
		 #pragma warning (pop)
		#endif
		break;

	case audioMasterIdle:
		if (insideVSTCallback == 0 && MessageManager::getInstance()->isThisTheMessageThread())
		{
			const IdleCallRecursionPreventer icrp;

		   #if JUCE_MAC
			if (getActiveEditor() != nullptr)
				dispatch (effEditIdle, 0, 0, 0, 0);
		   #endif

			Timer::callPendingTimersSynchronously();

			handleUpdateNowIfNeeded();

			for (int i = ComponentPeer::getNumPeers(); --i >= 0;)
				ComponentPeer::getPeer (i)->performAnyPendingRepaintsNow();
		}
		break;

	case audioMasterUpdateDisplay:
		triggerAsyncUpdate();
		break;

	case audioMasterTempoAt:
		// returns (10000 * bpm)
		break;

	case audioMasterNeedIdle:
		startTimer (50);
		break;

	case audioMasterSizeWindow:
		if (getActiveEditor() != nullptr)
			getActiveEditor()->setSize (index, value);

		return 1;

	case audioMasterGetSampleRate:
		return (VstIntPtr) (getSampleRate() > 0 ? getSampleRate() : defaultVSTSampleRateValue);

	case audioMasterGetBlockSize:
		return (VstIntPtr) (getBlockSize() > 0 ? getBlockSize() : defaultVSTBlockSizeValue);

	case audioMasterWantMidi:
		wantsMidiMessages = true;
		break;

	case audioMasterGetDirectory:
	  #if JUCE_MAC
		return (VstIntPtr) (void*) &module->parentDirFSSpec;
	  #else
		return (VstIntPtr) (pointer_sized_uint) module->fullParentDirectoryPathName.toUTF8().getAddress();
	  #endif

	case audioMasterGetAutomationState:
		// returns 0: not supported, 1: off, 2:read, 3:write, 4:read/write
		break;

	case audioMasterPinConnected:
		if (value == 0) // input
		{
			if (index < getNumInputChannels())
				return 0;
		}
		else // output
		{
			if (index < getNumOutputChannels())
				return 0;
		}

		return 1; // (not connected)

	case audioMasterIOChanged:
		setLatencySamples (effect->initialDelay);
		break;

	// none of these are handled (yet)..
	case audioMasterBeginEdit:
	case audioMasterEndEdit:
	case audioMasterSetTime:
	case audioMasterGetParameterQuantization:
	case audioMasterGetInputLatency:
	case audioMasterGetOutputLatency:
	case audioMasterGetPreviousPlug:
	case audioMasterGetNextPlug:
	case audioMasterWillReplaceOrAccumulate:
	case audioMasterGetCurrentProcessLevel:
	case audioMasterOfflineStart:
	case audioMasterOfflineRead:
	case audioMasterOfflineWrite:
	case audioMasterOfflineGetCurrentPass:
	case audioMasterOfflineGetCurrentMetaPass:
	case audioMasterVendorSpecific:
	case audioMasterSetIcon:
	case audioMasterGetLanguage:
	case audioMasterOpenWindow:
	case audioMasterCloseWindow:
		break;

	default:
		return handleGeneralCallback (opcode, index, value, ptr, opt);
	}

	return 0;
}

// entry point for all callbacks from the plugin
static VstIntPtr VSTCALLBACK audioMaster (AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
{
	try
	{
		if (effect != nullptr && effect->resvd2 != 0)
		{
			return ((VSTPluginInstance*)(effect->resvd2))
						->handleCallback (opcode, index, value, ptr, opt);
		}

		return handleGeneralCallback (opcode, index, value, ptr, opt);
	}
	catch (...)
	{
		return 0;
	}
}

String VSTPluginInstance::getVersion() const
{
	unsigned int v = dispatch (effGetVendorVersion, 0, 0, 0, 0);

	String s;

	if (v == 0 || v == -1)
		v = getVersionNumber();

	if (v != 0)
	{
		int versionBits[4];
		int n = 0;

		while (v != 0)
		{
			versionBits [n++] = (v & 0xff);
			v >>= 8;
		}

		s << 'V';

		while (n > 0)
		{
			s << versionBits [--n];

			if (n > 0)
				s << '.';
		}
	}

	return s;
}

int VSTPluginInstance::getUID() const
{
	int uid = effect != nullptr ? effect->uniqueID : 0;

	if (uid == 0)
		uid = module->file.hashCode();

	return uid;
}

String VSTPluginInstance::getCategory() const
{
	const char* result = nullptr;

	switch (dispatch (effGetPlugCategory, 0, 0, 0, 0))
	{
		case kPlugCategEffect:      result = "Effect"; break;
		case kPlugCategSynth:       result = "Synth"; break;
		case kPlugCategAnalysis:    result = "Analysis"; break;
		case kPlugCategMastering:   result = "Mastering"; break;
		case kPlugCategSpacializer: result = "Spacial"; break;
		case kPlugCategRoomFx:      result = "Reverb"; break;
		case kPlugSurroundFx:       result = "Surround"; break;
		case kPlugCategRestoration: result = "Restoration"; break;
		case kPlugCategGenerator:   result = "Tone generation"; break;
		default: break;
	}

	return result;
}

float VSTPluginInstance::getParameter (int index)
{
	if (effect != nullptr && isPositiveAndBelow (index, (int) effect->numParams))
	{
		try
		{
			const ScopedLock sl (lock);
			return effect->getParameter (effect, index);
		}
		catch (...)
		{
		}
	}

	return 0.0f;
}

void VSTPluginInstance::setParameter (int index, float newValue)
{
	if (effect != nullptr && isPositiveAndBelow (index, (int) effect->numParams))
	{
		try
		{
			const ScopedLock sl (lock);

			if (effect->getParameter (effect, index) != newValue)
				effect->setParameter (effect, index, newValue);
		}
		catch (...)
		{
		}
	}
}

const String VSTPluginInstance::getParameterName (int index)
{
	if (effect != nullptr)
	{
		jassert (index >= 0 && index < effect->numParams);

		char nm [256] = { 0 };
		dispatch (effGetParamName, index, 0, nm, 0);
		return String (nm).trim();
	}

	return String::empty;
}

const String VSTPluginInstance::getParameterLabel (int index) const
{
	if (effect != nullptr)
	{
		jassert (index >= 0 && index < effect->numParams);

		char nm [256] = { 0 };
		dispatch (effGetParamLabel, index, 0, nm, 0);
		return String (nm).trim();
	}

	return String::empty;
}

const String VSTPluginInstance::getParameterText (int index)
{
	if (effect != nullptr)
	{
		jassert (index >= 0 && index < effect->numParams);

		char nm [256] = { 0 };
		dispatch (effGetParamDisplay, index, 0, nm, 0);
		return String (nm).trim();
	}

	return String::empty;
}

bool VSTPluginInstance::isParameterAutomatable (int index) const
{
	if (effect != nullptr)
	{
		jassert (index >= 0 && index < effect->numParams);
		return dispatch (effCanBeAutomated, index, 0, 0, 0) != 0;
	}

	return false;
}

void VSTPluginInstance::createTempParameterStore (MemoryBlock& dest)
{
	dest.setSize (64 + 4 * getNumParameters());
	dest.fillWith (0);

	getCurrentProgramName().copyToUTF8 ((char*) dest.getData(), 63);

	float* const p = (float*) (((char*) dest.getData()) + 64);
	for (int i = 0; i < getNumParameters(); ++i)
		p[i] = getParameter(i);
}

void VSTPluginInstance::restoreFromTempParameterStore (const MemoryBlock& m)
{
	changeProgramName (getCurrentProgram(), (const char*) m.getData());

	float* p = (float*) (((char*) m.getData()) + 64);
	for (int i = 0; i < getNumParameters(); ++i)
		setParameter (i, p[i]);
}

void VSTPluginInstance::setCurrentProgram (int newIndex)
{
	if (getNumPrograms() > 0 && newIndex != getCurrentProgram())
		dispatch (effSetProgram, 0, jlimit (0, getNumPrograms() - 1, newIndex), 0, 0);
}

const String VSTPluginInstance::getProgramName (int index)
{
	if (index == getCurrentProgram())
	{
		return getCurrentProgramName();
	}
	else if (effect != nullptr)
	{
		char nm [256] = { 0 };

		if (dispatch (effGetProgramNameIndexed,
					  jlimit (0, getNumPrograms(), index),
					  -1, nm, 0) != 0)
		{
			return String (CharPointer_UTF8 (nm)).trim();
		}
	}

	return programNames [index];
}

void VSTPluginInstance::changeProgramName (int index, const String& newName)
{
	if (index == getCurrentProgram())
	{
		if (getNumPrograms() > 0 && newName != getCurrentProgramName())
			dispatch (effSetProgramName, 0, 0, (void*) newName.substring (0, 24).toUTF8().getAddress(), 0.0f);
	}
	else
	{
		jassertfalse; // xxx not implemented!
	}
}

void VSTPluginInstance::updateStoredProgramNames()
{
	if (effect != nullptr && getNumPrograms() > 0)
	{
		char nm[256] = { 0 };

		// only do this if the plugin can't use indexed names..
		if (dispatch (effGetProgramNameIndexed, 0, -1, nm, 0) == 0)
		{
			const int oldProgram = getCurrentProgram();
			MemoryBlock oldSettings;
			createTempParameterStore (oldSettings);

			for (int i = 0; i < getNumPrograms(); ++i)
			{
				setCurrentProgram (i);
				getCurrentProgramName();  // (this updates the list)
			}

			setCurrentProgram (oldProgram);
			restoreFromTempParameterStore (oldSettings);
		}
	}
}

const String VSTPluginInstance::getCurrentProgramName()
{
	String name;

	if (effect != nullptr)
	{
		{
			char nm[256] = { 0 };
			dispatch (effGetProgramName, 0, 0, nm, 0);
			name = String (CharPointer_UTF8 (nm)).trim();
		}

		const int index = getCurrentProgram();

		if (programNames[index].isEmpty())
		{
			while (programNames.size() < index)
				programNames.add (String::empty);

			programNames.set (index, name);
		}
	}

	return name;
}

const String VSTPluginInstance::getInputChannelName (int index) const
{
	if (index >= 0 && index < getNumInputChannels())
	{
		VstPinProperties pinProps;
		if (dispatch (effGetInputProperties, index, 0, &pinProps, 0.0f) != 0)
			return String (pinProps.label, sizeof (pinProps.label));
	}

	return String::empty;
}

bool VSTPluginInstance::isInputChannelStereoPair (int index) const
{
	if (index < 0 || index >= getNumInputChannels())
		return false;

	VstPinProperties pinProps;
	if (dispatch (effGetInputProperties, index, 0, &pinProps, 0.0f) != 0)
		return (pinProps.flags & kVstPinIsStereo) != 0;

	return true;
}

const String VSTPluginInstance::getOutputChannelName (int index) const
{
	if (index >= 0 && index < getNumOutputChannels())
	{
		VstPinProperties pinProps;
		if (dispatch (effGetOutputProperties, index, 0, &pinProps, 0.0f) != 0)
			return String (pinProps.label, sizeof (pinProps.label));
	}

	return String::empty;
}

bool VSTPluginInstance::isOutputChannelStereoPair (int index) const
{
	if (index < 0 || index >= getNumOutputChannels())
		return false;

	VstPinProperties pinProps;
	if (dispatch (effGetOutputProperties, index, 0, &pinProps, 0.0f) != 0)
		return (pinProps.flags & kVstPinIsStereo) != 0;

	return true;
}

void VSTPluginInstance::setPower (const bool on)
{
	dispatch (effMainsChanged, 0, on ? 1 : 0, 0, 0);
	isPowerOn = on;
}

const int defaultMaxSizeMB = 64;

void VSTPluginInstance::getStateInformation (MemoryBlock& destData)
{
	saveToFXBFile (destData, true, defaultMaxSizeMB);
}

void VSTPluginInstance::getCurrentProgramStateInformation (MemoryBlock& destData)
{
	saveToFXBFile (destData, false, defaultMaxSizeMB);
}

void VSTPluginInstance::setStateInformation (const void* data, int sizeInBytes)
{
	loadFromFXBFile (data, sizeInBytes);
}

void VSTPluginInstance::setCurrentProgramStateInformation (const void* data, int sizeInBytes)
{
	loadFromFXBFile (data, sizeInBytes);
}

VSTPluginFormat::VSTPluginFormat()
{
}

VSTPluginFormat::~VSTPluginFormat()
{
}

void VSTPluginFormat::findAllTypesForFile (OwnedArray <PluginDescription>& results,
										   const String& fileOrIdentifier)
{
	if (! fileMightContainThisPluginType (fileOrIdentifier))
		return;

	PluginDescription desc;
	desc.fileOrIdentifier = fileOrIdentifier;
	desc.uid = 0;

	ScopedPointer <VSTPluginInstance> instance (dynamic_cast <VSTPluginInstance*> (createInstanceFromDescription (desc)));

	if (instance == 0)
		return;

	try
	{
	   #if JUCE_MAC
		if (instance->module->resFileId != 0)
			UseResFile (instance->module->resFileId);
	   #endif

		instance->fillInPluginDescription (desc);

		VstPlugCategory category = (VstPlugCategory) instance->dispatch (effGetPlugCategory, 0, 0, 0, 0);

		if (category != kPlugCategShell)
		{
			// Normal plugin...
			results.add (new PluginDescription (desc));

			instance->dispatch (effOpen, 0, 0, 0, 0);
		}
		else
		{
			// It's a shell plugin, so iterate all the subtypes...
			for (;;)
			{
				char shellEffectName [64] = { 0 };
				const int uid = instance->dispatch (effShellGetNextPlugin, 0, 0, shellEffectName, 0);

				if (uid == 0)
				{
					break;
				}
				else
				{
					desc.uid = uid;
					desc.name = shellEffectName;
					desc.descriptiveName = shellEffectName;

					bool alreadyThere = false;

					for (int i = results.size(); --i >= 0;)
					{
						PluginDescription* const d = results.getUnchecked(i);

						if (d->isDuplicateOf (desc))
						{
							alreadyThere = true;
							break;
						}
					}

					if (! alreadyThere)
						results.add (new PluginDescription (desc));
				}
			}
		}
	}
	catch (...)
	{
		// crashed while loading...
	}
}

AudioPluginInstance* VSTPluginFormat::createInstanceFromDescription (const PluginDescription& desc)
{
	ScopedPointer <VSTPluginInstance> result;

	if (fileMightContainThisPluginType (desc.fileOrIdentifier))
	{
		File file (desc.fileOrIdentifier);

		const File previousWorkingDirectory (File::getCurrentWorkingDirectory());
		file.getParentDirectory().setAsCurrentWorkingDirectory();

		const ReferenceCountedObjectPtr <ModuleHandle> module (ModuleHandle::findOrCreateModule (file));

		if (module != nullptr)
		{
			shellUIDToCreate = desc.uid;

			result = new VSTPluginInstance (module);

			if (result->effect != nullptr)
			{
				result->effect->resvd2 = (VstIntPtr) (pointer_sized_int) (VSTPluginInstance*) result;
				result->initialise();
			}
			else
			{
				result = nullptr;
			}
		}

		previousWorkingDirectory.setAsCurrentWorkingDirectory();
	}

	return result.release();
}

bool VSTPluginFormat::fileMightContainThisPluginType (const String& fileOrIdentifier)
{
	const File f (fileOrIdentifier);

  #if JUCE_MAC
	if (f.isDirectory() && f.hasFileExtension (".vst"))
		return true;

   #if JUCE_PPC
	FSRef fileRef;
	if (makeFSRefFromPath (&fileRef, f.getFullPathName()))
	{
		const short resFileId = FSOpenResFile (&fileRef, fsRdPerm);

		if (resFileId != -1)
		{
			const int numEffects = Count1Resources ('aEff');
			CloseResFile (resFileId);

			if (numEffects > 0)
				return true;
		}
	}
   #endif

	return false;
  #elif JUCE_WINDOWS
	return f.existsAsFile() && f.hasFileExtension (".dll");
  #elif JUCE_LINUX
	return f.existsAsFile() && f.hasFileExtension (".so");
  #endif
}

String VSTPluginFormat::getNameOfPluginFromIdentifier (const String& fileOrIdentifier)
{
	return fileOrIdentifier;
}

bool VSTPluginFormat::doesPluginStillExist (const PluginDescription& desc)
{
	return File (desc.fileOrIdentifier).exists();
}

StringArray VSTPluginFormat::searchPathsForPlugins (const FileSearchPath& directoriesToSearch, const bool recursive)
{
	StringArray results;

	for (int j = 0; j < directoriesToSearch.getNumPaths(); ++j)
		recursiveFileSearch (results, directoriesToSearch [j], recursive);

	return results;
}

void VSTPluginFormat::recursiveFileSearch (StringArray& results, const File& dir, const bool recursive)
{
	// avoid allowing the dir iterator to be recursive, because we want to avoid letting it delve inside
	// .component or .vst directories.
	DirectoryIterator iter (dir, false, "*", File::findFilesAndDirectories);

	while (iter.next())
	{
		const File f (iter.getFile());
		bool isPlugin = false;

		if (fileMightContainThisPluginType (f.getFullPathName()))
		{
			isPlugin = true;
			results.add (f.getFullPathName());
		}

		if (recursive && (! isPlugin) && f.isDirectory())
			recursiveFileSearch (results, f, true);
	}
}

FileSearchPath VSTPluginFormat::getDefaultLocationsToSearch()
{
   #if JUCE_MAC
	return FileSearchPath ("~/Library/Audio/Plug-Ins/VST;/Library/Audio/Plug-Ins/VST");
   #elif JUCE_WINDOWS
	const String programFiles (File::getSpecialLocation (File::globalApplicationsDirectory).getFullPathName());

	return FileSearchPath (WindowsRegistry::getValue ("HKLM\\Software\\VST\\VSTPluginsPath",
													  programFiles + "\\Steinberg\\VstPlugins"));
   #elif JUCE_LINUX
	return FileSearchPath ("/usr/lib/vst");
   #endif
}

#endif
#undef log
#endif

/*** End of inlined file: juce_VSTPluginFormat.cpp ***/


/*** Start of inlined file: juce_AudioUnitPluginFormat.mm ***/
#if JUCE_PLUGINHOST_AU && JUCE_MAC

} // (juce namespace)

#include <AudioUnit/AudioUnit.h>
#include <AudioUnit/AUCocoaUIView.h>
#include <CoreAudioKit/AUGenericView.h>

#if JUCE_SUPPORT_CARBON
 #include <AudioToolbox/AudioUnitUtilities.h>
 #include <AudioUnit/AudioUnitCarbonView.h>
#endif

namespace juce
{

#if JUCE_SUPPORT_CARBON

#endif

// Change this to disable logging of various activities
#ifndef AU_LOGGING
 #define AU_LOGGING 1
#endif

#if AU_LOGGING
 #define log(a) Logger::writeToLog(a);
#else
 #define log(a)
#endif

namespace AudioUnitFormatHelpers
{
	static int insideCallback = 0;

	String osTypeToString (OSType type)
	{
		const juce_wchar s[4] = { (juce_wchar) ((type >> 24) & 0xff),
								  (juce_wchar) ((type >> 16) & 0xff),
								  (juce_wchar) ((type >> 8) & 0xff),
								  (juce_wchar) (type & 0xff) };
		return String (s, 4);
	}

	OSType stringToOSType (const String& s1)
	{
		const String s (s1 + "    ");

		return (((OSType) (unsigned char) s[0]) << 24)
			 | (((OSType) (unsigned char) s[1]) << 16)
			 | (((OSType) (unsigned char) s[2]) << 8)
			 |  ((OSType) (unsigned char) s[3]);
	}

	static const char* auIdentifierPrefix = "AudioUnit:";

	String createAUPluginIdentifier (const ComponentDescription& desc)
	{
		jassert (osTypeToString ('abcd') == "abcd"); // agh, must have got the endianness wrong..
		jassert (stringToOSType ("abcd") == (OSType) 'abcd'); // ditto

		String s (auIdentifierPrefix);

		if (desc.componentType == kAudioUnitType_MusicDevice)
			s << "Synths/";
		else if (desc.componentType == kAudioUnitType_MusicEffect
				  || desc.componentType == kAudioUnitType_Effect)
			s << "Effects/";
		else if (desc.componentType == kAudioUnitType_Generator)
			s << "Generators/";
		else if (desc.componentType == kAudioUnitType_Panner)
			s << "Panners/";

		s << osTypeToString (desc.componentType) << ","
		  << osTypeToString (desc.componentSubType) << ","
		  << osTypeToString (desc.componentManufacturer);

		return s;
	}

	void getAUDetails (ComponentRecord* comp, String& name, String& manufacturer)
	{
		Handle componentNameHandle = NewHandle (sizeof (void*));
		Handle componentInfoHandle = NewHandle (sizeof (void*));

		if (componentNameHandle != 0 && componentInfoHandle != 0)
		{
			ComponentDescription desc;

			if (GetComponentInfo (comp, &desc, componentNameHandle, componentInfoHandle, 0) == noErr)
			{
				ConstStr255Param nameString = (ConstStr255Param) (*componentNameHandle);
				ConstStr255Param infoString = (ConstStr255Param) (*componentInfoHandle);

				if (nameString != 0 && nameString[0] != 0)
				{
					const String all ((const char*) nameString + 1, nameString[0]);
					DBG ("name: "+ all);

					manufacturer = all.upToFirstOccurrenceOf (":", false, false).trim();
					name = all.fromFirstOccurrenceOf (":", false, false).trim();
				}

				if (infoString != 0 && infoString[0] != 0)
				{
					DBG ("info: " + String ((const char*) infoString + 1, infoString[0]));
				}

				if (name.isEmpty())
					name = "<Unknown>";
			}

			DisposeHandle (componentNameHandle);
			DisposeHandle (componentInfoHandle);
		}
	}

	bool getComponentDescFromIdentifier (const String& fileOrIdentifier, ComponentDescription& desc,
										 String& name, String& version, String& manufacturer)
	{
		zerostruct (desc);

		if (fileOrIdentifier.startsWithIgnoreCase (auIdentifierPrefix))
		{
			String s (fileOrIdentifier.substring (jmax (fileOrIdentifier.lastIndexOfChar (':'),
														fileOrIdentifier.lastIndexOfChar ('/')) + 1));

			StringArray tokens;
			tokens.addTokens (s, ",", String::empty);
			tokens.trim();
			tokens.removeEmptyStrings();

			if (tokens.size() == 3)
			{
				desc.componentType = stringToOSType (tokens[0]);
				desc.componentSubType = stringToOSType (tokens[1]);
				desc.componentManufacturer = stringToOSType (tokens[2]);

				ComponentRecord* comp = FindNextComponent (0, &desc);

				if (comp != nullptr)
				{
					getAUDetails (comp, name, manufacturer);
					return true;
				}
			}
		}

		return false;
	}

	bool getComponentDescFromFile (const String& fileOrIdentifier, ComponentDescription& desc,
								   String& name, String& version, String& manufacturer)
	{
		zerostruct (desc);

		if (getComponentDescFromIdentifier (fileOrIdentifier, desc, name, version, manufacturer))
			return true;

		const File file (fileOrIdentifier);
		if (! file.hasFileExtension (".component"))
			return false;

		const char* const utf8 = fileOrIdentifier.toUTF8();
		CFURLRef url = CFURLCreateFromFileSystemRepresentation (0, (const UInt8*) utf8,
																strlen (utf8), file.isDirectory());
		if (url != 0)
		{
			CFBundleRef bundleRef = CFBundleCreate (kCFAllocatorDefault, url);
			CFRelease (url);

			if (bundleRef != 0)
			{
				CFTypeRef bundleName = CFBundleGetValueForInfoDictionaryKey (bundleRef, CFSTR("CFBundleName"));

				if (bundleName != 0 && CFGetTypeID (bundleName) == CFStringGetTypeID())
					name = String::fromCFString ((CFStringRef) bundleName);

				if (name.isEmpty())
					name = file.getFileNameWithoutExtension();

				CFTypeRef versionString = CFBundleGetValueForInfoDictionaryKey (bundleRef, CFSTR("CFBundleVersion"));

				if (versionString != 0 && CFGetTypeID (versionString) == CFStringGetTypeID())
					version = String::fromCFString ((CFStringRef) versionString);

				CFTypeRef manuString = CFBundleGetValueForInfoDictionaryKey (bundleRef, CFSTR("CFBundleGetInfoString"));

				if (manuString != 0 && CFGetTypeID (manuString) == CFStringGetTypeID())
					manufacturer = String::fromCFString ((CFStringRef) manuString);

				short resFileId = CFBundleOpenBundleResourceMap (bundleRef);
				UseResFile (resFileId);

				for (int i = 1; i <= Count1Resources ('thng'); ++i)
				{
					Handle h = Get1IndResource ('thng', i);

					if (h != 0)
					{
						HLock (h);
						const uint32* const types = (const uint32*) *h;

						if (types[0] == kAudioUnitType_MusicDevice
							 || types[0] == kAudioUnitType_MusicEffect
							 || types[0] == kAudioUnitType_Effect
							 || types[0] == kAudioUnitType_Generator
							 || types[0] == kAudioUnitType_Panner)
						{
							desc.componentType = types[0];
							desc.componentSubType = types[1];
							desc.componentManufacturer = types[2];
							break;
						}

						HUnlock (h);
						ReleaseResource (h);
					}
				}

				CFBundleCloseBundleResourceMap (bundleRef, resFileId);
				CFRelease (bundleRef);
			}
		}

		return desc.componentType != 0 && desc.componentSubType != 0;
	}

	const char* getCategory (OSType type) noexcept
	{
		switch (type)
		{
			case kAudioUnitType_Effect:
			case kAudioUnitType_MusicEffect:    return "Effect";
			case kAudioUnitType_MusicDevice:    return "Synth";
			case kAudioUnitType_Generator:      return "Generator";
			case kAudioUnitType_Panner:         return "Panner";
			default: break;
		}

		return nullptr;
	}
}

class AudioUnitPluginWindowCarbon;
class AudioUnitPluginWindowCocoa;

class AudioUnitPluginInstance     : public AudioPluginInstance
{
public:
	AudioUnitPluginInstance (const String& fileOrIdentifier)
		: fileOrIdentifier (fileOrIdentifier),
		  wantsMidiMessages (false), wasPlaying (false), prepared (false),
		  currentBuffer (nullptr),
		  numInputBusChannels (0),
		  numOutputBusChannels (0),
		  numInputBusses (0),
		  numOutputBusses (0),
		  audioUnit (0)
	{
		using namespace AudioUnitFormatHelpers;

		try
		{
			++insideCallback;

			log ("Opening AU: " + fileOrIdentifier);

			if (getComponentDescFromFile (fileOrIdentifier, componentDesc, pluginName, version, manufacturer))
			{
				ComponentRecord* const comp = FindNextComponent (0, &componentDesc);

				if (comp != nullptr)
				{
					audioUnit = (AudioUnit) OpenComponent (comp);

					wantsMidiMessages = componentDesc.componentType == kAudioUnitType_MusicDevice
						|| componentDesc.componentType == kAudioUnitType_MusicEffect;
				}
			}

			--insideCallback;
		}
		catch (...)
		{
			--insideCallback;
		}
	}

	~AudioUnitPluginInstance()
	{
		const ScopedLock sl (lock);

		jassert (AudioUnitFormatHelpers::insideCallback == 0);

		if (audioUnit != 0)
		{
			AudioUnitUninitialize (audioUnit);
			CloseComponent (audioUnit);
			audioUnit = 0;
		}
	}

	void initialise()
	{
		refreshParameterListFromPlugin();
		updateNumChannels();
		setPluginCallbacks();
		setPlayConfigDetails (numInputBusChannels * numInputBusses,
							  numOutputBusChannels * numOutputBusses, 0, 0);
		setLatencySamples (0);
	}

	// AudioPluginInstance methods:

	void fillInPluginDescription (PluginDescription& desc) const
	{
		desc.name = pluginName;
		desc.descriptiveName = pluginName;
		desc.fileOrIdentifier = AudioUnitFormatHelpers::createAUPluginIdentifier (componentDesc);
		desc.uid = ((int) componentDesc.componentType)
					^ ((int) componentDesc.componentSubType)
					^ ((int) componentDesc.componentManufacturer);
		desc.lastFileModTime = Time();
		desc.pluginFormatName = "AudioUnit";
		desc.category = AudioUnitFormatHelpers::getCategory (componentDesc.componentType);
		desc.manufacturerName = manufacturer;
		desc.version = version;
		desc.numInputChannels = getNumInputChannels();
		desc.numOutputChannels = getNumOutputChannels();
		desc.isInstrument = (componentDesc.componentType == kAudioUnitType_MusicDevice);
	}

	void* getPlatformSpecificData()             { return audioUnit; }
	const String getName() const                { return pluginName; }
	bool acceptsMidi() const                    { return wantsMidiMessages; }
	bool producesMidi() const                   { return false; }

	// AudioProcessor methods:

	void prepareToPlay (double sampleRate_, int estimatedSamplesPerBlock)
	{
		if (audioUnit != 0)
		{
			releaseResources();
			updateNumChannels();

			Float64 sampleRateIn = 0, sampleRateOut = 0;
			UInt32 sampleRateSize = sizeof (sampleRateIn);
			const Float64 sr = sampleRate_;

			for (int i = 0; i < numInputBusses; ++i)
			{
				AudioUnitGetProperty (audioUnit, kAudioUnitProperty_SampleRate, kAudioUnitScope_Input, i, &sampleRateIn, &sampleRateSize);

				if (sampleRateIn != sr)
					AudioUnitSetProperty (audioUnit, kAudioUnitProperty_SampleRate, kAudioUnitScope_Input, i, &sr, sizeof (sr));
			}

			for (int i = 0; i < numOutputBusses; ++i)
			{
				AudioUnitGetProperty (audioUnit, kAudioUnitProperty_SampleRate, kAudioUnitScope_Output, i, &sampleRateOut, &sampleRateSize);

				if (sampleRateOut != sr)
					AudioUnitSetProperty (audioUnit, kAudioUnitProperty_SampleRate, kAudioUnitScope_Output, i, &sr, sizeof (sr));
			}

			setPlayConfigDetails (numInputBusChannels * numInputBusses,
								  numOutputBusChannels * numOutputBusses,
								  sampleRate_, estimatedSamplesPerBlock);

			Float64 latencySecs = 0.0;
			UInt32 latencySize = sizeof (latencySecs);
			AudioUnitGetProperty (audioUnit, kAudioUnitProperty_Latency, kAudioUnitScope_Global,
								  0, &latencySecs, &latencySize);

			setLatencySamples (roundToInt (latencySecs * sampleRate_));

			for (int i = 0; i < numInputBusses; ++i)   AudioUnitReset (audioUnit, kAudioUnitScope_Input, i);
			for (int i = 0; i < numOutputBusses; ++i)  AudioUnitReset (audioUnit, kAudioUnitScope_Output, i);

			AudioUnitReset (audioUnit, kAudioUnitScope_Global, 0);

			{
				AudioStreamBasicDescription stream = { 0 };
				stream.mSampleRate       = sampleRate_;
				stream.mFormatID         = kAudioFormatLinearPCM;
				stream.mFormatFlags      = kAudioFormatFlagsNativeFloatPacked | kAudioFormatFlagIsNonInterleaved | kAudioFormatFlagsNativeEndian;
				stream.mFramesPerPacket  = 1;
				stream.mBytesPerPacket   = 4;
				stream.mBytesPerFrame    = 4;
				stream.mBitsPerChannel   = 32;
				stream.mChannelsPerFrame = numInputBusChannels;

				for (int i = 0; i < numInputBusses; ++i)
					AudioUnitSetProperty (audioUnit, kAudioUnitProperty_StreamFormat,
										  kAudioUnitScope_Input, i, &stream, sizeof (stream));

				stream.mChannelsPerFrame = numOutputBusChannels;

				for (int i = 0; i < numOutputBusses; ++i)
					AudioUnitSetProperty (audioUnit, kAudioUnitProperty_StreamFormat,
										  kAudioUnitScope_Output, i, &stream, sizeof (stream));
			}

			if (numOutputBusses != 0 && numOutputBusChannels != 0)
				outputBufferList.calloc (numOutputBusses, getAudioBufferSizeInBytes());

			zerostruct (timeStamp);
			timeStamp.mSampleTime = 0;
			timeStamp.mHostTime = AudioGetCurrentHostTime();
			timeStamp.mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid;

			currentBuffer = nullptr;
			wasPlaying = false;

			prepared = (AudioUnitInitialize (audioUnit) == noErr);
		}
	}

	void releaseResources()
	{
		if (prepared)
		{
			AudioUnitUninitialize (audioUnit);

			for (int i = 0; i < numInputBusses; ++i)   AudioUnitReset (audioUnit, kAudioUnitScope_Input, i);
			for (int i = 0; i < numOutputBusses; ++i)  AudioUnitReset (audioUnit, kAudioUnitScope_Output, i);

			AudioUnitReset (audioUnit, kAudioUnitScope_Global, 0);

			outputBufferList.free();
			currentBuffer = nullptr;
			prepared = false;
		}
	}

	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
	{
		const int numSamples = buffer.getNumSamples();

		if (prepared)
		{
			timeStamp.mHostTime = AudioGetCurrentHostTime();

			for (int i = 0; i < numOutputBusses; ++i)
			{
				AudioBufferList* const abl = getAudioBufferListForBus(i);
				abl->mNumberBuffers = numOutputBusChannels;

				for (int j = 0; j < numOutputBusChannels; ++j)
				{
					abl->mBuffers[j].mNumberChannels = 1;
					abl->mBuffers[j].mDataByteSize = sizeof (float) * numSamples;
					abl->mBuffers[j].mData = buffer.getSampleData (i * numOutputBusChannels + j, 0);
				}
			}

			currentBuffer = &buffer;

			if (wantsMidiMessages)
			{
				const uint8* midiEventData;
				int midiEventSize, midiEventPosition;
				MidiBuffer::Iterator i (midiMessages);

				while (i.getNextEvent (midiEventData, midiEventSize, midiEventPosition))
				{
					if (midiEventSize <= 3)
						MusicDeviceMIDIEvent (audioUnit,
											  midiEventData[0], midiEventData[1], midiEventData[2],
											  midiEventPosition);
					else
						MusicDeviceSysEx (audioUnit, midiEventData, midiEventSize);
				}

				midiMessages.clear();
			}

			AudioUnitRenderActionFlags flags = 0;

			for (int i = 0; i < numOutputBusses; ++i)
				AudioUnitRender (audioUnit, &flags, &timeStamp, i, numSamples, getAudioBufferListForBus (i));

			timeStamp.mSampleTime += numSamples;
		}
		else
		{
			// Plugin not working correctly, so just bypass..
			for (int i = 0; i < getNumOutputChannels(); ++i)
				buffer.clear (i, 0, buffer.getNumSamples());
		}
	}

	bool hasEditor() const                  { return true; }
	AudioProcessorEditor* createEditor();

	const String getInputChannelName (int index) const
	{
		if (isPositiveAndBelow (index, getNumInputChannels()))
			return "Input " + String (index + 1);

		return String::empty;
	}

	const String getOutputChannelName (int index) const
	{
		if (isPositiveAndBelow (index, getNumOutputChannels()))
			return "Output " + String (index + 1);

		return String::empty;
	}

	bool isInputChannelStereoPair (int index) const    { return isPositiveAndBelow (index, getNumInputChannels()); }
	bool isOutputChannelStereoPair (int index) const   { return isPositiveAndBelow (index, getNumOutputChannels()); }

	int getNumParameters()              { return parameterIds.size(); }

	float getParameter (int index)
	{
		const ScopedLock sl (lock);

		Float32 value = 0.0f;

		if (audioUnit != 0 && isPositiveAndBelow (index, parameterIds.size()))
			AudioUnitGetParameter (audioUnit,
								   (UInt32) parameterIds.getUnchecked (index),
								   kAudioUnitScope_Global, 0,
								   &value);

		return value;
	}

	void setParameter (int index, float newValue)
	{
		const ScopedLock sl (lock);

		if (audioUnit != 0 && isPositiveAndBelow (index, parameterIds.size()))
			AudioUnitSetParameter (audioUnit,
								   (UInt32) parameterIds.getUnchecked (index),
								   kAudioUnitScope_Global, 0,
								   newValue, 0);
	}

	const String getParameterName (int index)
	{
		AudioUnitParameterInfo info = { 0 };
		UInt32 sz = sizeof (info);
		String name;

		if (AudioUnitGetProperty (audioUnit,
								  kAudioUnitProperty_ParameterInfo,
								  kAudioUnitScope_Global,
								  parameterIds [index], &info, &sz) == noErr)
		{
			if ((info.flags & kAudioUnitParameterFlag_HasCFNameString) != 0)
				name = String::fromCFString (info.cfNameString);
			else
				name = String (info.name, sizeof (info.name));
		}

		return name;
	}

	const String getParameterText (int index)   { return String (getParameter (index)); }

	bool isParameterAutomatable (int index) const
	{
		AudioUnitParameterInfo info;
		UInt32 sz = sizeof (info);

		if (AudioUnitGetProperty (audioUnit, kAudioUnitProperty_ParameterInfo,
								  kAudioUnitScope_Global, parameterIds [index], &info, &sz) == noErr)
		{
			return (info.flags & kAudioUnitParameterFlag_NonRealTime) == 0;
		}

		return true;
	}

	int getNumPrograms()
	{
		CFArrayRef presets;
		UInt32 sz = sizeof (CFArrayRef);
		int num = 0;

		if (AudioUnitGetProperty (audioUnit, kAudioUnitProperty_FactoryPresets,
								  kAudioUnitScope_Global, 0, &presets, &sz) == noErr)
		{
			num = (int) CFArrayGetCount (presets);
			CFRelease (presets);
		}

		return num;
	}

	int getCurrentProgram()
	{
		AUPreset current;
		current.presetNumber = 0;
		UInt32 sz = sizeof (AUPreset);

		AudioUnitGetProperty (audioUnit, kAudioUnitProperty_PresentPreset,
							  kAudioUnitScope_Global, 0, &current, &sz);

		return current.presetNumber;
	}

	void setCurrentProgram (int newIndex)
	{
		AUPreset current;
		current.presetNumber = newIndex;
		current.presetName = CFSTR("");

		AudioUnitSetProperty (audioUnit, kAudioUnitProperty_PresentPreset,
							  kAudioUnitScope_Global, 0, &current, sizeof (AUPreset));
	}

	const String getProgramName (int index)
	{
		String s;
		CFArrayRef presets;
		UInt32 sz = sizeof (CFArrayRef);

		if (AudioUnitGetProperty (audioUnit, kAudioUnitProperty_FactoryPresets,
								  kAudioUnitScope_Global, 0, &presets, &sz) == noErr)
		{
			for (CFIndex i = 0; i < CFArrayGetCount (presets); ++i)
			{
				const AUPreset* p = (const AUPreset*) CFArrayGetValueAtIndex (presets, i);

				if (p != nullptr && p->presetNumber == index)
				{
					s = String::fromCFString (p->presetName);
					break;
				}
			}

			CFRelease (presets);
		}

		return s;
	}

	void changeProgramName (int index, const String& newName)
	{
		jassertfalse; // xxx not implemented!
	}

	void getStateInformation (MemoryBlock& destData)
	{
		getCurrentProgramStateInformation (destData);
	}

	void getCurrentProgramStateInformation (MemoryBlock& destData)
	{
		CFPropertyListRef propertyList = 0;
		UInt32 sz = sizeof (CFPropertyListRef);

		if (AudioUnitGetProperty (audioUnit,
								  kAudioUnitProperty_ClassInfo,
								  kAudioUnitScope_Global,
								  0, &propertyList, &sz) == noErr)
		{
			CFWriteStreamRef stream = CFWriteStreamCreateWithAllocatedBuffers (kCFAllocatorDefault, kCFAllocatorDefault);
			CFWriteStreamOpen (stream);

			CFIndex bytesWritten = CFPropertyListWriteToStream (propertyList, stream, kCFPropertyListBinaryFormat_v1_0, 0);
			CFWriteStreamClose (stream);

			CFDataRef data = (CFDataRef) CFWriteStreamCopyProperty (stream, kCFStreamPropertyDataWritten);

			destData.setSize (bytesWritten);
			destData.copyFrom (CFDataGetBytePtr (data), 0, destData.getSize());
			CFRelease (data);

			CFRelease (stream);
			CFRelease (propertyList);
		}
	}

	void setStateInformation (const void* data, int sizeInBytes)
	{
		setCurrentProgramStateInformation (data, sizeInBytes);
	}

	void setCurrentProgramStateInformation (const void* data, int sizeInBytes)
	{
		CFReadStreamRef stream = CFReadStreamCreateWithBytesNoCopy (kCFAllocatorDefault,
																	(const UInt8*) data,
																	sizeInBytes,
																	kCFAllocatorNull);
		CFReadStreamOpen (stream);

		CFPropertyListFormat format = kCFPropertyListBinaryFormat_v1_0;
		CFPropertyListRef propertyList = CFPropertyListCreateFromStream (kCFAllocatorDefault,
																		 stream,
																		 0,
																		 kCFPropertyListImmutable,
																		 &format,
																		 0);
		CFRelease (stream);

		if (propertyList != 0)
			AudioUnitSetProperty (audioUnit,
								  kAudioUnitProperty_ClassInfo,
								  kAudioUnitScope_Global,
								  0, &propertyList, sizeof (propertyList));
	}

	void refreshParameterListFromPlugin()
	{
		parameterIds.clear();

		if (audioUnit != 0)
		{
			UInt32 paramListSize = 0;
			AudioUnitGetProperty (audioUnit, kAudioUnitProperty_ParameterList, kAudioUnitScope_Global,
								  0, 0, &paramListSize);

			if (paramListSize > 0)
			{
				parameterIds.insertMultiple (0, 0, paramListSize / sizeof (int));

				AudioUnitGetProperty (audioUnit, kAudioUnitProperty_ParameterList, kAudioUnitScope_Global,
									  0, parameterIds.getRawDataPointer(), &paramListSize);
			}
		}
	}

private:

	friend class AudioUnitPluginWindowCarbon;
	friend class AudioUnitPluginWindowCocoa;
	friend class AudioUnitPluginFormat;

	ComponentDescription componentDesc;
	String pluginName, manufacturer, version;
	String fileOrIdentifier;
	CriticalSection lock;
	bool wantsMidiMessages, wasPlaying, prepared;

	HeapBlock <AudioBufferList> outputBufferList;
	AudioTimeStamp timeStamp;
	AudioSampleBuffer* currentBuffer;
	int numInputBusChannels, numOutputBusChannels, numInputBusses, numOutputBusses;

	AudioUnit audioUnit;
	Array <int> parameterIds;

	void setPluginCallbacks()
	{
		if (audioUnit != 0)
		{
			{
				AURenderCallbackStruct info = { 0 };
				info.inputProcRefCon = this;
				info.inputProc = renderGetInputCallback;

				for (int i = 0; i < numInputBusses; ++i)
					AudioUnitSetProperty (audioUnit, kAudioUnitProperty_SetRenderCallback,
										  kAudioUnitScope_Input, i, &info, sizeof (info));
			}

			{
				HostCallbackInfo info = { 0 };
				info.hostUserData = this;
				info.beatAndTempoProc = getBeatAndTempoCallback;
				info.musicalTimeLocationProc = getMusicalTimeLocationCallback;
				info.transportStateProc = getTransportStateCallback;

				AudioUnitSetProperty (audioUnit, kAudioUnitProperty_HostCallbacks, kAudioUnitScope_Global,
									  0, &info, sizeof (info));
			}
		}
	}

	OSStatus renderGetInput (AudioUnitRenderActionFlags* ioActionFlags,
							 const AudioTimeStamp* inTimeStamp,
							 UInt32 inBusNumber,
							 UInt32 inNumberFrames,
							 AudioBufferList* ioData) const
	{
		if (currentBuffer != nullptr)
		{
			jassert (inNumberFrames == currentBuffer->getNumSamples()); // if this ever happens, might need to add extra handling

			for (int i = 0; i < ioData->mNumberBuffers; ++i)
			{
				const int bufferChannel = inBusNumber * numInputBusChannels + i;

				if (bufferChannel < currentBuffer->getNumChannels())
				{
					memcpy (ioData->mBuffers[i].mData,
							currentBuffer->getSampleData (bufferChannel, 0),
							sizeof (float) * inNumberFrames);
				}
				else
				{
					zeromem (ioData->mBuffers[i].mData,
							 sizeof (float) * inNumberFrames);
				}
			}
		}

		return noErr;
	}

	OSStatus getBeatAndTempo (Float64* outCurrentBeat, Float64* outCurrentTempo) const
	{
		AudioPlayHead* const ph = getPlayHead();
		AudioPlayHead::CurrentPositionInfo result;

		if (ph != nullptr && ph->getCurrentPosition (result))
		{
			if (outCurrentBeat  != nullptr)    *outCurrentBeat  = result.ppqPosition;
			if (outCurrentTempo != nullptr)    *outCurrentTempo = result.bpm;
		}
		else
		{
			if (outCurrentBeat  != nullptr)    *outCurrentBeat  = 0;
			if (outCurrentTempo != nullptr)    *outCurrentTempo = 120.0;
		}

		return noErr;
	}

	OSStatus getMusicalTimeLocation (UInt32* outDeltaSampleOffsetToNextBeat, Float32* outTimeSig_Numerator,
									 UInt32* outTimeSig_Denominator, Float64* outCurrentMeasureDownBeat) const
	{
		AudioPlayHead* const ph = getPlayHead();
		AudioPlayHead::CurrentPositionInfo result;

		if (ph != nullptr && ph->getCurrentPosition (result))
		{
			if (outTimeSig_Numerator != nullptr)            *outTimeSig_Numerator   = result.timeSigNumerator;
			if (outTimeSig_Denominator != nullptr)          *outTimeSig_Denominator = result.timeSigDenominator;
			if (outDeltaSampleOffsetToNextBeat != nullptr)  *outDeltaSampleOffsetToNextBeat = 0; //xxx
			if (outCurrentMeasureDownBeat != nullptr)       *outCurrentMeasureDownBeat = result.ppqPositionOfLastBarStart; //xxx wrong
		}
		else
		{
			if (outDeltaSampleOffsetToNextBeat != nullptr)  *outDeltaSampleOffsetToNextBeat = 0;
			if (outTimeSig_Numerator != nullptr)            *outTimeSig_Numerator = 4;
			if (outTimeSig_Denominator != nullptr)          *outTimeSig_Denominator = 4;
			if (outCurrentMeasureDownBeat != nullptr)       *outCurrentMeasureDownBeat = 0;
		}

		return noErr;
	}

	OSStatus getTransportState (Boolean* outIsPlaying, Boolean* outTransportStateChanged,
								Float64* outCurrentSampleInTimeLine, Boolean* outIsCycling,
								Float64* outCycleStartBeat, Float64* outCycleEndBeat)
	{
		AudioPlayHead* const ph = getPlayHead();
		AudioPlayHead::CurrentPositionInfo result;

		if (ph != nullptr && ph->getCurrentPosition (result))
		{
			if (outIsPlaying != nullptr)
				*outIsPlaying = result.isPlaying;

			if (outTransportStateChanged != nullptr)
			{
				*outTransportStateChanged = result.isPlaying != wasPlaying;
				wasPlaying = result.isPlaying;
			}

			if (outCurrentSampleInTimeLine != nullptr)
				*outCurrentSampleInTimeLine = roundToInt (result.timeInSeconds * getSampleRate());

			if (outIsCycling != nullptr)        *outIsCycling = false;
			if (outCycleStartBeat != nullptr)   *outCycleStartBeat = 0;
			if (outCycleEndBeat != nullptr)     *outCycleEndBeat = 0;
		}
		else
		{
			if (outIsPlaying != nullptr)                *outIsPlaying = false;
			if (outTransportStateChanged != nullptr)    *outTransportStateChanged = false;
			if (outCurrentSampleInTimeLine != nullptr)  *outCurrentSampleInTimeLine = 0;
			if (outIsCycling != nullptr)                *outIsCycling = false;
			if (outCycleStartBeat != nullptr)           *outCycleStartBeat = 0;
			if (outCycleEndBeat != nullptr)             *outCycleEndBeat = 0;
		}

		return noErr;
	}

	static OSStatus renderGetInputCallback (void* inRefCon, AudioUnitRenderActionFlags* ioActionFlags,
											const AudioTimeStamp* inTimeStamp, UInt32 inBusNumber,
											UInt32 inNumberFrames, AudioBufferList* ioData)
	{
		return static_cast <AudioUnitPluginInstance*> (inRefCon)
					->renderGetInput (ioActionFlags, inTimeStamp, inBusNumber, inNumberFrames, ioData);
	}

	static OSStatus getBeatAndTempoCallback (void* inHostUserData, Float64* outCurrentBeat, Float64* outCurrentTempo)
	{
		return static_cast <AudioUnitPluginInstance*> (inHostUserData)
					->getBeatAndTempo (outCurrentBeat, outCurrentTempo);
	}

	static OSStatus getMusicalTimeLocationCallback (void* inHostUserData, UInt32* outDeltaSampleOffsetToNextBeat,
													Float32* outTimeSig_Numerator, UInt32* outTimeSig_Denominator,
													Float64* outCurrentMeasureDownBeat)
	{
		return static_cast <AudioUnitPluginInstance*> (inHostUserData)
					->getMusicalTimeLocation (outDeltaSampleOffsetToNextBeat, outTimeSig_Numerator,
											  outTimeSig_Denominator, outCurrentMeasureDownBeat);
	}

	static OSStatus getTransportStateCallback (void* inHostUserData, Boolean* outIsPlaying, Boolean* outTransportStateChanged,
											   Float64* outCurrentSampleInTimeLine, Boolean* outIsCycling,
											   Float64* outCycleStartBeat, Float64* outCycleEndBeat)
	{
		return static_cast <AudioUnitPluginInstance*> (inHostUserData)
					->getTransportState (outIsPlaying, outTransportStateChanged, outCurrentSampleInTimeLine,
										 outIsCycling, outCycleStartBeat, outCycleEndBeat);
	}

	size_t getAudioBufferSizeInBytes() const noexcept
	{
		return offsetof (AudioBufferList, mBuffers) + (sizeof (AudioBuffer) * numOutputBusChannels);
	}

	AudioBufferList* getAudioBufferListForBus (int busIndex) const noexcept
	{
		return addBytesToPointer (outputBufferList.getData(), getAudioBufferSizeInBytes() * busIndex);
	}

	int getElementCount (AudioUnitScope scope) const noexcept
	{
		UInt32 count;
		UInt32 countSize = sizeof (count);

		if (AudioUnitGetProperty (audioUnit, kAudioUnitProperty_ElementCount, scope, 0, &count, &countSize) != noErr
			 || countSize == 0)
			count = 1;

		return (int) count;
	}

	void updateNumChannels()
	{
		numInputBusses = getElementCount (kAudioUnitScope_Input);
		numOutputBusses = getElementCount (kAudioUnitScope_Output);

		AUChannelInfo supportedChannels [128];
		UInt32 supportedChannelsSize = sizeof (supportedChannels);

		if (AudioUnitGetProperty (audioUnit, kAudioUnitProperty_SupportedNumChannels, kAudioUnitScope_Global,
								  0, supportedChannels, &supportedChannelsSize) == noErr
			 && supportedChannelsSize > 0)
		{
			int explicitNumIns = 0;
			int explicitNumOuts = 0;
			int maximumNumIns = 0;
			int maximumNumOuts = 0;

			for (int i = 0; i < supportedChannelsSize / sizeof (AUChannelInfo); ++i)
			{
				const int inChannels  = (int) supportedChannels[i].inChannels;
				const int outChannels = (int) supportedChannels[i].outChannels;

				if (inChannels < 0)
					maximumNumIns  = jmin (maximumNumIns, inChannels);
				else
					explicitNumIns = jmax (explicitNumIns, inChannels);

				if (outChannels < 0)
					maximumNumOuts  = jmin (maximumNumOuts, outChannels);
				else
					explicitNumOuts = jmax (explicitNumOuts, outChannels);
			}

			if ((maximumNumIns == -1 && maximumNumOuts == -1)  // (special meaning: any number of ins/outs, as long as they match)
				|| (maximumNumIns == -2 && maximumNumOuts == -1) // (special meaning: any number of ins/outs, even if they don't match)
				|| (maximumNumIns == -1 && maximumNumOuts == -2))
			{
				numInputBusChannels = numOutputBusChannels = 2;
			}
			else
			{
				numInputBusChannels = explicitNumIns;
				numOutputBusChannels = explicitNumOuts;

				if (maximumNumIns == -1 || (maximumNumIns < 0 && explicitNumIns <= -maximumNumIns))
					numInputBusChannels = 2;

				if (maximumNumOuts == -1 || (maximumNumOuts < 0 && explicitNumOuts <= -maximumNumOuts))
					numOutputBusChannels = 2;
			}
		}
		else
		{
			// (this really means the plugin will take any number of ins/outs as long
			// as they are the same)
			numInputBusChannels = numOutputBusChannels = 2;
		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioUnitPluginInstance);
};

class AudioUnitPluginWindowCocoa    : public AudioProcessorEditor,
									  public Timer
{
public:
	AudioUnitPluginWindowCocoa (AudioUnitPluginInstance& plugin_, const bool createGenericViewIfNeeded)
		: AudioProcessorEditor (&plugin_),
		  plugin (plugin_)
	{
		addAndMakeVisible (&wrapper);

		setOpaque (true);
		setVisible (true);
		setSize (100, 100);

		createView (createGenericViewIfNeeded);
	}

	~AudioUnitPluginWindowCocoa()
	{
		const bool wasValid = isValid();

		wrapper.setVisible (false);
		removeChildComponent (&wrapper);
		wrapper.setView (nil);

		if (wasValid)
			plugin.editorBeingDeleted (this);
	}

	bool isValid() const        { return wrapper.getView() != nil; }

	void paint (Graphics& g)
	{
		g.fillAll (Colours::white);
	}

	void resized()
	{
		wrapper.setSize (getWidth(), getHeight());
	}

	void timerCallback()
	{
		wrapper.resizeToFitView();
		startTimer (jmin (713, getTimerInterval() + 51));
	}

	void childBoundsChanged (Component* child)
	{
		setSize (wrapper.getWidth(), wrapper.getHeight());
		startTimer (70);
	}

private:
	AudioUnitPluginInstance& plugin;
	NSViewComponent wrapper;

	bool createView (const bool createGenericViewIfNeeded)
	{
		NSView* pluginView = nil;
		UInt32 dataSize = 0;
		Boolean isWritable = false;

		AudioUnitInitialize (plugin.audioUnit);

		if (AudioUnitGetPropertyInfo (plugin.audioUnit, kAudioUnitProperty_CocoaUI, kAudioUnitScope_Global,
									  0, &dataSize, &isWritable) == noErr
			 && dataSize != 0
			 && AudioUnitGetPropertyInfo (plugin.audioUnit, kAudioUnitProperty_CocoaUI, kAudioUnitScope_Global,
										  0, &dataSize, &isWritable) == noErr)
		{
			HeapBlock <AudioUnitCocoaViewInfo> info;
			info.calloc (dataSize, 1);

			if (AudioUnitGetProperty (plugin.audioUnit, kAudioUnitProperty_CocoaUI, kAudioUnitScope_Global,
									  0, info, &dataSize) == noErr)
			{
				NSString* viewClassName = (NSString*) (info->mCocoaAUViewClass[0]);
				CFStringRef path = CFURLCopyPath (info->mCocoaAUViewBundleLocation);
				NSString* unescapedPath = (NSString*) CFURLCreateStringByReplacingPercentEscapes (0, path, CFSTR (""));
				CFRelease (path);
				NSBundle* viewBundle = [NSBundle bundleWithPath: [unescapedPath autorelease]];
				Class viewClass = [viewBundle classNamed: viewClassName];

				if ([viewClass conformsToProtocol: @protocol (AUCocoaUIBase)]
					 && [viewClass instancesRespondToSelector: @selector (interfaceVersion)]
					 && [viewClass instancesRespondToSelector: @selector (uiViewForAudioUnit: withSize:)])
				{
					id factory = [[[viewClass alloc] init] autorelease];
					pluginView = [factory uiViewForAudioUnit: plugin.audioUnit
													withSize: NSMakeSize (getWidth(), getHeight())];
				}

				for (int i = (dataSize - sizeof (CFURLRef)) / sizeof (CFStringRef); --i >= 0;)
					CFRelease (info->mCocoaAUViewClass[i]);

				CFRelease (info->mCocoaAUViewBundleLocation);
			}
		}

		if (createGenericViewIfNeeded && (pluginView == 0))
		{
			{
				// This forces CoreAudio.component to be loaded, otherwise the AUGenericView will assert
				ComponentDescription desc;
				String name, version, manufacturer;
				AudioUnitFormatHelpers::getComponentDescFromIdentifier ("AudioUnit:Output/auou,genr,appl",
																		desc, name, version, manufacturer);
			}

			pluginView = [[AUGenericView alloc] initWithAudioUnit: plugin.audioUnit];
		}

		wrapper.setView (pluginView);

		if (pluginView != nil)
		{
			timerCallback();
			startTimer (70);
		}

		return pluginView != nil;
	}
};

#if JUCE_SUPPORT_CARBON

class AudioUnitPluginWindowCarbon   : public AudioProcessorEditor
{
public:
	AudioUnitPluginWindowCarbon (AudioUnitPluginInstance& plugin_)
		: AudioProcessorEditor (&plugin_),
		  plugin (plugin_),
		  componentRecord (nullptr),
		  viewComponent (0)
	{
		addAndMakeVisible (innerWrapper = new InnerWrapperComponent (*this));

		setOpaque (true);
		setVisible (true);
		setSize (400, 300);

		UInt32 propertySize;
		if (AudioUnitGetPropertyInfo (plugin.audioUnit, kAudioUnitProperty_GetUIComponentList,
									  kAudioUnitScope_Global, 0, &propertySize, NULL) == noErr
			 && propertySize > 0)
		{
			ComponentDescription views [propertySize / sizeof (ComponentDescription)];

			if (AudioUnitGetProperty (plugin.audioUnit, kAudioUnitProperty_GetUIComponentList,
									  kAudioUnitScope_Global, 0, &views[0], &propertySize) == noErr)
			{
				componentRecord = FindNextComponent (0, &views[0]);
			}
		}
	}

	~AudioUnitPluginWindowCarbon()
	{
		innerWrapper = nullptr;

		if (isValid())
			plugin.editorBeingDeleted (this);
	}

	bool isValid() const noexcept           { return componentRecord != nullptr; }

	void paint (Graphics& g)
	{
		g.fillAll (Colours::black);
	}

	void resized()
	{
		if (innerWrapper != nullptr)
			innerWrapper->setSize (getWidth(), getHeight());
	}

	bool keyStateChanged (bool)         { return false; }
	bool keyPressed (const KeyPress&)   { return false; }

	AudioUnit getAudioUnit() const      { return plugin.audioUnit; }

	AudioUnitCarbonView getViewComponent()
	{
		if (viewComponent == 0 && componentRecord != nullptr)
			viewComponent = (AudioUnitCarbonView) OpenComponent (componentRecord);

		return viewComponent;
	}

	void closeViewComponent()
	{
		if (viewComponent != 0)
		{
			log ("Closing AU GUI: " + plugin.getName());

			CloseComponent (viewComponent);
			viewComponent = 0;
		}
	}

private:

	AudioUnitPluginInstance& plugin;
	ComponentRecord* componentRecord;
	AudioUnitCarbonView viewComponent;

	class InnerWrapperComponent   : public CarbonViewWrapperComponent
	{
	public:
		InnerWrapperComponent (AudioUnitPluginWindowCarbon& owner_)
			: owner (owner_)
		{
		}

		~InnerWrapperComponent()
		{
			deleteWindow();
		}

		HIViewRef attachView (WindowRef windowRef, HIViewRef rootView)
		{
			log ("Opening AU GUI: " + owner.plugin.getName());

			AudioUnitCarbonView viewComponent = owner.getViewComponent();

			if (viewComponent == 0)
				return 0;

			Float32Point pos = { 0, 0 };
			Float32Point size = { 250, 200 };

			HIViewRef pluginView = 0;

			AudioUnitCarbonViewCreate (viewComponent,
									   owner.getAudioUnit(),
									   windowRef,
									   rootView,
									   &pos,
									   &size,
									   (ControlRef*) &pluginView);

			return pluginView;
		}

		void removeView (HIViewRef)
		{
			owner.closeViewComponent();
		}

	private:
		AudioUnitPluginWindowCarbon& owner;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InnerWrapperComponent);
	};

	friend class InnerWrapperComponent;
	ScopedPointer<InnerWrapperComponent> innerWrapper;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioUnitPluginWindowCarbon);
};

#endif

AudioProcessorEditor* AudioUnitPluginInstance::createEditor()
{
	ScopedPointer<AudioProcessorEditor> w (new AudioUnitPluginWindowCocoa (*this, false));

	if (! static_cast <AudioUnitPluginWindowCocoa*> (w.get())->isValid())
		w = nullptr;

   #if JUCE_SUPPORT_CARBON
	if (w == nullptr)
	{
		w = new AudioUnitPluginWindowCarbon (*this);

		if (! static_cast <AudioUnitPluginWindowCarbon*> (w.get())->isValid())
			w = nullptr;
	}
   #endif

	if (w == nullptr)
		w = new AudioUnitPluginWindowCocoa (*this, true); // use AUGenericView as a fallback

	return w.release();
}

AudioUnitPluginFormat::AudioUnitPluginFormat()
{
}

AudioUnitPluginFormat::~AudioUnitPluginFormat()
{
}

void AudioUnitPluginFormat::findAllTypesForFile (OwnedArray <PluginDescription>& results,
												 const String& fileOrIdentifier)
{
	if (! fileMightContainThisPluginType (fileOrIdentifier))
		return;

	PluginDescription desc;
	desc.fileOrIdentifier = fileOrIdentifier;
	desc.uid = 0;

	try
	{
		ScopedPointer <AudioPluginInstance> createdInstance (createInstanceFromDescription (desc));
		AudioUnitPluginInstance* const auInstance = dynamic_cast <AudioUnitPluginInstance*> ((AudioPluginInstance*) createdInstance);

		if (auInstance != nullptr)
		{
			auInstance->fillInPluginDescription (desc);
			results.add (new PluginDescription (desc));
		}
	}
	catch (...)
	{
		// crashed while loading...
	}
}

AudioPluginInstance* AudioUnitPluginFormat::createInstanceFromDescription (const PluginDescription& desc)
{
	if (fileMightContainThisPluginType (desc.fileOrIdentifier))
	{
		ScopedPointer <AudioUnitPluginInstance> result (new AudioUnitPluginInstance (desc.fileOrIdentifier));

		if (result->audioUnit != 0)
		{
			result->initialise();
			return result.release();
		}
	}

	return nullptr;
}

StringArray AudioUnitPluginFormat::searchPathsForPlugins (const FileSearchPath& /*directoriesToSearch*/,
														  const bool /*recursive*/)
{
	StringArray result;
	ComponentRecord* comp = nullptr;

	for (;;)
	{
		ComponentDescription desc = { 0 };
		comp = FindNextComponent (comp, &desc);

		if (comp == 0)
			break;

		GetComponentInfo (comp, &desc, 0, 0, 0);

		if (desc.componentType == kAudioUnitType_MusicDevice
			 || desc.componentType == kAudioUnitType_MusicEffect
			 || desc.componentType == kAudioUnitType_Effect
			 || desc.componentType == kAudioUnitType_Generator
			 || desc.componentType == kAudioUnitType_Panner)
		{
			const String s (AudioUnitFormatHelpers::createAUPluginIdentifier (desc));
			DBG (s);
			result.add (s);
		}
	}

	return result;
}

bool AudioUnitPluginFormat::fileMightContainThisPluginType (const String& fileOrIdentifier)
{
	ComponentDescription desc;

	String name, version, manufacturer;
	if (AudioUnitFormatHelpers::getComponentDescFromIdentifier (fileOrIdentifier, desc, name, version, manufacturer))
		return FindNextComponent (0, &desc) != 0;

	const File f (fileOrIdentifier);

	return f.hasFileExtension (".component")
			 && f.isDirectory();
}

String AudioUnitPluginFormat::getNameOfPluginFromIdentifier (const String& fileOrIdentifier)
{
	ComponentDescription desc;
	String name, version, manufacturer;
	AudioUnitFormatHelpers::getComponentDescFromIdentifier (fileOrIdentifier, desc, name, version, manufacturer);

	if (name.isEmpty())
		name = fileOrIdentifier;

	return name;
}

bool AudioUnitPluginFormat::doesPluginStillExist (const PluginDescription& desc)
{
	if (desc.fileOrIdentifier.startsWithIgnoreCase (AudioUnitFormatHelpers::auIdentifierPrefix))
		return fileMightContainThisPluginType (desc.fileOrIdentifier);
	else
		return File (desc.fileOrIdentifier).exists();
}

FileSearchPath AudioUnitPluginFormat::getDefaultLocationsToSearch()
{
	return FileSearchPath ("/(Default AudioUnit locations)");
}

#undef log

#endif

/*** End of inlined file: juce_AudioUnitPluginFormat.mm ***/


/*** Start of inlined file: juce_KnownPluginList.cpp ***/
KnownPluginList::KnownPluginList()  {}
KnownPluginList::~KnownPluginList() {}

void KnownPluginList::clear()
{
	if (types.size() > 0)
	{
		types.clear();
		sendChangeMessage();
	}
}

PluginDescription* KnownPluginList::getTypeForFile (const String& fileOrIdentifier) const
{
	for (int i = 0; i < types.size(); ++i)
		if (types.getUnchecked(i)->fileOrIdentifier == fileOrIdentifier)
			return types.getUnchecked(i);

	return nullptr;
}

PluginDescription* KnownPluginList::getTypeForIdentifierString (const String& identifierString) const
{
	for (int i = 0; i < types.size(); ++i)
		if (types.getUnchecked(i)->createIdentifierString() == identifierString)
			return types.getUnchecked(i);

	return nullptr;
}

bool KnownPluginList::addType (const PluginDescription& type)
{
	for (int i = types.size(); --i >= 0;)
	{
		if (types.getUnchecked(i)->isDuplicateOf (type))
		{
			// strange - found a duplicate plugin with different info..
			jassert (types.getUnchecked(i)->name == type.name);
			jassert (types.getUnchecked(i)->isInstrument == type.isInstrument);

			*types.getUnchecked(i) = type;
			return false;
		}
	}

	types.add (new PluginDescription (type));
	sendChangeMessage();
	return true;
}

void KnownPluginList::removeType (const int index)
{
	types.remove (index);
	sendChangeMessage();
}

namespace
{
	Time getPluginFileModTime (const String& fileOrIdentifier)
	{
		if (fileOrIdentifier.startsWithChar ('/') || fileOrIdentifier[1] == ':')
			return File (fileOrIdentifier).getLastModificationTime();

		return Time();
	}

	bool timesAreDifferent (const Time& t1, const Time& t2) noexcept
	{
		return t1 != t2 || t1 == Time();
	}

	enum { menuIdBase = 0x324503f4 };
}

bool KnownPluginList::isListingUpToDate (const String& fileOrIdentifier) const
{
	if (getTypeForFile (fileOrIdentifier) == 0)
		return false;

	for (int i = types.size(); --i >= 0;)
	{
		const PluginDescription* const d = types.getUnchecked(i);

		if (d->fileOrIdentifier == fileOrIdentifier
			 && timesAreDifferent (d->lastFileModTime, getPluginFileModTime (fileOrIdentifier)))
		{
			return false;
		}
	}

	return true;
}

bool KnownPluginList::scanAndAddFile (const String& fileOrIdentifier,
									  const bool dontRescanIfAlreadyInList,
									  OwnedArray <PluginDescription>& typesFound,
									  AudioPluginFormat& format)
{
	bool addedOne = false;

	if (dontRescanIfAlreadyInList
		 && getTypeForFile (fileOrIdentifier) != nullptr)
	{
		bool needsRescanning = false;

		for (int i = types.size(); --i >= 0;)
		{
			const PluginDescription* const d = types.getUnchecked(i);

			if (d->fileOrIdentifier == fileOrIdentifier && d->pluginFormatName == format.getName())
			{
				if (timesAreDifferent (d->lastFileModTime, getPluginFileModTime (fileOrIdentifier)))
					needsRescanning = true;
				else
					typesFound.add (new PluginDescription (*d));
			}
		}

		if (! needsRescanning)
			return false;
	}

	OwnedArray <PluginDescription> found;
	format.findAllTypesForFile (found, fileOrIdentifier);

	for (int i = 0; i < found.size(); ++i)
	{
		PluginDescription* const desc = found.getUnchecked(i);
		jassert (desc != nullptr);

		if (addType (*desc))
		{
			addedOne = true;
			typesFound.add (new PluginDescription (*desc));
		}
	}

	return addedOne;
}

void KnownPluginList::scanAndAddDragAndDroppedFiles (const StringArray& files,
													 OwnedArray <PluginDescription>& typesFound)
{
	for (int i = 0; i < files.size(); ++i)
	{
		for (int j = 0; j < AudioPluginFormatManager::getInstance()->getNumFormats(); ++j)
		{
			AudioPluginFormat* const format = AudioPluginFormatManager::getInstance()->getFormat (j);

			if (scanAndAddFile (files[i], true, typesFound, *format))
				return;
		}

		const File f (files[i]);

		if (f.isDirectory())
		{
			StringArray s;

			{
				Array<File> subFiles;
				f.findChildFiles (subFiles, File::findFilesAndDirectories, false);

				for (int j = 0; j < subFiles.size(); ++j)
					s.add (subFiles.getReference(j).getFullPathName());
			}

			scanAndAddDragAndDroppedFiles (s, typesFound);
		}
	}
}

struct PluginSorter
{
	PluginSorter (KnownPluginList::SortMethod method_) noexcept  : method (method_) {}

	int compareElements (const PluginDescription* const first,
						 const PluginDescription* const second) const
	{
		int diff = 0;

		switch (method)
		{
			case KnownPluginList::sortByCategory:           diff = first->category.compareLexicographically (second->category); break;
			case KnownPluginList::sortByManufacturer:       diff = first->manufacturerName.compareLexicographically (second->manufacturerName); break;
			case KnownPluginList::sortByFileSystemLocation: diff = lastPathPart (first->fileOrIdentifier).compare (lastPathPart (second->fileOrIdentifier)); break;
			default: break;
		}

		if (diff == 0)
			diff = first->name.compareLexicographically (second->name);

		return diff;
	}

private:
	static String lastPathPart (const String& path)
	{
		return path.replaceCharacter ('\\', '/').upToLastOccurrenceOf ("/", false, false);
	}

	KnownPluginList::SortMethod method;
};

void KnownPluginList::sort (const SortMethod method)
{
	if (method != defaultOrder)
	{
		PluginSorter sorter (method);
		types.sort (sorter, true);

		sendChangeMessage();
	}
}

XmlElement* KnownPluginList::createXml() const
{
	XmlElement* const e = new XmlElement ("KNOWNPLUGINS");

	for (int i = 0; i < types.size(); ++i)
		e->addChildElement (types.getUnchecked(i)->createXml());

	return e;
}

void KnownPluginList::recreateFromXml (const XmlElement& xml)
{
	clear();

	if (xml.hasTagName ("KNOWNPLUGINS"))
	{
		forEachXmlChildElement (xml, e)
		{
			PluginDescription info;

			if (info.loadFromXml (*e))
				addType (info);
		}
	}
}

// This is used to turn a bunch of paths into a nested menu structure.
class PluginFilesystemTree
{
public:
	void buildTree (const Array <PluginDescription*>& allPlugins)
	{
		for (int i = 0; i < allPlugins.size(); ++i)
		{
			String path (allPlugins.getUnchecked(i)
							->fileOrIdentifier.replaceCharacter ('\\', '/')
											  .upToLastOccurrenceOf ("/", false, false));

			if (path.substring (1, 2) == ":")
				path = path.substring (2);

			addPlugin (allPlugins.getUnchecked(i), path);
		}

		optimise();
	}

	void addToMenu (PopupMenu& m, const OwnedArray <PluginDescription>& allPlugins) const
	{
		int i;
		for (i = 0; i < subFolders.size(); ++i)
		{
			const PluginFilesystemTree* const sub = subFolders.getUnchecked(i);

			PopupMenu subMenu;
			sub->addToMenu (subMenu, allPlugins);

		   #if JUCE_MAC
			// avoid the special AU formatting nonsense on Mac..
			m.addSubMenu (sub->folder.fromFirstOccurrenceOf (":", false, false), subMenu);
		   #else
			m.addSubMenu (sub->folder, subMenu);
		   #endif
		}

		for (i = 0; i < plugins.size(); ++i)
		{
			PluginDescription* const plugin = plugins.getUnchecked(i);

			m.addItem (allPlugins.indexOf (plugin) + menuIdBase,
					   plugin->name, true, false);
		}
	}

private:
	String folder;
	OwnedArray <PluginFilesystemTree> subFolders;
	Array <PluginDescription*> plugins;

	void addPlugin (PluginDescription* const pd, const String& path)
	{
		if (path.isEmpty())
		{
			plugins.add (pd);
		}
		else
		{
			const String firstSubFolder (path.upToFirstOccurrenceOf ("/", false, false));
			const String remainingPath (path.fromFirstOccurrenceOf ("/", false, false));

			for (int i = subFolders.size(); --i >= 0;)
			{
				if (subFolders.getUnchecked(i)->folder.equalsIgnoreCase (firstSubFolder))
				{
					subFolders.getUnchecked(i)->addPlugin (pd, remainingPath);
					return;
				}
			}

			PluginFilesystemTree* const newFolder = new PluginFilesystemTree();
			newFolder->folder = firstSubFolder;
			subFolders.add (newFolder);

			newFolder->addPlugin (pd, remainingPath);
		}
	}

	// removes any deeply nested folders that don't contain any actual plugins
	void optimise()
	{
		for (int i = subFolders.size(); --i >= 0;)
		{
			PluginFilesystemTree* const sub = subFolders.getUnchecked(i);

			sub->optimise();

			if (sub->plugins.size() == 0)
			{
				for (int j = 0; j < sub->subFolders.size(); ++j)
					subFolders.add (sub->subFolders.getUnchecked(j));

				sub->subFolders.clear (false);
				subFolders.remove (i);
			}
		}
	}
};

void KnownPluginList::addToMenu (PopupMenu& menu, const SortMethod sortMethod) const
{
	Array <PluginDescription*> sorted;

	{
		PluginSorter sorter (sortMethod);

		for (int i = 0; i < types.size(); ++i)
			sorted.addSorted (sorter, types.getUnchecked(i));
	}

	if (sortMethod == sortByCategory
		 || sortMethod == sortByManufacturer)
	{
		String lastSubMenuName;
		PopupMenu sub;

		for (int i = 0; i < sorted.size(); ++i)
		{
			const PluginDescription* const pd = sorted.getUnchecked(i);
			String thisSubMenuName (sortMethod == sortByCategory ? pd->category
																 : pd->manufacturerName);

			if (! thisSubMenuName.containsNonWhitespaceChars())
				thisSubMenuName = "Other";

			if (thisSubMenuName != lastSubMenuName)
			{
				if (sub.getNumItems() > 0)
				{
					menu.addSubMenu (lastSubMenuName, sub);
					sub.clear();
				}

				lastSubMenuName = thisSubMenuName;
			}

			sub.addItem (types.indexOf (pd) + menuIdBase, pd->name, true, false);
		}

		if (sub.getNumItems() > 0)
			menu.addSubMenu (lastSubMenuName, sub);
	}
	else if (sortMethod == sortByFileSystemLocation)
	{
		PluginFilesystemTree root;
		root.buildTree (sorted);
		root.addToMenu (menu, types);
	}
	else
	{
		for (int i = 0; i < sorted.size(); ++i)
		{
			const PluginDescription* const pd = sorted.getUnchecked(i);
			menu.addItem (types.indexOf (pd) + menuIdBase, pd->name, true, false);
		}
	}
}

int KnownPluginList::getIndexChosenByMenu (const int menuResultCode) const
{
	const int i = menuResultCode - menuIdBase;

	return isPositiveAndBelow (i, types.size()) ? i : -1;
}

/*** End of inlined file: juce_KnownPluginList.cpp ***/


/*** Start of inlined file: juce_PluginDirectoryScanner.cpp ***/
PluginDirectoryScanner::PluginDirectoryScanner (KnownPluginList& listToAddTo,
												AudioPluginFormat& formatToLookFor,
												FileSearchPath directoriesToSearch,
												const bool recursive,
												const File& deadMansPedalFile_)
	: list (listToAddTo),
	  format (formatToLookFor),
	  deadMansPedalFile (deadMansPedalFile_),
	  nextIndex (0),
	  progress (0)
{
	directoriesToSearch.removeRedundantPaths();

	filesOrIdentifiersToScan = format.searchPathsForPlugins (directoriesToSearch, recursive);

	// If any plugins have crashed recently when being loaded, move them to the
	// end of the list to give the others a chance to load correctly..
	const StringArray crashedPlugins (getDeadMansPedalFile());

	for (int i = 0; i < crashedPlugins.size(); ++i)
	{
		const String f = crashedPlugins[i];

		for (int j = filesOrIdentifiersToScan.size(); --j >= 0;)
			if (f == filesOrIdentifiersToScan[j])
				filesOrIdentifiersToScan.move (j, -1);
	}
}

PluginDirectoryScanner::~PluginDirectoryScanner()
{
}

const String PluginDirectoryScanner::getNextPluginFileThatWillBeScanned() const
{
	return format.getNameOfPluginFromIdentifier (filesOrIdentifiersToScan [nextIndex]);
}

bool PluginDirectoryScanner::scanNextFile (const bool dontRescanIfAlreadyInList)
{
	String file (filesOrIdentifiersToScan [nextIndex]);

	if (file.isNotEmpty() && ! list.isListingUpToDate (file))
	{
		OwnedArray <PluginDescription> typesFound;

		// Add this plugin to the end of the dead-man's pedal list in case it crashes...
		StringArray crashedPlugins (getDeadMansPedalFile());
		crashedPlugins.removeString (file);
		crashedPlugins.add (file);
		setDeadMansPedalFile (crashedPlugins);

		list.scanAndAddFile (file,
							 dontRescanIfAlreadyInList,
							 typesFound,
							 format);

		// Managed to load without crashing, so remove it from the dead-man's-pedal..
		crashedPlugins.removeString (file);
		setDeadMansPedalFile (crashedPlugins);

		if (typesFound.size() == 0)
			failedFiles.add (file);
	}

	return skipNextFile();
}

bool PluginDirectoryScanner::skipNextFile()
{
	if (nextIndex >= filesOrIdentifiersToScan.size())
		return false;

	progress = ++nextIndex / (float) filesOrIdentifiersToScan.size();
	return nextIndex < filesOrIdentifiersToScan.size();
}

StringArray PluginDirectoryScanner::getDeadMansPedalFile()
{
	StringArray lines;

	if (deadMansPedalFile != File::nonexistent)
	{
		deadMansPedalFile.readLines (lines);
		lines.removeEmptyStrings();
	}

	return lines;
}

void PluginDirectoryScanner::setDeadMansPedalFile (const StringArray& newContents)
{
	if (deadMansPedalFile != File::nonexistent)
		deadMansPedalFile.replaceWithText (newContents.joinIntoString ("\n"), true, true);
}

/*** End of inlined file: juce_PluginDirectoryScanner.cpp ***/


/*** Start of inlined file: juce_PluginListComponent.cpp ***/
PluginListComponent::PluginListComponent (KnownPluginList& listToEdit,
										  const File& deadMansPedalFile_,
										  PropertiesFile* const propertiesToUse_)
	: list (listToEdit),
	  deadMansPedalFile (deadMansPedalFile_),
	  optionsButton ("Options..."),
	  propertiesToUse (propertiesToUse_)
{
	listBox.setModel (this);
	addAndMakeVisible (&listBox);

	addAndMakeVisible (&optionsButton);
	optionsButton.addListener (this);
	optionsButton.setTriggeredOnMouseDown (true);

	setSize (400, 600);
	list.addChangeListener (this);
	updateList();
}

PluginListComponent::~PluginListComponent()
{
	list.removeChangeListener (this);
}

void PluginListComponent::resized()
{
	listBox.setBounds (0, 0, getWidth(), getHeight() - 30);
	optionsButton.changeWidthToFitText (24);
	optionsButton.setTopLeftPosition (8, getHeight() - 28);
}

void PluginListComponent::changeListenerCallback (ChangeBroadcaster*)
{
	updateList();
}

void PluginListComponent::updateList()
{
	listBox.updateContent();
	listBox.repaint();
}

int PluginListComponent::getNumRows()
{
	return list.getNumTypes();
}

void PluginListComponent::paintListBoxItem (int row, Graphics& g, int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		g.fillAll (findColour (TextEditor::highlightColourId));

	const PluginDescription* const pd = list.getType (row);

	if (pd != nullptr)
	{
		GlyphArrangement ga;
		ga.addCurtailedLineOfText (Font (height * 0.7f, Font::bold), pd->name, 8.0f, height * 0.8f, width - 10.0f, true);

		g.setColour (Colours::black);
		ga.draw (g);

		const Rectangle<float> bb (ga.getBoundingBox (0, -1, false));

		String desc;
		desc << pd->pluginFormatName
			 << (pd->isInstrument ? " instrument" : " effect")
			 << " - "
			 << pd->numInputChannels << (pd->numInputChannels == 1 ? " in" : " ins")
			 << " / "
			 << pd->numOutputChannels << (pd->numOutputChannels == 1 ? " out" : " outs");

		if (pd->manufacturerName.isNotEmpty())
			desc << " - " << pd->manufacturerName;

		if (pd->version.isNotEmpty())
			desc << " - " << pd->version;

		 if (pd->category.isNotEmpty())
			desc << " - category: '" << pd->category << '\'';

		g.setColour (Colours::grey);

		ga.clear();
		ga.addCurtailedLineOfText (Font (height * 0.6f), desc, bb.getRight() + 10.0f, height * 0.8f, width - bb.getRight() - 12.0f, true);
		ga.draw (g);
	}
}

void PluginListComponent::deleteKeyPressed (int lastRowSelected)
{
	list.removeType (lastRowSelected);
}

void PluginListComponent::optionsMenuCallback (int result)
{
	switch (result)
	{
		case 1:     list.clear(); break;

		case 2:     list.sort (KnownPluginList::sortAlphabetically); break;
		case 3:     list.sort (KnownPluginList::sortByCategory); break;
		case 4:     list.sort (KnownPluginList::sortByManufacturer); break;

		case 5:
		{
			const SparseSet <int> selected (listBox.getSelectedRows());

			for (int i = list.getNumTypes(); --i >= 0;)
				if (selected.contains (i))
					list.removeType (i);

			break;
		}

		case 6:
		{
			const PluginDescription* const desc = list.getType (listBox.getSelectedRow());

			if (desc != nullptr && File (desc->fileOrIdentifier).existsAsFile())
				File (desc->fileOrIdentifier).getParentDirectory().startAsProcess();

			break;
		}

		case 7:
			for (int i = list.getNumTypes(); --i >= 0;)
				if (! AudioPluginFormatManager::getInstance()->doesPluginStillExist (*list.getType (i)))
					list.removeType (i);

			break;

		default:
			if (result != 0)
			{
				typeToScan = result - 10;
				startTimer (1);
			}

			break;
	}
}

void PluginListComponent::optionsMenuStaticCallback (int result, PluginListComponent* pluginList)
{
	if (pluginList != nullptr)
		pluginList->optionsMenuCallback (result);
}

void PluginListComponent::buttonClicked (Button* button)
{
	if (button == &optionsButton)
	{
		PopupMenu menu;
		menu.addItem (1, TRANS("Clear list"));
		menu.addItem (5, TRANS("Remove selected plugin from list"), listBox.getNumSelectedRows() > 0);
		menu.addItem (6, TRANS("Show folder containing selected plugin"), listBox.getNumSelectedRows() > 0);
		menu.addItem (7, TRANS("Remove any plugins whose files no longer exist"));
		menu.addSeparator();
		menu.addItem (2, TRANS("Sort alphabetically"));
		menu.addItem (3, TRANS("Sort by category"));
		menu.addItem (4, TRANS("Sort by manufacturer"));
		menu.addSeparator();

		for (int i = 0; i < AudioPluginFormatManager::getInstance()->getNumFormats(); ++i)
		{
			AudioPluginFormat* const format = AudioPluginFormatManager::getInstance()->getFormat (i);

			if (format->getDefaultLocationsToSearch().getNumPaths() > 0)
				menu.addItem (10 + i, "Scan for new or updated " + format->getName() + " plugins...");
		}

		menu.showMenuAsync (PopupMenu::Options().withTargetComponent (&optionsButton),
							ModalCallbackFunction::forComponent (optionsMenuStaticCallback, this));
	}
}

void PluginListComponent::timerCallback()
{
	stopTimer();
	scanFor (AudioPluginFormatManager::getInstance()->getFormat (typeToScan));
}

bool PluginListComponent::isInterestedInFileDrag (const StringArray& /*files*/)
{
	return true;
}

void PluginListComponent::filesDropped (const StringArray& files, int, int)
{
	OwnedArray <PluginDescription> typesFound;
	list.scanAndAddDragAndDroppedFiles (files, typesFound);
}

void PluginListComponent::scanFor (AudioPluginFormat* format)
{
#if JUCE_MODAL_LOOPS_PERMITTED
	if (format == nullptr)
		return;

	FileSearchPath path (format->getDefaultLocationsToSearch());

	if (propertiesToUse != nullptr)
		path = propertiesToUse->getValue ("lastPluginScanPath_" + format->getName(), path.toString());

	{
		AlertWindow aw (TRANS("Select folders to scan..."), String::empty, AlertWindow::NoIcon);
		FileSearchPathListComponent pathList;
		pathList.setSize (500, 300);
		pathList.setPath (path);

		aw.addCustomComponent (&pathList);
		aw.addButton (TRANS("Scan"), 1, KeyPress::returnKey);
		aw.addButton (TRANS("Cancel"), 0, KeyPress::escapeKey);

		if (aw.runModalLoop() == 0)
			return;

		path = pathList.getPath();
	}

	if (propertiesToUse != nullptr)
	{
		propertiesToUse->setValue ("lastPluginScanPath_" + format->getName(), path.toString());
		propertiesToUse->saveIfNeeded();
	}

	double progress = 0.0;

	AlertWindow aw (TRANS("Scanning for plugins..."),
					TRANS("Searching for all possible plugin files..."), AlertWindow::NoIcon);

	aw.addButton (TRANS("Cancel"), 0, KeyPress::escapeKey);
	aw.addProgressBarComponent (progress);
	aw.enterModalState();

	MessageManager::getInstance()->runDispatchLoopUntil (300);

	PluginDirectoryScanner scanner (list, *format, path, true, deadMansPedalFile);

	for (;;)
	{
		aw.setMessage (TRANS("Testing:\n\n") + scanner.getNextPluginFileThatWillBeScanned());

		MessageManager::getInstance()->runDispatchLoopUntil (100);

		if (! scanner.scanNextFile (true))
			break;

		if (! aw.isCurrentlyModal())
			break;

		progress = scanner.getProgress();
	}

	if (scanner.getFailedFiles().size() > 0)
	{
		StringArray shortNames;

		for (int i = 0; i < scanner.getFailedFiles().size(); ++i)
			shortNames.add (File (scanner.getFailedFiles()[i]).getFileName());

		AlertWindow::showMessageBox (AlertWindow::InfoIcon,
									 TRANS("Scan complete"),
									 TRANS("Note that the following files appeared to be plugin files, but failed to load correctly:\n\n")
										+ shortNames.joinIntoString (", "));
	}
#else
	jassertfalse; // this method needs refactoring to work without modal loops..
#endif
}

/*** End of inlined file: juce_PluginListComponent.cpp ***/

// END_AUTOINCLUDE

}

/*** End of inlined file: juce_audio_processors.cpp ***/


#endif

