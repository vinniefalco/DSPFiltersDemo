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

/*** Start of inlined file: juce_opengl.cpp ***/
#if defined (__JUCE_OPENGL_JUCEHEADER__) && ! JUCE_AMALGAMATED_INCLUDE
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

#include "juce_opengl_amalgam.h"

#if JUCE_IOS
 #import <QuartzCore/QuartzCore.h>

#elif JUCE_WINDOWS
 #include <windowsx.h>

 #if JUCE_MSVC && ! JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment(lib, "OpenGL32.Lib")
 #endif

#elif JUCE_LINUX
 /* Got an include error here?

	If you want to install OpenGL support, the packages to get are "mesa-common-dev"
	and "freeglut3-dev".
 */
 #include <GL/glx.h>

#elif JUCE_MAC
 #include <OpenGL/CGLCurrent.h> // These are both just needed with the 10.5 SDK
 #include <OpenGL/OpenGL.h>

#elif JUCE_ANDROID
 #ifndef GL_GLEXT_PROTOTYPES
  #define GL_GLEXT_PROTOTYPES 1
 #endif
 #include <GLES2/gl2.h>
#endif

namespace juce
{


/*** Start of inlined file: juce_MissingGLDefinitions.h ***/
#ifndef GL_CLAMP_TO_EDGE
 #define GL_CLAMP_TO_EDGE        0x812f
#endif

#ifndef GL_NUM_EXTENSIONS
 #define GL_NUM_EXTENSIONS       0x821d
#endif

#ifndef GL_BGRA_EXT
 #define GL_BGRA_EXT             0x80e1
#endif

#ifndef GL_DEPTH24_STENCIL8
 #define GL_DEPTH24_STENCIL8     0x88F0
#endif

#ifndef GL_RGBA8
 #define GL_RGBA8                GL_RGBA
#endif

#if JUCE_ANDROID
 #define JUCE_RGBA_FORMAT        GL_RGBA
#else
 #define JUCE_RGBA_FORMAT        GL_BGRA_EXT
#endif

#ifndef GL_COLOR_ATTACHMENT0
 #define GL_COLOR_ATTACHMENT0    0x8CE0
#endif

#ifndef GL_DEPTH_ATTACHMENT
 #define GL_DEPTH_ATTACHMENT     0x8D00
#endif

#ifndef GL_FRAMEBUFFER
 #define GL_FRAMEBUFFER          0x8D40
#endif

#ifndef GL_FRAMEBUFFER_BINDING
 #define GL_FRAMEBUFFER_BINDING  0x8CA6
#endif

#ifndef GL_FRAMEBUFFER_COMPLETE
 #define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#endif

#ifndef GL_RENDERBUFFER
 #define GL_RENDERBUFFER         0x8D41
#endif

#ifndef GL_RENDERBUFFER_DEPTH_SIZE
 #define GL_RENDERBUFFER_DEPTH_SIZE  0x8D54
#endif

#ifndef GL_STENCIL_ATTACHMENT
 #define GL_STENCIL_ATTACHMENT   0x8D20
#endif

#if JUCE_WINDOWS
namespace
{
	enum
	{
		WGL_NUMBER_PIXEL_FORMATS_ARB    = 0x2000,
		WGL_DRAW_TO_WINDOW_ARB          = 0x2001,
		WGL_ACCELERATION_ARB            = 0x2003,
		WGL_SWAP_METHOD_ARB             = 0x2007,
		WGL_SUPPORT_OPENGL_ARB          = 0x2010,
		WGL_PIXEL_TYPE_ARB              = 0x2013,
		WGL_DOUBLE_BUFFER_ARB           = 0x2011,
		WGL_COLOR_BITS_ARB              = 0x2014,
		WGL_RED_BITS_ARB                = 0x2015,
		WGL_GREEN_BITS_ARB              = 0x2017,
		WGL_BLUE_BITS_ARB               = 0x2019,
		WGL_ALPHA_BITS_ARB              = 0x201B,
		WGL_DEPTH_BITS_ARB              = 0x2022,
		WGL_STENCIL_BITS_ARB            = 0x2023,
		WGL_FULL_ACCELERATION_ARB       = 0x2027,
		WGL_ACCUM_RED_BITS_ARB          = 0x201E,
		WGL_ACCUM_GREEN_BITS_ARB        = 0x201F,
		WGL_ACCUM_BLUE_BITS_ARB         = 0x2020,
		WGL_ACCUM_ALPHA_BITS_ARB        = 0x2021,
		WGL_STEREO_ARB                  = 0x2012,
		WGL_SAMPLE_BUFFERS_ARB          = 0x2041,
		WGL_SAMPLES_ARB                 = 0x2042,
		WGL_TYPE_RGBA_ARB               = 0x202B,

		GL_OPERAND0_RGB                 = 0x8590,
		GL_OPERAND1_RGB                 = 0x8591,
		GL_OPERAND0_ALPHA               = 0x8598,
		GL_OPERAND1_ALPHA               = 0x8599,
		GL_SRC0_RGB                     = 0x8580,
		GL_SRC1_RGB                     = 0x8581,
		GL_SRC0_ALPHA                   = 0x8588,
		GL_SRC1_ALPHA                   = 0x8589,
		GL_TEXTURE0                     = 0x84C0,
		GL_TEXTURE1                     = 0x84C1,
		GL_TEXTURE2                     = 0x84C2,
		GL_COMBINE                      = 0x8570,
		GL_COMBINE_RGB                  = 0x8571,
		GL_COMBINE_ALPHA                = 0x8572,
		GL_PREVIOUS                     = 0x8578,
		GL_COMPILE_STATUS               = 0x8B81,
		GL_LINK_STATUS                  = 0x8B82,
		GL_SHADING_LANGUAGE_VERSION     = 0x8B8C,
		GL_FRAGMENT_SHADER              = 0x8B30,
		GL_VERTEX_SHADER                = 0x8B31,
		GL_ARRAY_BUFFER                 = 0x8892,
		GL_ELEMENT_ARRAY_BUFFER         = 0x8893,
		GL_STATIC_DRAW                  = 0x88E4,
		GL_DYNAMIC_DRAW                 = 0x88E8
	};

	typedef char GLchar;
	typedef pointer_sized_int GLsizeiptr;
	typedef pointer_sized_int GLintptr;
}
#endif

/*** End of inlined file: juce_MissingGLDefinitions.h ***/

void OpenGLExtensionFunctions::initialise()
{
   #if JUCE_WINDOWS || JUCE_LINUX
	#define JUCE_INIT_GL_FUNCTION(name, returnType, params, callparams) \
		name = (type_ ## name) OpenGLHelpers::getExtensionFunction (#name);
	#define JUCE_INIT_GL_FUNCTION_EXT(name, returnType, params, callparams) \
		name = (type_ ## name) OpenGLHelpers::getExtensionFunction (#name); \
		if (name == nullptr) \
			name = (type_ ## name) OpenGLHelpers::getExtensionFunction (JUCE_STRINGIFY (name ## EXT));

	JUCE_GL_EXTENSION_FUNCTIONS (JUCE_INIT_GL_FUNCTION, JUCE_INIT_GL_FUNCTION_EXT)
	#undef JUCE_INIT_GL_FUNCTION
	#undef JUCE_INIT_GL_FUNCTION_EXT
   #endif
}

#if JUCE_OPENGL_ES
 #define JUCE_DECLARE_GL_FUNCTION(name, returnType, params, callparams) \
	inline returnType OpenGLExtensionFunctions::name params { return ::name callparams; }

 JUCE_GL_EXTENSION_FUNCTIONS (JUCE_DECLARE_GL_FUNCTION, JUCE_DECLARE_GL_FUNCTION)
 #undef JUCE_DECLARE_GL_FUNCTION
#endif

#undef JUCE_GL_EXTENSION_FUNCTIONS

#if JUCE_OPENGL_ES
 #define JUCE_MEDIUMP "mediump"
 #define JUCE_HIGHP   "highp"
#else
 #define JUCE_MEDIUMP
 #define JUCE_HIGHP
#endif

#if JUCE_DEBUG && ! defined (JUCE_CHECK_OPENGL_ERROR)
static const char* getGLErrorMessage (const GLenum e)
{
	switch (e)
	{
		case GL_INVALID_ENUM:       return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:      return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:  return "GL_INVALID_OPERATION";
	   #ifdef GL_STACK_OVERFLOW
		case GL_STACK_OVERFLOW:     return "GL_STACK_OVERFLOW";
	   #endif
	   #ifdef GL_STACK_UNDERFLOW
		case GL_STACK_UNDERFLOW:    return "GL_STACK_UNDERFLOW";
	   #endif
		case GL_OUT_OF_MEMORY:      return "GL_OUT_OF_MEMORY";
		default:                    break;
	}

	return "Unknown error";
}

static void checkGLError (const char* file, const int line)
{
	for (;;)
	{
		const GLenum e = glGetError();

		if (e == GL_NO_ERROR)
			break;

		DBG ("***** " << getGLErrorMessage (e) << "  at " << file << " : " << line);
		jassertfalse;
	}
}

 #define JUCE_CHECK_OPENGL_ERROR checkGLError (__FILE__, __LINE__);
#else
 #define JUCE_CHECK_OPENGL_ERROR ;
#endif

static void clearGLError()
{
	while (glGetError() != GL_NO_ERROR) {}
}


/*** Start of inlined file: juce_OpenGLFrameBuffer.cpp ***/
class OpenGLFrameBuffer::Pimpl
{
public:
	Pimpl (OpenGLContext& context_, const int width_, const int height_,
		   const bool wantsDepthBuffer, const bool wantsStencilBuffer)
		: context (context_),
		  width (width_),
		  height (height_),
		  textureID (0),
		  frameBufferID (0),
		  depthOrStencilBuffer (0),
		  hasDepthBuffer (false),
		  hasStencilBuffer (false)
	{
		// Framebuffer objects can only be created when the current thread has an active OpenGL
		// context. You'll need to create this object in one of the OpenGLContext's callbacks.
		jassert (OpenGLHelpers::isContextActive());

	   #if JUCE_WINDOWS || JUCE_LINUX
		if (context.extensions.glGenFramebuffers == nullptr)
			return;
	   #endif

		context.extensions.glGenFramebuffers (1, &frameBufferID);
		context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, frameBufferID);
		JUCE_CHECK_OPENGL_ERROR

		glGenTextures (1, &textureID);
		glBindTexture (GL_TEXTURE_2D, textureID);
		JUCE_CHECK_OPENGL_ERROR

		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		JUCE_CHECK_OPENGL_ERROR

		context.extensions.glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

		if (wantsDepthBuffer || wantsStencilBuffer)
		{
			context.extensions.glGenRenderbuffers (1, &depthOrStencilBuffer);
			context.extensions.glBindRenderbuffer (GL_RENDERBUFFER, depthOrStencilBuffer);
			jassert (context.extensions.glIsRenderbuffer (depthOrStencilBuffer));

			context.extensions.glRenderbufferStorage (GL_RENDERBUFFER,
									  (wantsDepthBuffer && wantsStencilBuffer) ? GL_DEPTH24_STENCIL8
																			  #if JUCE_OPENGL_ES
																			   : GL_DEPTH_COMPONENT16,
																			  #else
																			   : GL_DEPTH_COMPONENT,
																			  #endif
									  width, height);

			GLint params = 0;
			context.extensions.glGetRenderbufferParameteriv (GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE, &params);
			context.extensions.glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthOrStencilBuffer);

			if (wantsStencilBuffer)
				context.extensions.glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthOrStencilBuffer);

			hasDepthBuffer = wantsDepthBuffer;
			hasStencilBuffer = wantsStencilBuffer;
		}

		context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, 0);
	}

	~Pimpl()
	{
		if (OpenGLHelpers::isContextActive())
		{
			if (textureID != 0)
				glDeleteTextures (1, &textureID);

			if (depthOrStencilBuffer != 0)
				context.extensions.glDeleteRenderbuffers (1, &depthOrStencilBuffer);

			if (frameBufferID != 0)
				context.extensions.glDeleteFramebuffers (1, &frameBufferID);

			JUCE_CHECK_OPENGL_ERROR
		}
	}

	bool createdOk() const
	{
		return frameBufferID != 0 && textureID != 0;
	}

	void bind()
	{
		context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, frameBufferID);
		JUCE_CHECK_OPENGL_ERROR
	}

	void unbind()
	{
		context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, 0);
		JUCE_CHECK_OPENGL_ERROR
	}

	OpenGLContext& context;
	const int width, height;
	GLuint textureID, frameBufferID, depthOrStencilBuffer;
	bool hasDepthBuffer, hasStencilBuffer;

private:
	bool checkStatus() noexcept
	{
		const GLenum status = context.extensions.glCheckFramebufferStatus (GL_FRAMEBUFFER);

		return status == GL_NO_ERROR
			|| status == GL_FRAMEBUFFER_COMPLETE;
	}

	JUCE_DECLARE_NON_COPYABLE (Pimpl);
};

class OpenGLFrameBuffer::SavedState
{
public:
	SavedState (OpenGLFrameBuffer& buffer, const int w, const int h)
		: width (w), height (h),
		  data (w * h)
	{
		buffer.readPixels (data, Rectangle<int> (w, h));
	}

	bool restore (OpenGLContext& context, OpenGLFrameBuffer& buffer)
	{
		if (buffer.initialise (context, width, height))
		{
			buffer.writePixels (data, Rectangle<int> (width, height));
			return true;
		}

		return false;
	}

private:
	const int width, height;
	HeapBlock <PixelARGB> data;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SavedState);
};

OpenGLFrameBuffer::OpenGLFrameBuffer() {}
OpenGLFrameBuffer::~OpenGLFrameBuffer() {}

bool OpenGLFrameBuffer::initialise (OpenGLContext& context, int width, int height)
{
	jassert (context.isActive()); // The context must be active when creating a framebuffer!

	pimpl = nullptr;
	pimpl = new Pimpl (context, width, height, false, false);

	if (! pimpl->createdOk())
		pimpl = nullptr;

	return pimpl != nullptr;
}

bool OpenGLFrameBuffer::initialise (OpenGLContext& context, const Image& image)
{
	if (! image.isARGB())
		return initialise (context, image.convertedToFormat (Image::ARGB));

	Image::BitmapData bitmap (image, Image::BitmapData::readOnly);

	return initialise (context, bitmap.width, bitmap.height)
			&& writePixels ((const PixelARGB*) bitmap.data, image.getBounds());
}

bool OpenGLFrameBuffer::initialise (OpenGLFrameBuffer& other)
{
	const Pimpl* const p = other.pimpl;

	if (p == nullptr)
	{
		pimpl = nullptr;
		return true;
	}

	const Rectangle<int> area (pimpl->width, pimpl->height);

	if (initialise (p->context, area.getWidth(), area.getHeight()))
	{
		pimpl->bind();

	   #if ! JUCE_ANDROID
		glEnable (GL_TEXTURE_2D);
		clearGLError();
	   #endif
		glBindTexture (GL_TEXTURE_2D, p->textureID);
		pimpl->context.copyTexture (area, area, area.getWidth(), area.getHeight());
		glBindTexture (GL_TEXTURE_2D, 0);
		JUCE_CHECK_OPENGL_ERROR

		pimpl->unbind();
		return true;
	}

	return false;
}

void OpenGLFrameBuffer::release()
{
	pimpl = nullptr;
	savedState = nullptr;
}

void OpenGLFrameBuffer::saveAndRelease()
{
	if (pimpl != nullptr)
	{
		savedState = new SavedState (*this, pimpl->width, pimpl->height);
		pimpl = nullptr;
	}
}

bool OpenGLFrameBuffer::reloadSavedCopy (OpenGLContext& context)
{
	if (savedState != nullptr)
	{
		ScopedPointer<SavedState> state (savedState);

		if (state->restore (context, *this))
			return true;

		savedState = state;
	}

	return false;
}

int OpenGLFrameBuffer::getWidth() const noexcept            { return pimpl != nullptr ? pimpl->width : 0; }
int OpenGLFrameBuffer::getHeight() const noexcept           { return pimpl != nullptr ? pimpl->height : 0; }
GLuint OpenGLFrameBuffer::getTextureID() const noexcept     { return pimpl != nullptr ? pimpl->textureID : 0; }

bool OpenGLFrameBuffer::makeCurrentRenderingTarget()
{
	// trying to use a framebuffer after saving it with saveAndRelease()! Be sure to call
	// reloadSavedCopy() to put it back into GPU memory before using it..
	jassert (savedState == nullptr);

	if (pimpl == nullptr)
		return false;

	pimpl->bind();
	return true;
}

GLuint OpenGLFrameBuffer::getFrameBufferID() const
{
	return pimpl != nullptr ? pimpl->frameBufferID : 0;
}

GLuint OpenGLFrameBuffer::getCurrentFrameBufferTarget()
{
	GLint fb;
	glGetIntegerv (GL_FRAMEBUFFER_BINDING, &fb);
	return (GLuint) fb;
}

void OpenGLFrameBuffer::releaseAsRenderingTarget()
{
	if (pimpl != nullptr)
		pimpl->context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::clear (const Colour& colour)
{
	if (makeCurrentRenderingTarget())
	{
		OpenGLHelpers::clear (colour);
		releaseAsRenderingTarget();
	}
}

void OpenGLFrameBuffer::makeCurrentAndClear()
{
	if (makeCurrentRenderingTarget())
	{
		glClearColor (0, 0, 0, 0);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}

bool OpenGLFrameBuffer::readPixels (PixelARGB* target, const Rectangle<int>& area)
{
	if (! makeCurrentRenderingTarget())
		return false;

	glPixelStorei (GL_PACK_ALIGNMENT, 4);
	glReadPixels (area.getX(), area.getY(), area.getWidth(), area.getHeight(),
				  JUCE_RGBA_FORMAT, GL_UNSIGNED_BYTE, target);
	pimpl->context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, 0);
	glPixelStorei (GL_PACK_ALIGNMENT, 0);
	JUCE_CHECK_OPENGL_ERROR
	return true;
}

bool OpenGLFrameBuffer::writePixels (const PixelARGB* data, const Rectangle<int>& area)
{
	if (! makeCurrentRenderingTarget())
		return false;

	glDisable (GL_DEPTH_TEST);
	glDisable (GL_BLEND);
	JUCE_CHECK_OPENGL_ERROR

	OpenGLTexture tex;
	tex.loadARGBFlipped (data, area.getWidth(), area.getHeight());

   #if JUCE_OPENGL_ES && JUCE_USE_OPENGL_FIXED_FUNCTION
	const int texH = tex.getHeight();
	tex.bind();
	const GLint cropRect[4] = { 0, texH - area.getHeight(), area.getWidth(), area.getHeight() };
	glTexParameteriv (GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, cropRect);
	glEnable (GL_TEXTURE_2D);
	clearGLError();
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	glDrawTexiOES (area.getX(), area.getY(), 1, area.getWidth(), area.getHeight());
	glBindTexture (GL_TEXTURE_2D, 0);
   #else
	pimpl->context.copyTexture (area, area, pimpl->width, pimpl->height);
   #endif

	pimpl->context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, 0);
	JUCE_CHECK_OPENGL_ERROR
	return true;
}

#if JUCE_USE_OPENGL_FIXED_FUNCTION
void OpenGLFrameBuffer::draw2D (float x1, float y1,
								float x2, float y2,
								float x3, float y3,
								float x4, float y4,
								const Colour& colour) const
{
	if (pimpl != nullptr)
	{
		glBindTexture (GL_TEXTURE_2D, pimpl->textureID);
		OpenGLHelpers::drawQuad2D (x1, y1, x2, y2, x3, y3, x4, y4, colour);
		glBindTexture (GL_TEXTURE_2D, 0);
	}
}

void OpenGLFrameBuffer::draw3D (float x1, float y1, float z1,
								float x2, float y2, float z2,
								float x3, float y3, float z3,
								float x4, float y4, float z4,
								const Colour& colour) const
{
	if (pimpl != nullptr)
	{
		glBindTexture (GL_TEXTURE_2D, pimpl->textureID);
		OpenGLHelpers::drawQuad3D (x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, colour);
		glBindTexture (GL_TEXTURE_2D, 0);
	}
}

void OpenGLFrameBuffer::drawAt (float x1, float y1) const
{
	if (pimpl != nullptr)
	{
	   #if ! JUCE_ANDROID
		glEnable (GL_TEXTURE_2D);
		clearGLError();
	   #endif
		glBindTexture (GL_TEXTURE_2D, pimpl->textureID);

		glDisableClientState (GL_COLOR_ARRAY);
		glDisableClientState (GL_NORMAL_ARRAY);

		const GLfloat vertices[] = { x1, y1,
									 x1 + pimpl->width, y1,
									 x1, y1 + pimpl->height,
									 x1 + pimpl->width, y1 + pimpl->height };

		const GLfloat textureCoords[] = { 0, 0, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f };

		glEnableClientState (GL_VERTEX_ARRAY);
		glVertexPointer (2, GL_FLOAT, 0, vertices);

		glEnableClientState (GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer (2, GL_FLOAT, 0, textureCoords);

		glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture (GL_TEXTURE_2D, 0);
	}
}
#endif

/*** End of inlined file: juce_OpenGLFrameBuffer.cpp ***/


/*** Start of inlined file: juce_OpenGLGraphicsContext.cpp ***/
namespace OpenGLRendering
{

struct Target
{
	Target (OpenGLContext& context_, GLuint frameBufferID_, int width, int height) noexcept
		: context (context_), frameBufferID (frameBufferID_), bounds (width, height)
	{}

	Target (OpenGLContext& context_, OpenGLFrameBuffer& frameBuffer_, const Point<int>& origin) noexcept
		: context (context_), frameBufferID (frameBuffer_.getFrameBufferID()),
		  bounds (origin.x, origin.y, frameBuffer_.getWidth(), frameBuffer_.getHeight())
	{
		jassert (frameBufferID != 0); // trying to render into an uninitialised framebuffer object.
	}

	Target (const Target& other) noexcept
		: context (other.context), frameBufferID (other.frameBufferID), bounds (other.bounds)
	{}

	Target& operator= (const Target& other) noexcept
	{
		frameBufferID = other.frameBufferID;
		bounds = other.bounds;
		return *this;
	}

	void makeActive() const noexcept
	{
	   #if JUCE_WINDOWS
		if (context.extensions.glBindFramebuffer != nullptr)
	   #endif
			context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, frameBufferID);

		glViewport (0, 0, bounds.getWidth(), bounds.getHeight());
		glDisable (GL_DEPTH_TEST);
	}

	OpenGLContext& context;
	GLuint frameBufferID;
	Rectangle<int> bounds;
};

#if JUCE_USE_OPENGL_SHADERS

class PositionedTexture
{
public:
	PositionedTexture (OpenGLTexture& texture, const EdgeTable& et, const Rectangle<int>& clip_)
		: clip (clip_.getIntersection (et.getMaximumBounds()))
	{
		if (clip.contains (et.getMaximumBounds()))
		{
			createMap (texture, et);
		}
		else
		{
			EdgeTable et2 (clip);
			et2.clipToEdgeTable (et);
			createMap (texture, et2);
		}
	}

	PositionedTexture (GLuint textureID_, const Rectangle<int> area_, const Rectangle<int> clip_) noexcept
		: textureID (textureID_), area (area_), clip (clip_)
	{}

	GLuint textureID;
	Rectangle<int> area, clip;

private:
	void createMap (OpenGLTexture& texture, const EdgeTable& et)
	{
		EdgeTableAlphaMap alphaMap (et);
		texture.loadAlpha (alphaMap.data, alphaMap.area.getWidth(), alphaMap.area.getHeight());
		textureID = texture.getTextureID();
		area = alphaMap.area;
	}

	struct EdgeTableAlphaMap
	{
		EdgeTableAlphaMap (const EdgeTable& et)
			: area (et.getMaximumBounds().withSize (nextPowerOfTwo (et.getMaximumBounds().getWidth()),
													nextPowerOfTwo (et.getMaximumBounds().getHeight())))
		{
			data.calloc (area.getWidth() * area.getHeight());
			et.iterate (*this);
		}

		inline void setEdgeTableYPos (const int y) noexcept
		{
			currentLine = data + (area.getBottom() - 1 - y) * area.getWidth() - area.getX();
		}

		inline void handleEdgeTablePixel (const int x, const int alphaLevel) const noexcept
		{
			currentLine[x] = (uint8) alphaLevel;
		}

		inline void handleEdgeTablePixelFull (const int x) const noexcept
		{
			currentLine[x] = 255;
		}

		inline void handleEdgeTableLine (int x, int width, const int alphaLevel) const noexcept
		{
			memset (currentLine + x, (uint8) alphaLevel, width);
		}

		inline void handleEdgeTableLineFull (int x, int width) const noexcept
		{
			memset (currentLine + x, 255, width);
		}

		HeapBlock<uint8> data;
		const Rectangle<int> area;

	private:
		uint8* currentLine;

		JUCE_DECLARE_NON_COPYABLE (EdgeTableAlphaMap);
	};
};

class ShaderPrograms  : public ReferenceCountedObject
{
public:
	ShaderPrograms (OpenGLContext& context)
		: solidColourProgram (context),
		  solidColourMasked (context),
		  radialGradient (context),
		  radialGradientMasked (context),
		  linearGradient1 (context),
		  linearGradient1Masked (context),
		  linearGradient2 (context),
		  linearGradient2Masked (context),
		  image (context),
		  imageMasked (context),
		  tiledImage (context),
		  tiledImageMasked (context),
		  copyTexture (context),
		  maskTexture (context)
	{}

	typedef ReferenceCountedObjectPtr<ShaderPrograms> Ptr;

	struct ShaderProgramHolder
	{
		ShaderProgramHolder (OpenGLContext& context, const char* fragmentShader)
			: program (context)
		{
			JUCE_CHECK_OPENGL_ERROR
			program.addShader ("attribute vec2 position;"
							   "attribute vec4 colour;"
							   "uniform vec4 screenBounds;"
							   "varying " JUCE_MEDIUMP " vec4 frontColour;"
							   "varying " JUCE_HIGHP " vec2 pixelPos;"
							   "void main()"
							   "{"
							   " frontColour = colour;"
							   " vec2 adjustedPos = position - screenBounds.xy;"
							   " pixelPos = adjustedPos;"
							   " vec2 scaledPos = adjustedPos / screenBounds.zw;"
							   " gl_Position = vec4 (scaledPos.x - 1.0, 1.0 - scaledPos.y, 0, 1.0);"
							   "}", GL_VERTEX_SHADER);

			program.addShader (fragmentShader, GL_FRAGMENT_SHADER);
			program.link();
			JUCE_CHECK_OPENGL_ERROR
		}

		OpenGLShaderProgram program;
	};

	struct ShaderBase   : public ShaderProgramHolder
	{
		ShaderBase (OpenGLContext& context, const char* fragmentShader)
			: ShaderProgramHolder (context, fragmentShader),
			  positionAttribute (program, "position"),
			  colourAttribute (program, "colour"),
			  screenBounds (program, "screenBounds")
		{}

		void set2DBounds (const Rectangle<float>& bounds)
		{
			screenBounds.set (bounds.getX(), bounds.getY(), 0.5f * bounds.getWidth(), 0.5f * bounds.getHeight());
		}

		void bindAttributes (OpenGLContext& context)
		{
			context.extensions.glVertexAttribPointer (positionAttribute.attributeID, 2, GL_SHORT, GL_FALSE, 8, (void*) 0);
			context.extensions.glVertexAttribPointer (colourAttribute.attributeID, 4, GL_UNSIGNED_BYTE, GL_TRUE, 8, (void*) 4);
			context.extensions.glEnableVertexAttribArray (positionAttribute.attributeID);
			context.extensions.glEnableVertexAttribArray (colourAttribute.attributeID);
		}

		void unbindAttributes (OpenGLContext& context)
		{
			context.extensions.glDisableVertexAttribArray (positionAttribute.attributeID);
			context.extensions.glDisableVertexAttribArray (colourAttribute.attributeID);
		}

		OpenGLShaderProgram::Attribute positionAttribute, colourAttribute;

	private:
		OpenGLShaderProgram::Uniform screenBounds;
	};

	struct MaskedShaderParams
	{
		MaskedShaderParams (OpenGLShaderProgram& program)
			: maskTexture (program, "maskTexture"),
			  maskBounds  (program, "maskBounds")
		{}

		void setBounds (const Rectangle<int>& area, const Target& target, const GLint textureIndex) const
		{
			maskTexture.set (textureIndex);
			maskBounds.set (area.getX() - target.bounds.getX(),
							area.getY() - target.bounds.getY(),
							area.getWidth(), area.getHeight());
		}

		OpenGLShaderProgram::Uniform maskTexture, maskBounds;
	};

	#define JUCE_DECLARE_VARYING_COLOUR   "varying " JUCE_MEDIUMP " vec4 frontColour;"
	#define JUCE_DECLARE_VARYING_PIXELPOS "varying " JUCE_HIGHP " vec2 pixelPos;"

	struct SolidColourProgram  : public ShaderBase
	{
		SolidColourProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_VARYING_COLOUR
						  "void main() { gl_FragColor = frontColour; }")
		{}
	};

   #if JUCE_ANDROID
	#define JUCE_DECLARE_SWIZZLE_FUNCTION "\n" JUCE_MEDIUMP " vec4 swizzleRGBOrder (in " JUCE_MEDIUMP " vec4 c) { return vec4 (c.b, c.g, c.r, c.a); }\n"
   #else
	#define JUCE_DECLARE_SWIZZLE_FUNCTION "\n" JUCE_MEDIUMP " vec4 swizzleRGBOrder (in " JUCE_MEDIUMP " vec4 c) { return c; }\n"
   #endif

	#define JUCE_DECLARE_MASK_UNIFORMS  "uniform sampler2D maskTexture;" \
										"uniform ivec4 maskBounds;"
	#define JUCE_FRAGCOORD_TO_MASK_POS  "vec2 ((pixelPos.x - float (maskBounds.x)) / float (maskBounds.z)," \
											  "1.0 - (pixelPos.y - float (maskBounds.y)) / float (maskBounds.w))"
	#define JUCE_GET_MASK_ALPHA         "texture2D (maskTexture, " JUCE_FRAGCOORD_TO_MASK_POS ").a"

	struct SolidColourMaskedProgram  : public ShaderBase
	{
		SolidColourMaskedProgram (OpenGLContext& context)
			: ShaderBase (context,
						  JUCE_DECLARE_MASK_UNIFORMS JUCE_DECLARE_VARYING_COLOUR JUCE_DECLARE_VARYING_PIXELPOS
						  "void main() {"
							"gl_FragColor = frontColour * " JUCE_GET_MASK_ALPHA ";"
						  "}"),
			  maskParams (program)
		{}

		MaskedShaderParams maskParams;
	};

	struct RadialGradientParams
	{
		RadialGradientParams (OpenGLShaderProgram& program)
			: gradientTexture (program, "gradientTexture"),
			  matrix (program, "matrix")
		{}

		void setMatrix (const Point<float>& p1, const Point<float>& p2, const Point<float>& p3)
		{
			const AffineTransform t (AffineTransform::fromTargetPoints (p1.x, p1.y,  0.0f, 0.0f,
																		p2.x, p2.y,  1.0f, 0.0f,
																		p3.x, p3.y,  0.0f, 1.0f));
			const GLfloat m[] = { t.mat00, t.mat01, t.mat02, t.mat10, t.mat11, t.mat12 };
			matrix.set (m, 6);
		}

		OpenGLShaderProgram::Uniform gradientTexture, matrix;
	};

	#define JUCE_DECLARE_MATRIX_UNIFORM   "uniform " JUCE_HIGHP " float matrix[6];"
	#define JUCE_DECLARE_RADIAL_UNIFORMS  "uniform sampler2D gradientTexture;" JUCE_DECLARE_MATRIX_UNIFORM
	#define JUCE_MATRIX_TIMES_FRAGCOORD   "(mat2 (matrix[0], matrix[3], matrix[1], matrix[4]) * pixelPos" \
										  " + vec2 (matrix[2], matrix[5]))"
	#define JUCE_GET_TEXTURE_COLOUR       "(frontColour.a * swizzleRGBOrder (texture2D (gradientTexture, vec2 (gradientPos, 0.5))))"

	struct RadialGradientProgram  : public ShaderBase
	{
		RadialGradientProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_VARYING_PIXELPOS
						  JUCE_DECLARE_RADIAL_UNIFORMS JUCE_DECLARE_VARYING_COLOUR JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_MEDIUMP " float gradientPos = length (" JUCE_MATRIX_TIMES_FRAGCOORD ");"
							"gl_FragColor = " JUCE_GET_TEXTURE_COLOUR ";"
						  "}"),
			  gradientParams (program)
		{}

		RadialGradientParams gradientParams;
	};

	struct RadialGradientMaskedProgram  : public ShaderBase
	{
		RadialGradientMaskedProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_VARYING_PIXELPOS
						  JUCE_DECLARE_RADIAL_UNIFORMS JUCE_DECLARE_VARYING_COLOUR
						  JUCE_DECLARE_MASK_UNIFORMS JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_MEDIUMP " float gradientPos = length (" JUCE_MATRIX_TIMES_FRAGCOORD ");"
							"gl_FragColor = " JUCE_GET_TEXTURE_COLOUR " * " JUCE_GET_MASK_ALPHA ";"
						  "}"),
			  gradientParams (program),
			  maskParams (program)
		{}

		RadialGradientParams gradientParams;
		MaskedShaderParams maskParams;
	};

	struct LinearGradientParams
	{
		LinearGradientParams (OpenGLShaderProgram& program)
			: gradientTexture (program, "gradientTexture"),
			  gradientInfo (program, "gradientInfo")
		{}

		OpenGLShaderProgram::Uniform gradientTexture, gradientInfo;
	};

	#define JUCE_DECLARE_LINEAR_UNIFORMS  "uniform sampler2D gradientTexture;" \
										  "uniform " JUCE_MEDIUMP " vec4 gradientInfo;" \
										  JUCE_DECLARE_VARYING_COLOUR JUCE_DECLARE_VARYING_PIXELPOS
	#define JUCE_CALC_LINEAR_GRAD_POS1    JUCE_MEDIUMP " float gradientPos = (pixelPos.y - (gradientInfo.y + (gradientInfo.z * (pixelPos.x - gradientInfo.x)))) / gradientInfo.w;"
	#define JUCE_CALC_LINEAR_GRAD_POS2    JUCE_MEDIUMP " float gradientPos = (pixelPos.x - (gradientInfo.x + (gradientInfo.z * (pixelPos.y - gradientInfo.y)))) / gradientInfo.w;"

	struct LinearGradient1Program  : public ShaderBase
	{
		LinearGradient1Program (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_LINEAR_UNIFORMS  // gradientInfo: x = x1, y = y1, z = (y2 - y1) / (x2 - x1), w = length
						  JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_CALC_LINEAR_GRAD_POS1
							"gl_FragColor = " JUCE_GET_TEXTURE_COLOUR ";"
						  "}"),
			  gradientParams (program)
		{}

		LinearGradientParams gradientParams;
	};

	struct LinearGradient1MaskedProgram  : public ShaderBase
	{
		LinearGradient1MaskedProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_LINEAR_UNIFORMS  // gradientInfo: x = x1, y = y1, z = (y2 - y1) / (x2 - x1), w = length
						  JUCE_DECLARE_MASK_UNIFORMS JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_CALC_LINEAR_GRAD_POS1
							"gl_FragColor = " JUCE_GET_TEXTURE_COLOUR " * " JUCE_GET_MASK_ALPHA ";"
						  "}"),
			  gradientParams (program),
			  maskParams (program)
		{}

		LinearGradientParams gradientParams;
		MaskedShaderParams maskParams;
	};

	struct LinearGradient2Program  : public ShaderBase
	{
		LinearGradient2Program (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_LINEAR_UNIFORMS  // gradientInfo: x = x1, y = y1, z = (x2 - x1) / (y2 - y1), y = y1, w = length
						  JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_CALC_LINEAR_GRAD_POS2
							"gl_FragColor = " JUCE_GET_TEXTURE_COLOUR ";"
						  "}"),
			  gradientParams (program)
		{}

		LinearGradientParams gradientParams;
	};

	struct LinearGradient2MaskedProgram  : public ShaderBase
	{
		LinearGradient2MaskedProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_LINEAR_UNIFORMS  // gradientInfo: x = x1, y = y1, z = (x2 - x1) / (y2 - y1), y = y1, w = length
						  JUCE_DECLARE_MASK_UNIFORMS JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_CALC_LINEAR_GRAD_POS2
							"gl_FragColor = " JUCE_GET_TEXTURE_COLOUR " * " JUCE_GET_MASK_ALPHA ";"
						  "}"),
			  gradientParams (program),
			  maskParams (program)
		{}

		LinearGradientParams gradientParams;
		MaskedShaderParams maskParams;
	};

	struct ImageParams
	{
		ImageParams (OpenGLShaderProgram& program)
			: imageTexture (program, "imageTexture"),
			  matrix (program, "matrix"),
			  imageLimits (program, "imageLimits")
		{}

		void setMatrix (const AffineTransform& trans,
						const int imageWidth, const int imageHeight,
						const float fullWidthProportion, const float fullHeightProportion,
						const float targetX, const float targetY) const
		{
			const AffineTransform t (trans.translated (-targetX, -targetY)
										.inverted().scaled (fullWidthProportion / imageWidth,
															fullHeightProportion / imageHeight));

			const GLfloat m[] = { t.mat00, t.mat01, t.mat02, t.mat10, t.mat11, t.mat12 };
			matrix.set (m, 6);

			const float halfPixelX = 0.5f / imageWidth;
			const float halfPixelY = 0.5f / imageHeight;
			imageLimits.set (halfPixelX, halfPixelY,
							 fullWidthProportion - halfPixelX,
							 fullHeightProportion - halfPixelY);
		}

		void setMatrix (const AffineTransform& trans, const OpenGLTextureFromImage& image,
						const float targetX, const float targetY) const
		{
			setMatrix (trans,
					   image.imageWidth, image.imageHeight,
					   image.fullWidthProportion, image.fullHeightProportion,
					   targetX, targetY);
		}

		OpenGLShaderProgram::Uniform imageTexture, matrix, imageLimits;
	};

	#define JUCE_DECLARE_IMAGE_UNIFORMS "uniform sampler2D imageTexture;" \
										"uniform " JUCE_MEDIUMP " vec4 imageLimits;" \
										JUCE_DECLARE_MATRIX_UNIFORM JUCE_DECLARE_VARYING_COLOUR JUCE_DECLARE_VARYING_PIXELPOS
	#define JUCE_GET_IMAGE_PIXEL        "swizzleRGBOrder (texture2D (imageTexture, vec2 (texturePos.x, 1.0 - texturePos.y)))"
	#define JUCE_CLAMP_TEXTURE_COORD    JUCE_HIGHP " vec2 texturePos = clamp (" JUCE_MATRIX_TIMES_FRAGCOORD ", imageLimits.xy, imageLimits.zw);"
	#define JUCE_MOD_TEXTURE_COORD      JUCE_HIGHP " vec2 texturePos = clamp (mod (" JUCE_MATRIX_TIMES_FRAGCOORD ", imageLimits.zw + imageLimits.xy), imageLimits.xy, imageLimits.zw);"

	struct ImageProgram  : public ShaderBase
	{
		ImageProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_IMAGE_UNIFORMS JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_CLAMP_TEXTURE_COORD
							"gl_FragColor = frontColour.a * " JUCE_GET_IMAGE_PIXEL ";"
						  "}"),
			  imageParams (program)
		{}

		ImageParams imageParams;
	};

	struct ImageMaskedProgram  : public ShaderBase
	{
		ImageMaskedProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_IMAGE_UNIFORMS JUCE_DECLARE_MASK_UNIFORMS JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_CLAMP_TEXTURE_COORD
							"gl_FragColor = frontColour.a * " JUCE_GET_IMAGE_PIXEL " * " JUCE_GET_MASK_ALPHA ";"
						  "}"),
			  imageParams (program),
			  maskParams (program)
		{}

		ImageParams imageParams;
		MaskedShaderParams maskParams;
	};

	struct TiledImageProgram  : public ShaderBase
	{
		TiledImageProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_IMAGE_UNIFORMS JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_MOD_TEXTURE_COORD
							"gl_FragColor = frontColour.a * " JUCE_GET_IMAGE_PIXEL ";"
						  "}"),
			  imageParams (program)
		{}

		ImageParams imageParams;
	};

	struct TiledImageMaskedProgram  : public ShaderBase
	{
		TiledImageMaskedProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_IMAGE_UNIFORMS JUCE_DECLARE_MASK_UNIFORMS JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_MOD_TEXTURE_COORD
							"gl_FragColor = frontColour.a * " JUCE_GET_IMAGE_PIXEL " * " JUCE_GET_MASK_ALPHA ";"
						  "}"),
			  imageParams (program),
			  maskParams (program)
		{}

		ImageParams imageParams;
		MaskedShaderParams maskParams;
	};

	struct CopyTextureProgram  : public ShaderBase
	{
		CopyTextureProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_IMAGE_UNIFORMS JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_MOD_TEXTURE_COORD
							"gl_FragColor = frontColour.a * " JUCE_GET_IMAGE_PIXEL ";"
						  "}"),
			  imageParams (program)
		{}

		ImageParams imageParams;
	};

	struct MaskTextureProgram  : public ShaderBase
	{
		MaskTextureProgram (OpenGLContext& context)
			: ShaderBase (context, JUCE_DECLARE_IMAGE_UNIFORMS JUCE_DECLARE_SWIZZLE_FUNCTION
						  "void main()"
						  "{"
							JUCE_HIGHP " vec2 texturePos = " JUCE_MATRIX_TIMES_FRAGCOORD ";"
							JUCE_HIGHP " float roundingError = 0.00001;"
							"if (texturePos.x >= imageLimits.x - roundingError"
								 "&& texturePos.y >= imageLimits.y - roundingError"
								 "&& texturePos.x <= imageLimits.z + roundingError"
								 "&& texturePos.y <= imageLimits.w + roundingError)"
							 "gl_FragColor = frontColour * " JUCE_GET_IMAGE_PIXEL ".a;"
							"else "
							 "gl_FragColor = vec4 (0, 0, 0, 0);"
						  "}"),
			  imageParams (program)
		{}

		ImageParams imageParams;
	};

	SolidColourProgram solidColourProgram;
	SolidColourMaskedProgram solidColourMasked;
	RadialGradientProgram radialGradient;
	RadialGradientMaskedProgram radialGradientMasked;
	LinearGradient1Program linearGradient1;
	LinearGradient1MaskedProgram linearGradient1Masked;
	LinearGradient2Program linearGradient2;
	LinearGradient2MaskedProgram linearGradient2Masked;
	ImageProgram image;
	ImageMaskedProgram imageMasked;
	TiledImageProgram tiledImage;
	TiledImageMaskedProgram tiledImageMasked;
	CopyTextureProgram copyTexture;
	MaskTextureProgram maskTexture;
};

struct StateHelpers
{
	struct BlendingMode
	{
		BlendingMode() noexcept
			: blendingEnabled (false), srcFunction (0), dstFunction (0)
		{}

		void resync() noexcept
		{
			glDisable (GL_BLEND);
			srcFunction = dstFunction = 0;
		}

		template <class QuadQueueType>
		void setPremultipliedBlendingMode (QuadQueueType& quadQueue) noexcept
		{
			setBlendFunc (quadQueue, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		}

		template <class QuadQueueType>
		void setBlendFunc (QuadQueueType& quadQueue, GLenum src, GLenum dst)
		{
			if (! blendingEnabled)
			{
				quadQueue.flush();
				blendingEnabled = true;
				glEnable (GL_BLEND);
			}

			if (srcFunction != src || dstFunction != dst)
			{
				quadQueue.flush();
				srcFunction = src;
				dstFunction = dst;
				glBlendFunc (src, dst);
			}
		}

		template <class QuadQueueType>
		void disableBlend (QuadQueueType& quadQueue) noexcept
		{
			if (blendingEnabled)
			{
				quadQueue.flush();
				blendingEnabled = false;
				glDisable (GL_BLEND);
			}
		}

		template <class QuadQueueType>
		void setBlendMode (QuadQueueType& quadQueue, const bool replaceExistingContents) noexcept
		{
			if (replaceExistingContents)
				disableBlend (quadQueue);
			else
				setPremultipliedBlendingMode (quadQueue);
		}

	private:
		bool blendingEnabled;
		GLenum srcFunction, dstFunction;
	};

	template <class QuadQueueType>
	struct EdgeTableRenderer
	{
		EdgeTableRenderer (QuadQueueType& quadQueue_, const PixelARGB& colour_) noexcept
			: quadQueue (quadQueue_), colour (colour_)
		{}

		void setEdgeTableYPos (const int y) noexcept
		{
			currentY = y;
		}

		void handleEdgeTablePixel (const int x, const int alphaLevel) noexcept
		{
			PixelARGB c (colour);
			c.multiplyAlpha (alphaLevel);
			quadQueue.add (x, currentY, 1, 1, c);
		}

		void handleEdgeTablePixelFull (const int x) noexcept
		{
			quadQueue.add (x, currentY, 1, 1, colour);
		}

		void handleEdgeTableLine (const int x, const int width, const int alphaLevel) noexcept
		{
			PixelARGB c (colour);
			c.multiplyAlpha (alphaLevel);
			quadQueue.add (x, currentY, width, 1, c);
		}

		void handleEdgeTableLineFull (const int x, const int width) noexcept
		{
			quadQueue.add (x, currentY, width, 1, colour);
		}

	private:
		QuadQueueType& quadQueue;
		const PixelARGB colour;
		int currentY;

		JUCE_DECLARE_NON_COPYABLE (EdgeTableRenderer);
	};

	template <class QuadQueueType>
	struct FloatRectangleRenderer
	{
		FloatRectangleRenderer (QuadQueueType& quadQueue_, const PixelARGB& colour_) noexcept
			: quadQueue (quadQueue_), colour (colour_)
		{}

		void operator() (const int x, const int y, const int w, const int h, const int alpha) noexcept
		{
			if (w > 0 && h > 0)
			{
				PixelARGB c (colour);
				c.multiplyAlpha (alpha);
				quadQueue.add (x, y, w, h, c);
			}
		}

	private:
		QuadQueueType& quadQueue;
		const PixelARGB colour;

		JUCE_DECLARE_NON_COPYABLE (FloatRectangleRenderer);
	};

	struct ActiveTextures
	{
		ActiveTextures (const OpenGLContext& context_) noexcept
			: texturesEnabled (0), currentActiveTexture (0), context (context_)
		{}

		void clear() noexcept
		{
			zeromem (currentTextureID, sizeof (currentTextureID));
		}

		void clearCurrent() noexcept
		{
			currentTextureID [currentActiveTexture] = 0;
		}

		template <class QuadQueueType>
		void setTexturesEnabled (QuadQueueType& quadQueue, const int textureIndexMask) noexcept
		{
			if (texturesEnabled != textureIndexMask)
			{
				quadQueue.flush();

				for (int i = 3; --i >= 0;)
				{
					if ((texturesEnabled & (1 << i)) != (textureIndexMask & (1 << i)))
					{
						setActiveTexture (i);
						JUCE_CHECK_OPENGL_ERROR

					   #if ! JUCE_ANDROID
						if ((textureIndexMask & (1 << i)) != 0)
							glEnable (GL_TEXTURE_2D);
						else
						{
							glDisable (GL_TEXTURE_2D);
							currentTextureID[i] = 0;
						}

						clearGLError();
					   #endif
					}
				}

				texturesEnabled = textureIndexMask;
			}
		}

		template <class QuadQueueType>
		void disableTextures (QuadQueueType& quadQueue) noexcept
		{
			setTexturesEnabled (quadQueue, 0);
		}

		template <class QuadQueueType>
		void setSingleTextureMode (QuadQueueType& quadQueue) noexcept
		{
			setTexturesEnabled (quadQueue, 1);
			setActiveTexture (0);
		}

		template <class QuadQueueType>
		void setTwoTextureMode (QuadQueueType& quadQueue, GLuint texture1, GLuint texture2)
		{
			JUCE_CHECK_OPENGL_ERROR
			setTexturesEnabled (quadQueue, 3);

			if (currentActiveTexture == 0)
			{
				bindTexture (texture1);
				setActiveTexture (1);
				bindTexture (texture2);
			}
			else
			{
				setActiveTexture (1);
				bindTexture (texture2);
				setActiveTexture (0);
				bindTexture (texture1);
			}
			JUCE_CHECK_OPENGL_ERROR
		}

		void setActiveTexture (const int index) noexcept
		{
			if (currentActiveTexture != index)
			{
				currentActiveTexture = index;
				context.extensions.glActiveTexture (GL_TEXTURE0 + index);
				JUCE_CHECK_OPENGL_ERROR
			}
		}

		void bindTexture (const GLuint textureID) noexcept
		{
			if (currentTextureID [currentActiveTexture] != textureID)
			{
				currentTextureID [currentActiveTexture] = textureID;
				glBindTexture (GL_TEXTURE_2D, textureID);
				JUCE_CHECK_OPENGL_ERROR
			}
			else
			{
			   #if JUCE_DEBUG
				GLint t = 0;
				glGetIntegerv (GL_TEXTURE_BINDING_2D, &t);
				jassert (t == (GLint) textureID);
			   #endif
			}
		}

	private:
		GLuint currentTextureID [3];
		int texturesEnabled, currentActiveTexture;
		const OpenGLContext& context;

		ActiveTextures& operator= (const ActiveTextures&);
	};

	struct TextureCache
	{
		TextureCache() noexcept
			: activeGradientIndex (0), gradientNeedsRefresh (true)
		{}

		OpenGLTexture* getTexture (ActiveTextures& activeTextures, int w, int h)
		{
			if (textures.size() < numTexturesToCache)
			{
				activeTextures.clear();
				return new OpenGLTexture();
			}

			for (int i = 0; i < numTexturesToCache - 2; ++i)
			{
				const OpenGLTexture* const t = textures.getUnchecked(i);
				if (t->getWidth() == w && t->getHeight() == h)
					return textures.removeAndReturn (i);
			}

			return textures.removeAndReturn (0);
		}

		void releaseTexture (ActiveTextures& activeTextures, OpenGLTexture* texture)
		{
			activeTextures.clearCurrent();
			textures.add (texture);
		}

		void resetGradient() noexcept
		{
			gradientNeedsRefresh = true;
		}

		void bindTextureForGradient (ActiveTextures& activeTextures, const ColourGradient& gradient)
		{
			if (gradientNeedsRefresh)
			{
				gradientNeedsRefresh = false;

				if (gradientTextures.size() < numGradientTexturesToCache)
				{
					activeGradientIndex = gradientTextures.size();
					activeTextures.clear();
					gradientTextures.add (new OpenGLTexture());
				}
				else
				{
					activeGradientIndex = (activeGradientIndex + 1) % numGradientTexturesToCache;
				}

				JUCE_CHECK_OPENGL_ERROR;
				PixelARGB lookup [gradientTextureSize];
				gradient.createLookupTable (lookup, gradientTextureSize);
				gradientTextures.getUnchecked (activeGradientIndex)->loadARGB (lookup, gradientTextureSize, 1);
			}

			activeTextures.bindTexture (gradientTextures.getUnchecked (activeGradientIndex)->getTextureID());
		}

		enum { gradientTextureSize = 256 };

	private:
		enum { numTexturesToCache = 8, numGradientTexturesToCache = 10 };
		OwnedArray<OpenGLTexture> textures, gradientTextures;
		int activeGradientIndex;
		bool gradientNeedsRefresh;
	};

	struct ShaderQuadQueue
	{
		ShaderQuadQueue (const OpenGLContext& context_) noexcept
			: context (context_), numVertices (0)
		{}

		~ShaderQuadQueue() noexcept
		{
			static_jassert (sizeof (VertexInfo) == 8);
			context.extensions.glDeleteBuffers (2, buffers);
		}

		void initialise() noexcept
		{
			JUCE_CHECK_OPENGL_ERROR
			for (int i = 0, v = 0; i < numQuads * 6; i += 6, v += 4)
			{
				indexData[i] = (GLushort) v;
				indexData[i + 1] = indexData[i + 3] = (GLushort) (v + 1);
				indexData[i + 2] = indexData[i + 4] = (GLushort) (v + 2);
				indexData[i + 5] = (GLushort) (v + 3);
			}

			context.extensions.glGenBuffers (2, buffers);
			context.extensions.glBindBuffer (GL_ARRAY_BUFFER, buffers[0]);
			context.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
			context.extensions.glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indexData), indexData, GL_STATIC_DRAW);
			JUCE_CHECK_OPENGL_ERROR
		}

		void add (const int x, const int y, const int w, const int h, const PixelARGB& colour) noexcept
		{
			jassert (w > 0 && h > 0);

			VertexInfo* const v = vertexData + numVertices;
			v[0].x = v[2].x = (GLshort) x;
			v[0].y = v[1].y = (GLshort) y;
			v[1].x = v[3].x = (GLshort) (x + w);
			v[2].y = v[3].y = (GLshort) (y + h);

			const GLuint rgba = colour.getInRGBAMemoryOrder();
			v[0].colour = rgba;
			v[1].colour = rgba;
			v[2].colour = rgba;
			v[3].colour = rgba;

			numVertices += 4;

			if (numVertices > numQuads * 4 - 4)
				draw();
		}

		void add (const Rectangle<int>& r, const PixelARGB& colour) noexcept
		{
			add (r.getX(), r.getY(), r.getWidth(), r.getHeight(), colour);
		}

		void add (const Rectangle<float>& r, const PixelARGB& colour) noexcept
		{
			FloatRectangleRenderer<ShaderQuadQueue> frr (*this, colour);
			RenderingHelpers::FloatRectangleRasterisingInfo (r).iterate (frr);
		}

		void add (const RectangleList& list, const PixelARGB& colour) noexcept
		{
			for (RectangleList::Iterator i (list); i.next();)
				add (*i.getRectangle(), colour);
		}

		void add (const RectangleList& list, const Rectangle<int>& clip, const PixelARGB& colour) noexcept
		{
			for (RectangleList::Iterator i (list); i.next();)
			{
				const Rectangle<int> r (i.getRectangle()->getIntersection (clip));

				if (! r.isEmpty())
					add (r, colour);
			}
		}

		void add (const EdgeTable& et, const PixelARGB& colour)
		{
			EdgeTableRenderer<ShaderQuadQueue> etr (*this, colour);
			et.iterate (etr);
		}

		void flush() noexcept
		{
			if (numVertices > 0)
				draw();
		}

	private:
		struct VertexInfo
		{
			GLshort x, y;
			GLuint colour;
		};

	   #if ! (JUCE_MAC || JUCE_ANDROID || JUCE_IOS)
		enum { numQuads = 64 }; // (had problems with my drivers segfaulting when these buffers are any larger)
	   #else
		enum { numQuads = 8192 };
	   #endif

		GLuint buffers[2];
		VertexInfo vertexData [numQuads * 4];
		GLushort indexData [numQuads * 6];
		const OpenGLContext& context;
		int numVertices;

		void draw() noexcept
		{
			context.extensions.glBufferData (GL_ARRAY_BUFFER, numVertices * sizeof (VertexInfo), vertexData, GL_DYNAMIC_DRAW);
			glDrawElements (GL_TRIANGLES, (numVertices * 3) / 2, GL_UNSIGNED_SHORT, 0);
			numVertices = 0;
		}

		ShaderQuadQueue& operator= (const ShaderQuadQueue&);
	};

	struct CurrentShader
	{
		CurrentShader (OpenGLContext& context_) noexcept
			: context (context_),
			  activeShader (nullptr)
		{
			const char programValueID[] = "GraphicsContextPrograms";
			programs = static_cast <ShaderPrograms*> (context.getAssociatedObject (programValueID));

			if (programs == nullptr)
			{
				programs = new ShaderPrograms (context);
				context.setAssociatedObject (programValueID, programs);
			}
		}

		void setShader (const Rectangle<int>& bounds, ShaderQuadQueue& quadQueue, ShaderPrograms::ShaderBase& shader)
		{
			if (activeShader != &shader)
			{
				quadQueue.flush();
				activeShader = &shader;
				shader.program.use();
				shader.bindAttributes (context);

				currentBounds = bounds;
				shader.set2DBounds (bounds.toFloat());

				JUCE_CHECK_OPENGL_ERROR
			}
			else if (bounds != currentBounds)
			{
				currentBounds = bounds;
				shader.set2DBounds (bounds.toFloat());
			}
		}

		void setShader (Target& target, ShaderQuadQueue& quadQueue, ShaderPrograms::ShaderBase& shader)
		{
			setShader (target.bounds, quadQueue, shader);
		}

		void clearShader (ShaderQuadQueue& quadQueue)
		{
			if (activeShader != nullptr)
			{
				quadQueue.flush();
				activeShader->unbindAttributes (context);
				activeShader = nullptr;
				context.extensions.glUseProgram (0);
			}
		}

		OpenGLContext& context;
		ShaderPrograms::Ptr programs;

	private:
		ShaderPrograms::ShaderBase* activeShader;
		Rectangle<int> currentBounds;

		CurrentShader& operator= (const CurrentShader&);
	};
};

class GLState
{
public:
	GLState (const Target& target_) noexcept
		: target (target_),
		  activeTextures (target_.context),
		  currentShader (target_.context),
		  shaderQuadQueue (target_.context),
		  previousFrameBufferTarget (OpenGLFrameBuffer::getCurrentFrameBufferTarget())
	{
		// This object can only be created and used when the current thread has an active OpenGL context.
		jassert (OpenGLHelpers::isContextActive());

		JUCE_CHECK_OPENGL_ERROR
		target.makeActive();
		blendMode.resync();
		JUCE_CHECK_OPENGL_ERROR

	   #ifdef GL_COLOR_ARRAY
		glDisableClientState (GL_COLOR_ARRAY);
		glDisableClientState (GL_NORMAL_ARRAY);
		glDisableClientState (GL_VERTEX_ARRAY);
		glDisableClientState (GL_INDEX_ARRAY);

		for (int i = 3; --i >= 0;)
		{
			activeTextures.setActiveTexture (i);
			glDisableClientState (GL_TEXTURE_COORD_ARRAY);
		}
	   #endif

		JUCE_CHECK_OPENGL_ERROR
		activeTextures.clear();
		shaderQuadQueue.initialise();
		JUCE_CHECK_OPENGL_ERROR
	}

	~GLState()
	{
		flush();
		target.context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, previousFrameBufferTarget);

	   #if defined (GL_INDEX_ARRAY)
		glDisableClientState (GL_INDEX_ARRAY);
	   #endif

		target.context.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
		target.context.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void flush()
	{
		currentShader.clearShader (shaderQuadQueue);
		shaderQuadQueue.flush();
		JUCE_CHECK_OPENGL_ERROR
	}

	void setShader (ShaderPrograms::ShaderBase& shader)
	{
		currentShader.setShader (target, shaderQuadQueue, shader);
		JUCE_CHECK_OPENGL_ERROR
	}

	void setShaderForGradientFill (const ColourGradient& g, const AffineTransform& transform,
								   const int maskTextureID, const Rectangle<int>* const maskArea)
	{
		JUCE_CHECK_OPENGL_ERROR
		activeTextures.disableTextures (shaderQuadQueue);
		blendMode.setPremultipliedBlendingMode (shaderQuadQueue);
		JUCE_CHECK_OPENGL_ERROR

		if (maskArea != nullptr)
		{
			activeTextures.setTexturesEnabled (shaderQuadQueue, 3);
			activeTextures.setActiveTexture (1);
			activeTextures.bindTexture (maskTextureID);
			activeTextures.setActiveTexture (0);
			textureCache.bindTextureForGradient (activeTextures, g);
		}
		else
		{
			activeTextures.setSingleTextureMode (shaderQuadQueue);
			textureCache.bindTextureForGradient (activeTextures, g);
		}

		const AffineTransform t (transform.translated ((float) -target.bounds.getX(), (float) -target.bounds.getY()));
		Point<float> p1 (g.point1.transformedBy (t));
		const Point<float> p2 (g.point2.transformedBy (t));
		const Point<float> p3 (Point<float> (g.point1.x + (g.point2.y - g.point1.y),
											 g.point1.y - (g.point2.x - g.point1.x)).transformedBy (t));

		ShaderPrograms* const programs = currentShader.programs;
		const ShaderPrograms::MaskedShaderParams* maskParams = nullptr;

		if (g.isRadial)
		{
			ShaderPrograms::RadialGradientParams* gradientParams;

			if (maskArea == nullptr)
			{
				setShader (programs->radialGradient);
				gradientParams = &programs->radialGradient.gradientParams;
			}
			else
			{
				setShader (programs->radialGradientMasked);
				gradientParams = &programs->radialGradientMasked.gradientParams;
				maskParams = &programs->radialGradientMasked.maskParams;
			}

			gradientParams->setMatrix (p1, p2, p3);
		}
		else
		{
			p1 = Line<float> (p1, p3).findNearestPointTo (p2);
			const Point<float> delta (p2.x - p1.x, p1.y - p2.y);
			const ShaderPrograms::LinearGradientParams* gradientParams;
			float grad, length;

			if (std::abs (delta.x) < std::abs (delta.y))
			{
				if (maskArea == nullptr)
				{
					setShader (programs->linearGradient1);
					gradientParams = &(programs->linearGradient1.gradientParams);
				}
				else
				{
					setShader (programs->linearGradient1Masked);
					gradientParams = &(programs->linearGradient1Masked.gradientParams);
					maskParams = &programs->linearGradient1Masked.maskParams;
				}

				grad = delta.x / delta.y;
				length = (p2.y - grad * p2.x) - (p1.y - grad * p1.x);
			}
			else
			{
				if (maskArea == nullptr)
				{
					setShader (programs->linearGradient2);
					gradientParams = &(programs->linearGradient2.gradientParams);
				}
				else
				{
					setShader (programs->linearGradient2Masked);
					gradientParams = &(programs->linearGradient2Masked.gradientParams);
					maskParams = &programs->linearGradient2Masked.maskParams;
				}

				grad = delta.y / delta.x;
				length = (p2.x - grad * p2.y) - (p1.x - grad * p1.y);
			}

			gradientParams->gradientInfo.set (p1.x, p1.y, grad, length);
		}

		if (maskParams != nullptr)
			maskParams->setBounds (*maskArea, target, 1);

		JUCE_CHECK_OPENGL_ERROR
	}

	void setShaderForTiledImageFill (const OpenGLTextureFromImage& image, const AffineTransform& transform,
									 const int maskTextureID, const Rectangle<int>* const maskArea, const bool clampTiledImages)
	{
		blendMode.setPremultipliedBlendingMode (shaderQuadQueue);

		ShaderPrograms* const programs = currentShader.programs;

		const ShaderPrograms::MaskedShaderParams* maskParams = nullptr;
		const ShaderPrograms::ImageParams* imageParams;

		if (maskArea != nullptr)
		{
			activeTextures.setTwoTextureMode (shaderQuadQueue, image.textureID, maskTextureID);

			if (clampTiledImages)
			{
				setShader (programs->imageMasked);
				imageParams = &programs->imageMasked.imageParams;
				maskParams = &programs->imageMasked.maskParams;
			}
			else
			{
				setShader (programs->tiledImageMasked);
				imageParams = &programs->tiledImageMasked.imageParams;
				maskParams = &programs->tiledImageMasked.maskParams;
			}
		}
		else
		{
			activeTextures.setSingleTextureMode (shaderQuadQueue);
			activeTextures.bindTexture (image.textureID);

			if (clampTiledImages)
			{
				setShader (programs->image);
				imageParams = &programs->image.imageParams;
			}
			else
			{
				setShader (programs->tiledImage);
				imageParams = &programs->tiledImage.imageParams;
			}
		}

		imageParams->setMatrix (transform, image, (float) target.bounds.getX(), (float) target.bounds.getY());

		if (maskParams != nullptr)
			maskParams->setBounds (*maskArea, target, 1);
	}

	Target target;

	StateHelpers::BlendingMode blendMode;
	StateHelpers::ActiveTextures activeTextures;
	StateHelpers::TextureCache textureCache;
	StateHelpers::CurrentShader currentShader;
	StateHelpers::ShaderQuadQueue shaderQuadQueue;

private:
	GLuint previousFrameBufferTarget;
};

class ClipRegionBase  : public SingleThreadedReferenceCountedObject
{
public:
	ClipRegionBase (GLState& state_) noexcept : state (state_) {}
	virtual ~ClipRegionBase() {}

	typedef ReferenceCountedObjectPtr<ClipRegionBase> Ptr;

	virtual Ptr clone() const = 0;
	virtual Ptr clipToRectangle (const Rectangle<int>&) = 0;
	virtual Ptr clipToRectangleList (const RectangleList&) = 0;
	virtual Ptr excludeClipRectangle (const Rectangle<int>&) = 0;
	virtual Ptr clipToPath (const Path& p, const AffineTransform&) = 0;
	virtual Ptr clipToImageAlpha (const OpenGLTextureFromImage&, const AffineTransform&) = 0;
	virtual Ptr clipToTexture (const PositionedTexture&) = 0;
	virtual Rectangle<int> getClipBounds() const = 0;
	virtual void fillRect (const Rectangle<int>& area, const FillType&, bool replaceContents) = 0;
	virtual void fillRect (const Rectangle<float>& area, const FillType&) = 0;
	virtual void fillEdgeTable (EdgeTable& et, const FillType& fill) = 0;
	virtual void drawImage (const Image&, const AffineTransform&, float alpha,
							const Rectangle<int>& clip, EdgeTable* mask) = 0;

	GLState& state;

	JUCE_DECLARE_NON_COPYABLE (ClipRegionBase);
};

class ClipRegion_Mask  : public ClipRegionBase
{
public:
	ClipRegion_Mask (const ClipRegion_Mask& other)
		: ClipRegionBase (other.state),
		  clip (other.clip),
		  maskArea (other.clip)
	{
		TargetSaver ts (state.target.context);
		state.currentShader.clearShader (state.shaderQuadQueue);
		state.shaderQuadQueue.flush();
		state.activeTextures.setSingleTextureMode (state.shaderQuadQueue);
		state.activeTextures.clear();
		mask.initialise (state.target.context, maskArea.getWidth(), maskArea.getHeight());
		maskArea.setSize (mask.getWidth(), mask.getHeight());
		makeActive();

		state.blendMode.disableBlend (state.shaderQuadQueue);
		state.activeTextures.setSingleTextureMode (state.shaderQuadQueue);
		state.activeTextures.bindTexture (other.mask.getTextureID());

		state.currentShader.setShader (maskArea, state.shaderQuadQueue, state.currentShader.programs->copyTexture);
		state.currentShader.programs->copyTexture.imageParams.imageTexture.set (0);
		state.currentShader.programs->copyTexture.imageParams
			.setMatrix (AffineTransform::translation ((float) other.maskArea.getX(), (float) other.maskArea.getY()),
						other.maskArea.getWidth(), other.maskArea.getHeight(), 1.0f, 1.0f,
						(float) maskArea.getX(), (float) maskArea.getY());

		state.shaderQuadQueue.add (clip, PixelARGB (0xffffffff));
		state.shaderQuadQueue.flush();
	}

	ClipRegion_Mask (GLState& state_, const RectangleList& r)
		: ClipRegionBase (state_),
		  clip (r.getBounds()),
		  maskArea (clip)
	{
		TargetSaver ts (state.target.context);
		state.currentShader.clearShader (state.shaderQuadQueue);
		state.shaderQuadQueue.flush();
		state.activeTextures.clear();
		mask.initialise (state.target.context, maskArea.getWidth(), maskArea.getHeight());
		maskArea.setSize (mask.getWidth(), mask.getHeight());
		mask.makeCurrentAndClear();
		makeActive();
		state.blendMode.setBlendMode (state.shaderQuadQueue, true);
		state.currentShader.setShader (maskArea, state.shaderQuadQueue, state.currentShader.programs->solidColourProgram);
		state.shaderQuadQueue.add (r, PixelARGB (0xffffffff));
		state.shaderQuadQueue.flush();
	}

	Ptr clone() const                       { return new ClipRegion_Mask (*this); }
	Rectangle<int> getClipBounds() const    { return clip; }

	Ptr clipToRectangle (const Rectangle<int>& r)
	{
		clip = clip.getIntersection (r);
		return clip.isEmpty() ? nullptr : this;
	}

	Ptr clipToRectangleList (const RectangleList& r)
	{
		clip = clip.getIntersection (r.getBounds());
		if (clip.isEmpty())
			return nullptr;

		RectangleList excluded (clip);

		if (excluded.subtract (r))
		{
			if (excluded.getNumRectangles() == 1)
				return excludeClipRectangle (excluded.getRectangle (0));

			TargetSaver ts (state.target.context);
			makeActive();
			state.blendMode.setBlendMode (state.shaderQuadQueue, true);
			state.currentShader.setShader (maskArea, state.shaderQuadQueue, state.currentShader.programs->solidColourProgram);
			state.shaderQuadQueue.add (excluded, PixelARGB (0));
			state.shaderQuadQueue.flush();
		}

		return this;
	}

	Ptr excludeClipRectangle (const Rectangle<int>& r)
	{
		if (r.contains (clip))
			return nullptr;

		TargetSaver ts (state.target.context);
		makeActive();
		state.blendMode.setBlendMode (state.shaderQuadQueue, true);
		state.currentShader.setShader (maskArea, state.shaderQuadQueue, state.currentShader.programs->solidColourProgram);
		state.shaderQuadQueue.add (r, PixelARGB (0));
		state.shaderQuadQueue.flush();
		return this;
	}

	Ptr clipToPath (const Path& p, const AffineTransform& t)
	{
		EdgeTable et (clip, p, t);

		if (! et.isEmpty())
		{
			TargetSaver ts (state.target.context);
			state.currentShader.clearShader (state.shaderQuadQueue);
			state.shaderQuadQueue.flush();
			state.activeTextures.clear();

			OpenGLTexture texture;
			PositionedTexture pt (texture, et, clip);
			return clipToTexture (pt);
		}

		return nullptr;
	}

	Ptr clipToTexture (const PositionedTexture& pt)
	{
		clip = clip.getIntersection (pt.clip);

		if (clip.isEmpty())
			return nullptr;

		TargetSaver ts (state.target.context);
		makeActive();

		state.activeTextures.setSingleTextureMode (state.shaderQuadQueue);
		state.activeTextures.bindTexture (pt.textureID);

		state.currentShader.setShader (maskArea, state.shaderQuadQueue, state.currentShader.programs->maskTexture);
		state.currentShader.programs->maskTexture.imageParams.imageTexture.set (0);
		state.currentShader.programs->maskTexture.imageParams
			.setMatrix (AffineTransform::translation ((float) pt.area.getX(), (float) pt.area.getY()),
						pt.area.getWidth(), pt.area.getHeight(), 1.0f, 1.0f,
						(float) maskArea.getX(), (float) maskArea.getY());

		state.blendMode.setBlendFunc (state.shaderQuadQueue, GL_ZERO, GL_SRC_ALPHA);
		state.shaderQuadQueue.add (clip, PixelARGB (0xffffffff));
		state.shaderQuadQueue.flush();
		return this;
	}

	Ptr clipToImageAlpha (const OpenGLTextureFromImage& image, const AffineTransform& transform)
	{
		TargetSaver ts (state.target.context);
		makeActive();
		state.activeTextures.setSingleTextureMode (state.shaderQuadQueue);
		state.activeTextures.bindTexture (image.textureID);

		state.currentShader.setShader (maskArea, state.shaderQuadQueue, state.currentShader.programs->maskTexture);
		state.currentShader.programs->maskTexture.imageParams.imageTexture.set (0);
		state.currentShader.programs->maskTexture.imageParams
			.setMatrix (transform, image, (float) maskArea.getX(), (float) maskArea.getY());

		state.shaderQuadQueue.add (clip, PixelARGB (0xffffffff));
		state.shaderQuadQueue.flush();
		return this;
	}

	void fillRect (const Rectangle<int>& area, const FillType& fill, bool replaceContents)
	{
		(void) replaceContents; jassert (! replaceContents);
		const Rectangle<int> r (clip.getIntersection (area));

		if (! r.isEmpty())
		{
			ShaderFillOperation fillOp (*this, fill, false);
			state.shaderQuadQueue.add (r, fill.colour.getPixelARGB());
		}
	}

	void fillRect (const Rectangle<float>& area, const FillType& fill)
	{
		ShaderFillOperation fillOp (*this, fill, false);

		FloatRectangleRenderer frr (*this, fill);
		RenderingHelpers::FloatRectangleRasterisingInfo (area).iterate (frr);
	}

	void fillEdgeTable (EdgeTable& et, const FillType& fill)
	{
		if (et.getMaximumBounds().intersects (clip))
		{
			if (! clip.contains (et.getMaximumBounds()))
				et.clipToRectangle (clip);

			ShaderFillOperation fillOp (*this, fill, false);
			state.shaderQuadQueue.add (et, fill.colour.getPixelARGB());
		}
	}

	void drawImage (const Image& image, const AffineTransform& transform,
					float alpha, const Rectangle<int>& clipArea, EdgeTable* et)
	{
		const Rectangle<int> r (clip.getIntersection (clipArea));

		if (! r.isEmpty())
		{
			const PixelARGB colour (Colours::white.withAlpha (alpha).getPixelARGB());
			ShaderFillOperation fillOp (*this, FillType (image, transform), true);

			if (et != nullptr)
			{
				et->clipToRectangle (r);

				if (! et->isEmpty())
					state.shaderQuadQueue.add (*et, colour);
			}
			else
			{
				state.shaderQuadQueue.add (r, colour);
			}
		}

		state.currentShader.clearShader (state.shaderQuadQueue);
	}

private:
	OpenGLFrameBuffer mask;
	Rectangle<int> clip, maskArea;

	struct ShaderFillOperation
	{
		ShaderFillOperation (const ClipRegion_Mask& clip, const FillType& fill, const bool clampTiledImages)
			: state (clip.state)
		{
			const GLuint maskTextureID = clip.mask.getTextureID();

			if (fill.isColour())
			{
				state.blendMode.setPremultipliedBlendingMode (state.shaderQuadQueue);
				state.activeTextures.setSingleTextureMode (state.shaderQuadQueue);
				state.activeTextures.bindTexture (maskTextureID);

				state.setShader (state.currentShader.programs->solidColourMasked);
				state.currentShader.programs->solidColourMasked.maskParams.setBounds (clip.maskArea, state.target, 0);
			}
			else if (fill.isGradient())
			{
				state.setShaderForGradientFill (*fill.gradient, fill.transform, maskTextureID, &clip.maskArea);
			}
			else
			{
				jassert (fill.isTiledImage());
				image = new OpenGLTextureFromImage (fill.image);
				state.setShaderForTiledImageFill (*image, fill.transform, maskTextureID, &clip.maskArea, clampTiledImages);
			}
		}

		~ShaderFillOperation()
		{
			state.shaderQuadQueue.flush();
		}

		GLState& state;
		ScopedPointer<OpenGLTextureFromImage> image;

		JUCE_DECLARE_NON_COPYABLE (ShaderFillOperation);
	};

	struct TargetSaver
	{
		TargetSaver (const OpenGLContext& context_)
			: context (context_), oldFramebuffer (OpenGLFrameBuffer::getCurrentFrameBufferTarget())
		{
			glGetIntegerv (GL_VIEWPORT, oldViewport);
		}

		~TargetSaver()
		{
			context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, oldFramebuffer);
			glViewport (oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
		}

	private:
		const OpenGLContext& context;
		GLuint oldFramebuffer;
		GLint oldViewport[4];

		TargetSaver& operator= (const TargetSaver&);
	};

	void makeActive()
	{
		state.shaderQuadQueue.flush();
		state.activeTextures.clear();
		mask.makeCurrentRenderingTarget();
		glViewport (0, 0, maskArea.getWidth(), maskArea.getHeight());
	}

	struct FloatRectangleRenderer
	{
		FloatRectangleRenderer (ClipRegion_Mask& owner_, const FillType& fill_) noexcept
			: owner (owner_), originalColour (fill_.colour.getPixelARGB())
		{}

		void operator() (int x, int y, int w, int h, const int alpha) noexcept
		{
			if (owner.clip.intersectRectangle (x, y, w, h))
			{
				PixelARGB col (originalColour);
				col.multiplyAlpha (alpha);
				owner.state.shaderQuadQueue.add (x, y, w, h, col);
			}
		}

	private:
		ClipRegion_Mask& owner;
		const PixelARGB originalColour;

		JUCE_DECLARE_NON_COPYABLE (FloatRectangleRenderer);
	};

	ClipRegion_Mask& operator= (const ClipRegion_Mask&);
};

class ClipRegion_RectangleList  : public ClipRegionBase
{
public:
	ClipRegion_RectangleList (GLState& state_, const Rectangle<int>& r) noexcept
		: ClipRegionBase (state_), clip (r)
	{}

	ClipRegion_RectangleList (GLState& state_, const RectangleList& r) noexcept
		: ClipRegionBase (state_), clip (r)
	{}

	Ptr clone() const       { return new ClipRegion_RectangleList (state, clip); }

	Ptr clipToTexture (const PositionedTexture& t)                                                  { return toMask()->clipToTexture (t); }
	Ptr clipToPath (const Path& p, const AffineTransform& transform)                                { return toMask()->clipToPath (p, transform); }
	Ptr clipToImageAlpha (const OpenGLTextureFromImage& image, const AffineTransform& transform)    { return toMask()->clipToImageAlpha (image, transform); }

	void fillRect (const Rectangle<int>& area, const FillType& fill, bool replaceContents)
	{
		ShaderFillOperation fillOp (*this, fill, replaceContents || fill.colour.isOpaque(), false);
		state.shaderQuadQueue.add (clip, area, fill.colour.getPixelARGB());
	}

	void fillRect (const Rectangle<float>& area, const FillType& fill)
	{
		const PixelARGB colour (fill.colour.getPixelARGB());
		ShaderFillOperation fillOp (*this, fill, false, false);

		for (RectangleList::Iterator i (clip); i.next();)
		{
			const Rectangle<float> r (i.getRectangle()->toFloat().getIntersection (area));
			if (! r.isEmpty())
				state.shaderQuadQueue.add (r, colour);
		}
	}

	void drawImage (const Image& image, const AffineTransform& transform,
					float alpha, const Rectangle<int>& clipArea, EdgeTable* et)
	{
		FillType fill (image, transform);
		const PixelARGB colour (Colours::white.withAlpha (alpha).getPixelARGB());

		ShaderFillOperation fillOp (*this, fill, false, true);

		if (et != nullptr)
		{
			if (! clip.containsRectangle (et->getMaximumBounds()))
				et->clipToEdgeTable (EdgeTable (clip));

			state.shaderQuadQueue.add (*et, colour);
		}
		else
		{
			state.shaderQuadQueue.add (clip, clipArea, colour);
		}

		state.currentShader.clearShader (state.shaderQuadQueue);
	}

	void fillEdgeTable (EdgeTable& et, const FillType& fill)
	{
		if (clip.intersects (et.getMaximumBounds()))
		{
			if (! clip.containsRectangle (et.getMaximumBounds()))
				et.clipToEdgeTable (EdgeTable (clip));

			ShaderFillOperation fillOp (*this, fill, false, true);
			state.shaderQuadQueue.add (et, fill.colour.getPixelARGB());
		}
	}

	Rectangle<int> getClipBounds() const                { return clip.getBounds(); }
	Ptr clipToRectangle (const Rectangle<int>& r)       { return clip.clipTo (r) ? this : nullptr; }
	Ptr clipToRectangleList (const RectangleList& r)    { return clip.clipTo (r) ? this : nullptr; }
	Ptr excludeClipRectangle (const Rectangle<int>& r)  { clip.subtract (r); return clip.isEmpty() ? nullptr : this; }

private:
	RectangleList clip;

	Ptr toMask() const    { return new ClipRegion_Mask (state, clip); }

	struct ShaderFillOperation
	{
		ShaderFillOperation (const ClipRegion_RectangleList& clip, const FillType& fill,
							 const bool replaceContents, const bool clampTiledImages)
			: state (clip.state)
		{
			if (fill.isColour())
			{
				state.activeTextures.disableTextures (state.shaderQuadQueue);
				state.blendMode.setBlendMode (state.shaderQuadQueue, replaceContents);
				state.setShader (state.currentShader.programs->solidColourProgram);
			}
			else if (fill.isGradient())
			{
				state.setShaderForGradientFill (*fill.gradient, fill.transform, 0, nullptr);
			}
			else
			{
				jassert (fill.isTiledImage());
				image = new OpenGLTextureFromImage (fill.image);
				state.setShaderForTiledImageFill (*image, fill.transform, 0, nullptr, clampTiledImages);
			}
		}

		~ShaderFillOperation()
		{
			if (image != nullptr)
				state.shaderQuadQueue.flush();
		}

		GLState& state;
		ScopedPointer<OpenGLTextureFromImage> image;

		JUCE_DECLARE_NON_COPYABLE (ShaderFillOperation);
	};

	JUCE_DECLARE_NON_COPYABLE (ClipRegion_RectangleList);
};

class SavedState
{
public:
	SavedState (GLState* const state_)
		: clip (new ClipRegion_RectangleList (*state_, state_->target.bounds)),
		  transform (0, 0), interpolationQuality (Graphics::mediumResamplingQuality),
		  state (state_), transparencyLayerAlpha (1.0f)
	{}

	SavedState (const SavedState& other)
		: clip (other.clip), transform (other.transform), font (other.font),
		  fillType (other.fillType), interpolationQuality (other.interpolationQuality),
		  state (other.state), transparencyLayerAlpha (other.transparencyLayerAlpha),
		  transparencyLayer (other.transparencyLayer), previousTarget (other.previousTarget.createCopy())
	{}

	bool clipToRectangle (const Rectangle<int>& r)
	{
		if (clip != nullptr)
		{
			if (transform.isOnlyTranslated)
			{
				cloneClipIfMultiplyReferenced();
				clip = clip->clipToRectangle (transform.translated (r));
			}
			else
			{
				Path p;
				p.addRectangle (r);
				clipToPath (p, AffineTransform::identity);
			}
		}

		return clip != nullptr;
	}

	bool clipToRectangleList (const RectangleList& r)
	{
		if (clip != nullptr)
		{
			if (transform.isOnlyTranslated)
			{
				cloneClipIfMultiplyReferenced();
				RectangleList offsetList (r);
				offsetList.offsetAll (transform.xOffset, transform.yOffset);
				clip = clip->clipToRectangleList (offsetList);
			}
			else
			{
				clipToPath (r.toPath(), AffineTransform::identity);
			}
		}

		return clip != nullptr;
	}

	bool excludeClipRectangle (const Rectangle<int>& r)
	{
		if (clip != nullptr)
		{
			cloneClipIfMultiplyReferenced();

			if (transform.isOnlyTranslated)
			{
				clip = clip->excludeClipRectangle (transform.translated (r));
			}
			else
			{
				Path p;
				p.addRectangle (r.toFloat());
				p.applyTransform (transform.complexTransform);
				p.addRectangle (clip->getClipBounds().toFloat());
				p.setUsingNonZeroWinding (false);
				clip = clip->clipToPath (p, AffineTransform::identity);
			}
		}

		return clip != nullptr;
	}

	void clipToPath (const Path& p, const AffineTransform& t)
	{
		if (clip != nullptr)
		{
			cloneClipIfMultiplyReferenced();
			clip = clip->clipToPath (p, transform.getTransformWith (t));
		}
	}

	void clipToImageAlpha (const Image& sourceImage, const AffineTransform& t)
	{
		if (clip != nullptr)
		{
			Path p;
			p.addRectangle (sourceImage.getBounds());
			clipToPath (p, t);

			if (sourceImage.hasAlphaChannel() && clip != nullptr)
			{
				cloneClipIfMultiplyReferenced();
				clip = clip->clipToImageAlpha (sourceImage, transform.getTransformWith (t));
			}
		}
	}

	bool clipRegionIntersects (const Rectangle<int>& r) const
	{
		return clip != nullptr
				&& (transform.isOnlyTranslated ? clip->getClipBounds().intersects (transform.translated (r))
											   : getClipBounds().intersects (r));
	}

	Rectangle<int> getClipBounds() const
	{
		return clip != nullptr ? transform.deviceSpaceToUserSpace (clip->getClipBounds())
							   : Rectangle<int>();
	}

	SavedState* beginTransparencyLayer (float opacity)
	{
		SavedState* const s = new SavedState (*this);

		if (clip != nullptr)
		{
			const Rectangle<int> clipBounds (clip->getClipBounds());

			state->flush();
			s->transparencyLayer = Image (OpenGLImageType().create (Image::ARGB, clipBounds.getWidth(), clipBounds.getHeight(), true));
			s->previousTarget = new Target (state->target);
			state->target = Target (state->target.context, *OpenGLImageType::getFrameBufferFrom (s->transparencyLayer), clipBounds.getPosition());
			s->transparencyLayerAlpha = opacity;
			s->cloneClipIfMultiplyReferenced();

			s->state->target.makeActive();
		}

		return s;
	}

	void endTransparencyLayer (SavedState& finishedLayerState)
	{
		if (clip != nullptr)
		{
			jassert (finishedLayerState.previousTarget != nullptr);

			state->flush();
			state->target = *finishedLayerState.previousTarget;
			finishedLayerState.previousTarget = nullptr;

			state->target.makeActive();
			const Rectangle<int> clipBounds (clip->getClipBounds());

			clip->drawImage (finishedLayerState.transparencyLayer,
							 AffineTransform::translation ((float) clipBounds.getX(), (float) clipBounds.getY()),
							 finishedLayerState.transparencyLayerAlpha, clipBounds, nullptr);
		}
	}

	void fillRect (const Rectangle<int>& r, const bool replaceContents)
	{
		if (clip != nullptr)
		{
			if (transform.isOnlyTranslated)
			{
				clip->fillRect (r.translated (transform.xOffset, transform.yOffset),
								getFillType(), replaceContents);
			}
			else
			{
				Path p;
				p.addRectangle (r);
				fillPath (p, AffineTransform::identity);
			}
		}
	}

	void fillRect (const Rectangle<float>& r)
	{
		if (clip != nullptr)
		{
			if (transform.isOnlyTranslated)
			{
				const Rectangle<float> c (r.translated ((float) transform.xOffset, (float) transform.yOffset)
										   .getIntersection (clip->getClipBounds().toFloat()));

				if (! c.isEmpty())
					clip->fillRect (c, getFillType());
			}
			else
			{
				Path p;
				p.addRectangle (r);
				fillPath (p, AffineTransform::identity);
			}
		}
	}

	void fillPath (const Path& path, const AffineTransform& t)
	{
		if (clip != nullptr)
		{
			EdgeTable et (clip->getClipBounds(), path, transform.getTransformWith (t));
			fillEdgeTable (et);
		}
	}

	void drawGlyph (int glyphNumber, const AffineTransform& t)
	{
		if (clip != nullptr)
		{
			if (transform.isOnlyTranslated && t.isOnlyTranslation())
			{
				RenderingHelpers::GlyphCache <RenderingHelpers::CachedGlyphEdgeTable <SavedState>, SavedState>::getInstance()
					.drawGlyph (*this, font, glyphNumber,
								transform.xOffset + t.getTranslationX(),
								transform.yOffset + t.getTranslationY());
			}
			else
			{
				const float fontHeight = font.getHeight();

				const ScopedPointer<EdgeTable> et (font.getTypeface()->getEdgeTableForGlyph
						(glyphNumber, transform.getTransformWith (AffineTransform::scale (fontHeight * font.getHorizontalScale(), fontHeight)
																				  .followedBy (t))));

				if (et != nullptr)
					fillEdgeTable (*et);
			}
		}
	}

	void fillEdgeTable (const EdgeTable& et, const float x, const int y)
	{
		if (clip != nullptr)
		{
			EdgeTable et2 (et);
			et2.translate (x, y);

			fillEdgeTable (et2);
		}
	}

	void drawLine (const Line <float>& line)
	{
		Path p;
		p.addLineSegment (line, 1.0f);
		fillPath (p, AffineTransform::identity);
	}

	void drawImage (const Image& image, const AffineTransform& trans)
	{
		if (clip == nullptr || fillType.colour.isTransparent())
			return;

		const Rectangle<int> clipBounds (clip->getClipBounds());
		const AffineTransform t (transform.getTransformWith (trans));
		const float alpha = fillType.colour.getFloatAlpha();

		if (t.isOnlyTranslation())
		{
			int tx = (int) (t.getTranslationX() * 256.0f);
			int ty = (int) (t.getTranslationY() * 256.0f);

			if (((tx | ty) & 0xf8) == 0)
			{
				tx = ((tx + 128) >> 8);
				ty = ((ty + 128) >> 8);

				clip->drawImage (image, t, alpha, Rectangle<int> (tx, ty, image.getWidth(), image.getHeight()), nullptr);
				return;
			}
		}

		if (! t.isSingularity())
		{
			Path p;
			p.addRectangle (image.getBounds());
			EdgeTable et (clipBounds, p, t);

			clip->drawImage (image, t, alpha, clipBounds, &et);
		}
	}

	void setFillType (const FillType& newFill)
	{
		fillType = newFill;
		state->textureCache.resetGradient();
	}

	ClipRegionBase::Ptr clip;
	RenderingHelpers::TranslationOrTransform transform;
	Font font;
	FillType fillType;
	Graphics::ResamplingQuality interpolationQuality;
	GLState* state;

private:
	float transparencyLayerAlpha;
	Image transparencyLayer;
	ScopedPointer<Target> previousTarget;

	void cloneClipIfMultiplyReferenced()
	{
		if (clip->getReferenceCount() > 1)
			clip = clip->clone();
	}

	FillType getFillType() const
	{
		return fillType.transformed (transform.getTransform());
	}

	void fillEdgeTable (EdgeTable& et) const
	{
		clip->fillEdgeTable (et, getFillType());
	}

	SavedState& operator= (const SavedState&);
};

class ShaderContext   : public LowLevelGraphicsContext
{
public:
	ShaderContext (const Target& target)
	   : glState (target), stack (new SavedState (&glState))
	{}

	bool isVectorDevice() const                                         { return false; }
	void setOrigin (int x, int y)                                       { stack->transform.setOrigin (x, y); }
	void addTransform (const AffineTransform& t)                        { stack->transform.addTransform (t); }
	float getScaleFactor()                                              { return stack->transform.getScaleFactor(); }
	Rectangle<int> getClipBounds() const                                { return stack->getClipBounds(); }
	bool isClipEmpty() const                                            { return stack->clip == nullptr; }
	bool clipRegionIntersects (const Rectangle<int>& r)                 { return stack->clipRegionIntersects (r); }
	bool clipToRectangle (const Rectangle<int>& r)                      { return stack->clipToRectangle (r); }
	bool clipToRectangleList (const RectangleList& r)                   { return stack->clipToRectangleList (r); }
	void excludeClipRectangle (const Rectangle<int>& r)                 { stack->excludeClipRectangle (r); }
	void clipToPath (const Path& path, const AffineTransform& t)        { stack->clipToPath (path, t); }
	void clipToImageAlpha (const Image& im, const AffineTransform& t)   { stack->clipToImageAlpha (im, t); }
	void saveState()                                                    { stack.save(); }
	void restoreState()                                                 { stack.restore(); }
	void beginTransparencyLayer (float opacity)                         { stack.beginTransparencyLayer (opacity); }
	void endTransparencyLayer()                                         { stack.endTransparencyLayer(); }
	void setFill (const FillType& fillType)                             { stack->setFillType (fillType); }
	void setOpacity (float newOpacity)                                  { stack->fillType.setOpacity (newOpacity); }
	void setInterpolationQuality (Graphics::ResamplingQuality quality)  { stack->interpolationQuality = quality; }
	void fillRect (const Rectangle<int>& r, bool replace)               { stack->fillRect (r, replace); }
	void fillPath (const Path& path, const AffineTransform& t)          { stack->fillPath (path, t); }
	void drawImage (const Image& im, const AffineTransform& t)          { stack->drawImage (im, t); }
	void drawVerticalLine (int x, float top, float bottom)              { if (top < bottom) stack->fillRect (Rectangle<float> ((float) x, top, 1.0f, bottom - top)); }
	void drawHorizontalLine (int y, float left, float right)            { if (left < right) stack->fillRect (Rectangle<float> (left, (float) y, right - left, 1.0f)); }
	void drawGlyph (int glyphNumber, const AffineTransform& t)          { stack->drawGlyph (glyphNumber, t); }
	void drawLine (const Line <float>& line)                            { stack->drawLine (line); }
	void setFont (const Font& newFont)                                  { stack->font = newFont; }
	const Font& getFont()                                               { return stack->font; }

private:
	GLState glState;
	RenderingHelpers::SavedStateStack<SavedState> stack;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShaderContext);
};

#endif

class NonShaderContext   : public LowLevelGraphicsSoftwareRenderer
{
public:
	NonShaderContext (const Target& target_, const Image& image_)
		: LowLevelGraphicsSoftwareRenderer (image_), target (target_), image (image_)
	{}

	~NonShaderContext()
	{
		JUCE_CHECK_OPENGL_ERROR
		const GLuint previousFrameBufferTarget = OpenGLFrameBuffer::getCurrentFrameBufferTarget();

	   #if ! JUCE_ANDROID
		target.context.extensions.glActiveTexture (GL_TEXTURE0);
		glEnable (GL_TEXTURE_2D);
		clearGLError();
	   #endif

		OpenGLTexture texture;
		texture.loadImage (image);
		texture.bind();

		target.makeActive();
		target.context.copyTexture (target.bounds, Rectangle<int> (texture.getWidth(),
																   texture.getHeight()),
									target.bounds.getWidth(), target.bounds.getHeight());
		glBindTexture (GL_TEXTURE_2D, 0);

	   #if JUCE_WINDOWS
		if (target.context.extensions.glBindFramebuffer != nullptr)
	   #endif
			target.context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, previousFrameBufferTarget);

		JUCE_CHECK_OPENGL_ERROR
	}

private:
	Target target;
	Image image;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NonShaderContext);
};

LowLevelGraphicsContext* createOpenGLContext (const Target&);
LowLevelGraphicsContext* createOpenGLContext (const Target& target)
{
   #if JUCE_USE_OPENGL_SHADERS
	if (target.context.areShadersAvailable())
		return new ShaderContext (target);
   #endif

	Image tempImage (Image::ARGB, target.bounds.getWidth(), target.bounds.getHeight(), true, SoftwareImageType());
	return new NonShaderContext (target, tempImage);
}

}

LowLevelGraphicsContext* createOpenGLGraphicsContext (OpenGLContext& context)
{
	return createOpenGLGraphicsContext (context, context.getFrameBufferID(),
										context.getWidth(), context.getHeight());
}

LowLevelGraphicsContext* createOpenGLGraphicsContext (OpenGLContext& context, OpenGLFrameBuffer& target)
{
	using namespace OpenGLRendering;
	return createOpenGLContext (Target (context, target, Point<int>()));
}

LowLevelGraphicsContext* createOpenGLGraphicsContext (OpenGLContext& context, unsigned int frameBufferID, int width, int height)
{
	using namespace OpenGLRendering;
	return createOpenGLContext (Target (context, frameBufferID, width, height));
}

/*** End of inlined file: juce_OpenGLGraphicsContext.cpp ***/


/*** Start of inlined file: juce_OpenGLHelpers.cpp ***/
void OpenGLHelpers::resetErrorState()
{
	while (glGetError() != GL_NO_ERROR) {}
}

void* OpenGLHelpers::getExtensionFunction (const char* functionName)
{
   #if JUCE_WINDOWS
	return (void*) wglGetProcAddress (functionName);
   #elif JUCE_LINUX
	return (void*) glXGetProcAddress ((const GLubyte*) functionName);
   #else
	static void* handle = dlopen (nullptr, RTLD_LAZY);
	return dlsym (handle, functionName);
   #endif
}

bool OpenGLHelpers::isExtensionSupported (const char* const extensionName)
{
	jassert (extensionName != nullptr); // you must supply a genuine string for this.
	jassert (isContextActive()); // An OpenGL context will need to be active before calling this.

	const char* extensions = (const char*) glGetString (GL_EXTENSIONS);
	jassert (extensions != nullptr); // Perhaps you didn't activate an OpenGL context before calling this?

	for (;;)
	{
		const char* found = strstr (extensions, extensionName);

		if (found == nullptr)
			break;

		extensions = found + strlen (extensionName);

		if (extensions[0] == ' ' || extensions[0] == 0)
			return true;
	}

	return false;
}

void OpenGLHelpers::clear (const Colour& colour)
{
	glClearColor (colour.getFloatRed(), colour.getFloatGreen(),
				  colour.getFloatBlue(), colour.getFloatAlpha());

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

#if JUCE_USE_OPENGL_FIXED_FUNCTION
void OpenGLHelpers::setColour (const Colour& colour)
{
	glColor4f (colour.getFloatRed(), colour.getFloatGreen(),
			   colour.getFloatBlue(), colour.getFloatAlpha());
}

void OpenGLHelpers::prepareFor2D (const int width, const int height)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

   #if JUCE_OPENGL_ES
	glOrthof (0.0f, (GLfloat) width, 0.0f, (GLfloat) height, 0.0f, 1.0f);
   #else
	glOrtho  (0.0, width, 0.0, height, 0, 1);
   #endif

	glViewport (0, 0, width, height);
}

void OpenGLHelpers::setPerspective (double fovy, double aspect, double zNear, double zFar)
{
	glLoadIdentity();

   #if JUCE_OPENGL_ES
	const GLfloat ymax = (GLfloat) (zNear * tan (fovy * double_Pi / 360.0));
	const GLfloat ymin = -ymax;

	glFrustumf (ymin * (GLfloat) aspect, ymax * (GLfloat) aspect, ymin, ymax, (GLfloat) zNear, (GLfloat) zFar);
   #else
	const double ymax = zNear * tan (fovy * double_Pi / 360.0);
	const double ymin = -ymax;

	glFrustum  (ymin * aspect, ymax * aspect, ymin, ymax, zNear, zFar);
   #endif
}

void OpenGLHelpers::applyTransform (const AffineTransform& t)
{
	const GLfloat m[] = { t.mat00, t.mat10, 0, 0,
						  t.mat01, t.mat11, 0, 0,
						  0,       0,       1, 0,
						  t.mat02, t.mat12, 0, 1 };
	glMultMatrixf (m);
}

void OpenGLHelpers::applyMatrix (const float matrixValues[16])
{
	glMultMatrixf (matrixValues);
}

#if ! JUCE_OPENGL_ES
void OpenGLHelpers::applyMatrix (const double matrixValues[16])
{
	glMultMatrixd (matrixValues);
}
#endif
#endif

void OpenGLHelpers::enableScissorTest (const Rectangle<int>& clip)
{
	glEnable (GL_SCISSOR_TEST);
	glScissor (clip.getX(), clip.getY(), clip.getWidth(), clip.getHeight());
}

#if JUCE_USE_OPENGL_FIXED_FUNCTION
void OpenGLHelpers::drawQuad2D (float x1, float y1,
								float x2, float y2,
								float x3, float y3,
								float x4, float y4,
								const Colour& colour)
{
	const GLfloat vertices[]      = { x1, y1, x2, y2, x4, y4, x3, y3 };
	const GLfloat textureCoords[] = { 0, 0, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f };

	setColour (colour);

	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (2, GL_FLOAT, 0, vertices);

	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer (2, GL_FLOAT, 0, textureCoords);

	glDisableClientState (GL_COLOR_ARRAY);
	glDisableClientState (GL_NORMAL_ARRAY);

	glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

void OpenGLHelpers::drawQuad3D (float x1, float y1, float z1,
								float x2, float y2, float z2,
								float x3, float y3, float z3,
								float x4, float y4, float z4,
								const Colour& colour)
{
	const GLfloat vertices[]      = { x1, y1, z1, x2, y2, z2, x4, y4, z4, x3, y3, z3 };
	const GLfloat textureCoords[] = { 0, 0, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f };

	setColour (colour);

	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (3, GL_FLOAT, 0, vertices);

	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer (2, GL_FLOAT, 0, textureCoords);

	glDisableClientState (GL_COLOR_ARRAY);
	glDisableClientState (GL_NORMAL_ARRAY);

	glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

void OpenGLHelpers::drawTriangleStrip (const GLfloat* const vertices, const GLfloat* const textureCoords, const int numVertices) noexcept
{
   #if ! JUCE_ANDROID
	glEnable (GL_TEXTURE_2D);
	clearGLError();
   #endif
	glDisableClientState (GL_COLOR_ARRAY);
	glDisableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (2, GL_FLOAT, 0, vertices);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer (2, GL_FLOAT, 0, textureCoords);
	glDrawArrays (GL_TRIANGLE_STRIP, 0, numVertices);
}

void OpenGLHelpers::drawTriangleStrip (const GLfloat* const vertices, const GLfloat* const textureCoords,
									   const int numVertices, const GLuint textureID) noexcept
{
	jassert (textureID != 0);
	glBindTexture (GL_TEXTURE_2D, textureID);
	drawTriangleStrip (vertices, textureCoords, numVertices);
	glBindTexture (GL_TEXTURE_2D, 0);
}

void OpenGLHelpers::drawTextureQuad (GLuint textureID, const Rectangle<int>& rect)
{
	const GLfloat l = (GLfloat) rect.getX();
	const GLfloat t = (GLfloat) rect.getY();
	const GLfloat r = (GLfloat) rect.getRight();
	const GLfloat b = (GLfloat) rect.getBottom();

	const GLfloat vertices[]      = { l, t, r, t, l, b, r, b };
	const GLfloat textureCoords[] = { 0, 1.0f, 1.0f, 1.0f, 0, 0, 1.0f, 0 };

	drawTriangleStrip (vertices, textureCoords, 4, textureID);
}

void OpenGLHelpers::fillRectWithTexture (const Rectangle<int>& rect, GLuint textureID, const float alpha)
{
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glColor4f (alpha, alpha, alpha, alpha);

	drawTextureQuad (textureID, rect);
}

void OpenGLHelpers::fillRectWithColour (const Rectangle<int>& rect, const Colour& colour)
{
	glEnableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_COLOR_ARRAY);
	glDisableClientState (GL_NORMAL_ARRAY);
	setColour (colour);
	fillRect (rect);
}

void OpenGLHelpers::fillRect (const Rectangle<int>& rect)
{
	const GLfloat vertices[] = { (GLfloat) rect.getX(),     (GLfloat) rect.getY(),
								 (GLfloat) rect.getRight(), (GLfloat) rect.getY(),
								 (GLfloat) rect.getX(),     (GLfloat) rect.getBottom(),
								 (GLfloat) rect.getRight(), (GLfloat) rect.getBottom() };

	glVertexPointer (2, GL_FLOAT, 0, vertices);
	glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}
#endif

OpenGLTextureFromImage::OpenGLTextureFromImage (const Image& image)
	: imageWidth (image.getWidth()),
	  imageHeight (image.getHeight())
{
	JUCE_CHECK_OPENGL_ERROR
	OpenGLFrameBuffer* const fb = OpenGLImageType::getFrameBufferFrom (image);

	if (fb != nullptr)
	{
		textureID = fb->getTextureID();
		fullWidthProportion  = 1.0f;
		fullHeightProportion = 1.0f;
	}
	else
	{
		texture = new OpenGLTexture();
		texture->loadImage (image);
		textureID = texture->getTextureID();

		fullWidthProportion  = imageWidth  / (float) texture->getWidth();
		fullHeightProportion = imageHeight / (float) texture->getHeight();
	}

	JUCE_CHECK_OPENGL_ERROR
}

OpenGLTextureFromImage::~OpenGLTextureFromImage() {}

/*** End of inlined file: juce_OpenGLHelpers.cpp ***/


/*** Start of inlined file: juce_OpenGLImage.cpp ***/
class OpenGLFrameBufferImage   : public ImagePixelData
{
public:
	OpenGLFrameBufferImage (OpenGLContext& context_, int width, int height)
		: ImagePixelData (Image::ARGB, width, height),
		  context (context_),
		  pixelStride (4),
		  lineStride (width * pixelStride)
	{
	}

	bool initialise()
	{
		return frameBuffer.initialise (context, width, height);
	}

	LowLevelGraphicsContext* createLowLevelContext()
	{
		return createOpenGLGraphicsContext (context, frameBuffer);
	}

	ImageType* createType() const     { return new OpenGLImageType(); }

	ImagePixelData* clone()
	{
		OpenGLFrameBufferImage* im = new OpenGLFrameBufferImage (context, width, height);
		im->incReferenceCount();

		{
			Image newImage (im);
			Graphics g (newImage);
			g.drawImageAt (Image (this), 0, 0, false);
		}

		im->resetReferenceCount();
		return im;
	}

	void initialiseBitmapData (Image::BitmapData& bitmapData, int x, int y, Image::BitmapData::ReadWriteMode mode)
	{
		bitmapData.pixelFormat = pixelFormat;
		bitmapData.lineStride  = lineStride;
		bitmapData.pixelStride = pixelStride;

		switch (mode)
		{
			case Image::BitmapData::writeOnly:  DataReleaser<Dummy,  Writer>::initialise (frameBuffer, bitmapData, x, y); break;
			case Image::BitmapData::readOnly:   DataReleaser<Reader, Dummy> ::initialise (frameBuffer, bitmapData, x, y); break;
			case Image::BitmapData::readWrite:  DataReleaser<Reader, Writer>::initialise (frameBuffer, bitmapData, x, y); break;
			default:                            jassertfalse; break;
		}
	}

	OpenGLContext& context;
	OpenGLFrameBuffer frameBuffer;

private:
	int pixelStride, lineStride;

	struct Dummy
	{
		Dummy (OpenGLFrameBuffer&, int, int, int, int) noexcept {}
		static void read (OpenGLFrameBuffer&, Image::BitmapData& , int, int) noexcept {}
		static void write (const PixelARGB*) noexcept {}
	};

	struct Reader
	{
		static void read (OpenGLFrameBuffer& frameBuffer, Image::BitmapData& bitmapData, int x, int y)
		{
			frameBuffer.readPixels ((PixelARGB*) bitmapData.data,
									Rectangle<int> (x, frameBuffer.getHeight() - (y + bitmapData.height), bitmapData.width, bitmapData.height));

			verticalRowFlip ((PixelARGB*) bitmapData.data, bitmapData.width, bitmapData.height);
		}

		static void verticalRowFlip (PixelARGB* const data, const int w, const int h)
		{
			HeapBlock<PixelARGB> tempRow (w);
			const int rowSize = sizeof (PixelARGB) * w;

			for (int y = 0; y < h / 2; ++y)
			{
				PixelARGB* const row1 = data + y * w;
				PixelARGB* const row2 = data + (h - 1 - y) * w;
				memcpy (tempRow, row1, rowSize);
				memcpy (row1, row2, rowSize);
				memcpy (row2, tempRow, rowSize);
			}
		}
	};

	struct Writer
	{
		Writer (OpenGLFrameBuffer& frameBuffer_, int x, int y, int w, int h) noexcept
			: frameBuffer (frameBuffer_), area (x, y, w, h)
		{}

		void write (const PixelARGB* const data) const noexcept
		{
			HeapBlock<PixelARGB> invertedCopy (area.getWidth() * area.getHeight());
			const int rowSize = sizeof (PixelARGB) * area.getWidth();

			for (int y = 0; y < area.getHeight(); ++y)
				memcpy (invertedCopy + area.getWidth() * y,
						data + area.getWidth() * (area.getHeight() - 1 - y), rowSize);

			frameBuffer.writePixels (invertedCopy, area);
		}

		OpenGLFrameBuffer& frameBuffer;
		const Rectangle<int> area;

		JUCE_DECLARE_NON_COPYABLE (Writer);
	};

	template <class ReaderType, class WriterType>
	struct DataReleaser  : public Image::BitmapData::BitmapDataReleaser
	{
		DataReleaser (OpenGLFrameBuffer& frameBuffer, int x, int y, int w, int h)
			: data (w * h),
			  writer (frameBuffer, x, y, w, h)
		{}

		~DataReleaser()
		{
			writer.write (data);
		}

		static void initialise (OpenGLFrameBuffer& frameBuffer, Image::BitmapData& bitmapData, int x, int y)
		{
			DataReleaser* r = new DataReleaser (frameBuffer, x, y, bitmapData.width, bitmapData.height);
			bitmapData.dataReleaser = r;
			bitmapData.data = (uint8*) (r->data + (x + y * bitmapData.width));

			ReaderType::read (frameBuffer, bitmapData, x, y);
		}

		HeapBlock<PixelARGB> data;
		WriterType writer;
	};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLFrameBufferImage);
};

OpenGLImageType::OpenGLImageType() {}
OpenGLImageType::~OpenGLImageType() {}

int OpenGLImageType::getTypeID() const
{
	return 3;
}

ImagePixelData* OpenGLImageType::create (Image::PixelFormat, int width, int height, bool /*shouldClearImage*/) const
{
	OpenGLContext* currentContext = OpenGLContext::getCurrentContext();
	jassert (currentContext != nullptr); // an OpenGL image can only be created when a valid context is active!

	ScopedPointer<OpenGLFrameBufferImage> im (new OpenGLFrameBufferImage (*currentContext, width, height));

	if (! im->initialise())
		return nullptr;

	im->frameBuffer.clear (Colours::transparentBlack);
	return im.release();
}

OpenGLFrameBuffer* OpenGLImageType::getFrameBufferFrom (const Image& image)
{
	OpenGLFrameBufferImage* const glImage = dynamic_cast<OpenGLFrameBufferImage*> (image.getPixelData());

	return glImage != nullptr ? &(glImage->frameBuffer) : nullptr;
}

/*** End of inlined file: juce_OpenGLImage.cpp ***/


/*** Start of inlined file: juce_OpenGLPixelFormat.cpp ***/
OpenGLPixelFormat::OpenGLPixelFormat (const int bitsPerRGBComponent,
									  const int alphaBits_,
									  const int depthBufferBits_,
									  const int stencilBufferBits_) noexcept
	: redBits (bitsPerRGBComponent),
	  greenBits (bitsPerRGBComponent),
	  blueBits (bitsPerRGBComponent),
	  alphaBits (alphaBits_),
	  depthBufferBits (depthBufferBits_),
	  stencilBufferBits (stencilBufferBits_),
	  accumulationBufferRedBits (0),
	  accumulationBufferGreenBits (0),
	  accumulationBufferBlueBits (0),
	  accumulationBufferAlphaBits (0),
	  multisamplingLevel (0)
{
}

bool OpenGLPixelFormat::operator== (const OpenGLPixelFormat& other) const noexcept
{
	return redBits == other.redBits
			&& greenBits == other.greenBits
			&& blueBits  == other.blueBits
			&& alphaBits == other.alphaBits
			&& depthBufferBits == other.depthBufferBits
			&& stencilBufferBits == other.stencilBufferBits
			&& accumulationBufferRedBits   == other.accumulationBufferRedBits
			&& accumulationBufferGreenBits == other.accumulationBufferGreenBits
			&& accumulationBufferBlueBits  == other.accumulationBufferBlueBits
			&& accumulationBufferAlphaBits == other.accumulationBufferAlphaBits
			&& multisamplingLevel == other.multisamplingLevel;
}

bool OpenGLPixelFormat::operator!= (const OpenGLPixelFormat& other) const noexcept
{
	return ! operator== (other);
}

/*** End of inlined file: juce_OpenGLPixelFormat.cpp ***/


/*** Start of inlined file: juce_OpenGLShaderProgram.cpp ***/
#if JUCE_USE_OPENGL_SHADERS

OpenGLShaderProgram::OpenGLShaderProgram (const OpenGLContext& context_) noexcept
	: context (context_)
{
	// This object can only be created and used when the current thread has an active OpenGL context.
	jassert (OpenGLHelpers::isContextActive());

	programID = context.extensions.glCreateProgram();
}

OpenGLShaderProgram::~OpenGLShaderProgram() noexcept
{
	context.extensions.glDeleteProgram (programID);
}

double OpenGLShaderProgram::getLanguageVersion()
{
   #if JUCE_OPENGL_ES
	jassertfalse; // doesn't work in ES
	return 0;
   #else
	return String ((const char*) glGetString (GL_SHADING_LANGUAGE_VERSION))
			.upToFirstOccurrenceOf (" ", false, false).getDoubleValue();
   #endif
}

bool OpenGLShaderProgram::addShader (const char* const code, GLenum type)
{
	GLuint shaderID = context.extensions.glCreateShader (type);
	context.extensions.glShaderSource (shaderID, 1, (const GLchar**) &code, nullptr);
	context.extensions.glCompileShader (shaderID);

	GLint status = GL_FALSE;
	context.extensions.glGetShaderiv (shaderID, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLchar infoLog [16384];
		GLsizei infologLength = 0;
		context.extensions.glGetShaderInfoLog (shaderID, sizeof (infoLog), &infologLength, infoLog);
		errorLog = String (infoLog, infologLength);

	   #if JUCE_DEBUG
		DBG (errorLog);
		jassertfalse;
	   #endif

		return false;
	}

	context.extensions.glAttachShader (programID, shaderID);
	context.extensions.glDeleteShader (shaderID);
	JUCE_CHECK_OPENGL_ERROR
	return true;
}

bool OpenGLShaderProgram::link() noexcept
{
	context.extensions.glLinkProgram (programID);

	GLint status = GL_FALSE;
	context.extensions.glGetProgramiv (programID, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLchar infoLog [16384];
		GLsizei infologLength = 0;
		context.extensions.glGetProgramInfoLog (programID, sizeof (infoLog), &infologLength, infoLog);
		errorLog = String (infoLog, infologLength);

	   #if JUCE_DEBUG
		DBG (errorLog);
		jassertfalse;
	   #endif
	}

	JUCE_CHECK_OPENGL_ERROR
	return status != GL_FALSE;
}

void OpenGLShaderProgram::use() const noexcept
{
	context.extensions.glUseProgram (programID);
}

OpenGLShaderProgram::Uniform::Uniform (const OpenGLShaderProgram& program, const char* const name)
	: uniformID (program.context.extensions.glGetUniformLocation (program.programID, name)), context (program.context)
{
	jassert (uniformID >= 0);
}

OpenGLShaderProgram::Attribute::Attribute (const OpenGLShaderProgram& program, const char* name)
	: attributeID (program.context.extensions.glGetAttribLocation (program.programID, name))
{
	jassert (attributeID >= 0);
}

void OpenGLShaderProgram::Uniform::set (GLfloat n1) const noexcept                                    { context.extensions.glUniform1f (uniformID, n1); }
void OpenGLShaderProgram::Uniform::set (GLint n1) const noexcept                                      { context.extensions.glUniform1i (uniformID, n1); }
void OpenGLShaderProgram::Uniform::set (GLfloat n1, GLfloat n2) const noexcept                        { context.extensions.glUniform2f (uniformID, n1, n2); }
void OpenGLShaderProgram::Uniform::set (GLfloat n1, GLfloat n2, GLfloat n3) const noexcept            { context.extensions.glUniform3f (uniformID, n1, n2, n3); }
void OpenGLShaderProgram::Uniform::set (GLfloat n1, GLfloat n2, GLfloat n3, float n4) const noexcept  { context.extensions.glUniform4f (uniformID, n1, n2, n3, n4); }
void OpenGLShaderProgram::Uniform::set (GLint n1, GLint n2, GLint n3, GLint n4) const noexcept        { context.extensions.glUniform4i (uniformID, n1, n2, n3, n4); }
void OpenGLShaderProgram::Uniform::set (const GLfloat* values, GLsizei numValues) const noexcept      { context.extensions.glUniform1fv (uniformID, numValues, values); }

#endif

/*** End of inlined file: juce_OpenGLShaderProgram.cpp ***/


/*** Start of inlined file: juce_OpenGLTexture.cpp ***/
OpenGLTexture::OpenGLTexture()
	: textureID (0), width (0), height (0)
{
}

OpenGLTexture::~OpenGLTexture()
{
	release();
}

bool OpenGLTexture::isValidSize (int width, int height)
{
	return isPowerOfTwo (width) && isPowerOfTwo (height);
}

void OpenGLTexture::create (const int w, const int h, const void* pixels, GLenum type)
{
	// Texture objects can only be created when the current thread has an active OpenGL
	// context. You'll need to create this object in one of the OpenGLContext's callbacks.
	jassert (OpenGLHelpers::isContextActive());

	jassert (isValidSize (w, h)); // Perhaps these dimensions must be a power-of-two?

	width  = w;
	height = h;

	if (textureID == 0)
	{
		JUCE_CHECK_OPENGL_ERROR
		glGenTextures (1, &textureID);
		glBindTexture (GL_TEXTURE_2D, textureID);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		JUCE_CHECK_OPENGL_ERROR
	}
	else
	{
		glBindTexture (GL_TEXTURE_2D, textureID);
		JUCE_CHECK_OPENGL_ERROR;
	}

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	JUCE_CHECK_OPENGL_ERROR
	glTexImage2D (GL_TEXTURE_2D, 0, type == GL_ALPHA ? GL_ALPHA : GL_RGBA,
				  w, h, 0, type, GL_UNSIGNED_BYTE, pixels);
	JUCE_CHECK_OPENGL_ERROR
}

template <class PixelType>
struct Flipper
{
	static void flip (HeapBlock<PixelARGB>& dataCopy, const uint8* srcData, const int lineStride,
					  const int w, const int h, const int textureW, const int textureH)
	{
		dataCopy.malloc (textureW * textureH);

		for (int y = 0; y < h; ++y)
		{
			const PixelType* src = (const PixelType*) srcData;
			PixelARGB* const dst = (PixelARGB*) (dataCopy + textureW * (textureH - 1 - y));

			for (int x = 0; x < w; ++x)
				dst[x].set (src[x]);

			srcData += lineStride;
		}
	}
};

void OpenGLTexture::loadImage (const Image& image)
{
	const int imageW = image.getWidth();
	const int imageH = image.getHeight();
	const int textureW = nextPowerOfTwo (imageW);
	const int textureH = nextPowerOfTwo (imageH);

	HeapBlock<PixelARGB> dataCopy;
	Image::BitmapData srcData (image, Image::BitmapData::readOnly);

	switch (srcData.pixelFormat)
	{
		case Image::ARGB:           Flipper<PixelARGB> ::flip (dataCopy, srcData.data, srcData.lineStride, imageW, imageH, textureW, textureH); break;
		case Image::RGB:            Flipper<PixelRGB>  ::flip (dataCopy, srcData.data, srcData.lineStride, imageW, imageH, textureW, textureH); break;
		case Image::SingleChannel:  Flipper<PixelAlpha>::flip (dataCopy, srcData.data, srcData.lineStride, imageW, imageH, textureW, textureH); break;
		default: break;
	}

	create (textureW, textureH, dataCopy, JUCE_RGBA_FORMAT);
}

void OpenGLTexture::loadARGB (const PixelARGB* pixels, const int w, const int h)
{
	jassert (isValidSize (w, h));
	create (w, h, pixels, JUCE_RGBA_FORMAT);
}

void OpenGLTexture::loadAlpha (const uint8* pixels, int w, int h)
{
	jassert (isValidSize (w, h));
	create (w, h, pixels, GL_ALPHA);
}

void OpenGLTexture::loadARGBFlipped (const PixelARGB* pixels, int w, int h)
{
	const int textureW = nextPowerOfTwo (w);
	const int textureH = nextPowerOfTwo (h);

	HeapBlock<PixelARGB> flippedCopy;
	Flipper<PixelARGB>::flip (flippedCopy, (const uint8*) pixels, 4 * w, w, h, textureW, textureH);

	loadARGB (flippedCopy, textureW, textureH);
}

void OpenGLTexture::release()
{
	if (textureID != 0)
	{
		if (OpenGLHelpers::isContextActive())
			glDeleteTextures (1, &textureID);

		textureID = 0;
		width = 0;
		height = 0;
	}
}

void OpenGLTexture::bind() const
{
	glBindTexture (GL_TEXTURE_2D, textureID);
}

void OpenGLTexture::unbind() const
{
	glBindTexture (GL_TEXTURE_2D, 0);
}

#if JUCE_USE_OPENGL_FIXED_FUNCTION
void OpenGLTexture::draw2D (float x1, float y1,
							float x2, float y2,
							float x3, float y3,
							float x4, float y4,
							const Colour& colour) const
{
	bind();
	OpenGLHelpers::drawQuad2D (x1, y1, x2, y2, x3, y3, x4, y4, colour);
	unbind();
}

void OpenGLTexture::draw3D (float x1, float y1, float z1,
							float x2, float y2, float z2,
							float x3, float y3, float z3,
							float x4, float y4, float z4,
							const Colour& colour) const
{
	bind();
	OpenGLHelpers::drawQuad3D (x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, colour);
	unbind();
}
#endif

/*** End of inlined file: juce_OpenGLTexture.cpp ***/

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


/*** Start of inlined file: juce_mac_CoreGraphicsHelpers.h ***/
#ifndef __JUCE_MAC_COREGRAPHICSHELPERS_JUCEHEADER__
#define __JUCE_MAC_COREGRAPHICSHELPERS_JUCEHEADER__

namespace
{
	template <class RectType>
	Rectangle<int> convertToRectInt (const RectType& r) noexcept
	{
		return Rectangle<int> ((int) r.origin.x, (int) r.origin.y, (int) r.size.width, (int) r.size.height);
	}

	template <class RectType>
	Rectangle<float> convertToRectFloat (const RectType& r) noexcept
	{
		return Rectangle<float> (r.origin.x, r.origin.y, r.size.width, r.size.height);
	}

	template <class RectType>
	CGRect convertToCGRect (const RectType& r) noexcept
	{
		return CGRectMake ((CGFloat) r.getX(), (CGFloat) r.getY(), (CGFloat) r.getWidth(), (CGFloat) r.getHeight());
	}
}

extern CGImageRef juce_createCoreGraphicsImage (const Image&, const bool forAlpha, CGColorSpaceRef, const bool mustOutliveSource);

extern CGContextRef juce_getImageContext (const Image&);

#endif   // __JUCE_MAC_COREGRAPHICSHELPERS_JUCEHEADER__

/*** End of inlined file: juce_mac_CoreGraphicsHelpers.h ***/

 #if JUCE_MAC

/*** Start of inlined file: juce_OpenGL_osx.h ***/
} // (juce namespace)

#define ThreadSafeNSOpenGLView MakeObjCClassName(ThreadSafeNSOpenGLView)

@interface ThreadSafeNSOpenGLView  : NSOpenGLView
{
	juce::CriticalSection* contextLock;
	bool needsUpdate;
}

- (id) initWithFrame: (NSRect) frameRect pixelFormat: (NSOpenGLPixelFormat*) format;
- (bool) makeActive;
- (void) reshape;
- (void) rightMouseDown: (NSEvent*) ev;
- (void) rightMouseUp: (NSEvent*) ev;
@end

@implementation ThreadSafeNSOpenGLView

- (id) initWithFrame: (NSRect) frameRect
		 pixelFormat: (NSOpenGLPixelFormat*) format
{
	contextLock = new juce::CriticalSection();
	self = [super initWithFrame: frameRect pixelFormat: format];
	needsUpdate = true;

	if (self != nil)
		[[NSNotificationCenter defaultCenter] addObserver: self
												 selector: @selector (_surfaceNeedsUpdate:)
													 name: NSViewGlobalFrameDidChangeNotification
												   object: self];
	return self;
}

- (void) dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver: self];
	delete contextLock;
	[super dealloc];
}

- (bool) makeActive
{
	const juce::ScopedLock sl (*contextLock);

	if ([self openGLContext] == nil)
		return false;

	[[self openGLContext] makeCurrentContext];

	if (needsUpdate)
	{
		[super update];
		needsUpdate = false;
	}

	return true;
}

- (void) _surfaceNeedsUpdate: (NSNotification*) notification
{
	(void) notification;
	const juce::ScopedLock sl (*contextLock);
	needsUpdate = true;
}

- (void) update
{
	const juce::ScopedLock sl (*contextLock);
	needsUpdate = true;
}

- (void) reshape
{
	const juce::ScopedLock sl (*contextLock);
	needsUpdate = true;
}

- (void) rightMouseDown: (NSEvent*) ev  { [[self superview] rightMouseDown: ev]; }
- (void) rightMouseUp:   (NSEvent*) ev  { [[self superview] rightMouseUp:   ev]; }

@end

namespace juce
{

class OpenGLContext::NativeContext
{
public:
	NativeContext (Component& component,
				   const OpenGLPixelFormat& pixelFormat,
				   const NativeContext* contextToShareWith)
	{
		NSOpenGLPixelFormatAttribute attribs[] =
		{
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFAMPSafe,
			NSOpenGLPFAClosestPolicy,
			NSOpenGLPFANoRecovery,
			NSOpenGLPFAColorSize,   (NSOpenGLPixelFormatAttribute) (pixelFormat.redBits + pixelFormat.greenBits + pixelFormat.blueBits),
			NSOpenGLPFAAlphaSize,   (NSOpenGLPixelFormatAttribute) pixelFormat.alphaBits,
			NSOpenGLPFADepthSize,   (NSOpenGLPixelFormatAttribute) pixelFormat.depthBufferBits,
			NSOpenGLPFAStencilSize, (NSOpenGLPixelFormatAttribute) pixelFormat.stencilBufferBits,
			NSOpenGLPFAAccumSize,   (NSOpenGLPixelFormatAttribute) (pixelFormat.accumulationBufferRedBits + pixelFormat.accumulationBufferGreenBits
										+ pixelFormat.accumulationBufferBlueBits + pixelFormat.accumulationBufferAlphaBits),
			pixelFormat.multisamplingLevel > 0 ? NSOpenGLPFASamples : (NSOpenGLPixelFormatAttribute) 0,
			(NSOpenGLPixelFormatAttribute) pixelFormat.multisamplingLevel,
			0
		};

		NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc] initWithAttributes: attribs];

		view = [[ThreadSafeNSOpenGLView alloc] initWithFrame: NSMakeRect (0, 0, 100.0f, 100.0f)
												 pixelFormat: format];

		NSOpenGLContext* const sharedContext
			= contextToShareWith != nullptr ? contextToShareWith->renderContext : nil;

		renderContext = [[[NSOpenGLContext alloc] initWithFormat: format
													shareContext: sharedContext] autorelease];

		setSwapInterval (1);

		[view setOpenGLContext: renderContext];
		[format release];

		viewAttachment = NSViewComponent::attachViewToComponent (component, view);
	}

	~NativeContext()
	{
		[renderContext clearDrawable];
		[renderContext setView: nil];
		[view setOpenGLContext: nil];
		renderContext = nil;
	}

	void initialiseOnRenderThread() {}
	void shutdownOnRenderThread() {}

	bool createdOk() const noexcept             { return getRawContext() != nullptr; }
	void* getRawContext() const noexcept        { return static_cast <void*> (renderContext); }
	GLuint getFrameBufferID() const noexcept    { return 0; }

	bool makeActive() const noexcept
	{
		jassert (renderContext != nil);

		if ([renderContext view] != view)
			[renderContext setView: view];

		[view makeActive];
		return true;
	}

	bool isActive() const noexcept
	{
		return [NSOpenGLContext currentContext] == renderContext;
	}

	struct Locker
	{
		Locker (NativeContext& nc) : cglContext ((CGLContextObj) [nc.renderContext CGLContextObj])
		{
			CGLLockContext (cglContext);
		}

		~Locker()
		{
			CGLUnlockContext (cglContext);
		}

	private:
		CGLContextObj cglContext;
	};

	void swapBuffers()
	{
		[renderContext flushBuffer];
	}

	void updateWindowPosition (const Rectangle<int>&) {}

	bool setSwapInterval (int numFramesPerSwap)
	{
		[renderContext setValues: (const GLint*) &numFramesPerSwap
					forParameter: NSOpenGLCPSwapInterval];
		return true;
	}

	int getSwapInterval() const
	{
		GLint numFrames = 0;
		[renderContext getValues: &numFrames
					forParameter: NSOpenGLCPSwapInterval];

		return numFrames;
	}

private:
	NSOpenGLContext* renderContext;
	ThreadSafeNSOpenGLView* view;
	ReferenceCountedObjectPtr<ReferenceCountedObject> viewAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext);
};

bool OpenGLHelpers::isContextActive()
{
	return CGLGetCurrentContext() != 0;
}

/*** End of inlined file: juce_OpenGL_osx.h ***/


 #else

/*** Start of inlined file: juce_OpenGL_ios.h ***/
} // (juce namespace)

@interface JuceGLView   : UIView
{
}
+ (Class) layerClass;
@end

@implementation JuceGLView
+ (Class) layerClass
{
	return [CAEAGLLayer class];
}
@end

namespace juce
{

class OpenGLContext::NativeContext
{
public:
	NativeContext (Component& component,
				   const OpenGLPixelFormat& pixelFormat,
				   const NativeContext* const contextToShareWith)
		: frameBufferHandle (0), colorBufferHandle (0), depthBufferHandle (0),
		  lastWidth (0), lastHeight (0), needToRebuildBuffers (false),
		  swapFrames (0), useDepthBuffer (pixelFormat.depthBufferBits > 0)
	{
		JUCE_AUTORELEASEPOOL
		ComponentPeer* const peer = component.getPeer();
		jassert (peer != nullptr);

		const Rectangle<int> bounds (peer->getComponent()->getLocalArea (&component, component.getLocalBounds()));
		lastWidth  = bounds.getWidth();
		lastHeight = bounds.getHeight();

		view = [[JuceGLView alloc] initWithFrame: getCGRectFor (bounds)];
		view.opaque = YES;
		view.hidden = NO;
		view.backgroundColor = [UIColor blackColor];
		view.userInteractionEnabled = NO;

		glLayer = (CAEAGLLayer*) [view layer];
		[((UIView*) peer->getNativeHandle()) addSubview: view];

		context = [EAGLContext alloc];

		const NSUInteger type = kEAGLRenderingAPIOpenGLES2;

		if (contextToShareWith != nullptr)
			[context initWithAPI: type  sharegroup: [contextToShareWith->context sharegroup]];
		else
			[context initWithAPI: type];

		// I'd prefer to put this stuff in the initialiseOnRenderThread() call, but doing
		// so causes myserious timing-related failures.
		[EAGLContext setCurrentContext: context];
		createGLBuffers();
		[EAGLContext setCurrentContext: nil];
	}

	~NativeContext()
	{
		[context release];
		context = nil;

		[view removeFromSuperview];
		[view release];
	}

	void initialiseOnRenderThread() {}

	void shutdownOnRenderThread()
	{
		JUCE_CHECK_OPENGL_ERROR
		freeGLBuffers();
	}

	bool createdOk() const noexcept             { return getRawContext() != nullptr; }
	void* getRawContext() const noexcept        { return glLayer; }
	GLuint getFrameBufferID() const noexcept    { return frameBufferHandle; }

	bool makeActive() const noexcept
	{
		if (! [EAGLContext setCurrentContext: context])
			return false;

		glBindFramebuffer (GL_FRAMEBUFFER, frameBufferHandle);
		return true;
	}

	bool isActive() const noexcept
	{
		return [EAGLContext currentContext] == context;
	}

	void swapBuffers()
	{
		glBindRenderbuffer (GL_RENDERBUFFER, colorBufferHandle);
		[context presentRenderbuffer: GL_RENDERBUFFER];

		if (needToRebuildBuffers)
		{
			needToRebuildBuffers = false;

			freeGLBuffers();
			createGLBuffers();
			makeActive();
		}
	}

	void updateWindowPosition (const Rectangle<int>& bounds)
	{
		view.frame = getCGRectFor (bounds);

		if (lastWidth != bounds.getWidth() || lastHeight != bounds.getHeight())
		{
			lastWidth  = bounds.getWidth();
			lastHeight = bounds.getHeight();
			needToRebuildBuffers = true;
		}
	}

	bool setSwapInterval (const int numFramesPerSwap) noexcept
	{
		swapFrames = numFramesPerSwap;
		return false;
	}

	int getSwapInterval() const noexcept    { return swapFrames; }

	struct Locker { Locker (NativeContext&) {} };

private:
	JuceGLView* view;
	CAEAGLLayer* glLayer;
	EAGLContext* context;
	GLuint frameBufferHandle, colorBufferHandle, depthBufferHandle;
	int volatile lastWidth, lastHeight;
	bool volatile needToRebuildBuffers;
	int swapFrames;
	bool useDepthBuffer;

	static CGRect getCGRectFor (const Rectangle<int>& bounds)
	{
		return CGRectMake ((CGFloat) bounds.getX(),
						   (CGFloat) bounds.getY(),
						   (CGFloat) bounds.getWidth(),
						   (CGFloat) bounds.getHeight());
	}

	void createGLBuffers()
	{
		glGenFramebuffers (1, &frameBufferHandle);
		glGenRenderbuffers (1, &colorBufferHandle);

		glBindRenderbuffer (GL_RENDERBUFFER, colorBufferHandle);
		bool ok = [context renderbufferStorage: GL_RENDERBUFFER fromDrawable: glLayer];
		jassert (ok); (void) ok;

		if (useDepthBuffer)
		{
			GLint width, height;
			glGetRenderbufferParameteriv (GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
			glGetRenderbufferParameteriv (GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

			glGenRenderbuffers (1, &depthBufferHandle);
			glBindRenderbuffer (GL_RENDERBUFFER, depthBufferHandle);
			glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		}

		glBindRenderbuffer (GL_RENDERBUFFER, colorBufferHandle);

		glBindFramebuffer (GL_FRAMEBUFFER, frameBufferHandle);
		glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBufferHandle);

		if (useDepthBuffer)
			glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferHandle);

		jassert (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		JUCE_CHECK_OPENGL_ERROR
	}

	void freeGLBuffers()
	{
		JUCE_CHECK_OPENGL_ERROR
		[context renderbufferStorage: GL_RENDERBUFFER fromDrawable: nil];

		if (frameBufferHandle != 0)
		{
			glDeleteFramebuffers (1, &frameBufferHandle);
			frameBufferHandle = 0;
		}

		if (colorBufferHandle != 0)
		{
			glDeleteRenderbuffers (1, &colorBufferHandle);
			colorBufferHandle = 0;
		}

		if (depthBufferHandle != 0)
		{
			glDeleteRenderbuffers (1, &depthBufferHandle);
			depthBufferHandle = 0;
		}

		JUCE_CHECK_OPENGL_ERROR
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext);
};

bool OpenGLHelpers::isContextActive()
{
	return [EAGLContext currentContext] != nil;
}

/*** End of inlined file: juce_OpenGL_ios.h ***/


 #endif

#elif JUCE_WINDOWS

/*** Start of inlined file: juce_OpenGL_win32.h ***/
extern ComponentPeer* createNonRepaintingEmbeddedWindowsPeer (Component*, void* parent);

class OpenGLContext::NativeContext
{
public:
	NativeContext (Component& component,
				   const OpenGLPixelFormat& pixelFormat,
				   const NativeContext* contextToShareWith)
	{
		createNativeWindow (component);

		PIXELFORMATDESCRIPTOR pfd;
		initialisePixelFormatDescriptor (pfd, pixelFormat);

		const int pixFormat = ChoosePixelFormat (dc, &pfd);
		if (pixFormat != 0)
			SetPixelFormat (dc, pixFormat, &pfd);

		renderContext = wglCreateContext (dc);

		if (renderContext != 0)
		{
			makeActive();
			initialiseGLExtensions();

			const int wglFormat = wglChoosePixelFormatExtension (pixelFormat);
			wglMakeCurrent (0, 0);

			if (wglFormat != pixFormat && wglFormat != 0)
			{
				// can't change the pixel format of a window, so need to delete the
				// old one and create a new one..
				releaseDC();
				nativeWindow = nullptr;
				createNativeWindow (component);

				if (SetPixelFormat (dc, wglFormat, &pfd))
				{
					deleteRenderContext();
					renderContext = wglCreateContext (dc);
				}
			}

			if (contextToShareWith != nullptr)
				wglShareLists (contextToShareWith->renderContext, renderContext);

			component.getTopLevelComponent()->repaint();
			component.repaint();
		}
	}

	~NativeContext()
	{
		deleteRenderContext();
		releaseDC();
	}

	void initialiseOnRenderThread() {}
	void shutdownOnRenderThread() {}

	bool makeActive() const noexcept        { return wglMakeCurrent (dc, renderContext) != FALSE; }
	bool isActive() const noexcept          { return wglGetCurrentContext() == renderContext; }
	void swapBuffers() const noexcept       { SwapBuffers (dc); }

	bool setSwapInterval (int numFramesPerSwap)
	{
		jassert (isActive()); // this can only be called when the context is active..
		return wglSwapIntervalEXT != nullptr && wglSwapIntervalEXT (numFramesPerSwap) != FALSE;
	}

	int getSwapInterval() const
	{
		jassert (isActive()); // this can only be called when the context is active..
		return wglGetSwapIntervalEXT != nullptr ? wglGetSwapIntervalEXT() : 0;
	}

	void updateWindowPosition (const Rectangle<int>& bounds)
	{
		if (nativeWindow != nullptr)
			SetWindowPos ((HWND) nativeWindow->getNativeHandle(), 0,
						  bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
						  SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

	bool createdOk() const noexcept                 { return getRawContext() != nullptr; }
	void* getRawContext() const noexcept            { return renderContext; }
	unsigned int getFrameBufferID() const noexcept  { return 0; }

	struct Locker { Locker (NativeContext&) {} };

private:
	Component dummyComponent;
	ScopedPointer<ComponentPeer> nativeWindow;
	HGLRC renderContext;
	HDC dc;

	#define JUCE_DECLARE_WGL_EXTENSION_FUNCTION(name, returnType, params) \
		typedef returnType (__stdcall *type_ ## name) params; type_ ## name name;

	JUCE_DECLARE_WGL_EXTENSION_FUNCTION (wglChoosePixelFormatARB,  BOOL, (HDC, const int*, const FLOAT*, UINT, int*, UINT*))
	JUCE_DECLARE_WGL_EXTENSION_FUNCTION (wglSwapIntervalEXT,       BOOL, (int))
	JUCE_DECLARE_WGL_EXTENSION_FUNCTION (wglGetSwapIntervalEXT,    int, ())
	#undef JUCE_DECLARE_WGL_EXTENSION_FUNCTION

	void initialiseGLExtensions()
	{
		#define JUCE_INIT_WGL_FUNCTION(name)    name = (type_ ## name) OpenGLHelpers::getExtensionFunction (#name);
		JUCE_INIT_WGL_FUNCTION (wglChoosePixelFormatARB);
		JUCE_INIT_WGL_FUNCTION (wglSwapIntervalEXT);
		JUCE_INIT_WGL_FUNCTION (wglGetSwapIntervalEXT);
		#undef JUCE_INIT_WGL_FUNCTION
	}

	void createNativeWindow (Component& component)
	{
		Component* topComp = component.getTopLevelComponent();
		nativeWindow = createNonRepaintingEmbeddedWindowsPeer (&dummyComponent, topComp->getWindowHandle());
		updateWindowPosition (topComp->getLocalArea (&component, component.getLocalBounds()));
		nativeWindow->setVisible (true);
		dc = GetDC ((HWND) nativeWindow->getNativeHandle());
	}

	void deleteRenderContext()
	{
		if (renderContext != 0)
		{
			wglDeleteContext (renderContext);
			renderContext = 0;
		}
	}

	void releaseDC()
	{
		ReleaseDC ((HWND) nativeWindow->getNativeHandle(), dc);
	}

	static void initialisePixelFormatDescriptor (PIXELFORMATDESCRIPTOR& pfd, const OpenGLPixelFormat& pixelFormat)
	{
		zerostruct (pfd);
		pfd.nSize           = sizeof (pfd);
		pfd.nVersion        = 1;
		pfd.dwFlags         = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
		pfd.iPixelType      = PFD_TYPE_RGBA;
		pfd.iLayerType      = PFD_MAIN_PLANE;
		pfd.cColorBits      = (BYTE) (pixelFormat.redBits + pixelFormat.greenBits + pixelFormat.blueBits);
		pfd.cRedBits        = (BYTE) pixelFormat.redBits;
		pfd.cGreenBits      = (BYTE) pixelFormat.greenBits;
		pfd.cBlueBits       = (BYTE) pixelFormat.blueBits;
		pfd.cAlphaBits      = (BYTE) pixelFormat.alphaBits;
		pfd.cDepthBits      = (BYTE) pixelFormat.depthBufferBits;
		pfd.cStencilBits    = (BYTE) pixelFormat.stencilBufferBits;
		pfd.cAccumBits      = (BYTE) (pixelFormat.accumulationBufferRedBits + pixelFormat.accumulationBufferGreenBits
										+ pixelFormat.accumulationBufferBlueBits + pixelFormat.accumulationBufferAlphaBits);
		pfd.cAccumRedBits   = (BYTE) pixelFormat.accumulationBufferRedBits;
		pfd.cAccumGreenBits = (BYTE) pixelFormat.accumulationBufferGreenBits;
		pfd.cAccumBlueBits  = (BYTE) pixelFormat.accumulationBufferBlueBits;
		pfd.cAccumAlphaBits = (BYTE) pixelFormat.accumulationBufferAlphaBits;
	}

	int wglChoosePixelFormatExtension (const OpenGLPixelFormat& pixelFormat) const
	{
		int format = 0;

		if (wglChoosePixelFormatARB != nullptr)
		{
			int atts[64];
			int n = 0;

			atts[n++] = WGL_DRAW_TO_WINDOW_ARB;   atts[n++] = GL_TRUE;
			atts[n++] = WGL_SUPPORT_OPENGL_ARB;   atts[n++] = GL_TRUE;
			atts[n++] = WGL_DOUBLE_BUFFER_ARB;    atts[n++] = GL_TRUE;
			atts[n++] = WGL_PIXEL_TYPE_ARB;       atts[n++] = WGL_TYPE_RGBA_ARB;

			atts[n++] = WGL_COLOR_BITS_ARB;  atts[n++] = pixelFormat.redBits + pixelFormat.greenBits + pixelFormat.blueBits;
			atts[n++] = WGL_RED_BITS_ARB;    atts[n++] = pixelFormat.redBits;
			atts[n++] = WGL_GREEN_BITS_ARB;  atts[n++] = pixelFormat.greenBits;
			atts[n++] = WGL_BLUE_BITS_ARB;   atts[n++] = pixelFormat.blueBits;
			atts[n++] = WGL_ALPHA_BITS_ARB;  atts[n++] = pixelFormat.alphaBits;
			atts[n++] = WGL_DEPTH_BITS_ARB;  atts[n++] = pixelFormat.depthBufferBits;

			atts[n++] = WGL_STENCIL_BITS_ARB;       atts[n++] = pixelFormat.stencilBufferBits;
			atts[n++] = WGL_ACCUM_RED_BITS_ARB;     atts[n++] = pixelFormat.accumulationBufferRedBits;
			atts[n++] = WGL_ACCUM_GREEN_BITS_ARB;   atts[n++] = pixelFormat.accumulationBufferGreenBits;
			atts[n++] = WGL_ACCUM_BLUE_BITS_ARB;    atts[n++] = pixelFormat.accumulationBufferBlueBits;
			atts[n++] = WGL_ACCUM_ALPHA_BITS_ARB;   atts[n++] = pixelFormat.accumulationBufferAlphaBits;

			if (pixelFormat.multisamplingLevel > 0
				  && OpenGLHelpers::isExtensionSupported ("GL_ARB_multisample"))
			{
				atts[n++] = WGL_SAMPLE_BUFFERS_ARB;
				atts[n++] = 1;
				atts[n++] = WGL_SAMPLES_ARB;
				atts[n++] = pixelFormat.multisamplingLevel;
			}

			atts[n++] = 0;
			jassert (n <= numElementsInArray (atts));

			UINT formatsCount = 0;
			wglChoosePixelFormatARB (dc, atts, nullptr, 1, &format, &formatsCount);
		}

		return format;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext);
};

bool OpenGLHelpers::isContextActive()
{
	return wglGetCurrentContext() != 0;
}

/*** End of inlined file: juce_OpenGL_win32.h ***/


#elif JUCE_LINUX

/*** Start of inlined file: juce_OpenGL_linux.h ***/
extern Display* display;
extern XContext windowHandleXContext;

class OpenGLContext::NativeContext
{
public:
	NativeContext (Component& component,
				   const OpenGLPixelFormat& pixelFormat,
				   const NativeContext* contextToShareWith_)
		: renderContext (0), embeddedWindow (0), swapFrames (0), bestVisual (0),
		  contextToShareWith (contextToShareWith_)
	{
		ScopedXLock xlock;
		XSync (display, False);

		GLint attribs[] =
		{
			GLX_RGBA,
			GLX_DOUBLEBUFFER,
			GLX_RED_SIZE,         pixelFormat.redBits,
			GLX_GREEN_SIZE,       pixelFormat.greenBits,
			GLX_BLUE_SIZE,        pixelFormat.blueBits,
			GLX_ALPHA_SIZE,       pixelFormat.alphaBits,
			GLX_DEPTH_SIZE,       pixelFormat.depthBufferBits,
			GLX_STENCIL_SIZE,     pixelFormat.stencilBufferBits,
			GLX_ACCUM_RED_SIZE,   pixelFormat.accumulationBufferRedBits,
			GLX_ACCUM_GREEN_SIZE, pixelFormat.accumulationBufferGreenBits,
			GLX_ACCUM_BLUE_SIZE,  pixelFormat.accumulationBufferBlueBits,
			GLX_ACCUM_ALPHA_SIZE, pixelFormat.accumulationBufferAlphaBits,
			None
		};

		bestVisual = glXChooseVisual (display, DefaultScreen (display), attribs);
		if (bestVisual == nullptr)
			return;

		ComponentPeer* const peer = component.getPeer();
		Window windowH = (Window) peer->getNativeHandle();

		Colormap colourMap = XCreateColormap (display, windowH, bestVisual->visual, AllocNone);
		XSetWindowAttributes swa;
		swa.colormap = colourMap;
		swa.border_pixel = 0;
		swa.event_mask = ExposureMask | StructureNotifyMask;

		const Rectangle<int> bounds (component.getTopLevelComponent()
										->getLocalArea (&component, component.getLocalBounds()));

		embeddedWindow = XCreateWindow (display, windowH,
										bounds.getX(), bounds.getY(),
										jmax (1, bounds.getWidth()),
										jmax (1, bounds.getHeight()),
										0, bestVisual->depth,
										InputOutput,
										bestVisual->visual,
										CWBorderPixel | CWColormap | CWEventMask,
										&swa);

		XSaveContext (display, (XID) embeddedWindow, windowHandleXContext, (XPointer) peer);

		XMapWindow (display, embeddedWindow);
		XFreeColormap (display, colourMap);

		XSync (display, False);
	}

	~NativeContext()
	{
		if (embeddedWindow != 0)
		{
			ScopedXLock xlock;
			XUnmapWindow (display, embeddedWindow);
			XDestroyWindow (display, embeddedWindow);
		}

		if (bestVisual != nullptr)
			XFree (bestVisual);
	}

	void initialiseOnRenderThread()
	{
		ScopedXLock xlock;
		renderContext = glXCreateContext (display, bestVisual,
										  contextToShareWith != nullptr ? contextToShareWith->renderContext : 0,
										  GL_TRUE);
		makeActive();
	}

	void shutdownOnRenderThread()
	{
		glXMakeCurrent (display, None, 0);
		glXDestroyContext (display, renderContext);
		renderContext = nullptr;
	}

	bool makeActive() const noexcept
	{
		return renderContext != 0
				 && glXMakeCurrent (display, embeddedWindow, renderContext);
	}

	bool isActive() const noexcept
	{
		return glXGetCurrentContext() == renderContext && renderContext != 0;
	}

	void swapBuffers()
	{
		glXSwapBuffers (display, embeddedWindow);
	}

	void updateWindowPosition (const Rectangle<int>& newBounds)
	{
		bounds = newBounds;

		ScopedXLock xlock;
		XMoveResizeWindow (display, embeddedWindow,
						   bounds.getX(), bounds.getY(),
						   jmax (1, bounds.getWidth()),
						   jmax (1, bounds.getHeight()));
	}

	bool setSwapInterval (int numFramesPerSwap)
	{
		if (numFramesPerSwap == swapFrames)
			return true;

		PFNGLXSWAPINTERVALSGIPROC GLXSwapIntervalSGI
			= (PFNGLXSWAPINTERVALSGIPROC) OpenGLHelpers::getExtensionFunction ("glXSwapIntervalSGI");

		if (GLXSwapIntervalSGI != nullptr)
		{
			swapFrames = numFramesPerSwap;
			GLXSwapIntervalSGI (numFramesPerSwap);
			return true;
		}

		return false;
	}

	int getSwapInterval() const                 { return swapFrames; }
	bool createdOk() const noexcept             { return true; }
	void* getRawContext() const noexcept        { return renderContext; }
	GLuint getFrameBufferID() const noexcept    { return 0; }

	struct Locker { Locker (NativeContext&) {} };

private:
	GLXContext renderContext;
	Window embeddedWindow;

	int swapFrames;
	Rectangle<int> bounds;
	XVisualInfo* bestVisual;
	const NativeContext* contextToShareWith;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext);
};

bool OpenGLHelpers::isContextActive()
{
	ScopedXLock xlock;
	return glXGetCurrentContext() != 0;
}

/*** End of inlined file: juce_OpenGL_linux.h ***/


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



/*** Start of inlined file: juce_OpenGL_android.h ***/
#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD) \
 METHOD (layout,        "layout",        "(IIII)V") \
 METHOD (requestRender, "requestRender", "()V") \

DECLARE_JNI_CLASS (OpenGLView, JUCE_ANDROID_ACTIVITY_CLASSPATH "$OpenGLView");
#undef JNI_CLASS_MEMBERS

extern jobject createOpenGLView (ComponentPeer*);

class OpenGLContext::NativeContext
{
public:
	NativeContext (Component& component_,
				   const OpenGLPixelFormat& pixelFormat,
				   const NativeContext* contextToShareWith)
		: component (component_),
		  isInsideGLCallback (false)
	{
		{
			const ScopedLock sl (contextListLock);
			glView = GlobalRef (createOpenGLView (component.getPeer()));
			contextList.add (this);
		}

		updateWindowPosition (component.getTopLevelComponent()
								->getLocalArea (&component, component.getLocalBounds()));
	}

	~NativeContext()
	{
		{
			const ScopedLock sl (contextListLock);
			contextList.removeValue (this);
		}

		android.activity.callVoidMethod (JuceAppActivity.deleteView, glView.get());
		glView.clear();
	}

	void initialiseOnRenderThread() {}
	void shutdownOnRenderThread() {}

	bool makeActive() const noexcept            { return isInsideGLCallback; }
	bool isActive() const noexcept              { return isInsideGLCallback; }

	void swapBuffers() const noexcept           {}
	bool setSwapInterval (const int)            { return false; }
	int getSwapInterval() const                 { return 0; }

	bool createdOk() const noexcept             { return getRawContext() != nullptr; }
	void* getRawContext() const noexcept        { return glView.get(); }
	GLuint getFrameBufferID() const noexcept    { return 0; }

	void updateWindowPosition (const Rectangle<int>& bounds)
	{
		if (lastBounds != bounds)
		{
			lastBounds = bounds;
			glView.callVoidMethod (OpenGLView.layout,
								   bounds.getX(), bounds.getY(),
								   bounds.getRight(), bounds.getBottom());
		}
	}

	void triggerRepaint()
	{
		glView.callVoidMethod (OpenGLView.requestRender);
	}

	void contextCreatedCallback();
	void contextChangedSize() {}
	void renderCallback();

	static NativeContext* findContextFor (JNIEnv* env, jobject glView)
	{
		const ScopedLock sl (contextListLock);

		for (int i = contextList.size(); --i >= 0;)
		{
			NativeContext* const c = contextList.getUnchecked(i);

			if (env->IsSameObject (c->glView.get(), glView))
				return c;
		}

		return nullptr;
	}

	static NativeContext* getActiveContext() noexcept
	{
		const ScopedLock sl (contextListLock);

		for (int i = contextList.size(); --i >= 0;)
		{
			NativeContext* const c = contextList.getUnchecked(i);

			if (c->isInsideGLCallback)
				return c;
		}

		return nullptr;
	}

	struct Locker { Locker (NativeContext&) {} };

	Component& component;

private:
	GlobalRef glView;
	Rectangle<int> lastBounds;
	bool isInsideGLCallback;

	typedef Array<NativeContext*> ContextArray;
	static CriticalSection contextListLock;
	static ContextArray contextList;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext);
};

CriticalSection OpenGLContext::NativeContext::contextListLock;
OpenGLContext::NativeContext::ContextArray OpenGLContext::NativeContext::contextList;

bool OpenGLHelpers::isContextActive()
{
	return OpenGLContext::NativeContext::getActiveContext() != nullptr;
}

#define GL_VIEW_CLASS_NAME    JUCE_JOIN_MACRO (JUCE_ANDROID_ACTIVITY_CLASSNAME, _00024OpenGLView)

JUCE_JNI_CALLBACK (GL_VIEW_CLASS_NAME, contextCreated, void, (JNIEnv* env, jobject view))
{
	threadLocalJNIEnvHolder.getOrAttach();

	OpenGLContext::NativeContext* const context = OpenGLContext::NativeContext::findContextFor (env, view);
	jassert (context != nullptr);

	if (context != nullptr)
		context->contextCreatedCallback();
}

JUCE_JNI_CALLBACK (GL_VIEW_CLASS_NAME, contextChangedSize, void, (JNIEnv* env, jobject view))
{
	OpenGLContext::NativeContext* const context = OpenGLContext::NativeContext::findContextFor (env, view);

	if (context != nullptr)
		context->contextChangedSize();
}

JUCE_JNI_CALLBACK (GL_VIEW_CLASS_NAME, render, void, (JNIEnv* env, jobject view))
{
	OpenGLContext::NativeContext* const context = OpenGLContext::NativeContext::findContextFor (env, view);

	if (context != nullptr)
		context->renderCallback();
}

/*** End of inlined file: juce_OpenGL_android.h ***/

#endif


/*** Start of inlined file: juce_OpenGLContext.cpp ***/
class OpenGLContext::CachedImage  : public CachedComponentImage,
									public Thread
{
public:
	CachedImage (OpenGLContext& context_,
				 Component& component_,
				 const OpenGLPixelFormat& pixelFormat,
				 const OpenGLContext* contextToShareWith)
		: Thread ("OpenGL Rendering"),
		  context (context_), component (component_),
		 #if JUCE_OPENGL_ES
		  shadersAvailable (true),
		 #else
		  shadersAvailable (false),
		 #endif
		  needsUpdate (true)
	{
		nativeContext = new NativeContext (component, pixelFormat,
										   contextToShareWith != nullptr ? contextToShareWith->nativeContext
																		 : nullptr);

		if (nativeContext->createdOk())
		{
			context.nativeContext = nativeContext;

		   #if ! JUCE_ANDROID
			startThread (6);
		   #endif
		}
		else
		{
			nativeContext = nullptr;
		}
	}

	~CachedImage()
	{
	   #if ! JUCE_ANDROID
		stopThread (10000);
	   #endif
	}

	void paint (Graphics&)
	{
		ComponentPeer* const peer = component.getPeer();

		if (peer != nullptr)
			peer->addMaskedRegion (peer->getComponent()->getLocalArea (&component, component.getLocalBounds()));
	}

	void invalidateAll()
	{
		validArea.clear();
		triggerRepaint();
	}

	void invalidate (const Rectangle<int>& area)
	{
		validArea.subtract (area);
		triggerRepaint();
	}

	void releaseResources() {}

	void triggerRepaint()
	{
		needsUpdate = true;

	   #if JUCE_ANDROID
		if (nativeContext != nullptr)
			nativeContext->triggerRepaint();
	   #else
		notify();
	   #endif
	}

	bool ensureFrameBufferSize (int width, int height)
	{
		const int fbW = cachedImageFrameBuffer.getWidth();
		const int fbH = cachedImageFrameBuffer.getHeight();

		if (fbW != width || fbH != height || ! cachedImageFrameBuffer.isValid())
		{
			if (! cachedImageFrameBuffer.initialise (context, width, height))
				return false;

			validArea.clear();
			JUCE_CHECK_OPENGL_ERROR
		}

		return true;
	}

	void clearRegionInFrameBuffer (const RectangleList& list)
	{
		glClearColor (0, 0, 0, 0);
		glEnable (GL_SCISSOR_TEST);

		const GLuint previousFrameBufferTarget = OpenGLFrameBuffer::getCurrentFrameBufferTarget();
		cachedImageFrameBuffer.makeCurrentRenderingTarget();

		for (RectangleList::Iterator i (list); i.next();)
		{
			const Rectangle<int>& r = *i.getRectangle();
			glScissor (r.getX(), component.getHeight() - r.getBottom(), r.getWidth(), r.getHeight());
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		glDisable (GL_SCISSOR_TEST);
		context.extensions.glBindFramebuffer (GL_FRAMEBUFFER, previousFrameBufferTarget);
		JUCE_CHECK_OPENGL_ERROR
	}

	bool renderFrame()
	{
		if (! context.makeActive())
			return false;

		NativeContext::Locker locker (*nativeContext);

		JUCE_CHECK_OPENGL_ERROR
		glViewport (0, 0, component.getWidth(), component.getHeight());

		if (context.renderer != nullptr)
		{
			context.renderer->renderOpenGL();
			clearGLError();
		}

		if (context.renderComponents)
			paintComponent();

		context.swapBuffers();
		return true;
	}

	void paintComponent()
	{
		if (needsUpdate)
		{
			MessageManagerLock mm (this);
			if (! mm.lockWasGained())
				return;

			needsUpdate = false;

			// you mustn't set your own cached image object when attaching a GL context!
			jassert (get (component) == this);

			const Rectangle<int> bounds (component.getLocalBounds());
			if (! ensureFrameBufferSize (bounds.getWidth(), bounds.getHeight()))
				return;

			RectangleList invalid (bounds);
			invalid.subtract (validArea);
			validArea = bounds;

			if (! invalid.isEmpty())
			{
				clearRegionInFrameBuffer (invalid);

				{
					ScopedPointer<LowLevelGraphicsContext> g (createOpenGLGraphicsContext (context, cachedImageFrameBuffer));
					g->clipToRectangleList (invalid);
					paintOwner (*g);
					JUCE_CHECK_OPENGL_ERROR
				}

				context.makeActive();
			}

			JUCE_CHECK_OPENGL_ERROR
		}

	   #if ! JUCE_ANDROID
		glEnable (GL_TEXTURE_2D);
		clearGLError();
	   #endif
		context.extensions.glActiveTexture (GL_TEXTURE0);
		glBindTexture (GL_TEXTURE_2D, cachedImageFrameBuffer.getTextureID());

		const Rectangle<int> cacheBounds (cachedImageFrameBuffer.getWidth(), cachedImageFrameBuffer.getHeight());
		context.copyTexture (cacheBounds, cacheBounds, context.getWidth(), context.getHeight());
		glBindTexture (GL_TEXTURE_2D, 0);
		JUCE_CHECK_OPENGL_ERROR
	}

	void paintOwner (LowLevelGraphicsContext& context)
	{
		Graphics g (&context);

	   #if JUCE_ENABLE_REPAINT_DEBUGGING
		g.saveState();
	   #endif

		JUCE_TRY
		{
			component.paintEntireComponent (g, false);
		}
		JUCE_CATCH_EXCEPTION

	   #if JUCE_ENABLE_REPAINT_DEBUGGING
		// enabling this code will fill all areas that get repainted with a colour overlay, to show
		// clearly when things are being repainted.
		g.restoreState();

		static Random rng;
		g.fillAll (Colour ((uint8) rng.nextInt (255),
						   (uint8) rng.nextInt (255),
						   (uint8) rng.nextInt (255),
						   (uint8) 0x50));
	   #endif
	}

	void run()
	{
		{
			// Allow the message thread to finish setting-up the context before using it..
			MessageManagerLock mml (this);
			if (! mml.lockWasGained())
				return;
		}

		nativeContext->makeActive();
		initialiseOnThread();

	   #if JUCE_USE_OPENGL_SHADERS && ! JUCE_OPENGL_ES
		shadersAvailable = OpenGLShaderProgram::getLanguageVersion() > 0;
	   #endif

		while (! threadShouldExit())
		{
			const uint32 frameRenderStartTime = Time::getMillisecondCounter();

			if (renderFrame())
				waitForNextFrame (frameRenderStartTime);
		}

		shutdownOnThread();
	}

	void initialiseOnThread()
	{
		associatedObjectNames.clear();
		associatedObjects.clear();

		nativeContext->initialiseOnRenderThread();
		glViewport (0, 0, component.getWidth(), component.getHeight());

		context.extensions.initialise();

		if (context.renderer != nullptr)
			context.renderer->newOpenGLContextCreated();
	}

	void shutdownOnThread()
	{
		if (context.renderer != nullptr)
			context.renderer->openGLContextClosing();

		nativeContext->shutdownOnRenderThread();

		associatedObjectNames.clear();
		associatedObjects.clear();
	}

	void waitForNextFrame (const uint32 frameRenderStartTime)
	{
		const int defaultFPS = 60;

		const int elapsed = (int) (Time::getMillisecondCounter() - frameRenderStartTime);
		wait (jmax (1, (1000 / defaultFPS) - elapsed));
	}

	static CachedImage* get (Component& c) noexcept
	{
		return dynamic_cast<CachedImage*> (c.getCachedComponentImage());
	}

	ScopedPointer<NativeContext> nativeContext;

	OpenGLContext& context;
	Component& component;

	OpenGLFrameBuffer cachedImageFrameBuffer;
	RectangleList validArea;

	StringArray associatedObjectNames;
	ReferenceCountedArray<ReferenceCountedObject> associatedObjects;

	WaitableEvent canPaintNowFlag, finishedPaintingFlag;
	bool volatile shadersAvailable;
	bool volatile needsUpdate;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CachedImage);
};

#if JUCE_ANDROID
void OpenGLContext::NativeContext::contextCreatedCallback()
{
	isInsideGLCallback = true;

	CachedImage* const c = CachedImage::get (component);
	jassert (c != nullptr);

	if (c != nullptr)
		c->initialiseOnThread();

	isInsideGLCallback = false;
}

void OpenGLContext::NativeContext::renderCallback()
{
	isInsideGLCallback = true;

	CachedImage* const c = CachedImage::get (component);

	if (c != nullptr)
		c->renderFrame();

	isInsideGLCallback = false;
}
#endif

class OpenGLContext::Attachment  : public ComponentMovementWatcher
{
public:
	Attachment (OpenGLContext& context_, Component& comp)
	   : ComponentMovementWatcher (&comp), context (context_)
	{
		if (canBeAttached (comp))
			attach();
	}

	~Attachment()
	{
		detach();
	}

	void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/)
	{
		Component* const comp = getComponent();

		if (isAttached (*comp) != canBeAttached (*comp))
			componentVisibilityChanged();

		context.width  = comp->getWidth();
		context.height = comp->getHeight();

		if (comp->getWidth() > 0 && comp->getHeight() > 0
			 && context.nativeContext != nullptr)
		{
			context.nativeContext->updateWindowPosition (comp->getTopLevelComponent()
															->getLocalArea (comp, comp->getLocalBounds()));
		}
	}

	void componentPeerChanged()
	{
		detach();
		componentVisibilityChanged();
	}

	void componentVisibilityChanged()
	{
		Component* const comp = getComponent();

		if (canBeAttached (*comp))
		{
			if (! isAttached (*comp))
				attach();
		}
		else
		{
			detach();
		}
	}

   #if JUCE_DEBUG || JUCE_LOG_ASSERTIONS
	void componentBeingDeleted (Component& component)
	{
		/* You must call detach() or delete your OpenGLContext to remove it
		   from a component BEFORE deleting the component that it is using!
		*/
		jassertfalse;

		ComponentMovementWatcher::componentBeingDeleted (component);
	}
   #endif

private:
	OpenGLContext& context;

	static bool canBeAttached (const Component& comp) noexcept
	{
		return comp.getWidth() > 0 && comp.getHeight() > 0 && comp.isShowing();
	}

	static bool isAttached (const Component& comp) noexcept
	{
		return comp.getCachedComponentImage() != nullptr;
	}

	void attach()
	{
		Component* const comp = getComponent();
		comp->setCachedComponentImage (new CachedImage (context, *comp,
														context.pixelFormat,
														context.contextToShareWith));
	}

	void detach()
	{
		getComponent()->setCachedComponentImage (nullptr);
		context.nativeContext = nullptr;
	}
};

OpenGLContext::OpenGLContext()
	: nativeContext (nullptr), renderer (nullptr), contextToShareWith (nullptr),
	  width (0), height (0), renderComponents (true)
{
}

OpenGLContext::~OpenGLContext()
{
	detach();
}

void OpenGLContext::setRenderer (OpenGLRenderer* rendererToUse) noexcept
{
	// This method must not be called when the context has already been attached!
	// Call it before attaching your context, or use detach() first, before calling this!
	jassert (nativeContext == nullptr);

	renderer = rendererToUse;
}

void OpenGLContext::setComponentPaintingEnabled (bool shouldPaintComponent) noexcept
{
	// This method must not be called when the context has already been attached!
	// Call it before attaching your context, or use detach() first, before calling this!
	jassert (nativeContext == nullptr);

	renderComponents = shouldPaintComponent;
}

void OpenGLContext::setPixelFormat (const OpenGLPixelFormat& preferredPixelFormat) noexcept
{
	// This method must not be called when the context has already been attached!
	// Call it before attaching your context, or use detach() first, before calling this!
	jassert (nativeContext == nullptr);

	pixelFormat = preferredPixelFormat;
}

void OpenGLContext::setContextToShareWith (const OpenGLContext* context) noexcept
{
	// This method must not be called when the context has already been attached!
	// Call it before attaching your context, or use detach() first, before calling this!
	jassert (nativeContext == nullptr);

	contextToShareWith = context;
}

void OpenGLContext::attachTo (Component& component)
{
	component.repaint();

	if (getTargetComponent() != &component)
	{
		detach();

		width  = component.getWidth();
		height = component.getHeight();

		attachment = new Attachment (*this, component);
	}
}

void OpenGLContext::detach()
{
	attachment = nullptr;
	nativeContext = nullptr;
	width = height = 0;
}

bool OpenGLContext::isAttached() const noexcept
{
	return nativeContext != nullptr;
}

Component* OpenGLContext::getTargetComponent() const noexcept
{
	return attachment != nullptr ? attachment->getComponent() : nullptr;
}

OpenGLContext* OpenGLContext::getCurrentContext()
{
   #if JUCE_ANDROID
	NativeContext* const nc = NativeContext::getActiveContext();
	if (nc == nullptr)
		return nullptr;

	CachedImage* currentContext = CachedImage::get (nc->component);
   #else
	CachedImage* currentContext = dynamic_cast <CachedImage*> (Thread::getCurrentThread());
   #endif

	return currentContext != nullptr ? &currentContext->context : nullptr;
}

bool OpenGLContext::makeActive() const noexcept     { return nativeContext != nullptr && nativeContext->makeActive(); }
bool OpenGLContext::isActive() const noexcept       { return nativeContext != nullptr && nativeContext->isActive(); }

void OpenGLContext::triggerRepaint()
{
	CachedImage* const currentContext
			= dynamic_cast <CachedImage*> (Thread::getCurrentThread());

	if (currentContext != nullptr)
	{
		currentContext->triggerRepaint();
		currentContext->component.repaint();
	}
}

void OpenGLContext::swapBuffers()
{
	if (nativeContext != nullptr)
		nativeContext->swapBuffers();
}

unsigned int OpenGLContext::getFrameBufferID() const noexcept
{
	return nativeContext != nullptr ? nativeContext->getFrameBufferID() : 0;
}

bool OpenGLContext::setSwapInterval (int numFramesPerSwap)
{
	return nativeContext != nullptr && nativeContext->setSwapInterval (numFramesPerSwap);
}

int OpenGLContext::getSwapInterval() const
{
	return nativeContext != nullptr ? nativeContext->getSwapInterval() : 0;
}

void* OpenGLContext::getRawContext() const noexcept
{
	return nativeContext != nullptr ? nativeContext->getRawContext() : nullptr;
}

OpenGLContext::CachedImage* OpenGLContext::getCachedImage() const noexcept
{
	Component* const comp = getTargetComponent();
	return comp != nullptr ? CachedImage::get (*comp) : nullptr;
}

bool OpenGLContext::areShadersAvailable() const
{
	CachedImage* const c = getCachedImage();
	return c != nullptr && c->shadersAvailable;
}

ReferenceCountedObject* OpenGLContext::getAssociatedObject (const char* name) const
{
	jassert (name != nullptr);

	CachedImage* const c = getCachedImage();

	// This method must only be called from an openGL rendering callback.
	jassert (c != nullptr && nativeContext != nullptr);
	jassert (getCurrentContext() != nullptr);

	const int index = c->associatedObjectNames.indexOf (name);
	return index >= 0 ? c->associatedObjects.getUnchecked (index) : nullptr;
}

void OpenGLContext::setAssociatedObject (const char* name, ReferenceCountedObject* newObject)
{
	jassert (name != nullptr);

	CachedImage* const c = getCachedImage();

	// This method must only be called from an openGL rendering callback.
	jassert (c != nullptr && nativeContext != nullptr);
	jassert (getCurrentContext() != nullptr);

	const int index = c->associatedObjectNames.indexOf (name);

	if (index >= 0)
	{
		c->associatedObjects.set (index, newObject);
	}
	else
	{
		c->associatedObjectNames.add (name);
		c->associatedObjects.add (newObject);
	}
}

void OpenGLContext::copyTexture (const Rectangle<int>& targetClipArea,
								 const Rectangle<int>& anchorPosAndTextureSize,
								 const int contextWidth, const int contextHeight)
{
	if (contextWidth <= 0 || contextHeight <= 0)
		return;

	JUCE_CHECK_OPENGL_ERROR
	glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);

   #if JUCE_USE_OPENGL_SHADERS
	if (areShadersAvailable())
	{
		struct OverlayShaderProgram  : public ReferenceCountedObject
		{
			OverlayShaderProgram (OpenGLContext& context)
				: program (context), builder (program), params (program)
			{}

			static const OverlayShaderProgram& select (OpenGLContext& context)
			{
				static const char programValueID[] = "juceGLComponentOverlayShader";
				OverlayShaderProgram* program = static_cast <OverlayShaderProgram*> (context.getAssociatedObject (programValueID));

				if (program == nullptr)
				{
					program = new OverlayShaderProgram (context);
					context.setAssociatedObject (programValueID, program);
				}

				program->program.use();
				return *program;
			}

			struct ProgramBuilder
			{
				ProgramBuilder (OpenGLShaderProgram& program)
				{
					program.addShader ("attribute " JUCE_HIGHP " vec2 position;"
									   "uniform " JUCE_HIGHP " vec2 screenSize;"
									   "varying " JUCE_HIGHP " vec2 pixelPos;"
									   "void main()"
									   "{"
										"pixelPos = position;"
										JUCE_HIGHP " vec2 scaled = position / (0.5 * screenSize.xy);"
										"gl_Position = vec4 (scaled.x - 1.0, 1.0 - scaled.y, 0, 1.0);"
									   "}",
									   GL_VERTEX_SHADER);

					program.addShader ("uniform sampler2D imageTexture;"
									   "uniform " JUCE_HIGHP " float textureBounds[4];"
									   "varying " JUCE_HIGHP " vec2 pixelPos;"
									   "void main()"
									   "{"
										JUCE_HIGHP " vec2 texturePos = (pixelPos - vec2 (textureBounds[0], textureBounds[1]))"
																		 "/ vec2 (textureBounds[2], textureBounds[3]);"
										"gl_FragColor = texture2D (imageTexture, vec2 (texturePos.x, 1.0 - texturePos.y));"
									   "}",
									   GL_FRAGMENT_SHADER);
					program.link();
				}
			};

			struct Params
			{
				Params (OpenGLShaderProgram& program)
					: positionAttribute (program, "position"),
					  screenSize (program, "screenSize"),
					  imageTexture (program, "imageTexture"),
					  textureBounds (program, "textureBounds")
				{}

				void set (const float targetWidth, const float targetHeight, const Rectangle<float>& bounds) const
				{
					const GLfloat m[] = { bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight() };
					textureBounds.set (m, 4);
					imageTexture.set (0);
					screenSize.set (targetWidth, targetHeight);
				}

				OpenGLShaderProgram::Attribute positionAttribute;
				OpenGLShaderProgram::Uniform screenSize, imageTexture, textureBounds;
			};

			OpenGLShaderProgram program;
			ProgramBuilder builder;
			Params params;
		};

		const GLshort left   = (GLshort) targetClipArea.getX();
		const GLshort top    = (GLshort) targetClipArea.getY();
		const GLshort right  = (GLshort) targetClipArea.getRight();
		const GLshort bottom = (GLshort) targetClipArea.getBottom();
		const GLshort vertices[] = { left, bottom, right, bottom, left, top, right, top };

		const OverlayShaderProgram& program = OverlayShaderProgram::select (*this);
		program.params.set ((float) contextWidth, (float) contextHeight, anchorPosAndTextureSize.toFloat());

		extensions.glVertexAttribPointer (program.params.positionAttribute.attributeID, 2, GL_SHORT, GL_FALSE, 4, vertices);
		extensions.glEnableVertexAttribArray (program.params.positionAttribute.attributeID);
		JUCE_CHECK_OPENGL_ERROR

		glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);

		extensions.glUseProgram (0);
		extensions.glDisableVertexAttribArray (program.params.positionAttribute.attributeID);
	}
	#if JUCE_USE_OPENGL_FIXED_FUNCTION
	else
	#endif
   #endif

   #if JUCE_USE_OPENGL_FIXED_FUNCTION
	{
		glEnable (GL_SCISSOR_TEST);
		glScissor (targetClipArea.getX(), contextHeight - targetClipArea.getBottom(),
				   targetClipArea.getWidth(), targetClipArea.getHeight());

		JUCE_CHECK_OPENGL_ERROR
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glDisableClientState (GL_COLOR_ARRAY);
		glDisableClientState (GL_NORMAL_ARRAY);
		glEnableClientState (GL_VERTEX_ARRAY);
		glEnableClientState (GL_TEXTURE_COORD_ARRAY);
		OpenGLHelpers::prepareFor2D (contextWidth, contextHeight);
		JUCE_CHECK_OPENGL_ERROR

		const GLfloat textureCoords[] = { 0, 0, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f };
		glTexCoordPointer (2, GL_FLOAT, 0, textureCoords);

		const GLshort left   = (GLshort) anchorPosAndTextureSize.getX();
		const GLshort right  = (GLshort) anchorPosAndTextureSize.getRight();
		const GLshort top    = (GLshort) (contextHeight - anchorPosAndTextureSize.getY());
		const GLshort bottom = (GLshort) (contextHeight - anchorPosAndTextureSize.getBottom());
		const GLshort vertices[] = { left, bottom, right, bottom, left, top, right, top };
		glVertexPointer (2, GL_SHORT, 0, vertices);

		glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
		glDisable (GL_SCISSOR_TEST);
	}
   #endif

	JUCE_CHECK_OPENGL_ERROR
}

/*** End of inlined file: juce_OpenGLContext.cpp ***/

}

/*** End of inlined file: juce_opengl.cpp ***/


#endif

