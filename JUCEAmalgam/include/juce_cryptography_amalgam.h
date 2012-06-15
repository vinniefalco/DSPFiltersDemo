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


/*** Start of inlined file: juce_cryptography.h ***/
#ifndef __JUCE_CRYPTOGRAPHY_JUCEHEADER__
#define __JUCE_CRYPTOGRAPHY_JUCEHEADER__

#include "juce_core_amalgam.h"

namespace juce
{

// START_AUTOINCLUDE encryption, hashing
#ifndef __JUCE_BLOWFISH_JUCEHEADER__

/*** Start of inlined file: juce_BlowFish.h ***/
#ifndef __JUCE_BLOWFISH_JUCEHEADER__
#define __JUCE_BLOWFISH_JUCEHEADER__

/**
	BlowFish encryption class.

*/
class JUCE_API  BlowFish
{
public:

	/** Creates an object that can encode/decode based on the specified key.

		The key data can be up to 72 bytes long.
	*/
	BlowFish (const void* keyData, int keyBytes);

	/** Creates a copy of another blowfish object. */
	BlowFish (const BlowFish& other);

	/** Copies another blowfish object. */
	BlowFish& operator= (const BlowFish& other) noexcept;

	/** Destructor. */
	~BlowFish() noexcept;

	/** Encrypts a pair of 32-bit integers. */
	void encrypt (uint32& data1, uint32& data2) const noexcept;

	/** Decrypts a pair of 32-bit integers. */
	void decrypt (uint32& data1, uint32& data2) const noexcept;

private:

	uint32 p[18];
	HeapBlock <uint32> s[4];

	uint32 F (uint32) const noexcept;

	JUCE_LEAK_DETECTOR (BlowFish);
};

#endif   // __JUCE_BLOWFISH_JUCEHEADER__

/*** End of inlined file: juce_BlowFish.h ***/


#endif
#ifndef __JUCE_PRIMES_JUCEHEADER__

/*** Start of inlined file: juce_Primes.h ***/
#ifndef __JUCE_PRIMES_JUCEHEADER__
#define __JUCE_PRIMES_JUCEHEADER__

/**
	Prime number creation class.

	This class contains static methods for generating and testing prime numbers.

	@see BigInteger
*/
class JUCE_API  Primes
{
public:

	/** Creates a random prime number with a given bit-length.

		The certainty parameter specifies how many iterations to use when testing
		for primality. A safe value might be anything over about 20-30.

		The randomSeeds parameter lets you optionally pass it a set of values with
		which to seed the random number generation, improving the security of the
		keys generated.
	*/
	static BigInteger createProbablePrime (int bitLength,
										   int certainty,
										   const int* randomSeeds = 0,
										   int numRandomSeeds = 0);

	/** Tests a number to see if it's prime.

		This isn't a bulletproof test, it uses a Miller-Rabin test to determine
		whether the number is prime.

		The certainty parameter specifies how many iterations to use when testing - a
		safe value might be anything over about 20-30.
	*/
	static bool isProbablyPrime (const BigInteger& number, int certainty);

private:
	Primes();

	JUCE_DECLARE_NON_COPYABLE (Primes);
};

#endif   // __JUCE_PRIMES_JUCEHEADER__

/*** End of inlined file: juce_Primes.h ***/


#endif
#ifndef __JUCE_RSAKEY_JUCEHEADER__

/*** Start of inlined file: juce_RSAKey.h ***/
#ifndef __JUCE_RSAKEY_JUCEHEADER__
#define __JUCE_RSAKEY_JUCEHEADER__

/**
	RSA public/private key-pair encryption class.

	An object of this type makes up one half of a public/private RSA key pair. Use the
	createKeyPair() method to create a matching pair for encoding/decoding.
*/
class JUCE_API  RSAKey
{
public:

	/** Creates a null key object.

		Initialise a pair of objects for use with the createKeyPair() method.
	*/
	RSAKey();

	/** Loads a key from an encoded string representation.

		This reloads a key from a string created by the toString() method.
	*/
	explicit RSAKey (const String& stringRepresentation);

	/** Destructor. */
	~RSAKey();

	bool operator== (const RSAKey& other) const noexcept;
	bool operator!= (const RSAKey& other) const noexcept;

	/** Turns the key into a string representation.

		This can be reloaded using the constructor that takes a string.
	*/
	String toString() const;

	/** Encodes or decodes a value.

		Call this on the public key object to encode some data, then use the matching
		private key object to decode it.

		Returns false if the operation couldn't be completed, e.g. if this key hasn't been
		initialised correctly.

		NOTE: This method dumbly applies this key to this data. If you encode some data
		and then try to decode it with a key that doesn't match, this method will still
		happily do its job and return true, but the result won't be what you were expecting.
		It's your responsibility to check that the result is what you wanted.
	*/
	bool applyToValue (BigInteger& value) const;

	/** Creates a public/private key-pair.

		Each key will perform one-way encryption that can only be reversed by
		using the other key.

		The numBits parameter specifies the size of key, e.g. 128, 256, 512 bit. Bigger
		sizes are more secure, but this method will take longer to execute.

		The randomSeeds parameter lets you optionally pass it a set of values with
		which to seed the random number generation, improving the security of the
		keys generated. If you supply these, make sure you provide more than 2 values,
		and the more your provide, the better the security.
	*/
	static void createKeyPair (RSAKey& publicKey,
							   RSAKey& privateKey,
							   int numBits,
							   const int* randomSeeds = nullptr,
							   int numRandomSeeds = 0);

protected:

	BigInteger part1, part2;

private:

	static BigInteger findBestCommonDivisor (const BigInteger& p, const BigInteger& q);

	JUCE_LEAK_DETECTOR (RSAKey);
};

#endif   // __JUCE_RSAKEY_JUCEHEADER__

/*** End of inlined file: juce_RSAKey.h ***/


#endif
#ifndef __JUCE_MD5_JUCEHEADER__

/*** Start of inlined file: juce_MD5.h ***/
#ifndef __JUCE_MD5_JUCEHEADER__
#define __JUCE_MD5_JUCEHEADER__

/**
	MD5 checksum class.

	Create one of these with a block of source data or a stream, and it calculates
	the MD5 checksum of that data.

	You can then retrieve this checksum as a 16-byte block, or as a hex string.
	@see SHA256
*/
class JUCE_API  MD5
{
public:

	/** Creates a null MD5 object. */
	MD5() noexcept;

	/** Creates a copy of another MD5. */
	MD5 (const MD5& other) noexcept;

	/** Copies another MD5. */
	MD5& operator= (const MD5& other) noexcept;

	/** Creates a checksum for a block of binary data. */
	explicit MD5 (const MemoryBlock& data) noexcept;

	/** Creates a checksum for a block of binary data. */
	MD5 (const void* data, size_t numBytes) noexcept;

	/** Creates a checksum for the input from a stream.

		This will read up to the given number of bytes from the stream, and produce the
		checksum of that. If the number of bytes to read is negative, it'll read
		until the stream is exhausted.
	*/
	MD5 (InputStream& input, int64 numBytesToRead = -1);

	/** Creates a checksum for a file. */
	explicit MD5 (const File& file);

	/** Creates a checksum from a UTF-8 buffer.
		E.g.
		@code MD5 checksum (myString.toUTF8());
		@endcode
	*/
	explicit MD5 (const CharPointer_UTF8& utf8Text) noexcept;

	/** Destructor. */
	~MD5() noexcept;

	/** Returns the checksum as a 16-byte block of data. */
	MemoryBlock getRawChecksumData() const;

	/** Returns a pointer to the 16-byte array of result data. */
	const uint8* getChecksumDataArray() const noexcept          { return result; }

	/** Returns the checksum as a 32-digit hex string. */
	String toHexString() const;

	/** Creates an MD5 from a little-endian UTF-32 encoded string.

		Note that this method is provided for backwards-compatibility with the old
		version of this class, which had a constructor that took a string and performed
		this operation on it. In new code, you shouldn't use this, and are recommended to
		use the constructor that takes a CharPointer_UTF8 instead.
	*/
	static MD5 fromUTF32 (const String&);

	bool operator== (const MD5&) const noexcept;
	bool operator!= (const MD5&) const noexcept;

private:

	uint8 result [16];

	void processData (const void*, size_t) noexcept;
	void processStream (InputStream&, int64);

	JUCE_LEAK_DETECTOR (MD5);
};

#endif   // __JUCE_MD5_JUCEHEADER__

/*** End of inlined file: juce_MD5.h ***/


#endif
#ifndef __JUCE_SHA256_JUCEHEADER__

/*** Start of inlined file: juce_SHA256.h ***/
#ifndef __JUCE_SHA256_JUCEHEADER__
#define __JUCE_SHA256_JUCEHEADER__

/**
	SHA-256 secure hash generator.

	Create one of these objects from a block of source data or a stream, and it
	calculates the SHA-256 hash of that data.

	You can retrieve the hash as a raw 32-byte block, or as a 64-digit hex string.
	@see MD5
*/
class JUCE_API  SHA256
{
public:

	/** Creates an empty SHA256 object.
		The default constructor just creates a hash filled with zeros. (This is not
		equal to the hash of an empty block of data).
	*/
	SHA256() noexcept;

	/** Destructor. */
	~SHA256() noexcept;

	/** Creates a copy of another SHA256. */
	SHA256 (const SHA256& other) noexcept;

	/** Copies another SHA256. */
	SHA256& operator= (const SHA256& other) noexcept;

	/** Creates a hash from a block of raw data. */
	explicit SHA256 (const MemoryBlock& data);

	/** Creates a hash from a block of raw data. */
	SHA256 (const void* data, size_t numBytes);

	/** Creates a hash from the contents of a stream.

		This will read from the stream until the stream is exhausted, or until
		maxBytesToRead bytes have been read. If maxBytesToRead is negative, the entire
		stream will be read.
	*/
	SHA256 (InputStream& input, int64 maxBytesToRead = -1);

	/** Reads a file and generates the hash of its contents.
		If the file can't be opened, the hash will be left uninitialised (i.e. full
		of zeros).
	*/
	explicit SHA256 (const File& file);

	/** Creates a checksum from a UTF-8 buffer.
		E.g.
		@code SHA256 checksum (myString.toUTF8());
		@endcode
	*/
	explicit SHA256 (const CharPointer_UTF8& utf8Text) noexcept;

	/** Returns the hash as a 32-byte block of data. */
	MemoryBlock getRawData() const;

	/** Returns the checksum as a 64-digit hex string. */
	String toHexString() const;

	bool operator== (const SHA256&) const noexcept;
	bool operator!= (const SHA256&) const noexcept;

private:

	uint8 result [32];
	void process (const void*, size_t);

	JUCE_LEAK_DETECTOR (SHA256);
};

#endif   // __JUCE_SHA256_JUCEHEADER__

/*** End of inlined file: juce_SHA256.h ***/


#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_CRYPTOGRAPHY_JUCEHEADER__

/*** End of inlined file: juce_cryptography.h ***/

