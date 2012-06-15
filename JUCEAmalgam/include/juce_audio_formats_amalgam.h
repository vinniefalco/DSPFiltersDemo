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

// must come before juce_audio_devices.h because of JUCE_CD_READER

/*** Start of inlined file: juce_audio_formats.h ***/
#ifndef __JUCE_AUDIO_FORMATS_JUCEHEADER__
#define __JUCE_AUDIO_FORMATS_JUCEHEADER__

#include "juce_audio_basics_amalgam.h"

/** Config: JUCE_USE_FLAC
	Enables the FLAC audio codec classes (available on all platforms).
	If your app doesn't need to read FLAC files, you might want to disable this to
	reduce the size of your codebase and build time.
*/
#ifndef JUCE_USE_FLAC
 #define JUCE_USE_FLAC 1
#endif

/** Config: JUCE_USE_OGGVORBIS
	Enables the Ogg-Vorbis audio codec classes (available on all platforms).
	If your app doesn't need to read Ogg-Vorbis files, you might want to disable this to
	reduce the size of your codebase and build time.
*/
#ifndef JUCE_USE_OGGVORBIS
 #define JUCE_USE_OGGVORBIS 1
#endif

/** Config: JUCE_USE_MP3AUDIOFORMAT
	Enables the software-based MP3AudioFormat class.
	IMPORTANT DISCLAIMER: By choosing to enable the JUCE_USE_MP3AUDIOFORMAT flag and to compile
	this MP3 code into your software, you do so AT YOUR OWN RISK! By doing so, you are agreeing
	that Raw Material Software is in no way responsible for any patent, copyright, or other
	legal issues that you may suffer as a result.

	The code in juce_MP3AudioFormat.cpp is NOT guaranteed to be free from infringements of 3rd-party
	intellectual property. If you wish to use it, please seek your own independent advice about the
	legality of doing so. If you are not willing to accept full responsibility for the consequences
	of using this code, then do not enable this setting.
*/
#ifndef JUCE_USE_MP3AUDIOFORMAT
 #define JUCE_USE_MP3AUDIOFORMAT 0
#endif

/** Config: JUCE_USE_WINDOWS_MEDIA_FORMAT
	Enables the Windows Media SDK codecs.
*/
#ifndef JUCE_USE_WINDOWS_MEDIA_FORMAT
 #define JUCE_USE_WINDOWS_MEDIA_FORMAT 1
#endif

#if ! JUCE_MSVC
 #undef JUCE_USE_WINDOWS_MEDIA_FORMAT
 #define JUCE_USE_WINDOWS_MEDIA_FORMAT 0
#endif

namespace juce
{

// START_AUTOINCLUDE format, codecs, sampler
#ifndef __JUCE_AUDIOFORMAT_JUCEHEADER__

/*** Start of inlined file: juce_AudioFormat.h ***/
#ifndef __JUCE_AUDIOFORMAT_JUCEHEADER__
#define __JUCE_AUDIOFORMAT_JUCEHEADER__


/*** Start of inlined file: juce_AudioFormatReader.h ***/
#ifndef __JUCE_AUDIOFORMATREADER_JUCEHEADER__
#define __JUCE_AUDIOFORMATREADER_JUCEHEADER__

class AudioFormat;

/**
	Reads samples from an audio file stream.

	A subclass that reads a specific type of audio format will be created by
	an AudioFormat object.

	@see AudioFormat, AudioFormatWriter
*/
class JUCE_API  AudioFormatReader
{
protected:

	/** Creates an AudioFormatReader object.

		@param sourceStream     the stream to read from - this will be deleted
								by this object when it is no longer needed. (Some
								specialised readers might not use this parameter and
								can leave it as 0).
		@param formatName       the description that will be returned by the getFormatName()
								method
	*/
	AudioFormatReader (InputStream* sourceStream,
					   const String& formatName);

public:
	/** Destructor. */
	virtual ~AudioFormatReader();

	/** Returns a description of what type of format this is.

		E.g. "AIFF"
	*/
	const String& getFormatName() const noexcept    { return formatName; }

	/** Reads samples from the stream.

		@param destSamples          an array of buffers into which the sample data for each
									channel will be written.
									If the format is fixed-point, each channel will be written
									as an array of 32-bit signed integers using the full
									range -0x80000000 to 0x7fffffff, regardless of the source's
									bit-depth. If it is a floating-point format, you should cast
									the resulting array to a (float**) to get the values (in the
									range -1.0 to 1.0 or beyond)
									If the format is stereo, then destSamples[0] is the left channel
									data, and destSamples[1] is the right channel.
									The numDestChannels parameter indicates how many pointers this array
									contains, but some of these pointers can be null if you don't want to
									read data for some of the channels
		@param numDestChannels      the number of array elements in the destChannels array
		@param startSampleInSource  the position in the audio file or stream at which the samples
									should be read, as a number of samples from the start of the
									stream. It's ok for this to be beyond the start or end of the
									available data - any samples that are out-of-range will be returned
									as zeros.
		@param numSamplesToRead     the number of samples to read. If this is greater than the number
									of samples that the file or stream contains. the result will be padded
									with zeros
		@param fillLeftoverChannelsWithCopies   if true, this indicates that if there's no source data available
									for some of the channels that you pass in, then they should be filled with
									copies of valid source channels.
									E.g. if you're reading a mono file and you pass 2 channels to this method, then
									if fillLeftoverChannelsWithCopies is true, both destination channels will be filled
									with the same data from the file's single channel. If fillLeftoverChannelsWithCopies
									was false, then only the first channel would be filled with the file's contents, and
									the second would be cleared. If there are many channels, e.g. you try to read 4 channels
									from a stereo file, then the last 3 would all end up with copies of the same data.
		@returns                    true if the operation succeeded, false if there was an error. Note
									that reading sections of data beyond the extent of the stream isn't an
									error - the reader should just return zeros for these regions
		@see readMaxLevels
	*/
	bool read (int* const* destSamples,
			   int numDestChannels,
			   int64 startSampleInSource,
			   int numSamplesToRead,
			   bool fillLeftoverChannelsWithCopies);

	/** Fills a section of an AudioSampleBuffer from this reader.

		This will convert the reader's fixed- or floating-point data to
		the buffer's floating-point format, and will try to intelligently
		cope with mismatches between the number of channels in the reader
		and the buffer.
	*/
	void read (AudioSampleBuffer* buffer,
			   int startSampleInDestBuffer,
			   int numSamples,
			   int64 readerStartSample,
			   bool useReaderLeftChan,
			   bool useReaderRightChan);

	/** Finds the highest and lowest sample levels from a section of the audio stream.

		This will read a block of samples from the stream, and measure the
		highest and lowest sample levels from the channels in that section, returning
		these as normalised floating-point levels.

		@param startSample          the offset into the audio stream to start reading from. It's
									ok for this to be beyond the start or end of the stream.
		@param numSamples           how many samples to read
		@param lowestLeft           on return, this is the lowest absolute sample from the left channel
		@param highestLeft          on return, this is the highest absolute sample from the left channel
		@param lowestRight          on return, this is the lowest absolute sample from the right
									channel (if there is one)
		@param highestRight         on return, this is the highest absolute sample from the right
									channel (if there is one)
		@see read
	*/
	virtual void readMaxLevels (int64 startSample,
								int64 numSamples,
								float& lowestLeft,
								float& highestLeft,
								float& lowestRight,
								float& highestRight);

	/** Scans the source looking for a sample whose magnitude is in a specified range.

		This will read from the source, either forwards or backwards between two sample
		positions, until it finds a sample whose magnitude lies between two specified levels.

		If it finds a suitable sample, it returns its position; if not, it will return -1.

		There's also a minimumConsecutiveSamples setting to help avoid spikes or zero-crossing
		points when you're searching for a continuous range of samples

		@param startSample              the first sample to look at
		@param numSamplesToSearch       the number of samples to scan. If this value is negative,
										the search will go backwards
		@param magnitudeRangeMinimum    the lowest magnitude (inclusive) that is considered a hit, from 0 to 1.0
		@param magnitudeRangeMaximum    the highest magnitude (inclusive) that is considered a hit, from 0 to 1.0
		@param minimumConsecutiveSamples    if this is > 0, the method will only look for a sequence
											of this many consecutive samples, all of which lie
											within the target range. When it finds such a sequence,
											it returns the position of the first in-range sample
											it found (i.e. the earliest one if scanning forwards, the
											latest one if scanning backwards)
	*/
	int64 searchForLevel (int64 startSample,
						  int64 numSamplesToSearch,
						  double magnitudeRangeMinimum,
						  double magnitudeRangeMaximum,
						  int minimumConsecutiveSamples);

	/** The sample-rate of the stream. */
	double sampleRate;

	/** The number of bits per sample, e.g. 16, 24, 32. */
	unsigned int bitsPerSample;

	/** The total number of samples in the audio stream. */
	int64 lengthInSamples;

	/** The total number of channels in the audio stream. */
	unsigned int numChannels;

	/** Indicates whether the data is floating-point or fixed. */
	bool usesFloatingPointData;

	/** A set of metadata values that the reader has pulled out of the stream.

		Exactly what these values are depends on the format, so you can
		check out the format implementation code to see what kind of stuff
		they understand.
	*/
	StringPairArray metadataValues;

	/** The input stream, for use by subclasses. */
	InputStream* input;

	/** Subclasses must implement this method to perform the low-level read operation.

		Callers should use read() instead of calling this directly.

		@param destSamples  the array of destination buffers to fill. Some of these
							pointers may be null
		@param numDestChannels  the number of items in the destSamples array. This
							value is guaranteed not to be greater than the number of
							channels that this reader object contains
		@param startOffsetInDestBuffer      the number of samples from the start of the
							dest data at which to begin writing
		@param startSampleInFile    the number of samples into the source data at which
							to begin reading. This value is guaranteed to be >= 0.
		@param numSamples   the number of samples to read
	*/
	virtual bool readSamples (int** destSamples,
							  int numDestChannels,
							  int startOffsetInDestBuffer,
							  int64 startSampleInFile,
							  int numSamples) = 0;

protected:

	/** Used by AudioFormatReader subclasses to copy data to different formats. */
	template <class DestSampleType, class SourceSampleType, class SourceEndianness>
	struct ReadHelper
	{
		typedef AudioData::Pointer <DestSampleType, AudioData::NativeEndian, AudioData::NonInterleaved, AudioData::NonConst>    DestType;
		typedef AudioData::Pointer <SourceSampleType, SourceEndianness, AudioData::Interleaved, AudioData::Const>               SourceType;

		static void read (int** destData, int destOffset, int numDestChannels, const void* sourceData, int numSourceChannels, int numSamples) noexcept
		{
			for (int i = 0; i < numDestChannels; ++i)
			{
				if (destData[i] != nullptr)
				{
					DestType dest (destData[i]);
					dest += destOffset;

					if (i < numSourceChannels)
						dest.convertSamples (SourceType (addBytesToPointer (sourceData, i * SourceType::getBytesPerSample()), numSourceChannels), numSamples);
					else
						dest.clearSamples (numSamples);
				}
			}
		}
	};

private:
	String formatName;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFormatReader);
};

#endif   // __JUCE_AUDIOFORMATREADER_JUCEHEADER__

/*** End of inlined file: juce_AudioFormatReader.h ***/


/*** Start of inlined file: juce_AudioFormatWriter.h ***/
#ifndef __JUCE_AUDIOFORMATWRITER_JUCEHEADER__
#define __JUCE_AUDIOFORMATWRITER_JUCEHEADER__

/**
	Writes samples to an audio file stream.

	A subclass that writes a specific type of audio format will be created by
	an AudioFormat object.

	After creating one of these with the AudioFormat::createWriterFor() method
	you can call its write() method to store the samples, and then delete it.

	@see AudioFormat, AudioFormatReader
*/
class JUCE_API  AudioFormatWriter
{
protected:

	/** Creates an AudioFormatWriter object.

		@param destStream       the stream to write to - this will be deleted
								by this object when it is no longer needed
		@param formatName       the description that will be returned by the getFormatName()
								method
		@param sampleRate       the sample rate to use - the base class just stores
								this value, it doesn't do anything with it
		@param numberOfChannels the number of channels to write - the base class just stores
								this value, it doesn't do anything with it
		@param bitsPerSample    the bit depth of the stream - the base class just stores
								this value, it doesn't do anything with it
	*/
	AudioFormatWriter (OutputStream* destStream,
					   const String& formatName,
					   double sampleRate,
					   unsigned int numberOfChannels,
					   unsigned int bitsPerSample);

public:
	/** Destructor. */
	virtual ~AudioFormatWriter();

	/** Returns a description of what type of format this is.

		E.g. "AIFF file"
	*/
	const String& getFormatName() const noexcept        { return formatName; }

	/** Writes a set of samples to the audio stream.

		Note that if you're trying to write the contents of an AudioSampleBuffer, you
		can use AudioSampleBuffer::writeToAudioWriter().

		@param samplesToWrite   an array of arrays containing the sample data for
								each channel to write. This is a zero-terminated
								array of arrays, and can contain a different number
								of channels than the actual stream uses, and the
								writer should do its best to cope with this.
								If the format is fixed-point, each channel will be formatted
								as an array of signed integers using the full 32-bit
								range -0x80000000 to 0x7fffffff, regardless of the source's
								bit-depth. If it is a floating-point format, you should treat
								the arrays as arrays of floats, and just cast it to an (int**)
								to pass it into the method.
		@param numSamples       the number of samples to write
	*/
	virtual bool write (const int** samplesToWrite,
						int numSamples) = 0;

	/** Reads a section of samples from an AudioFormatReader, and writes these to
		the output.

		This will take care of any floating-point conversion that's required to convert
		between the two formats. It won't deal with sample-rate conversion, though.

		If numSamplesToRead < 0, it will write the entire length of the reader.

		@returns false if it can't read or write properly during the operation
	*/
	bool writeFromAudioReader (AudioFormatReader& reader,
							   int64 startSample,
							   int64 numSamplesToRead);

	/** Reads some samples from an AudioSource, and writes these to the output.

		The source must already have been initialised with the AudioSource::prepareToPlay() method

		@param source               the source to read from
		@param numSamplesToRead     total number of samples to read and write
		@param samplesPerBlock      the maximum number of samples to fetch from the source
		@returns false if it can't read or write properly during the operation
	*/
	bool writeFromAudioSource (AudioSource& source,
							   int numSamplesToRead,
							   int samplesPerBlock = 2048);

	/** Writes some samples from an AudioSampleBuffer. */
	bool writeFromAudioSampleBuffer (const AudioSampleBuffer& source,
									 int startSample, int numSamples);

	/** Returns the sample rate being used. */
	double getSampleRate() const noexcept       { return sampleRate; }

	/** Returns the number of channels being written. */
	int getNumChannels() const noexcept         { return (int) numChannels; }

	/** Returns the bit-depth of the data being written. */
	int getBitsPerSample() const noexcept       { return (int) bitsPerSample; }

	/** Returns true if it's a floating-point format, false if it's fixed-point. */
	bool isFloatingPoint() const noexcept       { return usesFloatingPointData; }

	/**
		Provides a FIFO for an AudioFormatWriter, allowing you to push incoming
		data into a buffer which will be flushed to disk by a background thread.
	*/
	class ThreadedWriter
	{
	public:
		/** Creates a ThreadedWriter for a given writer and a thread.

			The writer object which is passed in here will be owned and deleted by
			the ThreadedWriter when it is no longer needed.

			To stop the writer and flush the buffer to disk, simply delete this object.
		*/
		ThreadedWriter (AudioFormatWriter* writer,
						TimeSliceThread& backgroundThread,
						int numSamplesToBuffer);

		/** Destructor. */
		~ThreadedWriter();

		/** Pushes some incoming audio data into the FIFO.

			If there's enough free space in the buffer, this will add the data to it,

			If the FIFO is too full to accept this many samples, the method will return
			false - then you could either wait until the background thread has had time to
			consume some of the buffered data and try again, or you can give up
			and lost this block.

			The data must be an array containing the same number of channels as the
			AudioFormatWriter object is using. None of these channels can be null.
		*/
		bool write (const float** data, int numSamples);

		class JUCE_API  IncomingDataReceiver
		{
		public:
			IncomingDataReceiver() {}
			virtual ~IncomingDataReceiver() {}

			virtual void reset (int numChannels, double sampleRate, int64 totalSamplesInSource) = 0;
			virtual void addBlock (int64 sampleNumberInSource, const AudioSampleBuffer& newData,
								   int startOffsetInBuffer, int numSamples) = 0;
		};

		/** Allows you to specify a callback that this writer should update with the
			incoming data.
			The receiver will be cleared and will the writer will begin adding data to
			it as the data arrives. Pass a null pointer to remove the current receiver.

			The object passed-in must not be deleted while this writer is still using it.
		*/
		void setDataReceiver (IncomingDataReceiver* receiver);

	private:
		class Buffer;
		friend class ScopedPointer<Buffer>;
		ScopedPointer<Buffer> buffer;
	};

protected:

	/** The sample rate of the stream. */
	double sampleRate;

	/** The number of channels being written to the stream. */
	unsigned int numChannels;

	/** The bit depth of the file. */
	unsigned int bitsPerSample;

	/** True if it's a floating-point format, false if it's fixed-point. */
	bool usesFloatingPointData;

	/** The output stream for Use by subclasses. */
	OutputStream* output;

	/** Used by AudioFormatWriter subclasses to copy data to different formats. */
	template <class DestSampleType, class SourceSampleType, class DestEndianness>
	struct WriteHelper
	{
		typedef AudioData::Pointer <DestSampleType, DestEndianness, AudioData::Interleaved, AudioData::NonConst>                DestType;
		typedef AudioData::Pointer <SourceSampleType, AudioData::NativeEndian, AudioData::NonInterleaved, AudioData::Const>     SourceType;

		static void write (void* destData, int numDestChannels, const int** source,
						   int numSamples, const int sourceOffset = 0) noexcept
		{
			for (int i = 0; i < numDestChannels; ++i)
			{
				const DestType dest (addBytesToPointer (destData, i * DestType::getBytesPerSample()), numDestChannels);

				if (*source != nullptr)
				{
					dest.convertSamples (SourceType (*source + sourceOffset), numSamples);
					++source;
				}
				else
				{
					dest.clearSamples (numSamples);
				}
			}
		}
	};

private:
	String formatName;
	friend class ThreadedWriter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFormatWriter);
};

#endif   // __JUCE_AUDIOFORMATWRITER_JUCEHEADER__

/*** End of inlined file: juce_AudioFormatWriter.h ***/

/**
	Subclasses of AudioFormat are used to read and write different audio
	file formats.

	@see AudioFormatReader, AudioFormatWriter, WavAudioFormat, AiffAudioFormat
*/
class JUCE_API  AudioFormat
{
public:

	/** Destructor. */
	virtual ~AudioFormat();

	/** Returns the name of this format.

		e.g. "WAV file" or "AIFF file"
	*/
	const String& getFormatName() const;

	/** Returns all the file extensions that might apply to a file of this format.

		The first item will be the one that's preferred when creating a new file.

		So for a wav file this might just return ".wav"; for an AIFF file it might
		return two items, ".aif" and ".aiff"
	*/
	const StringArray& getFileExtensions() const;

	/** Returns true if this the given file can be read by this format.

		Subclasses shouldn't do too much work here, just check the extension or
		file type. The base class implementation just checks the file's extension
		against one of the ones that was registered in the constructor.
	*/
	virtual bool canHandleFile (const File& fileToTest);

	/** Returns a set of sample rates that the format can read and write. */
	virtual Array<int> getPossibleSampleRates() = 0;

	/** Returns a set of bit depths that the format can read and write. */
	virtual Array<int> getPossibleBitDepths() = 0;

	/** Returns true if the format can do 2-channel audio. */
	virtual bool canDoStereo() = 0;

	/** Returns true if the format can do 1-channel audio. */
	virtual bool canDoMono() = 0;

	/** Returns true if the format uses compressed data. */
	virtual bool isCompressed();

	/** Returns a list of different qualities that can be used when writing.

		Non-compressed formats will just return an empty array, but for something
		like Ogg-Vorbis or MP3, it might return a list of bit-rates, etc.

		When calling createWriterFor(), an index from this array is passed in to
		tell the format which option is required.
	*/
	virtual StringArray getQualityOptions();

	/** Tries to create an object that can read from a stream containing audio
		data in this format.

		The reader object that is returned can be used to read from the stream, and
		should then be deleted by the caller.

		@param sourceStream                 the stream to read from - the AudioFormatReader object
											that is returned will delete this stream when it no longer
											needs it.
		@param deleteStreamIfOpeningFails   if no reader can be created, this determines whether this method
											should delete the stream object that was passed-in. (If a valid
											reader is returned, it will always be in charge of deleting the
											stream, so this parameter is ignored)
		@see AudioFormatReader
	*/
	virtual AudioFormatReader* createReaderFor (InputStream* sourceStream,
												bool deleteStreamIfOpeningFails) = 0;

	/** Tries to create an object that can write to a stream with this audio format.

		The writer object that is returned can be used to write to the stream, and
		should then be deleted by the caller.

		If the stream can't be created for some reason (e.g. the parameters passed in
		here aren't suitable), this will return 0.

		@param streamToWriteTo      the stream that the data will go to - this will be
									deleted by the AudioFormatWriter object when it's no longer
									needed. If no AudioFormatWriter can be created by this method,
									the stream will NOT be deleted, so that the caller can re-use it
									to try to open a different format, etc
		@param sampleRateToUse      the sample rate for the file, which must be one of the ones
									returned by getPossibleSampleRates()
		@param numberOfChannels     the number of channels - this must be either 1 or 2, and
									the choice will depend on the results of canDoMono() and
									canDoStereo()
		@param bitsPerSample        the bits per sample to use - this must be one of the values
									returned by getPossibleBitDepths()
		@param metadataValues       a set of metadata values that the writer should try to write
									to the stream. Exactly what these are depends on the format,
									and the subclass doesn't actually have to do anything with
									them if it doesn't want to. Have a look at the specific format
									implementation classes to see possible values that can be
									used
		@param qualityOptionIndex   the index of one of compression qualities returned by the
									getQualityOptions() method. If there aren't any quality options
									for this format, just pass 0 in this parameter, as it'll be
									ignored
		@see AudioFormatWriter
	*/
	virtual AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
												double sampleRateToUse,
												unsigned int numberOfChannels,
												int bitsPerSample,
												const StringPairArray& metadataValues,
												int qualityOptionIndex) = 0;

protected:
	/** Creates an AudioFormat object.

		@param formatName       this sets the value that will be returned by getFormatName()
		@param fileExtensions   a zero-terminated list of file extensions - this is what will
								be returned by getFileExtension()
	*/
	AudioFormat (const String& formatName,
				 const StringArray& fileExtensions);

private:

	String formatName;
	StringArray fileExtensions;
};

#endif   // __JUCE_AUDIOFORMAT_JUCEHEADER__

/*** End of inlined file: juce_AudioFormat.h ***/


#endif
#ifndef __JUCE_AUDIOFORMATMANAGER_JUCEHEADER__

/*** Start of inlined file: juce_AudioFormatManager.h ***/
#ifndef __JUCE_AUDIOFORMATMANAGER_JUCEHEADER__
#define __JUCE_AUDIOFORMATMANAGER_JUCEHEADER__

/**
	A class for keeping a list of available audio formats, and for deciding which
	one to use to open a given file.

	After creating an AudioFormatManager object, you should call registerFormat()
	or registerBasicFormats() to give it a list of format types that it can use.

	@see AudioFormat
*/
class JUCE_API  AudioFormatManager
{
public:

	/** Creates an empty format manager.

		Before it'll be any use, you'll need to call registerFormat() with all the
		formats you want it to be able to recognise.
	*/
	AudioFormatManager();

	/** Destructor. */
	~AudioFormatManager();

	/** Adds a format to the manager's list of available file types.

		The object passed-in will be deleted by this object, so don't keep a pointer
		to it!

		If makeThisTheDefaultFormat is true, then the getDefaultFormat() method will
		return this one when called.
	*/
	void registerFormat (AudioFormat* newFormat,
						 bool makeThisTheDefaultFormat);

	/** Handy method to make it easy to register the formats that come with Juce.

		Currently, this will add WAV and AIFF to the list.
	*/
	void registerBasicFormats();

	/** Clears the list of known formats. */
	void clearFormats();

	/** Returns the number of currently registered file formats. */
	int getNumKnownFormats() const;

	/** Returns one of the registered file formats. */
	AudioFormat* getKnownFormat (int index) const;

	/** Looks for which of the known formats is listed as being for a given file
		extension.

		The extension may have a dot before it, so e.g. ".wav" or "wav" are both ok.
	*/
	AudioFormat* findFormatForFileExtension (const String& fileExtension) const;

	/** Returns the format which has been set as the default one.

		You can set a format as being the default when it is registered. It's useful
		when you want to write to a file, because the best format may change between
		platforms, e.g. AIFF is preferred on the Mac, WAV on Windows.

		If none has been set as the default, this method will just return the first
		one in the list.
	*/
	AudioFormat* getDefaultFormat() const;

	/** Returns a set of wildcards for file-matching that contains the extensions for
		all known formats.

		E.g. if might return "*.wav;*.aiff" if it just knows about wavs and aiffs.
	*/
	String getWildcardForAllFormats() const;

	/** Searches through the known formats to try to create a suitable reader for
		this file.

		If none of the registered formats can open the file, it'll return 0. If it
		returns a reader, it's the caller's responsibility to delete the reader.
	*/
	AudioFormatReader* createReaderFor (const File& audioFile);

	/** Searches through the known formats to try to create a suitable reader for
		this stream.

		The stream object that is passed-in will be deleted by this method or by the
		reader that is returned, so the caller should not keep any references to it.

		The stream that is passed-in must be capable of being repositioned so
		that all the formats can have a go at opening it.

		If none of the registered formats can open the stream, it'll return 0. If it
		returns a reader, it's the caller's responsibility to delete the reader.
	*/
	AudioFormatReader* createReaderFor (InputStream* audioFileStream);

private:

	OwnedArray<AudioFormat> knownFormats;
	int defaultFormatIndex;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFormatManager);
};

#endif   // __JUCE_AUDIOFORMATMANAGER_JUCEHEADER__

/*** End of inlined file: juce_AudioFormatManager.h ***/


#endif
#ifndef __JUCE_AUDIOFORMATREADER_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOFORMATREADERSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_AudioFormatReaderSource.h ***/
#ifndef __JUCE_AUDIOFORMATREADERSOURCE_JUCEHEADER__
#define __JUCE_AUDIOFORMATREADERSOURCE_JUCEHEADER__

/**
	A type of AudioSource that will read from an AudioFormatReader.

	@see PositionableAudioSource, AudioTransportSource, BufferingAudioSource
*/
class JUCE_API  AudioFormatReaderSource  : public PositionableAudioSource
{
public:

	/** Creates an AudioFormatReaderSource for a given reader.

		@param sourceReader                     the reader to use as the data source - this must
												not be null
		@param deleteReaderWhenThisIsDeleted    if true, the reader passed-in will be deleted
												when this object is deleted; if false it will be
												left up to the caller to manage its lifetime
	*/
	AudioFormatReaderSource (AudioFormatReader* sourceReader,
							 bool deleteReaderWhenThisIsDeleted);

	/** Destructor. */
	~AudioFormatReaderSource();

	/** Toggles loop-mode.

		If set to true, it will continuously loop the input source. If false,
		it will just emit silence after the source has finished.

		@see isLooping
	*/
	void setLooping (bool shouldLoop);

	/** Returns whether loop-mode is turned on or not. */
	bool isLooping() const                                      { return looping; }

	/** Returns the reader that's being used. */
	AudioFormatReader* getAudioFormatReader() const noexcept    { return reader; }

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

private:

	OptionalScopedPointer<AudioFormatReader> reader;

	int64 volatile nextPlayPos;
	bool volatile looping;

	void readBufferSection (int start, int length, AudioSampleBuffer& buffer, int startSample);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFormatReaderSource);
};

#endif   // __JUCE_AUDIOFORMATREADERSOURCE_JUCEHEADER__

/*** End of inlined file: juce_AudioFormatReaderSource.h ***/


#endif
#ifndef __JUCE_AUDIOFORMATWRITER_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOSUBSECTIONREADER_JUCEHEADER__

/*** Start of inlined file: juce_AudioSubsectionReader.h ***/
#ifndef __JUCE_AUDIOSUBSECTIONREADER_JUCEHEADER__
#define __JUCE_AUDIOSUBSECTIONREADER_JUCEHEADER__

/**
	This class is used to wrap an AudioFormatReader and only read from a
	subsection of the file.

	So if you have a reader which can read a 1000 sample file, you could wrap it
	in one of these to only access, e.g. samples 100 to 200, and any samples
	outside that will come back as 0. Accessing sample 0 from this reader will
	actually read the first sample from the other's subsection, which might
	be at a non-zero position.

	@see AudioFormatReader
*/
class JUCE_API  AudioSubsectionReader  : public AudioFormatReader
{
public:

	/** Creates a AudioSubsectionReader for a given data source.

		@param sourceReader             the source reader from which we'll be taking data
		@param subsectionStartSample    the sample within the source reader which will be
										mapped onto sample 0 for this reader.
		@param subsectionLength         the number of samples from the source that will
										make up the subsection. If this reader is asked for
										any samples beyond this region, it will return zero.
		@param deleteSourceWhenDeleted  if true, the sourceReader object will be deleted when
										this object is deleted.
	*/
	AudioSubsectionReader (AudioFormatReader* sourceReader,
						   int64 subsectionStartSample,
						   int64 subsectionLength,
						   bool deleteSourceWhenDeleted);

	/** Destructor. */
	~AudioSubsectionReader();

	bool readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
					  int64 startSampleInFile, int numSamples);

	void readMaxLevels (int64 startSample,
						int64 numSamples,
						float& lowestLeft,
						float& highestLeft,
						float& lowestRight,
						float& highestRight);

private:

	AudioFormatReader* const source;
	int64 startSample, length;
	const bool deleteSourceWhenDeleted;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSubsectionReader);
};

#endif   // __JUCE_AUDIOSUBSECTIONREADER_JUCEHEADER__

/*** End of inlined file: juce_AudioSubsectionReader.h ***/


#endif

/*** Start of inlined file: juce_AiffAudioFormat.h ***/
/**
	Reads and Writes AIFF format audio files.

	@see AudioFormat
*/
class JUCE_API  AiffAudioFormat  : public AudioFormat
{
public:

	/** Creates an format object. */
	AiffAudioFormat();

	/** Destructor. */
	~AiffAudioFormat();

	Array<int> getPossibleSampleRates();
	Array<int> getPossibleBitDepths();
	bool canDoStereo();
	bool canDoMono();

   #if JUCE_MAC
	bool canHandleFile (const File& fileToTest);
   #endif

	AudioFormatReader* createReaderFor (InputStream* sourceStream,
										bool deleteStreamIfOpeningFails);

	AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
										double sampleRateToUse,
										unsigned int numberOfChannels,
										int bitsPerSample,
										const StringPairArray& metadataValues,
										int qualityOptionIndex);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AiffAudioFormat);
};

/*** End of inlined file: juce_AiffAudioFormat.h ***/



/*** Start of inlined file: juce_CoreAudioFormat.h ***/
#if JUCE_MAC || JUCE_IOS

/**
	OSX and iOS only - This uses the AudioToolbox framework to read any audio
	format that the system has a codec for.

	This should be able to understand formats such as mp3, m4a, etc.

	@see AudioFormat
 */
class JUCE_API  CoreAudioFormat     : public AudioFormat
{
public:

	/** Creates a format object. */
	CoreAudioFormat();

	/** Destructor. */
	~CoreAudioFormat();

	Array<int> getPossibleSampleRates();
	Array<int> getPossibleBitDepths();
	bool canDoStereo();
	bool canDoMono();

	AudioFormatReader* createReaderFor (InputStream* sourceStream,
										bool deleteStreamIfOpeningFails);

	AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
										double sampleRateToUse,
										unsigned int numberOfChannels,
										int bitsPerSample,
										const StringPairArray& metadataValues,
										int qualityOptionIndex);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoreAudioFormat);
};

#endif

/*** End of inlined file: juce_CoreAudioFormat.h ***/


/*** Start of inlined file: juce_FlacAudioFormat.h ***/
#if JUCE_USE_FLAC || defined (DOXYGEN)

/**
	Reads and writes the lossless-compression FLAC audio format.

	To compile this, you'll need to set the JUCE_USE_FLAC flag.

	@see AudioFormat
*/
class JUCE_API  FlacAudioFormat    : public AudioFormat
{
public:

	FlacAudioFormat();
	~FlacAudioFormat();

	Array<int> getPossibleSampleRates();
	Array<int> getPossibleBitDepths();
	bool canDoStereo();
	bool canDoMono();
	bool isCompressed();
	StringArray getQualityOptions();

	AudioFormatReader* createReaderFor (InputStream* sourceStream,
										bool deleteStreamIfOpeningFails);

	AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
										double sampleRateToUse,
										unsigned int numberOfChannels,
										int bitsPerSample,
										const StringPairArray& metadataValues,
										int qualityOptionIndex);
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlacAudioFormat);
};

#endif

/*** End of inlined file: juce_FlacAudioFormat.h ***/


/*** Start of inlined file: juce_MP3AudioFormat.h ***/
#if JUCE_USE_MP3AUDIOFORMAT

/**
	Software-based MP3 decoding format (doesn't currently provide an encoder).

	IMPORTANT DISCLAIMER: By choosing to enable the JUCE_USE_MP3AUDIOFORMAT flag and
	to compile the MP3 code into your software, you do so AT YOUR OWN RISK! By doing so,
	you are agreeing that Raw Material Software is in no way responsible for any patent,
	copyright, or other legal issues that you may suffer as a result.

	The code in juce_MP3AudioFormat.cpp is NOT guaranteed to be free from infringements of 3rd-party
	intellectual property. If you wish to use it, please seek your own independent advice about the
	legality of doing so. If you are not willing to accept full responsibility for the consequences
	of using this code, then do not enable the JUCE_USE_MP3AUDIOFORMAT setting.
*/
class MP3AudioFormat  : public AudioFormat
{
public:

	MP3AudioFormat();
	~MP3AudioFormat();

	Array<int> getPossibleSampleRates();
	Array<int> getPossibleBitDepths();
	bool canDoStereo();
	bool canDoMono();
	bool isCompressed();
	StringArray getQualityOptions();

	AudioFormatReader* createReaderFor (InputStream*, bool deleteStreamIfOpeningFails);

	AudioFormatWriter* createWriterFor (OutputStream*, double sampleRateToUse,
										unsigned int numberOfChannels, int bitsPerSample,
										const StringPairArray& metadataValues, int qualityOptionIndex);
};

#endif

/*** End of inlined file: juce_MP3AudioFormat.h ***/


/*** Start of inlined file: juce_OggVorbisAudioFormat.h ***/
#if JUCE_USE_OGGVORBIS || defined (DOXYGEN)

/**
	Reads and writes the Ogg-Vorbis audio format.

	To compile this, you'll need to set the JUCE_USE_OGGVORBIS flag.

	@see AudioFormat,
*/
class JUCE_API  OggVorbisAudioFormat : public AudioFormat
{
public:

	OggVorbisAudioFormat();
	~OggVorbisAudioFormat();

	Array<int> getPossibleSampleRates();
	Array<int> getPossibleBitDepths();
	bool canDoStereo();
	bool canDoMono();
	bool isCompressed();
	StringArray getQualityOptions();

	/** Tries to estimate the quality level of an ogg file based on its size.

		If it can't read the file for some reason, this will just return 1 (medium quality),
		otherwise it will return the approximate quality setting that would have been used
		to create the file.

		@see getQualityOptions
	*/
	int estimateOggFileQuality (const File& source);

	/** Metadata property name used by the Ogg writer - if you set a string for this
		value, it will be written into the ogg file as the name of the encoder app.

		@see createWriterFor
	*/
	static const char* const encoderName;

	AudioFormatReader* createReaderFor (InputStream* sourceStream,
										bool deleteStreamIfOpeningFails);

	AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
										double sampleRateToUse,
										unsigned int numberOfChannels,
										int bitsPerSample,
										const StringPairArray& metadataValues,
										int qualityOptionIndex);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OggVorbisAudioFormat);
};

#endif

/*** End of inlined file: juce_OggVorbisAudioFormat.h ***/


/*** Start of inlined file: juce_QuickTimeAudioFormat.h ***/
#if JUCE_QUICKTIME

/**
	Uses QuickTime to read the audio track a movie or media file.

	As well as QuickTime movies, this should also manage to open other audio
	files that quicktime can understand, like mp3, m4a, etc.

	@see AudioFormat
*/
class JUCE_API  QuickTimeAudioFormat  : public AudioFormat
{
public:

	/** Creates a format object. */
	QuickTimeAudioFormat();

	/** Destructor. */
	~QuickTimeAudioFormat();

	Array<int> getPossibleSampleRates();
	Array<int> getPossibleBitDepths();
	bool canDoStereo();
	bool canDoMono();

	AudioFormatReader* createReaderFor (InputStream* sourceStream,
										bool deleteStreamIfOpeningFails);

	AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
										double sampleRateToUse,
										unsigned int numberOfChannels,
										int bitsPerSample,
										const StringPairArray& metadataValues,
										int qualityOptionIndex);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QuickTimeAudioFormat);
};

#endif

/*** End of inlined file: juce_QuickTimeAudioFormat.h ***/


/*** Start of inlined file: juce_WavAudioFormat.h ***/
/**
	Reads and Writes WAV format audio files.

	@see AudioFormat
*/
class JUCE_API  WavAudioFormat  : public AudioFormat
{
public:

	/** Creates a format object. */
	WavAudioFormat();

	/** Destructor. */
	~WavAudioFormat();

	/** Metadata property name used by wav readers and writers for adding
		a BWAV chunk to the file.

		@see AudioFormatReader::metadataValues, createWriterFor
	*/
	static const char* const bwavDescription;

	/** Metadata property name used by wav readers and writers for adding
		a BWAV chunk to the file.

		@see AudioFormatReader::metadataValues, createWriterFor
	*/
	static const char* const bwavOriginator;

	/** Metadata property name used by wav readers and writers for adding
		a BWAV chunk to the file.

		@see AudioFormatReader::metadataValues, createWriterFor
	*/
	static const char* const bwavOriginatorRef;

	/** Metadata property name used by wav readers and writers for adding
		a BWAV chunk to the file.

		Date format is: yyyy-mm-dd

		@see AudioFormatReader::metadataValues, createWriterFor
	*/
	static const char* const bwavOriginationDate;

	/** Metadata property name used by wav readers and writers for adding
		a BWAV chunk to the file.

		Time format is: hh-mm-ss

		@see AudioFormatReader::metadataValues, createWriterFor
	*/
	static const char* const bwavOriginationTime;

	/** Metadata property name used by wav readers and writers for adding
		a BWAV chunk to the file.

		This is the number of samples from the start of an edit that the
		file is supposed to begin at. Seems like an obvious mistake to
		only allow a file to occur in an edit once, but that's the way
		it is..

		@see AudioFormatReader::metadataValues, createWriterFor
	*/
	static const char* const bwavTimeReference;

	/** Metadata property name used by wav readers and writers for adding
		a BWAV chunk to the file.

		This is a

		@see AudioFormatReader::metadataValues, createWriterFor
	*/
	static const char* const bwavCodingHistory;

	/** Utility function to fill out the appropriate metadata for a BWAV file.

		This just makes it easier than using the property names directly, and it
		fills out the time and date in the right format.
	*/
	static StringPairArray createBWAVMetadata (const String& description,
											   const String& originator,
											   const String& originatorRef,
											   const Time& dateAndTime,
											   const int64 timeReferenceSamples,
											   const String& codingHistory);

	Array<int> getPossibleSampleRates();
	Array<int> getPossibleBitDepths();
	bool canDoStereo();
	bool canDoMono();

	AudioFormatReader* createReaderFor (InputStream* sourceStream,
										bool deleteStreamIfOpeningFails);

	AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
										double sampleRateToUse,
										unsigned int numberOfChannels,
										int bitsPerSample,
										const StringPairArray& metadataValues,
										int qualityOptionIndex);

	/** Utility function to replace the metadata in a wav file with a new set of values.

		If possible, this cheats by overwriting just the metadata region of the file, rather
		than by copying the whole file again.
	*/
	bool replaceMetadataInFile (const File& wavFile, const StringPairArray& newMetadata);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavAudioFormat);
};

/*** End of inlined file: juce_WavAudioFormat.h ***/


/*** Start of inlined file: juce_WindowsMediaAudioFormat.h ***/
#if JUCE_WINDOWS

/**
	Audio format which uses the Windows Media codecs (Windows only).
*/
class WindowsMediaAudioFormat  : public AudioFormat
{
public:

	WindowsMediaAudioFormat();
	~WindowsMediaAudioFormat();

	Array<int> getPossibleSampleRates();
	Array<int> getPossibleBitDepths();
	bool canDoStereo();
	bool canDoMono();

	AudioFormatReader* createReaderFor (InputStream*, bool deleteStreamIfOpeningFails);

	AudioFormatWriter* createWriterFor (OutputStream*, double sampleRateToUse,
										unsigned int numberOfChannels, int bitsPerSample,
										const StringPairArray& metadataValues, int qualityOptionIndex);
};

#endif

/*** End of inlined file: juce_WindowsMediaAudioFormat.h ***/

#ifndef __JUCE_SAMPLER_JUCEHEADER__

/*** Start of inlined file: juce_Sampler.h ***/
#ifndef __JUCE_SAMPLER_JUCEHEADER__
#define __JUCE_SAMPLER_JUCEHEADER__

/**
	A subclass of SynthesiserSound that represents a sampled audio clip.

	This is a pretty basic sampler, and just attempts to load the whole audio stream
	into memory.

	To use it, create a Synthesiser, add some SamplerVoice objects to it, then
	give it some SampledSound objects to play.

	@see SamplerVoice, Synthesiser, SynthesiserSound
*/
class JUCE_API  SamplerSound    : public SynthesiserSound
{
public:

	/** Creates a sampled sound from an audio reader.

		This will attempt to load the audio from the source into memory and store
		it in this object.

		@param name         a name for the sample
		@param source       the audio to load. This object can be safely deleted by the
							caller after this constructor returns
		@param midiNotes    the set of midi keys that this sound should be played on. This
							is used by the SynthesiserSound::appliesToNote() method
		@param midiNoteForNormalPitch   the midi note at which the sample should be played
										with its natural rate. All other notes will be pitched
										up or down relative to this one
		@param attackTimeSecs   the attack (fade-in) time, in seconds
		@param releaseTimeSecs  the decay (fade-out) time, in seconds
		@param maxSampleLengthSeconds   a maximum length of audio to read from the audio
										source, in seconds
	*/
	SamplerSound (const String& name,
				  AudioFormatReader& source,
				  const BigInteger& midiNotes,
				  int midiNoteForNormalPitch,
				  double attackTimeSecs,
				  double releaseTimeSecs,
				  double maxSampleLengthSeconds);

	/** Destructor. */
	~SamplerSound();

	/** Returns the sample's name */
	const String& getName() const                           { return name; }

	/** Returns the audio sample data.
		This could be 0 if there was a problem loading it.
	*/
	AudioSampleBuffer* getAudioData() const                 { return data; }

	bool appliesToNote (const int midiNoteNumber);
	bool appliesToChannel (const int midiChannel);

private:

	friend class SamplerVoice;

	String name;
	ScopedPointer <AudioSampleBuffer> data;
	double sourceSampleRate;
	BigInteger midiNotes;
	int length, attackSamples, releaseSamples;
	int midiRootNote;

	JUCE_LEAK_DETECTOR (SamplerSound);
};

/**
	A subclass of SynthesiserVoice that can play a SamplerSound.

	To use it, create a Synthesiser, add some SamplerVoice objects to it, then
	give it some SampledSound objects to play.

	@see SamplerSound, Synthesiser, SynthesiserVoice
*/
class JUCE_API  SamplerVoice    : public SynthesiserVoice
{
public:

	/** Creates a SamplerVoice.
	*/
	SamplerVoice();

	/** Destructor. */
	~SamplerVoice();

	bool canPlaySound (SynthesiserSound* sound);

	void startNote (const int midiNoteNumber,
					const float velocity,
					SynthesiserSound* sound,
					const int currentPitchWheelPosition);

	void stopNote (const bool allowTailOff);

	void pitchWheelMoved (const int newValue);
	void controllerMoved (const int controllerNumber,
						  const int newValue);

	void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples);

private:

	double pitchRatio;
	double sourceSamplePosition;
	float lgain, rgain, attackReleaseLevel, attackDelta, releaseDelta;
	bool isInAttack, isInRelease;

	JUCE_LEAK_DETECTOR (SamplerVoice);
};

#endif   // __JUCE_SAMPLER_JUCEHEADER__

/*** End of inlined file: juce_Sampler.h ***/


#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_AUDIO_FORMATS_JUCEHEADER__

/*** End of inlined file: juce_audio_formats.h ***/


