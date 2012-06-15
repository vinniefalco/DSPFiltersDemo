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


/*** Start of inlined file: juce_opengl.h ***/
#ifndef __JUCE_OPENGL_JUCEHEADER__
#define __JUCE_OPENGL_JUCEHEADER__

#include "juce_gui_extra_amalgam.h"

#undef JUCE_OPENGL
#define JUCE_OPENGL 1

#if JUCE_IOS || JUCE_ANDROID
 #define JUCE_OPENGL_ES 1
 #define JUCE_USE_OPENGL_FIXED_FUNCTION 0
#endif

#if JUCE_WINDOWS
 #ifndef APIENTRY
  #define APIENTRY __stdcall
  #define CLEAR_TEMP_APIENTRY 1
 #endif
 #ifndef WINGDIAPI
  #define WINGDIAPI __declspec(dllimport)
  #define CLEAR_TEMP_WINGDIAPI 1
 #endif
 #include <gl/GL.h>
 #ifdef CLEAR_TEMP_WINGDIAPI
  #undef WINGDIAPI
  #undef CLEAR_TEMP_WINGDIAPI
 #endif
 #ifdef CLEAR_TEMP_APIENTRY
  #undef APIENTRY
  #undef CLEAR_TEMP_APIENTRY
 #endif
#elif JUCE_LINUX
 #include <GL/gl.h>
 #undef KeyPress
#elif JUCE_IOS
 #include <OpenGLES/ES2/gl.h>
#elif JUCE_MAC
 #include <OpenGL/gl.h>
 #include "OpenGL/glext.h"
#elif JUCE_ANDROID
 #include <GLES2/gl2.h>
#endif

#if ! defined (JUCE_USE_OPENGL_SHADERS)
 #define JUCE_USE_OPENGL_SHADERS 1
#endif

#ifndef JUCE_USE_OPENGL_FIXED_FUNCTION
 #define JUCE_USE_OPENGL_FIXED_FUNCTION 1
#endif

namespace juce
{


/*** Start of inlined file: juce_OpenGLHelpers.h ***/
#ifndef __JUCE_OPENGLHELPERS_JUCEHEADER__
#define __JUCE_OPENGLHELPERS_JUCEHEADER__

class OpenGLTexture;
class OpenGLFrameBuffer;

/**
	A set of miscellaneous openGL helper functions.
*/
class JUCE_API  OpenGLHelpers
{
public:
	/** Clears the GL error state. */
	static void resetErrorState();

	/** Returns true if the current thread has an active OpenGL context. */
	static bool isContextActive();

	/** Clears the current context using the given colour. */
	static void clear (const Colour& colour);

	static void enableScissorTest (const Rectangle<int>& clip);

	/** Checks whether the current context supports the specified extension. */
	static bool isExtensionSupported (const char* extensionName);

	/** Returns the address of a named GL extension function */
	static void* getExtensionFunction (const char* functionName);

   #if JUCE_USE_OPENGL_FIXED_FUNCTION
	/** Sets the current colour using a JUCE colour. */
	static void setColour (const Colour& colour);

	/** Gives the current context an orthoganal rendering mode for 2D drawing into the given size. */
	static void prepareFor2D (int width, int height);

	/** This does the same job as gluPerspective(). */
	static void setPerspective (double fovy, double aspect, double zNear, double zFar);

	static void applyTransform (const AffineTransform& t);

	static void applyMatrix (const float matrixValues[16]);
   #if ! JUCE_OPENGL_ES
	static void applyMatrix (const double matrixValues[16]);
   #endif

	/** Draws a 2D quad with the specified corner points. */
	static void drawQuad2D (float x1, float y1,
							float x2, float y2,
							float x3, float y3,
							float x4, float y4,
							const Colour& colour);

	/** Draws a 3D quad with the specified corner points. */
	static void drawQuad3D (float x1, float y1, float z1,
							float x2, float y2, float z2,
							float x3, float y3, float z3,
							float x4, float y4, float z4,
							const Colour& colour);
	static void drawTriangleStrip (const GLfloat* const vertices, const GLfloat* const textureCoords, const int numVertices) noexcept;

	static void drawTriangleStrip (const GLfloat* const vertices, const GLfloat* const textureCoords,
								   const int numVertices, const GLuint textureID) noexcept;

	static void drawTextureQuad (GLuint textureID, const Rectangle<int>& rect);

	static void fillRectWithTexture (const Rectangle<int>& rect, GLuint textureID, const float alpha);

	/** Fills a rectangle with the specified colour. */
	static void fillRectWithColour (const Rectangle<int>& rect,
									const Colour& colour);

	static void fillRect (const Rectangle<int>& rect);
   #endif
};

/**
	Used as a local object while rendering, this will create a temporary texture ID
	from an image in the quickest way possible.

	If the image is backed by an OpenGL framebuffer, it will use that directly; otherwise,
	this object will create a temporary texture or framebuffer and copy the image.
*/
class JUCE_API  OpenGLTextureFromImage
{
public:
	OpenGLTextureFromImage (const Image& image);
	~OpenGLTextureFromImage();

	GLuint textureID;
	const int imageWidth, imageHeight;
	float fullWidthProportion, fullHeightProportion;

private:
	ScopedPointer<OpenGLTexture> texture;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLTextureFromImage);
};

#endif   // __JUCE_OPENGLHELPERS_JUCEHEADER__

/*** End of inlined file: juce_OpenGLHelpers.h ***/

// START_AUTOINCLUDE opengl
#ifndef __JUCE_DRAGGABLE3DORIENTATION_JUCEHEADER__

/*** Start of inlined file: juce_Draggable3DOrientation.h ***/
#ifndef __JUCE_DRAGGABLE3DORIENTATION_JUCEHEADER__
#define __JUCE_DRAGGABLE3DORIENTATION_JUCEHEADER__


/*** Start of inlined file: juce_Quaternion.h ***/
#ifndef __JUCE_QUATERNION_JUCEHEADER__
#define __JUCE_QUATERNION_JUCEHEADER__


/*** Start of inlined file: juce_Matrix3D.h ***/
#ifndef __JUCE_MATRIX3D_JUCEHEADER__
#define __JUCE_MATRIX3D_JUCEHEADER__

/**
	A 4x4 transformation matrix.

	@see AffineTransform
*/
template <typename Type>
class Matrix3D
{
public:
	Matrix3D() noexcept
	{
		zeromem (mat, sizeof (mat));
		mat[0] = mat[1 + 1 * 4] = mat[2 + 2 * 4] = mat[3 + 3 * 4] = (Type) 1;
	}

	Matrix3D (const Matrix3D& other) noexcept
	{
		memcpy (mat, other.mat, sizeof (mat));
	}

	Matrix3D (const AffineTransform& transform) noexcept
	{
		mat[0]  = transform.mat00;  mat[1] =  transform.mat10;  mat[2]  = 0;         mat[3]  = 0;
		mat[4]  = transform.mat01;  mat[5] =  transform.mat11;  mat[6]  = 0;         mat[7]  = 0;
		mat[8]  = 0;                mat[9] =  0;                mat[10] = (Type) 1;  mat[11] = 0;
		mat[12] = transform.mat02;  mat[13] = transform.mat12;  mat[14] = 0;         mat[15] = (Type) 1;
	}

	Matrix3D (const Type* values) noexcept
	{
		memcpy (mat, values, sizeof (mat));
	}

	Matrix3D (const Type& m00, const Type& m10, const Type& m20, const Type& m30,
			  const Type& m01, const Type& m11, const Type& m21, const Type& m31,
			  const Type& m02, const Type& m12, const Type& m22, const Type& m32,
			  const Type& m03, const Type& m13, const Type& m23, const Type& m33) noexcept
	{
		mat[0]  = m00;  mat[1]  = m10;  mat[2]  = m20;  mat[3]  = m30;
		mat[4]  = m01;  mat[5]  = m11;  mat[6]  = m21;  mat[7]  = m31;
		mat[8]  = m02;  mat[9]  = m12;  mat[10] = m22;  mat[11] = m32;
		mat[12] = m03;  mat[13] = m13;  mat[14] = m23;  mat[15] = m33;
	}

	Matrix3D& operator= (const Matrix3D& other) noexcept
	{
		memcpy (mat, other.mat, sizeof (mat));
		return *this;
	}

   #if JUCE_USE_OPENGL_FIXED_FUNCTION
	/** Multiplies the active OpenGL context's matrix by this one. */
	void applyToOpenGL() const noexcept
	{
		OpenGLHelpers::applyMatrix (mat);
	}
   #endif

	/** The 4x4 matrix values. These are stored in the standard OpenGL order. */
	Type mat[16];
};

#endif   // __JUCE_MATRIX3D_JUCEHEADER__

/*** End of inlined file: juce_Matrix3D.h ***/


/*** Start of inlined file: juce_Vector3D.h ***/
#ifndef __JUCE_VECTOR3D_JUCEHEADER__
#define __JUCE_VECTOR3D_JUCEHEADER__

/**
	A three-coordinate vector.
*/
template <typename Type>
class Vector3D
{
public:
	Vector3D() noexcept  : x(), y(), z() {}
	Vector3D (const Type& xValue, const Type& yValue, const Type& zValue) noexcept  : x (xValue), y (yValue), z (zValue) {}
	Vector3D (const Vector3D& other) noexcept   : x (other.x), y (other.y), z (other.z) {}
	Vector3D& operator= (const Vector3D& other) noexcept     { x = other.x;  y = other.y;  z = other.z;  return *this; }

	/** Returns a vector that lies along the X axis. */
	static Vector3D xAxis() noexcept        { return Vector3D ((Type) 1, 0, 0); }
	/** Returns a vector that lies along the Y axis. */
	static Vector3D yAxis() noexcept        { return Vector3D (0, (Type) 1, 0); }
	/** Returns a vector that lies along the Z axis. */
	static Vector3D zAxis() noexcept        { return Vector3D (0, 0, (Type) 1); }

	Vector3D& operator+= (const Vector3D& other) noexcept        { x += other.x;  y += other.y;  z += other.z;  return *this; }
	Vector3D& operator-= (const Vector3D& other) noexcept        { x -= other.x;  y -= other.y;  z -= other.z;  return *this; }
	Vector3D& operator*= (const Type& scaleFactor) noexcept      { x *= scaleFactor;  y *= scaleFactor;  z *= scaleFactor;  return *this; }
	Vector3D& operator/= (const Type& scaleFactor) noexcept      { x /= scaleFactor;  y /= scaleFactor;  z /= scaleFactor;  return *this; }

	Vector3D operator+ (const Vector3D& other) const noexcept    { return Vector3D (x + other.x, y + other.y, z + other.z); }
	Vector3D operator- (const Vector3D& other) const noexcept    { return Vector3D (x - other.x, y - other.y, z - other.z); }
	Vector3D operator* (const Type& scaleFactor) const noexcept  { return Vector3D (x * scaleFactor, y * scaleFactor, z * scaleFactor); }
	Vector3D operator/ (const Type& scaleFactor) const noexcept  { return Vector3D (x / scaleFactor, y / scaleFactor, z / scaleFactor); }
	Vector3D operator-() const noexcept                          { return Vector3D (-x, -y, -z); }

	/** Returns the dot-product of these two vectors. */
	Type operator* (const Vector3D& other) const noexcept        { return x * other.x + y * other.y + z * other.z; }

	/** Returns the cross-product of these two vectors. */
	Vector3D operator^ (const Vector3D& other) const noexcept    { return Vector3D (y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }

	Type length() const noexcept            { return std::sqrt (lengthSquared()); }
	Type lengthSquared() const noexcept     { return x * x + y * y + z * z; }

	Vector3D normalised() const noexcept    { return *this / length(); }

	/** Returns true if the vector is practically equal to the origin. */
	bool lengthIsBelowEpsilon() const noexcept
	{
		const Type epsilon (std::numeric_limits<Type>::epsilon());
		return ! (x < -epsilon || x > epsilon || y < -epsilon || y > epsilon || z < -epsilon || z > epsilon);
	}

	Type x, y, z;
};

#endif   // __JUCE_VECTOR3D_JUCEHEADER__

/*** End of inlined file: juce_Vector3D.h ***/

/**
	Holds a quaternion (a 3D vector and a scalar value).
*/
template <typename Type>
class Quaternion
{
public:
	Quaternion() noexcept  : scalar() {}
	Quaternion (const Quaternion& other) noexcept                                     : vector (other.vector), scalar (other.scalar) {}
	Quaternion (const Vector3D<Type>& vector_, const Type& scalar_) noexcept          : vector (vector_), scalar (scalar_) {}
	Quaternion (const Type& x, const Type& y, const Type& z, const Type& w) noexcept  : vector (x, y, z), scalar (w) {}

	/** Creates a quaternion from an angle and an axis. */
	static Quaternion fromAngle (const Type& angle, const Vector3D<Type>& axis) noexcept
	{
		return Quaternion (axis.normalised() * std::sin (angle / (Type) 2), std::cos (angle / (Type) 2));
	}

	Quaternion& operator= (const Quaternion& other) noexcept
	{
		vector = other.vector;
		scalar = other.scalar;
		return *this;
	}

	Quaternion& operator*= (const Quaternion& other) noexcept
	{
		const Type oldScalar (scalar);
		scalar = (scalar * other.scalar) - (vector * other.vector);
		vector = (other.vector * oldScalar) + (vector * other.scalar) + (vector ^ other.vector);
		return *this;
	}

	Type length() const noexcept        { return std::sqrt (normal()); }
	Type normal() const noexcept        { return scalar * scalar + vector.lengthSquared(); }

	Quaternion normalised() const noexcept
	{
		const Type len (length());
		jassert (len > 0);
		return Quaternion (vector / len, scalar / len);
	}

	/** Returns the matrix that will perform the rotation specified by this quaternion. */
	Matrix3D<Type> getRotationMatrix() const noexcept
	{
		const Type norm (normal());
		const Type s (norm > 0 ? ((Type) 2) / norm : 0);
		const Type xs (s * vector.x),  ys (s * vector.y),  zs (s * vector.z);
		const Type wx (xs * scalar),   wy (ys * scalar),   wz (zs * scalar);
		const Type xx (xs * vector.x), xy (ys * vector.x), xz (zs * vector.x);
		const Type yy (ys * vector.y), yz (zs * vector.y), zz (zs * vector.z);

		return Matrix3D<Type> (((Type) 1) - (yy + zz), xy - wz, xz + wy, 0,
							   xy + wz, ((Type) 1) - (xx+ zz),  yz - wx, 0,
							   xz - wy, yz + wx, ((Type) 1) - (xx + yy), 0,
							   0, 0, 0, (Type) 1);
	}

	/** The vector part of the quaternion. */
	Vector3D<Type> vector;

	/** The scalar part of the quaternion. */
	Type scalar;
};

#endif   // __JUCE_QUATERNION_JUCEHEADER__

/*** End of inlined file: juce_Quaternion.h ***/

/**
	Stores a 3D orientation, which can be rotated by dragging with the mouse.
*/
class Draggable3DOrientation
{
public:
	typedef Vector3D<GLfloat> VectorType;

	/** Creates a Draggable3DOrientation, initially set up to be aligned along the X axis. */
	Draggable3DOrientation (float objectRadius = 0.5f) noexcept
		: radius (jmax (0.1f, objectRadius)),
		  quaternion (VectorType::xAxis(), 0)
	{
	}

	/** Resets the orientation, specifying the axis to align it along. */
	void reset (const VectorType& axis) noexcept
	{
		quaternion = QuaternionType (axis, 0);
	}

	/** Sets the viewport area within which mouse-drag positions will occur.
		You'll need to set this rectangle before calling mouseDown. The centre of the
		rectangle is assumed to be the centre of the object that will be rotated, and
		the size of the rectangle will be used to scale the object radius - see setRadius().
	*/
	void setViewport (const Rectangle<int>& newArea) noexcept
	{
		area = newArea;
	}

	/** Sets the size of the rotated object, as a proportion of the viewport's size.
		@see setViewport
	*/
	void setRadius (float newRadius) noexcept
	{
		radius = jmax (0.1f, newRadius);
	}

	/** Begins a mouse-drag operation.
		You must call this before any calls to mouseDrag(). The position that is supplied
		will be treated as being relative to the centre of the rectangle passed to setViewport().
	*/
	template <typename Type>
	void mouseDown (const Point<Type>& mousePos) noexcept
	{
		lastMouse = mousePosToProportion (mousePos.toFloat());
	}

	/** Continues a mouse-drag operation.
		After calling mouseDown() to begin a drag sequence, you can call this method
		to continue it.
	*/
	template <typename Type>
	void mouseDrag (const Point<Type>& mousePos) noexcept
	{
		const VectorType oldPos (projectOnSphere (lastMouse));
		lastMouse = mousePosToProportion (mousePos.toFloat());
		const VectorType newPos (projectOnSphere (lastMouse));

		quaternion *= rotationFromMove (oldPos, newPos);
	}

	/** Returns the matrix that should be used to apply the current orientation.
		@see applyToOpenGLMatrix
	*/
	Matrix3D<GLfloat> getRotationMatrix() const noexcept
	{
		return quaternion.getRotationMatrix();
	}

   #if JUCE_USE_OPENGL_FIXED_FUNCTION
	/** Applies this rotation to the active OpenGL context's matrix. */
	void applyToOpenGLMatrix() const noexcept
	{
		getRotationMatrix().applyToOpenGL();
	}
   #endif

private:
	typedef Quaternion<GLfloat> QuaternionType;
	Rectangle<int> area;
	float radius;
	QuaternionType quaternion;
	Point<float> lastMouse;

	Point<float> mousePosToProportion (const Point<float>& mousePos) const noexcept
	{
		const int scale = (jmin (area.getWidth(), area.getHeight()) / 2);

		// You must call setViewport() to give this object a valid window size before
		// calling any of the mouse input methods!
		jassert (scale > 0);

		return Point<float> ((mousePos.x - area.getCentreX()) / scale,
							 (area.getCentreY() - mousePos.y) / scale);
	}

	VectorType projectOnSphere (const Point<float>& pos) const noexcept
	{
		const GLfloat radiusSquared = radius * radius;
		const GLfloat xySquared = pos.x * pos.x + pos.y * pos.y;

		return VectorType (pos.x, pos.y,
						   xySquared < radiusSquared * 0.5f ? std::sqrt (radiusSquared - xySquared)
															: (radiusSquared / (2.0f * std::sqrt (xySquared))));
	}

	QuaternionType rotationFromMove (const VectorType& from, const VectorType& to) const noexcept
	{
		VectorType rotationAxis (to ^ from);

		if (rotationAxis.lengthIsBelowEpsilon())
			rotationAxis = VectorType::xAxis();

		const GLfloat d = jlimit (-1.0f, 1.0f, (from - to).length() / (2.0f * radius));

		return QuaternionType::fromAngle (2.0f * std::asin (d), rotationAxis);
	}
};

#endif   // __JUCE_DRAGGABLE3DORIENTATION_JUCEHEADER__

/*** End of inlined file: juce_Draggable3DOrientation.h ***/


#endif
#ifndef __JUCE_MATRIX3D_JUCEHEADER__

#endif
#ifndef __JUCE_OPENGLCONTEXT_JUCEHEADER__

/*** Start of inlined file: juce_OpenGLContext.h ***/
#ifndef __JUCE_OPENGLCONTEXT_JUCEHEADER__
#define __JUCE_OPENGLCONTEXT_JUCEHEADER__


/*** Start of inlined file: juce_OpenGLPixelFormat.h ***/
#ifndef __JUCE_OPENGLPIXELFORMAT_JUCEHEADER__
#define __JUCE_OPENGLPIXELFORMAT_JUCEHEADER__

/**
	Represents the various properties of an OpenGL pixel format.

	@see OpenGLContext::setPixelFormat
*/
class JUCE_API  OpenGLPixelFormat
{
public:

	/** Creates an OpenGLPixelFormat.

		The default constructor just initialises the object as a simple 8-bit
		RGBA format.
	*/
	OpenGLPixelFormat (int bitsPerRGBComponent = 8,
					   int alphaBits = 8,
					   int depthBufferBits = 16,
					   int stencilBufferBits = 0) noexcept;

	bool operator== (const OpenGLPixelFormat&) const noexcept;
	bool operator!= (const OpenGLPixelFormat&) const noexcept;

	int redBits;          /**< The number of bits per pixel to use for the red channel. */
	int greenBits;        /**< The number of bits per pixel to use for the green channel. */
	int blueBits;         /**< The number of bits per pixel to use for the blue channel. */
	int alphaBits;        /**< The number of bits per pixel to use for the alpha channel. */

	int depthBufferBits;      /**< The number of bits per pixel to use for a depth buffer. */
	int stencilBufferBits;    /**< The number of bits per pixel to use for a stencil buffer. */

	int accumulationBufferRedBits;    /**< The number of bits per pixel to use for an accumulation buffer's red channel. */
	int accumulationBufferGreenBits;  /**< The number of bits per pixel to use for an accumulation buffer's green channel. */
	int accumulationBufferBlueBits;   /**< The number of bits per pixel to use for an accumulation buffer's blue channel. */
	int accumulationBufferAlphaBits;  /**< The number of bits per pixel to use for an accumulation buffer's alpha channel. */

	uint8 multisamplingLevel;         /**< The number of samples to use for full-scene multisampled anti-aliasing (if available). */
};

#endif   // __JUCE_OPENGLPIXELFORMAT_JUCEHEADER__

/*** End of inlined file: juce_OpenGLPixelFormat.h ***/


/*** Start of inlined file: juce_OpenGLExtensions.h ***/
#ifndef __JUCE_OPENGLEXTENSIONS_JUCEHEADER__
#define __JUCE_OPENGLEXTENSIONS_JUCEHEADER__

#if JUCE_MAC && (JUCE_PPC || ((! defined (MAC_OS_X_VERSION_10_6)) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6))
 #define JUCE_EXT(func) func ## EXT
#else
 #define JUCE_EXT(func) func
#endif

/** @internal This macro contains a list of GL extension functions that need to be dynamically loaded on Windows/Linux.
	@see OpenGLExtensionFunctions
*/
#define JUCE_GL_BASIC_EXTENSION_FUNCTIONS(USE_FUNCTION, EXT_FUNCTION) \
	USE_FUNCTION (glActiveTexture,          void, (GLenum p1), (p1))\
	USE_FUNCTION (glBindBuffer,             void, (GLenum p1, GLuint p2), (p1, p2))\
	USE_FUNCTION (glDeleteBuffers,          void, (GLsizei p1, const GLuint* p2), (p1, p2))\
	USE_FUNCTION (glGenBuffers,             void, (GLsizei p1, GLuint* p2), (p1, p2))\
	USE_FUNCTION (glBufferData,             void, (GLenum p1, GLsizeiptr p2, const GLvoid* p3, GLenum p4), (p1, p2, p3, p4))\
	USE_FUNCTION (glBufferSubData,          void, (GLenum p1, GLintptr p2, GLsizeiptr p3, const GLvoid* p4), (p1, p2, p3, p4))\
	EXT_FUNCTION (glIsRenderbuffer,         GLboolean, (GLuint p1), (p1))\
	EXT_FUNCTION (glBindRenderbuffer,       void, (GLenum p1, GLuint p2), (p1, p2))\
	EXT_FUNCTION (glDeleteRenderbuffers,    void, (GLsizei p1, const GLuint* p2), (p1, p2))\
	EXT_FUNCTION (glGenRenderbuffers,       void, (GLsizei p1, GLuint* p2), (p1, p2))\
	EXT_FUNCTION (glRenderbufferStorage,    void, (GLenum p1, GLenum p2, GLsizei p3, GLsizei p4), (p1, p2, p3, p4))\
	EXT_FUNCTION (glGetRenderbufferParameteriv,  void, (GLenum p1, GLenum p2, GLint* p3), (p1, p2, p3))\
	EXT_FUNCTION (glIsFramebuffer,          GLboolean, (GLuint p1), (p1))\
	EXT_FUNCTION (glBindFramebuffer,        void, (GLenum p1, GLuint p2), (p1, p2))\
	EXT_FUNCTION (glDeleteFramebuffers,     void, (GLsizei p1, const GLuint* p2), (p1, p2))\
	EXT_FUNCTION (glGenFramebuffers,        void, (GLsizei p1, GLuint* p2), (p1, p2))\
	EXT_FUNCTION (glCheckFramebufferStatus, GLenum, (GLenum p1), (p1))\
	EXT_FUNCTION (glFramebufferTexture2D,   void, (GLenum p1, GLenum p2, GLenum p3, GLuint p4, GLint p5), (p1, p2, p3, p4, p5))\
	EXT_FUNCTION (glFramebufferRenderbuffer,  void, (GLenum p1, GLenum p2, GLenum p3, GLuint p4), (p1, p2, p3, p4))\
	EXT_FUNCTION (glGetFramebufferAttachmentParameteriv, void, (GLenum p1, GLenum p2, GLenum p3, GLint* p4), (p1, p2, p3, p4))

#if JUCE_USE_OPENGL_SHADERS
 #define JUCE_GL_EXTENSION_FUNCTIONS1(USE_FUNCTION, EXT_FUNCTION) \
	JUCE_GL_BASIC_EXTENSION_FUNCTIONS(USE_FUNCTION, EXT_FUNCTION) \
	USE_FUNCTION (glCreateProgram,          GLuint, (), ())\
	USE_FUNCTION (glDeleteProgram,          void, (GLuint p1), (p1))\
	USE_FUNCTION (glCreateShader,           GLuint, (GLenum p1), (p1))\
	USE_FUNCTION (glDeleteShader,           void, (GLuint p1), (p1))\
	USE_FUNCTION (glShaderSource,           void, (GLuint p1, GLsizei p2, const GLchar** p3, const GLint* p4), (p1, p2, p3, p4))\
	USE_FUNCTION (glCompileShader,          void, (GLuint p1), (p1))\
	USE_FUNCTION (glAttachShader,           void, (GLuint p1, GLuint p2), (p1, p2))\
	USE_FUNCTION (glLinkProgram,            void, (GLuint p1), (p1))\
	USE_FUNCTION (glUseProgram,             void, (GLuint p1), (p1))\
	USE_FUNCTION (glGetShaderiv,            void, (GLuint p1, GLenum p2, GLint* p3), (p1, p2, p3))\
	USE_FUNCTION (glGetShaderInfoLog,       void, (GLuint p1, GLsizei p2, GLsizei* p3, GLchar* p4), (p1, p2, p3, p4))\
	USE_FUNCTION (glGetProgramInfoLog,      void, (GLuint p1, GLsizei p2, GLsizei* p3, GLchar* p4), (p1, p2, p3, p4))\
	USE_FUNCTION (glGetProgramiv,           void, (GLuint p1, GLenum p2, GLint* p3), (p1, p2, p3))\
	USE_FUNCTION (glGetUniformLocation,     GLint, (GLuint p1, const GLchar* p2), (p1, p2))\
	USE_FUNCTION (glGetAttribLocation,      GLint, (GLuint p1, const GLchar* p2), (p1, p2))\
	USE_FUNCTION (glVertexAttribPointer,    void, (GLuint p1, GLint p2, GLenum p3, GLboolean p4, GLsizei p5, const GLvoid* p6), (p1, p2, p3, p4, p5, p6))\
	USE_FUNCTION (glEnableVertexAttribArray,  void, (GLuint p1), (p1))\
	USE_FUNCTION (glDisableVertexAttribArray, void, (GLuint p1), (p1))\
	USE_FUNCTION (glUniform1f,              void, (GLint p1, GLfloat p2), (p1, p2))\
	USE_FUNCTION (glUniform1i,              void, (GLint p1, GLint p2), (p1, p2))\
	USE_FUNCTION (glUniform2f,              void, (GLint p1, GLfloat p2, GLfloat p3), (p1, p2, p3))\
	USE_FUNCTION (glUniform3f,              void, (GLint p1, GLfloat p2, GLfloat p3, GLfloat p4), (p1, p2, p3, p4))\
	USE_FUNCTION (glUniform4f,              void, (GLint p1, GLfloat p2, GLfloat p3, GLfloat p4, GLfloat p5), (p1, p2, p3, p4, p5))\
	USE_FUNCTION (glUniform4i,              void, (GLint p1, GLint p2, GLint p3, GLint p4, GLint p5), (p1, p2, p3, p4, p5))\
	USE_FUNCTION (glUniform1fv,             void, (GLint p1, GLsizei p2, const GLfloat* p3), (p1, p2, p3))
#else
 #define JUCE_GL_EXTENSION_FUNCTIONS1(USE_FUNCTION) JUCE_GL_BASIC_EXTENSION_FUNCTIONS(USE_FUNCTION, EXT_FUNCTION)
#endif

#if JUCE_USE_OPENGL_FIXED_FUNCTION
 #define JUCE_GL_EXTENSION_FUNCTIONS(USE_FUNCTION, EXT_FUNCTION) JUCE_GL_EXTENSION_FUNCTIONS1(USE_FUNCTION, EXT_FUNCTION) \
	USE_FUNCTION (glClientActiveTexture,    void, (GLenum p1), (p1))
#else
 #define JUCE_GL_EXTENSION_FUNCTIONS(USE_FUNCTION, EXT_FUNCTION) JUCE_GL_EXTENSION_FUNCTIONS1(USE_FUNCTION, EXT_FUNCTION)
#endif

/** This class contains a generated list of OpenGL extension functions, which are either dynamically loaded
	for a specific GL context, or simply call-through to the appropriate OS function where available.
*/
struct OpenGLExtensionFunctions
{
	void initialise();

   #if JUCE_WINDOWS && ! DOXYGEN
	typedef char GLchar;
	typedef pointer_sized_int GLsizeiptr;
	typedef pointer_sized_int GLintptr;
   #endif

   #if JUCE_WINDOWS || JUCE_LINUX
	#if JUCE_WINDOWS
	 #define JUCE_GL_STDCALL __stdcall
	#else
	 #define JUCE_GL_STDCALL
	#endif

	#define JUCE_DECLARE_GL_FUNCTION(name, returnType, params, callparams)      typedef returnType (JUCE_GL_STDCALL *type_ ## name) params; type_ ## name name;
	JUCE_GL_EXTENSION_FUNCTIONS (JUCE_DECLARE_GL_FUNCTION, JUCE_DECLARE_GL_FUNCTION)
   #elif JUCE_OPENGL_ES
	#define JUCE_DECLARE_GL_FUNCTION(name, returnType, params, callparams)      inline static returnType name params;
	JUCE_GL_EXTENSION_FUNCTIONS (JUCE_DECLARE_GL_FUNCTION, JUCE_DECLARE_GL_FUNCTION)
   #else
	#define JUCE_DECLARE_GL_FUNCTION(name, returnType, params, callparams)      inline static returnType name params { return ::name callparams; }
	#define JUCE_DECLARE_GL_FUNCTION_EXT(name, returnType, params, callparams)  inline static returnType name params { return ::name ## EXT callparams; }
	JUCE_GL_EXTENSION_FUNCTIONS (JUCE_DECLARE_GL_FUNCTION, JUCE_DECLARE_GL_FUNCTION_EXT)
	#undef JUCE_DECLARE_GL_FUNCTION_EXT
   #endif

	#undef JUCE_DECLARE_GL_FUNCTION
};

#endif   // __JUCE_OPENGLEXTENSIONS_JUCEHEADER__

/*** End of inlined file: juce_OpenGLExtensions.h ***/


/*** Start of inlined file: juce_OpenGLRenderer.h ***/
#ifndef __JUCE_OPENGLRENDERER_JUCEHEADER__
#define __JUCE_OPENGLRENDERER_JUCEHEADER__

/**
	A base class that should be implemented by classes which want to render openGL
	on a background thread.

	@see OpenGLContext
*/
class JUCE_API  OpenGLRenderer
{
public:
	OpenGLRenderer() {}
	virtual ~OpenGLRenderer() {}

	/** Called when a new GL context has been created.
		You can use this as an opportunity to create your textures, shaders, etc.
		When the method is invoked, the new GL context will be active.
		Note that this callback will be made on a background thread, so make sure
		that your implementation is thread-safe.
	*/
	virtual void newOpenGLContextCreated() = 0;

	/** Called when you should render the next openGL frame.
		Note that this callback will be made on a background thread, so make sure
		that your implementation is thread-safe.
	*/
	virtual void renderOpenGL() = 0;

	/** Called when the current openGL context is about to close.
		You can use this opportunity to release any GL resources that you may have
		created.

		Note that this callback will be made on a background thread, so make sure
		that your implementation is thread-safe.

		(Also note that on Android, this callback won't happen, because there's currently
		no way to implement it..)
	*/
	virtual void openGLContextClosing() = 0;
};

#endif   // __JUCE_OPENGLRENDERER_JUCEHEADER__

/*** End of inlined file: juce_OpenGLRenderer.h ***/

/**
	Creates an OpenGL context, which can be attached to a component.

	To render some OpenGL, you should create an instance of an OpenGLContext,
	and call attachTo() to make it use a component as its render target.

	To provide threaded rendering, you can supply an OpenGLRenderer object that
	will be used to render each frame.

	Before your target component or OpenGLRenderer is deleted, you MUST call
	detach() or delete the OpenGLContext to allow the background thread to
	stop and the native resources to be freed safely.

	@see OpenGLRenderer
*/
class JUCE_API  OpenGLContext
{
public:
	OpenGLContext();

	/** Destructor. */
	~OpenGLContext();

	/** Gives the context an OpenGLRenderer to use to do the drawing.
		The object that you give it will not be owned by the context, so it's the caller's
		responsibility to manage its lifetime and make sure that it doesn't get deleted
		while the context may be using it. To stop the context using a renderer, just call
		this method with a null pointer.
		Note: This must be called BEFORE attaching your context to a target component!
	*/
	void setRenderer (OpenGLRenderer* rendererToUse) noexcept;

	/** Enables or disables the use of the GL context to perform 2D rendering
		of the component to which it is attached.
		If this is false, then only your OpenGLRenderer will be used to perform
		any rendering. If true, then each time your target's paint() method needs
		to be called, an OpenGLGraphicsContext will be used to render it, (after
		calling your OpenGLRenderer if there is one).

		By default this is set to true. If you're not using any paint() method functionality
		and are doing all your rendering in an OpenGLRenderer, you should disable it
		to improve performance.

		Note: This must be called BEFORE attaching your context to a target component!
	*/
	void setComponentPaintingEnabled (bool shouldPaintComponent) noexcept;

	/** Sets the pixel format which you'd like to use for the target GL surface.
		Note: This must be called BEFORE attaching your context to a target component!
	*/
	void setPixelFormat (const OpenGLPixelFormat& preferredPixelFormat) noexcept;

	/** Provides a context with which you'd like this context's resources to be shared.
		The object passed-in here must not be deleted while the context may still be
		using it! To turn off sharing, you can call this method with a null pointer.
		Note: This must be called BEFORE attaching your context to a target component!
	*/
	void setContextToShareWith (const OpenGLContext* contextToShareWith) noexcept;

	/** Attaches the context to a target component.

		If the component is not fully visible, this call will wait until the component
		is shown before actually creating a native context for it.

		When a native context is created, a thread is started, and will be used to call
		the OpenGLRenderer methods. The context will be floated above the target component,
		and when the target moves, it will track it. If the component is hidden/shown, the
		context may be deleted and re-created.
	*/
	void attachTo (Component& component);

	/** Detaches the context from its target component and deletes any native resources.
		If the context has not been attached, this will do nothing. Otherwise, it will block
		until the context and its thread have been cleaned up.
	*/
	void detach();

	/** Returns true if the context is attached to a component and is on-screen.
		Note that if you call attachTo() for a non-visible component, this method will
		return false until the component is made visible.
	*/
	bool isAttached() const noexcept;

	/** Returns the component to which this context is currently attached, or nullptr. */
	Component* getTargetComponent() const noexcept;

	/** Returns the context that's currently in active use by the calling thread, or
		nullptr if no context is active.
	*/
	static OpenGLContext* getCurrentContext();

	/** Asynchronously causes a repaint to be made. */
	void triggerRepaint();

	/** Returns the width of this context */
	inline int getWidth() const noexcept                    { return width; }

	/** Returns the height of this context */
	inline int getHeight() const noexcept                   { return height; }

	/** If this context is backed by a frame buffer, this returns its ID number,
		or 0 if the context does not use a framebuffer.
	*/
	unsigned int getFrameBufferID() const noexcept;

	/** Returns true if shaders can be used in this context. */
	bool areShadersAvailable() const;

	/** This structure holds a set of dynamically loaded GL functions for use on this context. */
	OpenGLExtensionFunctions extensions;

	/** This retrieves an object that was previously stored with setAssociatedObject().
		If no object is found with the given name, this will return nullptr.
		This method must only be called from within the GL rendering methods.
		@see setAssociatedObject
	*/
	ReferenceCountedObject* getAssociatedObject (const char* name) const;

	/** Attaches a named object to the context, which will be deleted when the context is
		destroyed.

		This allows you to store an object which will be released before the context is
		deleted. The main purpose is for caching GL objects such as shader programs, which
		will become invalid when the context is deleted.

		This method must only be called from within the GL rendering methods.
	*/
	void setAssociatedObject (const char* name, ReferenceCountedObject* newObject);

	/** Makes this context the currently active one.
		You should never need to call this in normal use - the context will already be
		active when OpenGLRenderer::renderOpenGL() is invoked.
	*/
	bool makeActive() const noexcept;

	/** Returns true if this context is currently active for the calling thread. */
	bool isActive() const noexcept;

	/** Swaps the buffers (if the context can do this).
		There's normally no need to call this directly - the buffers will be swapped
		automatically after your OpenGLRenderer::renderOpenGL() method has been called.
	*/
	void swapBuffers();

	/** Sets whether the context checks the vertical sync before swapping.

		The value is the number of frames to allow between buffer-swapping. This is
		fairly system-dependent, but 0 turns off syncing, 1 makes it swap on frame-boundaries,
		and greater numbers indicate that it should swap less often.

		Returns true if it sets the value successfully - some platforms won't support
		this setting.
	*/
	bool setSwapInterval (int numFramesPerSwap);

	/** Returns the current swap-sync interval.
		See setSwapInterval() for info about the value returned.
	*/
	int getSwapInterval() const;

	/** Returns an OS-dependent handle to some kind of underlting OS-provided GL context.

		The exact type of the value returned will depend on the OS and may change
		if the implementation changes. If you want to use this, digging around in the
		native code is probably the best way to find out what it is.
	*/
	void* getRawContext() const noexcept;

	/** Draws the currently selected texture into this context at its original size.

		@param targetClipArea  the target area to draw into (in top-left origin coords)
		@param anchorPosAndTextureSize  the position of this rectangle is the texture's top-left
										anchor position in the target space, and the size must be
										the total size of the texture.
		@param contextWidth     the width of the context or framebuffer that is being drawn into,
								used for scaling of the coordinates.
		@param contextHeight    the height of the context or framebuffer that is being drawn into,
								used for vertical flipping of the y coordinates.
	*/
	void copyTexture (const Rectangle<int>& targetClipArea,
					  const Rectangle<int>& anchorPosAndTextureSize,
					  int contextWidth, int contextHeight);

   #ifndef DOXYGEN
	class NativeContext;
   #endif

private:
	class CachedImage;
	class Attachment;
	NativeContext* nativeContext;
	OpenGLRenderer* renderer;
	ScopedPointer<Attachment> attachment;
	OpenGLPixelFormat pixelFormat;
	const OpenGLContext* contextToShareWith;
	int width, height;
	bool renderComponents;

	CachedImage* getCachedImage() const noexcept;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLContext);
};

#endif   // __JUCE_OPENGLCONTEXT_JUCEHEADER__

/*** End of inlined file: juce_OpenGLContext.h ***/


#endif
#ifndef __JUCE_OPENGLFRAMEBUFFER_JUCEHEADER__

/*** Start of inlined file: juce_OpenGLFrameBuffer.h ***/
#ifndef __JUCE_OPENGLFRAMEBUFFER_JUCEHEADER__
#define __JUCE_OPENGLFRAMEBUFFER_JUCEHEADER__

/**
	Creates an openGL frame buffer.
*/
class JUCE_API  OpenGLFrameBuffer
{
public:
	/** Creates an uninitialised buffer.
		To actually allocate the buffer, use initialise().
	*/
	OpenGLFrameBuffer();

	/** Destructor. */
	~OpenGLFrameBuffer();

	/** Tries to allocates a buffer of the given size.
		Note that a valid openGL context must be selected when you call this method,
		or it will fail.
	*/
	bool initialise (OpenGLContext& context, int width, int height);

	/** Tries to allocates a buffer containing a copy of a given image.
		Note that a valid openGL context must be selected when you call this method,
		or it will fail.
	*/
	bool initialise (OpenGLContext& context, const Image& content);

	/** Tries to allocate a copy of another framebuffer.
	*/
	bool initialise (OpenGLFrameBuffer& other);

	/** Releases the buffer, if one has been allocated.
		Any saved state that was created with saveAndRelease() will also be freed by this call.
	*/
	void release();

	/** If the framebuffer is active, this will save a stashed copy of its contents in main memory,
		and will release the GL buffer.
		After saving, the original state can be restored again by calling reloadSavedCopy().
	*/
	void saveAndRelease();

	/** Restores the framebuffer content that was previously saved using saveAndRelease().
		After saving to main memory, the original state can be restored by calling restoreToGPUMemory().
	*/
	bool reloadSavedCopy (OpenGLContext& context);

	/** Returns true if a valid buffer has been allocated. */
	bool isValid() const noexcept                       { return pimpl != nullptr; }

	/** Returns the width of the buffer. */
	int getWidth() const noexcept;

	/** Returns the height of the buffer. */
	int getHeight() const noexcept;

	/** Returns the texture ID number for using this buffer as a texture. */
	GLuint getTextureID() const noexcept;

	/** Selects this buffer as the current OpenGL rendering target. */
	bool makeCurrentRenderingTarget();

	/** Deselects this buffer as the current OpenGL rendering target. */
	void releaseAsRenderingTarget();

	/** Returns the ID of this framebuffer, or 0 if it isn't initialised. */
	GLuint getFrameBufferID() const;

	/** Returns the current frame buffer ID for the current context. */
	static GLuint getCurrentFrameBufferTarget();

	/** Clears the framebuffer with the specified colour. */
	void clear (const Colour& colour);

	/** Selects the framebuffer as the current target, and clears it to transparent. */
	void makeCurrentAndClear();

   #if JUCE_USE_OPENGL_FIXED_FUNCTION
	/** Draws this framebuffer onto the current context, with the specified corner positions. */
	void draw2D (float x1, float y1,
				 float x2, float y2,
				 float x3, float y3,
				 float x4, float y4,
				 const Colour& colour) const;

	/** Draws this framebuffer onto the current context, with the specified corner positions. */
	void draw3D (float x1, float y1, float z1,
				 float x2, float y2, float z2,
				 float x3, float y3, float z3,
				 float x4, float y4, float z4,
				 const Colour& colour) const;

	/** Draws the framebuffer at a given position. */
	void drawAt (float x1, float y1) const;
   #endif

	/** Reads an area of pixels from the framebuffer into a 32-bit ARGB pixel array.
		The lineStride is measured as a number of pixels, not bytes - pass a stride
		of 0 to indicate a packed array.
	*/
	bool readPixels (PixelARGB* targetData, const Rectangle<int>& sourceArea);

	/** Writes an area of pixels into the framebuffer from a specified pixel array.
		The lineStride is measured as a number of pixels, not bytes - pass a stride
		of 0 to indicate a packed array.
	*/
	bool writePixels (const PixelARGB* srcData, const Rectangle<int>& targetArea);

private:
	class Pimpl;
	friend class ScopedPointer<Pimpl>;
	ScopedPointer<Pimpl> pimpl;

	class SavedState;
	friend class ScopedPointer<SavedState>;
	ScopedPointer<SavedState> savedState;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLFrameBuffer);
};

#endif   // __JUCE_OPENGLFRAMEBUFFER_JUCEHEADER__

/*** End of inlined file: juce_OpenGLFrameBuffer.h ***/


#endif
#ifndef __JUCE_OPENGLGRAPHICSCONTEXT_JUCEHEADER__

/*** Start of inlined file: juce_OpenGLGraphicsContext.h ***/
#ifndef __JUCE_OPENGLGRAPHICSCONTEXT_JUCEHEADER__
#define __JUCE_OPENGLGRAPHICSCONTEXT_JUCEHEADER__

/** Creates a graphics context object that will render into the given OpenGL target.
	The caller is responsible for deleting this object when no longer needed.
*/
LowLevelGraphicsContext* createOpenGLGraphicsContext (OpenGLContext& target);

/** Creates a graphics context object that will render into the given OpenGL target.
	The caller is responsible for deleting this object when no longer needed.
*/
LowLevelGraphicsContext* createOpenGLGraphicsContext (OpenGLContext& context,
													  OpenGLFrameBuffer& target);

/** Creates a graphics context object that will render into the given OpenGL target.
	The caller is responsible for deleting this object when no longer needed.
*/
LowLevelGraphicsContext* createOpenGLGraphicsContext (OpenGLContext& context,
													  unsigned int frameBufferID,
													  int width, int height);

#endif   // __JUCE_OPENGLGRAPHICSCONTEXT_JUCEHEADER__

/*** End of inlined file: juce_OpenGLGraphicsContext.h ***/


#endif
#ifndef __JUCE_OPENGLHELPERS_JUCEHEADER__

#endif
#ifndef __JUCE_OPENGLIMAGE_JUCEHEADER__

/*** Start of inlined file: juce_OpenGLImage.h ***/
#ifndef __JUCE_OPENGLIMAGE_JUCEHEADER__
#define __JUCE_OPENGLIMAGE_JUCEHEADER__

/**
	A type of ImagePixelData that stores its image data in an OpenGL
	framebuffer, allowing a JUCE Image object to wrap a framebuffer.

	By creating an Image from an instance of an OpenGLFrameBufferImage,
	you can then use a Graphics object to draw into the framebuffer using normal
	JUCE 2D operations.

	@see Image, ImageType, ImagePixelData, OpenGLFrameBuffer
*/
class JUCE_API  OpenGLImageType     : public ImageType
{
public:
	OpenGLImageType();
	~OpenGLImageType();

	ImagePixelData* create (Image::PixelFormat, int width, int height, bool shouldClearImage) const;
	int getTypeID() const;

	static OpenGLFrameBuffer* getFrameBufferFrom (const Image&);
};

#endif   // __JUCE_OPENGLIMAGE_JUCEHEADER__

/*** End of inlined file: juce_OpenGLImage.h ***/


#endif
#ifndef __JUCE_OPENGLPIXELFORMAT_JUCEHEADER__

#endif
#ifndef __JUCE_OPENGLRENDERER_JUCEHEADER__

#endif
#ifndef __JUCE_OPENGLSHADERPROGRAM_JUCEHEADER__

/*** Start of inlined file: juce_OpenGLShaderProgram.h ***/
#ifndef __JUCE_OPENGLSHADERPROGRAM_JUCEHEADER__
#define __JUCE_OPENGLSHADERPROGRAM_JUCEHEADER__

/**
	Manages an OpenGL shader program.
*/
class JUCE_API  OpenGLShaderProgram
{
public:
	OpenGLShaderProgram (const OpenGLContext& context) noexcept;
	~OpenGLShaderProgram() noexcept;

	/** Returns the version of GLSL that the current context supports.
		E.g.
		@code
		if (OpenGLShaderProgram::getLanguageVersion() > 1.199)
		{
			// ..do something that requires GLSL 1.2 or above..
		}
		@endcode
	*/
	static double getLanguageVersion();

	/** Compiles and adds a shader to this program.

		After adding all your shaders, remember to call link() to link them into
		a usable program.

		If your app is built in debug mode, this method will assert if the program
		fails to compile correctly.

		The shaderType parameter could be GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.

		@returns  true if the shader compiled successfully. If not, you can call
				  getLastError() to find out what happened.
	*/
	bool addShader (const char* const shaderSourceCode, GLenum shaderType);

	/** Links all the compiled shaders into a usable program.
		If your app is built in debug mode, this method will assert if the program
		fails to link correctly.
		@returns  true if the program linked successfully. If not, you can call
				  getLastError() to find out what happened.
	*/
	bool link() noexcept;

	/** Get the output for the last shader compilation or link that failed. */
	const String& getLastError() const noexcept            { return errorLog; }

	/** Selects this program into the current context. */
	void use() const noexcept;

	/** Represents an openGL uniform value.
		After a program has been linked, you can create Uniform objects to let you
		set the uniforms that your shaders use.

		Be careful not to call the set() functions unless the appropriate program
		is loaded into the current context.
	*/
	struct Uniform
	{
		/** Initialises a uniform.
			The program must have been successfully linked when this
			constructor is called.
		*/
		Uniform (const OpenGLShaderProgram& program, const char* uniformName);

		/** Sets a float uniform. */
		void set (GLfloat n1) const noexcept;
		/** Sets an int uniform. */
		void set (GLint n1) const noexcept;
		/** Sets a vec2 uniform. */
		void set (GLfloat n1, GLfloat n2) const noexcept;
		/** Sets a vec3 uniform. */
		void set (GLfloat n1, GLfloat n2, GLfloat n3) const noexcept;
		/** Sets a vec4 uniform. */
		void set (GLfloat n1, GLfloat n2, GLfloat n3, float n4) const noexcept;
		/** Sets an ivec4 uniform. */
		void set (GLint n1, GLint n2, GLint n3, GLint n4) const noexcept;
		/** Sets a vector float uniform. */
		void set (const GLfloat* values, int numValues) const noexcept;

		/** The uniform's ID number.
			If the uniform couldn't be found, this value will be < 0.
		*/
		GLint uniformID;

	private:
		const OpenGLContext& context;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Uniform);
	};

	/** Represents an openGL vertex attribute value.
		After a program has been linked, you can create Attribute objects to let you
		set the attributes that your vertex shaders use.
	*/
	struct Attribute
	{
		/** Initialises an attribute.
			The program must have been successfully linked when this
			constructor is called.
		*/
		Attribute (const OpenGLShaderProgram& program, const char* attributeName);

		/** The attribute's ID number.
			If the uniform couldn't be found, this value will be < 0.
		*/
		GLint attributeID;
	};

	/** The ID number of the compiled program. */
	GLuint programID;

private:
	const OpenGLContext& context;
	String errorLog;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLShaderProgram);
};

#endif   // __JUCE_OPENGLSHADERPROGRAM_JUCEHEADER__

/*** End of inlined file: juce_OpenGLShaderProgram.h ***/


#endif
#ifndef __JUCE_OPENGLTEXTURE_JUCEHEADER__

/*** Start of inlined file: juce_OpenGLTexture.h ***/
#ifndef __JUCE_OPENGLTEXTURE_JUCEHEADER__
#define __JUCE_OPENGLTEXTURE_JUCEHEADER__

/**
	Creates an openGL texture from an Image.
*/
class JUCE_API  OpenGLTexture
{
public:
	OpenGLTexture();
	~OpenGLTexture();

	/** Creates a texture from the given image.

		Note that if the image's dimensions aren't a power-of-two, the texture may
		be created with a larger size.

		The image will be arranged so that its top-left corner is at texture
		coordinate (0, 1).
	*/
	void loadImage (const Image& image);

	/** Creates a texture from a raw array of pixels.
		The width and height provided must be valid - i.e. power-of-two unless
		the underlying GL system allows otherwise.
		The data is sent directly to the OpenGL driver without being flipped vertically,
		so the first pixel will be mapped onto texture coordinate (0, 0).
		bottom-left corner of the texture
	*/
	void loadARGB (const PixelARGB* pixels, int width, int height);

	/** Creates a texture from a raw array of pixels.
		This is like loadARGB, but will vertically flip the data so that the first
		pixel ends up at texture coordinate (0, 1), and if the width and height are
		not powers-of-two, it will compensate by using a larger texture size.
	*/
	void loadARGBFlipped (const PixelARGB* pixels, int width, int height);

	/** Creates an alpha-channel texture from an array of alpha values.
		The width and height provided must be valid - i.e. power-of-two unless
		the underlying GL system allows otherwise.
		The data is sent directly to the OpenGL driver without being flipped vertically,
		so the first pixel will be mapped onto texture coordinate (0, 0).
		bottom-left corner of the texture
	*/
	void loadAlpha (const uint8* pixels, int width, int height);

	/** Frees the texture, if there is one. */
	void release();

	/** Binds the texture to the currently selected openGL context. */
	void bind() const;

	/** Unbinds the texture to the currently selected openGL context. */
	void unbind() const;

   #if JUCE_USE_OPENGL_FIXED_FUNCTION
	/** Draws this texture into the current context, with the specified corner positions. */
	void draw2D (float x1, float y1,
				 float x2, float y2,
				 float x3, float y3,
				 float x4, float y4,
				 const Colour& colour) const;

	/** Draws this texture into the current context, with the specified corner positions. */
	void draw3D (float x1, float y1, float z1,
				 float x2, float y2, float z2,
				 float x3, float y3, float z3,
				 float x4, float y4, float z4,
				 const Colour& colour) const;
   #endif

	/** Returns the GL texture ID number. */
	GLuint getTextureID() const noexcept        { return textureID; }

	int getWidth() const noexcept               { return width; }
	int getHeight() const noexcept              { return height; }

	/** Returns true if a texture can be created with the given size.
		Some systems may require that the sizes are powers-of-two.
	*/
	static bool isValidSize (int width, int height);

private:
	GLuint textureID;
	int width, height;

	void create (int w, int h, const void*, GLenum type);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLTexture);
};

#endif   // __JUCE_OPENGLTEXTURE_JUCEHEADER__

/*** End of inlined file: juce_OpenGLTexture.h ***/


#endif
#ifndef __JUCE_QUATERNION_JUCEHEADER__

#endif
#ifndef __JUCE_VECTOR3D_JUCEHEADER__

#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_OPENGL_JUCEHEADER__

/*** End of inlined file: juce_opengl.h ***/

