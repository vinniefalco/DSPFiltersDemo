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

/*** Start of inlined file: juce_video.cpp ***/
#if defined (__JUCE_VIDEO_JUCEHEADER__) && ! JUCE_AMALGAMATED_INCLUDE
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

#include "juce_gui_extra_amalgam.h"
#include "juce_video_amalgam.h"

#if JUCE_MAC
 #if JUCE_QUICKTIME
  #define Point CarbonDummyPointName
  #define Component CarbonDummyCompName
  #import <QTKit/QTKit.h>
  #undef Point
  #undef Component
 #endif

#elif JUCE_WINDOWS
 #if JUCE_QUICKTIME
  /* If you've got an include error here, you probably need to install the QuickTime SDK and
	 add its header directory to your include path.

	 Alternatively, if you don't need any QuickTime services, just set the JUCE_QUICKTIME flag to 0.
  */
  #include <Movies.h>
  #include <QTML.h>
  #include <QuickTimeComponents.h>
  #include <MediaHandlers.h>
  #include <ImageCodec.h>

  /* If you've got QuickTime 7 installed, then these COM objects should be found in
	 the "\Program Files\Quicktime" directory. You'll need to add this directory to
	 your include search path to make these import statements work.
  */
  #import <QTOLibrary.dll>
  #import <QTOControl.dll>

  #if JUCE_MSVC && ! JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
   #pragma comment (lib, "QTMLClient.lib")
  #endif
 #endif

 #if JUCE_USE_CAMERA || JUCE_DIRECTSHOW
  /* If you're using the camera classes, you'll need access to a few DirectShow headers.
	 These files are provided in the normal Windows SDK. */
  #include <dshow.h>
  #include <dshowasf.h>
 #endif

 #if JUCE_DIRECTSHOW && JUCE_MEDIAFOUNDATION
  #include <evr.h>
 #endif

 #if JUCE_USE_CAMERA && JUCE_MSVC && ! JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment (lib, "Strmiids.lib")
  #pragma comment (lib, "wmvcore.lib")
 #endif

 #if JUCE_MEDIAFOUNDATION && JUCE_MSVC && ! JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment (lib, "mfuuid.lib")
 #endif

 #if JUCE_DIRECTSHOW && JUCE_MSVC && ! JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment (lib, "strmiids.lib")
 #endif
#endif

using namespace juce;

namespace juce
{

#if JUCE_MAC || JUCE_IOS

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

 #if JUCE_USE_CAMERA

/*** Start of inlined file: juce_mac_CameraDevice.mm ***/
#if ! JUCE_QUICKTIME
 #error "On the Mac, cameras use Quicktime, so if you turn on JUCE_USE_CAMERA, you also need to enable JUCE_QUICKTIME"
#endif

#define QTCaptureCallbackDelegate MakeObjCClassName(QTCaptureCallbackDelegate)

class QTCameraDeviceInteral;

} // (juce namespace)

@interface QTCaptureCallbackDelegate    : NSObject
{
@public
	CameraDevice* owner;
	QTCameraDeviceInteral* internal;
	int64 firstPresentationTime;
	int64 averageTimeOffset;
}

- (QTCaptureCallbackDelegate*) initWithOwner: (CameraDevice*) owner internalDev: (QTCameraDeviceInteral*) d;
- (void) dealloc;

- (void) captureOutput: (QTCaptureOutput*) captureOutput
		 didOutputVideoFrame: (CVImageBufferRef) videoFrame
		 withSampleBuffer: (QTSampleBuffer*) sampleBuffer
		 fromConnection: (QTCaptureConnection*) connection;

- (void) captureOutput: (QTCaptureFileOutput*) captureOutput
		 didOutputSampleBuffer: (QTSampleBuffer*) sampleBuffer
		 fromConnection: (QTCaptureConnection*) connection;
@end

namespace juce
{

extern Image juce_createImageFromCIImage (CIImage* im, int w, int h);

class QTCameraDeviceInteral
{
public:
	QTCameraDeviceInteral (CameraDevice* owner, const int index)
		: input (nil),
		  audioDevice (nil),
		  audioInput (nil),
		  session (nil),
		  fileOutput (nil),
		  imageOutput (nil)
	{
		JUCE_AUTORELEASEPOOL

		session = [[QTCaptureSession alloc] init];

		NSArray* devs = [QTCaptureDevice inputDevicesWithMediaType: QTMediaTypeVideo];
		device = (QTCaptureDevice*) [devs objectAtIndex: index];
		callbackDelegate = [[QTCaptureCallbackDelegate alloc] initWithOwner: owner
																internalDev: this];

		NSError* err = nil;
		[device retain];
		[device open: &err];

		if (err == nil)
		{
			input      = [[QTCaptureDeviceInput alloc] initWithDevice: device];
			audioInput = [[QTCaptureDeviceInput alloc] initWithDevice: device];

			[session addInput: input error: &err];

			if (err == nil)
			{
				resetFile();

				imageOutput = [[QTCaptureDecompressedVideoOutput alloc] init];
				[imageOutput setDelegate: callbackDelegate];

				if (err == nil)
				{
					[session startRunning];
					return;
				}
			}
		}

		openingError = nsStringToJuce ([err description]);
		DBG (openingError);
	}

	~QTCameraDeviceInteral()
	{
		[session stopRunning];
		[session removeOutput: imageOutput];

		[session release];
		[input release];
		[device release];
		[audioDevice release];
		[audioInput release];
		[fileOutput release];
		[imageOutput release];
		[callbackDelegate release];
	}

	void resetFile()
	{
		[fileOutput recordToOutputFileURL: nil];
		[session removeOutput: fileOutput];
		[fileOutput release];
		fileOutput = [[QTCaptureMovieFileOutput alloc] init];

		[session removeInput: audioInput];
		[audioInput release];
		audioInput = nil;
		[audioDevice release];
		audioDevice = nil;

		[fileOutput setDelegate: callbackDelegate];
	}

	void addDefaultAudioInput()
	{
		NSError* err = nil;
		audioDevice = [QTCaptureDevice defaultInputDeviceWithMediaType: QTMediaTypeSound];

		if ([audioDevice open: &err])
			[audioDevice retain];
		else
			audioDevice = nil;

		if (audioDevice != nil)
		{
			audioInput = [[QTCaptureDeviceInput alloc] initWithDevice: audioDevice];
			[session addInput: audioInput error: &err];
		}
	}

	void addListener (CameraDevice::Listener* listenerToAdd)
	{
		const ScopedLock sl (listenerLock);

		if (listeners.size() == 0)
			[session addOutput: imageOutput error: nil];

		listeners.addIfNotAlreadyThere (listenerToAdd);
	}

	void removeListener (CameraDevice::Listener* listenerToRemove)
	{
		const ScopedLock sl (listenerLock);
		listeners.removeValue (listenerToRemove);

		if (listeners.size() == 0)
			[session removeOutput: imageOutput];
	}

	void callListeners (CIImage* frame, int w, int h)
	{
		Image image (juce_createImageFromCIImage (frame, w, h));

		const ScopedLock sl (listenerLock);

		for (int i = listeners.size(); --i >= 0;)
		{
			CameraDevice::Listener* const l = listeners[i];

			if (l != nullptr)
				l->imageReceived (image);
		}
	}

	QTCaptureDevice* device;
	QTCaptureDeviceInput* input;
	QTCaptureDevice* audioDevice;
	QTCaptureDeviceInput* audioInput;
	QTCaptureSession* session;
	QTCaptureMovieFileOutput* fileOutput;
	QTCaptureDecompressedVideoOutput* imageOutput;
	QTCaptureCallbackDelegate* callbackDelegate;
	String openingError;

	Array<CameraDevice::Listener*> listeners;
	CriticalSection listenerLock;
};

} // (juce namespace)

@implementation QTCaptureCallbackDelegate

- (QTCaptureCallbackDelegate*) initWithOwner: (CameraDevice*) owner_
								 internalDev: (QTCameraDeviceInteral*) d
{
	[super init];
	owner = owner_;
	internal = d;
	firstPresentationTime = 0;
	averageTimeOffset = 0;
	return self;
}

- (void) dealloc
{
	[super dealloc];
}

- (void) captureOutput: (QTCaptureOutput*) captureOutput
		 didOutputVideoFrame: (CVImageBufferRef) videoFrame
		 withSampleBuffer: (QTSampleBuffer*) sampleBuffer
		 fromConnection: (QTCaptureConnection*) connection
{
	if (internal->listeners.size() > 0)
	{
		JUCE_AUTORELEASEPOOL

		internal->callListeners ([CIImage imageWithCVImageBuffer: videoFrame],
								 CVPixelBufferGetWidth (videoFrame),
								 CVPixelBufferGetHeight (videoFrame));
	}
}

- (void) captureOutput: (QTCaptureFileOutput*) captureOutput
		 didOutputSampleBuffer: (QTSampleBuffer*) sampleBuffer
		 fromConnection: (QTCaptureConnection*) connection
{
	const Time now (Time::getCurrentTime());

   #if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
	NSNumber* hosttime = (NSNumber*) [sampleBuffer attributeForKey: QTSampleBufferHostTimeAttribute];
   #else
	NSNumber* hosttime = (NSNumber*) [sampleBuffer attributeForKey: nsStringLiteral ("hostTime")];
   #endif

	int64 presentationTime = (hosttime != nil)
			? ((int64) AudioConvertHostTimeToNanos ([hosttime unsignedLongLongValue]) / 1000000 + 40)
			: (([sampleBuffer presentationTime].timeValue * 1000) / [sampleBuffer presentationTime].timeScale + 50);

	const int64 timeDiff = now.toMilliseconds() - presentationTime;

	if (firstPresentationTime == 0)
	{
		firstPresentationTime = presentationTime;
		averageTimeOffset = timeDiff;
	}
	else
	{
		averageTimeOffset = (averageTimeOffset * 120 + timeDiff * 8) / 128;
	}
}

@end

namespace juce
{

class QTCaptureViewerComp : public NSViewComponent
{
public:
	QTCaptureViewerComp (CameraDevice* const cameraDevice, QTCameraDeviceInteral* const internal)
	{
		JUCE_AUTORELEASEPOOL
		captureView = [[QTCaptureView alloc] init];
		[captureView setCaptureSession: internal->session];

		setSize (640, 480); //  xxx need to somehow get the movie size - how?
		setView (captureView);
	}

	~QTCaptureViewerComp()
	{
		setView (0);
		[captureView setCaptureSession: nil];
		[captureView release];
	}

	QTCaptureView* captureView;
};

CameraDevice::CameraDevice (const String& name_, int index)
	: name (name_)
{
	isRecording = false;
	internal = new QTCameraDeviceInteral (this, index);
}

CameraDevice::~CameraDevice()
{
	stopRecording();
	delete static_cast <QTCameraDeviceInteral*> (internal);
	internal = nullptr;
}

Component* CameraDevice::createViewerComponent()
{
	return new QTCaptureViewerComp (this, static_cast <QTCameraDeviceInteral*> (internal));
}

String CameraDevice::getFileExtension()
{
	return ".mov";
}

void CameraDevice::startRecordingToFile (const File& file, int quality)
{
	stopRecording();

	QTCameraDeviceInteral* const d = static_cast <QTCameraDeviceInteral*> (internal);
	d->callbackDelegate->firstPresentationTime = 0;
	file.deleteFile();

	// In some versions of QT (e.g. on 10.5), if you record video without audio, the speed comes
	// out wrong, so we'll put some audio in there too..,
	d->addDefaultAudioInput();

	[d->session addOutput: d->fileOutput error: nil];

	NSEnumerator* connectionEnumerator = [[d->fileOutput connections] objectEnumerator];

	for (;;)
	{
		QTCaptureConnection* connection = [connectionEnumerator nextObject];
		if (connection == nil)
			break;

		QTCompressionOptions* options = nil;
		NSString* mediaType = [connection mediaType];

		if ([mediaType isEqualToString: QTMediaTypeVideo])
			options = [QTCompressionOptions compressionOptionsWithIdentifier:
							quality >= 1 ? nsStringLiteral ("QTCompressionOptionsSD480SizeH264Video")
										 : nsStringLiteral ("QTCompressionOptions240SizeH264Video")];
		else if ([mediaType isEqualToString: QTMediaTypeSound])
			options = [QTCompressionOptions compressionOptionsWithIdentifier: nsStringLiteral ("QTCompressionOptionsHighQualityAACAudio")];

		[d->fileOutput setCompressionOptions: options forConnection: connection];
	}

	[d->fileOutput recordToOutputFileURL: [NSURL fileURLWithPath: juceStringToNS (file.getFullPathName())]];
	isRecording = true;
}

Time CameraDevice::getTimeOfFirstRecordedFrame() const
{
	QTCameraDeviceInteral* const d = static_cast <QTCameraDeviceInteral*> (internal);
	if (d->callbackDelegate->firstPresentationTime != 0)
		return Time (d->callbackDelegate->firstPresentationTime + d->callbackDelegate->averageTimeOffset);

	return Time();
}

void CameraDevice::stopRecording()
{
	if (isRecording)
	{
		static_cast <QTCameraDeviceInteral*> (internal)->resetFile();
		isRecording = false;
	}
}

void CameraDevice::addListener (Listener* listenerToAdd)
{
	if (listenerToAdd != nullptr)
		static_cast <QTCameraDeviceInteral*> (internal)->addListener (listenerToAdd);
}

void CameraDevice::removeListener (Listener* listenerToRemove)
{
	if (listenerToRemove != nullptr)
		static_cast <QTCameraDeviceInteral*> (internal)->removeListener (listenerToRemove);
}

StringArray CameraDevice::getAvailableDevices()
{
	JUCE_AUTORELEASEPOOL

	StringArray results;
	NSArray* devs = [QTCaptureDevice inputDevicesWithMediaType: QTMediaTypeVideo];

	for (int i = 0; i < (int) [devs count]; ++i)
	{
		QTCaptureDevice* dev = (QTCaptureDevice*) [devs objectAtIndex: i];
		results.add (nsStringToJuce ([dev localizedDisplayName]));
	}

	return results;
}

CameraDevice* CameraDevice::openDevice (int index,
										int minWidth, int minHeight,
										int maxWidth, int maxHeight)
{
	ScopedPointer <CameraDevice> d (new CameraDevice (getAvailableDevices() [index], index));

	if (static_cast <QTCameraDeviceInteral*> (d->internal)->openingError.isEmpty())
		return d.release();

	return nullptr;
}

/*** End of inlined file: juce_mac_CameraDevice.mm ***/


 #endif

 #if JUCE_QUICKTIME

/*** Start of inlined file: juce_mac_QuickTimeMovieComponent.mm ***/
#if JUCE_QUICKTIME

} // (juce namespace)

#define NonInterceptingQTMovieView MakeObjCClassName(NonInterceptingQTMovieView)

@interface NonInterceptingQTMovieView   : QTMovieView
{
}

- (id) initWithFrame: (NSRect) frame;
- (BOOL) acceptsFirstMouse: (NSEvent*) theEvent;
- (NSView*) hitTest: (NSPoint) p;

@end

@implementation NonInterceptingQTMovieView

- (id) initWithFrame: (NSRect) frame
{
	self = [super initWithFrame: frame];
	[self setNextResponder: [self superview]];
	return self;
}

- (void) dealloc
{
	[super dealloc];
}

- (NSView*) hitTest: (NSPoint) point
{
	return [self isControllerVisible] ? [super hitTest: point] : nil;
}

- (BOOL) acceptsFirstMouse: (NSEvent*) theEvent
{
	return YES;
}

@end

namespace juce
{

#define theMovie (static_cast <QTMovie*> (movie))

QuickTimeMovieComponent::QuickTimeMovieComponent()
	: movie (0)
{
	setOpaque (true);
	setVisible (true);

	QTMovieView* view = [[NonInterceptingQTMovieView alloc] initWithFrame: NSMakeRect (0, 0, 100.0f, 100.0f)];
	setView (view);
	[view setWantsLayer: YES]; // prevents the view failing to redraw correctly when paused.
	[view release];
}

QuickTimeMovieComponent::~QuickTimeMovieComponent()
{
	closeMovie();
	setView (nil);
}

bool QuickTimeMovieComponent::isQuickTimeAvailable() noexcept
{
	return true;
}

static QTMovie* openMovieFromStream (InputStream* movieStream, File& movieFile)
{
	// unfortunately, QTMovie objects can only be created on the main thread..
	jassert (MessageManager::getInstance()->isThisTheMessageThread());

	QTMovie* movie = nil;

	FileInputStream* const fin = dynamic_cast <FileInputStream*> (movieStream);

	if (fin != nullptr)
	{
		movieFile = fin->getFile();
		movie = [QTMovie movieWithFile: juceStringToNS (movieFile.getFullPathName())
								 error: nil];
	}
	else
	{
		MemoryBlock temp;
		movieStream->readIntoMemoryBlock (temp);

		const char* const suffixesToTry[] = { ".mov", ".mp3", ".avi", ".m4a" };

		for (int i = 0; i < numElementsInArray (suffixesToTry); ++i)
		{
			movie = [QTMovie movieWithDataReference: [QTDataReference dataReferenceWithReferenceToData: [NSData dataWithBytes: temp.getData()
																													   length: temp.getSize()]
																								  name: [NSString stringWithUTF8String: suffixesToTry[i]]
																							  MIMEType: nsEmptyString()]
											  error: nil];

			if (movie != 0)
				break;
		}
	}

	return movie;
}

bool QuickTimeMovieComponent::loadMovie (const File& movieFile_,
										 const bool isControllerVisible_)
{
	return loadMovie ((InputStream*) movieFile_.createInputStream(), isControllerVisible_);
}

bool QuickTimeMovieComponent::loadMovie (InputStream* movieStream,
										 const bool controllerVisible_)
{
	closeMovie();

	if (getPeer() == nullptr)
	{
		// To open a movie, this component must be visible inside a functioning window, so that
		// the QT control can be assigned to the window.
		jassertfalse;
		return false;
	}

	movie = openMovieFromStream (movieStream, movieFile);

	[theMovie retain];
	QTMovieView* view = (QTMovieView*) getView();
	[view setMovie: theMovie];
	[view setControllerVisible: controllerVisible_];
	setLooping (looping);

	return movie != nil;
}

bool QuickTimeMovieComponent::loadMovie (const URL& movieURL,
										 const bool isControllerVisible_)
{
	// unfortunately, QTMovie objects can only be created on the main thread..
	jassert (MessageManager::getInstance()->isThisTheMessageThread());

	closeMovie();

	if (getPeer() == nullptr)
	{
		// To open a movie, this component must be visible inside a functioning window, so that
		// the QT control can be assigned to the window.
		jassertfalse;
		return false;
	}

	NSURL* url = [NSURL URLWithString: juceStringToNS (movieURL.toString (true))];
	NSError* err;
	if ([QTMovie canInitWithURL: url])
		movie = [QTMovie movieWithURL: url error: &err];

	[theMovie retain];
	QTMovieView* view = (QTMovieView*) getView();
	[view setMovie: theMovie];
	[view setControllerVisible: controllerVisible];
	setLooping (looping);

	return movie != nil;
}

void QuickTimeMovieComponent::closeMovie()
{
	stop();
	QTMovieView* view = (QTMovieView*) getView();
	[view setMovie: nil];
	[theMovie release];
	movie = 0;
	movieFile = File::nonexistent;
}

bool QuickTimeMovieComponent::isMovieOpen() const
{
	return movie != nil;
}

File QuickTimeMovieComponent::getCurrentMovieFile() const
{
	return movieFile;
}

void QuickTimeMovieComponent::play()
{
	[theMovie play];
}

void QuickTimeMovieComponent::stop()
{
	[theMovie stop];
}

bool QuickTimeMovieComponent::isPlaying() const
{
	return movie != 0  && [theMovie rate] != 0;
}

void QuickTimeMovieComponent::setPosition (const double seconds)
{
	if (movie != 0)
	{
		QTTime t;
		t.timeValue = (uint64) (100000.0 * seconds);
		t.timeScale = 100000;
		t.flags = 0;

		[theMovie setCurrentTime: t];
	}
}

double QuickTimeMovieComponent::getPosition() const
{
	if (movie == 0)
		return 0.0;

	QTTime t = [theMovie currentTime];
	return t.timeValue / (double) t.timeScale;
}

void QuickTimeMovieComponent::setSpeed (const float newSpeed)
{
	[theMovie setRate: newSpeed];
}

double QuickTimeMovieComponent::getMovieDuration() const
{
	if (movie == 0)
		return 0.0;

	QTTime t = [theMovie duration];
	return t.timeValue / (double) t.timeScale;
}

void QuickTimeMovieComponent::setLooping (const bool shouldLoop)
{
	looping = shouldLoop;

	[theMovie setAttribute: [NSNumber numberWithBool: shouldLoop]
					forKey: QTMovieLoopsAttribute];
}

bool QuickTimeMovieComponent::isLooping() const
{
	return looping;
}

void QuickTimeMovieComponent::setMovieVolume (const float newVolume)
{
	[theMovie setVolume: newVolume];
}

float QuickTimeMovieComponent::getMovieVolume() const
{
	return movie != 0 ? [theMovie volume] : 0.0f;
}

void QuickTimeMovieComponent::getMovieNormalSize (int& width, int& height) const
{
	width = 0;
	height = 0;

	if (movie != 0)
	{
		NSSize s = [[theMovie attributeForKey: QTMovieNaturalSizeAttribute] sizeValue];
		width = (int) s.width;
		height = (int) s.height;
	}
}

void QuickTimeMovieComponent::paint (Graphics& g)
{
	if (movie == 0)
		g.fillAll (Colours::black);
}

bool QuickTimeMovieComponent::isControllerVisible() const
{
	return controllerVisible;
}

void QuickTimeMovieComponent::goToStart()
{
	setPosition (0.0);
}

void QuickTimeMovieComponent::setBoundsWithCorrectAspectRatio (const Rectangle<int>& spaceToFitWithin,
															   const RectanglePlacement& placement)
{
	int normalWidth, normalHeight;
	getMovieNormalSize (normalWidth, normalHeight);

	const Rectangle<int> normalSize (0, 0, normalWidth, normalHeight);

	if (! (spaceToFitWithin.isEmpty() || normalSize.isEmpty()))
		setBounds (placement.appliedTo (normalSize, spaceToFitWithin));
	else
		setBounds (spaceToFitWithin);
}

#if ! (JUCE_MAC && JUCE_64BIT)

bool juce_OpenQuickTimeMovieFromStream (InputStream* movieStream, Movie& result, Handle& dataHandle)
{
	if (movieStream == nullptr)
		return false;

	File file;
	QTMovie* movie = openMovieFromStream (movieStream, file);

	if (movie != nil)
		result = [movie quickTimeMovie];

	return movie != nil;
}

#endif
#endif

/*** End of inlined file: juce_mac_QuickTimeMovieComponent.mm ***/


 #endif

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


 #if JUCE_USE_CAMERA

/*** Start of inlined file: juce_win32_CameraDevice.cpp ***/
interface ISampleGrabberCB  : public IUnknown
{
	virtual STDMETHODIMP SampleCB (double, IMediaSample*) = 0;
	virtual STDMETHODIMP BufferCB (double, BYTE*, long) = 0;
};

interface ISampleGrabber  : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE SetOneShot (BOOL) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetMediaType (const AM_MEDIA_TYPE*) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType (AM_MEDIA_TYPE*) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetBufferSamples (BOOL) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer (long*, long*) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentSample (IMediaSample**) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetCallback (ISampleGrabberCB*, long) = 0;
};

static const IID IID_ISampleGrabberCB  = { 0x0579154A, 0x2B53, 0x4994, { 0xB0, 0xD0, 0xE7, 0x73, 0x14, 0x8E, 0xFF, 0x85 } };
static const IID IID_ISampleGrabber    = { 0x6B652FFF, 0x11FE, 0x4fce, { 0x92, 0xAD, 0x02, 0x66, 0xB5, 0xD7, 0xC7, 0x8F } };
static const CLSID CLSID_SampleGrabber = { 0xC1F400A0, 0x3F08, 0x11d3, { 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37 } };
static const CLSID CLSID_NullRenderer  = { 0xC1F400A4, 0x3F08, 0x11d3, { 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37 } };

class DShowCameraDeviceInteral  : public ChangeBroadcaster
{
public:
	DShowCameraDeviceInteral (CameraDevice* const owner_,
							  const ComSmartPtr <ICaptureGraphBuilder2>& captureGraphBuilder_,
							  const ComSmartPtr <IBaseFilter>& filter_,
							  int minWidth, int minHeight,
							  int maxWidth, int maxHeight)
	  : owner (owner_),
		captureGraphBuilder (captureGraphBuilder_),
		filter (filter_),
		ok (false),
		imageNeedsFlipping (false),
		width (0),
		height (0),
		activeUsers (0),
		recordNextFrameTime (false),
		previewMaxFPS (60)
	{
		HRESULT hr = graphBuilder.CoCreateInstance (CLSID_FilterGraph);
		if (FAILED (hr))
			return;

		hr = captureGraphBuilder->SetFiltergraph (graphBuilder);
		if (FAILED (hr))
			return;

		hr = graphBuilder.QueryInterface (mediaControl);
		if (FAILED (hr))
			return;

		{
			ComSmartPtr <IAMStreamConfig> streamConfig;

			hr = captureGraphBuilder->FindInterface (&PIN_CATEGORY_CAPTURE, 0, filter,
													 IID_IAMStreamConfig, (void**) streamConfig.resetAndGetPointerAddress());

			if (streamConfig != nullptr)
			{
				getVideoSizes (streamConfig);

				if (! selectVideoSize (streamConfig, minWidth, minHeight, maxWidth, maxHeight))
					return;
			}
		}

		hr = graphBuilder->AddFilter (filter, _T("Video Capture"));
		if (FAILED (hr))
			return;

		hr = smartTee.CoCreateInstance (CLSID_SmartTee);
		if (FAILED (hr))
			return;

		hr = graphBuilder->AddFilter (smartTee, _T("Smart Tee"));
		if (FAILED (hr))
			return;

		if (! connectFilters (filter, smartTee))
			return;

		ComSmartPtr <IBaseFilter> sampleGrabberBase;
		hr = sampleGrabberBase.CoCreateInstance (CLSID_SampleGrabber);
		if (FAILED (hr))
			return;

		hr = sampleGrabberBase.QueryInterface (IID_ISampleGrabber, sampleGrabber);
		if (FAILED (hr))
			return;

		{
			AM_MEDIA_TYPE mt = { 0 };
			mt.majortype = MEDIATYPE_Video;
			mt.subtype = MEDIASUBTYPE_RGB24;
			mt.formattype = FORMAT_VideoInfo;
			sampleGrabber->SetMediaType (&mt);
		}

		callback = new GrabberCallback (*this);
		hr = sampleGrabber->SetCallback (callback, 1);

		hr = graphBuilder->AddFilter (sampleGrabberBase, _T("Sample Grabber"));
		if (FAILED (hr))
			return;

		ComSmartPtr <IPin> grabberInputPin;
		if (! (getPin (smartTee, PINDIR_OUTPUT, smartTeeCaptureOutputPin, "capture")
				&& getPin (smartTee, PINDIR_OUTPUT, smartTeePreviewOutputPin, "preview")
				&& getPin (sampleGrabberBase, PINDIR_INPUT, grabberInputPin)))
			return;

		hr = graphBuilder->Connect (smartTeePreviewOutputPin, grabberInputPin);
		if (FAILED (hr))
			return;

		AM_MEDIA_TYPE mt = { 0 };
		hr = sampleGrabber->GetConnectedMediaType (&mt);
		VIDEOINFOHEADER* pVih = (VIDEOINFOHEADER*) (mt.pbFormat);
		width = pVih->bmiHeader.biWidth;
		height = pVih->bmiHeader.biHeight;

		ComSmartPtr <IBaseFilter> nullFilter;
		hr = nullFilter.CoCreateInstance (CLSID_NullRenderer);
		hr = graphBuilder->AddFilter (nullFilter, _T("Null Renderer"));

		if (connectFilters (sampleGrabberBase, nullFilter)
			  && addGraphToRot())
		{
			activeImage = Image (Image::RGB, width, height, true);
			loadingImage = Image (Image::RGB, width, height, true);

			ok = true;
		}
	}

	~DShowCameraDeviceInteral()
	{
		if (mediaControl != nullptr)
			mediaControl->Stop();

		removeGraphFromRot();

		for (int i = viewerComps.size(); --i >= 0;)
			viewerComps.getUnchecked(i)->ownerDeleted();

		callback = nullptr;
		graphBuilder = nullptr;
		sampleGrabber = nullptr;
		mediaControl = nullptr;
		filter = nullptr;
		captureGraphBuilder = nullptr;
		smartTee = nullptr;
		smartTeePreviewOutputPin = nullptr;
		smartTeeCaptureOutputPin = nullptr;
		asfWriter = nullptr;
	}

	void addUser()
	{
		if (ok && activeUsers++ == 0)
			mediaControl->Run();
	}

	void removeUser()
	{
		if (ok && --activeUsers == 0)
			mediaControl->Stop();
	}

	int getPreviewMaxFPS() const
	{
		return previewMaxFPS;
	}

	void handleFrame (double /*time*/, BYTE* buffer, long /*bufferSize*/)
	{
		if (recordNextFrameTime)
		{
			const double defaultCameraLatency = 0.1;

			firstRecordedTime = Time::getCurrentTime() - RelativeTime (defaultCameraLatency);
			recordNextFrameTime = false;

			ComSmartPtr <IPin> pin;
			if (getPin (filter, PINDIR_OUTPUT, pin))
			{
				ComSmartPtr <IAMPushSource> pushSource;
				HRESULT hr = pin.QueryInterface (pushSource);

				if (pushSource != nullptr)
				{
					REFERENCE_TIME latency = 0;
					hr = pushSource->GetLatency (&latency);

					firstRecordedTime = firstRecordedTime - RelativeTime ((double) latency);
				}
			}
		}

		{
			const int lineStride = width * 3;
			const ScopedLock sl (imageSwapLock);

			{
				const Image::BitmapData destData (loadingImage, 0, 0, width, height, Image::BitmapData::writeOnly);

				for (int i = 0; i < height; ++i)
					memcpy (destData.getLinePointer ((height - 1) - i),
							buffer + lineStride * i,
							lineStride);
			}

			imageNeedsFlipping = true;
		}

		if (listeners.size() > 0)
			callListeners (loadingImage);

		sendChangeMessage();
	}

	void drawCurrentImage (Graphics& g, int x, int y, int w, int h)
	{
		if (imageNeedsFlipping)
		{
			const ScopedLock sl (imageSwapLock);
			swapVariables (loadingImage, activeImage);
			imageNeedsFlipping = false;
		}

		RectanglePlacement rp (RectanglePlacement::centred);
		double dx = 0, dy = 0, dw = width, dh = height;
		rp.applyTo (dx, dy, dw, dh, x, y, w, h);
		const int rx = roundToInt (dx), ry = roundToInt (dy);
		const int rw = roundToInt (dw), rh = roundToInt (dh);

		{
			Graphics::ScopedSaveState ss (g);

			g.excludeClipRegion (Rectangle<int> (rx, ry, rw, rh));
			g.fillAll (Colours::black);
		}

		g.drawImage (activeImage, rx, ry, rw, rh, 0, 0, width, height);
	}

	bool createFileCaptureFilter (const File& file, int quality)
	{
		removeFileCaptureFilter();
		file.deleteFile();
		mediaControl->Stop();
		firstRecordedTime = Time();
		recordNextFrameTime = true;
		previewMaxFPS = 60;

		HRESULT hr = asfWriter.CoCreateInstance (CLSID_WMAsfWriter);

		if (SUCCEEDED (hr))
		{
			ComSmartPtr <IFileSinkFilter> fileSink;
			hr = asfWriter.QueryInterface (fileSink);

			if (SUCCEEDED (hr))
			{
				hr = fileSink->SetFileName (file.getFullPathName().toWideCharPointer(), 0);

				if (SUCCEEDED (hr))
				{
					hr = graphBuilder->AddFilter (asfWriter, _T("AsfWriter"));

					if (SUCCEEDED (hr))
					{
						ComSmartPtr <IConfigAsfWriter> asfConfig;
						hr = asfWriter.QueryInterface (asfConfig);
						asfConfig->SetIndexMode (true);
						ComSmartPtr <IWMProfileManager> profileManager;
						hr = WMCreateProfileManager (profileManager.resetAndGetPointerAddress());

						// This gibberish is the DirectShow profile for a video-only wmv file.
						String prof ("<profile version=\"589824\" storageformat=\"1\" name=\"Quality\" description=\"Quality type for output.\">"
									   "<streamconfig majortype=\"{73646976-0000-0010-8000-00AA00389B71}\" streamnumber=\"1\" "
													 "streamname=\"Video Stream\" inputname=\"Video409\" bitrate=\"894960\" "
													 "bufferwindow=\"0\" reliabletransport=\"1\" decodercomplexity=\"AU\" rfc1766langid=\"en-us\">"
										 "<videomediaprops maxkeyframespacing=\"50000000\" quality=\"90\"/>"
										 "<wmmediatype subtype=\"{33564D57-0000-0010-8000-00AA00389B71}\" bfixedsizesamples=\"0\" "
													  "btemporalcompression=\"1\" lsamplesize=\"0\">"
										 "<videoinfoheader dwbitrate=\"894960\" dwbiterrorrate=\"0\" avgtimeperframe=\"$AVGTIMEPERFRAME\">"
											 "<rcsource left=\"0\" top=\"0\" right=\"$WIDTH\" bottom=\"$HEIGHT\"/>"
											 "<rctarget left=\"0\" top=\"0\" right=\"$WIDTH\" bottom=\"$HEIGHT\"/>"
											 "<bitmapinfoheader biwidth=\"$WIDTH\" biheight=\"$HEIGHT\" biplanes=\"1\" bibitcount=\"24\" "
															   "bicompression=\"WMV3\" bisizeimage=\"0\" bixpelspermeter=\"0\" biypelspermeter=\"0\" "
															   "biclrused=\"0\" biclrimportant=\"0\"/>"
										   "</videoinfoheader>"
										 "</wmmediatype>"
									   "</streamconfig>"
									 "</profile>");

						const int fps[] = { 10, 15, 30 };
						int maxFramesPerSecond = fps [jlimit (0, numElementsInArray (fps) - 1, quality & 0xff)];

						if ((quality & 0xff000000) != 0) // (internal hacky way to pass explicit frame rates for testing)
							maxFramesPerSecond = (quality >> 24) & 0xff;

						prof = prof.replace ("$WIDTH", String (width))
								   .replace ("$HEIGHT", String (height))
								   .replace ("$AVGTIMEPERFRAME", String (10000000 / maxFramesPerSecond));

						ComSmartPtr <IWMProfile> currentProfile;
						hr = profileManager->LoadProfileByData (prof.toWideCharPointer(), currentProfile.resetAndGetPointerAddress());
						hr = asfConfig->ConfigureFilterUsingProfile (currentProfile);

						if (SUCCEEDED (hr))
						{
							ComSmartPtr <IPin> asfWriterInputPin;

							if (getPin (asfWriter, PINDIR_INPUT, asfWriterInputPin, "Video Input 01"))
							{
								hr = graphBuilder->Connect (smartTeeCaptureOutputPin, asfWriterInputPin);

								if (SUCCEEDED (hr) && ok && activeUsers > 0
									 && SUCCEEDED (mediaControl->Run()))
								{
									previewMaxFPS = (quality < 2) ? 15 : 25; // throttle back the preview comps to try to leave the cpu free for encoding

									if ((quality & 0x00ff0000) != 0)  // (internal hacky way to pass explicit frame rates for testing)
										previewMaxFPS = (quality >> 16) & 0xff;

									return true;
								}
							}
						}
					}
				}
			}
		}

		removeFileCaptureFilter();

		if (ok && activeUsers > 0)
			mediaControl->Run();

		return false;
	}

	void removeFileCaptureFilter()
	{
		mediaControl->Stop();

		if (asfWriter != nullptr)
		{
			graphBuilder->RemoveFilter (asfWriter);
			asfWriter = nullptr;
		}

		if (ok && activeUsers > 0)
			mediaControl->Run();

		previewMaxFPS = 60;
	}

	void addListener (CameraDevice::Listener* listenerToAdd)
	{
		const ScopedLock sl (listenerLock);

		if (listeners.size() == 0)
			addUser();

		listeners.addIfNotAlreadyThere (listenerToAdd);
	}

	void removeListener (CameraDevice::Listener* listenerToRemove)
	{
		const ScopedLock sl (listenerLock);
		listeners.removeValue (listenerToRemove);

		if (listeners.size() == 0)
			removeUser();
	}

	void callListeners (const Image& image)
	{
		const ScopedLock sl (listenerLock);

		for (int i = listeners.size(); --i >= 0;)
		{
			CameraDevice::Listener* const l = listeners[i];

			if (l != nullptr)
				l->imageReceived (image);
		}
	}

	class DShowCaptureViewerComp   : public Component,
									 public ChangeListener
	{
	public:
		DShowCaptureViewerComp (DShowCameraDeviceInteral* const owner_)
			: owner (owner_), maxFPS (15), lastRepaintTime (0)
		{
			setOpaque (true);
			owner->addChangeListener (this);
			owner->addUser();
			owner->viewerComps.add (this);
			setSize (owner->width, owner->height);
		}

		~DShowCaptureViewerComp()
		{
			if (owner != nullptr)
			{
				owner->viewerComps.removeValue (this);
				owner->removeUser();
				owner->removeChangeListener (this);
			}
		}

		void ownerDeleted()
		{
			owner = nullptr;
		}

		void paint (Graphics& g)
		{
			g.setColour (Colours::black);
			g.setImageResamplingQuality (Graphics::lowResamplingQuality);

			if (owner != nullptr)
				owner->drawCurrentImage (g, 0, 0, getWidth(), getHeight());
			else
				g.fillAll (Colours::black);
		}

		void changeListenerCallback (ChangeBroadcaster*)
		{
			const int64 now = Time::currentTimeMillis();

			if (now >= lastRepaintTime + (1000 / maxFPS))
			{
				lastRepaintTime = now;
				repaint();

				if (owner != nullptr)
					maxFPS = owner->getPreviewMaxFPS();
			}
		}

	private:
		DShowCameraDeviceInteral* owner;
		int maxFPS;
		int64 lastRepaintTime;
	};

	bool ok;
	int width, height;
	Time firstRecordedTime;

	Array <DShowCaptureViewerComp*> viewerComps;

private:
	CameraDevice* const owner;
	ComSmartPtr <ICaptureGraphBuilder2> captureGraphBuilder;
	ComSmartPtr <IBaseFilter> filter;
	ComSmartPtr <IBaseFilter> smartTee;
	ComSmartPtr <IGraphBuilder> graphBuilder;
	ComSmartPtr <ISampleGrabber> sampleGrabber;
	ComSmartPtr <IMediaControl> mediaControl;
	ComSmartPtr <IPin> smartTeePreviewOutputPin;
	ComSmartPtr <IPin> smartTeeCaptureOutputPin;
	ComSmartPtr <IBaseFilter> asfWriter;
	int activeUsers;
	Array <int> widths, heights;
	DWORD graphRegistrationID;

	CriticalSection imageSwapLock;
	bool imageNeedsFlipping;
	Image loadingImage;
	Image activeImage;

	bool recordNextFrameTime;
	int previewMaxFPS;

	void getVideoSizes (IAMStreamConfig* const streamConfig)
	{
		widths.clear();
		heights.clear();

		int count = 0, size = 0;
		streamConfig->GetNumberOfCapabilities (&count, &size);

		if (size == sizeof (VIDEO_STREAM_CONFIG_CAPS))
		{
			for (int i = 0; i < count; ++i)
			{
				VIDEO_STREAM_CONFIG_CAPS scc;
				AM_MEDIA_TYPE* config;

				HRESULT hr = streamConfig->GetStreamCaps (i, &config, (BYTE*) &scc);

				if (SUCCEEDED (hr))
				{
					const int w = scc.InputSize.cx;
					const int h = scc.InputSize.cy;

					bool duplicate = false;

					for (int j = widths.size(); --j >= 0;)
					{
						if (w == widths.getUnchecked (j) && h == heights.getUnchecked (j))
						{
							duplicate = true;
							break;
						}
					}

					if (! duplicate)
					{
						DBG ("Camera capture size: " + String (w) + ", " + String (h));
						widths.add (w);
						heights.add (h);
					}

					deleteMediaType (config);
				}
			}
		}
	}

	bool selectVideoSize (IAMStreamConfig* const streamConfig,
						  const int minWidth, const int minHeight,
						  const int maxWidth, const int maxHeight)
	{
		int count = 0, size = 0, bestArea = 0, bestIndex = -1;
		streamConfig->GetNumberOfCapabilities (&count, &size);

		if (size == sizeof (VIDEO_STREAM_CONFIG_CAPS))
		{
			AM_MEDIA_TYPE* config;
			VIDEO_STREAM_CONFIG_CAPS scc;

			for (int i = 0; i < count; ++i)
			{
				HRESULT hr = streamConfig->GetStreamCaps (i, &config, (BYTE*) &scc);

				if (SUCCEEDED (hr))
				{
					if (scc.InputSize.cx >= minWidth
						 && scc.InputSize.cy >= minHeight
						 && scc.InputSize.cx <= maxWidth
						 && scc.InputSize.cy <= maxHeight)
					{
						int area = scc.InputSize.cx * scc.InputSize.cy;
						if (area > bestArea)
						{
							bestIndex = i;
							bestArea = area;
						}
					}

					deleteMediaType (config);
				}
			}

			if (bestIndex >= 0)
			{
				HRESULT hr = streamConfig->GetStreamCaps (bestIndex, &config, (BYTE*) &scc);

				hr = streamConfig->SetFormat (config);
				deleteMediaType (config);
				return SUCCEEDED (hr);
			}
		}

		return false;
	}

	static bool getPin (IBaseFilter* filter, const PIN_DIRECTION wantedDirection,
						ComSmartPtr<IPin>& result, const char* pinName = nullptr)
	{
		ComSmartPtr <IEnumPins> enumerator;
		ComSmartPtr <IPin> pin;

		filter->EnumPins (enumerator.resetAndGetPointerAddress());

		while (enumerator->Next (1, pin.resetAndGetPointerAddress(), 0) == S_OK)
		{
			PIN_DIRECTION dir;
			pin->QueryDirection (&dir);

			if (wantedDirection == dir)
			{
				PIN_INFO info = { 0 };
				pin->QueryPinInfo (&info);

				if (pinName == nullptr || String (pinName).equalsIgnoreCase (String (info.achName)))
				{
					result = pin;
					return true;
				}
			}
		}

		return false;
	}

	bool connectFilters (IBaseFilter* const first, IBaseFilter* const second) const
	{
		ComSmartPtr <IPin> in, out;

		return getPin (first, PINDIR_OUTPUT, out)
				&& getPin (second, PINDIR_INPUT, in)
				&& SUCCEEDED (graphBuilder->Connect (out, in));
	}

	bool addGraphToRot()
	{
		ComSmartPtr <IRunningObjectTable> rot;
		if (FAILED (GetRunningObjectTable (0, rot.resetAndGetPointerAddress())))
			return false;

		ComSmartPtr <IMoniker> moniker;
		WCHAR buffer[128];
		HRESULT hr = CreateItemMoniker (_T("!"), buffer, moniker.resetAndGetPointerAddress());
		if (FAILED (hr))
			return false;

		graphRegistrationID = 0;
		return SUCCEEDED (rot->Register (0, graphBuilder, moniker, &graphRegistrationID));
	}

	void removeGraphFromRot()
	{
		ComSmartPtr <IRunningObjectTable> rot;

		if (SUCCEEDED (GetRunningObjectTable (0, rot.resetAndGetPointerAddress())))
			rot->Revoke (graphRegistrationID);
	}

	static void deleteMediaType (AM_MEDIA_TYPE* const pmt)
	{
		if (pmt->cbFormat != 0)
			CoTaskMemFree ((PVOID) pmt->pbFormat);

		if (pmt->pUnk != nullptr)
			pmt->pUnk->Release();

		CoTaskMemFree (pmt);
	}

	class GrabberCallback   : public ComBaseClassHelperBase <ISampleGrabberCB>
	{
	public:
		GrabberCallback (DShowCameraDeviceInteral& owner_)  : owner (owner_) {}

		JUCE_COMRESULT QueryInterface (REFIID refId, void** result)
		{
			if (refId == IID_ISampleGrabberCB)  { AddRef(); *result = dynamic_cast <ISampleGrabberCB*> (this); return S_OK; }
			if (refId == IID_IUnknown)          { AddRef(); *result = dynamic_cast <IUnknown*> (this); return S_OK; }

			*result = nullptr;
			return E_NOINTERFACE;
		}

		STDMETHODIMP SampleCB (double /*SampleTime*/, IMediaSample* /*pSample*/)  { return E_FAIL; }

		STDMETHODIMP BufferCB (double time, BYTE* buffer, long bufferSize)
		{
			owner.handleFrame (time, buffer, bufferSize);
			return S_OK;
		}

	private:
		DShowCameraDeviceInteral& owner;

		JUCE_DECLARE_NON_COPYABLE (GrabberCallback);
	};

	ComSmartPtr <GrabberCallback> callback;
	Array <CameraDevice::Listener*> listeners;
	CriticalSection listenerLock;

	JUCE_DECLARE_NON_COPYABLE (DShowCameraDeviceInteral);
};

CameraDevice::CameraDevice (const String& name_, int /*index*/)
	: name (name_)
{
	isRecording = false;
}

CameraDevice::~CameraDevice()
{
	stopRecording();
	delete static_cast <DShowCameraDeviceInteral*> (internal);
	internal = nullptr;
}

Component* CameraDevice::createViewerComponent()
{
	return new DShowCameraDeviceInteral::DShowCaptureViewerComp (static_cast <DShowCameraDeviceInteral*> (internal));
}

String CameraDevice::getFileExtension()
{
	return ".wmv";
}

void CameraDevice::startRecordingToFile (const File& file, int quality)
{
	stopRecording();

	DShowCameraDeviceInteral* const d = (DShowCameraDeviceInteral*) internal;
	d->addUser();
	isRecording = d->createFileCaptureFilter (file, quality);
}

Time CameraDevice::getTimeOfFirstRecordedFrame() const
{
	DShowCameraDeviceInteral* const d = (DShowCameraDeviceInteral*) internal;
	return d->firstRecordedTime;
}

void CameraDevice::stopRecording()
{
	if (isRecording)
	{
		DShowCameraDeviceInteral* const d = (DShowCameraDeviceInteral*) internal;
		d->removeFileCaptureFilter();
		d->removeUser();
		isRecording = false;
	}
}

void CameraDevice::addListener (Listener* listenerToAdd)
{
	DShowCameraDeviceInteral* const d = (DShowCameraDeviceInteral*) internal;

	if (listenerToAdd != nullptr)
		d->addListener (listenerToAdd);
}

void CameraDevice::removeListener (Listener* listenerToRemove)
{
	DShowCameraDeviceInteral* const d = (DShowCameraDeviceInteral*) internal;

	if (listenerToRemove != nullptr)
		d->removeListener (listenerToRemove);
}

namespace
{
	ComSmartPtr <IBaseFilter> enumerateCameras (StringArray* const names,
												const int deviceIndexToOpen,
												String& name)
	{
		int index = 0;
		ComSmartPtr <IBaseFilter> result;

		ComSmartPtr <ICreateDevEnum> pDevEnum;
		HRESULT hr = pDevEnum.CoCreateInstance (CLSID_SystemDeviceEnum);

		if (SUCCEEDED (hr))
		{
			ComSmartPtr <IEnumMoniker> enumerator;
			hr = pDevEnum->CreateClassEnumerator (CLSID_VideoInputDeviceCategory, enumerator.resetAndGetPointerAddress(), 0);

			if (SUCCEEDED (hr) && enumerator != nullptr)
			{
				ComSmartPtr <IMoniker> moniker;
				ULONG fetched;

				while (enumerator->Next (1, moniker.resetAndGetPointerAddress(), &fetched) == S_OK)
				{
					ComSmartPtr <IBaseFilter> captureFilter;
					hr = moniker->BindToObject (0, 0, IID_IBaseFilter, (void**) captureFilter.resetAndGetPointerAddress());

					if (SUCCEEDED (hr))
					{
						ComSmartPtr <IPropertyBag> propertyBag;
						hr = moniker->BindToStorage (0, 0, IID_IPropertyBag, (void**) propertyBag.resetAndGetPointerAddress());

						if (SUCCEEDED (hr))
						{
							VARIANT var;
							var.vt = VT_BSTR;

							hr = propertyBag->Read (_T("FriendlyName"), &var, 0);
							propertyBag = nullptr;

							if (SUCCEEDED (hr))
							{
								if (names != nullptr)
									names->add (var.bstrVal);

								if (index == deviceIndexToOpen)
								{
									name = var.bstrVal;
									result = captureFilter;
									break;
								}

								++index;
							}
						}
					}
				}
			}
		}

		return result;
	}
}

StringArray CameraDevice::getAvailableDevices()
{
	StringArray devs;
	String dummy;
	enumerateCameras (&devs, -1, dummy);
	return devs;
}

CameraDevice* CameraDevice::openDevice (int index,
										int minWidth, int minHeight,
										int maxWidth, int maxHeight)
{
	ComSmartPtr <ICaptureGraphBuilder2> captureGraphBuilder;
	HRESULT hr = captureGraphBuilder.CoCreateInstance (CLSID_CaptureGraphBuilder2);

	if (SUCCEEDED (hr))
	{
		String name;
		const ComSmartPtr <IBaseFilter> filter (enumerateCameras (0, index, name));

		if (filter != nullptr)
		{
			ScopedPointer <CameraDevice> cam (new CameraDevice (name, index));

			DShowCameraDeviceInteral* const intern
				= new DShowCameraDeviceInteral (cam, captureGraphBuilder, filter,
												minWidth, minHeight, maxWidth, maxHeight);
			cam->internal = intern;

			if (intern->ok)
				return cam.release();
		}
	}

	return nullptr;
}

/*** End of inlined file: juce_win32_CameraDevice.cpp ***/


 #endif

 #if JUCE_DIRECTSHOW

/*** Start of inlined file: juce_win32_DirectShowComponent.cpp ***/
namespace DirectShowHelpers
{
	bool checkDShowAvailability()
	{
		ComSmartPtr <IGraphBuilder> graph;
		return SUCCEEDED (graph.CoCreateInstance (CLSID_FilterGraph));
	}

	class VideoRenderer
	{
	public:
		VideoRenderer() {}
		virtual ~VideoRenderer() {}

		virtual HRESULT create (ComSmartPtr <IGraphBuilder>& graphBuilder,
								ComSmartPtr <IBaseFilter>& baseFilter, HWND hwnd) = 0;

		virtual void setVideoWindow (HWND hwnd) = 0;
		virtual void setVideoPosition (HWND hwnd, long videoWidth, long videoHeight) = 0;
		virtual void repaintVideo (HWND hwnd, HDC hdc) = 0;
		virtual void displayModeChanged() = 0;
		virtual HRESULT getVideoSize (long& videoWidth, long& videoHeight) = 0;
	};

	class VMR7  : public VideoRenderer
	{
	public:
		VMR7() {}

		HRESULT create (ComSmartPtr <IGraphBuilder>& graphBuilder,
						ComSmartPtr <IBaseFilter>& baseFilter, HWND hwnd)
		{
			ComSmartPtr <IVMRFilterConfig> filterConfig;

			HRESULT hr = baseFilter.CoCreateInstance (CLSID_VideoMixingRenderer);

			if (SUCCEEDED (hr))
				hr = graphBuilder->AddFilter (baseFilter, L"VMR-7");

			if (SUCCEEDED (hr))
				hr = baseFilter.QueryInterface (filterConfig);

			if (SUCCEEDED (hr))
				hr = filterConfig->SetRenderingMode (VMRMode_Windowless);

			if (SUCCEEDED (hr))
				hr = baseFilter.QueryInterface (windowlessControl);

			if (SUCCEEDED (hr))
				hr = windowlessControl->SetVideoClippingWindow (hwnd);

			if (SUCCEEDED (hr))
				hr = windowlessControl->SetAspectRatioMode (VMR_ARMODE_LETTER_BOX);

			return hr;
		}

		void setVideoWindow (HWND hwnd)
		{
			windowlessControl->SetVideoClippingWindow (hwnd);
		}

		void setVideoPosition (HWND hwnd, long videoWidth, long videoHeight)
		{
			RECT src, dest;

			SetRect (&src, 0, 0, videoWidth, videoHeight);
			GetClientRect (hwnd, &dest);

			windowlessControl->SetVideoPosition (&src, &dest);
		}

		void repaintVideo (HWND hwnd, HDC hdc)
		{
			windowlessControl->RepaintVideo (hwnd, hdc);
		}

		void displayModeChanged()
		{
			windowlessControl->DisplayModeChanged();
		}

		HRESULT getVideoSize (long& videoWidth, long& videoHeight)
		{
			return windowlessControl->GetNativeVideoSize (&videoWidth, &videoHeight, nullptr, nullptr);
		}

	private:
		ComSmartPtr <IVMRWindowlessControl> windowlessControl;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VMR7);
	};

#if JUCE_MEDIAFOUNDATION
	class EVR : public VideoRenderer
	{
	public:
		EVR() {}

		HRESULT create (ComSmartPtr <IGraphBuilder>& graphBuilder,
						ComSmartPtr <IBaseFilter>& baseFilter, HWND hwnd)
		{
			ComSmartPtr <IMFGetService> getService;

			HRESULT hr = baseFilter.CoCreateInstance (CLSID_EnhancedVideoRenderer);

			if (SUCCEEDED (hr))
				hr = graphBuilder->AddFilter (baseFilter, L"EVR");

			if (SUCCEEDED (hr))
				hr = baseFilter.QueryInterface (getService);

			if (SUCCEEDED (hr))
				hr = getService->GetService (MR_VIDEO_RENDER_SERVICE, IID_IMFVideoDisplayControl,
											 (LPVOID*) videoDisplayControl.resetAndGetPointerAddress());

			if (SUCCEEDED (hr))
				hr = videoDisplayControl->SetVideoWindow (hwnd);

			if (SUCCEEDED (hr))
				hr = videoDisplayControl->SetAspectRatioMode (MFVideoARMode_PreservePicture);

			return hr;
		}

		void setVideoWindow (HWND hwnd)
		{
			videoDisplayControl->SetVideoWindow (hwnd);
		}

		void setVideoPosition (HWND hwnd, long /*videoWidth*/, long /*videoHeight*/)
		{
			const MFVideoNormalizedRect src = { 0.0f, 0.0f, 1.0f, 1.0f };

			RECT dest;
			GetClientRect (hwnd, &dest);

			videoDisplayControl->SetVideoPosition (&src, &dest);
		}

		void repaintVideo (HWND /*hwnd*/, HDC /*hdc*/)
		{
			videoDisplayControl->RepaintVideo();
		}

		void displayModeChanged() {}

		HRESULT getVideoSize (long& videoWidth, long& videoHeight)
		{
			SIZE sz;
			HRESULT hr = videoDisplayControl->GetNativeVideoSize (&sz, nullptr);
			videoWidth  = sz.cx;
			videoHeight = sz.cy;
			return hr;
		}

	private:
		ComSmartPtr <IMFVideoDisplayControl> videoDisplayControl;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EVR);
	};
#endif
}

class DirectShowComponent::DirectShowContext
{
public:
	DirectShowContext (DirectShowComponent& component_, VideoRendererType type_)
		: component (component_),
		  hwnd (0),
		  hdc (0),
		  state (uninitializedState),
		  hasVideo (false),
		  videoWidth (0),
		  videoHeight (0),
		  type (type_)
	{
		CoInitialize (0);

		if (type == dshowDefault)
		{
			type = dshowVMR7;

		   #if JUCE_MEDIAFOUNDATION
			if (SystemStats::getOperatingSystemType() >= SystemStats::WinVista)
				type = dshowEVR;
		   #endif
		}
	}

	~DirectShowContext()
	{
		release();
		CoUninitialize();
	}

	void updateWindowPosition (const Rectangle<int>& newBounds)
	{
		nativeWindow->setWindowPosition (newBounds);
	}

	void showWindow (bool shouldBeVisible)
	{
		nativeWindow->showWindow (shouldBeVisible);
	}

	void repaint()
	{
		if (hasVideo)
			videoRenderer->repaintVideo (nativeWindow->getHandle(), nativeWindow->getContext());
	}

	void updateVideoPosition()
	{
		if (hasVideo)
			videoRenderer->setVideoPosition (nativeWindow->getHandle(), videoWidth, videoHeight);
	}

	void displayResolutionChanged()
	{
		if (hasVideo)
			videoRenderer->displayModeChanged();
	}

	void peerChanged()
	{
		deleteNativeWindow();

		mediaEvent->SetNotifyWindow (0, 0, 0);
		if (videoRenderer != nullptr)
			videoRenderer->setVideoWindow (nullptr);

		createNativeWindow();

		mediaEvent->SetNotifyWindow ((OAHWND) hwnd, graphEventID, 0);
		if (videoRenderer != nullptr)
			videoRenderer->setVideoWindow (hwnd);
	}

	bool loadFile (const String& fileOrURLPath)
	{
		jassert (state == uninitializedState);

		if (! createNativeWindow())
			return false;

		HRESULT hr = graphBuilder.CoCreateInstance (CLSID_FilterGraph);

		// basic playback interfaces
		if (SUCCEEDED (hr))   hr = graphBuilder.QueryInterface (mediaControl);
		if (SUCCEEDED (hr))   hr = graphBuilder.QueryInterface (mediaPosition);
		if (SUCCEEDED (hr))   hr = graphBuilder.QueryInterface (mediaEvent);
		if (SUCCEEDED (hr))   hr = graphBuilder.QueryInterface (basicAudio);

		// video renderer interface
		if (SUCCEEDED (hr))
		{
		   #if JUCE_MEDIAFOUNDATION
			if (type == dshowEVR)
				videoRenderer = new DirectShowHelpers::EVR();
			else
		   #endif
				videoRenderer = new DirectShowHelpers::VMR7();

			hr = videoRenderer->create (graphBuilder, baseFilter, hwnd);
		}

		// build filter graph
		if (SUCCEEDED (hr))
			hr = graphBuilder->RenderFile (fileOrURLPath.toWideCharPointer(), nullptr);

		// remove video renderer if not connected (no video)
		if (SUCCEEDED (hr))
		{
			if (isRendererConnected())
			{
				hasVideo = true;
				hr = videoRenderer->getVideoSize (videoWidth, videoHeight);
			}
			else
			{
				hasVideo = false;
				graphBuilder->RemoveFilter (baseFilter);
				videoRenderer = nullptr;
				baseFilter = nullptr;
			}
		}

		// set window to receive events
		if (SUCCEEDED (hr))
			hr = mediaEvent->SetNotifyWindow ((OAHWND) hwnd, graphEventID, 0);

		if (SUCCEEDED (hr))
		{
			state = stoppedState;
			return true;
		}

		release();
		return false;
	}

	void release()
	{
		if (mediaControl != nullptr)
			mediaControl->Stop();

		if (mediaEvent != nullptr)
			mediaEvent->SetNotifyWindow (0, 0, 0);

		if (videoRenderer != nullptr)
			videoRenderer->setVideoWindow (0);

		hasVideo = false;
		videoRenderer = nullptr;

		baseFilter = nullptr;
		basicAudio = nullptr;
		mediaEvent = nullptr;
		mediaPosition = nullptr;
		mediaControl = nullptr;
		graphBuilder = nullptr;

		state = uninitializedState;

		videoWidth = 0;
		videoHeight = 0;

		if (nativeWindow != nullptr)
			deleteNativeWindow();
	}

	void graphEventProc()
	{
		LONG ec;
		LONG_PTR p1, p2;

		jassert (mediaEvent != nullptr);

		while (SUCCEEDED (mediaEvent->GetEvent (&ec, &p1, &p2, 0)))
		{
			switch (ec)
			{
			case EC_REPAINT:
				component.repaint();
				break;

			case EC_COMPLETE:
				if (component.isLooping())
					component.goToStart();
				else
					component.stop();
				break;

			case EC_USERABORT:
			case EC_ERRORABORT:
			case EC_ERRORABORTEX:
				component.closeMovie();
				break;

			default:
				break;
			}

			mediaEvent->FreeEventParams (ec, p1, p2);
		}
	}

	void run()
	{
		mediaControl->Run();
		state = runningState;
	}

	void stop()
	{
		mediaControl->Stop();
		state = stoppedState;
	}

	void pause()
	{
		mediaControl->Pause();
		state = pausedState;
	}

	bool isInitialised() const noexcept  { return state != uninitializedState; }
	bool isRunning() const noexcept      { return state == runningState; }
	bool isPaused() const noexcept       { return state == pausedState; }
	bool isStopped() const noexcept      { return state == stoppedState; }
	bool containsVideo() const noexcept  { return hasVideo; }
	int getVideoWidth() const noexcept   { return (int) videoWidth; }
	int getVideoHeight() const noexcept  { return (int) videoHeight; }

	double getDuration() const
	{
		REFTIME duration;
		mediaPosition->get_Duration (&duration);
		return duration;
	}

	double getPosition() const
	{
		REFTIME seconds;
		mediaPosition->get_CurrentPosition (&seconds);
		return seconds;
	}

	void setSpeed (const float newSpeed)        { mediaPosition->put_Rate (newSpeed); }
	void setPosition (const double seconds)     { mediaPosition->put_CurrentPosition (seconds); }
	void setVolume (const float newVolume)      { basicAudio->put_Volume (convertToDShowVolume (newVolume)); }

	// in DirectShow, full volume is 0, silence is -10000
	static long convertToDShowVolume (const float vol) noexcept
	{
		if (vol >= 1.0f) return 0;
		if (vol <= 0.0f) return -10000;

		return roundToInt ((vol * 10000.0f) - 10000.0f);
	}

	float getVolume() const
	{
		long volume;
		basicAudio->get_Volume (&volume);
		return (volume + 10000) / 10000.0f;
	}

private:

	enum { graphEventID = WM_APP + 0x43f0 };

	DirectShowComponent& component;
	HWND hwnd;
	HDC hdc;

	enum State { uninitializedState, runningState, pausedState, stoppedState };
	State state;

	bool hasVideo;
	long videoWidth, videoHeight;

	VideoRendererType type;

	ComSmartPtr <IGraphBuilder> graphBuilder;
	ComSmartPtr <IMediaControl> mediaControl;
	ComSmartPtr <IMediaPosition> mediaPosition;
	ComSmartPtr <IMediaEventEx> mediaEvent;
	ComSmartPtr <IBasicAudio> basicAudio;
	ComSmartPtr <IBaseFilter> baseFilter;

	ScopedPointer <DirectShowHelpers::VideoRenderer> videoRenderer;

	class NativeWindowClass   : public DeletedAtShutdown
	{
	private:
		NativeWindowClass()
			: atom (0)
		{
			String windowClassName ("JUCE_DIRECTSHOW_");
			windowClassName << (int) (Time::currentTimeMillis() & 0x7fffffff);

			HINSTANCE moduleHandle = (HINSTANCE) Process::getCurrentModuleInstanceHandle();

			TCHAR moduleFile [1024] = { 0 };
			GetModuleFileName (moduleHandle, moduleFile, 1024);

			WNDCLASSEX wcex = { 0 };
			wcex.cbSize         = sizeof (wcex);
			wcex.style          = CS_OWNDC;
			wcex.lpfnWndProc    = (WNDPROC) wndProc;
			wcex.lpszClassName  = windowClassName.toWideCharPointer();
			wcex.hInstance      = moduleHandle;

			atom = RegisterClassEx (&wcex);
			jassert (atom != 0);
		}

		~NativeWindowClass()
		{
			if (atom != 0)
				UnregisterClass (getWindowClassName(), (HINSTANCE) Process::getCurrentModuleInstanceHandle());

			clearSingletonInstance();
		}

		static LRESULT CALLBACK wndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			DirectShowContext* c = (DirectShowContext*) GetWindowLongPtr (hwnd, GWLP_USERDATA);

			if (c != nullptr)
			{
				switch (msg)
				{
					case WM_ERASEBKGND:       return 1;
					case WM_DISPLAYCHANGE:    c->displayResolutionChanged(); break;
					case graphEventID:        c->graphEventProc(); return 0;
					default:                  break;
				}
			}

			return DefWindowProc (hwnd, msg, wParam, lParam);
		}

	public:
		bool isRegistered() const noexcept              { return atom != 0; }
		LPCTSTR getWindowClassName() const noexcept     { return (LPCTSTR) MAKELONG (atom, 0); }

		juce_DeclareSingleton_SingleThreaded_Minimal (NativeWindowClass);

	private:
		ATOM atom;

		JUCE_DECLARE_NON_COPYABLE (NativeWindowClass);
	};

	class NativeWindow
	{
	public:
		NativeWindow (HWND parentToAddTo, void* const userData)
			: hwnd (0), hdc (0)
		{
			NativeWindowClass* const wc = NativeWindowClass::getInstance();

			if (wc->isRegistered())
			{
				DWORD exstyle = 0;
				DWORD type = WS_CHILD;

				hwnd = CreateWindowEx (exstyle, wc->getWindowClassName(),
									   L"", type, 0, 0, 0, 0, parentToAddTo, 0,
									   (HINSTANCE) Process::getCurrentModuleInstanceHandle(), 0);

				if (hwnd != 0)
				{
					hdc = GetDC (hwnd);
					SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) userData);
				}
			}

			jassert (hwnd != 0);
		}

		~NativeWindow()
		{
			if (hwnd != 0)
			{
				SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) 0);
				DestroyWindow (hwnd);
			}
		}

		HWND getHandle() const noexcept   { return hwnd; }
		HDC getContext() const noexcept   { return hdc; }

		void setWindowPosition (const Rectangle<int>& newBounds)
		{
			SetWindowPos (hwnd, 0, newBounds.getX(), newBounds.getY(),
						  newBounds.getWidth(), newBounds.getHeight(),
						  SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER);
		}

		void showWindow (const bool shouldBeVisible)
		{
			ShowWindow (hwnd, shouldBeVisible ? SW_SHOWNA : SW_HIDE);
		}

	private:
		HWND hwnd;
		HDC hdc;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeWindow);
	};

	ScopedPointer<NativeWindow> nativeWindow;

	bool createNativeWindow()
	{
		jassert (nativeWindow == nullptr);

		ComponentPeer* topLevelPeer = component.getTopLevelComponent()->getPeer();

		jassert (topLevelPeer != nullptr);

		if (topLevelPeer != nullptr)
		{
			nativeWindow = new NativeWindow ((HWND) topLevelPeer->getNativeHandle(), this);

			hwnd = nativeWindow->getHandle();

			if (hwnd != 0)
			{
				hdc = GetDC (hwnd);
				component.updateContextPosition();
				component.showContext (component.isShowing());
				return true;
			}
			else
			{
				nativeWindow = nullptr;
			}
		}

		return false;
	}

	void deleteNativeWindow()
	{
		jassert (nativeWindow != nullptr);
		ReleaseDC (hwnd, hdc);
		hwnd = 0;
		hdc = 0;
		nativeWindow = nullptr;
	}

	bool isRendererConnected()
	{
		ComSmartPtr <IEnumPins> enumPins;

		HRESULT hr = baseFilter->EnumPins (enumPins.resetAndGetPointerAddress());

		if (SUCCEEDED (hr))
			hr = enumPins->Reset();

		ComSmartPtr<IPin> pin;

		while (SUCCEEDED (hr)
				&& enumPins->Next (1, pin.resetAndGetPointerAddress(), nullptr) == S_OK)
		{
			ComSmartPtr<IPin> otherPin;

			hr = pin->ConnectedTo (otherPin.resetAndGetPointerAddress());

			if (SUCCEEDED (hr))
			{
				PIN_DIRECTION direction;
				hr = pin->QueryDirection (&direction);

				if (SUCCEEDED (hr) && direction == PINDIR_INPUT)
					return true;
			}
			else if (hr == VFW_E_NOT_CONNECTED)
			{
				hr = S_OK;
			}
		}

		return false;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectShowContext);
};

juce_ImplementSingleton_SingleThreaded (DirectShowComponent::DirectShowContext::NativeWindowClass);

class DirectShowComponent::DirectShowComponentWatcher   : public ComponentMovementWatcher
{
public:
	DirectShowComponentWatcher (DirectShowComponent* const owner_)
		: ComponentMovementWatcher (owner_),
		  owner (owner_)
	{
	}

	void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/)
	{
		if (owner->videoLoaded)
			owner->updateContextPosition();
	}

	void componentPeerChanged()
	{
		if (owner->videoLoaded)
			owner->recreateNativeWindowAsync();
	}

	void componentVisibilityChanged()
	{
		if (owner->videoLoaded)
			owner->showContext (owner->isShowing());
	}

private:
	DirectShowComponent* const owner;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectShowComponentWatcher);
};

DirectShowComponent::DirectShowComponent (VideoRendererType type)
	: videoLoaded (false),
	  looping (false),
	  needToUpdateViewport (true),
	  needToRecreateNativeWindow (false)
{
	setOpaque (true);
	context = new DirectShowContext (*this, type);
	componentWatcher = new DirectShowComponentWatcher (this);
}

DirectShowComponent::~DirectShowComponent()
{
	componentWatcher = nullptr;
}

bool DirectShowComponent::isDirectShowAvailable()
{
	static bool isDSAvailable = DirectShowHelpers::checkDShowAvailability();
	return isDSAvailable;
}

void DirectShowComponent::recreateNativeWindowAsync()
{
	needToRecreateNativeWindow = true;
	repaint();
}

void DirectShowComponent::updateContextPosition()
{
	needToUpdateViewport = true;

	if (getWidth() > 0 && getHeight() > 0)
	{
		Component* const topComp = getTopLevelComponent();

		if (topComp->getPeer() != nullptr)
			context->updateWindowPosition (topComp->getLocalArea (this, getLocalBounds()));
	}
}

void DirectShowComponent::showContext (const bool shouldBeVisible)
{
	context->showWindow (shouldBeVisible);
}

void DirectShowComponent::paint (Graphics& g)
{
	if (videoLoaded)
	{
		if (needToRecreateNativeWindow)
		{
			context->peerChanged();
			needToRecreateNativeWindow = false;
		}

		if (needToUpdateViewport)
		{
			context->updateVideoPosition();
			needToUpdateViewport = false;
		}

		context->repaint();

		ComponentPeer* const peer = getPeer();

		if (peer != nullptr)
			peer->addMaskedRegion (getScreenBounds() - peer->getScreenPosition());
	}
	else
	{
		g.fillAll (Colours::grey);
	}
}

bool DirectShowComponent::loadMovie (const String& fileOrURLPath)
{
	closeMovie();

	videoLoaded = context->loadFile (fileOrURLPath);

	if (videoLoaded)
	{
		videoPath = fileOrURLPath;
		context->updateVideoPosition();
	}

	return videoLoaded;
}

bool DirectShowComponent::loadMovie (const File& videoFile)
{
	return loadMovie (videoFile.getFullPathName());
}

bool DirectShowComponent::loadMovie (const URL& videoURL)
{
	return loadMovie (videoURL.toString (false));
}

void DirectShowComponent::closeMovie()
{
	if (videoLoaded)
		context->release();

	videoLoaded = false;
	videoPath = String::empty;
}

File DirectShowComponent::getCurrentMoviePath() const           { return videoPath; }
bool DirectShowComponent::isMovieOpen() const                   { return videoLoaded; }
double DirectShowComponent::getMovieDuration() const            { return videoLoaded ? context->getDuration() : 0.0; }
void DirectShowComponent::setLooping (const bool shouldLoop)    { looping = shouldLoop; }
bool DirectShowComponent::isLooping() const                     { return looping; }

void DirectShowComponent::getMovieNormalSize (int &width, int &height) const
{
	width = context->getVideoWidth();
	height = context->getVideoHeight();
}

void DirectShowComponent::setBoundsWithCorrectAspectRatio (const Rectangle<int>& spaceToFitWithin,
														   const RectanglePlacement& placement)
{
	int normalWidth, normalHeight;
	getMovieNormalSize (normalWidth, normalHeight);

	const Rectangle<int> normalSize (0, 0, normalWidth, normalHeight);

	if (! (spaceToFitWithin.isEmpty() || normalSize.isEmpty()))
		setBounds (placement.appliedTo (normalSize, spaceToFitWithin));
	else
		setBounds (spaceToFitWithin);
}

void DirectShowComponent::play()
{
	if (videoLoaded)
		context->run();
}

void DirectShowComponent::stop()
{
	if (videoLoaded)
		context->pause();
}

bool DirectShowComponent::isPlaying() const
{
	return context->isRunning();
}

void DirectShowComponent::goToStart()
{
	setPosition (0.0);
}

void DirectShowComponent::setPosition (const double seconds)
{
	if (videoLoaded)
		context->setPosition (seconds);
}

double DirectShowComponent::getPosition() const
{
	return videoLoaded ? context->getPosition() : 0.0;
}

void DirectShowComponent::setSpeed (const float newSpeed)
{
	if (videoLoaded)
		context->setSpeed (newSpeed);
}

void DirectShowComponent::setMovieVolume (const float newVolume)
{
	if (videoLoaded)
		context->setVolume (newVolume);
}

float DirectShowComponent::getMovieVolume() const
{
	return videoLoaded ? context->getVolume() : 0.0f;
}

/*** End of inlined file: juce_win32_DirectShowComponent.cpp ***/


 #endif

 #if JUCE_QUICKTIME

/*** Start of inlined file: juce_win32_QuickTimeMovieComponent.cpp ***/
using namespace QTOLibrary;
using namespace QTOControlLib;

bool juce_OpenQuickTimeMovieFromStream (InputStream* input, Movie& movie, Handle& dataHandle);

static bool isQTAvailable = false;

class QuickTimeMovieComponent::Pimpl
{
public:
	Pimpl() : dataHandle (0)
	{
	}

	~Pimpl()
	{
		clearHandle();
	}

	void clearHandle()
	{
		if (dataHandle != 0)
		{
			DisposeHandle (dataHandle);
			dataHandle = 0;
		}
	}

	IQTControlPtr qtControl;
	IQTMoviePtr qtMovie;
	Handle dataHandle;
};

QuickTimeMovieComponent::QuickTimeMovieComponent()
	: movieLoaded (false),
	  controllerVisible (true)
{
	pimpl = new Pimpl();
	setMouseEventsAllowed (false);
}

QuickTimeMovieComponent::~QuickTimeMovieComponent()
{
	closeMovie();
	pimpl->qtControl = 0;
	deleteControl();
	pimpl = nullptr;
}

bool QuickTimeMovieComponent::isQuickTimeAvailable() noexcept
{
	if (! isQTAvailable)
		isQTAvailable = (InitializeQTML (0) == noErr) && (EnterMovies() == noErr);

	return isQTAvailable;
}

void QuickTimeMovieComponent::createControlIfNeeded()
{
	if (isShowing() && ! isControlCreated())
	{
		const IID qtIID = __uuidof (QTControl);

		if (createControl (&qtIID))
		{
			const IID qtInterfaceIID = __uuidof (IQTControl);
			pimpl->qtControl = (IQTControl*) queryInterface (&qtInterfaceIID);

			if (pimpl->qtControl != nullptr)
			{
				pimpl->qtControl->Release(); // it has one ref too many at this point

				pimpl->qtControl->QuickTimeInitialize();
				pimpl->qtControl->PutSizing (qtMovieFitsControl);

				if (movieFile != File::nonexistent)
					loadMovie (movieFile, controllerVisible);
			}
		}
	}
}

bool QuickTimeMovieComponent::isControlCreated() const
{
	return isControlOpen();
}

bool QuickTimeMovieComponent::loadMovie (InputStream* movieStream,
										 const bool isControllerVisible)
{
	const ScopedPointer<InputStream> movieStreamDeleter (movieStream);

	movieFile = File::nonexistent;
	movieLoaded = false;
	pimpl->qtMovie = 0;
	controllerVisible = isControllerVisible;
	createControlIfNeeded();

	if (isControlCreated())
	{
		if (pimpl->qtControl != 0)
		{
			pimpl->qtControl->Put_MovieHandle (0);
			pimpl->clearHandle();

			Movie movie;
			if (juce_OpenQuickTimeMovieFromStream (movieStream, movie, pimpl->dataHandle))
			{
				pimpl->qtControl->Put_MovieHandle ((long) (pointer_sized_int) movie);

				pimpl->qtMovie = pimpl->qtControl->GetMovie();

				if (pimpl->qtMovie != 0)
					pimpl->qtMovie->PutMovieControllerType (isControllerVisible ? qtMovieControllerTypeStandard
																				: qtMovieControllerTypeNone);
			}

			if (movie == 0)
				pimpl->clearHandle();
		}

		movieLoaded = (pimpl->qtMovie != 0);
	}
	else
	{
		// You're trying to open a movie when the control hasn't yet been created, probably because
		// you've not yet added this component to a Window and made the whole component hierarchy visible.
		jassertfalse;
	}

	return movieLoaded;
}

void QuickTimeMovieComponent::closeMovie()
{
	stop();
	movieFile = File::nonexistent;
	movieLoaded = false;
	pimpl->qtMovie = 0;

	if (pimpl->qtControl != 0)
		pimpl->qtControl->Put_MovieHandle (0);

	pimpl->clearHandle();
}

File QuickTimeMovieComponent::getCurrentMovieFile() const
{
	return movieFile;
}

bool QuickTimeMovieComponent::isMovieOpen() const
{
	return movieLoaded;
}

double QuickTimeMovieComponent::getMovieDuration() const
{
	if (pimpl->qtMovie != 0)
		return pimpl->qtMovie->GetDuration() / (double) pimpl->qtMovie->GetTimeScale();

	return 0.0;
}

void QuickTimeMovieComponent::getMovieNormalSize (int& width, int& height) const
{
	if (pimpl->qtMovie != 0)
	{
		struct QTRECT r = pimpl->qtMovie->GetNaturalRect();

		width = r.right - r.left;
		height = r.bottom - r.top;
	}
	else
	{
		width = height = 0;
	}
}

void QuickTimeMovieComponent::play()
{
	if (pimpl->qtMovie != 0)
		pimpl->qtMovie->Play();
}

void QuickTimeMovieComponent::stop()
{
	if (pimpl->qtMovie != 0)
		pimpl->qtMovie->Stop();
}

bool QuickTimeMovieComponent::isPlaying() const
{
	return pimpl->qtMovie != 0 && pimpl->qtMovie->GetRate() != 0.0f;
}

void QuickTimeMovieComponent::setPosition (const double seconds)
{
	if (pimpl->qtMovie != 0)
		pimpl->qtMovie->PutTime ((long) (seconds * pimpl->qtMovie->GetTimeScale()));
}

double QuickTimeMovieComponent::getPosition() const
{
	if (pimpl->qtMovie != 0)
		return pimpl->qtMovie->GetTime() / (double) pimpl->qtMovie->GetTimeScale();

	return 0.0;
}

void QuickTimeMovieComponent::setSpeed (const float newSpeed)
{
	if (pimpl->qtMovie != 0)
		pimpl->qtMovie->PutRate (newSpeed);
}

void QuickTimeMovieComponent::setMovieVolume (const float newVolume)
{
	if (pimpl->qtMovie != 0)
	{
		pimpl->qtMovie->PutAudioVolume (newVolume);
		pimpl->qtMovie->PutAudioMute (newVolume <= 0);
	}
}

float QuickTimeMovieComponent::getMovieVolume() const
{
	if (pimpl->qtMovie != 0)
		return pimpl->qtMovie->GetAudioVolume();

	return 0.0f;
}

void QuickTimeMovieComponent::setLooping (const bool shouldLoop)
{
	if (pimpl->qtMovie != 0)
		pimpl->qtMovie->PutLoop (shouldLoop);
}

bool QuickTimeMovieComponent::isLooping() const
{
	return pimpl->qtMovie != 0 && pimpl->qtMovie->GetLoop();
}

bool QuickTimeMovieComponent::isControllerVisible() const
{
	return controllerVisible;
}

void QuickTimeMovieComponent::parentHierarchyChanged()
{
	createControlIfNeeded();
	QTCompBaseClass::parentHierarchyChanged();
}

void QuickTimeMovieComponent::visibilityChanged()
{
	createControlIfNeeded();
	QTCompBaseClass::visibilityChanged();
}

void QuickTimeMovieComponent::paint (Graphics& g)
{
	if (! isControlCreated())
		g.fillAll (Colours::black);
}

static Handle createHandleDataRef (Handle dataHandle, const char* fileName)
{
	Handle dataRef = 0;
	OSStatus err = PtrToHand (&dataHandle, &dataRef, sizeof (Handle));
	if (err == noErr)
	{
		Str255 suffix;

	   #if JUCE_MSVC
		#pragma warning (push)
		#pragma warning (disable: 4244 4996)
	   #endif

		suffix[0] = strlen (fileName);
		strncpy ((char*) suffix + 1, fileName, 128);

	   #if JUCE_MSVC
		#pragma warning (pop)
	   #endif

		err = PtrAndHand (suffix, dataRef, suffix[0] + 1);

		if (err == noErr)
		{
			long atoms[3];
			atoms[0] = EndianU32_NtoB (3 * sizeof (long));
			atoms[1] = EndianU32_NtoB (kDataRefExtensionMacOSFileType);
			atoms[2] = EndianU32_NtoB (MovieFileType);

			err = PtrAndHand (atoms, dataRef, 3 * sizeof (long));

			if (err == noErr)
				return dataRef;
		}

		DisposeHandle (dataRef);
	}

	return 0;
}

static CFStringRef juceStringToCFString (const String& s)
{
	return CFStringCreateWithCString (kCFAllocatorDefault, s.toUTF8(), kCFStringEncodingUTF8);
}

static bool openMovie (QTNewMoviePropertyElement* props, int prop, Movie& movie)
{
	Boolean trueBool = true;
	props[prop].propClass = kQTPropertyClass_MovieInstantiation;
	props[prop].propID = kQTMovieInstantiationPropertyID_DontResolveDataRefs;
	props[prop].propValueSize = sizeof (trueBool);
	props[prop].propValueAddress = &trueBool;
	++prop;

	props[prop].propClass = kQTPropertyClass_MovieInstantiation;
	props[prop].propID = kQTMovieInstantiationPropertyID_AsyncOK;
	props[prop].propValueSize = sizeof (trueBool);
	props[prop].propValueAddress = &trueBool;
	++prop;

	Boolean isActive = true;
	props[prop].propClass = kQTPropertyClass_NewMovieProperty;
	props[prop].propID = kQTNewMoviePropertyID_Active;
	props[prop].propValueSize = sizeof (isActive);
	props[prop].propValueAddress = &isActive;
	++prop;

	MacSetPort (0);

	jassert (prop <= 5);
	OSStatus err = NewMovieFromProperties (prop, props, 0, 0, &movie);

	return err == noErr;
}

bool juce_OpenQuickTimeMovieFromStream (InputStream* input, Movie& movie, Handle& dataHandle)
{
	if (input == nullptr)
		return false;

	dataHandle = 0;
	bool ok = false;

	QTNewMoviePropertyElement props[5] = { 0 };
	int prop = 0;

	DataReferenceRecord dr;
	props[prop].propClass = kQTPropertyClass_DataLocation;
	props[prop].propID = kQTDataLocationPropertyID_DataReference;
	props[prop].propValueSize = sizeof (dr);
	props[prop].propValueAddress = &dr;
	++prop;

	FileInputStream* const fin = dynamic_cast <FileInputStream*> (input);

	if (fin != nullptr)
	{
		CFStringRef filePath = juceStringToCFString (fin->getFile().getFullPathName());

		QTNewDataReferenceFromFullPathCFString (filePath, (QTPathStyle) kQTNativeDefaultPathStyle, 0,
												&dr.dataRef, &dr.dataRefType);

		ok = openMovie (props, prop, movie);

		DisposeHandle (dr.dataRef);
		CFRelease (filePath);
	}
	else
	{
		// sanity-check because this currently needs to load the whole stream into memory..
		jassert (input->getTotalLength() < 50 * 1024 * 1024);

		dataHandle = NewHandle ((Size) input->getTotalLength());
		HLock (dataHandle);
		// read the entire stream into memory - this is a pain, but can't get it to work
		// properly using a custom callback to supply the data.
		input->read (*dataHandle, (int) input->getTotalLength());
		HUnlock (dataHandle);

		// different types to get QT to try. (We should really be a bit smarter here by
		// working out in advance which one the stream contains, rather than just trying
		// each one)
		const char* const suffixesToTry[] = { "\04.mov", "\04.mp3",
											  "\04.avi", "\04.m4a" };

		for (int i = 0; i < numElementsInArray (suffixesToTry) && ! ok; ++i)
		{
			/*  // this fails for some bizarre reason - it can be bodged to work with
				// movies, but can't seem to do it for other file types..
			QTNewMovieUserProcRecord procInfo;
			procInfo.getMovieUserProc = NewGetMovieUPP (readMovieStreamProc);
			procInfo.getMovieUserProcRefcon = this;
			procInfo.defaultDataRef.dataRef = dataRef;
			procInfo.defaultDataRef.dataRefType = HandleDataHandlerSubType;

			props[prop].propClass = kQTPropertyClass_DataLocation;
			props[prop].propID = kQTDataLocationPropertyID_MovieUserProc;
			props[prop].propValueSize = sizeof (procInfo);
			props[prop].propValueAddress = (void*) &procInfo;
			++prop; */

			dr.dataRef = createHandleDataRef (dataHandle, suffixesToTry [i]);
			dr.dataRefType = HandleDataHandlerSubType;
			ok = openMovie (props, prop, movie);

			DisposeHandle (dr.dataRef);
		}
	}

	return ok;
}

bool QuickTimeMovieComponent::loadMovie (const File& movieFile_,
										 const bool isControllerVisible)
{
	const bool ok = loadMovie (static_cast <InputStream*> (movieFile_.createInputStream()), isControllerVisible);
	movieFile = movieFile_;
	return ok;
}

bool QuickTimeMovieComponent::loadMovie (const URL& movieURL,
										 const bool isControllerVisible)
{
	return loadMovie (static_cast <InputStream*> (movieURL.createInputStream (false)), isControllerVisible);
}

void QuickTimeMovieComponent::goToStart()
{
	setPosition (0.0);
}

void QuickTimeMovieComponent::setBoundsWithCorrectAspectRatio (const Rectangle<int>& spaceToFitWithin,
															   const RectanglePlacement& placement)
{
	int normalWidth, normalHeight;
	getMovieNormalSize (normalWidth, normalHeight);

	const Rectangle<int> normalSize (0, 0, normalWidth, normalHeight);

	if (! (spaceToFitWithin.isEmpty() || normalSize.isEmpty()))
		setBounds (placement.appliedTo (normalSize, spaceToFitWithin));
	else
		setBounds (spaceToFitWithin);
}

/*** End of inlined file: juce_win32_QuickTimeMovieComponent.cpp ***/


 #endif

#elif JUCE_LINUX

#elif JUCE_ANDROID
 #if JUCE_USE_CAMERA

/*** Start of inlined file: juce_android_CameraDevice.cpp ***/
// TODO
class AndroidCameraInternal
{
public:
	AndroidCameraInternal()
	{
	}

	~AndroidCameraInternal()
	{
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndroidCameraInternal);
};

CameraDevice::CameraDevice (const String& name_, int /*index*/)
	: name (name_)
{
	internal = new AndroidCameraInternal();

	// TODO
}

CameraDevice::~CameraDevice()
{
	stopRecording();
	delete static_cast <AndroidCameraInternal*> (internal);
	internal = 0;
}

Component* CameraDevice::createViewerComponent()
{
	// TODO

	return nullptr;
}

String CameraDevice::getFileExtension()
{
	return ".m4a";  // TODO correct?
}

void CameraDevice::startRecordingToFile (const File& file, int quality)
{
	// TODO
}

Time CameraDevice::getTimeOfFirstRecordedFrame() const
{
	// TODO
	return Time();
}

void CameraDevice::stopRecording()
{
	// TODO
}

void CameraDevice::addListener (Listener* listenerToAdd)
{
	// TODO
}

void CameraDevice::removeListener (Listener* listenerToRemove)
{
	// TODO
}

StringArray CameraDevice::getAvailableDevices()
{
	StringArray devs;

	// TODO

	return devs;
}

CameraDevice* CameraDevice::openDevice (int index,
										int minWidth, int minHeight,
										int maxWidth, int maxHeight)
{
	// TODO

	return nullptr;
}

/*** End of inlined file: juce_android_CameraDevice.cpp ***/


 #endif
#endif

}

/*** End of inlined file: juce_video.cpp ***/


#endif

