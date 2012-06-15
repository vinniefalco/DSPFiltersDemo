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

/*** Start of inlined file: juce_gui_extra.cpp ***/
#if defined (__JUCE_GUI_EXTRA_JUCEHEADER__) && ! JUCE_AMALGAMATED_INCLUDE
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

#if JUCE_MAC
 #define Point CarbonDummyPointName
 #define Component CarbonDummyCompName
 #import <WebKit/WebKit.h>
 #import <IOKit/IOKitLib.h>
 #import <IOKit/IOCFPlugIn.h>
 #import <IOKit/hid/IOHIDLib.h>
 #import <IOKit/hid/IOHIDKeys.h>
 #import <IOKit/pwr_mgt/IOPMLib.h>
 #import <Carbon/Carbon.h> // still needed for SetSystemUIMode()
 #undef Point
 #undef Component

#elif JUCE_IOS

#elif JUCE_WINDOWS
 #include <windowsx.h>
 #include <vfw.h>
 #include <commdlg.h>

 #if JUCE_WEB_BROWSER
  #include <Exdisp.h>
  #include <exdispid.h>
 #endif

#elif JUCE_LINUX
 #include <X11/Xlib.h>
 #include <X11/Xatom.h>
 #include <X11/Xutil.h>
 #undef SIZEOF
 #undef KeyPress
#endif

namespace juce
{

// START_AUTOINCLUDE documents/*.cpp, code_editor/*.cpp, embedding/*.cpp, lookandfeel/*.cpp, misc/*.cpp

/*** Start of inlined file: juce_FileBasedDocument.cpp ***/
FileBasedDocument::FileBasedDocument (const String& fileExtension_,
									  const String& fileWildcard_,
									  const String& openFileDialogTitle_,
									  const String& saveFileDialogTitle_)
	: changedSinceSave (false),
	  fileExtension (fileExtension_),
	  fileWildcard (fileWildcard_),
	  openFileDialogTitle (openFileDialogTitle_),
	  saveFileDialogTitle (saveFileDialogTitle_)
{
}

FileBasedDocument::~FileBasedDocument()
{
}

void FileBasedDocument::setChangedFlag (const bool hasChanged)
{
	if (changedSinceSave != hasChanged)
	{
		changedSinceSave = hasChanged;
		sendChangeMessage();
	}
}

void FileBasedDocument::changed()
{
	changedSinceSave = true;
	sendChangeMessage();
}

void FileBasedDocument::setFile (const File& newFile)
{
	if (documentFile != newFile)
	{
		documentFile = newFile;
		changed();
	}
}

#if JUCE_MODAL_LOOPS_PERMITTED
bool FileBasedDocument::loadFrom (const File& newFile,
								  const bool showMessageOnFailure)
{
	MouseCursor::showWaitCursor();

	const File oldFile (documentFile);
	documentFile = newFile;

	String error;

	if (newFile.existsAsFile())
	{
		error = loadDocument (newFile);

		if (error.isEmpty())
		{
			setChangedFlag (false);
			MouseCursor::hideWaitCursor();

			setLastDocumentOpened (newFile);
			return true;
		}
	}
	else
	{
		error = "The file doesn't exist";
	}

	documentFile = oldFile;
	MouseCursor::hideWaitCursor();

	if (showMessageOnFailure)
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 TRANS("Failed to open file..."),
									 TRANS("There was an error while trying to load the file:\n\n")
									   + newFile.getFullPathName()
									   + "\n\n"
									   + error);
	}

	return false;
}

bool FileBasedDocument::loadFromUserSpecifiedFile (const bool showMessageOnFailure)
{
	FileChooser fc (openFileDialogTitle,
					getLastDocumentOpened(),
					fileWildcard);

	if (fc.browseForFileToOpen())
		return loadFrom (fc.getResult(), showMessageOnFailure);

	return false;
}

FileBasedDocument::SaveResult FileBasedDocument::save (const bool askUserForFileIfNotSpecified,
													   const bool showMessageOnFailure)
{
	return saveAs (documentFile,
				   false,
				   askUserForFileIfNotSpecified,
				   showMessageOnFailure);
}

FileBasedDocument::SaveResult FileBasedDocument::saveAs (const File& newFile,
														 const bool warnAboutOverwritingExistingFiles,
														 const bool askUserForFileIfNotSpecified,
														 const bool showMessageOnFailure)
{
	if (newFile == File::nonexistent)
	{
		if (askUserForFileIfNotSpecified)
		{
			return saveAsInteractive (true);
		}
		else
		{
			// can't save to an unspecified file
			jassertfalse;
			return failedToWriteToFile;
		}
	}

	if (warnAboutOverwritingExistingFiles && newFile.exists())
	{
		if (! AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
											TRANS("File already exists"),
											TRANS("There's already a file called:\n\n")
											  + newFile.getFullPathName()
											  + TRANS("\n\nAre you sure you want to overwrite it?"),
											TRANS("overwrite"),
											TRANS("cancel")))
		{
			return userCancelledSave;
		}
	}

	MouseCursor::showWaitCursor();

	const File oldFile (documentFile);
	documentFile = newFile;

	String error (saveDocument (newFile));

	if (error.isEmpty())
	{
		setChangedFlag (false);
		MouseCursor::hideWaitCursor();

		return savedOk;
	}

	documentFile = oldFile;
	MouseCursor::hideWaitCursor();

	if (showMessageOnFailure)
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 TRANS("Error writing to file..."),
									 TRANS("An error occurred while trying to save \"")
										+ getDocumentTitle()
										+ TRANS("\" to the file:\n\n")
										+ newFile.getFullPathName()
										+ "\n\n"
										+ error);
	}

	return failedToWriteToFile;
}

FileBasedDocument::SaveResult FileBasedDocument::saveIfNeededAndUserAgrees()
{
	if (! hasChangedSinceSaved())
		return savedOk;

	const int r = AlertWindow::showYesNoCancelBox (AlertWindow::QuestionIcon,
												   TRANS("Closing document..."),
												   TRANS("Do you want to save the changes to \"")
													   + getDocumentTitle() + "\"?",
												   TRANS("save"),
												   TRANS("discard changes"),
												   TRANS("cancel"));

	if (r == 1)
	{
		// save changes
		return save (true, true);
	}
	else if (r == 2)
	{
		// discard changes
		return savedOk;
	}

	return userCancelledSave;
}

File FileBasedDocument::getSuggestedSaveAsFile (const File& defaultFile)
{
	return defaultFile.withFileExtension (fileExtension).getNonexistentSibling (true);
}

FileBasedDocument::SaveResult FileBasedDocument::saveAsInteractive (const bool warnAboutOverwritingExistingFiles)
{
	File f;

	if (documentFile.existsAsFile())
		f = documentFile;
	else
		f = getLastDocumentOpened();

	String legalFilename (File::createLegalFileName (getDocumentTitle()));

	if (legalFilename.isEmpty())
		legalFilename = "unnamed";

	if (f.existsAsFile() || f.getParentDirectory().isDirectory())
		f = f.getSiblingFile (legalFilename);
	else
		f = File::getSpecialLocation (File::userDocumentsDirectory).getChildFile (legalFilename);

	f = getSuggestedSaveAsFile (f);

	FileChooser fc (saveFileDialogTitle, f, fileWildcard);

	if (fc.browseForFileToSave (warnAboutOverwritingExistingFiles))
	{
		File chosen (fc.getResult());
		if (chosen.getFileExtension().isEmpty())
		{
			chosen = chosen.withFileExtension (fileExtension);

			if (chosen.exists())
			{
				if (! AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
													TRANS("File already exists"),
													TRANS("There's already a file called:")
													  + "\n\n" + chosen.getFullPathName()
													  + "\n\n" + TRANS("Are you sure you want to overwrite it?"),
													TRANS("overwrite"),
													TRANS("cancel")))
				{
					return userCancelledSave;
				}
			}
		}

		setLastDocumentOpened (chosen);
		return saveAs (chosen, false, false, true);
	}

	return userCancelledSave;
}
#endif

/*** End of inlined file: juce_FileBasedDocument.cpp ***/



/*** Start of inlined file: juce_CodeDocument.cpp ***/
class CodeDocumentLine
{
public:
	CodeDocumentLine (const String::CharPointerType& line_,
					  const int lineLength_,
					  const int numNewLineChars,
					  const int lineStartInFile_)
		: line (line_, (size_t) lineLength_),
		  lineStartInFile (lineStartInFile_),
		  lineLength (lineLength_),
		  lineLengthWithoutNewLines (lineLength_ - numNewLineChars)
	{
	}

	static void createLines (Array <CodeDocumentLine*>& newLines, const String& text)
	{
		String::CharPointerType t (text.getCharPointer());
		int charNumInFile = 0;
		bool finished = false;

		while (! (finished || t.isEmpty()))
		{
			String::CharPointerType startOfLine (t);
			int startOfLineInFile = charNumInFile;
			int lineLength = 0;
			int numNewLineChars = 0;

			for (;;)
			{
				const juce_wchar c = t.getAndAdvance();

				if (c == 0)
				{
					finished = true;
					break;
				}

				++charNumInFile;
				++lineLength;

				if (c == '\r')
				{
					++numNewLineChars;

					if (*t == '\n')
					{
						++t;
						++charNumInFile;
						++lineLength;
						++numNewLineChars;
					}

					break;
				}

				if (c == '\n')
				{
					++numNewLineChars;
					break;
				}
			}

			newLines.add (new CodeDocumentLine (startOfLine, lineLength,
												numNewLineChars, startOfLineInFile));
		}

		jassert (charNumInFile == text.length());
	}

	bool endsWithLineBreak() const noexcept
	{
		return lineLengthWithoutNewLines != lineLength;
	}

	void updateLength() noexcept
	{
		lineLength = 0;
		lineLengthWithoutNewLines = 0;

		String::CharPointerType t (line.getCharPointer());

		for (;;)
		{
			const juce_wchar c = t.getAndAdvance();

			if (c == 0)
				break;

			++lineLength;

			if (c != '\n' && c != '\r')
				lineLengthWithoutNewLines = lineLength;
		}
	}

	String line;
	int lineStartInFile, lineLength, lineLengthWithoutNewLines;
};

CodeDocument::Iterator::Iterator (CodeDocument* const document_)
	: document (document_),
	  charPointer (nullptr),
	  line (0),
	  position (0)
{
}

CodeDocument::Iterator::Iterator (const CodeDocument::Iterator& other)
	: document (other.document),
	  charPointer (other.charPointer),
	  line (other.line),
	  position (other.position)
{
}

CodeDocument::Iterator& CodeDocument::Iterator::operator= (const CodeDocument::Iterator& other) noexcept
{
	document = other.document;
	charPointer = other.charPointer;
	line = other.line;
	position = other.position;

	return *this;
}

CodeDocument::Iterator::~Iterator() noexcept
{
}

juce_wchar CodeDocument::Iterator::nextChar()
{
	for (;;)
	{
		if (charPointer.getAddress() == nullptr)
		{
			CodeDocumentLine* const l = document->lines[line];

			if (l == nullptr)
				return 0;

			charPointer = l->line.getCharPointer();
		}

		const juce_wchar result = charPointer.getAndAdvance();

		if (result == 0)
		{
			++line;
			charPointer = nullptr;
		}
		else
		{
			++position;
			return result;
		}
	}
}

void CodeDocument::Iterator::skip()
{
	nextChar();
}

void CodeDocument::Iterator::skipToEndOfLine()
{
	if (charPointer.getAddress() == nullptr)
	{
		CodeDocumentLine* const l = document->lines[line];

		if (l == nullptr)
			return;

		charPointer = l->line.getCharPointer();
	}

	position += (int) charPointer.length();
	++line;
	charPointer = nullptr;
}

juce_wchar CodeDocument::Iterator::peekNextChar() const
{
	if (charPointer.getAddress() == nullptr)
	{
		CodeDocumentLine* const l = document->lines[line];

		if (l == nullptr)
			return 0;

		charPointer = l->line.getCharPointer();
	}

	const juce_wchar c = *charPointer;

	if (c != 0)
		return c;

	CodeDocumentLine* const l = document->lines [line + 1];
	return l == nullptr ? 0 : l->line[0];
}

void CodeDocument::Iterator::skipWhitespace()
{
	while (CharacterFunctions::isWhitespace (peekNextChar()))
		skip();
}

bool CodeDocument::Iterator::isEOF() const noexcept
{
	return charPointer.getAddress() == nullptr && line >= document->lines.size();
}

CodeDocument::Position::Position() noexcept
	: owner (0), characterPos (0), line (0),
	  indexInLine (0), positionMaintained (false)
{
}

CodeDocument::Position::Position (const CodeDocument* const ownerDocument,
								  const int line_, const int indexInLine_) noexcept
	: owner (const_cast <CodeDocument*> (ownerDocument)),
	  characterPos (0), line (line_),
	  indexInLine (indexInLine_), positionMaintained (false)
{
	setLineAndIndex (line_, indexInLine_);
}

CodeDocument::Position::Position (const CodeDocument* const ownerDocument,
								  const int characterPos_) noexcept
	: owner (const_cast <CodeDocument*> (ownerDocument)),
	  positionMaintained (false)
{
	setPosition (characterPos_);
}

CodeDocument::Position::Position (const Position& other) noexcept
	: owner (other.owner), characterPos (other.characterPos), line (other.line),
	  indexInLine (other.indexInLine), positionMaintained (false)
{
	jassert (*this == other);
}

CodeDocument::Position::~Position()
{
	setPositionMaintained (false);
}

CodeDocument::Position& CodeDocument::Position::operator= (const Position& other)
{
	if (this != &other)
	{
		const bool wasPositionMaintained = positionMaintained;
		if (owner != other.owner)
			setPositionMaintained (false);

		owner = other.owner;
		line = other.line;
		indexInLine = other.indexInLine;
		characterPos = other.characterPos;
		setPositionMaintained (wasPositionMaintained);

		jassert (*this == other);
	}

	return *this;
}

bool CodeDocument::Position::operator== (const Position& other) const noexcept
{
	jassert ((characterPos == other.characterPos)
			   == (line == other.line && indexInLine == other.indexInLine));

	return characterPos == other.characterPos
			&& line == other.line
			&& indexInLine == other.indexInLine
			&& owner == other.owner;
}

bool CodeDocument::Position::operator!= (const Position& other) const noexcept
{
	return ! operator== (other);
}

void CodeDocument::Position::setLineAndIndex (const int newLineNum, const int newIndexInLine)
{
	jassert (owner != nullptr);

	if (owner->lines.size() == 0)
	{
		line = 0;
		indexInLine = 0;
		characterPos = 0;
	}
	else
	{
		if (newLineNum >= owner->lines.size())
		{
			line = owner->lines.size() - 1;

			CodeDocumentLine* const l = owner->lines.getUnchecked (line);
			jassert (l != nullptr);

			indexInLine = l->lineLengthWithoutNewLines;
			characterPos = l->lineStartInFile + indexInLine;
		}
		else
		{
			line = jmax (0, newLineNum);

			CodeDocumentLine* const l = owner->lines.getUnchecked (line);
			jassert (l != nullptr);

			if (l->lineLengthWithoutNewLines > 0)
				indexInLine = jlimit (0, l->lineLengthWithoutNewLines, newIndexInLine);
			else
				indexInLine = 0;

			characterPos = l->lineStartInFile + indexInLine;
		}
	}
}

void CodeDocument::Position::setPosition (const int newPosition)
{
	jassert (owner != nullptr);

	line = 0;
	indexInLine = 0;
	characterPos = 0;

	if (newPosition > 0)
	{
		int lineStart = 0;
		int lineEnd = owner->lines.size();

		for (;;)
		{
			if (lineEnd - lineStart < 4)
			{
				for (int i = lineStart; i < lineEnd; ++i)
				{
					CodeDocumentLine* const l = owner->lines.getUnchecked (i);

					int index = newPosition - l->lineStartInFile;

					if (index >= 0 && (index < l->lineLength || i == lineEnd - 1))
					{
						line = i;
						indexInLine = jmin (l->lineLengthWithoutNewLines, index);
						characterPos = l->lineStartInFile + indexInLine;
					}
				}

				break;
			}
			else
			{
				const int midIndex = (lineStart + lineEnd + 1) / 2;
				CodeDocumentLine* const mid = owner->lines.getUnchecked (midIndex);

				if (newPosition >= mid->lineStartInFile)
					lineStart = midIndex;
				else
					lineEnd = midIndex;
			}
		}
	}
}

void CodeDocument::Position::moveBy (int characterDelta)
{
	jassert (owner != nullptr);

	if (characterDelta == 1)
	{
		setPosition (getPosition());

		// If moving right, make sure we don't get stuck between the \r and \n characters..
		if (line < owner->lines.size())
		{
			CodeDocumentLine* const l = owner->lines.getUnchecked (line);
			if (indexInLine + characterDelta < l->lineLength
				 && indexInLine + characterDelta >= l->lineLengthWithoutNewLines + 1)
				++characterDelta;
		}
	}

	setPosition (characterPos + characterDelta);
}

const CodeDocument::Position CodeDocument::Position::movedBy (const int characterDelta) const
{
	CodeDocument::Position p (*this);
	p.moveBy (characterDelta);
	return p;
}

const CodeDocument::Position CodeDocument::Position::movedByLines (const int deltaLines) const
{
	CodeDocument::Position p (*this);
	p.setLineAndIndex (getLineNumber() + deltaLines, getIndexInLine());
	return p;
}

const juce_wchar CodeDocument::Position::getCharacter() const
{
	const CodeDocumentLine* const l = owner->lines [line];
	return l == nullptr ? 0 : l->line [getIndexInLine()];
}

String CodeDocument::Position::getLineText() const
{
	const CodeDocumentLine* const l = owner->lines [line];
	return l == nullptr ? String::empty : l->line;
}

void CodeDocument::Position::setPositionMaintained (const bool isMaintained)
{
	if (isMaintained != positionMaintained)
	{
		positionMaintained = isMaintained;

		if (owner != nullptr)
		{
			if (isMaintained)
			{
				jassert (! owner->positionsToMaintain.contains (this));
				owner->positionsToMaintain.add (this);
			}
			else
			{
				// If this happens, you may have deleted the document while there are Position objects that are still using it...
				jassert (owner->positionsToMaintain.contains (this));
				owner->positionsToMaintain.removeValue (this);
			}
		}
	}
}

CodeDocument::CodeDocument()
	: undoManager (std::numeric_limits<int>::max(), 10000),
	  currentActionIndex (0),
	  indexOfSavedState (-1),
	  maximumLineLength (-1),
	  newLineChars ("\r\n")
{
}

CodeDocument::~CodeDocument()
{
}

String CodeDocument::getAllContent() const
{
	return getTextBetween (Position (this, 0),
						   Position (this, lines.size(), 0));
}

String CodeDocument::getTextBetween (const Position& start, const Position& end) const
{
	if (end.getPosition() <= start.getPosition())
		return String::empty;

	const int startLine = start.getLineNumber();
	const int endLine = end.getLineNumber();

	if (startLine == endLine)
	{
		CodeDocumentLine* const line = lines [startLine];
		return (line == nullptr) ? String::empty : line->line.substring (start.getIndexInLine(), end.getIndexInLine());
	}

	MemoryOutputStream mo;
	mo.preallocate ((size_t) (end.getPosition() - start.getPosition() + 4));

	const int maxLine = jmin (lines.size() - 1, endLine);

	for (int i = jmax (0, startLine); i <= maxLine; ++i)
	{
		const CodeDocumentLine* line = lines.getUnchecked(i);
		int len = line->lineLength;

		if (i == startLine)
		{
			const int index = start.getIndexInLine();
			mo << line->line.substring (index, len);
		}
		else if (i == endLine)
		{
			len = end.getIndexInLine();
			mo << line->line.substring (0, len);
		}
		else
		{
			mo << line->line;
		}
	}

	return mo.toString();
}

int CodeDocument::getNumCharacters() const noexcept
{
	const CodeDocumentLine* const lastLine = lines.getLast();
	return (lastLine == nullptr) ? 0 : lastLine->lineStartInFile + lastLine->lineLength;
}

String CodeDocument::getLine (const int lineIndex) const noexcept
{
	const CodeDocumentLine* const line = lines [lineIndex];
	return (line == nullptr) ? String::empty : line->line;
}

int CodeDocument::getMaximumLineLength() noexcept
{
	if (maximumLineLength < 0)
	{
		maximumLineLength = 0;

		for (int i = lines.size(); --i >= 0;)
			maximumLineLength = jmax (maximumLineLength, lines.getUnchecked(i)->lineLength);
	}

	return maximumLineLength;
}

void CodeDocument::deleteSection (const Position& startPosition, const Position& endPosition)
{
	remove (startPosition.getPosition(), endPosition.getPosition(), true);
}

void CodeDocument::insertText (const Position& position, const String& text)
{
	insert (text, position.getPosition(), true);
}

void CodeDocument::replaceAllContent (const String& newContent)
{
	remove (0, getNumCharacters(), true);
	insert (newContent, 0, true);
}

bool CodeDocument::loadFromStream (InputStream& stream)
{
	remove (0, getNumCharacters(), false);
	insert (stream.readEntireStreamAsString(), 0, false);
	setSavePoint();
	clearUndoHistory();
	return true;
}

bool CodeDocument::writeToStream (OutputStream& stream)
{
	for (int i = 0; i < lines.size(); ++i)
	{
		String temp (lines.getUnchecked(i)->line); // use a copy to avoid bloating the memory footprint of the stored string.
		const char* utf8 = temp.toUTF8();

		if (! stream.write (utf8, (int) strlen (utf8)))
			return false;
	}

	return true;
}

void CodeDocument::setNewLineCharacters (const String& newLineChars_) noexcept
{
	jassert (newLineChars_ == "\r\n" || newLineChars_ == "\n" || newLineChars_ == "\r");
	newLineChars = newLineChars_;
}

void CodeDocument::newTransaction()
{
	undoManager.beginNewTransaction (String::empty);
}

void CodeDocument::undo()
{
	newTransaction();
	undoManager.undo();
}

void CodeDocument::redo()
{
	undoManager.redo();
}

void CodeDocument::clearUndoHistory()
{
	undoManager.clearUndoHistory();
}

void CodeDocument::setSavePoint() noexcept
{
	indexOfSavedState = currentActionIndex;
}

bool CodeDocument::hasChangedSinceSavePoint() const noexcept
{
	return currentActionIndex != indexOfSavedState;
}

namespace CodeDocumentHelpers
{
	static int getCharacterType (const juce_wchar character) noexcept
	{
		return (CharacterFunctions::isLetterOrDigit (character) || character == '_')
					? 2 : (CharacterFunctions::isWhitespace (character) ? 0 : 1);
	}
}

const CodeDocument::Position CodeDocument::findWordBreakAfter (const Position& position) const noexcept
{
	Position p (position);
	const int maxDistance = 256;
	int i = 0;

	while (i < maxDistance
			&& CharacterFunctions::isWhitespace (p.getCharacter())
			&& (i == 0 || (p.getCharacter() != '\n'
							&& p.getCharacter() != '\r')))
	{
		++i;
		p.moveBy (1);
	}

	if (i == 0)
	{
		const int type = CodeDocumentHelpers::getCharacterType (p.getCharacter());

		while (i < maxDistance && type == CodeDocumentHelpers::getCharacterType (p.getCharacter()))
		{
			++i;
			p.moveBy (1);
		}

		while (i < maxDistance
				&& CharacterFunctions::isWhitespace (p.getCharacter())
				&& (i == 0 || (p.getCharacter() != '\n'
								&& p.getCharacter() != '\r')))
		{
			++i;
			p.moveBy (1);
		}
	}

	return p;
}

const CodeDocument::Position CodeDocument::findWordBreakBefore (const Position& position) const noexcept
{
	Position p (position);
	const int maxDistance = 256;
	int i = 0;
	bool stoppedAtLineStart = false;

	while (i < maxDistance)
	{
		const juce_wchar c = p.movedBy (-1).getCharacter();

		if (c == '\r' || c == '\n')
		{
			stoppedAtLineStart = true;

			if (i > 0)
				break;
		}

		if (! CharacterFunctions::isWhitespace (c))
			break;

		p.moveBy (-1);
		++i;
	}

	if (i < maxDistance && ! stoppedAtLineStart)
	{
		const int type = CodeDocumentHelpers::getCharacterType (p.movedBy (-1).getCharacter());

		while (i < maxDistance && type == CodeDocumentHelpers::getCharacterType (p.movedBy (-1).getCharacter()))
		{
			p.moveBy (-1);
			++i;
		}
	}

	return p;
}

void CodeDocument::checkLastLineStatus()
{
	while (lines.size() > 0
			&& lines.getLast()->lineLength == 0
			&& (lines.size() == 1 || ! lines.getUnchecked (lines.size() - 2)->endsWithLineBreak()))
	{
		// remove any empty lines at the end if the preceding line doesn't end in a newline.
		lines.removeLast();
	}

	const CodeDocumentLine* const lastLine = lines.getLast();

	if (lastLine != nullptr && lastLine->endsWithLineBreak())
	{
		// check that there's an empty line at the end if the preceding one ends in a newline..
		lines.add (new CodeDocumentLine (String::empty.getCharPointer(), 0, 0, lastLine->lineStartInFile + lastLine->lineLength));
	}
}

void CodeDocument::addListener (CodeDocument::Listener* const listener) noexcept
{
	listeners.add (listener);
}

void CodeDocument::removeListener (CodeDocument::Listener* const listener) noexcept
{
	listeners.remove (listener);
}

void CodeDocument::sendListenerChangeMessage (const int startLine, const int endLine)
{
	Position startPos (this, startLine, 0);
	Position endPos (this, endLine, 0);

	listeners.call (&CodeDocument::Listener::codeDocumentChanged, startPos, endPos);
}

class CodeDocumentInsertAction   : public UndoableAction
{
public:
	CodeDocumentInsertAction (CodeDocument& owner_, const String& text_, const int insertPos_) noexcept
		: owner (owner_),
		  text (text_),
		  insertPos (insertPos_)
	{
	}

	bool perform()
	{
		owner.currentActionIndex++;
		owner.insert (text, insertPos, false);
		return true;
	}

	bool undo()
	{
		owner.currentActionIndex--;
		owner.remove (insertPos, insertPos + text.length(), false);
		return true;
	}

	int getSizeInUnits()        { return text.length() + 32; }

private:
	CodeDocument& owner;
	const String text;
	int insertPos;

	JUCE_DECLARE_NON_COPYABLE (CodeDocumentInsertAction);
};

void CodeDocument::insert (const String& text, const int insertPos, const bool undoable)
{
	if (text.isEmpty())
		return;

	if (undoable)
	{
		undoManager.perform (new CodeDocumentInsertAction (*this, text, insertPos));
	}
	else
	{
		Position pos (this, insertPos);
		const int firstAffectedLine = pos.getLineNumber();
		int lastAffectedLine = firstAffectedLine + 1;

		CodeDocumentLine* const firstLine = lines [firstAffectedLine];
		String textInsideOriginalLine (text);

		if (firstLine != nullptr)
		{
			const int index = pos.getIndexInLine();
			textInsideOriginalLine = firstLine->line.substring (0, index)
									 + textInsideOriginalLine
									 + firstLine->line.substring (index);
		}

		maximumLineLength = -1;
		Array <CodeDocumentLine*> newLines;
		CodeDocumentLine::createLines (newLines, textInsideOriginalLine);
		jassert (newLines.size() > 0);

		CodeDocumentLine* const newFirstLine = newLines.getUnchecked (0);
		newFirstLine->lineStartInFile = firstLine != nullptr ? firstLine->lineStartInFile : 0;
		lines.set (firstAffectedLine, newFirstLine);

		if (newLines.size() > 1)
		{
			for (int i = 1; i < newLines.size(); ++i)
			{
				CodeDocumentLine* const l = newLines.getUnchecked (i);
				lines.insert (firstAffectedLine + i, l);
			}

			lastAffectedLine = lines.size();
		}

		int i, lineStart = newFirstLine->lineStartInFile;
		for (i = firstAffectedLine; i < lines.size(); ++i)
		{
			CodeDocumentLine* const l = lines.getUnchecked (i);
			l->lineStartInFile = lineStart;
			lineStart += l->lineLength;
		}

		checkLastLineStatus();

		const int newTextLength = text.length();
		for (i = 0; i < positionsToMaintain.size(); ++i)
		{
			CodeDocument::Position* const p = positionsToMaintain.getUnchecked(i);

			if (p->getPosition() >= insertPos)
				p->setPosition (p->getPosition() + newTextLength);
		}

		sendListenerChangeMessage (firstAffectedLine, lastAffectedLine);
	}
}

class CodeDocumentDeleteAction  : public UndoableAction
{
public:
	CodeDocumentDeleteAction (CodeDocument& owner_, const int startPos_, const int endPos_) noexcept
		: owner (owner_),
		  startPos (startPos_),
		  endPos (endPos_)
	{
		removedText = owner.getTextBetween (CodeDocument::Position (&owner, startPos),
											CodeDocument::Position (&owner, endPos));
	}

	bool perform()
	{
		owner.currentActionIndex++;
		owner.remove (startPos, endPos, false);
		return true;
	}

	bool undo()
	{
		owner.currentActionIndex--;
		owner.insert (removedText, startPos, false);
		return true;
	}

	int getSizeInUnits()    { return removedText.length() + 32; }

private:
	CodeDocument& owner;
	int startPos, endPos;
	String removedText;

	JUCE_DECLARE_NON_COPYABLE (CodeDocumentDeleteAction);
};

void CodeDocument::remove (const int startPos, const int endPos, const bool undoable)
{
	if (endPos <= startPos)
		return;

	if (undoable)
	{
		undoManager.perform (new CodeDocumentDeleteAction (*this, startPos, endPos));
	}
	else
	{
		Position startPosition (this, startPos);
		Position endPosition (this, endPos);

		maximumLineLength = -1;
		const int firstAffectedLine = startPosition.getLineNumber();
		const int endLine = endPosition.getLineNumber();
		int lastAffectedLine = firstAffectedLine + 1;
		CodeDocumentLine* const firstLine = lines.getUnchecked (firstAffectedLine);

		if (firstAffectedLine == endLine)
		{
			firstLine->line = firstLine->line.substring (0, startPosition.getIndexInLine())
							+ firstLine->line.substring (endPosition.getIndexInLine());
			firstLine->updateLength();
		}
		else
		{
			lastAffectedLine = lines.size();

			CodeDocumentLine* const lastLine = lines.getUnchecked (endLine);
			jassert (lastLine != nullptr);

			firstLine->line = firstLine->line.substring (0, startPosition.getIndexInLine())
								+ lastLine->line.substring (endPosition.getIndexInLine());
			firstLine->updateLength();

			int numLinesToRemove = endLine - firstAffectedLine;
			lines.removeRange (firstAffectedLine + 1, numLinesToRemove);
		}

		int i;
		for (i = firstAffectedLine + 1; i < lines.size(); ++i)
		{
			CodeDocumentLine* const l = lines.getUnchecked (i);
			const CodeDocumentLine* const previousLine = lines.getUnchecked (i - 1);
			l->lineStartInFile = previousLine->lineStartInFile + previousLine->lineLength;
		}

		checkLastLineStatus();

		const int totalChars = getNumCharacters();

		for (i = 0; i < positionsToMaintain.size(); ++i)
		{
			CodeDocument::Position* p = positionsToMaintain.getUnchecked(i);

			if (p->getPosition() > startPosition.getPosition())
				p->setPosition (jmax (startPos, p->getPosition() + startPos - endPos));

			if (p->getPosition() > totalChars)
				p->setPosition (totalChars);
		}

		sendListenerChangeMessage (firstAffectedLine, lastAffectedLine);
	}
}

/*** End of inlined file: juce_CodeDocument.cpp ***/


/*** Start of inlined file: juce_CodeEditorComponent.cpp ***/
class CodeEditorComponent::CodeEditorLine
{
public:
	CodeEditorLine() noexcept
	   : highlightColumnStart (0), highlightColumnEnd (0)
	{
	}

	bool update (CodeDocument& document, int lineNum,
				 CodeDocument::Iterator& source,
				 CodeTokeniser* analyser, const int spacesPerTab,
				 const CodeDocument::Position& selectionStart,
				 const CodeDocument::Position& selectionEnd)
	{
		Array <SyntaxToken> newTokens;
		newTokens.ensureStorageAllocated (8);

		if (analyser == nullptr)
		{
			newTokens.add (SyntaxToken (document.getLine (lineNum), -1));
		}
		else if (lineNum < document.getNumLines())
		{
			const CodeDocument::Position pos (&document, lineNum, 0);
			createTokens (pos.getPosition(), pos.getLineText(),
						  source, analyser, newTokens);
		}

		replaceTabsWithSpaces (newTokens, spacesPerTab);

		int newHighlightStart = 0;
		int newHighlightEnd = 0;

		if (selectionStart.getLineNumber() <= lineNum && selectionEnd.getLineNumber() >= lineNum)
		{
			const String line (document.getLine (lineNum));

			CodeDocument::Position lineStart (&document, lineNum, 0), lineEnd (&document, lineNum + 1, 0);
			newHighlightStart = indexToColumn (jmax (0, selectionStart.getPosition() - lineStart.getPosition()),
											   line, spacesPerTab);
			newHighlightEnd = indexToColumn (jmin (lineEnd.getPosition() - lineStart.getPosition(), selectionEnd.getPosition() - lineStart.getPosition()),
											 line, spacesPerTab);
		}

		if (newHighlightStart != highlightColumnStart || newHighlightEnd != highlightColumnEnd)
		{
			highlightColumnStart = newHighlightStart;
			highlightColumnEnd = newHighlightEnd;
		}
		else
		{
			if (tokens.size() == newTokens.size())
			{
				bool allTheSame = true;

				for (int i = newTokens.size(); --i >= 0;)
				{
					if (tokens.getReference(i) != newTokens.getReference(i))
					{
						allTheSame = false;
						break;
					}
				}

				if (allTheSame)
					return false;
			}
		}

		tokens.swapWithArray (newTokens);
		return true;
	}

	void draw (CodeEditorComponent& owner, Graphics& g, const Font& font,
			   float x, const int y, const int baselineOffset, const int lineHeight,
			   const Colour& highlightColour) const
	{
		if (highlightColumnStart < highlightColumnEnd)
		{
			g.setColour (highlightColour);
			g.fillRect (roundToInt (x + highlightColumnStart * owner.getCharWidth()), y,
						roundToInt ((highlightColumnEnd - highlightColumnStart) * owner.getCharWidth()), lineHeight);
		}

		int lastType = std::numeric_limits<int>::min();

		for (int i = 0; i < tokens.size(); ++i)
		{
			SyntaxToken& token = tokens.getReference(i);

			if (lastType != token.tokenType)
			{
				lastType = token.tokenType;
				g.setColour (owner.getColourForTokenType (lastType));
			}

			g.drawSingleLineText (token.text, roundToInt (x), y + baselineOffset);

			if (i < tokens.size() - 1)
			{
				if (token.width < 0)
					token.width = font.getStringWidthFloat (token.text);

				x += token.width;
			}
		}
	}

private:
	struct SyntaxToken
	{
		SyntaxToken (const String& text_, const int type) noexcept
			: text (text_), tokenType (type), width (-1.0f)
		{
		}

		bool operator!= (const SyntaxToken& other) const noexcept
		{
			return text != other.text || tokenType != other.tokenType;
		}

		String text;
		int tokenType;
		float width;
	};

	Array <SyntaxToken> tokens;
	int highlightColumnStart, highlightColumnEnd;

	static void createTokens (int startPosition, const String& lineText,
							  CodeDocument::Iterator& source,
							  CodeTokeniser* analyser,
							  Array <SyntaxToken>& newTokens)
	{
		CodeDocument::Iterator lastIterator (source);
		const int lineLength = lineText.length();

		for (;;)
		{
			int tokenType = analyser->readNextToken (source);
			int tokenStart = lastIterator.getPosition();
			int tokenEnd = source.getPosition();

			if (tokenEnd <= tokenStart)
				break;

			tokenEnd -= startPosition;

			if (tokenEnd > 0)
			{
				tokenStart -= startPosition;
				newTokens.add (SyntaxToken (lineText.substring (jmax (0, tokenStart), tokenEnd),
											tokenType));

				if (tokenEnd >= lineLength)
					break;
			}

			lastIterator = source;
		}

		source = lastIterator;
	}

	static void replaceTabsWithSpaces (Array <SyntaxToken>& tokens, const int spacesPerTab)
	{
		int x = 0;
		for (int i = 0; i < tokens.size(); ++i)
		{
			SyntaxToken& t = tokens.getReference(i);

			for (;;)
			{
				int tabPos = t.text.indexOfChar ('\t');
				if (tabPos < 0)
					break;

				const int spacesNeeded = spacesPerTab - ((tabPos + x) % spacesPerTab);
				t.text = t.text.replaceSection (tabPos, 1, String::repeatedString (" ", spacesNeeded));
			}

			x += t.text.length();
		}
	}

	int indexToColumn (int index, const String& line, int spacesPerTab) const noexcept
	{
		jassert (index <= line.length());

		String::CharPointerType t (line.getCharPointer());
		int col = 0;
		for (int i = 0; i < index; ++i)
		{
			if (t.getAndAdvance() != '\t')
				++col;
			else
				col += spacesPerTab - (col % spacesPerTab);
		}

		return col;
	}
};

CodeEditorComponent::CodeEditorComponent (CodeDocument& document_,
										  CodeTokeniser* const codeTokeniser_)
	: document (document_),
	  firstLineOnScreen (0),
	  gutter (5),
	  spacesPerTab (4),
	  lineHeight (0),
	  linesOnScreen (0),
	  columnsOnScreen (0),
	  scrollbarThickness (16),
	  columnToTryToMaintain (-1),
	  useSpacesForTabs (false),
	  xOffset (0),
	  verticalScrollBar (true),
	  horizontalScrollBar (false),
	  codeTokeniser (codeTokeniser_)
{
	caretPos = CodeDocument::Position (&document_, 0, 0);
	caretPos.setPositionMaintained (true);

	selectionStart = CodeDocument::Position (&document_, 0, 0);
	selectionStart.setPositionMaintained (true);

	selectionEnd = CodeDocument::Position (&document_, 0, 0);
	selectionEnd.setPositionMaintained (true);

	setOpaque (true);
	setMouseCursor (MouseCursor (MouseCursor::IBeamCursor));
	setWantsKeyboardFocus (true);

	addAndMakeVisible (&verticalScrollBar);
	verticalScrollBar.setSingleStepSize (1.0);

	addAndMakeVisible (&horizontalScrollBar);
	horizontalScrollBar.setSingleStepSize (1.0);

	addAndMakeVisible (caret = getLookAndFeel().createCaretComponent (this));

	Font f (12.0f);
	f.setTypefaceName (Font::getDefaultMonospacedFontName());
	setFont (f);

	resetToDefaultColours();

	verticalScrollBar.addListener (this);
	horizontalScrollBar.addListener (this);
	document.addListener (this);
}

CodeEditorComponent::~CodeEditorComponent()
{
	document.removeListener (this);
}

void CodeEditorComponent::loadContent (const String& newContent)
{
	clearCachedIterators (0);
	document.replaceAllContent (newContent);
	document.clearUndoHistory();
	document.setSavePoint();
	caretPos.setPosition (0);
	selectionStart.setPosition (0);
	selectionEnd.setPosition (0);
	scrollToLine (0);
}

bool CodeEditorComponent::isTextInputActive() const
{
	return true;
}

void CodeEditorComponent::setTemporaryUnderlining (const Array <Range<int> >&)
{
	jassertfalse; // TODO Windows IME not yet supported for this comp..
}

Rectangle<int> CodeEditorComponent::getCaretRectangle()
{
	return getLocalArea (caret, caret->getLocalBounds());
}

void CodeEditorComponent::codeDocumentChanged (const CodeDocument::Position& affectedTextStart,
											   const CodeDocument::Position& affectedTextEnd)
{
	clearCachedIterators (affectedTextStart.getLineNumber());

	triggerAsyncUpdate();

	updateCaretPosition();
	columnToTryToMaintain = -1;

	if (affectedTextEnd.getPosition() >= selectionStart.getPosition()
		 && affectedTextStart.getPosition() <= selectionEnd.getPosition())
		deselectAll();

	if (caretPos.getPosition() > affectedTextEnd.getPosition()
		 || caretPos.getPosition() < affectedTextStart.getPosition())
		moveCaretTo (affectedTextStart, false);

	updateScrollBars();
}

void CodeEditorComponent::resized()
{
	linesOnScreen = (getHeight() - scrollbarThickness) / lineHeight;
	columnsOnScreen = (int) ((getWidth() - scrollbarThickness) / charWidth);
	lines.clear();
	rebuildLineTokens();
	updateCaretPosition();

	verticalScrollBar.setBounds (getWidth() - scrollbarThickness, 0, scrollbarThickness, getHeight() - scrollbarThickness);
	horizontalScrollBar.setBounds (gutter, getHeight() - scrollbarThickness, getWidth() - scrollbarThickness - gutter, scrollbarThickness);
	updateScrollBars();
}

void CodeEditorComponent::paint (Graphics& g)
{
	handleUpdateNowIfNeeded();

	g.fillAll (findColour (CodeEditorComponent::backgroundColourId));

	g.reduceClipRegion (gutter, 0, verticalScrollBar.getX() - gutter, horizontalScrollBar.getY());

	g.setFont (font);
	const int baselineOffset = (int) font.getAscent();
	const Colour defaultColour (findColour (CodeEditorComponent::defaultTextColourId));
	const Colour highlightColour (findColour (CodeEditorComponent::highlightColourId));

	const Rectangle<int> clip (g.getClipBounds());
	const int firstLineToDraw = jmax (0, clip.getY() / lineHeight);
	const int lastLineToDraw = jmin (lines.size(), clip.getBottom() / lineHeight + 1);

	for (int j = firstLineToDraw; j < lastLineToDraw; ++j)
	{
		lines.getUnchecked(j)->draw (*this, g, font,
									 (float) (gutter - xOffset * charWidth),
									 lineHeight * j, baselineOffset, lineHeight,
									 highlightColour);
	}
}

void CodeEditorComponent::setScrollbarThickness (const int thickness)
{
	if (scrollbarThickness != thickness)
	{
		scrollbarThickness = thickness;
		resized();
	}
}

void CodeEditorComponent::handleAsyncUpdate()
{
	rebuildLineTokens();
}

void CodeEditorComponent::rebuildLineTokens()
{
	cancelPendingUpdate();

	const int numNeeded = linesOnScreen + 1;

	int minLineToRepaint = numNeeded;
	int maxLineToRepaint = 0;

	if (numNeeded != lines.size())
	{
		lines.clear();

		for (int i = numNeeded; --i >= 0;)
			lines.add (new CodeEditorLine());

		minLineToRepaint = 0;
		maxLineToRepaint = numNeeded;
	}

	jassert (numNeeded == lines.size());

	CodeDocument::Iterator source (&document);
	getIteratorForPosition (CodeDocument::Position (&document, firstLineOnScreen, 0).getPosition(), source);

	for (int i = 0; i < numNeeded; ++i)
	{
		CodeEditorLine* const line = lines.getUnchecked(i);

		if (line->update (document, firstLineOnScreen + i, source, codeTokeniser, spacesPerTab,
						  selectionStart, selectionEnd))
		{
			minLineToRepaint = jmin (minLineToRepaint, i);
			maxLineToRepaint = jmax (maxLineToRepaint, i);
		}
	}

	if (minLineToRepaint <= maxLineToRepaint)
	{
		repaint (gutter, lineHeight * minLineToRepaint - 1,
				 verticalScrollBar.getX() - gutter,
				 lineHeight * (1 + maxLineToRepaint - minLineToRepaint) + 2);
	}
}

void CodeEditorComponent::updateCaretPosition()
{
	caret->setCaretPosition (getCharacterBounds (getCaretPos()));
}

void CodeEditorComponent::moveCaretTo (const CodeDocument::Position& newPos, const bool highlighting)
{
	caretPos = newPos;
	columnToTryToMaintain = -1;

	if (highlighting)
	{
		if (dragType == notDragging)
		{
			if (abs (caretPos.getPosition() - selectionStart.getPosition())
				  < abs (caretPos.getPosition() - selectionEnd.getPosition()))
				dragType = draggingSelectionStart;
			else
				dragType = draggingSelectionEnd;
		}

		if (dragType == draggingSelectionStart)
		{
			selectionStart = caretPos;

			if (selectionEnd.getPosition() < selectionStart.getPosition())
			{
				const CodeDocument::Position temp (selectionStart);
				selectionStart = selectionEnd;
				selectionEnd = temp;

				dragType = draggingSelectionEnd;
			}
		}
		else
		{
			selectionEnd = caretPos;

			if (selectionEnd.getPosition() < selectionStart.getPosition())
			{
				const CodeDocument::Position temp (selectionStart);
				selectionStart = selectionEnd;
				selectionEnd = temp;

				dragType = draggingSelectionStart;
			}
		}

		triggerAsyncUpdate();
	}
	else
	{
		deselectAll();
	}

	updateCaretPosition();
	scrollToKeepCaretOnScreen();
	updateScrollBars();
}

void CodeEditorComponent::deselectAll()
{
	if (selectionStart != selectionEnd)
		triggerAsyncUpdate();

	selectionStart = caretPos;
	selectionEnd = caretPos;
}

void CodeEditorComponent::updateScrollBars()
{
	verticalScrollBar.setRangeLimits (0, jmax (document.getNumLines(), firstLineOnScreen + linesOnScreen));
	verticalScrollBar.setCurrentRange (firstLineOnScreen, linesOnScreen);

	horizontalScrollBar.setRangeLimits (0, jmax ((double) document.getMaximumLineLength(), xOffset + columnsOnScreen));
	horizontalScrollBar.setCurrentRange (xOffset, columnsOnScreen);
}

void CodeEditorComponent::scrollToLineInternal (int newFirstLineOnScreen)
{
	newFirstLineOnScreen = jlimit (0, jmax (0, document.getNumLines() - 1),
								   newFirstLineOnScreen);

	if (newFirstLineOnScreen != firstLineOnScreen)
	{
		firstLineOnScreen = newFirstLineOnScreen;
		updateCaretPosition();

		updateCachedIterators (firstLineOnScreen);
		triggerAsyncUpdate();
	}
}

void CodeEditorComponent::scrollToColumnInternal (double column)
{
	const double newOffset = jlimit (0.0, document.getMaximumLineLength() + 3.0, column);

	if (xOffset != newOffset)
	{
		xOffset = newOffset;
		updateCaretPosition();
		repaint();
	}
}

void CodeEditorComponent::scrollToLine (int newFirstLineOnScreen)
{
	scrollToLineInternal (newFirstLineOnScreen);
	updateScrollBars();
}

void CodeEditorComponent::scrollToColumn (int newFirstColumnOnScreen)
{
	scrollToColumnInternal (newFirstColumnOnScreen);
	updateScrollBars();
}

void CodeEditorComponent::scrollBy (int deltaLines)
{
	scrollToLine (firstLineOnScreen + deltaLines);
}

void CodeEditorComponent::scrollToKeepCaretOnScreen()
{
	if (caretPos.getLineNumber() < firstLineOnScreen)
		scrollBy (caretPos.getLineNumber() - firstLineOnScreen);
	else if (caretPos.getLineNumber() >= firstLineOnScreen + linesOnScreen)
		scrollBy (caretPos.getLineNumber() - (firstLineOnScreen + linesOnScreen - 1));

	const int column = indexToColumn (caretPos.getLineNumber(), caretPos.getIndexInLine());
	if (column >= xOffset + columnsOnScreen - 1)
		scrollToColumn (column + 1 - columnsOnScreen);
	else if (column < xOffset)
		scrollToColumn (column);
}

Rectangle<int> CodeEditorComponent::getCharacterBounds (const CodeDocument::Position& pos) const
{
	return Rectangle<int> (roundToInt ((gutter - xOffset * charWidth) + indexToColumn (pos.getLineNumber(), pos.getIndexInLine()) * charWidth),
						   (pos.getLineNumber() - firstLineOnScreen) * lineHeight,
						   roundToInt (charWidth),
						   lineHeight);
}

CodeDocument::Position CodeEditorComponent::getPositionAt (int x, int y)
{
	const int line = y / lineHeight + firstLineOnScreen;
	const int column = roundToInt ((x - (gutter - xOffset * charWidth)) / charWidth);
	const int index = columnToIndex (line, column);

	return CodeDocument::Position (&document, line, index);
}

void CodeEditorComponent::insertTextAtCaret (const String& newText)
{
	document.deleteSection (selectionStart, selectionEnd);

	if (newText.isNotEmpty())
		document.insertText (caretPos, newText);

	scrollToKeepCaretOnScreen();
}

void CodeEditorComponent::insertTabAtCaret()
{
	if (CharacterFunctions::isWhitespace (caretPos.getCharacter())
		 && caretPos.getLineNumber() == caretPos.movedBy (1).getLineNumber())
	{
		moveCaretTo (document.findWordBreakAfter (caretPos), false);
	}

	if (useSpacesForTabs)
	{
		const int caretCol = indexToColumn (caretPos.getLineNumber(), caretPos.getIndexInLine());
		const int spacesNeeded = spacesPerTab - (caretCol % spacesPerTab);
		insertTextAtCaret (String::repeatedString (" ", spacesNeeded));
	}
	else
	{
		insertTextAtCaret ("\t");
	}
}

void CodeEditorComponent::cut()
{
	insertTextAtCaret (String::empty);
}

bool CodeEditorComponent::copyToClipboard()
{
	newTransaction();

	const String selection (document.getTextBetween (selectionStart, selectionEnd));

	if (selection.isNotEmpty())
		SystemClipboard::copyTextToClipboard (selection);

	return true;
}

bool CodeEditorComponent::cutToClipboard()
{
	copyToClipboard();
	cut();
	newTransaction();
	return true;
}

bool CodeEditorComponent::pasteFromClipboard()
{
	newTransaction();
	const String clip (SystemClipboard::getTextFromClipboard());

	if (clip.isNotEmpty())
		insertTextAtCaret (clip);

	newTransaction();
	return true;
}

bool CodeEditorComponent::moveCaretLeft (const bool moveInWholeWordSteps, const bool selecting)
{
	newTransaction();

	if (moveInWholeWordSteps)
		moveCaretTo (document.findWordBreakBefore (caretPos), selecting);
	else
		moveCaretTo (caretPos.movedBy (-1), selecting);

	return true;
}

bool CodeEditorComponent::moveCaretRight (const bool moveInWholeWordSteps, const bool selecting)
{
	newTransaction();

	if (moveInWholeWordSteps)
		moveCaretTo (document.findWordBreakAfter (caretPos), selecting);
	else
		moveCaretTo (caretPos.movedBy (1), selecting);

	return true;
}

void CodeEditorComponent::moveLineDelta (const int delta, const bool selecting)
{
	CodeDocument::Position pos (caretPos);
	const int newLineNum = pos.getLineNumber() + delta;

	if (columnToTryToMaintain < 0)
		columnToTryToMaintain = indexToColumn (pos.getLineNumber(), pos.getIndexInLine());

	pos.setLineAndIndex (newLineNum, columnToIndex (newLineNum, columnToTryToMaintain));

	const int colToMaintain = columnToTryToMaintain;
	moveCaretTo (pos, selecting);
	columnToTryToMaintain = colToMaintain;
}

bool CodeEditorComponent::moveCaretDown (const bool selecting)
{
	newTransaction();

	if (caretPos.getLineNumber() == document.getNumLines() - 1)
		moveCaretTo (CodeDocument::Position (&document, std::numeric_limits<int>::max(), std::numeric_limits<int>::max()), selecting);
	else
		moveLineDelta (1, selecting);

	return true;
}

bool CodeEditorComponent::moveCaretUp (const bool selecting)
{
	newTransaction();

	if (caretPos.getLineNumber() == 0)
		moveCaretTo (CodeDocument::Position (&document, 0, 0), selecting);
	else
		moveLineDelta (-1, selecting);

	return true;
}

bool CodeEditorComponent::pageDown (const bool selecting)
{
	newTransaction();
	scrollBy (jlimit (0, linesOnScreen, 1 + document.getNumLines() - firstLineOnScreen - linesOnScreen));
	moveLineDelta (linesOnScreen, selecting);
	return true;
}

bool CodeEditorComponent::pageUp (const bool selecting)
{
	newTransaction();
	scrollBy (-linesOnScreen);
	moveLineDelta (-linesOnScreen, selecting);
	return true;
}

bool CodeEditorComponent::scrollUp()
{
	newTransaction();
	scrollBy (1);

	if (caretPos.getLineNumber() < firstLineOnScreen)
		moveLineDelta (1, false);

	return true;
}

bool CodeEditorComponent::scrollDown()
{
	newTransaction();
	scrollBy (-1);

	if (caretPos.getLineNumber() >= firstLineOnScreen + linesOnScreen)
		moveLineDelta (-1, false);

	return true;
}

bool CodeEditorComponent::moveCaretToTop (const bool selecting)
{
	newTransaction();
	moveCaretTo (CodeDocument::Position (&document, 0, 0), selecting);
	return true;
}

namespace CodeEditorHelpers
{
	static int findFirstNonWhitespaceChar (const String& line) noexcept
	{
		String::CharPointerType t (line.getCharPointer());
		int i = 0;

		while (! t.isEmpty())
		{
			if (! t.isWhitespace())
				return i;

			++t;
			++i;
		}

		return 0;
	}
}

bool CodeEditorComponent::moveCaretToStartOfLine (const bool selecting)
{
	newTransaction();

	int index = CodeEditorHelpers::findFirstNonWhitespaceChar (caretPos.getLineText());

	if (index >= caretPos.getIndexInLine() && caretPos.getIndexInLine() > 0)
		index = 0;

	moveCaretTo (CodeDocument::Position (&document, caretPos.getLineNumber(), index), selecting);
	return true;
}

bool CodeEditorComponent::moveCaretToEnd (const bool selecting)
{
	newTransaction();
	moveCaretTo (CodeDocument::Position (&document, std::numeric_limits<int>::max(), std::numeric_limits<int>::max()), selecting);
	return true;
}

bool CodeEditorComponent::moveCaretToEndOfLine (const bool selecting)
{
	newTransaction();
	moveCaretTo (CodeDocument::Position (&document, caretPos.getLineNumber(), std::numeric_limits<int>::max()), selecting);
	return true;
}

bool CodeEditorComponent::deleteBackwards (const bool moveInWholeWordSteps)
{
	if (moveInWholeWordSteps)
	{
		cut(); // in case something is already highlighted
		moveCaretTo (document.findWordBreakBefore (caretPos), true);
	}
	else
	{
		if (selectionStart == selectionEnd)
			selectionStart.moveBy (-1);
	}

	cut();
	return true;
}

bool CodeEditorComponent::deleteForwards (const bool moveInWholeWordSteps)
{
	if (moveInWholeWordSteps)
	{
		cut(); // in case something is already highlighted
		moveCaretTo (document.findWordBreakAfter (caretPos), true);
	}
	else
	{
		if (selectionStart == selectionEnd)
			selectionEnd.moveBy (1);
		else
			newTransaction();
	}

	cut();
	return true;
}

bool CodeEditorComponent::selectAll()
{
	newTransaction();
	moveCaretTo (CodeDocument::Position (&document, std::numeric_limits<int>::max(), std::numeric_limits<int>::max()), false);
	moveCaretTo (CodeDocument::Position (&document, 0, 0), true);
	return true;
}

bool CodeEditorComponent::undo()
{
	document.undo();
	scrollToKeepCaretOnScreen();
	return true;
}

bool CodeEditorComponent::redo()
{
	document.redo();
	scrollToKeepCaretOnScreen();
	return true;
}

void CodeEditorComponent::newTransaction()
{
	document.newTransaction();
	startTimer (600);
}

void CodeEditorComponent::timerCallback()
{
	newTransaction();
}

Range<int> CodeEditorComponent::getHighlightedRegion() const
{
	return Range<int> (selectionStart.getPosition(), selectionEnd.getPosition());
}

void CodeEditorComponent::setHighlightedRegion (const Range<int>& newRange)
{
	moveCaretTo (CodeDocument::Position (&document, newRange.getStart()), false);
	moveCaretTo (CodeDocument::Position (&document, newRange.getEnd()), true);
}

String CodeEditorComponent::getTextInRange (const Range<int>& range) const
{
	return document.getTextBetween (CodeDocument::Position (&document, range.getStart()),
									CodeDocument::Position (&document, range.getEnd()));
}

bool CodeEditorComponent::keyPressed (const KeyPress& key)
{
	if (! TextEditorKeyMapper<CodeEditorComponent>::invokeKeyFunction (*this, key))
	{
		if (key == KeyPress::tabKey || key.getTextCharacter() == '\t')
		{
			insertTabAtCaret();
		}
		else if (key == KeyPress::returnKey)
		{
			newTransaction();
			insertTextAtCaret (document.getNewLineCharacters());
		}
		else if (key.isKeyCode (KeyPress::escapeKey))
		{
			newTransaction();
		}
		else if (key.getTextCharacter() >= ' ')
		{
			insertTextAtCaret (String::charToString (key.getTextCharacter()));
		}
		else
		{
			return false;
		}
	}

	return true;
}

void CodeEditorComponent::mouseDown (const MouseEvent& e)
{
	newTransaction();
	dragType = notDragging;

	if (! e.mods.isPopupMenu())
	{
		beginDragAutoRepeat (100);
		moveCaretTo (getPositionAt (e.x, e.y), e.mods.isShiftDown());
	}
	else
	{
		/*PopupMenu m;
		addPopupMenuItems (m, &e);

		const int result = m.show();

		if (result != 0)
			performPopupMenuAction (result);
		*/
	}
}

void CodeEditorComponent::mouseDrag (const MouseEvent& e)
{
	if (! e.mods.isPopupMenu())
		moveCaretTo (getPositionAt (e.x, e.y), true);
}

void CodeEditorComponent::mouseUp (const MouseEvent&)
{
	newTransaction();
	beginDragAutoRepeat (0);
	dragType = notDragging;
}

void CodeEditorComponent::mouseDoubleClick (const MouseEvent& e)
{
	CodeDocument::Position tokenStart (getPositionAt (e.x, e.y));
	CodeDocument::Position tokenEnd (tokenStart);

	if (e.getNumberOfClicks() > 2)
	{
		tokenStart.setLineAndIndex (tokenStart.getLineNumber(), 0);
		tokenEnd.setLineAndIndex (tokenStart.getLineNumber() + 1, 0);
	}
	else
	{
		while (CharacterFunctions::isLetterOrDigit (tokenEnd.getCharacter()))
			tokenEnd.moveBy (1);

		tokenStart = tokenEnd;

		while (tokenStart.getIndexInLine() > 0
				&& CharacterFunctions::isLetterOrDigit (tokenStart.movedBy (-1).getCharacter()))
			tokenStart.moveBy (-1);
	}

	moveCaretTo (tokenEnd, false);
	moveCaretTo (tokenStart, true);
}

void CodeEditorComponent::mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY)
{
	if ((verticalScrollBar.isVisible() && wheelIncrementY != 0)
		 || (horizontalScrollBar.isVisible() && wheelIncrementX != 0))
	{
		verticalScrollBar.mouseWheelMove (e, 0, wheelIncrementY);
		horizontalScrollBar.mouseWheelMove (e, wheelIncrementX, 0);
	}
	else
	{
		Component::mouseWheelMove (e, wheelIncrementX, wheelIncrementY);
	}
}

void CodeEditorComponent::scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
	if (scrollBarThatHasMoved == &verticalScrollBar)
		scrollToLineInternal ((int) newRangeStart);
	else
		scrollToColumnInternal (newRangeStart);
}

void CodeEditorComponent::focusGained (FocusChangeType)
{
	updateCaretPosition();
}

void CodeEditorComponent::focusLost (FocusChangeType)
{
	updateCaretPosition();
}

void CodeEditorComponent::setTabSize (const int numSpaces, const bool insertSpaces)
{
	useSpacesForTabs = insertSpaces;

	if (spacesPerTab != numSpaces)
	{
		spacesPerTab = numSpaces;
		triggerAsyncUpdate();
	}
}

int CodeEditorComponent::indexToColumn (int lineNum, int index) const noexcept
{
	String::CharPointerType t (document.getLine (lineNum).getCharPointer());

	int col = 0;
	for (int i = 0; i < index; ++i)
	{
		if (t.isEmpty())
		{
			jassertfalse;
			break;
		}

		if (t.getAndAdvance() != '\t')
			++col;
		else
			col += getTabSize() - (col % getTabSize());
	}

	return col;
}

int CodeEditorComponent::columnToIndex (int lineNum, int column) const noexcept
{
	String::CharPointerType t (document.getLine (lineNum).getCharPointer());

	int i = 0, col = 0;

	while (! t.isEmpty())
	{
		if (t.getAndAdvance() != '\t')
			++col;
		else
			col += getTabSize() - (col % getTabSize());

		if (col > column)
			break;

		++i;
	}

	return i;
}

void CodeEditorComponent::setFont (const Font& newFont)
{
	font = newFont;
	charWidth = font.getStringWidthFloat ("0");
	lineHeight = roundToInt (font.getHeight());
	resized();
}

void CodeEditorComponent::resetToDefaultColours()
{
	coloursForTokenCategories.clear();

	if (codeTokeniser != nullptr)
	{
		for (int i = codeTokeniser->getTokenTypes().size(); --i >= 0;)
			setColourForTokenType (i, codeTokeniser->getDefaultColour (i));
	}
}

void CodeEditorComponent::setColourForTokenType (const int tokenType, const Colour& colour)
{
	jassert (tokenType < 256);

	while (coloursForTokenCategories.size() < tokenType)
		coloursForTokenCategories.add (Colours::black);

	coloursForTokenCategories.set (tokenType, colour);
	repaint();
}

Colour CodeEditorComponent::getColourForTokenType (const int tokenType) const
{
	if (! isPositiveAndBelow (tokenType, coloursForTokenCategories.size()))
		return findColour (CodeEditorComponent::defaultTextColourId);

	return coloursForTokenCategories.getReference (tokenType);
}

void CodeEditorComponent::clearCachedIterators (const int firstLineToBeInvalid)
{
	int i;
	for (i = cachedIterators.size(); --i >= 0;)
		if (cachedIterators.getUnchecked (i)->getLine() < firstLineToBeInvalid)
			break;

	cachedIterators.removeRange (jmax (0, i - 1), cachedIterators.size());
}

void CodeEditorComponent::updateCachedIterators (int maxLineNum)
{
	const int maxNumCachedPositions = 5000;
	const int linesBetweenCachedSources = jmax (10, document.getNumLines() / maxNumCachedPositions);

	if (cachedIterators.size() == 0)
		cachedIterators.add (new CodeDocument::Iterator (&document));

	if (codeTokeniser == nullptr)
		return;

	for (;;)
	{
		CodeDocument::Iterator* last = cachedIterators.getLast();

		if (last->getLine() >= maxLineNum)
			break;

		CodeDocument::Iterator* t = new CodeDocument::Iterator (*last);
		cachedIterators.add (t);
		const int targetLine = last->getLine() + linesBetweenCachedSources;

		for (;;)
		{
			codeTokeniser->readNextToken (*t);

			if (t->getLine() >= targetLine)
				break;

			if (t->isEOF())
				return;
		}
	}
}

void CodeEditorComponent::getIteratorForPosition (int position, CodeDocument::Iterator& source)
{
	if (codeTokeniser == nullptr)
		return;

	for (int i = cachedIterators.size(); --i >= 0;)
	{
		CodeDocument::Iterator* t = cachedIterators.getUnchecked (i);
		if (t->getPosition() <= position)
		{
			source = *t;
			break;
		}
	}

	while (source.getPosition() < position)
	{
		const CodeDocument::Iterator original (source);
		codeTokeniser->readNextToken (source);

		if (source.getPosition() > position || source.isEOF())
		{
			source = original;
			break;
		}
	}
}

/*** End of inlined file: juce_CodeEditorComponent.cpp ***/


/*** Start of inlined file: juce_CPlusPlusCodeTokeniser.cpp ***/
CPlusPlusCodeTokeniser::CPlusPlusCodeTokeniser() {}
CPlusPlusCodeTokeniser::~CPlusPlusCodeTokeniser() {}

namespace CppTokeniser
{
	static bool isIdentifierStart (const juce_wchar c) noexcept
	{
		return CharacterFunctions::isLetter (c)
				|| c == '_' || c == '@';
	}

	static bool isIdentifierBody (const juce_wchar c) noexcept
	{
		return CharacterFunctions::isLetterOrDigit (c)
				|| c == '_' || c == '@';
	}

	static bool isReservedKeyword (String::CharPointerType token, const int tokenLength) noexcept
	{
		static const char* const keywords2Char[] =
			{ "if", "do", "or", "id", 0 };

		static const char* const keywords3Char[] =
			{ "for", "int", "new", "try", "xor", "and", "asm", "not", 0 };

		static const char* const keywords4Char[] =
			{ "bool", "void", "this", "true", "long", "else", "char",
			  "enum", "case", "goto", "auto", 0 };

		static const char* const keywords5Char[] =
			{  "while", "bitor", "break", "catch", "class", "compl", "const", "false",
				"float", "short", "throw", "union", "using", "or_eq", 0 };

		static const char* const keywords6Char[] =
			{ "return", "struct", "and_eq", "bitand", "delete", "double", "extern",
			  "friend", "inline", "not_eq", "public", "sizeof", "static", "signed",
			  "switch", "typeid", "wchar_t", "xor_eq", 0};

		static const char* const keywords7Char[] =
			{ "default", "mutable", "private", "typedef", "nullptr", "virtual", 0 };

		static const char* const keywordsOther[] =
			{ "noexcept", "const_cast", "continue", "explicit", "namespace",
			  "operator", "protected", "register", "reinterpret_cast", "static_cast",
			  "template", "typename", "unsigned", "volatile", "constexpr",
			  "@implementation", "@interface", "@end", "@synthesize", "@dynamic", "@public",
			  "@private", "@property", "@protected", "@class", 0 };

		const char* const* k;

		switch (tokenLength)
		{
			case 2:     k = keywords2Char; break;
			case 3:     k = keywords3Char; break;
			case 4:     k = keywords4Char; break;
			case 5:     k = keywords5Char; break;
			case 6:     k = keywords6Char; break;
			case 7:     k = keywords7Char; break;

			default:
				if (tokenLength < 2 || tokenLength > 16)
					return false;

				k = keywordsOther;
				break;
		}

		int i = 0;
		while (k[i] != 0)
		{
			if (token.compare (CharPointer_ASCII (k[i])) == 0)
				return true;

			++i;
		}

		return false;
	}

	static int parseIdentifier (CodeDocument::Iterator& source) noexcept
	{
		int tokenLength = 0;
		String::CharPointerType::CharType possibleIdentifier [100];
		String::CharPointerType possible (possibleIdentifier);

		while (isIdentifierBody (source.peekNextChar()))
		{
			const juce_wchar c = source.nextChar();

			if (tokenLength < 20)
				possible.write (c);

			++tokenLength;
		}

		if (tokenLength > 1 && tokenLength <= 16)
		{
			possible.writeNull();

			if (isReservedKeyword (String::CharPointerType (possibleIdentifier), tokenLength))
				return CPlusPlusCodeTokeniser::tokenType_builtInKeyword;
		}

		return CPlusPlusCodeTokeniser::tokenType_identifier;
	}

	static bool skipNumberSuffix (CodeDocument::Iterator& source)
	{
		const juce_wchar c = source.peekNextChar();
		if (c == 'l' || c == 'L' || c == 'u' || c == 'U')
			source.skip();

		if (CharacterFunctions::isLetterOrDigit (source.peekNextChar()))
			return false;

		return true;
	}

	static bool isHexDigit (const juce_wchar c) noexcept
	{
		return (c >= '0' && c <= '9')
				|| (c >= 'a' && c <= 'f')
				|| (c >= 'A' && c <= 'F');
	}

	static bool parseHexLiteral (CodeDocument::Iterator& source) noexcept
	{
		if (source.nextChar() != '0')
			return false;

		juce_wchar c = source.nextChar();
		if (c != 'x' && c != 'X')
			return false;

		int numDigits = 0;
		while (isHexDigit (source.peekNextChar()))
		{
			++numDigits;
			source.skip();
		}

		if (numDigits == 0)
			return false;

		return skipNumberSuffix (source);
	}

	static bool isOctalDigit (const juce_wchar c) noexcept
	{
		return c >= '0' && c <= '7';
	}

	static bool parseOctalLiteral (CodeDocument::Iterator& source) noexcept
	{
		if (source.nextChar() != '0')
			return false;

		if (! isOctalDigit (source.nextChar()))
			 return false;

		while (isOctalDigit (source.peekNextChar()))
			source.skip();

		return skipNumberSuffix (source);
	}

	static bool isDecimalDigit (const juce_wchar c) noexcept
	{
		return c >= '0' && c <= '9';
	}

	static bool parseDecimalLiteral (CodeDocument::Iterator& source) noexcept
	{
		int numChars = 0;
		while (isDecimalDigit (source.peekNextChar()))
		{
			++numChars;
			source.skip();
		}

		if (numChars == 0)
			return false;

		return skipNumberSuffix (source);
	}

	static bool parseFloatLiteral (CodeDocument::Iterator& source) noexcept
	{
		int numDigits = 0;

		while (isDecimalDigit (source.peekNextChar()))
		{
			source.skip();
			++numDigits;
		}

		const bool hasPoint = (source.peekNextChar() == '.');

		if (hasPoint)
		{
			source.skip();

			while (isDecimalDigit (source.peekNextChar()))
			{
				source.skip();
				++numDigits;
			}
		}

		if (numDigits == 0)
			return false;

		juce_wchar c = source.peekNextChar();
		const bool hasExponent = (c == 'e' || c == 'E');

		if (hasExponent)
		{
			source.skip();

			c = source.peekNextChar();
			if (c == '+' || c == '-')
				source.skip();

			int numExpDigits = 0;
			while (isDecimalDigit (source.peekNextChar()))
			{
				source.skip();
				++numExpDigits;
			}

			if (numExpDigits == 0)
				return false;
		}

		c = source.peekNextChar();
		if (c == 'f' || c == 'F')
			source.skip();
		else if (! (hasExponent || hasPoint))
			return false;

		return true;
	}

	static int parseNumber (CodeDocument::Iterator& source)
	{
		const CodeDocument::Iterator original (source);

		if (parseFloatLiteral (source))
			return CPlusPlusCodeTokeniser::tokenType_floatLiteral;

		source = original;

		if (parseHexLiteral (source))
			return CPlusPlusCodeTokeniser::tokenType_integerLiteral;

		source = original;

		if (parseOctalLiteral (source))
			return CPlusPlusCodeTokeniser::tokenType_integerLiteral;

		source = original;

		if (parseDecimalLiteral (source))
			return CPlusPlusCodeTokeniser::tokenType_integerLiteral;

		source = original;
		source.skip();

		return CPlusPlusCodeTokeniser::tokenType_error;
	}

	static void skipQuotedString (CodeDocument::Iterator& source) noexcept
	{
		const juce_wchar quote = source.nextChar();

		for (;;)
		{
			const juce_wchar c = source.nextChar();

			if (c == quote || c == 0)
				break;

			if (c == '\\')
				source.skip();
		}
	}

	static void skipComment (CodeDocument::Iterator& source) noexcept
	{
		bool lastWasStar = false;

		for (;;)
		{
			const juce_wchar c = source.nextChar();

			if (c == 0 || (c == '/' && lastWasStar))
				break;

			lastWasStar = (c == '*');
		}
	}
}

int CPlusPlusCodeTokeniser::readNextToken (CodeDocument::Iterator& source)
{
	int result = tokenType_error;
	source.skipWhitespace();

	juce_wchar firstChar = source.peekNextChar();

	switch (firstChar)
	{
	case 0:
		source.skip();
		break;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		result = CppTokeniser::parseNumber (source);
		break;

	case '.':
		result = CppTokeniser::parseNumber (source);

		if (result == tokenType_error)
			result = tokenType_punctuation;

		break;

	case ',':
	case ';':
	case ':':
		source.skip();
		result = tokenType_punctuation;
		break;

	case '(':
	case ')':
	case '{':
	case '}':
	case '[':
	case ']':
		source.skip();
		result = tokenType_bracket;
		break;

	case '"':
	case '\'':
		CppTokeniser::skipQuotedString (source);
		result = tokenType_stringLiteral;
		break;

	case '+':
		result = tokenType_operator;
		source.skip();

		if (source.peekNextChar() == '+')
			source.skip();
		else if (source.peekNextChar() == '=')
			source.skip();

		break;

	case '-':
		source.skip();
		result = CppTokeniser::parseNumber (source);

		if (result == tokenType_error)
		{
			result = tokenType_operator;

			if (source.peekNextChar() == '-')
				source.skip();
			else if (source.peekNextChar() == '=')
				source.skip();
		}
		break;

	case '*':
	case '%':
	case '=':
	case '!':
		result = tokenType_operator;
		source.skip();

		if (source.peekNextChar() == '=')
			source.skip();

		break;

	case '/':
		result = tokenType_operator;
		source.skip();

		if (source.peekNextChar() == '=')
		{
			source.skip();
		}
		else if (source.peekNextChar() == '/')
		{
			result = tokenType_comment;
			source.skipToEndOfLine();
		}
		else if (source.peekNextChar() == '*')
		{
			source.skip();
			result = tokenType_comment;
			CppTokeniser::skipComment (source);
		}

		break;

	case '?':
	case '~':
		source.skip();
		result = tokenType_operator;
		break;

	case '<':
		source.skip();
		result = tokenType_operator;

		if (source.peekNextChar() == '=')
		{
			source.skip();
		}
		else if (source.peekNextChar() == '<')
		{
			source.skip();

			if (source.peekNextChar() == '=')
				source.skip();
		}

		break;

	case '>':
		source.skip();
		result = tokenType_operator;

		if (source.peekNextChar() == '=')
		{
			source.skip();
		}
		else if (source.peekNextChar() == '<')
		{
			source.skip();

			if (source.peekNextChar() == '=')
				source.skip();
		}

		break;

	case '|':
		source.skip();
		result = tokenType_operator;

		if (source.peekNextChar() == '=')
		{
			source.skip();
		}
		else if (source.peekNextChar() == '|')
		{
			source.skip();

			if (source.peekNextChar() == '=')
				source.skip();
		}

		break;

	case '&':
		source.skip();
		result = tokenType_operator;

		if (source.peekNextChar() == '=')
		{
			source.skip();
		}
		else if (source.peekNextChar() == '&')
		{
			source.skip();

			if (source.peekNextChar() == '=')
				source.skip();
		}

		break;

	case '^':
		source.skip();
		result = tokenType_operator;

		if (source.peekNextChar() == '=')
		{
			source.skip();
		}
		else if (source.peekNextChar() == '^')
		{
			source.skip();

			if (source.peekNextChar() == '=')
				source.skip();
		}

		break;

	case '#':
		result = tokenType_preprocessor;
		source.skipToEndOfLine();
		break;

	default:
		if (CppTokeniser::isIdentifierStart (firstChar))
			result = CppTokeniser::parseIdentifier (source);
		else
			source.skip();

		break;
	}

	return result;
}

StringArray CPlusPlusCodeTokeniser::getTokenTypes()
{
	const char* const types[] =
	{
		"Error",
		"Comment",
		"C++ keyword",
		"Identifier",
		"Integer literal",
		"Float literal",
		"String literal",
		"Operator",
		"Bracket",
		"Punctuation",
		"Preprocessor line",
		0
	};

	return StringArray (types);
}

Colour CPlusPlusCodeTokeniser::getDefaultColour (const int tokenType)
{
	const uint32 colours[] =
	{
		0xffcc0000,  // error
		0xff00aa00,  // comment
		0xff0000cc,  // keyword
		0xff000000,  // identifier
		0xff880000,  // int literal
		0xff885500,  // float literal
		0xff990099,  // string literal
		0xff225500,  // operator
		0xff000055,  // bracket
		0xff004400,  // punctuation
		0xff660000   // preprocessor
	};

	if (tokenType >= 0 && tokenType < numElementsInArray (colours))
		return Colour (colours [tokenType]);

	return Colours::black;
}

bool CPlusPlusCodeTokeniser::isReservedKeyword (const String& token) noexcept
{
	return CppTokeniser::isReservedKeyword (token.getCharPointer(), token.length());
}

/*** End of inlined file: juce_CPlusPlusCodeTokeniser.cpp ***/


/*** Start of inlined file: juce_OldSchoolLookAndFeel.cpp ***/
OldSchoolLookAndFeel::OldSchoolLookAndFeel()
{
	setColour (TextButton::buttonColourId,          Colour (0xffbbbbff));
	setColour (ListBox::outlineColourId,            findColour (ComboBox::outlineColourId));
	setColour (ScrollBar::thumbColourId,            Colour (0xffbbbbdd));
	setColour (ScrollBar::backgroundColourId,       Colours::transparentBlack);
	setColour (Slider::thumbColourId,               Colours::white);
	setColour (Slider::trackColourId,               Colour (0x7f000000));
	setColour (Slider::textBoxOutlineColourId,      Colours::grey);
	setColour (ProgressBar::backgroundColourId,     Colours::white.withAlpha (0.6f));
	setColour (ProgressBar::foregroundColourId,     Colours::green.withAlpha (0.7f));
	setColour (PopupMenu::backgroundColourId,             Colour (0xffeef5f8));
	setColour (PopupMenu::highlightedBackgroundColourId,  Colour (0xbfa4c2ce));
	setColour (PopupMenu::highlightedTextColourId,        Colours::black);
	setColour (TextEditor::focusedOutlineColourId,  findColour (TextButton::buttonColourId));

	scrollbarShadow.setShadowProperties (2.2f, 0.5f, 0, 0);
}

OldSchoolLookAndFeel::~OldSchoolLookAndFeel()
{
}

void OldSchoolLookAndFeel::drawButtonBackground (Graphics& g,
												 Button& button,
												 const Colour& backgroundColour,
												 bool isMouseOverButton,
												 bool isButtonDown)
{
	const int width = button.getWidth();
	const int height = button.getHeight();

	const float indent = 2.0f;
	const int cornerSize = jmin (roundToInt (width * 0.4f),
								 roundToInt (height * 0.4f));

	Path p;
	p.addRoundedRectangle (indent, indent,
						   width - indent * 2.0f,
						   height - indent * 2.0f,
						   (float) cornerSize);

	Colour bc (backgroundColour.withMultipliedSaturation (0.3f));

	if (isMouseOverButton)
	{
		if (isButtonDown)
			bc = bc.brighter();
		else if (bc.getBrightness() > 0.5f)
			bc = bc.darker (0.1f);
		else
			bc = bc.brighter (0.1f);
	}

	g.setColour (bc);
	g.fillPath (p);

	g.setColour (bc.contrasting().withAlpha ((isMouseOverButton) ? 0.6f : 0.4f));
	g.strokePath (p, PathStrokeType ((isMouseOverButton) ? 2.0f : 1.4f));
}

void OldSchoolLookAndFeel::drawTickBox (Graphics& g,
										Component& /*component*/,
										float x, float y, float w, float h,
										const bool ticked,
										const bool isEnabled,
										const bool /*isMouseOverButton*/,
										const bool isButtonDown)
{
	Path box;
	box.addRoundedRectangle (0.0f, 2.0f, 6.0f, 6.0f, 1.0f);

	g.setColour (isEnabled ? Colours::blue.withAlpha (isButtonDown ? 0.3f : 0.1f)
						   : Colours::lightgrey.withAlpha (0.1f));

	AffineTransform trans (AffineTransform::scale (w / 9.0f, h / 9.0f).translated (x, y));

	g.fillPath (box, trans);

	g.setColour (Colours::black.withAlpha (0.6f));
	g.strokePath (box, PathStrokeType (0.9f), trans);

	if (ticked)
	{
		Path tick;
		tick.startNewSubPath (1.5f, 3.0f);
		tick.lineTo (3.0f, 6.0f);
		tick.lineTo (6.0f, 0.0f);

		g.setColour (isEnabled ? Colours::black : Colours::grey);
		g.strokePath (tick, PathStrokeType (2.5f), trans);
	}
}

void OldSchoolLookAndFeel::drawToggleButton (Graphics& g,
											 ToggleButton& button,
											 bool isMouseOverButton,
											 bool isButtonDown)
{
	if (button.hasKeyboardFocus (true))
	{
		g.setColour (button.findColour (TextEditor::focusedOutlineColourId));
		g.drawRect (0, 0, button.getWidth(), button.getHeight());
	}

	const int tickWidth = jmin (20, button.getHeight() - 4);

	drawTickBox (g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
				 (float) tickWidth, (float) tickWidth,
				 button.getToggleState(),
				 button.isEnabled(),
				 isMouseOverButton,
				 isButtonDown);

	g.setColour (button.findColour (ToggleButton::textColourId));
	g.setFont (jmin (15.0f, button.getHeight() * 0.6f));

	if (! button.isEnabled())
		g.setOpacity (0.5f);

	const int textX = tickWidth + 5;

	g.drawFittedText (button.getButtonText(),
					  textX, 4,
					  button.getWidth() - textX - 2, button.getHeight() - 8,
					  Justification::centredLeft, 10);
}

void OldSchoolLookAndFeel::drawProgressBar (Graphics& g, ProgressBar& progressBar,
											int width, int height,
											double progress, const String& textToShow)
{
	if (progress < 0 || progress >= 1.0)
	{
		LookAndFeel::drawProgressBar (g, progressBar, width, height, progress, textToShow);
	}
	else
	{
		const Colour background (progressBar.findColour (ProgressBar::backgroundColourId));
		const Colour foreground (progressBar.findColour (ProgressBar::foregroundColourId));

		g.fillAll (background);
		g.setColour (foreground);

		g.fillRect (1, 1,
					jlimit (0, width - 2, roundToInt (progress * (width - 2))),
					height - 2);

		if (textToShow.isNotEmpty())
		{
			g.setColour (Colour::contrasting (background, foreground));
			g.setFont (height * 0.6f);

			g.drawText (textToShow, 0, 0, width, height, Justification::centred, false);
		}
	}
}

void OldSchoolLookAndFeel::drawScrollbarButton (Graphics& g,
												ScrollBar& bar,
												int width, int height,
												int buttonDirection,
												bool isScrollbarVertical,
												bool isMouseOverButton,
												bool isButtonDown)
{
	if (isScrollbarVertical)
		width -= 2;
	else
		height -= 2;

	Path p;

	if (buttonDirection == 0)
		p.addTriangle (width * 0.5f, height * 0.2f,
					   width * 0.1f, height * 0.7f,
					   width * 0.9f, height * 0.7f);
	else if (buttonDirection == 1)
		p.addTriangle (width * 0.8f, height * 0.5f,
					   width * 0.3f, height * 0.1f,
					   width * 0.3f, height * 0.9f);
	else if (buttonDirection == 2)
		p.addTriangle (width * 0.5f, height * 0.8f,
					   width * 0.1f, height * 0.3f,
					   width * 0.9f, height * 0.3f);
	else if (buttonDirection == 3)
		p.addTriangle (width * 0.2f, height * 0.5f,
					   width * 0.7f, height * 0.1f,
					   width * 0.7f, height * 0.9f);

	if (isButtonDown)
		g.setColour (Colours::white);
	else if (isMouseOverButton)
		g.setColour (Colours::white.withAlpha (0.7f));
	else
		g.setColour (bar.findColour (ScrollBar::thumbColourId).withAlpha (0.5f));

	g.fillPath (p);

	g.setColour (Colours::black.withAlpha (0.5f));
	g.strokePath (p, PathStrokeType (0.5f));
}

void OldSchoolLookAndFeel::drawScrollbar (Graphics& g,
										  ScrollBar& bar,
										  int x, int y,
										  int width, int height,
										  bool isScrollbarVertical,
										  int thumbStartPosition,
										  int thumbSize,
										  bool isMouseOver,
										  bool isMouseDown)
{
	g.fillAll (bar.findColour (ScrollBar::backgroundColourId));

	g.setColour (bar.findColour (ScrollBar::thumbColourId)
					.withAlpha ((isMouseOver || isMouseDown) ? 0.4f : 0.15f));

	if (thumbSize > 0.0f)
	{
		Rectangle<int> thumb;

		if (isScrollbarVertical)
		{
			width -= 2;
			g.fillRect (x + roundToInt (width * 0.35f), y,
						roundToInt (width * 0.3f), height);

			thumb.setBounds (x + 1, thumbStartPosition,
							 width - 2, thumbSize);
		}
		else
		{
			height -= 2;
			g.fillRect (x, y + roundToInt (height * 0.35f),
						width, roundToInt (height * 0.3f));

			thumb.setBounds (thumbStartPosition, y + 1,
							 thumbSize, height - 2);
		}

		g.setColour (bar.findColour (ScrollBar::thumbColourId)
						.withAlpha ((isMouseOver || isMouseDown) ? 0.95f : 0.7f));

		g.fillRect (thumb);

		g.setColour (Colours::black.withAlpha ((isMouseOver || isMouseDown) ? 0.4f : 0.25f));
		g.drawRect (thumb.getX(), thumb.getY(), thumb.getWidth(), thumb.getHeight());

		if (thumbSize > 16)
		{
			for (int i = 3; --i >= 0;)
			{
				const float linePos = thumbStartPosition + thumbSize / 2 + (i - 1) * 4.0f;
				g.setColour (Colours::black.withAlpha (0.15f));

				if (isScrollbarVertical)
				{
					g.drawLine (x + width * 0.2f, linePos, width * 0.8f, linePos);
					g.setColour (Colours::white.withAlpha (0.15f));
					g.drawLine (width * 0.2f, linePos - 1, width * 0.8f, linePos - 1);
				}
				else
				{
					g.drawLine (linePos, height * 0.2f, linePos, height * 0.8f);
					g.setColour (Colours::white.withAlpha (0.15f));
					g.drawLine (linePos - 1, height * 0.2f, linePos - 1, height * 0.8f);
				}
			}
		}
	}
}

ImageEffectFilter* OldSchoolLookAndFeel::getScrollbarEffect()
{
	return &scrollbarShadow;
}

void OldSchoolLookAndFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
	g.fillAll (findColour (PopupMenu::backgroundColourId));

	g.setColour (Colours::black.withAlpha (0.6f));
	g.drawRect (0, 0, width, height);
}

void OldSchoolLookAndFeel::drawMenuBarBackground (Graphics& g, int /*width*/, int /*height*/,
												  bool, MenuBarComponent& menuBar)
{
	g.fillAll (menuBar.findColour (PopupMenu::backgroundColourId));
}

void OldSchoolLookAndFeel::drawTextEditorOutline (Graphics& g, int width, int height, TextEditor& textEditor)
{
	if (textEditor.isEnabled())
	{
		g.setColour (textEditor.findColour (TextEditor::outlineColourId));
		g.drawRect (0, 0, width, height);
	}
}

void OldSchoolLookAndFeel::drawComboBox (Graphics& g, int width, int height,
										 const bool isButtonDown,
										 int buttonX, int buttonY,
										 int buttonW, int buttonH,
										 ComboBox& box)
{
	g.fillAll (box.findColour (ComboBox::backgroundColourId));

	g.setColour (box.findColour ((isButtonDown) ? ComboBox::buttonColourId
												: ComboBox::backgroundColourId));
	g.fillRect (buttonX, buttonY, buttonW, buttonH);

	g.setColour (box.findColour (ComboBox::outlineColourId));
	g.drawRect (0, 0, width, height);

	const float arrowX = 0.2f;
	const float arrowH = 0.3f;

	if (box.isEnabled())
	{
		Path p;
		p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.45f - arrowH),
					   buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.45f,
					   buttonX + buttonW * arrowX,          buttonY + buttonH * 0.45f);

		p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.55f + arrowH),
					   buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.55f,
					   buttonX + buttonW * arrowX,          buttonY + buttonH * 0.55f);

		g.setColour (box.findColour ((isButtonDown) ? ComboBox::backgroundColourId
													: ComboBox::buttonColourId));
		g.fillPath (p);
	}
}

const Font OldSchoolLookAndFeel::getComboBoxFont (ComboBox& box)
{
	Font f (jmin (15.0f, box.getHeight() * 0.85f));
	f.setHorizontalScale (0.9f);
	return f;
}

static void drawTriangle (Graphics& g, float x1, float y1, float x2, float y2, float x3, float y3, const Colour& fill, const Colour& outline) noexcept
{
	Path p;
	p.addTriangle (x1, y1, x2, y2, x3, y3);
	g.setColour (fill);
	g.fillPath (p);

	g.setColour (outline);
	g.strokePath (p, PathStrokeType (0.3f));
}

void OldSchoolLookAndFeel::drawLinearSlider (Graphics& g,
											 int x, int y,
											 int w, int h,
											 float sliderPos,
											 float minSliderPos,
											 float maxSliderPos,
											 const Slider::SliderStyle style,
											 Slider& slider)
{
	g.fillAll (slider.findColour (Slider::backgroundColourId));

	if (style == Slider::LinearBar)
	{
		g.setColour (slider.findColour (Slider::thumbColourId));
		g.fillRect (x, y, (int) sliderPos - x, h);

		g.setColour (slider.findColour (Slider::textBoxTextColourId).withMultipliedAlpha (0.5f));
		g.drawRect (x, y, (int) sliderPos - x, h);
	}
	else
	{
		g.setColour (slider.findColour (Slider::trackColourId)
						   .withMultipliedAlpha (slider.isEnabled() ? 1.0f : 0.3f));

		if (slider.isHorizontal())
		{
			g.fillRect (x, y + roundToInt (h * 0.6f),
						w, roundToInt (h * 0.2f));
		}
		else
		{
			g.fillRect (x + roundToInt (w * 0.5f - jmin (3.0f, w * 0.1f)), y,
						jmin (4, roundToInt (w * 0.2f)), h);
		}

		float alpha = 0.35f;

		if (slider.isEnabled())
			alpha = slider.isMouseOverOrDragging() ? 1.0f : 0.7f;

		const Colour fill (slider.findColour (Slider::thumbColourId).withAlpha (alpha));
		const Colour outline (Colours::black.withAlpha (slider.isEnabled() ? 0.7f : 0.35f));

		if (style == Slider::TwoValueVertical || style == Slider::ThreeValueVertical)
		{
			drawTriangle (g, x + w * 0.5f + jmin (4.0f, w * 0.3f), minSliderPos,
						  x + w * 0.5f - jmin (8.0f, w * 0.4f), minSliderPos - 7.0f,
						  x + w * 0.5f - jmin (8.0f, w * 0.4f), minSliderPos,
						  fill, outline);

			drawTriangle (g, x + w * 0.5f + jmin (4.0f, w * 0.3f), maxSliderPos,
						  x + w * 0.5f - jmin (8.0f, w * 0.4f), maxSliderPos,
						  x + w * 0.5f - jmin (8.0f, w * 0.4f), maxSliderPos + 7.0f,
						  fill, outline);
		}
		else if (style == Slider::TwoValueHorizontal || style == Slider::ThreeValueHorizontal)
		{
			drawTriangle (g, minSliderPos, y + h * 0.6f - jmin (4.0f, h * 0.3f),
						  minSliderPos - 7.0f, y + h * 0.9f ,
						  minSliderPos, y + h * 0.9f,
						  fill, outline);

			drawTriangle (g, maxSliderPos, y + h * 0.6f - jmin (4.0f, h * 0.3f),
						  maxSliderPos, y + h * 0.9f,
						  maxSliderPos + 7.0f, y + h * 0.9f,
						  fill, outline);
		}

		if (style == Slider::LinearHorizontal || style == Slider::ThreeValueHorizontal)
		{
			drawTriangle (g, sliderPos, y + h * 0.9f,
						  sliderPos - 7.0f, y + h * 0.2f,
						  sliderPos + 7.0f, y + h * 0.2f,
						  fill, outline);
		}
		else if (style == Slider::LinearVertical || style == Slider::ThreeValueVertical)
		{
			drawTriangle (g, x + w * 0.5f - jmin (4.0f, w * 0.3f), sliderPos,
						  x + w * 0.5f + jmin (8.0f, w * 0.4f), sliderPos - 7.0f,
						  x + w * 0.5f + jmin (8.0f, w * 0.4f), sliderPos + 7.0f,
						  fill, outline);
		}
	}
}

Button* OldSchoolLookAndFeel::createSliderButton (const bool isIncrement)
{
	if (isIncrement)
		return new ArrowButton ("u", 0.75f, Colours::white.withAlpha (0.8f));
	else
		return new ArrowButton ("d", 0.25f, Colours::white.withAlpha (0.8f));
}

ImageEffectFilter* OldSchoolLookAndFeel::getSliderEffect()
{
	return &scrollbarShadow;
}

int OldSchoolLookAndFeel::getSliderThumbRadius (Slider&)
{
	return 8;
}

void OldSchoolLookAndFeel::drawCornerResizer (Graphics& g,
											  int w, int h,
											  bool isMouseOver,
											  bool isMouseDragging)
{
	g.setColour ((isMouseOver || isMouseDragging) ? Colours::lightgrey
												  : Colours::darkgrey);

	const float lineThickness = jmin (w, h) * 0.1f;

	for (float i = 0.0f; i < 1.0f; i += 0.3f)
	{
		g.drawLine (w * i,
					h + 1.0f,
					w + 1.0f,
					h * i,
					lineThickness);
	}
}

Button* OldSchoolLookAndFeel::createDocumentWindowButton (int buttonType)
{
	Path shape;

	if (buttonType == DocumentWindow::closeButton)
	{
		shape.addLineSegment (Line<float> (0.0f, 0.0f, 1.0f, 1.0f), 0.35f);
		shape.addLineSegment (Line<float> (1.0f, 0.0f, 0.0f, 1.0f), 0.35f);

		ShapeButton* const b = new ShapeButton ("close",
												Colour (0x7fff3333),
												Colour (0xd7ff3333),
												Colour (0xf7ff3333));

		b->setShape (shape, true, true, true);
		return b;
	}
	else if (buttonType == DocumentWindow::minimiseButton)
	{
		shape.addLineSegment (Line<float> (0.0f, 0.5f, 1.0f, 0.5f), 0.25f);

		DrawableButton* b = new DrawableButton ("minimise", DrawableButton::ImageFitted);
		DrawablePath dp;
		dp.setPath (shape);
		dp.setFill (Colours::black.withAlpha (0.3f));
		b->setImages (&dp);
		return b;
	}
	else if (buttonType == DocumentWindow::maximiseButton)
	{
		shape.addLineSegment (Line<float> (0.5f, 0.0f, 0.5f, 1.0f), 0.25f);
		shape.addLineSegment (Line<float> (0.0f, 0.5f, 1.0f, 0.5f), 0.25f);

		DrawableButton* b = new DrawableButton ("maximise", DrawableButton::ImageFitted);
		DrawablePath dp;
		dp.setPath (shape);
		dp.setFill (Colours::black.withAlpha (0.3f));
		b->setImages (&dp);
		return b;
	}

	jassertfalse;
	return nullptr;
}

void OldSchoolLookAndFeel::positionDocumentWindowButtons (DocumentWindow&,
														  int titleBarX,
														  int titleBarY,
														  int titleBarW,
														  int titleBarH,
														  Button* minimiseButton,
														  Button* maximiseButton,
														  Button* closeButton,
														  bool positionTitleBarButtonsOnLeft)
{
	titleBarY += titleBarH / 8;
	titleBarH -= titleBarH / 4;

	const int buttonW = titleBarH;

	int x = positionTitleBarButtonsOnLeft ? titleBarX + 4
										  : titleBarX + titleBarW - buttonW - 4;

	if (closeButton != nullptr)
	{
		closeButton->setBounds (x, titleBarY, buttonW, titleBarH);
		x += positionTitleBarButtonsOnLeft ? buttonW + buttonW / 5
										   : -(buttonW + buttonW / 5);
	}

	if (positionTitleBarButtonsOnLeft)
		std::swap (minimiseButton, maximiseButton);

	if (maximiseButton != nullptr)
	{
		maximiseButton->setBounds (x, titleBarY - 2, buttonW, titleBarH);
		x += positionTitleBarButtonsOnLeft ? buttonW : -buttonW;
	}

	if (minimiseButton != nullptr)
		minimiseButton->setBounds (x, titleBarY - 2, buttonW, titleBarH);
}

/*** End of inlined file: juce_OldSchoolLookAndFeel.cpp ***/


/*** Start of inlined file: juce_BubbleMessageComponent.cpp ***/
BubbleMessageComponent::BubbleMessageComponent (int fadeOutLengthMs)
	: fadeOutLength (fadeOutLengthMs),
	  deleteAfterUse (false)
{
}

BubbleMessageComponent::~BubbleMessageComponent()
{
	Desktop::getInstance().getAnimator().fadeOut (this, fadeOutLength);
}

void BubbleMessageComponent::showAt (int x, int y,
									 const String& text,
									 const int numMillisecondsBeforeRemoving,
									 const bool removeWhenMouseClicked,
									 const bool deleteSelfAfterUse)
{
	createLayout (text);
	setPosition (x, y);
	init (numMillisecondsBeforeRemoving, removeWhenMouseClicked, deleteSelfAfterUse);
}

void BubbleMessageComponent::showAt (Component* const component,
									 const String& text,
									 const int numMillisecondsBeforeRemoving,
									 const bool removeWhenMouseClicked,
									 const bool deleteSelfAfterUse)
{
	createLayout (text);
	setPosition (component);
	init (numMillisecondsBeforeRemoving, removeWhenMouseClicked, deleteSelfAfterUse);
}

void BubbleMessageComponent::createLayout (const String& text)
{
	AttributedString attString;
	attString.append (text, Font (14.0f));
	attString.setJustification (Justification::centred);

	textLayout.createLayoutWithBalancedLineLengths (attString, 256);
}

void BubbleMessageComponent::init (const int numMillisecondsBeforeRemoving,
								   const bool removeWhenMouseClicked,
								   const bool deleteSelfAfterUse)
{
	setVisible (true);

	deleteAfterUse = deleteSelfAfterUse;

	if (numMillisecondsBeforeRemoving > 0)
		expiryTime = Time::getMillisecondCounter() + numMillisecondsBeforeRemoving;
	else
		expiryTime = 0;

	startTimer (77);

	mouseClickCounter = Desktop::getInstance().getMouseButtonClickCounter();

	if (! (removeWhenMouseClicked && isShowing()))
		mouseClickCounter += 0xfffff;

	repaint();
}

void BubbleMessageComponent::getContentSize (int& w, int& h)
{
	w = (int) (textLayout.getWidth() + 16.0f);
	h = (int) (textLayout.getHeight() + 16.0f);
}

void BubbleMessageComponent::paintContent (Graphics& g, int w, int h)
{
	g.setColour (findColour (TooltipWindow::textColourId));

	textLayout.draw (g, Rectangle<float> (0.0f, 0.0f, (float) w, (float) h));
}

void BubbleMessageComponent::timerCallback()
{
	if (Desktop::getInstance().getMouseButtonClickCounter() > mouseClickCounter)
	{
		stopTimer();
		setVisible (false);

		if (deleteAfterUse)
			delete this;
	}
	else if (expiryTime != 0 && Time::getMillisecondCounter() > expiryTime)
	{
		stopTimer();

		if (deleteAfterUse)
			delete this;
		else
			Desktop::getInstance().getAnimator().fadeOut (this, fadeOutLength);
	}
}

/*** End of inlined file: juce_BubbleMessageComponent.cpp ***/


/*** Start of inlined file: juce_ColourSelector.cpp ***/
class ColourComponentSlider  : public Slider
{
public:
	ColourComponentSlider (const String& name)
		: Slider (name)
	{
		setRange (0.0, 255.0, 1.0);
	}

	String getTextFromValue (double value)
	{
		return String::toHexString ((int) value).toUpperCase().paddedLeft ('0', 2);
	}

	double getValueFromText (const String& text)
	{
		return (double) text.getHexValue32();
	}

private:
	JUCE_DECLARE_NON_COPYABLE (ColourComponentSlider);
};

class ColourSpaceMarker  : public Component
{
public:
	ColourSpaceMarker()
	{
		setInterceptsMouseClicks (false, false);
	}

	void paint (Graphics& g)
	{
		g.setColour (Colour::greyLevel (0.1f));
		g.drawEllipse (1.0f, 1.0f, getWidth() - 2.0f, getHeight() - 2.0f, 1.0f);
		g.setColour (Colour::greyLevel (0.9f));
		g.drawEllipse (2.0f, 2.0f, getWidth() - 4.0f, getHeight() - 4.0f, 1.0f);
	}

private:
	JUCE_DECLARE_NON_COPYABLE (ColourSpaceMarker);
};

class ColourSelector::ColourSpaceView  : public Component
{
public:
	ColourSpaceView (ColourSelector& owner_,
					 float& h_, float& s_, float& v_,
					 const int edgeSize)
		: owner (owner_),
		  h (h_), s (s_), v (v_),
		  lastHue (0.0f),
		  edge (edgeSize)
	{
		addAndMakeVisible (&marker);
		setMouseCursor (MouseCursor::CrosshairCursor);
	}

	void paint (Graphics& g)
	{
		if (colours.isNull())
		{
			const int width = getWidth() / 2;
			const int height = getHeight() / 2;
			colours = Image (Image::RGB, width, height, false);

			Image::BitmapData pixels (colours, Image::BitmapData::writeOnly);

			for (int y = 0; y < height; ++y)
			{
				const float val = 1.0f - y / (float) height;

				for (int x = 0; x < width; ++x)
				{
					const float sat = x / (float) width;

					pixels.setPixelColour (x, y, Colour (h, sat, val, 1.0f));
				}
			}
		}

		g.setOpacity (1.0f);
		g.drawImage (colours, edge, edge, getWidth() - edge * 2, getHeight() - edge * 2,
					 0, 0, colours.getWidth(), colours.getHeight());
	}

	void mouseDown (const MouseEvent& e)
	{
		mouseDrag (e);
	}

	void mouseDrag (const MouseEvent& e)
	{
		const float sat = (e.x - edge) / (float) (getWidth() - edge * 2);
		const float val = 1.0f - (e.y - edge) / (float) (getHeight() - edge * 2);

		owner.setSV (sat, val);
	}

	void updateIfNeeded()
	{
		if (lastHue != h)
		{
			lastHue = h;
			colours = Image::null;
			repaint();
		}

		updateMarker();
	}

	void resized()
	{
		colours = Image::null;
		updateMarker();
	}

private:
	ColourSelector& owner;
	float& h;
	float& s;
	float& v;
	float lastHue;
	ColourSpaceMarker marker;
	const int edge;
	Image colours;

	void updateMarker()
	{
		marker.setBounds (roundToInt ((getWidth() - edge * 2) * s),
						  roundToInt ((getHeight() - edge * 2) * (1.0f - v)),
						  edge * 2, edge * 2);
	}

	JUCE_DECLARE_NON_COPYABLE (ColourSpaceView);
};

class HueSelectorMarker  : public Component
{
public:
	HueSelectorMarker()
	{
		setInterceptsMouseClicks (false, false);
	}

	void paint (Graphics& g)
	{
		Path p;
		p.addTriangle (1.0f, 1.0f,
					   getWidth() * 0.3f, getHeight() * 0.5f,
					   1.0f, getHeight() - 1.0f);

		p.addTriangle (getWidth() - 1.0f, 1.0f,
					   getWidth() * 0.7f, getHeight() * 0.5f,
					   getWidth() - 1.0f, getHeight() - 1.0f);

		g.setColour (Colours::white.withAlpha (0.75f));
		g.fillPath (p);

		g.setColour (Colours::black.withAlpha (0.75f));
		g.strokePath (p, PathStrokeType (1.2f));
	}

private:
	JUCE_DECLARE_NON_COPYABLE (HueSelectorMarker);
};

class ColourSelector::HueSelectorComp  : public Component
{
public:
	HueSelectorComp (ColourSelector& owner_,
					 float& h_, float& s_, float& v_,
					 const int edgeSize)
		: owner (owner_),
		  h (h_), s (s_), v (v_),
		  lastHue (0.0f),
		  edge (edgeSize)
	{
		addAndMakeVisible (&marker);
	}

	void paint (Graphics& g)
	{
		const float yScale = 1.0f / (getHeight() - edge * 2);

		const Rectangle<int> clip (g.getClipBounds());

		for (int y = jmin (clip.getBottom(), getHeight() - edge); --y >= jmax (edge, clip.getY());)
		{
			g.setColour (Colour ((y - edge) * yScale, 1.0f, 1.0f, 1.0f));
			g.fillRect (edge, y, getWidth() - edge * 2, 1);
		}
	}

	void resized()
	{
		marker.setBounds (0, roundToInt ((getHeight() - edge * 2) * h),
						  getWidth(), edge * 2);
	}

	void mouseDown (const MouseEvent& e)
	{
		mouseDrag (e);
	}

	void mouseDrag (const MouseEvent& e)
	{
		owner.setHue ((e.y - edge) / (float) (getHeight() - edge * 2));
	}

	void updateIfNeeded()
	{
		resized();
	}

private:
	ColourSelector& owner;
	float& h;
	float& s;
	float& v;
	float lastHue;
	HueSelectorMarker marker;
	const int edge;

	JUCE_DECLARE_NON_COPYABLE (HueSelectorComp);
};

class ColourSelector::SwatchComponent   : public Component
{
public:
	SwatchComponent (ColourSelector& owner_, int index_)
		: owner (owner_), index (index_)
	{
	}

	void paint (Graphics& g)
	{
		const Colour colour (owner.getSwatchColour (index));

		g.fillCheckerBoard (getLocalBounds(), 6, 6,
							Colour (0xffdddddd).overlaidWith (colour),
							Colour (0xffffffff).overlaidWith (colour));
	}

	void mouseDown (const MouseEvent&)
	{
		PopupMenu m;
		m.addItem (1, TRANS("Use this swatch as the current colour"));
		m.addSeparator();
		m.addItem (2, TRANS("Set this swatch to the current colour"));

		m.showMenuAsync (PopupMenu::Options().withTargetComponent (this),
						 ModalCallbackFunction::forComponent (menuStaticCallback, this));
	}

private:
	ColourSelector& owner;
	const int index;

	static void menuStaticCallback (int result, SwatchComponent* comp)
	{
		if (comp != nullptr)
		{
			if (result == 1)
				comp->setColourFromSwatch();
			else if (result == 2)
				comp->setSwatchFromColour();
		}
	}

	void setColourFromSwatch()
	{
		owner.setCurrentColour (owner.getSwatchColour (index));
	}

	void setSwatchFromColour()
	{
		if (owner.getSwatchColour (index) != owner.getCurrentColour())
		{
			owner.setSwatchColour (index, owner.getCurrentColour());
			repaint();
		}
	}

	JUCE_DECLARE_NON_COPYABLE (SwatchComponent);
};

ColourSelector::ColourSelector (const int flags_,
								const int edgeGap_,
								const int gapAroundColourSpaceComponent)
	: colour (Colours::white),
	  flags (flags_),
	  edgeGap (edgeGap_)
{
	// not much point having a selector with no components in it!
	jassert ((flags_ & (showColourAtTop | showSliders | showColourspace)) != 0);

	updateHSV();

	if ((flags & showSliders) != 0)
	{
		addAndMakeVisible (sliders[0] = new ColourComponentSlider (TRANS ("red")));
		addAndMakeVisible (sliders[1] = new ColourComponentSlider (TRANS ("green")));
		addAndMakeVisible (sliders[2] = new ColourComponentSlider (TRANS ("blue")));
		addChildComponent (sliders[3] = new ColourComponentSlider (TRANS ("alpha")));

		sliders[3]->setVisible ((flags & showAlphaChannel) != 0);

		for (int i = 4; --i >= 0;)
			sliders[i]->addListener (this);
	}

	if ((flags & showColourspace) != 0)
	{
		addAndMakeVisible (colourSpace = new ColourSpaceView (*this, h, s, v, gapAroundColourSpaceComponent));
		addAndMakeVisible (hueSelector = new HueSelectorComp (*this, h, s, v, gapAroundColourSpaceComponent));
	}

	update();
}

ColourSelector::~ColourSelector()
{
	dispatchPendingMessages();
	swatchComponents.clear();
}

Colour ColourSelector::getCurrentColour() const
{
	return ((flags & showAlphaChannel) != 0) ? colour
											 : colour.withAlpha ((uint8) 0xff);
}

void ColourSelector::setCurrentColour (const Colour& c)
{
	if (c != colour)
	{
		colour = ((flags & showAlphaChannel) != 0) ? c : c.withAlpha ((uint8) 0xff);

		updateHSV();
		update();
	}
}

void ColourSelector::setHue (float newH)
{
	newH = jlimit (0.0f, 1.0f, newH);

	if (h != newH)
	{
		h = newH;
		colour = Colour (h, s, v, colour.getFloatAlpha());
		update();
	}
}

void ColourSelector::setSV (float newS, float newV)
{
	newS = jlimit (0.0f, 1.0f, newS);
	newV = jlimit (0.0f, 1.0f, newV);

	if (s != newS || v != newV)
	{
		s = newS;
		v = newV;
		colour = Colour (h, s, v, colour.getFloatAlpha());
		update();
	}
}

void ColourSelector::updateHSV()
{
	colour.getHSB (h, s, v);
}

void ColourSelector::update()
{
	if (sliders[0] != nullptr)
	{
		sliders[0]->setValue ((int) colour.getRed());
		sliders[1]->setValue ((int) colour.getGreen());
		sliders[2]->setValue ((int) colour.getBlue());
		sliders[3]->setValue ((int) colour.getAlpha());
	}

	if (colourSpace != nullptr)
	{
		colourSpace->updateIfNeeded();
		hueSelector->updateIfNeeded();
	}

	if ((flags & showColourAtTop) != 0)
		repaint (previewArea);

	sendChangeMessage();
}

void ColourSelector::paint (Graphics& g)
{
	g.fillAll (findColour (backgroundColourId));

	if ((flags & showColourAtTop) != 0)
	{
		const Colour currentColour (getCurrentColour());

		g.fillCheckerBoard (previewArea, 10, 10,
							Colour (0xffdddddd).overlaidWith (currentColour),
							Colour (0xffffffff).overlaidWith (currentColour));

		g.setColour (Colours::white.overlaidWith (currentColour).contrasting());
		g.setFont (14.0f, Font::bold);
		g.drawText (currentColour.toDisplayString ((flags & showAlphaChannel) != 0),
					previewArea.getX(), previewArea.getY(), previewArea.getWidth(), previewArea.getHeight(),
					Justification::centred, false);
	}

	if ((flags & showSliders) != 0)
	{
		g.setColour (findColour (labelTextColourId));
		g.setFont (11.0f);

		for (int i = 4; --i >= 0;)
		{
			if (sliders[i]->isVisible())
				g.drawText (sliders[i]->getName() + ":",
							0, sliders[i]->getY(),
							sliders[i]->getX() - 8, sliders[i]->getHeight(),
							Justification::centredRight, false);
		}
	}
}

void ColourSelector::resized()
{
	const int swatchesPerRow = 8;
	const int swatchHeight = 22;

	const int numSliders = ((flags & showAlphaChannel) != 0) ? 4 : 3;
	const int numSwatches = getNumSwatches();

	const int swatchSpace = numSwatches > 0 ? edgeGap + swatchHeight * ((numSwatches + 7) / swatchesPerRow) : 0;
	const int sliderSpace = ((flags & showSliders) != 0)  ? jmin (22 * numSliders + edgeGap, proportionOfHeight (0.3f)) : 0;
	const int topSpace = ((flags & showColourAtTop) != 0) ? jmin (30 + edgeGap * 2, proportionOfHeight (0.2f)) : edgeGap;

	previewArea.setBounds (edgeGap, edgeGap, getWidth() - edgeGap * 2, topSpace - edgeGap * 2);

	int y = topSpace;

	if ((flags & showColourspace) != 0)
	{
		const int hueWidth = jmin (50, proportionOfWidth (0.15f));

		colourSpace->setBounds (edgeGap, y,
								getWidth() - hueWidth - edgeGap - 4,
								getHeight() - topSpace - sliderSpace - swatchSpace - edgeGap);

		hueSelector->setBounds (colourSpace->getRight() + 4, y,
								getWidth() - edgeGap - (colourSpace->getRight() + 4),
								colourSpace->getHeight());

		y = getHeight() - sliderSpace - swatchSpace - edgeGap;
	}

	if ((flags & showSliders) != 0)
	{
		const int sliderHeight = jmax (4, sliderSpace / numSliders);

		for (int i = 0; i < numSliders; ++i)
		{
			sliders[i]->setBounds (proportionOfWidth (0.2f), y,
								   proportionOfWidth (0.72f), sliderHeight - 2);

			y += sliderHeight;
		}
	}

	if (numSwatches > 0)
	{
		const int startX = 8;
		const int xGap = 4;
		const int yGap = 4;
		const int swatchWidth = (getWidth() - startX * 2) / swatchesPerRow;
		y += edgeGap;

		if (swatchComponents.size() != numSwatches)
		{
			swatchComponents.clear();

			for (int i = 0; i < numSwatches; ++i)
			{
				SwatchComponent* const sc = new SwatchComponent (*this, i);
				swatchComponents.add (sc);
				addAndMakeVisible (sc);
			}
		}

		int x = startX;

		for (int i = 0; i < swatchComponents.size(); ++i)
		{
			SwatchComponent* const sc = swatchComponents.getUnchecked(i);

			sc->setBounds (x + xGap / 2,
						   y + yGap / 2,
						   swatchWidth - xGap,
						   swatchHeight - yGap);

			if (((i + 1) % swatchesPerRow) == 0)
			{
				x = startX;
				y += swatchHeight;
			}
			else
			{
				x += swatchWidth;
			}
		}
	}
}

void ColourSelector::sliderValueChanged (Slider*)
{
	if (sliders[0] != nullptr)
		setCurrentColour (Colour ((uint8) sliders[0]->getValue(),
								  (uint8) sliders[1]->getValue(),
								  (uint8) sliders[2]->getValue(),
								  (uint8) sliders[3]->getValue()));
}

int ColourSelector::getNumSwatches() const
{
	return 0;
}

Colour ColourSelector::getSwatchColour (const int) const
{
	jassertfalse; // if you've overridden getNumSwatches(), you also need to implement this method
	return Colours::black;
}

void ColourSelector::setSwatchColour (const int, const Colour&) const
{
	jassertfalse; // if you've overridden getNumSwatches(), you also need to implement this method
}

/*** End of inlined file: juce_ColourSelector.cpp ***/


/*** Start of inlined file: juce_KeyMappingEditorComponent.cpp ***/
class KeyMappingEditorComponent::ChangeKeyButton  : public Button
{
public:
	ChangeKeyButton (KeyMappingEditorComponent& owner_,
					 const CommandID commandID_,
					 const String& keyName,
					 const int keyNum_)
		: Button (keyName),
		  owner (owner_),
		  commandID (commandID_),
		  keyNum (keyNum_)
	{
		setWantsKeyboardFocus (false);
		setTriggeredOnMouseDown (keyNum >= 0);

		setTooltip (keyNum_ < 0 ? TRANS("adds a new key-mapping")
								: TRANS("click to change this key-mapping"));
	}

	void paintButton (Graphics& g, bool /*isOver*/, bool /*isDown*/)
	{
		getLookAndFeel().drawKeymapChangeButton (g, getWidth(), getHeight(), *this,
												 keyNum >= 0 ? getName() : String::empty);
	}

	static void menuCallback (int result, ChangeKeyButton* button)
	{
		if (button != nullptr)
		{
			switch (result)
			{
				case 1: button->assignNewKey(); break;
				case 2: button->owner.getMappings().removeKeyPress (button->commandID, button->keyNum); break;
				default: break;
			}
		}
	}

	void clicked()
	{
		if (keyNum >= 0)
		{
			// existing key clicked..
			PopupMenu m;
			m.addItem (1, TRANS("change this key-mapping"));
			m.addSeparator();
			m.addItem (2, TRANS("remove this key-mapping"));

			m.showMenuAsync (PopupMenu::Options(),
							 ModalCallbackFunction::forComponent (menuCallback, this));
		}
		else
		{
			assignNewKey();  // + button pressed..
		}
	}

	void fitToContent (const int h) noexcept
	{
		if (keyNum < 0)
		{
			setSize (h, h);
		}
		else
		{
			Font f (h * 0.6f);
			setSize (jlimit (h * 4, h * 8, 6 + f.getStringWidth (getName())), h);
		}
	}

	class KeyEntryWindow  : public AlertWindow
	{
	public:
		KeyEntryWindow (KeyMappingEditorComponent& owner_)
			: AlertWindow (TRANS("New key-mapping"),
						   TRANS("Please press a key combination now..."),
						   AlertWindow::NoIcon),
			  owner (owner_)
		{
			addButton (TRANS("Ok"), 1);
			addButton (TRANS("Cancel"), 0);

			// (avoid return + escape keys getting processed by the buttons..)
			for (int i = getNumChildComponents(); --i >= 0;)
				getChildComponent (i)->setWantsKeyboardFocus (false);

			setWantsKeyboardFocus (true);
			grabKeyboardFocus();
		}

		bool keyPressed (const KeyPress& key)
		{
			lastPress = key;
			String message (TRANS("Key: ") + owner.getDescriptionForKeyPress (key));

			const CommandID previousCommand = owner.getMappings().findCommandForKeyPress (key);

			if (previousCommand != 0)
				message << "\n\n" << TRANS("(Currently assigned to \"")
						<< owner.getMappings().getCommandManager()->getNameOfCommand (previousCommand) << "\")";

			setMessage (message);
			return true;
		}

		bool keyStateChanged (bool)
		{
			return true;
		}

		KeyPress lastPress;

	private:
		KeyMappingEditorComponent& owner;

		JUCE_DECLARE_NON_COPYABLE (KeyEntryWindow);
	};

	static void assignNewKeyCallback (int result, ChangeKeyButton* button, KeyPress newKey)
	{
		if (result != 0 && button != nullptr)
			button->setNewKey (newKey, true);
	}

	void setNewKey (const KeyPress& newKey, bool dontAskUser)
	{
		if (newKey.isValid())
		{
			const CommandID previousCommand = owner.getMappings().findCommandForKeyPress (newKey);

			if (previousCommand == 0 || dontAskUser)
			{
				owner.getMappings().removeKeyPress (newKey);

				if (keyNum >= 0)
					owner.getMappings().removeKeyPress (commandID, keyNum);

				owner.getMappings().addKeyPress (commandID, newKey, keyNum);
			}
			else
			{
				AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
											  TRANS("Change key-mapping"),
											  TRANS("This key is already assigned to the command \"")
												+ owner.getMappings().getCommandManager()->getNameOfCommand (previousCommand)
												+ TRANS("\"\n\nDo you want to re-assign it to this new command instead?"),
											  TRANS("Re-assign"),
											  TRANS("Cancel"),
											  this,
											  ModalCallbackFunction::forComponent (assignNewKeyCallback,
																				   this, KeyPress (newKey)));
			}
		}
	}

	static void keyChosen (int result, ChangeKeyButton* button)
	{
		if (result != 0 && button != nullptr && button->currentKeyEntryWindow != nullptr)
		{
			button->currentKeyEntryWindow->setVisible (false);
			button->setNewKey (button->currentKeyEntryWindow->lastPress, false);
		}

		button->currentKeyEntryWindow = nullptr;
	}

	void assignNewKey()
	{
		currentKeyEntryWindow = new KeyEntryWindow (owner);
		currentKeyEntryWindow->enterModalState (true, ModalCallbackFunction::forComponent (keyChosen, this));
	}

private:
	KeyMappingEditorComponent& owner;
	const CommandID commandID;
	const int keyNum;
	ScopedPointer<KeyEntryWindow> currentKeyEntryWindow;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChangeKeyButton);
};

class KeyMappingEditorComponent::ItemComponent  : public Component
{
public:
	ItemComponent (KeyMappingEditorComponent& owner_, const CommandID commandID_)
		: owner (owner_), commandID (commandID_)
	{
		setInterceptsMouseClicks (false, true);

		const bool isReadOnly = owner.isCommandReadOnly (commandID);

		const Array <KeyPress> keyPresses (owner.getMappings().getKeyPressesAssignedToCommand (commandID));

		for (int i = 0; i < jmin ((int) maxNumAssignments, keyPresses.size()); ++i)
			addKeyPressButton (owner.getDescriptionForKeyPress (keyPresses.getReference (i)), i, isReadOnly);

		addKeyPressButton (String::empty, -1, isReadOnly);
	}

	void addKeyPressButton (const String& desc, const int index, const bool isReadOnly)
	{
		ChangeKeyButton* const b = new ChangeKeyButton (owner, commandID, desc, index);
		keyChangeButtons.add (b);

		b->setEnabled (! isReadOnly);
		b->setVisible (keyChangeButtons.size() <= (int) maxNumAssignments);
		addChildComponent (b);
	}

	void paint (Graphics& g)
	{
		g.setFont (getHeight() * 0.7f);
		g.setColour (findColour (KeyMappingEditorComponent::textColourId));

		g.drawFittedText (owner.getMappings().getCommandManager()->getNameOfCommand (commandID),
						  4, 0, jmax (40, getChildComponent (0)->getX() - 5), getHeight(),
						  Justification::centredLeft, true);
	}

	void resized()
	{
		int x = getWidth() - 4;

		for (int i = keyChangeButtons.size(); --i >= 0;)
		{
			ChangeKeyButton* const b = keyChangeButtons.getUnchecked(i);

			b->fitToContent (getHeight() - 2);
			b->setTopRightPosition (x, 1);
			x = b->getX() - 5;
		}
	}

private:
	KeyMappingEditorComponent& owner;
	OwnedArray<ChangeKeyButton> keyChangeButtons;
	const CommandID commandID;

	enum { maxNumAssignments = 3 };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ItemComponent);
};

class KeyMappingEditorComponent::MappingItem  : public TreeViewItem
{
public:
	MappingItem (KeyMappingEditorComponent& owner_, const CommandID commandID_)
		: owner (owner_), commandID (commandID_)
	{
	}

	String getUniqueName() const                { return String ((int) commandID) + "_id"; }
	bool mightContainSubItems()                 { return false; }
	int getItemHeight() const                   { return 20; }

	Component* createItemComponent()
	{
		return new ItemComponent (owner, commandID);
	}

private:
	KeyMappingEditorComponent& owner;
	const CommandID commandID;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MappingItem);
};

class KeyMappingEditorComponent::CategoryItem  : public TreeViewItem
{
public:
	CategoryItem (KeyMappingEditorComponent& owner_, const String& name)
		: owner (owner_), categoryName (name)
	{
	}

	String getUniqueName() const                { return categoryName + "_cat"; }
	bool mightContainSubItems()                 { return true; }
	int getItemHeight() const                   { return 28; }

	void paintItem (Graphics& g, int width, int height)
	{
		g.setFont (height * 0.6f, Font::bold);
		g.setColour (owner.findColour (KeyMappingEditorComponent::textColourId));

		g.drawText (categoryName,
					2, 0, width - 2, height,
					Justification::centredLeft, true);
	}

	void itemOpennessChanged (bool isNowOpen)
	{
		if (isNowOpen)
		{
			if (getNumSubItems() == 0)
			{
				Array <CommandID> commands (owner.getMappings().getCommandManager()->getCommandsInCategory (categoryName));

				for (int i = 0; i < commands.size(); ++i)
				{
					if (owner.shouldCommandBeIncluded (commands[i]))
						addSubItem (new MappingItem (owner, commands[i]));
				}
			}
		}
		else
		{
			clearSubItems();
		}
	}

private:
	KeyMappingEditorComponent& owner;
	String categoryName;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CategoryItem);
};

class KeyMappingEditorComponent::TopLevelItem   : public TreeViewItem,
												  public ChangeListener,
												  public ButtonListener
{
public:
	TopLevelItem (KeyMappingEditorComponent& owner_)
		: owner (owner_)
	{
		setLinesDrawnForSubItems (false);
		owner.getMappings().addChangeListener (this);
	}

	~TopLevelItem()
	{
		owner.getMappings().removeChangeListener (this);
	}

	bool mightContainSubItems()             { return true; }
	String getUniqueName() const            { return "keys"; }

	void changeListenerCallback (ChangeBroadcaster*)
	{
		const OpennessRestorer opennessRestorer (*this);
		clearSubItems();

		const StringArray categories (owner.getMappings().getCommandManager()->getCommandCategories());

		for (int i = 0; i < categories.size(); ++i)
		{
			const Array <CommandID> commands (owner.getMappings().getCommandManager()->getCommandsInCategory (categories[i]));
			int count = 0;

			for (int j = 0; j < commands.size(); ++j)
				if (owner.shouldCommandBeIncluded (commands[j]))
					++count;

			if (count > 0)
				addSubItem (new CategoryItem (owner, categories[i]));
		}
	}

	static void resetToDefaultsCallback (int result, KeyMappingEditorComponent* owner)
	{
		if (result != 0 && owner != nullptr)
			owner->getMappings().resetToDefaultMappings();
	}

	void buttonClicked (Button*)
	{
		AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon,
									  TRANS("Reset to defaults"),
									  TRANS("Are you sure you want to reset all the key-mappings to their default state?"),
									  TRANS("Reset"),
									  String::empty,
									  &owner,
									  ModalCallbackFunction::forComponent (resetToDefaultsCallback, &owner));
	}

private:
	KeyMappingEditorComponent& owner;
};

KeyMappingEditorComponent::KeyMappingEditorComponent (KeyPressMappingSet& mappingManager,
													  const bool showResetToDefaultButton)
	: mappings (mappingManager),
	  resetButton (TRANS ("reset to defaults"))
{
	treeItem = new TopLevelItem (*this);

	if (showResetToDefaultButton)
	{
		addAndMakeVisible (&resetButton);
		resetButton.addListener (treeItem);
	}

	addAndMakeVisible (&tree);
	tree.setColour (TreeView::backgroundColourId, findColour (backgroundColourId));
	tree.setRootItemVisible (false);
	tree.setDefaultOpenness (true);
	tree.setRootItem (treeItem);
}

KeyMappingEditorComponent::~KeyMappingEditorComponent()
{
	tree.setRootItem (nullptr);
}

void KeyMappingEditorComponent::setColours (const Colour& mainBackground,
											const Colour& textColour)
{
	setColour (backgroundColourId, mainBackground);
	setColour (textColourId, textColour);
	tree.setColour (TreeView::backgroundColourId, mainBackground);
}

void KeyMappingEditorComponent::parentHierarchyChanged()
{
	treeItem->changeListenerCallback (nullptr);
}

void KeyMappingEditorComponent::resized()
{
	int h = getHeight();

	if (resetButton.isVisible())
	{
		const int buttonHeight = 20;
		h -= buttonHeight + 8;
		int x = getWidth() - 8;

		resetButton.changeWidthToFitText (buttonHeight);
		resetButton.setTopRightPosition (x, h + 6);
	}

	tree.setBounds (0, 0, getWidth(), h);
}

bool KeyMappingEditorComponent::shouldCommandBeIncluded (const CommandID commandID)
{
	const ApplicationCommandInfo* const ci = mappings.getCommandManager()->getCommandForID (commandID);

	return ci != nullptr && (ci->flags & ApplicationCommandInfo::hiddenFromKeyEditor) == 0;
}

bool KeyMappingEditorComponent::isCommandReadOnly (const CommandID commandID)
{
	const ApplicationCommandInfo* const ci = mappings.getCommandManager()->getCommandForID (commandID);

	return ci != nullptr && (ci->flags & ApplicationCommandInfo::readOnlyInKeyEditor) != 0;
}

String KeyMappingEditorComponent::getDescriptionForKeyPress (const KeyPress& key)
{
	return key.getTextDescription();
}

/*** End of inlined file: juce_KeyMappingEditorComponent.cpp ***/


/*** Start of inlined file: juce_PreferencesPanel.cpp ***/
PreferencesPanel::PreferencesPanel()
	: buttonSize (70)
{
}

PreferencesPanel::~PreferencesPanel()
{
}

int PreferencesPanel::getButtonSize() const noexcept
{
	return buttonSize;
}

void PreferencesPanel::setButtonSize (int newSize)
{
	buttonSize = newSize;
	resized();
}

void PreferencesPanel::addSettingsPage (const String& title,
										const Drawable* icon,
										const Drawable* overIcon,
										const Drawable* downIcon)
{
	DrawableButton* const button = new DrawableButton (title, DrawableButton::ImageAboveTextLabel);
	buttons.add (button);

	button->setImages (icon, overIcon, downIcon);
	button->setRadioGroupId (1);
	button->addListener (this);
	button->setClickingTogglesState (true);
	button->setWantsKeyboardFocus (false);
	addAndMakeVisible (button);

	resized();

	if (currentPage == nullptr)
		setCurrentPage (title);
}

void PreferencesPanel::addSettingsPage (const String& title, const void* imageData, const int imageDataSize)
{
	DrawableImage icon, iconOver, iconDown;
	icon.setImage (ImageCache::getFromMemory (imageData, imageDataSize));

	iconOver.setImage (ImageCache::getFromMemory (imageData, imageDataSize));
	iconOver.setOverlayColour (Colours::black.withAlpha (0.12f));

	iconDown.setImage (ImageCache::getFromMemory (imageData, imageDataSize));
	iconDown.setOverlayColour (Colours::black.withAlpha (0.25f));

	addSettingsPage (title, &icon, &iconOver, &iconDown);
}

void PreferencesPanel::showInDialogBox (const String& dialogTitle, int dialogWidth, int dialogHeight, const Colour& backgroundColour)
{
	setSize (dialogWidth, dialogHeight);
	DialogWindow::showDialog (dialogTitle, this, 0, backgroundColour, false);
}

void PreferencesPanel::resized()
{
	for (int i = 0; i < buttons.size(); ++i)
		buttons.getUnchecked(i)->setBounds (i * buttonSize, 0, buttonSize, buttonSize);

	if (currentPage != nullptr)
		currentPage->setBounds (getLocalBounds().withTop (buttonSize + 5));
}

void PreferencesPanel::paint (Graphics& g)
{
	g.setColour (Colours::grey);
	g.fillRect (0, buttonSize + 2, getWidth(), 1);
}

void PreferencesPanel::setCurrentPage (const String& pageName)
{
	if (currentPageName != pageName)
	{
		currentPageName = pageName;

		currentPage = nullptr;
		currentPage = createComponentForPage (pageName);

		if (currentPage != nullptr)
		{
			addAndMakeVisible (currentPage);
			currentPage->toBack();
			resized();
		}

		for (int i = 0; i < buttons.size(); ++i)
		{
			if (buttons.getUnchecked(i)->getName() == pageName)
			{
				buttons.getUnchecked(i)->setToggleState (true, false);
				break;
			}
		}
	}
}

void PreferencesPanel::buttonClicked (Button*)
{
	for (int i = 0; i < buttons.size(); ++i)
	{
		if (buttons.getUnchecked(i)->getToggleState())
		{
			setCurrentPage (buttons.getUnchecked(i)->getName());
			break;
		}
	}
}

/*** End of inlined file: juce_PreferencesPanel.cpp ***/


/*** Start of inlined file: juce_RecentlyOpenedFilesList.cpp ***/
RecentlyOpenedFilesList::RecentlyOpenedFilesList()
	: maxNumberOfItems (10)
{
}

RecentlyOpenedFilesList::~RecentlyOpenedFilesList()
{
}

void RecentlyOpenedFilesList::setMaxNumberOfItems (const int newMaxNumber)
{
	maxNumberOfItems = jmax (1, newMaxNumber);

	files.removeRange (maxNumberOfItems, getNumFiles());
}

int RecentlyOpenedFilesList::getNumFiles() const
{
	return files.size();
}

File RecentlyOpenedFilesList::getFile (const int index) const
{
	return File (files [index]);
}

void RecentlyOpenedFilesList::clear()
{
	files.clear();
}

void RecentlyOpenedFilesList::addFile (const File& file)
{
	removeFile (file);
	files.insert (0, file.getFullPathName());

	setMaxNumberOfItems (maxNumberOfItems);
}

void RecentlyOpenedFilesList::removeFile (const File& file)
{
	files.removeString (file.getFullPathName());
}

void RecentlyOpenedFilesList::removeNonExistentFiles()
{
	for (int i = getNumFiles(); --i >= 0;)
		if (! getFile(i).exists())
			files.remove (i);
}

int RecentlyOpenedFilesList::createPopupMenuItems (PopupMenu& menuToAddTo,
												   const int baseItemId,
												   const bool showFullPaths,
												   const bool dontAddNonExistentFiles,
												   const File** filesToAvoid)
{
	int num = 0;

	for (int i = 0; i < getNumFiles(); ++i)
	{
		const File f (getFile(i));

		if ((! dontAddNonExistentFiles) || f.exists())
		{
			bool needsAvoiding = false;

			if (filesToAvoid != nullptr)
			{
				for (const File** avoid = filesToAvoid; *avoid != nullptr; ++avoid)
				{
					if (f == **avoid)
					{
						needsAvoiding = true;
						break;
					}
				}
			}

			if (! needsAvoiding)
			{
				menuToAddTo.addItem (baseItemId + i,
									 showFullPaths ? f.getFullPathName()
												   : f.getFileName());
				++num;
			}
		}
	}

	return num;
}

String RecentlyOpenedFilesList::toString() const
{
	return files.joinIntoString ("\n");
}

void RecentlyOpenedFilesList::restoreFromString (const String& stringifiedVersion)
{
	clear();
	files.addLines (stringifiedVersion);

	setMaxNumberOfItems (maxNumberOfItems);
}

/*** End of inlined file: juce_RecentlyOpenedFilesList.cpp ***/


/*** Start of inlined file: juce_SplashScreen.cpp ***/
SplashScreen::SplashScreen()
{
	setOpaque (true);
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::show (const String& title,
						 const Image& backgroundImage_,
						 const int minimumTimeToDisplayFor,
						 const bool useDropShadow,
						 const bool removeOnMouseClick)
{
	backgroundImage = backgroundImage_;

	jassert (backgroundImage_.isValid());

	if (backgroundImage_.isValid())
	{
		setOpaque (! backgroundImage_.hasAlphaChannel());

		show (title,
			  backgroundImage_.getWidth(),
			  backgroundImage_.getHeight(),
			  minimumTimeToDisplayFor,
			  useDropShadow,
			  removeOnMouseClick);
	}
}

void SplashScreen::show (const String& title,
						 const int width,
						 const int height,
						 const int minimumTimeToDisplayFor,
						 const bool useDropShadow,
						 const bool removeOnMouseClick)
{
	setName (title);
	setAlwaysOnTop (true);
	setVisible (true);
	centreWithSize (width, height);

	addToDesktop (useDropShadow ? ComponentPeer::windowHasDropShadow : 0);
	toFront (false);

   #if JUCE_MODAL_LOOPS_PERMITTED
	MessageManager::getInstance()->runDispatchLoopUntil (300);
   #endif

	repaint();

	originalClickCounter = removeOnMouseClick
								? Desktop::getMouseButtonClickCounter()
								: std::numeric_limits<int>::max();

	earliestTimeToDelete = Time::getCurrentTime() + RelativeTime::milliseconds (minimumTimeToDisplayFor);

	startTimer (50);
}

void SplashScreen::paint (Graphics& g)
{
	g.setOpacity (1.0f);

	g.drawImage (backgroundImage,
				 0, 0, getWidth(), getHeight(),
				 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
}

void SplashScreen::timerCallback()
{
	if (Time::getCurrentTime() > earliestTimeToDelete
		 || Desktop::getMouseButtonClickCounter() > originalClickCounter)
	{
		delete this;
	}
}

/*** End of inlined file: juce_SplashScreen.cpp ***/


/*** Start of inlined file: juce_SystemTrayIconComponent.cpp ***/
#if JUCE_WINDOWS || JUCE_LINUX

SystemTrayIconComponent::SystemTrayIconComponent()
{
	addToDesktop (0);
}

SystemTrayIconComponent::~SystemTrayIconComponent()
{
}

#endif

/*** End of inlined file: juce_SystemTrayIconComponent.cpp ***/

// END_AUTOINCLUDE

}

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

/*** Start of inlined file: juce_mac_NSViewComponent.mm ***/
class NSViewAttachment  : public ReferenceCountedObject,
						  public ComponentMovementWatcher
{
public:
	NSViewAttachment (NSView* const view_, Component& owner_)
		: ComponentMovementWatcher (&owner_),
		  view (view_),
		  owner (owner_),
		  currentPeer (nullptr)
	{
		[view_ retain];

		if (owner.isShowing())
			componentPeerChanged();
	}

	~NSViewAttachment()
	{
		removeFromParent();
		[view release];
	}

	void componentMovedOrResized (Component& comp, bool wasMoved, bool wasResized)
	{
		ComponentMovementWatcher::componentMovedOrResized (comp, wasMoved, wasResized);

		// The ComponentMovementWatcher version of this method avoids calling
		// us when the top-level comp is resized, but for an NSView we need to know this
		// because with inverted co-ords, we need to update the position even if the
		// top-left pos hasn't changed
		if (comp.isOnDesktop() && wasResized)
			componentMovedOrResized (wasMoved, wasResized);
	}

	void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/)
	{
		Component* const topComp = owner.getTopLevelComponent();

		if (topComp->getPeer() != nullptr)
		{
			const Rectangle<int> area (topComp->getLocalArea (&owner, owner.getLocalBounds()));

			NSRect r = NSMakeRect ((float) area.getX(), (float) area.getY(), (float) area.getWidth(), (float) area.getHeight());
			r.origin.y = [[view superview] frame].size.height - (r.origin.y + r.size.height);
			[view setFrame: r];
		}
	}

	void componentPeerChanged()
	{
		ComponentPeer* const peer = owner.getPeer();

		if (currentPeer != peer)
		{
			removeFromParent();
			currentPeer = peer;

			if (peer != nullptr)
			{
				NSView* const peerView = (NSView*) peer->getNativeHandle();
				[peerView addSubview: view];
				componentMovedOrResized (false, false);
			}
		}

		[view setHidden: ! owner.isShowing()];
	}

	void componentVisibilityChanged()
	{
		componentPeerChanged();
	}

	NSView* const view;

private:
	Component& owner;
	ComponentPeer* currentPeer;

	void removeFromParent()
	{
		if ([view superview] != nil)
			[view removeFromSuperview]; // Must be careful not to call this unless it's required - e.g. some Apple AU views
										// override the call and use it as a sign that they're being deleted, which breaks everything..
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NSViewAttachment);
};

NSViewComponent::NSViewComponent() {}
NSViewComponent::~NSViewComponent() {}

void NSViewComponent::setView (void* const view)
{
	if (view != getView())
	{
		attachment = nullptr;

		if (view != nullptr)
			attachment = attachViewToComponent (*this, view);
	}
}

void* NSViewComponent::getView() const
{
	return attachment != nullptr ? static_cast <NSViewAttachment*> (attachment.getObject())->view
								 : nullptr;
}

void NSViewComponent::resizeToFitView()
{
	if (attachment != nullptr)
	{
		NSRect r = [static_cast <NSViewAttachment*> (attachment.getObject())->view frame];
		setBounds (Rectangle<int> ((int) r.size.width, (int) r.size.height));
	}
}

void NSViewComponent::paint (Graphics&) {}

ReferenceCountedObject* NSViewComponent::attachViewToComponent (Component& comp, void* const view)
{
	return new NSViewAttachment ((NSView*) view, comp);
}

/*** End of inlined file: juce_mac_NSViewComponent.mm ***/



/*** Start of inlined file: juce_mac_AppleRemote.mm ***/
AppleRemoteDevice::AppleRemoteDevice()
	: device (0),
	  queue (0),
	  remoteId (0)
{
}

AppleRemoteDevice::~AppleRemoteDevice()
{
	stop();
}

namespace
{
	io_object_t getAppleRemoteDevice()
	{
		CFMutableDictionaryRef dict = IOServiceMatching ("AppleIRController");

		io_iterator_t iter = 0;
		io_object_t iod = 0;

		if (IOServiceGetMatchingServices (kIOMasterPortDefault, dict, &iter) == kIOReturnSuccess
			 && iter != 0)
		{
			iod = IOIteratorNext (iter);
		}

		IOObjectRelease (iter);
		return iod;
	}

	bool createAppleRemoteInterface (io_object_t iod, void** device)
	{
		jassert (*device == nullptr);
		io_name_t classname;

		if (IOObjectGetClass (iod, classname) == kIOReturnSuccess)
		{
			IOCFPlugInInterface** cfPlugInInterface = nullptr;
			SInt32 score = 0;

			if (IOCreatePlugInInterfaceForService (iod,
												   kIOHIDDeviceUserClientTypeID,
												   kIOCFPlugInInterfaceID,
												   &cfPlugInInterface,
												   &score) == kIOReturnSuccess)
			{
				HRESULT hr = (*cfPlugInInterface)->QueryInterface (cfPlugInInterface,
																   CFUUIDGetUUIDBytes (kIOHIDDeviceInterfaceID),
																   device);

				(void) hr;

				(*cfPlugInInterface)->Release (cfPlugInInterface);
			}
		}

		return *device != 0;
	}

	void appleRemoteQueueCallback (void* const target, const IOReturn result, void*, void*)
	{
		if (result == kIOReturnSuccess)
			((AppleRemoteDevice*) target)->handleCallbackInternal();
	}
}

bool AppleRemoteDevice::start (const bool inExclusiveMode)
{
	if (queue != 0)
		return true;

	stop();

	bool result = false;
	io_object_t iod = getAppleRemoteDevice();

	if (iod != 0)
	{
		if (createAppleRemoteInterface (iod, &device) && open (inExclusiveMode))
			result = true;
		else
			stop();

		IOObjectRelease (iod);
	}

	return result;
}

void AppleRemoteDevice::stop()
{
	if (queue != 0)
	{
		(*(IOHIDQueueInterface**) queue)->stop ((IOHIDQueueInterface**) queue);
		(*(IOHIDQueueInterface**) queue)->dispose ((IOHIDQueueInterface**) queue);
		(*(IOHIDQueueInterface**) queue)->Release ((IOHIDQueueInterface**) queue);
		queue = 0;
	}

	if (device != 0)
	{
		(*(IOHIDDeviceInterface**) device)->close ((IOHIDDeviceInterface**) device);
		(*(IOHIDDeviceInterface**) device)->Release ((IOHIDDeviceInterface**) device);
		device = 0;
	}
}

bool AppleRemoteDevice::isActive() const
{
	return queue != 0;
}

bool AppleRemoteDevice::open (const bool openInExclusiveMode)
{
	Array <int> cookies;

	CFArrayRef elements;
	IOHIDDeviceInterface122** const device122 = (IOHIDDeviceInterface122**) device;

	if ((*device122)->copyMatchingElements (device122, 0, &elements) != kIOReturnSuccess)
		return false;

	for (int i = 0; i < CFArrayGetCount (elements); ++i)
	{
		CFDictionaryRef element = (CFDictionaryRef) CFArrayGetValueAtIndex (elements, i);

		// get the cookie
		CFTypeRef object = CFDictionaryGetValue (element, CFSTR (kIOHIDElementCookieKey));

		if (object == 0 || CFGetTypeID (object) != CFNumberGetTypeID())
			continue;

		long number;
		if (! CFNumberGetValue ((CFNumberRef) object, kCFNumberLongType, &number))
			continue;

		cookies.add ((int) number);
	}

	CFRelease (elements);

	if ((*(IOHIDDeviceInterface**) device)
			->open ((IOHIDDeviceInterface**) device,
					openInExclusiveMode ? kIOHIDOptionsTypeSeizeDevice
										: kIOHIDOptionsTypeNone) == KERN_SUCCESS)
	{
		queue = (*(IOHIDDeviceInterface**) device)->allocQueue ((IOHIDDeviceInterface**) device);

		if (queue != 0)
		{
			(*(IOHIDQueueInterface**) queue)->create ((IOHIDQueueInterface**) queue, 0, 12);

			for (int i = 0; i < cookies.size(); ++i)
			{
				IOHIDElementCookie cookie = (IOHIDElementCookie) cookies.getUnchecked(i);
				(*(IOHIDQueueInterface**) queue)->addElement ((IOHIDQueueInterface**) queue, cookie, 0);
			}

			CFRunLoopSourceRef eventSource;

			if ((*(IOHIDQueueInterface**) queue)
					->createAsyncEventSource ((IOHIDQueueInterface**) queue, &eventSource) == KERN_SUCCESS)
			{
				if ((*(IOHIDQueueInterface**) queue)->setEventCallout ((IOHIDQueueInterface**) queue,
																	   appleRemoteQueueCallback, this, 0) == KERN_SUCCESS)
				{
					CFRunLoopAddSource (CFRunLoopGetCurrent(), eventSource, kCFRunLoopDefaultMode);

					(*(IOHIDQueueInterface**) queue)->start ((IOHIDQueueInterface**) queue);

					return true;
				}
			}
		}
	}

	return false;
}

void AppleRemoteDevice::handleCallbackInternal()
{
	int totalValues = 0;
	AbsoluteTime nullTime = { 0, 0 };
	char cookies [12];
	int numCookies = 0;

	while (numCookies < numElementsInArray (cookies))
	{
		IOHIDEventStruct e;

		if ((*(IOHIDQueueInterface**) queue)->getNextEvent ((IOHIDQueueInterface**) queue, &e, nullTime, 0) != kIOReturnSuccess)
			break;

		if ((int) e.elementCookie == 19)
		{
			remoteId = e.value;
			buttonPressed (switched, false);
		}
		else
		{
			totalValues += e.value;
			cookies [numCookies++] = (char) (pointer_sized_int) e.elementCookie;
		}
	}

	cookies [numCookies++] = 0;
	//DBG (String::toHexString ((uint8*) cookies, numCookies, 1) + " "  + String (totalValues));

	static const char buttonPatterns[] =
	{
		0x1f, 0x14, 0x12, 0x1f, 0x14, 0x12, 0,
		0x1f, 0x15, 0x12, 0x1f, 0x15, 0x12, 0,
		0x1f, 0x1d, 0x1c, 0x12, 0,
		0x1f, 0x1e, 0x1c, 0x12, 0,
		0x1f, 0x16, 0x12, 0x1f, 0x16, 0x12, 0,
		0x1f, 0x17, 0x12, 0x1f, 0x17, 0x12, 0,
		0x1f, 0x12, 0x04, 0x02, 0,
		0x1f, 0x12, 0x03, 0x02, 0,
		0x1f, 0x12, 0x1f, 0x12, 0,
		0x23, 0x1f, 0x12, 0x23, 0x1f, 0x12, 0,
		19, 0
	};

	int buttonNum = (int) menuButton;
	int i = 0;

	while (i < numElementsInArray (buttonPatterns))
	{
		if (strcmp (cookies, buttonPatterns + i) == 0)
		{
			buttonPressed ((ButtonType) buttonNum, totalValues > 0);
			break;
		}

		i += (int) strlen (buttonPatterns + i) + 1;
		++buttonNum;
	}
}

/*** End of inlined file: juce_mac_AppleRemote.mm ***/

 #endif

 #if JUCE_IOS

/*** Start of inlined file: juce_ios_UIViewComponent.mm ***/
class UIViewComponent::Pimpl  : public ComponentMovementWatcher
{
public:
	Pimpl (UIView* const view_, Component& owner_)
		: ComponentMovementWatcher (&owner_),
		  view (view_),
		  owner (owner_),
		  currentPeer (nullptr)
	{
		[view_ retain];

		if (owner.isShowing())
			componentPeerChanged();
	}

	~Pimpl()
	{
		[view removeFromSuperview];
		[view release];
	}

	void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/)
	{
		Component* const topComp = owner.getTopLevelComponent();

		if (topComp->getPeer() != nullptr)
		{
			const Point<int> pos (topComp->getLocalPoint (&owner, Point<int>()));

			[view setFrame: CGRectMake ((float) pos.getX(), (float) pos.getY(),
										(float) owner.getWidth(), (float) owner.getHeight())];
		}
	}

	void componentPeerChanged()
	{
		ComponentPeer* const peer = owner.getPeer();

		if (currentPeer != peer)
		{
			if ([view superview] != nil)
				[view removeFromSuperview]; // Must be careful not to call this unless it's required - e.g. some Apple AU views
											// override the call and use it as a sign that they're being deleted, which breaks everything..
			currentPeer = peer;

			if (peer != nullptr)
			{
				UIView* peerView = (UIView*) peer->getNativeHandle();
				[peerView addSubview: view];
				componentMovedOrResized (false, false);
			}
		}

		[view setHidden: ! owner.isShowing()];
	 }

	void componentVisibilityChanged()
	{
		componentPeerChanged();
	}

	Rectangle<int> getViewBounds() const
	{
		CGRect r = [view frame];
		return Rectangle<int> ((int) r.size.width, (int) r.size.height);
	}

	UIView* const view;

private:
	Component& owner;
	ComponentPeer* currentPeer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pimpl);
};

UIViewComponent::UIViewComponent() {}
UIViewComponent::~UIViewComponent() {}

void UIViewComponent::setView (void* const view)
{
	if (view != getView())
	{
		pimpl = nullptr;

		if (view != nullptr)
			pimpl = new Pimpl ((UIView*) view, *this);
	}
}

void* UIViewComponent::getView() const
{
	return pimpl == nullptr ? nullptr : pimpl->view;
}

void UIViewComponent::resizeToFitView()
{
	if (pimpl != nullptr)
		setBounds (pimpl->getViewBounds());
}

void UIViewComponent::paint (Graphics&) {}

/*** End of inlined file: juce_ios_UIViewComponent.mm ***/


 #endif

 #if JUCE_WEB_BROWSER

/*** Start of inlined file: juce_mac_WebBrowserComponent.mm ***/
} // (juce namespace)

class WebBrowserComponentInternal;

#if JUCE_MAC

#define DownloadClickDetector MakeObjCClassName(DownloadClickDetector)

@interface DownloadClickDetector   : NSObject
{
	juce::WebBrowserComponent* ownerComponent;
}

- (DownloadClickDetector*) initWithWebBrowserOwner: (juce::WebBrowserComponent*) ownerComponent;

- (void) webView: (WebView*) webView decidePolicyForNavigationAction: (NSDictionary*) actionInformation
															 request: (NSURLRequest*) request
															   frame: (WebFrame*) frame
													decisionListener: (id <WebPolicyDecisionListener>) listener;
- (void) webView: (WebView*) webView didFinishLoadForFrame: (WebFrame*) frame;

@end

@implementation DownloadClickDetector

- (DownloadClickDetector*) initWithWebBrowserOwner: (juce::WebBrowserComponent*) ownerComponent_
{
	[super init];
	ownerComponent = ownerComponent_;
	return self;
}

- (void) webView: (WebView*) sender decidePolicyForNavigationAction: (NSDictionary*) actionInformation
															request: (NSURLRequest*) request
															  frame: (WebFrame*) frame
												   decisionListener: (id <WebPolicyDecisionListener>) listener
{
	(void) sender; (void) request; (void) frame;

	NSURL* url = [actionInformation valueForKey: nsStringLiteral ("WebActionOriginalURLKey")];

	if (ownerComponent->pageAboutToLoad (nsStringToJuce ([url absoluteString])))
		[listener use];
	else
		[listener ignore];
}

- (void) webView: (WebView*) sender didFinishLoadForFrame: (WebFrame*) frame
{
	if ([frame isEqual: [sender mainFrame]])
	{
		NSURL* url = [[[frame dataSource] request] URL];
		ownerComponent->pageFinishedLoading (nsStringToJuce ([url absoluteString]));
	}
}

@end

#else

@interface WebViewTapDetector  : NSObject <UIGestureRecognizerDelegate>
{
}

- (BOOL) gestureRecognizer: (UIGestureRecognizer*) gestureRecognizer
		 shouldRecognizeSimultaneouslyWithGestureRecognizer: (UIGestureRecognizer*) otherGestureRecognizer;
@end

@implementation WebViewTapDetector

- (BOOL) gestureRecognizer: (UIGestureRecognizer*) gestureRecognizer
		 shouldRecognizeSimultaneouslyWithGestureRecognizer: (UIGestureRecognizer*) otherGestureRecognizer
{
	return YES;
}

@end

@interface WebViewURLChangeDetector : NSObject <UIWebViewDelegate>
{
	juce::WebBrowserComponent* ownerComponent;
}

- (WebViewURLChangeDetector*) initWithWebBrowserOwner: (juce::WebBrowserComponent*) ownerComponent;
- (BOOL) webView: (UIWebView*) webView shouldStartLoadWithRequest: (NSURLRequest*) request navigationType: (UIWebViewNavigationType) navigationType;
@end

@implementation WebViewURLChangeDetector

- (WebViewURLChangeDetector*) initWithWebBrowserOwner: (juce::WebBrowserComponent*) ownerComponent_
{
	[super init];
	ownerComponent = ownerComponent_;
	return self;
}

- (BOOL) webView: (UIWebView*) webView shouldStartLoadWithRequest: (NSURLRequest*) request navigationType: (UIWebViewNavigationType) navigationType
{
	return ownerComponent->pageAboutToLoad (nsStringToJuce (request.URL.absoluteString));
}
@end
#endif

namespace juce
{

class WebBrowserComponentInternal
								   #if JUCE_MAC
									: public NSViewComponent
								   #else
									: public UIViewComponent
								   #endif
{
public:
	WebBrowserComponentInternal (WebBrowserComponent* owner)
	{
	   #if JUCE_MAC
		webView = [[WebView alloc] initWithFrame: NSMakeRect (0, 0, 100.0f, 100.0f)
									   frameName: nsEmptyString()
									   groupName: nsEmptyString()];
		setView (webView);

		clickListener = [[DownloadClickDetector alloc] initWithWebBrowserOwner: owner];
		[webView setPolicyDelegate: clickListener];
		[webView setFrameLoadDelegate: clickListener];
	   #else
		webView = [[UIWebView alloc] initWithFrame: CGRectMake (0, 0, 1.0f, 1.0f)];
		setView (webView);

		tapDetector = [[WebViewTapDetector alloc] init];
		urlDetector = [[WebViewURLChangeDetector alloc] initWithWebBrowserOwner: owner];
		gestureRecogniser = nil;
		webView.delegate = urlDetector;
	   #endif
	}

	~WebBrowserComponentInternal()
	{
	   #if JUCE_MAC
		[webView setPolicyDelegate: nil];
		[webView setFrameLoadDelegate: nil];
		[clickListener release];
	   #else
		webView.delegate = nil;
		[webView removeGestureRecognizer: gestureRecogniser];
		[gestureRecogniser release];
		[tapDetector release];
		[urlDetector release];
	   #endif

		setView (nil);
	}

	void goToURL (const String& url,
				  const StringArray* headers,
				  const MemoryBlock* postData)
	{
		NSMutableURLRequest* r
			= [NSMutableURLRequest requestWithURL: [NSURL URLWithString: juceStringToNS (url)]
									  cachePolicy: NSURLRequestUseProtocolCachePolicy
								  timeoutInterval: 30.0];

		if (postData != nullptr && postData->getSize() > 0)
		{
			[r setHTTPMethod: nsStringLiteral ("POST")];
			[r setHTTPBody: [NSData dataWithBytes: postData->getData()
										   length: postData->getSize()]];
		}

		if (headers != nullptr)
		{
			for (int i = 0; i < headers->size(); ++i)
			{
				const String headerName  ((*headers)[i].upToFirstOccurrenceOf (":", false, false).trim());
				const String headerValue ((*headers)[i].fromFirstOccurrenceOf (":", false, false).trim());

				[r setValue: juceStringToNS (headerValue)
				   forHTTPHeaderField: juceStringToNS (headerName)];
			}
		}

		stop();

	   #if JUCE_MAC
		[[webView mainFrame] loadRequest: r];
	   #else
		[webView loadRequest: r];
	   #endif
	}

	void goBack()       { [webView goBack]; }
	void goForward()    { [webView goForward]; }

   #if JUCE_MAC
	void stop()         { [webView stopLoading: nil]; }
	void refresh()      { [webView reload: nil]; }
   #else
	void stop()         { [webView stopLoading]; }
	void refresh()      { [webView reload]; }
   #endif

	void mouseMove (const MouseEvent&)
	{
		// WebKit doesn't capture mouse-moves itself, so it seems the only way to make
		// them work is to push them via this non-public method..
		if ([webView respondsToSelector: @selector (_updateMouseoverWithFakeEvent)])
			[webView performSelector:    @selector (_updateMouseoverWithFakeEvent)];
	}

private:
   #if JUCE_MAC
	WebView* webView;
	DownloadClickDetector* clickListener;
   #else
	UIWebView* webView;
	WebViewTapDetector* tapDetector;
	WebViewURLChangeDetector* urlDetector;
	UITapGestureRecognizer* gestureRecogniser;
   #endif
};

WebBrowserComponent::WebBrowserComponent (const bool unloadPageWhenBrowserIsHidden_)
	: browser (nullptr),
	  blankPageShown (false),
	  unloadPageWhenBrowserIsHidden (unloadPageWhenBrowserIsHidden_)
{
	setOpaque (true);

	addAndMakeVisible (browser = new WebBrowserComponentInternal (this));
}

WebBrowserComponent::~WebBrowserComponent()
{
	deleteAndZero (browser);
}

void WebBrowserComponent::goToURL (const String& url,
								   const StringArray* headers,
								   const MemoryBlock* postData)
{
	lastURL = url;

	lastHeaders.clear();
	if (headers != nullptr)
		lastHeaders = *headers;

	lastPostData.setSize (0);
	if (postData != nullptr)
		lastPostData = *postData;

	blankPageShown = false;

	browser->goToURL (url, headers, postData);
}

void WebBrowserComponent::stop()
{
	browser->stop();
}

void WebBrowserComponent::goBack()
{
	lastURL = String::empty;
	blankPageShown = false;
	browser->goBack();
}

void WebBrowserComponent::goForward()
{
	lastURL = String::empty;
	browser->goForward();
}

void WebBrowserComponent::refresh()
{
	browser->refresh();
}

void WebBrowserComponent::paint (Graphics&)
{
}

void WebBrowserComponent::checkWindowAssociation()
{
	if (isShowing())
	{
		if (blankPageShown)
			goBack();
	}
	else
	{
		if (unloadPageWhenBrowserIsHidden && ! blankPageShown)
		{
			// when the component becomes invisible, some stuff like flash
			// carries on playing audio, so we need to force it onto a blank
			// page to avoid this, (and send it back when it's made visible again).

			blankPageShown = true;
			browser->goToURL ("about:blank", 0, 0);
		}
	}
}

void WebBrowserComponent::reloadLastURL()
{
	if (lastURL.isNotEmpty())
	{
		goToURL (lastURL, &lastHeaders, &lastPostData);
		lastURL = String::empty;
	}
}

void WebBrowserComponent::parentHierarchyChanged()
{
	checkWindowAssociation();
}

void WebBrowserComponent::resized()
{
	browser->setSize (getWidth(), getHeight());
}

void WebBrowserComponent::visibilityChanged()
{
	checkWindowAssociation();
}

bool WebBrowserComponent::pageAboutToLoad (const String&)  { return true; }
void WebBrowserComponent::pageFinishedLoading (const String&) {}

/*** End of inlined file: juce_mac_WebBrowserComponent.mm ***/


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


/*** Start of inlined file: juce_win32_ActiveXComponent.cpp ***/
extern int64 getMouseEventTime();

namespace ActiveXHelpers
{

	class JuceIStorage   : public ComBaseClassHelper <IStorage>
	{
	public:
		JuceIStorage() {}

		JUCE_COMRESULT CreateStream (const WCHAR*, DWORD, DWORD, DWORD, IStream**)           { return E_NOTIMPL; }
		JUCE_COMRESULT OpenStream (const WCHAR*, void*, DWORD, DWORD, IStream**)             { return E_NOTIMPL; }
		JUCE_COMRESULT CreateStorage (const WCHAR*, DWORD, DWORD, DWORD, IStorage**)         { return E_NOTIMPL; }
		JUCE_COMRESULT OpenStorage (const WCHAR*, IStorage*, DWORD, SNB, DWORD, IStorage**)  { return E_NOTIMPL; }
		JUCE_COMRESULT CopyTo (DWORD, IID const*, SNB, IStorage*)                            { return E_NOTIMPL; }
		JUCE_COMRESULT MoveElementTo (const OLECHAR*,IStorage*, const OLECHAR*, DWORD)       { return E_NOTIMPL; }
		JUCE_COMRESULT Commit (DWORD)                                                        { return E_NOTIMPL; }
		JUCE_COMRESULT Revert()                                                              { return E_NOTIMPL; }
		JUCE_COMRESULT EnumElements (DWORD, void*, DWORD, IEnumSTATSTG**)                    { return E_NOTIMPL; }
		JUCE_COMRESULT DestroyElement (const OLECHAR*)                                       { return E_NOTIMPL; }
		JUCE_COMRESULT RenameElement (const WCHAR*, const WCHAR*)                            { return E_NOTIMPL; }
		JUCE_COMRESULT SetElementTimes (const WCHAR*, FILETIME const*, FILETIME const*, FILETIME const*)    { return E_NOTIMPL; }
		JUCE_COMRESULT SetClass (REFCLSID)                                                   { return S_OK; }
		JUCE_COMRESULT SetStateBits (DWORD, DWORD)                                           { return E_NOTIMPL; }
		JUCE_COMRESULT Stat (STATSTG*, DWORD)                                                { return E_NOTIMPL; }
	};

	class JuceOleInPlaceFrame   : public ComBaseClassHelper <IOleInPlaceFrame>
	{
	public:
		JuceOleInPlaceFrame (HWND window_)   : window (window_) {}

		JUCE_COMRESULT GetWindow (HWND* lphwnd)                      { *lphwnd = window; return S_OK; }
		JUCE_COMRESULT ContextSensitiveHelp (BOOL)                   { return E_NOTIMPL; }
		JUCE_COMRESULT GetBorder (LPRECT)                            { return E_NOTIMPL; }
		JUCE_COMRESULT RequestBorderSpace (LPCBORDERWIDTHS)          { return E_NOTIMPL; }
		JUCE_COMRESULT SetBorderSpace (LPCBORDERWIDTHS)              { return E_NOTIMPL; }
		JUCE_COMRESULT SetActiveObject (IOleInPlaceActiveObject*, LPCOLESTR)     { return S_OK; }
		JUCE_COMRESULT InsertMenus (HMENU, LPOLEMENUGROUPWIDTHS)     { return E_NOTIMPL; }
		JUCE_COMRESULT SetMenu (HMENU, HOLEMENU, HWND)               { return S_OK; }
		JUCE_COMRESULT RemoveMenus (HMENU)                           { return E_NOTIMPL; }
		JUCE_COMRESULT SetStatusText (LPCOLESTR)                     { return S_OK; }
		JUCE_COMRESULT EnableModeless (BOOL)                         { return S_OK; }
		JUCE_COMRESULT TranslateAccelerator (LPMSG, WORD)            { return E_NOTIMPL; }

	private:
		HWND window;
	};

	class JuceIOleInPlaceSite   : public ComBaseClassHelper <IOleInPlaceSite>
	{
	public:
		JuceIOleInPlaceSite (HWND window_)
			: window (window_),
			  frame (new JuceOleInPlaceFrame (window))
		{}

		~JuceIOleInPlaceSite()
		{
			frame->Release();
		}

		JUCE_COMRESULT GetWindow (HWND* lphwnd)      { *lphwnd = window; return S_OK; }
		JUCE_COMRESULT ContextSensitiveHelp (BOOL)   { return E_NOTIMPL; }
		JUCE_COMRESULT CanInPlaceActivate()          { return S_OK; }
		JUCE_COMRESULT OnInPlaceActivate()           { return S_OK; }
		JUCE_COMRESULT OnUIActivate()                { return S_OK; }

		JUCE_COMRESULT GetWindowContext (LPOLEINPLACEFRAME* lplpFrame, LPOLEINPLACEUIWINDOW* lplpDoc, LPRECT, LPRECT, LPOLEINPLACEFRAMEINFO lpFrameInfo)
		{
			/* Note: if you call AddRef on the frame here, then some types of object (e.g. web browser control) cause leaks..
			   If you don't call AddRef then others crash (e.g. QuickTime).. Bit of a catch-22, so letting it leak is probably preferable.
			*/
			if (lplpFrame != nullptr) { frame->AddRef(); *lplpFrame = frame; }
			if (lplpDoc != nullptr)   *lplpDoc = nullptr;
			lpFrameInfo->fMDIApp = FALSE;
			lpFrameInfo->hwndFrame = window;
			lpFrameInfo->haccel = 0;
			lpFrameInfo->cAccelEntries = 0;
			return S_OK;
		}

		JUCE_COMRESULT Scroll (SIZE)                 { return E_NOTIMPL; }
		JUCE_COMRESULT OnUIDeactivate (BOOL)         { return S_OK; }
		JUCE_COMRESULT OnInPlaceDeactivate()         { return S_OK; }
		JUCE_COMRESULT DiscardUndoState()            { return E_NOTIMPL; }
		JUCE_COMRESULT DeactivateAndUndo()           { return E_NOTIMPL; }
		JUCE_COMRESULT OnPosRectChange (LPCRECT)     { return S_OK; }

	private:
		HWND window;
		JuceOleInPlaceFrame* frame;
	};

	class JuceIOleClientSite  : public ComBaseClassHelper <IOleClientSite>
	{
	public:
		JuceIOleClientSite (HWND window)
			: inplaceSite (new JuceIOleInPlaceSite (window))
		{}

		~JuceIOleClientSite()
		{
			inplaceSite->Release();
		}

		JUCE_COMRESULT QueryInterface (REFIID type, void** result)
		{
			if (type == IID_IOleInPlaceSite)
			{
				inplaceSite->AddRef();
				*result = static_cast <IOleInPlaceSite*> (inplaceSite);
				return S_OK;
			}

			return ComBaseClassHelper <IOleClientSite>::QueryInterface (type, result);
		}

		JUCE_COMRESULT SaveObject()                                  { return E_NOTIMPL; }
		JUCE_COMRESULT GetMoniker (DWORD, DWORD, IMoniker**)         { return E_NOTIMPL; }
		JUCE_COMRESULT GetContainer (LPOLECONTAINER* ppContainer)    { *ppContainer = nullptr; return E_NOINTERFACE; }
		JUCE_COMRESULT ShowObject()                                  { return S_OK; }
		JUCE_COMRESULT OnShowWindow (BOOL)                           { return E_NOTIMPL; }
		JUCE_COMRESULT RequestNewObjectLayout()                      { return E_NOTIMPL; }

	private:
		JuceIOleInPlaceSite* inplaceSite;
	};

	static Array<ActiveXControlComponent*> activeXComps;

	HWND getHWND (const ActiveXControlComponent* const component)
	{
		HWND hwnd = 0;

		const IID iid = IID_IOleWindow;
		IOleWindow* const window = (IOleWindow*) component->queryInterface (&iid);

		if (window != nullptr)
		{
			window->GetWindow (&hwnd);
			window->Release();
		}

		return hwnd;
	}

	void offerActiveXMouseEventToPeer (ComponentPeer* const peer, HWND hwnd, UINT message, LPARAM lParam)
	{
		RECT activeXRect, peerRect;
		GetWindowRect (hwnd, &activeXRect);
		GetWindowRect ((HWND) peer->getNativeHandle(), &peerRect);

		switch (message)
		{
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
				peer->handleMouseEvent (0, Point<int> (GET_X_LPARAM (lParam) + activeXRect.left - peerRect.left,
													   GET_Y_LPARAM (lParam) + activeXRect.top  - peerRect.top),
										ModifierKeys::getCurrentModifiersRealtime(),
										getMouseEventTime());
				break;

			default:
				break;
		}
	}
}

class ActiveXControlComponent::Pimpl  : public ComponentMovementWatcher
{
public:
	Pimpl (HWND hwnd, ActiveXControlComponent& owner_)
		: ComponentMovementWatcher (&owner_),
		  owner (owner_),
		  controlHWND (0),
		  storage (new ActiveXHelpers::JuceIStorage()),
		  clientSite (new ActiveXHelpers::JuceIOleClientSite (hwnd)),
		  control (nullptr),
		  originalWndProc (0)
	{
	}

	~Pimpl()
	{
		if (control != nullptr)
		{
			control->Close (OLECLOSE_NOSAVE);
			control->Release();
		}

		clientSite->Release();
		storage->Release();
	}

	void setControlBounds (const Rectangle<int>& bounds) const
	{
		if (controlHWND != 0)
			MoveWindow (controlHWND, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), TRUE);
	}

	void setControlVisible (bool shouldBeVisible) const
	{
		if (controlHWND != 0)
			ShowWindow (controlHWND, shouldBeVisible ? SW_SHOWNA : SW_HIDE);
	}

	void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/)
	{
		Component* const topComp = owner.getTopLevelComponent();

		if (topComp->getPeer() != nullptr)
			setControlBounds (topComp->getLocalArea (&owner, owner.getLocalBounds()));
	}

	void componentPeerChanged()
	{
		componentMovedOrResized (true, true);
	}

	void componentVisibilityChanged()
	{
		setControlVisible (owner.isShowing());
		componentPeerChanged();
	}

	// intercepts events going to an activeX control, so we can sneakily use the mouse events
	static LRESULT CALLBACK activeXHookWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		for (int i = ActiveXHelpers::activeXComps.size(); --i >= 0;)
		{
			const ActiveXControlComponent* const ax = ActiveXHelpers::activeXComps.getUnchecked(i);

			if (ax->control != nullptr && ax->control->controlHWND == hwnd)
			{
				switch (message)
				{
				case WM_MOUSEMOVE:
				case WM_LBUTTONDOWN:
				case WM_MBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_MBUTTONUP:
				case WM_RBUTTONUP:
				case WM_LBUTTONDBLCLK:
				case WM_MBUTTONDBLCLK:
				case WM_RBUTTONDBLCLK:
					if (ax->isShowing())
					{
						ComponentPeer* const peer = ax->getPeer();

						if (peer != nullptr)
						{
							ActiveXHelpers::offerActiveXMouseEventToPeer (peer, hwnd, message, lParam);

							if (! ax->areMouseEventsAllowed())
								return 0;
						}
					}
					break;

				default:
					break;
				}

				return CallWindowProc (ax->control->originalWndProc, hwnd, message, wParam, lParam);
			}
		}

		return DefWindowProc (hwnd, message, wParam, lParam);
	}

private:
	ActiveXControlComponent& owner;

public:
	HWND controlHWND;
	IStorage* storage;
	IOleClientSite* clientSite;
	IOleObject* control;
	WNDPROC originalWndProc;

};

ActiveXControlComponent::ActiveXControlComponent()
	: mouseEventsAllowed (true)
{
	ActiveXHelpers::activeXComps.add (this);
}

ActiveXControlComponent::~ActiveXControlComponent()
{
	deleteControl();
	ActiveXHelpers::activeXComps.removeValue (this);
}

void ActiveXControlComponent::paint (Graphics& g)
{
	if (control == nullptr)
		g.fillAll (Colours::lightgrey);
}

bool ActiveXControlComponent::createControl (const void* controlIID)
{
	deleteControl();
	ComponentPeer* const peer = getPeer();

	// the component must have already been added to a real window when you call this!
	jassert (peer != nullptr);

	if (peer != nullptr)
	{
		const Rectangle<int> bounds (getTopLevelComponent()->getLocalArea (this, getLocalBounds()));
		HWND hwnd = (HWND) peer->getNativeHandle();

		ScopedPointer<Pimpl> newControl (new Pimpl (hwnd, *this));

		HRESULT hr;
		if ((hr = OleCreate (*(const IID*) controlIID, IID_IOleObject, 1 /*OLERENDER_DRAW*/, 0,
							 newControl->clientSite, newControl->storage,
							 (void**) &(newControl->control))) == S_OK)
		{
			newControl->control->SetHostNames (L"Juce", 0);

			if (OleSetContainedObject (newControl->control, TRUE) == S_OK)
			{
				RECT rect;
				rect.left   = bounds.getX();
				rect.top    = bounds.getY();
				rect.right  = bounds.getRight();
				rect.bottom = bounds.getBottom();

				if (newControl->control->DoVerb (OLEIVERB_SHOW, 0, newControl->clientSite, 0, hwnd, &rect) == S_OK)
				{
					control = newControl;
					control->controlHWND = ActiveXHelpers::getHWND (this);

					if (control->controlHWND != 0)
					{
						control->setControlBounds (bounds);

						control->originalWndProc = (WNDPROC) GetWindowLongPtr ((HWND) control->controlHWND, GWLP_WNDPROC);
						SetWindowLongPtr ((HWND) control->controlHWND, GWLP_WNDPROC, (LONG_PTR) Pimpl::activeXHookWndProc);
					}

					return true;
				}
			}
		}
	}

	return false;
}

void ActiveXControlComponent::deleteControl()
{
	control = nullptr;
}

void* ActiveXControlComponent::queryInterface (const void* iid) const
{
	void* result = nullptr;

	if (control != nullptr && control->control != nullptr
		 && SUCCEEDED (control->control->QueryInterface (*(const IID*) iid, &result)))
		return result;

	return nullptr;
}

void ActiveXControlComponent::setMouseEventsAllowed (const bool eventsCanReachControl)
{
	mouseEventsAllowed = eventsCanReachControl;
}

/*** End of inlined file: juce_win32_ActiveXComponent.cpp ***/

 #if JUCE_WEB_BROWSER

/*** Start of inlined file: juce_win32_WebBrowserComponent.cpp ***/
class WebBrowserComponentInternal   : public ActiveXControlComponent
{
public:
	WebBrowserComponentInternal()
		: browser (nullptr),
		  connectionPoint (nullptr),
		  adviseCookie (0)
	{
	}

	~WebBrowserComponentInternal()
	{
		if (connectionPoint != nullptr)
			connectionPoint->Unadvise (adviseCookie);

		if (browser != nullptr)
			browser->Release();
	}

	void createBrowser()
	{
		createControl (&CLSID_WebBrowser);
		browser = (IWebBrowser2*) queryInterface (&IID_IWebBrowser2);

		IConnectionPointContainer* connectionPointContainer = (IConnectionPointContainer*) queryInterface (&IID_IConnectionPointContainer);

		if (connectionPointContainer != nullptr)
		{
			connectionPointContainer->FindConnectionPoint (DIID_DWebBrowserEvents2,
														   &connectionPoint);

			if (connectionPoint != nullptr)
			{
				WebBrowserComponent* const owner = dynamic_cast <WebBrowserComponent*> (getParentComponent());
				jassert (owner != nullptr);

				EventHandler* handler = new EventHandler (*owner);
				connectionPoint->Advise (handler, &adviseCookie);
				handler->Release();
			}
		}
	}

	void goToURL (const String& url,
				  const StringArray* headers,
				  const MemoryBlock* postData)
	{
		if (browser != nullptr)
		{
			LPSAFEARRAY sa = nullptr;

			VARIANT flags, frame, postDataVar, headersVar;  // (_variant_t isn't available in all compilers)
			VariantInit (&flags);
			VariantInit (&frame);
			VariantInit (&postDataVar);
			VariantInit (&headersVar);

			if (headers != nullptr)
			{
				V_VT (&headersVar) = VT_BSTR;
				V_BSTR (&headersVar) = SysAllocString ((const OLECHAR*) headers->joinIntoString ("\r\n").toWideCharPointer());
			}

			if (postData != nullptr && postData->getSize() > 0)
			{
				LPSAFEARRAY sa = SafeArrayCreateVector (VT_UI1, 0, (ULONG) postData->getSize());

				if (sa != 0)
				{
					void* data = nullptr;
					SafeArrayAccessData (sa, &data);
					jassert (data != nullptr);

					if (data != nullptr)
					{
						postData->copyTo (data, 0, postData->getSize());
						SafeArrayUnaccessData (sa);

						VARIANT postDataVar2;
						VariantInit (&postDataVar2);
						V_VT (&postDataVar2) = VT_ARRAY | VT_UI1;
						V_ARRAY (&postDataVar2) = sa;

						postDataVar = postDataVar2;
					}
				}
			}

			browser->Navigate ((BSTR) (const OLECHAR*) url.toWideCharPointer(),
							   &flags, &frame,
							   &postDataVar, &headersVar);

			if (sa != 0)
				SafeArrayDestroy (sa);

			VariantClear (&flags);
			VariantClear (&frame);
			VariantClear (&postDataVar);
			VariantClear (&headersVar);
		}
	}

	IWebBrowser2* browser;

private:
	IConnectionPoint* connectionPoint;
	DWORD adviseCookie;

	class EventHandler  : public ComBaseClassHelper <IDispatch>,
						  public ComponentMovementWatcher
	{
	public:
		EventHandler (WebBrowserComponent& owner_)
			: ComponentMovementWatcher (&owner_),
			  owner (owner_)
		{
		}

		JUCE_COMRESULT GetTypeInfoCount (UINT*)                                  { return E_NOTIMPL; }
		JUCE_COMRESULT GetTypeInfo (UINT, LCID, ITypeInfo**)                     { return E_NOTIMPL; }
		JUCE_COMRESULT GetIDsOfNames (REFIID, LPOLESTR*, UINT, LCID, DISPID*)    { return E_NOTIMPL; }

		JUCE_COMRESULT Invoke (DISPID dispIdMember, REFIID /*riid*/, LCID /*lcid*/, WORD /*wFlags*/, DISPPARAMS* pDispParams,
							   VARIANT* /*pVarResult*/, EXCEPINFO* /*pExcepInfo*/, UINT* /*puArgErr*/)
		{
			if (dispIdMember == DISPID_BEFORENAVIGATE2)
			{
				*pDispParams->rgvarg->pboolVal
					= owner.pageAboutToLoad (getStringFromVariant (pDispParams->rgvarg[5].pvarVal)) ? VARIANT_FALSE
																									: VARIANT_TRUE;
				return S_OK;
			}
			else if (dispIdMember == DISPID_DOCUMENTCOMPLETE)
			{
				owner.pageFinishedLoading (getStringFromVariant (pDispParams->rgvarg[0].pvarVal));
				return S_OK;
			}

			return E_NOTIMPL;
		}

		void componentMovedOrResized (bool, bool )  {}
		void componentPeerChanged()                 {}
		void componentVisibilityChanged()           { owner.visibilityChanged(); }

	private:
		WebBrowserComponent& owner;

		static String getStringFromVariant (VARIANT* v)
		{
			return (v->vt & VT_BYREF) != 0 ? *v->pbstrVal
										   : v->bstrVal;
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EventHandler);
	};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebBrowserComponentInternal);
};

WebBrowserComponent::WebBrowserComponent (const bool unloadPageWhenBrowserIsHidden_)
	: browser (nullptr),
	  blankPageShown (false),
	  unloadPageWhenBrowserIsHidden (unloadPageWhenBrowserIsHidden_)
{
	setOpaque (true);
	addAndMakeVisible (browser = new WebBrowserComponentInternal());
}

WebBrowserComponent::~WebBrowserComponent()
{
	delete browser;
}

void WebBrowserComponent::goToURL (const String& url,
								   const StringArray* headers,
								   const MemoryBlock* postData)
{
	lastURL = url;

	lastHeaders.clear();
	if (headers != nullptr)
		lastHeaders = *headers;

	lastPostData.setSize (0);
	if (postData != nullptr)
		lastPostData = *postData;

	blankPageShown = false;

	browser->goToURL (url, headers, postData);
}

void WebBrowserComponent::stop()
{
	if (browser->browser != nullptr)
		browser->browser->Stop();
}

void WebBrowserComponent::goBack()
{
	lastURL = String::empty;
	blankPageShown = false;

	if (browser->browser != nullptr)
		browser->browser->GoBack();
}

void WebBrowserComponent::goForward()
{
	lastURL = String::empty;

	if (browser->browser != nullptr)
		browser->browser->GoForward();
}

void WebBrowserComponent::refresh()
{
	if (browser->browser != nullptr)
		browser->browser->Refresh();
}

void WebBrowserComponent::paint (Graphics& g)
{
	if (browser->browser == nullptr)
		g.fillAll (Colours::white);
}

void WebBrowserComponent::checkWindowAssociation()
{
	if (isShowing())
	{
		if (browser->browser == nullptr && getPeer() != nullptr)
		{
			browser->createBrowser();
			reloadLastURL();
		}
		else
		{
			if (blankPageShown)
				goBack();
		}
	}
	else
	{
		if (browser != nullptr && unloadPageWhenBrowserIsHidden && ! blankPageShown)
		{
			// when the component becomes invisible, some stuff like flash
			// carries on playing audio, so we need to force it onto a blank
			// page to avoid this..

			blankPageShown = true;
			browser->goToURL ("about:blank", 0, 0);
		}
	}
}

void WebBrowserComponent::reloadLastURL()
{
	if (lastURL.isNotEmpty())
	{
		goToURL (lastURL, &lastHeaders, &lastPostData);
		lastURL = String::empty;
	}
}

void WebBrowserComponent::parentHierarchyChanged()
{
	checkWindowAssociation();
}

void WebBrowserComponent::resized()
{
	browser->setSize (getWidth(), getHeight());
}

void WebBrowserComponent::visibilityChanged()
{
	checkWindowAssociation();
}

bool WebBrowserComponent::pageAboutToLoad (const String&)  { return true; }
void WebBrowserComponent::pageFinishedLoading (const String&) {}

/*** End of inlined file: juce_win32_WebBrowserComponent.cpp ***/


 #endif

/*** Start of inlined file: juce_win32_SystemTrayIcon.cpp ***/
// (these functions are from juce_win32_Windowing.cpp)
extern void* getUser32Function (const char*);

namespace IconConverters
{
	extern HICON createHICONFromImage (const Image&, BOOL isIcon, int hotspotX, int hotspotY);
}

class SystemTrayIconComponent::Pimpl
{
public:
	Pimpl (SystemTrayIconComponent& owner_, HICON hicon, HWND hwnd)
		: owner (owner_),
		  originalWndProc ((WNDPROC) GetWindowLongPtr (hwnd, GWLP_WNDPROC)),
		  taskbarCreatedMessage (RegisterWindowMessage (TEXT ("TaskbarCreated")))
	{
		SetWindowLongPtr (hwnd, GWLP_WNDPROC, (LONG_PTR) hookedWndProc);

		zerostruct (iconData);
		iconData.cbSize = sizeof (iconData);
		iconData.hWnd = hwnd;
		iconData.uID = (UINT) (pointer_sized_int) hwnd;
		iconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		iconData.uCallbackMessage = WM_TRAYNOTIFY;
		iconData.hIcon = hicon;

		Shell_NotifyIcon (NIM_ADD, &iconData);

		// In order to receive the "TaskbarCreated" message, we need to request that it's not filtered out.
		// (Need to load dynamically, as ChangeWindowMessageFilter is only available in Vista and later)
		typedef BOOL (WINAPI* ChangeWindowMessageFilterType) (UINT, DWORD);
		ChangeWindowMessageFilterType changeWindowMessageFilter
			= (ChangeWindowMessageFilterType) getUser32Function ("ChangeWindowMessageFilter");

		if (changeWindowMessageFilter != nullptr)
			changeWindowMessageFilter (taskbarCreatedMessage, 1 /* MSGFLT_ADD */);
	}

	~Pimpl()
	{
		SetWindowLongPtr (iconData.hWnd, GWLP_WNDPROC, (LONG_PTR) originalWndProc);

		iconData.uFlags = 0;
		Shell_NotifyIcon (NIM_DELETE, &iconData);
		DestroyIcon (iconData.hIcon);
	}

	void updateIcon (HICON hicon)
	{
		HICON oldIcon = iconData.hIcon;

		iconData.hIcon = hicon;
		iconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		Shell_NotifyIcon (NIM_MODIFY, &iconData);

		DestroyIcon (oldIcon);
	}

	void setToolTip (const String& toolTip)
	{
		iconData.uFlags = NIF_TIP;
		toolTip.copyToUTF16 (iconData.szTip, sizeof (iconData.szTip) - 1);
		Shell_NotifyIcon (NIM_MODIFY, &iconData);
	}

	void handleTaskBarEvent (const LPARAM lParam)
	{
		if (owner.isCurrentlyBlockedByAnotherModalComponent())
		{
			if (lParam == WM_LBUTTONDOWN || lParam == WM_RBUTTONDOWN
				 || lParam == WM_LBUTTONDBLCLK || lParam == WM_LBUTTONDBLCLK)
			{
				Component* const current = Component::getCurrentlyModalComponent();

				if (current != nullptr)
					current->inputAttemptWhenModal();
			}
		}
		else
		{
			ModifierKeys eventMods (ModifierKeys::getCurrentModifiersRealtime());

			if (lParam == WM_LBUTTONDOWN || lParam == WM_LBUTTONDBLCLK)
				eventMods = eventMods.withFlags (ModifierKeys::leftButtonModifier);
			else if (lParam == WM_RBUTTONDOWN || lParam == WM_RBUTTONDBLCLK)
				eventMods = eventMods.withFlags (ModifierKeys::rightButtonModifier);
			else if (lParam == WM_LBUTTONUP || lParam == WM_RBUTTONUP)
				eventMods = eventMods.withoutMouseButtons();

			const MouseEvent e (Desktop::getInstance().getMainMouseSource(),
								Point<int>(), eventMods, &owner, &owner, Time (getMouseEventTime()),
								Point<int>(), Time (getMouseEventTime()), 1, false);

			if (lParam == WM_LBUTTONDOWN || lParam == WM_RBUTTONDOWN)
			{
				SetFocus (iconData.hWnd);
				SetForegroundWindow (iconData.hWnd);
				owner.mouseDown (e);
			}
			else if (lParam == WM_LBUTTONUP || lParam == WM_RBUTTONUP)
			{
				owner.mouseUp (e);
			}
			else if (lParam == WM_LBUTTONDBLCLK || lParam == WM_LBUTTONDBLCLK)
			{
				owner.mouseDoubleClick (e);
			}
			else if (lParam == WM_MOUSEMOVE)
			{
				owner.mouseMove (e);
			}
		}
	}

	static Pimpl* getPimpl (HWND hwnd)
	{
		if (JuceWindowIdentifier::isJUCEWindow (hwnd))
		{
			ComponentPeer* peer = (ComponentPeer*) GetWindowLongPtr (hwnd, 8);

			if (peer != nullptr)
			{
				SystemTrayIconComponent* const iconComp = dynamic_cast<SystemTrayIconComponent*> (peer->getComponent());

				if (iconComp != nullptr)
					return iconComp->pimpl;
			}
		}

		return nullptr;
	}

	static LRESULT CALLBACK hookedWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Pimpl* const p = getPimpl (hwnd);

		if (p != nullptr)
			return p->windowProc  (hwnd, message, wParam, lParam);
		else
			return DefWindowProcW (hwnd, message, wParam, lParam);
   }

	LRESULT windowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_TRAYNOTIFY)
		{
			handleTaskBarEvent (lParam);
		}
		else if (message == taskbarCreatedMessage)
		{
			iconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			Shell_NotifyIcon (NIM_ADD, &iconData);
		}

		return CallWindowProc (originalWndProc, hwnd, message, wParam, lParam);
	}

private:
	SystemTrayIconComponent& owner;
	NOTIFYICONDATA iconData;
	WNDPROC originalWndProc;
	const DWORD taskbarCreatedMessage;
	enum { WM_TRAYNOTIFY = WM_USER + 100 };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pimpl);
};

void SystemTrayIconComponent::setIconImage (const Image& newImage)
{
	if (newImage.isValid())
	{
		HICON hicon = IconConverters::createHICONFromImage (newImage, TRUE, 0, 0);

		if (pimpl == nullptr)
			pimpl = new Pimpl (*this, hicon, (HWND) getWindowHandle());
		else
			pimpl->updateIcon (hicon);
	}
	else
	{
		pimpl = nullptr;
	}
}

void SystemTrayIconComponent::setIconTooltip (const String& tooltip)
{
	if (pimpl != nullptr)
		pimpl->setToolTip (tooltip);
}

/*** End of inlined file: juce_win32_SystemTrayIcon.cpp ***/


#elif JUCE_LINUX
 #if JUCE_WEB_BROWSER

/*** Start of inlined file: juce_linux_WebBrowserComponent.cpp ***/
/*
	Sorry.. This class isn't implemented on Linux!
*/

WebBrowserComponent::WebBrowserComponent (const bool unloadPageWhenBrowserIsHidden_)
	: browser (0),
	  blankPageShown (false),
	  unloadPageWhenBrowserIsHidden (unloadPageWhenBrowserIsHidden_)
{
	setOpaque (true);
}

WebBrowserComponent::~WebBrowserComponent()
{
}

void WebBrowserComponent::goToURL (const String& url,
								   const StringArray* headers,
								   const MemoryBlock* postData)
{
	lastURL = url;

	lastHeaders.clear();
	if (headers != nullptr)
		lastHeaders = *headers;

	lastPostData.setSize (0);
	if (postData != nullptr)
		lastPostData = *postData;

	blankPageShown = false;

}

void WebBrowserComponent::stop()
{
}

void WebBrowserComponent::goBack()
{
	lastURL = String::empty;
	blankPageShown = false;

}

void WebBrowserComponent::goForward()
{
	lastURL = String::empty;

}

void WebBrowserComponent::refresh()
{
}

void WebBrowserComponent::paint (Graphics& g)
{
	g.fillAll (Colours::white);
}

void WebBrowserComponent::checkWindowAssociation()
{
}

void WebBrowserComponent::reloadLastURL()
{
	if (lastURL.isNotEmpty())
	{
		goToURL (lastURL, &lastHeaders, &lastPostData);
		lastURL = String::empty;
	}
}

void WebBrowserComponent::parentHierarchyChanged()
{
	checkWindowAssociation();
}

void WebBrowserComponent::resized()
{
}

void WebBrowserComponent::visibilityChanged()
{
	checkWindowAssociation();
}

bool WebBrowserComponent::pageAboutToLoad (const String&)  { return true; }
void WebBrowserComponent::pageFinishedLoading (const String&) {}

/*** End of inlined file: juce_linux_WebBrowserComponent.cpp ***/


 #endif

/*** Start of inlined file: juce_linux_SystemTrayIcon.cpp ***/
extern Display* display;

class SystemTrayIconComponent::Pimpl
{
public:
	Pimpl (const Image& image_, Window windowH)
		: image (image_)
	{
		ScopedXLock xlock;

		Screen* const screen = XDefaultScreenOfDisplay (display);
		const int screenNumber = XScreenNumberOfScreen (screen);

		String screenAtom ("_NET_SYSTEM_TRAY_S");
		screenAtom << screenNumber;
		Atom selectionAtom = XInternAtom (display, screenAtom.toUTF8(), false);

		XGrabServer (display);
		Window managerWin = XGetSelectionOwner (display, selectionAtom);

		if (managerWin != None)
			XSelectInput (display, managerWin, StructureNotifyMask);

		XUngrabServer (display);
		XFlush (display);

		if (managerWin != None)
		{
			XEvent ev = { 0 };
			ev.xclient.type = ClientMessage;
			ev.xclient.window = managerWin;
			ev.xclient.message_type = XInternAtom (display, "_NET_SYSTEM_TRAY_OPCODE", False);
			ev.xclient.format = 32;
			ev.xclient.data.l[0] = CurrentTime;
			ev.xclient.data.l[1] = 0 /*SYSTEM_TRAY_REQUEST_DOCK*/;
			ev.xclient.data.l[2] = windowH;
			ev.xclient.data.l[3] = 0;
			ev.xclient.data.l[4] = 0;

			XSendEvent (display, managerWin, False, NoEventMask, &ev);
			XSync (display, False);
		}

		// For older KDE's ...
		long atomData = 1;
		Atom trayAtom = XInternAtom (display, "KWM_DOCKWINDOW", false);
		XChangeProperty (display, windowH, trayAtom, trayAtom, 32, PropModeReplace, (unsigned char*) &atomData, 1);

		// For more recent KDE's...
		trayAtom = XInternAtom (display, "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR", false);
		XChangeProperty (display, windowH, trayAtom, XA_WINDOW, 32, PropModeReplace, (unsigned char*) &windowH, 1);

		// A minimum size must be specified for GNOME and Xfce, otherwise the icon is displayed with a width of 1
		XSizeHints* hints = XAllocSizeHints();
		hints->flags = PMinSize;
		hints->min_width = 22;
		hints->min_height = 22;
		XSetWMNormalHints (display, windowH, hints);
		XFree (hints);
	}

	Image image;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pimpl);
};

void SystemTrayIconComponent::setIconImage (const Image& newImage)
{
	pimpl = nullptr;

	if (newImage.isValid())
	{
		if (! isOnDesktop())
			addToDesktop (0);

		pimpl = new Pimpl (newImage, (Window) getWindowHandle());

		setVisible (true);
		toFront (false);
	}

	repaint();
}

void SystemTrayIconComponent::paint (Graphics& g)
{
	if (pimpl != nullptr)
		g.drawImageWithin (pimpl->image, 0, 0, getWidth(), getHeight(),
						   RectanglePlacement::xLeft | RectanglePlacement::yTop | RectanglePlacement::onlyReduceInSize, false);
}

void SystemTrayIconComponent::setIconTooltip (const String& tooltip)
{
	// xxx not yet implemented!
}

/*** End of inlined file: juce_linux_SystemTrayIcon.cpp ***/


#elif JUCE_ANDROID
 #if JUCE_WEB_BROWSER

/*** Start of inlined file: juce_android_WebBrowserComponent.cpp ***/
WebBrowserComponent::WebBrowserComponent (const bool unloadPageWhenBrowserIsHidden_)
	: browser (nullptr),
	  blankPageShown (false),
	  unloadPageWhenBrowserIsHidden (unloadPageWhenBrowserIsHidden_)
{
	setOpaque (true);
}

WebBrowserComponent::~WebBrowserComponent()
{
}

void WebBrowserComponent::goToURL (const String& url,
								   const StringArray* headers,
								   const MemoryBlock* postData)
{
	lastURL = url;

	lastHeaders.clear();
	if (headers != nullptr)
		lastHeaders = *headers;

	lastPostData.setSize (0);
	if (postData != nullptr)
		lastPostData = *postData;

	blankPageShown = false;

}

void WebBrowserComponent::stop()
{
}

void WebBrowserComponent::goBack()
{
	lastURL = String::empty;
	blankPageShown = false;

}

void WebBrowserComponent::goForward()
{
	lastURL = String::empty;

}

void WebBrowserComponent::refresh()
{
}

void WebBrowserComponent::paint (Graphics& g)
{
	g.fillAll (Colours::white);
}

void WebBrowserComponent::checkWindowAssociation()
{
}

void WebBrowserComponent::reloadLastURL()
{
	if (lastURL.isNotEmpty())
	{
		goToURL (lastURL, &lastHeaders, &lastPostData);
		lastURL = String::empty;
	}
}

void WebBrowserComponent::parentHierarchyChanged()
{
	checkWindowAssociation();
}

void WebBrowserComponent::resized()
{
}

void WebBrowserComponent::visibilityChanged()
{
	checkWindowAssociation();
}

bool WebBrowserComponent::pageAboutToLoad (const String&)  { return true; }
void WebBrowserComponent::pageFinishedLoading (const String&) {}

/*** End of inlined file: juce_android_WebBrowserComponent.cpp ***/


 #endif
#endif

}

/*** End of inlined file: juce_gui_extra.cpp ***/


#endif

