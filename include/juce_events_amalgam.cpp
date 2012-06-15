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

/*** Start of inlined file: juce_events.cpp ***/
#if defined (__JUCE_EVENTS_JUCEHEADER__) && ! JUCE_AMALGAMATED_INCLUDE
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

#include "juce_events_amalgam.h"

#if JUCE_CATCH_UNHANDLED_EXCEPTIONS && JUCE_MODULE_AVAILABLE_juce_gui_basics
#include "juce_gui_basics_amalgam.h"
#endif

#if JUCE_MAC
 #import <IOKit/IOKitLib.h>
 #import <IOKit/IOCFPlugIn.h>
 #import <IOKit/hid/IOHIDLib.h>
 #import <IOKit/hid/IOHIDKeys.h>
 #import <IOKit/pwr_mgt/IOPMLib.h>

#elif JUCE_LINUX
 #include <X11/Xlib.h>
 #include <X11/Xresource.h>
 #include <X11/Xutil.h>
 #undef KeyPress
 #include <unistd.h>
#endif

namespace juce
{

// START_AUTOINCLUDE messages/*.cpp, broadcasters/*.cpp,
// timers/*.cpp, interprocess/*.cpp

/*** Start of inlined file: juce_ApplicationBase.cpp ***/
JUCEApplicationBase::CreateInstanceFunction JUCEApplicationBase::createInstance = 0;
JUCEApplicationBase* JUCEApplicationBase::appInstance = nullptr;

JUCEApplicationBase::JUCEApplicationBase()
{
	jassert (isStandaloneApp() && appInstance == nullptr);
	appInstance = this;
}

JUCEApplicationBase::~JUCEApplicationBase()
{
	jassert (appInstance == this);
	appInstance = nullptr;
}

// This is called on the Mac and iOS where the OS doesn't allow the stack to unwind on shutdown..
void JUCEApplicationBase::appWillTerminateByForce()
{
	JUCE_AUTORELEASEPOOL

	{
		const ScopedPointer<JUCEApplicationBase> app (appInstance);

		if (app != nullptr)
			app->shutdownApp();
	}

	DeletedAtShutdown::deleteAll();
	MessageManager::deleteInstance();
}

/*** End of inlined file: juce_ApplicationBase.cpp ***/



/*** Start of inlined file: juce_DeletedAtShutdown.cpp ***/
static SpinLock deletedAtShutdownLock;

DeletedAtShutdown::DeletedAtShutdown()
{
	const SpinLock::ScopedLockType sl (deletedAtShutdownLock);
	getObjects().add (this);
}

DeletedAtShutdown::~DeletedAtShutdown()
{
	const SpinLock::ScopedLockType sl (deletedAtShutdownLock);
	getObjects().removeValue (this);
}

void DeletedAtShutdown::deleteAll()
{
	// make a local copy of the array, so it can't get into a loop if something
	// creates another DeletedAtShutdown object during its destructor.
	Array <DeletedAtShutdown*> localCopy;

	{
		const SpinLock::ScopedLockType sl (deletedAtShutdownLock);
		localCopy = getObjects();
	}

	for (int i = localCopy.size(); --i >= 0;)
	{
		JUCE_TRY
		{
			DeletedAtShutdown* deletee = localCopy.getUnchecked(i);

			// double-check that it's not already been deleted during another object's destructor.
			{
				const SpinLock::ScopedLockType sl (deletedAtShutdownLock);
				if (! getObjects().contains (deletee))
					deletee = nullptr;
			}

			delete deletee;
		}
		JUCE_CATCH_EXCEPTION
	}

	// if no objects got re-created during shutdown, this should have been emptied by their
	// destructors
	jassert (getObjects().size() == 0);

	getObjects().clear(); // just to make sure the array doesn't have any memory still allocated
}

Array <DeletedAtShutdown*>& DeletedAtShutdown::getObjects()
{
	static Array <DeletedAtShutdown*> objects;
	return objects;
}

/*** End of inlined file: juce_DeletedAtShutdown.cpp ***/


/*** Start of inlined file: juce_MessageListener.cpp ***/
Message::Message() noexcept {}
Message::~Message() {}

void Message::messageCallback()
{
	MessageListener* const r = recipient;
	if (r != nullptr)
		r->handleMessage (*this);
}

MessageListener::MessageListener() noexcept
{
	// Are you trying to create a messagelistener before or after juce has been intialised??
	jassert (MessageManager::getInstanceWithoutCreating() != nullptr);
}

MessageListener::~MessageListener()
{
	masterReference.clear();
}

void MessageListener::postMessage (Message* const message) const
{
	message->recipient = const_cast <MessageListener*> (this);
	message->post();
}

/*** End of inlined file: juce_MessageListener.cpp ***/


/*** Start of inlined file: juce_MessageManager.cpp ***/
class MessageManager::QuitMessage   : public MessageManager::MessageBase
{
public:
	QuitMessage() {}

	void messageCallback()
	{
		MessageManager* const mm = MessageManager::instance;
		if (mm != nullptr)
			mm->quitMessageReceived = true;
	}

	JUCE_DECLARE_NON_COPYABLE (QuitMessage);
};

MessageManager::MessageManager() noexcept
  : quitMessagePosted (false),
	quitMessageReceived (false),
	messageThreadId (Thread::getCurrentThreadId()),
	threadWithLock (0)
{
	if (JUCEApplicationBase::isStandaloneApp())
		Thread::setCurrentThreadName ("Juce Message Thread");
}

MessageManager::~MessageManager() noexcept
{
	broadcaster = nullptr;

	doPlatformSpecificShutdown();

	jassert (instance == this);
	instance = nullptr;  // do this last in case this instance is still needed by doPlatformSpecificShutdown()
}

MessageManager* MessageManager::instance = nullptr;

MessageManager* MessageManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new MessageManager();
		doPlatformSpecificInitialisation();
	}

	return instance;
}

inline MessageManager* MessageManager::getInstanceWithoutCreating() noexcept
{
	return instance;
}

void MessageManager::deleteInstance()
{
	deleteAndZero (instance);
}

void MessageManager::MessageBase::post()
{
	MessageManager* const mm = MessageManager::instance;

	if (mm == nullptr || mm->quitMessagePosted || ! postMessageToSystemQueue (this))
		Ptr deleter (this); // (this will delete messages that were just created with a 0 ref count)
}

#if JUCE_MODAL_LOOPS_PERMITTED && ! (JUCE_MAC || JUCE_IOS)
void MessageManager::runDispatchLoop()
{
	jassert (isThisTheMessageThread()); // must only be called by the message thread

	runDispatchLoopUntil (-1);
}

void MessageManager::stopDispatchLoop()
{
	(new QuitMessage())->post();
	quitMessagePosted = true;
}

bool MessageManager::runDispatchLoopUntil (int millisecondsToRunFor)
{
	jassert (isThisTheMessageThread()); // must only be called by the message thread

	const int64 endTime = Time::currentTimeMillis() + millisecondsToRunFor;

	while ((millisecondsToRunFor < 0 || endTime > Time::currentTimeMillis())
			&& ! quitMessageReceived)
	{
		JUCE_TRY
		{
			if (! dispatchNextMessageOnSystemQueue (millisecondsToRunFor >= 0))
			{
				const int msToWait = (int) (endTime - Time::currentTimeMillis());

				if (msToWait > 0)
					Thread::sleep (jmin (5, msToWait));
			}
		}
		JUCE_CATCH_EXCEPTION
	}

	return ! quitMessageReceived;
}

#endif

class AsyncFunctionCallback   : public MessageManager::MessageBase
{
public:
	AsyncFunctionCallback (MessageCallbackFunction* const f, void* const param)
		: result (nullptr), func (f), parameter (param)
	{}

	void messageCallback()
	{
		result = (*func) (parameter);
		finished.signal();
	}

	WaitableEvent finished;
	void* volatile result;

private:
	MessageCallbackFunction* const func;
	void* const parameter;

	JUCE_DECLARE_NON_COPYABLE (AsyncFunctionCallback);
};

void* MessageManager::callFunctionOnMessageThread (MessageCallbackFunction* const func, void* const parameter)
{
	if (isThisTheMessageThread())
		return func (parameter);

	// If this thread has the message manager locked, then this will deadlock!
	jassert (! currentThreadHasLockedMessageManager());

	const ReferenceCountedObjectPtr<AsyncFunctionCallback> message (new AsyncFunctionCallback (func, parameter));
	message->post();
	message->finished.wait();
	return message->result;
}

void MessageManager::deliverBroadcastMessage (const String& value)
{
	if (broadcaster != nullptr)
		broadcaster->sendActionMessage (value);
}

void MessageManager::registerBroadcastListener (ActionListener* const listener)
{
	if (broadcaster == nullptr)
		broadcaster = new ActionBroadcaster();

	broadcaster->addActionListener (listener);
}

void MessageManager::deregisterBroadcastListener (ActionListener* const listener)
{
	if (broadcaster != nullptr)
		broadcaster->removeActionListener (listener);
}

bool MessageManager::isThisTheMessageThread() const noexcept
{
	return Thread::getCurrentThreadId() == messageThreadId;
}

void MessageManager::setCurrentThreadAsMessageThread()
{
	const Thread::ThreadID thisThread = Thread::getCurrentThreadId();

	if (messageThreadId != thisThread)
	{
		messageThreadId = thisThread;

		// This is needed on windows to make sure the message window is created by this thread
		doPlatformSpecificShutdown();
		doPlatformSpecificInitialisation();
	}
}

bool MessageManager::currentThreadHasLockedMessageManager() const noexcept
{
	const Thread::ThreadID thisThread = Thread::getCurrentThreadId();
	return thisThread == messageThreadId || thisThread == threadWithLock;
}

/*  The only safe way to lock the message thread while another thread does
	some work is by posting a special message, whose purpose is to tie up the event
	loop until the other thread has finished its business.

	Any other approach can get horribly deadlocked if the OS uses its own hidden locks which
	get locked before making an event callback, because if the same OS lock gets indirectly
	accessed from another thread inside a MM lock, you're screwed. (this is exactly what happens
	in Cocoa).
*/
class MessageManagerLock::BlockingMessage   : public MessageManager::MessageBase
{
public:
	BlockingMessage() noexcept {}

	void messageCallback()
	{
		lockedEvent.signal();
		releaseEvent.wait();
	}

	WaitableEvent lockedEvent, releaseEvent;

	JUCE_DECLARE_NON_COPYABLE (BlockingMessage);
};

MessageManagerLock::MessageManagerLock (Thread* const threadToCheck)
	: blockingMessage(), locked (attemptLock (threadToCheck, nullptr))
{
}

MessageManagerLock::MessageManagerLock (ThreadPoolJob* const jobToCheckForExitSignal)
	: blockingMessage(), locked (attemptLock (nullptr, jobToCheckForExitSignal))
{
}

bool MessageManagerLock::attemptLock (Thread* const threadToCheck, ThreadPoolJob* const job)
{
	MessageManager* const mm = MessageManager::instance;

	if (mm == nullptr)
		return false;

	if (mm->currentThreadHasLockedMessageManager())
		return true;

	if (threadToCheck == nullptr && job == nullptr)
	{
		mm->lockingLock.enter();
	}
	else
	{
		while (! mm->lockingLock.tryEnter())
		{
			if ((threadToCheck != nullptr && threadToCheck->threadShouldExit())
				  || (job != nullptr && job->shouldExit()))
				return false;

			Thread::yield();
		}
	}

	blockingMessage = new BlockingMessage();
	blockingMessage->post();

	while (! blockingMessage->lockedEvent.wait (20))
	{
		if ((threadToCheck != nullptr && threadToCheck->threadShouldExit())
			  || (job != nullptr && job->shouldExit()))
		{
			blockingMessage->releaseEvent.signal();
			blockingMessage = nullptr;
			mm->lockingLock.exit();
			return false;
		}
	}

	jassert (mm->threadWithLock == 0);

	mm->threadWithLock = Thread::getCurrentThreadId();
	return true;
}

MessageManagerLock::~MessageManagerLock() noexcept
{
	if (blockingMessage != nullptr)
	{
		MessageManager* const mm = MessageManager::instance;

		jassert (mm == nullptr || mm->currentThreadHasLockedMessageManager());

		blockingMessage->releaseEvent.signal();
		blockingMessage = nullptr;

		if (mm != nullptr)
		{
			mm->threadWithLock = 0;
			mm->lockingLock.exit();
		}
	}
}

JUCE_API void JUCE_CALLTYPE initialiseJuce_GUI();
JUCE_API void JUCE_CALLTYPE initialiseJuce_GUI()
{
	JUCE_AUTORELEASEPOOL
	MessageManager::getInstance();
}

JUCE_API void JUCE_CALLTYPE shutdownJuce_GUI();
JUCE_API void JUCE_CALLTYPE shutdownJuce_GUI()
{
	JUCE_AUTORELEASEPOOL
	DeletedAtShutdown::deleteAll();
	MessageManager::deleteInstance();
}

/*** End of inlined file: juce_MessageManager.cpp ***/


/*** Start of inlined file: juce_ActionBroadcaster.cpp ***/
class ActionBroadcaster::ActionMessage  : public MessageManager::MessageBase
{
public:
	ActionMessage (const ActionBroadcaster* const broadcaster_,
				   const String& messageText,
				   ActionListener* const listener_) noexcept
		: broadcaster (const_cast <ActionBroadcaster*> (broadcaster_)),
		  message (messageText),
		  listener (listener_)
	{}

	void messageCallback()
	{
		const ActionBroadcaster* const b = broadcaster;

		if (b != nullptr && b->actionListeners.contains (listener))
			listener->actionListenerCallback (message);
	}

private:
	WeakReference<ActionBroadcaster> broadcaster;
	const String message;
	ActionListener* const listener;

	JUCE_DECLARE_NON_COPYABLE (ActionMessage);
};

ActionBroadcaster::ActionBroadcaster()
{
	// are you trying to create this object before or after juce has been intialised??
	jassert (MessageManager::getInstanceWithoutCreating() != nullptr);
}

ActionBroadcaster::~ActionBroadcaster()
{
	// all event-based objects must be deleted BEFORE juce is shut down!
	jassert (MessageManager::getInstanceWithoutCreating() != nullptr);

	masterReference.clear();
}

void ActionBroadcaster::addActionListener (ActionListener* const listener)
{
	const ScopedLock sl (actionListenerLock);

	if (listener != nullptr)
		actionListeners.add (listener);
}

void ActionBroadcaster::removeActionListener (ActionListener* const listener)
{
	const ScopedLock sl (actionListenerLock);
	actionListeners.removeValue (listener);
}

void ActionBroadcaster::removeAllActionListeners()
{
	const ScopedLock sl (actionListenerLock);
	actionListeners.clear();
}

void ActionBroadcaster::sendActionMessage (const String& message) const
{
	const ScopedLock sl (actionListenerLock);

	for (int i = actionListeners.size(); --i >= 0;)
		(new ActionMessage (this, message, actionListeners.getUnchecked(i)))->post();
}

/*** End of inlined file: juce_ActionBroadcaster.cpp ***/


/*** Start of inlined file: juce_AsyncUpdater.cpp ***/
class AsyncUpdater::AsyncUpdaterMessage  : public CallbackMessage
{
public:
	AsyncUpdaterMessage (AsyncUpdater& owner_)
		: owner (owner_)
	{
	}

	void messageCallback()
	{
		if (shouldDeliver.compareAndSetBool (0, 1))
			owner.handleAsyncUpdate();
	}

	Atomic<int> shouldDeliver;

private:
	AsyncUpdater& owner;

	JUCE_DECLARE_NON_COPYABLE (AsyncUpdaterMessage);
};

AsyncUpdater::AsyncUpdater()
{
	message = new AsyncUpdaterMessage (*this);
}

AsyncUpdater::~AsyncUpdater()
{
	// You're deleting this object with a background thread while there's an update
	// pending on the main event thread - that's pretty dodgy threading, as the callback could
	// happen after this destructor has finished. You should either use a MessageManagerLock while
	// deleting this object, or find some other way to avoid such a race condition.
	jassert ((! isUpdatePending()) || MessageManager::getInstance()->currentThreadHasLockedMessageManager());

	message->shouldDeliver.set (0);
}

void AsyncUpdater::triggerAsyncUpdate()
{
	if (message->shouldDeliver.compareAndSetBool (1, 0))
		message->post();
}

void AsyncUpdater::cancelPendingUpdate() noexcept
{
	message->shouldDeliver.set (0);
}

void AsyncUpdater::handleUpdateNowIfNeeded()
{
	// This can only be called by the event thread.
	jassert (MessageManager::getInstance()->currentThreadHasLockedMessageManager());

	if (message->shouldDeliver.exchange (0) != 0)
		handleAsyncUpdate();
}

bool AsyncUpdater::isUpdatePending() const noexcept
{
	return message->shouldDeliver.value != 0;
}

/*** End of inlined file: juce_AsyncUpdater.cpp ***/


/*** Start of inlined file: juce_ChangeBroadcaster.cpp ***/
ChangeBroadcaster::ChangeBroadcaster() noexcept
{
	callback.owner = this;
}

ChangeBroadcaster::~ChangeBroadcaster()
{
}

void ChangeBroadcaster::addChangeListener (ChangeListener* const listener)
{
	// Listeners can only be safely added when the event thread is locked
	// You can  use a MessageManagerLock if you need to call this from another thread.
	jassert (MessageManager::getInstance()->currentThreadHasLockedMessageManager());

	changeListeners.add (listener);
}

void ChangeBroadcaster::removeChangeListener (ChangeListener* const listener)
{
	// Listeners can only be safely added when the event thread is locked
	// You can  use a MessageManagerLock if you need to call this from another thread.
	jassert (MessageManager::getInstance()->currentThreadHasLockedMessageManager());

	changeListeners.remove (listener);
}

void ChangeBroadcaster::removeAllChangeListeners()
{
	// Listeners can only be safely added when the event thread is locked
	// You can  use a MessageManagerLock if you need to call this from another thread.
	jassert (MessageManager::getInstance()->currentThreadHasLockedMessageManager());

	changeListeners.clear();
}

void ChangeBroadcaster::sendChangeMessage()
{
	if (changeListeners.size() > 0)
		callback.triggerAsyncUpdate();
}

void ChangeBroadcaster::sendSynchronousChangeMessage()
{
	// This can only be called by the event thread.
	jassert (MessageManager::getInstance()->isThisTheMessageThread());

	callback.cancelPendingUpdate();
	callListeners();
}

void ChangeBroadcaster::dispatchPendingMessages()
{
	callback.handleUpdateNowIfNeeded();
}

void ChangeBroadcaster::callListeners()
{
	changeListeners.call (&ChangeListener::changeListenerCallback, this);
}

ChangeBroadcaster::ChangeBroadcasterCallback::ChangeBroadcasterCallback()
	: owner (nullptr)
{
}

void ChangeBroadcaster::ChangeBroadcasterCallback::handleAsyncUpdate()
{
	jassert (owner != nullptr);
	owner->callListeners();
}

/*** End of inlined file: juce_ChangeBroadcaster.cpp ***/


/*** Start of inlined file: juce_MultiTimer.cpp ***/
class MultiTimer::MultiTimerCallback    : public Timer
{
public:
	MultiTimerCallback (const int timerId_, MultiTimer& owner_)
		: timerId (timerId_),
		  owner (owner_)
	{
	}

	void timerCallback()
	{
		owner.timerCallback (timerId);
	}

	const int timerId;

private:
	MultiTimer& owner;
};

MultiTimer::MultiTimer() noexcept
{
}

MultiTimer::MultiTimer (const MultiTimer&) noexcept
{
}

MultiTimer::~MultiTimer()
{
	const SpinLock::ScopedLockType sl (timerListLock);
	timers.clear();
}

void MultiTimer::startTimer (const int timerId, const int intervalInMilliseconds) noexcept
{
	const SpinLock::ScopedLockType sl (timerListLock);

	for (int i = timers.size(); --i >= 0;)
	{
		MultiTimerCallback* const t = timers.getUnchecked(i);

		if (t->timerId == timerId)
		{
			t->startTimer (intervalInMilliseconds);
			return;
		}
	}

	MultiTimerCallback* const newTimer = new MultiTimerCallback (timerId, *this);
	timers.add (newTimer);
	newTimer->startTimer (intervalInMilliseconds);
}

void MultiTimer::stopTimer (const int timerId) noexcept
{
	const SpinLock::ScopedLockType sl (timerListLock);

	for (int i = timers.size(); --i >= 0;)
	{
		MultiTimerCallback* const t = timers.getUnchecked(i);

		if (t->timerId == timerId)
			t->stopTimer();
	}
}

bool MultiTimer::isTimerRunning (const int timerId) const noexcept
{
	const SpinLock::ScopedLockType sl (timerListLock);

	for (int i = timers.size(); --i >= 0;)
	{
		const MultiTimerCallback* const t = timers.getUnchecked(i);
		if (t->timerId == timerId)
			return t->isTimerRunning();
	}

	return false;
}

int MultiTimer::getTimerInterval (const int timerId) const noexcept
{
	const SpinLock::ScopedLockType sl (timerListLock);

	for (int i = timers.size(); --i >= 0;)
	{
		const MultiTimerCallback* const t = timers.getUnchecked(i);
		if (t->timerId == timerId)
			return t->getTimerInterval();
	}

	return 0;
}

/*** End of inlined file: juce_MultiTimer.cpp ***/


/*** Start of inlined file: juce_Timer.cpp ***/
class Timer::TimerThread  : private Thread,
							private DeletedAtShutdown,
							private AsyncUpdater
{
public:
	typedef CriticalSection LockType; // (mysteriously, using a SpinLock here causes problems on some XP machines..)

	TimerThread()
		: Thread ("Juce Timer"),
		  firstTimer (nullptr),
		  callbackNeeded (0)
	{
		triggerAsyncUpdate();
	}

	~TimerThread() noexcept
	{
		stopThread (4000);

		jassert (instance == this || instance == nullptr);
		if (instance == this)
			instance = nullptr;
	}

	void run()
	{
		uint32 lastTime = Time::getMillisecondCounter();
		MessageManager::MessageBase::Ptr messageToSend (new CallTimersMessage());

		while (! threadShouldExit())
		{
			const uint32 now = Time::getMillisecondCounter();

			if (now == lastTime)
			{
				wait (1);
				continue;
			}

			const int elapsed = (int) (now >= lastTime ? (now - lastTime)
													   : (std::numeric_limits<uint32>::max() - (lastTime - now)));
			lastTime = now;

			const int timeUntilFirstTimer = getTimeUntilFirstTimer (elapsed);

			if (timeUntilFirstTimer <= 0)
			{
				/* If we managed to set the atomic boolean to true then send a message, this is needed
				   as a memory barrier so the message won't be sent before callbackNeeded is set to true,
				   but if it fails it means the message-thread changed the value from under us so at least
				   some processing is happenening and we can just loop around and try again
				*/
				if (callbackNeeded.compareAndSetBool (1, 0))
				{
					messageToSend->post();

					/* Sometimes our message can get discarded by the OS (e.g. when running as an RTAS
					   when the app has a modal loop), so this is how long to wait before assuming the
					   message has been lost and trying again.
					*/
					const uint32 messageDeliveryTimeout = now + 2000;

					while (callbackNeeded.get() != 0)
					{
						wait (4);

						if (threadShouldExit())
							return;

						if (Time::getMillisecondCounter() > messageDeliveryTimeout)
							break;
					}
				}
			}
			else
			{
				// don't wait for too long because running this loop also helps keep the
				// Time::getApproximateMillisecondTimer value stay up-to-date
				wait (jlimit (1, 50, timeUntilFirstTimer));
			}
		}
	}

	void callTimers()
	{
		const LockType::ScopedLockType sl (lock);

		while (firstTimer != nullptr && firstTimer->countdownMs <= 0)
		{
			Timer* const t = firstTimer;
			t->countdownMs = t->periodMs;

			removeTimer (t);
			addTimer (t);

			const LockType::ScopedUnlockType ul (lock);

			JUCE_TRY
			{
				t->timerCallback();
			}
			JUCE_CATCH_EXCEPTION
		}

		/* This is needed as a memory barrier to make sure all processing of current timers is done
		   before the boolean is set. This set should never fail since if it was false in the first place,
		   we wouldn't get a message (so it can't be changed from false to true from under us), and if we
		   get a message then the value is true and the other thread can only set it to true again and
		   we will get another callback to set it to false.
		*/
		callbackNeeded.set (0);
	}

	void callTimersSynchronously()
	{
		if (! isThreadRunning())
		{
			// (This is relied on by some plugins in cases where the MM has
			// had to restart and the async callback never started)
			cancelPendingUpdate();
			triggerAsyncUpdate();
		}

		callTimers();
	}

	static inline void add (Timer* const tim) noexcept
	{
		if (instance == nullptr)
			instance = new TimerThread();

		instance->addTimer (tim);
	}

	static inline void remove (Timer* const tim) noexcept
	{
		if (instance != nullptr)
			instance->removeTimer (tim);
	}

	static inline void resetCounter (Timer* const tim, const int newCounter) noexcept
	{
		if (instance != nullptr)
		{
			tim->countdownMs = newCounter;
			tim->periodMs = newCounter;

			if ((tim->next != nullptr && tim->next->countdownMs < tim->countdownMs)
				 || (tim->previous != nullptr && tim->previous->countdownMs > tim->countdownMs))
			{
				instance->removeTimer (tim);
				instance->addTimer (tim);
			}
		}
	}

	static TimerThread* instance;
	static LockType lock;

private:
	Timer* volatile firstTimer;
	Atomic <int> callbackNeeded;

	struct CallTimersMessage  : public MessageManager::MessageBase
	{
		CallTimersMessage() {}

		void messageCallback()
		{
			if (instance != nullptr)
				instance->callTimers();
		}
	};

	void addTimer (Timer* const t) noexcept
	{
	   #if JUCE_DEBUG
		Timer* tt = firstTimer;

		while (tt != nullptr)
		{
			// trying to add a timer that's already here - shouldn't get to this point,
			// so if you get this assertion, let me know!
			jassert (tt != t);

			tt = tt->next;
		}

		jassert (t->previous == nullptr && t->next == nullptr);
	   #endif

		Timer* i = firstTimer;

		if (i == nullptr || i->countdownMs > t->countdownMs)
		{
			t->next = firstTimer;
			firstTimer = t;
		}
		else
		{
			while (i->next != nullptr && i->next->countdownMs <= t->countdownMs)
				i = i->next;

			jassert (i != nullptr);

			t->next = i->next;
			t->previous = i;
			i->next = t;
		}

		if (t->next != nullptr)
			t->next->previous = t;

		jassert ((t->next == nullptr || t->next->countdownMs >= t->countdownMs)
				  && (t->previous == nullptr || t->previous->countdownMs <= t->countdownMs));

		notify();
	}

	void removeTimer (Timer* const t) noexcept
	{
	   #if JUCE_DEBUG
		Timer* tt = firstTimer;
		bool found = false;

		while (tt != nullptr)
		{
			if (tt == t)
			{
				found = true;
				break;
			}

			tt = tt->next;
		}

		// trying to remove a timer that's not here - shouldn't get to this point,
		// so if you get this assertion, let me know!
		jassert (found);
	   #endif

		if (t->previous != nullptr)
		{
			jassert (firstTimer != t);
			t->previous->next = t->next;
		}
		else
		{
			jassert (firstTimer == t);
			firstTimer = t->next;
		}

		if (t->next != nullptr)
			t->next->previous = t->previous;

		t->next = nullptr;
		t->previous = nullptr;
	}

	int getTimeUntilFirstTimer (const int numMillisecsElapsed) const
	{
		const LockType::ScopedLockType sl (lock);

		for (Timer* t = firstTimer; t != nullptr; t = t->next)
			t->countdownMs -= numMillisecsElapsed;

		return firstTimer != nullptr ? firstTimer->countdownMs : 1000;
	}

	void handleAsyncUpdate()
	{
		startThread (7);
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimerThread);
};

Timer::TimerThread* Timer::TimerThread::instance = nullptr;
Timer::TimerThread::LockType Timer::TimerThread::lock;

#if JUCE_DEBUG
static SortedSet <Timer*> activeTimers;
#endif

Timer::Timer() noexcept
   : countdownMs (0),
	 periodMs (0),
	 previous (nullptr),
	 next (nullptr)
{
   #if JUCE_DEBUG
	const TimerThread::LockType::ScopedLockType sl (TimerThread::lock);
	activeTimers.add (this);
   #endif
}

Timer::Timer (const Timer&) noexcept
   : countdownMs (0),
	 periodMs (0),
	 previous (nullptr),
	 next (nullptr)
{
   #if JUCE_DEBUG
	const TimerThread::LockType::ScopedLockType sl (TimerThread::lock);
	activeTimers.add (this);
   #endif
}

Timer::~Timer()
{
	stopTimer();

   #if JUCE_DEBUG
	activeTimers.removeValue (this);
   #endif
}

void Timer::startTimer (const int interval) noexcept
{
	const TimerThread::LockType::ScopedLockType sl (TimerThread::lock);

   #if JUCE_DEBUG
	// this isn't a valid object! Your timer might be a dangling pointer or something..
	jassert (activeTimers.contains (this));
   #endif

	if (periodMs == 0)
	{
		countdownMs = interval;
		periodMs = jmax (1, interval);
		TimerThread::add (this);
	}
	else
	{
		TimerThread::resetCounter (this, interval);
	}
}

void Timer::stopTimer() noexcept
{
	const TimerThread::LockType::ScopedLockType sl (TimerThread::lock);

   #if JUCE_DEBUG
	// this isn't a valid object! Your timer might be a dangling pointer or something..
	jassert (activeTimers.contains (this));
   #endif

	if (periodMs > 0)
	{
		TimerThread::remove (this);
		periodMs = 0;
	}
}

void JUCE_CALLTYPE Timer::callPendingTimersSynchronously()
{
	if (TimerThread::instance != nullptr)
		TimerThread::instance->callTimersSynchronously();
}

/*** End of inlined file: juce_Timer.cpp ***/


/*** Start of inlined file: juce_InterprocessConnection.cpp ***/
InterprocessConnection::InterprocessConnection (const bool callbacksOnMessageThread,
												const uint32 magicMessageHeaderNumber)
	: Thread ("Juce IPC connection"),
	  callbackConnectionState (false),
	  useMessageThread (callbacksOnMessageThread),
	  magicMessageHeader (magicMessageHeaderNumber),
	  pipeReceiveMessageTimeout (-1)
{
}

InterprocessConnection::~InterprocessConnection()
{
	callbackConnectionState = false;
	disconnect();
	masterReference.clear();
}

bool InterprocessConnection::connectToSocket (const String& hostName,
											  const int portNumber,
											  const int timeOutMillisecs)
{
	disconnect();

	const ScopedLock sl (pipeAndSocketLock);
	socket = new StreamingSocket();

	if (socket->connect (hostName, portNumber, timeOutMillisecs))
	{
		connectionMadeInt();
		startThread();
		return true;
	}
	else
	{
		socket = nullptr;
		return false;
	}
}

bool InterprocessConnection::connectToPipe (const String& pipeName,
											const int pipeReceiveMessageTimeoutMs)
{
	disconnect();

	ScopedPointer <NamedPipe> newPipe (new NamedPipe());

	if (newPipe->openExisting (pipeName))
	{
		const ScopedLock sl (pipeAndSocketLock);
		pipeReceiveMessageTimeout = pipeReceiveMessageTimeoutMs;
		initialiseWithPipe (newPipe.release());
		return true;
	}

	return false;
}

bool InterprocessConnection::createPipe (const String& pipeName,
										 const int pipeReceiveMessageTimeoutMs)
{
	disconnect();

	ScopedPointer <NamedPipe> newPipe (new NamedPipe());

	if (newPipe->createNewPipe (pipeName))
	{
		const ScopedLock sl (pipeAndSocketLock);
		pipeReceiveMessageTimeout = pipeReceiveMessageTimeoutMs;
		initialiseWithPipe (newPipe.release());
		return true;
	}

	return false;
}

void InterprocessConnection::disconnect()
{
	if (socket != nullptr)
		socket->close();

	if (pipe != nullptr)
		pipe->close();

	stopThread (4000);

	{
		const ScopedLock sl (pipeAndSocketLock);
		socket = nullptr;
		pipe = nullptr;
	}

	connectionLostInt();
}

bool InterprocessConnection::isConnected() const
{
	const ScopedLock sl (pipeAndSocketLock);

	return ((socket != nullptr && socket->isConnected())
			  || (pipe != nullptr && pipe->isOpen()))
			&& isThreadRunning();
}

String InterprocessConnection::getConnectedHostName() const
{
	if (pipe != nullptr)
	{
		return "localhost";
	}
	else if (socket != nullptr)
	{
		if (! socket->isLocal())
			return socket->getHostName();

		return "localhost";
	}

	return String::empty;
}

bool InterprocessConnection::sendMessage (const MemoryBlock& message)
{
	uint32 messageHeader[2];
	messageHeader [0] = ByteOrder::swapIfBigEndian (magicMessageHeader);
	messageHeader [1] = ByteOrder::swapIfBigEndian ((uint32) message.getSize());

	MemoryBlock messageData (sizeof (messageHeader) + message.getSize());
	messageData.copyFrom (messageHeader, 0, sizeof (messageHeader));
	messageData.copyFrom (message.getData(), sizeof (messageHeader), message.getSize());

	int bytesWritten = 0;

	const ScopedLock sl (pipeAndSocketLock);

	if (socket != nullptr)
		bytesWritten = socket->write (messageData.getData(), (int) messageData.getSize());
	else if (pipe != nullptr)
		bytesWritten = pipe->write (messageData.getData(), (int) messageData.getSize());

	return bytesWritten == (int) messageData.getSize();
}

void InterprocessConnection::initialiseWithSocket (StreamingSocket* const socket_)
{
	jassert (socket == 0);
	socket = socket_;
	connectionMadeInt();
	startThread();
}

void InterprocessConnection::initialiseWithPipe (NamedPipe* const pipe_)
{
	jassert (pipe == 0);
	pipe = pipe_;
	connectionMadeInt();
	startThread();
}

struct ConnectionStateMessage  : public MessageManager::MessageBase
{
	ConnectionStateMessage (InterprocessConnection* owner_, bool connectionMade_) noexcept
		: owner (owner_), connectionMade (connectionMade_)
	{}

	void messageCallback()
	{
		InterprocessConnection* const ipc = owner;
		if (ipc != nullptr)
		{
			if (connectionMade)
				ipc->connectionMade();
			else
				ipc->connectionLost();
		}
	}

	WeakReference<InterprocessConnection> owner;
	bool connectionMade;
};

void InterprocessConnection::connectionMadeInt()
{
	if (! callbackConnectionState)
	{
		callbackConnectionState = true;

		if (useMessageThread)
			(new ConnectionStateMessage (this, true))->post();
		else
			connectionMade();
	}
}

void InterprocessConnection::connectionLostInt()
{
	if (callbackConnectionState)
	{
		callbackConnectionState = false;

		if (useMessageThread)
			(new ConnectionStateMessage (this, false))->post();
		else
			connectionLost();
	}
}

struct DataDeliveryMessage  : public Message
{
	DataDeliveryMessage (InterprocessConnection* owner_, const MemoryBlock& data_)
		: owner (owner_), data (data_)
	{}

	void messageCallback()
	{
		InterprocessConnection* const ipc = owner;
		if (ipc != nullptr)
			ipc->messageReceived (data);
	}

	WeakReference<InterprocessConnection> owner;
	MemoryBlock data;
};

void InterprocessConnection::deliverDataInt (const MemoryBlock& data)
{
	jassert (callbackConnectionState);

	if (useMessageThread)
		(new DataDeliveryMessage (this, data))->post();
	else
		messageReceived (data);
}

bool InterprocessConnection::readNextMessageInt()
{
	const int maximumMessageSize = 1024 * 1024 * 10; // sanity check

	uint32 messageHeader[2];
	const int bytes = socket != nullptr ? socket->read (messageHeader, sizeof (messageHeader), true)
										: pipe  ->read (messageHeader, sizeof (messageHeader), pipeReceiveMessageTimeout);

	if (bytes == sizeof (messageHeader)
		 && ByteOrder::swapIfBigEndian (messageHeader[0]) == magicMessageHeader)
	{
		int bytesInMessage = (int) ByteOrder::swapIfBigEndian (messageHeader[1]);

		if (bytesInMessage > 0 && bytesInMessage < maximumMessageSize)
		{
			MemoryBlock messageData ((size_t) bytesInMessage, true);
			int bytesRead = 0;

			while (bytesInMessage > 0)
			{
				if (threadShouldExit())
					return false;

				const int numThisTime = jmin (bytesInMessage, 65536);
				const int bytesIn = socket != nullptr ? socket->read (static_cast <char*> (messageData.getData()) + bytesRead, numThisTime, true)
													  : pipe  ->read (static_cast <char*> (messageData.getData()) + bytesRead, numThisTime, pipeReceiveMessageTimeout);

				if (bytesIn <= 0)
					break;

				bytesRead += bytesIn;
				bytesInMessage -= bytesIn;
			}

			if (bytesRead >= 0)
				deliverDataInt (messageData);
		}
	}
	else if (bytes < 0)
	{
		{
			const ScopedLock sl (pipeAndSocketLock);
			socket = nullptr;
		}

		connectionLostInt();
		return false;
	}

	return true;
}

void InterprocessConnection::run()
{
	while (! threadShouldExit())
	{
		if (socket != nullptr)
		{
			const int ready = socket->waitUntilReady (true, 0);

			if (ready < 0)
			{
				{
					const ScopedLock sl (pipeAndSocketLock);
					socket = nullptr;
				}

				connectionLostInt();
				break;
			}
			else if (ready > 0)
			{
				if (! readNextMessageInt())
					break;
			}
			else
			{
				Thread::sleep (2);
			}
		}
		else if (pipe != nullptr)
		{
			if (! pipe->isOpen())
			{
				{
					const ScopedLock sl (pipeAndSocketLock);
					pipe = nullptr;
				}

				connectionLostInt();
				break;
			}
			else
			{
				if (! readNextMessageInt())
					break;
			}
		}
		else
		{
			break;
		}
	}
}

/*** End of inlined file: juce_InterprocessConnection.cpp ***/


/*** Start of inlined file: juce_InterprocessConnectionServer.cpp ***/
InterprocessConnectionServer::InterprocessConnectionServer()
	: Thread ("Juce IPC server")
{
}

InterprocessConnectionServer::~InterprocessConnectionServer()
{
	stop();
}

bool InterprocessConnectionServer::beginWaitingForSocket (const int portNumber)
{
	stop();

	socket = new StreamingSocket();

	if (socket->createListener (portNumber))
	{
		startThread();
		return true;
	}

	socket = nullptr;
	return false;
}

void InterprocessConnectionServer::stop()
{
	signalThreadShouldExit();

	if (socket != nullptr)
		socket->close();

	stopThread (4000);
	socket = nullptr;
}

void InterprocessConnectionServer::run()
{
	while ((! threadShouldExit()) && socket != nullptr)
	{
		ScopedPointer <StreamingSocket> clientSocket (socket->waitForNextConnection());

		if (clientSocket != nullptr)
		{
			InterprocessConnection* newConnection = createConnectionObject();

			if (newConnection != nullptr)
				newConnection->initialiseWithSocket (clientSocket.release());
		}
	}
}

/*** End of inlined file: juce_InterprocessConnectionServer.cpp ***/

// END_AUTOINCLUDE

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


/*** Start of inlined file: juce_osx_MessageQueue.h ***/
#ifndef __JUCE_OSX_MESSAGEQUEUE_JUCEHEADER__
#define __JUCE_OSX_MESSAGEQUEUE_JUCEHEADER__

/* An internal message pump class used in OSX and iOS. */
class MessageQueue
{
public:
	MessageQueue()
	{
	   #if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_4 && ! JUCE_IOS
		runLoop = CFRunLoopGetMain();
	   #else
		runLoop = CFRunLoopGetCurrent();
	   #endif

		CFRunLoopSourceContext sourceContext = { 0 };
		sourceContext.info = this;
		sourceContext.perform = runLoopSourceCallback;
		runLoopSource = CFRunLoopSourceCreate (kCFAllocatorDefault, 1, &sourceContext);
		CFRunLoopAddSource (runLoop, runLoopSource, kCFRunLoopCommonModes);
	}

	~MessageQueue()
	{
		CFRunLoopRemoveSource (runLoop, runLoopSource, kCFRunLoopCommonModes);
		CFRunLoopSourceInvalidate (runLoopSource);
		CFRelease (runLoopSource);
	}

	void post (MessageManager::MessageBase* const message)
	{
		messages.add (message);
		CFRunLoopSourceSignal (runLoopSource);
		CFRunLoopWakeUp (runLoop);
	}

private:
	ReferenceCountedArray <MessageManager::MessageBase, CriticalSection> messages;
	CriticalSection lock;
	CFRunLoopRef runLoop;
	CFRunLoopSourceRef runLoopSource;

	bool deliverNextMessage()
	{
		const MessageManager::MessageBase::Ptr nextMessage (messages.removeAndReturn (0));

		if (nextMessage == nullptr)
			return false;

		JUCE_AUTORELEASEPOOL

		JUCE_TRY
		{
			nextMessage->messageCallback();
		}
		JUCE_CATCH_EXCEPTION

		return true;
	}

	void runLoopCallback()
	{
		for (int i = 4; --i >= 0;)
			if (! deliverNextMessage())
				return;

		CFRunLoopSourceSignal (runLoopSource);
		CFRunLoopWakeUp (runLoop);
	}

	static void runLoopSourceCallback (void* info)
	{
		static_cast <MessageQueue*> (info)->runLoopCallback();
	}
};

#endif   // __JUCE_OSX_MESSAGEQUEUE_JUCEHEADER__

/*** End of inlined file: juce_osx_MessageQueue.h ***/


/*** Start of inlined file: juce_mac_MessageManager.mm ***/
typedef void (*AppFocusChangeCallback)();
AppFocusChangeCallback appFocusChangeCallback = nullptr;

typedef bool (*CheckEventBlockedByModalComps) (NSEvent*);
CheckEventBlockedByModalComps isEventBlockedByModalComps = nullptr;

/* When you use multiple DLLs which share similarly-named obj-c classes - like
   for example having more than one juce plugin loaded into a host, then when a
   method is called, the actual code that runs might actually be in a different module
   than the one you expect... So any calls to library functions or statics that are
   made inside obj-c methods will probably end up getting executed in a different DLL's
   memory space. Not a great thing to happen - this obviously leads to bizarre crashes.

   To work around this insanity, I'm only allowing obj-c methods to make calls to
   virtual methods of an object that's known to live inside the right module's space.
*/
class AppDelegateRedirector
{
public:
	AppDelegateRedirector() {}
	virtual ~AppDelegateRedirector() {}

	virtual NSApplicationTerminateReply shouldTerminate()
	{
		if (JUCEApplicationBase::getInstance() != nullptr)
		{
			JUCEApplicationBase::getInstance()->systemRequestedQuit();

			if (! MessageManager::getInstance()->hasStopMessageBeenSent())
				return NSTerminateCancel;
		}

		return NSTerminateNow;
	}

	virtual void willTerminate()
	{
		JUCEApplicationBase::appWillTerminateByForce();
	}

	virtual BOOL openFile (NSString* filename)
	{
		if (JUCEApplicationBase::getInstance() != nullptr)
		{
			JUCEApplicationBase::getInstance()->anotherInstanceStarted (quotedIfContainsSpaces (filename));
			return YES;
		}

		return NO;
	}

	virtual void openFiles (NSArray* filenames)
	{
		StringArray files;
		for (unsigned int i = 0; i < [filenames count]; ++i)
			files.add (quotedIfContainsSpaces ((NSString*) [filenames objectAtIndex: i]));

		if (files.size() > 0 && JUCEApplicationBase::getInstance() != nullptr)
			JUCEApplicationBase::getInstance()->anotherInstanceStarted (files.joinIntoString (" "));
	}

	virtual void focusChanged()
	{
		if (appFocusChangeCallback != nullptr)
			(*appFocusChangeCallback)();
	}

	virtual void deleteSelf()
	{
		delete this;
	}

	void postMessage (MessageManager::MessageBase* const m)
	{
		messageQueue.post (m);
	}

	static NSString* getBroacastEventName()
	{
		return juceStringToNS ("juce_" + String::toHexString (File::getSpecialLocation (File::currentExecutableFile).hashCode64()));
	}

private:
	CFRunLoopRef runLoop;
	CFRunLoopSourceRef runLoopSource;
	MessageQueue messageQueue;

	static String quotedIfContainsSpaces (NSString* file)
	{
		String s (nsStringToJuce (file));
		if (s.containsChar (' '))
			s = s.quoted ('"');

		return s;
	}
};

} // (juce namespace)

using namespace juce;

#define JuceAppDelegate MakeObjCClassName(JuceAppDelegate)

@interface JuceAppDelegate   : NSObject
{
@public
	AppDelegateRedirector* redirector;
}

- (JuceAppDelegate*) init;
- (void) dealloc;
- (void) unregisterObservers;
- (BOOL) application: (NSApplication*) theApplication openFile: (NSString*) filename;
- (void) application: (NSApplication*) sender openFiles: (NSArray*) filenames;
- (NSApplicationTerminateReply) applicationShouldTerminate: (NSApplication*) app;
- (void) applicationWillTerminate: (NSNotification*) aNotification;
- (void) applicationDidBecomeActive: (NSNotification*) aNotification;
- (void) applicationDidResignActive: (NSNotification*) aNotification;
- (void) applicationWillUnhide: (NSNotification*) aNotification;
- (void) broadcastMessageCallback: (NSNotification*) info;
- (void) dummyMethod;
@end

@implementation JuceAppDelegate

- (JuceAppDelegate*) init
{
	[super init];

	redirector = new AppDelegateRedirector();

	if (JUCEApplicationBase::isStandaloneApp())
	{
		[NSApp setDelegate: self];

		[[NSDistributedNotificationCenter defaultCenter] addObserver: self
															selector: @selector (broadcastMessageCallback:)
																name: AppDelegateRedirector::getBroacastEventName()
															  object: nil];
	}
	else
	{
		NSNotificationCenter* center = [NSNotificationCenter defaultCenter];

		[center addObserver: self selector: @selector (applicationDidResignActive:)
					   name: NSApplicationDidResignActiveNotification object: NSApp];

		[center addObserver: self selector: @selector (applicationDidBecomeActive:)
					   name: NSApplicationDidBecomeActiveNotification object: NSApp];

		[center addObserver: self selector: @selector (applicationWillUnhide:)
					   name: NSApplicationWillUnhideNotification object: NSApp];
	}

	return self;
}

- (void) dealloc
{
	redirector->deleteSelf();
	[super dealloc];
}

- (void) unregisterObservers
{
	[[NSRunLoop currentRunLoop] cancelPerformSelectorsWithTarget: self];
	[[NSNotificationCenter defaultCenter] removeObserver: self];

	if (JUCEApplicationBase::isStandaloneApp())
	{
		[NSApp setDelegate: nil];

		[[NSDistributedNotificationCenter defaultCenter] removeObserver: self
																   name: AppDelegateRedirector::getBroacastEventName()
																 object: nil];
	}
}

- (NSApplicationTerminateReply) applicationShouldTerminate: (NSApplication*) app
{
	(void) app;
	return redirector->shouldTerminate();
}

- (void) applicationWillTerminate: (NSNotification*) aNotification
{
	(void) aNotification;
	redirector->willTerminate();
}

- (BOOL) application: (NSApplication*) app openFile: (NSString*) filename
{
	(void) app;
	return redirector->openFile (filename);
}

- (void) application: (NSApplication*) sender openFiles: (NSArray*) filenames
{
	(void) sender;
	return redirector->openFiles (filenames);
}

- (void) applicationDidBecomeActive: (NSNotification*) notification
{
	(void) notification;
	redirector->focusChanged();
}

- (void) applicationDidResignActive: (NSNotification*) notification
{
	(void) notification;
	redirector->focusChanged();
}

- (void) applicationWillUnhide: (NSNotification*) notification
{
	(void) notification;
	redirector->focusChanged();
}

- (void) broadcastMessageCallback: (NSNotification*) n
{
	NSDictionary* dict = (NSDictionary*) [n userInfo];
	const String messageString (nsStringToJuce ((NSString*) [dict valueForKey: nsStringLiteral ("message")]));
	MessageManager::getInstance()->deliverBroadcastMessage (messageString);
}

- (void) dummyMethod  {}   // (used as a way of running a dummy thread)

@end

namespace juce
{

static JuceAppDelegate* juceAppDelegate = nil;

void MessageManager::runDispatchLoop()
{
	if (! quitMessagePosted) // check that the quit message wasn't already posted..
	{
		JUCE_AUTORELEASEPOOL

		// must only be called by the message thread!
		jassert (isThisTheMessageThread());

	  #if JUCE_CATCH_UNHANDLED_EXCEPTIONS
		@try
		{
			[NSApp run];
		}
		@catch (NSException* e)
		{
			// An AppKit exception will kill the app, but at least this provides a chance to log it.,
			std::runtime_error ex (std::string ("NSException: ") + [[e name] UTF8String] + ", Reason:" + [[e reason] UTF8String]);
			JUCEApplication::sendUnhandledException (&ex, __FILE__, __LINE__);
		}
		@finally
		{
		}
	   #else
		[NSApp run];
	   #endif
	}
}

void MessageManager::stopDispatchLoop()
{
	quitMessagePosted = true;
	[NSApp stop: nil];
	[NSApp activateIgnoringOtherApps: YES]; // (if the app is inactive, it sits there and ignores the quit request until the next time it gets activated)
	[NSEvent startPeriodicEventsAfterDelay: 0 withPeriod: 0.1];
}

#if JUCE_MODAL_LOOPS_PERMITTED
bool MessageManager::runDispatchLoopUntil (int millisecondsToRunFor)
{
	jassert (millisecondsToRunFor >= 0);
	jassert (isThisTheMessageThread()); // must only be called by the message thread

	uint32 endTime = Time::getMillisecondCounter() + (uint32) millisecondsToRunFor;

	while (! quitMessagePosted)
	{
		JUCE_AUTORELEASEPOOL

		CFRunLoopRunInMode (kCFRunLoopDefaultMode, 0.001, true);

		NSEvent* e = [NSApp nextEventMatchingMask: NSAnyEventMask
										untilDate: [NSDate dateWithTimeIntervalSinceNow: 0.001]
										   inMode: NSDefaultRunLoopMode
										  dequeue: YES];

		if (e != nil && (isEventBlockedByModalComps == nullptr || ! (*isEventBlockedByModalComps) (e)))
			[NSApp sendEvent: e];

		if (Time::getMillisecondCounter() >= endTime)
			break;
	}

	return ! quitMessagePosted;
}
#endif

void initialiseNSApplication();
void initialiseNSApplication()
{
	JUCE_AUTORELEASEPOOL
	[NSApplication sharedApplication];
}

void MessageManager::doPlatformSpecificInitialisation()
{
	if (juceAppDelegate == nil)
		juceAppDelegate = [[JuceAppDelegate alloc] init];

   #if ! (defined (MAC_OS_X_VERSION_10_5) && MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5)
	// This launches a dummy thread, which forces Cocoa to initialise NSThreads correctly (needed prior to 10.5)
	if (! [NSThread isMultiThreaded])
		[NSThread detachNewThreadSelector: @selector (dummyMethod)
								 toTarget: juceAppDelegate
							   withObject: nil];
   #endif
}

void MessageManager::doPlatformSpecificShutdown()
{
	if (juceAppDelegate != nil)
	{
		[juceAppDelegate unregisterObservers];
		[juceAppDelegate release];
		juceAppDelegate = nil;
	}
}

bool MessageManager::postMessageToSystemQueue (MessageBase* message)
{
	juceAppDelegate->redirector->postMessage (message);
	return true;
}

void MessageManager::broadcastMessage (const String& message)
{
	NSDictionary* info = [NSDictionary dictionaryWithObject: juceStringToNS (message)
													 forKey: nsStringLiteral ("message")];

	[[NSDistributedNotificationCenter defaultCenter] postNotificationName: AppDelegateRedirector::getBroacastEventName()
																   object: nil
																 userInfo: info];
}

/*** End of inlined file: juce_mac_MessageManager.mm ***/

#elif JUCE_IOS


/*** Start of inlined file: juce_ios_MessageManager.mm ***/
void MessageManager::runDispatchLoop()
{
	jassert (isThisTheMessageThread()); // must only be called by the message thread
	runDispatchLoopUntil (-1);
}

void MessageManager::stopDispatchLoop()
{
	[[[UIApplication sharedApplication] delegate] applicationWillTerminate: [UIApplication sharedApplication]];
	exit (0); // iPhone apps get no mercy..
}

bool MessageManager::runDispatchLoopUntil (int millisecondsToRunFor)
{
	JUCE_AUTORELEASEPOOL
	jassert (isThisTheMessageThread()); // must only be called by the message thread

	uint32 endTime = Time::getMillisecondCounter() + millisecondsToRunFor;
	NSDate* endDate = [NSDate dateWithTimeIntervalSinceNow: millisecondsToRunFor * 0.001];

	while (! quitMessagePosted)
	{
		JUCE_AUTORELEASEPOOL

		[[NSRunLoop currentRunLoop] runMode: NSDefaultRunLoopMode
								 beforeDate: endDate];

		if (millisecondsToRunFor >= 0 && Time::getMillisecondCounter() >= endTime)
			break;
	}

	return ! quitMessagePosted;
}

static ScopedPointer<MessageQueue> messageQueue;

void MessageManager::doPlatformSpecificInitialisation()
{
	if (messageQueue == nullptr)
		messageQueue = new MessageQueue();
}

void MessageManager::doPlatformSpecificShutdown()
{
	messageQueue = nullptr;
}

bool MessageManager::postMessageToSystemQueue (MessageManager::MessageBase* const message)
{
	if (messageQueue != nullptr)
		messageQueue->post (message);

	return true;
}

void MessageManager::broadcastMessage (const String&)
{
}

/*** End of inlined file: juce_ios_MessageManager.mm ***/

#elif JUCE_WINDOWS

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



/*** Start of inlined file: juce_win32_Messaging.cpp ***/
extern HWND juce_messageWindowHandle;

typedef bool (*CheckEventBlockedByModalComps) (const MSG&);
CheckEventBlockedByModalComps isEventBlockedByModalComps = nullptr;

namespace WindowsMessageHelpers
{
	const unsigned int specialId   = WM_APP + 0x4400;
	const unsigned int broadcastId = WM_APP + 0x4403;

	const TCHAR messageWindowName[] = _T("JUCEWindow");
	ScopedPointer<HiddenMessageWindow> messageWindow;

	void dispatchMessageFromLParam (LPARAM lParam)
	{
		MessageManager::MessageBase* const message = reinterpret_cast <MessageManager::MessageBase*> (lParam);

		JUCE_TRY
		{
			message->messageCallback();
		}
		JUCE_CATCH_EXCEPTION

		message->decReferenceCount();
	}

	LRESULT CALLBACK messageWndProc (HWND h, const UINT message, const WPARAM wParam, const LPARAM lParam) noexcept
	{
		if (h == juce_messageWindowHandle)
		{
			if (message == specialId)
			{
				// (These are trapped early in our dispatch loop, but must also be checked
				// here in case some 3rd-party code is running the dispatch loop).
				dispatchMessageFromLParam (lParam);
				return 0;
			}
			else if (message == broadcastId)
			{
				const ScopedPointer<String> messageString ((String*) lParam);
				MessageManager::getInstance()->deliverBroadcastMessage (*messageString);
				return 0;
			}
			else if (message == WM_COPYDATA)
			{
				const COPYDATASTRUCT* const data = reinterpret_cast <const COPYDATASTRUCT*> (lParam);

				if (data->dwData == broadcastId)
				{
					const String messageString (CharPointer_UTF32 ((const CharPointer_UTF32::CharType*) data->lpData),
												data->cbData / sizeof (CharPointer_UTF32::CharType));

					PostMessage (juce_messageWindowHandle, broadcastId, 0, (LPARAM) new String (messageString));
					return 0;
				}
			}
		}

		return DefWindowProc (h, message, wParam, lParam);
	}

	BOOL CALLBACK broadcastEnumWindowProc (HWND hwnd, LPARAM lParam)
	{
		if (hwnd != juce_messageWindowHandle)
			reinterpret_cast <Array<HWND>*> (lParam)->add (hwnd);

		return TRUE;
	}
}

bool MessageManager::dispatchNextMessageOnSystemQueue (const bool returnIfNoPendingMessages)
{
	using namespace WindowsMessageHelpers;
	MSG m;

	if (returnIfNoPendingMessages && ! PeekMessage (&m, (HWND) 0, 0, 0, 0))
		return false;

	if (GetMessage (&m, (HWND) 0, 0, 0) >= 0)
	{
		if (m.message == specialId && m.hwnd == juce_messageWindowHandle)
		{
			dispatchMessageFromLParam (m.lParam);
		}
		else if (m.message == WM_QUIT)
		{
			JUCEApplicationBase* const app = JUCEApplicationBase::getInstance();
			if (app != nullptr)
				app->systemRequestedQuit();
		}
		else if (isEventBlockedByModalComps == nullptr || ! isEventBlockedByModalComps (m))
		{
			if ((m.message == WM_LBUTTONDOWN || m.message == WM_RBUTTONDOWN)
				  && ! JuceWindowIdentifier::isJUCEWindow (m.hwnd))
			{
				// if it's someone else's window being clicked on, and the focus is
				// currently on a juce window, pass the kb focus over..
				HWND currentFocus = GetFocus();

				if (currentFocus == 0 || JuceWindowIdentifier::isJUCEWindow (currentFocus))
					SetFocus (m.hwnd);
			}

			TranslateMessage (&m);
			DispatchMessage (&m);
		}
	}

	return true;
}

bool MessageManager::postMessageToSystemQueue (MessageManager::MessageBase* const message)
{
	message->incReferenceCount();
	return PostMessage (juce_messageWindowHandle, WindowsMessageHelpers::specialId, 0, (LPARAM) message) != 0;
}

void MessageManager::broadcastMessage (const String& value)
{
	Array<HWND> windows;
	EnumWindows (&WindowsMessageHelpers::broadcastEnumWindowProc, (LPARAM) &windows);

	const String localCopy (value);

	COPYDATASTRUCT data;
	data.dwData = WindowsMessageHelpers::broadcastId;
	data.cbData = (localCopy.length() + 1) * sizeof (CharPointer_UTF32::CharType);
	data.lpData = (void*) localCopy.toUTF32().getAddress();

	for (int i = windows.size(); --i >= 0;)
	{
		HWND hwnd = windows.getUnchecked(i);

		TCHAR windowName [64]; // no need to read longer strings than this
		GetWindowText (hwnd, windowName, 64);
		windowName [63] = 0;

		if (String (windowName) == WindowsMessageHelpers::messageWindowName)
		{
			DWORD_PTR result;
			SendMessageTimeout (hwnd, WM_COPYDATA,
								(WPARAM) juce_messageWindowHandle,
								(LPARAM) &data,
								SMTO_BLOCK | SMTO_ABORTIFHUNG, 8000, &result);
		}
	}
}

void MessageManager::doPlatformSpecificInitialisation()
{
	OleInitialize (0);

	using namespace WindowsMessageHelpers;
	messageWindow = new HiddenMessageWindow (messageWindowName, (WNDPROC) messageWndProc);
	juce_messageWindowHandle = messageWindow->getHWND();
}

void MessageManager::doPlatformSpecificShutdown()
{
	WindowsMessageHelpers::messageWindow = nullptr;

	OleUninitialize();
}

/*** End of inlined file: juce_win32_Messaging.cpp ***/

#elif JUCE_LINUX


/*** Start of inlined file: juce_linux_Messaging.cpp ***/
#if JUCE_DEBUG && ! defined (JUCE_DEBUG_XERRORS)
 #define JUCE_DEBUG_XERRORS 1
#endif

Display* display = nullptr;
Window juce_messageWindowHandle = None;
XContext windowHandleXContext;   // This is referenced from Windowing.cpp

extern void juce_windowMessageReceive (XEvent* event);  // Defined in Windowing.cpp
extern void juce_handleSelectionRequest (XSelectionRequestEvent &evt);  // Defined in Clipboard.cpp

ScopedXLock::ScopedXLock()       { XLockDisplay (display); }
ScopedXLock::~ScopedXLock()      { XUnlockDisplay (display); }

class InternalMessageQueue
{
public:
	InternalMessageQueue()
		: bytesInSocket (0),
		  totalEventCount (0)
	{
		int ret = ::socketpair (AF_LOCAL, SOCK_STREAM, 0, fd);
		(void) ret; jassert (ret == 0);
	}

	~InternalMessageQueue()
	{
		close (fd[0]);
		close (fd[1]);

		clearSingletonInstance();
	}

	void postMessage (MessageManager::MessageBase* const msg)
	{
		const int maxBytesInSocketQueue = 128;

		ScopedLock sl (lock);
		queue.add (msg);

		if (bytesInSocket < maxBytesInSocketQueue)
		{
			++bytesInSocket;

			ScopedUnlock ul (lock);
			const unsigned char x = 0xff;
			size_t bytesWritten = write (fd[0], &x, 1);
			(void) bytesWritten;
		}
	}

	bool isEmpty() const
	{
		ScopedLock sl (lock);
		return queue.size() == 0;
	}

	bool dispatchNextEvent()
	{
		// This alternates between giving priority to XEvents or internal messages,
		// to keep everything running smoothly..
		if ((++totalEventCount & 1) != 0)
			return dispatchNextXEvent() || dispatchNextInternalMessage();
		else
			return dispatchNextInternalMessage() || dispatchNextXEvent();
	}

	// Wait for an event (either XEvent, or an internal Message)
	bool sleepUntilEvent (const int timeoutMs)
	{
		if (! isEmpty())
			return true;

		if (display != 0)
		{
			ScopedXLock xlock;
			if (XPending (display))
				return true;
		}

		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = timeoutMs * 1000;
		int fd0 = getWaitHandle();
		int fdmax = fd0;

		fd_set readset;
		FD_ZERO (&readset);
		FD_SET (fd0, &readset);

		if (display != 0)
		{
			ScopedXLock xlock;
			int fd1 = XConnectionNumber (display);
			FD_SET (fd1, &readset);
			fdmax = jmax (fd0, fd1);
		}

		const int ret = select (fdmax + 1, &readset, 0, 0, &tv);
		return (ret > 0); // ret <= 0 if error or timeout
	}

	juce_DeclareSingleton_SingleThreaded_Minimal (InternalMessageQueue);

private:
	CriticalSection lock;
	ReferenceCountedArray <MessageManager::MessageBase> queue;
	int fd[2];
	int bytesInSocket;
	int totalEventCount;

	int getWaitHandle() const noexcept      { return fd[1]; }

	static bool setNonBlocking (int handle)
	{
		int socketFlags = fcntl (handle, F_GETFL, 0);
		if (socketFlags == -1)
			return false;

		socketFlags |= O_NONBLOCK;
		return fcntl (handle, F_SETFL, socketFlags) == 0;
	}

	static bool dispatchNextXEvent()
	{
		if (display == 0)
			return false;

		XEvent evt;

		{
			ScopedXLock xlock;
			if (! XPending (display))
				return false;

			XNextEvent (display, &evt);
		}

		if (evt.type == SelectionRequest && evt.xany.window == juce_messageWindowHandle)
			juce_handleSelectionRequest (evt.xselectionrequest);
		else if (evt.xany.window != juce_messageWindowHandle)
			juce_windowMessageReceive (&evt);

		return true;
	}

	MessageManager::MessageBase::Ptr popNextMessage()
	{
		const ScopedLock sl (lock);

		if (bytesInSocket > 0)
		{
			--bytesInSocket;

			const ScopedUnlock ul (lock);
			unsigned char x;
			size_t numBytes = read (fd[1], &x, 1);
			(void) numBytes;
		}

		return queue.removeAndReturn (0);
	}

	bool dispatchNextInternalMessage()
	{
		const MessageManager::MessageBase::Ptr msg (popNextMessage());

		if (msg == nullptr)
			return false;

		JUCE_TRY
		{
			msg->messageCallback();
		}
		JUCE_CATCH_EXCEPTION

		return true;
	}
};

juce_ImplementSingleton_SingleThreaded (InternalMessageQueue);

namespace LinuxErrorHandling
{
	static bool errorOccurred = false;
	static bool keyboardBreakOccurred = false;
	static XErrorHandler oldErrorHandler = (XErrorHandler) 0;
	static XIOErrorHandler oldIOErrorHandler = (XIOErrorHandler) 0;

	// Usually happens when client-server connection is broken
	int ioErrorHandler (Display* display)
	{
		DBG ("ERROR: connection to X server broken.. terminating.");

		if (JUCEApplicationBase::isStandaloneApp())
			MessageManager::getInstance()->stopDispatchLoop();

		errorOccurred = true;
		return 0;
	}

	int errorHandler (Display* display, XErrorEvent* event)
	{
	   #if JUCE_DEBUG_XERRORS
		char errorStr[64] = { 0 };
		char requestStr[64] = { 0 };

		XGetErrorText (display, event->error_code, errorStr, 64);
		XGetErrorDatabaseText (display, "XRequest", String (event->request_code).toUTF8(), "Unknown", requestStr, 64);
		DBG ("ERROR: X returned " << errorStr << " for operation " << requestStr);
	   #endif

		return 0;
	}

	void installXErrorHandlers()
	{
		oldIOErrorHandler = XSetIOErrorHandler (ioErrorHandler);
		oldErrorHandler = XSetErrorHandler (errorHandler);
	}

	void removeXErrorHandlers()
	{
		if (JUCEApplicationBase::isStandaloneApp())
		{
			XSetIOErrorHandler (oldIOErrorHandler);
			oldIOErrorHandler = 0;

			XSetErrorHandler (oldErrorHandler);
			oldErrorHandler = 0;
		}
	}

	void keyboardBreakSignalHandler (int sig)
	{
		if (sig == SIGINT)
			keyboardBreakOccurred = true;
	}

	void installKeyboardBreakHandler()
	{
		struct sigaction saction;
		sigset_t maskSet;
		sigemptyset (&maskSet);
		saction.sa_handler = keyboardBreakSignalHandler;
		saction.sa_mask = maskSet;
		saction.sa_flags = 0;
		sigaction (SIGINT, &saction, 0);
	}
}

void MessageManager::doPlatformSpecificInitialisation()
{
	if (JUCEApplicationBase::isStandaloneApp())
	{
		// Initialise xlib for multiple thread support
		static bool initThreadCalled = false;

		if (! initThreadCalled)
		{
			if (! XInitThreads())
			{
				// This is fatal!  Print error and closedown
				Logger::outputDebugString ("Failed to initialise xlib thread support.");
				Process::terminate();
				return;
			}

			initThreadCalled = true;
		}

		LinuxErrorHandling::installXErrorHandlers();
		LinuxErrorHandling::installKeyboardBreakHandler();
	}

	// Create the internal message queue
	InternalMessageQueue::getInstance();

	// Try to connect to a display
	String displayName (getenv ("DISPLAY"));
	if (displayName.isEmpty())
		displayName = ":0.0";

	display = XOpenDisplay (displayName.toUTF8());

	if (display != 0)  // This is not fatal! we can run headless.
	{
		// Create a context to store user data associated with Windows we create
		windowHandleXContext = XUniqueContext();

		// We're only interested in client messages for this window, which are always sent
		XSetWindowAttributes swa;
		swa.event_mask = NoEventMask;

		// Create our message window (this will never be mapped)
		const int screen = DefaultScreen (display);
		juce_messageWindowHandle = XCreateWindow (display, RootWindow (display, screen),
												  0, 0, 1, 1, 0, 0, InputOnly,
												  DefaultVisual (display, screen),
												  CWEventMask, &swa);
	}
}

void MessageManager::doPlatformSpecificShutdown()
{
	InternalMessageQueue::deleteInstance();

	if (display != 0 && ! LinuxErrorHandling::errorOccurred)
	{
		XDestroyWindow (display, juce_messageWindowHandle);
		XCloseDisplay (display);

		juce_messageWindowHandle = 0;
		display = nullptr;

		LinuxErrorHandling::removeXErrorHandlers();
	}
}

bool MessageManager::postMessageToSystemQueue (MessageManager::MessageBase* const message)
{
	if (LinuxErrorHandling::errorOccurred)
		return false;

	InternalMessageQueue::getInstanceWithoutCreating()->postMessage (message);
	return true;
}

void MessageManager::broadcastMessage (const String& value)
{
	/* TODO */
}

// this function expects that it will NEVER be called simultaneously for two concurrent threads
bool MessageManager::dispatchNextMessageOnSystemQueue (bool returnIfNoPendingMessages)
{
	while (! LinuxErrorHandling::errorOccurred)
	{
		if (LinuxErrorHandling::keyboardBreakOccurred)
		{
			LinuxErrorHandling::errorOccurred = true;

			if (JUCEApplicationBase::isStandaloneApp())
				Process::terminate();

			break;
		}

		InternalMessageQueue* const queue = InternalMessageQueue::getInstanceWithoutCreating();
		jassert (queue != nullptr);

		if (queue->dispatchNextEvent())
			return true;

		if (returnIfNoPendingMessages)
			break;

		queue->sleepUntilEvent (2000);
	}

	return false;
}

/*** End of inlined file: juce_linux_Messaging.cpp ***/

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



/*** Start of inlined file: juce_android_Messaging.cpp ***/
void MessageManager::doPlatformSpecificInitialisation() {}
void MessageManager::doPlatformSpecificShutdown() {}

bool MessageManager::dispatchNextMessageOnSystemQueue (const bool returnIfNoPendingMessages)
{
	Logger::outputDebugString ("*** Modal loops are not possible in Android!! Exiting...");
	exit (1);

	return true;
}

bool MessageManager::postMessageToSystemQueue (MessageManager::MessageBase* const message)
{
	message->incReferenceCount();
	android.activity.callVoidMethod (JuceAppActivity.postMessage, (jlong) (pointer_sized_uint) message);
	return true;
}

JUCE_JNI_CALLBACK (JUCE_ANDROID_ACTIVITY_CLASSNAME, deliverMessage, void, (jobject activity, jlong value))
{
	JUCE_TRY
	{
		MessageManager::MessageBase* const message = (MessageManager::MessageBase*) (pointer_sized_uint) value;
		message->messageCallback();
		message->decReferenceCount();
	}
	JUCE_CATCH_EXCEPTION
}

void MessageManager::broadcastMessage (const String&)
{
}

void MessageManager::runDispatchLoop()
{
}

void MessageManager::stopDispatchLoop()
{
	struct QuitCallback  : public CallbackMessage
	{
		QuitCallback() {}

		void messageCallback()
		{
			android.activity.callVoidMethod (JuceAppActivity.finish);
		}
	};

	(new QuitCallback())->post();
	quitMessagePosted = true;
}

/*** End of inlined file: juce_android_Messaging.cpp ***/

#endif

}

/*** End of inlined file: juce_events.cpp ***/


#endif

