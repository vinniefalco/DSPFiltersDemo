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

/*** Start of inlined file: juce_audio_basics.cpp ***/
#if defined (__JUCE_AUDIO_BASICS_JUCEHEADER__) && ! JUCE_AMALGAMATED_INCLUDE
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

#include "juce_audio_basics_amalgam.h"

namespace juce
{

// START_AUTOINCLUDE buffers/*.cpp, effects/*.cpp, midi/*.cpp, sources/*.cpp, synthesisers/*.cpp

/*** Start of inlined file: juce_AudioDataConverters.cpp ***/
void AudioDataConverters::convertFloatToInt16LE (const float* source, void* dest, int numSamples, const int destBytesPerSample)
{
	const double maxVal = (double) 0x7fff;
	char* intData = static_cast <char*> (dest);

	if (dest != (void*) source || destBytesPerSample <= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			*(uint16*) intData = ByteOrder::swapIfBigEndian ((uint16) (short) roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])));
			intData += destBytesPerSample;
		}
	}
	else
	{
		intData += destBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= destBytesPerSample;
			*(uint16*) intData = ByteOrder::swapIfBigEndian ((uint16) (short) roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])));
		}
	}
}

void AudioDataConverters::convertFloatToInt16BE (const float* source, void* dest, int numSamples, const int destBytesPerSample)
{
	const double maxVal = (double) 0x7fff;
	char* intData = static_cast <char*> (dest);

	if (dest != (void*) source || destBytesPerSample <= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			*(uint16*) intData = ByteOrder::swapIfLittleEndian ((uint16) (short) roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])));
			intData += destBytesPerSample;
		}
	}
	else
	{
		intData += destBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= destBytesPerSample;
			*(uint16*) intData = ByteOrder::swapIfLittleEndian ((uint16) (short) roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])));
		}
	}
}

void AudioDataConverters::convertFloatToInt24LE (const float* source, void* dest, int numSamples, const int destBytesPerSample)
{
	const double maxVal = (double) 0x7fffff;
	char* intData = static_cast <char*> (dest);

	if (dest != (void*) source || destBytesPerSample <= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			ByteOrder::littleEndian24BitToChars (roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])), intData);
			intData += destBytesPerSample;
		}
	}
	else
	{
		intData += destBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= destBytesPerSample;
			ByteOrder::littleEndian24BitToChars (roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])), intData);
		}
	}
}

void AudioDataConverters::convertFloatToInt24BE (const float* source, void* dest, int numSamples, const int destBytesPerSample)
{
	const double maxVal = (double) 0x7fffff;
	char* intData = static_cast <char*> (dest);

	if (dest != (void*) source || destBytesPerSample <= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			ByteOrder::bigEndian24BitToChars (roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])), intData);
			intData += destBytesPerSample;
		}
	}
	else
	{
		intData += destBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= destBytesPerSample;
			ByteOrder::bigEndian24BitToChars (roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])), intData);
		}
	}
}

void AudioDataConverters::convertFloatToInt32LE (const float* source, void* dest, int numSamples, const int destBytesPerSample)
{
	const double maxVal = (double) 0x7fffffff;
	char* intData = static_cast <char*> (dest);

	if (dest != (void*) source || destBytesPerSample <= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			*(uint32*)intData = ByteOrder::swapIfBigEndian ((uint32) roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])));
			intData += destBytesPerSample;
		}
	}
	else
	{
		intData += destBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= destBytesPerSample;
			*(uint32*)intData = ByteOrder::swapIfBigEndian ((uint32) roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])));
		}
	}
}

void AudioDataConverters::convertFloatToInt32BE (const float* source, void* dest, int numSamples, const int destBytesPerSample)
{
	const double maxVal = (double) 0x7fffffff;
	char* intData = static_cast <char*> (dest);

	if (dest != (void*) source || destBytesPerSample <= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			*(uint32*)intData = ByteOrder::swapIfLittleEndian ((uint32) roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])));
			intData += destBytesPerSample;
		}
	}
	else
	{
		intData += destBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= destBytesPerSample;
			*(uint32*)intData = ByteOrder::swapIfLittleEndian ((uint32) roundToInt (jlimit (-maxVal, maxVal, maxVal * source[i])));
		}
	}
}

void AudioDataConverters::convertFloatToFloat32LE (const float* source, void* dest, int numSamples, const int destBytesPerSample)
{
	jassert (dest != (void*) source || destBytesPerSample <= 4); // This op can't be performed on in-place data!

	char* d = static_cast <char*> (dest);

	for (int i = 0; i < numSamples; ++i)
	{
		*(float*) d = source[i];

	   #if JUCE_BIG_ENDIAN
		*(uint32*) d = ByteOrder::swap (*(uint32*) d);
	   #endif

		d += destBytesPerSample;
	}
}

void AudioDataConverters::convertFloatToFloat32BE (const float* source, void* dest, int numSamples, const int destBytesPerSample)
{
	jassert (dest != (void*) source || destBytesPerSample <= 4); // This op can't be performed on in-place data!

	char* d = static_cast <char*> (dest);

	for (int i = 0; i < numSamples; ++i)
	{
		*(float*) d = source[i];

	   #if JUCE_LITTLE_ENDIAN
		*(uint32*) d = ByteOrder::swap (*(uint32*) d);
	   #endif

		d += destBytesPerSample;
	}
}

void AudioDataConverters::convertInt16LEToFloat (const void* const source, float* const dest, int numSamples, const int srcBytesPerSample)
{
	const float scale = 1.0f / 0x7fff;
	const char* intData = static_cast <const char*> (source);

	if (source != (void*) dest || srcBytesPerSample >= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			dest[i] = scale * (short) ByteOrder::swapIfBigEndian (*(uint16*)intData);
			intData += srcBytesPerSample;
		}
	}
	else
	{
		intData += srcBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= srcBytesPerSample;
			dest[i] = scale * (short) ByteOrder::swapIfBigEndian (*(uint16*)intData);
		}
	}
}

void AudioDataConverters::convertInt16BEToFloat (const void* const source, float* const dest, int numSamples, const int srcBytesPerSample)
{
	const float scale = 1.0f / 0x7fff;
	const char* intData = static_cast <const char*> (source);

	if (source != (void*) dest || srcBytesPerSample >= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			dest[i] = scale * (short) ByteOrder::swapIfLittleEndian (*(uint16*)intData);
			intData += srcBytesPerSample;
		}
	}
	else
	{
		intData += srcBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= srcBytesPerSample;
			dest[i] = scale * (short) ByteOrder::swapIfLittleEndian (*(uint16*)intData);
		}
	}
}

void AudioDataConverters::convertInt24LEToFloat (const void* const source, float* const dest, int numSamples, const int srcBytesPerSample)
{
	const float scale = 1.0f / 0x7fffff;
	const char* intData = static_cast <const char*> (source);

	if (source != (void*) dest || srcBytesPerSample >= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			dest[i] = scale * (short) ByteOrder::littleEndian24Bit (intData);
			intData += srcBytesPerSample;
		}
	}
	else
	{
		intData += srcBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= srcBytesPerSample;
			dest[i] = scale * (short) ByteOrder::littleEndian24Bit (intData);
		}
	}
}

void AudioDataConverters::convertInt24BEToFloat (const void* const source, float* const dest, int numSamples, const int srcBytesPerSample)
{
	const float scale = 1.0f / 0x7fffff;
	const char* intData = static_cast <const char*> (source);

	if (source != (void*) dest || srcBytesPerSample >= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			dest[i] = scale * (short) ByteOrder::bigEndian24Bit (intData);
			intData += srcBytesPerSample;
		}
	}
	else
	{
		intData += srcBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= srcBytesPerSample;
			dest[i] = scale * (short) ByteOrder::bigEndian24Bit (intData);
		}
	}
}

void AudioDataConverters::convertInt32LEToFloat (const void* const source, float* const dest, int numSamples, const int srcBytesPerSample)
{
	const float scale = 1.0f / 0x7fffffff;
	const char* intData = static_cast <const char*> (source);

	if (source != (void*) dest || srcBytesPerSample >= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			dest[i] = scale * (int) ByteOrder::swapIfBigEndian (*(uint32*) intData);
			intData += srcBytesPerSample;
		}
	}
	else
	{
		intData += srcBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= srcBytesPerSample;
			dest[i] = scale * (int) ByteOrder::swapIfBigEndian (*(uint32*) intData);
		}
	}
}

void AudioDataConverters::convertInt32BEToFloat (const void* const source, float* const dest, int numSamples, const int srcBytesPerSample)
{
	const float scale = 1.0f / 0x7fffffff;
	const char* intData = static_cast <const char*> (source);

	if (source != (void*) dest || srcBytesPerSample >= 4)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			dest[i] = scale * (int) ByteOrder::swapIfLittleEndian (*(uint32*) intData);
			intData += srcBytesPerSample;
		}
	}
	else
	{
		intData += srcBytesPerSample * numSamples;

		for (int i = numSamples; --i >= 0;)
		{
			intData -= srcBytesPerSample;
			dest[i] = scale * (int) ByteOrder::swapIfLittleEndian (*(uint32*) intData);
		}
	}
}

void AudioDataConverters::convertFloat32LEToFloat (const void* const source, float* const dest, int numSamples, const int srcBytesPerSample)
{
	const char* s = static_cast <const char*> (source);

	for (int i = 0; i < numSamples; ++i)
	{
		dest[i] = *(float*)s;

	   #if JUCE_BIG_ENDIAN
		uint32* const d = (uint32*) (dest + i);
		*d = ByteOrder::swap (*d);
	   #endif

		s += srcBytesPerSample;
	}
}

void AudioDataConverters::convertFloat32BEToFloat (const void* const source, float* const dest, int numSamples, const int srcBytesPerSample)
{
	const char* s = static_cast <const char*> (source);

	for (int i = 0; i < numSamples; ++i)
	{
		dest[i] = *(float*)s;

	   #if JUCE_LITTLE_ENDIAN
		uint32* const d = (uint32*) (dest + i);
		*d = ByteOrder::swap (*d);
	   #endif

		s += srcBytesPerSample;
	}
}

void AudioDataConverters::convertFloatToFormat (const DataFormat destFormat,
												const float* const source,
												void* const dest,
												const int numSamples)
{
	switch (destFormat)
	{
		case int16LE:       convertFloatToInt16LE   (source, dest, numSamples); break;
		case int16BE:       convertFloatToInt16BE   (source, dest, numSamples); break;
		case int24LE:       convertFloatToInt24LE   (source, dest, numSamples); break;
		case int24BE:       convertFloatToInt24BE   (source, dest, numSamples); break;
		case int32LE:       convertFloatToInt32LE   (source, dest, numSamples); break;
		case int32BE:       convertFloatToInt32BE   (source, dest, numSamples); break;
		case float32LE:     convertFloatToFloat32LE (source, dest, numSamples); break;
		case float32BE:     convertFloatToFloat32BE (source, dest, numSamples); break;
		default:            jassertfalse; break;
	}
}

void AudioDataConverters::convertFormatToFloat (const DataFormat sourceFormat,
												const void* const source,
												float* const dest,
												const int numSamples)
{
	switch (sourceFormat)
	{
		case int16LE:       convertInt16LEToFloat   (source, dest, numSamples); break;
		case int16BE:       convertInt16BEToFloat   (source, dest, numSamples); break;
		case int24LE:       convertInt24LEToFloat   (source, dest, numSamples); break;
		case int24BE:       convertInt24BEToFloat   (source, dest, numSamples); break;
		case int32LE:       convertInt32LEToFloat   (source, dest, numSamples); break;
		case int32BE:       convertInt32BEToFloat   (source, dest, numSamples); break;
		case float32LE:     convertFloat32LEToFloat (source, dest, numSamples); break;
		case float32BE:     convertFloat32BEToFloat (source, dest, numSamples); break;
		default:            jassertfalse; break;
	}
}

void AudioDataConverters::interleaveSamples (const float** const source,
											 float* const dest,
											 const int numSamples,
											 const int numChannels)
{
	for (int chan = 0; chan < numChannels; ++chan)
	{
		int i = chan;
		const float* src = source [chan];

		for (int j = 0; j < numSamples; ++j)
		{
			dest [i] = src [j];
			i += numChannels;
		}
	}
}

void AudioDataConverters::deinterleaveSamples (const float* const source,
											   float** const dest,
											   const int numSamples,
											   const int numChannels)
{
	for (int chan = 0; chan < numChannels; ++chan)
	{
		int i = chan;
		float* dst = dest [chan];

		for (int j = 0; j < numSamples; ++j)
		{
			dst [j] = source [i];
			i += numChannels;
		}
	}
}

#if JUCE_UNIT_TESTS

class AudioConversionTests  : public UnitTest
{
public:
	AudioConversionTests() : UnitTest ("Audio data conversion") {}

	template <class F1, class E1, class F2, class E2>
	struct Test5
	{
		static void test (UnitTest& unitTest)
		{
			test (unitTest, false);
			test (unitTest, true);
		}

		static void test (UnitTest& unitTest, bool inPlace)
		{
			const int numSamples = 2048;
			int32 original [numSamples], converted [numSamples], reversed [numSamples];
			Random r;

			{
				AudioData::Pointer<F1, E1, AudioData::NonInterleaved, AudioData::NonConst> d (original);
				bool clippingFailed = false;

				for (int i = 0; i < numSamples / 2; ++i)
				{
					d.setAsFloat (r.nextFloat() * 2.2f - 1.1f);

					if (! d.isFloatingPoint())
						clippingFailed = d.getAsFloat() > 1.0f || d.getAsFloat() < -1.0f || clippingFailed;

					++d;
					d.setAsInt32 (r.nextInt());
					++d;
				}

				unitTest.expect (! clippingFailed);
			}

			// convert data from the source to dest format..
			ScopedPointer<AudioData::Converter> conv (new AudioData::ConverterInstance <AudioData::Pointer<F1, E1, AudioData::NonInterleaved, AudioData::Const>,
																						AudioData::Pointer<F2, E2, AudioData::NonInterleaved, AudioData::NonConst> >());
			conv->convertSamples (inPlace ? reversed : converted, original, numSamples);

			// ..and back again..
			conv = new AudioData::ConverterInstance <AudioData::Pointer<F2, E2, AudioData::NonInterleaved, AudioData::Const>,
													 AudioData::Pointer<F1, E1, AudioData::NonInterleaved, AudioData::NonConst> >();
			if (! inPlace)
				zeromem (reversed, sizeof (reversed));

			conv->convertSamples (reversed, inPlace ? reversed : converted, numSamples);

			{
				int biggestDiff = 0;
				AudioData::Pointer<F1, E1, AudioData::NonInterleaved, AudioData::Const> d1 (original);
				AudioData::Pointer<F1, E1, AudioData::NonInterleaved, AudioData::Const> d2 (reversed);

				const int errorMargin = 2 * AudioData::Pointer<F1, E1, AudioData::NonInterleaved, AudioData::Const>::get32BitResolution()
											+ AudioData::Pointer<F2, E2, AudioData::NonInterleaved, AudioData::Const>::get32BitResolution();

				for (int i = 0; i < numSamples; ++i)
				{
					biggestDiff = jmax (biggestDiff, std::abs (d1.getAsInt32() - d2.getAsInt32()));
					++d1;
					++d2;
				}

				unitTest.expect (biggestDiff <= errorMargin);
			}
		}
	};

	template <class F1, class E1, class FormatType>
	struct Test3
	{
		static void test (UnitTest& unitTest)
		{
			Test5 <F1, E1, FormatType, AudioData::BigEndian>::test (unitTest);
			Test5 <F1, E1, FormatType, AudioData::LittleEndian>::test (unitTest);
		}
	};

	template <class FormatType, class Endianness>
	struct Test2
	{
		static void test (UnitTest& unitTest)
		{
			Test3 <FormatType, Endianness, AudioData::Int8>::test (unitTest);
			Test3 <FormatType, Endianness, AudioData::UInt8>::test (unitTest);
			Test3 <FormatType, Endianness, AudioData::Int16>::test (unitTest);
			Test3 <FormatType, Endianness, AudioData::Int24>::test (unitTest);
			Test3 <FormatType, Endianness, AudioData::Int32>::test (unitTest);
			Test3 <FormatType, Endianness, AudioData::Float32>::test (unitTest);
		}
	};

	template <class FormatType>
	struct Test1
	{
		static void test (UnitTest& unitTest)
		{
			Test2 <FormatType, AudioData::BigEndian>::test (unitTest);
			Test2 <FormatType, AudioData::LittleEndian>::test (unitTest);
		}
	};

	void runTest()
	{
		beginTest ("Round-trip conversion: Int8");
		Test1 <AudioData::Int8>::test (*this);
		beginTest ("Round-trip conversion: Int16");
		Test1 <AudioData::Int16>::test (*this);
		beginTest ("Round-trip conversion: Int24");
		Test1 <AudioData::Int24>::test (*this);
		beginTest ("Round-trip conversion: Int32");
		Test1 <AudioData::Int32>::test (*this);
		beginTest ("Round-trip conversion: Float32");
		Test1 <AudioData::Float32>::test (*this);
	}
};

static AudioConversionTests audioConversionUnitTests;

#endif

/*** End of inlined file: juce_AudioDataConverters.cpp ***/



/*** Start of inlined file: juce_AudioSampleBuffer.cpp ***/
AudioSampleBuffer::AudioSampleBuffer (const int numChannels_,
									  const int numSamples) noexcept
  : numChannels (numChannels_),
	size (numSamples)
{
	jassert (numSamples >= 0);
	jassert (numChannels_ > 0);

	allocateData();
}

AudioSampleBuffer::AudioSampleBuffer (const AudioSampleBuffer& other) noexcept
  : numChannels (other.numChannels),
	size (other.size)
{
	allocateData();
	const size_t numBytes = sizeof (float) * (size_t) size;

	for (int i = 0; i < numChannels; ++i)
		memcpy (channels[i], other.channels[i], numBytes);
}

void AudioSampleBuffer::allocateData()
{
	const size_t channelListSize = sizeof (float*) * (size_t) (numChannels + 1);
	allocatedBytes = (size_t) numChannels * (size_t) size * sizeof (float) + channelListSize + 32;
	allocatedData.malloc (allocatedBytes);
	channels = reinterpret_cast <float**> (allocatedData.getData());

	float* chan = (float*) (allocatedData + channelListSize);
	for (int i = 0; i < numChannels; ++i)
	{
		channels[i] = chan;
		chan += size;
	}

	channels [numChannels] = 0;
}

AudioSampleBuffer::AudioSampleBuffer (float** dataToReferTo,
									  const int numChannels_,
									  const int numSamples) noexcept
	: numChannels (numChannels_),
	  size (numSamples),
	  allocatedBytes (0)
{
	jassert (numChannels_ > 0);
	allocateChannels (dataToReferTo, 0);
}

AudioSampleBuffer::AudioSampleBuffer (float** dataToReferTo,
									  const int numChannels_,
									  const int startSample,
									  const int numSamples) noexcept
	: numChannels (numChannels_),
	  size (numSamples),
	  allocatedBytes (0)
{
	jassert (numChannels_ > 0);
	allocateChannels (dataToReferTo, startSample);
}

void AudioSampleBuffer::setDataToReferTo (float** dataToReferTo,
										  const int newNumChannels,
										  const int newNumSamples) noexcept
{
	jassert (newNumChannels > 0);

	allocatedBytes = 0;
	allocatedData.free();

	numChannels = newNumChannels;
	size = newNumSamples;

	allocateChannels (dataToReferTo, 0);
}

void AudioSampleBuffer::allocateChannels (float** const dataToReferTo, int offset)
{
	// (try to avoid doing a malloc here, as that'll blow up things like Pro-Tools)
	if (numChannels < (int) numElementsInArray (preallocatedChannelSpace))
	{
		channels = static_cast <float**> (preallocatedChannelSpace);
	}
	else
	{
		allocatedData.malloc ((size_t) numChannels + 1, sizeof (float*));
		channels = reinterpret_cast <float**> (allocatedData.getData());
	}

	for (int i = 0; i < numChannels; ++i)
	{
		// you have to pass in the same number of valid pointers as numChannels
		jassert (dataToReferTo[i] != nullptr);

		channels[i] = dataToReferTo[i] + offset;
	}

	channels [numChannels] = 0;
}

AudioSampleBuffer& AudioSampleBuffer::operator= (const AudioSampleBuffer& other) noexcept
{
	if (this != &other)
	{
		setSize (other.getNumChannels(), other.getNumSamples(), false, false, false);

		const size_t numBytes = sizeof (float) * (size_t) size;

		for (int i = 0; i < numChannels; ++i)
			memcpy (channels[i], other.channels[i], numBytes);
	}

	return *this;
}

AudioSampleBuffer::~AudioSampleBuffer() noexcept
{
}

void AudioSampleBuffer::setSize (const int newNumChannels,
								 const int newNumSamples,
								 const bool keepExistingContent,
								 const bool clearExtraSpace,
								 const bool avoidReallocating) noexcept
{
	jassert (newNumChannels > 0);
	jassert (newNumSamples >= 0);

	if (newNumSamples != size || newNumChannels != numChannels)
	{
		const size_t channelListSize = sizeof (float*) * (size_t) (newNumChannels + 1);
		const size_t newTotalBytes = ((size_t) newNumChannels * (size_t) newNumSamples * sizeof (float)) + channelListSize + 32;

		if (keepExistingContent)
		{
			HeapBlock <char, true> newData;
			newData.allocate (newTotalBytes, clearExtraSpace);

			const size_t numBytesToCopy = sizeof (float) * (size_t) jmin (newNumSamples, size);

			float** const newChannels = reinterpret_cast <float**> (newData.getData());
			float* newChan = reinterpret_cast <float*> (newData + channelListSize);

			for (int j = 0; j < newNumChannels; ++j)
			{
				newChannels[j] = newChan;
				newChan += newNumSamples;
			}

			const int numChansToCopy = jmin (numChannels, newNumChannels);
			for (int i = 0; i < numChansToCopy; ++i)
				memcpy (newChannels[i], channels[i], numBytesToCopy);

			allocatedData.swapWith (newData);
			allocatedBytes = newTotalBytes;
			channels = newChannels;
		}
		else
		{
			if (avoidReallocating && allocatedBytes >= newTotalBytes)
			{
				if (clearExtraSpace)
					allocatedData.clear (newTotalBytes);
			}
			else
			{
				allocatedBytes = newTotalBytes;
				allocatedData.allocate (newTotalBytes, clearExtraSpace);
				channels = reinterpret_cast <float**> (allocatedData.getData());
			}

			float* chan = reinterpret_cast <float*> (allocatedData + channelListSize);
			for (int i = 0; i < newNumChannels; ++i)
			{
				channels[i] = chan;
				chan += newNumSamples;
			}
		}

		channels [newNumChannels] = 0;
		size = newNumSamples;
		numChannels = newNumChannels;
	}
}

void AudioSampleBuffer::clear() noexcept
{
	for (int i = 0; i < numChannels; ++i)
		zeromem (channels[i], sizeof (float) * (size_t) size);
}

void AudioSampleBuffer::clear (const int startSample,
							   const int numSamples) noexcept
{
	jassert (startSample >= 0 && startSample + numSamples <= size);

	for (int i = 0; i < numChannels; ++i)
		zeromem (channels [i] + startSample, sizeof (float) * (size_t) numSamples);
}

void AudioSampleBuffer::clear (const int channel,
							   const int startSample,
							   const int numSamples) noexcept
{
	jassert (isPositiveAndBelow (channel, numChannels));
	jassert (startSample >= 0 && startSample + numSamples <= size);

	zeromem (channels [channel] + startSample, sizeof (float) * (size_t) numSamples);
}

void AudioSampleBuffer::applyGain (const int channel,
								   const int startSample,
								   int numSamples,
								   const float gain) noexcept
{
	jassert (isPositiveAndBelow (channel, numChannels));
	jassert (startSample >= 0 && startSample + numSamples <= size);

	if (gain != 1.0f)
	{
		float* d = channels [channel] + startSample;

		if (gain == 0.0f)
		{
			zeromem (d, sizeof (float) * (size_t) numSamples);
		}
		else
		{
			while (--numSamples >= 0)
				*d++ *= gain;
		}
	}
}

void AudioSampleBuffer::applyGainRamp (const int channel,
									   const int startSample,
									   int numSamples,
									   float startGain,
									   float endGain) noexcept
{
	if (startGain == endGain)
	{
		applyGain (channel, startSample, numSamples, startGain);
	}
	else
	{
		jassert (isPositiveAndBelow (channel, numChannels));
		jassert (startSample >= 0 && startSample + numSamples <= size);

		const float increment = (endGain - startGain) / numSamples;
		float* d = channels [channel] + startSample;

		while (--numSamples >= 0)
		{
			*d++ *= startGain;
			startGain += increment;
		}
	}
}

void AudioSampleBuffer::applyGain (const int startSample,
								   const int numSamples,
								   const float gain) noexcept
{
	for (int i = 0; i < numChannels; ++i)
		applyGain (i, startSample, numSamples, gain);
}

void AudioSampleBuffer::addFrom (const int destChannel,
								 const int destStartSample,
								 const AudioSampleBuffer& source,
								 const int sourceChannel,
								 const int sourceStartSample,
								 int numSamples,
								 const float gain) noexcept
{
	jassert (&source != this || sourceChannel != destChannel);
	jassert (isPositiveAndBelow (destChannel, numChannels));
	jassert (destStartSample >= 0 && destStartSample + numSamples <= size);
	jassert (isPositiveAndBelow (sourceChannel, source.numChannels));
	jassert (sourceStartSample >= 0 && sourceStartSample + numSamples <= source.size);

	if (gain != 0.0f && numSamples > 0)
	{
		float* d = channels [destChannel] + destStartSample;
		const float* s  = source.channels [sourceChannel] + sourceStartSample;

		if (gain != 1.0f)
		{
			while (--numSamples >= 0)
				*d++ += gain * *s++;
		}
		else
		{
			while (--numSamples >= 0)
				*d++ += *s++;
		}
	}
}

void AudioSampleBuffer::addFrom (const int destChannel,
								 const int destStartSample,
								 const float* source,
								 int numSamples,
								 const float gain) noexcept
{
	jassert (isPositiveAndBelow (destChannel, numChannels));
	jassert (destStartSample >= 0 && destStartSample + numSamples <= size);
	jassert (source != nullptr);

	if (gain != 0.0f && numSamples > 0)
	{
		float* d = channels [destChannel] + destStartSample;

		if (gain != 1.0f)
		{
			while (--numSamples >= 0)
				*d++ += gain * *source++;
		}
		else
		{
			while (--numSamples >= 0)
				*d++ += *source++;
		}
	}
}

void AudioSampleBuffer::addFromWithRamp (const int destChannel,
										 const int destStartSample,
										 const float* source,
										 int numSamples,
										 float startGain,
										 const float endGain) noexcept
{
	jassert (isPositiveAndBelow (destChannel, numChannels));
	jassert (destStartSample >= 0 && destStartSample + numSamples <= size);
	jassert (source != nullptr);

	if (startGain == endGain)
	{
		addFrom (destChannel,
				 destStartSample,
				 source,
				 numSamples,
				 startGain);
	}
	else
	{
		if (numSamples > 0 && (startGain != 0.0f || endGain != 0.0f))
		{
			const float increment = (endGain - startGain) / numSamples;
			float* d = channels [destChannel] + destStartSample;

			while (--numSamples >= 0)
			{
				*d++ += startGain * *source++;
				startGain += increment;
			}
		}
	}
}

void AudioSampleBuffer::copyFrom (const int destChannel,
								  const int destStartSample,
								  const AudioSampleBuffer& source,
								  const int sourceChannel,
								  const int sourceStartSample,
								  int numSamples) noexcept
{
	jassert (&source != this || sourceChannel != destChannel);
	jassert (isPositiveAndBelow (destChannel, numChannels));
	jassert (destStartSample >= 0 && destStartSample + numSamples <= size);
	jassert (isPositiveAndBelow (sourceChannel, source.numChannels));
	jassert (sourceStartSample >= 0 && sourceStartSample + numSamples <= source.size);

	if (numSamples > 0)
	{
		memcpy (channels [destChannel] + destStartSample,
				source.channels [sourceChannel] + sourceStartSample,
				sizeof (float) * (size_t) numSamples);
	}
}

void AudioSampleBuffer::copyFrom (const int destChannel,
								  const int destStartSample,
								  const float* source,
								  int numSamples) noexcept
{
	jassert (isPositiveAndBelow (destChannel, numChannels));
	jassert (destStartSample >= 0 && destStartSample + numSamples <= size);
	jassert (source != nullptr);

	if (numSamples > 0)
	{
		memcpy (channels [destChannel] + destStartSample,
				source,
				sizeof (float) * (size_t) numSamples);
	}
}

void AudioSampleBuffer::copyFrom (const int destChannel,
								  const int destStartSample,
								  const float* source,
								  int numSamples,
								  const float gain) noexcept
{
	jassert (isPositiveAndBelow (destChannel, numChannels));
	jassert (destStartSample >= 0 && destStartSample + numSamples <= size);
	jassert (source != nullptr);

	if (numSamples > 0)
	{
		float* d = channels [destChannel] + destStartSample;

		if (gain != 1.0f)
		{
			if (gain == 0)
			{
				zeromem (d, sizeof (float) * (size_t) numSamples);
			}
			else
			{
				while (--numSamples >= 0)
					*d++ = gain * *source++;
			}
		}
		else
		{
			memcpy (d, source, sizeof (float) * (size_t) numSamples);
		}
	}
}

void AudioSampleBuffer::copyFromWithRamp (const int destChannel,
										  const int destStartSample,
										  const float* source,
										  int numSamples,
										  float startGain,
										  float endGain) noexcept
{
	jassert (isPositiveAndBelow (destChannel, numChannels));
	jassert (destStartSample >= 0 && destStartSample + numSamples <= size);
	jassert (source != nullptr);

	if (startGain == endGain)
	{
		copyFrom (destChannel,
				  destStartSample,
				  source,
				  numSamples,
				  startGain);
	}
	else
	{
		if (numSamples > 0 && (startGain != 0.0f || endGain != 0.0f))
		{
			const float increment = (endGain - startGain) / numSamples;
			float* d = channels [destChannel] + destStartSample;

			while (--numSamples >= 0)
			{
				*d++ = startGain * *source++;
				startGain += increment;
			}
		}
	}
}

void AudioSampleBuffer::findMinMax (const int channel,
									const int startSample,
									int numSamples,
									float& minVal,
									float& maxVal) const noexcept
{
	jassert (isPositiveAndBelow (channel, numChannels));
	jassert (startSample >= 0 && startSample + numSamples <= size);

	findMinAndMax (channels [channel] + startSample, numSamples, minVal, maxVal);
}

float AudioSampleBuffer::getMagnitude (const int channel,
									   const int startSample,
									   const int numSamples) const noexcept
{
	jassert (isPositiveAndBelow (channel, numChannels));
	jassert (startSample >= 0 && startSample + numSamples <= size);

	float mn, mx;
	findMinMax (channel, startSample, numSamples, mn, mx);

	return jmax (mn, -mn, mx, -mx);
}

float AudioSampleBuffer::getMagnitude (const int startSample,
									   const int numSamples) const noexcept
{
	float mag = 0.0f;

	for (int i = 0; i < numChannels; ++i)
		mag = jmax (mag, getMagnitude (i, startSample, numSamples));

	return mag;
}

float AudioSampleBuffer::getRMSLevel (const int channel,
									  const int startSample,
									  const int numSamples) const noexcept
{
	jassert (isPositiveAndBelow (channel, numChannels));
	jassert (startSample >= 0 && startSample + numSamples <= size);

	if (numSamples <= 0 || channel < 0 || channel >= numChannels)
		return 0.0f;

	const float* const data = channels [channel] + startSample;
	double sum = 0.0;

	for (int i = 0; i < numSamples; ++i)
	{
		const float sample = data [i];
		sum += sample * sample;
	}

	return (float) std::sqrt (sum / numSamples);
}

/*** End of inlined file: juce_AudioSampleBuffer.cpp ***/


/*** Start of inlined file: juce_IIRFilter.cpp ***/
#if JUCE_INTEL
 #define JUCE_SNAP_TO_ZERO(n)    if (! (n < -1.0e-8 || n > 1.0e-8)) n = 0;
#else
 #define JUCE_SNAP_TO_ZERO(n)
#endif

IIRFilter::IIRFilter()
	: active (false)
{
	reset();
}

IIRFilter::IIRFilter (const IIRFilter& other)
	: active (other.active)
{
	const ScopedLock sl (other.processLock);
	memcpy (coefficients, other.coefficients, sizeof (coefficients));
	reset();
}

IIRFilter::~IIRFilter()
{
}

void IIRFilter::reset() noexcept
{
	const ScopedLock sl (processLock);

	x1 = 0;
	x2 = 0;
	y1 = 0;
	y2 = 0;
}

float IIRFilter::processSingleSampleRaw (const float in) noexcept
{
	float out = coefficients[0] * in
				 + coefficients[1] * x1
				 + coefficients[2] * x2
				 - coefficients[4] * y1
				 - coefficients[5] * y2;

	JUCE_SNAP_TO_ZERO (out);

	x2 = x1;
	x1 = in;
	y2 = y1;
	y1 = out;

	return out;
}

void IIRFilter::processSamples (float* const samples,
								const int numSamples) noexcept
{
	const ScopedLock sl (processLock);

	if (active)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			const float in = samples[i];

			float out = coefficients[0] * in
						 + coefficients[1] * x1
						 + coefficients[2] * x2
						 - coefficients[4] * y1
						 - coefficients[5] * y2;

			JUCE_SNAP_TO_ZERO (out);

			x2 = x1;
			x1 = in;
			y2 = y1;
			y1 = out;

			samples[i] = out;
		}
	}
}

void IIRFilter::makeLowPass (const double sampleRate,
							 const double frequency) noexcept
{
	jassert (sampleRate > 0);

	const double n = 1.0 / tan (double_Pi * frequency / sampleRate);
	const double nSquared = n * n;
	const double c1 = 1.0 / (1.0 + std::sqrt (2.0) * n + nSquared);

	setCoefficients (c1,
					 c1 * 2.0f,
					 c1,
					 1.0,
					 c1 * 2.0 * (1.0 - nSquared),
					 c1 * (1.0 - std::sqrt (2.0) * n + nSquared));
}

void IIRFilter::makeHighPass (const double sampleRate,
							  const double frequency) noexcept
{
	const double n = tan (double_Pi * frequency / sampleRate);
	const double nSquared = n * n;
	const double c1 = 1.0 / (1.0 + std::sqrt (2.0) * n + nSquared);

	setCoefficients (c1,
					 c1 * -2.0f,
					 c1,
					 1.0,
					 c1 * 2.0 * (nSquared - 1.0),
					 c1 * (1.0 - std::sqrt (2.0) * n + nSquared));
}

void IIRFilter::makeLowShelf (const double sampleRate,
							  const double cutOffFrequency,
							  const double Q,
							  const float gainFactor) noexcept
{
	jassert (sampleRate > 0);
	jassert (Q > 0);

	const double A = jmax (0.0f, gainFactor);
	const double aminus1 = A - 1.0;
	const double aplus1 = A + 1.0;
	const double omega = (double_Pi * 2.0 * jmax (cutOffFrequency, 2.0)) / sampleRate;
	const double coso = std::cos (omega);
	const double beta = std::sin (omega) * std::sqrt (A) / Q;
	const double aminus1TimesCoso = aminus1 * coso;

	setCoefficients (A * (aplus1 - aminus1TimesCoso + beta),
					 A * 2.0 * (aminus1 - aplus1 * coso),
					 A * (aplus1 - aminus1TimesCoso - beta),
					 aplus1 + aminus1TimesCoso + beta,
					 -2.0 * (aminus1 + aplus1 * coso),
					 aplus1 + aminus1TimesCoso - beta);
}

void IIRFilter::makeHighShelf (const double sampleRate,
							   const double cutOffFrequency,
							   const double Q,
							   const float gainFactor) noexcept
{
	jassert (sampleRate > 0);
	jassert (Q > 0);

	const double A = jmax (0.0f, gainFactor);
	const double aminus1 = A - 1.0;
	const double aplus1 = A + 1.0;
	const double omega = (double_Pi * 2.0 * jmax (cutOffFrequency, 2.0)) / sampleRate;
	const double coso = std::cos (omega);
	const double beta = std::sin (omega) * std::sqrt (A) / Q;
	const double aminus1TimesCoso = aminus1 * coso;

	setCoefficients (A * (aplus1 + aminus1TimesCoso + beta),
					 A * -2.0 * (aminus1 + aplus1 * coso),
					 A * (aplus1 + aminus1TimesCoso - beta),
					 aplus1 - aminus1TimesCoso + beta,
					 2.0 * (aminus1 - aplus1 * coso),
					 aplus1 - aminus1TimesCoso - beta);
}

void IIRFilter::makeBandPass (const double sampleRate,
							  const double centreFrequency,
							  const double Q,
							  const float gainFactor) noexcept
{
	jassert (sampleRate > 0);
	jassert (Q > 0);

	const double A = jmax (0.0f, gainFactor);
	const double omega = (double_Pi * 2.0 * jmax (centreFrequency, 2.0)) / sampleRate;
	const double alpha = 0.5 * std::sin (omega) / Q;
	const double c2 = -2.0 * std::cos (omega);
	const double alphaTimesA = alpha * A;
	const double alphaOverA = alpha / A;

	setCoefficients (1.0 + alphaTimesA,
					 c2,
					 1.0 - alphaTimesA,
					 1.0 + alphaOverA,
					 c2,
					 1.0 - alphaOverA);
}

void IIRFilter::makeInactive() noexcept
{
	const ScopedLock sl (processLock);
	active = false;
}

void IIRFilter::copyCoefficientsFrom (const IIRFilter& other) noexcept
{
	const ScopedLock sl (processLock);

	memcpy (coefficients, other.coefficients, sizeof (coefficients));
	active = other.active;
}

void IIRFilter::setCoefficients (double c1, double c2, double c3,
								 double c4, double c5, double c6) noexcept
{
	const double a = 1.0 / c4;

	c1 *= a;
	c2 *= a;
	c3 *= a;
	c5 *= a;
	c6 *= a;

	const ScopedLock sl (processLock);

	coefficients[0] = (float) c1;
	coefficients[1] = (float) c2;
	coefficients[2] = (float) c3;
	coefficients[3] = (float) c4;
	coefficients[4] = (float) c5;
	coefficients[5] = (float) c6;

	active = true;
}

#undef JUCE_SNAP_TO_ZERO

/*** End of inlined file: juce_IIRFilter.cpp ***/


/*** Start of inlined file: juce_MidiBuffer.cpp ***/
namespace MidiBufferHelpers
{
	inline int getEventTime (const void* const d) noexcept
	{
		return *static_cast <const int*> (d);
	}

	inline uint16 getEventDataSize (const void* const d) noexcept
	{
		return *reinterpret_cast <const uint16*> (static_cast <const char*> (d) + sizeof (int));
	}

	inline uint16 getEventTotalSize (const void* const d) noexcept
	{
		return getEventDataSize (d) + sizeof (int) + sizeof (uint16);
	}

	static int findActualEventLength (const uint8* const data, const int maxBytes) noexcept
	{
		unsigned int byte = (unsigned int) *data;
		int size = 0;

		if (byte == 0xf0 || byte == 0xf7)
		{
			const uint8* d = data + 1;

			while (d < data + maxBytes)
				if (*d++ == 0xf7)
					break;

			size = (int) (d - data);
		}
		else if (byte == 0xff)
		{
			int n;
			const int bytesLeft = MidiMessage::readVariableLengthVal (data + 1, n);
			size = jmin (maxBytes, n + 2 + bytesLeft);
		}
		else if (byte >= 0x80)
		{
			size = jmin (maxBytes, MidiMessage::getMessageLengthFromFirstByte ((uint8) byte));
		}

		return size;
	}
}

MidiBuffer::MidiBuffer() noexcept
	: bytesUsed (0)
{
}

MidiBuffer::MidiBuffer (const MidiMessage& message) noexcept
	: bytesUsed (0)
{
	addEvent (message, 0);
}

MidiBuffer::MidiBuffer (const MidiBuffer& other) noexcept
	: data (other.data),
	  bytesUsed (other.bytesUsed)
{
}

MidiBuffer& MidiBuffer::operator= (const MidiBuffer& other) noexcept
{
	bytesUsed = other.bytesUsed;
	data = other.data;

	return *this;
}

void MidiBuffer::swapWith (MidiBuffer& other) noexcept
{
	data.swapWith (other.data);
	std::swap (bytesUsed, other.bytesUsed);
}

MidiBuffer::~MidiBuffer()
{
}

inline uint8* MidiBuffer::getData() const noexcept
{
	return static_cast <uint8*> (data.getData());
}

void MidiBuffer::clear() noexcept
{
	bytesUsed = 0;
}

void MidiBuffer::clear (const int startSample, const int numSamples)
{
	uint8* const start = findEventAfter (getData(), startSample - 1);
	uint8* const end   = findEventAfter (start, startSample + numSamples - 1);

	if (end > start)
	{
		const int bytesToMove = bytesUsed - (int) (end - getData());

		if (bytesToMove > 0)
			memmove (start, end, (size_t) bytesToMove);

		bytesUsed -= (int) (end - start);
	}
}

void MidiBuffer::addEvent (const MidiMessage& m, const int sampleNumber)
{
	addEvent (m.getRawData(), m.getRawDataSize(), sampleNumber);
}

void MidiBuffer::addEvent (const void* const newData, const int maxBytes, const int sampleNumber)
{
	const int numBytes = MidiBufferHelpers::findActualEventLength (static_cast <const uint8*> (newData), maxBytes);

	if (numBytes > 0)
	{
		size_t spaceNeeded = (size_t) bytesUsed + (size_t) numBytes + sizeof (int) + sizeof (uint16);
		data.ensureSize ((spaceNeeded + spaceNeeded / 2 + 8) & ~(size_t) 7);

		uint8* d = findEventAfter (getData(), sampleNumber);
		const int bytesToMove = bytesUsed - (int) (d - getData());

		if (bytesToMove > 0)
			memmove (d + numBytes + sizeof (int) + sizeof (uint16), d, (size_t) bytesToMove);

		*reinterpret_cast <int*> (d) = sampleNumber;
		d += sizeof (int);
		*reinterpret_cast <uint16*> (d) = (uint16) numBytes;
		d += sizeof (uint16);

		memcpy (d, newData, (size_t) numBytes);

		bytesUsed += sizeof (int) + sizeof (uint16) + (size_t) numBytes;
	}
}

void MidiBuffer::addEvents (const MidiBuffer& otherBuffer,
							const int startSample,
							const int numSamples,
							const int sampleDeltaToAdd)
{
	Iterator i (otherBuffer);
	i.setNextSamplePosition (startSample);

	const uint8* eventData;
	int eventSize, position;

	while (i.getNextEvent (eventData, eventSize, position)
			&& (position < startSample + numSamples || numSamples < 0))
	{
		addEvent (eventData, eventSize, position + sampleDeltaToAdd);
	}
}

void MidiBuffer::ensureSize (size_t minimumNumBytes)
{
	data.ensureSize (minimumNumBytes);
}

bool MidiBuffer::isEmpty() const noexcept
{
	return bytesUsed == 0;
}

int MidiBuffer::getNumEvents() const noexcept
{
	int n = 0;
	const uint8* d = getData();
	const uint8* const end = d + bytesUsed;

	while (d < end)
	{
		d += MidiBufferHelpers::getEventTotalSize (d);
		++n;
	}

	return n;
}

int MidiBuffer::getFirstEventTime() const noexcept
{
	return bytesUsed > 0 ? MidiBufferHelpers::getEventTime (data.getData()) : 0;
}

int MidiBuffer::getLastEventTime() const noexcept
{
	if (bytesUsed == 0)
		return 0;

	const uint8* d = getData();
	const uint8* const endData = d + bytesUsed;

	for (;;)
	{
		const uint8* const nextOne = d + MidiBufferHelpers::getEventTotalSize (d);

		if (nextOne >= endData)
			return MidiBufferHelpers::getEventTime (d);

		d = nextOne;
	}
}

uint8* MidiBuffer::findEventAfter (uint8* d, const int samplePosition) const noexcept
{
	const uint8* const endData = getData() + bytesUsed;

	while (d < endData && MidiBufferHelpers::getEventTime (d) <= samplePosition)
		d += MidiBufferHelpers::getEventTotalSize (d);

	return d;
}

MidiBuffer::Iterator::Iterator (const MidiBuffer& buffer_) noexcept
	: buffer (buffer_),
	  data (buffer_.getData())
{
}

MidiBuffer::Iterator::~Iterator() noexcept
{
}

void MidiBuffer::Iterator::setNextSamplePosition (const int samplePosition) noexcept
{
	data = buffer.getData();
	const uint8* dataEnd = data + buffer.bytesUsed;

	while (data < dataEnd && MidiBufferHelpers::getEventTime (data) < samplePosition)
		data += MidiBufferHelpers::getEventTotalSize (data);
}

bool MidiBuffer::Iterator::getNextEvent (const uint8* &midiData, int& numBytes, int& samplePosition) noexcept
{
	if (data >= buffer.getData() + buffer.bytesUsed)
		return false;

	samplePosition = MidiBufferHelpers::getEventTime (data);
	numBytes = MidiBufferHelpers::getEventDataSize (data);
	data += sizeof (int) + sizeof (uint16);
	midiData = data;
	data += numBytes;

	return true;
}

bool MidiBuffer::Iterator::getNextEvent (MidiMessage& result, int& samplePosition) noexcept
{
	if (data >= buffer.getData() + buffer.bytesUsed)
		return false;

	samplePosition = MidiBufferHelpers::getEventTime (data);
	const int numBytes = MidiBufferHelpers::getEventDataSize (data);
	data += sizeof (int) + sizeof (uint16);
	result = MidiMessage (data, numBytes, samplePosition);
	data += numBytes;

	return true;
}

/*** End of inlined file: juce_MidiBuffer.cpp ***/


/*** Start of inlined file: juce_MidiFile.cpp ***/
namespace MidiFileHelpers
{
	static void writeVariableLengthInt (OutputStream& out, unsigned int v)
	{
		unsigned int buffer = v & 0x7f;

		while ((v >>= 7) != 0)
		{
			buffer <<= 8;
			buffer |= ((v & 0x7f) | 0x80);
		}

		for (;;)
		{
			out.writeByte ((char) buffer);

			if (buffer & 0x80)
				buffer >>= 8;
			else
				break;
		}
	}

	static bool parseMidiHeader (const uint8* &data, short& timeFormat, short& fileType, short& numberOfTracks) noexcept
	{
		unsigned int ch = ByteOrder::bigEndianInt (data);
		data += 4;

		if (ch != ByteOrder::bigEndianInt ("MThd"))
		{
			bool ok = false;

			if (ch == ByteOrder::bigEndianInt ("RIFF"))
			{
				for (int i = 0; i < 8; ++i)
				{
					ch = ByteOrder::bigEndianInt (data);
					data += 4;

					if (ch == ByteOrder::bigEndianInt ("MThd"))
					{
						ok = true;
						break;
					}
				}
			}

			if (! ok)
				return false;
		}

		unsigned int bytesRemaining = ByteOrder::bigEndianInt (data);
		data += 4;
		fileType = (short) ByteOrder::bigEndianShort (data);
		data += 2;
		numberOfTracks = (short) ByteOrder::bigEndianShort (data);
		data += 2;
		timeFormat = (short) ByteOrder::bigEndianShort (data);
		data += 2;
		bytesRemaining -= 6;
		data += bytesRemaining;

		return true;
	}

	static double convertTicksToSeconds (const double time,
										 const MidiMessageSequence& tempoEvents,
										 const int timeFormat)
	{
		if (timeFormat > 0)
		{
			double lastTime = 0.0, correctedTime = 0.0;
			const double tickLen = 1.0 / (timeFormat & 0x7fff);
			double secsPerTick = 0.5 * tickLen;
			const int numEvents = tempoEvents.getNumEvents();

			for (int i = 0; i < numEvents; ++i)
			{
				const MidiMessage& m = tempoEvents.getEventPointer(i)->message;
				const double eventTime = m.getTimeStamp();

				if (eventTime >= time)
					break;

				correctedTime += (eventTime - lastTime) * secsPerTick;
				lastTime = eventTime;

				if (m.isTempoMetaEvent())
					secsPerTick = tickLen * m.getTempoSecondsPerQuarterNote();

				while (i + 1 < numEvents)
				{
					const MidiMessage& m2 = tempoEvents.getEventPointer(i + 1)->message;

					if (m2.getTimeStamp() != eventTime)
						break;

					if (m2.isTempoMetaEvent())
						secsPerTick = tickLen * m2.getTempoSecondsPerQuarterNote();

					++i;
				}
			}

			return correctedTime + (time - lastTime) * secsPerTick;
		}
		else
		{
			return time / (((timeFormat & 0x7fff) >> 8) * (timeFormat & 0xff));
		}
	}

	// a comparator that puts all the note-offs before note-ons that have the same time
	struct Sorter
	{
		static int compareElements (const MidiMessageSequence::MidiEventHolder* const first,
									const MidiMessageSequence::MidiEventHolder* const second) noexcept
		{
			const double diff = (first->message.getTimeStamp() - second->message.getTimeStamp());

			if (diff > 0) return 1;
			if (diff < 0) return -1;
			if (first->message.isNoteOff() && second->message.isNoteOn())   return -1;
			if (first->message.isNoteOn()  && second->message.isNoteOff())  return 1;

			return 0;
		}
	};
}

MidiFile::MidiFile()
   : timeFormat ((short) (unsigned short) 0xe728)
{
}

MidiFile::~MidiFile()
{
}

void MidiFile::clear()
{
	tracks.clear();
}

int MidiFile::getNumTracks() const noexcept
{
	return tracks.size();
}

const MidiMessageSequence* MidiFile::getTrack (const int index) const noexcept
{
	return tracks [index];
}

void MidiFile::addTrack (const MidiMessageSequence& trackSequence)
{
	tracks.add (new MidiMessageSequence (trackSequence));
}

short MidiFile::getTimeFormat() const noexcept
{
	return timeFormat;
}

void MidiFile::setTicksPerQuarterNote (const int ticks) noexcept
{
	timeFormat = (short) ticks;
}

void MidiFile::setSmpteTimeFormat (const int framesPerSecond,
								   const int subframeResolution) noexcept
{
	timeFormat = (short) (((-framesPerSecond) << 8) | subframeResolution);
}

void MidiFile::findAllTempoEvents (MidiMessageSequence& tempoChangeEvents) const
{
	for (int i = tracks.size(); --i >= 0;)
	{
		const int numEvents = tracks.getUnchecked(i)->getNumEvents();

		for (int j = 0; j < numEvents; ++j)
		{
			const MidiMessage& m = tracks.getUnchecked(i)->getEventPointer (j)->message;

			if (m.isTempoMetaEvent())
				tempoChangeEvents.addEvent (m);
		}
	}
}

void MidiFile::findAllTimeSigEvents (MidiMessageSequence& timeSigEvents) const
{
	for (int i = tracks.size(); --i >= 0;)
	{
		const int numEvents = tracks.getUnchecked(i)->getNumEvents();

		for (int j = 0; j < numEvents; ++j)
		{
			const MidiMessage& m = tracks.getUnchecked(i)->getEventPointer (j)->message;

			if (m.isTimeSignatureMetaEvent())
				timeSigEvents.addEvent (m);
		}
	}
}

double MidiFile::getLastTimestamp() const
{
	double t = 0.0;

	for (int i = tracks.size(); --i >= 0;)
		t = jmax (t, tracks.getUnchecked(i)->getEndTime());

	return t;
}

bool MidiFile::readFrom (InputStream& sourceStream)
{
	clear();
	MemoryBlock data;

	const int maxSensibleMidiFileSize = 2 * 1024 * 1024;

	// (put a sanity-check on the file size, as midi files are generally small)
	if (sourceStream.readIntoMemoryBlock (data, maxSensibleMidiFileSize))
	{
		size_t size = data.getSize();
		const uint8* d = static_cast <const uint8*> (data.getData());
		short fileType, expectedTracks;

		if (size > 16 && MidiFileHelpers::parseMidiHeader (d, timeFormat, fileType, expectedTracks))
		{
			size -= (size_t) (d - static_cast <const uint8*> (data.getData()));

			int track = 0;

			while (size > 0 && track < expectedTracks)
			{
				const int chunkType = (int) ByteOrder::bigEndianInt (d);
				d += 4;
				const int chunkSize = (int) ByteOrder::bigEndianInt (d);
				d += 4;

				if (chunkSize <= 0)
					break;

				if (chunkType == (int) ByteOrder::bigEndianInt ("MTrk"))
					readNextTrack (d, chunkSize);

				size -= (size_t) chunkSize + 8;
				d += chunkSize;
				++track;
			}

			return true;
		}
	}

	return false;
}

void MidiFile::readNextTrack (const uint8* data, int size)
{
	double time = 0;
	uint8 lastStatusByte = 0;

	MidiMessageSequence result;

	while (size > 0)
	{
		int bytesUsed;
		const int delay = MidiMessage::readVariableLengthVal (data, bytesUsed);
		data += bytesUsed;
		size -= bytesUsed;
		time += delay;

		int messSize = 0;
		const MidiMessage mm (data, size, messSize, lastStatusByte, time);

		if (messSize <= 0)
			break;

		size -= messSize;
		data += messSize;

		result.addEvent (mm);

		const uint8 firstByte = *(mm.getRawData());
		if ((firstByte & 0xf0) != 0xf0)
			lastStatusByte = firstByte;
	}

	// use a sort that puts all the note-offs before note-ons that have the same time
	MidiFileHelpers::Sorter sorter;
	result.list.sort (sorter, true);

	result.updateMatchedPairs();

	addTrack (result);
}

void MidiFile::convertTimestampTicksToSeconds()
{
	MidiMessageSequence tempoEvents;
	findAllTempoEvents (tempoEvents);
	findAllTimeSigEvents (tempoEvents);

	for (int i = 0; i < tracks.size(); ++i)
	{
		const MidiMessageSequence& ms = *tracks.getUnchecked(i);

		for (int j = ms.getNumEvents(); --j >= 0;)
		{
			MidiMessage& m = ms.getEventPointer(j)->message;

			m.setTimeStamp (MidiFileHelpers::convertTicksToSeconds (m.getTimeStamp(),
																	tempoEvents,
																	timeFormat));
		}
	}
}

bool MidiFile::writeTo (OutputStream& out)
{
	out.writeIntBigEndian ((int) ByteOrder::bigEndianInt ("MThd"));
	out.writeIntBigEndian (6);
	out.writeShortBigEndian (1); // type
	out.writeShortBigEndian ((short) tracks.size());
	out.writeShortBigEndian (timeFormat);

	for (int i = 0; i < tracks.size(); ++i)
		writeTrack (out, i);

	out.flush();
	return true;
}

void MidiFile::writeTrack (OutputStream& mainOut, const int trackNum)
{
	MemoryOutputStream out;
	const MidiMessageSequence& ms = *tracks.getUnchecked (trackNum);

	int lastTick = 0;
	uint8 lastStatusByte = 0;

	for (int i = 0; i < ms.getNumEvents(); ++i)
	{
		const MidiMessage& mm = ms.getEventPointer(i)->message;

		if (! mm.isEndOfTrackMetaEvent())
		{
			const int tick = roundToInt (mm.getTimeStamp());
			const int delta = jmax (0, tick - lastTick);
			MidiFileHelpers::writeVariableLengthInt (out, (uint32) delta);
			lastTick = tick;

			const uint8* data = mm.getRawData();
			int dataSize = mm.getRawDataSize();

			const uint8 statusByte = data[0];

			if (statusByte == lastStatusByte
				 && (statusByte & 0xf0) != 0xf0
				 && dataSize > 1
				 && i > 0)
			{
				++data;
				--dataSize;
			}
			else if (statusByte == 0xf0)  // Write sysex message with length bytes.
			{
				out.writeByte ((char) statusByte);

				++data;
				--dataSize;

				MidiFileHelpers::writeVariableLengthInt (out, (uint32) dataSize);
			}

			out.write (data, dataSize);
			lastStatusByte = statusByte;
		}
	}

	{
		out.writeByte (0); // (tick delta)
		const MidiMessage m (MidiMessage::endOfTrack());
		out.write (m.getRawData(), m.getRawDataSize());
	}

	mainOut.writeIntBigEndian ((int) ByteOrder::bigEndianInt ("MTrk"));
	mainOut.writeIntBigEndian ((int) out.getDataSize());
	mainOut << out;
}

/*** End of inlined file: juce_MidiFile.cpp ***/


/*** Start of inlined file: juce_MidiKeyboardState.cpp ***/
MidiKeyboardState::MidiKeyboardState()
{
	zerostruct (noteStates);
}

MidiKeyboardState::~MidiKeyboardState()
{
}

void MidiKeyboardState::reset()
{
	const ScopedLock sl (lock);
	zerostruct (noteStates);
	eventsToAdd.clear();
}

bool MidiKeyboardState::isNoteOn (const int midiChannel, const int n) const noexcept
{
	jassert (midiChannel >= 0 && midiChannel <= 16);

	return isPositiveAndBelow (n, (int) 128)
			&& (noteStates[n] & (1 << (midiChannel - 1))) != 0;
}

bool MidiKeyboardState::isNoteOnForChannels (const int midiChannelMask, const int n) const noexcept
{
	return isPositiveAndBelow (n, (int) 128)
			&& (noteStates[n] & midiChannelMask) != 0;
}

void MidiKeyboardState::noteOn (const int midiChannel, const int midiNoteNumber, const float velocity)
{
	jassert (midiChannel >= 0 && midiChannel <= 16);
	jassert (isPositiveAndBelow (midiNoteNumber, (int) 128));

	const ScopedLock sl (lock);

	if (isPositiveAndBelow (midiNoteNumber, (int) 128))
	{
		const int timeNow = (int) Time::getMillisecondCounter();
		eventsToAdd.addEvent (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity), timeNow);
		eventsToAdd.clear (0, timeNow - 500);

		noteOnInternal (midiChannel, midiNoteNumber, velocity);
	}
}

void MidiKeyboardState::noteOnInternal  (const int midiChannel, const int midiNoteNumber, const float velocity)
{
	if (isPositiveAndBelow (midiNoteNumber, (int) 128))
	{
		noteStates [midiNoteNumber] |= (1 << (midiChannel - 1));

		for (int i = listeners.size(); --i >= 0;)
			listeners.getUnchecked(i)->handleNoteOn (this, midiChannel, midiNoteNumber, velocity);
	}
}

void MidiKeyboardState::noteOff (const int midiChannel, const int midiNoteNumber)
{
	const ScopedLock sl (lock);

	if (isNoteOn (midiChannel, midiNoteNumber))
	{
		const int timeNow = (int) Time::getMillisecondCounter();
		eventsToAdd.addEvent (MidiMessage::noteOff (midiChannel, midiNoteNumber), timeNow);
		eventsToAdd.clear (0, timeNow - 500);

		noteOffInternal (midiChannel, midiNoteNumber);
	}
}

void MidiKeyboardState::noteOffInternal  (const int midiChannel, const int midiNoteNumber)
{
	if (isNoteOn (midiChannel, midiNoteNumber))
	{
		noteStates [midiNoteNumber] &= ~(1 << (midiChannel - 1));

		for (int i = listeners.size(); --i >= 0;)
			listeners.getUnchecked(i)->handleNoteOff (this, midiChannel, midiNoteNumber);
	}
}

void MidiKeyboardState::allNotesOff (const int midiChannel)
{
	const ScopedLock sl (lock);

	if (midiChannel <= 0)
	{
		for (int i = 1; i <= 16; ++i)
			allNotesOff (i);
	}
	else
	{
		for (int i = 0; i < 128; ++i)
			noteOff (midiChannel, i);
	}
}

void MidiKeyboardState::processNextMidiEvent (const MidiMessage& message)
{
	if (message.isNoteOn())
	{
		noteOnInternal (message.getChannel(), message.getNoteNumber(), message.getFloatVelocity());
	}
	else if (message.isNoteOff())
	{
		noteOffInternal (message.getChannel(), message.getNoteNumber());
	}
	else if (message.isAllNotesOff())
	{
		for (int i = 0; i < 128; ++i)
			noteOffInternal (message.getChannel(), i);
	}
}

void MidiKeyboardState::processNextMidiBuffer (MidiBuffer& buffer,
											   const int startSample,
											   const int numSamples,
											   const bool injectIndirectEvents)
{
	MidiBuffer::Iterator i (buffer);
	MidiMessage message (0xf4, 0.0);
	int time;

	const ScopedLock sl (lock);

	while (i.getNextEvent (message, time))
		processNextMidiEvent (message);

	if (injectIndirectEvents)
	{
		MidiBuffer::Iterator i2 (eventsToAdd);
		const int firstEventToAdd = eventsToAdd.getFirstEventTime();
		const double scaleFactor = numSamples / (double) (eventsToAdd.getLastEventTime() + 1 - firstEventToAdd);

		while (i2.getNextEvent (message, time))
		{
			const int pos = jlimit (0, numSamples - 1, roundToInt ((time - firstEventToAdd) * scaleFactor));
			buffer.addEvent (message, startSample + pos);
		}
	}

	eventsToAdd.clear();
}

void MidiKeyboardState::addListener (MidiKeyboardStateListener* const listener)
{
	const ScopedLock sl (lock);
	listeners.addIfNotAlreadyThere (listener);
}

void MidiKeyboardState::removeListener (MidiKeyboardStateListener* const listener)
{
	const ScopedLock sl (lock);
	listeners.removeValue (listener);
}

/*** End of inlined file: juce_MidiKeyboardState.cpp ***/


/*** Start of inlined file: juce_MidiMessage.cpp ***/
namespace MidiHelpers
{
	inline uint8 initialByte (const int type, const int channel) noexcept
	{
		return (uint8) (type | jlimit (0, 15, channel - 1));
	}

	inline uint8 validVelocity (const int v) noexcept
	{
		return (uint8) jlimit (0, 127, v);
	}
}

int MidiMessage::readVariableLengthVal (const uint8* data, int& numBytesUsed) noexcept
{
	numBytesUsed = 0;
	int v = 0;
	int i;

	do
	{
		i = (int) *data++;

		if (++numBytesUsed > 6)
			break;

		v = (v << 7) + (i & 0x7f);

	} while (i & 0x80);

	return v;
}

int MidiMessage::getMessageLengthFromFirstByte (const uint8 firstByte) noexcept
{
	// this method only works for valid starting bytes of a short midi message
	jassert (firstByte >= 0x80 && firstByte != 0xf0 && firstByte != 0xf7);

	static const char messageLengths[] =
	{
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		1, 2, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};

	return messageLengths [firstByte & 0x7f];
}

inline void MidiMessage::setToUseInternalData() noexcept
{
	data = static_cast <uint8*> (preallocatedData.asBytes);
}

inline bool MidiMessage::usesAllocatedData() const noexcept
{
	return data != static_cast <const uint8*> (preallocatedData.asBytes);
}

inline void MidiMessage::freeData() noexcept
{
	if (usesAllocatedData())
		delete[] data;
}

MidiMessage::MidiMessage() noexcept
   : timeStamp (0),
	 data (static_cast<uint8*> (preallocatedData.asBytes)),
	 size (2)
{
	preallocatedData.asBytes[0] = 0xf0;
	preallocatedData.asBytes[1] = 0xf7;
}

MidiMessage::MidiMessage (const void* const d, const int dataSize, const double t)
   : timeStamp (t),
	 size (dataSize)
{
	jassert (dataSize > 0);

	if (dataSize <= 4)
		setToUseInternalData();
	else
		data = new uint8 [dataSize];

	memcpy (data, d, (size_t) dataSize);

	// check that the length matches the data..
	jassert (size > 3 || data[0] >= 0xf0 || getMessageLengthFromFirstByte (data[0]) == size);
}

MidiMessage::MidiMessage (const int byte1, const double t) noexcept
   : timeStamp (t),
	 data (static_cast<uint8*> (preallocatedData.asBytes)),
	 size (1)
{
	preallocatedData.asBytes[0] = (uint8) byte1;

	// check that the length matches the data..
	jassert (byte1 >= 0xf0 || getMessageLengthFromFirstByte ((uint8) byte1) == 1);
}

MidiMessage::MidiMessage (const int byte1, const int byte2, const double t) noexcept
   : timeStamp (t),
	 data (static_cast<uint8*> (preallocatedData.asBytes)),
	 size (2)
{
	preallocatedData.asBytes[0] = (uint8) byte1;
	preallocatedData.asBytes[1] = (uint8) byte2;

	// check that the length matches the data..
	jassert (byte1 >= 0xf0 || getMessageLengthFromFirstByte ((uint8) byte1) == 2);
}

MidiMessage::MidiMessage (const int byte1, const int byte2, const int byte3, const double t) noexcept
   : timeStamp (t),
	 data (static_cast<uint8*> (preallocatedData.asBytes)),
	 size (3)
{
	preallocatedData.asBytes[0] = (uint8) byte1;
	preallocatedData.asBytes[1] = (uint8) byte2;
	preallocatedData.asBytes[2] = (uint8) byte3;

	// check that the length matches the data..
	jassert (byte1 >= 0xf0 || getMessageLengthFromFirstByte ((uint8) byte1) == 3);
}

MidiMessage::MidiMessage (const MidiMessage& other)
   : timeStamp (other.timeStamp),
	 size (other.size)
{
	if (other.usesAllocatedData())
	{
		data = new uint8 [size];
		memcpy (data, other.data, (size_t) size);
	}
	else
	{
		setToUseInternalData();
		preallocatedData.asInt32 = other.preallocatedData.asInt32;
	}
}

MidiMessage::MidiMessage (const MidiMessage& other, const double newTimeStamp)
   : timeStamp (newTimeStamp),
	 size (other.size)
{
	if (other.usesAllocatedData())
	{
		data = new uint8 [size];
		memcpy (data, other.data, (size_t) size);
	}
	else
	{
		setToUseInternalData();
		preallocatedData.asInt32 = other.preallocatedData.asInt32;
	}
}

MidiMessage::MidiMessage (const void* src_, int sz, int& numBytesUsed, const uint8 lastStatusByte, double t)
	: timeStamp (t),
	  data (static_cast<uint8*> (preallocatedData.asBytes))
{
	const uint8* src = static_cast <const uint8*> (src_);
	unsigned int byte = (unsigned int) *src;

	if (byte < 0x80)
	{
		byte = (unsigned int) (uint8) lastStatusByte;
		numBytesUsed = -1;
	}
	else
	{
		numBytesUsed = 0;
		--sz;
		++src;
	}

	if (byte >= 0x80)
	{
		if (byte == 0xf0)
		{
			const uint8* d = src;
			bool haveReadAllLengthBytes = false;
			int numVariableLengthSysexBytes = 0;

			while (d < src + sz)
			{
				if (*d >= 0x80)
				{
					if (*d == 0xf7)
					{
						++d;  // include the trailing 0xf7 when we hit it
						break;
					}

					if (haveReadAllLengthBytes) // if we see a 0x80 bit set after the initial data length
						break;                  // bytes, assume it's the end of the sysex

					++numVariableLengthSysexBytes;
					++d;
					continue;
				}

				if (! haveReadAllLengthBytes)
				{
					haveReadAllLengthBytes = true;
					++numVariableLengthSysexBytes;
				}

				++d;
			}

			size = 1 + (int) (d - src);

			data = new uint8 [size - numVariableLengthSysexBytes];
			*data = (uint8) byte;
			memcpy (data + 1, src + numVariableLengthSysexBytes, (size_t) (size - numVariableLengthSysexBytes - 1));
		}
		else if (byte == 0xff)
		{
			int n;
			const int bytesLeft = readVariableLengthVal (src + 1, n);
			size = jmin (sz + 1, n + 2 + bytesLeft);

			data = new uint8 [size];
			*data = (uint8) byte;
			memcpy (data + 1, src, (size_t) size - 1);
		}
		else
		{
			preallocatedData.asInt32 = 0;
			size = getMessageLengthFromFirstByte ((uint8) byte);
			data[0] = (uint8) byte;

			if (size > 1)
			{
				data[1] = src[0];

				if (size > 2)
					data[2] = src[1];
			}
		}

		numBytesUsed += size;
	}
	else
	{
		preallocatedData.asInt32 = 0;
		size = 0;
	}
}

MidiMessage& MidiMessage::operator= (const MidiMessage& other)
{
	if (this != &other)
	{
		timeStamp = other.timeStamp;
		size = other.size;

		freeData();

		if (other.usesAllocatedData())
		{
			data = new uint8 [size];
			memcpy (data, other.data, (size_t) size);
		}
		else
		{
			setToUseInternalData();
			preallocatedData.asInt32 = other.preallocatedData.asInt32;
		}
	}

	return *this;
}

#if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
MidiMessage::MidiMessage (MidiMessage&& other) noexcept
   : timeStamp (other.timeStamp),
	 size (other.size)
{
	if (other.usesAllocatedData())
	{
		data = other.data;
		other.setToUseInternalData();
	}
	else
	{
		setToUseInternalData();
		preallocatedData.asInt32 = other.preallocatedData.asInt32;
	}
}

MidiMessage& MidiMessage::operator= (MidiMessage&& other) noexcept
{
	jassert (this != &other); // shouldn't be possible

	timeStamp = other.timeStamp;
	size = other.size;

	freeData();

	if (other.usesAllocatedData())
	{
		data = other.data;
		other.setToUseInternalData();
	}
	else
	{
		setToUseInternalData();
		preallocatedData.asInt32 = other.preallocatedData.asInt32;
	}

	return *this;
}
#endif

MidiMessage::~MidiMessage()
{
	freeData();
}

int MidiMessage::getChannel() const noexcept
{
	if ((data[0] & 0xf0) != 0xf0)
		return (data[0] & 0xf) + 1;

	return 0;
}

bool MidiMessage::isForChannel (const int channel) const noexcept
{
	jassert (channel > 0 && channel <= 16); // valid channels are numbered 1 to 16

	return ((data[0] & 0xf) == channel - 1)
			 && ((data[0] & 0xf0) != 0xf0);
}

void MidiMessage::setChannel (const int channel) noexcept
{
	jassert (channel > 0 && channel <= 16); // valid channels are numbered 1 to 16

	if ((data[0] & 0xf0) != (uint8) 0xf0)
		data[0] = (uint8) ((data[0] & (uint8) 0xf0)
							| (uint8)(channel - 1));
}

bool MidiMessage::isNoteOn (const bool returnTrueForVelocity0) const noexcept
{
	return ((data[0] & 0xf0) == 0x90)
			 && (returnTrueForVelocity0 || data[2] != 0);
}

bool MidiMessage::isNoteOff (const bool returnTrueForNoteOnVelocity0) const noexcept
{
	return ((data[0] & 0xf0) == 0x80)
			|| (returnTrueForNoteOnVelocity0 && (data[2] == 0) && ((data[0] & 0xf0) == 0x90));
}

bool MidiMessage::isNoteOnOrOff() const noexcept
{
	const int d = data[0] & 0xf0;
	return (d == 0x90) || (d == 0x80);
}

int MidiMessage::getNoteNumber() const noexcept
{
	return data[1];
}

void MidiMessage::setNoteNumber (const int newNoteNumber) noexcept
{
	if (isNoteOnOrOff())
		data[1] = (uint8) (newNoteNumber & 127);
}

uint8 MidiMessage::getVelocity() const noexcept
{
	if (isNoteOnOrOff())
		return data[2];

	return 0;
}

float MidiMessage::getFloatVelocity() const noexcept
{
	return getVelocity() * (1.0f / 127.0f);
}

void MidiMessage::setVelocity (const float newVelocity) noexcept
{
	if (isNoteOnOrOff())
		data[2] = MidiHelpers::validVelocity (roundToInt (newVelocity * 127.0f));
}

void MidiMessage::multiplyVelocity (const float scaleFactor) noexcept
{
	if (isNoteOnOrOff())
		data[2] = MidiHelpers::validVelocity (roundToInt (scaleFactor * data[2]));
}

bool MidiMessage::isAftertouch() const noexcept
{
	return (data[0] & 0xf0) == 0xa0;
}

int MidiMessage::getAfterTouchValue() const noexcept
{
	jassert (isAftertouch());
	return data[2];
}

MidiMessage MidiMessage::aftertouchChange (const int channel,
										   const int noteNum,
										   const int aftertouchValue) noexcept
{
	jassert (channel > 0 && channel <= 16); // valid channels are numbered 1 to 16
	jassert (isPositiveAndBelow (noteNum, (int) 128));
	jassert (isPositiveAndBelow (aftertouchValue, (int) 128));

	return MidiMessage (MidiHelpers::initialByte (0xa0, channel),
						noteNum & 0x7f,
						aftertouchValue & 0x7f);
}

bool MidiMessage::isChannelPressure() const noexcept
{
	return (data[0] & 0xf0) == 0xd0;
}

int MidiMessage::getChannelPressureValue() const noexcept
{
	jassert (isChannelPressure());
	return data[1];
}

MidiMessage MidiMessage::channelPressureChange (const int channel, const int pressure) noexcept
{
	jassert (channel > 0 && channel <= 16); // valid channels are numbered 1 to 16
	jassert (isPositiveAndBelow (pressure, (int) 128));

	return MidiMessage (MidiHelpers::initialByte (0xd0, channel), pressure & 0x7f);
}

bool MidiMessage::isSustainPedalOn() const noexcept     { return isControllerOfType (0x40) && data[2] >= 64; }
bool MidiMessage::isSustainPedalOff() const noexcept    { return isControllerOfType (0x40) && data[2] <  64; }

bool MidiMessage::isSostenutoPedalOn() const noexcept   { return isControllerOfType (0x42) && data[2] >= 64; }
bool MidiMessage::isSostenutoPedalOff() const noexcept  { return isControllerOfType (0x42) && data[2] <  64; }

bool MidiMessage::isSoftPedalOn() const noexcept        { return isControllerOfType (0x43) && data[2] >= 64; }
bool MidiMessage::isSoftPedalOff() const noexcept       { return isControllerOfType (0x43) && data[2] <  64; }

bool MidiMessage::isProgramChange() const noexcept
{
	return (data[0] & 0xf0) == 0xc0;
}

int MidiMessage::getProgramChangeNumber() const noexcept
{
	jassert (isProgramChange());
	return data[1];
}

MidiMessage MidiMessage::programChange (const int channel, const int programNumber) noexcept
{
	jassert (channel > 0 && channel <= 16); // valid channels are numbered 1 to 16

	return MidiMessage (MidiHelpers::initialByte (0xc0, channel), programNumber & 0x7f);
}

bool MidiMessage::isPitchWheel() const noexcept
{
	return (data[0] & 0xf0) == 0xe0;
}

int MidiMessage::getPitchWheelValue() const noexcept
{
	jassert (isPitchWheel());
	return data[1] | (data[2] << 7);
}

MidiMessage MidiMessage::pitchWheel (const int channel, const int position) noexcept
{
	jassert (channel > 0 && channel <= 16); // valid channels are numbered 1 to 16
	jassert (isPositiveAndBelow (position, (int) 0x4000));

	return MidiMessage (MidiHelpers::initialByte (0xe0, channel),
						position & 127, (position >> 7) & 127);
}

bool MidiMessage::isController() const noexcept
{
	return (data[0] & 0xf0) == 0xb0;
}

bool MidiMessage::isControllerOfType (const int controllerType) const noexcept
{
	return (data[0] & 0xf0) == 0xb0 && data[1] == controllerType;
}

int MidiMessage::getControllerNumber() const noexcept
{
	jassert (isController());
	return data[1];
}

int MidiMessage::getControllerValue() const noexcept
{
	jassert (isController());
	return data[2];
}

MidiMessage MidiMessage::controllerEvent (const int channel, const int controllerType, const int value) noexcept
{
	// the channel must be between 1 and 16 inclusive
	jassert (channel > 0 && channel <= 16);

	return MidiMessage (MidiHelpers::initialByte (0xb0, channel),
						controllerType & 127, value & 127);
}

MidiMessage MidiMessage::noteOn (const int channel, const int noteNumber, const float velocity) noexcept
{
	return noteOn (channel, noteNumber, (uint8) (velocity * 127.0f + 0.5f));
}

MidiMessage MidiMessage::noteOn (const int channel, const int noteNumber, const uint8 velocity) noexcept
{
	jassert (channel > 0 && channel <= 16);
	jassert (isPositiveAndBelow (noteNumber, (int) 128));

	return MidiMessage (MidiHelpers::initialByte (0x90, channel),
						noteNumber & 127, MidiHelpers::validVelocity (velocity));
}

MidiMessage MidiMessage::noteOff (const int channel, const int noteNumber, uint8 velocity) noexcept
{
	jassert (channel > 0 && channel <= 16);
	jassert (isPositiveAndBelow (noteNumber, (int) 128));

	return MidiMessage (MidiHelpers::initialByte (0x80, channel),
						noteNumber & 127, MidiHelpers::validVelocity (velocity));
}

MidiMessage MidiMessage::allNotesOff (const int channel) noexcept
{
	return controllerEvent (channel, 123, 0);
}

bool MidiMessage::isAllNotesOff() const noexcept
{
	return (data[0] & 0xf0) == 0xb0 && data[1] == 123;
}

MidiMessage MidiMessage::allSoundOff (const int channel) noexcept
{
	return controllerEvent (channel, 120, 0);
}

bool MidiMessage::isAllSoundOff() const noexcept
{
	return (data[0] & 0xf0) == 0xb0 && data[1] == 120;
}

MidiMessage MidiMessage::allControllersOff (const int channel) noexcept
{
	return controllerEvent (channel, 121, 0);
}

MidiMessage MidiMessage::masterVolume (const float volume)
{
	const int vol = jlimit (0, 0x3fff, roundToInt (volume * 0x4000));

	const uint8 buf[] = { 0xf0, 0x7f, 0x7f, 0x04, 0x01,
						  (uint8) (vol & 0x7f),
						  (uint8) (vol >> 7),
						  0xf7 };

	return MidiMessage (buf, 8);
}

bool MidiMessage::isSysEx() const noexcept
{
	return *data == 0xf0;
}

MidiMessage MidiMessage::createSysExMessage (const uint8* sysexData, const int dataSize)
{
	HeapBlock<uint8> m ((size_t) dataSize + 2);

	m[0] = 0xf0;
	memcpy (m + 1, sysexData, (size_t) dataSize);
	m[dataSize + 1] = 0xf7;

	return MidiMessage (m, dataSize + 2);
}

const uint8* MidiMessage::getSysExData() const noexcept
{
	return isSysEx() ? getRawData() + 1 : nullptr;
}

int MidiMessage::getSysExDataSize() const noexcept
{
	return isSysEx() ? size - 2 : 0;
}

bool MidiMessage::isMetaEvent() const noexcept      { return *data == 0xff; }
bool MidiMessage::isActiveSense() const noexcept    { return *data == 0xfe; }

int MidiMessage::getMetaEventType() const noexcept
{
	return *data != 0xff ? -1 : data[1];
}

int MidiMessage::getMetaEventLength() const noexcept
{
	if (*data == 0xff)
	{
		int n;
		return jmin (size - 2, readVariableLengthVal (data + 2, n));
	}

	return 0;
}

const uint8* MidiMessage::getMetaEventData() const noexcept
{
	jassert (isMetaEvent());

	int n;
	const uint8* d = data + 2;
	readVariableLengthVal (d, n);
	return d + n;
}

bool MidiMessage::isTrackMetaEvent() const noexcept         { return getMetaEventType() == 0; }
bool MidiMessage::isEndOfTrackMetaEvent() const noexcept    { return getMetaEventType() == 47; }

bool MidiMessage::isTextMetaEvent() const noexcept
{
	const int t = getMetaEventType();
	return t > 0 && t < 16;
}

String MidiMessage::getTextFromTextMetaEvent() const
{
	return String (reinterpret_cast <const char*> (getMetaEventData()), (size_t) getMetaEventLength());
}

bool MidiMessage::isTrackNameEvent() const noexcept         { return (data[1] == 3)    && (*data == 0xff); }
bool MidiMessage::isTempoMetaEvent() const noexcept         { return (data[1] == 81)   && (*data == 0xff); }
bool MidiMessage::isMidiChannelMetaEvent() const noexcept   { return (data[1] == 0x20) && (*data == 0xff) && (data[2] == 1); }

int MidiMessage::getMidiChannelMetaEventChannel() const noexcept
{
	jassert (isMidiChannelMetaEvent());
	return data[3] + 1;
}

double MidiMessage::getTempoSecondsPerQuarterNote() const noexcept
{
	if (! isTempoMetaEvent())
		return 0.0;

	const uint8* const d = getMetaEventData();

	return (((unsigned int) d[0] << 16)
			 | ((unsigned int) d[1] << 8)
			 | d[2])
			/ 1000000.0;
}

double MidiMessage::getTempoMetaEventTickLength (const short timeFormat) const noexcept
{
	if (timeFormat > 0)
	{
		if (! isTempoMetaEvent())
			return 0.5 / timeFormat;

		return getTempoSecondsPerQuarterNote() / timeFormat;
	}
	else
	{
		const int frameCode = (-timeFormat) >> 8;
		double framesPerSecond;

		switch (frameCode)
		{
			case 24: framesPerSecond = 24.0;   break;
			case 25: framesPerSecond = 25.0;   break;
			case 29: framesPerSecond = 29.97;  break;
			case 30: framesPerSecond = 30.0;   break;
			default: framesPerSecond = 30.0;   break;
		}

		return (1.0 / framesPerSecond) / (timeFormat & 0xff);
	}
}

MidiMessage MidiMessage::tempoMetaEvent (int microsecondsPerQuarterNote) noexcept
{
	const uint8 d[] = { 0xff, 81, 3,
						(uint8) (microsecondsPerQuarterNote >> 16),
						(uint8) (microsecondsPerQuarterNote >> 8),
						(uint8) microsecondsPerQuarterNote };

	return MidiMessage (d, 6, 0.0);
}

bool MidiMessage::isTimeSignatureMetaEvent() const noexcept
{
	return (data[1] == 0x58) && (*data == (uint8) 0xff);
}

void MidiMessage::getTimeSignatureInfo (int& numerator, int& denominator) const noexcept
{
	if (isTimeSignatureMetaEvent())
	{
		const uint8* const d = getMetaEventData();
		numerator = d[0];
		denominator = 1 << d[1];
	}
	else
	{
		numerator = 4;
		denominator = 4;
	}
}

MidiMessage MidiMessage::timeSignatureMetaEvent (const int numerator, const int denominator)
{
	int n = 1;
	int powerOfTwo = 0;

	while (n < denominator)
	{
		n <<= 1;
		++powerOfTwo;
	}

	const uint8 d[] = { 0xff, 0x58, 0x04, (uint8) numerator,
						(uint8) powerOfTwo, 1, 96 };

	return MidiMessage (d, 7, 0.0);
}

MidiMessage MidiMessage::midiChannelMetaEvent (const int channel) noexcept
{
	const uint8 d[] = { 0xff, 0x20, 0x01, (uint8) jlimit (0, 0xff, channel - 1) };

	return MidiMessage (d, 4, 0.0);
}

bool MidiMessage::isKeySignatureMetaEvent() const noexcept
{
	return getMetaEventType() == 89;
}

int MidiMessage::getKeySignatureNumberOfSharpsOrFlats() const noexcept
{
	return (int) *getMetaEventData();
}

MidiMessage MidiMessage::endOfTrack() noexcept
{
	return MidiMessage (0xff, 0x2f, 0, 0.0);
}

bool MidiMessage::isSongPositionPointer() const noexcept         { return *data == 0xf2; }
int MidiMessage::getSongPositionPointerMidiBeat() const noexcept { return data[1] | (data[2] << 7); }

MidiMessage MidiMessage::songPositionPointer (const int positionInMidiBeats) noexcept
{
	return MidiMessage (0xf2,
						positionInMidiBeats & 127,
						(positionInMidiBeats >> 7) & 127);
}

bool MidiMessage::isMidiStart() const noexcept            { return *data == 0xfa; }
MidiMessage MidiMessage::midiStart() noexcept             { return MidiMessage (0xfa); }

bool MidiMessage::isMidiContinue() const noexcept         { return *data == 0xfb; }
MidiMessage MidiMessage::midiContinue() noexcept          { return MidiMessage (0xfb); }

bool MidiMessage::isMidiStop() const noexcept             { return *data == 0xfc; }
MidiMessage MidiMessage::midiStop() noexcept              { return MidiMessage (0xfc); }

bool MidiMessage::isMidiClock() const noexcept            { return *data == 0xf8; }
MidiMessage MidiMessage::midiClock() noexcept             { return MidiMessage (0xf8); }

bool MidiMessage::isQuarterFrame() const noexcept               { return *data == 0xf1; }
int MidiMessage::getQuarterFrameSequenceNumber() const noexcept { return ((int) data[1]) >> 4; }
int MidiMessage::getQuarterFrameValue() const noexcept          { return ((int) data[1]) & 0x0f; }

MidiMessage MidiMessage::quarterFrame (const int sequenceNumber, const int value) noexcept
{
	return MidiMessage (0xf1, (sequenceNumber << 4) | value);
}

bool MidiMessage::isFullFrame() const noexcept
{
	return data[0] == 0xf0
			&& data[1] == 0x7f
			&& size >= 10
			&& data[3] == 0x01
			&& data[4] == 0x01;
}

void MidiMessage::getFullFrameParameters (int& hours, int& minutes, int& seconds, int& frames,
										  MidiMessage::SmpteTimecodeType& timecodeType) const noexcept
{
	jassert (isFullFrame());

	timecodeType = (SmpteTimecodeType) (data[5] >> 5);
	hours   = data[5] & 0x1f;
	minutes = data[6];
	seconds = data[7];
	frames  = data[8];
}

MidiMessage MidiMessage::fullFrame (const int hours, const int minutes,
									const int seconds, const int frames,
									MidiMessage::SmpteTimecodeType timecodeType)
{
	const uint8 d[] = { 0xf0, 0x7f, 0x7f, 0x01, 0x01,
						(uint8) ((hours & 0x01f) | (timecodeType << 5)),
						(uint8) minutes,
						(uint8) seconds,
						(uint8) frames,
						0xf7 };

	return MidiMessage (d, 10, 0.0);
}

bool MidiMessage::isMidiMachineControlMessage() const noexcept
{
	return data[0] == 0xf0
		&& data[1] == 0x7f
		&& data[3] == 0x06
		&& size > 5;
}

MidiMessage::MidiMachineControlCommand MidiMessage::getMidiMachineControlCommand() const noexcept
{
	jassert (isMidiMachineControlMessage());

	return (MidiMachineControlCommand) data[4];
}

MidiMessage MidiMessage::midiMachineControlCommand (MidiMessage::MidiMachineControlCommand command)
{
	const uint8 d[] = { 0xf0, 0x7f, 0, 6, (uint8) command, 0xf7 };

	return MidiMessage (d, 6, 0.0);
}

bool MidiMessage::isMidiMachineControlGoto (int& hours, int& minutes, int& seconds, int& frames) const noexcept
{
	if (size >= 12
		 && data[0] == 0xf0
		 && data[1] == 0x7f
		 && data[3] == 0x06
		 && data[4] == 0x44
		 && data[5] == 0x06
		 && data[6] == 0x01)
	{
		hours = data[7] % 24;   // (that some machines send out hours > 24)
		minutes = data[8];
		seconds = data[9];
		frames = data[10];

		return true;
	}

	return false;
}

MidiMessage MidiMessage::midiMachineControlGoto (int hours, int minutes, int seconds, int frames)
{
	const uint8 d[] = { 0xf0, 0x7f, 0, 6, 0x44, 6, 1,
						(uint8) hours,
						(uint8) minutes,
						(uint8) seconds,
						(uint8) frames,
						0xf7 };

	return MidiMessage (d, 12, 0.0);
}

String MidiMessage::getMidiNoteName (int note, bool useSharps, bool includeOctaveNumber, int octaveNumForMiddleC)
{
	static const char* const sharpNoteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
	static const char* const flatNoteNames[]  = { "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" };

	if (isPositiveAndBelow (note, (int) 128))
	{
		String s (useSharps ? sharpNoteNames [note % 12]
							: flatNoteNames  [note % 12]);

		if (includeOctaveNumber)
			s << (note / 12 + (octaveNumForMiddleC - 5));

		return s;
	}

	return String::empty;
}

const double MidiMessage::getMidiNoteInHertz (int noteNumber, const double frequencyOfA) noexcept
{
	noteNumber -= 12 * 6 + 9; // now 0 = A
	return frequencyOfA * pow (2.0, noteNumber / 12.0);
}

String MidiMessage::getGMInstrumentName (const int n)
{
	const char* names[] =
	{
		"Acoustic Grand Piano", "Bright Acoustic Piano", "Electric Grand Piano", "Honky-tonk Piano",
		"Electric Piano 1", "Electric Piano 2", "Harpsichord", "Clavinet", "Celesta", "Glockenspiel",
		"Music Box", "Vibraphone", "Marimba", "Xylophone", "Tubular Bells", "Dulcimer", "Drawbar Organ",
		"Percussive Organ", "Rock Organ", "Church Organ", "Reed Organ", "Accordion", "Harmonica",
		"Tango Accordion", "Acoustic Guitar (nylon)", "Acoustic Guitar (steel)", "Electric Guitar (jazz)",
		"Electric Guitar (clean)", "Electric Guitar (mute)", "Overdriven Guitar", "Distortion Guitar",
		"Guitar Harmonics", "Acoustic Bass", "Electric Bass (finger)", "Electric Bass (pick)",
		"Fretless Bass", "Slap Bass 1", "Slap Bass 2", "Synth Bass 1", "Synth Bass 2", "Violin",
		"Viola", "Cello", "Contrabass", "Tremolo Strings", "Pizzicato Strings", "Orchestral Harp",
		"Timpani", "String Ensemble 1", "String Ensemble 2", "SynthStrings 1", "SynthStrings 2",
		"Choir Aahs", "Voice Oohs", "Synth Voice", "Orchestra Hit", "Trumpet", "Trombone", "Tuba",
		"Muted Trumpet", "French Horn", "Brass Section", "SynthBrass 1", "SynthBrass 2", "Soprano Sax",
		"Alto Sax", "Tenor Sax", "Baritone Sax", "Oboe", "English Horn", "Bassoon", "Clarinet",
		"Piccolo", "Flute", "Recorder", "Pan Flute", "Blown Bottle", "Shakuhachi", "Whistle",
		"Ocarina", "Lead 1 (square)", "Lead 2 (sawtooth)", "Lead 3 (calliope)", "Lead 4 (chiff)",
		"Lead 5 (charang)", "Lead 6 (voice)", "Lead 7 (fifths)", "Lead 8 (bass+lead)", "Pad 1 (new age)",
		"Pad 2 (warm)", "Pad 3 (polysynth)", "Pad 4 (choir)", "Pad 5 (bowed)", "Pad 6 (metallic)",
		"Pad 7 (halo)", "Pad 8 (sweep)", "FX 1 (rain)", "FX 2 (soundtrack)", "FX 3 (crystal)",
		"FX 4 (atmosphere)", "FX 5 (brightness)", "FX 6 (goblins)", "FX 7 (echoes)", "FX 8 (sci-fi)",
		"Sitar", "Banjo", "Shamisen", "Koto", "Kalimba", "Bag pipe", "Fiddle", "Shanai", "Tinkle Bell",
		"Agogo", "Steel Drums", "Woodblock", "Taiko Drum", "Melodic Tom", "Synth Drum", "Reverse Cymbal",
		"Guitar Fret Noise", "Breath Noise", "Seashore", "Bird Tweet", "Telephone Ring", "Helicopter",
		"Applause", "Gunshot"
	};

	return isPositiveAndBelow (n, (int) 128) ? names[n] : (const char*) 0;
}

String MidiMessage::getGMInstrumentBankName (const int n)
{
	const char* names[] =
	{
		"Piano", "Chromatic Percussion", "Organ", "Guitar",
		"Bass", "Strings", "Ensemble", "Brass",
		"Reed", "Pipe", "Synth Lead", "Synth Pad",
		"Synth Effects", "Ethnic", "Percussive", "Sound Effects"
	};

	return isPositiveAndBelow (n, (int) 16) ? names[n] : (const char*) 0;
}

String MidiMessage::getRhythmInstrumentName (const int n)
{
	const char* names[] =
	{
		"Acoustic Bass Drum", "Bass Drum 1", "Side Stick", "Acoustic Snare",
		"Hand Clap", "Electric Snare", "Low Floor Tom", "Closed Hi-Hat", "High Floor Tom",
		"Pedal Hi-Hat", "Low Tom", "Open Hi-Hat", "Low-Mid Tom", "Hi-Mid Tom", "Crash Cymbal 1",
		"High Tom", "Ride Cymbal 1", "Chinese Cymbal", "Ride Bell", "Tambourine", "Splash Cymbal",
		"Cowbell", "Crash Cymbal 2", "Vibraslap", "Ride Cymbal 2", "Hi Bongo", "Low Bongo",
		"Mute Hi Conga", "Open Hi Conga", "Low Conga", "High Timbale", "Low Timbale", "High Agogo",
		"Low Agogo", "Cabasa", "Maracas", "Short Whistle", "Long Whistle", "Short Guiro",
		"Long Guiro", "Claves", "Hi Wood Block", "Low Wood Block", "Mute Cuica", "Open Cuica",
		"Mute Triangle", "Open Triangle"
	};

	return (n >= 35 && n <= 81) ? names [n - 35] : (const char*) nullptr;
}

String MidiMessage::getControllerName (const int n)
{
	const char* names[] =
	{
		"Bank Select", "Modulation Wheel (coarse)", "Breath controller (coarse)",
		0, "Foot Pedal (coarse)", "Portamento Time (coarse)",
		"Data Entry (coarse)", "Volume (coarse)", "Balance (coarse)",
		0, "Pan position (coarse)", "Expression (coarse)", "Effect Control 1 (coarse)",
		"Effect Control 2 (coarse)", 0, 0, "General Purpose Slider 1", "General Purpose Slider 2",
		"General Purpose Slider 3", "General Purpose Slider 4", 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, "Bank Select (fine)", "Modulation Wheel (fine)", "Breath controller (fine)",
		0, "Foot Pedal (fine)", "Portamento Time (fine)", "Data Entry (fine)", "Volume (fine)",
		"Balance (fine)", 0, "Pan position (fine)", "Expression (fine)", "Effect Control 1 (fine)",
		"Effect Control 2 (fine)", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		"Hold Pedal (on/off)", "Portamento (on/off)", "Sustenuto Pedal (on/off)", "Soft Pedal (on/off)",
		"Legato Pedal (on/off)", "Hold 2 Pedal (on/off)", "Sound Variation", "Sound Timbre",
		"Sound Release Time", "Sound Attack Time", "Sound Brightness", "Sound Control 6",
		"Sound Control 7", "Sound Control 8", "Sound Control 9", "Sound Control 10",
		"General Purpose Button 1 (on/off)", "General Purpose Button 2 (on/off)",
		"General Purpose Button 3 (on/off)", "General Purpose Button 4 (on/off)",
		0, 0, 0, 0, 0, 0, 0, "Reverb Level", "Tremolo Level",  "Chorus Level", "Celeste Level",
		"Phaser Level", "Data Button increment", "Data Button decrement", "Non-registered Parameter (fine)",
		"Non-registered Parameter (coarse)", "Registered Parameter (fine)", "Registered Parameter (coarse)",
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "All Sound Off", "All Controllers Off",
		"Local Keyboard (on/off)", "All Notes Off", "Omni Mode Off", "Omni Mode On", "Mono Operation",
		"Poly Operation"
	};

	return isPositiveAndBelow (n, (int) 128) ? names[n] : (const char*) nullptr;
}

/*** End of inlined file: juce_MidiMessage.cpp ***/


/*** Start of inlined file: juce_MidiMessageSequence.cpp ***/
MidiMessageSequence::MidiMessageSequence()
{
}

MidiMessageSequence::MidiMessageSequence (const MidiMessageSequence& other)
{
	list.ensureStorageAllocated (other.list.size());

	for (int i = 0; i < other.list.size(); ++i)
		list.add (new MidiEventHolder (other.list.getUnchecked(i)->message));
}

MidiMessageSequence& MidiMessageSequence::operator= (const MidiMessageSequence& other)
{
	MidiMessageSequence otherCopy (other);
	swapWith (otherCopy);
	return *this;
}

void MidiMessageSequence::swapWith (MidiMessageSequence& other) noexcept
{
	list.swapWithArray (other.list);
}

MidiMessageSequence::~MidiMessageSequence()
{
}

void MidiMessageSequence::clear()
{
	list.clear();
}

int MidiMessageSequence::getNumEvents() const
{
	return list.size();
}

MidiMessageSequence::MidiEventHolder* MidiMessageSequence::getEventPointer (const int index) const
{
	return list [index];
}

double MidiMessageSequence::getTimeOfMatchingKeyUp (const int index) const
{
	const MidiEventHolder* const meh = list [index];

	if (meh != nullptr && meh->noteOffObject != nullptr)
		return meh->noteOffObject->message.getTimeStamp();
	else
		return 0.0;
}

int MidiMessageSequence::getIndexOfMatchingKeyUp (const int index) const
{
	const MidiEventHolder* const meh = list [index];

	return meh != nullptr ? list.indexOf (meh->noteOffObject) : -1;
}

int MidiMessageSequence::getIndexOf (MidiEventHolder* const event) const
{
	return list.indexOf (event);
}

int MidiMessageSequence::getNextIndexAtTime (const double timeStamp) const
{
	const int numEvents = list.size();

	int i;
	for (i = 0; i < numEvents; ++i)
		if (list.getUnchecked(i)->message.getTimeStamp() >= timeStamp)
			break;

	return i;
}

double MidiMessageSequence::getStartTime() const
{
	return getEventTime (0);
}

double MidiMessageSequence::getEndTime() const
{
	return getEventTime (list.size() - 1);
}

double MidiMessageSequence::getEventTime (const int index) const
{
	const MidiEventHolder* const e = list [index];
	return e != nullptr ? e->message.getTimeStamp() : 0.0;
}

MidiMessageSequence::MidiEventHolder* MidiMessageSequence::addEvent (const MidiMessage& newMessage,
																	 double timeAdjustment)
{
	MidiEventHolder* const newOne = new MidiEventHolder (newMessage);

	timeAdjustment += newMessage.getTimeStamp();
	newOne->message.setTimeStamp (timeAdjustment);

	int i;
	for (i = list.size(); --i >= 0;)
		if (list.getUnchecked(i)->message.getTimeStamp() <= timeAdjustment)
			break;

	list.insert (i + 1, newOne);
	return newOne;
}

void MidiMessageSequence::deleteEvent (const int index,
									   const bool deleteMatchingNoteUp)
{
	if (isPositiveAndBelow (index, list.size()))
	{
		if (deleteMatchingNoteUp)
			deleteEvent (getIndexOfMatchingKeyUp (index), false);

		list.remove (index);
	}
}

struct MidiMessageSequenceSorter
{
	static int compareElements (const MidiMessageSequence::MidiEventHolder* const first,
								const MidiMessageSequence::MidiEventHolder* const second) noexcept
	{
		const double diff = first->message.getTimeStamp() - second->message.getTimeStamp();
		return (diff > 0) - (diff < 0);
	}
};

void MidiMessageSequence::addSequence (const MidiMessageSequence& other,
									   double timeAdjustment,
									   double firstAllowableTime,
									   double endOfAllowableDestTimes)
{
	firstAllowableTime -= timeAdjustment;
	endOfAllowableDestTimes -= timeAdjustment;

	for (int i = 0; i < other.list.size(); ++i)
	{
		const MidiMessage& m = other.list.getUnchecked(i)->message;
		const double t = m.getTimeStamp();

		if (t >= firstAllowableTime && t < endOfAllowableDestTimes)
		{
			MidiEventHolder* const newOne = new MidiEventHolder (m);
			newOne->message.setTimeStamp (timeAdjustment + t);

			list.add (newOne);
		}
	}

	MidiMessageSequenceSorter sorter;
	list.sort (sorter, true);
}

void MidiMessageSequence::updateMatchedPairs()
{
	for (int i = 0; i < list.size(); ++i)
	{
		const MidiMessage& m1 = list.getUnchecked(i)->message;

		if (m1.isNoteOn())
		{
			list.getUnchecked(i)->noteOffObject = nullptr;
			const int note = m1.getNoteNumber();
			const int chan = m1.getChannel();
			const int len = list.size();

			for (int j = i + 1; j < len; ++j)
			{
				const MidiMessage& m = list.getUnchecked(j)->message;

				if (m.getNoteNumber() == note && m.getChannel() == chan)
				{
					if (m.isNoteOff())
					{
						list.getUnchecked(i)->noteOffObject = list[j];
						break;
					}
					else if (m.isNoteOn())
					{
						list.insert (j, new MidiEventHolder (MidiMessage::noteOff (chan, note)));
						list.getUnchecked(j)->message.setTimeStamp (m.getTimeStamp());
						list.getUnchecked(i)->noteOffObject = list[j];
						break;
					}
				}
			}
		}
	}
}

void MidiMessageSequence::addTimeToMessages (const double delta)
{
	for (int i = list.size(); --i >= 0;)
		list.getUnchecked (i)->message.setTimeStamp (list.getUnchecked (i)->message.getTimeStamp()
													  + delta);
}

void MidiMessageSequence::extractMidiChannelMessages (const int channelNumberToExtract,
													  MidiMessageSequence& destSequence,
													  const bool alsoIncludeMetaEvents) const
{
	for (int i = 0; i < list.size(); ++i)
	{
		const MidiMessage& mm = list.getUnchecked(i)->message;

		if (mm.isForChannel (channelNumberToExtract)
			 || (alsoIncludeMetaEvents && mm.isMetaEvent()))
		{
			destSequence.addEvent (mm);
		}
	}
}

void MidiMessageSequence::extractSysExMessages (MidiMessageSequence& destSequence) const
{
	for (int i = 0; i < list.size(); ++i)
	{
		const MidiMessage& mm = list.getUnchecked(i)->message;

		if (mm.isSysEx())
			destSequence.addEvent (mm);
	}
}

void MidiMessageSequence::deleteMidiChannelMessages (const int channelNumberToRemove)
{
	for (int i = list.size(); --i >= 0;)
		if (list.getUnchecked(i)->message.isForChannel (channelNumberToRemove))
			list.remove(i);
}

void MidiMessageSequence::deleteSysExMessages()
{
	for (int i = list.size(); --i >= 0;)
		if (list.getUnchecked(i)->message.isSysEx())
			list.remove(i);
}

void MidiMessageSequence::createControllerUpdatesForTime (const int channelNumber,
														  const double time,
														  OwnedArray<MidiMessage>& dest)
{
	bool doneProg = false;
	bool donePitchWheel = false;
	Array <int> doneControllers;
	doneControllers.ensureStorageAllocated (32);

	for (int i = list.size(); --i >= 0;)
	{
		const MidiMessage& mm = list.getUnchecked(i)->message;

		if (mm.isForChannel (channelNumber)
			 && mm.getTimeStamp() <= time)
		{
			if (mm.isProgramChange())
			{
				if (! doneProg)
				{
					dest.add (new MidiMessage (mm, 0.0));
					doneProg = true;
				}
			}
			else if (mm.isController())
			{
				if (! doneControllers.contains (mm.getControllerNumber()))
				{
					dest.add (new MidiMessage (mm, 0.0));
					doneControllers.add (mm.getControllerNumber());
				}
			}
			else if (mm.isPitchWheel())
			{
				if (! donePitchWheel)
				{
					dest.add (new MidiMessage (mm, 0.0));
					donePitchWheel = true;
				}
			}
		}
	}
}

MidiMessageSequence::MidiEventHolder::MidiEventHolder (const MidiMessage& message_)
   : message (message_),
	 noteOffObject (nullptr)
{
}

MidiMessageSequence::MidiEventHolder::~MidiEventHolder()
{
}

/*** End of inlined file: juce_MidiMessageSequence.cpp ***/


/*** Start of inlined file: juce_BufferingAudioSource.cpp ***/
BufferingAudioSource::BufferingAudioSource (PositionableAudioSource* source_,
											TimeSliceThread& backgroundThread_,
											const bool deleteSourceWhenDeleted,
											const int numberOfSamplesToBuffer_,
											const int numberOfChannels_)
	: source (source_, deleteSourceWhenDeleted),
	  backgroundThread (backgroundThread_),
	  numberOfSamplesToBuffer (jmax (1024, numberOfSamplesToBuffer_)),
	  numberOfChannels (numberOfChannels_),
	  buffer (numberOfChannels_, 0),
	  bufferValidStart (0),
	  bufferValidEnd (0),
	  nextPlayPos (0),
	  wasSourceLooping (false),
	  isPrepared (false)
{
	jassert (source_ != nullptr);

	jassert (numberOfSamplesToBuffer_ > 1024); // not much point using this class if you're
											   //  not using a larger buffer..
}

BufferingAudioSource::~BufferingAudioSource()
{
	releaseResources();
}

void BufferingAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate_)
{
	const int bufferSizeNeeded = jmax (samplesPerBlockExpected * 2, numberOfSamplesToBuffer);

	if (sampleRate_ != sampleRate
		 || bufferSizeNeeded != buffer.getNumSamples()
		 || ! isPrepared)
	{
		backgroundThread.removeTimeSliceClient (this);

		isPrepared = true;
		sampleRate = sampleRate_;

		source->prepareToPlay (samplesPerBlockExpected, sampleRate_);

		buffer.setSize (numberOfChannels, bufferSizeNeeded);
		buffer.clear();

		bufferValidStart = 0;
		bufferValidEnd = 0;

		backgroundThread.addTimeSliceClient (this);

		while (bufferValidEnd - bufferValidStart < jmin (((int) sampleRate_) / 4,
														 buffer.getNumSamples() / 2))
		{
			backgroundThread.moveToFrontOfQueue (this);
			Thread::sleep (5);
		}
	}
}

void BufferingAudioSource::releaseResources()
{
	isPrepared = false;
	backgroundThread.removeTimeSliceClient (this);

	buffer.setSize (numberOfChannels, 0);
	source->releaseResources();
}

void BufferingAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
	const ScopedLock sl (bufferStartPosLock);

	const int validStart = (int) (jlimit (bufferValidStart, bufferValidEnd, nextPlayPos) - nextPlayPos);
	const int validEnd   = (int) (jlimit (bufferValidStart, bufferValidEnd, nextPlayPos + info.numSamples) - nextPlayPos);

	if (validStart == validEnd)
	{
		// total cache miss
		info.clearActiveBufferRegion();
	}
	else
	{
		if (validStart > 0)
			info.buffer->clear (info.startSample, validStart);  // partial cache miss at start

		if (validEnd < info.numSamples)
			info.buffer->clear (info.startSample + validEnd,
								info.numSamples - validEnd);    // partial cache miss at end

		if (validStart < validEnd)
		{
			for (int chan = jmin (numberOfChannels, info.buffer->getNumChannels()); --chan >= 0;)
			{
				jassert (buffer.getNumSamples() > 0);
				const int startBufferIndex = (int) ((validStart + nextPlayPos) % buffer.getNumSamples());
				const int endBufferIndex   = (int) ((validEnd + nextPlayPos)   % buffer.getNumSamples());

				if (startBufferIndex < endBufferIndex)
				{
					info.buffer->copyFrom (chan, info.startSample + validStart,
										   buffer,
										   chan, startBufferIndex,
										   validEnd - validStart);
				}
				else
				{
					const int initialSize = buffer.getNumSamples() - startBufferIndex;

					info.buffer->copyFrom (chan, info.startSample + validStart,
										   buffer,
										   chan, startBufferIndex,
										   initialSize);

					info.buffer->copyFrom (chan, info.startSample + validStart + initialSize,
										   buffer,
										   chan, 0,
										   (validEnd - validStart) - initialSize);
				}
			}
		}

		nextPlayPos += info.numSamples;
	}
}

int64 BufferingAudioSource::getNextReadPosition() const
{
	jassert (source->getTotalLength() > 0);
	return (source->isLooping() && nextPlayPos > 0)
					? nextPlayPos % source->getTotalLength()
					: nextPlayPos;
}

void BufferingAudioSource::setNextReadPosition (int64 newPosition)
{
	const ScopedLock sl (bufferStartPosLock);

	nextPlayPos = newPosition;
	backgroundThread.moveToFrontOfQueue (this);
}

bool BufferingAudioSource::readNextBufferChunk()
{
	int64 newBVS, newBVE, sectionToReadStart, sectionToReadEnd;

	{
		const ScopedLock sl (bufferStartPosLock);

		if (wasSourceLooping != isLooping())
		{
			wasSourceLooping = isLooping();
			bufferValidStart = 0;
			bufferValidEnd = 0;
		}

		newBVS = jmax ((int64) 0, nextPlayPos);
		newBVE = newBVS + buffer.getNumSamples() - 4;
		sectionToReadStart = 0;
		sectionToReadEnd = 0;

		const int maxChunkSize = 2048;

		if (newBVS < bufferValidStart || newBVS >= bufferValidEnd)
		{
			newBVE = jmin (newBVE, newBVS + maxChunkSize);

			sectionToReadStart = newBVS;
			sectionToReadEnd = newBVE;

			bufferValidStart = 0;
			bufferValidEnd = 0;
		}
		else if (std::abs ((int) (newBVS - bufferValidStart)) > 512
				  || std::abs ((int) (newBVE - bufferValidEnd)) > 512)
		{
			newBVE = jmin (newBVE, bufferValidEnd + maxChunkSize);

			sectionToReadStart = bufferValidEnd;
			sectionToReadEnd = newBVE;

			bufferValidStart = newBVS;
			bufferValidEnd = jmin (bufferValidEnd, newBVE);
		}
	}

	if (sectionToReadStart != sectionToReadEnd)
	{
		jassert (buffer.getNumSamples() > 0);
		const int bufferIndexStart = (int) (sectionToReadStart % buffer.getNumSamples());
		const int bufferIndexEnd   = (int) (sectionToReadEnd   % buffer.getNumSamples());

		if (bufferIndexStart < bufferIndexEnd)
		{
			readBufferSection (sectionToReadStart,
							   (int) (sectionToReadEnd - sectionToReadStart),
							   bufferIndexStart);
		}
		else
		{
			const int initialSize = buffer.getNumSamples() - bufferIndexStart;

			readBufferSection (sectionToReadStart,
							   initialSize,
							   bufferIndexStart);

			readBufferSection (sectionToReadStart + initialSize,
							   (int) (sectionToReadEnd - sectionToReadStart) - initialSize,
							   0);
		}

		const ScopedLock sl2 (bufferStartPosLock);

		bufferValidStart = newBVS;
		bufferValidEnd = newBVE;

		return true;
	}
	else
	{
		return false;
	}
}

void BufferingAudioSource::readBufferSection (const int64 start, const int length, const int bufferOffset)
{
	if (source->getNextReadPosition() != start)
		source->setNextReadPosition (start);

	AudioSourceChannelInfo info (&buffer, bufferOffset, length);
	source->getNextAudioBlock (info);
}

int BufferingAudioSource::useTimeSlice()
{
	return readNextBufferChunk() ? 1 : 100;
}

/*** End of inlined file: juce_BufferingAudioSource.cpp ***/


/*** Start of inlined file: juce_ChannelRemappingAudioSource.cpp ***/
ChannelRemappingAudioSource::ChannelRemappingAudioSource (AudioSource* const source_,
														  const bool deleteSourceWhenDeleted)
   : source (source_, deleteSourceWhenDeleted),
	 requiredNumberOfChannels (2),
	 buffer (2, 16)
{
	remappedInfo.buffer = &buffer;
	remappedInfo.startSample = 0;
}

ChannelRemappingAudioSource::~ChannelRemappingAudioSource() {}

void ChannelRemappingAudioSource::setNumberOfChannelsToProduce (const int requiredNumberOfChannels_)
{
	const ScopedLock sl (lock);
	requiredNumberOfChannels = requiredNumberOfChannels_;
}

void ChannelRemappingAudioSource::clearAllMappings()
{
	const ScopedLock sl (lock);

	remappedInputs.clear();
	remappedOutputs.clear();
}

void ChannelRemappingAudioSource::setInputChannelMapping (const int destIndex, const int sourceIndex)
{
	const ScopedLock sl (lock);

	while (remappedInputs.size() < destIndex)
		remappedInputs.add (-1);

	remappedInputs.set (destIndex, sourceIndex);
}

void ChannelRemappingAudioSource::setOutputChannelMapping (const int sourceIndex, const int destIndex)
{
	const ScopedLock sl (lock);

	while (remappedOutputs.size() < sourceIndex)
		remappedOutputs.add (-1);

	remappedOutputs.set (sourceIndex, destIndex);
}

int ChannelRemappingAudioSource::getRemappedInputChannel (const int inputChannelIndex) const
{
	const ScopedLock sl (lock);

	if (inputChannelIndex >= 0 && inputChannelIndex < remappedInputs.size())
		return remappedInputs.getUnchecked (inputChannelIndex);

	return -1;
}

int ChannelRemappingAudioSource::getRemappedOutputChannel (const int outputChannelIndex) const
{
	const ScopedLock sl (lock);

	if (outputChannelIndex >= 0 && outputChannelIndex < remappedOutputs.size())
		return remappedOutputs .getUnchecked (outputChannelIndex);

	return -1;
}

void ChannelRemappingAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	source->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void ChannelRemappingAudioSource::releaseResources()
{
	source->releaseResources();
}

void ChannelRemappingAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	const ScopedLock sl (lock);

	buffer.setSize (requiredNumberOfChannels, bufferToFill.numSamples, false, false, true);

	const int numChans = bufferToFill.buffer->getNumChannels();

	int i;
	for (i = 0; i < buffer.getNumChannels(); ++i)
	{
		const int remappedChan = getRemappedInputChannel (i);

		if (remappedChan >= 0 && remappedChan < numChans)
		{
			buffer.copyFrom (i, 0, *bufferToFill.buffer,
							 remappedChan,
							 bufferToFill.startSample,
							 bufferToFill.numSamples);
		}
		else
		{
			buffer.clear (i, 0, bufferToFill.numSamples);
		}
	}

	remappedInfo.numSamples = bufferToFill.numSamples;

	source->getNextAudioBlock (remappedInfo);

	bufferToFill.clearActiveBufferRegion();

	for (i = 0; i < requiredNumberOfChannels; ++i)
	{
		const int remappedChan = getRemappedOutputChannel (i);

		if (remappedChan >= 0 && remappedChan < numChans)
		{
			bufferToFill.buffer->addFrom (remappedChan, bufferToFill.startSample,
										  buffer, i, 0, bufferToFill.numSamples);

		}
	}
}

XmlElement* ChannelRemappingAudioSource::createXml() const
{
	XmlElement* e = new XmlElement ("MAPPINGS");

	String ins, outs;
	int i;

	const ScopedLock sl (lock);

	for (i = 0; i < remappedInputs.size(); ++i)
		ins << remappedInputs.getUnchecked(i) << ' ';

	for (i = 0; i < remappedOutputs.size(); ++i)
		outs << remappedOutputs.getUnchecked(i) << ' ';

	e->setAttribute ("inputs", ins.trimEnd());
	e->setAttribute ("outputs", outs.trimEnd());

	return e;
}

void ChannelRemappingAudioSource::restoreFromXml (const XmlElement& e)
{
	if (e.hasTagName ("MAPPINGS"))
	{
		const ScopedLock sl (lock);

		clearAllMappings();

		StringArray ins, outs;
		ins.addTokens (e.getStringAttribute ("inputs"), false);
		outs.addTokens (e.getStringAttribute ("outputs"), false);

		int i;
		for (i = 0; i < ins.size(); ++i)
			remappedInputs.add (ins[i].getIntValue());

		for (i = 0; i < outs.size(); ++i)
			remappedOutputs.add (outs[i].getIntValue());
	}
}

/*** End of inlined file: juce_ChannelRemappingAudioSource.cpp ***/


/*** Start of inlined file: juce_IIRFilterAudioSource.cpp ***/
IIRFilterAudioSource::IIRFilterAudioSource (AudioSource* const inputSource,
											const bool deleteInputWhenDeleted)
	: input (inputSource, deleteInputWhenDeleted)
{
	jassert (inputSource != nullptr);

	for (int i = 2; --i >= 0;)
		iirFilters.add (new IIRFilter());
}

IIRFilterAudioSource::~IIRFilterAudioSource()  {}

void IIRFilterAudioSource::setFilterParameters (const IIRFilter& newSettings)
{
	for (int i = iirFilters.size(); --i >= 0;)
		iirFilters.getUnchecked(i)->copyCoefficientsFrom (newSettings);
}

void IIRFilterAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	input->prepareToPlay (samplesPerBlockExpected, sampleRate);

	for (int i = iirFilters.size(); --i >= 0;)
		iirFilters.getUnchecked(i)->reset();
}

void IIRFilterAudioSource::releaseResources()
{
	input->releaseResources();
}

void IIRFilterAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	input->getNextAudioBlock (bufferToFill);

	const int numChannels = bufferToFill.buffer->getNumChannels();

	while (numChannels > iirFilters.size())
		iirFilters.add (new IIRFilter (*iirFilters.getUnchecked (0)));

	for (int i = 0; i < numChannels; ++i)
		iirFilters.getUnchecked(i)
			->processSamples (bufferToFill.buffer->getSampleData (i, bufferToFill.startSample),
							  bufferToFill.numSamples);
}

/*** End of inlined file: juce_IIRFilterAudioSource.cpp ***/


/*** Start of inlined file: juce_MixerAudioSource.cpp ***/
MixerAudioSource::MixerAudioSource()
	: tempBuffer (2, 0),
	  currentSampleRate (0.0),
	  bufferSizeExpected (0)
{
}

MixerAudioSource::~MixerAudioSource()
{
	removeAllInputs();
}

void MixerAudioSource::addInputSource (AudioSource* input, const bool deleteWhenRemoved)
{
	if (input != nullptr && ! inputs.contains (input))
	{
		double localRate;
		int localBufferSize;

		{
			const ScopedLock sl (lock);
			localRate = currentSampleRate;
			localBufferSize = bufferSizeExpected;
		}

		if (localRate > 0.0)
			input->prepareToPlay (localBufferSize, localRate);

		const ScopedLock sl (lock);

		inputsToDelete.setBit (inputs.size(), deleteWhenRemoved);
		inputs.add (input);
	}
}

void MixerAudioSource::removeInputSource (AudioSource* const input)
{
	if (input != nullptr)
	{
		ScopedPointer<AudioSource> toDelete;

		{
			const ScopedLock sl (lock);
			const int index = inputs.indexOf (input);

			if (index < 0)
				return;

			if (inputsToDelete [index])
				toDelete = input;

			inputsToDelete.shiftBits (index, 1);
			inputs.remove (index);
		}

		input->releaseResources();
	}
}

void MixerAudioSource::removeAllInputs()
{
	OwnedArray<AudioSource> toDelete;

	{
		const ScopedLock sl (lock);

		for (int i = inputs.size(); --i >= 0;)
			if (inputsToDelete[i])
				toDelete.add (inputs.getUnchecked(i));

		inputs.clear();
	}

	for (int i = toDelete.size(); --i >= 0;)
		toDelete.getUnchecked(i)->releaseResources();
}

void MixerAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	tempBuffer.setSize (2, samplesPerBlockExpected);

	const ScopedLock sl (lock);

	currentSampleRate = sampleRate;
	bufferSizeExpected = samplesPerBlockExpected;

	for (int i = inputs.size(); --i >= 0;)
		inputs.getUnchecked(i)->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MixerAudioSource::releaseResources()
{
	const ScopedLock sl (lock);

	for (int i = inputs.size(); --i >= 0;)
		inputs.getUnchecked(i)->releaseResources();

	tempBuffer.setSize (2, 0);

	currentSampleRate = 0;
	bufferSizeExpected = 0;
}

void MixerAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
	const ScopedLock sl (lock);

	if (inputs.size() > 0)
	{
		inputs.getUnchecked(0)->getNextAudioBlock (info);

		if (inputs.size() > 1)
		{
			tempBuffer.setSize (jmax (1, info.buffer->getNumChannels()),
								info.buffer->getNumSamples());

			AudioSourceChannelInfo info2 (&tempBuffer, 0, info.numSamples);

			for (int i = 1; i < inputs.size(); ++i)
			{
				inputs.getUnchecked(i)->getNextAudioBlock (info2);

				for (int chan = 0; chan < info.buffer->getNumChannels(); ++chan)
					info.buffer->addFrom (chan, info.startSample, tempBuffer, chan, 0, info.numSamples);
			}
		}
	}
	else
	{
		info.clearActiveBufferRegion();
	}
}

/*** End of inlined file: juce_MixerAudioSource.cpp ***/


/*** Start of inlined file: juce_ResamplingAudioSource.cpp ***/
ResamplingAudioSource::ResamplingAudioSource (AudioSource* const inputSource,
											  const bool deleteInputWhenDeleted,
											  const int numChannels_)
	: input (inputSource, deleteInputWhenDeleted),
	  ratio (1.0),
	  lastRatio (1.0),
	  buffer (numChannels_, 0),
	  sampsInBuffer (0),
	  numChannels (numChannels_)
{
	jassert (input != nullptr);
}

ResamplingAudioSource::~ResamplingAudioSource() {}

void ResamplingAudioSource::setResamplingRatio (const double samplesInPerOutputSample)
{
	jassert (samplesInPerOutputSample > 0);

	const SpinLock::ScopedLockType sl (ratioLock);
	ratio = jmax (0.0, samplesInPerOutputSample);
}

void ResamplingAudioSource::prepareToPlay (int samplesPerBlockExpected,
										   double sampleRate)
{
	const SpinLock::ScopedLockType sl (ratioLock);

	input->prepareToPlay (samplesPerBlockExpected, sampleRate);

	buffer.setSize (numChannels, roundToInt (samplesPerBlockExpected * ratio) + 32);
	buffer.clear();
	sampsInBuffer = 0;
	bufferPos = 0;
	subSampleOffset = 0.0;

	filterStates.calloc ((size_t) numChannels);
	srcBuffers.calloc ((size_t) numChannels);
	destBuffers.calloc ((size_t) numChannels);
	createLowPass (ratio);
	resetFilters();
}

void ResamplingAudioSource::releaseResources()
{
	input->releaseResources();
	buffer.setSize (numChannels, 0);
}

void ResamplingAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
	double localRatio;

	{
		const SpinLock::ScopedLockType sl (ratioLock);
		localRatio = ratio;
	}

	if (lastRatio != localRatio)
	{
		createLowPass (localRatio);
		lastRatio = localRatio;
	}

	const int sampsNeeded = roundToInt (info.numSamples * localRatio) + 2;

	int bufferSize = buffer.getNumSamples();

	if (bufferSize < sampsNeeded + 8)
	{
		bufferPos %= bufferSize;
		bufferSize = sampsNeeded + 32;
		buffer.setSize (buffer.getNumChannels(), bufferSize, true, true);
	}

	bufferPos %= bufferSize;

	int endOfBufferPos = bufferPos + sampsInBuffer;
	const int channelsToProcess = jmin (numChannels, info.buffer->getNumChannels());

	while (sampsNeeded > sampsInBuffer)
	{
		endOfBufferPos %= bufferSize;

		int numToDo = jmin (sampsNeeded - sampsInBuffer,
							bufferSize - endOfBufferPos);

		AudioSourceChannelInfo readInfo (&buffer, endOfBufferPos, numToDo);
		input->getNextAudioBlock (readInfo);

		if (localRatio > 1.0001)
		{
			// for down-sampling, pre-apply the filter..

			for (int i = channelsToProcess; --i >= 0;)
				applyFilter (buffer.getSampleData (i, endOfBufferPos), numToDo, filterStates[i]);
		}

		sampsInBuffer += numToDo;
		endOfBufferPos += numToDo;
	}

	for (int channel = 0; channel < channelsToProcess; ++channel)
	{
		destBuffers[channel] = info.buffer->getSampleData (channel, info.startSample);
		srcBuffers[channel] = buffer.getSampleData (channel, 0);
	}

	int nextPos = (bufferPos + 1) % bufferSize;
	for (int m = info.numSamples; --m >= 0;)
	{
		const float alpha = (float) subSampleOffset;
		const float invAlpha = 1.0f - alpha;

		for (int channel = 0; channel < channelsToProcess; ++channel)
			*destBuffers[channel]++ = srcBuffers[channel][bufferPos] * invAlpha + srcBuffers[channel][nextPos] * alpha;

		subSampleOffset += localRatio;

		jassert (sampsInBuffer > 0);

		while (subSampleOffset >= 1.0)
		{
			if (++bufferPos >= bufferSize)
				bufferPos = 0;

			--sampsInBuffer;

			nextPos = (bufferPos + 1) % bufferSize;
			subSampleOffset -= 1.0;
		}
	}

	if (localRatio < 0.9999)
	{
		// for up-sampling, apply the filter after transposing..
		for (int i = channelsToProcess; --i >= 0;)
			applyFilter (info.buffer->getSampleData (i, info.startSample), info.numSamples, filterStates[i]);
	}
	else if (localRatio <= 1.0001 && info.numSamples > 0)
	{
		// if the filter's not currently being applied, keep it stoked with the last couple of samples to avoid discontinuities
		for (int i = channelsToProcess; --i >= 0;)
		{
			const float* const endOfBuffer = info.buffer->getSampleData (i, info.startSample + info.numSamples - 1);
			FilterState& fs = filterStates[i];

			if (info.numSamples > 1)
			{
				fs.y2 = fs.x2 = *(endOfBuffer - 1);
			}
			else
			{
				fs.y2 = fs.y1;
				fs.x2 = fs.x1;
			}

			fs.y1 = fs.x1 = *endOfBuffer;
		}
	}

	jassert (sampsInBuffer >= 0);
}

void ResamplingAudioSource::createLowPass (const double frequencyRatio)
{
	const double proportionalRate = (frequencyRatio > 1.0) ? 0.5 / frequencyRatio
														   : 0.5 * frequencyRatio;

	const double n = 1.0 / std::tan (double_Pi * jmax (0.001, proportionalRate));
	const double nSquared = n * n;
	const double c1 = 1.0 / (1.0 + std::sqrt (2.0) * n + nSquared);

	setFilterCoefficients (c1,
						   c1 * 2.0f,
						   c1,
						   1.0,
						   c1 * 2.0 * (1.0 - nSquared),
						   c1 * (1.0 - std::sqrt (2.0) * n + nSquared));
}

void ResamplingAudioSource::setFilterCoefficients (double c1, double c2, double c3, double c4, double c5, double c6)
{
	const double a = 1.0 / c4;

	c1 *= a;
	c2 *= a;
	c3 *= a;
	c5 *= a;
	c6 *= a;

	coefficients[0] = c1;
	coefficients[1] = c2;
	coefficients[2] = c3;
	coefficients[3] = c4;
	coefficients[4] = c5;
	coefficients[5] = c6;
}

void ResamplingAudioSource::resetFilters()
{
	filterStates.clear ((size_t) numChannels);
}

void ResamplingAudioSource::applyFilter (float* samples, int num, FilterState& fs)
{
	while (--num >= 0)
	{
		const double in = *samples;

		double out = coefficients[0] * in
					 + coefficients[1] * fs.x1
					 + coefficients[2] * fs.x2
					 - coefficients[4] * fs.y1
					 - coefficients[5] * fs.y2;

	   #if JUCE_INTEL
		if (! (out < -1.0e-8 || out > 1.0e-8))
			out = 0;
	   #endif

		fs.x2 = fs.x1;
		fs.x1 = in;
		fs.y2 = fs.y1;
		fs.y1 = out;

		*samples++ = (float) out;
	}
}

/*** End of inlined file: juce_ResamplingAudioSource.cpp ***/


/*** Start of inlined file: juce_ReverbAudioSource.cpp ***/
ReverbAudioSource::ReverbAudioSource (AudioSource* const inputSource, const bool deleteInputWhenDeleted)
   : input (inputSource, deleteInputWhenDeleted),
	 bypass (false)
{
	jassert (inputSource != nullptr);
}

ReverbAudioSource::~ReverbAudioSource() {}

void ReverbAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	const ScopedLock sl (lock);
	input->prepareToPlay (samplesPerBlockExpected, sampleRate);
	reverb.setSampleRate (sampleRate);
}

void ReverbAudioSource::releaseResources() {}

void ReverbAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	const ScopedLock sl (lock);

	input->getNextAudioBlock (bufferToFill);

	if (! bypass)
	{
		float* const firstChannel = bufferToFill.buffer->getSampleData (0, bufferToFill.startSample);

		if (bufferToFill.buffer->getNumChannels() > 1)
		{
			reverb.processStereo (firstChannel,
								  bufferToFill.buffer->getSampleData (1, bufferToFill.startSample),
								  bufferToFill.numSamples);
		}
		else
		{
			reverb.processMono (firstChannel, bufferToFill.numSamples);
		}
	}
}

void ReverbAudioSource::setParameters (const Reverb::Parameters& newParams)
{
	const ScopedLock sl (lock);
	reverb.setParameters (newParams);
}

void ReverbAudioSource::setBypassed (bool b) noexcept
{
	if (bypass != b)
	{
		const ScopedLock sl (lock);
		bypass = b;
		reverb.reset();
	}
}

/*** End of inlined file: juce_ReverbAudioSource.cpp ***/


/*** Start of inlined file: juce_ToneGeneratorAudioSource.cpp ***/
ToneGeneratorAudioSource::ToneGeneratorAudioSource()
	: frequency (1000.0),
	  sampleRate (44100.0),
	  currentPhase (0.0),
	  phasePerSample (0.0),
	  amplitude (0.5f)
{
}

ToneGeneratorAudioSource::~ToneGeneratorAudioSource()
{
}

void ToneGeneratorAudioSource::setAmplitude (const float newAmplitude)
{
	amplitude = newAmplitude;
}

void ToneGeneratorAudioSource::setFrequency (const double newFrequencyHz)
{
	frequency = newFrequencyHz;
	phasePerSample = 0.0;
}

void ToneGeneratorAudioSource::prepareToPlay (int /*samplesPerBlockExpected*/,
											  double sampleRate_)
{
	currentPhase = 0.0;
	phasePerSample = 0.0;
	sampleRate = sampleRate_;
}

void ToneGeneratorAudioSource::releaseResources()
{
}

void ToneGeneratorAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
	if (phasePerSample == 0.0)
		phasePerSample = double_Pi * 2.0 / (sampleRate / frequency);

	for (int i = 0; i < info.numSamples; ++i)
	{
		const float sample = amplitude * (float) std::sin (currentPhase);
		currentPhase += phasePerSample;

		for (int j = info.buffer->getNumChannels(); --j >= 0;)
			*info.buffer->getSampleData (j, info.startSample + i) = sample;
	}
}

/*** End of inlined file: juce_ToneGeneratorAudioSource.cpp ***/


/*** Start of inlined file: juce_Synthesiser.cpp ***/
SynthesiserSound::SynthesiserSound()
{
}

SynthesiserSound::~SynthesiserSound()
{
}

SynthesiserVoice::SynthesiserVoice()
	: currentSampleRate (44100.0),
	  currentlyPlayingNote (-1),
	  noteOnTime (0),
	  keyIsDown (false),
	  sostenutoPedalDown (false)
{
}

SynthesiserVoice::~SynthesiserVoice()
{
}

bool SynthesiserVoice::isPlayingChannel (const int midiChannel) const
{
	return currentlyPlayingSound != nullptr
			&& currentlyPlayingSound->appliesToChannel (midiChannel);
}

void SynthesiserVoice::setCurrentPlaybackSampleRate (const double newRate)
{
	currentSampleRate = newRate;
}

void SynthesiserVoice::clearCurrentNote()
{
	currentlyPlayingNote = -1;
	currentlyPlayingSound = nullptr;
}

Synthesiser::Synthesiser()
	: sampleRate (0),
	  lastNoteOnCounter (0),
	  shouldStealNotes (true)
{
	for (int i = 0; i < numElementsInArray (lastPitchWheelValues); ++i)
		lastPitchWheelValues[i] = 0x2000;
}

Synthesiser::~Synthesiser()
{
}

SynthesiserVoice* Synthesiser::getVoice (const int index) const
{
	const ScopedLock sl (lock);
	return voices [index];
}

void Synthesiser::clearVoices()
{
	const ScopedLock sl (lock);
	voices.clear();
}

void Synthesiser::addVoice (SynthesiserVoice* const newVoice)
{
	const ScopedLock sl (lock);
	voices.add (newVoice);
}

void Synthesiser::removeVoice (const int index)
{
	const ScopedLock sl (lock);
	voices.remove (index);
}

void Synthesiser::clearSounds()
{
	const ScopedLock sl (lock);
	sounds.clear();
}

void Synthesiser::addSound (const SynthesiserSound::Ptr& newSound)
{
	const ScopedLock sl (lock);
	sounds.add (newSound);
}

void Synthesiser::removeSound (const int index)
{
	const ScopedLock sl (lock);
	sounds.remove (index);
}

void Synthesiser::setNoteStealingEnabled (const bool shouldStealNotes_)
{
	shouldStealNotes = shouldStealNotes_;
}

void Synthesiser::setCurrentPlaybackSampleRate (const double newRate)
{
	if (sampleRate != newRate)
	{
		const ScopedLock sl (lock);

		allNotesOff (0, false);

		sampleRate = newRate;

		for (int i = voices.size(); --i >= 0;)
			voices.getUnchecked (i)->setCurrentPlaybackSampleRate (newRate);
	}
}

void Synthesiser::renderNextBlock (AudioSampleBuffer& outputBuffer,
								   const MidiBuffer& midiData,
								   int startSample,
								   int numSamples)
{
	// must set the sample rate before using this!
	jassert (sampleRate != 0);

	const ScopedLock sl (lock);

	MidiBuffer::Iterator midiIterator (midiData);
	midiIterator.setNextSamplePosition (startSample);
	MidiMessage m (0xf4, 0.0);

	while (numSamples > 0)
	{
		int midiEventPos;
		const bool useEvent = midiIterator.getNextEvent (m, midiEventPos)
								&& midiEventPos < startSample + numSamples;

		const int numThisTime = useEvent ? midiEventPos - startSample
										 : numSamples;

		if (numThisTime > 0)
		{
			for (int i = voices.size(); --i >= 0;)
				voices.getUnchecked (i)->renderNextBlock (outputBuffer, startSample, numThisTime);
		}

		if (useEvent)
			handleMidiEvent (m);

		startSample += numThisTime;
		numSamples -= numThisTime;
	}
}

void Synthesiser::handleMidiEvent (const MidiMessage& m)
{
	if (m.isNoteOn())
	{
		noteOn (m.getChannel(),
				m.getNoteNumber(),
				m.getFloatVelocity());
	}
	else if (m.isNoteOff())
	{
		noteOff (m.getChannel(),
				 m.getNoteNumber(),
				 true);
	}
	else if (m.isAllNotesOff() || m.isAllSoundOff())
	{
		allNotesOff (m.getChannel(), true);
	}
	else if (m.isPitchWheel())
	{
		const int channel = m.getChannel();
		const int wheelPos = m.getPitchWheelValue();
		lastPitchWheelValues [channel - 1] = wheelPos;

		handlePitchWheel (channel, wheelPos);
	}
	else if (m.isController())
	{
		handleController (m.getChannel(),
						  m.getControllerNumber(),
						  m.getControllerValue());
	}
}

void Synthesiser::noteOn (const int midiChannel,
						  const int midiNoteNumber,
						  const float velocity)
{
	const ScopedLock sl (lock);

	for (int i = sounds.size(); --i >= 0;)
	{
		SynthesiserSound* const sound = sounds.getUnchecked(i);

		if (sound->appliesToNote (midiNoteNumber)
			 && sound->appliesToChannel (midiChannel))
		{
			// If hitting a note that's still ringing, stop it first (it could be
			// still playing because of the sustain or sostenuto pedal).
			for (int j = voices.size(); --j >= 0;)
			{
				SynthesiserVoice* const voice = voices.getUnchecked (j);

				if (voice->getCurrentlyPlayingNote() == midiNoteNumber
					 && voice->isPlayingChannel (midiChannel))
					stopVoice (voice, true);
			}

			startVoice (findFreeVoice (sound, shouldStealNotes),
						sound, midiChannel, midiNoteNumber, velocity);
		}
	}
}

void Synthesiser::startVoice (SynthesiserVoice* const voice,
							  SynthesiserSound* const sound,
							  const int midiChannel,
							  const int midiNoteNumber,
							  const float velocity)
{
	if (voice != nullptr && sound != nullptr)
	{
		if (voice->currentlyPlayingSound != nullptr)
			voice->stopNote (false);

		voice->startNote (midiNoteNumber, velocity, sound,
						  lastPitchWheelValues [midiChannel - 1]);

		voice->currentlyPlayingNote = midiNoteNumber;
		voice->noteOnTime = ++lastNoteOnCounter;
		voice->currentlyPlayingSound = sound;
		voice->keyIsDown = true;
		voice->sostenutoPedalDown = false;
	}
}

void Synthesiser::stopVoice (SynthesiserVoice* voice, const bool allowTailOff)
{
	jassert (voice != nullptr);

	voice->stopNote (allowTailOff);

	// the subclass MUST call clearCurrentNote() if it's not tailing off! RTFM for stopNote()!
	jassert (allowTailOff || (voice->getCurrentlyPlayingNote() < 0 && voice->getCurrentlyPlayingSound() == 0));
}

void Synthesiser::noteOff (const int midiChannel,
						   const int midiNoteNumber,
						   const bool allowTailOff)
{
	const ScopedLock sl (lock);

	for (int i = voices.size(); --i >= 0;)
	{
		SynthesiserVoice* const voice = voices.getUnchecked (i);

		if (voice->getCurrentlyPlayingNote() == midiNoteNumber)
		{
			SynthesiserSound* const sound = voice->getCurrentlyPlayingSound();

			if (sound != nullptr
				 && sound->appliesToNote (midiNoteNumber)
				 && sound->appliesToChannel (midiChannel))
			{
				voice->keyIsDown = false;

				if (! (sustainPedalsDown [midiChannel] || voice->sostenutoPedalDown))
					stopVoice (voice, allowTailOff);
			}
		}
	}
}

void Synthesiser::allNotesOff (const int midiChannel, const bool allowTailOff)
{
	const ScopedLock sl (lock);

	for (int i = voices.size(); --i >= 0;)
	{
		SynthesiserVoice* const voice = voices.getUnchecked (i);

		if (midiChannel <= 0 || voice->isPlayingChannel (midiChannel))
			voice->stopNote (allowTailOff);
	}

	sustainPedalsDown.clear();
}

void Synthesiser::handlePitchWheel (const int midiChannel, const int wheelValue)
{
	const ScopedLock sl (lock);

	for (int i = voices.size(); --i >= 0;)
	{
		SynthesiserVoice* const voice = voices.getUnchecked (i);

		if (midiChannel <= 0 || voice->isPlayingChannel (midiChannel))
			voice->pitchWheelMoved (wheelValue);
	}
}

void Synthesiser::handleController (const int midiChannel,
									const int controllerNumber,
									const int controllerValue)
{
	switch (controllerNumber)
	{
		case 0x40:  handleSustainPedal   (midiChannel, controllerValue >= 64); break;
		case 0x42:  handleSostenutoPedal (midiChannel, controllerValue >= 64); break;
		case 0x43:  handleSoftPedal      (midiChannel, controllerValue >= 64); break;
		default:    break;
	}

	const ScopedLock sl (lock);

	for (int i = voices.size(); --i >= 0;)
	{
		SynthesiserVoice* const voice = voices.getUnchecked (i);

		if (midiChannel <= 0 || voice->isPlayingChannel (midiChannel))
			voice->controllerMoved (controllerNumber, controllerValue);
	}
}

void Synthesiser::handleSustainPedal (int midiChannel, bool isDown)
{
	jassert (midiChannel > 0 && midiChannel <= 16);
	const ScopedLock sl (lock);

	if (isDown)
	{
		sustainPedalsDown.setBit (midiChannel);
	}
	else
	{
		for (int i = voices.size(); --i >= 0;)
		{
			SynthesiserVoice* const voice = voices.getUnchecked (i);

			if (voice->isPlayingChannel (midiChannel) && ! voice->keyIsDown)
				stopVoice (voice, true);
		}

		sustainPedalsDown.clearBit (midiChannel);
	}
}

void Synthesiser::handleSostenutoPedal (int midiChannel, bool isDown)
{
	jassert (midiChannel > 0 && midiChannel <= 16);
	const ScopedLock sl (lock);

	for (int i = voices.size(); --i >= 0;)
	{
		SynthesiserVoice* const voice = voices.getUnchecked (i);

		if (voice->isPlayingChannel (midiChannel))
		{
			if (isDown)
				voice->sostenutoPedalDown = true;
			else if (voice->sostenutoPedalDown)
				stopVoice (voice, true);
		}
	}
}

void Synthesiser::handleSoftPedal (int midiChannel, bool /*isDown*/)
{
	(void) midiChannel;
	jassert (midiChannel > 0 && midiChannel <= 16);
}

SynthesiserVoice* Synthesiser::findFreeVoice (SynthesiserSound* soundToPlay,
											  const bool stealIfNoneAvailable) const
{
	const ScopedLock sl (lock);

	for (int i = voices.size(); --i >= 0;)
		if (voices.getUnchecked (i)->getCurrentlyPlayingNote() < 0
			 && voices.getUnchecked (i)->canPlaySound (soundToPlay))
			return voices.getUnchecked (i);

	if (stealIfNoneAvailable)
	{
		// currently this just steals the one that's been playing the longest, but could be made a bit smarter..
		SynthesiserVoice* oldest = nullptr;

		for (int i = voices.size(); --i >= 0;)
		{
			SynthesiserVoice* const voice = voices.getUnchecked (i);

			if (voice->canPlaySound (soundToPlay)
				 && (oldest == nullptr || oldest->noteOnTime > voice->noteOnTime))
				oldest = voice;
		}

		jassert (oldest != nullptr);
		return oldest;
	}

	return nullptr;
}

/*** End of inlined file: juce_Synthesiser.cpp ***/

// END_AUTOINCLUDE

}

/*** End of inlined file: juce_audio_basics.cpp ***/


#endif

