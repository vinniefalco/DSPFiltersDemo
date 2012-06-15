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


/*** Start of inlined file: juce_video.h ***/
#ifndef __JUCE_VIDEO_JUCEHEADER__
#define __JUCE_VIDEO_JUCEHEADER__

#include "juce_gui_extra_amalgam.h"

/** Config: JUCE_DIRECTSHOW
	Enables DirectShow media-streaming architecture (MS Windows only).
*/
#ifndef JUCE_DIRECTSHOW
 #define JUCE_DIRECTSHOW 0
#endif

/** Config: JUCE_MEDIAFOUNDATION
	Enables Media Foundation multimedia platform (Windows Vista and above).
*/
#ifndef JUCE_MEDIAFOUNDATION
 #define JUCE_MEDIAFOUNDATION 0
#endif

#if ! JUCE_WINDOWS
 #undef JUCE_DIRECTSHOW
 #undef JUCE_MEDIAFOUNDATION
#endif

/** Config: JUCE_QUICKTIME
	Enables the QuickTimeMovieComponent class (Mac and Windows).
	If you're building on Windows, you'll need to have the Apple QuickTime SDK
	installed, and its header files will need to be on your include path.
*/
#if ! (defined (JUCE_QUICKTIME) || JUCE_LINUX || JUCE_IOS || JUCE_ANDROID || (JUCE_WINDOWS && ! JUCE_MSVC))
 #define JUCE_QUICKTIME 0
#endif

/** Config: JUCE_USE_CAMERA
	Enables web-cam support using the CameraDevice class (Mac and Windows).
*/
#if (JUCE_QUICKTIME || JUCE_WINDOWS) && ! defined (JUCE_USE_CAMERA)
 #define JUCE_USE_CAMERA 0
#endif

#if ! (JUCE_MAC || JUCE_WINDOWS)
 #undef JUCE_QUICKTIME
 #undef JUCE_USE_CAMERA
#endif

namespace juce
{

// START_AUTOINCLUDE playback, capture
#ifndef __JUCE_DIRECTSHOWCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_DirectShowComponent.h ***/
#ifndef __JUCE_DIRECTSHOWCOMPONENT_JUCEHEADER__
#define __JUCE_DIRECTSHOWCOMPONENT_JUCEHEADER__

#if JUCE_DIRECTSHOW || DOXYGEN

/**
	A window that can play back a DirectShow video.

	@note Controller is not implemented
*/
class JUCE_API  DirectShowComponent  : public Component
{
public:

	/** DirectShow video renderer type.

		See MSDN for adivce about choosing the right renderer.
	*/
	enum VideoRendererType
	{
		dshowDefault, /**< VMR7 for Windows XP, EVR for Windows Vista and later */
		dshowVMR7,    /**< Video Mixing Renderer 7 */
		dshowEVR      /**< Enhanced Video Renderer */
	};

	/** Creates a DirectShowComponent, initially blank.

		Use the loadMovie() method to load a video once you've added the
		component to a window, (or put it on the desktop as a heavyweight window).
		Loading a video when the component isn't visible can cause problems, as
		DirectShow needs a window handle to initialise properly.

		@see VideoRendererType
	*/
	DirectShowComponent (VideoRendererType type = dshowDefault);

	/** Destructor. */
	~DirectShowComponent();

	/** Returns true if DirectShow is installed and working on this machine. */
	static bool isDirectShowAvailable();

	/** Tries to load a DirectShow video from a file or URL into the player.

		It's best to call this function once you've added the component to a window,
		(or put it on the desktop as a heavyweight window). Loading a video when the
		component isn't visible can cause problems, because DirectShow needs a window
		handle to do its stuff.

		@param fileOrURLPath    the file or URL path to open
		@returns true if the video opens successfully
	*/
	bool loadMovie (const String& fileOrURLPath);

	/** Tries to load a DirectShow video from a file into the player.

		It's best to call this function once you've added the component to a window,
		(or put it on the desktop as a heavyweight window). Loading a video when the
		component isn't visible can cause problems, because DirectShow needs a window
		handle to do its stuff.

		@param videoFile    the video file to open
		@returns true if the video opens successfully
	*/
	bool loadMovie (const File& videoFile);

	/** Tries to load a DirectShow video from a URL into the player.

		It's best to call this function once you've added the component to a window,
		(or put it on the desktop as a heavyweight window). Loading a video when the
		component isn't visible can cause problems, because DirectShow needs a window
		handle to do its stuff.

		@param videoURL    the video URL to open
		@returns true if the video opens successfully
	*/
	bool loadMovie (const URL& videoURL);

	/** Closes the video, if one is open. */
	void closeMovie();

	/** Returns the file path or URL from which the video file was loaded.
		If there isn't one, this returns an empty string.
	*/
	File getCurrentMoviePath() const;

	/** Returns true if there's currently a video open. */
	bool isMovieOpen() const;

	/** Returns the length of the video, in seconds. */
	double getMovieDuration() const;

	/** Returns the video's natural size, in pixels.

		You can use this to resize the component to show the video at its preferred
		scale.

		If no video is loaded, the size returned will be 0 x 0.
	*/
	void getMovieNormalSize (int& width, int& height) const;

	/** This will position the component within a given area, keeping its aspect
		ratio correct according to the video's normal size.

		The component will be made as large as it can go within the space, and will
		be aligned according to the justification value if this means there are gaps at
		the top or sides.

		@note Not implemented
	*/
	void setBoundsWithCorrectAspectRatio (const Rectangle<int>& spaceToFitWithin,
										  const RectanglePlacement& placement);

	/** Starts the video playing. */
	void play();

	/** Stops the video playing. */
	void stop();

	/** Returns true if the video is currently playing. */
	bool isPlaying() const;

	/** Moves the video's position back to the start. */
	void goToStart();

	/** Sets the video's position to a given time. */
	void setPosition (double seconds);

	/** Returns the current play position of the video. */
	double getPosition() const;

	/** Changes the video playback rate.

		A value of 1 is normal speed, greater values play it proportionately faster,
		smaller values play it slower.
	*/
	void setSpeed (float newSpeed);

	/** Changes the video's playback volume.

		@param newVolume    the volume in the range 0 (silent) to 1.0 (full)
	*/
	void setMovieVolume (float newVolume);

	/** Returns the video's playback volume.

		@returns the volume in the range 0 (silent) to 1.0 (full)
	*/
	float getMovieVolume() const;

	/** Tells the video whether it should loop. */
	void setLooping (bool shouldLoop);

	/** Returns true if the video is currently looping.

		@see setLooping
	*/
	bool isLooping() const;

	/** @internal */
	void paint (Graphics& g);

private:

	String videoPath;
	bool videoLoaded, looping;

	class DirectShowContext;
	friend class DirectShowContext;
	friend class ScopedPointer <DirectShowContext>;
	ScopedPointer <DirectShowContext> context;

	class DirectShowComponentWatcher;
	friend class DirectShowComponentWatcher;
	friend class ScopedPointer <DirectShowComponentWatcher>;
	ScopedPointer <DirectShowComponentWatcher> componentWatcher;

	bool needToUpdateViewport, needToRecreateNativeWindow;

	void updateContextPosition();
	void showContext (bool shouldBeVisible);
	void recreateNativeWindowAsync();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectShowComponent);
};

#endif
#endif   // __JUCE_DIRECTSHOWCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_DirectShowComponent.h ***/


#endif
#ifndef __JUCE_QUICKTIMEMOVIECOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_QuickTimeMovieComponent.h ***/
#ifndef __JUCE_QUICKTIMEMOVIECOMPONENT_JUCEHEADER__
#define __JUCE_QUICKTIMEMOVIECOMPONENT_JUCEHEADER__

// (NB: This stuff mustn't go inside the "#if QUICKTIME" block, or it'll break the
// amalgamated build)
#ifndef DOXYGEN
 #if JUCE_WINDOWS
  typedef ActiveXControlComponent QTCompBaseClass;
 #elif JUCE_MAC
  typedef NSViewComponent QTCompBaseClass;
 #endif
#endif

#if JUCE_QUICKTIME || DOXYGEN

/**
	A window that can play back a QuickTime movie.

*/
class JUCE_API  QuickTimeMovieComponent     : public QTCompBaseClass
{
public:

	/** Creates a QuickTimeMovieComponent, initially blank.

		Use the loadMovie() method to load a movie once you've added the
		component to a window, (or put it on the desktop as a heavyweight window).
		Loading a movie when the component isn't visible can cause problems, as
		QuickTime needs a window handle to initialise properly.
	*/
	QuickTimeMovieComponent();

	/** Destructor. */
	~QuickTimeMovieComponent();

	/** Returns true if QT is installed and working on this machine.
	*/
	static bool isQuickTimeAvailable() noexcept;

	/** Tries to load a QuickTime movie from a file into the player.

		It's best to call this function once you've added the component to a window,
		(or put it on the desktop as a heavyweight window). Loading a movie when the
		component isn't visible can cause problems, because QuickTime needs a window
		handle to do its stuff.

		@param movieFile    the .mov file to open
		@param isControllerVisible  whether to show a controller bar at the bottom
		@returns true if the movie opens successfully
	*/
	bool loadMovie (const File& movieFile,
					bool isControllerVisible);

	/** Tries to load a QuickTime movie from a URL into the player.

		It's best to call this function once you've added the component to a window,
		(or put it on the desktop as a heavyweight window). Loading a movie when the
		component isn't visible can cause problems, because QuickTime needs a window
		handle to do its stuff.

		@param movieURL    the .mov file to open
		@param isControllerVisible  whether to show a controller bar at the bottom
		@returns true if the movie opens successfully
	*/
	bool loadMovie (const URL& movieURL,
					bool isControllerVisible);

	/** Tries to load a QuickTime movie from a stream into the player.

		It's best to call this function once you've added the component to a window,
		(or put it on the desktop as a heavyweight window). Loading a movie when the
		component isn't visible can cause problems, because QuickTime needs a window
		handle to do its stuff.

		@param movieStream    a stream containing a .mov file. The component may try
							  to read the whole stream before playing, rather than
							  streaming from it.
		@param isControllerVisible  whether to show a controller bar at the bottom
		@returns true if the movie opens successfully
	*/
	bool loadMovie (InputStream* movieStream,
					bool isControllerVisible);

	/** Closes the movie, if one is open. */
	void closeMovie();

	/** Returns the movie file that is currently open.

		If there isn't one, this returns File::nonexistent
	*/
	File getCurrentMovieFile() const;

	/** Returns true if there's currently a movie open. */
	bool isMovieOpen() const;

	/** Returns the length of the movie, in seconds. */
	double getMovieDuration() const;

	/** Returns the movie's natural size, in pixels.

		You can use this to resize the component to show the movie at its preferred
		scale.

		If no movie is loaded, the size returned will be 0 x 0.
	*/
	void getMovieNormalSize (int& width, int& height) const;

	/** This will position the component within a given area, keeping its aspect
		ratio correct according to the movie's normal size.

		The component will be made as large as it can go within the space, and will
		be aligned according to the justification value if this means there are gaps at
		the top or sides.
	*/
	void setBoundsWithCorrectAspectRatio (const Rectangle<int>& spaceToFitWithin,
										  const RectanglePlacement& placement);

	/** Starts the movie playing. */
	void play();

	/** Stops the movie playing. */
	void stop();

	/** Returns true if the movie is currently playing. */
	bool isPlaying() const;

	/** Moves the movie's position back to the start. */
	void goToStart();

	/** Sets the movie's position to a given time. */
	void setPosition (double seconds);

	/** Returns the current play position of the movie. */
	double getPosition() const;

	/** Changes the movie playback rate.

		A value of 1 is normal speed, greater values play it proportionately faster,
		smaller values play it slower.
	*/
	void setSpeed (float newSpeed);

	/** Changes the movie's playback volume.

		@param newVolume    the volume in the range 0 (silent) to 1.0 (full)
	*/
	void setMovieVolume (float newVolume);

	/** Returns the movie's playback volume.

		@returns the volume in the range 0 (silent) to 1.0 (full)
	*/
	float getMovieVolume() const;

	/** Tells the movie whether it should loop. */
	void setLooping (bool shouldLoop);

	/** Returns true if the movie is currently looping.

		@see setLooping
	*/
	bool isLooping() const;

	/** True if the native QuickTime controller bar is shown in the window.

		@see loadMovie
	*/
	bool isControllerVisible() const;

	/** @internal */
	void paint (Graphics& g);

private:

	File movieFile;
	bool movieLoaded, controllerVisible, looping;

#if JUCE_WINDOWS
	void parentHierarchyChanged();
	void visibilityChanged();

	void createControlIfNeeded();
	bool isControlCreated() const;

	class Pimpl;
	friend class ScopedPointer <Pimpl>;
	ScopedPointer <Pimpl> pimpl;
#else
	void* movie;
#endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QuickTimeMovieComponent);
};

#endif
#endif   // __JUCE_QUICKTIMEMOVIECOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_QuickTimeMovieComponent.h ***/


#endif
#ifndef __JUCE_CAMERADEVICE_JUCEHEADER__

/*** Start of inlined file: juce_CameraDevice.h ***/
#ifndef __JUCE_CAMERADEVICE_JUCEHEADER__
#define __JUCE_CAMERADEVICE_JUCEHEADER__

#if JUCE_USE_CAMERA || DOXYGEN

/**
	Controls any video capture devices that might be available.

	Use getAvailableDevices() to list the devices that are attached to the
	system, then call openDevice to open one for use. Once you have a CameraDevice
	object, you can get a viewer component from it, and use its methods to
	stream to a file or capture still-frames.
*/
class JUCE_API  CameraDevice
{
public:
	/** Destructor. */
	virtual ~CameraDevice();

	/** Returns a list of the available cameras on this machine.

		You can open one of these devices by calling openDevice().
	*/
	static StringArray getAvailableDevices();

	/** Opens a camera device.

		The index parameter indicates which of the items returned by getAvailableDevices()
		to open.

		The size constraints allow the method to choose between different resolutions if
		the camera supports this. If the resolution cam't be specified (e.g. on the Mac)
		then these will be ignored.
	*/
	static CameraDevice* openDevice (int deviceIndex,
									 int minWidth = 128, int minHeight = 64,
									 int maxWidth = 1024, int maxHeight = 768);

	/** Returns the name of this device */
	String getName() const                { return name; }

	/** Creates a component that can be used to display a preview of the
		video from this camera.
	*/
	Component* createViewerComponent();

	/** Starts recording video to the specified file.

		You should use getFileExtension() to find out the correct extension to
		use for your filename.

		If the file exists, it will be deleted before the recording starts.

		This method may not start recording instantly, so if you need to know the
		exact time at which the file begins, you can call getTimeOfFirstRecordedFrame()
		after the recording has finished.

		The quality parameter can be 0, 1, or 2, to indicate low, medium, or high. It may
		or may not be used, depending on the driver.
	*/
	void startRecordingToFile (const File& file, int quality = 2);

	/** Stops recording, after a call to startRecordingToFile().
	*/
	void stopRecording();

	/** Returns the file extension that should be used for the files
		that you pass to startRecordingToFile().

		This may be platform-specific, e.g. ".mov" or ".avi".
	*/
	static String getFileExtension();

	/** After calling stopRecording(), this method can be called to return the timestamp
		of the first frame that was written to the file.
	*/
	Time getTimeOfFirstRecordedFrame() const;

	/**
		Receives callbacks with images from a CameraDevice.

		@see CameraDevice::addListener
	*/
	class JUCE_API  Listener
	{
	public:
		Listener() {}
		virtual ~Listener() {}

		/** This method is called when a new image arrives.

			This may be called by any thread, so be careful about thread-safety,
			and make sure that you process the data as quickly as possible to
			avoid glitching!
		*/
		virtual void imageReceived (const Image& image) = 0;
	};

	/** Adds a listener to receive images from the camera.

		Be very careful not to delete the listener without first removing it by calling
		removeListener().
	*/
	void addListener (Listener* listenerToAdd);

	/** Removes a listener that was previously added with addListener().
	*/
	void removeListener (Listener* listenerToRemove);

protected:
   #ifndef DOXYGEN
	CameraDevice (const String& name, int index);
   #endif

private:
	void* internal;
	bool isRecording;
	String name;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CameraDevice);
};

/** This typedef is just for compatibility with old code - newer code should use the CameraDevice::Listener class directly. */
typedef CameraDevice::Listener CameraImageListener;

#endif
#endif   // __JUCE_CAMERADEVICE_JUCEHEADER__

/*** End of inlined file: juce_CameraDevice.h ***/


#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_VIDEO_JUCEHEADER__

/*** End of inlined file: juce_video.h ***/

