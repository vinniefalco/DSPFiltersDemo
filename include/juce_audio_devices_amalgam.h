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


/*** Start of inlined file: juce_audio_devices.h ***/
#ifndef __JUCE_AUDIO_DEVICES_JUCEHEADER__
#define __JUCE_AUDIO_DEVICES_JUCEHEADER__

#include "juce_events_amalgam.h"
#include "juce_audio_basics_amalgam.h"
#include "juce_audio_formats_amalgam.h"

/** Config: JUCE_ASIO
	Enables ASIO audio devices (MS Windows only).
	Turning this on means that you'll need to have the Steinberg ASIO SDK installed
	on your Windows build machine.

	See the comments in the ASIOAudioIODevice class's header file for more
	info about this.
*/
#ifndef JUCE_ASIO
 #define JUCE_ASIO 0
#endif

/** Config: JUCE_WASAPI
	Enables WASAPI audio devices (Windows Vista and above).
*/
#ifndef JUCE_WASAPI
 #define JUCE_WASAPI 1
#endif

/** Config: JUCE_DIRECTSOUND
	Enables DirectSound audio (MS Windows only).
*/
#ifndef JUCE_DIRECTSOUND
 #define JUCE_DIRECTSOUND 1
#endif

/** Config: JUCE_ALSA
	Enables ALSA audio devices (Linux only).
*/
#ifndef JUCE_ALSA
 #define JUCE_ALSA 1
#endif

/** Config: JUCE_JACK
	Enables JACK audio devices (Linux only).
*/
#ifndef JUCE_JACK
 #define JUCE_JACK 0
#endif

/** Config: JUCE_USE_ANDROID_OPENSLES
	Enables OpenSLES devices (Android only).
*/
#ifndef JUCE_USE_ANDROID_OPENSLES
 #if JUCE_ANDROID_API_VERSION > 8
  #define JUCE_USE_ANDROID_OPENSLES 1
 #else
  #define JUCE_USE_ANDROID_OPENSLES 0
 #endif
#endif

/** Config: JUCE_USE_CDREADER
	Enables the AudioCDReader class (on supported platforms).
*/
#ifndef JUCE_USE_CDREADER
 #define JUCE_USE_CDREADER 0
#endif

/** Config: JUCE_USE_CDBURNER
	Enables the AudioCDBurner class (on supported platforms).
*/
#ifndef JUCE_USE_CDBURNER
 #define JUCE_USE_CDBURNER 0
#endif

namespace juce
{

// START_AUTOINCLUDE audio_io, midi_io, sources, audio_cd
#ifndef __JUCE_AUDIODEVICEMANAGER_JUCEHEADER__

/*** Start of inlined file: juce_AudioDeviceManager.h ***/
#ifndef __JUCE_AUDIODEVICEMANAGER_JUCEHEADER__
#define __JUCE_AUDIODEVICEMANAGER_JUCEHEADER__


/*** Start of inlined file: juce_AudioIODeviceType.h ***/
#ifndef __JUCE_AUDIOIODEVICETYPE_JUCEHEADER__
#define __JUCE_AUDIOIODEVICETYPE_JUCEHEADER__


/*** Start of inlined file: juce_AudioIODevice.h ***/
#ifndef __JUCE_AUDIOIODEVICE_JUCEHEADER__
#define __JUCE_AUDIOIODEVICE_JUCEHEADER__

class AudioIODevice;

/**
	One of these is passed to an AudioIODevice object to stream the audio data
	in and out.

	The AudioIODevice will repeatedly call this class's audioDeviceIOCallback()
	method on its own high-priority audio thread, when it needs to send or receive
	the next block of data.

	@see AudioIODevice, AudioDeviceManager
*/
class JUCE_API  AudioIODeviceCallback
{
public:
	/** Destructor. */
	virtual ~AudioIODeviceCallback()  {}

	/** Processes a block of incoming and outgoing audio data.

		The subclass's implementation should use the incoming audio for whatever
		purposes it needs to, and must fill all the output channels with the next
		block of output data before returning.

		The channel data is arranged with the same array indices as the channel name
		array returned by AudioIODevice::getOutputChannelNames(), but those channels
		that aren't specified in AudioIODevice::open() will have a null pointer for their
		associated channel, so remember to check for this.

		@param inputChannelData     a set of arrays containing the audio data for each
									incoming channel - this data is valid until the function
									returns. There will be one channel of data for each input
									channel that was enabled when the audio device was opened
									(see AudioIODevice::open())
		@param numInputChannels     the number of pointers to channel data in the
									inputChannelData array.
		@param outputChannelData    a set of arrays which need to be filled with the data
									that should be sent to each outgoing channel of the device.
									There will be one channel of data for each output channel
									that was enabled when the audio device was opened (see
									AudioIODevice::open())
									The initial contents of the array is undefined, so the
									callback function must fill all the channels with zeros if
									its output is silence. Failing to do this could cause quite
									an unpleasant noise!
		@param numOutputChannels    the number of pointers to channel data in the
									outputChannelData array.
		@param numSamples           the number of samples in each channel of the input and
									output arrays. The number of samples will depend on the
									audio device's buffer size and will usually remain constant,
									although this isn't guaranteed, so make sure your code can
									cope with reasonable changes in the buffer size from one
									callback to the next.
	*/
	virtual void audioDeviceIOCallback (const float** inputChannelData,
										int numInputChannels,
										float** outputChannelData,
										int numOutputChannels,
										int numSamples) = 0;

	/** Called to indicate that the device is about to start calling back.

		This will be called just before the audio callbacks begin, either when this
		callback has just been added to an audio device, or after the device has been
		restarted because of a sample-rate or block-size change.

		You can use this opportunity to find out the sample rate and block size
		that the device is going to use by calling the AudioIODevice::getCurrentSampleRate()
		and AudioIODevice::getCurrentBufferSizeSamples() on the supplied pointer.

		@param device       the audio IO device that will be used to drive the callback.
							Note that if you're going to store this this pointer, it is
							only valid until the next time that audioDeviceStopped is called.
	*/
	virtual void audioDeviceAboutToStart (AudioIODevice* device) = 0;

	/** Called to indicate that the device has stopped. */
	virtual void audioDeviceStopped() = 0;

	/** This can be overridden to be told if the device generates an error while operating.
		Be aware that this could be called by any thread! And not all devices perform
		this callback.
	*/
	virtual void audioDeviceError (const String& errorMessage);
};

/**
	Base class for an audio device with synchronised input and output channels.

	Subclasses of this are used to implement different protocols such as DirectSound,
	ASIO, CoreAudio, etc.

	To create one of these, you'll need to use the AudioIODeviceType class - see the
	documentation for that class for more info.

	For an easier way of managing audio devices and their settings, have a look at the
	AudioDeviceManager class.

	@see AudioIODeviceType, AudioDeviceManager
*/
class JUCE_API  AudioIODevice
{
public:
	/** Destructor. */
	virtual ~AudioIODevice();

	/** Returns the device's name, (as set in the constructor). */
	const String& getName() const noexcept                          { return name; }

	/** Returns the type of the device.

		E.g. "CoreAudio", "ASIO", etc. - this comes from the AudioIODeviceType that created it.
	*/
	const String& getTypeName() const noexcept                      { return typeName; }

	/** Returns the names of all the available output channels on this device.
		To find out which of these are currently in use, call getActiveOutputChannels().
	*/
	virtual StringArray getOutputChannelNames() = 0;

	/** Returns the names of all the available input channels on this device.
		To find out which of these are currently in use, call getActiveInputChannels().
	*/
	virtual StringArray getInputChannelNames() = 0;

	/** Returns the number of sample-rates this device supports.

		To find out which rates are available on this device, use this method to
		find out how many there are, and getSampleRate() to get the rates.

		@see getSampleRate
	*/
	virtual int getNumSampleRates() = 0;

	/** Returns one of the sample-rates this device supports.

		To find out which rates are available on this device, use getNumSampleRates() to
		find out how many there are, and getSampleRate() to get the individual rates.

		The sample rate is set by the open() method.

		(Note that for DirectSound some rates might not work, depending on combinations
		of i/o channels that are being opened).

		@see getNumSampleRates
	*/
	virtual double getSampleRate (int index) = 0;

	/** Returns the number of sizes of buffer that are available.

		@see getBufferSizeSamples, getDefaultBufferSize
	*/
	virtual int getNumBufferSizesAvailable() = 0;

	/** Returns one of the possible buffer-sizes.

		@param index    the index of the buffer-size to use, from 0 to getNumBufferSizesAvailable() - 1
		@returns a number of samples
		@see getNumBufferSizesAvailable, getDefaultBufferSize
	*/
	virtual int getBufferSizeSamples (int index) = 0;

	/** Returns the default buffer-size to use.

		@returns a number of samples
		@see getNumBufferSizesAvailable, getBufferSizeSamples
	*/
	virtual int getDefaultBufferSize() = 0;

	/** Tries to open the device ready to play.

		@param inputChannels        a BigInteger in which a set bit indicates that the corresponding
									input channel should be enabled
		@param outputChannels       a BigInteger in which a set bit indicates that the corresponding
									output channel should be enabled
		@param sampleRate           the sample rate to try to use - to find out which rates are
									available, see getNumSampleRates() and getSampleRate()
		@param bufferSizeSamples    the size of i/o buffer to use - to find out the available buffer
									sizes, see getNumBufferSizesAvailable() and getBufferSizeSamples()
		@returns    an error description if there's a problem, or an empty string if it succeeds in
					opening the device
		@see close
	*/
	virtual String open (const BigInteger& inputChannels,
						 const BigInteger& outputChannels,
						 double sampleRate,
						 int bufferSizeSamples) = 0;

	/** Closes and releases the device if it's open. */
	virtual void close() = 0;

	/** Returns true if the device is still open.

		A device might spontaneously close itself if something goes wrong, so this checks if
		it's still open.
	*/
	virtual bool isOpen() = 0;

	/** Starts the device actually playing.

		This must be called after the device has been opened.

		@param callback     the callback to use for streaming the data.
		@see AudioIODeviceCallback, open
	*/
	virtual void start (AudioIODeviceCallback* callback) = 0;

	/** Stops the device playing.

		Once a device has been started, this will stop it. Any pending calls to the
		callback class will be flushed before this method returns.
	*/
	virtual void stop() = 0;

	/** Returns true if the device is still calling back.

		The device might mysteriously stop, so this checks whether it's
		still playing.
	*/
	virtual bool isPlaying() = 0;

	/** Returns the last error that happened if anything went wrong. */
	virtual String getLastError() = 0;

	/** Returns the buffer size that the device is currently using.

		If the device isn't actually open, this value doesn't really mean much.
	*/
	virtual int getCurrentBufferSizeSamples() = 0;

	/** Returns the sample rate that the device is currently using.

		If the device isn't actually open, this value doesn't really mean much.
	*/
	virtual double getCurrentSampleRate() = 0;

	/** Returns the device's current physical bit-depth.

		If the device isn't actually open, this value doesn't really mean much.
	*/
	virtual int getCurrentBitDepth() = 0;

	/** Returns a mask showing which of the available output channels are currently
		enabled.
		@see getOutputChannelNames
	*/
	virtual BigInteger getActiveOutputChannels() const = 0;

	/** Returns a mask showing which of the available input channels are currently
		enabled.
		@see getInputChannelNames
	*/
	virtual BigInteger getActiveInputChannels() const = 0;

	/** Returns the device's output latency.

		This is the delay in samples between a callback getting a block of data, and
		that data actually getting played.
	*/
	virtual int getOutputLatencyInSamples() = 0;

	/** Returns the device's input latency.

		This is the delay in samples between some audio actually arriving at the soundcard,
		and the callback getting passed this block of data.
	*/
	virtual int getInputLatencyInSamples() = 0;

	/** True if this device can show a pop-up control panel for editing its settings.

		This is generally just true of ASIO devices. If true, you can call showControlPanel()
		to display it.
	*/
	virtual bool hasControlPanel() const;

	/** Shows a device-specific control panel if there is one.

		This should only be called for devices which return true from hasControlPanel().
	*/
	virtual bool showControlPanel();

protected:
	/** Creates a device, setting its name and type member variables. */
	AudioIODevice (const String& deviceName,
				   const String& typeName);

	/** @internal */
	String name, typeName;
};

#endif   // __JUCE_AUDIOIODEVICE_JUCEHEADER__

/*** End of inlined file: juce_AudioIODevice.h ***/

class AudioDeviceManager;

/**
	Represents a type of audio driver, such as DirectSound, ASIO, CoreAudio, etc.

	To get a list of available audio driver types, use the AudioDeviceManager::createAudioDeviceTypes()
	method. Each of the objects returned can then be used to list the available
	devices of that type. E.g.
	@code
	OwnedArray <AudioIODeviceType> types;
	myAudioDeviceManager.createAudioDeviceTypes (types);

	for (int i = 0; i < types.size(); ++i)
	{
		String typeName (types[i]->getTypeName());  // This will be things like "DirectSound", "CoreAudio", etc.

		types[i]->scanForDevices();                 // This must be called before getting the list of devices

		StringArray deviceNames (types[i]->getDeviceNames());  // This will now return a list of available devices of this type

		for (int j = 0; j < deviceNames.size(); ++j)
		{
			AudioIODevice* device = types[i]->createDevice (deviceNames [j]);

			...
		}
	}
	@endcode

	For an easier way of managing audio devices and their settings, have a look at the
	AudioDeviceManager class.

	@see AudioIODevice, AudioDeviceManager
*/
class JUCE_API  AudioIODeviceType
{
public:

	/** Returns the name of this type of driver that this object manages.

		This will be something like "DirectSound", "ASIO", "CoreAudio", "ALSA", etc.
	*/
	const String& getTypeName() const noexcept                      { return typeName; }

	/** Refreshes the object's cached list of known devices.

		This must be called at least once before calling getDeviceNames() or any of
		the other device creation methods.
	*/
	virtual void scanForDevices() = 0;

	/** Returns the list of available devices of this type.

		The scanForDevices() method must have been called to create this list.

		@param wantInputNames     only really used by DirectSound where devices are split up
								  into inputs and outputs, this indicates whether to use
								  the input or output name to refer to a pair of devices.
	*/
	virtual StringArray getDeviceNames (bool wantInputNames = false) const = 0;

	/** Returns the name of the default device.

		This will be one of the names from the getDeviceNames() list.

		@param forInput     if true, this means that a default input device should be
							returned; if false, it should return the default output
	*/
	virtual int getDefaultDeviceIndex (bool forInput) const = 0;

	/** Returns the index of a given device in the list of device names.
		If asInput is true, it shows the index in the inputs list, otherwise it
		looks for it in the outputs list.
	*/
	virtual int getIndexOfDevice (AudioIODevice* device, bool asInput) const = 0;

	/** Returns true if two different devices can be used for the input and output.
	*/
	virtual bool hasSeparateInputsAndOutputs() const = 0;

	/** Creates one of the devices of this type.

		The deviceName must be one of the strings returned by getDeviceNames(), and
		scanForDevices() must have been called before this method is used.
	*/
	virtual AudioIODevice* createDevice (const String& outputDeviceName,
										 const String& inputDeviceName) = 0;

	/**
		A class for receiving events when audio devices are inserted or removed.

		You can register a AudioIODeviceType::Listener with an~AudioIODeviceType object
		using the AudioIODeviceType::addListener() method, and it will be called when
		devices of that type are added or removed.

		@see AudioIODeviceType::addListener, AudioIODeviceType::removeListener
	*/
	class Listener
	{
	public:
		virtual ~Listener() {}

		/** Called when the list of available audio devices changes. */
		virtual void audioDeviceListChanged() = 0;
	};

	/** Adds a listener that will be called when this type of device is added or
		removed from the system.
	*/
	void addListener (Listener* listener);

	/** Removes a listener that was previously added with addListener(). */
	void removeListener (Listener* listener);

	/** Destructor. */
	virtual ~AudioIODeviceType();

	/** Creates a CoreAudio device type if it's available on this platform, or returns null. */
	static AudioIODeviceType* createAudioIODeviceType_CoreAudio();
	/** Creates an iOS device type if it's available on this platform, or returns null. */
	static AudioIODeviceType* createAudioIODeviceType_iOSAudio();
	/** Creates a WASAPI device type if it's available on this platform, or returns null. */
	static AudioIODeviceType* createAudioIODeviceType_WASAPI();
	/** Creates a DirectSound device type if it's available on this platform, or returns null. */
	static AudioIODeviceType* createAudioIODeviceType_DirectSound();
	/** Creates an ASIO device type if it's available on this platform, or returns null. */
	static AudioIODeviceType* createAudioIODeviceType_ASIO();
	/** Creates an ALSA device type if it's available on this platform, or returns null. */
	static AudioIODeviceType* createAudioIODeviceType_ALSA();
	/** Creates a JACK device type if it's available on this platform, or returns null. */
	static AudioIODeviceType* createAudioIODeviceType_JACK();
	/** Creates an Android device type if it's available on this platform, or returns null. */
	static AudioIODeviceType* createAudioIODeviceType_Android();
	/** Creates an Android OpenSLES device type if it's available on this platform, or returns null. */
	static AudioIODeviceType* createAudioIODeviceType_OpenSLES();

protected:
	explicit AudioIODeviceType (const String& typeName);

	/** Synchronously calls all the registered device list change listeners. */
	void callDeviceChangeListeners();

private:
	String typeName;
	ListenerList<Listener> listeners;

	JUCE_DECLARE_NON_COPYABLE (AudioIODeviceType);
};

#endif   // __JUCE_AUDIOIODEVICETYPE_JUCEHEADER__

/*** End of inlined file: juce_AudioIODeviceType.h ***/


/*** Start of inlined file: juce_MidiInput.h ***/
#ifndef __JUCE_MIDIINPUT_JUCEHEADER__
#define __JUCE_MIDIINPUT_JUCEHEADER__

class MidiInput;

/**
	Receives incoming messages from a physical MIDI input device.

	This class is overridden to handle incoming midi messages. See the MidiInput
	class for more details.

	@see MidiInput
*/
class JUCE_API  MidiInputCallback
{
public:
	/** Destructor. */
	virtual ~MidiInputCallback()  {}

	/** Receives an incoming message.

		A MidiInput object will call this method when a midi event arrives. It'll be
		called on a high-priority system thread, so avoid doing anything time-consuming
		in here, and avoid making any UI calls. You might find the MidiBuffer class helpful
		for queueing incoming messages for use later.

		@param source   the MidiInput object that generated the message
		@param message  the incoming message. The message's timestamp is set to a value
						equivalent to (Time::getMillisecondCounter() / 1000.0) to specify the
						time when the message arrived.
	*/
	virtual void handleIncomingMidiMessage (MidiInput* source,
											const MidiMessage& message) = 0;

	/** Notification sent each time a packet of a multi-packet sysex message arrives.

		If a long sysex message is broken up into multiple packets, this callback is made
		for each packet that arrives until the message is finished, at which point
		the normal handleIncomingMidiMessage() callback will be made with the entire
		message.

		The message passed in will contain the start of a sysex, but won't be finished
		with the terminating 0xf7 byte.
	*/
	virtual void handlePartialSysexMessage (MidiInput* source,
											const uint8* messageData,
											const int numBytesSoFar,
											const double timestamp)
	{
		// (this bit is just to avoid compiler warnings about unused variables)
		(void) source; (void) messageData; (void) numBytesSoFar; (void) timestamp;
	}
};

/**
	Represents a midi input device.

	To create one of these, use the static getDevices() method to find out what inputs are
	available, and then use the openDevice() method to try to open one.

	@see MidiOutput
*/
class JUCE_API  MidiInput
{
public:

	/** Returns a list of the available midi input devices.

		You can open one of the devices by passing its index into the
		openDevice() method.

		@see getDefaultDeviceIndex, openDevice
	*/
	static StringArray getDevices();

	/** Returns the index of the default midi input device to use.

		This refers to the index in the list returned by getDevices().
	*/
	static int getDefaultDeviceIndex();

	/** Tries to open one of the midi input devices.

		This will return a MidiInput object if it manages to open it. You can then
		call start() and stop() on this device, and delete it when no longer needed.

		If the device can't be opened, this will return a null pointer.

		@param deviceIndex  the index of a device from the list returned by getDevices()
		@param callback     the object that will receive the midi messages from this device.

		@see MidiInputCallback, getDevices
	*/
	static MidiInput* openDevice (int deviceIndex,
								  MidiInputCallback* callback);

   #if JUCE_LINUX || JUCE_MAC || JUCE_IOS || DOXYGEN
	/** This will try to create a new midi input device (Not available on Windows).

		This will attempt to create a new midi input device with the specified name,
		for other apps to connect to.

		Returns 0 if a device can't be created.

		@param deviceName   the name to use for the new device
		@param callback     the object that will receive the midi messages from this device.
	*/
	static MidiInput* createNewDevice (const String& deviceName,
									   MidiInputCallback* callback);
   #endif

	/** Destructor. */
	virtual ~MidiInput();

	/** Returns the name of this device. */
	const String& getName() const noexcept                      { return name; }

	/** Allows you to set a custom name for the device, in case you don't like the name
		it was given when created.
	*/
	void setName (const String& newName) noexcept               { name = newName; }

	/** Starts the device running.

		After calling this, the device will start sending midi messages to the
		MidiInputCallback object that was specified when the openDevice() method
		was called.

		@see stop
	*/
	virtual void start();

	/** Stops the device running.

		@see start
	*/
	virtual void stop();

protected:

	String name;
	void* internal;

	explicit MidiInput (const String& name);

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiInput);
};

#endif   // __JUCE_MIDIINPUT_JUCEHEADER__

/*** End of inlined file: juce_MidiInput.h ***/


/*** Start of inlined file: juce_MidiOutput.h ***/
#ifndef __JUCE_MIDIOUTPUT_JUCEHEADER__
#define __JUCE_MIDIOUTPUT_JUCEHEADER__

/**
	Controls a physical MIDI output device.

	To create one of these, use the static getDevices() method to get a list of the
	available output devices, then use the openDevice() method to try to open one.

	@see MidiInput
*/
class JUCE_API  MidiOutput  : private Thread
{
public:

	/** Returns a list of the available midi output devices.

		You can open one of the devices by passing its index into the
		openDevice() method.

		@see getDefaultDeviceIndex, openDevice
	*/
	static StringArray getDevices();

	/** Returns the index of the default midi output device to use.

		This refers to the index in the list returned by getDevices().
	*/
	static int getDefaultDeviceIndex();

	/** Tries to open one of the midi output devices.

		This will return a MidiOutput object if it manages to open it. You can then
		send messages to this device, and delete it when no longer needed.

		If the device can't be opened, this will return a null pointer.

		@param deviceIndex  the index of a device from the list returned by getDevices()
		@see getDevices
	*/
	static MidiOutput* openDevice (int deviceIndex);

   #if JUCE_LINUX || JUCE_MAC || JUCE_IOS || DOXYGEN
	/** This will try to create a new midi output device (Not available on Windows).

		This will attempt to create a new midi output device that other apps can connect
		to and use as their midi input.

		Returns 0 if a device can't be created.

		@param deviceName   the name to use for the new device
	*/
	static MidiOutput* createNewDevice (const String& deviceName);
   #endif

	/** Destructor. */
	virtual ~MidiOutput();

	/** Makes this device output a midi message.

		@see MidiMessage
	*/
	virtual void sendMessageNow (const MidiMessage& message);

	/** This lets you supply a block of messages that will be sent out at some point
		in the future.

		The MidiOutput class has an internal thread that can send out timestamped
		messages - this appends a set of messages to its internal buffer, ready for
		sending.

		This will only work if you've already started the thread with startBackgroundThread().

		A time is supplied, at which the block of messages should be sent. This time uses
		the same time base as Time::getMillisecondCounter(), and must be in the future.

		The samplesPerSecondForBuffer parameter indicates the number of samples per second
		used by the MidiBuffer. Each event in a MidiBuffer has a sample position, and the
		samplesPerSecondForBuffer value is needed to convert this sample position to a
		real time.
	*/
	virtual void sendBlockOfMessages (const MidiBuffer& buffer,
									  double millisecondCounterToStartAt,
									  double samplesPerSecondForBuffer);

	/** Gets rid of any midi messages that had been added by sendBlockOfMessages().
	*/
	virtual void clearAllPendingMessages();

	/** Starts up a background thread so that the device can send blocks of data.

		Call this to get the device ready, before using sendBlockOfMessages().
	*/
	virtual void startBackgroundThread();

	/** Stops the background thread, and clears any pending midi events.

		@see startBackgroundThread
	*/
	virtual void stopBackgroundThread();

protected:

	void* internal;
	CriticalSection lock;
	struct PendingMessage;
	PendingMessage* firstMessage;

	MidiOutput();
	void run();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiOutput);
};

#endif   // __JUCE_MIDIOUTPUT_JUCEHEADER__

/*** End of inlined file: juce_MidiOutput.h ***/

/**
	Manages the state of some audio and midi i/o devices.

	This class keeps tracks of a currently-selected audio device, through
	with which it continuously streams data from an audio callback, as well as
	one or more midi inputs.

	The idea is that your application will create one global instance of this object,
	and let it take care of creating and deleting specific types of audio devices
	internally. So when the device is changed, your callbacks will just keep running
	without having to worry about this.

	The manager can save and reload all of its device settings as XML, which
	makes it very easy for you to save and reload the audio setup of your
	application.

	And to make it easy to let the user change its settings, there's a component
	to do just that - the AudioDeviceSelectorComponent class, which contains a set of
	device selection/sample-rate/latency controls.

	To use an AudioDeviceManager, create one, and use initialise() to set it up. Then
	call addAudioCallback() to register your audio callback with it, and use that to process
	your audio data.

	The manager also acts as a handy hub for incoming midi messages, allowing a
	listener to register for messages from either a specific midi device, or from whatever
	the current default midi input device is. The listener then doesn't have to worry about
	re-registering with different midi devices if they are changed or deleted.

	And yet another neat trick is that amount of CPU time being used is measured and
	available with the getCpuUsage() method.

	The AudioDeviceManager is a ChangeBroadcaster, and will send a change message to
	listeners whenever one of its settings is changed.

	@see AudioDeviceSelectorComponent, AudioIODevice, AudioIODeviceType
*/
class JUCE_API  AudioDeviceManager  : public ChangeBroadcaster
{
public:

	/** Creates a default AudioDeviceManager.

		Initially no audio device will be selected. You should call the initialise() method
		and register an audio callback with setAudioCallback() before it'll be able to
		actually make any noise.
	*/
	AudioDeviceManager();

	/** Destructor. */
	~AudioDeviceManager();

	/**
		This structure holds a set of properties describing the current audio setup.

		An AudioDeviceManager uses this class to save/load its current settings, and to
		specify your preferred options when opening a device.

		@see AudioDeviceManager::setAudioDeviceSetup(), AudioDeviceManager::initialise()
	*/
	struct JUCE_API  AudioDeviceSetup
	{
		/** Creates an AudioDeviceSetup object.

			The default constructor sets all the member variables to indicate default values.
			You can then fill-in any values you want to before passing the object to
			AudioDeviceManager::initialise().
		*/
		AudioDeviceSetup();

		bool operator== (const AudioDeviceSetup& other) const;

		/** The name of the audio device used for output.
			The name has to be one of the ones listed by the AudioDeviceManager's currently
			selected device type.
			This may be the same as the input device.
			An empty string indicates the default device.
		*/
		String outputDeviceName;

		/** The name of the audio device used for input.
			This may be the same as the output device.
			An empty string indicates the default device.
		*/
		String inputDeviceName;

		/** The current sample rate.
			This rate is used for both the input and output devices.
			A value of 0 indicates the default rate.
		*/
		double sampleRate;

		/** The buffer size, in samples.
			This buffer size is used for both the input and output devices.
			A value of 0 indicates the default buffer size.
		*/
		int bufferSize;

		/** The set of active input channels.
			The bits that are set in this array indicate the channels of the
			input device that are active.
			If useDefaultInputChannels is true, this value is ignored.
		*/
		BigInteger inputChannels;

		/** If this is true, it indicates that the inputChannels array
			should be ignored, and instead, the device's default channels
			should be used.
		*/
		bool useDefaultInputChannels;

		/** The set of active output channels.
			The bits that are set in this array indicate the channels of the
			input device that are active.
			If useDefaultOutputChannels is true, this value is ignored.
		*/
		BigInteger outputChannels;

		/** If this is true, it indicates that the outputChannels array
			should be ignored, and instead, the device's default channels
			should be used.
		*/
		bool useDefaultOutputChannels;
	};

	/** Opens a set of audio devices ready for use.

		This will attempt to open either a default audio device, or one that was
		previously saved as XML.

		@param numInputChannelsNeeded       a minimum number of input channels needed
											by your app.
		@param numOutputChannelsNeeded      a minimum number of output channels to open
		@param savedState                   either a previously-saved state that was produced
											by createStateXml(), or 0 if you want the manager
											to choose the best device to open.
		@param selectDefaultDeviceOnFailure if true, then if the device specified in the XML
											fails to open, then a default device will be used
											instead. If false, then on failure, no device is
											opened.
		@param preferredDefaultDeviceName   if this is not empty, and there's a device with this
											name, then that will be used as the default device
											(assuming that there wasn't one specified in the XML).
											The string can actually be a simple wildcard, containing "*"
											and "?" characters
		@param preferredSetupOptions        if this is non-null, the structure will be used as the
											set of preferred settings when opening the device. If you
											use this parameter, the preferredDefaultDeviceName
											field will be ignored

		@returns an error message if anything went wrong, or an empty string if it worked ok.
	*/
	String initialise (int numInputChannelsNeeded,
					   int numOutputChannelsNeeded,
					   const XmlElement* savedState,
					   bool selectDefaultDeviceOnFailure,
					   const String& preferredDefaultDeviceName = String::empty,
					   const AudioDeviceSetup* preferredSetupOptions = 0);

	/** Returns some XML representing the current state of the manager.

		This stores the current device, its samplerate, block size, etc, and
		can be restored later with initialise().

		Note that this can return a null pointer if no settings have been explicitly changed
		(i.e. if the device manager has just been left in its default state).
	*/
	XmlElement* createStateXml() const;

	/** Returns the current device properties that are in use.

		@see setAudioDeviceSetup
	*/
	void getAudioDeviceSetup (AudioDeviceSetup& setup);

	/** Changes the current device or its settings.

		If you want to change a device property, like the current sample rate or
		block size, you can call getAudioDeviceSetup() to retrieve the current
		settings, then tweak the appropriate fields in the AudioDeviceSetup structure,
		and pass it back into this method to apply the new settings.

		@param newSetup             the settings that you'd like to use
		@param treatAsChosenDevice  if this is true and if the device opens correctly, these new
									settings will be taken as having been explicitly chosen by the
									user, and the next time createStateXml() is called, these settings
									will be returned. If it's false, then the device is treated as a
									temporary or default device, and a call to createStateXml() will
									return either the last settings that were made with treatAsChosenDevice
									as true, or the last XML settings that were passed into initialise().
		@returns an error message if anything went wrong, or an empty string if it worked ok.

		@see getAudioDeviceSetup
	*/
	String setAudioDeviceSetup (const AudioDeviceSetup& newSetup,
								bool treatAsChosenDevice);

	/** Returns the currently-active audio device. */
	AudioIODevice* getCurrentAudioDevice() const noexcept               { return currentAudioDevice; }

	/** Returns the type of audio device currently in use.
		@see setCurrentAudioDeviceType
	*/
	String getCurrentAudioDeviceType() const                            { return currentDeviceType; }

	/** Returns the currently active audio device type object.
		Don't keep a copy of this pointer - it's owned by the device manager and could
		change at any time.
	*/
	AudioIODeviceType* getCurrentDeviceTypeObject() const;

	/** Changes the class of audio device being used.

		This switches between, e.g. ASIO and DirectSound. On the Mac you probably won't ever call
		this because there's only one type: CoreAudio.

		For a list of types, see getAvailableDeviceTypes().
	*/
	void setCurrentAudioDeviceType (const String& type,
									bool treatAsChosenDevice);

	/** Closes the currently-open device.

		You can call restartLastAudioDevice() later to reopen it in the same state
		that it was just in.
	*/
	void closeAudioDevice();

	/** Tries to reload the last audio device that was running.

		Note that this only reloads the last device that was running before
		closeAudioDevice() was called - it doesn't reload any kind of saved-state,
		and can only be called after a device has been opened with SetAudioDevice().

		If a device is already open, this call will do nothing.
	*/
	void restartLastAudioDevice();

	/** Registers an audio callback to be used.

		The manager will redirect callbacks from whatever audio device is currently
		in use to all registered callback objects. If more than one callback is
		active, they will all be given the same input data, and their outputs will
		be summed.

		If necessary, this method will invoke audioDeviceAboutToStart() on the callback
		object before returning.

		To remove a callback, use removeAudioCallback().
	*/
	void addAudioCallback (AudioIODeviceCallback* newCallback);

	/** Deregisters a previously added callback.

		If necessary, this method will invoke audioDeviceStopped() on the callback
		object before returning.

		@see addAudioCallback
	*/
	void removeAudioCallback (AudioIODeviceCallback* callback);

	/** Returns the average proportion of available CPU being spent inside the audio callbacks.

		Returns a value between 0 and 1.0
	*/
	double getCpuUsage() const;

	/** Enables or disables a midi input device.

		The list of devices can be obtained with the MidiInput::getDevices() method.

		Any incoming messages from enabled input devices will be forwarded on to all the
		listeners that have been registered with the addMidiInputCallback() method. They
		can either register for messages from a particular device, or from just the
		"default" midi input.

		Routing the midi input via an AudioDeviceManager means that when a listener
		registers for the default midi input, this default device can be changed by the
		manager without the listeners having to know about it or re-register.

		It also means that a listener can stay registered for a midi input that is disabled
		or not present, so that when the input is re-enabled, the listener will start
		receiving messages again.

		@see addMidiInputCallback, isMidiInputEnabled
	*/
	void setMidiInputEnabled (const String& midiInputDeviceName, bool enabled);

	/** Returns true if a given midi input device is being used.

		@see setMidiInputEnabled
	*/
	bool isMidiInputEnabled (const String& midiInputDeviceName) const;

	/** Registers a listener for callbacks when midi events arrive from a midi input.

		The device name can be empty to indicate that it wants events from whatever the
		current "default" device is. Or it can be the name of one of the midi input devices
		(see MidiInput::getDevices() for the names).

		Only devices which are enabled (see the setMidiInputEnabled() method) will have their
		events forwarded on to listeners.
	*/
	void addMidiInputCallback (const String& midiInputDeviceName,
							   MidiInputCallback* callback);

	/** Removes a listener that was previously registered with addMidiInputCallback().
	*/
	void removeMidiInputCallback (const String& midiInputDeviceName,
								  MidiInputCallback* callback);

	/** Sets a midi output device to use as the default.

		The list of devices can be obtained with the MidiOutput::getDevices() method.

		The specified device will be opened automatically and can be retrieved with the
		getDefaultMidiOutput() method.

		Pass in an empty string to deselect all devices. For the default device, you
		can use MidiOutput::getDevices() [MidiOutput::getDefaultDeviceIndex()].

		@see getDefaultMidiOutput, getDefaultMidiOutputName
	*/
	void setDefaultMidiOutput (const String& deviceName);

	/** Returns the name of the default midi output.

		@see setDefaultMidiOutput, getDefaultMidiOutput
	*/
	String getDefaultMidiOutputName() const                         { return defaultMidiOutputName; }

	/** Returns the current default midi output device.

		If no device has been selected, or the device can't be opened, this will
		return 0.

		@see getDefaultMidiOutputName
	*/
	MidiOutput* getDefaultMidiOutput() const noexcept               { return defaultMidiOutput; }

	/** Returns a list of the types of device supported.
	*/
	const OwnedArray <AudioIODeviceType>& getAvailableDeviceTypes();

	/** Creates a list of available types.

		This will add a set of new AudioIODeviceType objects to the specified list, to
		represent each available types of device.

		You can override this if your app needs to do something specific, like avoid
		using DirectSound devices, etc.
	*/
	virtual void createAudioDeviceTypes (OwnedArray <AudioIODeviceType>& types);

	/** Plays a beep through the current audio device.

		This is here to allow the audio setup UI panels to easily include a "test"
		button so that the user can check where the audio is coming from.
	*/
	void playTestSound();

	/** Turns on level-measuring.

		When enabled, the device manager will measure the peak input level
		across all channels, and you can get this level by calling getCurrentInputLevel().

		This is mainly intended for audio setup UI panels to use to create a mic
		level display, so that the user can check that they've selected the right
		device.

		A simple filter is used to make the level decay smoothly, but this is
		only intended for giving rough feedback, and not for any kind of accurate
		measurement.
	*/
	void enableInputLevelMeasurement (bool enableMeasurement);

	/** Returns the current input level.

		To use this, you must first enable it by calling enableInputLevelMeasurement().

		See enableInputLevelMeasurement() for more info.
	*/
	double getCurrentInputLevel() const;

	/** Returns the a lock that can be used to synchronise access to the audio callback.
		Obviously while this is locked, you're blocking the audio thread from running, so
		it must only be used for very brief periods when absolutely necessary.
	*/
	CriticalSection& getAudioCallbackLock() noexcept        { return audioCallbackLock; }

	/** Returns the a lock that can be used to synchronise access to the midi callback.
		Obviously while this is locked, you're blocking the midi system from running, so
		it must only be used for very brief periods when absolutely necessary.
	*/
	CriticalSection& getMidiCallbackLock() noexcept         { return midiCallbackLock; }

private:

	OwnedArray <AudioIODeviceType> availableDeviceTypes;
	OwnedArray <AudioDeviceSetup> lastDeviceTypeConfigs;

	AudioDeviceSetup currentSetup;
	ScopedPointer <AudioIODevice> currentAudioDevice;
	Array <AudioIODeviceCallback*> callbacks;
	int numInputChansNeeded, numOutputChansNeeded;
	String currentDeviceType;
	BigInteger inputChannels, outputChannels;
	ScopedPointer <XmlElement> lastExplicitSettings;
	mutable bool listNeedsScanning;
	bool useInputNames;
	int inputLevelMeasurementEnabledCount;
	double inputLevel;
	ScopedPointer <AudioSampleBuffer> testSound;
	int testSoundPosition;
	AudioSampleBuffer tempBuffer;

	StringArray midiInsFromXml;
	OwnedArray <MidiInput> enabledMidiInputs;
	Array <MidiInputCallback*> midiCallbacks;
	StringArray midiCallbackDevices;
	String defaultMidiOutputName;
	ScopedPointer <MidiOutput> defaultMidiOutput;
	CriticalSection audioCallbackLock, midiCallbackLock;

	double cpuUsageMs, timeToCpuScale;

	class CallbackHandler  : public AudioIODeviceCallback,
							 public MidiInputCallback,
							 public AudioIODeviceType::Listener
	{
	public:
		void audioDeviceIOCallback (const float**, int, float**, int, int);
		void audioDeviceAboutToStart (AudioIODevice*);
		void audioDeviceStopped();
		void handleIncomingMidiMessage (MidiInput*, const MidiMessage&);
		void audioDeviceListChanged();

		AudioDeviceManager* owner;
	};

	CallbackHandler callbackHandler;
	friend class CallbackHandler;

	void audioDeviceIOCallbackInt (const float** inputChannelData, int totalNumInputChannels,
								   float** outputChannelData, int totalNumOutputChannels, int numSamples);
	void audioDeviceAboutToStartInt (AudioIODevice*);
	void audioDeviceStoppedInt();
	void handleIncomingMidiMessageInt (MidiInput*, const MidiMessage&);
	void audioDeviceListChanged();

	String restartDevice (int blockSizeToUse, double sampleRateToUse,
						  const BigInteger& ins, const BigInteger& outs);
	void stopDevice();

	void updateXml();

	void createDeviceTypesIfNeeded();
	void scanDevicesIfNeeded();
	void deleteCurrentDevice();
	double chooseBestSampleRate (double preferred) const;
	int chooseBestBufferSize (int preferred) const;
	void insertDefaultDeviceNames (AudioDeviceSetup&) const;

	AudioIODeviceType* findType (const String& inputName, const String& outputName);
	AudioIODeviceType* findType (const String& typeName);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioDeviceManager);
};

#endif   // __JUCE_AUDIODEVICEMANAGER_JUCEHEADER__

/*** End of inlined file: juce_AudioDeviceManager.h ***/


#endif
#ifndef __JUCE_AUDIOIODEVICE_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOIODEVICETYPE_JUCEHEADER__

#endif
#ifndef __JUCE_MIDIINPUT_JUCEHEADER__

#endif
#ifndef __JUCE_MIDIMESSAGECOLLECTOR_JUCEHEADER__

/*** Start of inlined file: juce_MidiMessageCollector.h ***/
#ifndef __JUCE_MIDIMESSAGECOLLECTOR_JUCEHEADER__
#define __JUCE_MIDIMESSAGECOLLECTOR_JUCEHEADER__

/**
	Collects incoming realtime MIDI messages and turns them into blocks suitable for
	processing by a block-based audio callback.

	The class can also be used as either a MidiKeyboardStateListener or a MidiInputCallback
	so it can easily use a midi input or keyboard component as its source.

	@see MidiMessage, MidiInput
*/
class JUCE_API  MidiMessageCollector    : public MidiKeyboardStateListener,
										  public MidiInputCallback
{
public:

	/** Creates a MidiMessageCollector. */
	MidiMessageCollector();

	/** Destructor. */
	~MidiMessageCollector();

	/** Clears any messages from the queue.

		You need to call this method before starting to use the collector, so that
		it knows the correct sample rate to use.
	*/
	void reset (double sampleRate);

	/** Takes an incoming real-time message and adds it to the queue.

		The message's timestamp is taken, and it will be ready for retrieval as part
		of the block returned by the next call to removeNextBlockOfMessages().

		This method is fully thread-safe when overlapping calls are made with
		removeNextBlockOfMessages().
	*/
	void addMessageToQueue (const MidiMessage& message);

	/** Removes all the pending messages from the queue as a buffer.

		This will also correct the messages' timestamps to make sure they're in
		the range 0 to numSamples - 1.

		This call should be made regularly by something like an audio processing
		callback, because the time that it happens is used in calculating the
		midi event positions.

		This method is fully thread-safe when overlapping calls are made with
		addMessageToQueue().
	*/
	void removeNextBlockOfMessages (MidiBuffer& destBuffer, int numSamples);

	/** @internal */
	void handleNoteOn (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity);
	/** @internal */
	void handleNoteOff (MidiKeyboardState* source, int midiChannel, int midiNoteNumber);
	/** @internal */
	void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);

private:

	double lastCallbackTime;
	CriticalSection midiCallbackLock;
	MidiBuffer incomingMessages;
	double sampleRate;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiMessageCollector);
};

#endif   // __JUCE_MIDIMESSAGECOLLECTOR_JUCEHEADER__

/*** End of inlined file: juce_MidiMessageCollector.h ***/


#endif
#ifndef __JUCE_MIDIOUTPUT_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOSOURCEPLAYER_JUCEHEADER__

/*** Start of inlined file: juce_AudioSourcePlayer.h ***/
#ifndef __JUCE_AUDIOSOURCEPLAYER_JUCEHEADER__
#define __JUCE_AUDIOSOURCEPLAYER_JUCEHEADER__

/**
	Wrapper class to continuously stream audio from an audio source to an
	AudioIODevice.

	This object acts as an AudioIODeviceCallback, so can be attached to an
	output device, and will stream audio from an AudioSource.
*/
class JUCE_API  AudioSourcePlayer  : public AudioIODeviceCallback
{
public:

	/** Creates an empty AudioSourcePlayer. */
	AudioSourcePlayer();

	/** Destructor.

		Make sure this object isn't still being used by an AudioIODevice before
		deleting it!
	*/
	virtual ~AudioSourcePlayer();

	/** Changes the current audio source to play from.

		If the source passed in is already being used, this method will do nothing.
		If the source is not null, its prepareToPlay() method will be called
		before it starts being used for playback.

		If there's another source currently playing, its releaseResources() method
		will be called after it has been swapped for the new one.

		@param newSource                the new source to use - this will NOT be deleted
										by this object when no longer needed, so it's the
										caller's responsibility to manage it.
	*/
	void setSource (AudioSource* newSource);

	/** Returns the source that's playing.

		May return 0 if there's no source.
	*/
	AudioSource* getCurrentSource() const noexcept      { return source; }

	/** Sets a gain to apply to the audio data.
		@see getGain
	*/
	void setGain (float newGain) noexcept;

	/** Returns the current gain.
		@see setGain
	*/
	float getGain() const noexcept                      { return gain; }

	/** Implementation of the AudioIODeviceCallback method. */
	void audioDeviceIOCallback (const float** inputChannelData,
								int totalNumInputChannels,
								float** outputChannelData,
								int totalNumOutputChannels,
								int numSamples);

	/** Implementation of the AudioIODeviceCallback method. */
	void audioDeviceAboutToStart (AudioIODevice* device);

	/** Implementation of the AudioIODeviceCallback method. */
	void audioDeviceStopped();

private:

	CriticalSection readLock;
	AudioSource* source;
	double sampleRate;
	int bufferSize;
	float* channels [128];
	float* outputChans [128];
	const float* inputChans [128];
	AudioSampleBuffer tempBuffer;
	float lastGain, gain;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSourcePlayer);
};

#endif   // __JUCE_AUDIOSOURCEPLAYER_JUCEHEADER__

/*** End of inlined file: juce_AudioSourcePlayer.h ***/


#endif
#ifndef __JUCE_AUDIOTRANSPORTSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_AudioTransportSource.h ***/
#ifndef __JUCE_AUDIOTRANSPORTSOURCE_JUCEHEADER__
#define __JUCE_AUDIOTRANSPORTSOURCE_JUCEHEADER__

/**
	An AudioSource that takes a PositionableAudioSource and allows it to be
	played, stopped, started, etc.

	This can also be told use a buffer and background thread to read ahead, and
	if can correct for different sample-rates.

	You may want to use one of these along with an AudioSourcePlayer and AudioIODevice
	to control playback of an audio file.

	@see AudioSource, AudioSourcePlayer
*/
class JUCE_API  AudioTransportSource  : public PositionableAudioSource,
										public ChangeBroadcaster
{
public:

	/** Creates an AudioTransportSource.

		After creating one of these, use the setSource() method to select an input source.
	*/
	AudioTransportSource();

	/** Destructor. */
	~AudioTransportSource();

	/** Sets the reader that is being used as the input source.

		This will stop playback, reset the position to 0 and change to the new reader.

		The source passed in will not be deleted by this object, so must be managed by
		the caller.

		@param newSource                        the new input source to use. This may be zero
		@param readAheadBufferSize              a size of buffer to use for reading ahead. If this
												is zero, no reading ahead will be done; if it's
												greater than zero, a BufferingAudioSource will be used
												to do the reading-ahead. If you set a non-zero value here,
												you'll also need to set the readAheadThread parameter.
		@param readAheadThread                  if you set readAheadBufferSize to a non-zero value, then
												you'll also need to supply this TimeSliceThread object for
												the background reader to use. The thread object must not be
												deleted while the AudioTransport source is still using it.
		@param sourceSampleRateToCorrectFor     if this is non-zero, it specifies the sample
												rate of the source, and playback will be sample-rate
												adjusted to maintain playback at the correct pitch. If
												this is 0, no sample-rate adjustment will be performed
		@param maxNumChannels                   the maximum number of channels that may need to be played
	*/
	void setSource (PositionableAudioSource* newSource,
					int readAheadBufferSize = 0,
					TimeSliceThread* readAheadThread = nullptr,
					double sourceSampleRateToCorrectFor = 0.0,
					int maxNumChannels = 2);

	/** Changes the current playback position in the source stream.

		The next time the getNextAudioBlock() method is called, this
		is the time from which it'll read data.

		@see getPosition
	*/
	void setPosition (double newPosition);

	/** Returns the position that the next data block will be read from

		This is a time in seconds.
	*/
	double getCurrentPosition() const;

	/** Returns the stream's length in seconds. */
	double getLengthInSeconds() const;

	/** Returns true if the player has stopped because its input stream ran out of data.
	*/
	bool hasStreamFinished() const noexcept             { return inputStreamEOF; }

	/** Starts playing (if a source has been selected).

		If it starts playing, this will send a message to any ChangeListeners
		that are registered with this object.
	*/
	void start();

	/** Stops playing.

		If it's actually playing, this will send a message to any ChangeListeners
		that are registered with this object.
	*/
	void stop();

	/** Returns true if it's currently playing. */
	bool isPlaying() const noexcept     { return playing; }

	/** Changes the gain to apply to the output.

		@param newGain  a factor by which to multiply the outgoing samples,
						so 1.0 = 0dB, 0.5 = -6dB, 2.0 = 6dB, etc.
	*/
	void setGain (float newGain) noexcept;

	/** Returns the current gain setting.

		@see setGain
	*/
	float getGain() const noexcept      { return gain; }

	/** Implementation of the AudioSource method. */
	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);

	/** Implementation of the AudioSource method. */
	void releaseResources();

	/** Implementation of the AudioSource method. */
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

	/** Implements the PositionableAudioSource method. */
	void setNextReadPosition (int64 newPosition);

	/** Implements the PositionableAudioSource method. */
	int64 getNextReadPosition() const;

	/** Implements the PositionableAudioSource method. */
	int64 getTotalLength() const;

	/** Implements the PositionableAudioSource method. */
	bool isLooping() const;

private:

	PositionableAudioSource* source;
	ResamplingAudioSource* resamplerSource;
	BufferingAudioSource* bufferingSource;
	PositionableAudioSource* positionableSource;
	AudioSource* masterSource;

	CriticalSection callbackLock;
	float volatile gain, lastGain;
	bool volatile playing, stopped;
	double sampleRate, sourceSampleRate;
	int blockSize, readAheadBufferSize;
	bool isPrepared, inputStreamEOF;

	void releaseMasterResources();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioTransportSource);
};

#endif   // __JUCE_AUDIOTRANSPORTSOURCE_JUCEHEADER__

/*** End of inlined file: juce_AudioTransportSource.h ***/


#endif
#ifndef __JUCE_AUDIOCDBURNER_JUCEHEADER__

/*** Start of inlined file: juce_AudioCDBurner.h ***/
#ifndef __JUCE_AUDIOCDBURNER_JUCEHEADER__
#define __JUCE_AUDIOCDBURNER_JUCEHEADER__

#if JUCE_USE_CDBURNER || DOXYGEN

/**
*/
class AudioCDBurner     : public ChangeBroadcaster
{
public:

	/** Returns a list of available optical drives.

		Use openDevice() to open one of the items from this list.
	*/
	static StringArray findAvailableDevices();

	/** Tries to open one of the optical drives.

		The deviceIndex is an index into the array returned by findAvailableDevices().
	*/
	static AudioCDBurner* openDevice (const int deviceIndex);

	/** Destructor. */
	~AudioCDBurner();

	enum DiskState
	{
		unknown,                /**< An error condition, if the device isn't responding. */
		trayOpen,               /**< The drive is currently open. Note that a slot-loading drive
									 may seem to be permanently open. */
		noDisc,                 /**< The drive has no disk in it. */
		writableDiskPresent,    /**< The drive contains a writeable disk. */
		readOnlyDiskPresent     /**< The drive contains a read-only disk. */
	};

	/** Returns the current status of the device.

		To get informed when the drive's status changes, attach a ChangeListener to
		the AudioCDBurner.
	*/
	DiskState getDiskState() const;

	/** Returns true if there's a writable disk in the drive. */
	bool isDiskPresent() const;

	/** Sends an eject signal to the drive.
		The eject will happen asynchronously, so you can use getDiskState() and
		waitUntilStateChange() to monitor its progress.
	*/
	bool openTray();

	/** Blocks the current thread until the drive's state changes, or until the timeout expires.
		@returns    the device's new state
	*/
	DiskState waitUntilStateChange (int timeOutMilliseconds);

	/** Returns the set of possible write speeds that the device can handle.
		These are as a multiple of 'normal' speed, so e.g. '24x' returns 24, etc.
		Note that if there's no media present in the drive, this value may be unavailable!
		@see setWriteSpeed, getWriteSpeed
	*/
	Array<int> getAvailableWriteSpeeds() const;

	/** Tries to enable or disable buffer underrun safety on devices that support it.
		@returns    true if it's now enabled. If the device doesn't support it, this
					will always return false.
	*/
	bool setBufferUnderrunProtection (bool shouldBeEnabled);

	/** Returns the number of free blocks on the disk.

		There are 75 blocks per second, at 44100Hz.
	*/
	int getNumAvailableAudioBlocks() const;

	/** Adds a track to be written.

		The source passed-in here will be kept by this object, and it will
		be used and deleted at some point in the future, either during the
		burn() method or when this AudioCDBurner object is deleted. Your caller
		method shouldn't keep a reference to it or use it again after passing
		it in here.
	*/
	bool addAudioTrack (AudioSource* source, int numSamples);

	/** Receives progress callbacks during a cd-burn operation.
		@see AudioCDBurner::burn()
	*/
	class BurnProgressListener
	{
	public:
		BurnProgressListener() noexcept {}
		virtual ~BurnProgressListener() {}

		/** Called at intervals to report on the progress of the AudioCDBurner.

			To cancel the burn, return true from this method.
		*/
		virtual bool audioCDBurnProgress (float proportionComplete) = 0;
	};

	/** Runs the burn process.
		This method will block until the operation is complete.

		@param listener             the object to receive callbacks about progress
		@param ejectDiscAfterwards  whether to eject the disk after the burn completes
		@param performFakeBurnForTesting    if true, no data will actually be written to the disk
		@param writeSpeed           one of the write speeds from getAvailableWriteSpeeds(), or
									0 or less to mean the fastest speed.
	*/
	String burn (BurnProgressListener* listener,
				 bool ejectDiscAfterwards,
				 bool performFakeBurnForTesting,
				 int writeSpeed);

	/** If a burn operation is currently in progress, this tells it to stop
		as soon as possible.

		It's also possible to stop the burn process by returning true from
		BurnProgressListener::audioCDBurnProgress()
	*/
	void abortBurn();

private:

	AudioCDBurner (const int deviceIndex);

	class Pimpl;
	friend class ScopedPointer<Pimpl>;
	ScopedPointer<Pimpl> pimpl;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioCDBurner);
};

#endif
#endif   // __JUCE_AUDIOCDBURNER_JUCEHEADER__

/*** End of inlined file: juce_AudioCDBurner.h ***/


#endif
#ifndef __JUCE_AUDIOCDREADER_JUCEHEADER__

/*** Start of inlined file: juce_AudioCDReader.h ***/
#ifndef __JUCE_AUDIOCDREADER_JUCEHEADER__
#define __JUCE_AUDIOCDREADER_JUCEHEADER__

#if JUCE_USE_CDREADER || DOXYGEN

/**
	A type of AudioFormatReader that reads from an audio CD.

	One of these can be used to read a CD as if it's one big audio stream. Use the
	getPositionOfTrackStart() method to find where the individual tracks are
	within the stream.

	@see AudioFormatReader
*/
class JUCE_API  AudioCDReader  : public AudioFormatReader
{
public:

	/** Returns a list of names of Audio CDs currently available for reading.

		If there's a CD drive but no CD in it, this might return an empty list, or
		possibly a device that can be opened but which has no tracks, depending
		on the platform.

		@see createReaderForCD
	*/
	static StringArray getAvailableCDNames();

	/** Tries to create an AudioFormatReader that can read from an Audio CD.

		@param index    the index of one of the available CDs - use getAvailableCDNames()
						to find out how many there are.
		@returns        a new AudioCDReader object, or nullptr if it couldn't be created. The
						caller will be responsible for deleting the object returned.
	*/
	static AudioCDReader* createReaderForCD (const int index);

	/** Destructor. */
	~AudioCDReader();

	/** Implementation of the AudioFormatReader method. */
	bool readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
					  int64 startSampleInFile, int numSamples);

	/** Checks whether the CD has been removed from the drive. */
	bool isCDStillPresent() const;

	/** Returns the total number of tracks (audio + data). */
	int getNumTracks() const;

	/** Finds the sample offset of the start of a track.
		@param trackNum     the track number, where trackNum = 0 is the first track
							and trackNum = getNumTracks() means the end of the CD.
	*/
	int getPositionOfTrackStart (int trackNum) const;

	/** Returns true if a given track is an audio track.
		@param trackNum     the track number, where 0 is the first track.
	*/
	bool isTrackAudio (int trackNum) const;

	/** Returns an array of sample offsets for the start of each track, followed by
		the sample position of the end of the CD.
	*/
	const Array<int>& getTrackOffsets() const;

	/** Refreshes the object's table of contents.

		If the disc has been ejected and a different one put in since this
		object was created, this will cause it to update its idea of how many tracks
		there are, etc.
	*/
	void refreshTrackLengths();

	/** Enables scanning for indexes within tracks.
		@see getLastIndex
	*/
	void enableIndexScanning (bool enabled);

	/** Returns the index number found during the last read() call.

		Index scanning is turned off by default - turn it on with enableIndexScanning().

		Then when the read() method is called, if it comes across an index within that
		block, the index number is stored and returned by this method.

		Some devices might not support indexes, of course.

		(If you don't know what CD indexes are, it's unlikely you'll ever need them).

		@see enableIndexScanning
	*/
	int getLastIndex() const;

	/** Scans a track to find the position of any indexes within it.
		@param trackNumber  the track to look in, where 0 is the first track on the disc
		@returns    an array of sample positions of any index points found (not including
					the index that marks the start of the track)
	*/
	Array<int> findIndexesInTrack (const int trackNumber);

	/** Returns the CDDB id number for the CD.
		It's not a great way of identifying a disc, but it's traditional.
	*/
	int getCDDBId();

	/** Tries to eject the disk.
		Ejecting the disk might not actually be possible, e.g. if some other process is using it.
	*/
	void ejectDisk();

	enum
	{
		framesPerSecond = 75,
		samplesPerFrame = 44100 / framesPerSecond
	};

private:

	Array<int> trackStartSamples;

   #if JUCE_MAC
	File volumeDir;
	Array<File> tracks;
	int currentReaderTrack;
	ScopedPointer <AudioFormatReader> reader;
	AudioCDReader (const File& volume);

   #elif JUCE_WINDOWS
	bool audioTracks [100];
	void* handle;
	MemoryBlock buffer;
	bool indexingEnabled;
	int lastIndex, firstFrameInBuffer, samplesInBuffer;
	AudioCDReader (void* handle);
	int getIndexAt (int samplePos);

   #elif JUCE_LINUX
	AudioCDReader();
   #endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioCDReader);
};

#endif
#endif   // __JUCE_AUDIOCDREADER_JUCEHEADER__

/*** End of inlined file: juce_AudioCDReader.h ***/


#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_AUDIO_DEVICES_JUCEHEADER__

/*** End of inlined file: juce_audio_devices.h ***/

