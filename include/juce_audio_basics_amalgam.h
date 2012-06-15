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


/*** Start of inlined file: juce_audio_basics.h ***/
#ifndef __JUCE_AUDIO_BASICS_JUCEHEADER__
#define __JUCE_AUDIO_BASICS_JUCEHEADER__

#include "juce_core_amalgam.h"

namespace juce
{

// START_AUTOINCLUDE buffers, effects, midi, sources, synthesisers
#ifndef __JUCE_AUDIODATACONVERTERS_JUCEHEADER__

/*** Start of inlined file: juce_AudioDataConverters.h ***/
#ifndef __JUCE_AUDIODATACONVERTERS_JUCEHEADER__
#define __JUCE_AUDIODATACONVERTERS_JUCEHEADER__

/**
	This class a container which holds all the classes pertaining to the AudioData::Pointer
	audio sample format class.

	@see AudioData::Pointer.
*/
class JUCE_API  AudioData
{
public:

	// These types can be used as the SampleFormat template parameter for the AudioData::Pointer class.

	class Int8;       /**< Used as a template parameter for AudioData::Pointer. Indicates an 8-bit integer packed data format. */
	class UInt8;      /**< Used as a template parameter for AudioData::Pointer. Indicates an 8-bit unsigned integer packed data format. */
	class Int16;      /**< Used as a template parameter for AudioData::Pointer. Indicates an 16-bit integer packed data format. */
	class Int24;      /**< Used as a template parameter for AudioData::Pointer. Indicates an 24-bit integer packed data format. */
	class Int32;      /**< Used as a template parameter for AudioData::Pointer. Indicates an 32-bit integer packed data format. */
	class Float32;    /**< Used as a template parameter for AudioData::Pointer. Indicates an 32-bit float data format. */

	// These types can be used as the Endianness template parameter for the AudioData::Pointer class.

	class BigEndian;      /**< Used as a template parameter for AudioData::Pointer. Indicates that the samples are stored in big-endian order. */
	class LittleEndian;   /**< Used as a template parameter for AudioData::Pointer. Indicates that the samples are stored in little-endian order. */
	class NativeEndian;   /**< Used as a template parameter for AudioData::Pointer. Indicates that the samples are stored in the CPU's native endianness. */

	// These types can be used as the InterleavingType template parameter for the AudioData::Pointer class.

	class NonInterleaved; /**< Used as a template parameter for AudioData::Pointer. Indicates that the samples are stored contiguously. */
	class Interleaved;    /**< Used as a template parameter for AudioData::Pointer. Indicates that the samples are interleaved with a number of other channels. */

	// These types can be used as the Constness template parameter for the AudioData::Pointer class.

	class NonConst; /**< Used as a template parameter for AudioData::Pointer. Indicates that the pointer can be used for non-const data. */
	class Const;    /**< Used as a template parameter for AudioData::Pointer. Indicates that the samples can only be used for const data.. */

  #ifndef DOXYGEN

	class BigEndian
	{
	public:
		template <class SampleFormatType> static inline float getAsFloat (SampleFormatType& s) noexcept                         { return s.getAsFloatBE(); }
		template <class SampleFormatType> static inline void setAsFloat (SampleFormatType& s, float newValue) noexcept          { s.setAsFloatBE (newValue); }
		template <class SampleFormatType> static inline int32 getAsInt32 (SampleFormatType& s) noexcept                         { return s.getAsInt32BE(); }
		template <class SampleFormatType> static inline void setAsInt32 (SampleFormatType& s, int32 newValue) noexcept          { s.setAsInt32BE (newValue); }
		template <class SourceType, class DestType> static inline void copyFrom (DestType& dest, SourceType& source) noexcept   { dest.copyFromBE (source); }
		enum { isBigEndian = 1 };
	};

	class LittleEndian
	{
	public:
		template <class SampleFormatType> static inline float getAsFloat (SampleFormatType& s) noexcept                         { return s.getAsFloatLE(); }
		template <class SampleFormatType> static inline void setAsFloat (SampleFormatType& s, float newValue) noexcept          { s.setAsFloatLE (newValue); }
		template <class SampleFormatType> static inline int32 getAsInt32 (SampleFormatType& s) noexcept                         { return s.getAsInt32LE(); }
		template <class SampleFormatType> static inline void setAsInt32 (SampleFormatType& s, int32 newValue) noexcept          { s.setAsInt32LE (newValue); }
		template <class SourceType, class DestType> static inline void copyFrom (DestType& dest, SourceType& source) noexcept   { dest.copyFromLE (source); }
		enum { isBigEndian = 0 };
	};

	#if JUCE_BIG_ENDIAN
	 class NativeEndian   : public BigEndian  {};
	#else
	 class NativeEndian   : public LittleEndian  {};
	#endif

	class Int8
	{
	public:
		inline Int8 (void* data_) noexcept  : data (static_cast <int8*> (data_))  {}

		inline void advance() noexcept                          { ++data; }
		inline void skip (int numSamples) noexcept              { data += numSamples; }
		inline float getAsFloatLE() const noexcept              { return (float) (*data * (1.0 / (1.0 + maxValue))); }
		inline float getAsFloatBE() const noexcept              { return getAsFloatLE(); }
		inline void setAsFloatLE (float newValue) noexcept      { *data = (int8) jlimit ((int) -maxValue, (int) maxValue, roundToInt (newValue * (1.0 + maxValue))); }
		inline void setAsFloatBE (float newValue) noexcept      { setAsFloatLE (newValue); }
		inline int32 getAsInt32LE() const noexcept              { return (int) (*data << 24); }
		inline int32 getAsInt32BE() const noexcept              { return getAsInt32LE(); }
		inline void setAsInt32LE (int newValue) noexcept        { *data = (int8) (newValue >> 24); }
		inline void setAsInt32BE (int newValue) noexcept        { setAsInt32LE (newValue); }
		inline void clear() noexcept                            { *data = 0; }
		inline void clearMultiple (int num) noexcept            { zeromem (data, (size_t) (num * bytesPerSample)) ;}
		template <class SourceType> inline void copyFromLE (SourceType& source) noexcept    { setAsInt32LE (source.getAsInt32()); }
		template <class SourceType> inline void copyFromBE (SourceType& source) noexcept    { setAsInt32BE (source.getAsInt32()); }
		inline void copyFromSameType (Int8& source) noexcept    { *data = *source.data; }

		int8* data;
		enum { bytesPerSample = 1, maxValue = 0x7f, resolution = (1 << 24), isFloat = 0 };
	};

	class UInt8
	{
	public:
		inline UInt8 (void* data_) noexcept  : data (static_cast <uint8*> (data_))  {}

		inline void advance() noexcept                          { ++data; }
		inline void skip (int numSamples) noexcept              { data += numSamples; }
		inline float getAsFloatLE() const noexcept              { return (float) ((*data - 128) * (1.0 / (1.0 + maxValue))); }
		inline float getAsFloatBE() const noexcept              { return getAsFloatLE(); }
		inline void setAsFloatLE (float newValue) noexcept      { *data = (uint8) jlimit (0, 255, 128 + roundToInt (newValue * (1.0 + maxValue))); }
		inline void setAsFloatBE (float newValue) noexcept      { setAsFloatLE (newValue); }
		inline int32 getAsInt32LE() const noexcept              { return (int) ((*data - 128) << 24); }
		inline int32 getAsInt32BE() const noexcept              { return getAsInt32LE(); }
		inline void setAsInt32LE (int newValue) noexcept        { *data = (uint8) (128 + (newValue >> 24)); }
		inline void setAsInt32BE (int newValue) noexcept        { setAsInt32LE (newValue); }
		inline void clear() noexcept                            { *data = 128; }
		inline void clearMultiple (int num) noexcept            { memset (data, 128, (size_t) num) ;}
		template <class SourceType> inline void copyFromLE (SourceType& source) noexcept    { setAsInt32LE (source.getAsInt32()); }
		template <class SourceType> inline void copyFromBE (SourceType& source) noexcept    { setAsInt32BE (source.getAsInt32()); }
		inline void copyFromSameType (UInt8& source) noexcept   { *data = *source.data; }

		uint8* data;
		enum { bytesPerSample = 1, maxValue = 0x7f, resolution = (1 << 24), isFloat = 0 };
	};

	class Int16
	{
	public:
		inline Int16 (void* data_) noexcept  : data (static_cast <uint16*> (data_))  {}

		inline void advance() noexcept                          { ++data; }
		inline void skip (int numSamples) noexcept              { data += numSamples; }
		inline float getAsFloatLE() const noexcept              { return (float) ((1.0 / (1.0 + maxValue)) * (int16) ByteOrder::swapIfBigEndian (*data)); }
		inline float getAsFloatBE() const noexcept              { return (float) ((1.0 / (1.0 + maxValue)) * (int16) ByteOrder::swapIfLittleEndian (*data)); }
		inline void setAsFloatLE (float newValue) noexcept      { *data = ByteOrder::swapIfBigEndian ((uint16) jlimit ((int) -maxValue, (int) maxValue, roundToInt (newValue * (1.0 + maxValue)))); }
		inline void setAsFloatBE (float newValue) noexcept      { *data = ByteOrder::swapIfLittleEndian ((uint16) jlimit ((int) -maxValue, (int) maxValue, roundToInt (newValue * (1.0 + maxValue)))); }
		inline int32 getAsInt32LE() const noexcept              { return (int32) (ByteOrder::swapIfBigEndian ((uint16) *data) << 16); }
		inline int32 getAsInt32BE() const noexcept              { return (int32) (ByteOrder::swapIfLittleEndian ((uint16) *data) << 16); }
		inline void setAsInt32LE (int32 newValue) noexcept      { *data = ByteOrder::swapIfBigEndian ((uint16) (newValue >> 16)); }
		inline void setAsInt32BE (int32 newValue) noexcept      { *data = ByteOrder::swapIfLittleEndian ((uint16) (newValue >> 16)); }
		inline void clear() noexcept                            { *data = 0; }
		inline void clearMultiple (int num) noexcept            { zeromem (data, (size_t) (num * bytesPerSample)) ;}
		template <class SourceType> inline void copyFromLE (SourceType& source) noexcept    { setAsInt32LE (source.getAsInt32()); }
		template <class SourceType> inline void copyFromBE (SourceType& source) noexcept    { setAsInt32BE (source.getAsInt32()); }
		inline void copyFromSameType (Int16& source) noexcept   { *data = *source.data; }

		uint16* data;
		enum { bytesPerSample = 2, maxValue = 0x7fff, resolution = (1 << 16), isFloat = 0 };
	};

	class Int24
	{
	public:
		inline Int24 (void* data_) noexcept  : data (static_cast <char*> (data_))  {}

		inline void advance() noexcept                          { data += 3; }
		inline void skip (int numSamples) noexcept              { data += 3 * numSamples; }
		inline float getAsFloatLE() const noexcept              { return (float) (ByteOrder::littleEndian24Bit (data) * (1.0 / (1.0 + maxValue))); }
		inline float getAsFloatBE() const noexcept              { return (float) (ByteOrder::bigEndian24Bit (data) * (1.0 / (1.0 + maxValue))); }
		inline void setAsFloatLE (float newValue) noexcept      { ByteOrder::littleEndian24BitToChars (jlimit ((int) -maxValue, (int) maxValue, roundToInt (newValue * (1.0 + maxValue))), data); }
		inline void setAsFloatBE (float newValue) noexcept      { ByteOrder::bigEndian24BitToChars (jlimit ((int) -maxValue, (int) maxValue, roundToInt (newValue * (1.0 + maxValue))), data); }
		inline int32 getAsInt32LE() const noexcept              { return (int32) ByteOrder::littleEndian24Bit (data) << 8; }
		inline int32 getAsInt32BE() const noexcept              { return (int32) ByteOrder::bigEndian24Bit (data) << 8; }
		inline void setAsInt32LE (int32 newValue) noexcept      { ByteOrder::littleEndian24BitToChars (newValue >> 8, data); }
		inline void setAsInt32BE (int32 newValue) noexcept      { ByteOrder::bigEndian24BitToChars (newValue >> 8, data); }
		inline void clear() noexcept                            { data[0] = 0; data[1] = 0; data[2] = 0; }
		inline void clearMultiple (int num) noexcept            { zeromem (data, (size_t) (num * bytesPerSample)) ;}
		template <class SourceType> inline void copyFromLE (SourceType& source) noexcept    { setAsInt32LE (source.getAsInt32()); }
		template <class SourceType> inline void copyFromBE (SourceType& source) noexcept    { setAsInt32BE (source.getAsInt32()); }
		inline void copyFromSameType (Int24& source) noexcept   { data[0] = source.data[0]; data[1] = source.data[1]; data[2] = source.data[2]; }

		char* data;
		enum { bytesPerSample = 3, maxValue = 0x7fffff, resolution = (1 << 8), isFloat = 0 };
	};

	class Int32
	{
	public:
		inline Int32 (void* data_) noexcept  : data (static_cast <uint32*> (data_))  {}

		inline void advance() noexcept                          { ++data; }
		inline void skip (int numSamples) noexcept              { data += numSamples; }
		inline float getAsFloatLE() const noexcept              { return (float) ((1.0 / (1.0 + maxValue)) * (int32) ByteOrder::swapIfBigEndian (*data)); }
		inline float getAsFloatBE() const noexcept              { return (float) ((1.0 / (1.0 + maxValue)) * (int32) ByteOrder::swapIfLittleEndian (*data)); }
		inline void setAsFloatLE (float newValue) noexcept      { *data = ByteOrder::swapIfBigEndian ((uint32) (maxValue * jlimit (-1.0, 1.0, (double) newValue))); }
		inline void setAsFloatBE (float newValue) noexcept      { *data = ByteOrder::swapIfLittleEndian ((uint32) (maxValue * jlimit (-1.0, 1.0, (double) newValue))); }
		inline int32 getAsInt32LE() const noexcept              { return (int32) ByteOrder::swapIfBigEndian (*data); }
		inline int32 getAsInt32BE() const noexcept              { return (int32) ByteOrder::swapIfLittleEndian (*data); }
		inline void setAsInt32LE (int32 newValue) noexcept      { *data = ByteOrder::swapIfBigEndian ((uint32) newValue); }
		inline void setAsInt32BE (int32 newValue) noexcept      { *data = ByteOrder::swapIfLittleEndian ((uint32) newValue); }
		inline void clear() noexcept                            { *data = 0; }
		inline void clearMultiple (int num) noexcept            { zeromem (data, (size_t) (num * bytesPerSample)) ;}
		template <class SourceType> inline void copyFromLE (SourceType& source) noexcept    { setAsInt32LE (source.getAsInt32()); }
		template <class SourceType> inline void copyFromBE (SourceType& source) noexcept    { setAsInt32BE (source.getAsInt32()); }
		inline void copyFromSameType (Int32& source) noexcept   { *data = *source.data; }

		uint32* data;
		enum { bytesPerSample = 4, maxValue = 0x7fffffff, resolution = 1, isFloat = 0 };
	};

	class Float32
	{
	public:
		inline Float32 (void* data_) noexcept : data (static_cast <float*> (data_))  {}

		inline void advance() noexcept                          { ++data; }
		inline void skip (int numSamples) noexcept              { data += numSamples; }
	   #if JUCE_BIG_ENDIAN
		inline float getAsFloatBE() const noexcept              { return *data; }
		inline void setAsFloatBE (float newValue) noexcept      { *data = newValue; }
		inline float getAsFloatLE() const noexcept              { union { uint32 asInt; float asFloat; } n; n.asInt = ByteOrder::swap (*(uint32*) data); return n.asFloat; }
		inline void setAsFloatLE (float newValue) noexcept      { union { uint32 asInt; float asFloat; } n; n.asFloat = newValue; *(uint32*) data = ByteOrder::swap (n.asInt); }
	   #else
		inline float getAsFloatLE() const noexcept              { return *data; }
		inline void setAsFloatLE (float newValue) noexcept      { *data = newValue; }
		inline float getAsFloatBE() const noexcept              { union { uint32 asInt; float asFloat; } n; n.asInt = ByteOrder::swap (*(uint32*) data); return n.asFloat; }
		inline void setAsFloatBE (float newValue) noexcept      { union { uint32 asInt; float asFloat; } n; n.asFloat = newValue; *(uint32*) data = ByteOrder::swap (n.asInt); }
	   #endif
		inline int32 getAsInt32LE() const noexcept              { return (int32) roundToInt (jlimit (-1.0, 1.0, (double) getAsFloatLE()) * (double) maxValue); }
		inline int32 getAsInt32BE() const noexcept              { return (int32) roundToInt (jlimit (-1.0, 1.0, (double) getAsFloatBE()) * (double) maxValue); }
		inline void setAsInt32LE (int32 newValue) noexcept      { setAsFloatLE ((float) (newValue * (1.0 / (1.0 + maxValue)))); }
		inline void setAsInt32BE (int32 newValue) noexcept      { setAsFloatBE ((float) (newValue * (1.0 / (1.0 + maxValue)))); }
		inline void clear() noexcept                            { *data = 0; }
		inline void clearMultiple (int num) noexcept            { zeromem (data, (size_t) (num * bytesPerSample)) ;}
		template <class SourceType> inline void copyFromLE (SourceType& source) noexcept    { setAsFloatLE (source.getAsFloat()); }
		template <class SourceType> inline void copyFromBE (SourceType& source) noexcept    { setAsFloatBE (source.getAsFloat()); }
		inline void copyFromSameType (Float32& source) noexcept { *data = *source.data; }

		float* data;
		enum { bytesPerSample = 4, maxValue = 0x7fffffff, resolution = (1 << 8), isFloat = 1 };
	};

	class NonInterleaved
	{
	public:
		inline NonInterleaved() noexcept {}
		inline NonInterleaved (const NonInterleaved&) noexcept {}
		inline NonInterleaved (const int) noexcept {}
		inline void copyFrom (const NonInterleaved&) noexcept {}
		template <class SampleFormatType> inline void advanceData (SampleFormatType& s) noexcept                    { s.advance(); }
		template <class SampleFormatType> inline void advanceDataBy (SampleFormatType& s, int numSamples) noexcept  { s.skip (numSamples); }
		template <class SampleFormatType> inline void clear (SampleFormatType& s, int numSamples) noexcept          { s.clearMultiple (numSamples); }
		template <class SampleFormatType> inline static int getNumBytesBetweenSamples (const SampleFormatType&) noexcept { return SampleFormatType::bytesPerSample; }

		enum { isInterleavedType = 0, numInterleavedChannels = 1 };
	};

	class Interleaved
	{
	public:
		inline Interleaved() noexcept : numInterleavedChannels (1) {}
		inline Interleaved (const Interleaved& other) noexcept : numInterleavedChannels (other.numInterleavedChannels) {}
		inline Interleaved (const int numInterleavedChannels_) noexcept : numInterleavedChannels (numInterleavedChannels_) {}
		inline void copyFrom (const Interleaved& other) noexcept { numInterleavedChannels = other.numInterleavedChannels; }
		template <class SampleFormatType> inline void advanceData (SampleFormatType& s) noexcept                    { s.skip (numInterleavedChannels); }
		template <class SampleFormatType> inline void advanceDataBy (SampleFormatType& s, int numSamples) noexcept  { s.skip (numInterleavedChannels * numSamples); }
		template <class SampleFormatType> inline void clear (SampleFormatType& s, int numSamples) noexcept          { while (--numSamples >= 0) { s.clear(); s.skip (numInterleavedChannels); } }
		template <class SampleFormatType> inline int getNumBytesBetweenSamples (const SampleFormatType&) const noexcept { return numInterleavedChannels * SampleFormatType::bytesPerSample; }
		int numInterleavedChannels;
		enum { isInterleavedType = 1 };
	};

	class NonConst
	{
	public:
		typedef void VoidType;
		static inline void* toVoidPtr (VoidType* v) noexcept { return v; }
		enum { isConst = 0 };
	};

	class Const
	{
	public:
		typedef const void VoidType;
		static inline void* toVoidPtr (VoidType* v) noexcept { return const_cast <void*> (v); }
		enum { isConst = 1 };
	};
  #endif

	/**
		A pointer to a block of audio data with a particular encoding.

		This object can be used to read and write from blocks of encoded audio samples. To create one, you specify
		the audio format as a series of template parameters, e.g.
		@code
		// this creates a pointer for reading from a const array of 16-bit little-endian packed samples.
		AudioData::Pointer <AudioData::Int16,
							AudioData::LittleEndian,
							AudioData::NonInterleaved,
							AudioData::Const> pointer (someRawAudioData);

		// These methods read the sample that is being pointed to
		float firstSampleAsFloat = pointer.getAsFloat();
		int32 firstSampleAsInt = pointer.getAsInt32();
		++pointer; // moves the pointer to the next sample.
		pointer += 3; // skips the next 3 samples.
		@endcode

		The convertSamples() method lets you copy a range of samples from one format to another, automatically
		converting its format.

		@see AudioData::Converter
	*/
	template <typename SampleFormat,
			  typename Endianness,
			  typename InterleavingType,
			  typename Constness>
	class Pointer  : private InterleavingType  // (inherited for EBCO)
	{
	public:

		/** Creates a non-interleaved pointer from some raw data in the appropriate format.
			This constructor is only used if you've specified the AudioData::NonInterleaved option -
			for interleaved formats, use the constructor that also takes a number of channels.
		*/
		Pointer (typename Constness::VoidType* sourceData) noexcept
			: data (Constness::toVoidPtr (sourceData))
		{
			// If you're using interleaved data, call the other constructor! If you're using non-interleaved data,
			// you should pass NonInterleaved as the template parameter for the interleaving type!
			static_jassert (InterleavingType::isInterleavedType == 0);
		}

		/** Creates a pointer from some raw data in the appropriate format with the specified number of interleaved channels.
			For non-interleaved data, use the other constructor.
		*/
		Pointer (typename Constness::VoidType* sourceData, int numInterleavedChannels) noexcept
			: InterleavingType (numInterleavedChannels), data (Constness::toVoidPtr (sourceData))
		{
		}

		/** Creates a copy of another pointer. */
		Pointer (const Pointer& other) noexcept
			: InterleavingType (other), data (other.data)
		{
		}

		Pointer& operator= (const Pointer& other) noexcept
		{
			InterleavingType::operator= (other);
			data = other.data;
			return *this;
		}

		/** Returns the value of the first sample as a floating point value.
			The value will be in the range -1.0 to 1.0 for integer formats. For floating point
			formats, the value could be outside that range, although -1 to 1 is the standard range.
		*/
		inline float getAsFloat() const noexcept                { return Endianness::getAsFloat (data); }

		/** Sets the value of the first sample as a floating point value.

			(This method can only be used if the AudioData::NonConst option was used).
			The value should be in the range -1.0 to 1.0 - for integer formats, values outside that
			range will be clipped. For floating point formats, any value passed in here will be
			written directly, although -1 to 1 is the standard range.
		*/
		inline void setAsFloat (float newValue) noexcept
		{
			static_jassert (Constness::isConst == 0); // trying to write to a const pointer! For a writeable one, use AudioData::NonConst instead!
			Endianness::setAsFloat (data, newValue);
		}

		/** Returns the value of the first sample as a 32-bit integer.
			The value returned will be in the range 0x80000000 to 0x7fffffff, and shorter values will be
			shifted to fill this range (e.g. if you're reading from 24-bit data, the values will be shifted up
			by 8 bits when returned here). If the source data is floating point, values beyond -1.0 to 1.0 will
			be clipped so that -1.0 maps onto -0x7fffffff and 1.0 maps to 0x7fffffff.
		*/
		inline int32 getAsInt32() const noexcept                { return Endianness::getAsInt32 (data); }

		/** Sets the value of the first sample as a 32-bit integer.
			This will be mapped to the range of the format that is being written - see getAsInt32().
		*/
		inline void setAsInt32 (int32 newValue) noexcept
		{
			static_jassert (Constness::isConst == 0); // trying to write to a const pointer! For a writeable one, use AudioData::NonConst instead!
			Endianness::setAsInt32 (data, newValue);
		}

		/** Moves the pointer along to the next sample. */
		inline Pointer& operator++() noexcept                   { advance(); return *this; }

		/** Moves the pointer back to the previous sample. */
		inline Pointer& operator--() noexcept                   { this->advanceDataBy (data, -1); return *this; }

		/** Adds a number of samples to the pointer's position. */
		Pointer& operator+= (int samplesToJump) noexcept        { this->advanceDataBy (data, samplesToJump); return *this; }

		/** Writes a stream of samples into this pointer from another pointer.
			This will copy the specified number of samples, converting between formats appropriately.
		*/
		void convertSamples (Pointer source, int numSamples) const noexcept
		{
			static_jassert (Constness::isConst == 0); // trying to write to a const pointer! For a writeable one, use AudioData::NonConst instead!

			Pointer dest (*this);
			while (--numSamples >= 0)
			{
				dest.data.copyFromSameType (source.data);
				dest.advance();
				source.advance();
			}
		}

		/** Writes a stream of samples into this pointer from another pointer.
			This will copy the specified number of samples, converting between formats appropriately.
		*/
		template <class OtherPointerType>
		void convertSamples (OtherPointerType source, int numSamples) const noexcept
		{
			static_jassert (Constness::isConst == 0); // trying to write to a const pointer! For a writeable one, use AudioData::NonConst instead!

			Pointer dest (*this);

			if (source.getRawData() != getRawData() || source.getNumBytesBetweenSamples() >= getNumBytesBetweenSamples())
			{
				while (--numSamples >= 0)
				{
					Endianness::copyFrom (dest.data, source);
					dest.advance();
					++source;
				}
			}
			else // copy backwards if we're increasing the sample width..
			{
				dest += numSamples;
				source += numSamples;

				while (--numSamples >= 0)
					Endianness::copyFrom ((--dest).data, --source);
			}
		}

		/** Sets a number of samples to zero. */
		void clearSamples (int numSamples) const noexcept
		{
			Pointer dest (*this);
			dest.clear (dest.data, numSamples);
		}

		/** Returns true if the pointer is using a floating-point format. */
		static bool isFloatingPoint() noexcept                  { return (bool) SampleFormat::isFloat; }

		/** Returns true if the format is big-endian. */
		static bool isBigEndian() noexcept                      { return (bool) Endianness::isBigEndian; }

		/** Returns the number of bytes in each sample (ignoring the number of interleaved channels). */
		static int getBytesPerSample() noexcept                 { return (int) SampleFormat::bytesPerSample; }

		/** Returns the number of interleaved channels in the format. */
		int getNumInterleavedChannels() const noexcept          { return (int) this->numInterleavedChannels; }

		/** Returns the number of bytes between the start address of each sample. */
		int getNumBytesBetweenSamples() const noexcept          { return InterleavingType::getNumBytesBetweenSamples (data); }

		/** Returns the accuracy of this format when represented as a 32-bit integer.
			This is the smallest number above 0 that can be represented in the sample format, converted to
			a 32-bit range. E,g. if the format is 8-bit, its resolution is 0x01000000; if the format is 24-bit,
			its resolution is 0x100.
		*/
		static int get32BitResolution() noexcept                { return (int) SampleFormat::resolution; }

		/** Returns a pointer to the underlying data. */
		const void* getRawData() const noexcept                 { return data.data; }

	private:

		SampleFormat data;

		inline void advance() noexcept                          { this->advanceData (data); }

		Pointer operator++ (int); // private to force you to use the more efficient pre-increment!
		Pointer operator-- (int);
	};

	/** A base class for objects that are used to convert between two different sample formats.

		The AudioData::ConverterInstance implements this base class and can be templated, so
		you can create an instance that converts between two particular formats, and then
		store this in the abstract base class.

		@see AudioData::ConverterInstance
	*/
	class Converter
	{
	public:
		virtual ~Converter() {}

		/** Converts a sequence of samples from the converter's source format into the dest format. */
		virtual void convertSamples (void* destSamples, const void* sourceSamples, int numSamples) const = 0;

		/** Converts a sequence of samples from the converter's source format into the dest format.
			This method takes sub-channel indexes, which can be used with interleaved formats in order to choose a
			particular sub-channel of the data to be used.
		*/
		virtual void convertSamples (void* destSamples, int destSubChannel,
									 const void* sourceSamples, int sourceSubChannel, int numSamples) const = 0;
	};

	/**
		A class that converts between two templated AudioData::Pointer types, and which
		implements the AudioData::Converter interface.

		This can be used as a concrete instance of the AudioData::Converter abstract class.

		@see AudioData::Converter
	*/
	template <class SourceSampleType, class DestSampleType>
	class ConverterInstance  : public Converter
	{
	public:
		ConverterInstance (int numSourceChannels = 1, int numDestChannels = 1)
			: sourceChannels (numSourceChannels), destChannels (numDestChannels)
		{}

		~ConverterInstance() {}

		void convertSamples (void* dest, const void* source, int numSamples) const
		{
			SourceSampleType s (source, sourceChannels);
			DestSampleType d (dest, destChannels);
			d.convertSamples (s, numSamples);
		}

		void convertSamples (void* dest, int destSubChannel,
							 const void* source, int sourceSubChannel, int numSamples) const
		{
			jassert (destSubChannel < destChannels && sourceSubChannel < sourceChannels);

			SourceSampleType s (addBytesToPointer (source, sourceSubChannel * SourceSampleType::getBytesPerSample()), sourceChannels);
			DestSampleType d (addBytesToPointer (dest, destSubChannel * DestSampleType::getBytesPerSample()), destChannels);
			d.convertSamples (s, numSamples);
		}

	private:
		JUCE_DECLARE_NON_COPYABLE (ConverterInstance);

		const int sourceChannels, destChannels;
	};
};

/**
	A set of routines to convert buffers of 32-bit floating point data to and from
	various integer formats.

	Note that these functions are deprecated - the AudioData class provides a much more
	flexible set of conversion classes now.
*/
class JUCE_API  AudioDataConverters
{
public:

	static void convertFloatToInt16LE (const float* source, void* dest, int numSamples, int destBytesPerSample = 2);
	static void convertFloatToInt16BE (const float* source, void* dest, int numSamples, int destBytesPerSample = 2);

	static void convertFloatToInt24LE (const float* source, void* dest, int numSamples, int destBytesPerSample = 3);
	static void convertFloatToInt24BE (const float* source, void* dest, int numSamples, int destBytesPerSample = 3);

	static void convertFloatToInt32LE (const float* source, void* dest, int numSamples, int destBytesPerSample = 4);
	static void convertFloatToInt32BE (const float* source, void* dest, int numSamples, int destBytesPerSample = 4);

	static void convertFloatToFloat32LE (const float* source, void* dest, int numSamples, int destBytesPerSample = 4);
	static void convertFloatToFloat32BE (const float* source, void* dest, int numSamples, int destBytesPerSample = 4);

	static void convertInt16LEToFloat (const void* source, float* dest, int numSamples, int srcBytesPerSample = 2);
	static void convertInt16BEToFloat (const void* source, float* dest, int numSamples, int srcBytesPerSample = 2);

	static void convertInt24LEToFloat (const void* source, float* dest, int numSamples, int srcBytesPerSample = 3);
	static void convertInt24BEToFloat (const void* source, float* dest, int numSamples, int srcBytesPerSample = 3);

	static void convertInt32LEToFloat (const void* source, float* dest, int numSamples, int srcBytesPerSample = 4);
	static void convertInt32BEToFloat (const void* source, float* dest, int numSamples, int srcBytesPerSample = 4);

	static void convertFloat32LEToFloat (const void* source, float* dest, int numSamples, int srcBytesPerSample = 4);
	static void convertFloat32BEToFloat (const void* source, float* dest, int numSamples, int srcBytesPerSample = 4);

	enum DataFormat
	{
		int16LE,
		int16BE,
		int24LE,
		int24BE,
		int32LE,
		int32BE,
		float32LE,
		float32BE,
	};

	static void convertFloatToFormat (DataFormat destFormat,
									  const float* source, void* dest, int numSamples);

	static void convertFormatToFloat (DataFormat sourceFormat,
									  const void* source, float* dest, int numSamples);

	static void interleaveSamples (const float** source, float* dest,
								   int numSamples, int numChannels);

	static void deinterleaveSamples (const float* source, float** dest,
									 int numSamples, int numChannels);

private:
	AudioDataConverters();
	JUCE_DECLARE_NON_COPYABLE (AudioDataConverters);
};

#endif   // __JUCE_AUDIODATACONVERTERS_JUCEHEADER__

/*** End of inlined file: juce_AudioDataConverters.h ***/


#endif
#ifndef __JUCE_AUDIOSAMPLEBUFFER_JUCEHEADER__

/*** Start of inlined file: juce_AudioSampleBuffer.h ***/
#ifndef __JUCE_AUDIOSAMPLEBUFFER_JUCEHEADER__
#define __JUCE_AUDIOSAMPLEBUFFER_JUCEHEADER__

/**
	A multi-channel buffer of 32-bit floating point audio samples.

*/
class JUCE_API  AudioSampleBuffer
{
public:

	/** Creates a buffer with a specified number of channels and samples.

		The contents of the buffer will initially be undefined, so use clear() to
		set all the samples to zero.

		The buffer will allocate its memory internally, and this will be released
		when the buffer is deleted. If the memory can't be allocated, this will
		throw a std::bad_alloc exception.
	*/
	AudioSampleBuffer (int numChannels,
					   int numSamples) noexcept;

	/** Creates a buffer using a pre-allocated block of memory.

		Note that if the buffer is resized or its number of channels is changed, it
		will re-allocate memory internally and copy the existing data to this new area,
		so it will then stop directly addressing this memory.

		@param dataToReferTo    a pre-allocated array containing pointers to the data
								for each channel that should be used by this buffer. The
								buffer will only refer to this memory, it won't try to delete
								it when the buffer is deleted or resized.
		@param numChannels      the number of channels to use - this must correspond to the
								number of elements in the array passed in
		@param numSamples       the number of samples to use - this must correspond to the
								size of the arrays passed in
	*/
	AudioSampleBuffer (float** dataToReferTo,
					   int numChannels,
					   int numSamples) noexcept;

	/** Creates a buffer using a pre-allocated block of memory.

		Note that if the buffer is resized or its number of channels is changed, it
		will re-allocate memory internally and copy the existing data to this new area,
		so it will then stop directly addressing this memory.

		@param dataToReferTo    a pre-allocated array containing pointers to the data
								for each channel that should be used by this buffer. The
								buffer will only refer to this memory, it won't try to delete
								it when the buffer is deleted or resized.
		@param numChannels      the number of channels to use - this must correspond to the
								number of elements in the array passed in
		@param startSample      the offset within the arrays at which the data begins
		@param numSamples       the number of samples to use - this must correspond to the
								size of the arrays passed in
	*/
	AudioSampleBuffer (float** dataToReferTo,
					   int numChannels,
					   int startSample,
					   int numSamples) noexcept;

	/** Copies another buffer.

		This buffer will make its own copy of the other's data, unless the buffer was created
		using an external data buffer, in which case boths buffers will just point to the same
		shared block of data.
	*/
	AudioSampleBuffer (const AudioSampleBuffer& other) noexcept;

	/** Copies another buffer onto this one.

		This buffer's size will be changed to that of the other buffer.
	*/
	AudioSampleBuffer& operator= (const AudioSampleBuffer& other) noexcept;

	/** Destructor.

		This will free any memory allocated by the buffer.
	*/
	virtual ~AudioSampleBuffer() noexcept;

	/** Returns the number of channels of audio data that this buffer contains.

		@see getSampleData
	*/
	int getNumChannels() const noexcept     { return numChannels; }

	/** Returns the number of samples allocated in each of the buffer's channels.

		@see getSampleData
	*/
	int getNumSamples() const noexcept      { return size; }

	/** Returns a pointer one of the buffer's channels.

		For speed, this doesn't check whether the channel number is out of range,
		so be careful when using it!
	*/
	float* getSampleData (const int channelNumber) const noexcept
	{
		jassert (isPositiveAndBelow (channelNumber, numChannels));
		return channels [channelNumber];
	}

	/** Returns a pointer to a sample in one of the buffer's channels.

		For speed, this doesn't check whether the channel and sample number
		are out-of-range, so be careful when using it!
	*/
	float* getSampleData (const int channelNumber,
						  const int sampleOffset) const noexcept
	{
		jassert (isPositiveAndBelow (channelNumber, numChannels));
		jassert (isPositiveAndBelow (sampleOffset, size));
		return channels [channelNumber] + sampleOffset;
	}

	/** Returns an array of pointers to the channels in the buffer.

		Don't modify any of the pointers that are returned, and bear in mind that
		these will become invalid if the buffer is resized.
	*/
	float** getArrayOfChannels() const noexcept         { return channels; }

	/** Changes the buffer's size or number of channels.

		This can expand or contract the buffer's length, and add or remove channels.

		If keepExistingContent is true, it will try to preserve as much of the
		old data as it can in the new buffer.

		If clearExtraSpace is true, then any extra channels or space that is
		allocated will be also be cleared. If false, then this space is left
		uninitialised.

		If avoidReallocating is true, then changing the buffer's size won't reduce the
		amount of memory that is currently allocated (but it will still increase it if
		the new size is bigger than the amount it currently has). If this is false, then
		a new allocation will be done so that the buffer uses takes up the minimum amount
		of memory that it needs.

		If the required memory can't be allocated, this will throw a std::bad_alloc exception.
	*/
	void setSize (int newNumChannels,
				  int newNumSamples,
				  bool keepExistingContent = false,
				  bool clearExtraSpace = false,
				  bool avoidReallocating = false) noexcept;

	/** Makes this buffer point to a pre-allocated set of channel data arrays.

		There's also a constructor that lets you specify arrays like this, but this
		lets you change the channels dynamically.

		Note that if the buffer is resized or its number of channels is changed, it
		will re-allocate memory internally and copy the existing data to this new area,
		so it will then stop directly addressing this memory.

		@param dataToReferTo    a pre-allocated array containing pointers to the data
								for each channel that should be used by this buffer. The
								buffer will only refer to this memory, it won't try to delete
								it when the buffer is deleted or resized.
		@param numChannels      the number of channels to use - this must correspond to the
								number of elements in the array passed in
		@param numSamples       the number of samples to use - this must correspond to the
								size of the arrays passed in
	*/
	void setDataToReferTo (float** dataToReferTo,
						   int numChannels,
						   int numSamples) noexcept;

	/** Clears all the samples in all channels. */
	void clear() noexcept;

	/** Clears a specified region of all the channels.

		For speed, this doesn't check whether the channel and sample number
		are in-range, so be careful!
	*/
	void clear (int startSample,
				int numSamples) noexcept;

	/** Clears a specified region of just one channel.

		For speed, this doesn't check whether the channel and sample number
		are in-range, so be careful!
	*/
	void clear (int channel,
				int startSample,
				int numSamples) noexcept;

	/** Applies a gain multiple to a region of one channel.

		For speed, this doesn't check whether the channel and sample number
		are in-range, so be careful!
	*/
	void applyGain (int channel,
					int startSample,
					int numSamples,
					float gain) noexcept;

	/** Applies a gain multiple to a region of all the channels.

		For speed, this doesn't check whether the sample numbers
		are in-range, so be careful!
	*/
	void applyGain (int startSample,
					int numSamples,
					float gain) noexcept;

	/** Applies a range of gains to a region of a channel.

		The gain that is applied to each sample will vary from
		startGain on the first sample to endGain on the last Sample,
		so it can be used to do basic fades.

		For speed, this doesn't check whether the sample numbers
		are in-range, so be careful!
	*/
	void applyGainRamp (int channel,
						int startSample,
						int numSamples,
						float startGain,
						float endGain) noexcept;

	/** Adds samples from another buffer to this one.

		@param destChannel          the channel within this buffer to add the samples to
		@param destStartSample      the start sample within this buffer's channel
		@param source               the source buffer to add from
		@param sourceChannel        the channel within the source buffer to read from
		@param sourceStartSample    the offset within the source buffer's channel to start reading samples from
		@param numSamples           the number of samples to process
		@param gainToApplyToSource  an optional gain to apply to the source samples before they are
									added to this buffer's samples

		@see copyFrom
	*/
	void addFrom (int destChannel,
				  int destStartSample,
				  const AudioSampleBuffer& source,
				  int sourceChannel,
				  int sourceStartSample,
				  int numSamples,
				  float gainToApplyToSource = 1.0f) noexcept;

	/** Adds samples from an array of floats to one of the channels.

		@param destChannel          the channel within this buffer to add the samples to
		@param destStartSample      the start sample within this buffer's channel
		@param source               the source data to use
		@param numSamples           the number of samples to process
		@param gainToApplyToSource  an optional gain to apply to the source samples before they are
									added to this buffer's samples

		@see copyFrom
	*/
	void addFrom (int destChannel,
				  int destStartSample,
				  const float* source,
				  int numSamples,
				  float gainToApplyToSource = 1.0f) noexcept;

	/** Adds samples from an array of floats, applying a gain ramp to them.

		@param destChannel          the channel within this buffer to add the samples to
		@param destStartSample      the start sample within this buffer's channel
		@param source               the source data to use
		@param numSamples           the number of samples to process
		@param startGain            the gain to apply to the first sample (this is multiplied with
									the source samples before they are added to this buffer)
		@param endGain              the gain to apply to the final sample. The gain is linearly
									interpolated between the first and last samples.
	*/
	void addFromWithRamp (int destChannel,
						  int destStartSample,
						  const float* source,
						  int numSamples,
						  float startGain,
						  float endGain) noexcept;

	/** Copies samples from another buffer to this one.

		@param destChannel          the channel within this buffer to copy the samples to
		@param destStartSample      the start sample within this buffer's channel
		@param source               the source buffer to read from
		@param sourceChannel        the channel within the source buffer to read from
		@param sourceStartSample    the offset within the source buffer's channel to start reading samples from
		@param numSamples           the number of samples to process

		@see addFrom
	*/
	void copyFrom (int destChannel,
				   int destStartSample,
				   const AudioSampleBuffer& source,
				   int sourceChannel,
				   int sourceStartSample,
				   int numSamples) noexcept;

	/** Copies samples from an array of floats into one of the channels.

		@param destChannel          the channel within this buffer to copy the samples to
		@param destStartSample      the start sample within this buffer's channel
		@param source               the source buffer to read from
		@param numSamples           the number of samples to process

		@see addFrom
	*/
	void copyFrom (int destChannel,
				   int destStartSample,
				   const float* source,
				   int numSamples) noexcept;

	/** Copies samples from an array of floats into one of the channels, applying a gain to it.

		@param destChannel          the channel within this buffer to copy the samples to
		@param destStartSample      the start sample within this buffer's channel
		@param source               the source buffer to read from
		@param numSamples           the number of samples to process
		@param gain                 the gain to apply

		@see addFrom
	*/
	void copyFrom (int destChannel,
				   int destStartSample,
				   const float* source,
				   int numSamples,
				   float gain) noexcept;

	/** Copies samples from an array of floats into one of the channels, applying a gain ramp.

		@param destChannel          the channel within this buffer to copy the samples to
		@param destStartSample      the start sample within this buffer's channel
		@param source               the source buffer to read from
		@param numSamples           the number of samples to process
		@param startGain            the gain to apply to the first sample (this is multiplied with
									the source samples before they are copied to this buffer)
		@param endGain              the gain to apply to the final sample. The gain is linearly
									interpolated between the first and last samples.

		@see addFrom
	*/
	void copyFromWithRamp (int destChannel,
						   int destStartSample,
						   const float* source,
						   int numSamples,
						   float startGain,
						   float endGain) noexcept;

	/** Finds the highest and lowest sample values in a given range.

		@param channel      the channel to read from
		@param startSample  the start sample within the channel
		@param numSamples   the number of samples to check
		@param minVal       on return, the lowest value that was found
		@param maxVal       on return, the highest value that was found
	*/
	void findMinMax (int channel,
					 int startSample,
					 int numSamples,
					 float& minVal,
					 float& maxVal) const noexcept;

	/** Finds the highest absolute sample value within a region of a channel.
	*/
	float getMagnitude (int channel,
						int startSample,
						int numSamples) const noexcept;

	/** Finds the highest absolute sample value within a region on all channels.
	*/
	float getMagnitude (int startSample,
						int numSamples) const noexcept;

	/** Returns the root mean squared level for a region of a channel.
	*/
	float getRMSLevel (int channel,
					   int startSample,
					   int numSamples) const noexcept;

private:

	int numChannels, size;
	size_t allocatedBytes;
	float** channels;
	HeapBlock <char, true> allocatedData;
	float* preallocatedChannelSpace [32];

	void allocateData();
	void allocateChannels (float** dataToReferTo, int offset);

	JUCE_LEAK_DETECTOR (AudioSampleBuffer);
};

#endif   // __JUCE_AUDIOSAMPLEBUFFER_JUCEHEADER__

/*** End of inlined file: juce_AudioSampleBuffer.h ***/


#endif
#ifndef __JUCE_DECIBELS_JUCEHEADER__

/*** Start of inlined file: juce_Decibels.h ***/
#ifndef __JUCE_DECIBELS_JUCEHEADER__
#define __JUCE_DECIBELS_JUCEHEADER__

/**
	This class contains some helpful static methods for dealing with decibel values.
*/
class Decibels
{
public:

	/** Converts a dBFS value to its equivalent gain level.

		A gain of 1.0 = 0 dB, and lower gains map onto negative decibel values. Any
		decibel value lower than minusInfinityDb will return a gain of 0.
	*/
	template <typename Type>
	static Type decibelsToGain (const Type decibels,
								const Type minusInfinityDb = (Type) defaultMinusInfinitydB)
	{
		return decibels > minusInfinityDb ? powf ((Type) 10.0, decibels * (Type) 0.05)
										  : Type();
	}

	/** Converts a gain level into a dBFS value.

		A gain of 1.0 = 0 dB, and lower gains map onto negative decibel values.
		If the gain is 0 (or negative), then the method will return the value
		provided as minusInfinityDb.
	*/
	template <typename Type>
	static Type gainToDecibels (const Type gain,
								const Type minusInfinityDb = (Type) defaultMinusInfinitydB)
	{
		return gain > Type() ? jmax (minusInfinityDb, (Type) std::log10 (gain) * (Type) 20.0)
							 : minusInfinityDb;
	}

	/** Converts a decibel reading to a string, with the 'dB' suffix.
		If the decibel value is lower than minusInfinityDb, the return value will
		be "-INF dB".
	*/
	template <typename Type>
	static String toString (const Type decibels,
							const int decimalPlaces = 2,
							const Type minusInfinityDb = (Type) defaultMinusInfinitydB)
	{
		String s;

		if (decibels <= minusInfinityDb)
		{
			s = "-INF dB";
		}
		else
		{
			if (decibels >= Type())
				s << '+';

			s << String (decibels, decimalPlaces) << " dB";
		}

		return s;
	}

private:

	enum
	{
		defaultMinusInfinitydB = -100
	};

	Decibels(); // This class can't be instantiated, it's just a holder for static methods..
	JUCE_DECLARE_NON_COPYABLE (Decibels);
};

#endif   // __JUCE_DECIBELS_JUCEHEADER__

/*** End of inlined file: juce_Decibels.h ***/


#endif
#ifndef __JUCE_IIRFILTER_JUCEHEADER__

/*** Start of inlined file: juce_IIRFilter.h ***/
#ifndef __JUCE_IIRFILTER_JUCEHEADER__
#define __JUCE_IIRFILTER_JUCEHEADER__

/**
	An IIR filter that can perform low, high, or band-pass filtering on an
	audio signal.

	@see IIRFilterAudioSource
*/
class JUCE_API  IIRFilter
{
public:

	/** Creates a filter.

		Initially the filter is inactive, so will have no effect on samples that
		you process with it. Use the appropriate method to turn it into the type
		of filter needed.
	*/
	IIRFilter();

	/** Creates a copy of another filter. */
	IIRFilter (const IIRFilter& other);

	/** Destructor. */
	~IIRFilter();

	/** Resets the filter's processing pipeline, ready to start a new stream of data.

		Note that this clears the processing state, but the type of filter and
		its coefficients aren't changed. To put a filter into an inactive state, use
		the makeInactive() method.
	*/
	void reset() noexcept;

	/** Performs the filter operation on the given set of samples.
	*/
	void processSamples (float* samples,
						 int numSamples) noexcept;

	/** Processes a single sample, without any locking or checking.

		Use this if you need fast processing of a single value, but be aware that
		this isn't thread-safe in the way that processSamples() is.
	*/
	float processSingleSampleRaw (float sample) noexcept;

	/** Sets the filter up to act as a low-pass filter.
	*/
	void makeLowPass (double sampleRate,
					  double frequency) noexcept;

	/** Sets the filter up to act as a high-pass filter.
	*/
	void makeHighPass (double sampleRate,
					   double frequency) noexcept;

	/** Sets the filter up to act as a low-pass shelf filter with variable Q and gain.

		The gain is a scale factor that the low frequencies are multiplied by, so values
		greater than 1.0 will boost the low frequencies, values less than 1.0 will
		attenuate them.
	*/
	void makeLowShelf (double sampleRate,
					   double cutOffFrequency,
					   double Q,
					   float gainFactor) noexcept;

	/** Sets the filter up to act as a high-pass shelf filter with variable Q and gain.

		The gain is a scale factor that the high frequencies are multiplied by, so values
		greater than 1.0 will boost the high frequencies, values less than 1.0 will
		attenuate them.
	*/
	void makeHighShelf (double sampleRate,
						double cutOffFrequency,
						double Q,
						float gainFactor) noexcept;

	/** Sets the filter up to act as a band pass filter centred around a
		frequency, with a variable Q and gain.

		The gain is a scale factor that the centre frequencies are multiplied by, so
		values greater than 1.0 will boost the centre frequencies, values less than
		1.0 will attenuate them.
	*/
	void makeBandPass (double sampleRate,
					   double centreFrequency,
					   double Q,
					   float gainFactor) noexcept;

	/** Clears the filter's coefficients so that it becomes inactive.
	*/
	void makeInactive() noexcept;

	/** Makes this filter duplicate the set-up of another one.
	*/
	void copyCoefficientsFrom (const IIRFilter& other) noexcept;

protected:

	CriticalSection processLock;

	void setCoefficients (double c1, double c2, double c3,
						  double c4, double c5, double c6) noexcept;

	bool active;
	float coefficients[6];
	float x1, x2, y1, y2;

	// (use the copyCoefficientsFrom() method instead of this operator)
	IIRFilter& operator= (const IIRFilter&);
	JUCE_LEAK_DETECTOR (IIRFilter);
};

#endif   // __JUCE_IIRFILTER_JUCEHEADER__

/*** End of inlined file: juce_IIRFilter.h ***/


#endif
#ifndef __JUCE_REVERB_JUCEHEADER__

/*** Start of inlined file: juce_Reverb.h ***/
#ifndef __JUCE_REVERB_JUCEHEADER__
#define __JUCE_REVERB_JUCEHEADER__

/**
	Performs a simple reverb effect on a stream of audio data.

	This is a simple stereo reverb, based on the technique and tunings used in FreeVerb.
	Use setSampleRate() to prepare it, and then call processStereo() or processMono() to
	apply the reverb to your audio data.

	@see ReverbAudioSource
*/
class Reverb
{
public:

	Reverb()
	{
		setParameters (Parameters());
		setSampleRate (44100.0);
	}

	/** Holds the parameters being used by a Reverb object. */
	struct Parameters
	{
		Parameters() noexcept
		  : roomSize   (0.5f),
			damping    (0.5f),
			wetLevel   (0.33f),
			dryLevel   (0.4f),
			width      (1.0f),
			freezeMode (0)
		{}

		float roomSize;     /**< Room size, 0 to 1.0, where 1.0 is big, 0 is small. */
		float damping;      /**< Damping, 0 to 1.0, where 0 is not damped, 1.0 is fully damped. */
		float wetLevel;     /**< Wet level, 0 to 1.0 */
		float dryLevel;     /**< Dry level, 0 to 1.0 */
		float width;        /**< Reverb width, 0 to 1.0, where 1.0 is very wide. */
		float freezeMode;   /**< Freeze mode - values < 0.5 are "normal" mode, values > 0.5
								 put the reverb into a continuous feedback loop. */
	};

	/** Returns the reverb's current parameters. */
	const Parameters& getParameters() const noexcept    { return parameters; }

	/** Applies a new set of parameters to the reverb.
		Note that this doesn't attempt to lock the reverb, so if you call this in parallel with
		the process method, you may get artifacts.
	*/
	void setParameters (const Parameters& newParams)
	{
		const float wetScaleFactor = 3.0f;
		const float dryScaleFactor = 2.0f;

		const float wet = newParams.wetLevel * wetScaleFactor;
		wet1 = wet * (newParams.width * 0.5f + 0.5f);
		wet2 = wet * (1.0f - newParams.width) * 0.5f;
		dry = newParams.dryLevel * dryScaleFactor;
		gain = isFrozen (newParams.freezeMode) ? 0.0f : 0.015f;
		parameters = newParams;
		shouldUpdateDamping = true;
	}

	/** Sets the sample rate that will be used for the reverb.
		You must call this before the process methods, in order to tell it the correct sample rate.
	*/
	void setSampleRate (const double sampleRate)
	{
		jassert (sampleRate > 0);

		static const short combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // (at 44100Hz)
		static const short allPassTunings[] = { 556, 441, 341, 225 };
		const int stereoSpread = 23;
		const int intSampleRate = (int) sampleRate;

		int i;
		for (i = 0; i < numCombs; ++i)
		{
			comb[0][i].setSize ((intSampleRate * combTunings[i]) / 44100);
			comb[1][i].setSize ((intSampleRate * (combTunings[i] + stereoSpread)) / 44100);
		}

		for (i = 0; i < numAllPasses; ++i)
		{
			allPass[0][i].setSize ((intSampleRate * allPassTunings[i]) / 44100);
			allPass[1][i].setSize ((intSampleRate * (allPassTunings[i] + stereoSpread)) / 44100);
		}

		shouldUpdateDamping = true;
	}

	/** Clears the reverb's buffers. */
	void reset()
	{
		for (int j = 0; j < numChannels; ++j)
		{
			int i;
			for (i = 0; i < numCombs; ++i)
				comb[j][i].clear();

			for (i = 0; i < numAllPasses; ++i)
				allPass[j][i].clear();
		}
	}

	/** Applies the reverb to two stereo channels of audio data. */
	void processStereo (float* const left, float* const right, const int numSamples) noexcept
	{
		jassert (left != nullptr && right != nullptr);

		if (shouldUpdateDamping)
			updateDamping();

		for (int i = 0; i < numSamples; ++i)
		{
			const float input = (left[i] + right[i]) * gain;
			float outL = 0, outR = 0;

			int j;
			for (j = 0; j < numCombs; ++j)  // accumulate the comb filters in parallel
			{
				outL += comb[0][j].process (input);
				outR += comb[1][j].process (input);
			}

			for (j = 0; j < numAllPasses; ++j)  // run the allpass filters in series
			{
				outL = allPass[0][j].process (outL);
				outR = allPass[1][j].process (outR);
			}

			left[i]  = outL * wet1 + outR * wet2 + left[i]  * dry;
			right[i] = outR * wet1 + outL * wet2 + right[i] * dry;
		}
	}

	/** Applies the reverb to a single mono channel of audio data. */
	void processMono (float* const samples, const int numSamples) noexcept
	{
		jassert (samples != nullptr);

		if (shouldUpdateDamping)
			updateDamping();

		for (int i = 0; i < numSamples; ++i)
		{
			const float input = samples[i] * gain;
			float output = 0;

			int j;
			for (j = 0; j < numCombs; ++j)  // accumulate the comb filters in parallel
				output += comb[0][j].process (input);

			for (j = 0; j < numAllPasses; ++j)  // run the allpass filters in series
				output = allPass[0][j].process (output);

			samples[i] = output * wet1 + input * dry;
		}
	}

private:

	Parameters parameters;

	volatile bool shouldUpdateDamping;
	float gain, wet1, wet2, dry;

	inline static bool isFrozen (const float freezeMode) noexcept  { return freezeMode >= 0.5f; }

	void updateDamping() noexcept
	{
		const float roomScaleFactor = 0.28f;
		const float roomOffset = 0.7f;
		const float dampScaleFactor = 0.4f;

		shouldUpdateDamping = false;

		if (isFrozen (parameters.freezeMode))
			setDamping (1.0f, 0.0f);
		else
			setDamping (parameters.damping * dampScaleFactor,
						parameters.roomSize * roomScaleFactor + roomOffset);
	}

	void setDamping (const float dampingToUse, const float roomSizeToUse) noexcept
	{
		for (int j = 0; j < numChannels; ++j)
			for (int i = numCombs; --i >= 0;)
				comb[j][i].setFeedbackAndDamp (roomSizeToUse, dampingToUse);
	}

	class CombFilter
	{
	public:
		CombFilter() noexcept  : bufferSize (0), bufferIndex (0) {}

		void setSize (const int size)
		{
			if (size != bufferSize)
			{
				bufferIndex = 0;
				buffer.malloc ((size_t) size);
				bufferSize = size;
			}

			clear();
		}

		void clear() noexcept
		{
			last = 0;
			buffer.clear ((size_t) bufferSize);
		}

		void setFeedbackAndDamp (const float f, const float d) noexcept
		{
			damp1 = d;
			damp2 = 1.0f - d;
			feedback = f;
		}

		inline float process (const float input) noexcept
		{
			const float output = buffer [bufferIndex];
			last = (output * damp2) + (last * damp1);
			JUCE_UNDENORMALISE (last);

			float temp = input + (last * feedback);
			JUCE_UNDENORMALISE (temp);
			buffer [bufferIndex] = temp;
			bufferIndex = (bufferIndex + 1) % bufferSize;
			return output;
		}

	private:
		HeapBlock<float> buffer;
		int bufferSize, bufferIndex;
		float feedback, last, damp1, damp2;

		JUCE_DECLARE_NON_COPYABLE (CombFilter);
	};

	class AllPassFilter
	{
	public:
		AllPassFilter() noexcept  : bufferSize (0), bufferIndex (0) {}

		void setSize (const int size)
		{
			if (size != bufferSize)
			{
				bufferIndex = 0;
				buffer.malloc ((size_t) size);
				bufferSize = size;
			}

			clear();
		}

		void clear() noexcept
		{
			buffer.clear ((size_t) bufferSize);
		}

		inline float process (const float input) noexcept
		{
			const float bufferedValue = buffer [bufferIndex];
			float temp = input + (bufferedValue * 0.5f);
			JUCE_UNDENORMALISE (temp);
			buffer [bufferIndex] = temp;
			bufferIndex = (bufferIndex + 1) % bufferSize;
			return bufferedValue - input;
		}

	private:
		HeapBlock<float> buffer;
		int bufferSize, bufferIndex;

		JUCE_DECLARE_NON_COPYABLE (AllPassFilter);
	};

	enum { numCombs = 8, numAllPasses = 4, numChannels = 2 };

	CombFilter comb [numChannels][numCombs];
	AllPassFilter allPass [numChannels][numAllPasses];

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Reverb);
};

#endif   // __JUCE_REVERB_JUCEHEADER__

/*** End of inlined file: juce_Reverb.h ***/


#endif
#ifndef __JUCE_MIDIBUFFER_JUCEHEADER__

/*** Start of inlined file: juce_MidiBuffer.h ***/
#ifndef __JUCE_MIDIBUFFER_JUCEHEADER__
#define __JUCE_MIDIBUFFER_JUCEHEADER__


/*** Start of inlined file: juce_MidiMessage.h ***/
#ifndef __JUCE_MIDIMESSAGE_JUCEHEADER__
#define __JUCE_MIDIMESSAGE_JUCEHEADER__

/**
	Encapsulates a MIDI message.

	@see MidiMessageSequence, MidiOutput, MidiInput
*/
class JUCE_API  MidiMessage
{
public:

	/** Creates a 3-byte short midi message.

		@param byte1            message byte 1
		@param byte2            message byte 2
		@param byte3            message byte 3
		@param timeStamp        the time to give the midi message - this value doesn't
								use any particular units, so will be application-specific
	*/
	MidiMessage (int byte1, int byte2, int byte3, double timeStamp = 0) noexcept;

	/** Creates a 2-byte short midi message.

		@param byte1            message byte 1
		@param byte2            message byte 2
		@param timeStamp        the time to give the midi message - this value doesn't
								use any particular units, so will be application-specific
	*/
	MidiMessage (int byte1, int byte2, double timeStamp = 0) noexcept;

	/** Creates a 1-byte short midi message.

		@param byte1            message byte 1
		@param timeStamp        the time to give the midi message - this value doesn't
								use any particular units, so will be application-specific
	*/
	MidiMessage (int byte1, double timeStamp = 0) noexcept;

	/** Creates a midi message from a block of data. */
	MidiMessage (const void* data, int numBytes, double timeStamp = 0);

	/** Reads the next midi message from some data.

		This will read as many bytes from a data stream as it needs to make a
		complete message, and will return the number of bytes it used. This lets
		you read a sequence of midi messages from a file or stream.

		@param data             the data to read from
		@param maxBytesToUse    the maximum number of bytes it's allowed to read
		@param numBytesUsed     returns the number of bytes that were actually needed
		@param lastStatusByte   in a sequence of midi messages, the initial byte
								can be dropped from a message if it's the same as the
								first byte of the previous message, so this lets you
								supply the byte to use if the first byte of the message
								has in fact been dropped.
		@param timeStamp        the time to give the midi message - this value doesn't
								use any particular units, so will be application-specific
	*/
	MidiMessage (const void* data, int maxBytesToUse,
				 int& numBytesUsed, uint8 lastStatusByte,
				 double timeStamp = 0);

	/** Creates an active-sense message.
		Since the MidiMessage has to contain a valid message, this default constructor
		just initialises it with an empty sysex message.
	*/
	MidiMessage() noexcept;

	/** Creates a copy of another midi message. */
	MidiMessage (const MidiMessage& other);

	/** Creates a copy of another midi message, with a different timestamp. */
	MidiMessage (const MidiMessage& other, double newTimeStamp);

	/** Destructor. */
	~MidiMessage();

	/** Copies this message from another one. */
	MidiMessage& operator= (const MidiMessage& other);

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	MidiMessage (MidiMessage&& other) noexcept;
	MidiMessage& operator= (MidiMessage&& other) noexcept;
   #endif

	/** Returns a pointer to the raw midi data.

		@see getRawDataSize
	*/
	uint8* getRawData() const noexcept                          { return data; }

	/** Returns the number of bytes of data in the message.

		@see getRawData
	*/
	int getRawDataSize() const noexcept                         { return size; }

	/** Returns the timestamp associated with this message.

		The exact meaning of this time and its units will vary, as messages are used in
		a variety of different contexts.

		If you're getting the message from a midi file, this could be a time in seconds, or
		a number of ticks - see MidiFile::convertTimestampTicksToSeconds().

		If the message is being used in a MidiBuffer, it might indicate the number of
		audio samples from the start of the buffer.

		If the message was created by a MidiInput, see MidiInputCallback::handleIncomingMidiMessage()
		for details of the way that it initialises this value.

		@see setTimeStamp, addToTimeStamp
	*/
	double getTimeStamp() const noexcept                        { return timeStamp; }

	/** Changes the message's associated timestamp.

		The units for the timestamp will be application-specific - see the notes for getTimeStamp().

		@see addToTimeStamp, getTimeStamp
	*/
	void setTimeStamp (double newTimestamp) noexcept      { timeStamp = newTimestamp; }

	/** Adds a value to the message's timestamp.

		The units for the timestamp will be application-specific.
	*/
	void addToTimeStamp (double delta) noexcept           { timeStamp += delta; }

	/** Returns the midi channel associated with the message.

		@returns    a value 1 to 16 if the message has a channel, or 0 if it hasn't (e.g.
					if it's a sysex)
		@see isForChannel, setChannel
	*/
	int getChannel() const noexcept;

	/** Returns true if the message applies to the given midi channel.

		@param channelNumber    the channel number to look for, in the range 1 to 16
		@see getChannel, setChannel
	*/
	bool isForChannel (int channelNumber) const noexcept;

	/** Changes the message's midi channel.

		This won't do anything for non-channel messages like sysexes.

		@param newChannelNumber    the channel number to change it to, in the range 1 to 16
	*/
	void setChannel (int newChannelNumber) noexcept;

	/** Returns true if this is a system-exclusive message.
	*/
	bool isSysEx() const noexcept;

	/** Returns a pointer to the sysex data inside the message.

		If this event isn't a sysex event, it'll return 0.

		@see getSysExDataSize
	*/
	const uint8* getSysExData() const noexcept;

	/** Returns the size of the sysex data.

		This value excludes the 0xf0 header byte and the 0xf7 at the end.

		@see getSysExData
	*/
	int getSysExDataSize() const noexcept;

	/** Returns true if this message is a 'key-down' event.

		@param returnTrueForVelocity0   if true, then if this event is a note-on with
						velocity 0, it will still be considered to be a note-on and the
						method will return true. If returnTrueForVelocity0 is false, then
						if this is a note-on event with velocity 0, it'll be regarded as
						a note-off, and the method will return false

		@see isNoteOff, getNoteNumber, getVelocity, noteOn
	*/
	bool isNoteOn (bool returnTrueForVelocity0 = false) const noexcept;

	/** Creates a key-down message (using a floating-point velocity).

		@param channel      the midi channel, in the range 1 to 16
		@param noteNumber   the key number, 0 to 127
		@param velocity     in the range 0 to 1.0
		@see isNoteOn
	*/
	static MidiMessage noteOn (int channel, int noteNumber, float velocity) noexcept;

	/** Creates a key-down message (using an integer velocity).

		@param channel      the midi channel, in the range 1 to 16
		@param noteNumber   the key number, 0 to 127
		@param velocity     in the range 0 to 127
		@see isNoteOn
	*/
	static MidiMessage noteOn (int channel, int noteNumber, uint8 velocity) noexcept;

	/** Returns true if this message is a 'key-up' event.

		If returnTrueForNoteOnVelocity0 is true, then his will also return true
		for a note-on event with a velocity of 0.

		@see isNoteOn, getNoteNumber, getVelocity, noteOff
	*/
	bool isNoteOff (bool returnTrueForNoteOnVelocity0 = true) const noexcept;

	/** Creates a key-up message.

		@param channel      the midi channel, in the range 1 to 16
		@param noteNumber   the key number, 0 to 127
		@param velocity     in the range 0 to 127
		@see isNoteOff
	*/
	static MidiMessage noteOff (int channel, int noteNumber, uint8 velocity = 0) noexcept;

	/** Returns true if this message is a 'key-down' or 'key-up' event.

		@see isNoteOn, isNoteOff
	*/
	bool isNoteOnOrOff() const noexcept;

	/** Returns the midi note number for note-on and note-off messages.

		If the message isn't a note-on or off, the value returned is undefined.

		@see isNoteOff, getMidiNoteName, getMidiNoteInHertz, setNoteNumber
	*/
	int getNoteNumber() const noexcept;

	/** Changes the midi note number of a note-on or note-off message.

		If the message isn't a note on or off, this will do nothing.
	*/
	void setNoteNumber (int newNoteNumber) noexcept;

	/** Returns the velocity of a note-on or note-off message.

		The value returned will be in the range 0 to 127.
		If the message isn't a note-on or off event, it will return 0.

		@see getFloatVelocity
	*/
	uint8 getVelocity() const noexcept;

	/** Returns the velocity of a note-on or note-off message.

		The value returned will be in the range 0 to 1.0
		If the message isn't a note-on or off event, it will return 0.

		@see getVelocity, setVelocity
	*/
	float getFloatVelocity() const noexcept;

	/** Changes the velocity of a note-on or note-off message.

		If the message isn't a note on or off, this will do nothing.

		@param newVelocity  the new velocity, in the range 0 to 1.0
		@see getFloatVelocity, multiplyVelocity
	*/
	void setVelocity (float newVelocity) noexcept;

	/** Multiplies the velocity of a note-on or note-off message by a given amount.

		If the message isn't a note on or off, this will do nothing.

		@param scaleFactor  the value by which to multiply the velocity
		@see setVelocity
	*/
	void multiplyVelocity (float scaleFactor) noexcept;

	/** Returns true if this message is a 'sustain pedal down' controller message. */
	bool isSustainPedalOn() const noexcept;
	/** Returns true if this message is a 'sustain pedal up' controller message. */
	bool isSustainPedalOff() const noexcept;

	/** Returns true if this message is a 'sostenuto pedal down' controller message. */
	bool isSostenutoPedalOn() const noexcept;
	/** Returns true if this message is a 'sostenuto pedal up' controller message. */
	bool isSostenutoPedalOff() const noexcept;

	/** Returns true if this message is a 'soft pedal down' controller message. */
	bool isSoftPedalOn() const noexcept;
	/** Returns true if this message is a 'soft pedal up' controller message. */
	bool isSoftPedalOff() const noexcept;

	/** Returns true if the message is a program (patch) change message.

		@see getProgramChangeNumber, getGMInstrumentName
	*/
	bool isProgramChange() const noexcept;

	/** Returns the new program number of a program change message.

		If the message isn't a program change, the value returned will be
		nonsense.

		@see isProgramChange, getGMInstrumentName
	*/
	int getProgramChangeNumber() const noexcept;

	/** Creates a program-change message.

		@param channel          the midi channel, in the range 1 to 16
		@param programNumber    the midi program number, 0 to 127
		@see isProgramChange, getGMInstrumentName
	*/
	static MidiMessage programChange (int channel, int programNumber) noexcept;

	/** Returns true if the message is a pitch-wheel move.

		@see getPitchWheelValue, pitchWheel
	*/
	bool isPitchWheel() const noexcept;

	/** Returns the pitch wheel position from a pitch-wheel move message.

		The value returned is a 14-bit number from 0 to 0x3fff, indicating the wheel position.
		If called for messages which aren't pitch wheel events, the number returned will be
		nonsense.

		@see isPitchWheel
	*/
	int getPitchWheelValue() const noexcept;

	/** Creates a pitch-wheel move message.

		@param channel      the midi channel, in the range 1 to 16
		@param position     the wheel position, in the range 0 to 16383
		@see isPitchWheel
	*/
	static MidiMessage pitchWheel (int channel, int position) noexcept;

	/** Returns true if the message is an aftertouch event.

		For aftertouch events, use the getNoteNumber() method to find out the key
		that it applies to, and getAftertouchValue() to find out the amount. Use
		getChannel() to find out the channel.

		@see getAftertouchValue, getNoteNumber
	*/
	bool isAftertouch() const noexcept;

	/** Returns the amount of aftertouch from an aftertouch messages.

		The value returned is in the range 0 to 127, and will be nonsense for messages
		other than aftertouch messages.

		@see isAftertouch
	*/
	int getAfterTouchValue() const noexcept;

	/** Creates an aftertouch message.

		@param channel              the midi channel, in the range 1 to 16
		@param noteNumber           the key number, 0 to 127
		@param aftertouchAmount     the amount of aftertouch, 0 to 127
		@see isAftertouch
	*/
	static MidiMessage aftertouchChange (int channel,
										 int noteNumber,
										 int aftertouchAmount) noexcept;

	/** Returns true if the message is a channel-pressure change event.

		This is like aftertouch, but common to the whole channel rather than a specific
		note. Use getChannelPressureValue() to find out the pressure, and getChannel()
		to find out the channel.

		@see channelPressureChange
	*/
	bool isChannelPressure() const noexcept;

	/** Returns the pressure from a channel pressure change message.

		@returns the pressure, in the range 0 to 127
		@see isChannelPressure, channelPressureChange
	*/
	int getChannelPressureValue() const noexcept;

	/** Creates a channel-pressure change event.

		@param channel              the midi channel: 1 to 16
		@param pressure             the pressure, 0 to 127
		@see isChannelPressure
	*/
	static MidiMessage channelPressureChange (int channel, int pressure) noexcept;

	/** Returns true if this is a midi controller message.

		@see getControllerNumber, getControllerValue, controllerEvent
	*/
	bool isController() const noexcept;

	/** Returns the controller number of a controller message.

		The name of the controller can be looked up using the getControllerName() method.

		Note that the value returned is invalid for messages that aren't controller changes.

		@see isController, getControllerName, getControllerValue
	*/
	int getControllerNumber() const noexcept;

	/** Returns the controller value from a controller message.

		A value 0 to 127 is returned to indicate the new controller position.

		Note that the value returned is invalid for messages that aren't controller changes.

		@see isController, getControllerNumber
	*/
	int getControllerValue() const noexcept;

	/** Returns true if this message is a controller message and if it has the specified
		controller type.
	*/
	bool isControllerOfType (int controllerType) const noexcept;

	/** Creates a controller message.

		@param channel          the midi channel, in the range 1 to 16
		@param controllerType   the type of controller
		@param value            the controller value
		@see isController
	*/
	static MidiMessage controllerEvent (int channel,
										int controllerType,
										int value) noexcept;

	/** Checks whether this message is an all-notes-off message.

		@see allNotesOff
	*/
	bool isAllNotesOff() const noexcept;

	/** Checks whether this message is an all-sound-off message.

		@see allSoundOff
	*/
	bool isAllSoundOff() const noexcept;

	/** Creates an all-notes-off message.

		@param channel              the midi channel, in the range 1 to 16
		@see isAllNotesOff
	*/
	static MidiMessage allNotesOff (int channel) noexcept;

	/** Creates an all-sound-off message.

		@param channel              the midi channel, in the range 1 to 16
		@see isAllSoundOff
	*/
	static MidiMessage allSoundOff (int channel) noexcept;

	/** Creates an all-controllers-off message.

		@param channel              the midi channel, in the range 1 to 16
	*/
	static MidiMessage allControllersOff (int channel) noexcept;

	/** Returns true if this event is a meta-event.

		Meta-events are things like tempo changes, track names, etc.

		@see getMetaEventType, isTrackMetaEvent, isEndOfTrackMetaEvent,
			 isTextMetaEvent, isTrackNameEvent, isTempoMetaEvent, isTimeSignatureMetaEvent,
			 isKeySignatureMetaEvent, isMidiChannelMetaEvent
	*/
	bool isMetaEvent() const noexcept;

	/** Returns a meta-event's type number.

		If the message isn't a meta-event, this will return -1.

		@see isMetaEvent, isTrackMetaEvent, isEndOfTrackMetaEvent,
			 isTextMetaEvent, isTrackNameEvent, isTempoMetaEvent, isTimeSignatureMetaEvent,
			 isKeySignatureMetaEvent, isMidiChannelMetaEvent
	*/
	int getMetaEventType() const noexcept;

	/** Returns a pointer to the data in a meta-event.

		@see isMetaEvent, getMetaEventLength
	*/
	const uint8* getMetaEventData() const noexcept;

	/** Returns the length of the data for a meta-event.

		@see isMetaEvent, getMetaEventData
	*/
	int getMetaEventLength() const noexcept;

	/** Returns true if this is a 'track' meta-event. */
	bool isTrackMetaEvent() const noexcept;

	/** Returns true if this is an 'end-of-track' meta-event. */
	bool isEndOfTrackMetaEvent() const noexcept;

	/** Creates an end-of-track meta-event.

		@see isEndOfTrackMetaEvent
	*/
	static MidiMessage endOfTrack() noexcept;

	/** Returns true if this is an 'track name' meta-event.

		You can use the getTextFromTextMetaEvent() method to get the track's name.
	*/
	bool isTrackNameEvent() const noexcept;

	/** Returns true if this is a 'text' meta-event.

		@see getTextFromTextMetaEvent
	*/
	bool isTextMetaEvent() const noexcept;

	/** Returns the text from a text meta-event.

		@see isTextMetaEvent
	*/
	String getTextFromTextMetaEvent() const;

	/** Returns true if this is a 'tempo' meta-event.

		@see getTempoMetaEventTickLength, getTempoSecondsPerQuarterNote
	*/
	bool isTempoMetaEvent() const noexcept;

	/** Returns the tick length from a tempo meta-event.

		@param timeFormat   the 16-bit time format value from the midi file's header.
		@returns the tick length (in seconds).
		@see isTempoMetaEvent
	*/
	double getTempoMetaEventTickLength (short timeFormat) const noexcept;

	/** Calculates the seconds-per-quarter-note from a tempo meta-event.

		@see isTempoMetaEvent, getTempoMetaEventTickLength
	*/
	double getTempoSecondsPerQuarterNote() const noexcept;

	/** Creates a tempo meta-event.

		@see isTempoMetaEvent
	*/
	static MidiMessage tempoMetaEvent (int microsecondsPerQuarterNote) noexcept;

	/** Returns true if this is a 'time-signature' meta-event.

		@see getTimeSignatureInfo
	*/
	bool isTimeSignatureMetaEvent() const noexcept;

	/** Returns the time-signature values from a time-signature meta-event.

		@see isTimeSignatureMetaEvent
	*/
	void getTimeSignatureInfo (int& numerator, int& denominator) const noexcept;

	/** Creates a time-signature meta-event.

		@see isTimeSignatureMetaEvent
	*/
	static MidiMessage timeSignatureMetaEvent (int numerator, int denominator);

	/** Returns true if this is a 'key-signature' meta-event.

		@see getKeySignatureNumberOfSharpsOrFlats
	*/
	bool isKeySignatureMetaEvent() const noexcept;

	/** Returns the key from a key-signature meta-event.

		@see isKeySignatureMetaEvent
	*/
	int getKeySignatureNumberOfSharpsOrFlats() const noexcept;

	/** Returns true if this is a 'channel' meta-event.

		A channel meta-event specifies the midi channel that should be used
		for subsequent meta-events.

		@see getMidiChannelMetaEventChannel
	*/
	bool isMidiChannelMetaEvent() const noexcept;

	/** Returns the channel number from a channel meta-event.

		@returns the channel, in the range 1 to 16.
		@see isMidiChannelMetaEvent
	*/
	int getMidiChannelMetaEventChannel() const noexcept;

	/** Creates a midi channel meta-event.

		@param channel              the midi channel, in the range 1 to 16
		@see isMidiChannelMetaEvent
	*/
	static MidiMessage midiChannelMetaEvent (int channel) noexcept;

	/** Returns true if this is an active-sense message. */
	bool isActiveSense() const noexcept;

	/** Returns true if this is a midi start event.

		@see midiStart
	*/
	bool isMidiStart() const noexcept;

	/** Creates a midi start event. */
	static MidiMessage midiStart() noexcept;

	/** Returns true if this is a midi continue event.

		@see midiContinue
	*/
	bool isMidiContinue() const noexcept;

	/** Creates a midi continue event. */
	static MidiMessage midiContinue() noexcept;

	/** Returns true if this is a midi stop event.

		@see midiStop
	*/
	bool isMidiStop() const noexcept;

	/** Creates a midi stop event. */
	static MidiMessage midiStop() noexcept;

	/** Returns true if this is a midi clock event.

		@see midiClock, songPositionPointer
	*/
	bool isMidiClock() const noexcept;

	/** Creates a midi clock event. */
	static MidiMessage midiClock() noexcept;

	/** Returns true if this is a song-position-pointer message.

		@see getSongPositionPointerMidiBeat, songPositionPointer
	*/
	bool isSongPositionPointer() const noexcept;

	/** Returns the midi beat-number of a song-position-pointer message.

		@see isSongPositionPointer, songPositionPointer
	*/
	int getSongPositionPointerMidiBeat() const noexcept;

	/** Creates a song-position-pointer message.

		The position is a number of midi beats from the start of the song, where 1 midi
		beat is 6 midi clocks, and there are 24 midi clocks in a quarter-note. So there
		are 4 midi beats in a quarter-note.

		@see isSongPositionPointer, getSongPositionPointerMidiBeat
	*/
	static MidiMessage songPositionPointer (int positionInMidiBeats) noexcept;

	/** Returns true if this is a quarter-frame midi timecode message.

		@see quarterFrame, getQuarterFrameSequenceNumber, getQuarterFrameValue
	*/
	bool isQuarterFrame() const noexcept;

	/** Returns the sequence number of a quarter-frame midi timecode message.

		This will be a value between 0 and 7.

		@see isQuarterFrame, getQuarterFrameValue, quarterFrame
	*/
	int getQuarterFrameSequenceNumber() const noexcept;

	/** Returns the value from a quarter-frame message.

		This will be the lower nybble of the message's data-byte, a value
		between 0 and 15
	*/
	int getQuarterFrameValue() const noexcept;

	/** Creates a quarter-frame MTC message.

		@param sequenceNumber   a value 0 to 7 for the upper nybble of the message's data byte
		@param value            a value 0 to 15 for the lower nybble of the message's data byte
	*/
	static MidiMessage quarterFrame (int sequenceNumber, int value) noexcept;

	/** SMPTE timecode types.

		Used by the getFullFrameParameters() and fullFrame() methods.
	*/
	enum SmpteTimecodeType
	{
		fps24       = 0,
		fps25       = 1,
		fps30drop   = 2,
		fps30       = 3
	};

	/** Returns true if this is a full-frame midi timecode message.
	*/
	bool isFullFrame() const noexcept;

	/** Extracts the timecode information from a full-frame midi timecode message.

		You should only call this on messages where you've used isFullFrame() to
		check that they're the right kind.
	*/
	void getFullFrameParameters (int& hours,
								 int& minutes,
								 int& seconds,
								 int& frames,
								 SmpteTimecodeType& timecodeType) const noexcept;

	/** Creates a full-frame MTC message.
	*/
	static MidiMessage fullFrame (int hours,
								  int minutes,
								  int seconds,
								  int frames,
								  SmpteTimecodeType timecodeType);

	/** Types of MMC command.

		@see isMidiMachineControlMessage, getMidiMachineControlCommand, midiMachineControlCommand
	*/
	enum MidiMachineControlCommand
	{
		mmc_stop            = 1,
		mmc_play            = 2,
		mmc_deferredplay    = 3,
		mmc_fastforward     = 4,
		mmc_rewind          = 5,
		mmc_recordStart     = 6,
		mmc_recordStop      = 7,
		mmc_pause           = 9
	};

	/** Checks whether this is an MMC message.

		If it is, you can use the getMidiMachineControlCommand() to find out its type.
	*/
	bool isMidiMachineControlMessage() const noexcept;

	/** For an MMC message, this returns its type.

		Make sure it's actually an MMC message with isMidiMachineControlMessage() before
		calling this method.
	*/
	MidiMachineControlCommand getMidiMachineControlCommand() const noexcept;

	/** Creates an MMC message.
	*/
	static MidiMessage midiMachineControlCommand (MidiMachineControlCommand command);

	/** Checks whether this is an MMC "goto" message.

		If it is, the parameters passed-in are set to the time that the message contains.

		@see midiMachineControlGoto
	*/
	bool isMidiMachineControlGoto (int& hours,
								   int& minutes,
								   int& seconds,
								   int& frames) const noexcept;

	/** Creates an MMC "goto" message.

		This messages tells the device to go to a specific frame.

		@see isMidiMachineControlGoto
	*/
	static MidiMessage midiMachineControlGoto (int hours,
											   int minutes,
											   int seconds,
											   int frames);

	/** Creates a master-volume change message.

		@param volume   the volume, 0 to 1.0
	*/
	static MidiMessage masterVolume (float volume);

	/** Creates a system-exclusive message.

		The data passed in is wrapped with header and tail bytes of 0xf0 and 0xf7.
	*/
	static MidiMessage createSysExMessage (const uint8* sysexData,
										   int dataSize);

	/** Reads a midi variable-length integer.

		@param data             the data to read the number from
		@param numBytesUsed     on return, this will be set to the number of bytes that were read
	*/
	static int readVariableLengthVal (const uint8* data,
									  int& numBytesUsed) noexcept;

	/** Based on the first byte of a short midi message, this uses a lookup table
		to return the message length (either 1, 2, or 3 bytes).

		The value passed in must be 0x80 or higher.
	*/
	static int getMessageLengthFromFirstByte (const uint8 firstByte) noexcept;

	/** Returns the name of a midi note number.

		E.g "C", "D#", etc.

		@param noteNumber           the midi note number, 0 to 127
		@param useSharps            if true, sharpened notes are used, e.g. "C#", otherwise
									they'll be flattened, e.g. "Db"
		@param includeOctaveNumber  if true, the octave number will be appended to the string,
									e.g. "C#4"
		@param octaveNumForMiddleC  if an octave number is being appended, this indicates the
									number that will be used for middle C's octave

		@see getMidiNoteInHertz
	*/
	static String getMidiNoteName (int noteNumber,
								   bool useSharps,
								   bool includeOctaveNumber,
								   int octaveNumForMiddleC);

	/** Returns the frequency of a midi note number.

		The frequencyOfA parameter is an optional frequency for 'A', normally 440-444Hz for concert pitch.
		@see getMidiNoteName
	*/
	static const double getMidiNoteInHertz (int noteNumber, const double frequencyOfA = 440.0) noexcept;

	/** Returns the standard name of a GM instrument.

		@param midiInstrumentNumber     the program number 0 to 127
		@see getProgramChangeNumber
	*/
	static String getGMInstrumentName (int midiInstrumentNumber);

	/** Returns the name of a bank of GM instruments.

		@param midiBankNumber   the bank, 0 to 15
	*/
	static String getGMInstrumentBankName (int midiBankNumber);

	/** Returns the standard name of a channel 10 percussion sound.

		@param midiNoteNumber   the key number, 35 to 81
	*/
	static String getRhythmInstrumentName (int midiNoteNumber);

	/** Returns the name of a controller type number.

		@see getControllerNumber
	*/
	static String getControllerName (int controllerNumber);

private:

	double timeStamp;
	uint8* data;
	int size;

   #ifndef DOXYGEN
	union
	{
		uint8 asBytes[4];
		uint32 asInt32;
	} preallocatedData;
   #endif

	void freeData() noexcept;
	void setToUseInternalData() noexcept;
	bool usesAllocatedData() const noexcept;
};

#endif   // __JUCE_MIDIMESSAGE_JUCEHEADER__

/*** End of inlined file: juce_MidiMessage.h ***/

/**
	Holds a sequence of time-stamped midi events.

	Analogous to the AudioSampleBuffer, this holds a set of midi events with
	integer time-stamps. The buffer is kept sorted in order of the time-stamps.

	@see MidiMessage
*/
class JUCE_API  MidiBuffer
{
public:

	/** Creates an empty MidiBuffer. */
	MidiBuffer() noexcept;

	/** Creates a MidiBuffer containing a single midi message. */
	explicit MidiBuffer (const MidiMessage& message) noexcept;

	/** Creates a copy of another MidiBuffer. */
	MidiBuffer (const MidiBuffer& other) noexcept;

	/** Makes a copy of another MidiBuffer. */
	MidiBuffer& operator= (const MidiBuffer& other) noexcept;

	/** Destructor */
	~MidiBuffer();

	/** Removes all events from the buffer. */
	void clear() noexcept;

	/** Removes all events between two times from the buffer.

		All events for which (start <= event position < start + numSamples) will
		be removed.
	*/
	void clear (int start, int numSamples);

	/** Returns true if the buffer is empty.

		To actually retrieve the events, use a MidiBuffer::Iterator object
	*/
	bool isEmpty() const noexcept;

	/** Counts the number of events in the buffer.

		This is actually quite a slow operation, as it has to iterate through all
		the events, so you might prefer to call isEmpty() if that's all you need
		to know.
	*/
	int getNumEvents() const noexcept;

	/** Adds an event to the buffer.

		The sample number will be used to determine the position of the event in
		the buffer, which is always kept sorted. The MidiMessage's timestamp is
		ignored.

		If an event is added whose sample position is the same as one or more events
		already in the buffer, the new event will be placed after the existing ones.

		To retrieve events, use a MidiBuffer::Iterator object
	*/
	void addEvent (const MidiMessage& midiMessage, int sampleNumber);

	/** Adds an event to the buffer from raw midi data.

		The sample number will be used to determine the position of the event in
		the buffer, which is always kept sorted.

		If an event is added whose sample position is the same as one or more events
		already in the buffer, the new event will be placed after the existing ones.

		The event data will be inspected to calculate the number of bytes in length that
		the midi event really takes up, so maxBytesOfMidiData may be longer than the data
		that actually gets stored. E.g. if you pass in a note-on and a length of 4 bytes,
		it'll actually only store 3 bytes. If the midi data is invalid, it might not
		add an event at all.

		To retrieve events, use a MidiBuffer::Iterator object
	*/
	void addEvent (const void* rawMidiData,
				   int maxBytesOfMidiData,
				   int sampleNumber);

	/** Adds some events from another buffer to this one.

		@param otherBuffer          the buffer containing the events you want to add
		@param startSample          the lowest sample number in the source buffer for which
									events should be added. Any source events whose timestamp is
									less than this will be ignored
		@param numSamples           the valid range of samples from the source buffer for which
									events should be added - i.e. events in the source buffer whose
									timestamp is greater than or equal to (startSample + numSamples)
									will be ignored. If this value is less than 0, all events after
									startSample will be taken.
		@param sampleDeltaToAdd     a value which will be added to the source timestamps of the events
									that are added to this buffer
	*/
	void addEvents (const MidiBuffer& otherBuffer,
					int startSample,
					int numSamples,
					int sampleDeltaToAdd);

	/** Returns the sample number of the first event in the buffer.

		If the buffer's empty, this will just return 0.
	*/
	int getFirstEventTime() const noexcept;

	/** Returns the sample number of the last event in the buffer.

		If the buffer's empty, this will just return 0.
	*/
	int getLastEventTime() const noexcept;

	/** Exchanges the contents of this buffer with another one.

		This is a quick operation, because no memory allocating or copying is done, it
		just swaps the internal state of the two buffers.
	*/
	void swapWith (MidiBuffer& other) noexcept;

	/** Preallocates some memory for the buffer to use.
		This helps to avoid needing to reallocate space when the buffer has messages
		added to it.
	*/
	void ensureSize (size_t minimumNumBytes);

	/**
		Used to iterate through the events in a MidiBuffer.

		Note that altering the buffer while an iterator is using it isn't a
		safe operation.

		@see MidiBuffer
	*/
	class JUCE_API  Iterator
	{
	public:

		/** Creates an Iterator for this MidiBuffer. */
		Iterator (const MidiBuffer& buffer) noexcept;

		/** Destructor. */
		~Iterator() noexcept;

		/** Repositions the iterator so that the next event retrieved will be the first
			one whose sample position is at greater than or equal to the given position.
		*/
		void setNextSamplePosition (int samplePosition) noexcept;

		/** Retrieves a copy of the next event from the buffer.

			@param result   on return, this will be the message (the MidiMessage's timestamp
							is not set)
			@param samplePosition   on return, this will be the position of the event
			@returns        true if an event was found, or false if the iterator has reached
							the end of the buffer
		*/
		bool getNextEvent (MidiMessage& result,
						   int& samplePosition) noexcept;

		/** Retrieves the next event from the buffer.

			@param midiData     on return, this pointer will be set to a block of data containing
								the midi message. Note that to make it fast, this is a pointer
								directly into the MidiBuffer's internal data, so is only valid
								temporarily until the MidiBuffer is altered.
			@param numBytesOfMidiData   on return, this is the number of bytes of data used by the
										midi message
			@param samplePosition   on return, this will be the position of the event
			@returns        true if an event was found, or false if the iterator has reached
							the end of the buffer
		*/
		bool getNextEvent (const uint8* &midiData,
						   int& numBytesOfMidiData,
						   int& samplePosition) noexcept;

	private:

		const MidiBuffer& buffer;
		const uint8* data;

		JUCE_DECLARE_NON_COPYABLE (Iterator);
	};

private:

	friend class MidiBuffer::Iterator;
	MemoryBlock data;
	int bytesUsed;

	uint8* getData() const noexcept;
	uint8* findEventAfter (uint8*, int samplePosition) const noexcept;

	JUCE_LEAK_DETECTOR (MidiBuffer);
};

#endif   // __JUCE_MIDIBUFFER_JUCEHEADER__

/*** End of inlined file: juce_MidiBuffer.h ***/


#endif
#ifndef __JUCE_MIDIFILE_JUCEHEADER__

/*** Start of inlined file: juce_MidiFile.h ***/
#ifndef __JUCE_MIDIFILE_JUCEHEADER__
#define __JUCE_MIDIFILE_JUCEHEADER__


/*** Start of inlined file: juce_MidiMessageSequence.h ***/
#ifndef __JUCE_MIDIMESSAGESEQUENCE_JUCEHEADER__
#define __JUCE_MIDIMESSAGESEQUENCE_JUCEHEADER__

/**
	A sequence of timestamped midi messages.

	This allows the sequence to be manipulated, and also to be read from and
	written to a standard midi file.

	@see MidiMessage, MidiFile
*/
class JUCE_API  MidiMessageSequence
{
public:

	/** Creates an empty midi sequence object. */
	MidiMessageSequence();

	/** Creates a copy of another sequence. */
	MidiMessageSequence (const MidiMessageSequence& other);

	/** Replaces this sequence with another one. */
	MidiMessageSequence& operator= (const MidiMessageSequence& other);

	/** Destructor. */
	~MidiMessageSequence();

	/** Structure used to hold midi events in the sequence.

		These structures act as 'handles' on the events as they are moved about in
		the list, and make it quick to find the matching note-offs for note-on events.

		@see MidiMessageSequence::getEventPointer
	*/
	class MidiEventHolder
	{
	public:

		/** Destructor. */
		~MidiEventHolder();

		/** The message itself, whose timestamp is used to specify the event's time.
		*/
		MidiMessage message;

		/** The matching note-off event (if this is a note-on event).

			If this isn't a note-on, this pointer will be null.

			Use the MidiMessageSequence::updateMatchedPairs() method to keep these
			note-offs up-to-date after events have been moved around in the sequence
			or deleted.
		*/
		MidiEventHolder* noteOffObject;

	private:

		friend class MidiMessageSequence;
		MidiEventHolder (const MidiMessage& message);
		JUCE_LEAK_DETECTOR (MidiEventHolder);
	};

	/** Clears the sequence. */
	void clear();

	/** Returns the number of events in the sequence. */
	int getNumEvents() const;

	/** Returns a pointer to one of the events. */
	MidiEventHolder* getEventPointer (int index) const;

	/** Returns the time of the note-up that matches the note-on at this index.

		If the event at this index isn't a note-on, it'll just return 0.

		@see MidiMessageSequence::MidiEventHolder::noteOffObject
	*/
	double getTimeOfMatchingKeyUp (int index) const;

	/** Returns the index of the note-up that matches the note-on at this index.

		If the event at this index isn't a note-on, it'll just return -1.

		@see MidiMessageSequence::MidiEventHolder::noteOffObject
	*/
	int getIndexOfMatchingKeyUp (int index) const;

	/** Returns the index of an event. */
	int getIndexOf (MidiEventHolder* event) const;

	/** Returns the index of the first event on or after the given timestamp.

		If the time is beyond the end of the sequence, this will return the
		number of events.
	*/
	int getNextIndexAtTime (double timeStamp) const;

	/** Returns the timestamp of the first event in the sequence.

		@see getEndTime
	*/
	double getStartTime() const;

	/** Returns the timestamp of the last event in the sequence.

		@see getStartTime
	*/
	double getEndTime() const;

	/** Returns the timestamp of the event at a given index.

		If the index is out-of-range, this will return 0.0
	*/
	double getEventTime (int index) const;

	/** Inserts a midi message into the sequence.

		The index at which the new message gets inserted will depend on its timestamp,
		because the sequence is kept sorted.

		Remember to call updateMatchedPairs() after adding note-on events.

		@param newMessage       the new message to add (an internal copy will be made)
		@param timeAdjustment   an optional value to add to the timestamp of the message
								that will be inserted
		@see updateMatchedPairs
	*/
	MidiEventHolder* addEvent (const MidiMessage& newMessage,
							   double timeAdjustment = 0);

	/** Deletes one of the events in the sequence.

		Remember to call updateMatchedPairs() after removing events.

		@param index                 the index of the event to delete
		@param deleteMatchingNoteUp  whether to also remove the matching note-off
									 if the event you're removing is a note-on
	*/
	void deleteEvent (int index, bool deleteMatchingNoteUp);

	/** Merges another sequence into this one.

		Remember to call updateMatchedPairs() after using this method.

		@param other                    the sequence to add from
		@param timeAdjustmentDelta      an amount to add to the timestamps of the midi events
										as they are read from the other sequence
		@param firstAllowableDestTime   events will not be added if their time is earlier
										than this time. (This is after their time has been adjusted
										by the timeAdjustmentDelta)
		@param endOfAllowableDestTimes  events will not be added if their time is equal to
										or greater than this time. (This is after their time has
										been adjusted by the timeAdjustmentDelta)
	*/
	void addSequence (const MidiMessageSequence& other,
					  double timeAdjustmentDelta,
					  double firstAllowableDestTime,
					  double endOfAllowableDestTimes);

	/** Makes sure all the note-on and note-off pairs are up-to-date.

		Call this after moving messages about or deleting/adding messages, and it
		will scan the list and make sure all the note-offs in the MidiEventHolder
		structures are pointing at the correct ones.
	*/
	void updateMatchedPairs();

	/** Copies all the messages for a particular midi channel to another sequence.

		@param channelNumberToExtract   the midi channel to look for, in the range 1 to 16
		@param destSequence             the sequence that the chosen events should be copied to
		@param alsoIncludeMetaEvents    if true, any meta-events (which don't apply to a specific
										channel) will also be copied across.
		@see extractSysExMessages
	*/
	void extractMidiChannelMessages (int channelNumberToExtract,
									 MidiMessageSequence& destSequence,
									 bool alsoIncludeMetaEvents) const;

	/** Copies all midi sys-ex messages to another sequence.

		@param destSequence     this is the sequence to which any sys-exes in this sequence
								will be added
		@see extractMidiChannelMessages
	*/
	void extractSysExMessages (MidiMessageSequence& destSequence) const;

	/** Removes any messages in this sequence that have a specific midi channel.

		@param channelNumberToRemove    the midi channel to look for, in the range 1 to 16
	*/
	void deleteMidiChannelMessages (int channelNumberToRemove);

	/** Removes any sys-ex messages from this sequence.
	*/
	void deleteSysExMessages();

	/** Adds an offset to the timestamps of all events in the sequence.

		@param deltaTime    the amount to add to each timestamp.
	*/
	void addTimeToMessages (double deltaTime);

	/** Scans through the sequence to determine the state of any midi controllers at
		a given time.

		This will create a sequence of midi controller changes that can be
		used to set all midi controllers to the state they would be in at the
		specified time within this sequence.

		As well as controllers, it will also recreate the midi program number
		and pitch bend position.

		@param channelNumber    the midi channel to look for, in the range 1 to 16. Controllers
								for other channels will be ignored.
		@param time             the time at which you want to find out the state - there are
								no explicit units for this time measurement, it's the same units
								as used for the timestamps of the messages
		@param resultMessages   an array to which midi controller-change messages will be added. This
								will be the minimum number of controller changes to recreate the
								state at the required time.
	*/
	void createControllerUpdatesForTime (int channelNumber, double time,
										 OwnedArray<MidiMessage>& resultMessages);

	/** Swaps this sequence with another one. */
	void swapWith (MidiMessageSequence& other) noexcept;

private:

	friend class MidiFile;
	OwnedArray <MidiEventHolder> list;

	JUCE_LEAK_DETECTOR (MidiMessageSequence);
};

#endif   // __JUCE_MIDIMESSAGESEQUENCE_JUCEHEADER__

/*** End of inlined file: juce_MidiMessageSequence.h ***/

/**
	Reads/writes standard midi format files.

	To read a midi file, create a MidiFile object and call its readFrom() method. You
	can then get the individual midi tracks from it using the getTrack() method.

	To write a file, create a MidiFile object, add some MidiMessageSequence objects
	to it using the addTrack() method, and then call its writeTo() method to stream
	it out.

	@see MidiMessageSequence
*/
class JUCE_API  MidiFile
{
public:

	/** Creates an empty MidiFile object.
	*/
	MidiFile();

	/** Destructor. */
	~MidiFile();

	/** Returns the number of tracks in the file.

		@see getTrack, addTrack
	*/
	int getNumTracks() const noexcept;

	/** Returns a pointer to one of the tracks in the file.

		@returns a pointer to the track, or 0 if the index is out-of-range
		@see getNumTracks, addTrack
	*/
	const MidiMessageSequence* getTrack (int index) const noexcept;

	/** Adds a midi track to the file.

		This will make its own internal copy of the sequence that is passed-in.

		@see getNumTracks, getTrack
	*/
	void addTrack (const MidiMessageSequence& trackSequence);

	/** Removes all midi tracks from the file.

		@see getNumTracks
	*/
	void clear();

	/** Returns the raw time format code that will be written to a stream.

		After reading a midi file, this method will return the time-format that
		was read from the file's header. It can be changed using the setTicksPerQuarterNote()
		or setSmpteTimeFormat() methods.

		If the value returned is positive, it indicates the number of midi ticks
		per quarter-note - see setTicksPerQuarterNote().

		It it's negative, the upper byte indicates the frames-per-second (but negative), and
		the lower byte is the number of ticks per frame - see setSmpteTimeFormat().
	*/
	short getTimeFormat() const noexcept;

	/** Sets the time format to use when this file is written to a stream.

		If this is called, the file will be written as bars/beats using the
		specified resolution, rather than SMPTE absolute times, as would be
		used if setSmpteTimeFormat() had been called instead.

		@param ticksPerQuarterNote  e.g. 96, 960
		@see setSmpteTimeFormat
	*/
	void setTicksPerQuarterNote (int ticksPerQuarterNote) noexcept;

	/** Sets the time format to use when this file is written to a stream.

		If this is called, the file will be written using absolute times, rather
		than bars/beats as would be the case if setTicksPerBeat() had been called
		instead.

		@param framesPerSecond      must be 24, 25, 29 or 30
		@param subframeResolution   the sub-second resolution, e.g. 4 (midi time code),
									8, 10, 80 (SMPTE bit resolution), or 100. For millisecond
									timing, setSmpteTimeFormat (25, 40)
		@see setTicksPerBeat
	*/
	void setSmpteTimeFormat (int framesPerSecond,
							 int subframeResolution) noexcept;

	/** Makes a list of all the tempo-change meta-events from all tracks in the midi file.

		Useful for finding the positions of all the tempo changes in a file.

		@param tempoChangeEvents    a list to which all the events will be added
	*/
	void findAllTempoEvents (MidiMessageSequence& tempoChangeEvents) const;

	/** Makes a list of all the time-signature meta-events from all tracks in the midi file.

		Useful for finding the positions of all the tempo changes in a file.

		@param timeSigEvents        a list to which all the events will be added
	*/
	void findAllTimeSigEvents (MidiMessageSequence& timeSigEvents) const;

	/** Returns the latest timestamp in any of the tracks.

		(Useful for finding the length of the file).
	*/
	double getLastTimestamp() const;

	/** Reads a midi file format stream.

		After calling this, you can get the tracks that were read from the file by using the
		getNumTracks() and getTrack() methods.

		The timestamps of the midi events in the tracks will represent their positions in
		terms of midi ticks. To convert them to seconds, use the convertTimestampTicksToSeconds()
		method.

		@returns true if the stream was read successfully
	*/
	bool readFrom (InputStream& sourceStream);

	/** Writes the midi tracks as a standard midi file.

		@returns true if the operation succeeded.
	*/
	bool writeTo (OutputStream& destStream);

	/** Converts the timestamp of all the midi events from midi ticks to seconds.

		This will use the midi time format and tempo/time signature info in the
		tracks to convert all the timestamps to absolute values in seconds.
	*/
	void convertTimestampTicksToSeconds();

private:

	OwnedArray <MidiMessageSequence> tracks;
	short timeFormat;

	void readNextTrack (const uint8* data, int size);
	void writeTrack (OutputStream& mainOut, int trackNum);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiFile);
};

#endif   // __JUCE_MIDIFILE_JUCEHEADER__

/*** End of inlined file: juce_MidiFile.h ***/


#endif
#ifndef __JUCE_MIDIKEYBOARDSTATE_JUCEHEADER__

/*** Start of inlined file: juce_MidiKeyboardState.h ***/
#ifndef __JUCE_MIDIKEYBOARDSTATE_JUCEHEADER__
#define __JUCE_MIDIKEYBOARDSTATE_JUCEHEADER__

class MidiKeyboardState;

/**
	Receives events from a MidiKeyboardState object.

	@see MidiKeyboardState
*/
class JUCE_API  MidiKeyboardStateListener
{
public:

	MidiKeyboardStateListener() noexcept        {}
	virtual ~MidiKeyboardStateListener()        {}

	/** Called when one of the MidiKeyboardState's keys is pressed.

		This will be called synchronously when the state is either processing a
		buffer in its MidiKeyboardState::processNextMidiBuffer() method, or
		when a note is being played with its MidiKeyboardState::noteOn() method.

		Note that this callback could happen from an audio callback thread, so be
		careful not to block, and avoid any UI activity in the callback.
	*/
	virtual void handleNoteOn (MidiKeyboardState* source,
							   int midiChannel, int midiNoteNumber, float velocity) = 0;

	/** Called when one of the MidiKeyboardState's keys is released.

		This will be called synchronously when the state is either processing a
		buffer in its MidiKeyboardState::processNextMidiBuffer() method, or
		when a note is being played with its MidiKeyboardState::noteOff() method.

		Note that this callback could happen from an audio callback thread, so be
		careful not to block, and avoid any UI activity in the callback.
	*/
	virtual void handleNoteOff (MidiKeyboardState* source,
								int midiChannel, int midiNoteNumber) = 0;
};

/**
	Represents a piano keyboard, keeping track of which keys are currently pressed.

	This object can parse a stream of midi events, using them to update its idea
	of which keys are pressed for each individiual midi channel.

	When keys go up or down, it can broadcast these events to listener objects.

	It also allows key up/down events to be triggered with its noteOn() and noteOff()
	methods, and midi messages for these events will be merged into the
	midi stream that gets processed by processNextMidiBuffer().
*/
class JUCE_API  MidiKeyboardState
{
public:

	MidiKeyboardState();
	~MidiKeyboardState();

	/** Resets the state of the object.

		All internal data for all the channels is reset, but no events are sent as a
		result.

		If you want to release any keys that are currently down, and to send out note-up
		midi messages for this, use the allNotesOff() method instead.
	*/
	void reset();

	/** Returns true if the given midi key is currently held down for the given midi channel.

		The channel number must be between 1 and 16. If you want to see if any notes are
		on for a range of channels, use the isNoteOnForChannels() method.
	*/
	bool isNoteOn (int midiChannel, int midiNoteNumber) const noexcept;

	/** Returns true if the given midi key is currently held down on any of a set of midi channels.

		The channel mask has a bit set for each midi channel you want to test for - bit
		0 = midi channel 1, bit 1 = midi channel 2, etc.

		If a note is on for at least one of the specified channels, this returns true.
	*/
	bool isNoteOnForChannels (int midiChannelMask, int midiNoteNumber) const noexcept;

	/** Turns a specified note on.

		This will cause a suitable midi note-on event to be injected into the midi buffer during the
		next call to processNextMidiBuffer().

		It will also trigger a synchronous callback to the listeners to tell them that the key has
		gone down.
	*/
	void noteOn (int midiChannel, int midiNoteNumber, float velocity);

	/** Turns a specified note off.

		This will cause a suitable midi note-off event to be injected into the midi buffer during the
		next call to processNextMidiBuffer().

		It will also trigger a synchronous callback to the listeners to tell them that the key has
		gone up.

		But if the note isn't acutally down for the given channel, this method will in fact do nothing.
	*/
	void noteOff (int midiChannel, int midiNoteNumber);

	/** This will turn off any currently-down notes for the given midi channel.

		If you pass 0 for the midi channel, it will in fact turn off all notes on all channels.

		Calling this method will make calls to noteOff(), so can trigger synchronous callbacks
		and events being added to the midi stream.
	*/
	void allNotesOff (int midiChannel);

	/** Looks at a key-up/down event and uses it to update the state of this object.

		To process a buffer full of midi messages, use the processNextMidiBuffer() method
		instead.
	*/
	void processNextMidiEvent (const MidiMessage& message);

	/** Scans a midi stream for up/down events and adds its own events to it.

		This will look for any up/down events and use them to update the internal state,
		synchronously making suitable callbacks to the listeners.

		If injectIndirectEvents is true, then midi events to produce the recent noteOn()
		and noteOff() calls will be added into the buffer.

		Only the section of the buffer whose timestamps are between startSample and
		(startSample + numSamples) will be affected, and any events added will be placed
		between these times.

		If you're going to use this method, you'll need to keep calling it regularly for
		it to work satisfactorily.

		To process a single midi event at a time, use the processNextMidiEvent() method
		instead.
	*/
	void processNextMidiBuffer (MidiBuffer& buffer,
								int startSample,
								int numSamples,
								bool injectIndirectEvents);

	/** Registers a listener for callbacks when keys go up or down.

		@see removeListener
	*/
	void addListener (MidiKeyboardStateListener* listener);

	/** Deregisters a listener.

		@see addListener
	*/
	void removeListener (MidiKeyboardStateListener* listener);

private:

	CriticalSection lock;
	uint16 noteStates [128];
	MidiBuffer eventsToAdd;
	Array <MidiKeyboardStateListener*> listeners;

	void noteOnInternal (int midiChannel, int midiNoteNumber, float velocity);
	void noteOffInternal (int midiChannel, int midiNoteNumber);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiKeyboardState);
};

#endif   // __JUCE_MIDIKEYBOARDSTATE_JUCEHEADER__

/*** End of inlined file: juce_MidiKeyboardState.h ***/


#endif
#ifndef __JUCE_MIDIMESSAGE_JUCEHEADER__

#endif
#ifndef __JUCE_MIDIMESSAGESEQUENCE_JUCEHEADER__

#endif
#ifndef __JUCE_AUDIOSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_AudioSource.h ***/
#ifndef __JUCE_AUDIOSOURCE_JUCEHEADER__
#define __JUCE_AUDIOSOURCE_JUCEHEADER__

/**
	Used by AudioSource::getNextAudioBlock().
*/
struct JUCE_API  AudioSourceChannelInfo
{
	/** Creates an uninitialised AudioSourceChannelInfo. */
	AudioSourceChannelInfo() noexcept
	{
	}

	/** Creates an AudioSourceChannelInfo. */
	AudioSourceChannelInfo (AudioSampleBuffer* bufferToUse,
							int startSampleOffset, int numSamplesToUse) noexcept
		: buffer (bufferToUse),
		  startSample (startSampleOffset),
		  numSamples (numSamplesToUse)
	{
	}

	/** Creates an AudioSourceChannelInfo that uses the whole of a buffer.
		Note that the buffer provided must not be deleted while the
		AudioSourceChannelInfo is still using it.
	*/
	explicit AudioSourceChannelInfo (AudioSampleBuffer& bufferToUse) noexcept
		: buffer (&bufferToUse),
		  startSample (0),
		  numSamples (bufferToUse.getNumSamples())
	{
	}

	/** The destination buffer to fill with audio data.

		When the AudioSource::getNextAudioBlock() method is called, the active section
		of this buffer should be filled with whatever output the source produces.

		Only the samples specified by the startSample and numSamples members of this structure
		should be affected by the call.

		The contents of the buffer when it is passed to the the AudioSource::getNextAudioBlock()
		method can be treated as the input if the source is performing some kind of filter operation,
		but should be cleared if this is not the case - the clearActiveBufferRegion() is
		a handy way of doing this.

		The number of channels in the buffer could be anything, so the AudioSource
		must cope with this in whatever way is appropriate for its function.
	*/
	AudioSampleBuffer* buffer;

	/** The first sample in the buffer from which the callback is expected
		to write data. */
	int startSample;

	/** The number of samples in the buffer which the callback is expected to
		fill with data. */
	int numSamples;

	/** Convenient method to clear the buffer if the source is not producing any data. */
	void clearActiveBufferRegion() const
	{
		if (buffer != nullptr)
			buffer->clear (startSample, numSamples);
	}
};

/**
	Base class for objects that can produce a continuous stream of audio.

	An AudioSource has two states: 'prepared' and 'unprepared'.

	When a source needs to be played, it is first put into a 'prepared' state by a call to
	prepareToPlay(), and then repeated calls will be made to its getNextAudioBlock() method to
	process the audio data.

	Once playback has finished, the releaseResources() method is called to put the stream
	back into an 'unprepared' state.

	@see AudioFormatReaderSource, ResamplingAudioSource
*/
class JUCE_API  AudioSource
{
protected:

	/** Creates an AudioSource. */
	AudioSource() noexcept      {}

public:
	/** Destructor. */
	virtual ~AudioSource()      {}

	/** Tells the source to prepare for playing.

		An AudioSource has two states: prepared and unprepared.

		The prepareToPlay() method is guaranteed to be called at least once on an 'unpreprared'
		source to put it into a 'prepared' state before any calls will be made to getNextAudioBlock().
		This callback allows the source to initialise any resources it might need when playing.

		Once playback has finished, the releaseResources() method is called to put the stream
		back into an 'unprepared' state.

		Note that this method could be called more than once in succession without
		a matching call to releaseResources(), so make sure your code is robust and
		can handle that kind of situation.

		@param samplesPerBlockExpected  the number of samples that the source
										will be expected to supply each time its
										getNextAudioBlock() method is called. This
										number may vary slightly, because it will be dependent
										on audio hardware callbacks, and these aren't
										guaranteed to always use a constant block size, so
										the source should be able to cope with small variations.
		@param sampleRate               the sample rate that the output will be used at - this
										is needed by sources such as tone generators.
		@see releaseResources, getNextAudioBlock
	*/
	virtual void prepareToPlay (int samplesPerBlockExpected,
								double sampleRate) = 0;

	/** Allows the source to release anything it no longer needs after playback has stopped.

		This will be called when the source is no longer going to have its getNextAudioBlock()
		method called, so it should release any spare memory, etc. that it might have
		allocated during the prepareToPlay() call.

		Note that there's no guarantee that prepareToPlay() will actually have been called before
		releaseResources(), and it may be called more than once in succession, so make sure your
		code is robust and doesn't make any assumptions about when it will be called.

		@see prepareToPlay, getNextAudioBlock
	*/
	virtual void releaseResources() = 0;

	/** Called repeatedly to fetch subsequent blocks of audio data.

		After calling the prepareToPlay() method, this callback will be made each
		time the audio playback hardware (or whatever other destination the audio
		data is going to) needs another block of data.

		It will generally be called on a high-priority system thread, or possibly even
		an interrupt, so be careful not to do too much work here, as that will cause
		audio glitches!

		@see AudioSourceChannelInfo, prepareToPlay, releaseResources
	*/
	virtual void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) = 0;
};

#endif   // __JUCE_AUDIOSOURCE_JUCEHEADER__

/*** End of inlined file: juce_AudioSource.h ***/


#endif
#ifndef __JUCE_BUFFERINGAUDIOSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_BufferingAudioSource.h ***/
#ifndef __JUCE_BUFFERINGAUDIOSOURCE_JUCEHEADER__
#define __JUCE_BUFFERINGAUDIOSOURCE_JUCEHEADER__


/*** Start of inlined file: juce_PositionableAudioSource.h ***/
#ifndef __JUCE_POSITIONABLEAUDIOSOURCE_JUCEHEADER__
#define __JUCE_POSITIONABLEAUDIOSOURCE_JUCEHEADER__

/**
	A type of AudioSource which can be repositioned.

	The basic AudioSource just streams continuously with no idea of a current
	time or length, so the PositionableAudioSource is used for a finite stream
	that has a current read position.

	@see AudioSource, AudioTransportSource
*/
class JUCE_API  PositionableAudioSource  : public AudioSource
{
protected:

	/** Creates the PositionableAudioSource. */
	PositionableAudioSource() noexcept  {}

public:
	/** Destructor */
	~PositionableAudioSource()          {}

	/** Tells the stream to move to a new position.

		Calling this indicates that the next call to AudioSource::getNextAudioBlock()
		should return samples from this position.

		Note that this may be called on a different thread to getNextAudioBlock(),
		so the subclass should make sure it's synchronised.
	*/
	virtual void setNextReadPosition (int64 newPosition) = 0;

	/** Returns the position from which the next block will be returned.

		@see setNextReadPosition
	*/
	virtual int64 getNextReadPosition() const = 0;

	/** Returns the total length of the stream (in samples). */
	virtual int64 getTotalLength() const = 0;

	/** Returns true if this source is actually playing in a loop. */
	virtual bool isLooping() const = 0;

	/** Tells the source whether you'd like it to play in a loop. */
	virtual void setLooping (bool shouldLoop)       { (void) shouldLoop; }
};

#endif   // __JUCE_POSITIONABLEAUDIOSOURCE_JUCEHEADER__

/*** End of inlined file: juce_PositionableAudioSource.h ***/

/**
	An AudioSource which takes another source as input, and buffers it using a thread.

	Create this as a wrapper around another thread, and it will read-ahead with
	a background thread to smooth out playback. You can either create one of these
	directly, or use it indirectly using an AudioTransportSource.

	@see PositionableAudioSource, AudioTransportSource
*/
class JUCE_API  BufferingAudioSource  : public PositionableAudioSource,
										private TimeSliceClient
{
public:

	/** Creates a BufferingAudioSource.

		@param source                   the input source to read from
		@param backgroundThread         a background thread that will be used for the
										background read-ahead. This object must not be deleted
										until after any BufferedAudioSources that are using it
										have been deleted!
		@param deleteSourceWhenDeleted  if true, then the input source object will
										be deleted when this object is deleted
		@param numberOfSamplesToBuffer  the size of buffer to use for reading ahead
		@param numberOfChannels         the number of channels that will be played
	*/
	BufferingAudioSource (PositionableAudioSource* source,
						  TimeSliceThread& backgroundThread,
						  bool deleteSourceWhenDeleted,
						  int numberOfSamplesToBuffer,
						  int numberOfChannels = 2);

	/** Destructor.

		The input source may be deleted depending on whether the deleteSourceWhenDeleted
		flag was set in the constructor.
	*/
	~BufferingAudioSource();

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
	int64 getTotalLength() const                { return source->getTotalLength(); }

	/** Implements the PositionableAudioSource method. */
	bool isLooping() const                      { return source->isLooping(); }

private:

	OptionalScopedPointer<PositionableAudioSource> source;
	TimeSliceThread& backgroundThread;
	int numberOfSamplesToBuffer, numberOfChannels;
	AudioSampleBuffer buffer;
	CriticalSection bufferStartPosLock;
	int64 volatile bufferValidStart, bufferValidEnd, nextPlayPos;
	double volatile sampleRate;
	bool wasSourceLooping, isPrepared;

	friend class SharedBufferingAudioSourceThread;
	bool readNextBufferChunk();
	void readBufferSection (int64 start, int length, int bufferOffset);
	int useTimeSlice();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BufferingAudioSource);
};

#endif   // __JUCE_BUFFERINGAUDIOSOURCE_JUCEHEADER__

/*** End of inlined file: juce_BufferingAudioSource.h ***/


#endif
#ifndef __JUCE_CHANNELREMAPPINGAUDIOSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_ChannelRemappingAudioSource.h ***/
#ifndef __JUCE_CHANNELREMAPPINGAUDIOSOURCE_JUCEHEADER__
#define __JUCE_CHANNELREMAPPINGAUDIOSOURCE_JUCEHEADER__

/**
	An AudioSource that takes the audio from another source, and re-maps its
	input and output channels to a different arrangement.

	You can use this to increase or decrease the number of channels that an
	audio source uses, or to re-order those channels.

	Call the reset() method before using it to set up a default mapping, and then
	the setInputChannelMapping() and setOutputChannelMapping() methods to
	create an appropriate mapping, otherwise no channels will be connected and
	it'll produce silence.

	@see AudioSource
*/
class ChannelRemappingAudioSource  : public AudioSource
{
public:

	/** Creates a remapping source that will pass on audio from the given input.

		@param source       the input source to use. Make sure that this doesn't
							get deleted before the ChannelRemappingAudioSource object
		@param deleteSourceWhenDeleted  if true, the input source will be deleted
							when this object is deleted, if false, the caller is
							responsible for its deletion
	*/
	ChannelRemappingAudioSource (AudioSource* source,
								 bool deleteSourceWhenDeleted);

	/** Destructor. */
	~ChannelRemappingAudioSource();

	/** Specifies a number of channels that this audio source must produce from its
		getNextAudioBlock() callback.
	*/
	void setNumberOfChannelsToProduce (int requiredNumberOfChannels);

	/** Clears any mapped channels.

		After this, no channels are mapped, so this object will produce silence. Create
		some mappings with setInputChannelMapping() and setOutputChannelMapping().
	*/
	void clearAllMappings();

	/** Creates an input channel mapping.

		When the getNextAudioBlock() method is called, the data in channel sourceChannelIndex of the incoming
		data will be sent to destChannelIndex of our input source.

		@param destChannelIndex     the index of an input channel in our input audio source (i.e. the
									source specified when this object was created).
		@param sourceChannelIndex   the index of the input channel in the incoming audio data buffer
									during our getNextAudioBlock() callback
	*/
	void setInputChannelMapping (int destChannelIndex,
								 int sourceChannelIndex);

	/** Creates an output channel mapping.

		When the getNextAudioBlock() method is called, the data returned in channel sourceChannelIndex by
		our input audio source will be copied to channel destChannelIndex of the final buffer.

		@param sourceChannelIndex   the index of an output channel coming from our input audio source
									(i.e. the source specified when this object was created).
		@param destChannelIndex     the index of the output channel in the incoming audio data buffer
									during our getNextAudioBlock() callback
	*/
	void setOutputChannelMapping (int sourceChannelIndex,
								  int destChannelIndex);

	/** Returns the channel from our input that will be sent to channel inputChannelIndex of
		our input audio source.
	*/
	int getRemappedInputChannel (int inputChannelIndex) const;

	/** Returns the output channel to which channel outputChannelIndex of our input audio
		source will be sent to.
	*/
	int getRemappedOutputChannel (int outputChannelIndex) const;

	/** Returns an XML object to encapsulate the state of the mappings.

		@see restoreFromXml
	*/
	XmlElement* createXml() const;

	/** Restores the mappings from an XML object created by createXML().

		@see createXml
	*/
	void restoreFromXml (const XmlElement& e);

	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
	void releaseResources();
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

private:

	OptionalScopedPointer<AudioSource> source;
	Array <int> remappedInputs, remappedOutputs;
	int requiredNumberOfChannels;

	AudioSampleBuffer buffer;
	AudioSourceChannelInfo remappedInfo;

	CriticalSection lock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelRemappingAudioSource);
};

#endif   // __JUCE_CHANNELREMAPPINGAUDIOSOURCE_JUCEHEADER__

/*** End of inlined file: juce_ChannelRemappingAudioSource.h ***/


#endif
#ifndef __JUCE_IIRFILTERAUDIOSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_IIRFilterAudioSource.h ***/
#ifndef __JUCE_IIRFILTERAUDIOSOURCE_JUCEHEADER__
#define __JUCE_IIRFILTERAUDIOSOURCE_JUCEHEADER__

/**
	An AudioSource that performs an IIR filter on another source.
*/
class JUCE_API  IIRFilterAudioSource  : public AudioSource
{
public:

	/** Creates a IIRFilterAudioSource for a given input source.

		@param inputSource              the input source to read from - this must not be null
		@param deleteInputWhenDeleted   if true, the input source will be deleted when
										this object is deleted
	*/
	IIRFilterAudioSource (AudioSource* inputSource,
						  bool deleteInputWhenDeleted);

	/** Destructor. */
	~IIRFilterAudioSource();

	/** Changes the filter to use the same parameters as the one being passed in. */
	void setFilterParameters (const IIRFilter& newSettings);

	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
	void releaseResources();
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

private:

	OptionalScopedPointer<AudioSource> input;
	OwnedArray <IIRFilter> iirFilters;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIRFilterAudioSource);
};

#endif   // __JUCE_IIRFILTERAUDIOSOURCE_JUCEHEADER__

/*** End of inlined file: juce_IIRFilterAudioSource.h ***/


#endif
#ifndef __JUCE_MIXERAUDIOSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_MixerAudioSource.h ***/
#ifndef __JUCE_MIXERAUDIOSOURCE_JUCEHEADER__
#define __JUCE_MIXERAUDIOSOURCE_JUCEHEADER__

/**
	An AudioSource that mixes together the output of a set of other AudioSources.

	Input sources can be added and removed while the mixer is running as long as their
	prepareToPlay() and releaseResources() methods are called before and after adding
	them to the mixer.
*/
class JUCE_API  MixerAudioSource  : public AudioSource
{
public:

	/** Creates a MixerAudioSource. */
	MixerAudioSource();

	/** Destructor. */
	~MixerAudioSource();

	/** Adds an input source to the mixer.

		If the mixer is running you'll need to make sure that the input source
		is ready to play by calling its prepareToPlay() method before adding it.
		If the mixer is stopped, then its input sources will be automatically
		prepared when the mixer's prepareToPlay() method is called.

		@param newInput             the source to add to the mixer
		@param deleteWhenRemoved    if true, then this source will be deleted when
									no longer needed by the mixer.
	*/
	void addInputSource (AudioSource* newInput, bool deleteWhenRemoved);

	/** Removes an input source.
		If the source was added by calling addInputSource() with the deleteWhenRemoved
		flag set, it will be deleted by this method.
	*/
	void removeInputSource (AudioSource* input);

	/** Removes all the input sources.
		Any sources which were added by calling addInputSource() with the deleteWhenRemoved
		flag set will be deleted by this method.
	*/
	void removeAllInputs();

	/** Implementation of the AudioSource method.
		This will call prepareToPlay() on all its input sources.
	*/
	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);

	/** Implementation of the AudioSource method.
		This will call releaseResources() on all its input sources.
	*/
	void releaseResources();

	/** Implementation of the AudioSource method. */
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

private:

	Array <AudioSource*> inputs;
	BigInteger inputsToDelete;
	CriticalSection lock;
	AudioSampleBuffer tempBuffer;
	double currentSampleRate;
	int bufferSizeExpected;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerAudioSource);
};

#endif   // __JUCE_MIXERAUDIOSOURCE_JUCEHEADER__

/*** End of inlined file: juce_MixerAudioSource.h ***/


#endif
#ifndef __JUCE_POSITIONABLEAUDIOSOURCE_JUCEHEADER__

#endif
#ifndef __JUCE_RESAMPLINGAUDIOSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_ResamplingAudioSource.h ***/
#ifndef __JUCE_RESAMPLINGAUDIOSOURCE_JUCEHEADER__
#define __JUCE_RESAMPLINGAUDIOSOURCE_JUCEHEADER__

/**
	A type of AudioSource that takes an input source and changes its sample rate.

	@see AudioSource
*/
class JUCE_API  ResamplingAudioSource  : public AudioSource
{
public:

	/** Creates a ResamplingAudioSource for a given input source.

		@param inputSource              the input source to read from
		@param deleteInputWhenDeleted   if true, the input source will be deleted when
										this object is deleted
		@param numChannels              the number of channels to process
	*/
	ResamplingAudioSource (AudioSource* inputSource,
						   bool deleteInputWhenDeleted,
						   int numChannels = 2);

	/** Destructor. */
	~ResamplingAudioSource();

	/** Changes the resampling ratio.

		(This value can be changed at any time, even while the source is running).

		@param samplesInPerOutputSample     if set to 1.0, the input is passed through; higher
											values will speed it up; lower values will slow it
											down. The ratio must be greater than 0
	*/
	void setResamplingRatio (double samplesInPerOutputSample);

	/** Returns the current resampling ratio.

		This is the value that was set by setResamplingRatio().
	*/
	double getResamplingRatio() const noexcept                  { return ratio; }

	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
	void releaseResources();
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

private:

	OptionalScopedPointer<AudioSource> input;
	double ratio, lastRatio;
	AudioSampleBuffer buffer;
	int bufferPos, sampsInBuffer;
	double subSampleOffset;
	double coefficients[6];
	SpinLock ratioLock;
	const int numChannels;
	HeapBlock<float*> destBuffers, srcBuffers;

	void setFilterCoefficients (double c1, double c2, double c3, double c4, double c5, double c6);
	void createLowPass (double proportionalRate);

	struct FilterState
	{
		double x1, x2, y1, y2;
	};

	HeapBlock<FilterState> filterStates;
	void resetFilters();

	void applyFilter (float* samples, int num, FilterState& fs);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResamplingAudioSource);
};

#endif   // __JUCE_RESAMPLINGAUDIOSOURCE_JUCEHEADER__

/*** End of inlined file: juce_ResamplingAudioSource.h ***/


#endif
#ifndef __JUCE_REVERBAUDIOSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_ReverbAudioSource.h ***/
#ifndef __JUCE_REVERBAUDIOSOURCE_JUCEHEADER__
#define __JUCE_REVERBAUDIOSOURCE_JUCEHEADER__

/**
	An AudioSource that uses the Reverb class to apply a reverb to another AudioSource.

	@see Reverb
*/
class JUCE_API  ReverbAudioSource   : public AudioSource
{
public:
	/** Creates a ReverbAudioSource to process a given input source.

		@param inputSource              the input source to read from - this must not be null
		@param deleteInputWhenDeleted   if true, the input source will be deleted when
										this object is deleted
	*/
	ReverbAudioSource (AudioSource* inputSource,
					   bool deleteInputWhenDeleted);

	/** Destructor. */
	~ReverbAudioSource();

	/** Returns the parameters from the reverb. */
	const Reverb::Parameters& getParameters() const noexcept    { return reverb.getParameters(); }

	/** Changes the reverb's parameters. */
	void setParameters (const Reverb::Parameters& newParams);

	void setBypassed (bool isBypassed) noexcept;
	bool isBypassed() const noexcept                            { return bypass; }

	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
	void releaseResources();
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

private:

	CriticalSection lock;
	OptionalScopedPointer<AudioSource> input;
	Reverb reverb;
	volatile bool bypass;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioSource);
};

#endif   // __JUCE_REVERBAUDIOSOURCE_JUCEHEADER__

/*** End of inlined file: juce_ReverbAudioSource.h ***/


#endif
#ifndef __JUCE_TONEGENERATORAUDIOSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_ToneGeneratorAudioSource.h ***/
#ifndef __JUCE_TONEGENERATORAUDIOSOURCE_JUCEHEADER__
#define __JUCE_TONEGENERATORAUDIOSOURCE_JUCEHEADER__

/**
	A simple AudioSource that generates a sine wave.

*/
class JUCE_API  ToneGeneratorAudioSource  : public AudioSource
{
public:

	/** Creates a ToneGeneratorAudioSource. */
	ToneGeneratorAudioSource();

	/** Destructor. */
	~ToneGeneratorAudioSource();

	/** Sets the signal's amplitude. */
	void setAmplitude (float newAmplitude);

	/** Sets the signal's frequency. */
	void setFrequency (double newFrequencyHz);

	/** Implementation of the AudioSource method. */
	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);

	/** Implementation of the AudioSource method. */
	void releaseResources();

	/** Implementation of the AudioSource method. */
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

private:

	double frequency, sampleRate;
	double currentPhase, phasePerSample;
	float amplitude;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToneGeneratorAudioSource);
};

#endif   // __JUCE_TONEGENERATORAUDIOSOURCE_JUCEHEADER__

/*** End of inlined file: juce_ToneGeneratorAudioSource.h ***/


#endif
#ifndef __JUCE_SYNTHESISER_JUCEHEADER__

/*** Start of inlined file: juce_Synthesiser.h ***/
#ifndef __JUCE_SYNTHESISER_JUCEHEADER__
#define __JUCE_SYNTHESISER_JUCEHEADER__

/**
	Describes one of the sounds that a Synthesiser can play.

	A synthesiser can contain one or more sounds, and a sound can choose which
	midi notes and channels can trigger it.

	The SynthesiserSound is a passive class that just describes what the sound is -
	the actual audio rendering for a sound is done by a SynthesiserVoice. This allows
	more than one SynthesiserVoice to play the same sound at the same time.

	@see Synthesiser, SynthesiserVoice
*/
class JUCE_API  SynthesiserSound    : public ReferenceCountedObject
{
protected:

	SynthesiserSound();

public:
	/** Destructor. */
	virtual ~SynthesiserSound();

	/** Returns true if this sound should be played when a given midi note is pressed.

		The Synthesiser will use this information when deciding which sounds to trigger
		for a given note.
	*/
	virtual bool appliesToNote (const int midiNoteNumber) = 0;

	/** Returns true if the sound should be triggered by midi events on a given channel.

		The Synthesiser will use this information when deciding which sounds to trigger
		for a given note.
	*/
	virtual bool appliesToChannel (const int midiChannel) = 0;

	/**
	*/
	typedef ReferenceCountedObjectPtr <SynthesiserSound> Ptr;

private:

	JUCE_LEAK_DETECTOR (SynthesiserSound);
};

/**
	Represents a voice that a Synthesiser can use to play a SynthesiserSound.

	A voice plays a single sound at a time, and a synthesiser holds an array of
	voices so that it can play polyphonically.

	@see Synthesiser, SynthesiserSound
*/
class JUCE_API  SynthesiserVoice
{
public:

	/** Creates a voice. */
	SynthesiserVoice();

	/** Destructor. */
	virtual ~SynthesiserVoice();

	/** Returns the midi note that this voice is currently playing.

		Returns a value less than 0 if no note is playing.
	*/
	int getCurrentlyPlayingNote() const                               { return currentlyPlayingNote; }

	/** Returns the sound that this voice is currently playing.

		Returns 0 if it's not playing.
	*/
	SynthesiserSound::Ptr getCurrentlyPlayingSound() const            { return currentlyPlayingSound; }

	/** Must return true if this voice object is capable of playing the given sound.

		If there are different classes of sound, and different classes of voice, a voice can
		choose which ones it wants to take on.

		A typical implementation of this method may just return true if there's only one type
		of voice and sound, or it might check the type of the sound object passed-in and
		see if it's one that it understands.
	*/
	virtual bool canPlaySound (SynthesiserSound* sound) = 0;

	/** Called to start a new note.

		This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void startNote (const int midiNoteNumber,
							const float velocity,
							SynthesiserSound* sound,
							const int currentPitchWheelPosition) = 0;

	/** Called to stop a note.

		This will be called during the rendering callback, so must be fast and thread-safe.

		If allowTailOff is false or the voice doesn't want to tail-off, then it must stop all
		sound immediately, and must call clearCurrentNote() to reset the state of this voice
		and allow the synth to reassign it another sound.

		If allowTailOff is true and the voice decides to do a tail-off, then it's allowed to
		begin fading out its sound, and it can stop playing until it's finished. As soon as it
		finishes playing (during the rendering callback), it must make sure that it calls
		clearCurrentNote().
	*/
	virtual void stopNote (const bool allowTailOff) = 0;

	/** Called to let the voice know that the pitch wheel has been moved.

		This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void pitchWheelMoved (const int newValue) = 0;

	/** Called to let the voice know that a midi controller has been moved.

		This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void controllerMoved (const int controllerNumber,
								  const int newValue) = 0;

	/** Renders the next block of data for this voice.

		The output audio data must be added to the current contents of the buffer provided.
		Only the region of the buffer between startSample and (startSample + numSamples)
		should be altered by this method.

		If the voice is currently silent, it should just return without doing anything.

		If the sound that the voice is playing finishes during the course of this rendered
		block, it must call clearCurrentNote(), to tell the synthesiser that it has finished.

		The size of the blocks that are rendered can change each time it is called, and may
		involve rendering as little as 1 sample at a time. In between rendering callbacks,
		the voice's methods will be called to tell it about note and controller events.
	*/
	virtual void renderNextBlock (AudioSampleBuffer& outputBuffer,
								  int startSample,
								  int numSamples) = 0;

	/** Returns true if the voice is currently playing a sound which is mapped to the given
		midi channel.

		If it's not currently playing, this will return false.
	*/
	bool isPlayingChannel (int midiChannel) const;

	/** Changes the voice's reference sample rate.

		The rate is set so that subclasses know the output rate and can set their pitch
		accordingly.

		This method is called by the synth, and subclasses can access the current rate with
		the currentSampleRate member.
	*/
	void setCurrentPlaybackSampleRate (double newRate);

protected:

	/** Returns the current target sample rate at which rendering is being done.

		This is available for subclasses so they can pitch things correctly.
	*/
	double getSampleRate() const                                { return currentSampleRate; }

	/** Resets the state of this voice after a sound has finished playing.

		The subclass must call this when it finishes playing a note and becomes available
		to play new ones.

		It must either call it in the stopNote() method, or if the voice is tailing off,
		then it should call it later during the renderNextBlock method, as soon as it
		finishes its tail-off.

		It can also be called at any time during the render callback if the sound happens
		to have finished, e.g. if it's playing a sample and the sample finishes.
	*/
	void clearCurrentNote();

private:

	friend class Synthesiser;

	double currentSampleRate;
	int currentlyPlayingNote;
	uint32 noteOnTime;
	SynthesiserSound::Ptr currentlyPlayingSound;
	bool keyIsDown; // the voice may still be playing when the key is not down (i.e. sustain pedal)
	bool sostenutoPedalDown;

	JUCE_LEAK_DETECTOR (SynthesiserVoice);
};

/**
	Base class for a musical device that can play sounds.

	To create a synthesiser, you'll need to create a subclass of SynthesiserSound
	to describe each sound available to your synth, and a subclass of SynthesiserVoice
	which can play back one of these sounds.

	Then you can use the addVoice() and addSound() methods to give the synthesiser a
	set of sounds, and a set of voices it can use to play them. If you only give it
	one voice it will be monophonic - the more voices it has, the more polyphony it'll
	have available.

	Then repeatedly call the renderNextBlock() method to produce the audio. Any midi
	events that go in will be scanned for note on/off messages, and these are used to
	start and stop the voices playing the appropriate sounds.

	While it's playing, you can also cause notes to be triggered by calling the noteOn(),
	noteOff() and other controller methods.

	Before rendering, be sure to call the setCurrentPlaybackSampleRate() to tell it
	what the target playback rate is. This value is passed on to the voices so that
	they can pitch their output correctly.
*/
class JUCE_API  Synthesiser
{
public:

	/** Creates a new synthesiser.

		You'll need to add some sounds and voices before it'll make any sound..
	*/
	Synthesiser();

	/** Destructor. */
	virtual ~Synthesiser();

	/** Deletes all voices. */
	void clearVoices();

	/** Returns the number of voices that have been added. */
	int getNumVoices() const                                        { return voices.size(); }

	/** Returns one of the voices that have been added. */
	SynthesiserVoice* getVoice (int index) const;

	/** Adds a new voice to the synth.

		All the voices should be the same class of object and are treated equally.

		The object passed in will be managed by the synthesiser, which will delete
		it later on when no longer needed. The caller should not retain a pointer to the
		voice.
	*/
	void addVoice (SynthesiserVoice* newVoice);

	/** Deletes one of the voices. */
	void removeVoice (int index);

	/** Deletes all sounds. */
	void clearSounds();

	/** Returns the number of sounds that have been added to the synth. */
	int getNumSounds() const                                        { return sounds.size(); }

	/** Returns one of the sounds. */
	SynthesiserSound* getSound (int index) const                    { return sounds [index]; }

	/** Adds a new sound to the synthesiser.

		The object passed in is reference counted, so will be deleted when it is removed
		from the synthesiser, and when no voices are still using it.
	*/
	void addSound (const SynthesiserSound::Ptr& newSound);

	/** Removes and deletes one of the sounds. */
	void removeSound (int index);

	/** If set to true, then the synth will try to take over an existing voice if
		it runs out and needs to play another note.

		The value of this boolean is passed into findFreeVoice(), so the result will
		depend on the implementation of this method.
	*/
	void setNoteStealingEnabled (bool shouldStealNotes);

	/** Returns true if note-stealing is enabled.
		@see setNoteStealingEnabled
	*/
	bool isNoteStealingEnabled() const                              { return shouldStealNotes; }

	/** Triggers a note-on event.

		The default method here will find all the sounds that want to be triggered by
		this note/channel. For each sound, it'll try to find a free voice, and use the
		voice to start playing the sound.

		Subclasses might want to override this if they need a more complex algorithm.

		This method will be called automatically according to the midi data passed into
		renderNextBlock(), but may be called explicitly too.

		The midiChannel parameter is the channel, between 1 and 16 inclusive.
	*/
	virtual void noteOn (int midiChannel,
						 int midiNoteNumber,
						 float velocity);

	/** Triggers a note-off event.

		This will turn off any voices that are playing a sound for the given note/channel.

		If allowTailOff is true, the voices will be allowed to fade out the notes gracefully
		(if they can do). If this is false, the notes will all be cut off immediately.

		This method will be called automatically according to the midi data passed into
		renderNextBlock(), but may be called explicitly too.

		The midiChannel parameter is the channel, between 1 and 16 inclusive.
	*/
	virtual void noteOff (int midiChannel,
						  int midiNoteNumber,
						  bool allowTailOff);

	/** Turns off all notes.

		This will turn off any voices that are playing a sound on the given midi channel.

		If midiChannel is 0 or less, then all voices will be turned off, regardless of
		which channel they're playing. Otherwise it represents a valid midi channel, from
		1 to 16 inclusive.

		If allowTailOff is true, the voices will be allowed to fade out the notes gracefully
		(if they can do). If this is false, the notes will all be cut off immediately.

		This method will be called automatically according to the midi data passed into
		renderNextBlock(), but may be called explicitly too.
	*/
	virtual void allNotesOff (int midiChannel,
							  bool allowTailOff);

	/** Sends a pitch-wheel message.

		This will send a pitch-wheel message to any voices that are playing sounds on
		the given midi channel.

		This method will be called automatically according to the midi data passed into
		renderNextBlock(), but may be called explicitly too.

		@param midiChannel          the midi channel, from 1 to 16 inclusive
		@param wheelValue           the wheel position, from 0 to 0x3fff, as returned by MidiMessage::getPitchWheelValue()
	*/
	virtual void handlePitchWheel (int midiChannel,
								   int wheelValue);

	/** Sends a midi controller message.

		This will send a midi controller message to any voices that are playing sounds on
		the given midi channel.

		This method will be called automatically according to the midi data passed into
		renderNextBlock(), but may be called explicitly too.

		@param midiChannel          the midi channel, from 1 to 16 inclusive
		@param controllerNumber     the midi controller type, as returned by MidiMessage::getControllerNumber()
		@param controllerValue      the midi controller value, between 0 and 127, as returned by MidiMessage::getControllerValue()
	*/
	virtual void handleController (int midiChannel,
								   int controllerNumber,
								   int controllerValue);

	virtual void handleSustainPedal (int midiChannel, bool isDown);
	virtual void handleSostenutoPedal (int midiChannel, bool isDown);
	virtual void handleSoftPedal (int midiChannel, bool isDown);

	/** Tells the synthesiser what the sample rate is for the audio it's being used to
		render.

		This value is propagated to the voices so that they can use it to render the correct
		pitches.
	*/
	void setCurrentPlaybackSampleRate (double sampleRate);

	/** Creates the next block of audio output.

		This will process the next numSamples of data from all the voices, and add that output
		to the audio block supplied, starting from the offset specified. Note that the
		data will be added to the current contents of the buffer, so you should clear it
		before calling this method if necessary.

		The midi events in the inputMidi buffer are parsed for note and controller events,
		and these are used to trigger the voices. Note that the startSample offset applies
		both to the audio output buffer and the midi input buffer, so any midi events
		with timestamps outside the specified region will be ignored.
	*/
	void renderNextBlock (AudioSampleBuffer& outputAudio,
						  const MidiBuffer& inputMidi,
						  int startSample,
						  int numSamples);

protected:

	/** This is used to control access to the rendering callback and the note trigger methods. */
	CriticalSection lock;

	OwnedArray <SynthesiserVoice> voices;
	ReferenceCountedArray <SynthesiserSound> sounds;

	/** The last pitch-wheel values for each midi channel. */
	int lastPitchWheelValues [16];

	/** Searches through the voices to find one that's not currently playing, and which
		can play the given sound.

		Returns 0 if all voices are busy and stealing isn't enabled.

		This can be overridden to implement custom voice-stealing algorithms.
	*/
	virtual SynthesiserVoice* findFreeVoice (SynthesiserSound* soundToPlay,
											 const bool stealIfNoneAvailable) const;

	/** Starts a specified voice playing a particular sound.

		You'll probably never need to call this, it's used internally by noteOn(), but
		may be needed by subclasses for custom behaviours.
	*/
	void startVoice (SynthesiserVoice* voice,
					 SynthesiserSound* sound,
					 int midiChannel,
					 int midiNoteNumber,
					 float velocity);

private:

	double sampleRate;
	uint32 lastNoteOnCounter;
	bool shouldStealNotes;
	BigInteger sustainPedalsDown;

	void handleMidiEvent (const MidiMessage& m);
	void stopVoice (SynthesiserVoice* voice, bool allowTailOff);

   #if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// Note the new parameters for this method.
	virtual int findFreeVoice (const bool) const { return 0; }
   #endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synthesiser);
};

#endif   // __JUCE_SYNTHESISER_JUCEHEADER__

/*** End of inlined file: juce_Synthesiser.h ***/


#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_AUDIO_BASICS_JUCEHEADER__

/*** End of inlined file: juce_audio_basics.h ***/

