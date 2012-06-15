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


/*** Start of inlined file: juce_graphics.h ***/
#ifndef __JUCE_GRAPHICS_MODULE_JUCEHEADER__ // %%
#define __JUCE_GRAPHICS_MODULE_JUCEHEADER__

#include "juce_core_amalgam.h"
#include "juce_events_amalgam.h"

/** Config: JUCE_USE_COREIMAGE_LOADER

	On OSX, enabling this flag means that the CoreImage codecs will be used to load
	PNG/JPEG/GIF files. It is enabled by default, but you may want to disable it if
	you'd rather use libpng, libjpeg, etc.
*/
#ifndef JUCE_USE_COREIMAGE_LOADER
 #define JUCE_USE_COREIMAGE_LOADER 1
#endif

/** Config: JUCE_USE_DIRECTWRITE

	Enabling this flag means that DirectWrite will be used when available for font
	management and layout.
*/
#ifndef JUCE_USE_DIRECTWRITE
 #define JUCE_USE_DIRECTWRITE 1
#endif

#ifndef JUCE_INCLUDE_PNGLIB_CODE
 #define JUCE_INCLUDE_PNGLIB_CODE 1
#endif

#ifndef JUCE_INCLUDE_JPEGLIB_CODE
 #define JUCE_INCLUDE_JPEGLIB_CODE 1
#endif

#ifndef USE_COREGRAPHICS_RENDERING
 #define USE_COREGRAPHICS_RENDERING 1
#endif

namespace juce
{

// START_AUTOINCLUDE colour, geometry, placement, contexts, images,
// image_formats, fonts, effects
#ifndef __JUCE_COLOUR_JUCEHEADER__

/*** Start of inlined file: juce_Colour.h ***/
#ifndef __JUCE_COLOUR_JUCEHEADER__
#define __JUCE_COLOUR_JUCEHEADER__


/*** Start of inlined file: juce_PixelFormats.h ***/
#ifndef __JUCE_PIXELFORMATS_JUCEHEADER__
#define __JUCE_PIXELFORMATS_JUCEHEADER__

#ifndef DOXYGEN
 #if JUCE_MSVC
  #pragma pack (push, 1)
  #define PACKED
 #elif JUCE_GCC
  #define PACKED __attribute__((packed))
 #else
  #define PACKED
 #endif
#endif

class PixelRGB;
class PixelAlpha;

/**
	Represents a 32-bit ARGB pixel with premultiplied alpha, and can perform compositing
	operations with it.

	This is used internally by the imaging classes.

	@see PixelRGB
*/
class JUCE_API  PixelARGB
{
public:
	/** Creates a pixel without defining its colour. */
	PixelARGB() noexcept {}
	~PixelARGB() noexcept {}

	/** Creates a pixel from a 32-bit argb value.
	*/
	PixelARGB (const uint32 argb_) noexcept
		: argb (argb_)
	{
	}

	PixelARGB (const uint8 a, const uint8 r, const uint8 g, const uint8 b) noexcept
	{
		components.b = b;
		components.g = g;
		components.r = r;
		components.a = a;
	}

	forcedinline uint32 getARGB() const noexcept                { return argb; }
	forcedinline uint32 getUnpremultipliedARGB() const noexcept { PixelARGB p (argb); p.unpremultiply(); return p.getARGB(); }

	forcedinline uint32 getRB() const noexcept      { return 0x00ff00ff & argb; }
	forcedinline uint32 getAG() const noexcept      { return 0x00ff00ff & (argb >> 8); }

	forcedinline uint8 getAlpha() const noexcept    { return components.a; }
	forcedinline uint8 getRed() const noexcept      { return components.r; }
	forcedinline uint8 getGreen() const noexcept    { return components.g; }
	forcedinline uint8 getBlue() const noexcept     { return components.b; }

	/** Blends another pixel onto this one.

		This takes into account the opacity of the pixel being overlaid, and blends
		it accordingly.
	*/
	template <class Pixel>
	forcedinline void blend (const Pixel& src) noexcept
	{
		uint32 sargb = src.getARGB();
		const uint32 alpha = 0x100 - (sargb >> 24);

		sargb += 0x00ff00ff & ((getRB() * alpha) >> 8);
		sargb += 0xff00ff00 & (getAG() * alpha);

		argb = sargb;
	}

	/** Blends another pixel onto this one.

		This takes into account the opacity of the pixel being overlaid, and blends
		it accordingly.
	*/
	forcedinline void blend (const PixelRGB& src) noexcept;

	/** Blends another pixel onto this one, applying an extra multiplier to its opacity.

		The opacity of the pixel being overlaid is scaled by the extraAlpha factor before
		being used, so this can blend semi-transparently from a PixelRGB argument.
	*/
	template <class Pixel>
	forcedinline void blend (const Pixel& src, uint32 extraAlpha) noexcept
	{
		++extraAlpha;

		uint32 sargb = ((extraAlpha * src.getAG()) & 0xff00ff00)
						 | (((extraAlpha * src.getRB()) >> 8) & 0x00ff00ff);

		const uint32 alpha = 0x100 - (sargb >> 24);

		sargb += 0x00ff00ff & ((getRB() * alpha) >> 8);
		sargb += 0xff00ff00 & (getAG() * alpha);

		argb = sargb;
	}

	/** Blends another pixel with this one, creating a colour that is somewhere
		between the two, as specified by the amount.
	*/
	template <class Pixel>
	forcedinline void tween (const Pixel& src, const uint32 amount) noexcept
	{
		uint32 drb = getRB();
		drb += (((src.getRB() - drb) * amount) >> 8);
		drb &= 0x00ff00ff;

		uint32 dag = getAG();
		dag += (((src.getAG() - dag) * amount) >> 8);
		dag &= 0x00ff00ff;
		dag <<= 8;

		dag |= drb;
		argb = dag;
	}

	/** Copies another pixel colour over this one.

		This doesn't blend it - this colour is simply replaced by the other one.
	*/
	template <class Pixel>
	forcedinline void set (const Pixel& src) noexcept
	{
		argb = src.getARGB();
	}

	/** Replaces the colour's alpha value with another one. */
	forcedinline void setAlpha (const uint8 newAlpha) noexcept
	{
		components.a = newAlpha;
	}

	/** Multiplies the colour's alpha value with another one. */
	forcedinline void multiplyAlpha (int multiplier) noexcept
	{
		++multiplier;

		argb = ((((uint32) multiplier) * getAG()) & 0xff00ff00)
				| (((((uint32) multiplier) * getRB()) >> 8) & 0x00ff00ff);
	}

	forcedinline void multiplyAlpha (const float multiplier) noexcept
	{
		multiplyAlpha ((int) (multiplier * 255.0f));
	}

	/** Sets the pixel's colour from individual components. */
	void setARGB (const uint8 a, const uint8 r, const uint8 g, const uint8 b) noexcept
	{
		components.b = b;
		components.g = g;
		components.r = r;
		components.a = a;
	}

	/** Premultiplies the pixel's RGB values by its alpha. */
	forcedinline void premultiply() noexcept
	{
		const uint32 alpha = components.a;

		if (alpha < 0xff)
		{
			if (alpha == 0)
			{
				components.b = 0;
				components.g = 0;
				components.r = 0;
			}
			else
			{
				components.b = (uint8) ((components.b * alpha + 0x7f) >> 8);
				components.g = (uint8) ((components.g * alpha + 0x7f) >> 8);
				components.r = (uint8) ((components.r * alpha + 0x7f) >> 8);
			}
		}
	}

	/** Unpremultiplies the pixel's RGB values. */
	forcedinline void unpremultiply() noexcept
	{
		const uint32 alpha = components.a;

		if (alpha < 0xff)
		{
			if (alpha == 0)
			{
				components.b = 0;
				components.g = 0;
				components.r = 0;
			}
			else
			{
				components.b = (uint8) jmin ((uint32) 0xff, (components.b * 0xff) / alpha);
				components.g = (uint8) jmin ((uint32) 0xff, (components.g * 0xff) / alpha);
				components.r = (uint8) jmin ((uint32) 0xff, (components.r * 0xff) / alpha);
			}
		}
	}

	forcedinline void desaturate() noexcept
	{
		if (components.a < 0xff && components.a > 0)
		{
			const int newUnpremultipliedLevel = (0xff * ((int) components.r + (int) components.g + (int) components.b) / (3 * components.a));

			components.r = components.g = components.b
				= (uint8) ((newUnpremultipliedLevel * components.a + 0x7f) >> 8);
		}
		else
		{
			components.r = components.g = components.b
				= (uint8) (((int) components.r + (int) components.g + (int) components.b) / 3);
		}
	}

	/** Returns a uint32 which when written to memory, will be in the order r, g, b, a. */
	inline uint32 getInRGBAMemoryOrder() const noexcept
	{
	   #if JUCE_BIG_ENDIAN
		return (((uint32) components.r) << 24) | (((uint32) components.g) << 16) | (((uint32) components.b) << 8) | components.a;
	   #else
		return (((uint32) components.a) << 24) | (((uint32) components.b) << 16) | (((uint32) components.g) << 8) | components.r;
	   #endif
	}

	/** The indexes of the different components in the byte layout of this type of colour. */
   #if JUCE_BIG_ENDIAN
	enum { indexA = 0, indexR = 1, indexG = 2, indexB = 3 };
   #else
	enum { indexA = 3, indexR = 2, indexG = 1, indexB = 0 };
   #endif

private:

	struct Components
	{
	   #if JUCE_BIG_ENDIAN
		uint8 a : 8, r : 8, g : 8, b : 8;
	   #else
		uint8 b, g, r, a;
	   #endif
	} PACKED;

	union
	{
		uint32 argb;
		Components components;
	};
}
#ifndef DOXYGEN
 PACKED
#endif
;

/**
	Represents a 24-bit RGB pixel, and can perform compositing operations on it.

	This is used internally by the imaging classes.

	@see PixelARGB
*/
class JUCE_API  PixelRGB
{
public:
	/** Creates a pixel without defining its colour. */
	PixelRGB() noexcept {}
	~PixelRGB() noexcept {}

	/** Creates a pixel from a 32-bit argb value.

		(The argb format is that used by PixelARGB)
	*/
	PixelRGB (const uint32 argb) noexcept
	{
		r = (uint8) (argb >> 16);
		g = (uint8) (argb >> 8);
		b = (uint8) (argb);
	}

	forcedinline uint32 getARGB() const noexcept                { return 0xff000000 | b | (((uint32) g) << 8) | (((uint32) r) << 16); }
	forcedinline uint32 getUnpremultipliedARGB() const noexcept { return getARGB(); }

	forcedinline uint32 getRB() const noexcept      { return b | (uint32) (r << 16); }
	forcedinline uint32 getAG() const noexcept      { return (uint32) (0xff0000 | g); }

	forcedinline uint8 getAlpha() const noexcept    { return 0xff; }
	forcedinline uint8 getRed() const noexcept      { return r; }
	forcedinline uint8 getGreen() const noexcept    { return g; }
	forcedinline uint8 getBlue() const noexcept     { return b; }

	/** Blends another pixel onto this one.

		This takes into account the opacity of the pixel being overlaid, and blends
		it accordingly.
	*/
	template <class Pixel>
	forcedinline void blend (const Pixel& src) noexcept
	{
		uint32 sargb = src.getARGB();
		const uint32 alpha = 0x100 - (sargb >> 24);

		sargb += 0x00ff00ff & ((getRB() * alpha) >> 8);
		sargb += 0x0000ff00 & (g * alpha);

		r = (uint8) (sargb >> 16);
		g = (uint8) (sargb >> 8);
		b = (uint8) sargb;
	}

	forcedinline void blend (const PixelRGB& src) noexcept
	{
		set (src);
	}

	/** Blends another pixel onto this one, applying an extra multiplier to its opacity.

		The opacity of the pixel being overlaid is scaled by the extraAlpha factor before
		being used, so this can blend semi-transparently from a PixelRGB argument.
	*/
	template <class Pixel>
	forcedinline void blend (const Pixel& src, uint32 extraAlpha) noexcept
	{
		++extraAlpha;
		const uint32 srb = (extraAlpha * src.getRB()) >> 8;
		const uint32 sag = extraAlpha * src.getAG();
		uint32 sargb = (sag & 0xff00ff00) | (srb & 0x00ff00ff);

		const uint32 alpha = 0x100 - (sargb >> 24);

		sargb += 0x00ff00ff & ((getRB() * alpha) >> 8);
		sargb += 0x0000ff00 & (g * alpha);

		b = (uint8) sargb;
		g = (uint8) (sargb >> 8);
		r = (uint8) (sargb >> 16);
	}

	/** Blends another pixel with this one, creating a colour that is somewhere
		between the two, as specified by the amount.
	*/
	template <class Pixel>
	forcedinline void tween (const Pixel& src, const uint32 amount) noexcept
	{
		uint32 drb = getRB();
		drb += (((src.getRB() - drb) * amount) >> 8);

		uint32 dag = getAG();
		dag += (((src.getAG() - dag) * amount) >> 8);

		b = (uint8) drb;
		g = (uint8) dag;
		r = (uint8) (drb >> 16);
	}

	/** Copies another pixel colour over this one.

		This doesn't blend it - this colour is simply replaced by the other one.
		Because PixelRGB has no alpha channel, any alpha value in the source pixel
		is thrown away.
	*/
	template <class Pixel>
	forcedinline void set (const Pixel& src) noexcept
	{
		b = src.getBlue();
		g = src.getGreen();
		r = src.getRed();
	}

	/** This method is included for compatibility with the PixelARGB class. */
	forcedinline void setAlpha (const uint8) noexcept {}

	/** Multiplies the colour's alpha value with another one. */
	forcedinline void multiplyAlpha (int) noexcept {}

	/** Multiplies the colour's alpha value with another one. */
	forcedinline void multiplyAlpha (float) noexcept {}

	/** Sets the pixel's colour from individual components. */
	void setARGB (const uint8, const uint8 r_, const uint8 g_, const uint8 b_) noexcept
	{
		r = r_;
		g = g_;
		b = b_;
	}

	/** Premultiplies the pixel's RGB values by its alpha. */
	forcedinline void premultiply() noexcept {}

	/** Unpremultiplies the pixel's RGB values. */
	forcedinline void unpremultiply() noexcept {}

	forcedinline void desaturate() noexcept
	{
		r = g = b = (uint8) (((int) r + (int) g + (int) b) / 3);
	}

	/** The indexes of the different components in the byte layout of this type of colour. */
   #if JUCE_MAC
	enum { indexR = 0, indexG = 1, indexB = 2 };
   #else
	enum { indexR = 2, indexG = 1, indexB = 0 };
   #endif

private:

   #if JUCE_MAC
	uint8 r, g, b;
   #else
	uint8 b, g, r;
   #endif

}
#ifndef DOXYGEN
 PACKED
#endif
;

forcedinline void PixelARGB::blend (const PixelRGB& src) noexcept
{
	set (src);
}

/**
	Represents an 8-bit single-channel pixel, and can perform compositing operations on it.

	This is used internally by the imaging classes.

	@see PixelARGB, PixelRGB
*/
class JUCE_API  PixelAlpha
{
public:
	/** Creates a pixel without defining its colour. */
	PixelAlpha() noexcept {}
	~PixelAlpha() noexcept {}

	/** Creates a pixel from a 32-bit argb value.

		(The argb format is that used by PixelARGB)
	*/
	PixelAlpha (const uint32 argb) noexcept
	{
		a = (uint8) (argb >> 24);
	}

	forcedinline uint32 getARGB() const noexcept                { return (((uint32) a) << 24) | (((uint32) a) << 16) | (((uint32) a) << 8) | a; }
	forcedinline uint32 getUnpremultipliedARGB() const noexcept { return (((uint32) a) << 24) | 0xffffff; }

	forcedinline uint32 getRB() const noexcept      { return (((uint32) a) << 16) | a; }
	forcedinline uint32 getAG() const noexcept      { return (((uint32) a) << 16) | a; }

	forcedinline uint8 getAlpha() const noexcept    { return a; }
	forcedinline uint8 getRed() const noexcept      { return 0; }
	forcedinline uint8 getGreen() const noexcept    { return 0; }
	forcedinline uint8 getBlue() const noexcept     { return 0; }

	/** Blends another pixel onto this one.

		This takes into account the opacity of the pixel being overlaid, and blends
		it accordingly.
	*/
	template <class Pixel>
	forcedinline void blend (const Pixel& src) noexcept
	{
		const int srcA = src.getAlpha();
		a = (uint8) ((a * (0x100 - srcA) >> 8) + srcA);
	}

	/** Blends another pixel onto this one, applying an extra multiplier to its opacity.

		The opacity of the pixel being overlaid is scaled by the extraAlpha factor before
		being used, so this can blend semi-transparently from a PixelRGB argument.
	*/
	template <class Pixel>
	forcedinline void blend (const Pixel& src, uint32 extraAlpha) noexcept
	{
		++extraAlpha;
		const int srcAlpha = (int) ((extraAlpha * src.getAlpha()) >> 8);
		a = (uint8) ((a * (0x100 - srcAlpha) >> 8) + srcAlpha);
	}

	/** Blends another pixel with this one, creating a colour that is somewhere
		between the two, as specified by the amount.
	*/
	template <class Pixel>
	forcedinline void tween (const Pixel& src, const uint32 amount) noexcept
	{
		a += ((src.getAlpha() - a) * amount) >> 8;
	}

	/** Copies another pixel colour over this one.

		This doesn't blend it - this colour is simply replaced by the other one.
	*/
	template <class Pixel>
	forcedinline void set (const Pixel& src) noexcept
	{
		a = src.getAlpha();
	}

	/** Replaces the colour's alpha value with another one. */
	forcedinline void setAlpha (const uint8 newAlpha) noexcept
	{
		a = newAlpha;
	}

	/** Multiplies the colour's alpha value with another one. */
	forcedinline void multiplyAlpha (int multiplier) noexcept
	{
		++multiplier;
		a = (uint8) ((a * multiplier) >> 8);
	}

	forcedinline void multiplyAlpha (const float multiplier) noexcept
	{
		a = (uint8) (a * multiplier);
	}

	/** Sets the pixel's colour from individual components. */
	forcedinline void setARGB (const uint8 a_, const uint8 /*r*/, const uint8 /*g*/, const uint8 /*b*/) noexcept
	{
		a = a_;
	}

	/** Premultiplies the pixel's RGB values by its alpha. */
	forcedinline void premultiply() noexcept {}

	/** Unpremultiplies the pixel's RGB values. */
	forcedinline void unpremultiply() noexcept {}

	forcedinline void desaturate() noexcept {}

	/** The indexes of the different components in the byte layout of this type of colour. */
	enum { indexA = 0 };

private:

	uint8 a : 8;
}
#ifndef DOXYGEN
 PACKED
#endif
;

#if JUCE_MSVC
 #pragma pack (pop)
#endif

#undef PACKED

#endif   // __JUCE_PIXELFORMATS_JUCEHEADER__

/*** End of inlined file: juce_PixelFormats.h ***/

/**
	Represents a colour, also including a transparency value.

	The colour is stored internally as unsigned 8-bit red, green, blue and alpha values.
*/
class JUCE_API  Colour
{
public:

	/** Creates a transparent black colour. */
	Colour() noexcept;

	/** Creates a copy of another Colour object. */
	Colour (const Colour& other) noexcept;

	/** Creates a colour from a 32-bit ARGB value.

		The format of this number is:
			((alpha << 24) | (red << 16) | (green << 8) | blue).

		All components in the range 0x00 to 0xff.
		An alpha of 0x00 is completely transparent, alpha of 0xff is opaque.

		@see getPixelARGB
	*/
	explicit Colour (uint32 argb) noexcept;

	/** Creates an opaque colour using 8-bit red, green and blue values */
	Colour (uint8 red,
			uint8 green,
			uint8 blue) noexcept;

	/** Creates an opaque colour using 8-bit red, green and blue values */
	static Colour fromRGB (uint8 red,
						   uint8 green,
						   uint8 blue) noexcept;

	/** Creates a colour using 8-bit red, green, blue and alpha values. */
	Colour (uint8 red,
			uint8 green,
			uint8 blue,
			uint8 alpha) noexcept;

	/** Creates a colour using 8-bit red, green, blue and alpha values. */
	static Colour fromRGBA (uint8 red,
							uint8 green,
							uint8 blue,
							uint8 alpha) noexcept;

	/** Creates a colour from 8-bit red, green, and blue values, and a floating-point alpha.

		Alpha of 0.0 is transparent, alpha of 1.0f is opaque.
		Values outside the valid range will be clipped.
	*/
	Colour (uint8 red,
			uint8 green,
			uint8 blue,
			float alpha) noexcept;

	/** Creates a colour using floating point red, green, blue and alpha values.
		Numbers outside the range 0..1 will be clipped.
	*/
	static Colour fromFloatRGBA (float red,
								 float green,
								 float blue,
								 float alpha) noexcept;

	/** Creates a colour using floating point hue, saturation and brightness values, and an 8-bit alpha.

		The floating point values must be between 0.0 and 1.0.
		An alpha of 0x00 is completely transparent, alpha of 0xff is opaque.
		Values outside the valid range will be clipped.
	*/
	Colour (float hue,
			float saturation,
			float brightness,
			uint8 alpha) noexcept;

	/** Creates a colour using floating point hue, saturation, brightness and alpha values.

		All values must be between 0.0 and 1.0.
		Numbers outside the valid range will be clipped.
	*/
	Colour (float hue,
			float saturation,
			float brightness,
			float alpha) noexcept;

	/** Creates a colour using floating point hue, saturation and brightness values, and an 8-bit alpha.

		The floating point values must be between 0.0 and 1.0.
		An alpha of 0x00 is completely transparent, alpha of 0xff is opaque.
		Values outside the valid range will be clipped.
	*/
	static Colour fromHSV (float hue,
						   float saturation,
						   float brightness,
						   float alpha) noexcept;

	/** Destructor. */
	~Colour() noexcept;

	/** Copies another Colour object. */
	Colour& operator= (const Colour& other) noexcept;

	/** Compares two colours. */
	bool operator== (const Colour& other) const noexcept;
	/** Compares two colours. */
	bool operator!= (const Colour& other) const noexcept;

	/** Returns the red component of this colour.

		@returns a value between 0x00 and 0xff.
	*/
	uint8 getRed() const noexcept                       { return argb.getRed(); }

	/** Returns the green component of this colour.

		@returns a value between 0x00 and 0xff.
	*/
	uint8 getGreen() const noexcept                     { return argb.getGreen(); }

	/** Returns the blue component of this colour.

		@returns a value between 0x00 and 0xff.
	*/
	uint8 getBlue() const noexcept                      { return argb.getBlue(); }

	/** Returns the red component of this colour as a floating point value.

		@returns a value between 0.0 and 1.0
	*/
	float getFloatRed() const noexcept;

	/** Returns the green component of this colour as a floating point value.

		@returns a value between 0.0 and 1.0
	*/
	float getFloatGreen() const noexcept;

	/** Returns the blue component of this colour as a floating point value.

		@returns a value between 0.0 and 1.0
	*/
	float getFloatBlue() const noexcept;

	/** Returns a premultiplied ARGB pixel object that represents this colour.
	*/
	const PixelARGB getPixelARGB() const noexcept;

	/** Returns a 32-bit integer that represents this colour.

		The format of this number is:
			((alpha << 24) | (red << 16) | (green << 16) | blue).
	*/
	uint32 getARGB() const noexcept;

	/** Returns the colour's alpha (opacity).

		Alpha of 0x00 is completely transparent, 0xff is completely opaque.
	*/
	uint8 getAlpha() const noexcept                     { return argb.getAlpha(); }

	/** Returns the colour's alpha (opacity) as a floating point value.

		Alpha of 0.0 is completely transparent, 1.0 is completely opaque.
	*/
	float getFloatAlpha() const noexcept;

	/** Returns true if this colour is completely opaque.

		Equivalent to (getAlpha() == 0xff).
	*/
	bool isOpaque() const noexcept;

	/** Returns true if this colour is completely transparent.

		Equivalent to (getAlpha() == 0x00).
	*/
	bool isTransparent() const noexcept;

	/** Returns a colour that's the same colour as this one, but with a new alpha value. */
	Colour withAlpha (uint8 newAlpha) const noexcept;

	/** Returns a colour that's the same colour as this one, but with a new alpha value. */
	Colour withAlpha (float newAlpha) const noexcept;

	/** Returns a colour that's the same colour as this one, but with a modified alpha value.

		The new colour's alpha will be this object's alpha multiplied by the value passed-in.
	*/
	Colour withMultipliedAlpha (float alphaMultiplier) const noexcept;

	/** Returns a colour that is the result of alpha-compositing a new colour over this one.

		If the foreground colour is semi-transparent, it is blended onto this colour
		accordingly.
	*/
	Colour overlaidWith (const Colour& foregroundColour) const noexcept;

	/** Returns a colour that lies somewhere between this one and another.

		If amountOfOther is zero, the result is 100% this colour, if amountOfOther
		is 1.0, the result is 100% of the other colour.
	*/
	Colour interpolatedWith (const Colour& other, float proportionOfOther) const noexcept;

	/** Returns the colour's hue component.
		The value returned is in the range 0.0 to 1.0
	*/
	float getHue() const noexcept;

	/** Returns the colour's saturation component.
		The value returned is in the range 0.0 to 1.0
	*/
	float getSaturation() const noexcept;

	/** Returns the colour's brightness component.
		The value returned is in the range 0.0 to 1.0
	*/
	float getBrightness() const noexcept;

	/** Returns the colour's hue, saturation and brightness components all at once.
		The values returned are in the range 0.0 to 1.0
	*/
	void getHSB (float& hue,
				 float& saturation,
				 float& brightness) const noexcept;

	/** Returns a copy of this colour with a different hue. */
	Colour withHue (float newHue) const noexcept;

	/** Returns a copy of this colour with a different saturation. */
	Colour withSaturation (float newSaturation) const noexcept;

	/** Returns a copy of this colour with a different brightness.
		@see brighter, darker, withMultipliedBrightness
	*/
	Colour withBrightness (float newBrightness) const noexcept;

	/** Returns a copy of this colour with it hue rotated.

		The new colour's hue is ((this->getHue() + amountToRotate) % 1.0)

		@see brighter, darker, withMultipliedBrightness
	*/
	Colour withRotatedHue (float amountToRotate) const noexcept;

	/** Returns a copy of this colour with its saturation multiplied by the given value.

		The new colour's saturation is (this->getSaturation() * multiplier)
		(the result is clipped to legal limits).
	*/
	Colour withMultipliedSaturation (float multiplier) const noexcept;

	/** Returns a copy of this colour with its brightness multiplied by the given value.

		The new colour's saturation is (this->getBrightness() * multiplier)
		(the result is clipped to legal limits).
	*/
	Colour withMultipliedBrightness (float amount) const noexcept;

	/** Returns a brighter version of this colour.

		@param amountBrighter   how much brighter to make it - a value from 0 to 1.0 where 0 is
								unchanged, and higher values make it brighter
		@see withMultipliedBrightness
	*/
	Colour brighter (float amountBrighter = 0.4f) const noexcept;

	/** Returns a darker version of this colour.

		@param amountDarker     how much darker to make it - a value from 0 to 1.0 where 0 is
								unchanged, and higher values make it darker
		@see withMultipliedBrightness
	*/
	Colour darker (float amountDarker = 0.4f) const noexcept;

	/** Returns a colour that will be clearly visible against this colour.

		The amount parameter indicates how contrasting the new colour should
		be, so e.g. Colours::black.contrasting (0.1f) will return a colour
		that's just a little bit lighter; Colours::black.contrasting (1.0f) will
		return white; Colours::white.contrasting (1.0f) will return black, etc.
	*/
	Colour contrasting (float amount = 1.0f) const noexcept;

	/** Returns a colour that contrasts against two colours.

		Looks for a colour that contrasts with both of the colours passed-in.

		Handy for things like choosing a highlight colour in text editors, etc.
	*/
	static Colour contrasting (const Colour& colour1,
							   const Colour& colour2) noexcept;

	/** Returns an opaque shade of grey.

		@param brightness the level of grey to return - 0 is black, 1.0 is white
	*/
	static Colour greyLevel (float brightness) noexcept;

	/** Returns a stringified version of this colour.

		The string can be turned back into a colour using the fromString() method.
	*/
	String toString() const;

	/** Reads the colour from a string that was created with toString().
	*/
	static Colour fromString (const String& encodedColourString);

	/** Returns the colour as a hex string in the form RRGGBB or AARRGGBB. */
	String toDisplayString (bool includeAlphaValue) const;

private:

	PixelARGB argb;
};

#endif   // __JUCE_COLOUR_JUCEHEADER__

/*** End of inlined file: juce_Colour.h ***/


#endif
#ifndef __JUCE_COLOURGRADIENT_JUCEHEADER__

/*** Start of inlined file: juce_ColourGradient.h ***/
#ifndef __JUCE_COLOURGRADIENT_JUCEHEADER__
#define __JUCE_COLOURGRADIENT_JUCEHEADER__


/*** Start of inlined file: juce_Point.h ***/
#ifndef __JUCE_POINT_JUCEHEADER__
#define __JUCE_POINT_JUCEHEADER__


/*** Start of inlined file: juce_AffineTransform.h ***/
#ifndef __JUCE_AFFINETRANSFORM_JUCEHEADER__
#define __JUCE_AFFINETRANSFORM_JUCEHEADER__

/**
	Represents a 2D affine-transformation matrix.

	An affine transformation is a transformation such as a rotation, scale, shear,
	resize or translation.

	These are used for various 2D transformation tasks, e.g. with Path objects.

	@see Path, Point, Line
*/
class JUCE_API  AffineTransform
{
public:

	/** Creates an identity transform. */
	AffineTransform() noexcept;

	/** Creates a copy of another transform. */
	AffineTransform (const AffineTransform& other) noexcept;

	/** Creates a transform from a set of raw matrix values.

		The resulting matrix is:

			(mat00 mat01 mat02)
			(mat10 mat11 mat12)
			(  0     0     1  )
	*/
	AffineTransform (float mat00, float mat01, float mat02,
					 float mat10, float mat11, float mat12) noexcept;

	/** Copies from another AffineTransform object */
	AffineTransform& operator= (const AffineTransform& other) noexcept;

	/** Compares two transforms. */
	bool operator== (const AffineTransform& other) const noexcept;

	/** Compares two transforms. */
	bool operator!= (const AffineTransform& other) const noexcept;

	/** A ready-to-use identity transform, which you can use to append other
		transformations to.

		e.g. @code
		AffineTransform myTransform = AffineTransform::identity.rotated (.5f)
															   .scaled (2.0f);

		@endcode
	*/
	static const AffineTransform identity;

	/** Transforms a 2D co-ordinate using this matrix. */
	template <typename ValueType>
	void transformPoint (ValueType& x, ValueType& y) const noexcept
	{
		const ValueType oldX = x;
		x = static_cast <ValueType> (mat00 * oldX + mat01 * y + mat02);
		y = static_cast <ValueType> (mat10 * oldX + mat11 * y + mat12);
	}

	/** Transforms two 2D co-ordinates using this matrix.
		This is just a shortcut for calling transformPoint() on each of these pairs of
		coordinates in turn. (And putting all the calculations into one function hopefully
		also gives the compiler a bit more scope for pipelining it).
	*/
	template <typename ValueType>
	void transformPoints (ValueType& x1, ValueType& y1,
						  ValueType& x2, ValueType& y2) const noexcept
	{
		const ValueType oldX1 = x1, oldX2 = x2;
		x1 = static_cast <ValueType> (mat00 * oldX1 + mat01 * y1 + mat02);
		y1 = static_cast <ValueType> (mat10 * oldX1 + mat11 * y1 + mat12);
		x2 = static_cast <ValueType> (mat00 * oldX2 + mat01 * y2 + mat02);
		y2 = static_cast <ValueType> (mat10 * oldX2 + mat11 * y2 + mat12);
	}

	/** Transforms three 2D co-ordinates using this matrix.
		This is just a shortcut for calling transformPoint() on each of these pairs of
		coordinates in turn. (And putting all the calculations into one function hopefully
		also gives the compiler a bit more scope for pipelining it).
	*/
	template <typename ValueType>
	void transformPoints (ValueType& x1, ValueType& y1,
						  ValueType& x2, ValueType& y2,
						  ValueType& x3, ValueType& y3) const noexcept
	{
		const ValueType oldX1 = x1, oldX2 = x2, oldX3 = x3;
		x1 = static_cast <ValueType> (mat00 * oldX1 + mat01 * y1 + mat02);
		y1 = static_cast <ValueType> (mat10 * oldX1 + mat11 * y1 + mat12);
		x2 = static_cast <ValueType> (mat00 * oldX2 + mat01 * y2 + mat02);
		y2 = static_cast <ValueType> (mat10 * oldX2 + mat11 * y2 + mat12);
		x3 = static_cast <ValueType> (mat00 * oldX3 + mat01 * y3 + mat02);
		y3 = static_cast <ValueType> (mat10 * oldX3 + mat11 * y3 + mat12);
	}

	/** Returns a new transform which is the same as this one followed by a translation. */
	AffineTransform translated (float deltaX,
								float deltaY) const noexcept;

	/** Returns a new transform which is a translation. */
	static AffineTransform translation (float deltaX,
										float deltaY) noexcept;

	/** Returns a transform which is the same as this one followed by a rotation.

		The rotation is specified by a number of radians to rotate clockwise, centred around
		the origin (0, 0).
	*/
	AffineTransform rotated (float angleInRadians) const noexcept;

	/** Returns a transform which is the same as this one followed by a rotation about a given point.

		The rotation is specified by a number of radians to rotate clockwise, centred around
		the co-ordinates passed in.
	*/
	AffineTransform rotated (float angleInRadians,
							 float pivotX,
							 float pivotY) const noexcept;

	/** Returns a new transform which is a rotation about (0, 0). */
	static AffineTransform rotation (float angleInRadians) noexcept;

	/** Returns a new transform which is a rotation about a given point. */
	static AffineTransform rotation (float angleInRadians,
									 float pivotX,
									 float pivotY) noexcept;

	/** Returns a transform which is the same as this one followed by a re-scaling.
		The scaling is centred around the origin (0, 0).
	*/
	AffineTransform scaled (float factorX,
							float factorY) const noexcept;

	/** Returns a transform which is the same as this one followed by a re-scaling.
		The scaling is centred around the origin provided.
	*/
	AffineTransform scaled (float factorX, float factorY,
							float pivotX, float pivotY) const noexcept;

	/** Returns a new transform which is a re-scale about the origin. */
	static AffineTransform scale (float factorX,
								  float factorY) noexcept;

	/** Returns a new transform which is a re-scale centred around the point provided. */
	static AffineTransform scale (float factorX, float factorY,
								  float pivotX, float pivotY) noexcept;

	/** Returns a transform which is the same as this one followed by a shear.
		The shear is centred around the origin (0, 0).
	*/
	AffineTransform sheared (float shearX, float shearY) const noexcept;

	/** Returns a shear transform, centred around the origin (0, 0). */
	static AffineTransform shear (float shearX, float shearY) noexcept;

	/** Returns a transform that will flip co-ordinates vertically within a window of the given height.
		This is handy for converting between upside-down coordinate systems such as OpenGL or CoreGraphics.
	*/
	static AffineTransform verticalFlip (float height) noexcept;

	/** Returns a matrix which is the inverse operation of this one.

		Some matrices don't have an inverse - in this case, the method will just return
		an identity transform.
	*/
	AffineTransform inverted() const noexcept;

	/** Returns the transform that will map three known points onto three coordinates
		that are supplied.

		This returns the transform that will transform (0, 0) into (x00, y00),
		(1, 0) to (x10, y10), and (0, 1) to (x01, y01).
	*/
	static AffineTransform fromTargetPoints (float x00, float y00,
											 float x10, float y10,
											 float x01, float y01) noexcept;

	/** Returns the transform that will map three specified points onto three target points.
	*/
	static AffineTransform fromTargetPoints (float sourceX1, float sourceY1, float targetX1, float targetY1,
											 float sourceX2, float sourceY2, float targetX2, float targetY2,
											 float sourceX3, float sourceY3, float targetX3, float targetY3) noexcept;

	/** Returns the result of concatenating another transformation after this one. */
	AffineTransform followedBy (const AffineTransform& other) const noexcept;

	/** Returns true if this transform has no effect on points. */
	bool isIdentity() const noexcept;

	/** Returns true if this transform maps to a singularity - i.e. if it has no inverse. */
	bool isSingularity() const noexcept;

	/** Returns true if the transform only translates, and doesn't scale or rotate the
		points. */
	bool isOnlyTranslation() const noexcept;

	/** If this transform is only a translation, this returns the X offset.
		@see isOnlyTranslation
	*/
	float getTranslationX() const noexcept                  { return mat02; }

	/** If this transform is only a translation, this returns the X offset.
		@see isOnlyTranslation
	*/
	float getTranslationY() const noexcept                  { return mat12; }

	/** Returns the approximate scale factor by which lengths will be transformed.
		Obviously a length may be scaled by entirely different amounts depending on its
		direction, so this is only appropriate as a rough guide.
	*/
	float getScaleFactor() const noexcept;

	/* The transform matrix is:

		(mat00 mat01 mat02)
		(mat10 mat11 mat12)
		(  0     0     1  )
	*/
	float mat00, mat01, mat02;
	float mat10, mat11, mat12;

private:

	JUCE_LEAK_DETECTOR (AffineTransform);
};

#endif   // __JUCE_AFFINETRANSFORM_JUCEHEADER__

/*** End of inlined file: juce_AffineTransform.h ***/

/**
	A pair of (x, y) co-ordinates.

	The ValueType template should be a primitive type such as int, float, double,
	rather than a class.

	@see Line, Path, AffineTransform
*/
template <typename ValueType>
class Point
{
public:

	/** Creates a point with co-ordinates (0, 0). */
	Point() noexcept : x(), y() {}

	/** Creates a copy of another point. */
	Point (const Point& other) noexcept : x (other.x), y (other.y)  {}

	/** Creates a point from an (x, y) position. */
	Point (const ValueType initialX, const ValueType initialY) noexcept : x (initialX), y (initialY) {}

	/** Destructor. */
	~Point() noexcept {}

	/** Copies this point from another one. */
	Point& operator= (const Point& other) noexcept                      { x = other.x; y = other.y; return *this; }

	inline bool operator== (const Point& other) const noexcept          { return x == other.x && y == other.y; }
	inline bool operator!= (const Point& other) const noexcept          { return x != other.x || y != other.y; }

	/** Returns true if the point is (0, 0). */
	bool isOrigin() const noexcept                                      { return x == ValueType() && y == ValueType(); }

	/** Returns the point's x co-ordinate. */
	inline ValueType getX() const noexcept                              { return x; }

	/** Returns the point's y co-ordinate. */
	inline ValueType getY() const noexcept                              { return y; }

	/** Sets the point's x co-ordinate. */
	inline void setX (const ValueType newX) noexcept                    { x = newX; }

	/** Sets the point's y co-ordinate. */
	inline void setY (const ValueType newY) noexcept                    { y = newY; }

	/** Returns a point which has the same Y position as this one, but a new X. */
	Point withX (const ValueType newX) const noexcept                   { return Point (newX, y); }

	/** Returns a point which has the same X position as this one, but a new Y. */
	Point withY (const ValueType newY) const noexcept                   { return Point (x, newY); }

	/** Changes the point's x and y co-ordinates. */
	void setXY (const ValueType newX, const ValueType newY) noexcept    { x = newX; y = newY; }

	/** Adds a pair of co-ordinates to this value. */
	void addXY (const ValueType xToAdd, const ValueType yToAdd) noexcept { x += xToAdd; y += yToAdd; }

	/** Returns a point with a given offset from this one. */
	Point translated (const ValueType xDelta, const ValueType yDelta) const noexcept  { return Point (x + xDelta, y + yDelta); }

	/** Adds two points together. */
	Point operator+ (const Point& other) const noexcept                 { return Point (x + other.x, y + other.y); }

	/** Adds another point's co-ordinates to this one. */
	Point& operator+= (const Point& other) noexcept                     { x += other.x; y += other.y; return *this; }

	/** Subtracts one points from another. */
	Point operator- (const Point& other) const noexcept                 { return Point (x - other.x, y - other.y); }

	/** Subtracts another point's co-ordinates to this one. */
	Point& operator-= (const Point& other) noexcept                     { x -= other.x; y -= other.y; return *this; }

	/** Returns a point whose coordinates are multiplied by a given value. */
	Point operator* (const ValueType multiplier) const noexcept         { return Point (x * multiplier, y * multiplier); }

	/** Multiplies the point's co-ordinates by a value. */
	Point& operator*= (const ValueType multiplier) noexcept             { x *= multiplier; y *= multiplier; return *this; }

	/** Returns a point whose coordinates are divided by a given value. */
	Point operator/ (const ValueType divisor) const noexcept            { return Point (x / divisor, y / divisor); }

	/** Divides the point's co-ordinates by a value. */
	Point& operator/= (const ValueType divisor) noexcept                { x /= divisor; y /= divisor; return *this; }

	/** Returns the inverse of this point. */
	Point operator-() const noexcept                                    { return Point (-x, -y); }

	/** Returns the straight-line distance between this point and the origin. */
	ValueType getDistanceFromOrigin() const noexcept                    { return juce_hypot (x, y); }

	/** Returns the straight-line distance between this point and another one. */
	ValueType getDistanceFrom (const Point& other) const noexcept       { return juce_hypot (x - other.x, y - other.y); }

	/** This type will be double if the Point's type is double, otherwise it will be float. */
	typedef typename TypeHelpers::SmallestFloatType<ValueType>::type FloatType;

	/** Returns the angle from this point to another one.

		The return value is the number of radians clockwise from the 12 o'clock direction,
		where this point is the centre and the other point is on the circumference.
	*/
	FloatType getAngleToPoint (const Point& other) const noexcept
		{ return static_cast<FloatType> (std::atan2 (other.x - x, y - other.y)); }

	/** Taking this point to be the centre of a circle, this returns a point on its circumference.
		@param radius   the radius of the circle.
		@param angle    the angle of the point, in radians clockwise from the 12 o'clock position.
	*/
	Point<FloatType> getPointOnCircumference (const float radius, const float angle) const noexcept
		{ return Point<FloatType> (static_cast <FloatType> (x + radius * std::sin (angle)),
								   static_cast <FloatType> (y - radius * std::cos (angle))); }

	/** Taking this point to be the centre of an ellipse, this returns a point on its circumference.
		@param radiusX  the horizontal radius of the circle.
		@param radiusY  the vertical radius of the circle.
		@param angle    the angle of the point, in radians clockwise from the 12 o'clock position.
	*/
	Point<FloatType> getPointOnCircumference (const float radiusX, const float radiusY, const float angle) const noexcept
		{ return Point<FloatType> (static_cast <FloatType> (x + radiusX * std::sin (angle)),
								   static_cast <FloatType> (y - radiusY * std::cos (angle))); }

	/** Uses a transform to change the point's co-ordinates.
		This will only compile if ValueType = float!
		@see AffineTransform::transformPoint
	*/
	void applyTransform (const AffineTransform& transform) noexcept     { transform.transformPoint (x, y); }

	/** Returns the position of this point, if it is transformed by a given AffineTransform. */
	Point transformedBy (const AffineTransform& transform) const noexcept
		{ return Point (transform.mat00 * x + transform.mat01 * y + transform.mat02,
						transform.mat10 * x + transform.mat11 * y + transform.mat12); }

	/** Casts this point to a Point<int> object. */
	Point<int> toInt() const noexcept                             { return Point<int> (static_cast <int> (x), static_cast<int> (y)); }

	/** Casts this point to a Point<float> object. */
	Point<float> toFloat() const noexcept                         { return Point<float> (static_cast <float> (x), static_cast<float> (y)); }

	/** Casts this point to a Point<double> object. */
	Point<double> toDouble() const noexcept                       { return Point<double> (static_cast <double> (x), static_cast<double> (y)); }

	/** Returns the point as a string in the form "x, y". */
	String toString() const                                       { return String (x) + ", " + String (y); }

	ValueType x; /**< The point's X coordinate. */
	ValueType y; /**< The point's Y coordinate. */
};

#endif   // __JUCE_POINT_JUCEHEADER__

/*** End of inlined file: juce_Point.h ***/

/**
	Describes the layout and colours that should be used to paint a colour gradient.

	@see Graphics::setGradientFill
*/
class JUCE_API  ColourGradient
{
public:

	/** Creates a gradient object.

		(x1, y1) is the location to draw with colour1. Likewise (x2, y2) is where
		colour2 should be. In between them there's a gradient.

		If isRadial is true, the colours form a circular gradient with (x1, y1) at
		its centre.

		The alpha transparencies of the colours are used, so note that
		if you blend from transparent to a solid colour, the RGB of the transparent
		colour will become visible in parts of the gradient. e.g. blending
		from Colour::transparentBlack to Colours::white will produce a
		muddy grey colour midway, but Colour::transparentWhite to Colours::white
		will be white all the way across.

		@see ColourGradient
	*/
	ColourGradient (const Colour& colour1, float x1, float y1,
					const Colour& colour2, float x2, float y2,
					bool isRadial);

	/** Creates an uninitialised gradient.

		If you use this constructor instead of the other one, be sure to set all the
		object's public member variables before using it!
	*/
	ColourGradient() noexcept;

	/** Destructor */
	~ColourGradient();

	/** Removes any colours that have been added.

		This will also remove any start and end colours, so the gradient won't work. You'll
		need to add more colours with addColour().
	*/
	void clearColours();

	/** Adds a colour at a point along the length of the gradient.

		This allows the gradient to go through a spectrum of colours, instead of just a
		start and end colour.

		@param proportionAlongGradient      a value between 0 and 1.0, which is the proportion
											of the distance along the line between the two points
											at which the colour should occur.
		@param colour                       the colour that should be used at this point
		@returns the index at which the new point was added
	*/
	int addColour (double proportionAlongGradient,
				   const Colour& colour);

	/** Removes one of the colours from the gradient. */
	void removeColour (int index);

	/** Multiplies the alpha value of all the colours by the given scale factor */
	void multiplyOpacity (float multiplier) noexcept;

	/** Returns the number of colour-stops that have been added. */
	int getNumColours() const noexcept;

	/** Returns the position along the length of the gradient of the colour with this index.

		The index is from 0 to getNumColours() - 1. The return value will be between 0.0 and 1.0
	*/
	double getColourPosition (int index) const noexcept;

	/** Returns the colour that was added with a given index.
		The index is from 0 to getNumColours() - 1.
	*/
	Colour getColour (int index) const noexcept;

	/** Changes the colour at a given index.
		The index is from 0 to getNumColours() - 1.
	*/
	void setColour (int index, const Colour& newColour) noexcept;

	/** Returns the an interpolated colour at any position along the gradient.
		@param position     the position along the gradient, between 0 and 1
	*/
	Colour getColourAtPosition (double position) const noexcept;

	/** Creates a set of interpolated premultiplied ARGB values.
		This will resize the HeapBlock, fill it with the colours, and will return the number of
		colours that it added.
		When calling this, the ColourGradient must have at least 2 colour stops specified.
	*/
	int createLookupTable (const AffineTransform& transform, HeapBlock <PixelARGB>& resultLookupTable) const;

	/** Creates a set of interpolated premultiplied ARGB values.
		This will fill an array of a user-specified size with the gradient, interpolating to fit.
		The numEntries argument specifies the size of the array, and this size must be greater than zero.
		When calling this, the ColourGradient must have at least 2 colour stops specified.
	*/
	void createLookupTable (PixelARGB* resultLookupTable, int numEntries) const noexcept;

	/** Returns true if all colours are opaque. */
	bool isOpaque() const noexcept;

	/** Returns true if all colours are completely transparent. */
	bool isInvisible() const noexcept;

	Point<float> point1, point2;

	/** If true, the gradient should be filled circularly, centred around
		point1, with point2 defining a point on the circumference.

		If false, the gradient is linear between the two points.
	*/
	bool isRadial;

	bool operator== (const ColourGradient& other) const noexcept;
	bool operator!= (const ColourGradient& other) const noexcept;

private:

	struct ColourPoint
	{
		ColourPoint() noexcept {}

		ColourPoint (const double position_, const Colour& colour_) noexcept
			: position (position_), colour (colour_)
		{}

		bool operator== (const ColourPoint& other) const noexcept;
		bool operator!= (const ColourPoint& other) const noexcept;

		double position;
		Colour colour;
	};

	Array <ColourPoint> colours;

	JUCE_LEAK_DETECTOR (ColourGradient);
};

#endif   // __JUCE_COLOURGRADIENT_JUCEHEADER__

/*** End of inlined file: juce_ColourGradient.h ***/


#endif
#ifndef __JUCE_COLOURS_JUCEHEADER__

/*** Start of inlined file: juce_Colours.h ***/
#ifndef __JUCE_COLOURS_JUCEHEADER__
#define __JUCE_COLOURS_JUCEHEADER__

/**
	Contains a set of predefined named colours (mostly standard HTML colours)

	@see Colour, Colours::greyLevel
*/
class Colours
{
public:
	static JUCE_API const Colour

	transparentBlack,   /**< ARGB = 0x00000000 */
	transparentWhite,   /**< ARGB = 0x00ffffff */

	black,              /**< ARGB = 0xff000000 */
	white,              /**< ARGB = 0xffffffff */
	blue,               /**< ARGB = 0xff0000ff */
	grey,               /**< ARGB = 0xff808080 */
	green,              /**< ARGB = 0xff008000 */
	red,                /**< ARGB = 0xffff0000 */
	yellow,             /**< ARGB = 0xffffff00 */

	aliceblue,              antiquewhite,       aqua,               aquamarine,
	azure,                  beige,              bisque,             blanchedalmond,
	blueviolet,             brown,              burlywood,          cadetblue,
	chartreuse,             chocolate,          coral,              cornflowerblue,
	cornsilk,               crimson,            cyan,               darkblue,
	darkcyan,               darkgoldenrod,      darkgrey,           darkgreen,
	darkkhaki,              darkmagenta,        darkolivegreen,     darkorange,
	darkorchid,             darkred,            darksalmon,         darkseagreen,
	darkslateblue,          darkslategrey,      darkturquoise,      darkviolet,
	deeppink,               deepskyblue,        dimgrey,            dodgerblue,
	firebrick,              floralwhite,        forestgreen,        fuchsia,
	gainsboro,              gold,               goldenrod,          greenyellow,
	honeydew,               hotpink,            indianred,          indigo,
	ivory,                  khaki,              lavender,           lavenderblush,
	lemonchiffon,           lightblue,          lightcoral,         lightcyan,
	lightgoldenrodyellow,   lightgreen,         lightgrey,          lightpink,
	lightsalmon,            lightseagreen,      lightskyblue,       lightslategrey,
	lightsteelblue,         lightyellow,        lime,               limegreen,
	linen,                  magenta,            maroon,             mediumaquamarine,
	mediumblue,             mediumorchid,       mediumpurple,       mediumseagreen,
	mediumslateblue,        mediumspringgreen,  mediumturquoise,    mediumvioletred,
	midnightblue,           mintcream,          mistyrose,          navajowhite,
	navy,                   oldlace,            olive,              olivedrab,
	orange,                 orangered,          orchid,             palegoldenrod,
	palegreen,              paleturquoise,      palevioletred,      papayawhip,
	peachpuff,              peru,               pink,               plum,
	powderblue,             purple,             rosybrown,          royalblue,
	saddlebrown,            salmon,             sandybrown,         seagreen,
	seashell,               sienna,             silver,             skyblue,
	slateblue,              slategrey,          snow,               springgreen,
	steelblue,              tan,                teal,               thistle,
	tomato,                 turquoise,          violet,             wheat,
	whitesmoke,             yellowgreen;

	/** Attempts to look up a string in the list of known colour names, and return
		the appropriate colour.

		A non-case-sensitive search is made of the list of predefined colours, and
		if a match is found, that colour is returned. If no match is found, the
		colour passed in as the defaultColour parameter is returned.
	*/
	static JUCE_API Colour findColourForName (const String& colourName,
											  const Colour& defaultColour);

private:

	// this isn't a class you should ever instantiate - it's just here for the
	// static values in it.
	Colours();

	JUCE_DECLARE_NON_COPYABLE (Colours);
};

#endif   // __JUCE_COLOURS_JUCEHEADER__

/*** End of inlined file: juce_Colours.h ***/


#endif
#ifndef __JUCE_FILLTYPE_JUCEHEADER__

/*** Start of inlined file: juce_FillType.h ***/
#ifndef __JUCE_FILLTYPE_JUCEHEADER__
#define __JUCE_FILLTYPE_JUCEHEADER__


/*** Start of inlined file: juce_Image.h ***/
#ifndef __JUCE_IMAGE_JUCEHEADER__
#define __JUCE_IMAGE_JUCEHEADER__


/*** Start of inlined file: juce_GraphicsContext.h ***/
#ifndef __JUCE_GRAPHICSCONTEXT_JUCEHEADER__
#define __JUCE_GRAPHICSCONTEXT_JUCEHEADER__


/*** Start of inlined file: juce_Font.h ***/
#ifndef __JUCE_FONT_JUCEHEADER__
#define __JUCE_FONT_JUCEHEADER__


/*** Start of inlined file: juce_Typeface.h ***/
#ifndef __JUCE_TYPEFACE_JUCEHEADER__
#define __JUCE_TYPEFACE_JUCEHEADER__

class Path;
class Font;
class EdgeTable;
class AffineTransform;

/**
	A typeface represents a size-independent font.

	This base class is abstract, but calling createSystemTypefaceFor() will return
	a platform-specific subclass that can be used.

	The CustomTypeface subclass allow you to build your own typeface, and to
	load and save it in the Juce typeface format.

	Normally you should never need to deal directly with Typeface objects - the Font
	class does everything you typically need for rendering text.

	@see CustomTypeface, Font
*/
class JUCE_API  Typeface  : public SingleThreadedReferenceCountedObject
{
public:

	/** A handy typedef for a pointer to a typeface. */
	typedef ReferenceCountedObjectPtr <Typeface> Ptr;

	/** Returns the font family of the typeface.
		@see Font::getTypefaceName
	*/
	const String& getName() const noexcept      { return name; }

	/** Returns the font style of the typeface.
		@see Font::getTypefaceStyle
	*/
	const String& getStyle() const noexcept     { return style; }

	/** Creates a new system typeface. */
	static Ptr createSystemTypefaceFor (const Font& font);

	/** Destructor. */
	virtual ~Typeface();

	/** Returns true if this typeface can be used to render the specified font.
		When called, the font will already have been checked to make sure that its name and
		style flags match the typeface.
	*/
	virtual bool isSuitableForFont (const Font&) const          { return true; }

	/** Returns the ascent of the font, as a proportion of its height.
		The height is considered to always be normalised as 1.0, so this will be a
		value less that 1.0, indicating the proportion of the font that lies above
		its baseline.
	*/
	virtual float getAscent() const = 0;

	/** Returns the descent of the font, as a proportion of its height.
		The height is considered to always be normalised as 1.0, so this will be a
		value less that 1.0, indicating the proportion of the font that lies below
		its baseline.
	*/
	virtual float getDescent() const = 0;

	/** Measures the width of a line of text.

		The distance returned is based on the font having an normalised height of 1.0.

		You should never need to call this directly! Use Font::getStringWidth() instead!
	*/
	virtual float getStringWidth (const String& text) = 0;

	/** Converts a line of text into its glyph numbers and their positions.

		The distances returned are based on the font having an normalised height of 1.0.

		You should never need to call this directly! Use Font::getGlyphPositions() instead!
	*/
	virtual void getGlyphPositions (const String& text, Array <int>& glyphs, Array<float>& xOffsets) = 0;

	/** Returns the outline for a glyph.

		The path returned will be normalised to a font height of 1.0.
	*/
	virtual bool getOutlineForGlyph (int glyphNumber, Path& path) = 0;

	/** Returns a new EdgeTable that contains the path for the givem glyph, with the specified transform applied. */
	virtual EdgeTable* getEdgeTableForGlyph (int glyphNumber, const AffineTransform& transform);

	/** Returns true if the typeface uses hinting. */
	virtual bool isHinted() const                           { return false; }

	/** Changes the number of fonts that are cached in memory. */
	static void setTypefaceCacheSize (int numFontsToCache);

protected:

	String name, style;

	Typeface (const String& name, const String& style) noexcept;

	static Ptr getFallbackTypeface();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Typeface);
};

#endif   // __JUCE_TYPEFACE_JUCEHEADER__

/*** End of inlined file: juce_Typeface.h ***/

class LowLevelGraphicsContext;

/**
	Represents a particular font, including its size, style, etc.

	Apart from the typeface to be used, a Font object also dictates whether
	the font is bold, italic, underlined, how big it is, and its kerning and
	horizontal scale factor.

	@see Typeface
*/
class JUCE_API  Font
{
public:

	/** A combination of these values is used by the constructor to specify the
		style of font to use.
	*/
	enum FontStyleFlags
	{
		plain       = 0,    /**< indicates a plain, non-bold, non-italic version of the font. @see setStyleFlags */
		bold        = 1,    /**< boldens the font. @see setStyleFlags */
		italic      = 2,    /**< finds an italic version of the font. @see setStyleFlags */
		underlined  = 4     /**< underlines the font. @see setStyleFlags */
	};

	/** Creates a sans-serif font in a given size.

		@param fontHeight   the height in pixels (can be fractional)
		@param styleFlags   the style to use - this can be a combination of the
							Font::bold, Font::italic and Font::underlined, or
							just Font::plain for the normal style.
		@see FontStyleFlags, getDefaultSansSerifFontName
	*/
	Font (float fontHeight, int styleFlags = plain);

	/** Creates a font with a given typeface and parameters.

		@param typefaceName the font family of the typeface to use
		@param fontHeight   the height in pixels (can be fractional)
		@param styleFlags   the style to use - this can be a combination of the
							Font::bold, Font::italic and Font::underlined, or
							just Font::plain for the normal style.
		@see FontStyleFlags, getDefaultSansSerifFontName
	*/
	Font (const String& typefaceName, float fontHeight, int styleFlags);

	/** Creates a sans-serif font in a given style and size.

		@param typefaceStyle the font style of the typeface to use
		@param fontHeight   the height in pixels (can be fractional)
	*/
	Font (const String& typefaceStyle, float fontHeight);

	/** Creates a font with a given typeface and parameters.

		@param typefaceName  the font family of the typeface to use
		@param typefaceStyle the font style of the typeface to use
		@param fontHeight    the height in pixels (can be fractional)
	*/
	Font (const String& typefaceName, const String& typefaceStyle, float fontHeight);

	/** Creates a copy of another Font object. */
	Font (const Font& other) noexcept;

	/** Creates a font for a typeface. */
	Font (const Typeface::Ptr& typeface);

	/** Creates a basic sans-serif font at a default height.

		You should use one of the other constructors for creating a font that you're planning
		on drawing with - this constructor is here to help initialise objects before changing
		the font's settings later.
	*/
	Font();

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	Font (Font&& other) noexcept;
	Font& operator= (Font&& other) noexcept;
   #endif

	/** Copies this font from another one. */
	Font& operator= (const Font& other) noexcept;

	bool operator== (const Font& other) const noexcept;
	bool operator!= (const Font& other) const noexcept;

	/** Destructor. */
	~Font() noexcept;

	/** Changes the font family of the typeface.

		e.g. "Arial", "Courier", etc.

		This may also be set to Font::getDefaultSansSerifFontName(), Font::getDefaultSerifFontName(),
		or Font::getDefaultMonospacedFontName(), which are not actual platform-specific font family names,
		but are generic font family names that are used to represent the various default fonts.
		If you need to know the exact typeface font family being used, you can call
		Font::getTypeface()->getName(), which will give you the platform-specific font family.

		If a suitable font isn't found on the machine, it'll just use a default instead.
	*/
	void setTypefaceName (const String& faceName);

	/** Returns the font family of the typeface that this font uses.

		e.g. "Arial", "Courier", etc.

		This may also be set to Font::getDefaultSansSerifFontName(), Font::getDefaultSerifFontName(),
		or Font::getDefaultMonospacedFontName(), which are not actual platform-specific font family names,
		but are generic font familiy names that are used to represent the various default fonts.

		If you need to know the exact typeface font family being used, you can call
		Font::getTypeface()->getName(), which will give you the platform-specific font family.
	*/
	const String& getTypefaceName() const noexcept;

	/** Changes the font style of the typeface

		e.g. "Regular", "Italic", etc.

	*/
	void setTypefaceStyle (const String& typefaceStyle);

	/** Returns the font style of the typeface that this font uses.

		e.g. "Regular", "Italic", etc.

	*/
	const String& getTypefaceStyle() const noexcept;

	/** Returns a list of the styles that this font can use. */
	StringArray getAvailableStyles() const;

	/** Returns a typeface font family that represents the default sans-serif font.

		This is also the typeface that will be used when a font is created without
		specifying any typeface details.

		Note that this method just returns a generic placeholder string that means "the default
		sans-serif font" - it's not the actual font family of this font.

		@see setTypefaceName, getDefaultSerifFontName, getDefaultMonospacedFontName
	*/
	static const String& getDefaultSansSerifFontName();

	/** Returns a typeface font family that represents the default sans-serif font.

		Note that this method just returns a generic placeholder string that means "the default
		serif font" - it's not the actual font family of this font.

		@see setTypefaceName, getDefaultSansSerifFontName, getDefaultMonospacedFontName
	*/
	static const String& getDefaultSerifFontName();

	/** Returns a typeface font family that represents the default sans-serif font.

		Note that this method just returns a generic placeholder string that means "the default
		monospaced font" - it's not the actual font family of this font.

		@see setTypefaceName, getDefaultSansSerifFontName, getDefaultSerifFontName
	*/
	static const String& getDefaultMonospacedFontName();

	/** Returns a typeface font style that represents the default sans-serif font.

		Note that this method just returns a generic placeholder string that means "the default
		font style" - it's not the actual font style of this font.

		@see setTypefaceStyle
	*/
	static const String& getDefaultStyle();

	/** Returns the default system typeface for the given font. */
	static Typeface::Ptr getDefaultTypefaceForFont (const Font& font);

	/** Returns the total height of this font.

		This is the maximum height, from the top of the ascent to the bottom of the
		descenders.

		@see withHeight, setHeightWithoutChangingWidth, getAscent
	*/
	float getHeight() const noexcept;

	/** Returns a copy of this font with a new height. */
	Font withHeight (float height) const;

	/** Changes the font's height.
		@see getHeight, withHeight, setHeightWithoutChangingWidth
	*/
	void setHeight (float newHeight);

	/** Changes the font's height without changing its width.
		This alters the horizontal scale to compensate for the change in height.
	*/
	void setHeightWithoutChangingWidth (float newHeight);

	/** Returns the height of the font above its baseline.
		This is the maximum height from the baseline to the top.
		@see getHeight, getDescent
	*/
	float getAscent() const;

	/** Returns the amount that the font descends below its baseline.
		This is calculated as (getHeight() - getAscent()).
		@see getAscent, getHeight
	*/
	float getDescent() const;

	/** Returns the font's style flags.
		This will return a bitwise-or'ed combination of values from the FontStyleFlags
		enum, to describe whether the font is bold, italic, etc.
		@see FontStyleFlags, withStyle
	*/
	int getStyleFlags() const noexcept;

	/** Returns a copy of this font with the given set of style flags.
		@param styleFlags     a bitwise-or'ed combination of values from the FontStyleFlags enum.
		@see FontStyleFlags, getStyleFlags
	*/
	Font withStyle (int styleFlags) const;

	/** Changes the font's style.
		@param newFlags     a bitwise-or'ed combination of values from the FontStyleFlags enum.
		@see FontStyleFlags, withStyle
	*/
	void setStyleFlags (int newFlags);

	/** Makes the font bold or non-bold. */
	void setBold (bool shouldBeBold);
	/** Returns a copy of this font with the bold attribute set. */
	Font boldened() const;
	/** Returns true if the font is bold. */
	bool isBold() const noexcept;

	/** Makes the font italic or non-italic. */
	void setItalic (bool shouldBeItalic);
	/** Returns a copy of this font with the italic attribute set. */
	Font italicised() const;
	/** Returns true if the font is italic. */
	bool isItalic() const noexcept;

	/** Makes the font underlined or non-underlined. */
	void setUnderline (bool shouldBeUnderlined);
	/** Returns true if the font is underlined. */
	bool isUnderlined() const noexcept;

	/** Returns the font's horizontal scale.

		A value of 1.0 is the normal scale, less than this will be narrower, greater
		than 1.0 will be stretched out.

		@see withHorizontalScale
	*/
	float getHorizontalScale() const noexcept;

	/** Returns a copy of this font with a new horizontal scale.
		@param scaleFactor  a value of 1.0 is the normal scale, less than this will be
							narrower, greater than 1.0 will be stretched out.
		@see getHorizontalScale
	*/
	Font withHorizontalScale (float scaleFactor) const;

	/** Changes the font's horizontal scale factor.
		@param scaleFactor  a value of 1.0 is the normal scale, less than this will be
							narrower, greater than 1.0 will be stretched out.
	*/
	void setHorizontalScale (float scaleFactor);

	/** Returns the font's kerning.

		This is the extra space added between adjacent characters, as a proportion
		of the font's height.

		A value of zero is normal spacing, positive values will spread the letters
		out more, and negative values make them closer together.
	*/
	float getExtraKerningFactor() const noexcept;

	/** Returns a copy of this font with a new kerning factor.
		@param extraKerning     a multiple of the font's height that will be added
								to space between the characters. So a value of zero is
								normal spacing, positive values spread the letters out,
								negative values make them closer together.
	*/
	Font withExtraKerningFactor (float extraKerning) const;

	/** Changes the font's kerning.
		@param extraKerning     a multiple of the font's height that will be added
								to space between the characters. So a value of zero is
								normal spacing, positive values spread the letters out,
								negative values make them closer together.
	*/
	void setExtraKerningFactor (float extraKerning);

	/** Changes all the font's characteristics with one call. */
	void setSizeAndStyle (float newHeight,
						  int newStyleFlags,
						  float newHorizontalScale,
						  float newKerningAmount);

	/** Changes all the font's characteristics with one call. */
	void setSizeAndStyle (float newHeight,
						  const String& newStyle,
						  float newHorizontalScale,
						  float newKerningAmount);

	/** Returns the total width of a string as it would be drawn using this font.

		For a more accurate floating-point result, use getStringWidthFloat().
	*/
	int getStringWidth (const String& text) const;

	/** Returns the total width of a string as it would be drawn using this font.

		@see getStringWidth
	*/
	float getStringWidthFloat (const String& text) const;

	/** Returns the series of glyph numbers and their x offsets needed to represent a string.

		An extra x offset is added at the end of the run, to indicate where the right hand
		edge of the last character is.
	*/
	void getGlyphPositions (const String& text, Array <int>& glyphs, Array <float>& xOffsets) const;

	/** Returns the typeface used by this font.

		Note that the object returned may go out of scope if this font is deleted
		or has its style changed.
	*/
	Typeface* getTypeface() const;

	/** Creates an array of Font objects to represent all the fonts on the system.

		If you just need the font family names of the typefaces, you can also use
		findAllTypefaceNames() instead.

		@param results  the array to which new Font objects will be added.
	*/
	static void findFonts (Array<Font>& results);

	/** Returns a list of all the available typeface font families.

		The names returned can be passed into setTypefaceName().

		You can use this instead of findFonts() if you only need their font family names,
		and not font objects.
	*/
	static StringArray findAllTypefaceNames();

	/** Returns a list of all the available typeface font styles.

		The names returned can be passed into setTypefaceStyle().

		You can use this instead of findFonts() if you only need their styles, and not
		font objects.
	*/
	static StringArray findAllTypefaceStyles (const String& family);

	/** Returns the font family of the typeface to be used for rendering glyphs that aren't
		found in the requested typeface.
	*/
	static const String& getFallbackFontName();

	/** Sets the (platform-specific) font family of the typeface to use to find glyphs that
		aren't available in whatever font you're trying to use.
	*/
	static void setFallbackFontName (const String& name);

	/** Returns the font style of the typeface to be used for rendering glyphs that aren't
		found in the requested typeface.
	*/
	static const String& getFallbackFontStyle();

	/** Sets the (platform-specific) font style of the typeface to use to find glyphs that
		aren't available in whatever font you're trying to use.
	*/
	static void setFallbackFontStyle (const String& style);

	/** Creates a string to describe this font.
		The string will contain information to describe the font's typeface, size, and
		style. To recreate the font from this string, use fromString().
	*/
	String toString() const;

	/** Recreates a font from its stringified encoding.
		This method takes a string that was created by toString(), and recreates the
		original font.
	*/
	static Font fromString (const String& fontDescription);

private:

	class SharedFontInternal;
	ReferenceCountedObjectPtr <SharedFontInternal> font;
	void dupeInternalIfShared();

	JUCE_LEAK_DETECTOR (Font);
};

#endif   // __JUCE_FONT_JUCEHEADER__

/*** End of inlined file: juce_Font.h ***/


/*** Start of inlined file: juce_Rectangle.h ***/
#ifndef __JUCE_RECTANGLE_JUCEHEADER__
#define __JUCE_RECTANGLE_JUCEHEADER__

class RectangleList;

/**
	Manages a rectangle and allows geometric operations to be performed on it.

	@see RectangleList, Path, Line, Point
*/
template <typename ValueType>
class Rectangle
{
public:

	/** Creates a rectangle of zero size.

		The default co-ordinates will be (0, 0, 0, 0).
	*/
	Rectangle() noexcept
	  : w(), h()
	{
	}

	/** Creates a copy of another rectangle. */
	Rectangle (const Rectangle& other) noexcept
	  : pos (other.pos), w (other.w), h (other.h)
	{
	}

	/** Creates a rectangle with a given position and size. */
	Rectangle (const ValueType initialX, const ValueType initialY,
			   const ValueType width, const ValueType height) noexcept
	  : pos (initialX, initialY),
		w (width), h (height)
	{
	}

	/** Creates a rectangle with a given size, and a position of (0, 0). */
	Rectangle (const ValueType width, const ValueType height) noexcept
	  : w (width), h (height)
	{
	}

	/** Creates a Rectangle from the positions of two opposite corners. */
	Rectangle (const Point<ValueType>& corner1, const Point<ValueType>& corner2) noexcept
	  : pos (jmin (corner1.x, corner2.x),
			 jmin (corner1.y, corner2.y)),
		w (corner1.x - corner2.x),
		h (corner1.y - corner2.y)
	{
		if (w < ValueType()) w = -w;
		if (h < ValueType()) h = -h;
	}

	/** Creates a Rectangle from a set of left, right, top, bottom coordinates.
		The right and bottom values must be larger than the left and top ones, or the resulting
		rectangle will have a negative size.
	*/
	static Rectangle leftTopRightBottom (const ValueType left, const ValueType top,
										 const ValueType right, const ValueType bottom) noexcept
	{
		return Rectangle (left, top, right - left, bottom - top);
	}

	Rectangle& operator= (const Rectangle& other) noexcept
	{
		pos = other.pos;
		w = other.w; h = other.h;
		return *this;
	}

	/** Destructor. */
	~Rectangle() noexcept {}

	/** Returns true if the rectangle's width and height are both zero or less */
	bool isEmpty() const noexcept                                   { return w <= ValueType() || h <= ValueType(); }

	/** Returns the x co-ordinate of the rectangle's left-hand-side. */
	inline ValueType getX() const noexcept                          { return pos.x; }

	/** Returns the y co-ordinate of the rectangle's top edge. */
	inline ValueType getY() const noexcept                          { return pos.y; }

	/** Returns the width of the rectangle. */
	inline ValueType getWidth() const noexcept                      { return w; }

	/** Returns the height of the rectangle. */
	inline ValueType getHeight() const noexcept                     { return h; }

	/** Returns the x co-ordinate of the rectangle's right-hand-side. */
	inline ValueType getRight() const noexcept                      { return pos.x + w; }

	/** Returns the y co-ordinate of the rectangle's bottom edge. */
	inline ValueType getBottom() const noexcept                     { return pos.y + h; }

	/** Returns the x co-ordinate of the rectangle's centre. */
	ValueType getCentreX() const noexcept                           { return pos.x + w / (ValueType) 2; }

	/** Returns the y co-ordinate of the rectangle's centre. */
	ValueType getCentreY() const noexcept                           { return pos.y + h / (ValueType) 2; }

	/** Returns the centre point of the rectangle. */
	Point<ValueType> getCentre() const noexcept                     { return Point<ValueType> (pos.x + w / (ValueType) 2,
																							   pos.y + h / (ValueType) 2); }

	/** Returns the aspect ratio of the rectangle's width / height.
		If widthOverHeight is true, it returns width / height; if widthOverHeight is false,
		it returns height / width. */
	ValueType getAspectRatio (const bool widthOverHeight = true) const noexcept                     { return widthOverHeight ? w / h : h / w; }

	/** Returns the rectangle's top-left position as a Point. */
	inline const Point<ValueType>& getPosition() const noexcept                                     { return pos; }

	/** Changes the position of the rectangle's top-left corner (leaving its size unchanged). */
	inline void setPosition (const Point<ValueType>& newPos) noexcept                               { pos = newPos; }

	/** Changes the position of the rectangle's top-left corner (leaving its size unchanged). */
	inline void setPosition (const ValueType newX, const ValueType newY) noexcept                   { pos.setXY (newX, newY); }

	/** Returns the rectangle's top-left position as a Point. */
	const Point<ValueType>& getTopLeft() const noexcept                                             { return pos; }

	/** Returns the rectangle's top-right position as a Point. */
	Point<ValueType> getTopRight() const noexcept                                                   { return Point<ValueType> (pos.x + w, pos.y); }

	/** Returns the rectangle's bottom-left position as a Point. */
	Point<ValueType> getBottomLeft() const noexcept                                                 { return Point<ValueType> (pos.x, pos.y + h); }

	/** Returns the rectangle's bottom-right position as a Point. */
	Point<ValueType> getBottomRight() const noexcept                                                { return Point<ValueType> (pos.x + w, pos.y + h); }

	/** Changes the rectangle's size, leaving the position of its top-left corner unchanged. */
	void setSize (const ValueType newWidth, const ValueType newHeight) noexcept                     { w = newWidth; h = newHeight; }

	/** Changes all the rectangle's co-ordinates. */
	void setBounds (const ValueType newX, const ValueType newY,
					const ValueType newWidth, const ValueType newHeight) noexcept                   { pos.x = newX; pos.y = newY; w = newWidth; h = newHeight; }

	/** Changes the rectangle's X coordinate */
	inline void setX (const ValueType newX) noexcept                        { pos.x = newX; }

	/** Changes the rectangle's Y coordinate */
	inline void setY (const ValueType newY) noexcept                        { pos.y = newY; }

	/** Changes the rectangle's width */
	inline void setWidth (const ValueType newWidth) noexcept                { w = newWidth; }

	/** Changes the rectangle's height */
	inline void setHeight (const ValueType newHeight) noexcept              { h = newHeight; }

	/** Returns a rectangle which has the same size and y-position as this one, but with a different x-position. */
	Rectangle withX (const ValueType newX) const noexcept                   { return Rectangle (newX, pos.y, w, h); }

	/** Returns a rectangle which has the same size and x-position as this one, but with a different y-position. */
	Rectangle withY (const ValueType newY) const noexcept                   { return Rectangle (pos.x, newY, w, h); }

	/** Returns a rectangle with the same size as this one, but a new position. */
	Rectangle withPosition (const ValueType newX, const ValueType newY) const noexcept   { return Rectangle (newX, newY, w, h); }

	/** Returns a rectangle with the same size as this one, but a new position. */
	Rectangle withPosition (const Point<ValueType>& newPos) const noexcept  { return Rectangle (newPos.x, newPos.y, w, h); }

	/** Returns a rectangle whose size is the same as this one, but whose top-left position is (0, 0). */
	Rectangle withZeroOrigin() const noexcept                               { return Rectangle (w, h); }

	/** Returns a rectangle which has the same position and height as this one, but with a different width. */
	Rectangle withWidth (const ValueType newWidth) const noexcept           { return Rectangle (pos.x, pos.y, newWidth, h); }

	/** Returns a rectangle which has the same position and width as this one, but with a different height. */
	Rectangle withHeight (const ValueType newHeight) const noexcept         { return Rectangle (pos.x, pos.y, w, newHeight); }

	/** Returns a rectangle with the same position as this one, but a new size. */
	Rectangle withSize (const ValueType newWidth, const ValueType newHeight) const noexcept         { return Rectangle (pos.x, pos.y, newWidth, newHeight); }

	/** Moves the x position, adjusting the width so that the right-hand edge remains in the same place.
		If the x is moved to be on the right of the current right-hand edge, the width will be set to zero.
		@see withLeft
	*/
	void setLeft (const ValueType newLeft) noexcept                   { w = jmax (ValueType(), pos.x + w - newLeft); pos.x = newLeft; }

	/** Returns a new rectangle with a different x position, but the same right-hand edge as this one.
		If the new x is beyond the right of the current right-hand edge, the width will be set to zero.
		@see setLeft
	*/
	Rectangle withLeft (const ValueType newLeft) const noexcept       { return Rectangle (newLeft, pos.y, jmax (ValueType(), pos.x + w - newLeft), h); }

	/** Moves the y position, adjusting the height so that the bottom edge remains in the same place.
		If the y is moved to be below the current bottom edge, the height will be set to zero.
		@see withTop
	*/
	void setTop (const ValueType newTop) noexcept                     { h = jmax (ValueType(), pos.y + h - newTop); pos.y = newTop; }

	/** Returns a new rectangle with a different y position, but the same bottom edge as this one.
		If the new y is beyond the bottom of the current rectangle, the height will be set to zero.
		@see setTop
	*/
	Rectangle withTop (const ValueType newTop) const noexcept         { return Rectangle (pos.x, newTop, w, jmax (ValueType(), pos.y + h - newTop)); }

	/** Adjusts the width so that the right-hand edge of the rectangle has this new value.
		If the new right is below the current X value, the X will be pushed down to match it.
		@see getRight, withRight
	*/
	void setRight (const ValueType newRight) noexcept                 { pos.x = jmin (pos.x, newRight); w = newRight - pos.x; }

	/** Returns a new rectangle with a different right-hand edge position, but the same left-hand edge as this one.
		If the new right edge is below the current left-hand edge, the width will be set to zero.
		@see setRight
	*/
	Rectangle withRight (const ValueType newRight) const noexcept     { return Rectangle (jmin (pos.x, newRight), pos.y, jmax (ValueType(), newRight - pos.x), h); }

	/** Adjusts the height so that the bottom edge of the rectangle has this new value.
		If the new bottom is lower than the current Y value, the Y will be pushed down to match it.
		@see getBottom, withBottom
	*/
	void setBottom (const ValueType newBottom) noexcept               { pos.y = jmin (pos.y, newBottom); h = newBottom - pos.y; }

	/** Returns a new rectangle with a different bottom edge position, but the same top edge as this one.
		If the new y is beyond the bottom of the current rectangle, the height will be set to zero.
		@see setBottom
	*/
	Rectangle withBottom (const ValueType newBottom) const noexcept   { return Rectangle (pos.x, jmin (pos.y, newBottom), w, jmax (ValueType(), newBottom - pos.y)); }

	/** Moves the rectangle's position by adding amount to its x and y co-ordinates. */
	void translate (const ValueType deltaX,
					const ValueType deltaY) noexcept
	{
		pos.x += deltaX;
		pos.y += deltaY;
	}

	/** Returns a rectangle which is the same as this one moved by a given amount. */
	Rectangle translated (const ValueType deltaX,
						  const ValueType deltaY) const noexcept
	{
		return Rectangle (pos.x + deltaX, pos.y + deltaY, w, h);
	}

	/** Returns a rectangle which is the same as this one moved by a given amount. */
	Rectangle operator+ (const Point<ValueType>& deltaPosition) const noexcept
	{
		return Rectangle (pos.x + deltaPosition.x, pos.y + deltaPosition.y, w, h);
	}

	/** Moves this rectangle by a given amount. */
	Rectangle& operator+= (const Point<ValueType>& deltaPosition) noexcept
	{
		pos += deltaPosition;
		return *this;
	}

	/** Returns a rectangle which is the same as this one moved by a given amount. */
	Rectangle operator- (const Point<ValueType>& deltaPosition) const noexcept
	{
		return Rectangle (pos.x - deltaPosition.x, pos.y - deltaPosition.y, w, h);
	}

	/** Moves this rectangle by a given amount. */
	Rectangle& operator-= (const Point<ValueType>& deltaPosition) noexcept
	{
		pos -= deltaPosition;
		return *this;
	}

	/** Expands the rectangle by a given amount.

		Effectively, its new size is (x - deltaX, y - deltaY, w + deltaX * 2, h + deltaY * 2).
		@see expanded, reduce, reduced
	*/
	void expand (const ValueType deltaX,
				 const ValueType deltaY) noexcept
	{
		const ValueType nw = jmax (ValueType(), w + deltaX * 2);
		const ValueType nh = jmax (ValueType(), h + deltaY * 2);
		setBounds (pos.x - deltaX, pos.y - deltaY, nw, nh);
	}

	/** Returns a rectangle that is larger than this one by a given amount.

		Effectively, the rectangle returned is (x - deltaX, y - deltaY, w + deltaX * 2, h + deltaY * 2).
		@see expand, reduce, reduced
	*/
	Rectangle expanded (const ValueType deltaX,
						const ValueType deltaY) const noexcept
	{
		const ValueType nw = jmax (ValueType(), w + deltaX * 2);
		const ValueType nh = jmax (ValueType(), h + deltaY * 2);
		return Rectangle (pos.x - deltaX, pos.y - deltaY, nw, nh);
	}

	/** Shrinks the rectangle by a given amount.

		Effectively, its new size is (x + deltaX, y + deltaY, w - deltaX * 2, h - deltaY * 2).
		@see reduced, expand, expanded
	*/
	void reduce (const ValueType deltaX,
				 const ValueType deltaY) noexcept
	{
		expand (-deltaX, -deltaY);
	}

	/** Returns a rectangle that is smaller than this one by a given amount.

		Effectively, the rectangle returned is (x + deltaX, y + deltaY, w - deltaX * 2, h - deltaY * 2).
		@see reduce, expand, expanded
	*/
	Rectangle reduced (const ValueType deltaX,
					   const ValueType deltaY) const noexcept
	{
		return expanded (-deltaX, -deltaY);
	}

	/** Removes a strip from the top of this rectangle, reducing this rectangle
		by the specified amount and returning the section that was removed.

		E.g. if this rectangle is (100, 100, 300, 300) and amountToRemove is 50, this will
		return (100, 100, 300, 50) and leave this rectangle as (100, 150, 300, 250).

		If amountToRemove is greater than the height of this rectangle, it'll be clipped to
		that value.
	*/
	Rectangle removeFromTop (const ValueType amountToRemove) noexcept
	{
		const Rectangle r (pos.x, pos.y, w, jmin (amountToRemove, h));
		pos.y += r.h; h -= r.h;
		return r;
	}

	/** Removes a strip from the left-hand edge of this rectangle, reducing this rectangle
		by the specified amount and returning the section that was removed.

		E.g. if this rectangle is (100, 100, 300, 300) and amountToRemove is 50, this will
		return (100, 100, 50, 300) and leave this rectangle as (150, 100, 250, 300).

		If amountToRemove is greater than the width of this rectangle, it'll be clipped to
		that value.
	*/
	Rectangle removeFromLeft (const ValueType amountToRemove) noexcept
	{
		const Rectangle r (pos.x, pos.y, jmin (amountToRemove, w), h);
		pos.x += r.w; w -= r.w;
		return r;
	}

	/** Removes a strip from the right-hand edge of this rectangle, reducing this rectangle
		by the specified amount and returning the section that was removed.

		E.g. if this rectangle is (100, 100, 300, 300) and amountToRemove is 50, this will
		return (250, 100, 50, 300) and leave this rectangle as (100, 100, 250, 300).

		If amountToRemove is greater than the width of this rectangle, it'll be clipped to
		that value.
	*/
	Rectangle removeFromRight (ValueType amountToRemove) noexcept
	{
		amountToRemove = jmin (amountToRemove, w);
		const Rectangle r (pos.x + w - amountToRemove, pos.y, amountToRemove, h);
		w -= amountToRemove;
		return r;
	}

	/** Removes a strip from the bottom of this rectangle, reducing this rectangle
		by the specified amount and returning the section that was removed.

		E.g. if this rectangle is (100, 100, 300, 300) and amountToRemove is 50, this will
		return (100, 250, 300, 50) and leave this rectangle as (100, 100, 300, 250).

		If amountToRemove is greater than the height of this rectangle, it'll be clipped to
		that value.
	*/
	Rectangle removeFromBottom (ValueType amountToRemove) noexcept
	{
		amountToRemove = jmin (amountToRemove, h);
		const Rectangle r (pos.x, pos.y + h - amountToRemove, w, amountToRemove);
		h -= amountToRemove;
		return r;
	}

	/** Returns true if the two rectangles are identical. */
	bool operator== (const Rectangle& other) const noexcept     { return pos == other.pos && w == other.w && h == other.h; }

	/** Returns true if the two rectangles are not identical. */
	bool operator!= (const Rectangle& other) const noexcept     { return pos != other.pos || w != other.w || h != other.h; }

	/** Returns true if this co-ordinate is inside the rectangle. */
	bool contains (const ValueType xCoord, const ValueType yCoord) const noexcept
	{
		return xCoord >= pos.x && yCoord >= pos.y && xCoord < pos.x + w && yCoord < pos.y + h;
	}

	/** Returns true if this co-ordinate is inside the rectangle. */
	bool contains (const Point<ValueType>& point) const noexcept
	{
		return point.x >= pos.x && point.y >= pos.y && point.x < pos.x + w && point.y < pos.y + h;
	}

	/** Returns true if this other rectangle is completely inside this one. */
	bool contains (const Rectangle& other) const noexcept
	{
		return pos.x <= other.pos.x && pos.y <= other.pos.y
			&& pos.x + w >= other.pos.x + other.w && pos.y + h >= other.pos.y + other.h;
	}

	/** Returns the nearest point to the specified point that lies within this rectangle. */
	Point<ValueType> getConstrainedPoint (const Point<ValueType>& point) const noexcept
	{
		return Point<ValueType> (jlimit (pos.x, pos.x + w, point.x),
								 jlimit (pos.y, pos.y + h, point.y));
	}

	/** Returns true if any part of another rectangle overlaps this one. */
	bool intersects (const Rectangle& other) const noexcept
	{
		return pos.x + w > other.pos.x
			&& pos.y + h > other.pos.y
			&& pos.x < other.pos.x + other.w
			&& pos.y < other.pos.y + other.h
			&& w > ValueType() && h > ValueType();
	}

	/** Returns the region that is the overlap between this and another rectangle.
		If the two rectangles don't overlap, the rectangle returned will be empty.
	*/
	Rectangle getIntersection (const Rectangle& other) const noexcept
	{
		const ValueType nx = jmax (pos.x, other.pos.x);
		const ValueType ny = jmax (pos.y, other.pos.y);
		const ValueType nw = jmin (pos.x + w, other.pos.x + other.w) - nx;
		const ValueType nh = jmin (pos.y + h, other.pos.y + other.h) - ny;

		if (nw >= ValueType() && nh >= ValueType())
			return Rectangle (nx, ny, nw, nh);

		return Rectangle();
	}

	/** Clips a rectangle so that it lies only within this one.
		This is a non-static version of intersectRectangles().
		Returns false if the two regions didn't overlap.
	*/
	bool intersectRectangle (ValueType& otherX, ValueType& otherY, ValueType& otherW, ValueType& otherH) const noexcept
	{
		const ValueType maxX (jmax (otherX, pos.x));
		otherW = jmin (otherX + otherW, pos.x + w) - maxX;

		if (otherW > ValueType())
		{
			const ValueType maxY (jmax (otherY, pos.y));
			otherH = jmin (otherY + otherH, pos.y + h) - maxY;

			if (otherH > ValueType())
			{
				otherX = maxX; otherY = maxY;
				return true;
			}
		}

		return false;
	}

	/** Returns the smallest rectangle that contains both this one and the one passed-in.

		If either this or the other rectangle are empty, they will not be counted as
		part of the resulting region.
	*/
	Rectangle getUnion (const Rectangle& other) const noexcept
	{
		if (other.isEmpty())  return *this;
		if (isEmpty())        return other;

		const ValueType newX = jmin (pos.x, other.pos.x);
		const ValueType newY = jmin (pos.y, other.pos.y);

		return Rectangle (newX, newY,
						  jmax (pos.x + w, other.pos.x + other.w) - newX,
						  jmax (pos.y + h, other.pos.y + other.h) - newY);
	}

	/** If this rectangle merged with another one results in a simple rectangle, this
		will set this rectangle to the result, and return true.

		Returns false and does nothing to this rectangle if the two rectangles don't overlap,
		or if they form a complex region.
	*/
	bool enlargeIfAdjacent (const Rectangle& other) noexcept
	{
		if (pos.x == other.pos.x && getRight() == other.getRight()
			 && (other.getBottom() >= pos.y && other.pos.y <= getBottom()))
		{
			const ValueType newY = jmin (pos.y, other.pos.y);
			h = jmax (getBottom(), other.getBottom()) - newY;
			pos.y = newY;
			return true;
		}
		else if (pos.y == other.pos.y && getBottom() == other.getBottom()
				  && (other.getRight() >= pos.x && other.pos.x <= getRight()))
		{
			const ValueType newX = jmin (pos.x, other.pos.x);
			w = jmax (getRight(), other.getRight()) - newX;
			pos.x = newX;
			return true;
		}

		return false;
	}

	/** If after removing another rectangle from this one the result is a simple rectangle,
		this will set this object's bounds to be the result, and return true.

		Returns false and does nothing to this rectangle if the two rectangles don't overlap,
		or if removing the other one would form a complex region.
	*/
	bool reduceIfPartlyContainedIn (const Rectangle& other) noexcept
	{
		int inside = 0;
		const ValueType otherR (other.getRight());
		if (pos.x >= other.pos.x && pos.x < otherR) inside = 1;
		const ValueType otherB (other.getBottom());
		if (pos.y >= other.pos.y && pos.y < otherB) inside |= 2;
		const ValueType r (pos.x + w);
		if (r >= other.pos.x && r < otherR) inside |= 4;
		const ValueType b (pos.y + h);
		if (b >= other.pos.y && b < otherB) inside |= 8;

		switch (inside)
		{
			case 1 + 2 + 8:     w = r - otherR; pos.x = otherR; return true;
			case 1 + 2 + 4:     h = b - otherB; pos.y = otherB; return true;
			case 2 + 4 + 8:     w = other.pos.x - pos.x; return true;
			case 1 + 4 + 8:     h = other.pos.y - pos.y; return true;
		}

		return false;
	}

	/** Returns the smallest rectangle that can contain the shape created by applying
		a transform to this rectangle.

		This should only be used on floating point rectangles.
	*/
	Rectangle transformed (const AffineTransform& transform) const noexcept
	{
		float x1 = pos.x,     y1 = pos.y;
		float x2 = pos.x + w, y2 = pos.y;
		float x3 = pos.x,     y3 = pos.y + h;
		float x4 = x2,        y4 = y3;

		transform.transformPoints (x1, y1, x2, y2);
		transform.transformPoints (x3, y3, x4, y4);

		const float rx = jmin (x1, x2, x3, x4);
		const float ry = jmin (y1, y2, y3, y4);

		return Rectangle (rx, ry,
						  jmax (x1, x2, x3, x4) - rx,
						  jmax (y1, y2, y3, y4) - ry);
	}

	/** Returns the smallest integer-aligned rectangle that completely contains this one.
		This is only relevent for floating-point rectangles, of course.
		@see toFloat()
	*/
	Rectangle<int> getSmallestIntegerContainer() const noexcept
	{
		const int x1 = static_cast <int> (std::floor (static_cast<float> (pos.x)));
		const int y1 = static_cast <int> (std::floor (static_cast<float> (pos.y)));
		const int x2 = static_cast <int> (std::ceil  (static_cast<float> (pos.x + w)));
		const int y2 = static_cast <int> (std::ceil  (static_cast<float> (pos.y + h)));

		return Rectangle<int> (x1, y1, x2 - x1, y2 - y1);
	}

	/** Returns the smallest Rectangle that can contain a set of points. */
	static Rectangle findAreaContainingPoints (const Point<ValueType>* const points, const int numPoints) noexcept
	{
		if (numPoints == 0)
			return Rectangle();

		ValueType minX (points[0].x);
		ValueType maxX (minX);
		ValueType minY (points[0].y);
		ValueType maxY (minY);

		for (int i = 1; i < numPoints; ++i)
		{
			minX = jmin (minX, points[i].x);
			maxX = jmax (maxX, points[i].x);
			minY = jmin (minY, points[i].y);
			maxY = jmax (maxY, points[i].y);
		}

		return Rectangle (minX, minY, maxX - minX, maxY - minY);
	}

	/** Casts this rectangle to a Rectangle<float>.
		Obviously this is mainly useful for rectangles that use integer types.
		@see getSmallestIntegerContainer
	*/
	Rectangle<float> toFloat() const noexcept
	{
		return Rectangle<float> (static_cast<float> (pos.x), static_cast<float> (pos.y),
								 static_cast<float> (w),     static_cast<float> (h));
	}

	/** Static utility to intersect two sets of rectangular co-ordinates.
		Returns false if the two regions didn't overlap.
		@see intersectRectangle
	*/
	static bool intersectRectangles (ValueType& x1, ValueType& y1, ValueType& w1, ValueType& h1,
									 const ValueType x2, const ValueType y2, const ValueType w2, const ValueType h2) noexcept
	{
		const ValueType x (jmax (x1, x2));
		w1 = jmin (x1 + w1, x2 + w2) - x;

		if (w1 > ValueType())
		{
			const ValueType y (jmax (y1, y2));
			h1 = jmin (y1 + h1, y2 + h2) - y;

			if (h1 > ValueType())
			{
				x1 = x; y1 = y;
				return true;
			}
		}

		return false;
	}

	/** Creates a string describing this rectangle.

		The string will be of the form "x y width height", e.g. "100 100 400 200".

		Coupled with the fromString() method, this is very handy for things like
		storing rectangles (particularly component positions) in XML attributes.

		@see fromString
	*/
	String toString() const
	{
		String s;
		s.preallocateBytes (32);
		s << pos.x << ' ' << pos.y << ' ' << w << ' ' << h;
		return s;
	}

	/** Parses a string containing a rectangle's details.

		The string should contain 4 integer tokens, in the form "x y width height". They
		can be comma or whitespace separated.

		This method is intended to go with the toString() method, to form an easy way
		of saving/loading rectangles as strings.

		@see toString
	*/
	static Rectangle fromString (const String& stringVersion)
	{
		StringArray toks;
		toks.addTokens (stringVersion.trim(), ",; \t\r\n", String::empty);

		return Rectangle (parseIntAfterSpace (toks[0]),
						  parseIntAfterSpace (toks[1]),
						  parseIntAfterSpace (toks[2]),
						  parseIntAfterSpace (toks[3]));
	}

private:
	friend class RectangleList;
	Point<ValueType> pos;
	ValueType w, h;

	static int parseIntAfterSpace (const String& s) noexcept
		{ return s.getCharPointer().findEndOfWhitespace().getIntValue32(); }
};

#endif   // __JUCE_RECTANGLE_JUCEHEADER__

/*** End of inlined file: juce_Rectangle.h ***/


/*** Start of inlined file: juce_PathStrokeType.h ***/
#ifndef __JUCE_PATHSTROKETYPE_JUCEHEADER__
#define __JUCE_PATHSTROKETYPE_JUCEHEADER__


/*** Start of inlined file: juce_Path.h ***/
#ifndef __JUCE_PATH_JUCEHEADER__
#define __JUCE_PATH_JUCEHEADER__


/*** Start of inlined file: juce_Line.h ***/
#ifndef __JUCE_LINE_JUCEHEADER__
#define __JUCE_LINE_JUCEHEADER__

/**
	Represents a line.

	This class contains a bunch of useful methods for various geometric
	tasks.

	The ValueType template parameter should be a primitive type - float or double
	are what it's designed for. Integer types will work in a basic way, but some methods
	that perform mathematical operations may not compile, or they may not produce
	sensible results.

	@see Point, Rectangle, Path, Graphics::drawLine
*/
template <typename ValueType>
class Line
{
public:

	/** Creates a line, using (0, 0) as its start and end points. */
	Line() noexcept {}

	/** Creates a copy of another line. */
	Line (const Line& other) noexcept
		: start (other.start),
		  end (other.end)
	{
	}

	/** Creates a line based on the co-ordinates of its start and end points. */
	Line (ValueType startX, ValueType startY, ValueType endX, ValueType endY) noexcept
		: start (startX, startY),
		  end (endX, endY)
	{
	}

	/** Creates a line from its start and end points. */
	Line (const Point<ValueType>& startPoint,
		  const Point<ValueType>& endPoint) noexcept
		: start (startPoint),
		  end (endPoint)
	{
	}

	/** Copies a line from another one. */
	Line& operator= (const Line& other) noexcept
	{
		start = other.start;
		end = other.end;
		return *this;
	}

	/** Destructor. */
	~Line() noexcept {}

	/** Returns the x co-ordinate of the line's start point. */
	inline ValueType getStartX() const noexcept                             { return start.x; }

	/** Returns the y co-ordinate of the line's start point. */
	inline ValueType getStartY() const noexcept                             { return start.y; }

	/** Returns the x co-ordinate of the line's end point. */
	inline ValueType getEndX() const noexcept                               { return end.x; }

	/** Returns the y co-ordinate of the line's end point. */
	inline ValueType getEndY() const noexcept                               { return end.y; }

	/** Returns the line's start point. */
	inline const Point<ValueType>& getStart() const noexcept                { return start; }

	/** Returns the line's end point. */
	inline const Point<ValueType>& getEnd() const noexcept                  { return end; }

	/** Changes this line's start point */
	void setStart (ValueType newStartX, ValueType newStartY) noexcept       { start.setXY (newStartX, newStartY); }

	/** Changes this line's end point */
	void setEnd (ValueType newEndX, ValueType newEndY) noexcept             { end.setXY (newEndX, newEndY); }

	/** Changes this line's start point */
	void setStart (const Point<ValueType>& newStart) noexcept               { start = newStart; }

	/** Changes this line's end point */
	void setEnd (const Point<ValueType>& newEnd) noexcept                   { end = newEnd; }

	/** Returns a line that is the same as this one, but with the start and end reversed, */
	const Line reversed() const noexcept                                    { return Line (end, start); }

	/** Applies an affine transform to the line's start and end points. */
	void applyTransform (const AffineTransform& transform) noexcept
	{
		start.applyTransform (transform);
		end.applyTransform (transform);
	}

	/** Returns the length of the line. */
	ValueType getLength() const noexcept                                    { return start.getDistanceFrom (end); }

	/** Returns true if the line's start and end x co-ordinates are the same. */
	bool isVertical() const noexcept                                        { return start.x == end.x; }

	/** Returns true if the line's start and end y co-ordinates are the same. */
	bool isHorizontal() const noexcept                                      { return start.y == end.y; }

	/** Returns the line's angle.

		This value is the number of radians clockwise from the 12 o'clock direction,
		where the line's start point is considered to be at the centre.
	*/
	typename Point<ValueType>::FloatType getAngle() const noexcept          { return start.getAngleToPoint (end); }

	/** Compares two lines. */
	bool operator== (const Line& other) const noexcept                      { return start == other.start && end == other.end; }

	/** Compares two lines. */
	bool operator!= (const Line& other) const noexcept                      { return start != other.start || end != other.end; }

	/** Finds the intersection between two lines.

		@param line             the other line
		@param intersection     the position of the point where the lines meet (or
								where they would meet if they were infinitely long)
								the intersection (if the lines intersect). If the lines
								are parallel, this will just be set to the position
								of one of the line's endpoints.
		@returns    true if the line segments intersect; false if they dont. Even if they
					don't intersect, the intersection co-ordinates returned will still
					be valid
	*/
	bool intersects (const Line& line, Point<ValueType>& intersection) const noexcept
	{
		return findIntersection (start, end, line.start, line.end, intersection);
	}

	/** Finds the intersection between two lines.

		@param line     the line to intersect with
		@returns        the point at which the lines intersect, even if this lies beyond the end of the lines
	*/
	Point<ValueType> getIntersection (const Line& line) const noexcept
	{
		Point<ValueType> p;
		findIntersection (start, end, line.start, line.end, p);
		return p;
	}

	/** Returns the location of the point which is a given distance along this line.

		@param distanceFromStart    the distance to move along the line from its
									start point. This value can be negative or longer
									than the line itself
		@see getPointAlongLineProportionally
	*/
	Point<ValueType> getPointAlongLine (ValueType distanceFromStart) const noexcept
	{
		return start + (end - start) * (distanceFromStart / getLength());
	}

	/** Returns a point which is a certain distance along and to the side of this line.

		This effectively moves a given distance along the line, then another distance
		perpendicularly to this, and returns the resulting position.

		@param distanceFromStart    the distance to move along the line from its
									start point. This value can be negative or longer
									than the line itself
		@param perpendicularDistance    how far to move sideways from the line. If you're
									looking along the line from its start towards its
									end, then a positive value here will move to the
									right, negative value move to the left.
	*/
	Point<ValueType> getPointAlongLine (ValueType distanceFromStart,
										ValueType perpendicularDistance) const noexcept
	{
		const Point<ValueType> delta (end - start);
		const double length = juce_hypot ((double) delta.x,
										  (double) delta.y);
		if (length <= 0)
			return start;

		return Point<ValueType> (start.x + static_cast <ValueType> ((delta.x * distanceFromStart - delta.y * perpendicularDistance) / length),
								 start.y + static_cast <ValueType> ((delta.y * distanceFromStart + delta.x * perpendicularDistance) / length));
	}

	/** Returns the location of the point which is a given distance along this line
		proportional to the line's length.

		@param proportionOfLength   the distance to move along the line from its
									start point, in multiples of the line's length.
									So a value of 0.0 will return the line's start point
									and a value of 1.0 will return its end point. (This value
									can be negative or greater than 1.0).
		@see getPointAlongLine
	*/
	Point<ValueType> getPointAlongLineProportionally (ValueType proportionOfLength) const noexcept
	{
		return start + (end - start) * proportionOfLength;
	}

	/** Returns the smallest distance between this line segment and a given point.

		So if the point is close to the line, this will return the perpendicular
		distance from the line; if the point is a long way beyond one of the line's
		end-point's, it'll return the straight-line distance to the nearest end-point.

		pointOnLine receives the position of the point that is found.

		@returns the point's distance from the line
		@see getPositionAlongLineOfNearestPoint
	*/
	ValueType getDistanceFromPoint (const Point<ValueType>& targetPoint,
									Point<ValueType>& pointOnLine) const noexcept
	{
		const Point<ValueType> delta (end - start);
		const double length = delta.x * delta.x + delta.y * delta.y;

		if (length > 0)
		{
			const double prop = ((targetPoint.x - start.x) * delta.x
							   + (targetPoint.y - start.y) * delta.y) / length;

			if (prop >= 0 && prop <= 1.0)
			{
				pointOnLine = start + delta * static_cast <ValueType> (prop);
				return targetPoint.getDistanceFrom (pointOnLine);
			}
		}

		const float fromStart = targetPoint.getDistanceFrom (start);
		const float fromEnd = targetPoint.getDistanceFrom (end);

		if (fromStart < fromEnd)
		{
			pointOnLine = start;
			return fromStart;
		}
		else
		{
			pointOnLine = end;
			return fromEnd;
		}
	}

	/** Finds the point on this line which is nearest to a given point, and
		returns its position as a proportional position along the line.

		@returns    a value 0 to 1.0 which is the distance along this line from the
					line's start to the point which is nearest to the point passed-in. To
					turn this number into a position, use getPointAlongLineProportionally().
		@see getDistanceFromPoint, getPointAlongLineProportionally
	*/
	ValueType findNearestProportionalPositionTo (const Point<ValueType>& point) const noexcept
	{
		const Point<ValueType> delta (end - start);
		const double length = delta.x * delta.x + delta.y * delta.y;

		return length <= 0 ? 0
						   : jlimit (ValueType(), static_cast <ValueType> (1),
									 static_cast <ValueType> ((((point.x - start.x) * delta.x
															  + (point.y - start.y) * delta.y) / length)));
	}

	/** Finds the point on this line which is nearest to a given point.
		@see getDistanceFromPoint, findNearestProportionalPositionTo
	*/
	Point<ValueType> findNearestPointTo (const Point<ValueType>& point) const noexcept
	{
		return getPointAlongLineProportionally (findNearestProportionalPositionTo (point));
	}

	/** Returns true if the given point lies above this line.

		The return value is true if the point's y coordinate is less than the y
		coordinate of this line at the given x (assuming the line extends infinitely
		in both directions).
	*/
	bool isPointAbove (const Point<ValueType>& point) const noexcept
	{
		return start.x != end.x
				&& point.y < ((end.y - start.y)
									* (point.x - start.x)) / (end.x - start.x) + start.y;
	}

	/** Returns a shortened copy of this line.

		This will chop off part of the start of this line by a certain amount, (leaving the
		end-point the same), and return the new line.
	*/
	Line withShortenedStart (ValueType distanceToShortenBy) const noexcept
	{
		return Line (getPointAlongLine (jmin (distanceToShortenBy, getLength())), end);
	}

	/** Returns a shortened copy of this line.

		This will chop off part of the end of this line by a certain amount, (leaving the
		start-point the same), and return the new line.
	*/
	Line withShortenedEnd (ValueType distanceToShortenBy) const noexcept
	{
		const ValueType length = getLength();
		return Line (start, getPointAlongLine (length - jmin (distanceToShortenBy, length)));
	}

private:

	Point<ValueType> start, end;

	static bool findIntersection (const Point<ValueType>& p1, const Point<ValueType>& p2,
								  const Point<ValueType>& p3, const Point<ValueType>& p4,
								  Point<ValueType>& intersection) noexcept
	{
		if (p2 == p3)
		{
			intersection = p2;
			return true;
		}

		const Point<ValueType> d1 (p2 - p1);
		const Point<ValueType> d2 (p4 - p3);
		const ValueType divisor = d1.x * d2.y - d2.x * d1.y;

		if (divisor == 0)
		{
			if (! (d1.isOrigin() || d2.isOrigin()))
			{
				if (d1.y == 0 && d2.y != 0)
				{
					const ValueType along = (p1.y - p3.y) / d2.y;
					intersection = p1.withX (p3.x + along * d2.x);
					return along >= 0 && along <= static_cast <ValueType> (1);
				}
				else if (d2.y == 0 && d1.y != 0)
				{
					const ValueType along = (p3.y - p1.y) / d1.y;
					intersection = p3.withX (p1.x + along * d1.x);
					return along >= 0 && along <= static_cast <ValueType> (1);
				}
				else if (d1.x == 0 && d2.x != 0)
				{
					const ValueType along = (p1.x - p3.x) / d2.x;
					intersection = p1.withY (p3.y + along * d2.y);
					return along >= 0 && along <= static_cast <ValueType> (1);
				}
				else if (d2.x == 0 && d1.x != 0)
				{
					const ValueType along = (p3.x - p1.x) / d1.x;
					intersection = p3.withY (p1.y + along * d1.y);
					return along >= 0 && along <= static_cast <ValueType> (1);
				}
			}

			intersection = (p2 + p3) / static_cast <ValueType> (2);
			return false;
		}

		const ValueType along1 = ((p1.y - p3.y) * d2.x - (p1.x - p3.x) * d2.y) / divisor;
		intersection = p1 + d1 * along1;

		if (along1 < 0 || along1 > static_cast <ValueType> (1))
			return false;

		const ValueType along2 = ((p1.y - p3.y) * d1.x - (p1.x - p3.x) * d1.y) / divisor;
		return along2 >= 0 && along2 <= static_cast <ValueType> (1);
	}
};

#endif   // __JUCE_LINE_JUCEHEADER__

/*** End of inlined file: juce_Line.h ***/


/*** Start of inlined file: juce_Justification.h ***/
#ifndef __JUCE_JUSTIFICATION_JUCEHEADER__
#define __JUCE_JUSTIFICATION_JUCEHEADER__

/**
	Represents a type of justification to be used when positioning graphical items.

	e.g. it indicates whether something should be placed top-left, top-right,
	centred, etc.

	It is used in various places wherever this kind of information is needed.
*/
class JUCE_API  Justification
{
public:

	/** Creates a Justification object using a combination of flags. */
	inline Justification (int flags_) noexcept : flags (flags_) {}

	/** Creates a copy of another Justification object. */
	Justification (const Justification& other) noexcept;

	/** Copies another Justification object. */
	Justification& operator= (const Justification& other) noexcept;

	bool operator== (const Justification& other) const noexcept     { return flags == other.flags; }
	bool operator!= (const Justification& other) const noexcept     { return flags != other.flags; }

	/** Returns the raw flags that are set for this Justification object. */
	inline int getFlags() const noexcept                            { return flags; }

	/** Tests a set of flags for this object.

		@returns true if any of the flags passed in are set on this object.
	*/
	inline bool testFlags (int flagsToTest) const noexcept          { return (flags & flagsToTest) != 0; }

	/** Returns just the flags from this object that deal with vertical layout. */
	int getOnlyVerticalFlags() const noexcept;

	/** Returns just the flags from this object that deal with horizontal layout. */
	int getOnlyHorizontalFlags() const noexcept;

	/** Adjusts the position of a rectangle to fit it into a space.

		The (x, y) position of the rectangle will be updated to position it inside the
		given space according to the justification flags.
	*/
	template <typename ValueType>
	void applyToRectangle (ValueType& x, ValueType& y, ValueType w, ValueType h,
						   ValueType spaceX, ValueType spaceY, ValueType spaceW, ValueType spaceH) const noexcept
	{
		x = spaceX;
		if ((flags & horizontallyCentred) != 0)     x += (spaceW - w) / (ValueType) 2;
		else if ((flags & right) != 0)              x += spaceW - w;

		y = spaceY;
		if ((flags & verticallyCentred) != 0)       y += (spaceH - h) / (ValueType) 2;
		else if ((flags & bottom) != 0)             y += spaceH - h;
	}

	/** Returns the new position of a rectangle that has been justified to fit within a given space.
	*/
	template <typename ValueType>
	const Rectangle<ValueType> appliedToRectangle (const Rectangle<ValueType>& areaToAdjust,
												   const Rectangle<ValueType>& targetSpace) const noexcept
	{
		ValueType x = areaToAdjust.getX(), y = areaToAdjust.getY();
		applyToRectangle (x, y, areaToAdjust.getWidth(), areaToAdjust.getHeight(),
						  targetSpace.getX(), targetSpace.getY(), targetSpace.getWidth(), targetSpace.getHeight());
		return areaToAdjust.withPosition (x, y);
	}

	/** Flag values that can be combined and used in the constructor. */
	enum
	{

		/** Indicates that the item should be aligned against the left edge of the available space. */
		left                            = 1,

		/** Indicates that the item should be aligned against the right edge of the available space. */
		right                           = 2,

		/** Indicates that the item should be placed in the centre between the left and right
			sides of the available space. */
		horizontallyCentred             = 4,

		/** Indicates that the item should be aligned against the top edge of the available space. */
		top                             = 8,

		/** Indicates that the item should be aligned against the bottom edge of the available space. */
		bottom                          = 16,

		/** Indicates that the item should be placed in the centre between the top and bottom
			sides of the available space. */
		verticallyCentred               = 32,

		/** Indicates that lines of text should be spread out to fill the maximum width
			available, so that both margins are aligned vertically.
		*/
		horizontallyJustified           = 64,

		/** Indicates that the item should be centred vertically and horizontally.
			This is equivalent to (horizontallyCentred | verticallyCentred)
		*/
		centred                         = 36,

		/** Indicates that the item should be centred vertically but placed on the left hand side.
			This is equivalent to (left | verticallyCentred)
		*/
		centredLeft                     = 33,

		/** Indicates that the item should be centred vertically but placed on the right hand side.
			This is equivalent to (right | verticallyCentred)
		*/
		centredRight                    = 34,

		/** Indicates that the item should be centred horizontally and placed at the top.
			This is equivalent to (horizontallyCentred | top)
		*/
		centredTop                      = 12,

		/** Indicates that the item should be centred horizontally and placed at the bottom.
			This is equivalent to (horizontallyCentred | bottom)
		*/
		centredBottom                   = 20,

		/** Indicates that the item should be placed in the top-left corner.
			This is equivalent to (left | top)
		*/
		topLeft                         = 9,

		/** Indicates that the item should be placed in the top-right corner.
			This is equivalent to (right | top)
		*/
		topRight                        = 10,

		/** Indicates that the item should be placed in the bottom-left corner.
			This is equivalent to (left | bottom)
		*/
		bottomLeft                      = 17,

		/** Indicates that the item should be placed in the bottom-left corner.
			This is equivalent to (right | bottom)
		*/
		bottomRight                     = 18
	};

private:

	int flags;
};

#endif   // __JUCE_JUSTIFICATION_JUCEHEADER__

/*** End of inlined file: juce_Justification.h ***/

class Image;
class InputStream;
class OutputStream;

/**
	A path is a sequence of lines and curves that may either form a closed shape
	or be open-ended.

	To use a path, you can create an empty one, then add lines and curves to it
	to create shapes, then it can be rendered by a Graphics context or used
	for geometric operations.

	e.g. @code
	Path myPath;

	myPath.startNewSubPath (10.0f, 10.0f);          // move the current position to (10, 10)
	myPath.lineTo (100.0f, 200.0f);                 // draw a line from here to (100, 200)
	myPath.quadraticTo (0.0f, 150.0f, 5.0f, 50.0f); // draw a curve that ends at (5, 50)
	myPath.closeSubPath();                          // close the subpath with a line back to (10, 10)

	// add an ellipse as well, which will form a second sub-path within the path..
	myPath.addEllipse (50.0f, 50.0f, 40.0f, 30.0f);

	// double the width of the whole thing..
	myPath.applyTransform (AffineTransform::scale (2.0f, 1.0f));

	// and draw it to a graphics context with a 5-pixel thick outline.
	g.strokePath (myPath, PathStrokeType (5.0f));

	@endcode

	A path object can actually contain multiple sub-paths, which may themselves
	be open or closed.

	@see PathFlatteningIterator, PathStrokeType, Graphics
*/
class JUCE_API  Path
{
public:

	/** Creates an empty path. */
	Path();

	/** Creates a copy of another path. */
	Path (const Path& other);

	/** Destructor. */
	~Path();

	/** Copies this path from another one. */
	Path& operator= (const Path& other);

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	Path (Path&& other) noexcept;
	Path& operator= (Path&& other) noexcept;
   #endif

	bool operator== (const Path& other) const noexcept;
	bool operator!= (const Path& other) const noexcept;

	/** Returns true if the path doesn't contain any lines or curves. */
	bool isEmpty() const noexcept;

	/** Returns the smallest rectangle that contains all points within the path.
	*/
	Rectangle<float> getBounds() const noexcept;

	/** Returns the smallest rectangle that contains all points within the path
		after it's been transformed with the given tranasform matrix.
	*/
	Rectangle<float> getBoundsTransformed (const AffineTransform& transform) const noexcept;

	/** Checks whether a point lies within the path.

		This is only relevent for closed paths (see closeSubPath()), and
		may produce false results if used on a path which has open sub-paths.

		The path's winding rule is taken into account by this method.

		The tolerance parameter is the maximum error allowed when flattening the path,
		so this method could return a false positive when your point is up to this distance
		outside the path's boundary.

		@see closeSubPath, setUsingNonZeroWinding
	*/
	bool contains (float x, float y,
				   float tolerance = 1.0f) const;

	/** Checks whether a point lies within the path.

		This is only relevent for closed paths (see closeSubPath()), and
		may produce false results if used on a path which has open sub-paths.

		The path's winding rule is taken into account by this method.

		The tolerance parameter is the maximum error allowed when flattening the path,
		so this method could return a false positive when your point is up to this distance
		outside the path's boundary.

		@see closeSubPath, setUsingNonZeroWinding
	*/
	bool contains (const Point<float>& point,
				   float tolerance = 1.0f) const;

	/** Checks whether a line crosses the path.

		This will return positive if the line crosses any of the paths constituent
		lines or curves. It doesn't take into account whether the line is inside
		or outside the path, or whether the path is open or closed.

		The tolerance parameter is the maximum error allowed when flattening the path,
		so this method could return a false positive when your point is up to this distance
		outside the path's boundary.
	*/
	bool intersectsLine (const Line<float>& line,
						 float tolerance = 1.0f);

	/** Cuts off parts of a line to keep the parts that are either inside or
		outside this path.

		Note that this isn't smart enough to cope with situations where the
		line would need to be cut into multiple pieces to correctly clip against
		a re-entrant shape.

		@param line                     the line to clip
		@param keepSectionOutsidePath   if true, it's the section outside the path
										that will be kept; if false its the section inside
										the path
	*/
	Line<float> getClippedLine (const Line<float>& line, bool keepSectionOutsidePath) const;

	/** Returns the length of the path.
		@see getPointAlongPath
	*/
	float getLength (const AffineTransform& transform = AffineTransform::identity) const;

	/** Returns a point that is the specified distance along the path.
		If the distance is greater than the total length of the path, this will return the
		end point.
		@see getLength
	*/
	Point<float> getPointAlongPath (float distanceFromStart,
									const AffineTransform& transform = AffineTransform::identity) const;

	/** Finds the point along the path which is nearest to a given position.
		This sets pointOnPath to the nearest point, and returns the distance of this point from the start
		of the path.
	*/
	float getNearestPoint (const Point<float>& targetPoint,
						   Point<float>& pointOnPath,
						   const AffineTransform& transform = AffineTransform::identity) const;

	/** Removes all lines and curves, resetting the path completely. */
	void clear() noexcept;

	/** Begins a new subpath with a given starting position.

		This will move the path's current position to the co-ordinates passed in and
		make it ready to draw lines or curves starting from this position.

		After adding whatever lines and curves are needed, you can either
		close the current sub-path using closeSubPath() or call startNewSubPath()
		to move to a new sub-path, leaving the old one open-ended.

		@see lineTo, quadraticTo, cubicTo, closeSubPath
	*/
	void startNewSubPath (float startX, float startY);

	/** Begins a new subpath with a given starting position.

		This will move the path's current position to the co-ordinates passed in and
		make it ready to draw lines or curves starting from this position.

		After adding whatever lines and curves are needed, you can either
		close the current sub-path using closeSubPath() or call startNewSubPath()
		to move to a new sub-path, leaving the old one open-ended.

		@see lineTo, quadraticTo, cubicTo, closeSubPath
	*/
	void startNewSubPath (const Point<float>& start);

	/** Closes a the current sub-path with a line back to its start-point.

		When creating a closed shape such as a triangle, don't use 3 lineTo()
		calls - instead use two lineTo() calls, followed by a closeSubPath()
		to join the final point back to the start.

		This ensures that closes shapes are recognised as such, and this is
		important for tasks like drawing strokes, which needs to know whether to
		draw end-caps or not.

		@see startNewSubPath, lineTo, quadraticTo, cubicTo, closeSubPath
	*/
	void closeSubPath();

	/** Adds a line from the shape's last position to a new end-point.

		This will connect the end-point of the last line or curve that was added
		to a new point, using a straight line.

		See the class description for an example of how to add lines and curves to a path.

		@see startNewSubPath, quadraticTo, cubicTo, closeSubPath
	*/
	void lineTo (float endX, float endY);

	/** Adds a line from the shape's last position to a new end-point.

		This will connect the end-point of the last line or curve that was added
		to a new point, using a straight line.

		See the class description for an example of how to add lines and curves to a path.

		@see startNewSubPath, quadraticTo, cubicTo, closeSubPath
	*/
	void lineTo (const Point<float>& end);

	/** Adds a quadratic bezier curve from the shape's last position to a new position.

		This will connect the end-point of the last line or curve that was added
		to a new point, using a quadratic spline with one control-point.

		See the class description for an example of how to add lines and curves to a path.

		@see startNewSubPath, lineTo, cubicTo, closeSubPath
	*/
	void quadraticTo (float controlPointX,
					  float controlPointY,
					  float endPointX,
					  float endPointY);

	/** Adds a quadratic bezier curve from the shape's last position to a new position.

		This will connect the end-point of the last line or curve that was added
		to a new point, using a quadratic spline with one control-point.

		See the class description for an example of how to add lines and curves to a path.

		@see startNewSubPath, lineTo, cubicTo, closeSubPath
	*/
	void quadraticTo (const Point<float>& controlPoint,
					  const Point<float>& endPoint);

	/** Adds a cubic bezier curve from the shape's last position to a new position.

		This will connect the end-point of the last line or curve that was added
		to a new point, using a cubic spline with two control-points.

		See the class description for an example of how to add lines and curves to a path.

		@see startNewSubPath, lineTo, quadraticTo, closeSubPath
	*/
	void cubicTo (float controlPoint1X,
				  float controlPoint1Y,
				  float controlPoint2X,
				  float controlPoint2Y,
				  float endPointX,
				  float endPointY);

	/** Adds a cubic bezier curve from the shape's last position to a new position.

		This will connect the end-point of the last line or curve that was added
		to a new point, using a cubic spline with two control-points.

		See the class description for an example of how to add lines and curves to a path.

		@see startNewSubPath, lineTo, quadraticTo, closeSubPath
	*/
	void cubicTo (const Point<float>& controlPoint1,
				  const Point<float>& controlPoint2,
				  const Point<float>& endPoint);

	/** Returns the last point that was added to the path by one of the drawing methods.
	*/
	Point<float> getCurrentPosition() const;

	/** Adds a rectangle to the path.
		The rectangle is added as a new sub-path. (Any currently open paths will be left open).
		@see addRoundedRectangle, addTriangle
	*/
	void addRectangle (float x, float y, float width, float height);

	/** Adds a rectangle to the path.
		The rectangle is added as a new sub-path. (Any currently open paths will be left open).
		@see addRoundedRectangle, addTriangle
	*/
	template <typename ValueType>
	void addRectangle (const Rectangle<ValueType>& rectangle)
	{
		addRectangle (static_cast <float> (rectangle.getX()), static_cast <float> (rectangle.getY()),
					  static_cast <float> (rectangle.getWidth()), static_cast <float> (rectangle.getHeight()));
	}

	/** Adds a rectangle with rounded corners to the path.
		The rectangle is added as a new sub-path. (Any currently open paths will be left open).
		@see addRectangle, addTriangle
	*/
	void addRoundedRectangle (float x, float y, float width, float height,
							  float cornerSize);

	/** Adds a rectangle with rounded corners to the path.
		The rectangle is added as a new sub-path. (Any currently open paths will be left open).
		@see addRectangle, addTriangle
	*/
	void addRoundedRectangle (float x, float y, float width, float height,
							  float cornerSizeX,
							  float cornerSizeY);

	/** Adds a rectangle with rounded corners to the path.
		The rectangle is added as a new sub-path. (Any currently open paths will be left open).
		@see addRectangle, addTriangle
	*/
	template <typename ValueType>
	void addRoundedRectangle (const Rectangle<ValueType>& rectangle, float cornerSizeX, float cornerSizeY)
	{
		addRoundedRectangle (static_cast <float> (rectangle.getX()), static_cast <float> (rectangle.getY()),
							 static_cast <float> (rectangle.getWidth()), static_cast <float> (rectangle.getHeight()),
							 cornerSizeX, cornerSizeY);
	}

	/** Adds a rectangle with rounded corners to the path.
		The rectangle is added as a new sub-path. (Any currently open paths will be left open).
		@see addRectangle, addTriangle
	*/
	template <typename ValueType>
	void addRoundedRectangle (const Rectangle<ValueType>& rectangle, float cornerSize)
	{
		addRoundedRectangle (rectangle, cornerSize, cornerSize);
	}

	/** Adds a triangle to the path.

		The triangle is added as a new closed sub-path. (Any currently open paths will be left open).

		Note that whether the vertices are specified in clockwise or anticlockwise
		order will affect how the triangle is filled when it overlaps other
		shapes (the winding order setting will affect this of course).
	*/
	void addTriangle (float x1, float y1,
					  float x2, float y2,
					  float x3, float y3);

	/** Adds a quadrilateral to the path.

		The quad is added as a new closed sub-path. (Any currently open paths will be left open).

		Note that whether the vertices are specified in clockwise or anticlockwise
		order will affect how the quad is filled when it overlaps other
		shapes (the winding order setting will affect this of course).
	*/
	void addQuadrilateral (float x1, float y1,
						   float x2, float y2,
						   float x3, float y3,
						   float x4, float y4);

	/** Adds an ellipse to the path.

		The shape is added as a new sub-path. (Any currently open paths will be left open).

		@see addArc
	*/
	void addEllipse (float x, float y, float width, float height);

	/** Adds an elliptical arc to the current path.

		Note that when specifying the start and end angles, the curve will be drawn either clockwise
		or anti-clockwise according to whether the end angle is greater than the start. This means
		that sometimes you may need to use values greater than 2*Pi for the end angle.

		@param x            the left-hand edge of the rectangle in which the elliptical outline fits
		@param y            the top edge of the rectangle in which the elliptical outline fits
		@param width        the width of the rectangle in which the elliptical outline fits
		@param height       the height of the rectangle in which the elliptical outline fits
		@param fromRadians  the angle (clockwise) in radians at which to start the arc segment (where 0 is the
							top-centre of the ellipse)
		@param toRadians    the angle (clockwise) in radians at which to end the arc segment (where 0 is the
							top-centre of the ellipse). This angle can be greater than 2*Pi, so for example to
							draw a curve clockwise from the 9 o'clock position to the 3 o'clock position via
							12 o'clock, you'd use 1.5*Pi and 2.5*Pi as the start and finish points.
		@param startAsNewSubPath    if true, the arc will begin a new subpath from its starting point; if false,
							it will be added to the current sub-path, continuing from the current postition

		@see addCentredArc, arcTo, addPieSegment, addEllipse
	*/
	void addArc (float x, float y, float width, float height,
				 float fromRadians,
				 float toRadians,
				 bool startAsNewSubPath = false);

	/** Adds an arc which is centred at a given point, and can have a rotation specified.

		Note that when specifying the start and end angles, the curve will be drawn either clockwise
		or anti-clockwise according to whether the end angle is greater than the start. This means
		that sometimes you may need to use values greater than 2*Pi for the end angle.

		@param centreX      the centre x of the ellipse
		@param centreY      the centre y of the ellipse
		@param radiusX      the horizontal radius of the ellipse
		@param radiusY      the vertical radius of the ellipse
		@param rotationOfEllipse    an angle by which the whole ellipse should be rotated about its centre, in radians (clockwise)
		@param fromRadians  the angle (clockwise) in radians at which to start the arc segment (where 0 is the
							top-centre of the ellipse)
		@param toRadians    the angle (clockwise) in radians at which to end the arc segment (where 0 is the
							top-centre of the ellipse). This angle can be greater than 2*Pi, so for example to
							draw a curve clockwise from the 9 o'clock position to the 3 o'clock position via
							12 o'clock, you'd use 1.5*Pi and 2.5*Pi as the start and finish points.
		@param startAsNewSubPath    if true, the arc will begin a new subpath from its starting point; if false,
							it will be added to the current sub-path, continuing from the current postition

		@see addArc, arcTo
	*/
	void addCentredArc (float centreX, float centreY,
						float radiusX, float radiusY,
						float rotationOfEllipse,
						float fromRadians,
						float toRadians,
						bool startAsNewSubPath = false);

	/** Adds a "pie-chart" shape to the path.

		The shape is added as a new sub-path. (Any currently open paths will be
		left open).

		Note that when specifying the start and end angles, the curve will be drawn either clockwise
		or anti-clockwise according to whether the end angle is greater than the start. This means
		that sometimes you may need to use values greater than 2*Pi for the end angle.

		@param x            the left-hand edge of the rectangle in which the elliptical outline fits
		@param y            the top edge of the rectangle in which the elliptical outline fits
		@param width        the width of the rectangle in which the elliptical outline fits
		@param height       the height of the rectangle in which the elliptical outline fits
		@param fromRadians  the angle (clockwise) in radians at which to start the arc segment (where 0 is the
							top-centre of the ellipse)
		@param toRadians    the angle (clockwise) in radians at which to end the arc segment (where 0 is the
							top-centre of the ellipse)
		@param innerCircleProportionalSize  if this is > 0, then the pie will be drawn as a curved band around a hollow
							ellipse at its centre, where this value indicates the inner ellipse's size with
							respect to the outer one.

		@see addArc
	*/
	void addPieSegment (float x, float y,
						float width, float height,
						float fromRadians,
						float toRadians,
						float innerCircleProportionalSize);

	/** Adds a line with a specified thickness.

		The line is added as a new closed sub-path. (Any currently open paths will be
		left open).

		@see addArrow
	*/
	void addLineSegment (const Line<float>& line, float lineThickness);

	/** Adds a line with an arrowhead on the end.
		The arrow is added as a new closed sub-path. (Any currently open paths will be left open).
		@see PathStrokeType::createStrokeWithArrowheads
	*/
	void addArrow (const Line<float>& line,
				   float lineThickness,
				   float arrowheadWidth,
				   float arrowheadLength);

	/** Adds a polygon shape to the path.
		@see addStar
	*/
	void addPolygon (const Point<float>& centre,
					 int numberOfSides,
					 float radius,
					 float startAngle = 0.0f);

	/** Adds a star shape to the path.
		@see addPolygon
	*/
	void addStar (const Point<float>& centre,
				  int numberOfPoints,
				  float innerRadius,
				  float outerRadius,
				  float startAngle = 0.0f);

	/** Adds a speech-bubble shape to the path.

		@param bodyX            the left of the main body area of the bubble
		@param bodyY            the top of the main body area of the bubble
		@param bodyW            the width of the main body area of the bubble
		@param bodyH            the height of the main body area of the bubble
		@param cornerSize       the amount by which to round off the corners of the main body rectangle
		@param arrowTipX        the x position that the tip of the arrow should connect to
		@param arrowTipY        the y position that the tip of the arrow should connect to
		@param whichSide        the side to connect the arrow to: 0 = top, 1 = left, 2 = bottom, 3 = right
		@param arrowPositionAlongEdgeProportional   how far along the edge of the main rectangle the
								arrow's base should be - this is a proportional distance between 0 and 1.0
		@param arrowWidth       how wide the base of the arrow should be where it joins the main rectangle
	*/
	void addBubble (float bodyX, float bodyY,
					float bodyW, float bodyH,
					float cornerSize,
					float arrowTipX,
					float arrowTipY,
					int whichSide,
					float arrowPositionAlongEdgeProportional,
					float arrowWidth);

	/** Adds another path to this one.

		The new path is added as a new sub-path. (Any currently open paths in this
		path will be left open).

		@param pathToAppend     the path to add
	*/
	void addPath (const Path& pathToAppend);

	/** Adds another path to this one, transforming it on the way in.

		The new path is added as a new sub-path, its points being transformed by the given
		matrix before being added.

		@param pathToAppend     the path to add
		@param transformToApply an optional transform to apply to the incoming vertices
	*/
	void addPath (const Path& pathToAppend,
				  const AffineTransform& transformToApply);

	/** Swaps the contents of this path with another one.

		The internal data of the two paths is swapped over, so this is much faster than
		copying it to a temp variable and back.
	*/
	void swapWithPath (Path& other) noexcept;

	/** Applies a 2D transform to all the vertices in the path.

		@see AffineTransform, scaleToFit, getTransformToScaleToFit
	*/
	void applyTransform (const AffineTransform& transform) noexcept;

	/** Rescales this path to make it fit neatly into a given space.

		This is effectively a quick way of calling
		applyTransform (getTransformToScaleToFit (x, y, w, h, preserveProportions))

		@param x                    the x position of the rectangle to fit the path inside
		@param y                    the y position of the rectangle to fit the path inside
		@param width                the width of the rectangle to fit the path inside
		@param height               the height of the rectangle to fit the path inside
		@param preserveProportions  if true, it will fit the path into the space without altering its
									horizontal/vertical scale ratio; if false, it will distort the
									path to fill the specified ratio both horizontally and vertically

		@see applyTransform, getTransformToScaleToFit
	*/
	void scaleToFit (float x, float y, float width, float height,
					 bool preserveProportions) noexcept;

	/** Returns a transform that can be used to rescale the path to fit into a given space.

		@param x                    the x position of the rectangle to fit the path inside
		@param y                    the y position of the rectangle to fit the path inside
		@param width                the width of the rectangle to fit the path inside
		@param height               the height of the rectangle to fit the path inside
		@param preserveProportions  if true, it will fit the path into the space without altering its
									horizontal/vertical scale ratio; if false, it will distort the
									path to fill the specified ratio both horizontally and vertically
		@param justificationType    if the proportions are preseved, the resultant path may be smaller
									than the available rectangle, so this describes how it should be
									positioned within the space.
		@returns                    an appropriate transformation

		@see applyTransform, scaleToFit

	*/
	AffineTransform getTransformToScaleToFit (float x, float y, float width, float height,
											  bool preserveProportions,
											  const Justification& justificationType = Justification::centred) const;

	/** Creates a version of this path where all sharp corners have been replaced by curves.

		Wherever two lines meet at an angle, this will replace the corner with a curve
		of the given radius.
	*/
	Path createPathWithRoundedCorners (float cornerRadius) const;

	/** Changes the winding-rule to be used when filling the path.

		If set to true (which is the default), then the path uses a non-zero-winding rule
		to determine which points are inside the path. If set to false, it uses an
		alternate-winding rule.

		The winding-rule comes into play when areas of the shape overlap other
		areas, and determines whether the overlapping regions are considered to be
		inside or outside.

		Changing this value just sets a flag - it doesn't affect the contents of the
		path.

		@see isUsingNonZeroWinding
	*/
	void setUsingNonZeroWinding (bool isNonZeroWinding) noexcept;

	/** Returns the flag that indicates whether the path should use a non-zero winding rule.

		The default for a new path is true.

		@see setUsingNonZeroWinding
	*/
	bool isUsingNonZeroWinding() const                  { return useNonZeroWinding; }

	/** Iterates the lines and curves that a path contains.

		@see Path, PathFlatteningIterator
	*/
	class JUCE_API  Iterator
	{
	public:

		Iterator (const Path& path);
		~Iterator();

		/** Moves onto the next element in the path.

			If this returns false, there are no more elements. If it returns true,
			the elementType variable will be set to the type of the current element,
			and some of the x and y variables will be filled in with values.
		*/
		bool next();

		enum PathElementType
		{
			startNewSubPath,    /**< For this type, x1 and y1 will be set to indicate the first point in the subpath.  */
			lineTo,             /**< For this type, x1 and y1 indicate the end point of the line.  */
			quadraticTo,        /**< For this type, x1, y1, x2, y2 indicate the control point and endpoint of a quadratic curve. */
			cubicTo,            /**< For this type, x1, y1, x2, y2, x3, y3 indicate the two control points and the endpoint of a cubic curve. */
			closePath           /**< Indicates that the sub-path is being closed. None of the x or y values are valid in this case. */
		};

		PathElementType elementType;

		float x1, y1, x2, y2, x3, y3;

	private:
		const Path& path;
		size_t index;

		JUCE_DECLARE_NON_COPYABLE (Iterator);
	};

	/** Loads a stored path from a data stream.

		The data in the stream must have been written using writePathToStream().

		Note that this will append the stored path to whatever is currently in
		this path, so you might need to call clear() beforehand.

		@see loadPathFromData, writePathToStream
	*/
	void loadPathFromStream (InputStream& source);

	/** Loads a stored path from a block of data.

		This is similar to loadPathFromStream(), but just reads from a block
		of data. Useful if you're including stored shapes in your code as a
		block of static data.

		@see loadPathFromStream, writePathToStream
	*/
	void loadPathFromData (const void* data, size_t numberOfBytes);

	/** Stores the path by writing it out to a stream.

		After writing out a path, you can reload it using loadPathFromStream().

		@see loadPathFromStream, loadPathFromData
	*/
	void writePathToStream (OutputStream& destination) const;

	/** Creates a string containing a textual representation of this path.
		@see restoreFromString
	*/
	String toString() const;

	/** Restores this path from a string that was created with the toString() method.
		@see toString()
	*/
	void restoreFromString (const String& stringVersion);

private:

	friend class PathFlatteningIterator;
	friend class Path::Iterator;
	ArrayAllocationBase <float, DummyCriticalSection> data;
	size_t numElements;

	struct PathBounds
	{
		PathBounds() noexcept;
		Rectangle<float> getRectangle() const noexcept;
		void reset() noexcept;
		void reset (float, float) noexcept;
		void extend (float, float) noexcept;
		void extend (float, float, float, float) noexcept;

		float pathXMin, pathXMax, pathYMin, pathYMax;
	};

	PathBounds bounds;
	bool useNonZeroWinding;

	static const float lineMarker;
	static const float moveMarker;
	static const float quadMarker;
	static const float cubicMarker;
	static const float closeSubPathMarker;

	JUCE_LEAK_DETECTOR (Path);
};

#endif   // __JUCE_PATH_JUCEHEADER__

/*** End of inlined file: juce_Path.h ***/

/**
	Describes a type of stroke used to render a solid outline along a path.

	A PathStrokeType object can be used directly to create the shape of an outline
	around a path, and is used by Graphics::strokePath to specify the type of
	stroke to draw.

	@see Path, Graphics::strokePath
*/
class JUCE_API  PathStrokeType
{
public:

	/** The type of shape to use for the corners between two adjacent line segments. */
	enum JointStyle
	{
		mitered,    /**< Indicates that corners should be drawn with sharp joints.
						 Note that for angles that curve back on themselves, drawing a
						 mitre could require extending the point too far away from the
						 path, so a mitre limit is imposed and any corners that exceed it
						 are drawn as bevelled instead. */
		curved,     /**< Indicates that corners should be drawn as rounded-off. */
		beveled     /**< Indicates that corners should be drawn with a line flattening their
						 outside edge. */
	};

	/** The type shape to use for the ends of lines. */
	enum EndCapStyle
	{
		butt,       /**< Ends of lines are flat and don't extend beyond the end point. */
		square,     /**< Ends of lines are flat, but stick out beyond the end point for half
						 the thickness of the stroke. */
		rounded     /**< Ends of lines are rounded-off with a circular shape. */
	};

	/** Creates a stroke type.

		@param strokeThickness      the width of the line to use
		@param jointStyle           the type of joints to use for corners
		@param endStyle             the type of end-caps to use for the ends of open paths.
	*/
	PathStrokeType (float strokeThickness,
					JointStyle jointStyle = mitered,
					EndCapStyle endStyle = butt) noexcept;

	/** Createes a copy of another stroke type. */
	PathStrokeType (const PathStrokeType& other) noexcept;

	/** Copies another stroke onto this one. */
	PathStrokeType& operator= (const PathStrokeType& other) noexcept;

	/** Destructor. */
	~PathStrokeType() noexcept;

	/** Applies this stroke type to a path and returns the resultant stroke as another Path.

		@param destPath         the resultant stroked outline shape will be copied into this path.
								Note that it's ok for the source and destination Paths to be
								the same object, so you can easily turn a path into a stroked version
								of itself.
		@param sourcePath       the path to use as the source
		@param transform        an optional transform to apply to the points from the source path
								as they are being used
		@param extraAccuracy    if this is greater than 1.0, it will subdivide the path to
								a higher resolution, which improves the quality if you'll later want
								to enlarge the stroked path. So for example, if you're planning on drawing
								the stroke at 3x the size that you're creating it, you should set this to 3.

		@see createDashedStroke
	*/
	void createStrokedPath (Path& destPath,
							const Path& sourcePath,
							const AffineTransform& transform = AffineTransform::identity,
							float extraAccuracy = 1.0f) const;

	/** Applies this stroke type to a path, creating a dashed line.

		This is similar to createStrokedPath, but uses the array passed in to
		break the stroke up into a series of dashes.

		@param destPath         the resultant stroked outline shape will be copied into this path.
								Note that it's ok for the source and destination Paths to be
								the same object, so you can easily turn a path into a stroked version
								of itself.
		@param sourcePath       the path to use as the source
		@param dashLengths      An array of alternating on/off lengths. E.g. { 2, 3, 4, 5 } will create
								a line of length 2, then skip a length of 3, then add a line of length 4,
								skip 5, and keep repeating this pattern.
		@param numDashLengths   The number of lengths in the dashLengths array. This should really be
								an even number, otherwise the pattern will get out of step as it
								repeats.
		@param transform        an optional transform to apply to the points from the source path
								as they are being used
		@param extraAccuracy    if this is greater than 1.0, it will subdivide the path to
								a higher resolution, which improves the quality if you'll later want
								to enlarge the stroked path. So for example, if you're planning on drawing
								the stroke at 3x the size that you're creating it, you should set this to 3.
	*/
	void createDashedStroke (Path& destPath,
							 const Path& sourcePath,
							 const float* dashLengths,
							 int numDashLengths,
							 const AffineTransform& transform = AffineTransform::identity,
							 float extraAccuracy = 1.0f) const;

	/** Applies this stroke type to a path and returns the resultant stroke as another Path.

		@param destPath             the resultant stroked outline shape will be copied into this path.
									Note that it's ok for the source and destination Paths to be
									the same object, so you can easily turn a path into a stroked version
									of itself.
		@param sourcePath           the path to use as the source
		@param arrowheadStartWidth  the width of the arrowhead at the start of the path
		@param arrowheadStartLength the length of the arrowhead at the start of the path
		@param arrowheadEndWidth    the width of the arrowhead at the end of the path
		@param arrowheadEndLength   the length of the arrowhead at the end of the path
		@param transform            an optional transform to apply to the points from the source path
									as they are being used
		@param extraAccuracy        if this is greater than 1.0, it will subdivide the path to
									a higher resolution, which improves the quality if you'll later want
									to enlarge the stroked path. So for example, if you're planning on drawing
									the stroke at 3x the size that you're creating it, you should set this to 3.
		@see createDashedStroke
	*/
	void createStrokeWithArrowheads (Path& destPath,
									 const Path& sourcePath,
									 float arrowheadStartWidth, float arrowheadStartLength,
									 float arrowheadEndWidth, float arrowheadEndLength,
									 const AffineTransform& transform = AffineTransform::identity,
									 float extraAccuracy = 1.0f) const;

	/** Returns the stroke thickness. */
	float getStrokeThickness() const noexcept                   { return thickness; }

	/** Sets the stroke thickness. */
	void setStrokeThickness (float newThickness) noexcept       { thickness = newThickness; }

	/** Returns the joint style. */
	JointStyle getJointStyle() const noexcept                   { return jointStyle; }

	/** Sets the joint style. */
	void setJointStyle (JointStyle newStyle) noexcept           { jointStyle = newStyle; }

	/** Returns the end-cap style. */
	EndCapStyle getEndStyle() const noexcept                    { return endStyle; }

	/** Sets the end-cap style. */
	void setEndStyle (EndCapStyle newStyle) noexcept            { endStyle = newStyle; }

	/** Compares the stroke thickness, joint and end styles of two stroke types. */
	bool operator== (const PathStrokeType& other) const noexcept;

	/** Compares the stroke thickness, joint and end styles of two stroke types. */
	bool operator!= (const PathStrokeType& other) const noexcept;

private:

	float thickness;
	JointStyle jointStyle;
	EndCapStyle endStyle;

	JUCE_LEAK_DETECTOR (PathStrokeType);
};

#endif   // __JUCE_PATHSTROKETYPE_JUCEHEADER__

/*** End of inlined file: juce_PathStrokeType.h ***/


/*** Start of inlined file: juce_RectanglePlacement.h ***/
#ifndef __JUCE_RECTANGLEPLACEMENT_JUCEHEADER__
#define __JUCE_RECTANGLEPLACEMENT_JUCEHEADER__

/**
	Defines the method used to postion some kind of rectangular object within
	a rectangular viewport.

	Although similar to Justification, this is more specific, and has some extra
	options.
*/
class JUCE_API  RectanglePlacement
{
public:

	/** Creates a RectanglePlacement object using a combination of flags. */
	inline RectanglePlacement (int flags_) noexcept : flags (flags_) {}

	/** Creates a copy of another RectanglePlacement object. */
	RectanglePlacement (const RectanglePlacement& other) noexcept;

	/** Copies another RectanglePlacement object. */
	RectanglePlacement& operator= (const RectanglePlacement& other) noexcept;

	bool operator== (const RectanglePlacement& other) const noexcept;
	bool operator!= (const RectanglePlacement& other) const noexcept;

	/** Flag values that can be combined and used in the constructor. */
	enum
	{

		/** Indicates that the source rectangle's left edge should be aligned with the left edge of the target rectangle. */
		xLeft                                   = 1,

		/** Indicates that the source rectangle's right edge should be aligned with the right edge of the target rectangle. */
		xRight                                  = 2,

		/** Indicates that the source should be placed in the centre between the left and right
			sides of the available space. */
		xMid                                    = 4,

		/** Indicates that the source's top edge should be aligned with the top edge of the
			destination rectangle. */
		yTop                                    = 8,

		/** Indicates that the source's bottom edge should be aligned with the bottom edge of the
			destination rectangle. */
		yBottom                                 = 16,

		/** Indicates that the source should be placed in the centre between the top and bottom
			sides of the available space. */
		yMid                                    = 32,

		/** If this flag is set, then the source rectangle will be resized to completely fill
			the destination rectangle, and all other flags are ignored.
		*/
		stretchToFit                            = 64,

		/** If this flag is set, then the source rectangle will be resized so that it is the
			minimum size to completely fill the destination rectangle, without changing its
			aspect ratio. This means that some of the source rectangle may fall outside
			the destination.

			If this flag is not set, the source will be given the maximum size at which none
			of it falls outside the destination rectangle.
		*/
		fillDestination                         = 128,

		/** Indicates that the source rectangle can be reduced in size if required, but should
			never be made larger than its original size.
		*/
		onlyReduceInSize                        = 256,

		/** Indicates that the source rectangle can be enlarged if required, but should
			never be made smaller than its original size.
		*/
		onlyIncreaseInSize                      = 512,

		/** Indicates that the source rectangle's size should be left unchanged.
		*/
		doNotResize                             = (onlyIncreaseInSize | onlyReduceInSize),

		/** A shorthand value that is equivalent to (xMid | yMid). */
		centred                                 = 4 + 32
	};

	/** Returns the raw flags that are set for this object. */
	inline int getFlags() const noexcept                            { return flags; }

	/** Tests a set of flags for this object.

		@returns true if any of the flags passed in are set on this object.
	*/
	inline bool testFlags (int flagsToTest) const noexcept          { return (flags & flagsToTest) != 0; }

	/** Adjusts the position and size of a rectangle to fit it into a space.

		The source rectangle co-ordinates will be adjusted so that they fit into
		the destination rectangle based on this object's flags.
	*/
	void applyTo (double& sourceX,
				  double& sourceY,
				  double& sourceW,
				  double& sourceH,
				  double destinationX,
				  double destinationY,
				  double destinationW,
				  double destinationH) const noexcept;

	/** Returns the transform that should be applied to these source co-ordinates to fit them
		into the destination rectangle using the current flags.
	*/
	template <typename ValueType>
	const Rectangle<ValueType> appliedTo (const Rectangle<ValueType>& source,
										  const Rectangle<ValueType>& destination) const noexcept
	{
		double x = source.getX(), y = source.getY(), w = source.getWidth(), h = source.getHeight();
		applyTo (x, y, w, h, static_cast <double> (destination.getX()), static_cast <double> (destination.getY()),
				 static_cast <double> (destination.getWidth()), static_cast <double> (destination.getHeight()));
		return Rectangle<ValueType> (static_cast <ValueType> (x), static_cast <ValueType> (y),
									 static_cast <ValueType> (w), static_cast <ValueType> (h));
	}

	/** Returns the transform that should be applied to these source co-ordinates to fit them
		into the destination rectangle using the current flags.
	*/
	const AffineTransform getTransformToFit (const Rectangle<float>& source,
											 const Rectangle<float>& destination) const noexcept;

private:

	int flags;
};

#endif   // __JUCE_RECTANGLEPLACEMENT_JUCEHEADER__

/*** End of inlined file: juce_RectanglePlacement.h ***/

class LowLevelGraphicsContext;
class Image;
class FillType;
class RectangleList;

/**
	A graphics context, used for drawing a component or image.

	When a Component needs painting, a Graphics context is passed to its
	Component::paint() method, and this you then call methods within this
	object to actually draw the component's content.

	A Graphics can also be created from an image, to allow drawing directly onto
	that image.

	@see Component::paint
*/
class JUCE_API  Graphics
{
public:

	/** Creates a Graphics object to draw directly onto the given image.

		The graphics object that is created will be set up to draw onto the image,
		with the context's clipping area being the entire size of the image, and its
		origin being the image's origin. To draw into a subsection of an image, use the
		reduceClipRegion() and setOrigin() methods.

		Obviously you shouldn't delete the image before this context is deleted.
	*/
	explicit Graphics (const Image& imageToDrawOnto);

	/** Destructor. */
	~Graphics();

	/** Changes the current drawing colour.

		This sets the colour that will now be used for drawing operations - it also
		sets the opacity to that of the colour passed-in.

		If a brush is being used when this method is called, the brush will be deselected,
		and any subsequent drawing will be done with a solid colour brush instead.

		@see setOpacity
	*/
	void setColour (const Colour& newColour);

	/** Changes the opacity to use with the current colour.

		If a solid colour is being used for drawing, this changes its opacity
		to this new value (i.e. it doesn't multiply the colour's opacity by this amount).

		If a gradient is being used, this will have no effect on it.

		A value of 0.0 is completely transparent, 1.0 is completely opaque.
	*/
	void setOpacity (float newOpacity);

	/** Sets the context to use a gradient for its fill pattern.
	*/
	void setGradientFill (const ColourGradient& gradient);

	/** Sets the context to use a tiled image pattern for filling.
		Make sure that you don't delete this image while it's still being used by
		this context!
	*/
	void setTiledImageFill (const Image& imageToUse,
							int anchorX, int anchorY,
							float opacity);

	/** Changes the current fill settings.
		@see setColour, setGradientFill, setTiledImageFill
	*/
	void setFillType (const FillType& newFill);

	/** Changes the font to use for subsequent text-drawing functions.

		Note there's also a setFont (float, int) method to quickly change the size and
		style of the current font.

		@see drawSingleLineText, drawMultiLineText, drawTextAsPath, drawText, drawFittedText
	*/
	void setFont (const Font& newFont);

	/** Changes the size and style of the currently-selected font.

		This is a convenient shortcut that changes the context's current font to a
		different size or style. The typeface won't be changed.

		@see Font
	*/
	void setFont (float newFontHeight, int fontStyleFlags = Font::plain);

	/** Returns the currently selected font. */
	Font getCurrentFont() const;

	/** Draws a one-line text string.

		This will use the current colour (or brush) to fill the text. The font is the last
		one specified by setFont().

		@param text          the string to draw
		@param startX        the position to draw the left-hand edge of the text
		@param baselineY     the position of the text's baseline
		@param justification the horizontal flags indicate which end of the text string is
							 anchored at the specified point.
		@see drawMultiLineText, drawText, drawFittedText, GlyphArrangement::addLineOfText
	*/
	void drawSingleLineText (const String& text,
							 int startX, int baselineY,
							 const Justification& justification = Justification::left) const;

	/** Draws text across multiple lines.

		This will break the text onto a new line where there's a new-line or
		carriage-return character, or at a word-boundary when the text becomes wider
		than the size specified by the maximumLineWidth parameter.

		@see setFont, drawSingleLineText, drawFittedText, GlyphArrangement::addJustifiedText
	*/
	void drawMultiLineText (const String& text,
							int startX, int baselineY,
							int maximumLineWidth) const;

	/** Renders a string of text as a vector path.

		This allows a string to be transformed with an arbitrary AffineTransform and
		rendered using the current colour/brush. It's much slower than the normal text methods
		but more accurate.

		@see setFont
	*/
	void drawTextAsPath (const String& text,
						 const AffineTransform& transform) const;

	/** Draws a line of text within a specified rectangle.

		The text will be positioned within the rectangle based on the justification
		flags passed-in. If the string is too long to fit inside the rectangle, it will
		either be truncated or will have ellipsis added to its end (if the useEllipsesIfTooBig
		flag is true).

		@see drawSingleLineText, drawFittedText, drawMultiLineText, GlyphArrangement::addJustifiedText
	*/
	void drawText (const String& text,
				   int x, int y, int width, int height,
				   const Justification& justificationType,
				   bool useEllipsesIfTooBig) const;

	/** Tries to draw a text string inside a given space.

		This does its best to make the given text readable within the specified rectangle,
		so it useful for labelling things.

		If the text is too big, it'll be squashed horizontally or broken over multiple lines
		if the maximumLinesToUse value allows this. If the text just won't fit into the space,
		it'll cram as much as possible in there, and put some ellipsis at the end to show that
		it's been truncated.

		A Justification parameter lets you specify how the text is laid out within the rectangle,
		both horizontally and vertically.

		The minimumHorizontalScale parameter specifies how much the text can be squashed horizontally
		to try to squeeze it into the space. If you don't want any horizontal scaling to occur, you
		can set this value to 1.0f.

		@see GlyphArrangement::addFittedText
	*/
	void drawFittedText (const String& text,
						 int x, int y, int width, int height,
						 const Justification& justificationFlags,
						 int maximumNumberOfLines,
						 float minimumHorizontalScale = 0.7f) const;

	/** Fills the context's entire clip region with the current colour or brush.

		(See also the fillAll (const Colour&) method which is a quick way of filling
		it with a given colour).
	*/
	void fillAll() const;

	/** Fills the context's entire clip region with a given colour.

		This leaves the context's current colour and brush unchanged, it just
		uses the specified colour temporarily.
	*/
	void fillAll (const Colour& colourToUse) const;

	/** Fills a rectangle with the current colour or brush.

		@see drawRect, fillRoundedRectangle
	*/
	void fillRect (int x, int y, int width, int height) const;

	/** Fills a rectangle with the current colour or brush. */
	void fillRect (const Rectangle<int>& rectangle) const;

	/** Fills a rectangle with the current colour or brush.

		This uses sub-pixel positioning so is slower than the fillRect method which
		takes integer co-ordinates.
	*/
	void fillRect (float x, float y, float width, float height) const;

	/** Uses the current colour or brush to fill a rectangle with rounded corners.

		@see drawRoundedRectangle, Path::addRoundedRectangle
	*/
	void fillRoundedRectangle (float x, float y, float width, float height,
							   float cornerSize) const;

	/** Uses the current colour or brush to fill a rectangle with rounded corners.

		@see drawRoundedRectangle, Path::addRoundedRectangle
	*/
	void fillRoundedRectangle (const Rectangle<float>& rectangle,
							   float cornerSize) const;

	/** Fills a rectangle with a checkerboard pattern, alternating between two colours.
	*/
	void fillCheckerBoard (const Rectangle<int>& area,
						   int checkWidth, int checkHeight,
						   const Colour& colour1, const Colour& colour2) const;

	/** Draws four lines to form a rectangular outline, using the current colour or brush.

		The lines are drawn inside the given rectangle, and greater line thicknesses
		extend inwards.

		@see fillRect
	*/
	void drawRect (int x, int y, int width, int height,
				   int lineThickness = 1) const;

	/** Draws four lines to form a rectangular outline, using the current colour or brush.

		The lines are drawn inside the given rectangle, and greater line thicknesses
		extend inwards.

		@see fillRect
	*/
	void drawRect (float x, float y, float width, float height,
				   float lineThickness = 1.0f) const;

	/** Draws four lines to form a rectangular outline, using the current colour or brush.

		The lines are drawn inside the given rectangle, and greater line thicknesses
		extend inwards.

		@see fillRect
	*/
	void drawRect (const Rectangle<int>& rectangle,
				   int lineThickness = 1) const;

	/** Uses the current colour or brush to draw the outline of a rectangle with rounded corners.

		@see fillRoundedRectangle, Path::addRoundedRectangle
	*/
	void drawRoundedRectangle (float x, float y, float width, float height,
							   float cornerSize, float lineThickness) const;

	/** Uses the current colour or brush to draw the outline of a rectangle with rounded corners.

		@see fillRoundedRectangle, Path::addRoundedRectangle
	*/
	void drawRoundedRectangle (const Rectangle<float>& rectangle,
							   float cornerSize, float lineThickness) const;

	/** Draws a 3D raised (or indented) bevel using two colours.

		The bevel is drawn inside the given rectangle, and greater bevel thicknesses
		extend inwards.

		The top-left colour is used for the top- and left-hand edges of the
		bevel; the bottom-right colour is used for the bottom- and right-hand
		edges.

		If useGradient is true, then the bevel fades out to make it look more curved
		and less angular. If sharpEdgeOnOutside is true, the outside of the bevel is
		sharp, and it fades towards the centre; if sharpEdgeOnOutside is false, then
		the centre edges are sharp and it fades towards the outside.
	*/
	void drawBevel (int x, int y, int width, int height,
					int bevelThickness,
					const Colour& topLeftColour = Colours::white,
					const Colour& bottomRightColour = Colours::black,
					bool useGradient = true,
					bool sharpEdgeOnOutside = true) const;

	/** Draws a pixel using the current colour or brush.
	*/
	void setPixel (int x, int y) const;

	/** Fills an ellipse with the current colour or brush.

		The ellipse is drawn to fit inside the given rectangle.

		@see drawEllipse, Path::addEllipse
	*/
	void fillEllipse (float x, float y, float width, float height) const;

	/** Draws an elliptical stroke using the current colour or brush.

		@see fillEllipse, Path::addEllipse
	*/
	void drawEllipse (float x, float y, float width, float height,
					  float lineThickness) const;

	/** Draws a line between two points.

		The line is 1 pixel wide and drawn with the current colour or brush.
	*/
	void drawLine (float startX, float startY, float endX, float endY) const;

	/** Draws a line between two points with a given thickness.

		@see Path::addLineSegment
	*/
	void drawLine (float startX, float startY, float endX, float endY,
				   float lineThickness) const;

	/** Draws a line between two points.

		The line is 1 pixel wide and drawn with the current colour or brush.
	*/
	void drawLine (const Line<float>& line) const;

	/** Draws a line between two points with a given thickness.

		@see Path::addLineSegment
	*/
	void drawLine (const Line<float>& line, float lineThickness) const;

	/** Draws a dashed line using a custom set of dash-lengths.

		@param line             the line to draw
		@param dashLengths      a series of lengths to specify the on/off lengths - e.g.
								{ 4, 5, 6, 7 } will draw a line of 4 pixels, skip 5 pixels,
								draw 6 pixels, skip 7 pixels, and then repeat.
		@param numDashLengths   the number of elements in the array (this must be an even number).
		@param lineThickness    the thickness of the line to draw
		@param dashIndexToStartFrom     the index in the dash-length array to use for the first segment
		@see PathStrokeType::createDashedStroke
	*/
	void drawDashedLine (const Line<float>& line,
						 const float* dashLengths, int numDashLengths,
						 float lineThickness = 1.0f,
						 int dashIndexToStartFrom = 0) const;

	/** Draws a vertical line of pixels at a given x position.

		The x position is an integer, but the top and bottom of the line can be sub-pixel
		positions, and these will be anti-aliased if necessary.

		The bottom parameter must be greater than or equal to the top parameter.
	*/
	void drawVerticalLine (int x, float top, float bottom) const;

	/** Draws a horizontal line of pixels at a given y position.

		The y position is an integer, but the left and right ends of the line can be sub-pixel
		positions, and these will be anti-aliased if necessary.

		The right parameter must be greater than or equal to the left parameter.
	*/
	void drawHorizontalLine (int y, float left, float right) const;

	/** Fills a path using the currently selected colour or brush.
	*/
	void fillPath (const Path& path,
				   const AffineTransform& transform = AffineTransform::identity) const;

	/** Draws a path's outline using the currently selected colour or brush.
	*/
	void strokePath (const Path& path,
					 const PathStrokeType& strokeType,
					 const AffineTransform& transform = AffineTransform::identity) const;

	/** Draws a line with an arrowhead at its end.

		@param line             the line to draw
		@param lineThickness    the thickness of the line
		@param arrowheadWidth   the width of the arrow head (perpendicular to the line)
		@param arrowheadLength  the length of the arrow head (along the length of the line)
	*/
	void drawArrow (const Line<float>& line,
					float lineThickness,
					float arrowheadWidth,
					float arrowheadLength) const;

	/** Types of rendering quality that can be specified when drawing images.

		@see blendImage, Graphics::setImageResamplingQuality
	*/
	enum ResamplingQuality
	{
		lowResamplingQuality     = 0,    /**< Just uses a nearest-neighbour algorithm for resampling. */
		mediumResamplingQuality  = 1,    /**< Uses bilinear interpolation for upsampling and area-averaging for downsampling. */
		highResamplingQuality    = 2     /**< Uses bicubic interpolation for upsampling and area-averaging for downsampling. */
	};

	/** Changes the quality that will be used when resampling images.

		By default a Graphics object will be set to mediumRenderingQuality.

		@see Graphics::drawImage, Graphics::drawImageTransformed, Graphics::drawImageWithin
	*/
	void setImageResamplingQuality (const ResamplingQuality newQuality);

	/** Draws an image.

		This will draw the whole of an image, positioning its top-left corner at the
		given co-ordinates, and keeping its size the same. This is the simplest image
		drawing method - the others give more control over the scaling and clipping
		of the images.

		Images are composited using the context's current opacity, so if you
		don't want it to be drawn semi-transparently, be sure to call setOpacity (1.0f)
		(or setColour() with an opaque colour) before drawing images.
	*/
	void drawImageAt (const Image& imageToDraw, int topLeftX, int topLeftY,
					  bool fillAlphaChannelWithCurrentBrush = false) const;

	/** Draws part of an image, rescaling it to fit in a given target region.

		The specified area of the source image is rescaled and drawn to fill the
		specifed destination rectangle.

		Images are composited using the context's current opacity, so if you
		don't want it to be drawn semi-transparently, be sure to call setOpacity (1.0f)
		(or setColour() with an opaque colour) before drawing images.

		@param imageToDraw      the image to overlay
		@param destX            the left of the destination rectangle
		@param destY            the top of the destination rectangle
		@param destWidth        the width of the destination rectangle
		@param destHeight       the height of the destination rectangle
		@param sourceX          the left of the rectangle to copy from the source image
		@param sourceY          the top of the rectangle to copy from the source image
		@param sourceWidth      the width of the rectangle to copy from the source image
		@param sourceHeight     the height of the rectangle to copy from the source image
		@param fillAlphaChannelWithCurrentBrush     if true, then instead of drawing the source image's pixels,
													the source image's alpha channel is used as a mask with
													which to fill the destination using the current colour
													or brush. (If the source is has no alpha channel, then
													it will just fill the target with a solid rectangle)
		@see setImageResamplingQuality, drawImageAt, drawImageWithin, fillAlphaMap
	*/
	void drawImage (const Image& imageToDraw,
					int destX, int destY, int destWidth, int destHeight,
					int sourceX, int sourceY, int sourceWidth, int sourceHeight,
					bool fillAlphaChannelWithCurrentBrush = false) const;

	/** Draws an image, having applied an affine transform to it.

		This lets you throw the image around in some wacky ways, rotate it, shear,
		scale it, etc.

		Images are composited using the context's current opacity, so if you
		don't want it to be drawn semi-transparently, be sure to call setOpacity (1.0f)
		(or setColour() with an opaque colour) before drawing images.

		If fillAlphaChannelWithCurrentBrush is set to true, then the image's RGB channels
		are ignored and it is filled with the current brush, masked by its alpha channel.

		If you want to render only a subsection of an image, use Image::getClippedImage() to
		create the section that you need.

		@see setImageResamplingQuality, drawImage
	*/
	void drawImageTransformed (const Image& imageToDraw,
							   const AffineTransform& transform,
							   bool fillAlphaChannelWithCurrentBrush = false) const;

	/** Draws an image to fit within a designated rectangle.

		If the image is too big or too small for the space, it will be rescaled
		to fit as nicely as it can do without affecting its aspect ratio. It will
		then be placed within the target rectangle according to the justification flags
		specified.

		@param imageToDraw              the source image to draw
		@param destX                    top-left of the target rectangle to fit it into
		@param destY                    top-left of the target rectangle to fit it into
		@param destWidth                size of the target rectangle to fit the image into
		@param destHeight               size of the target rectangle to fit the image into
		@param placementWithinTarget    this specifies how the image should be positioned
										within the target rectangle - see the RectanglePlacement
										class for more details about this.
		@param fillAlphaChannelWithCurrentBrush     if true, then instead of drawing the image, just its
													alpha channel will be used as a mask with which to
													draw with the current brush or colour. This is
													similar to fillAlphaMap(), and see also drawImage()
		@see setImageResamplingQuality, drawImage, drawImageTransformed, drawImageAt, RectanglePlacement
	*/
	void drawImageWithin (const Image& imageToDraw,
						  int destX, int destY, int destWidth, int destHeight,
						  const RectanglePlacement& placementWithinTarget,
						  bool fillAlphaChannelWithCurrentBrush = false) const;

	/** Returns the position of the bounding box for the current clipping region.

		@see getClipRegion, clipRegionIntersects
	*/
	Rectangle<int> getClipBounds() const;

	/** Checks whether a rectangle overlaps the context's clipping region.

		If this returns false, no part of the given area can be drawn onto, so this
		method can be used to optimise a component's paint() method, by letting it
		avoid drawing complex objects that aren't within the region being repainted.
	*/
	bool clipRegionIntersects (const Rectangle<int>& area) const;

	/** Intersects the current clipping region with another region.

		@returns true if the resulting clipping region is non-zero in size
		@see setOrigin, clipRegionIntersects
	*/
	bool reduceClipRegion (int x, int y, int width, int height);

	/** Intersects the current clipping region with another region.

		@returns true if the resulting clipping region is non-zero in size
		@see setOrigin, clipRegionIntersects
	*/
	bool reduceClipRegion (const Rectangle<int>& area);

	/** Intersects the current clipping region with a rectangle list region.

		@returns true if the resulting clipping region is non-zero in size
		@see setOrigin, clipRegionIntersects
	*/
	bool reduceClipRegion (const RectangleList& clipRegion);

	/** Intersects the current clipping region with a path.

		@returns true if the resulting clipping region is non-zero in size
		@see reduceClipRegion
	*/
	bool reduceClipRegion (const Path& path, const AffineTransform& transform = AffineTransform::identity);

	/** Intersects the current clipping region with an image's alpha-channel.

		The current clipping path is intersected with the area covered by this image's
		alpha-channel, after the image has been transformed by the specified matrix.

		@param image    the image whose alpha-channel should be used. If the image doesn't
						have an alpha-channel, it is treated as entirely opaque.
		@param transform    a matrix to apply to the image
		@returns true if the resulting clipping region is non-zero in size
		@see reduceClipRegion
	*/
	bool reduceClipRegion (const Image& image, const AffineTransform& transform);

	/** Excludes a rectangle to stop it being drawn into. */
	void excludeClipRegion (const Rectangle<int>& rectangleToExclude);

	/** Returns true if no drawing can be done because the clip region is zero. */
	bool isClipEmpty() const;

	/** Saves the current graphics state on an internal stack.
		To restore the state, use restoreState().
		@see ScopedSaveState
	*/
	void saveState();

	/** Restores a graphics state that was previously saved with saveState().
		@see ScopedSaveState
	*/
	void restoreState();

	/** Uses RAII to save and restore the state of a graphics context.
		On construction, this calls Graphics::saveState(), and on destruction it calls
		Graphics::restoreState() on the Graphics object that you supply.
	*/
	class ScopedSaveState
	{
	public:
		ScopedSaveState (Graphics& g);
		~ScopedSaveState();

	private:
		Graphics& context;

		JUCE_DECLARE_NON_COPYABLE (ScopedSaveState);
	};

	/** Begins rendering to an off-screen bitmap which will later be flattened onto the current
		context with the given opacity.

		The context uses an internal stack of temporary image layers to do this. When you've
		finished drawing to the layer, call endTransparencyLayer() to complete the operation and
		composite the finished layer. Every call to beginTransparencyLayer() MUST be matched
		by a corresponding call to endTransparencyLayer()!

		This call also saves the current state, and endTransparencyLayer() restores it.
	*/
	void beginTransparencyLayer (float layerOpacity);

	/** Completes a drawing operation to a temporary semi-transparent buffer.
		See beginTransparencyLayer() for more details.
	*/
	void endTransparencyLayer();

	/** Moves the position of the context's origin.

		This changes the position that the context considers to be (0, 0) to
		the specified position.

		So if you call setOrigin (100, 100), then the position that was previously
		referred to as (100, 100) will subsequently be considered to be (0, 0).

		@see reduceClipRegion, addTransform
	*/
	void setOrigin (int newOriginX, int newOriginY);

	/** Adds a transformation which will be performed on all the graphics operations that
		the context subsequently performs.

		After calling this, all the coordinates that are passed into the context will be
		transformed by this matrix.

		@see setOrigin
	*/
	void addTransform (const AffineTransform& transform);

	/** Resets the current colour, brush, and font to default settings. */
	void resetToDefaultState();

	/** Returns true if this context is drawing to a vector-based device, such as a printer. */
	bool isVectorDevice() const;

	/** Create a graphics that uses a given low-level renderer.
		For internal use only.
		NB. The context will NOT be deleted by this object when it is deleted.
	*/
	Graphics (LowLevelGraphicsContext* internalContext) noexcept;

	/** @internal */
	LowLevelGraphicsContext* getInternalContext() const noexcept    { return context; }

private:

	LowLevelGraphicsContext* const context;
	ScopedPointer <LowLevelGraphicsContext> contextToDelete;

	bool saveStatePending;
	void saveStateIfPending();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Graphics);
};

#endif   // __JUCE_GRAPHICSCONTEXT_JUCEHEADER__

/*** End of inlined file: juce_GraphicsContext.h ***/

class ImageType;
class ImagePixelData;

/**
	Holds a fixed-size bitmap.

	The image is stored in either 24-bit RGB or 32-bit premultiplied-ARGB format.

	To draw into an image, create a Graphics object for it.
	e.g. @code

	// create a transparent 500x500 image..
	Image myImage (Image::RGB, 500, 500, true);

	Graphics g (myImage);
	g.setColour (Colours::red);
	g.fillEllipse (20, 20, 300, 200);  // draws a red ellipse in our image.
	@endcode

	Other useful ways to create an image are with the ImageCache class, or the
	ImageFileFormat, which provides a way to load common image files.

	@see Graphics, ImageFileFormat, ImageCache, ImageConvolutionKernel
*/
class JUCE_API  Image
{
public:

	/**
	*/
	enum PixelFormat
	{
		UnknownFormat,
		RGB,                /**<< each pixel is a 3-byte packed RGB colour value. For byte order, see the PixelRGB class. */
		ARGB,               /**<< each pixel is a 4-byte ARGB premultiplied colour value. For byte order, see the PixelARGB class. */
		SingleChannel       /**<< each pixel is a 1-byte alpha channel value. */
	};

	/** Creates a null image. */
	Image();

	/** Creates an image with a specified size and format.

		The image's internal type will be of the NativeImageType class - to specify a
		different type, use the other constructor, which takes an ImageType to use.

		@param format           the number of colour channels in the image
		@param imageWidth       the desired width of the image, in pixels - this value must be
								greater than zero (otherwise a width of 1 will be used)
		@param imageHeight      the desired width of the image, in pixels - this value must be
								greater than zero (otherwise a height of 1 will be used)
		@param clearImage       if true, the image will initially be cleared to black (if it's RGB)
								or transparent black (if it's ARGB). If false, the image may contain
								junk initially, so you need to make sure you overwrite it thoroughly.
	*/
	Image (PixelFormat format, int imageWidth, int imageHeight, bool clearImage);

	/** Creates an image with a specified size and format.

		@param format           the number of colour channels in the image
		@param imageWidth       the desired width of the image, in pixels - this value must be
								greater than zero (otherwise a width of 1 will be used)
		@param imageHeight      the desired width of the image, in pixels - this value must be
								greater than zero (otherwise a height of 1 will be used)
		@param clearImage       if true, the image will initially be cleared to black (if it's RGB)
								or transparent black (if it's ARGB). If false, the image may contain
								junk initially, so you need to make sure you overwrite it thoroughly.
		@param type             the type of image - this lets you specify the internal format that will
								be used to allocate and manage the image data.
	*/
	Image (PixelFormat format, int imageWidth, int imageHeight, bool clearImage, const ImageType& type);

	/** Creates a shared reference to another image.

		This won't create a duplicate of the image - when Image objects are copied, they simply
		point to the same shared image data. To make sure that an Image object has its own unique,
		unshared internal data, call duplicateIfShared().
	*/
	Image (const Image& other);

	/** Makes this image refer to the same underlying image as another object.

		This won't create a duplicate of the image - when Image objects are copied, they simply
		point to the same shared image data. To make sure that an Image object has its own unique,
		unshared internal data, call duplicateIfShared().
	*/
	Image& operator= (const Image&);

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	Image (Image&& other) noexcept;
	Image& operator= (Image&&) noexcept;
   #endif

	/** Destructor. */
	~Image();

	/** Returns true if the two images are referring to the same internal, shared image. */
	bool operator== (const Image& other) const noexcept     { return image == other.image; }

	/** Returns true if the two images are not referring to the same internal, shared image. */
	bool operator!= (const Image& other) const noexcept     { return image != other.image; }

	/** Returns true if this image isn't null.
		If you create an Image with the default constructor, it has no size or content, and is null
		until you reassign it to an Image which contains some actual data.
		The isNull() method is the opposite of isValid().
		@see isNull
	*/
	inline bool isValid() const noexcept                    { return image != nullptr; }

	/** Returns true if this image is not valid.
		If you create an Image with the default constructor, it has no size or content, and is null
		until you reassign it to an Image which contains some actual data.
		The isNull() method is the opposite of isValid().
		@see isValid
	*/
	inline bool isNull() const noexcept                     { return image == nullptr; }

	/** A null Image object that can be used when you need to return an invalid image.
		This object is the equivalient to an Image created with the default constructor.
	*/
	static const Image null;

	/** Returns the image's width (in pixels). */
	int getWidth() const noexcept;

	/** Returns the image's height (in pixels). */
	int getHeight() const noexcept;

	/** Returns a rectangle with the same size as this image.
		The rectangle's origin is always (0, 0).
	*/
	Rectangle<int> getBounds() const noexcept;

	/** Returns the image's pixel format. */
	PixelFormat getFormat() const noexcept;

	/** True if the image's format is ARGB. */
	bool isARGB() const noexcept;

	/** True if the image's format is RGB. */
	bool isRGB() const noexcept;

	/** True if the image's format is a single-channel alpha map. */
	bool isSingleChannel() const noexcept;

	/** True if the image contains an alpha-channel. */
	bool hasAlphaChannel() const noexcept;

	/** Clears a section of the image with a given colour.

		This won't do any alpha-blending - it just sets all pixels in the image to
		the given colour (which may be non-opaque if the image has an alpha channel).
	*/
	void clear (const Rectangle<int>& area, const Colour& colourToClearTo = Colour (0x00000000));

	/** Returns a rescaled version of this image.

		A new image is returned which is a copy of this one, rescaled to the given size.

		Note that if the new size is identical to the existing image, this will just return
		a reference to the original image, and won't actually create a duplicate.
	*/
	Image rescaled (int newWidth, int newHeight,
					Graphics::ResamplingQuality quality = Graphics::mediumResamplingQuality) const;

	/** Creates a copy of this image.
		Note that it's usually more efficient to use duplicateIfShared(), because it may not be necessary
		to copy an image if nothing else is using it.
		@see getReferenceCount
	*/
	Image createCopy() const;

	/** Returns a version of this image with a different image format.

		A new image is returned which has been converted to the specified format.

		Note that if the new format is no different to the current one, this will just return
		a reference to the original image, and won't actually create a copy.
	*/
	Image convertedToFormat (PixelFormat newFormat) const;

	/** Makes sure that no other Image objects share the same underlying data as this one.

		If no other Image objects refer to the same shared data as this one, this method has no
		effect. But if there are other references to the data, this will create a new copy of
		the data internally.

		Call this if you want to draw onto the image, but want to make sure that this doesn't
		affect any other code that may be sharing the same data.

		@see getReferenceCount
	*/
	void duplicateIfShared();

	/** Returns an image which refers to a subsection of this image.

		This will not make a copy of the original - the new image will keep a reference to it, so that
		if the original image is changed, the contents of the subsection will also change. Likewise if you
		draw into the subimage, you'll also be drawing onto that area of the original image. Note that if
		you use operator= to make the original Image object refer to something else, the subsection image
		won't pick up this change, it'll remain pointing at the original.

		The area passed-in will be clipped to the bounds of this image, so this may return a smaller
		image than the area you asked for, or even a null image if the area was out-of-bounds.
	*/
	Image getClippedImage (const Rectangle<int>& area) const;

	/** Returns the colour of one of the pixels in the image.

		If the co-ordinates given are beyond the image's boundaries, this will
		return Colours::transparentBlack.

		@see setPixelAt, Image::BitmapData::getPixelColour
	*/
	Colour getPixelAt (int x, int y) const;

	/** Sets the colour of one of the image's pixels.

		If the co-ordinates are beyond the image's boundaries, then nothing will happen.

		Note that this won't do any alpha-blending, it'll just replace the existing pixel
		with the given one. The colour's opacity will be ignored if this image doesn't have
		an alpha-channel.

		@see getPixelAt, Image::BitmapData::setPixelColour
	*/
	void setPixelAt (int x, int y, const Colour& colour);

	/** Changes the opacity of a pixel.

		This only has an effect if the image has an alpha channel and if the
		given co-ordinates are inside the image's boundary.

		The multiplier must be in the range 0 to 1.0, and the current alpha
		at the given co-ordinates will be multiplied by this value.

		@see setPixelAt
	*/
	void multiplyAlphaAt (int x, int y, float multiplier);

	/** Changes the overall opacity of the image.

		This will multiply the alpha value of each pixel in the image by the given
		amount (limiting the resulting alpha values between 0 and 255). This allows
		you to make an image more or less transparent.

		If the image doesn't have an alpha channel, this won't have any effect.
	*/
	void multiplyAllAlphas (float amountToMultiplyBy);

	/** Changes all the colours to be shades of grey, based on their current luminosity.
	*/
	void desaturate();

	/** Retrieves a section of an image as raw pixel data, so it can be read or written to.

		You should only use this class as a last resort - messing about with the internals of
		an image is only recommended for people who really know what they're doing!

		A BitmapData object should be used as a temporary, stack-based object. Don't keep one
		hanging around while the image is being used elsewhere.

		Depending on the way the image class is implemented, this may create a temporary buffer
		which is copied back to the image when the object is deleted, or it may just get a pointer
		directly into the image's raw data.

		You can use the stride and data values in this class directly, but don't alter them!
		The actual format of the pixel data depends on the image's format - see Image::getFormat(),
		and the PixelRGB, PixelARGB and PixelAlpha classes for more info.
	*/
	class BitmapData
	{
	public:
		enum ReadWriteMode
		{
			readOnly,
			writeOnly,
			readWrite
		};

		BitmapData (Image& image, int x, int y, int w, int h, ReadWriteMode mode);
		BitmapData (const Image& image, int x, int y, int w, int h);
		BitmapData (const Image& image, ReadWriteMode mode);
		~BitmapData();

		/** Returns a pointer to the start of a line in the image.
			The co-ordinate you provide here isn't checked, so it's the caller's responsibility to make
			sure it's not out-of-range.
		*/
		inline uint8* getLinePointer (int y) const noexcept                 { return data + y * lineStride; }

		/** Returns a pointer to a pixel in the image.
			The co-ordinates you give here are not checked, so it's the caller's responsibility to make sure they're
			not out-of-range.
		*/
		inline uint8* getPixelPointer (int x, int y) const noexcept         { return data + y * lineStride + x * pixelStride; }

		/** Returns the colour of a given pixel.
			For performance reasons, this won't do any bounds-checking on the coordinates, so it's the caller's
			repsonsibility to make sure they're within the image's size.
		*/
		Colour getPixelColour (int x, int y) const noexcept;

		/** Sets the colour of a given pixel.
			For performance reasons, this won't do any bounds-checking on the coordinates, so it's the caller's
			repsonsibility to make sure they're within the image's size.
		*/
		void setPixelColour (int x, int y, const Colour& colour) const noexcept;

		uint8* data;
		PixelFormat pixelFormat;
		int lineStride, pixelStride, width, height;

		/** Used internally by custom image types to manage pixel data lifetime. */
		class BitmapDataReleaser
		{
		protected:
			BitmapDataReleaser() {}
		public:
			virtual ~BitmapDataReleaser() {}
		};

		ScopedPointer<BitmapDataReleaser> dataReleaser;

	private:
		JUCE_DECLARE_NON_COPYABLE (BitmapData);
	};

	/** Copies a section of the image to somewhere else within itself. */
	void moveImageSection (int destX, int destY,
						   int sourceX, int sourceY,
						   int width, int height);

	/** Creates a RectangleList containing rectangles for all non-transparent pixels
		of the image.

		@param result           the list that will have the area added to it
		@param alphaThreshold   for a semi-transparent image, any pixels whose alpha is
								above this level will be considered opaque
	*/
	void createSolidAreaMask (RectangleList& result,
							  float alphaThreshold = 0.5f) const;

	/** Returns a NamedValueSet that is attached to the image and which can be used for
		associating custom values with it.

		If this is a null image, this will return a null pointer.
	*/
	NamedValueSet* getProperties() const;

	/** Creates a context suitable for drawing onto this image.
		Don't call this method directly! It's used internally by the Graphics class.
	*/
	LowLevelGraphicsContext* createLowLevelContext() const;

	/** Returns the number of Image objects which are currently referring to the same internal
		shared image data.

		@see duplicateIfShared
	*/
	int getReferenceCount() const noexcept;

	/** @internal */
	ImagePixelData* getPixelData() const noexcept       { return image; }

	/** @internal */
	explicit Image (ImagePixelData*);

private:

	ReferenceCountedObjectPtr<ImagePixelData> image;

	JUCE_LEAK_DETECTOR (Image);
};

/**
	This is a base class for holding image data in implementation-specific ways.

	You may never need to use this class directly - it's used internally
	by the Image class to store the actual image data. To access pixel data directly,
	you should use Image::BitmapData rather than this class.

	ImagePixelData objects are created indirectly, by subclasses of ImageType.
	@see Image, ImageType
*/
class JUCE_API  ImagePixelData  : public ReferenceCountedObject
{
public:
	ImagePixelData (Image::PixelFormat, int width, int height);
	~ImagePixelData();

	/** Creates a context that will draw into this image. */
	virtual LowLevelGraphicsContext* createLowLevelContext() = 0;
	/** Creates a copy of this image. */
	virtual ImagePixelData* clone() = 0;
	/** Creates an instance of the type of this image. */
	virtual ImageType* createType() const = 0;
	/** Initialises a BitmapData object. */
	virtual void initialiseBitmapData (Image::BitmapData&, int x, int y, Image::BitmapData::ReadWriteMode) = 0;

	/** The pixel format of the image data. */
	const Image::PixelFormat pixelFormat;
	const int width, height;

	/** User-defined settings that are attached to this image.
		@see Image::getProperties().
	*/
	NamedValueSet userData;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImagePixelData);
};

/**
	This base class is for handlers that control a type of image manipulation format,
	e.g. an in-memory bitmap, an OpenGL image, CoreGraphics image, etc.

	@see SoftwareImageType, NativeImageType, OpenGLImageType
*/
class JUCE_API  ImageType
{
public:
	ImageType();
	virtual ~ImageType();

	/** Creates a new image of this type, and the specified parameters. */
	virtual ImagePixelData* create (Image::PixelFormat format, int width, int height, bool shouldClearImage) const = 0;

	/** Must return a unique number to identify this type. */
	virtual int getTypeID() const = 0;

	/** Returns an image which is a copy of the source image, but using this type of storage mechanism.
		For example, to make sure that an image is stored in-memory, you could use:
		@code myImage = SoftwareImageType().convert (myImage); @endcode
	*/
	virtual Image convert (const Image& source) const;
};

/**
	An image storage type which holds the pixels in-memory as a simple block of values.
	@see ImageType, NativeImageType
*/
class JUCE_API  SoftwareImageType   : public ImageType
{
public:
	SoftwareImageType();
	~SoftwareImageType();

	ImagePixelData* create (Image::PixelFormat, int width, int height, bool clearImage) const;
	int getTypeID() const;
};

/**
	An image storage type which holds the pixels using whatever is the default storage
	format on the current platform.
	@see ImageType, SoftwareImageType
*/
class JUCE_API  NativeImageType   : public ImageType
{
public:
	NativeImageType();
	~NativeImageType();

	ImagePixelData* create (Image::PixelFormat, int width, int height, bool clearImage) const;
	int getTypeID() const;
};

#endif   // __JUCE_IMAGE_JUCEHEADER__

/*** End of inlined file: juce_Image.h ***/

/**
	Represents a colour or fill pattern to use for rendering paths.

	This is used by the Graphics and DrawablePath classes as a way to encapsulate
	a brush type. It can either be a solid colour, a gradient, or a tiled image.

	@see Graphics::setFillType, DrawablePath::setFill
*/
class JUCE_API  FillType
{
public:

	/** Creates a default fill type, of solid black. */
	FillType() noexcept;

	/** Creates a fill type of a solid colour.
		@see setColour
	*/
	FillType (const Colour& colour) noexcept;

	/** Creates a gradient fill type.
		@see setGradient
	*/
	FillType (const ColourGradient& gradient);

	/** Creates a tiled image fill type. The transform allows you to set the scaling, offset
		and rotation of the pattern.
		@see setTiledImage
	*/
	FillType (const Image& image, const AffineTransform& transform) noexcept;

	/** Creates a copy of another FillType. */
	FillType (const FillType& other);

	/** Makes a copy of another FillType. */
	FillType& operator= (const FillType& other);

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	FillType (FillType&& other) noexcept;
	FillType& operator= (FillType&& other) noexcept;
   #endif

	/** Destructor. */
	~FillType() noexcept;

	/** Returns true if this is a solid colour fill, and not a gradient or image. */
	bool isColour() const noexcept          { return gradient == nullptr && image.isNull(); }

	/** Returns true if this is a gradient fill. */
	bool isGradient() const noexcept        { return gradient != nullptr; }

	/** Returns true if this is a tiled image pattern fill. */
	bool isTiledImage() const noexcept      { return image.isValid(); }

	/** Turns this object into a solid colour fill.
		If the object was an image or gradient, those fields will no longer be valid. */
	void setColour (const Colour& newColour) noexcept;

	/** Turns this object into a gradient fill. */
	void setGradient (const ColourGradient& newGradient);

	/** Turns this object into a tiled image fill type. The transform allows you to set
		the scaling, offset and rotation of the pattern.
	*/
	void setTiledImage (const Image& image, const AffineTransform& transform) noexcept;

	/** Changes the opacity that should be used.
		If the fill is a solid colour, this just changes the opacity of that colour. For
		gradients and image tiles, it changes the opacity that will be used for them.
	*/
	void setOpacity (float newOpacity) noexcept;

	/** Returns the current opacity to be applied to the colour, gradient, or image.
		@see setOpacity
	*/
	float getOpacity() const noexcept       { return colour.getFloatAlpha(); }

	/** Returns true if this fill type is completely transparent. */
	bool isInvisible() const noexcept;

	/** Returns a copy of this fill, adding the specified transform applied to the
		existing transform.
	*/
	FillType transformed (const AffineTransform& transform) const;

	/** The solid colour being used.

		If the fill type is not a solid colour, the alpha channel of this colour indicates
		the opacity that should be used for the fill, and the RGB channels are ignored.
	*/
	Colour colour;

	/** Returns the gradient that should be used for filling.
		This will be zero if the object is some other type of fill.
		If a gradient is active, the overall opacity with which it should be applied
		is indicated by the alpha channel of the colour variable.
	*/
	ScopedPointer <ColourGradient> gradient;

	/** The image that should be used for tiling.
		If an image fill is active, the overall opacity with which it should be applied
		is indicated by the alpha channel of the colour variable.
	*/
	Image image;

	/** The transform that should be applied to the image or gradient that's being drawn. */
	AffineTransform transform;

	bool operator== (const FillType& other) const;
	bool operator!= (const FillType& other) const;

private:
	JUCE_LEAK_DETECTOR (FillType);
};

#endif   // __JUCE_FILLTYPE_JUCEHEADER__

/*** End of inlined file: juce_FillType.h ***/


#endif
#ifndef __JUCE_PIXELFORMATS_JUCEHEADER__

#endif
#ifndef __JUCE_AFFINETRANSFORM_JUCEHEADER__

#endif
#ifndef __JUCE_BORDERSIZE_JUCEHEADER__

/*** Start of inlined file: juce_BorderSize.h ***/
#ifndef __JUCE_BORDERSIZE_JUCEHEADER__
#define __JUCE_BORDERSIZE_JUCEHEADER__

/**
	Specifies a set of gaps to be left around the sides of a rectangle.

	This is basically the size of the spaces at the top, bottom, left and right of
	a rectangle. It's used by various component classes to specify borders.

	@see Rectangle
*/
template <typename ValueType>
class BorderSize
{
public:

	/** Creates a null border.
		All sizes are left as 0.
	*/
	BorderSize() noexcept
		: top(), left(), bottom(), right()
	{
	}

	/** Creates a copy of another border. */
	BorderSize (const BorderSize& other) noexcept
		: top (other.top), left (other.left), bottom (other.bottom), right (other.right)
	{
	}

	/** Creates a border with the given gaps. */
	BorderSize (ValueType topGap, ValueType leftGap, ValueType bottomGap, ValueType rightGap) noexcept
		: top (topGap), left (leftGap), bottom (bottomGap), right (rightGap)
	{
	}

	/** Creates a border with the given gap on all sides. */
	explicit BorderSize (ValueType allGaps) noexcept
		: top (allGaps), left (allGaps), bottom (allGaps), right (allGaps)
	{
	}

	/** Returns the gap that should be left at the top of the region. */
	ValueType getTop() const noexcept                   { return top; }

	/** Returns the gap that should be left at the top of the region. */
	ValueType getLeft() const noexcept                  { return left; }

	/** Returns the gap that should be left at the top of the region. */
	ValueType getBottom() const noexcept                { return bottom; }

	/** Returns the gap that should be left at the top of the region. */
	ValueType getRight() const noexcept                 { return right; }

	/** Returns the sum of the top and bottom gaps. */
	ValueType getTopAndBottom() const noexcept          { return top + bottom; }

	/** Returns the sum of the left and right gaps. */
	ValueType getLeftAndRight() const noexcept          { return left + right; }

	/** Returns true if this border has no thickness along any edge. */
	bool isEmpty() const noexcept                       { return left + right + top + bottom == ValueType(); }

	/** Changes the top gap. */
	void setTop (ValueType newTopGap) noexcept          { top = newTopGap; }

	/** Changes the left gap. */
	void setLeft (ValueType newLeftGap) noexcept        { left = newLeftGap; }

	/** Changes the bottom gap. */
	void setBottom (ValueType newBottomGap) noexcept    { bottom = newBottomGap; }

	/** Changes the right gap. */
	void setRight (ValueType newRightGap) noexcept      { right = newRightGap; }

	/** Returns a rectangle with these borders removed from it. */
	Rectangle<ValueType> subtractedFrom (const Rectangle<ValueType>& original) const noexcept
	{
		return Rectangle<ValueType> (original.getX() + left,
									 original.getY() + top,
									 original.getWidth() - (left + right),
									 original.getHeight() - (top + bottom));
	}

	/** Removes this border from a given rectangle. */
	void subtractFrom (Rectangle<ValueType>& rectangle) const noexcept
	{
		rectangle = subtractedFrom (rectangle);
	}

	/** Returns a rectangle with these borders added around it. */
	Rectangle<ValueType> addedTo (const Rectangle<ValueType>& original) const noexcept
	{
		return Rectangle<ValueType> (original.getX() - left,
									 original.getY() - top,
									 original.getWidth() + (left + right),
									 original.getHeight() + (top + bottom));
	}

	/** Adds this border around a given rectangle. */
	void addTo (Rectangle<ValueType>& rectangle) const noexcept
	{
		rectangle = addedTo (rectangle);
	}

	bool operator== (const BorderSize& other) const noexcept
	{
		return top == other.top && left == other.left && bottom == other.bottom && right == other.right;
	}

	bool operator!= (const BorderSize& other) const noexcept
	{
		return ! operator== (other);
	}

private:

	ValueType top, left, bottom, right;

	JUCE_LEAK_DETECTOR (BorderSize);
};

#endif   // __JUCE_BORDERSIZE_JUCEHEADER__

/*** End of inlined file: juce_BorderSize.h ***/


#endif
#ifndef __JUCE_EDGETABLE_JUCEHEADER__

/*** Start of inlined file: juce_EdgeTable.h ***/
#ifndef __JUCE_EDGETABLE_JUCEHEADER__
#define __JUCE_EDGETABLE_JUCEHEADER__


/*** Start of inlined file: juce_RectangleList.h ***/
#ifndef __JUCE_RECTANGLELIST_JUCEHEADER__
#define __JUCE_RECTANGLELIST_JUCEHEADER__

/**
	Maintains a set of rectangles as a complex region.

	This class allows a set of rectangles to be treated as a solid shape, and can
	add and remove rectangular sections of it, and simplify overlapping or
	adjacent rectangles.

	@see Rectangle
*/
class JUCE_API  RectangleList
{
public:

	/** Creates an empty RectangleList */
	RectangleList() noexcept;

	/** Creates a copy of another list */
	RectangleList (const RectangleList& other);

	/** Creates a list containing just one rectangle. */
	RectangleList (const Rectangle<int>& rect);

	/** Copies this list from another one. */
	RectangleList& operator= (const RectangleList& other);

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	RectangleList (RectangleList&& other) noexcept;
	RectangleList& operator= (RectangleList&& other) noexcept;
   #endif

	/** Destructor. */
	~RectangleList();

	/** Returns true if the region is empty. */
	bool isEmpty() const noexcept;

	/** Returns the number of rectangles in the list. */
	int getNumRectangles() const noexcept                       { return rects.size(); }

	/** Returns one of the rectangles at a particular index.

		@returns    the rectangle at the index, or an empty rectangle if the
					index is out-of-range.
	*/
	Rectangle<int> getRectangle (int index) const noexcept;

	/** Removes all rectangles to leave an empty region. */
	void clear();

	/** Merges a new rectangle into the list.

		The rectangle being added will first be clipped to remove any parts of it
		that overlap existing rectangles in the list.
	*/
	void add (int x, int y, int width, int height);

	/** Merges a new rectangle into the list.

		The rectangle being added will first be clipped to remove any parts of it
		that overlap existing rectangles in the list, and adjacent rectangles will be
		merged into it.
	*/
	void add (const Rectangle<int>& rect);

	/** Dumbly adds a rectangle to the list without checking for overlaps.

		This simply adds the rectangle to the end, it doesn't merge it or remove
		any overlapping bits.
	*/
	void addWithoutMerging (const Rectangle<int>& rect);

	/** Merges another rectangle list into this one.

		Any overlaps between the two lists will be clipped, so that the result is
		the union of both lists.
	*/
	void add (const RectangleList& other);

	/** Removes a rectangular region from the list.

		Any rectangles in the list which overlap this will be clipped and subdivided
		if necessary.
	*/
	void subtract (const Rectangle<int>& rect);

	/** Removes all areas in another RectangleList from this one.

		Any rectangles in the list which overlap this will be clipped and subdivided
		if necessary.

		@returns true if the resulting list is non-empty.
	*/
	bool subtract (const RectangleList& otherList);

	/** Removes any areas of the region that lie outside a given rectangle.

		Any rectangles in the list which overlap this will be clipped and subdivided
		if necessary.

		Returns true if the resulting region is not empty, false if it is empty.

		@see getIntersectionWith
	*/
	bool clipTo (const Rectangle<int>& rect);

	/** Removes any areas of the region that lie outside a given rectangle list.

		Any rectangles in this object which overlap the specified list will be clipped
		and subdivided if necessary.

		Returns true if the resulting region is not empty, false if it is empty.

		@see getIntersectionWith
	*/
	bool clipTo (const RectangleList& other);

	/** Creates a region which is the result of clipping this one to a given rectangle.

		Unlike the other clipTo method, this one doesn't affect this object - it puts the
		resulting region into the list whose reference is passed-in.

		Returns true if the resulting region is not empty, false if it is empty.

		@see clipTo
	*/
	bool getIntersectionWith (const Rectangle<int>& rect, RectangleList& destRegion) const;

	/** Swaps the contents of this and another list.

		This swaps their internal pointers, so is hugely faster than using copy-by-value
		to swap them.
	*/
	void swapWith (RectangleList& otherList) noexcept;

	/** Checks whether the region contains a given point.

		@returns true if the point lies within one of the rectangles in the list
	*/
	bool containsPoint (int x, int y) const noexcept;

	/** Checks whether the region contains the whole of a given rectangle.

		@returns    true all parts of the rectangle passed in lie within the region
					defined by this object
		@see intersectsRectangle, containsPoint
	*/
	bool containsRectangle (const Rectangle<int>& rectangleToCheck) const;

	/** Checks whether the region contains any part of a given rectangle.

		@returns    true if any part of the rectangle passed in lies within the region
					defined by this object
		@see containsRectangle
	*/
	bool intersectsRectangle (const Rectangle<int>& rectangleToCheck) const noexcept;

	/** Checks whether this region intersects any part of another one.

		@see intersectsRectangle
	*/
	bool intersects (const RectangleList& other) const noexcept;

	/** Returns the smallest rectangle that can enclose the whole of this region. */
	Rectangle<int> getBounds() const noexcept;

	/** Optimises the list into a minimum number of constituent rectangles.

		This will try to combine any adjacent rectangles into larger ones where
		possible, to simplify lists that might have been fragmented by repeated
		add/subtract calls.
	*/
	void consolidate();

	/** Adds an x and y value to all the co-ordinates. */
	void offsetAll (int dx, int dy) noexcept;

	/** Creates a Path object to represent this region. */
	Path toPath() const;

	/** An iterator for accessing all the rectangles in a RectangleList. */
	class JUCE_API  Iterator
	{
	public:

		Iterator (const RectangleList& list) noexcept;
		~Iterator();

		/** Advances to the next rectangle, and returns true if it's not finished.

			Call this before using getRectangle() to find the rectangle that was returned.
		*/
		bool next() noexcept;

		/** Returns the current rectangle. */
		const Rectangle<int>* getRectangle() const noexcept      { return current; }

	private:
		const Rectangle<int>* current;
		const RectangleList& owner;
		int index;

		JUCE_DECLARE_NON_COPYABLE (Iterator);
	};

private:

	friend class Iterator;
	Array <Rectangle<int> > rects;

	JUCE_LEAK_DETECTOR (RectangleList);
};

#endif   // __JUCE_RECTANGLELIST_JUCEHEADER__

/*** End of inlined file: juce_RectangleList.h ***/

class Path;
class Image;

/**
	A table of horizontal scan-line segments - used for rasterising Paths.

	@see Path, Graphics
*/
class JUCE_API  EdgeTable
{
public:

	/** Creates an edge table containing a path.

		A table is created with a fixed vertical range, and only sections of the path
		which lie within this range will be added to the table.

		@param clipLimits               only the region of the path that lies within this area will be added
		@param pathToAdd                the path to add to the table
		@param transform                a transform to apply to the path being added
	*/
	EdgeTable (const Rectangle<int>& clipLimits,
			   const Path& pathToAdd,
			   const AffineTransform& transform);

	/** Creates an edge table containing a rectangle. */
	explicit EdgeTable (const Rectangle<int>& rectangleToAdd);

	/** Creates an edge table containing a rectangle list. */
	explicit EdgeTable (const RectangleList& rectanglesToAdd);

	/** Creates an edge table containing a rectangle. */
	explicit EdgeTable (const Rectangle<float>& rectangleToAdd);

	/** Creates a copy of another edge table. */
	EdgeTable (const EdgeTable& other);

	/** Copies from another edge table. */
	EdgeTable& operator= (const EdgeTable& other);

	/** Destructor. */
	~EdgeTable();

	void clipToRectangle (const Rectangle<int>& r);
	void excludeRectangle (const Rectangle<int>& r);
	void clipToEdgeTable (const EdgeTable& other);
	void clipLineToMask (int x, int y, const uint8* mask, int maskStride, int numPixels);
	bool isEmpty() noexcept;
	const Rectangle<int>& getMaximumBounds() const noexcept      { return bounds; }
	void translate (float dx, int dy) noexcept;

	/** Reduces the amount of space the table has allocated.

		This will shrink the table down to use as little memory as possible - useful for
		read-only tables that get stored and re-used for rendering.
	*/
	void optimiseTable();

	/** Iterates the lines in the table, for rendering.

		This function will iterate each line in the table, and call a user-defined class
		to render each pixel or continuous line of pixels that the table contains.

		@param iterationCallback    this templated class must contain the following methods:
										@code
										inline void setEdgeTableYPos (int y);
										inline void handleEdgeTablePixel (int x, int alphaLevel) const;
										inline void handleEdgeTablePixelFull (int x) const;
										inline void handleEdgeTableLine (int x, int width, int alphaLevel) const;
										inline void handleEdgeTableLineFull (int x, int width) const;
										@endcode
										(these don't necessarily have to be 'const', but it might help it go faster)
	*/
	template <class EdgeTableIterationCallback>
	void iterate (EdgeTableIterationCallback& iterationCallback) const noexcept
	{
		const int* lineStart = table;

		for (int y = 0; y < bounds.getHeight(); ++y)
		{
			const int* line = lineStart;
			lineStart += lineStrideElements;
			int numPoints = line[0];

			if (--numPoints > 0)
			{
				int x = *++line;
				jassert ((x >> 8) >= bounds.getX() && (x >> 8) < bounds.getRight());
				int levelAccumulator = 0;

				iterationCallback.setEdgeTableYPos (bounds.getY() + y);

				while (--numPoints >= 0)
				{
					const int level = *++line;
					jassert (isPositiveAndBelow (level, (int) 256));
					const int endX = *++line;
					jassert (endX >= x);
					const int endOfRun = (endX >> 8);

					if (endOfRun == (x >> 8))
					{
						// small segment within the same pixel, so just save it for the next
						// time round..
						levelAccumulator += (endX - x) * level;
					}
					else
					{
						// plot the fist pixel of this segment, including any accumulated
						// levels from smaller segments that haven't been drawn yet
						levelAccumulator += (0x100 - (x & 0xff)) * level;
						levelAccumulator >>= 8;
						x >>= 8;

						if (levelAccumulator > 0)
						{
							if (levelAccumulator >= 255)
								iterationCallback.handleEdgeTablePixelFull (x);
							else
								iterationCallback.handleEdgeTablePixel (x, levelAccumulator);
						}

						// if there's a run of similar pixels, do it all in one go..
						if (level > 0)
						{
							jassert (endOfRun <= bounds.getRight());
							const int numPix = endOfRun - ++x;

							if (numPix > 0)
								iterationCallback.handleEdgeTableLine (x, numPix, level);
						}

						// save the bit at the end to be drawn next time round the loop.
						levelAccumulator = (endX & 0xff) * level;
					}

					x = endX;
				}

				levelAccumulator >>= 8;

				if (levelAccumulator > 0)
				{
					x >>= 8;
					jassert (x >= bounds.getX() && x < bounds.getRight());

					if (levelAccumulator >= 255)
						iterationCallback.handleEdgeTablePixelFull (x);
					else
						iterationCallback.handleEdgeTablePixel (x, levelAccumulator);
				}
			}
		}
	}

private:

	// table line format: number of points; point0 x, point0 levelDelta, point1 x, point1 levelDelta, etc
	HeapBlock<int> table;
	Rectangle<int> bounds;
	int maxEdgesPerLine, lineStrideElements;
	bool needToCheckEmptinesss;

	void addEdgePoint (int x, int y, int winding);
	void remapTableForNumEdges (int newNumEdgesPerLine);
	void intersectWithEdgeTableLine (int y, const int* otherLine);
	void clipEdgeTableLineToRange (int* line, int x1, int x2) noexcept;
	void sanitiseLevels (bool useNonZeroWinding) noexcept;
	static void copyEdgeTableData (int* dest, int destLineStride, const int* src, int srcLineStride, int numLines) noexcept;

	JUCE_LEAK_DETECTOR (EdgeTable);
};

#endif   // __JUCE_EDGETABLE_JUCEHEADER__

/*** End of inlined file: juce_EdgeTable.h ***/


#endif
#ifndef __JUCE_LINE_JUCEHEADER__

#endif
#ifndef __JUCE_PATH_JUCEHEADER__

#endif
#ifndef __JUCE_PATHITERATOR_JUCEHEADER__

/*** Start of inlined file: juce_PathIterator.h ***/
#ifndef __JUCE_PATHITERATOR_JUCEHEADER__
#define __JUCE_PATHITERATOR_JUCEHEADER__

/**
	Flattens a Path object into a series of straight-line sections.

	Use one of these to iterate through a Path object, and it will convert
	all the curves into line sections so it's easy to render or perform
	geometric operations on.

	@see Path
*/
class JUCE_API  PathFlatteningIterator
{
public:

	/** Creates a PathFlatteningIterator.

		After creation, use the next() method to initialise the fields in the
		object with the first line's position.

		@param path         the path to iterate along
		@param transform    a transform to apply to each point in the path being iterated
		@param tolerance    the amount by which the curves are allowed to deviate from the lines
							into which they are being broken down - a higher tolerance contains
							less lines, so can be generated faster, but will be less smooth.
	*/
	PathFlatteningIterator (const Path& path,
							const AffineTransform& transform = AffineTransform::identity,
							float tolerance = defaultTolerance);

	/** Destructor. */
	~PathFlatteningIterator();

	/** Fetches the next line segment from the path.

		This will update the member variables x1, y1, x2, y2, subPathIndex and closesSubPath
		so that they describe the new line segment.

		@returns false when there are no more lines to fetch.
	*/
	bool next();

	float x1;  /**< The x position of the start of the current line segment. */
	float y1;  /**< The y position of the start of the current line segment. */
	float x2;  /**< The x position of the end of the current line segment. */
	float y2;  /**< The y position of the end of the current line segment. */

	/** Indicates whether the current line segment is closing a sub-path.

		If the current line is the one that connects the end of a sub-path
		back to the start again, this will be true.
	*/
	bool closesSubPath;

	/** The index of the current line within the current sub-path.

		E.g. you can use this to see whether the line is the first one in the
		subpath by seeing if it's 0.
	*/
	int subPathIndex;

	/** Returns true if the current segment is the last in the current sub-path. */
	bool isLastInSubpath() const noexcept;

	/** This is the default value that should be used for the tolerance value (see the constructor parameters). */
	static const float defaultTolerance;

private:

	const Path& path;
	const AffineTransform transform;
	float* points;
	const float toleranceSquared;
	float subPathCloseX, subPathCloseY;
	const bool isIdentityTransform;

	HeapBlock <float> stackBase;
	float* stackPos;
	size_t index, stackSize;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathFlatteningIterator);
};

#endif   // __JUCE_PATHITERATOR_JUCEHEADER__

/*** End of inlined file: juce_PathIterator.h ***/


#endif
#ifndef __JUCE_PATHSTROKETYPE_JUCEHEADER__

#endif
#ifndef __JUCE_POINT_JUCEHEADER__

#endif
#ifndef __JUCE_RECTANGLE_JUCEHEADER__

#endif
#ifndef __JUCE_RECTANGLELIST_JUCEHEADER__

#endif
#ifndef __JUCE_JUSTIFICATION_JUCEHEADER__

#endif
#ifndef __JUCE_RECTANGLEPLACEMENT_JUCEHEADER__

#endif
#ifndef __JUCE_GRAPHICSCONTEXT_JUCEHEADER__

#endif
#ifndef __JUCE_LOWLEVELGRAPHICSCONTEXT_JUCEHEADER__

/*** Start of inlined file: juce_LowLevelGraphicsContext.h ***/
#ifndef __JUCE_LOWLEVELGRAPHICSCONTEXT_JUCEHEADER__
#define __JUCE_LOWLEVELGRAPHICSCONTEXT_JUCEHEADER__

class AttributedString;

/**
	Interface class for graphics context objects, used internally by the Graphics class.

	Users are not supposed to create instances of this class directly - do your drawing
	via the Graphics object instead.

	It's a base class for different types of graphics context, that may perform software-based
	or OS-accelerated rendering.

	E.g. the LowLevelGraphicsSoftwareRenderer renders onto an image in memory, but other
	subclasses could render directly to a windows HDC, a Quartz context, or an OpenGL
	context.
*/
class JUCE_API  LowLevelGraphicsContext
{
protected:

	LowLevelGraphicsContext();

public:
	virtual ~LowLevelGraphicsContext();

	/** Returns true if this device is vector-based, e.g. a printer. */
	virtual bool isVectorDevice() const = 0;

	/** Moves the origin to a new position.

		The co-ords are relative to the current origin, and indicate the new position
		of (0, 0).
	*/
	virtual void setOrigin (int x, int y) = 0;
	virtual void addTransform (const AffineTransform& transform) = 0;
	virtual float getScaleFactor() = 0;

	virtual bool clipToRectangle (const Rectangle<int>& r) = 0;
	virtual bool clipToRectangleList (const RectangleList& clipRegion) = 0;
	virtual void excludeClipRectangle (const Rectangle<int>& r) = 0;
	virtual void clipToPath (const Path& path, const AffineTransform& transform) = 0;
	virtual void clipToImageAlpha (const Image& sourceImage, const AffineTransform& transform) = 0;

	virtual bool clipRegionIntersects (const Rectangle<int>& r) = 0;
	virtual Rectangle<int> getClipBounds() const = 0;
	virtual bool isClipEmpty() const = 0;

	virtual void saveState() = 0;
	virtual void restoreState() = 0;

	virtual void beginTransparencyLayer (float opacity) = 0;
	virtual void endTransparencyLayer() = 0;

	virtual void setFill (const FillType& fillType) = 0;
	virtual void setOpacity (float newOpacity) = 0;
	virtual void setInterpolationQuality (Graphics::ResamplingQuality quality) = 0;

	virtual void fillRect (const Rectangle<int>& r, bool replaceExistingContents) = 0;
	virtual void fillPath (const Path& path, const AffineTransform& transform) = 0;

	virtual void drawImage (const Image& sourceImage, const AffineTransform& transform) = 0;

	virtual void drawLine (const Line <float>& line) = 0;
	virtual void drawVerticalLine (int x, float top, float bottom) = 0;
	virtual void drawHorizontalLine (int y, float left, float right) = 0;

	virtual void setFont (const Font& newFont) = 0;
	virtual const Font& getFont() = 0;
	virtual void drawGlyph (int glyphNumber, const AffineTransform& transform) = 0;
	virtual bool drawTextLayout (const AttributedString&, const Rectangle<float>&)  { return false; }
};

#endif   // __JUCE_LOWLEVELGRAPHICSCONTEXT_JUCEHEADER__

/*** End of inlined file: juce_LowLevelGraphicsContext.h ***/


#endif
#ifndef __JUCE_LOWLEVELGRAPHICSPOSTSCRIPTRENDERER_JUCEHEADER__

/*** Start of inlined file: juce_LowLevelGraphicsPostScriptRenderer.h ***/
#ifndef __JUCE_LOWLEVELGRAPHICSPOSTSCRIPTRENDERER_JUCEHEADER__
#define __JUCE_LOWLEVELGRAPHICSPOSTSCRIPTRENDERER_JUCEHEADER__

/**
	An implementation of LowLevelGraphicsContext that turns the drawing operations
	into a PostScript document.

*/
class JUCE_API  LowLevelGraphicsPostScriptRenderer    : public LowLevelGraphicsContext
{
public:

	LowLevelGraphicsPostScriptRenderer (OutputStream& resultingPostScript,
										const String& documentTitle,
										int totalWidth,
										int totalHeight);

	~LowLevelGraphicsPostScriptRenderer();

	bool isVectorDevice() const;
	void setOrigin (int x, int y);
	void addTransform (const AffineTransform& transform);
	float getScaleFactor();

	bool clipToRectangle (const Rectangle<int>& r);
	bool clipToRectangleList (const RectangleList& clipRegion);
	void excludeClipRectangle (const Rectangle<int>& r);
	void clipToPath (const Path& path, const AffineTransform& transform);
	void clipToImageAlpha (const Image& sourceImage, const AffineTransform& transform);

	void saveState();
	void restoreState();

	void beginTransparencyLayer (float opacity);
	void endTransparencyLayer();

	bool clipRegionIntersects (const Rectangle<int>& r);
	Rectangle<int> getClipBounds() const;
	bool isClipEmpty() const;

	void setFill (const FillType& fillType);
	void setOpacity (float opacity);
	void setInterpolationQuality (Graphics::ResamplingQuality quality);

	void fillRect (const Rectangle<int>& r, bool replaceExistingContents);
	void fillPath (const Path& path, const AffineTransform& transform);

	void drawImage (const Image& sourceImage, const AffineTransform& transform);

	void drawLine (const Line <float>& line);

	void drawVerticalLine (int x, float top, float bottom);
	void drawHorizontalLine (int x, float top, float bottom);

	const Font& getFont();
	void setFont (const Font& newFont);
	void drawGlyph (int glyphNumber, const AffineTransform& transform);

protected:

	OutputStream& out;
	int totalWidth, totalHeight;
	bool needToClip;
	Colour lastColour;

	struct SavedState
	{
		SavedState();
		~SavedState();

		RectangleList clip;
		int xOffset, yOffset;
		FillType fillType;
		Font font;

	private:
		SavedState& operator= (const SavedState&);
	};

	OwnedArray <SavedState> stateStack;

	void writeClip();
	void writeColour (const Colour& colour);
	void writePath (const Path& path) const;
	void writeXY (float x, float y) const;
	void writeTransform (const AffineTransform& trans) const;
	void writeImage (const Image& im, int sx, int sy, int maxW, int maxH) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowLevelGraphicsPostScriptRenderer);
};

#endif   // __JUCE_LOWLEVELGRAPHICSPOSTSCRIPTRENDERER_JUCEHEADER__

/*** End of inlined file: juce_LowLevelGraphicsPostScriptRenderer.h ***/


#endif
#ifndef __JUCE_LOWLEVELGRAPHICSSOFTWARERENDERER_JUCEHEADER__

/*** Start of inlined file: juce_LowLevelGraphicsSoftwareRenderer.h ***/
#ifndef __JUCE_LOWLEVELGRAPHICSSOFTWARERENDERER_JUCEHEADER__
#define __JUCE_LOWLEVELGRAPHICSSOFTWARERENDERER_JUCEHEADER__

#ifndef DOXYGEN

/*** Start of inlined file: juce_RenderingHelpers.h ***/
#ifndef __JUCE_RENDERINGHELPERS_JUCEHEADER__
#define __JUCE_RENDERINGHELPERS_JUCEHEADER__

namespace RenderingHelpers
{

/** Holds either a simple integer translation, or an affine transform.
*/
class TranslationOrTransform
{
public:
	TranslationOrTransform (int xOffset_, int yOffset_) noexcept
		: xOffset (xOffset_), yOffset (yOffset_), isOnlyTranslated (true)
	{
	}

	TranslationOrTransform (const TranslationOrTransform& other) noexcept
		: complexTransform (other.complexTransform),
		  xOffset (other.xOffset), yOffset (other.yOffset),
		  isOnlyTranslated (other.isOnlyTranslated)
	{
	}

	AffineTransform getTransform() const noexcept
	{
		return isOnlyTranslated ? AffineTransform::translation ((float) xOffset, (float) yOffset)
								: complexTransform;
	}

	AffineTransform getTransformWith (const AffineTransform& userTransform) const noexcept
	{
		return isOnlyTranslated ? userTransform.translated ((float) xOffset, (float) yOffset)
								: userTransform.followedBy (complexTransform);
	}

	void setOrigin (const int x, const int y) noexcept
	{
		if (isOnlyTranslated)
		{
			xOffset += x;
			yOffset += y;
		}
		else
		{
			complexTransform = AffineTransform::translation ((float) x, (float) y)
											   .followedBy (complexTransform);
		}
	}

	void addTransform (const AffineTransform& t) noexcept
	{
		if (isOnlyTranslated
			 && t.isOnlyTranslation()
			 && isIntegerTranslation (t))
		{
			xOffset += (int) t.getTranslationX();
			yOffset += (int) t.getTranslationY();
		}
		else
		{
			complexTransform = getTransformWith (t);
			isOnlyTranslated = false;
		}
	}

	float getScaleFactor() const noexcept
	{
		return isOnlyTranslated ? 1.0f : complexTransform.getScaleFactor();
	}

	void moveOriginInDeviceSpace (const int dx, const int dy) noexcept
	{
		if (isOnlyTranslated)
		{
			xOffset += dx;
			yOffset += dy;
		}
		else
		{
			complexTransform = complexTransform.translated ((float) dx, (float) dy);
		}
	}

	template <typename Type>
	Rectangle<Type> translated (const Rectangle<Type>& r) const noexcept
	{
		jassert (isOnlyTranslated);
		return r.translated (static_cast <Type> (xOffset),
							 static_cast <Type> (yOffset));
	}

	Rectangle<int> deviceSpaceToUserSpace (const Rectangle<int>& r) const noexcept
	{
		return isOnlyTranslated ? r.translated (-xOffset, -yOffset)
								: r.toFloat().transformed (complexTransform.inverted()).getSmallestIntegerContainer();
	}

	AffineTransform complexTransform;
	int xOffset, yOffset;
	bool isOnlyTranslated;

private:
	static inline bool isIntegerTranslation (const AffineTransform& t) noexcept
	{
		const int tx = (int) (t.getTranslationX() * 256.0f);
		const int ty = (int) (t.getTranslationY() * 256.0f);
		return ((tx | ty) & 0xf8) == 0;
	}
};

template <class CachedGlyphType, class RenderTargetType>
class GlyphCache  : private DeletedAtShutdown
{
public:
	GlyphCache()
	{
		addNewGlyphSlots (120);
	}

	~GlyphCache()
	{
		getSingletonPointer() = nullptr;
	}

	static GlyphCache& getInstance()
	{
		GlyphCache*& g = getSingletonPointer();

		if (g == nullptr)
			g = new GlyphCache();

		return *g;
	}

	void drawGlyph (RenderTargetType& target, const Font& font, const int glyphNumber, float x, float y)
	{
		++accessCounter;
		CachedGlyphType* glyph = nullptr;

		const ScopedReadLock srl (lock);

		for (int i = glyphs.size(); --i >= 0;)
		{
			CachedGlyphType* const g = glyphs.getUnchecked (i);

			if (g->glyph == glyphNumber && g->font == font)
			{
				glyph = g;
				++hits;
				break;
			}
		}

		if (glyph == nullptr)
		{
			++misses;
			const ScopedWriteLock swl (lock);

			if (hits.value + misses.value > glyphs.size() * 16)
			{
				if (misses.value * 2 > hits.value)
					addNewGlyphSlots (32);

				hits.set (0);
				misses.set (0);
				glyph = glyphs.getLast();
			}
			else
			{
				glyph = findLeastRecentlyUsedGlyph();
			}

			jassert (glyph != nullptr);
			glyph->generate (font, glyphNumber);
		}

		glyph->lastAccessCount = accessCounter.value;
		glyph->draw (target, x, y);
	}

private:
	friend class OwnedArray <CachedGlyphType>;
	OwnedArray <CachedGlyphType> glyphs;
	Atomic<int> accessCounter, hits, misses;
	ReadWriteLock lock;

	void addNewGlyphSlots (int num)
	{
		while (--num >= 0)
			glyphs.add (new CachedGlyphType());
	}

	CachedGlyphType* findLeastRecentlyUsedGlyph() const noexcept
	{
		CachedGlyphType* oldest = glyphs.getLast();
		int oldestCounter = oldest->lastAccessCount;

		for (int i = glyphs.size() - 1; --i >= 0;)
		{
			CachedGlyphType* const glyph = glyphs.getUnchecked(i);

			if (glyph->lastAccessCount <= oldestCounter)
			{
				oldestCounter = glyph->lastAccessCount;
				oldest = glyph;
			}
		}

		return oldest;
	}

	static GlyphCache*& getSingletonPointer() noexcept
	{
		static GlyphCache* g = nullptr;
		return g;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlyphCache);
};

template <class RendererType>
class CachedGlyphEdgeTable
{
public:
	CachedGlyphEdgeTable() : glyph (0), lastAccessCount (0) {}

	void draw (RendererType& state, float x, const float y) const
	{
		if (snapToIntegerCoordinate)
			x = std::floor (x + 0.5f);

		if (edgeTable != nullptr)
			state.fillEdgeTable (*edgeTable, x, roundToInt (y));
	}

	void generate (const Font& newFont, const int glyphNumber)
	{
		font = newFont;
		Typeface* const typeface = newFont.getTypeface();
		snapToIntegerCoordinate = typeface->isHinted();
		glyph = glyphNumber;

		const float fontHeight = font.getHeight();
		edgeTable = typeface->getEdgeTableForGlyph (glyphNumber,
													AffineTransform::scale (fontHeight * font.getHorizontalScale(), fontHeight)
																  #if JUCE_MAC || JUCE_IOS
																	.translated (0.0f, -0.5f)
																  #endif
													);
	}

	Font font;
	int glyph, lastAccessCount;
	bool snapToIntegerCoordinate;

private:
	ScopedPointer <EdgeTable> edgeTable;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CachedGlyphEdgeTable);
};

template <class StateObjectType>
class SavedStateStack
{
public:
	SavedStateStack (StateObjectType* const initialState) noexcept
		: currentState (initialState)
	{}

	inline StateObjectType* operator->() const noexcept     { return currentState; }
	inline StateObjectType& operator*()  const noexcept     { return *currentState; }

	void save()
	{
		stack.add (new StateObjectType (*currentState));
	}

	void restore()
	{
		StateObjectType* const top = stack.getLast();

		if (top != nullptr)
		{
			currentState = top;
			stack.removeLast (1, false);
		}
		else
		{
			jassertfalse; // trying to pop with an empty stack!
		}
	}

	void beginTransparencyLayer (float opacity)
	{
		save();
		currentState = currentState->beginTransparencyLayer (opacity);
	}

	void endTransparencyLayer()
	{
		const ScopedPointer<StateObjectType> finishedTransparencyLayer (currentState);
		restore();
		currentState->endTransparencyLayer (*finishedTransparencyLayer);
	}

private:
	ScopedPointer<StateObjectType> currentState;
	OwnedArray<StateObjectType> stack;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SavedStateStack);
};

// Calculates the alpha values and positions for rendering the edges of a non-pixel
// aligned rectangle.
struct FloatRectangleRasterisingInfo
{
	FloatRectangleRasterisingInfo (const Rectangle<float>& area)
		: left   (roundToInt (256.0f * area.getX())),
		  top    (roundToInt (256.0f * area.getY())),
		  right  (roundToInt (256.0f * area.getRight())),
		  bottom (roundToInt (256.0f * area.getBottom()))
	{
		if ((top >> 8) == (bottom >> 8))
		{
			topAlpha = bottom - top;
			bottomAlpha = 0;
			totalTop = top >> 8;
			totalBottom = bottom = top = totalTop + 1;
		}
		else
		{
			if ((top & 255) == 0)
			{
				topAlpha = 0;
				top = totalTop = (top >> 8);
			}
			else
			{
				topAlpha = 255 - (top & 255);
				totalTop = (top >> 8);
				top = totalTop + 1;
			}

			bottomAlpha = bottom & 255;
			bottom >>= 8;
			totalBottom = bottom + (bottomAlpha != 0 ? 1 : 0);
		}

		if ((left >> 8) == (right >> 8))
		{
			leftAlpha = right - left;
			rightAlpha = 0;
			totalLeft = (left >> 8);
			totalRight = right = left = totalLeft + 1;
		}
		else
		{
			if ((left & 255) == 0)
			{
				leftAlpha = 0;
				left = totalLeft = (left >> 8);
			}
			else
			{
				leftAlpha = 255 - (left & 255);
				totalLeft = (left >> 8);
				left = totalLeft + 1;
			}

			rightAlpha = right & 255;
			right >>= 8;
			totalRight = right + (rightAlpha != 0 ? 1 : 0);
		}
	}

	template <class Callback>
	void iterate (Callback& callback) const
	{
		if (topAlpha != 0)       callback (totalLeft, totalTop, totalRight - totalLeft, 1, topAlpha);
		if (bottomAlpha != 0)    callback (totalLeft, bottom,   totalRight - totalLeft, 1, bottomAlpha);
		if (leftAlpha != 0)      callback (totalLeft, totalTop, 1, totalBottom - totalTop, leftAlpha);
		if (rightAlpha != 0)     callback (right,     totalTop, 1, totalBottom - totalTop, rightAlpha);

		callback (left, top, right - left, bottom - top, 255);
	}

	inline bool isOnePixelWide() const noexcept            { return right - left == 1 && leftAlpha + rightAlpha == 0; }

	inline int getTopLeftCornerAlpha() const noexcept      { return (topAlpha * leftAlpha) >> 8; }
	inline int getTopRightCornerAlpha() const noexcept     { return (topAlpha * rightAlpha) >> 8; }
	inline int getBottomLeftCornerAlpha() const noexcept   { return (bottomAlpha * leftAlpha) >> 8; }
	inline int getBottomRightCornerAlpha() const noexcept  { return (bottomAlpha * rightAlpha) >> 8; }

	int left, top, right, bottom;  // bounds of the solid central area, excluding anti-aliased edges
	int totalTop, totalLeft, totalBottom, totalRight; // bounds of the total area, including edges
	int topAlpha, leftAlpha, bottomAlpha, rightAlpha; // alpha of each anti-aliased edge
};

}

#endif   // __JUCE_RENDERINGHELPERS_JUCEHEADER__

/*** End of inlined file: juce_RenderingHelpers.h ***/


#endif

/**
	A lowest-common-denominator implementation of LowLevelGraphicsContext that does all
	its rendering in memory.

	User code is not supposed to create instances of this class directly - do all your
	rendering via the Graphics class instead.
*/
class JUCE_API  LowLevelGraphicsSoftwareRenderer    : public LowLevelGraphicsContext
{
public:

	LowLevelGraphicsSoftwareRenderer (const Image& imageToRenderOn);
	LowLevelGraphicsSoftwareRenderer (const Image& imageToRenderOn, const Point<int>& origin, const RectangleList& initialClip);
	~LowLevelGraphicsSoftwareRenderer();

	bool isVectorDevice() const;
	void setOrigin (int x, int y);
	void addTransform (const AffineTransform&);
	float getScaleFactor();
	bool clipToRectangle (const Rectangle<int>&);
	bool clipToRectangleList (const RectangleList&);
	void excludeClipRectangle (const Rectangle<int>&);
	void clipToPath (const Path&, const AffineTransform&);
	void clipToImageAlpha (const Image&, const AffineTransform&);
	bool clipRegionIntersects (const Rectangle<int>&);
	Rectangle<int> getClipBounds() const;
	bool isClipEmpty() const;

	void saveState();
	void restoreState();

	void beginTransparencyLayer (float opacity);
	void endTransparencyLayer();

	void setFill (const FillType&);
	void setOpacity (float opacity);
	void setInterpolationQuality (Graphics::ResamplingQuality);

	void fillRect (const Rectangle<int>&, bool replaceExistingContents);
	void fillPath (const Path&, const AffineTransform&);

	void drawImage (const Image&, const AffineTransform&);

	void drawLine (const Line <float>& line);

	void drawVerticalLine (int x, float top, float bottom);
	void drawHorizontalLine (int x, float top, float bottom);

	void setFont (const Font&);
	const Font& getFont();
	void drawGlyph (int glyphNumber, float x, float y);
	void drawGlyph (int glyphNumber, const AffineTransform&);

   #ifndef DOXYGEN
	class SavedState;
   #endif

protected:
	RenderingHelpers::SavedStateStack<SavedState> savedState;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowLevelGraphicsSoftwareRenderer);
};

#endif   // __JUCE_LOWLEVELGRAPHICSSOFTWARERENDERER_JUCEHEADER__

/*** End of inlined file: juce_LowLevelGraphicsSoftwareRenderer.h ***/


#endif
#ifndef __JUCE_IMAGE_JUCEHEADER__

#endif
#ifndef __JUCE_IMAGECACHE_JUCEHEADER__

/*** Start of inlined file: juce_ImageCache.h ***/
#ifndef __JUCE_IMAGECACHE_JUCEHEADER__
#define __JUCE_IMAGECACHE_JUCEHEADER__

/**
	A global cache of images that have been loaded from files or memory.

	If you're loading an image and may need to use the image in more than one
	place, this is used to allow the same image to be shared rather than loading
	multiple copies into memory.

	Another advantage is that after images are released, they will be kept in
	memory for a few seconds before it is actually deleted, so if you're repeatedly
	loading/deleting the same image, it'll reduce the chances of having to reload it
	each time.

	@see Image, ImageFileFormat
*/
class JUCE_API  ImageCache
{
public:

	/** Loads an image from a file, (or just returns the image if it's already cached).

		If the cache already contains an image that was loaded from this file,
		that image will be returned. Otherwise, this method will try to load the
		file, add it to the cache, and return it.

		Remember that the image returned is shared, so drawing into it might
		affect other things that are using it! If you want to draw on it, first
		call Image::duplicateIfShared()

		@param file     the file to try to load
		@returns        the image, or null if it there was an error loading it
		@see getFromMemory, getFromCache, ImageFileFormat::loadFrom
	*/
	static Image getFromFile (const File& file);

	/** Loads an image from an in-memory image file, (or just returns the image if it's already cached).

		If the cache already contains an image that was loaded from this block of memory,
		that image will be returned. Otherwise, this method will try to load the
		file, add it to the cache, and return it.

		Remember that the image returned is shared, so drawing into it might
		affect other things that are using it! If you want to draw on it, first
		call Image::duplicateIfShared()

		@param imageData    the block of memory containing the image data
		@param dataSize     the data size in bytes
		@returns            the image, or an invalid image if it there was an error loading it
		@see getFromMemory, getFromCache, ImageFileFormat::loadFrom
	*/
	static Image getFromMemory (const void* imageData, int dataSize);

	/** Checks the cache for an image with a particular hashcode.

		If there's an image in the cache with this hashcode, it will be returned,
		otherwise it will return an invalid image.

		@param hashCode the hash code that was associated with the image by addImageToCache()
		@see addImageToCache
	*/
	static Image getFromHashCode (int64 hashCode);

	/** Adds an image to the cache with a user-defined hash-code.

		The image passed-in will be referenced (not copied) by the cache, so it's probably
		a good idea not to draw into it after adding it, otherwise this will affect all
		instances of it that may be in use.

		@param image    the image to add
		@param hashCode the hash-code to associate with it
		@see getFromHashCode
	*/
	static void addImageToCache (const Image& image, int64 hashCode);

	/** Changes the amount of time before an unused image will be removed from the cache.
		By default this is about 5 seconds.
	*/
	static void setCacheTimeout (int millisecs);

private:

	class Pimpl;
	friend class Pimpl;

	ImageCache();
	~ImageCache();

	JUCE_DECLARE_NON_COPYABLE (ImageCache);
};

#endif   // __JUCE_IMAGECACHE_JUCEHEADER__

/*** End of inlined file: juce_ImageCache.h ***/


#endif
#ifndef __JUCE_IMAGECONVOLUTIONKERNEL_JUCEHEADER__

/*** Start of inlined file: juce_ImageConvolutionKernel.h ***/
#ifndef __JUCE_IMAGECONVOLUTIONKERNEL_JUCEHEADER__
#define __JUCE_IMAGECONVOLUTIONKERNEL_JUCEHEADER__

/**
	Represents a filter kernel to use in convoluting an image.

	@see Image::applyConvolution
*/
class JUCE_API  ImageConvolutionKernel
{
public:

	/** Creates an empty convulution kernel.

		@param size     the length of each dimension of the kernel, so e.g. if the size
						is 5, it will create a 5x5 kernel
	*/
	ImageConvolutionKernel (int size);

	/** Destructor. */
	~ImageConvolutionKernel();

	/** Resets all values in the kernel to zero. */
	void clear();

	/** Returns one of the kernel values. */
	float getKernelValue (int x, int y) const noexcept;

	/** Sets the value of a specific cell in the kernel.

		The x and y parameters must be in the range 0 < x < getKernelSize().

		@see setOverallSum
	*/
	void setKernelValue (int x, int y, float value) noexcept;

	/** Rescales all values in the kernel to make the total add up to a fixed value.

		This will multiply all values in the kernel by (desiredTotalSum / currentTotalSum).
	*/
	void setOverallSum (float desiredTotalSum);

	/** Multiplies all values in the kernel by a value. */
	void rescaleAllValues (float multiplier);

	/** Intialises the kernel for a gaussian blur.

		@param blurRadius   this may be larger or smaller than the kernel's actual
							size but this will obviously be wasteful or clip at the
							edges. Ideally the kernel should be just larger than
							(blurRadius * 2).
	*/
	void createGaussianBlur (float blurRadius);

	/** Returns the size of the kernel.

		E.g. if it's a 3x3 kernel, this returns 3.
	*/
	int getKernelSize() const               { return size; }

	/** Applies the kernel to an image.

		@param destImage        the image that will receive the resultant convoluted pixels.
		@param sourceImage      the source image to read from - this can be the same image as
								the destination, but if different, it must be exactly the same
								size and format.
		@param destinationArea  the region of the image to apply the filter to
	*/
	void applyToImage (Image& destImage,
					   const Image& sourceImage,
					   const Rectangle<int>& destinationArea) const;

private:

	HeapBlock <float> values;
	const int size;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageConvolutionKernel);
};

#endif   // __JUCE_IMAGECONVOLUTIONKERNEL_JUCEHEADER__

/*** End of inlined file: juce_ImageConvolutionKernel.h ***/


#endif
#ifndef __JUCE_IMAGEFILEFORMAT_JUCEHEADER__

/*** Start of inlined file: juce_ImageFileFormat.h ***/
#ifndef __JUCE_IMAGEFILEFORMAT_JUCEHEADER__
#define __JUCE_IMAGEFILEFORMAT_JUCEHEADER__

/**
	Base-class for codecs that can read and write image file formats such
	as PNG, JPEG, etc.

	This class also contains static methods to make it easy to load images
	from files, streams or from memory.

	@see Image, ImageCache
*/
class JUCE_API  ImageFileFormat
{
protected:

	/** Creates an ImageFormat. */
	ImageFileFormat()                   {}

public:
	/** Destructor. */
	virtual ~ImageFileFormat()          {}

	/** Returns a description of this file format.

		E.g. "JPEG", "PNG"
	*/
	virtual String getFormatName() = 0;

	/** Returns true if the given stream seems to contain data that this format
		understands.

		The format class should only read the first few bytes of the stream and sniff
		for header bytes that it understands.

		@see decodeImage
	*/
	virtual bool canUnderstand (InputStream& input) = 0;

	/** Tries to decode and return an image from the given stream.

		This will be called for an image format after calling its canUnderStand() method
		to see if it can handle the stream.

		@param input    the stream to read the data from. The stream will be positioned
						at the start of the image data (but this may not necessarily
						be position 0)
		@returns        the image that was decoded, or an invalid image if it fails.
		@see loadFrom
	*/
	virtual Image decodeImage (InputStream& input) = 0;

	/** Attempts to write an image to a stream.

		To specify extra information like encoding quality, there will be appropriate parameters
		in the subclasses of the specific file types.

		@returns        true if it nothing went wrong.
	*/
	virtual bool writeImageToStream (const Image& sourceImage,
									 OutputStream& destStream) = 0;

	/** Tries the built-in decoders to see if it can find one to read this stream.

		There are currently built-in decoders for PNG, JPEG and GIF formats.

		The object that is returned should not be deleted by the caller.

		@see canUnderstand, decodeImage, loadFrom
	*/
	static ImageFileFormat* findImageFormatForStream (InputStream& input);

	/** Tries to load an image from a stream.

		This will use the findImageFormatForStream() method to locate a suitable
		codec, and use that to load the image.

		@returns        the image that was decoded, or an invalid image if it fails.
	*/
	static Image loadFrom (InputStream& input);

	/** Tries to load an image from a file.

		This will use the findImageFormatForStream() method to locate a suitable
		codec, and use that to load the image.

		@returns        the image that was decoded, or an invalid image if it fails.
	*/
	static Image loadFrom (const File& file);

	/** Tries to load an image from a block of raw image data.

		This will use the findImageFormatForStream() method to locate a suitable
		codec, and use that to load the image.

		@returns        the image that was decoded, or an invalid image if it fails.
	*/
	static Image loadFrom (const void* rawData,
						   size_t numBytesOfData);

};

/**
	A subclass of ImageFileFormat for reading and writing PNG files.

	@see ImageFileFormat, JPEGImageFormat
*/
class JUCE_API  PNGImageFormat  : public ImageFileFormat
{
public:

	PNGImageFormat();
	~PNGImageFormat();

	String getFormatName();
	bool canUnderstand (InputStream& input);
	Image decodeImage (InputStream& input);
	bool writeImageToStream (const Image& sourceImage, OutputStream& destStream);
};

/**
	A subclass of ImageFileFormat for reading and writing JPEG files.

	@see ImageFileFormat, PNGImageFormat
*/
class JUCE_API  JPEGImageFormat  : public ImageFileFormat
{
public:

	JPEGImageFormat();
	~JPEGImageFormat();

	/** Specifies the quality to be used when writing a JPEG file.

		@param newQuality  a value 0 to 1.0, where 0 is low quality, 1.0 is best, or
						   any negative value is "default" quality
	*/
	void setQuality (float newQuality);

	String getFormatName();
	bool canUnderstand (InputStream& input);
	Image decodeImage (InputStream& input);
	bool writeImageToStream (const Image& sourceImage, OutputStream& destStream);

private:
	float quality;
};

/**
	A subclass of ImageFileFormat for reading GIF files.

	@see ImageFileFormat, PNGImageFormat, JPEGImageFormat
*/
class JUCE_API  GIFImageFormat  : public ImageFileFormat
{
public:

	GIFImageFormat();
	~GIFImageFormat();

	String getFormatName();
	bool canUnderstand (InputStream& input);
	Image decodeImage (InputStream& input);
	bool writeImageToStream (const Image& sourceImage, OutputStream& destStream);
};

#endif   // __JUCE_IMAGEFILEFORMAT_JUCEHEADER__

/*** End of inlined file: juce_ImageFileFormat.h ***/


#endif
#ifndef __JUCE_ATTRIBUTEDSTRING_JUCEHEADER__

/*** Start of inlined file: juce_AttributedString.h ***/
#ifndef __JUCE_ATTRIBUTEDSTRING_JUCEHEADER__
#define __JUCE_ATTRIBUTEDSTRING_JUCEHEADER__

/**
	A text string with a set of colour/font settings that are associated with sub-ranges
	of the text.

	An attributed string lets you create a string with varied fonts, colours, word-wrapping,
	layout, etc., and draw it using AttributedString::draw().

	@see TextLayout
*/
class JUCE_API  AttributedString
{
public:
	/** Creates an empty attributed string. */
	AttributedString();

	/** Creates an attributed string with the given text. */
	explicit AttributedString (const String& text);

	AttributedString (const AttributedString&);
	AttributedString& operator= (const AttributedString&);
   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	AttributedString (AttributedString&&) noexcept;
	AttributedString& operator= (AttributedString&&) noexcept;
   #endif

	/** Destructor. */
	~AttributedString();

	/** Returns the complete text of this attributed string. */
	const String& getText() const noexcept                  { return text; }

	/** Replaces all the text.
		This will change the text, but won't affect any of the colour or font attributes
		that have been added.
	*/
	void setText (const String& newText);

	/** Appends some text (with a default font and colour). */
	void append (const String& textToAppend);
	/** Appends some text, with a specified font, and the default colour (black). */
	void append (const String& textToAppend, const Font& font);
	/** Appends some text, with a specified colour, and the default font. */
	void append (const String& textToAppend, const Colour& colour);
	/** Appends some text, with a specified font and colour. */
	void append (const String& textToAppend, const Font& font, const Colour& colour);

	/** Resets the string, clearing all text and attributes.
		Note that this won't affect global settings like the justification type,
		word-wrap mode, etc.
	*/
	void clear();

	/** Draws this string within the given area.
		The layout of the string within the rectangle is controlled by the justification
		value passed to setJustification().
	*/
	void draw (Graphics& g, const Rectangle<float>& area) const;

	/** Returns the justification that should be used for laying-out the text.
		This may include both vertical and horizontal flags.
	*/
	Justification getJustification() const noexcept         { return justification; }

	/** Sets the justification that should be used for laying-out the text.
		This may include both vertical and horizontal flags.
	*/
	void setJustification (const Justification& newJustification) noexcept;

	/** Types of word-wrap behaviour.
		@see getWordWrap, setWordWrap
	*/
	enum WordWrap
	{
		none,   /**< No word-wrapping: lines extend indefinitely. */
		byWord, /**< Lines are wrapped on a word boundary. */
		byChar, /**< Lines are wrapped on a character boundary. */
	};

	/** Returns the word-wrapping behaviour. */
	WordWrap getWordWrap() const noexcept                   { return wordWrap; }

	/** Sets the word-wrapping behaviour. */
	void setWordWrap (WordWrap newWordWrap) noexcept;

	/** Types of reading direction that can be used.
		@see getReadingDirection, setReadingDirection
	*/
	enum ReadingDirection
	{
		natural,
		leftToRight,
		rightToLeft,
	};

	/** Returns the reading direction for the text. */
	ReadingDirection getReadingDirection() const noexcept   { return readingDirection; }

	/** Sets the reading direction that should be used for the text. */
	void setReadingDirection (ReadingDirection newReadingDirection) noexcept;

	/** Returns the extra line-spacing distance. */
	float getLineSpacing() const noexcept                   { return lineSpacing; }

	/** Sets an extra line-spacing distance. */
	void setLineSpacing (float newLineSpacing) noexcept;

	/** An attribute that has been applied to a range of characters in an AttributedString. */
	class JUCE_API  Attribute
	{
	public:
		/** Creates an attribute that changes the colour for a range of characters.
			@see AttributedString::setColour()
		*/
		Attribute (const Range<int>& range, const Colour& colour);

		/** Creates an attribute that changes the font for a range of characters.
			@see AttributedString::setFont()
		*/
		Attribute (const Range<int>& range, const Font& font);

		Attribute (const Attribute&);
		~Attribute();

		/** If this attribute specifies a font, this returns it; otherwise it returns nullptr. */
		const Font* getFont() const noexcept            { return font; }

		/** If this attribute specifies a colour, this returns it; otherwise it returns nullptr. */
		const Colour* getColour() const noexcept        { return colour; }

		/** The range of characters to which this attribute will be applied. */
		const Range<int> range;

	private:
		ScopedPointer<Font> font;
		ScopedPointer<Colour> colour;

		Attribute& operator= (const Attribute&);

		JUCE_LEAK_DETECTOR (Attribute);
	};

	/** Returns the number of attributes that have been added to this string. */
	int getNumAttributes() const noexcept                       { return attributes.size(); }

	/** Returns one of the string's attributes.
		The index provided must be less than getNumAttributes(), and >= 0.
	*/
	const Attribute* getAttribute (int index) const noexcept    { return attributes.getUnchecked (index); }

	/** Adds a colour attribute for the specified range. */
	void setColour (const Range<int>& range, const Colour& colour);

	/** Removes all existing colour attributes, and applies this colour to the whole string. */
	void setColour (const Colour& colour);

	/** Adds a font attribute for the specified range. */
	void setFont (const Range<int>& range, const Font& font);

	/** Removes all existing font attributes, and applies this font to the whole string. */
	void setFont (const Font& font);

private:
	String text;
	float lineSpacing;
	Justification justification;
	WordWrap wordWrap;
	ReadingDirection readingDirection;
	OwnedArray<Attribute> attributes;

	JUCE_LEAK_DETECTOR (AttributedString);
};

#endif   // __JUCE_ATTRIBUTEDSTRING_JUCEHEADER__

/*** End of inlined file: juce_AttributedString.h ***/


#endif
#ifndef __JUCE_CUSTOMTYPEFACE_JUCEHEADER__

/*** Start of inlined file: juce_CustomTypeface.h ***/
#ifndef __JUCE_CUSTOMTYPEFACE_JUCEHEADER__
#define __JUCE_CUSTOMTYPEFACE_JUCEHEADER__

class InputStream;
class OutputStream;

/**
	A typeface that can be populated with custom glyphs.

	You can create a CustomTypeface if you need one that contains your own glyphs,
	or if you need to load a typeface from a Juce-formatted binary stream.

	If you want to create a copy of a native face, you can use addGlyphsFromOtherTypeface()
	to copy glyphs into this face.

	@see Typeface, Font
*/
class JUCE_API  CustomTypeface  : public Typeface
{
public:

	/** Creates a new, empty typeface. */
	CustomTypeface();

	/** Loads a typeface from a previously saved stream.
		The stream must have been created by writeToStream().
		@see writeToStream
	*/
	explicit CustomTypeface (InputStream& serialisedTypefaceStream);

	/** Destructor. */
	~CustomTypeface();

	/** Resets this typeface, deleting all its glyphs and settings. */
	void clear();

	/** Sets the vital statistics for the typeface.
		@param fontFamily the typeface's font family
		@param ascent     the ascent - this is normalised to a height of 1.0 and this is
						  the value that will be returned by Typeface::getAscent(). The
						  descent is assumed to be (1.0 - ascent)
		@param isBold     should be true if the typeface is bold
		@param isItalic   should be true if the typeface is italic
		@param defaultCharacter   the character to be used as a replacement if there's
						  no glyph available for the character that's being drawn
	*/
	void setCharacteristics (const String& fontFamily, float ascent,
							 bool isBold, bool isItalic,
							 juce_wchar defaultCharacter) noexcept;

	/** Sets the vital statistics for the typeface.
		@param fontFamily the typeface's font family
		@param fontStyle  the typeface's font style
		@param ascent     the ascent - this is normalised to a height of 1.0 and this is
						  the value that will be returned by Typeface::getAscent(). The
						  descent is assumed to be (1.0 - ascent)
		@param defaultCharacter  the character to be used as a replacement if there's
						  no glyph available for the character that's being drawn
	*/
	void setCharacteristics (const String& fontFamily, const String& fontStyle,
							 float ascent, juce_wchar defaultCharacter) noexcept;

	/** Adds a glyph to the typeface.

		The path that is passed in is normalised so that the font height is 1.0, and its
		origin is the anchor point of the character on its baseline.

		The width is the nominal width of the character, and any extra kerning values that
		are specified will be added to this width.
	*/
	void addGlyph (juce_wchar character, const Path& path, float width) noexcept;

	/** Specifies an extra kerning amount to be used between a pair of characters.
		The amount will be added to the nominal width of the first character when laying out a string.
	*/
	void addKerningPair (juce_wchar char1, juce_wchar char2, float extraAmount) noexcept;

	/** Adds a range of glyphs from another typeface.
		This will attempt to pull in the paths and kerning information from another typeface and
		add it to this one.
	*/
	void addGlyphsFromOtherTypeface (Typeface& typefaceToCopy, juce_wchar characterStartIndex, int numCharacters) noexcept;

	/** Saves this typeface as a Juce-formatted font file.
		A CustomTypeface can be created to reload the data that is written - see the CustomTypeface
		constructor.
	*/
	bool writeToStream (OutputStream& outputStream);

	// The following methods implement the basic Typeface behaviour.
	float getAscent() const;
	float getDescent() const;
	float getStringWidth (const String& text);
	void getGlyphPositions (const String& text, Array <int>& glyphs, Array<float>& xOffsets);
	bool getOutlineForGlyph (int glyphNumber, Path& path);
	EdgeTable* getEdgeTableForGlyph (int glyphNumber, const AffineTransform& transform);

protected:

	juce_wchar defaultCharacter;
	float ascent;

	/** If a subclass overrides this, it can load glyphs into the font on-demand.
		When methods such as getGlyphPositions() or getOutlineForGlyph() are asked for a
		particular character and there's no corresponding glyph, they'll call this
		method so that a subclass can try to add that glyph, returning true if it
		manages to do so.
	*/
	virtual bool loadGlyphIfPossible (juce_wchar characterNeeded);

private:

	class GlyphInfo;
	friend class OwnedArray<GlyphInfo>;
	OwnedArray <GlyphInfo> glyphs;
	short lookupTable [128];

	GlyphInfo* findGlyph (const juce_wchar character, bool loadIfNeeded) noexcept;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomTypeface);
};

#endif   // __JUCE_CUSTOMTYPEFACE_JUCEHEADER__

/*** End of inlined file: juce_CustomTypeface.h ***/


#endif
#ifndef __JUCE_FONT_JUCEHEADER__

#endif
#ifndef __JUCE_GLYPHARRANGEMENT_JUCEHEADER__

/*** Start of inlined file: juce_GlyphArrangement.h ***/
#ifndef __JUCE_GLYPHARRANGEMENT_JUCEHEADER__
#define __JUCE_GLYPHARRANGEMENT_JUCEHEADER__

/**
	A glyph from a particular font, with a particular size, style,
	typeface and position.

	You should rarely need to use this class directly - for most purposes, the
	GlyphArrangement class will do what you need for text layout.

	@see GlyphArrangement, Font
*/
class JUCE_API  PositionedGlyph
{
public:

	PositionedGlyph (const Font& font, juce_wchar character, int glyphNumber,
					 float anchorX, float baselineY, float width, bool isWhitespace);

	PositionedGlyph (const PositionedGlyph& other);
	PositionedGlyph& operator= (const PositionedGlyph& other);
	~PositionedGlyph();

	/** Returns the character the glyph represents. */
	juce_wchar getCharacter() const noexcept    { return character; }
	/** Checks whether the glyph is actually empty. */
	bool isWhitespace() const noexcept          { return whitespace; }

	/** Returns the position of the glyph's left-hand edge. */
	float getLeft() const noexcept              { return x; }
	/** Returns the position of the glyph's right-hand edge. */
	float getRight() const noexcept             { return x + w; }
	/** Returns the y position of the glyph's baseline. */
	float getBaselineY() const noexcept         { return y; }
	/** Returns the y position of the top of the glyph. */
	float getTop() const                        { return y - font.getAscent(); }
	/** Returns the y position of the bottom of the glyph. */
	float getBottom() const                     { return y + font.getDescent(); }
	/** Returns the bounds of the glyph. */
	Rectangle<float> getBounds() const          { return Rectangle<float> (x, getTop(), w, font.getHeight()); }

	/** Shifts the glyph's position by a relative amount. */
	void moveBy (float deltaX, float deltaY);

	/** Draws the glyph into a graphics context. */
	void draw (const Graphics& g) const;

	/** Draws the glyph into a graphics context, with an extra transform applied to it. */
	void draw (const Graphics& g, const AffineTransform& transform) const;

	/** Returns the path for this glyph.

		@param path     the glyph's outline will be appended to this path
	*/
	void createPath (Path& path) const;

	/** Checks to see if a point lies within this glyph. */
	bool hitTest (float x, float y) const;

private:

	friend class GlyphArrangement;
	Font font;
	juce_wchar character;
	int glyph;
	float x, y, w;
	bool whitespace;

	JUCE_LEAK_DETECTOR (PositionedGlyph);
};

/**
	A set of glyphs, each with a position.

	You can create a GlyphArrangement, text to it and then draw it onto a
	graphics context. It's used internally by the text methods in the
	Graphics class, but can be used directly if more control is needed.

	@see Font, PositionedGlyph
*/
class JUCE_API  GlyphArrangement
{
public:

	/** Creates an empty arrangement. */
	GlyphArrangement();

	/** Takes a copy of another arrangement. */
	GlyphArrangement (const GlyphArrangement& other);

	/** Copies another arrangement onto this one.
		To add another arrangement without clearing this one, use addGlyphArrangement().
	*/
	GlyphArrangement& operator= (const GlyphArrangement& other);

	/** Destructor. */
	~GlyphArrangement();

	/** Returns the total number of glyphs in the arrangement. */
	int getNumGlyphs() const noexcept                           { return glyphs.size(); }

	/** Returns one of the glyphs from the arrangement.

		@param index    the glyph's index, from 0 to (getNumGlyphs() - 1). Be
						careful not to pass an out-of-range index here, as it
						doesn't do any bounds-checking.
	*/
	PositionedGlyph& getGlyph (int index) const;

	/** Clears all text from the arrangement and resets it.
	*/
	void clear();

	/** Appends a line of text to the arrangement.

		This will add the text as a single line, where x is the left-hand edge of the
		first character, and y is the position for the text's baseline.

		If the text contains new-lines or carriage-returns, this will ignore them - use
		addJustifiedText() to add multi-line arrangements.
	*/
	void addLineOfText (const Font& font,
						const String& text,
						float x, float y);

	/** Adds a line of text, truncating it if it's wider than a specified size.

		This is the same as addLineOfText(), but if the line's width exceeds the value
		specified in maxWidthPixels, it will be truncated using either ellipsis (i.e. dots: "..."),
		if useEllipsis is true, or if this is false, it will just drop any subsequent characters.
	*/
	void addCurtailedLineOfText (const Font& font,
								 const String& text,
								 float x, float y,
								 float maxWidthPixels,
								 bool useEllipsis);

	/** Adds some multi-line text, breaking lines at word-boundaries if they are too wide.

		This will add text to the arrangement, breaking it into new lines either where there
		is a new-line or carriage-return character in the text, or where a line's width
		exceeds the value set in maxLineWidth.

		Each line that is added will be laid out using the flags set in horizontalLayout, so
		the lines can be left- or right-justified, or centred horizontally in the space
		between x and (x + maxLineWidth).

		The y co-ordinate is the position of the baseline of the first line of text - subsequent
		lines will be placed below it, separated by a distance of font.getHeight().
	*/
	void addJustifiedText (const Font& font,
						   const String& text,
						   float x, float y,
						   float maxLineWidth,
						   const Justification& horizontalLayout);

	/** Tries to fit some text withing a given space.

		This does its best to make the given text readable within the specified rectangle,
		so it useful for labelling things.

		If the text is too big, it'll be squashed horizontally or broken over multiple lines
		if the maximumLinesToUse value allows this. If the text just won't fit into the space,
		it'll cram as much as possible in there, and put some ellipsis at the end to show that
		it's been truncated.

		A Justification parameter lets you specify how the text is laid out within the rectangle,
		both horizontally and vertically.

		@see Graphics::drawFittedText
	*/
	void addFittedText (const Font& font,
						const String& text,
						float x, float y, float width, float height,
						const Justification& layout,
						int maximumLinesToUse,
						float minimumHorizontalScale = 0.7f);

	/** Appends another glyph arrangement to this one. */
	void addGlyphArrangement (const GlyphArrangement& other);

	/** Appends a custom glyph to the arrangement. */
	void addGlyph (const PositionedGlyph& glyph);

	/** Draws this glyph arrangement to a graphics context.

		This uses cached bitmaps so is much faster than the draw (Graphics&, const AffineTransform&)
		method, which renders the glyphs as filled vectors.
	*/
	void draw (const Graphics& g) const;

	/** Draws this glyph arrangement to a graphics context.

		This renders the paths as filled vectors, so is far slower than the draw (Graphics&)
		method for non-transformed arrangements.
	*/
	void draw (const Graphics& g, const AffineTransform& transform) const;

	/** Converts the set of glyphs into a path.

		@param path     the glyphs' outlines will be appended to this path
	*/
	void createPath (Path& path) const;

	/** Looks for a glyph that contains the given co-ordinate.

		@returns the index of the glyph, or -1 if none were found.
	*/
	int findGlyphIndexAt (float x, float y) const;

	/** Finds the smallest rectangle that will enclose a subset of the glyphs.

		@param startIndex               the first glyph to test
		@param numGlyphs                the number of glyphs to include; if this is < 0, all glyphs after
										startIndex will be included
		@param includeWhitespace        if true, the extent of any whitespace characters will also
										be taken into account
	*/
	Rectangle<float> getBoundingBox (int startIndex, int numGlyphs, bool includeWhitespace) const;

	/** Shifts a set of glyphs by a given amount.

		@param startIndex   the first glyph to transform
		@param numGlyphs    the number of glyphs to move; if this is < 0, all glyphs after
							startIndex will be used
		@param deltaX       the amount to add to their x-positions
		@param deltaY       the amount to add to their y-positions
	*/
	void moveRangeOfGlyphs (int startIndex, int numGlyphs,
							float deltaX, float deltaY);

	/** Removes a set of glyphs from the arrangement.

		@param startIndex   the first glyph to remove
		@param numGlyphs    the number of glyphs to remove; if this is < 0, all glyphs after
							startIndex will be deleted
	*/
	void removeRangeOfGlyphs (int startIndex, int numGlyphs);

	/** Expands or compresses a set of glyphs horizontally.

		@param startIndex               the first glyph to transform
		@param numGlyphs                the number of glyphs to stretch; if this is < 0, all glyphs after
										startIndex will be used
		@param horizontalScaleFactor    how much to scale their horizontal width by
	*/
	void stretchRangeOfGlyphs (int startIndex, int numGlyphs,
							   float horizontalScaleFactor);

	/** Justifies a set of glyphs within a given space.

		This moves the glyphs as a block so that the whole thing is located within the
		given rectangle with the specified layout.

		If the Justification::horizontallyJustified flag is specified, each line will
		be stretched out to fill the specified width.
	*/
	void justifyGlyphs (int startIndex, int numGlyphs,
						float x, float y, float width, float height,
						const Justification& justification);

private:

	OwnedArray <PositionedGlyph> glyphs;

	int insertEllipsis (const Font&, float maxXPos, int startIndex, int endIndex);
	int fitLineIntoSpace (int start, int numGlyphs, float x, float y, float w, float h, const Font&,
						  const Justification&, float minimumHorizontalScale);
	void spreadOutLine (int start, int numGlyphs, float targetWidth);

	JUCE_LEAK_DETECTOR (GlyphArrangement);
};

#endif   // __JUCE_GLYPHARRANGEMENT_JUCEHEADER__

/*** End of inlined file: juce_GlyphArrangement.h ***/


#endif
#ifndef __JUCE_TEXTLAYOUT_JUCEHEADER__

/*** Start of inlined file: juce_TextLayout.h ***/
#ifndef __JUCE_TEXTLAYOUT_JUCEHEADER__
#define __JUCE_TEXTLAYOUT_JUCEHEADER__

class Graphics;

/**
	A Pre-formatted piece of text, which may contain multiple fonts and colours.

	A TextLayout is created from an AttributedString, and once created can be
	quickly drawn into a Graphics context.

	@see AttributedString
*/
class JUCE_API  TextLayout
{
public:
	/** Creates an empty layout.
		Having created a TextLayout, you can populate it using createLayout() or
		createLayoutWithBalancedLineLengths().
	*/
	TextLayout();
	TextLayout (const TextLayout&);
	TextLayout& operator= (const TextLayout&);
   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	TextLayout (TextLayout&& other) noexcept;
	TextLayout& operator= (TextLayout&&) noexcept;
   #endif

	/** Destructor. */
	~TextLayout();

	/** Creates a layout from the given attributed string.
		This will replace any data that is currently stored in the layout.
	*/
	void createLayout (const AttributedString& text, float maxWidth);

	/** Creates a layout, attempting to choose a width which results in lines
		of a similar length.

		This will be slower than the normal createLayout method, but produces a
		tidier result.
	*/
	void createLayoutWithBalancedLineLengths (const AttributedString& text, float maxWidth);

	/** Draws the layout within the specified area.
		The position of the text within the rectangle is controlled by the justification
		flags set in the original AttributedString that was used to create this layout.
	*/
	void draw (Graphics& g, const Rectangle<float>& area) const;

	/** A positioned glyph. */
	class JUCE_API  Glyph
	{
	public:
		Glyph (int glyphCode, const Point<float>& anchor, float width) noexcept;
		Glyph (const Glyph&) noexcept;
		Glyph& operator= (const Glyph&) noexcept;
		~Glyph() noexcept;

		/** The code number of this glyph. */
		int glyphCode;

		/** The glyph's anchor point - this is relative to the line's origin.
			@see TextLayout::Line::lineOrigin
		*/
		Point<float> anchor;

		float width;

	private:
		JUCE_LEAK_DETECTOR (Glyph);
	};

	/** A sequence of glyphs with a common font and colour. */
	class JUCE_API  Run
	{
	public:
		Run() noexcept;
		Run (const Run&);
		Run (const Range<int>& stringRange, int numGlyphsToPreallocate);
		~Run() noexcept;

		Font font;              /**< The run's font. */
		Colour colour;          /**< The run's colour. */
		Array<Glyph> glyphs;    /**< The glyphs in this run. */
		Range<int> stringRange; /**< The character range that this run represents in the
									 original string that was used to create it. */
	private:
		Run& operator= (const Run&);
		JUCE_LEAK_DETECTOR (Run);
	};

	/** A line containing a sequence of glyph-runs. */
	class JUCE_API  Line
	{
	public:
		Line() noexcept;
		Line (const Line&);
		Line (const Range<int>& stringRange, const Point<float>& lineOrigin,
			  float ascent, float descent, float leading, int numRunsToPreallocate);
		~Line() noexcept;

		/** Returns the X position range which contains all the glyphs in this line. */
		Range<float> getLineBoundsX() const noexcept;

		OwnedArray<Run> runs;           /**< The glyph-runs in this line. */
		Range<int> stringRange;         /**< The character range that this line represents in the
											 original string that was used to create it. */
		Point<float> lineOrigin;        /**< The line's baseline origin. */
		float ascent, descent, leading;

	private:
		Line& operator= (const Line&);
		JUCE_LEAK_DETECTOR (Line);
	};

	/** Returns the maximum width of the content. */
	float getWidth() const noexcept     { return width; }

	/** Returns the maximum height of the content. */
	float getHeight() const noexcept;

	/** Returns the number of lines in the layout. */
	int getNumLines() const noexcept    { return lines.size(); }

	/** Returns one of the lines. */
	Line& getLine (int index) const;

	/** Adds a line to the layout. The layout will take ownership of this line object
		and will delete it when it is no longer needed. */
	void addLine (Line* line);

	/** Pre-allocates space for the specified number of lines. */
	void ensureStorageAllocated (int numLinesNeeded);

private:
	OwnedArray<Line> lines;
	float width;
	Justification justification;

	void createStandardLayout (const AttributedString&);
	bool createNativeLayout (const AttributedString&);
	void recalculateWidth();

	JUCE_LEAK_DETECTOR (TextLayout);
};

#endif   // __JUCE_TEXTLAYOUT_JUCEHEADER__

/*** End of inlined file: juce_TextLayout.h ***/


#endif
#ifndef __JUCE_TYPEFACE_JUCEHEADER__

#endif
#ifndef __JUCE_DROPSHADOWEFFECT_JUCEHEADER__

/*** Start of inlined file: juce_DropShadowEffect.h ***/
#ifndef __JUCE_DROPSHADOWEFFECT_JUCEHEADER__
#define __JUCE_DROPSHADOWEFFECT_JUCEHEADER__


/*** Start of inlined file: juce_ImageEffectFilter.h ***/
#ifndef __JUCE_IMAGEEFFECTFILTER_JUCEHEADER__
#define __JUCE_IMAGEEFFECTFILTER_JUCEHEADER__

/**
	A graphical effect filter that can be applied to components.

	An ImageEffectFilter can be applied to the image that a component
	paints before it hits the screen.

	This is used for adding effects like shadows, blurs, etc.

	@see Component::setComponentEffect
*/
class JUCE_API  ImageEffectFilter
{
public:

	/** Overridden to render the effect.

		The implementation of this method must use the image that is passed in
		as its source, and should render its output to the graphics context passed in.

		@param sourceImage      the image that the source component has just rendered with
								its paint() method. The image may or may not have an alpha
								channel, depending on whether the component is opaque.
		@param destContext      the graphics context to use to draw the resultant image.
		@param alpha            the alpha with which to draw the resultant image to the
								target context
	*/
	virtual void applyEffect (Image& sourceImage,
							  Graphics& destContext,
							  float alpha) = 0;

	/** Destructor. */
	virtual ~ImageEffectFilter() {}

};

#endif   // __JUCE_IMAGEEFFECTFILTER_JUCEHEADER__

/*** End of inlined file: juce_ImageEffectFilter.h ***/

/**
	An effect filter that adds a drop-shadow behind the image's content.

	(This will only work on images/components that aren't opaque, of course).

	When added to a component, this effect will draw a soft-edged
	shadow based on what gets drawn inside it. The shadow will also
	be applied to the component's children.

	For speed, this doesn't use a proper gaussian blur, but cheats by
	using a simple bilinear filter. If you need a really high-quality
	shadow, check out ImageConvolutionKernel::createGaussianBlur()

	@see Component::setComponentEffect
*/
class JUCE_API  DropShadowEffect  : public ImageEffectFilter
{
public:

	/** Creates a default drop-shadow effect.

		To customise the shadow's appearance, use the setShadowProperties()
		method.
	*/
	DropShadowEffect();

	/** Destructor. */
	~DropShadowEffect();

	/** Sets up parameters affecting the shadow's appearance.

		@param newRadius        the (approximate) radius of the blur used
		@param newOpacity       the opacity with which the shadow is rendered
		@param newShadowOffsetX allows the shadow to be shifted in relation to the
								component's contents
		@param newShadowOffsetY allows the shadow to be shifted in relation to the
								component's contents
	*/
	void setShadowProperties (float newRadius,
							  float newOpacity,
							  int newShadowOffsetX,
							  int newShadowOffsetY);

	/** @internal */
	void applyEffect (Image& sourceImage, Graphics& destContext, float alpha);

private:

	int offsetX, offsetY;
	float radius, opacity;

	JUCE_LEAK_DETECTOR (DropShadowEffect);
};

#endif   // __JUCE_DROPSHADOWEFFECT_JUCEHEADER__

/*** End of inlined file: juce_DropShadowEffect.h ***/


#endif
#ifndef __JUCE_GLOWEFFECT_JUCEHEADER__

/*** Start of inlined file: juce_GlowEffect.h ***/
#ifndef __JUCE_GLOWEFFECT_JUCEHEADER__
#define __JUCE_GLOWEFFECT_JUCEHEADER__

/**
	A component effect that adds a coloured blur around the component's contents.

	(This will only work on non-opaque components).

	@see Component::setComponentEffect, DropShadowEffect
*/
class JUCE_API  GlowEffect  : public ImageEffectFilter
{
public:

	/** Creates a default 'glow' effect.

		To customise its appearance, use the setGlowProperties() method.
	*/
	GlowEffect();

	/** Destructor. */
	~GlowEffect();

	/** Sets the glow's radius and colour.

		The radius is how large the blur should be, and the colour is
		used to render it (for a less intense glow, lower the colour's
		opacity).
	*/
	void setGlowProperties (float newRadius,
							const Colour& newColour);

	/** @internal */
	void applyEffect (Image& sourceImage, Graphics& destContext, float alpha);

private:

	float radius;
	Colour colour;

	JUCE_LEAK_DETECTOR (GlowEffect);
};

#endif   // __JUCE_GLOWEFFECT_JUCEHEADER__

/*** End of inlined file: juce_GlowEffect.h ***/


#endif
#ifndef __JUCE_IMAGEEFFECTFILTER_JUCEHEADER__

#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_GRAPHICS_JUCEHEADER__

/*** End of inlined file: juce_graphics.h ***/

