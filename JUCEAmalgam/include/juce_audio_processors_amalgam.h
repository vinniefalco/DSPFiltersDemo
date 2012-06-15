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


/*** Start of inlined file: juce_audio_processors.h ***/
#ifndef __JUCE_AUDIO_PROCESSORS_JUCEHEADER__
#define __JUCE_AUDIO_PROCESSORS_JUCEHEADER__

#include "juce_gui_basics_amalgam.h"
#include "juce_audio_basics_amalgam.h"

/** Config: JUCE_PLUGINHOST_VST
	Enables the VST audio plugin hosting classes. This requires the Steinberg VST SDK to be
	installed on your machine.

	@see VSTPluginFormat, AudioPluginFormat, AudioPluginFormatManager, JUCE_PLUGINHOST_AU
*/
#ifndef JUCE_PLUGINHOST_VST
 #define JUCE_PLUGINHOST_VST 0
#endif

/** Config: JUCE_PLUGINHOST_AU
	Enables the AudioUnit plugin hosting classes. This is Mac-only, of course.

	@see AudioUnitPluginFormat, AudioPluginFormat, AudioPluginFormatManager, JUCE_PLUGINHOST_VST
*/
#ifndef JUCE_PLUGINHOST_AU
 #define JUCE_PLUGINHOST_AU 0
#endif

#if ! (JUCE_PLUGINHOST_AU || JUCE_PLUGINHOST_VST)
// #error "You need to set either the JUCE_PLUGINHOST_AU anr/or JUCE_PLUGINHOST_VST flags if you're using this module!"
#endif

#if ! (defined (JUCE_SUPPORT_CARBON) || JUCE_64BIT)
 #define JUCE_SUPPORT_CARBON 1
#endif

namespace juce
{

// START_AUTOINCLUDE processors, format, format_types, scanning
#ifndef __JUCE_AUDIOPLAYHEAD_JUCEHEADER__

/*** Start of inlined file: juce_AudioPlayHead.h ***/
#ifndef __JUCE_AUDIOPLAYHEAD_JUCEHEADER__
#define __JUCE_AUDIOPLAYHEAD_JUCEHEADER__

/**
	A subclass of AudioPlayHead can supply information about the position and
	status of a moving play head during audio playback.

	One of these can be supplied to an AudioProcessor object so that it can find
	out about the position of the audio that it is rendering.

	@see AudioProcessor::setPlayHead, AudioProcessor::getPlayHead
*/
class JUCE_API  AudioPlayHead
{
protected:

	AudioPlayHead() {}

public:
	virtual ~AudioPlayHead() {}

	/** Frame rate types. */
	enum FrameRateType
	{
		fps24           = 0,
		fps25           = 1,
		fps2997         = 2,
		fps30           = 3,
		fps2997drop     = 4,
		fps30drop       = 5,
		fpsUnknown      = 99
	};

	/** This structure is filled-in by the AudioPlayHead::getCurrentPosition() method.
	*/
	struct JUCE_API  CurrentPositionInfo
	{
		/** The tempo in BPM */
		double bpm;

		/** Time signature numerator, e.g. the 3 of a 3/4 time sig */
		int timeSigNumerator;
		/** Time signature denominator, e.g. the 4 of a 3/4 time sig */
		int timeSigDenominator;

		/** The current play position, in seconds from the start of the edit. */
		double timeInSeconds;

		/** For timecode, the position of the start of the edit, in seconds from 00:00:00:00. */
		double editOriginTime;

		/** The current play position in pulses-per-quarter-note.

			This is the number of quarter notes since the edit start.
		*/
		double ppqPosition;

		/** The position of the start of the last bar, in pulses-per-quarter-note.

			This is the number of quarter notes from the start of the edit to the
			start of the current bar.

			Note - this value may be unavailable on some hosts, e.g. Pro-Tools. If
			it's not available, the value will be 0.
		*/
		double ppqPositionOfLastBarStart;

		/** The video frame rate, if applicable. */
		FrameRateType frameRate;

		/** True if the transport is currently playing. */
		bool isPlaying;

		/** True if the transport is currently recording.

			(When isRecording is true, then isPlaying will also be true).
		*/
		bool isRecording;

		/** The current cycle start position in pulses-per-quarter-note.
			Note that not all hosts or plugin formats may provide this value.
			@see isLooping
		*/
		double ppqLoopStart;

		/** The current cycle end position in pulses-per-quarter-note.
			Note that not all hosts or plugin formats may provide this value.
			@see isLooping
		*/
		double ppqLoopEnd;

		/** True if the transport is currently looping. */
		bool isLooping;

		bool operator== (const CurrentPositionInfo& other) const noexcept;
		bool operator!= (const CurrentPositionInfo& other) const noexcept;

		void resetToDefault();
	};

	/** Fills-in the given structure with details about the transport's
		position at the start of the current processing block.
	*/
	virtual bool getCurrentPosition (CurrentPositionInfo& result) = 0;
};

#endif   // __JUCE_AUDIOPLAYHEAD_JUCEHEADER__

/*** End of inlined file: juce_AudioPlayHead.h ***/


#endif
#ifndef __JUCE_AUDIOPLUGININSTANCE_JUCEHEADER__

/*** Start of inlined file: juce_AudioPluginInstance.h ***/
#ifndef __JUCE_AUDIOPLUGININSTANCE_JUCEHEADER__
#define __JUCE_AUDIOPLUGININSTANCE_JUCEHEADER__


/*** Start of inlined file: juce_AudioProcessor.h ***/
#ifndef __JUCE_AUDIOPROCESSOR_JUCEHEADER__
#define __JUCE_AUDIOPROCESSOR_JUCEHEADER__


/*** Start of inlined file: juce_AudioProcessorEditor.h ***/
#ifndef __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__
#define __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__

class AudioProcessor;

/**
	Base class for the component that acts as the GUI for an AudioProcessor.

	Derive your editor component from this class, and create an instance of it
	by overriding the AudioProcessor::createEditor() method.

	@see AudioProcessor, GenericAudioProcessorEditor
*/
class JUCE_API  AudioProcessorEditor  : public Component
{
protected:

	/** Creates an editor for the specified processor.
	*/
	AudioProcessorEditor (AudioProcessor* owner);

public:
	/** Destructor. */
	~AudioProcessorEditor();

	/** Returns a pointer to the processor that this editor represents. */
	AudioProcessor* getAudioProcessor() const noexcept        { return owner; }

private:

	AudioProcessor* const owner;

	JUCE_DECLARE_NON_COPYABLE (AudioProcessorEditor);
};

#endif   // __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessorEditor.h ***/


/*** Start of inlined file: juce_AudioProcessorListener.h ***/
#ifndef __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__
#define __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__

class AudioProcessor;

/**
	Base class for listeners that want to know about changes to an AudioProcessor.

	Use AudioProcessor::addListener() to register your listener with an AudioProcessor.

	@see AudioProcessor
*/
class JUCE_API  AudioProcessorListener
{
public:

	/** Destructor. */
	virtual ~AudioProcessorListener() {}

	/** Receives a callback when a parameter is changed.

		IMPORTANT NOTE: this will be called synchronously when a parameter changes, and
		many audio processors will change their parameter during their audio callback.
		This means that not only has your handler code got to be completely thread-safe,
		but it's also got to be VERY fast, and avoid blocking. If you need to handle
		this event on your message thread, use this callback to trigger an AsyncUpdater
		or ChangeBroadcaster which you can respond to on the message thread.
	*/
	virtual void audioProcessorParameterChanged (AudioProcessor* processor,
												 int parameterIndex,
												 float newValue) = 0;

	/** Called to indicate that something else in the plugin has changed, like its
		program, number of parameters, etc.

		IMPORTANT NOTE: this will be called synchronously, and many audio processors will
		call it during their audio callback. This means that not only has your handler code
		got to be completely thread-safe, but it's also got to be VERY fast, and avoid
		blocking. If you need to handle this event on your message thread, use this callback
		to trigger an AsyncUpdater or ChangeBroadcaster which you can respond to later on the
		message thread.
	*/
	virtual void audioProcessorChanged (AudioProcessor* processor) = 0;

	/** Indicates that a parameter change gesture has started.

		E.g. if the user is dragging a slider, this would be called when they first
		press the mouse button, and audioProcessorParameterChangeGestureEnd would be
		called when they release it.

		IMPORTANT NOTE: this will be called synchronously, and many audio processors will
		call it during their audio callback. This means that not only has your handler code
		got to be completely thread-safe, but it's also got to be VERY fast, and avoid
		blocking. If you need to handle this event on your message thread, use this callback
		to trigger an AsyncUpdater or ChangeBroadcaster which you can respond to later on the
		message thread.

		@see audioProcessorParameterChangeGestureEnd
	*/
	virtual void audioProcessorParameterChangeGestureBegin (AudioProcessor* processor,
															int parameterIndex);

	/** Indicates that a parameter change gesture has finished.

		E.g. if the user is dragging a slider, this would be called when they release
		the mouse button.

		IMPORTANT NOTE: this will be called synchronously, and many audio processors will
		call it during their audio callback. This means that not only has your handler code
		got to be completely thread-safe, but it's also got to be VERY fast, and avoid
		blocking. If you need to handle this event on your message thread, use this callback
		to trigger an AsyncUpdater or ChangeBroadcaster which you can respond to later on the
		message thread.

		@see audioProcessorParameterChangeGestureBegin
	*/
	virtual void audioProcessorParameterChangeGestureEnd (AudioProcessor* processor,
														  int parameterIndex);
};

#endif   // __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessorListener.h ***/

/**
	Base class for audio processing filters or plugins.

	This is intended to act as a base class of audio filter that is general enough to
	be wrapped as a VST, AU, RTAS, etc, or used internally.

	It is also used by the plugin hosting code as the wrapper around an instance
	of a loaded plugin.

	Derive your filter class from this base class, and if you're building a plugin,
	you should implement a global function called createPluginFilter() which creates
	and returns a new instance of your subclass.
*/
class JUCE_API  AudioProcessor
{
protected:

	/** Constructor.

		You can also do your initialisation tasks in the initialiseFilterInfo()
		call, which will be made after this object has been created.
	*/
	AudioProcessor();

public:
	/** Destructor. */
	virtual ~AudioProcessor();

	/** Returns the name of this processor.
	*/
	virtual const String getName() const = 0;

	/** Called before playback starts, to let the filter prepare itself.

		The sample rate is the target sample rate, and will remain constant until
		playback stops.

		The estimatedSamplesPerBlock value is a HINT about the typical number of
		samples that will be processed for each callback, but isn't any kind
		of guarantee. The actual block sizes that the host uses may be different
		each time the callback happens, and may be more or less than this value.
	*/
	virtual void prepareToPlay (double sampleRate,
								int estimatedSamplesPerBlock) = 0;

	/** Called after playback has stopped, to let the filter free up any resources it
		no longer needs.
	*/
	virtual void releaseResources() = 0;

	/** Renders the next block.

		When this method is called, the buffer contains a number of channels which is
		at least as great as the maximum number of input and output channels that
		this filter is using. It will be filled with the filter's input data and
		should be replaced with the filter's output.

		So for example if your filter has 2 input channels and 4 output channels, then
		the buffer will contain 4 channels, the first two being filled with the
		input data. Your filter should read these, do its processing, and replace
		the contents of all 4 channels with its output.

		Or if your filter has 5 inputs and 2 outputs, the buffer will have 5 channels,
		all filled with data, and your filter should overwrite the first 2 of these
		with its output. But be VERY careful not to write anything to the last 3
		channels, as these might be mapped to memory that the host assumes is read-only!

		Note that if you have more outputs than inputs, then only those channels that
		correspond to an input channel are guaranteed to contain sensible data - e.g.
		in the case of 2 inputs and 4 outputs, the first two channels contain the input,
		but the last two channels may contain garbage, so you should be careful not to
		let this pass through without being overwritten or cleared.

		Also note that the buffer may have more channels than are strictly necessary,
		but your should only read/write from the ones that your filter is supposed to
		be using.

		The number of samples in these buffers is NOT guaranteed to be the same for every
		callback, and may be more or less than the estimated value given to prepareToPlay().
		Your code must be able to cope with variable-sized blocks, or you're going to get
		clicks and crashes!

		If the filter is receiving a midi input, then the midiMessages array will be filled
		with the midi messages for this block. Each message's timestamp will indicate the
		message's time, as a number of samples from the start of the block.

		Any messages left in the midi buffer when this method has finished are assumed to
		be the filter's midi output. This means that your filter should be careful to
		clear any incoming messages from the array if it doesn't want them to be passed-on.

		Be very careful about what you do in this callback - it's going to be called by
		the audio thread, so any kind of interaction with the UI is absolutely
		out of the question. If you change a parameter in here and need to tell your UI to
		update itself, the best way is probably to inherit from a ChangeBroadcaster, let
		the UI components register as listeners, and then call sendChangeMessage() inside the
		processBlock() method to send out an asynchronous message. You could also use
		the AsyncUpdater class in a similar way.
	*/
	virtual void processBlock (AudioSampleBuffer& buffer,
							   MidiBuffer& midiMessages) = 0;

	/** Returns the current AudioPlayHead object that should be used to find
		out the state and position of the playhead.

		You can call this from your processBlock() method, and use the AudioPlayHead
		object to get the details about the time of the start of the block currently
		being processed.

		If the host hasn't supplied a playhead object, this will return 0.
	*/
	AudioPlayHead* getPlayHead() const noexcept                 { return playHead; }

	/** Returns the current sample rate.

		This can be called from your processBlock() method - it's not guaranteed
		to be valid at any other time, and may return 0 if it's unknown.
	*/
	double getSampleRate() const noexcept                       { return sampleRate; }

	/** Returns the current typical block size that is being used.

		This can be called from your processBlock() method - it's not guaranteed
		to be valid at any other time.

		Remember it's not the ONLY block size that may be used when calling
		processBlock, it's just the normal one. The actual block sizes used may be
		larger or smaller than this, and will vary between successive calls.
	*/
	int getBlockSize() const noexcept                           { return blockSize; }

	/** Returns the number of input channels that the host will be sending the filter.

		If writing a plugin, your configuration macros should specify the number of
		channels that your filter would prefer to have, and this method lets
		you know how many the host is actually using.

		Note that this method is only valid during or after the prepareToPlay()
		method call. Until that point, the number of channels will be unknown.
	*/
	int getNumInputChannels() const noexcept                    { return numInputChannels; }

	/** Returns the number of output channels that the host will be sending the filter.

		If writing a plugin, your configuration macros should specify the number of
		channels that your filter would prefer to have, and this method lets
		you know how many the host is actually using.

		Note that this method is only valid during or after the prepareToPlay()
		method call. Until that point, the number of channels will be unknown.
	*/
	int getNumOutputChannels() const noexcept                   { return numOutputChannels; }

	/** Returns a string containing a whitespace-separated list of speaker types
		corresponding to each input channel.
		For example in a 5.1 arrangement, the string may be "L R C Lfe Ls Rs"
		If the speaker arrangement is unknown, the returned string will be empty.
	*/
	const String& getInputSpeakerArrangement() const noexcept   { return inputSpeakerArrangement; }

	/** Returns a string containing a whitespace-separated list of speaker types
		corresponding to each output channel.
		For example in a 5.1 arrangement, the string may be "L R C Lfe Ls Rs"
		If the speaker arrangement is unknown, the returned string will be empty.
	*/
	const String& getOutputSpeakerArrangement() const noexcept  { return outputSpeakerArrangement; }

	/** Returns the name of one of the processor's input channels.

		The processor might not supply very useful names for channels, and this might be
		something like "1", "2", "left", "right", etc.
	*/
	virtual const String getInputChannelName (int channelIndex) const = 0;

	/** Returns the name of one of the processor's output channels.

		The processor might not supply very useful names for channels, and this might be
		something like "1", "2", "left", "right", etc.
	*/
	virtual const String getOutputChannelName (int channelIndex) const = 0;

	/** Returns true if the specified channel is part of a stereo pair with its neighbour. */
	virtual bool isInputChannelStereoPair (int index) const = 0;

	/** Returns true if the specified channel is part of a stereo pair with its neighbour. */
	virtual bool isOutputChannelStereoPair (int index) const = 0;

	/** This returns the number of samples delay that the filter imposes on the audio
		passing through it.

		The host will call this to find the latency - the filter itself should set this value
		by calling setLatencySamples() as soon as it can during its initialisation.
	*/
	int getLatencySamples() const noexcept                      { return latencySamples; }

	/** The filter should call this to set the number of samples delay that it introduces.

		The filter should call this as soon as it can during initialisation, and can call it
		later if the value changes.
	*/
	void setLatencySamples (int newLatency);

	/** Returns true if the processor wants midi messages. */
	virtual bool acceptsMidi() const = 0;

	/** Returns true if the processor produces midi messages. */
	virtual bool producesMidi() const = 0;

	/** This returns a critical section that will automatically be locked while the host
		is calling the processBlock() method.

		Use it from your UI or other threads to lock access to variables that are used
		by the process callback, but obviously be careful not to keep it locked for
		too long, because that could cause stuttering playback. If you need to do something
		that'll take a long time and need the processing to stop while it happens, use the
		suspendProcessing() method instead.

		@see suspendProcessing
	*/
	const CriticalSection& getCallbackLock() const noexcept     { return callbackLock; }

	/** Enables and disables the processing callback.

		If you need to do something time-consuming on a thread and would like to make sure
		the audio processing callback doesn't happen until you've finished, use this
		to disable the callback and re-enable it again afterwards.

		E.g.
		@code
		void loadNewPatch()
		{
			suspendProcessing (true);

			..do something that takes ages..

			suspendProcessing (false);
		}
		@endcode

		If the host tries to make an audio callback while processing is suspended, the
		filter will return an empty buffer, but won't block the audio thread like it would
		do if you use the getCallbackLock() critical section to synchronise access.

		If you're going to use this, your processBlock() method must call isSuspended() and
		check whether it's suspended or not. If it is, then it should skip doing any real
		processing, either emitting silence or passing the input through unchanged.

		@see getCallbackLock
	*/
	void suspendProcessing (bool shouldBeSuspended);

	/** Returns true if processing is currently suspended.
		@see suspendProcessing
	*/
	bool isSuspended() const noexcept                                   { return suspended; }

	/** A plugin can override this to be told when it should reset any playing voices.

		The default implementation does nothing, but a host may call this to tell the
		plugin that it should stop any tails or sounds that have been left running.
	*/
	virtual void reset();

	/** Returns true if the processor is being run in an offline mode for rendering.

		If the processor is being run live on realtime signals, this returns false.
		If the mode is unknown, this will assume it's realtime and return false.

		This value may be unreliable until the prepareToPlay() method has been called,
		and could change each time prepareToPlay() is called.

		@see setNonRealtime()
	*/
	bool isNonRealtime() const noexcept                                 { return nonRealtime; }

	/** Called by the host to tell this processor whether it's being used in a non-realime
		capacity for offline rendering or bouncing.

		Whatever value is passed-in will be
	*/
	void setNonRealtime (bool isNonRealtime) noexcept;

	/** Creates the filter's UI.

		This can return 0 if you want a UI-less filter, in which case the host may create
		a generic UI that lets the user twiddle the parameters directly.

		If you do want to pass back a component, the component should be created and set to
		the correct size before returning it. If you implement this method, you must
		also implement the hasEditor() method and make it return true.

		Remember not to do anything silly like allowing your filter to keep a pointer to
		the component that gets created - it could be deleted later without any warning, which
		would make your pointer into a dangler. Use the getActiveEditor() method instead.

		The correct way to handle the connection between an editor component and its
		filter is to use something like a ChangeBroadcaster so that the editor can
		register itself as a listener, and be told when a change occurs. This lets them
		safely unregister themselves when they are deleted.

		Here are a few things to bear in mind when writing an editor:

		- Initially there won't be an editor, until the user opens one, or they might
		  not open one at all. Your filter mustn't rely on it being there.
		- An editor object may be deleted and a replacement one created again at any time.
		- It's safe to assume that an editor will be deleted before its filter.

		@see hasEditor
	*/
	virtual AudioProcessorEditor* createEditor() = 0;

	/** Your filter must override this and return true if it can create an editor component.
		@see createEditor
	*/
	virtual bool hasEditor() const = 0;

	/** Returns the active editor, if there is one.

		Bear in mind this can return nullptr, even if an editor has previously been opened.
	*/
	AudioProcessorEditor* getActiveEditor() const noexcept             { return activeEditor; }

	/** Returns the active editor, or if there isn't one, it will create one.

		This may call createEditor() internally to create the component.
	*/
	AudioProcessorEditor* createEditorIfNeeded();

	/** This must return the correct value immediately after the object has been
		created, and mustn't change the number of parameters later.
	*/
	virtual int getNumParameters() = 0;

	/** Returns the name of a particular parameter. */
	virtual const String getParameterName (int parameterIndex) = 0;

	/** Called by the host to find out the value of one of the filter's parameters.

		The host will expect the value returned to be between 0 and 1.0.

		This could be called quite frequently, so try to make your code efficient.
		It's also likely to be called by non-UI threads, so the code in here should
		be thread-aware.
	*/
	virtual float getParameter (int parameterIndex) = 0;

	/** Returns the value of a parameter as a text string. */
	virtual const String getParameterText (int parameterIndex) = 0;

	/** The host will call this method to change the value of one of the filter's parameters.

		The host may call this at any time, including during the audio processing
		callback, so the filter has to process this very fast and avoid blocking.

		If you want to set the value of a parameter internally, e.g. from your
		editor component, then don't call this directly - instead, use the
		setParameterNotifyingHost() method, which will also send a message to
		the host telling it about the change. If the message isn't sent, the host
		won't be able to automate your parameters properly.

		The value passed will be between 0 and 1.0.
	*/
	virtual void setParameter (int parameterIndex,
							   float newValue) = 0;

	/** Your filter can call this when it needs to change one of its parameters.

		This could happen when the editor or some other internal operation changes
		a parameter. This method will call the setParameter() method to change the
		value, and will then send a message to the host telling it about the change.

		Note that to make sure the host correctly handles automation, you should call
		the beginParameterChangeGesture() and endParameterChangeGesture() methods to
		tell the host when the user has started and stopped changing the parameter.
	*/
	void setParameterNotifyingHost (int parameterIndex,
									float newValue);

	/** Returns true if the host can automate this parameter.

		By default, this returns true for all parameters.
	*/
	virtual bool isParameterAutomatable (int parameterIndex) const;

	/** Should return true if this parameter is a "meta" parameter.

		A meta-parameter is a parameter that changes other params. It is used
		by some hosts (e.g. AudioUnit hosts).

		By default this returns false.
	*/
	virtual bool isMetaParameter (int parameterIndex) const;

	/** Sends a signal to the host to tell it that the user is about to start changing this
		parameter.

		This allows the host to know when a parameter is actively being held by the user, and
		it may use this information to help it record automation.

		If you call this, it must be matched by a later call to endParameterChangeGesture().
	*/
	void beginParameterChangeGesture (int parameterIndex);

	/** Tells the host that the user has finished changing this parameter.

		This allows the host to know when a parameter is actively being held by the user, and
		it may use this information to help it record automation.

		A call to this method must follow a call to beginParameterChangeGesture().
	*/
	void endParameterChangeGesture (int parameterIndex);

	/** The filter can call this when something (apart from a parameter value) has changed.

		It sends a hint to the host that something like the program, number of parameters,
		etc, has changed, and that it should update itself.
	*/
	void updateHostDisplay();

	/** Returns the number of preset programs the filter supports.

		The value returned must be valid as soon as this object is created, and
		must not change over its lifetime.

		This value shouldn't be less than 1.
	*/
	virtual int getNumPrograms() = 0;

	/** Returns the number of the currently active program.
	*/
	virtual int getCurrentProgram() = 0;

	/** Called by the host to change the current program.
	*/
	virtual void setCurrentProgram (int index) = 0;

	/** Must return the name of a given program. */
	virtual const String getProgramName (int index) = 0;

	/** Called by the host to rename a program.
	*/
	virtual void changeProgramName (int index, const String& newName) = 0;

	/** The host will call this method when it wants to save the filter's internal state.

		This must copy any info about the filter's state into the block of memory provided,
		so that the host can store this and later restore it using setStateInformation().

		Note that there's also a getCurrentProgramStateInformation() method, which only
		stores the current program, not the state of the entire filter.

		See also the helper function copyXmlToBinary() for storing settings as XML.

		@see getCurrentProgramStateInformation
	*/
	virtual void getStateInformation (juce::MemoryBlock& destData) = 0;

	/** The host will call this method if it wants to save the state of just the filter's
		current program.

		Unlike getStateInformation, this should only return the current program's state.

		Not all hosts support this, and if you don't implement it, the base class
		method just calls getStateInformation() instead. If you do implement it, be
		sure to also implement getCurrentProgramStateInformation.

		@see getStateInformation, setCurrentProgramStateInformation
	*/
	virtual void getCurrentProgramStateInformation (juce::MemoryBlock& destData);

	/** This must restore the filter's state from a block of data previously created
		using getStateInformation().

		Note that there's also a setCurrentProgramStateInformation() method, which tries
		to restore just the current program, not the state of the entire filter.

		See also the helper function getXmlFromBinary() for loading settings as XML.

		@see setCurrentProgramStateInformation
	*/
	virtual void setStateInformation (const void* data, int sizeInBytes) = 0;

	/** The host will call this method if it wants to restore the state of just the filter's
		current program.

		Not all hosts support this, and if you don't implement it, the base class
		method just calls setStateInformation() instead. If you do implement it, be
		sure to also implement getCurrentProgramStateInformation.

		@see setStateInformation, getCurrentProgramStateInformation
	*/
	virtual void setCurrentProgramStateInformation (const void* data, int sizeInBytes);

	/** Adds a listener that will be called when an aspect of this processor changes. */
	void addListener (AudioProcessorListener* newListener);

	/** Removes a previously added listener. */
	void removeListener (AudioProcessorListener* listenerToRemove);

	/** Tells the processor to use this playhead object.
		The processor will not take ownership of the object, so the caller must delete it when
		it is no longer being used.
	*/
	void setPlayHead (AudioPlayHead* newPlayHead) noexcept;

	/** Not for public use - this is called before deleting an editor component. */
	void editorBeingDeleted (AudioProcessorEditor*) noexcept;

	/** Not for public use - this is called to initialise the processor before playing. */
	void setPlayConfigDetails (int numIns, int numOuts, double sampleRate, int blockSize) noexcept;

	/** Not for public use - this is called to initialise the processor before playing. */
	void setSpeakerArrangement (const String& inputs, const String& outputs);

protected:

	/** Helper function that just converts an xml element into a binary blob.

		Use this in your filter's getStateInformation() method if you want to
		store its state as xml.

		Then use getXmlFromBinary() to reverse this operation and retrieve the XML
		from a binary blob.
	*/
	static void copyXmlToBinary (const XmlElement& xml,
								 juce::MemoryBlock& destData);

	/** Retrieves an XML element that was stored as binary with the copyXmlToBinary() method.

		This might return 0 if the data's unsuitable or corrupted. Otherwise it will return
		an XmlElement object that the caller must delete when no longer needed.
	*/
	static XmlElement* getXmlFromBinary (const void* data, int sizeInBytes);

	/** @internal */
	AudioPlayHead* playHead;

	/** @internal */
	void sendParamChangeMessageToListeners (int parameterIndex, float newValue);

private:
	Array <AudioProcessorListener*> listeners;
	Component::SafePointer<AudioProcessorEditor> activeEditor;
	double sampleRate;
	int blockSize, numInputChannels, numOutputChannels, latencySamples;
	bool suspended, nonRealtime;
	CriticalSection callbackLock, listenerLock;
	String inputSpeakerArrangement, outputSpeakerArrangement;

   #if JUCE_DEBUG
	BigInteger changingParams;
   #endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessor);
};

#endif   // __JUCE_AUDIOPROCESSOR_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessor.h ***/


/*** Start of inlined file: juce_PluginDescription.h ***/
#ifndef __JUCE_PLUGINDESCRIPTION_JUCEHEADER__
#define __JUCE_PLUGINDESCRIPTION_JUCEHEADER__

/**
	A small class to represent some facts about a particular type of plugin.

	This class is for storing and managing the details about a plugin without
	actually having to load an instance of it.

	A KnownPluginList contains a list of PluginDescription objects.

	@see KnownPluginList
*/
class JUCE_API  PluginDescription
{
public:

	PluginDescription();
	PluginDescription (const PluginDescription& other);
	PluginDescription& operator= (const PluginDescription& other);
	~PluginDescription();

	/** The name of the plugin. */
	String name;

	/** A more descriptive name for the plugin.
		This may be the same as the 'name' field, but some plugins may provide an
		alternative name.
	*/
	String descriptiveName;

	/** The plugin format, e.g. "VST", "AudioUnit", etc.
	*/
	String pluginFormatName;

	/** A category, such as "Dynamics", "Reverbs", etc.
	*/
	String category;

	/** The manufacturer. */
	String manufacturerName;

	/** The version. This string doesn't have any particular format. */
	String version;

	/** Either the file containing the plugin module, or some other unique way
		of identifying it.

		E.g. for an AU, this would be an ID string that the component manager
		could use to retrieve the plugin. For a VST, it's the file path.
	*/
	String fileOrIdentifier;

	/** The last time the plugin file was changed.
		This is handy when scanning for new or changed plugins.
	*/
	Time lastFileModTime;

	/** A unique ID for the plugin.

		Note that this might not be unique between formats, e.g. a VST and some
		other format might actually have the same id.

		@see createIdentifierString
	*/
	int uid;

	/** True if the plugin identifies itself as a synthesiser. */
	bool isInstrument;

	/** The number of inputs. */
	int numInputChannels;

	/** The number of outputs. */
	int numOutputChannels;

	/** Returns true if the two descriptions refer the the same plugin.

		This isn't quite as simple as them just having the same file (because of
		shell plugins).
	*/
	bool isDuplicateOf (const PluginDescription& other) const;

	/** Returns a string that can be saved and used to uniquely identify the
		plugin again.

		This contains less info than the XML encoding, and is independent of the
		plugin's file location, so can be used to store a plugin ID for use
		across different machines.
	*/
	String createIdentifierString() const;

	/** Creates an XML object containing these details.

		@see loadFromXml
	*/
	XmlElement* createXml() const;

	/** Reloads the info in this structure from an XML record that was previously
		saved with createXML().

		Returns true if the XML was a valid plugin description.
	*/
	bool loadFromXml (const XmlElement& xml);

private:

	JUCE_LEAK_DETECTOR (PluginDescription);
};

#endif   // __JUCE_PLUGINDESCRIPTION_JUCEHEADER__

/*** End of inlined file: juce_PluginDescription.h ***/

/**
	Base class for an active instance of a plugin.

	This derives from the AudioProcessor class, and adds some extra functionality
	that helps when wrapping dynamically loaded plugins.

	@see AudioProcessor, AudioPluginFormat
*/
class JUCE_API  AudioPluginInstance   : public AudioProcessor
{
public:

	/** Destructor.

		Make sure that you delete any UI components that belong to this plugin before
		deleting the plugin.
	*/
	virtual ~AudioPluginInstance() {}

	/** Fills-in the appropriate parts of this plugin description object.
	*/
	virtual void fillInPluginDescription (PluginDescription& description) const = 0;

	/** Returns a pointer to some kind of platform-specific data about the plugin.

		E.g. For a VST, this value can be cast to an AEffect*. For an AudioUnit, it can be
		cast to an AudioUnit handle.
	*/
	virtual void* getPlatformSpecificData()         { return nullptr; }

protected:

	AudioPluginInstance() {}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginInstance);
};

#endif   // __JUCE_AUDIOPLUGININSTANCE_JUCEHEADER__

/*** End of inlined file: juce_AudioPluginInstance.h ***/


#endif
#ifndef __JUCE_AUDIOPROCESSOR_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOPROCESSORGRAPH_JUCEHEADER__

/*** Start of inlined file: juce_AudioProcessorGraph.h ***/
#ifndef __JUCE_AUDIOPROCESSORGRAPH_JUCEHEADER__
#define __JUCE_AUDIOPROCESSORGRAPH_JUCEHEADER__


/*** Start of inlined file: juce_AudioPluginFormatManager.h ***/
#ifndef __JUCE_AUDIOPLUGINFORMATMANAGER_JUCEHEADER__
#define __JUCE_AUDIOPLUGINFORMATMANAGER_JUCEHEADER__


/*** Start of inlined file: juce_AudioPluginFormat.h ***/
#ifndef __JUCE_AUDIOPLUGINFORMAT_JUCEHEADER__
#define __JUCE_AUDIOPLUGINFORMAT_JUCEHEADER__

class PluginDescription;

/**
	The base class for a type of plugin format, such as VST, AudioUnit, LADSPA, etc.

	Use the static getNumFormats() and getFormat() calls to find the types
	of format that are available.
*/
class JUCE_API  AudioPluginFormat
{
public:

	/** Destructor. */
	virtual ~AudioPluginFormat();

	/** Returns the format name.

		E.g. "VST", "AudioUnit", etc.
	*/
	virtual String getName() const = 0;

	/** This tries to create descriptions for all the plugin types available in
		a binary module file.

		The file will be some kind of DLL or bundle.

		Normally there will only be one type returned, but some plugins
		(e.g. VST shells) can use a single DLL to create a set of different plugin
		subtypes, so in that case, each subtype is returned as a separate object.
	*/
	virtual void findAllTypesForFile (OwnedArray <PluginDescription>& results,
									  const String& fileOrIdentifier) = 0;

	/** Tries to recreate a type from a previously generated PluginDescription.

		@see PluginDescription::createInstance
	*/
	virtual AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc) = 0;

	/** Should do a quick check to see if this file or directory might be a plugin of
		this format.

		This is for searching for potential files, so it shouldn't actually try to
		load the plugin or do anything time-consuming.
	*/
	virtual bool fileMightContainThisPluginType (const String& fileOrIdentifier) = 0;

	/** Returns a readable version of the name of the plugin that this identifier refers to.
	*/
	virtual String getNameOfPluginFromIdentifier (const String& fileOrIdentifier) = 0;

	/** Checks whether this plugin could possibly be loaded.

		It doesn't actually need to load it, just to check whether the file or component
		still exists.
	*/
	virtual bool doesPluginStillExist (const PluginDescription& desc) = 0;

	/** Searches a suggested set of directories for any plugins in this format.

		The path might be ignored, e.g. by AUs, which are found by the OS rather
		than manually.
	*/
	virtual StringArray searchPathsForPlugins (const FileSearchPath& directoriesToSearch,
											   bool recursive) = 0;

	/** Returns the typical places to look for this kind of plugin.

		Note that if this returns no paths, it means that the format can't be scanned-for
		(i.e. it's an internal format that doesn't live in files)
	*/
	virtual FileSearchPath getDefaultLocationsToSearch() = 0;

protected:

	AudioPluginFormat() noexcept;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginFormat);
};

#endif   // __JUCE_AUDIOPLUGINFORMAT_JUCEHEADER__

/*** End of inlined file: juce_AudioPluginFormat.h ***/

/**
	This maintains a list of known AudioPluginFormats.

	@see AudioPluginFormat
*/
class JUCE_API  AudioPluginFormatManager  : public DeletedAtShutdown
{
public:

	AudioPluginFormatManager();

	/** Destructor. */
	~AudioPluginFormatManager();

	juce_DeclareSingleton_SingleThreaded (AudioPluginFormatManager, false);

	/** Adds any formats that it knows about, e.g. VST.
	*/
	void addDefaultFormats();

	/** Returns the number of types of format that are available.

		Use getFormat() to get one of them.
	*/
	int getNumFormats();

	/** Returns one of the available formats.

		@see getNumFormats
	*/
	AudioPluginFormat* getFormat (int index);

	/** Adds a format to the list.

		The object passed in will be owned and deleted by the manager.
	*/
	void addFormat (AudioPluginFormat* format);

	/** Tries to load the type for this description, by trying all the formats
		that this manager knows about.

		The caller is responsible for deleting the object that is returned.

		If it can't load the plugin, it returns 0 and leaves a message in the
		errorMessage string.
	*/
	AudioPluginInstance* createPluginInstance (const PluginDescription& description,
											   String& errorMessage) const;

	/** Checks that the file or component for this plugin actually still exists.

		(This won't try to load the plugin)
	*/
	bool doesPluginStillExist (const PluginDescription& description) const;

private:

	OwnedArray <AudioPluginFormat> formats;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginFormatManager);
};

#endif   // __JUCE_AUDIOPLUGINFORMATMANAGER_JUCEHEADER__

/*** End of inlined file: juce_AudioPluginFormatManager.h ***/


/*** Start of inlined file: juce_KnownPluginList.h ***/
#ifndef __JUCE_KNOWNPLUGINLIST_JUCEHEADER__
#define __JUCE_KNOWNPLUGINLIST_JUCEHEADER__

/**
	Manages a list of plugin types.

	This can be easily edited, saved and loaded, and used to create instances of
	the plugin types in it.

	@see PluginListComponent
*/
class JUCE_API  KnownPluginList   : public ChangeBroadcaster
{
public:

	/** Creates an empty list.
	*/
	KnownPluginList();

	/** Destructor. */
	~KnownPluginList();

	/** Clears the list. */
	void clear();

	/** Returns the number of types currently in the list.
		@see getType
	*/
	int getNumTypes() const noexcept                                { return types.size(); }

	/** Returns one of the types.
		@see getNumTypes
	*/
	PluginDescription* getType (int index) const noexcept           { return types [index]; }

	/** Looks for a type in the list which comes from this file.
	*/
	PluginDescription* getTypeForFile (const String& fileOrIdentifier) const;

	/** Looks for a type in the list which matches a plugin type ID.

		The identifierString parameter must have been created by
		PluginDescription::createIdentifierString().
	*/
	PluginDescription* getTypeForIdentifierString (const String& identifierString) const;

	/** Adds a type manually from its description. */
	bool addType (const PluginDescription& type);

	/** Removes a type. */
	void removeType (int index);

	/** Looks for all types that can be loaded from a given file, and adds them
		to the list.

		If dontRescanIfAlreadyInList is true, then the file will only be loaded and
		re-tested if it's not already in the list, or if the file's modification
		time has changed since the list was created. If dontRescanIfAlreadyInList is
		false, the file will always be reloaded and tested.

		Returns true if any new types were added, and all the types found in this
		file (even if it was already known and hasn't been re-scanned) get returned
		in the array.
	*/
	bool scanAndAddFile (const String& possiblePluginFileOrIdentifier,
						 bool dontRescanIfAlreadyInList,
						 OwnedArray <PluginDescription>& typesFound,
						 AudioPluginFormat& formatToUse);

	/** Returns true if the specified file is already known about and if it
		hasn't been modified since our entry was created.
	*/
	bool isListingUpToDate (const String& possiblePluginFileOrIdentifier) const;

	/** Scans and adds a bunch of files that might have been dragged-and-dropped.

		If any types are found in the files, their descriptions are returned in the array.
	*/
	void scanAndAddDragAndDroppedFiles (const StringArray& filenames,
										OwnedArray <PluginDescription>& typesFound);

	/** Sort methods used to change the order of the plugins in the list.
	*/
	enum SortMethod
	{
		defaultOrder = 0,
		sortAlphabetically,
		sortByCategory,
		sortByManufacturer,
		sortByFileSystemLocation
	};

	/** Adds all the plugin types to a popup menu so that the user can select one.

		Depending on the sort method, it may add sub-menus for categories,
		manufacturers, etc.

		Use getIndexChosenByMenu() to find out the type that was chosen.
	*/
	void addToMenu (PopupMenu& menu,
					const SortMethod sortMethod) const;

	/** Converts a menu item index that has been chosen into its index in this list.

		Returns -1 if it's not an ID that was used.

		@see addToMenu
	*/
	int getIndexChosenByMenu (int menuResultCode) const;

	/** Sorts the list. */
	void sort (const SortMethod method);

	/** Creates some XML that can be used to store the state of this list. */
	XmlElement* createXml() const;

	/** Recreates the state of this list from its stored XML format. */
	void recreateFromXml (const XmlElement& xml);

private:

	OwnedArray <PluginDescription> types;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnownPluginList);
};

#endif   // __JUCE_KNOWNPLUGINLIST_JUCEHEADER__

/*** End of inlined file: juce_KnownPluginList.h ***/

/**
	A type of AudioProcessor which plays back a graph of other AudioProcessors.

	Use one of these objects if you want to wire-up a set of AudioProcessors
	and play back the result.

	Processors can be added to the graph as "nodes" using addNode(), and once
	added, you can connect any of their input or output channels to other
	nodes using addConnection().

	To play back a graph through an audio device, you might want to use an
	AudioProcessorPlayer object.
*/
class JUCE_API  AudioProcessorGraph   : public AudioProcessor,
										public AsyncUpdater
{
public:

	/** Creates an empty graph.
	*/
	AudioProcessorGraph();

	/** Destructor.
		Any processor objects that have been added to the graph will also be deleted.
	*/
	~AudioProcessorGraph();

	/** Represents one of the nodes, or processors, in an AudioProcessorGraph.

		To create a node, call AudioProcessorGraph::addNode().
	*/
	class JUCE_API  Node   : public ReferenceCountedObject
	{
	public:

		/** The ID number assigned to this node.
			This is assigned by the graph that owns it, and can't be changed.
		*/
		const uint32 nodeId;

		/** The actual processor object that this node represents. */
		AudioProcessor* getProcessor() const noexcept           { return processor; }

		/** A set of user-definable properties that are associated with this node.

			This can be used to attach values to the node for whatever purpose seems
			useful. For example, you might store an x and y position if your application
			is displaying the nodes on-screen.
		*/
		NamedValueSet properties;

		/** A convenient typedef for referring to a pointer to a node object. */
		typedef ReferenceCountedObjectPtr <Node> Ptr;

	private:

		friend class AudioProcessorGraph;

		const ScopedPointer<AudioProcessor> processor;
		bool isPrepared;

		Node (uint32 nodeId, AudioProcessor*) noexcept;

		void setParentGraph (AudioProcessorGraph*) const;
		void prepare (double sampleRate, int blockSize, AudioProcessorGraph*);
		void unprepare();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Node);
	};

	/** Represents a connection between two channels of two nodes in an AudioProcessorGraph.

		To create a connection, use AudioProcessorGraph::addConnection().
	*/
	struct JUCE_API  Connection
	{
	public:

		Connection (uint32 sourceNodeId, int sourceChannelIndex,
					uint32 destNodeId, int destChannelIndex) noexcept;

		/** The ID number of the node which is the input source for this connection.
			@see AudioProcessorGraph::getNodeForId
		*/
		uint32 sourceNodeId;

		/** The index of the output channel of the source node from which this
			connection takes its data.

			If this value is the special number AudioProcessorGraph::midiChannelIndex, then
			it is referring to the source node's midi output. Otherwise, it is the zero-based
			index of an audio output channel in the source node.
		*/
		int sourceChannelIndex;

		/** The ID number of the node which is the destination for this connection.
			@see AudioProcessorGraph::getNodeForId
		*/
		uint32 destNodeId;

		/** The index of the input channel of the destination node to which this
			connection delivers its data.

			If this value is the special number AudioProcessorGraph::midiChannelIndex, then
			it is referring to the destination node's midi input. Otherwise, it is the zero-based
			index of an audio input channel in the destination node.
		*/
		int destChannelIndex;

	private:

		JUCE_LEAK_DETECTOR (Connection);
	};

	/** Deletes all nodes and connections from this graph.

		Any processor objects in the graph will be deleted.
	*/
	void clear();

	/** Returns the number of nodes in the graph. */
	int getNumNodes() const                                         { return nodes.size(); }

	/** Returns a pointer to one of the nodes in the graph.
		This will return nullptr if the index is out of range.
		@see getNodeForId
	*/
	Node* getNode (const int index) const                           { return nodes [index]; }

	/** Searches the graph for a node with the given ID number and returns it.
		If no such node was found, this returns nullptr.
		@see getNode
	*/
	Node* getNodeForId (const uint32 nodeId) const;

	/** Adds a node to the graph.

		This creates a new node in the graph, for the specified processor. Once you have
		added a processor to the graph, the graph owns it and will delete it later when
		it is no longer needed.

		The optional nodeId parameter lets you specify an ID to use for the node, but
		if the value is already in use, this new node will overwrite the old one.

		If this succeeds, it returns a pointer to the newly-created node.
	*/
	Node* addNode (AudioProcessor* newProcessor, uint32 nodeId = 0);

	/** Deletes a node within the graph which has the specified ID.

		This will also delete any connections that are attached to this node.
	*/
	bool removeNode (uint32 nodeId);

	/** Returns the number of connections in the graph. */
	int getNumConnections() const                                       { return connections.size(); }

	/** Returns a pointer to one of the connections in the graph. */
	const Connection* getConnection (int index) const                   { return connections [index]; }

	/** Searches for a connection between some specified channels.

		If no such connection is found, this returns nullptr.
	*/
	const Connection* getConnectionBetween (uint32 sourceNodeId,
											int sourceChannelIndex,
											uint32 destNodeId,
											int destChannelIndex) const;

	/** Returns true if there is a connection between any of the channels of
		two specified nodes.
	*/
	bool isConnected (uint32 possibleSourceNodeId,
					  uint32 possibleDestNodeId) const;

	/** Returns true if it would be legal to connect the specified points.
	*/
	bool canConnect (uint32 sourceNodeId, int sourceChannelIndex,
					 uint32 destNodeId, int destChannelIndex) const;

	/** Attempts to connect two specified channels of two nodes.

		If this isn't allowed (e.g. because you're trying to connect a midi channel
		to an audio one or other such nonsense), then it'll return false.
	*/
	bool addConnection (uint32 sourceNodeId, int sourceChannelIndex,
						uint32 destNodeId, int destChannelIndex);

	/** Deletes the connection with the specified index.

		Returns true if a connection was actually deleted.
	*/
	void removeConnection (int index);

	/** Deletes any connection between two specified points.

		Returns true if a connection was actually deleted.
	*/
	bool removeConnection (uint32 sourceNodeId, int sourceChannelIndex,
						   uint32 destNodeId, int destChannelIndex);

	/** Removes all connections from the specified node.
	*/
	bool disconnectNode (uint32 nodeId);

	/** Returns true if the given connection's channel numbers map on to valid
		channels at each end.
		Even if a connection is valid when created, its status could change if
		a node changes its channel config.
	*/
	bool isConnectionLegal (const Connection* connection) const;

	/** Performs a sanity checks of all the connections.

		This might be useful if some of the processors are doing things like changing
		their channel counts, which could render some connections obsolete.
	*/
	bool removeIllegalConnections();

	/** A special number that represents the midi channel of a node.

		This is used as a channel index value if you want to refer to the midi input
		or output instead of an audio channel.
	*/
	static const int midiChannelIndex;

	/** A special type of AudioProcessor that can live inside an AudioProcessorGraph
		in order to use the audio that comes into and out of the graph itself.

		If you create an AudioGraphIOProcessor in "input" mode, it will act as a
		node in the graph which delivers the audio that is coming into the parent
		graph. This allows you to stream the data to other nodes and process the
		incoming audio.

		Likewise, one of these in "output" mode can be sent data which it will add to
		the sum of data being sent to the graph's output.

		@see AudioProcessorGraph
	*/
	class JUCE_API  AudioGraphIOProcessor     : public AudioPluginInstance
	{
	public:
		/** Specifies the mode in which this processor will operate.
		*/
		enum IODeviceType
		{
			audioInputNode,     /**< In this mode, the processor has output channels
									 representing all the audio input channels that are
									 coming into its parent audio graph. */
			audioOutputNode,    /**< In this mode, the processor has input channels
									 representing all the audio output channels that are
									 going out of its parent audio graph. */
			midiInputNode,      /**< In this mode, the processor has a midi output which
									 delivers the same midi data that is arriving at its
									 parent graph. */
			midiOutputNode      /**< In this mode, the processor has a midi input and
									 any data sent to it will be passed out of the parent
									 graph. */
		};

		/** Returns the mode of this processor. */
		IODeviceType getType() const                                { return type; }

		/** Returns the parent graph to which this processor belongs, or nullptr if it
			hasn't yet been added to one. */
		AudioProcessorGraph* getParentGraph() const                 { return graph; }

		/** True if this is an audio or midi input. */
		bool isInput() const;
		/** True if this is an audio or midi output. */
		bool isOutput() const;

		AudioGraphIOProcessor (const IODeviceType type);
		~AudioGraphIOProcessor();

		const String getName() const;
		void fillInPluginDescription (PluginDescription&) const;

		void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock);
		void releaseResources();
		void processBlock (AudioSampleBuffer&, MidiBuffer&);

		const String getInputChannelName (int channelIndex) const;
		const String getOutputChannelName (int channelIndex) const;
		bool isInputChannelStereoPair (int index) const;
		bool isOutputChannelStereoPair (int index) const;
		bool acceptsMidi() const;
		bool producesMidi() const;

		bool hasEditor() const;
		AudioProcessorEditor* createEditor();

		int getNumParameters();
		const String getParameterName (int);
		float getParameter (int);
		const String getParameterText (int);
		void setParameter (int, float);

		int getNumPrograms();
		int getCurrentProgram();
		void setCurrentProgram (int);
		const String getProgramName (int);
		void changeProgramName (int, const String&);

		void getStateInformation (juce::MemoryBlock& destData);
		void setStateInformation (const void* data, int sizeInBytes);

		/** @internal */
		void setParentGraph (AudioProcessorGraph*);

	private:
		const IODeviceType type;
		AudioProcessorGraph* graph;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioGraphIOProcessor);
	};

	// AudioProcessor methods:

	const String getName() const;

	void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock);
	void releaseResources();
	void processBlock (AudioSampleBuffer&, MidiBuffer&);

	const String getInputChannelName (int channelIndex) const;
	const String getOutputChannelName (int channelIndex) const;
	bool isInputChannelStereoPair (int index) const;
	bool isOutputChannelStereoPair (int index) const;

	bool acceptsMidi() const;
	bool producesMidi() const;

	bool hasEditor() const                          { return false; }
	AudioProcessorEditor* createEditor()            { return nullptr; }

	int getNumParameters()                          { return 0; }
	const String getParameterName (int)             { return String::empty; }
	float getParameter (int)                        { return 0; }
	const String getParameterText (int)             { return String::empty; }
	void setParameter (int, float)                  { }

	int getNumPrograms()                            { return 0; }
	int getCurrentProgram()                         { return 0; }
	void setCurrentProgram (int)                    { }
	const String getProgramName (int)               { return String::empty; }
	void changeProgramName (int, const String&)     { }

	void getStateInformation (juce::MemoryBlock& destData);
	void setStateInformation (const void* data, int sizeInBytes);

	/** @internal */
	void handleAsyncUpdate();

private:

	ReferenceCountedArray <Node> nodes;
	OwnedArray <Connection> connections;
	uint32 lastNodeId;
	AudioSampleBuffer renderingBuffers;
	OwnedArray <MidiBuffer> midiBuffers;

	CriticalSection renderLock;
	Array<void*> renderingOps;

	friend class AudioGraphIOProcessor;
	AudioSampleBuffer* currentAudioInputBuffer;
	AudioSampleBuffer currentAudioOutputBuffer;
	MidiBuffer* currentMidiInputBuffer;
	MidiBuffer currentMidiOutputBuffer;

	void clearRenderingSequence();
	void buildRenderingSequence();

	bool isAnInputTo (uint32 possibleInputId, uint32 possibleDestinationId, int recursionCheck) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessorGraph);
};

#endif   // __JUCE_AUDIOPROCESSORGRAPH_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessorGraph.h ***/


#endif
#ifndef __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__

#endif
#ifndef __JUCE_GENERICAUDIOPROCESSOREDITOR_JUCEHEADER__

/*** Start of inlined file: juce_GenericAudioProcessorEditor.h ***/
#ifndef __JUCE_GENERICAUDIOPROCESSOREDITOR_JUCEHEADER__
#define __JUCE_GENERICAUDIOPROCESSOREDITOR_JUCEHEADER__

/**
	A type of UI component that displays the parameters of an AudioProcessor as
	a simple list of sliders.

	This can be used for showing an editor for a processor that doesn't supply
	its own custom editor.

	@see AudioProcessor
*/
class JUCE_API  GenericAudioProcessorEditor      : public AudioProcessorEditor
{
public:

	GenericAudioProcessorEditor (AudioProcessor* owner);
	~GenericAudioProcessorEditor();

	void paint (Graphics& g);
	void resized();

private:

	PropertyPanel panel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenericAudioProcessorEditor);
};

#endif   // __JUCE_GENERICAUDIOPROCESSOREDITOR_JUCEHEADER__

/*** End of inlined file: juce_GenericAudioProcessorEditor.h ***/


#endif
#ifndef __JUCE_PLUGINDESCRIPTION_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOPLUGINFORMAT_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOPLUGINFORMATMANAGER_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOUNITPLUGINFORMAT_JUCEHEADER__

/*** Start of inlined file: juce_AudioUnitPluginFormat.h ***/
#ifndef __JUCE_AUDIOUNITPLUGINFORMAT_JUCEHEADER__
#define __JUCE_AUDIOUNITPLUGINFORMAT_JUCEHEADER__

#if JUCE_PLUGINHOST_AU && JUCE_MAC

/**
	Implements a plugin format manager for AudioUnits.
*/
class JUCE_API  AudioUnitPluginFormat   : public AudioPluginFormat
{
public:

	AudioUnitPluginFormat();
	~AudioUnitPluginFormat();

	String getName() const                { return "AudioUnit"; }
	void findAllTypesForFile (OwnedArray <PluginDescription>& results, const String& fileOrIdentifier);
	AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc);
	bool fileMightContainThisPluginType (const String& fileOrIdentifier);
	String getNameOfPluginFromIdentifier (const String& fileOrIdentifier);
	StringArray searchPathsForPlugins (const FileSearchPath& directoriesToSearch, bool recursive);
	bool doesPluginStillExist (const PluginDescription& desc);
	FileSearchPath getDefaultLocationsToSearch();

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioUnitPluginFormat);
};

#endif

#endif   // __JUCE_AUDIOUNITPLUGINFORMAT_JUCEHEADER__

/*** End of inlined file: juce_AudioUnitPluginFormat.h ***/


#endif
#ifndef __JUCE_DIRECTXPLUGINFORMAT_JUCEHEADER__

/*** Start of inlined file: juce_DirectXPluginFormat.h ***/
#ifndef __JUCE_DIRECTXPLUGINFORMAT_JUCEHEADER__
#define __JUCE_DIRECTXPLUGINFORMAT_JUCEHEADER__

#if JUCE_PLUGINHOST_DX && JUCE_WINDOWS

//   Sorry, this file is just a placeholder at the moment!...

/**
	Implements a plugin format manager for DirectX plugins.
*/
class JUCE_API  DirectXPluginFormat   : public AudioPluginFormat
{
public:

	DirectXPluginFormat();
	~DirectXPluginFormat();

	String getName() const                { return "DirectX"; }
	void findAllTypesForFile (OwnedArray <PluginDescription>& results, const String& fileOrIdentifier);
	AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc);
	bool fileMightContainThisPluginType (const String& fileOrIdentifier);
	String getNameOfPluginFromIdentifier (const String& fileOrIdentifier)  { return fileOrIdentifier; }
	FileSearchPath getDefaultLocationsToSearch();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectXPluginFormat);
};

#endif

#endif   // __JUCE_DIRECTXPLUGINFORMAT_JUCEHEADER__

/*** End of inlined file: juce_DirectXPluginFormat.h ***/


#endif
#ifndef __JUCE_LADSPAPLUGINFORMAT_JUCEHEADER__

/*** Start of inlined file: juce_LADSPAPluginFormat.h ***/
#ifndef __JUCE_LADSPAPLUGINFORMAT_JUCEHEADER__
#define __JUCE_LADSPAPLUGINFORMAT_JUCEHEADER__

#if JUCE_PLUGINHOST_LADSPA && JUCE_LINUX

//   Sorry, this file is just a placeholder at the moment!...

/**
	Implements a plugin format manager for DirectX plugins.
*/
class JUCE_API  LADSPAPluginFormat   : public AudioPluginFormat
{
public:

	LADSPAPluginFormat();
	~LADSPAPluginFormat();

	String getName() const                { return "LADSPA"; }
	void findAllTypesForFile (OwnedArray <PluginDescription>& results, const String& fileOrIdentifier);
	AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc);
	bool fileMightContainThisPluginType (const String& fileOrIdentifier);
	String getNameOfPluginFromIdentifier (const String& fileOrIdentifier)  { return fileOrIdentifier; }
	FileSearchPath getDefaultLocationsToSearch();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LADSPAPluginFormat);
};

#endif

#endif   // __JUCE_LADSPAPLUGINFORMAT_JUCEHEADER__

/*** End of inlined file: juce_LADSPAPluginFormat.h ***/


#endif

/*** Start of inlined file: juce_VSTMidiEventList.h ***/
#ifdef __aeffect__

#ifndef __JUCE_VSTMIDIEVENTLIST_JUCEHEADER__
#define __JUCE_VSTMIDIEVENTLIST_JUCEHEADER__

/** Holds a set of VSTMidiEvent objects and makes it easy to add
	events to the list.

	This is used by both the VST hosting code and the plugin wrapper.
*/
class VSTMidiEventList
{
public:

	VSTMidiEventList()
		: numEventsUsed (0), numEventsAllocated (0)
	{
	}

	~VSTMidiEventList()
	{
		freeEvents();
	}

	void clear()
	{
		numEventsUsed = 0;

		if (events != nullptr)
			events->numEvents = 0;
	}

	void addEvent (const void* const midiData, const int numBytes, const int frameOffset)
	{
		ensureSize (numEventsUsed + 1);

		VstMidiEvent* const e = (VstMidiEvent*) (events->events [numEventsUsed]);
		events->numEvents = ++numEventsUsed;

		if (numBytes <= 4)
		{
			if (e->type == kVstSysExType)
			{
				delete[] (((VstMidiSysexEvent*) e)->sysexDump);
				e->type = kVstMidiType;
				e->byteSize = sizeof (VstMidiEvent);
				e->noteLength = 0;
				e->noteOffset = 0;
				e->detune = 0;
				e->noteOffVelocity = 0;
			}

			e->deltaFrames = frameOffset;
			memcpy (e->midiData, midiData, numBytes);
		}
		else
		{
			VstMidiSysexEvent* const se = (VstMidiSysexEvent*) e;

			if (se->type == kVstSysExType)
				delete[] se->sysexDump;

			se->sysexDump = new char [numBytes];
			memcpy (se->sysexDump, midiData, numBytes);

			se->type = kVstSysExType;
			se->byteSize = sizeof (VstMidiSysexEvent);
			se->deltaFrames = frameOffset;
			se->flags = 0;
			se->dumpBytes = numBytes;
			se->resvd1 = 0;
			se->resvd2 = 0;
		}
	}

	// Handy method to pull the events out of an event buffer supplied by the host
	// or plugin.
	static void addEventsToMidiBuffer (const VstEvents* events, MidiBuffer& dest)
	{
		for (int i = 0; i < events->numEvents; ++i)
		{
			const VstEvent* const e = events->events[i];

			if (e != nullptr)
			{
				if (e->type == kVstMidiType)
				{
					dest.addEvent ((const juce::uint8*) ((const VstMidiEvent*) e)->midiData,
								   4, e->deltaFrames);
				}
				else if (e->type == kVstSysExType)
				{
					dest.addEvent ((const juce::uint8*) ((const VstMidiSysexEvent*) e)->sysexDump,
								   (int) ((const VstMidiSysexEvent*) e)->dumpBytes,
								   e->deltaFrames);
				}
			}
		}
	}

	void ensureSize (int numEventsNeeded)
	{
		if (numEventsNeeded > numEventsAllocated)
		{
			numEventsNeeded = (numEventsNeeded + 32) & ~31;

			const int size = 20 + sizeof (VstEvent*) * numEventsNeeded;

			if (events == nullptr)
				events.calloc (size, 1);
			else
				events.realloc (size, 1);

			for (int i = numEventsAllocated; i < numEventsNeeded; ++i)
				events->events[i] = allocateVSTEvent();

			numEventsAllocated = numEventsNeeded;
		}
	}

	void freeEvents()
	{
		if (events != nullptr)
		{
			for (int i = numEventsAllocated; --i >= 0;)
				freeVSTEvent (events->events[i]);

			events.free();
			numEventsUsed = 0;
			numEventsAllocated = 0;
		}
	}

	HeapBlock <VstEvents> events;

private:
	int numEventsUsed, numEventsAllocated;

	static VstEvent* allocateVSTEvent()
	{
		VstEvent* const e = (VstEvent*) std::calloc (1, sizeof (VstMidiEvent) > sizeof (VstMidiSysexEvent) ? sizeof (VstMidiEvent)
																										   : sizeof (VstMidiSysexEvent));
		e->type = kVstMidiType;
		e->byteSize = sizeof (VstMidiEvent);
		return e;
	}

	static void freeVSTEvent (VstEvent* e)
	{
		if (e->type == kVstSysExType)
			delete[] (((VstMidiSysexEvent*) e)->sysexDump);

		std::free (e);
	}
};

#endif   // __JUCE_VSTMIDIEVENTLIST_JUCEHEADER__
#endif   // __JUCE_VSTMIDIEVENTLIST_JUCEHEADER__

/*** End of inlined file: juce_VSTMidiEventList.h ***/


#ifndef __JUCE_VSTPLUGINFORMAT_JUCEHEADER__

/*** Start of inlined file: juce_VSTPluginFormat.h ***/
#ifndef __JUCE_VSTPLUGINFORMAT_JUCEHEADER__
#define __JUCE_VSTPLUGINFORMAT_JUCEHEADER__

#if JUCE_PLUGINHOST_VST

/**
	Implements a plugin format manager for VSTs.
*/
class JUCE_API  VSTPluginFormat   : public AudioPluginFormat
{
public:

	VSTPluginFormat();
	~VSTPluginFormat();

	String getName() const                { return "VST"; }
	void findAllTypesForFile (OwnedArray <PluginDescription>& results, const String& fileOrIdentifier);
	AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc);
	bool fileMightContainThisPluginType (const String& fileOrIdentifier);
	String getNameOfPluginFromIdentifier (const String& fileOrIdentifier);
	StringArray searchPathsForPlugins (const FileSearchPath& directoriesToSearch, bool recursive);
	bool doesPluginStillExist (const PluginDescription& desc);
	FileSearchPath getDefaultLocationsToSearch();

private:

	void recursiveFileSearch (StringArray& results, const File& dir, const bool recursive);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VSTPluginFormat);
};

#endif
#endif   // __JUCE_VSTPLUGINFORMAT_JUCEHEADER__

/*** End of inlined file: juce_VSTPluginFormat.h ***/


#endif
#ifndef __JUCE_KNOWNPLUGINLIST_JUCEHEADER__

#endif
#ifndef __JUCE_PLUGINDIRECTORYSCANNER_JUCEHEADER__

/*** Start of inlined file: juce_PluginDirectoryScanner.h ***/
#ifndef __JUCE_PLUGINDIRECTORYSCANNER_JUCEHEADER__
#define __JUCE_PLUGINDIRECTORYSCANNER_JUCEHEADER__

/**
	Scans a directory for plugins, and adds them to a KnownPluginList.

	To use one of these, create it and call scanNextFile() repeatedly, until
	it returns false.
*/
class JUCE_API  PluginDirectoryScanner
{
public:

	/**
		Creates a scanner.

		@param listToAddResultsTo       this will get the new types added to it.
		@param formatToLookFor          this is the type of format that you want to look for
		@param directoriesToSearch      the path to search
		@param searchRecursively        true to search recursively
		@param deadMansPedalFile        if this isn't File::nonexistent, then it will
										be used as a file to store the names of any plugins
										that crash during initialisation. If there are
										any plugins listed in it, then these will always
										be scanned after all other possible files have
										been tried - in this way, even if there's a few
										dodgy plugins in your path, then a couple of rescans
										will still manage to find all the proper plugins.
										It's probably best to choose a file in the user's
										application data directory (alongside your app's
										settings file) for this. The file format it uses
										is just a list of filenames of the modules that
										failed.
	*/
	PluginDirectoryScanner (KnownPluginList& listToAddResultsTo,
							AudioPluginFormat& formatToLookFor,
							FileSearchPath directoriesToSearch,
							bool searchRecursively,
							const File& deadMansPedalFile);

	/** Destructor. */
	~PluginDirectoryScanner();

	/** Tries the next likely-looking file.

		If dontRescanIfAlreadyInList is true, then the file will only be loaded and
		re-tested if it's not already in the list, or if the file's modification
		time has changed since the list was created. If dontRescanIfAlreadyInList is
		false, the file will always be reloaded and tested.

		Returns false when there are no more files to try.
	*/
	bool scanNextFile (bool dontRescanIfAlreadyInList);

	/** Skips over the next file without scanning it.
		Returns false when there are no more files to try.
	*/
	bool skipNextFile();

	/** Returns the description of the plugin that will be scanned during the next
		call to scanNextFile().

		This is handy if you want to show the user which file is currently getting
		scanned.
	*/
	const String getNextPluginFileThatWillBeScanned() const;

	/** Returns the estimated progress, between 0 and 1.
	*/
	float getProgress() const                                       { return progress; }

	/** This returns a list of all the filenames of things that looked like being
		a plugin file, but which failed to open for some reason.
	*/
	const StringArray& getFailedFiles() const noexcept              { return failedFiles; }

private:

	KnownPluginList& list;
	AudioPluginFormat& format;
	StringArray filesOrIdentifiersToScan;
	File deadMansPedalFile;
	StringArray failedFiles;
	int nextIndex;
	float progress;

	StringArray getDeadMansPedalFile();
	void setDeadMansPedalFile (const StringArray& newContents);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginDirectoryScanner);
};

#endif   // __JUCE_PLUGINDIRECTORYSCANNER_JUCEHEADER__

/*** End of inlined file: juce_PluginDirectoryScanner.h ***/


#endif
#ifndef __JUCE_PLUGINLISTCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_PluginListComponent.h ***/
#ifndef __JUCE_PLUGINLISTCOMPONENT_JUCEHEADER__
#define __JUCE_PLUGINLISTCOMPONENT_JUCEHEADER__

/**
	A component displaying a list of plugins, with options to scan for them,
	add, remove and sort them.
*/
class JUCE_API  PluginListComponent   : public Component,
										public FileDragAndDropTarget,
										public ListBoxModel,
										private ChangeListener,
										private ButtonListener,  // (can't use Button::Listener due to idiotic VC2005 bug)
										private Timer
{
public:

	/**
		Creates the list component.

		For info about the deadMansPedalFile, see the PluginDirectoryScanner constructor.

		The properties file, if supplied, is used to store the user's last search paths.
	*/
	PluginListComponent (KnownPluginList& listToRepresent,
						 const File& deadMansPedalFile,
						 PropertiesFile* propertiesToUse);

	/** Destructor. */
	~PluginListComponent();

	/** @internal */
	void resized();
	/** @internal */
	bool isInterestedInFileDrag (const StringArray&);
	/** @internal */
	void filesDropped (const StringArray&, int, int);
	/** @internal */
	int getNumRows();
	/** @internal */
	void paintListBoxItem (int row, Graphics&, int width, int height, bool rowIsSelected);
	/** @internal */
	void deleteKeyPressed (int lastRowSelected);

private:

	KnownPluginList& list;
	File deadMansPedalFile;
	ListBox listBox;
	TextButton optionsButton;
	PropertiesFile* propertiesToUse;
	int typeToScan;

	void scanFor (AudioPluginFormat*);
	static void optionsMenuStaticCallback (int result, PluginListComponent*);
	void optionsMenuCallback (int result);
	void updateList();

	void buttonClicked (Button*);
	void changeListenerCallback (ChangeBroadcaster*);
	void timerCallback();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginListComponent);
};

#endif   // __JUCE_PLUGINLISTCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_PluginListComponent.h ***/


#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_AUDIO_PROCESSORS_JUCEHEADER__

/*** End of inlined file: juce_audio_processors.h ***/

