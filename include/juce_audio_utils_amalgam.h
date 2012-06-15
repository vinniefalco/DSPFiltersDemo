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


/*** Start of inlined file: juce_audio_utils.h ***/
#ifndef __JUCE_AUDIO_UTILS_JUCEHEADER__
#define __JUCE_AUDIO_UTILS_JUCEHEADER__

#include "juce_gui_basics_amalgam.h"
#include "juce_audio_devices_amalgam.h"
#include "juce_audio_formats_amalgam.h"
#include "juce_audio_processors_amalgam.h"

namespace juce
{

#ifndef __JUCE_AUDIODEVICESELECTORCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_AudioDeviceSelectorComponent.h ***/
#ifndef __JUCE_AUDIODEVICESELECTORCOMPONENT_JUCEHEADER__
#define __JUCE_AUDIODEVICESELECTORCOMPONENT_JUCEHEADER__

/**
	A component containing controls to let the user change the audio settings of
	an AudioDeviceManager object.

	Very easy to use - just create one of these and show it to the user.

	@see AudioDeviceManager
*/
class JUCE_API  AudioDeviceSelectorComponent  : public Component,
												public ComboBoxListener, // (can't use ComboBox::Listener due to idiotic VC2005 bug)
												public ChangeListener
{
public:

	/** Creates the component.

		If your app needs only output channels, you might ask for a maximum of 0 input
		channels, and the component won't display any options for choosing the input
		channels. And likewise if you're doing an input-only app.

		@param deviceManager            the device manager that this component should control
		@param minAudioInputChannels    the minimum number of audio input channels that the application needs
		@param maxAudioInputChannels    the maximum number of audio input channels that the application needs
		@param minAudioOutputChannels   the minimum number of audio output channels that the application needs
		@param maxAudioOutputChannels   the maximum number of audio output channels that the application needs
		@param showMidiInputOptions     if true, the component will allow the user to select which midi inputs are enabled
		@param showMidiOutputSelector   if true, the component will let the user choose a default midi output device
		@param showChannelsAsStereoPairs    if true, channels will be treated as pairs; if false, channels will be
										treated as a set of separate mono channels.
		@param hideAdvancedOptionsWithButton    if true, only the minimum amount of UI components
										are shown, with an "advanced" button that shows the rest of them
	*/
	AudioDeviceSelectorComponent (AudioDeviceManager& deviceManager,
								  const int minAudioInputChannels,
								  const int maxAudioInputChannels,
								  const int minAudioOutputChannels,
								  const int maxAudioOutputChannels,
								  const bool showMidiInputOptions,
								  const bool showMidiOutputSelector,
								  const bool showChannelsAsStereoPairs,
								  const bool hideAdvancedOptionsWithButton);

	/** Destructor */
	~AudioDeviceSelectorComponent();

	/** @internal */
	void resized();
	/** @internal */
	void comboBoxChanged (ComboBox*);
	/** @internal */
	void changeListenerCallback (ChangeBroadcaster*);
	/** @internal */
	void childBoundsChanged (Component*);

private:

	AudioDeviceManager& deviceManager;
	ScopedPointer<ComboBox> deviceTypeDropDown;
	ScopedPointer<Label> deviceTypeDropDownLabel;
	ScopedPointer<Component> audioDeviceSettingsComp;
	String audioDeviceSettingsCompType;
	const int minOutputChannels, maxOutputChannels, minInputChannels, maxInputChannels;
	const bool showChannelsAsStereoPairs;
	const bool hideAdvancedOptionsWithButton;

	class MidiInputSelectorComponentListBox;
	friend class ScopedPointer<MidiInputSelectorComponentListBox>;
	ScopedPointer<MidiInputSelectorComponentListBox> midiInputsList;
	ScopedPointer<ComboBox> midiOutputSelector;
	ScopedPointer<Label> midiInputsLabel, midiOutputLabel;

	void updateAllControls();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioDeviceSelectorComponent);
};

#endif   // __JUCE_AUDIODEVICESELECTORCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_AudioDeviceSelectorComponent.h ***/


#endif
#ifndef __JUCE_AUDIOTHUMBNAILBASE_JUCEHEADER__

/*** Start of inlined file: juce_AudioThumbnailBase.h ***/
#ifndef __JUCE_AUDIOTHUMBNAILBASE_JUCEHEADER__
#define __JUCE_AUDIOTHUMBNAILBASE_JUCEHEADER__

class AudioThumbnailCache;

/**
	Provides a base for classes that can store and draw scaled views of an
	audio waveform.

	Typically, you'll want to use the derived class AudioThumbnail, which provides
	a concrete implementation.

	@see AudioThumbnail, AudioThumbnailCache
*/
class JUCE_API  AudioThumbnailBase    : public ChangeBroadcaster,
										public AudioFormatWriter::ThreadedWriter::IncomingDataReceiver
{
public:

	AudioThumbnailBase() {}
	virtual ~AudioThumbnailBase() {}

	/** Clears and resets the thumbnail. */
	virtual void clear() = 0;

	/** Specifies the file or stream that contains the audio file.

		For a file, just call
		@code
		setSource (new FileInputSource (file))
		@endcode

		You can pass a zero in here to clear the thumbnail.
		The source that is passed in will be deleted by this object when it is no longer needed.
		@returns true if the source could be opened as a valid audio file, false if this failed for
		some reason.
	*/
	virtual bool setSource (InputSource* newSource) = 0;

	/** Gives the thumbnail an AudioFormatReader to use directly.
		This will start parsing the audio in a background thread (unless the hash code
		can be looked-up successfully in the thumbnail cache). Note that the reader
		object will be held by the thumbnail and deleted later when no longer needed.
		The thumbnail will actually keep hold of this reader until you clear the thumbnail
		or change the input source, so the file will be held open for all this time. If
		you don't want the thumbnail to keep a file handle open continuously, you
		should use the setSource() method instead, which will only open the file when
		it needs to.
	*/
	virtual void setReader (AudioFormatReader* newReader, int64 hashCode) = 0;

	/** Reloads the low res thumbnail data from an input stream.

		This is not an audio file stream! It takes a stream of thumbnail data that would
		previously have been created by the saveTo() method.
		@see saveTo
	*/
	virtual void loadFrom (InputStream& input) = 0;

	/** Saves the low res thumbnail data to an output stream.

		The data that is written can later be reloaded using loadFrom().
		@see loadFrom
	*/
	virtual void saveTo (OutputStream& output) const = 0;

	/** Returns the number of channels in the file. */
	virtual int getNumChannels() const noexcept = 0;

	/** Returns the length of the audio file, in seconds. */
	virtual double getTotalLength() const noexcept = 0;

	/** Draws the waveform for a channel.

		The waveform will be drawn within  the specified rectangle, where startTime
		and endTime specify the times within the audio file that should be positioned
		at the left and right edges of the rectangle.

		The waveform will be scaled vertically so that a full-volume sample will fill
		the rectangle vertically, but you can also specify an extra vertical scale factor
		with the verticalZoomFactor parameter.
	*/
	virtual void drawChannel (Graphics& g,
							  const Rectangle<int>& area,
							  double startTimeSeconds,
							  double endTimeSeconds,
							  int channelNum,
							  float verticalZoomFactor) = 0;

	/** Draws the waveforms for all channels in the thumbnail.

		This will call drawChannel() to render each of the thumbnail's channels, stacked
		above each other within the specified area.

		@see drawChannel
	*/
	virtual void drawChannels (Graphics& g,
							   const Rectangle<int>& area,
							   double startTimeSeconds,
							   double endTimeSeconds,
							   float verticalZoomFactor) = 0;

	/** Returns true if the low res preview is fully generated. */
	virtual bool isFullyLoaded() const noexcept = 0;

	/** Returns the number of samples that have been set in the thumbnail. */
	virtual int64 getNumSamplesFinished() const noexcept = 0;

	/** Returns the highest level in the thumbnail.
		Note that because the thumb only stores low-resolution data, this isn't
		an accurate representation of the highest value, it's only a rough approximation.
	*/
	virtual float getApproximatePeak() const = 0;

	/** Reads the approximate min and max levels from a section of the thumbnail.
		The lowest and highest samples are returned in minValue and maxValue, but obviously
		because the thumb only stores low-resolution data, these numbers will only be a rough
		approximation of the true values.
	*/
	virtual void getApproximateMinMax (double startTime, double endTime, int channelIndex,
									   float& minValue, float& maxValue) const noexcept = 0;

	/** Returns the hash code that was set by setSource() or setReader(). */
	virtual int64 getHashCode() const = 0;
};

#endif

/*** End of inlined file: juce_AudioThumbnailBase.h ***/


#endif
#ifndef __JUCE_AUDIOTHUMBNAIL_JUCEHEADER__

/*** Start of inlined file: juce_AudioThumbnail.h ***/
#ifndef __JUCE_AUDIOTHUMBNAIL_JUCEHEADER__
#define __JUCE_AUDIOTHUMBNAIL_JUCEHEADER__

class AudioThumbnailCache;

/**
	Makes it easy to quickly draw scaled views of the waveform shape of an
	audio file.

	To use this class, just create an AudioThumbNail class for the file you want
	to draw, call setSource to tell it which file or resource to use, then call
	drawChannel() to draw it.

	The class will asynchronously scan the wavefile to create its scaled-down view,
	so you should make your UI repaint itself as this data comes in. To do this, the
	AudioThumbnail is a ChangeBroadcaster, and will broadcast a message when its
	listeners should repaint themselves.

	The thumbnail stores an internal low-res version of the wave data, and this can
	be loaded and saved to avoid having to scan the file again.

	@see AudioThumbnailCache, AudioThumbnailBase
*/
class JUCE_API  AudioThumbnail    : public AudioThumbnailBase
{
public:

	/** Creates an audio thumbnail.

		@param sourceSamplesPerThumbnailSample  when creating a stored, low-res version
						of the audio data, this is the scale at which it should be done. (This
						number is the number of original samples that will be averaged for each
						low-res sample)
		@param formatManagerToUse   the audio format manager that is used to open the file
		@param cacheToUse   an instance of an AudioThumbnailCache - this provides a background
							thread and storage that is used to by the thumbnail, and the cache
							object can be shared between multiple thumbnails
	*/
	AudioThumbnail (int sourceSamplesPerThumbnailSample,
					AudioFormatManager& formatManagerToUse,
					AudioThumbnailCache& cacheToUse);

	/** Destructor. */
	~AudioThumbnail();

	/** Clears and resets the thumbnail. */
	void clear();

	/** Specifies the file or stream that contains the audio file.

		For a file, just call
		@code
		setSource (new FileInputSource (file))
		@endcode

		You can pass a zero in here to clear the thumbnail.
		The source that is passed in will be deleted by this object when it is no longer needed.
		@returns true if the source could be opened as a valid audio file, false if this failed for
		some reason.
	*/
	bool setSource (InputSource* newSource);

	/** Gives the thumbnail an AudioFormatReader to use directly.
		This will start parsing the audio in a background thread (unless the hash code
		can be looked-up successfully in the thumbnail cache). Note that the reader
		object will be held by the thumbnail and deleted later when no longer needed.
		The thumbnail will actually keep hold of this reader until you clear the thumbnail
		or change the input source, so the file will be held open for all this time. If
		you don't want the thumbnail to keep a file handle open continuously, you
		should use the setSource() method instead, which will only open the file when
		it needs to.
	*/
	void setReader (AudioFormatReader* newReader, int64 hashCode);

	/** Resets the thumbnail, ready for adding data with the specified format.
		If you're going to generate a thumbnail yourself, call this before using addBlock()
		to add the data.
	*/
	void reset (int numChannels, double sampleRate, int64 totalSamplesInSource = 0);

	/** Adds a block of level data to the thumbnail.
		Call reset() before using this, to tell the thumbnail about the data format.
	*/
	void addBlock (int64 sampleNumberInSource, const AudioSampleBuffer& newData,
				   int startOffsetInBuffer, int numSamples);

	/** Reloads the low res thumbnail data from an input stream.

		This is not an audio file stream! It takes a stream of thumbnail data that would
		previously have been created by the saveTo() method.
		@see saveTo
	*/
	void loadFrom (InputStream& input);

	/** Saves the low res thumbnail data to an output stream.

		The data that is written can later be reloaded using loadFrom().
		@see loadFrom
	*/
	void saveTo (OutputStream& output) const;

	/** Returns the number of channels in the file. */
	int getNumChannels() const noexcept;

	/** Returns the length of the audio file, in seconds. */
	double getTotalLength() const noexcept;

	/** Draws the waveform for a channel.

		The waveform will be drawn within  the specified rectangle, where startTime
		and endTime specify the times within the audio file that should be positioned
		at the left and right edges of the rectangle.

		The waveform will be scaled vertically so that a full-volume sample will fill
		the rectangle vertically, but you can also specify an extra vertical scale factor
		with the verticalZoomFactor parameter.
	*/
	void drawChannel (Graphics& g,
					  const Rectangle<int>& area,
					  double startTimeSeconds,
					  double endTimeSeconds,
					  int channelNum,
					  float verticalZoomFactor);

	/** Draws the waveforms for all channels in the thumbnail.

		This will call drawChannel() to render each of the thumbnail's channels, stacked
		above each other within the specified area.

		@see drawChannel
	*/
	void drawChannels (Graphics& g,
					   const Rectangle<int>& area,
					   double startTimeSeconds,
					   double endTimeSeconds,
					   float verticalZoomFactor);

	/** Returns true if the low res preview is fully generated. */
	bool isFullyLoaded() const noexcept;

	/** Returns the number of samples that have been set in the thumbnail. */
	int64 getNumSamplesFinished() const noexcept;

	/** Returns the highest level in the thumbnail.
		Note that because the thumb only stores low-resolution data, this isn't
		an accurate representation of the highest value, it's only a rough approximation.
	*/
	float getApproximatePeak() const;

	/** Reads the approximate min and max levels from a section of the thumbnail.
		The lowest and highest samples are returned in minValue and maxValue, but obviously
		because the thumb only stores low-resolution data, these numbers will only be a rough
		approximation of the true values.
	*/
	void getApproximateMinMax (double startTime, double endTime, int channelIndex,
							   float& minValue, float& maxValue) const noexcept;

	/** Returns the hash code that was set by setSource() or setReader(). */
	int64 getHashCode() const;

private:

	AudioFormatManager& formatManagerToUse;
	AudioThumbnailCache& cache;

	class LevelDataSource;
	struct MinMaxValue;
	class ThumbData;
	class CachedWindow;

	friend class LevelDataSource;
	friend class ScopedPointer<LevelDataSource>;
	friend class ThumbData;
	friend class OwnedArray<ThumbData>;
	friend class CachedWindow;
	friend class ScopedPointer<CachedWindow>;

	ScopedPointer<LevelDataSource> source;
	ScopedPointer<CachedWindow> window;
	OwnedArray<ThumbData> channels;

	int32 samplesPerThumbSample;
	int64 totalSamples, numSamplesFinished;
	int32 numChannels;
	double sampleRate;
	CriticalSection lock;

	void clearChannelData();
	bool setDataSource (LevelDataSource* newSource);
	void setLevels (const MinMaxValue* const* values, int thumbIndex, int numChans, int numValues);
	void createChannels (int length);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioThumbnail);
};

#endif   // __JUCE_AUDIOTHUMBNAIL_JUCEHEADER__

/*** End of inlined file: juce_AudioThumbnail.h ***/


#endif
#ifndef __JUCE_AUDIOTHUMBNAILCACHE_JUCEHEADER__

/*** Start of inlined file: juce_AudioThumbnailCache.h ***/
#ifndef __JUCE_AUDIOTHUMBNAILCACHE_JUCEHEADER__
#define __JUCE_AUDIOTHUMBNAILCACHE_JUCEHEADER__

/**
	An instance of this class is used to manage multiple AudioThumbnail objects.

	The cache runs a single background thread that is shared by all the thumbnails
	that need it, and it maintains a set of low-res previews in memory, to avoid
	having to re-scan audio files too often.

	@see AudioThumbnail
*/
class JUCE_API  AudioThumbnailCache   : public TimeSliceThread
{
public:

	/** Creates a cache object.

		The maxNumThumbsToStore parameter lets you specify how many previews should
		be kept in memory at once.
	*/
	explicit AudioThumbnailCache (int maxNumThumbsToStore);

	/** Destructor. */
	~AudioThumbnailCache();

	/** Clears out any stored thumbnails.
	*/
	void clear();

	/** Reloads the specified thumb if this cache contains the appropriate stored
		data.

		This is called automatically by the AudioThumbnail class, so you shouldn't
		normally need to call it directly.
	*/
	bool loadThumb (AudioThumbnailBase& thumb, int64 hashCode);

	/** Stores the cachable data from the specified thumb in this cache.

		This is called automatically by the AudioThumbnail class, so you shouldn't
		normally need to call it directly.
	*/
	void storeThumb (const AudioThumbnailBase& thumb, int64 hashCode);

	/** Attempts to re-load a saved cache of thumbnails from a stream.
		The cache data must have been written by the writeToStream() method.
		This will replace all currently-loaded thumbnails with the new data.
	*/
	bool readFromStream (InputStream& source);

	/** Writes all currently-loaded cache data to a stream.
		The resulting data can be re-loaded with readFromStream().
	*/
	void writeToStream (OutputStream& stream);

private:

	class ThumbnailCacheEntry;
	friend class OwnedArray<ThumbnailCacheEntry>;
	OwnedArray<ThumbnailCacheEntry> thumbs;
	CriticalSection lock;
	int maxNumThumbsToStore;

	ThumbnailCacheEntry* findThumbFor (int64 hash) const;
	int findOldestThumb() const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioThumbnailCache);
};

#endif   // __JUCE_AUDIOTHUMBNAILCACHE_JUCEHEADER__

/*** End of inlined file: juce_AudioThumbnailCache.h ***/


#endif
#ifndef __JUCE_MIDIKEYBOARDCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_MidiKeyboardComponent.h ***/
#ifndef __JUCE_MIDIKEYBOARDCOMPONENT_JUCEHEADER__
#define __JUCE_MIDIKEYBOARDCOMPONENT_JUCEHEADER__

/**
	A component that displays a piano keyboard, whose notes can be clicked on.

	This component will mimic a physical midi keyboard, showing the current state of
	a MidiKeyboardState object. When the on-screen keys are clicked on, it will play these
	notes by calling the noteOn() and noteOff() methods of its MidiKeyboardState object.

	Another feature is that the computer keyboard can also be used to play notes. By
	default it maps the top two rows of a standard querty keyboard to the notes, but
	these can be remapped if needed. It will only respond to keypresses when it has
	the keyboard focus, so to disable this feature you can call setWantsKeyboardFocus (false).

	The component is also a ChangeBroadcaster, so if you want to be informed when the
	keyboard is scrolled, you can register a ChangeListener for callbacks.

	@see MidiKeyboardState
*/
class JUCE_API  MidiKeyboardComponent  : public Component,
										 public MidiKeyboardStateListener,
										 public ChangeBroadcaster,
										 private Timer
{
public:

	/** The direction of the keyboard.

		@see setOrientation
	*/
	enum Orientation
	{
		horizontalKeyboard,
		verticalKeyboardFacingLeft,
		verticalKeyboardFacingRight,
	};

	/** Creates a MidiKeyboardComponent.

		@param state        the midi keyboard model that this component will represent
		@param orientation  whether the keyboard is horizonal or vertical
	*/
	MidiKeyboardComponent (MidiKeyboardState& state,
						   Orientation orientation);

	/** Destructor. */
	~MidiKeyboardComponent();

	/** Changes the velocity used in midi note-on messages that are triggered by clicking
		on the component.

		Values are 0 to 1.0, where 1.0 is the heaviest.

		@see setMidiChannel
	*/
	void setVelocity (float velocity, bool useMousePositionForVelocity);

	/** Changes the midi channel number that will be used for events triggered by clicking
		on the component.

		The channel must be between 1 and 16 (inclusive). This is the channel that will be
		passed on to the MidiKeyboardState::noteOn() method when the user clicks the component.

		Although this is the channel used for outgoing events, the component can display
		incoming events from more than one channel - see setMidiChannelsToDisplay()

		@see setVelocity
	*/
	void setMidiChannel (int midiChannelNumber);

	/** Returns the midi channel that the keyboard is using for midi messages.

		@see setMidiChannel
	*/
	int getMidiChannel() const noexcept                             { return midiChannel; }

	/** Sets a mask to indicate which incoming midi channels should be represented by
		key movements.

		The mask is a set of bits, where bit 0 = midi channel 1, bit 1 = midi channel 2, etc.

		If the MidiKeyboardState has a key down for any of the channels whose bits are set
		in this mask, the on-screen keys will also go down.

		By default, this mask is set to 0xffff (all channels displayed).

		@see setMidiChannel
	*/
	void setMidiChannelsToDisplay (int midiChannelMask);

	/** Returns the current set of midi channels represented by the component.

		This is the value that was set with setMidiChannelsToDisplay().
	*/
	int getMidiChannelsToDisplay() const noexcept                   { return midiInChannelMask; }

	/** Changes the width used to draw the white keys. */
	void setKeyWidth (float widthInPixels);

	/** Returns the width that was set by setKeyWidth(). */
	float getKeyWidth() const noexcept                              { return keyWidth; }

	/** Changes the keyboard's current direction. */
	void setOrientation (Orientation newOrientation);

	/** Returns the keyboard's current direction. */
	const Orientation getOrientation() const noexcept               { return orientation; }

	/** Sets the range of midi notes that the keyboard will be limited to.

		By default the range is 0 to 127 (inclusive), but you can limit this if you
		only want a restricted set of the keys to be shown.

		Note that the values here are inclusive and must be between 0 and 127.
	*/
	void setAvailableRange (int lowestNote,
							int highestNote);

	/** Returns the first note in the available range.

		@see setAvailableRange
	*/
	int getRangeStart() const noexcept                              { return rangeStart; }

	/** Returns the last note in the available range.

		@see setAvailableRange
	*/
	int getRangeEnd() const noexcept                                { return rangeEnd; }

	/** If the keyboard extends beyond the size of the component, this will scroll
		it to show the given key at the start.

		Whenever the keyboard's position is changed, this will use the ChangeBroadcaster
		base class to send a callback to any ChangeListeners that have been registered.
	*/
	void setLowestVisibleKey (int noteNumber);

	/** Returns the number of the first key shown in the component.

		@see setLowestVisibleKey
	*/
	int getLowestVisibleKey() const noexcept                        { return (int) firstKey; }

	/** Returns the length of the black notes.

		This will be their vertical or horizontal length, depending on the keyboard's orientation.
	*/
	int getBlackNoteLength() const noexcept                         { return blackNoteLength; }

	/** If set to true, then scroll buttons will appear at either end of the keyboard
		if there are too many notes to fit them all in the component at once.
	*/
	void setScrollButtonsVisible (bool canScroll);

	/** A set of colour IDs to use to change the colour of various aspects of the keyboard.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		whiteNoteColourId               = 0x1005000,
		blackNoteColourId               = 0x1005001,
		keySeparatorLineColourId        = 0x1005002,
		mouseOverKeyOverlayColourId     = 0x1005003,  /**< This colour will be overlaid on the normal note colour. */
		keyDownOverlayColourId          = 0x1005004,  /**< This colour will be overlaid on the normal note colour. */
		textLabelColourId               = 0x1005005,
		upDownButtonBackgroundColourId  = 0x1005006,
		upDownButtonArrowColourId       = 0x1005007
	};

	/** Returns the position within the component of the left-hand edge of a key.

		Depending on the keyboard's orientation, this may be a horizontal or vertical
		distance, in either direction.
	*/
	int getKeyStartPosition (const int midiNoteNumber) const;

	/** Deletes all key-mappings.

		@see setKeyPressForNote
	*/
	void clearKeyMappings();

	/** Maps a key-press to a given note.

		@param key                  the key that should trigger the note
		@param midiNoteOffsetFromC  how many semitones above C the triggered note should
									be. The actual midi note that gets played will be
									this value + (12 * the current base octave). To change
									the base octave, see setKeyPressBaseOctave()
	*/
	void setKeyPressForNote (const KeyPress& key,
							 int midiNoteOffsetFromC);

	/** Removes any key-mappings for a given note.

		For a description of what the note number means, see setKeyPressForNote().
	*/
	void removeKeyPressForNote (int midiNoteOffsetFromC);

	/** Changes the base note above which key-press-triggered notes are played.

		The set of key-mappings that trigger notes can be moved up and down to cover
		the entire scale using this method.

		The value passed in is an octave number between 0 and 10 (inclusive), and
		indicates which C is the base note to which the key-mapped notes are
		relative.
	*/
	void setKeyPressBaseOctave (int newOctaveNumber);

	/** This sets the octave number which is shown as the octave number for middle C.

		This affects only the default implementation of getWhiteNoteText(), which
		passes this octave number to MidiMessage::getMidiNoteName() in order to
		get the note text. See MidiMessage::getMidiNoteName() for more info about
		the parameter.

		By default this value is set to 3.

		@see getOctaveForMiddleC
	*/
	void setOctaveForMiddleC (int octaveNumForMiddleC);

	/** This returns the value set by setOctaveForMiddleC().
		@see setOctaveForMiddleC
	*/
	int getOctaveForMiddleC() const noexcept            { return octaveNumForMiddleC; }

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void mouseMove (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseUp (const MouseEvent& e);
	/** @internal */
	void mouseEnter (const MouseEvent& e);
	/** @internal */
	void mouseExit (const MouseEvent& e);
	/** @internal */
	void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
	/** @internal */
	void timerCallback();
	/** @internal */
	bool keyStateChanged (bool isKeyDown);
	/** @internal */
	void focusLost (FocusChangeType cause);
	/** @internal */
	void handleNoteOn (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity);
	/** @internal */
	void handleNoteOff (MidiKeyboardState* source, int midiChannel, int midiNoteNumber);
	/** @internal */
	void colourChanged();

protected:

	/** Draws a white note in the given rectangle.

		isOver indicates whether the mouse is over the key, isDown indicates whether the key is
		currently pressed down.

		When doing this, be sure to note the keyboard's orientation.
	*/
	virtual void drawWhiteNote (int midiNoteNumber,
								Graphics& g,
								int x, int y, int w, int h,
								bool isDown, bool isOver,
								const Colour& lineColour,
								const Colour& textColour);

	/** Draws a black note in the given rectangle.

		isOver indicates whether the mouse is over the key, isDown indicates whether the key is
		currently pressed down.

		When doing this, be sure to note the keyboard's orientation.
	*/
	virtual void drawBlackNote (int midiNoteNumber,
								Graphics& g,
								int x, int y, int w, int h,
								bool isDown, bool isOver,
								const Colour& noteFillColour);

	/** Allows text to be drawn on the white notes.

		By default this is used to label the C in each octave, but could be used for other things.

		@see setOctaveForMiddleC
	*/
	virtual String getWhiteNoteText (const int midiNoteNumber);

	/** Draws the up and down buttons that change the base note. */
	virtual void drawUpDownButton (Graphics& g, int w, int h,
								   const bool isMouseOver,
								   const bool isButtonPressed,
								   const bool movesOctavesUp);

	/** Callback when the mouse is clicked on a key.

		You could use this to do things like handle right-clicks on keys, etc.

		Return true if you want the click to trigger the note, or false if you
		want to handle it yourself and not have the note played.

		@see mouseDraggedToKey
	*/
	virtual bool mouseDownOnKey (int midiNoteNumber, const MouseEvent& e);

	/** Callback when the mouse is dragged from one key onto another.

		@see mouseDownOnKey
	*/
	virtual void mouseDraggedToKey (int midiNoteNumber, const MouseEvent& e);

	/** Calculates the positon of a given midi-note.

		This can be overridden to create layouts with custom key-widths.

		@param midiNoteNumber   the note to find
		@param keyWidth         the desired width in pixels of one key - see setKeyWidth()
		@param x                the x position of the left-hand edge of the key (this method
								always works in terms of a horizontal keyboard)
		@param w                the width of the key
	*/
	virtual void getKeyPosition (int midiNoteNumber, float keyWidth,
								 int& x, int& w) const;

private:

	friend class MidiKeyboardUpDownButton;

	MidiKeyboardState& state;
	int xOffset, blackNoteLength;
	float keyWidth;
	Orientation orientation;

	int midiChannel, midiInChannelMask;
	float velocity;

	Array<int> mouseOverNotes, mouseDownNotes;
	BigInteger keysPressed, keysCurrentlyDrawnDown;
	bool shouldCheckState;

	int rangeStart, rangeEnd;
	float firstKey;
	bool canScroll, useMousePositionForVelocity, shouldCheckMousePos;
	ScopedPointer<Button> scrollDown, scrollUp;

	Array <KeyPress> keyPresses;
	Array <int> keyPressNotes;
	int keyMappingOctave;
	int octaveNumForMiddleC;

	static const uint8 whiteNotes[];
	static const uint8 blackNotes[];

	void getKeyPos (int midiNoteNumber, int& x, int& w) const;
	int xyToNote (const Point<int>&, float& mousePositionVelocity);
	int remappedXYToNote (const Point<int>&, float& mousePositionVelocity) const;
	void resetAnyKeysInUse();
	void updateNoteUnderMouse (const Point<int>&, bool isDown, int fingerNum);
	void updateNoteUnderMouse (const MouseEvent&, bool isDown);
	void repaintNote (const int midiNoteNumber);
	void setLowestVisibleKeyFloat (float noteNumber);
	Rectangle<int> getWhiteNotePos (int noteNumber) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiKeyboardComponent);
};

#endif   // __JUCE_MIDIKEYBOARDCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_MidiKeyboardComponent.h ***/


#endif
#ifndef __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__

/*** Start of inlined file: juce_AudioProcessorPlayer.h ***/
#ifndef __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__
#define __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__

/**
	An AudioIODeviceCallback object which streams audio through an AudioProcessor.

	To use one of these, just make it the callback used by your AudioIODevice, and
	give it a processor to use by calling setProcessor().

	It's also a MidiInputCallback, so you can connect it to both an audio and midi
	input to send both streams through the processor.

	@see AudioProcessor, AudioProcessorGraph
*/
class JUCE_API  AudioProcessorPlayer    : public AudioIODeviceCallback,
										  public MidiInputCallback
{
public:

	AudioProcessorPlayer();

	/** Destructor. */
	virtual ~AudioProcessorPlayer();

	/** Sets the processor that should be played.

		The processor that is passed in will not be deleted or owned by this object.
		To stop anything playing, pass in 0 to this method.
	*/
	void setProcessor (AudioProcessor* processorToPlay);

	/** Returns the current audio processor that is being played.
	*/
	AudioProcessor* getCurrentProcessor() const                     { return processor; }

	/** Returns a midi message collector that you can pass midi messages to if you
		want them to be injected into the midi stream that is being sent to the
		processor.
	*/
	MidiMessageCollector& getMidiMessageCollector()                 { return messageCollector; }

	/** @internal */
	void audioDeviceIOCallback (const float** inputChannelData,
								int totalNumInputChannels,
								float** outputChannelData,
								int totalNumOutputChannels,
								int numSamples);
	/** @internal */
	void audioDeviceAboutToStart (AudioIODevice* device);
	/** @internal */
	void audioDeviceStopped();
	/** @internal */
	void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);

private:

	AudioProcessor* processor;
	CriticalSection lock;
	double sampleRate;
	int blockSize;
	bool isPrepared;

	int numInputChans, numOutputChans;
	float* channels [128];
	AudioSampleBuffer tempBuffer;

	MidiBuffer incomingMidi;
	MidiMessageCollector messageCollector;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessorPlayer);
};

#endif   // __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessorPlayer.h ***/


#endif

}

#endif   // __JUCE_AUDIO_UTILS_JUCEHEADER__

/*** End of inlined file: juce_audio_utils.h ***/

