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


/*** Start of inlined file: juce_gui_basics.h ***/
#ifndef __JUCE_GUI_BASICS_JUCEHEADER__
#define __JUCE_GUI_BASICS_JUCEHEADER__

#include "juce_graphics_amalgam.h"
#include "juce_data_structures_amalgam.h"

/** Config: JUCE_ENABLE_REPAINT_DEBUGGING
	If this option is turned on, each area of the screen that gets repainted will
	flash in a random colour, so that you can see exactly which bits of your
	components are being drawn.
*/
#ifndef JUCE_ENABLE_REPAINT_DEBUGGING
 #define JUCE_ENABLE_REPAINT_DEBUGGING 0
#endif

/** JUCE_USE_XINERAMA: Enables Xinerama multi-monitor support (Linux only).
	Unless you specifically want to disable this, it's best to leave this option turned on.
*/
#ifndef JUCE_USE_XINERAMA
 #define JUCE_USE_XINERAMA 1
#endif

/** Config: JUCE_USE_XSHM
	Enables X shared memory for faster rendering on Linux. This is best left turned on
	unless you have a good reason to disable it.
*/
#ifndef JUCE_USE_XSHM
 #define JUCE_USE_XSHM 1
#endif

/** Config: JUCE_USE_XRENDER
	Enables XRender to allow semi-transparent windowing on Linux.
*/
#ifndef JUCE_USE_XRENDER
 #define JUCE_USE_XRENDER 0
#endif

/** Config: JUCE_USE_XCURSOR
	Uses XCursor to allow ARGB cursor on Linux. This is best left turned on unless you have
	a good reason to disable it.
*/
#ifndef JUCE_USE_XCURSOR
 #define JUCE_USE_XCURSOR 1
#endif

namespace juce
{

// START_AUTOINCLUDE components, mouse, keyboard, buttons, drawables,
// filebrowser, layout, lookandfeel, menus, positioning, properties,
// widgets, windows, commands, application, misc
#ifndef __JUCE_CACHEDCOMPONENTIMAGE_JUCEHEADER__

/*** Start of inlined file: juce_CachedComponentImage.h ***/
#ifndef __JUCE_CACHEDCOMPONENTIMAGE_JUCEHEADER__
#define __JUCE_CACHEDCOMPONENTIMAGE_JUCEHEADER__

class Component;

/**
	Base class used internally for structures that can store cached images of
	component state.

	Most people are unlikely to ever need to know about this class - it's really
	only for power-users!

	@see Component::setCachedComponentImage
*/
class JUCE_API  CachedComponentImage
{
public:
	CachedComponentImage() noexcept {}
	virtual ~CachedComponentImage() {}

	/** Called as part of the parent component's paint method, this must draw
		the given component into the target graphics context, using the cached
		version where possible.
	*/
	virtual void paint (Graphics&) = 0;

	/** Invalidates all cached image data. */
	virtual void invalidateAll() = 0;

	/** Invalidates a section of the cached image data. */
	virtual void invalidate (const Rectangle<int>& area) = 0;

	/** Called to indicate that the component is no longer active, so
		any cached data should be released if possible.
	*/
	virtual void releaseResources() = 0;
};

#endif   // __JUCE_CACHEDCOMPONENTIMAGE_JUCEHEADER__

/*** End of inlined file: juce_CachedComponentImage.h ***/


#endif
#ifndef __JUCE_COMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_Component.h ***/
#ifndef __JUCE_COMPONENT_JUCEHEADER__
#define __JUCE_COMPONENT_JUCEHEADER__


/*** Start of inlined file: juce_MouseCursor.h ***/
#ifndef __JUCE_MOUSECURSOR_JUCEHEADER__
#define __JUCE_MOUSECURSOR_JUCEHEADER__

class Image;
class ComponentPeer;
class Component;

/**
	Represents a mouse cursor image.

	This object can either be used to represent one of the standard mouse
	cursor shapes, or a custom one generated from an image.
*/
class JUCE_API  MouseCursor
{
public:

	/** The set of available standard mouse cursors. */
	enum StandardCursorType
	{
		NoCursor = 0,                   /**< An invisible cursor. */
		NormalCursor,                   /**< The stardard arrow cursor. */

		WaitCursor,                     /**< The normal hourglass or spinning-beachball 'busy' cursor. */
		IBeamCursor,                    /**< A vertical I-beam for positioning within text. */
		CrosshairCursor,                /**< A pair of crosshairs. */
		CopyingCursor,                  /**< The normal arrow cursor, but with a "+" on it to indicate
											 that you're dragging a copy of something. */

		PointingHandCursor,             /**< A hand with a pointing finger, for clicking on web-links. */
		DraggingHandCursor,             /**< An open flat hand for dragging heavy objects around. */

		LeftRightResizeCursor,          /**< An arrow pointing left and right. */
		UpDownResizeCursor,             /**< an arrow pointing up and down. */
		UpDownLeftRightResizeCursor,    /**< An arrow pointing up, down, left and right. */

		TopEdgeResizeCursor,            /**< A platform-specific cursor for resizing the top-edge of a window. */
		BottomEdgeResizeCursor,         /**< A platform-specific cursor for resizing the bottom-edge of a window. */
		LeftEdgeResizeCursor,           /**< A platform-specific cursor for resizing the left-edge of a window. */
		RightEdgeResizeCursor,          /**< A platform-specific cursor for resizing the right-edge of a window. */
		TopLeftCornerResizeCursor,      /**< A platform-specific cursor for resizing the top-left-corner of a window. */
		TopRightCornerResizeCursor,     /**< A platform-specific cursor for resizing the top-right-corner of a window. */
		BottomLeftCornerResizeCursor,   /**< A platform-specific cursor for resizing the bottom-left-corner of a window. */
		BottomRightCornerResizeCursor   /**< A platform-specific cursor for resizing the bottom-right-corner of a window. */
	};

	/** Creates the standard arrow cursor. */
	MouseCursor();

	/** Creates one of the standard mouse cursor */
	MouseCursor (StandardCursorType type);

	/** Creates a custom cursor from an image.

		@param image    the image to use for the cursor - if this is bigger than the
						system can manage, it might get scaled down first, and might
						also have to be turned to black-and-white if it can't do colour
						cursors.
		@param hotSpotX the x position of the cursor's hotspot within the image
		@param hotSpotY the y position of the cursor's hotspot within the image
	*/
	MouseCursor (const Image& image, int hotSpotX, int hotSpotY);

	/** Creates a copy of another cursor object. */
	MouseCursor (const MouseCursor& other);

	/** Copies this cursor from another object. */
	MouseCursor& operator= (const MouseCursor& other);

	/** Destructor. */
	~MouseCursor();

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	MouseCursor (MouseCursor&& other) noexcept;
	MouseCursor& operator= (MouseCursor&& other) noexcept;
   #endif

	/** Checks whether two mouse cursors are the same.

		For custom cursors, two cursors created from the same image won't be
		recognised as the same, only MouseCursor objects that have been
		copied from the same object.
	*/
	bool operator== (const MouseCursor& other) const noexcept;

	/** Checks whether two mouse cursors are the same.

		For custom cursors, two cursors created from the same image won't be
		recognised as the same, only MouseCursor objects that have been
		copied from the same object.
	*/
	bool operator!= (const MouseCursor& other) const noexcept;

	/** Makes the system show its default 'busy' cursor.

		This will turn the system cursor to an hourglass or spinning beachball
		until the next time the mouse is moved, or hideWaitCursor() is called.

		This is handy if the message loop is about to block for a couple of
		seconds while busy and you want to give the user feedback about this.

		@see MessageManager::setTimeBeforeShowingWaitCursor
	*/
	static void showWaitCursor();

	/** If showWaitCursor has been called, this will return the mouse to its
		normal state.

		This will look at what component is under the mouse, and update the
		cursor to be the correct one for that component.

		@see showWaitCursor
	*/
	static void hideWaitCursor();

private:

	class SharedCursorHandle;
	friend class SharedCursorHandle;
	SharedCursorHandle* cursorHandle;

	friend class MouseInputSourceInternal;
	void showInWindow (ComponentPeer* window) const;
	void showInAllWindows() const;
	void* getHandle() const noexcept;

	static void* createMouseCursorFromImage (const Image& image, int hotspotX, int hotspotY);
	static void* createStandardMouseCursor (MouseCursor::StandardCursorType type);
	static void deleteMouseCursor (void* cursorHandle, bool isStandard);

	JUCE_LEAK_DETECTOR (MouseCursor);
};

#endif   // __JUCE_MOUSECURSOR_JUCEHEADER__

/*** End of inlined file: juce_MouseCursor.h ***/


/*** Start of inlined file: juce_MouseListener.h ***/
#ifndef __JUCE_MOUSELISTENER_JUCEHEADER__
#define __JUCE_MOUSELISTENER_JUCEHEADER__

class MouseEvent;

/**
	A MouseListener can be registered with a component to receive callbacks
	about mouse events that happen to that component.

	@see Component::addMouseListener, Component::removeMouseListener
*/
class JUCE_API  MouseListener
{
public:
	/** Destructor. */
	virtual ~MouseListener()  {}

	/** Called when the mouse moves inside a component.

		If the mouse button isn't pressed and the mouse moves over a component,
		this will be called to let the component react to this.

		A component will always get a mouseEnter callback before a mouseMove.

		@param e    details about the position and status of the mouse event, including
					the source component in which it occurred
		@see mouseEnter, mouseExit, mouseDrag, contains
	*/
	virtual void mouseMove          (const MouseEvent& e);

	/** Called when the mouse first enters a component.

		If the mouse button isn't pressed and the mouse moves into a component,
		this will be called to let the component react to this.

		When the mouse button is pressed and held down while being moved in
		or out of a component, no mouseEnter or mouseExit callbacks are made - only
		mouseDrag messages are sent to the component that the mouse was originally
		clicked on, until the button is released.

		@param e    details about the position and status of the mouse event, including
					the source component in which it occurred
		@see mouseExit, mouseDrag, mouseMove, contains
	*/
	virtual void mouseEnter         (const MouseEvent& e);

	/** Called when the mouse moves out of a component.

		This will be called when the mouse moves off the edge of this
		component.

		If the mouse button was pressed, and it was then dragged off the
		edge of the component and released, then this callback will happen
		when the button is released, after the mouseUp callback.

		@param e    details about the position and status of the mouse event, including
					the source component in which it occurred
		@see mouseEnter, mouseDrag, mouseMove, contains
	*/
	virtual void mouseExit          (const MouseEvent& e);

	/** Called when a mouse button is pressed.

		The MouseEvent object passed in contains lots of methods for finding out
		which button was pressed, as well as which modifier keys (e.g. shift, ctrl)
		were held down at the time.

		Once a button is held down, the mouseDrag method will be called when the
		mouse moves, until the button is released.

		@param e    details about the position and status of the mouse event, including
					the source component in which it occurred
		@see mouseUp, mouseDrag, mouseDoubleClick, contains
	*/
	virtual void mouseDown          (const MouseEvent& e);

	/** Called when the mouse is moved while a button is held down.

		When a mouse button is pressed inside a component, that component
		receives mouseDrag callbacks each time the mouse moves, even if the
		mouse strays outside the component's bounds.

		@param e    details about the position and status of the mouse event, including
					the source component in which it occurred
		@see mouseDown, mouseUp, mouseMove, contains, setDragRepeatInterval
	*/
	virtual void mouseDrag          (const MouseEvent& e);

	/** Called when a mouse button is released.

		A mouseUp callback is sent to the component in which a button was pressed
		even if the mouse is actually over a different component when the
		button is released.

		The MouseEvent object passed in contains lots of methods for finding out
		which buttons were down just before they were released.

		@param e    details about the position and status of the mouse event, including
					the source component in which it occurred
		@see mouseDown, mouseDrag, mouseDoubleClick, contains
	*/
	virtual void mouseUp            (const MouseEvent& e);

	/** Called when a mouse button has been double-clicked on a component.

		The MouseEvent object passed in contains lots of methods for finding out
		which button was pressed, as well as which modifier keys (e.g. shift, ctrl)
		were held down at the time.

		@param e    details about the position and status of the mouse event, including
					the source component in which it occurred
		@see mouseDown, mouseUp
	*/
	virtual void mouseDoubleClick   (const MouseEvent& e);

	/** Called when the mouse-wheel is moved.

		This callback is sent to the component that the mouse is over when the
		wheel is moved.

		If not overridden, the component will forward this message to its parent, so
		that parent components can collect mouse-wheel messages that happen to
		child components which aren't interested in them.

		@param e    details about the position and status of the mouse event, including
					the source component in which it occurred
		@param wheelIncrementX   the speed and direction of the horizontal scroll-wheel - a positive
								 value means the wheel has been pushed to the right, negative means it
								 was pushed to the left
		@param wheelIncrementY   the speed and direction of the vertical scroll-wheel - a positive
								 value means the wheel has been pushed upwards, negative means it
								 was pushed downwards
	*/
	virtual void mouseWheelMove     (const MouseEvent& e,
									 float wheelIncrementX,
									 float wheelIncrementY);
};

#endif   // __JUCE_MOUSELISTENER_JUCEHEADER__

/*** End of inlined file: juce_MouseListener.h ***/


/*** Start of inlined file: juce_MouseEvent.h ***/
#ifndef __JUCE_MOUSEEVENT_JUCEHEADER__
#define __JUCE_MOUSEEVENT_JUCEHEADER__

class Component;
class MouseInputSource;

/*** Start of inlined file: juce_ModifierKeys.h ***/
#ifndef __JUCE_MODIFIERKEYS_JUCEHEADER__
#define __JUCE_MODIFIERKEYS_JUCEHEADER__

/**
	Represents the state of the mouse buttons and modifier keys.

	This is used both by mouse events and by KeyPress objects to describe
	the state of keys such as shift, control, alt, etc.

	@see KeyPress, MouseEvent::mods
*/
class JUCE_API  ModifierKeys
{
public:

	/** Creates a ModifierKeys object with no flags set. */
	ModifierKeys() noexcept;

	/** Creates a ModifierKeys object from a raw set of flags.

		@param flags to represent the keys that are down
		@see    shiftModifier, ctrlModifier, altModifier, leftButtonModifier,
				rightButtonModifier, commandModifier, popupMenuClickModifier
	*/
	ModifierKeys (int flags) noexcept;

	/** Creates a copy of another object. */
	ModifierKeys (const ModifierKeys& other) noexcept;

	/** Copies this object from another one. */
	ModifierKeys& operator= (const ModifierKeys& other) noexcept;

	/** Checks whether the 'command' key flag is set (or 'ctrl' on Windows/Linux).

		This is a platform-agnostic way of checking for the operating system's
		preferred command-key modifier - so on the Mac it tests for the Apple key, on
		Windows/Linux, it's actually checking for the CTRL key.
	*/
	inline bool isCommandDown() const noexcept          { return testFlags (commandModifier); }

	/** Checks whether the user is trying to launch a pop-up menu.

		This checks for platform-specific modifiers that might indicate that the user
		is following the operating system's normal method of showing a pop-up menu.

		So on Windows/Linux, this method is really testing for a right-click.
		On the Mac, it tests for either the CTRL key being down, or a right-click.
	*/
	inline bool isPopupMenu() const noexcept            { return testFlags (popupMenuClickModifier); }

	/** Checks whether the flag is set for the left mouse-button. */
	inline bool isLeftButtonDown() const noexcept       { return testFlags (leftButtonModifier); }

	/** Checks whether the flag is set for the right mouse-button.

		Note that for detecting popup-menu clicks, you should be using isPopupMenu() instead, as
		this is platform-independent (and makes your code more explanatory too).
	*/
	inline bool isRightButtonDown() const noexcept      { return testFlags (rightButtonModifier); }

	inline bool isMiddleButtonDown() const noexcept     { return testFlags (middleButtonModifier); }

	/** Tests for any of the mouse-button flags. */
	inline bool isAnyMouseButtonDown() const noexcept   { return testFlags (allMouseButtonModifiers); }

	/** Tests for any of the modifier key flags. */
	inline bool isAnyModifierKeyDown() const noexcept   { return testFlags ((shiftModifier | ctrlModifier | altModifier | commandModifier)); }

	/** Checks whether the shift key's flag is set. */
	inline bool isShiftDown() const noexcept            { return testFlags (shiftModifier); }

	/** Checks whether the CTRL key's flag is set.

		Remember that it's better to use the platform-agnostic routines to test for command-key and
		popup-menu modifiers.

		@see isCommandDown, isPopupMenu
	*/
	inline bool isCtrlDown() const noexcept             { return testFlags (ctrlModifier); }

	/** Checks whether the shift key's flag is set. */
	inline bool isAltDown() const noexcept              { return testFlags (altModifier); }

	/** Flags that represent the different keys. */
	enum Flags
	{
		/** Indicates no modifier keys. */
		noModifiers                             = 0,

		/** Shift key flag. */
		shiftModifier                           = 1,

		/** CTRL key flag. */
		ctrlModifier                            = 2,

		/** ALT key flag. */
		altModifier                             = 4,

		/** Left mouse button flag. */
		leftButtonModifier                      = 16,

		/** Right mouse button flag. */
		rightButtonModifier                     = 32,

		/** Middle mouse button flag. */
		middleButtonModifier                    = 64,

	   #if JUCE_MAC
		/** Command key flag - on windows this is the same as the CTRL key flag. */
		commandModifier                         = 8,

		/** Popup menu flag - on windows this is the same as rightButtonModifier, on the
			Mac it's the same as (rightButtonModifier | ctrlModifier). */
		popupMenuClickModifier                  = rightButtonModifier | ctrlModifier,
	   #else
		/** Command key flag - on windows this is the same as the CTRL key flag. */
		commandModifier                         = ctrlModifier,

		/** Popup menu flag - on windows this is the same as rightButtonModifier, on the
			Mac it's the same as (rightButtonModifier | ctrlModifier). */
		popupMenuClickModifier                  = rightButtonModifier,
	   #endif

		/** Represents a combination of all the shift, alt, ctrl and command key modifiers. */
		allKeyboardModifiers                    = shiftModifier | ctrlModifier | altModifier | commandModifier,

		/** Represents a combination of all the mouse buttons at once. */
		allMouseButtonModifiers                 = leftButtonModifier | rightButtonModifier | middleButtonModifier,
	};

	/** Returns a copy of only the mouse-button flags */
	ModifierKeys withOnlyMouseButtons() const noexcept                  { return ModifierKeys (flags & allMouseButtonModifiers); }

	/** Returns a copy of only the non-mouse flags */
	ModifierKeys withoutMouseButtons() const noexcept                   { return ModifierKeys (flags & ~allMouseButtonModifiers); }

	bool operator== (const ModifierKeys& other) const noexcept          { return flags == other.flags; }
	bool operator!= (const ModifierKeys& other) const noexcept          { return flags != other.flags; }

	/** Returns the raw flags for direct testing. */
	inline int getRawFlags() const noexcept                             { return flags; }

	inline const ModifierKeys withoutFlags (int rawFlagsToClear) const noexcept { return ModifierKeys (flags & ~rawFlagsToClear); }
	inline const ModifierKeys withFlags (int rawFlagsToSet) const noexcept      { return ModifierKeys (flags | rawFlagsToSet); }

	/** Tests a combination of flags and returns true if any of them are set. */
	inline bool testFlags (const int flagsToTest) const noexcept        { return (flags & flagsToTest) != 0; }

	/** Returns the total number of mouse buttons that are down. */
	int getNumMouseButtonsDown() const noexcept;

	/** Creates a ModifierKeys object to represent the last-known state of the
		keyboard and mouse buttons.

		@see getCurrentModifiersRealtime
	*/
	static ModifierKeys getCurrentModifiers() noexcept;

	/** Creates a ModifierKeys object to represent the current state of the
		keyboard and mouse buttons.

		This isn't often needed and isn't recommended, but will actively check all the
		mouse and key states rather than just returning their last-known state like
		getCurrentModifiers() does.

		This is only needed in special circumstances for up-to-date modifier information
		at times when the app's event loop isn't running normally.

		Another reason to avoid this method is that it's not stateless, and calling it may
		update the value returned by getCurrentModifiers(), which could cause subtle changes
		in the behaviour of some components.
	*/
	static ModifierKeys getCurrentModifiersRealtime() noexcept;

private:

	int flags;

	friend class ComponentPeer;
	friend class MouseInputSource;
	friend class MouseInputSourceInternal;

	static ModifierKeys currentModifiers;
	static void updateCurrentModifiers() noexcept;
};

#endif   // __JUCE_MODIFIERKEYS_JUCEHEADER__

/*** End of inlined file: juce_ModifierKeys.h ***/


/**
	Contains position and status information about a mouse event.

	@see MouseListener, Component::mouseMove, Component::mouseEnter, Component::mouseExit,
		 Component::mouseDown, Component::mouseUp, Component::mouseDrag
*/
class JUCE_API  MouseEvent
{
public:

	/** Creates a MouseEvent.

		Normally an application will never need to use this.

		@param source           the source that's invoking the event
		@param position         the position of the mouse, relative to the component that is passed-in
		@param modifiers        the key modifiers at the time of the event
		@param eventComponent   the component that the mouse event applies to
		@param originator       the component that originally received the event
		@param eventTime        the time the event happened
		@param mouseDownPos     the position of the corresponding mouse-down event (relative to the component that is passed-in).
								If there isn't a corresponding mouse-down (e.g. for a mouse-move), this will just be
								the same as the current mouse-x position.
		@param mouseDownTime    the time at which the corresponding mouse-down event happened
								If there isn't a corresponding mouse-down (e.g. for a mouse-move), this will just be
								the same as the current mouse-event time.
		@param numberOfClicks   how many clicks, e.g. a double-click event will be 2, a triple-click will be 3, etc
		@param mouseWasDragged  whether the mouse has been dragged significantly since the previous mouse-down
	*/
	MouseEvent (MouseInputSource& source,
				const Point<int>& position,
				const ModifierKeys& modifiers,
				Component* eventComponent,
				Component* originator,
				const Time& eventTime,
				const Point<int> mouseDownPos,
				const Time& mouseDownTime,
				int numberOfClicks,
				bool mouseWasDragged) noexcept;

	/** Destructor. */
	~MouseEvent() noexcept;

	/** The x-position of the mouse when the event occurred.

		This value is relative to the top-left of the component to which the
		event applies (as indicated by the MouseEvent::eventComponent field).
	*/
	const int x;

	/** The y-position of the mouse when the event occurred.

		This value is relative to the top-left of the component to which the
		event applies (as indicated by the MouseEvent::eventComponent field).
	*/
	const int y;

	/** The key modifiers associated with the event.

		This will let you find out which mouse buttons were down, as well as which
		modifier keys were held down.

		When used for mouse-up events, this will indicate the state of the mouse buttons
		just before they were released, so that you can tell which button they let go of.
	*/
	const ModifierKeys mods;

	/** The component that this event applies to.

		This is usually the component that the mouse was over at the time, but for mouse-drag
		events the mouse could actually be over a different component and the events are
		still sent to the component that the button was originally pressed on.

		The x and y member variables are relative to this component's position.

		If you use getEventRelativeTo() to retarget this object to be relative to a different
		component, this pointer will be updated, but originalComponent remains unchanged.

		@see originalComponent
	*/
	Component* const eventComponent;

	/** The component that the event first occurred on.

		If you use getEventRelativeTo() to retarget this object to be relative to a different
		component, this value remains unchanged to indicate the first component that received it.

		@see eventComponent
	*/
	Component* const originalComponent;

	/** The time that this mouse-event occurred. */
	const Time eventTime;

	/** The source device that generated this event. */
	MouseInputSource& source;

	/** Returns the x co-ordinate of the last place that a mouse was pressed.

		The co-ordinate is relative to the component specified in MouseEvent::component.

		@see getDistanceFromDragStart, getDistanceFromDragStartX, mouseWasClicked
	*/
	int getMouseDownX() const noexcept;

	/** Returns the y co-ordinate of the last place that a mouse was pressed.

		The co-ordinate is relative to the component specified in MouseEvent::component.

		@see getDistanceFromDragStart, getDistanceFromDragStartX, mouseWasClicked
	*/
	int getMouseDownY() const noexcept;

	/** Returns the co-ordinates of the last place that a mouse was pressed.

		The co-ordinates are relative to the component specified in MouseEvent::component.

		@see getDistanceFromDragStart, getDistanceFromDragStartX, mouseWasClicked
	*/
	Point<int> getMouseDownPosition() const noexcept;

	/** Returns the straight-line distance between where the mouse is now and where it
		was the last time the button was pressed.

		This is quite handy for things like deciding whether the user has moved far enough
		for it to be considered a drag operation.

		@see getDistanceFromDragStartX
	*/
	int getDistanceFromDragStart() const noexcept;

	/** Returns the difference between the mouse's current x postion and where it was
		when the button was last pressed.

		@see getDistanceFromDragStart
	*/
	int getDistanceFromDragStartX() const noexcept;

	/** Returns the difference between the mouse's current y postion and where it was
		when the button was last pressed.

		@see getDistanceFromDragStart
	*/
	int getDistanceFromDragStartY() const noexcept;

	/** Returns the difference between the mouse's current postion and where it was
		when the button was last pressed.

		@see getDistanceFromDragStart
	*/
	Point<int> getOffsetFromDragStart() const noexcept;

	/** Returns true if the mouse has just been clicked.

		Used in either your mouseUp() or mouseDrag() methods, this will tell you whether
		the user has dragged the mouse more than a few pixels from the place where the
		mouse-down occurred.

		Once they have dragged it far enough for this method to return false, it will continue
		to return false until the mouse-up, even if they move the mouse back to the same
		position where they originally pressed it. This means that it's very handy for
		objects that can either be clicked on or dragged, as you can use it in the mouseDrag()
		callback to ignore any small movements they might make while clicking.

		@returns    true if the mouse wasn't dragged by more than a few pixels between
					the last time the button was pressed and released.
	*/
	bool mouseWasClicked() const noexcept;

	/** For a click event, the number of times the mouse was clicked in succession.

		So for example a double-click event will return 2, a triple-click 3, etc.
	*/
	int getNumberOfClicks() const noexcept                              { return numberOfClicks; }

	/** Returns the time that the mouse button has been held down for.

		If called from a mouseDrag or mouseUp callback, this will return the
		number of milliseconds since the corresponding mouseDown event occurred.
		If called in other contexts, e.g. a mouseMove, then the returned value
		may be 0 or an undefined value.
	*/
	int getLengthOfMousePress() const noexcept;

	/** The position of the mouse when the event occurred.

		This position is relative to the top-left of the component to which the
		event applies (as indicated by the MouseEvent::eventComponent field).
	*/
	Point<int> getPosition() const noexcept;

	/** Returns the mouse x position of this event, in global screen co-ordinates.

		The co-ordinates are relative to the top-left of the main monitor.

		@see getScreenPosition
	*/
	int getScreenX() const;

	/** Returns the mouse y position of this event, in global screen co-ordinates.

		The co-ordinates are relative to the top-left of the main monitor.

		@see getScreenPosition
	*/
	int getScreenY() const;

	/** Returns the mouse position of this event, in global screen co-ordinates.

		The co-ordinates are relative to the top-left of the main monitor.

		@see getMouseDownScreenPosition
	*/
	Point<int> getScreenPosition() const;

	/** Returns the x co-ordinate at which the mouse button was last pressed.

		The co-ordinates are relative to the top-left of the main monitor.

		@see getMouseDownScreenPosition
	*/
	int getMouseDownScreenX() const;

	/** Returns the y co-ordinate at which the mouse button was last pressed.

		The co-ordinates are relative to the top-left of the main monitor.

		@see getMouseDownScreenPosition
	*/
	int getMouseDownScreenY() const;

	/** Returns the co-ordinates at which the mouse button was last pressed.

		The co-ordinates are relative to the top-left of the main monitor.

		@see getScreenPosition
	*/
	Point<int> getMouseDownScreenPosition() const;

	/** Creates a version of this event that is relative to a different component.

		The x and y positions of the event that is returned will have been
		adjusted to be relative to the new component.
		The component pointer that is passed-in must not be null.
	*/
	MouseEvent getEventRelativeTo (Component* newComponent) const noexcept;

	/** Creates a copy of this event with a different position.
		All other members of the event object are the same, but the x and y are
		replaced with these new values.
	*/
	MouseEvent withNewPosition (const Point<int>& newPosition) const noexcept;

	/** Changes the application-wide setting for the double-click time limit.

		This is the maximum length of time between mouse-clicks for it to be
		considered a double-click. It's used by the Component class.

		@see getDoubleClickTimeout, MouseListener::mouseDoubleClick
	*/
	static void setDoubleClickTimeout (int timeOutMilliseconds) noexcept;

	/** Returns the application-wide setting for the double-click time limit.

		This is the maximum length of time between mouse-clicks for it to be
		considered a double-click. It's used by the Component class.

		@see setDoubleClickTimeout, MouseListener::mouseDoubleClick
	*/
	static int getDoubleClickTimeout() noexcept;

private:

	const Point<int> mouseDownPos;
	const Time mouseDownTime;
	const uint8 numberOfClicks, wasMovedSinceMouseDown;

	MouseEvent& operator= (const MouseEvent&);
};

#endif   // __JUCE_MOUSEEVENT_JUCEHEADER__

/*** End of inlined file: juce_MouseEvent.h ***/


/*** Start of inlined file: juce_ComponentListener.h ***/
#ifndef __JUCE_COMPONENTLISTENER_JUCEHEADER__
#define __JUCE_COMPONENTLISTENER_JUCEHEADER__

class Component;

/**
	Gets informed about changes to a component's hierarchy or position.

	To monitor a component for changes, register a subclass of ComponentListener
	with the component using Component::addComponentListener().

	Be sure to deregister listeners before you delete them!

	@see Component::addComponentListener, Component::removeComponentListener
*/
class JUCE_API  ComponentListener
{
public:
	/** Destructor. */
	virtual ~ComponentListener()  {}

	/** Called when the component's position or size changes.

		@param component    the component that was moved or resized
		@param wasMoved     true if the component's top-left corner has just moved
		@param wasResized   true if the component's width or height has just changed
		@see Component::setBounds, Component::resized, Component::moved
	*/
	virtual void componentMovedOrResized (Component& component,
										  bool wasMoved,
										  bool wasResized);

	/** Called when the component is brought to the top of the z-order.

		@param component    the component that was moved
		@see Component::toFront, Component::broughtToFront
	*/
	virtual void componentBroughtToFront (Component& component);

	/** Called when the component is made visible or invisible.

		@param component    the component that changed
		@see Component::setVisible
	*/
	virtual void componentVisibilityChanged (Component& component);

	/** Called when the component has children added or removed.

		@param component    the component whose children were changed
		@see Component::childrenChanged, Component::addChildComponent,
			 Component::removeChildComponent
	*/
	virtual void componentChildrenChanged (Component& component);

	/** Called to indicate that the component's parents have changed.

		When a component is added or removed from its parent, all of its children
		will produce this notification (recursively - so all children of its
		children will also be called as well).

		@param component    the component that this listener is registered with
		@see Component::parentHierarchyChanged
	*/
	virtual void componentParentHierarchyChanged (Component& component);

	/** Called when the component's name is changed.

		@see Component::setName, Component::getName
	*/
	virtual void componentNameChanged (Component& component);

	/** Called when the component is in the process of being deleted.

		This callback is made from inside the destructor, so be very, very cautious
		about what you do in here.

		In particular, bear in mind that it's the Component base class's destructor that calls
		this - so if the object that's being deleted is a subclass of Component, then the
		subclass layers of the object will already have been destructed when it gets to this
		point!
	*/
	virtual void componentBeingDeleted (Component& component);
};

#endif   // __JUCE_COMPONENTLISTENER_JUCEHEADER__

/*** End of inlined file: juce_ComponentListener.h ***/


/*** Start of inlined file: juce_KeyListener.h ***/
#ifndef __JUCE_KEYLISTENER_JUCEHEADER__
#define __JUCE_KEYLISTENER_JUCEHEADER__


/*** Start of inlined file: juce_KeyPress.h ***/
#ifndef __JUCE_KEYPRESS_JUCEHEADER__
#define __JUCE_KEYPRESS_JUCEHEADER__

/**
	Represents a key press, including any modifier keys that are needed.

	E.g. a KeyPress might represent CTRL+C, SHIFT+ALT+H, Spacebar, Escape, etc.

	@see Component, KeyListener, Button::addShortcut, KeyPressMappingManager
*/
class JUCE_API  KeyPress
{
public:

	/** Creates an (invalid) KeyPress.

		@see isValid
	*/
	KeyPress() noexcept;

	/** Creates a KeyPress for a key and some modifiers.

		e.g.
		CTRL+C would be: KeyPress ('c', ModifierKeys::ctrlModifier)
		SHIFT+Escape would be: KeyPress (KeyPress::escapeKey, ModifierKeys::shiftModifier)

		@param keyCode      a code that represents the key - this value must be
							one of special constants listed in this class, or an
							8-bit character code such as a letter (case is ignored),
							digit or a simple key like "," or ".". Note that this
							isn't the same as the textCharacter parameter, so for example
							a keyCode of 'a' and a shift-key modifier should have a
							textCharacter value of 'A'.
		@param modifiers    the modifiers to associate with the keystroke
		@param textCharacter    the character that would be printed if someone typed
							this keypress into a text editor. This value may be
							null if the keypress is a non-printing character
		@see getKeyCode, isKeyCode, getModifiers
	*/
	KeyPress (int keyCode,
			  const ModifierKeys& modifiers,
			  juce_wchar textCharacter) noexcept;

	/** Creates a keypress with a keyCode but no modifiers or text character.
	*/
	KeyPress (int keyCode) noexcept;

	/** Creates a copy of another KeyPress. */
	KeyPress (const KeyPress& other) noexcept;

	/** Copies this KeyPress from another one. */
	KeyPress& operator= (const KeyPress& other) noexcept;

	/** Compares two KeyPress objects. */
	bool operator== (const KeyPress& other) const noexcept;

	/** Compares two KeyPress objects. */
	bool operator!= (const KeyPress& other) const noexcept;

	/** Returns true if this is a valid KeyPress.

		A null keypress can be created by the default constructor, in case it's
		needed.
	*/
	bool isValid() const noexcept                               { return keyCode != 0; }

	/** Returns the key code itself.

		This will either be one of the special constants defined in this class,
		or an 8-bit character code.
	*/
	int getKeyCode() const noexcept                             { return keyCode; }

	/** Returns the key modifiers.

		@see ModifierKeys
	*/
	ModifierKeys getModifiers() const noexcept                  { return mods; }

	/** Returns the character that is associated with this keypress.

		This is the character that you'd expect to see printed if you press this
		keypress in a text editor or similar component.
	*/
	juce_wchar getTextCharacter() const noexcept                { return textCharacter; }

	/** Checks whether the KeyPress's key is the same as the one provided, without checking
		the modifiers.

		The values for key codes can either be one of the special constants defined in
		this class, or an 8-bit character code.

		@see getKeyCode
	*/
	bool isKeyCode (int keyCodeToCompare) const noexcept        { return keyCode == keyCodeToCompare; }

	/** Converts a textual key description to a KeyPress.

		This attempts to decode a textual version of a keypress, e.g. "CTRL + C" or "SPACE".

		This isn't designed to cope with any kind of input, but should be given the
		strings that are created by the getTextDescription() method.

		If the string can't be parsed, the object returned will be invalid.

		@see getTextDescription
	*/
	static KeyPress createFromDescription (const String& textVersion);

	/** Creates a textual description of the key combination.

		e.g. "CTRL + C" or "DELETE".

		To store a keypress in a file, use this method, along with createFromDescription()
		to retrieve it later.
	*/
	String getTextDescription() const;

	/** Creates a textual description of the key combination, using unicode icon symbols if possible.

		On OSX, this uses the Apple symbols for command, option, shift, etc, instead of the textual
		modifier key descriptions that are returned by getTextDescription()
	*/
	String getTextDescriptionWithIcons() const;

	/** Checks whether the user is currently holding down the keys that make up this
		KeyPress.

		Note that this will return false if any extra modifier keys are
		down - e.g. if the keypress is CTRL+X and the user is actually holding CTRL+ALT+x
		then it will be false.
	*/
	bool isCurrentlyDown() const;

	/** Checks whether a particular key is held down, irrespective of modifiers.

		The values for key codes can either be one of the special constants defined in
		this class, or an 8-bit character code.
	*/
	static bool isKeyCurrentlyDown (int keyCode);

	// Key codes
	//
	// Note that the actual values of these are platform-specific and may change
	// without warning, so don't store them anywhere as constants. For persisting/retrieving
	// KeyPress objects, use getTextDescription() and createFromDescription() instead.
	//

	static const int spaceKey;      /**< key-code for the space bar */
	static const int escapeKey;     /**< key-code for the escape key */
	static const int returnKey;     /**< key-code for the return key*/
	static const int tabKey;        /**< key-code for the tab key*/

	static const int deleteKey;     /**< key-code for the delete key (not backspace) */
	static const int backspaceKey;  /**< key-code for the backspace key */
	static const int insertKey;     /**< key-code for the insert key */

	static const int upKey;         /**< key-code for the cursor-up key */
	static const int downKey;       /**< key-code for the cursor-down key */
	static const int leftKey;       /**< key-code for the cursor-left key */
	static const int rightKey;      /**< key-code for the cursor-right key */
	static const int pageUpKey;     /**< key-code for the page-up key */
	static const int pageDownKey;   /**< key-code for the page-down key */
	static const int homeKey;       /**< key-code for the home key */
	static const int endKey;        /**< key-code for the end key */

	static const int F1Key;         /**< key-code for the F1 key */
	static const int F2Key;         /**< key-code for the F2 key */
	static const int F3Key;         /**< key-code for the F3 key */
	static const int F4Key;         /**< key-code for the F4 key */
	static const int F5Key;         /**< key-code for the F5 key */
	static const int F6Key;         /**< key-code for the F6 key */
	static const int F7Key;         /**< key-code for the F7 key */
	static const int F8Key;         /**< key-code for the F8 key */
	static const int F9Key;         /**< key-code for the F9 key */
	static const int F10Key;        /**< key-code for the F10 key */
	static const int F11Key;        /**< key-code for the F11 key */
	static const int F12Key;        /**< key-code for the F12 key */
	static const int F13Key;        /**< key-code for the F13 key */
	static const int F14Key;        /**< key-code for the F14 key */
	static const int F15Key;        /**< key-code for the F15 key */
	static const int F16Key;        /**< key-code for the F16 key */

	static const int numberPad0;     /**< key-code for the 0 on the numeric keypad. */
	static const int numberPad1;     /**< key-code for the 1 on the numeric keypad. */
	static const int numberPad2;     /**< key-code for the 2 on the numeric keypad. */
	static const int numberPad3;     /**< key-code for the 3 on the numeric keypad. */
	static const int numberPad4;     /**< key-code for the 4 on the numeric keypad. */
	static const int numberPad5;     /**< key-code for the 5 on the numeric keypad. */
	static const int numberPad6;     /**< key-code for the 6 on the numeric keypad. */
	static const int numberPad7;     /**< key-code for the 7 on the numeric keypad. */
	static const int numberPad8;     /**< key-code for the 8 on the numeric keypad. */
	static const int numberPad9;     /**< key-code for the 9 on the numeric keypad. */

	static const int numberPadAdd;            /**< key-code for the add sign on the numeric keypad. */
	static const int numberPadSubtract;       /**< key-code for the subtract sign on the numeric keypad. */
	static const int numberPadMultiply;       /**< key-code for the multiply sign on the numeric keypad. */
	static const int numberPadDivide;         /**< key-code for the divide sign on the numeric keypad. */
	static const int numberPadSeparator;      /**< key-code for the comma on the numeric keypad. */
	static const int numberPadDecimalPoint;   /**< key-code for the decimal point sign on the numeric keypad. */
	static const int numberPadEquals;         /**< key-code for the equals key on the numeric keypad. */
	static const int numberPadDelete;         /**< key-code for the delete key on the numeric keypad. */

	static const int playKey;        /**< key-code for a multimedia 'play' key, (not all keyboards will have one) */
	static const int stopKey;        /**< key-code for a multimedia 'stop' key, (not all keyboards will have one) */
	static const int fastForwardKey; /**< key-code for a multimedia 'fast-forward' key, (not all keyboards will have one) */
	static const int rewindKey;      /**< key-code for a multimedia 'rewind' key, (not all keyboards will have one) */

private:

	int keyCode;
	ModifierKeys mods;
	juce_wchar textCharacter;

	JUCE_LEAK_DETECTOR (KeyPress);
};

#endif   // __JUCE_KEYPRESS_JUCEHEADER__

/*** End of inlined file: juce_KeyPress.h ***/

class Component;

/**
	Receives callbacks when keys are pressed.

	You can add a key listener to a component to be informed when that component
	gets key events. See the Component::addListener method for more details.

	@see KeyPress, Component::addKeyListener, KeyPressMappingManager
*/
class JUCE_API  KeyListener
{
public:
	/** Destructor. */
	virtual ~KeyListener()  {}

	/** Called to indicate that a key has been pressed.

		If your implementation returns true, then the key event is considered to have
		been consumed, and will not be passed on to any other components. If it returns
		false, then the key will be passed to other components that might want to use it.

		@param key                      the keystroke, including modifier keys
		@param originatingComponent     the component that received the key event
		@see keyStateChanged, Component::keyPressed
	*/
	virtual bool keyPressed (const KeyPress& key,
							 Component* originatingComponent) = 0;

	/** Called when any key is pressed or released.

		When this is called, classes that might be interested in
		the state of one or more keys can use KeyPress::isKeyCurrentlyDown() to
		check whether their key has changed.

		If your implementation returns true, then the key event is considered to have
		been consumed, and will not be passed on to any other components. If it returns
		false, then the key will be passed to other components that might want to use it.

		@param originatingComponent     the component that received the key event
		@param isKeyDown                true if a key is being pressed, false if one is being released
		@see KeyPress, Component::keyStateChanged
	*/
	virtual bool keyStateChanged (bool isKeyDown, Component* originatingComponent);
};

#endif   // __JUCE_KEYLISTENER_JUCEHEADER__

/*** End of inlined file: juce_KeyListener.h ***/


/*** Start of inlined file: juce_KeyboardFocusTraverser.h ***/
#ifndef __JUCE_KEYBOARDFOCUSTRAVERSER_JUCEHEADER__
#define __JUCE_KEYBOARDFOCUSTRAVERSER_JUCEHEADER__

class Component;

/**
	Controls the order in which focus moves between components.

	The default algorithm used by this class to work out the order of traversal
	is as follows:
	- if two components both have an explicit focus order specified, then the
	  one with the lowest number comes first (see the Component::setExplicitFocusOrder()
	  method).
	- any component with an explicit focus order greater than 0 comes before ones
	  that don't have an order specified.
	- any unspecified components are traversed in a left-to-right, then top-to-bottom
	  order.

	If you need traversal in a more customised way, you can create a subclass
	of KeyboardFocusTraverser that uses your own algorithm, and use
	Component::createFocusTraverser() to create it.

	@see Component::setExplicitFocusOrder, Component::createFocusTraverser
*/
class JUCE_API  KeyboardFocusTraverser
{
public:
	KeyboardFocusTraverser();

	/** Destructor. */
	virtual ~KeyboardFocusTraverser();

	/** Returns the component that should be given focus after the specified one
		when moving "forwards".

		The default implementation will return the next component which is to the
		right of or below this one.

		This may return 0 if there's no suitable candidate.
	*/
	virtual Component* getNextComponent (Component* current);

	/** Returns the component that should be given focus after the specified one
		when moving "backwards".

		The default implementation will return the next component which is to the
		left of or above this one.

		This may return 0 if there's no suitable candidate.
	*/
	virtual Component* getPreviousComponent (Component* current);

	/** Returns the component that should receive focus be default within the given
		parent component.

		The default implementation will just return the foremost child component that
		wants focus.

		This may return 0 if there's no suitable candidate.
	*/
	virtual Component* getDefaultComponent (Component* parentComponent);
};

#endif   // __JUCE_KEYBOARDFOCUSTRAVERSER_JUCEHEADER__

/*** End of inlined file: juce_KeyboardFocusTraverser.h ***/


/*** Start of inlined file: juce_ModalComponentManager.h ***/
#ifndef __JUCE_MODALCOMPONENTMANAGER_JUCEHEADER__
#define __JUCE_MODALCOMPONENTMANAGER_JUCEHEADER__

/**
	Manages the system's stack of modal components.

	Normally you'll just use the Component methods to invoke modal states in components,
	and won't have to deal with this class directly, but this is the singleton object that's
	used internally to manage the stack.

	@see Component::enterModalState, Component::exitModalState, Component::isCurrentlyModal,
		 Component::getCurrentlyModalComponent, Component::isCurrentlyBlockedByAnotherModalComponent
*/
class JUCE_API  ModalComponentManager   : public AsyncUpdater,
										  public DeletedAtShutdown
{
public:

	/** Receives callbacks when a modal component is dismissed.

		You can register a callback using Component::enterModalState() or
		ModalComponentManager::attachCallback().

		For some quick ways of creating callback objects, see the ModalCallbackFunction class.
		@see ModalCallbackFunction
	*/
	class Callback
	{
	public:
		/** */
		Callback() {}

		/** Destructor. */
		virtual ~Callback() {}

		/** Called to indicate that a modal component has been dismissed.

			You can register a callback using Component::enterModalState() or
			ModalComponentManager::attachCallback().

			The returnValue parameter is the value that was passed to Component::exitModalState()
			when the component was dismissed.

			The callback object will be deleted shortly after this method is called.
		*/
		virtual void modalStateFinished (int returnValue) = 0;
	};

	/** Returns the number of components currently being shown modally.
		@see getModalComponent
	*/
	int getNumModalComponents() const;

	/** Returns one of the components being shown modally.
		An index of 0 is the most recently-shown, topmost component.
	*/
	Component* getModalComponent (int index) const;

	/** Returns true if the specified component is in a modal state. */
	bool isModal (Component* component) const;

	/** Returns true if the specified component is currently the topmost modal component. */
	bool isFrontModalComponent (Component* component) const;

	/** Adds a new callback that will be called when the specified modal component is dismissed.

		If the component is modal, then when it is dismissed, either by being hidden, or by calling
		Component::exitModalState(), then the Callback::modalStateFinished() method will be
		called.

		Each component can have any number of callbacks associated with it, and this one is added
		to that list.

		The object that is passed in will be deleted by the manager when it's no longer needed. If
		the given component is not currently modal, the callback object is deleted immediately and
		no action is taken.
	*/
	void attachCallback (Component* component, Callback* callback);

	/** Brings any modal components to the front. */
	void bringModalComponentsToFront (bool topOneShouldGrabFocus = true);

   #if JUCE_MODAL_LOOPS_PERMITTED
	/** Runs the event loop until the currently topmost modal component is dismissed, and
		returns the exit code for that component.
	*/
	int runEventLoopForCurrentComponent();
   #endif

	juce_DeclareSingleton_SingleThreaded_Minimal (ModalComponentManager);

protected:
	/** Creates a ModalComponentManager.
		You shouldn't ever call the constructor - it's a singleton, so use ModalComponentManager::getInstance()
	*/
	ModalComponentManager();

	/** Destructor. */
	~ModalComponentManager();

	/** @internal */
	void handleAsyncUpdate();

private:

	class ModalItem;
	class ReturnValueRetriever;

	friend class Component;
	friend class OwnedArray <ModalItem>;
	OwnedArray <ModalItem> stack;

	void startModal (Component*, bool autoDelete);
	void endModal (Component*, int returnValue);
	void endModal (Component*);

	JUCE_DECLARE_NON_COPYABLE (ModalComponentManager);
};

/**
	This class provides some handy utility methods for creating ModalComponentManager::Callback
	objects that will invoke a static function with some parameters when a modal component is dismissed.
*/
class ModalCallbackFunction
{
public:

	/** This is a utility function to create a ModalComponentManager::Callback that will
		call a static function with a parameter.

		The function that you supply must take two parameters - the first being an int, which is
		the result code that was used when the modal component was dismissed, and the second
		can be a custom type. Note that this custom value will be copied and stored, so it must
		be a primitive type or a class that provides copy-by-value semantics.

		E.g. @code
		static void myCallbackFunction (int modalResult, double customValue)
		{
			if (modalResult == 1)
				doSomethingWith (customValue);
		}

		Component* someKindOfComp;
		...
		someKindOfComp->enterModalState (ModalCallbackFunction::create (myCallbackFunction, 3.0));
		@endcode
		@see ModalComponentManager::Callback
	*/
	template <typename ParamType>
	static ModalComponentManager::Callback* create (void (*functionToCall) (int, ParamType),
													ParamType parameterValue)
	{
		return new FunctionCaller1 <ParamType> (functionToCall, parameterValue);
	}

	/** This is a utility function to create a ModalComponentManager::Callback that will
		call a static function with two custom parameters.

		The function that you supply must take three parameters - the first being an int, which is
		the result code that was used when the modal component was dismissed, and the next two are
		your custom types. Note that these custom values will be copied and stored, so they must
		be primitive types or classes that provide copy-by-value semantics.

		E.g. @code
		static void myCallbackFunction (int modalResult, double customValue1, String customValue2)
		{
			if (modalResult == 1)
				doSomethingWith (customValue1, customValue2);
		}

		Component* someKindOfComp;
		...
		someKindOfComp->enterModalState (ModalCallbackFunction::create (myCallbackFunction, 3.0, String ("xyz")));
		@endcode
		@see ModalComponentManager::Callback
	*/
	template <typename ParamType1, typename ParamType2>
	static ModalComponentManager::Callback* withParam (void (*functionToCall) (int, ParamType1, ParamType2),
													   ParamType1 parameterValue1,
													   ParamType2 parameterValue2)
	{
		return new FunctionCaller2 <ParamType1, ParamType2> (functionToCall, parameterValue1, parameterValue2);
	}

	/** This is a utility function to create a ModalComponentManager::Callback that will
		call a static function with a component.

		The function that you supply must take two parameters - the first being an int, which is
		the result code that was used when the modal component was dismissed, and the second
		can be a Component class. The component will be stored as a WeakReference, so that if it gets
		deleted before this callback is invoked, the pointer that is passed to the function will be null.

		E.g. @code
		static void myCallbackFunction (int modalResult, Slider* mySlider)
		{
			if (modalResult == 1 && mySlider != nullptr) // (must check that mySlider isn't null in case it was deleted..)
				mySlider->setValue (0.0);
		}

		Component* someKindOfComp;
		Slider* mySlider;
		...
		someKindOfComp->enterModalState (ModalCallbackFunction::forComponent (myCallbackFunction, mySlider));
		@endcode
		@see ModalComponentManager::Callback
	*/
	template <class ComponentType>
	static ModalComponentManager::Callback* forComponent (void (*functionToCall) (int, ComponentType*),
														  ComponentType* component)
	{
		return new ComponentCaller1 <ComponentType> (functionToCall, component);
	}

	/** Creates a ModalComponentManager::Callback that will call a static function with a component.

		The function that you supply must take three parameters - the first being an int, which is
		the result code that was used when the modal component was dismissed, the second being a Component
		class, and the third being a custom type (which must be a primitive type or have copy-by-value semantics).
		The component will be stored as a WeakReference, so that if it gets deleted before this callback is
		invoked, the pointer that is passed into the function will be null.

		E.g. @code
		static void myCallbackFunction (int modalResult, Slider* mySlider, String customParam)
		{
			if (modalResult == 1 && mySlider != nullptr) // (must check that mySlider isn't null in case it was deleted..)
				mySlider->setName (customParam);
		}

		Component* someKindOfComp;
		Slider* mySlider;
		...
		someKindOfComp->enterModalState (ModalCallbackFunction::forComponent (myCallbackFunction, mySlider, String ("hello")));
		@endcode
		@see ModalComponentManager::Callback
	*/
	template <class ComponentType, typename ParamType>
	static ModalComponentManager::Callback* forComponent (void (*functionToCall) (int, ComponentType*, ParamType),
														  ComponentType* component,
														  ParamType param)
	{
		return new ComponentCaller2 <ComponentType, ParamType> (functionToCall, component, param);
	}

private:

	template <typename ParamType>
	class FunctionCaller1  : public ModalComponentManager::Callback
	{
	public:
		typedef void (*FunctionType) (int, ParamType);

		FunctionCaller1 (FunctionType& function_, ParamType& param_)
			: function (function_), param (param_) {}

		void modalStateFinished (int returnValue)  { function (returnValue, param); }

	private:
		const FunctionType function;
		ParamType param;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FunctionCaller1);
	};

	template <typename ParamType1, typename ParamType2>
	class FunctionCaller2  : public ModalComponentManager::Callback
	{
	public:
		typedef void (*FunctionType) (int, ParamType1, ParamType2);

		FunctionCaller2 (FunctionType& function_, ParamType1& param1_, ParamType2& param2_)
			: function (function_), param1 (param1_), param2 (param2_) {}

		void modalStateFinished (int returnValue)   { function (returnValue, param1, param2); }

	private:
		const FunctionType function;
		ParamType1 param1;
		ParamType2 param2;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FunctionCaller2);
	};

	template <typename ComponentType>
	class ComponentCaller1  : public ModalComponentManager::Callback
	{
	public:
		typedef void (*FunctionType) (int, ComponentType*);

		ComponentCaller1 (FunctionType& function_, ComponentType* comp_)
			: function (function_), comp (comp_) {}

		void modalStateFinished (int returnValue)
		{
			function (returnValue, static_cast <ComponentType*> (comp.get()));
		}

	private:
		const FunctionType function;
		WeakReference<Component> comp;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentCaller1);
	};

	template <typename ComponentType, typename ParamType1>
	class ComponentCaller2  : public ModalComponentManager::Callback
	{
	public:
		typedef void (*FunctionType) (int, ComponentType*, ParamType1);

		ComponentCaller2 (FunctionType& function_, ComponentType* comp_, ParamType1 param1_)
			: function (function_), comp (comp_), param1 (param1_) {}

		void modalStateFinished (int returnValue)
		{
			function (returnValue, static_cast <ComponentType*> (comp.get()), param1);
		}

	private:
		const FunctionType function;
		WeakReference<Component> comp;
		ParamType1 param1;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentCaller2);
	};

	ModalCallbackFunction();
	~ModalCallbackFunction();
	JUCE_DECLARE_NON_COPYABLE (ModalCallbackFunction);
};

#endif   // __JUCE_MODALCOMPONENTMANAGER_JUCEHEADER__

/*** End of inlined file: juce_ModalComponentManager.h ***/

class LookAndFeel;
class MouseInputSource;
class MouseInputSourceInternal;
class ComponentPeer;
class MarkerList;
class RelativeRectangle;
class CachedComponentImage;

/**
	The base class for all JUCE user-interface objects.

*/
class JUCE_API  Component  : public MouseListener
{
public:

	/** Creates a component.

		To get it to actually appear, you'll also need to:
		- Either add it to a parent component or use the addToDesktop() method to
		  make it a desktop window
		- Set its size and position to something sensible
		- Use setVisible() to make it visible

		And for it to serve any useful purpose, you'll need to write a
		subclass of Component or use one of the other types of component from
		the library.
	*/
	Component();

	/** Destructor.

		Note that when a component is deleted, any child components it contains are NOT
		automatically deleted. It's your responsibilty to manage their lifespan - you
		may want to use helper methods like deleteAllChildren(), or less haphazard
		approaches like using ScopedPointers or normal object aggregation to manage them.

		If the component being deleted is currently the child of another one, then during
		deletion, it will be removed from its parent, and the parent will receive a childrenChanged()
		callback. Any ComponentListener objects that have registered with it will also have their
		ComponentListener::componentBeingDeleted() methods called.
	*/
	virtual ~Component();

	/** Creates a component, setting its name at the same time.

		@see getName, setName
	*/
	explicit Component (const String& componentName);

	/** Returns the name of this component.

		@see setName
	*/
	const String& getName() const noexcept                  { return componentName; }

	/** Sets the name of this component.

		When the name changes, all registered ComponentListeners will receive a
		ComponentListener::componentNameChanged() callback.

		@see getName
	*/
	virtual void setName (const String& newName);

	/** Returns the ID string that was set by setComponentID().
		@see setComponentID, findChildWithID
	*/
	const String& getComponentID() const noexcept           { return componentID; }

	/** Sets the component's ID string.
		You can retrieve the ID using getComponentID().
		@see getComponentID, findChildWithID
	*/
	void setComponentID (const String& newID);

	/** Makes the component visible or invisible.

		This method will show or hide the component.
		Note that components default to being non-visible when first created.
		Also note that visible components won't be seen unless all their parent components
		are also visible.

		This method will call visibilityChanged() and also componentVisibilityChanged()
		for any component listeners that are interested in this component.

		@param shouldBeVisible  whether to show or hide the component
		@see isVisible, isShowing, visibilityChanged, ComponentListener::componentVisibilityChanged
	*/
	virtual void setVisible (bool shouldBeVisible);

	/** Tests whether the component is visible or not.

		this doesn't necessarily tell you whether this comp is actually on the screen
		because this depends on whether all the parent components are also visible - use
		isShowing() to find this out.

		@see isShowing, setVisible
	*/
	bool isVisible() const noexcept                         { return flags.visibleFlag; }

	/** Called when this component's visiblility changes.

		@see setVisible, isVisible
	*/
	virtual void visibilityChanged();

	/** Tests whether this component and all its parents are visible.

		@returns    true only if this component and all its parents are visible.
		@see isVisible
	*/
	bool isShowing() const;

	/** Makes this component appear as a window on the desktop.

		Note that before calling this, you should make sure that the component's opacity is
		set correctly using setOpaque(). If the component is non-opaque, the windowing
		system will try to create a special transparent window for it, which will generally take
		a lot more CPU to operate (and might not even be possible on some platforms).

		If the component is inside a parent component at the time this method is called, it
		will be first be removed from that parent. Likewise if a component on the desktop
		is subsequently added to another component, it'll be removed from the desktop.

		@param windowStyleFlags             a combination of the flags specified in the
											ComponentPeer::StyleFlags enum, which define the
											window's characteristics.
		@param nativeWindowToAttachTo       this allows an OS object to be passed-in as the window
											in which the juce component should place itself. On Windows,
											this would be a HWND, a HIViewRef on the Mac. Not necessarily
											supported on all platforms, and best left as 0 unless you know
											what you're doing
		@see removeFromDesktop, isOnDesktop, userTriedToCloseWindow,
			 getPeer, ComponentPeer::setMinimised, ComponentPeer::StyleFlags,
			 ComponentPeer::getStyleFlags, ComponentPeer::setFullScreen
	*/
	virtual void addToDesktop (int windowStyleFlags,
							   void* nativeWindowToAttachTo = nullptr);

	/** If the component is currently showing on the desktop, this will hide it.

		You can also use setVisible() to hide a desktop window temporarily, but
		removeFromDesktop() will free any system resources that are being used up.

		@see addToDesktop, isOnDesktop
	*/
	void removeFromDesktop();

	/** Returns true if this component is currently showing on the desktop.

		@see addToDesktop, removeFromDesktop
	*/
	bool isOnDesktop() const noexcept;

	/** Returns the heavyweight window that contains this component.

		If this component is itself on the desktop, this will return the window
		object that it is using. Otherwise, it will return the window of
		its top-level parent component.

		This may return 0 if there isn't a desktop component.

		@see addToDesktop, isOnDesktop
	*/
	ComponentPeer* getPeer() const;

	/** For components on the desktop, this is called if the system wants to close the window.

		This is a signal that either the user or the system wants the window to close. The
		default implementation of this method will trigger an assertion to warn you that your
		component should do something about it, but you can override this to ignore the event
		if you want.
	*/
	virtual void userTriedToCloseWindow();

	/** Called for a desktop component which has just been minimised or un-minimised.

		This will only be called for components on the desktop.

		@see getPeer, ComponentPeer::setMinimised, ComponentPeer::isMinimised
	*/
	virtual void minimisationStateChanged (bool isNowMinimised);

	/** Brings the component to the front of its siblings.

		If some of the component's siblings have had their 'always-on-top' flag set,
		then they will still be kept in front of this one (unless of course this
		one is also 'always-on-top').

		@param shouldAlsoGainFocus  if true, this will also try to assign keyboard focus
									to the component (see grabKeyboardFocus() for more details)
		@see toBack, toBehind, setAlwaysOnTop
	*/
	void toFront (bool shouldAlsoGainFocus);

	/** Changes this component's z-order to be at the back of all its siblings.

		If the component is set to be 'always-on-top', it will only be moved to the
		back of the other other 'always-on-top' components.

		@see toFront, toBehind, setAlwaysOnTop
	*/
	void toBack();

	/** Changes this component's z-order so that it's just behind another component.

		@see toFront, toBack
	*/
	void toBehind (Component* other);

	/** Sets whether the component should always be kept at the front of its siblings.

		@see isAlwaysOnTop
	*/
	void setAlwaysOnTop (bool shouldStayOnTop);

	/** Returns true if this component is set to always stay in front of its siblings.

		@see setAlwaysOnTop
	*/
	bool isAlwaysOnTop() const noexcept;

	/** Returns the x coordinate of the component's left edge.
		This is a distance in pixels from the left edge of the component's parent.

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to its bounding box.
	*/
	inline int getX() const noexcept                        { return bounds.getX(); }

	/** Returns the y coordinate of the top of this component.
		This is a distance in pixels from the top edge of the component's parent.

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to its bounding box.
	*/
	inline int getY() const noexcept                        { return bounds.getY(); }

	/** Returns the component's width in pixels. */
	inline int getWidth() const noexcept                    { return bounds.getWidth(); }

	/** Returns the component's height in pixels. */
	inline int getHeight() const noexcept                   { return bounds.getHeight(); }

	/** Returns the x coordinate of the component's right-hand edge.
		This is a distance in pixels from the left edge of the component's parent.

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to its bounding box.
	*/
	int getRight() const noexcept                           { return bounds.getRight(); }

	/** Returns the component's top-left position as a Point. */
	const Point<int>& getPosition() const noexcept          { return bounds.getPosition(); }

	/** Returns the y coordinate of the bottom edge of this component.
		This is a distance in pixels from the top edge of the component's parent.

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to its bounding box.
	*/
	int getBottom() const noexcept                          { return bounds.getBottom(); }

	/** Returns this component's bounding box.
		The rectangle returned is relative to the top-left of the component's parent.

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to its bounding box.
	*/
	const Rectangle<int>& getBounds() const noexcept        { return bounds; }

	/** Returns the component's bounds, relative to its own origin.
		This is like getBounds(), but returns the rectangle in local coordinates, In practice, it'll
		return a rectangle with position (0, 0), and the same size as this component.
	*/
	Rectangle<int> getLocalBounds() const noexcept;

	/** Returns the area of this component's parent which this component covers.

		The returned area is relative to the parent's coordinate space.
		If the component has an affine transform specified, then the resulting area will be
		the smallest rectangle that fully covers the component's transformed bounding box.
		If this component has no parent, the return value will simply be the same as getBounds().
	*/
	Rectangle<int> getBoundsInParent() const noexcept;

	/** Returns the region of this component that's not obscured by other, opaque components.

		The RectangleList that is returned represents the area of this component
		which isn't covered by opaque child components.

		If includeSiblings is true, it will also take into account any siblings
		that may be overlapping the component.
	*/
	void getVisibleArea (RectangleList& result, bool includeSiblings) const;

	/** Returns this component's x coordinate relative the the screen's top-left origin.
		@see getX, localPointToGlobal
	*/
	int getScreenX() const;

	/** Returns this component's y coordinate relative the the screen's top-left origin.
		@see getY, localPointToGlobal
	*/
	int getScreenY() const;

	/** Returns the position of this component's top-left corner relative to the screen's top-left.
		@see getScreenBounds
	*/
	Point<int> getScreenPosition() const;

	/** Returns the bounds of this component, relative to the screen's top-left.
		@see getScreenPosition
	*/
	Rectangle<int> getScreenBounds() const;

	/** Converts a point to be relative to this component's coordinate space.

		This takes a point relative to a different component, and returns its position relative to this
		component. If the sourceComponent parameter is null, the source point is assumed to be a global
		screen coordinate.
	*/
	Point<int> getLocalPoint (const Component* sourceComponent,
							  const Point<int>& pointRelativeToSourceComponent) const;

	/** Converts a rectangle to be relative to this component's coordinate space.

		This takes a rectangle that is relative to a different component, and returns its position relative
		to this component. If the sourceComponent parameter is null, the source rectangle is assumed to be
		a screen coordinate.

		If you've used setTransform() to apply one or more transforms to components, then the source rectangle
		may not actually be rectanglular when converted to the target space, so in that situation this will return
		the smallest rectangle that fully contains the transformed area.
	*/
	Rectangle<int> getLocalArea (const Component* sourceComponent,
								 const Rectangle<int>& areaRelativeToSourceComponent) const;

	/** Converts a point relative to this component's top-left into a screen coordinate.
		@see getLocalPoint, localAreaToGlobal
	*/
	Point<int> localPointToGlobal (const Point<int>& localPoint) const;

	/** Converts a rectangle from this component's coordinate space to a screen coordinate.

		If you've used setTransform() to apply one or more transforms to components, then the source rectangle
		may not actually be rectanglular when converted to the target space, so in that situation this will return
		the smallest rectangle that fully contains the transformed area.
		@see getLocalPoint, localPointToGlobal
	*/
	Rectangle<int> localAreaToGlobal (const Rectangle<int>& localArea) const;

	/** Moves the component to a new position.

		Changes the component's top-left position (without changing its size).
		The position is relative to the top-left of the component's parent.

		If the component actually moves, this method will make a synchronous call to moved().

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to whatever bounds you set for it.

		@see setBounds, ComponentListener::componentMovedOrResized
	*/
	void setTopLeftPosition (int x, int y);

	/** Moves the component to a new position.

		Changes the component's top-left position (without changing its size).
		The position is relative to the top-left of the component's parent.

		If the component actually moves, this method will make a synchronous call to moved().

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to whatever bounds you set for it.

		@see setBounds, ComponentListener::componentMovedOrResized
	*/
	void setTopLeftPosition (const Point<int>& newTopLeftPosition);

	/** Moves the component to a new position.

		Changes the position of the component's top-right corner (keeping it the same size).
		The position is relative to the top-left of the component's parent.

		If the component actually moves, this method will make a synchronous call to moved().

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to whatever bounds you set for it.
	*/
	void setTopRightPosition (int x, int y);

	/** Changes the size of the component.

		A synchronous call to resized() will be occur if the size actually changes.

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to whatever bounds you set for it.
	*/
	void setSize (int newWidth, int newHeight);

	/** Changes the component's position and size.

		The coordinates are relative to the top-left of the component's parent, or relative
		to the origin of the screen is the component is on the desktop.

		If this method changes the component's top-left position, it will make a synchronous
		call to moved(). If it changes the size, it will also make a call to resized().

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to whatever bounds you set for it.

		@see setTopLeftPosition, setSize, ComponentListener::componentMovedOrResized
	*/
	void setBounds (int x, int y, int width, int height);

	/** Changes the component's position and size.

		The coordinates are relative to the top-left of the component's parent, or relative
		to the origin of the screen is the component is on the desktop.

		If this method changes the component's top-left position, it will make a synchronous
		call to moved(). If it changes the size, it will also make a call to resized().

		Note that if you've used setTransform() to apply a transform, then the component's
		bounds will no longer be a direct reflection of the position at which it appears within
		its parent, as the transform will be applied to whatever bounds you set for it.

		@see setBounds
	*/
	void setBounds (const Rectangle<int>& newBounds);

	/** Changes the component's position and size.

		This is similar to the other setBounds() methods, but uses RelativeRectangle::applyToComponent()
		to set the position, This uses a Component::Positioner to make sure that any dynamic
		expressions are used in the RelativeRectangle will be automatically re-applied to the
		component's bounds when the source values change. See RelativeRectangle::applyToComponent()
		for more details.

		When using relative expressions, the following symbols are available:
		 - "left", "right", "top", "bottom" refer to the position of those edges in this component, so
		   e.g. for a component whose width is always 100, you might set the right edge to the "left + 100".
		 - "[id].left", "[id].right", "[id].top", "[id].bottom", "[id].width", "[id].height", where [id] is
		   the identifier of one of this component's siblings. A component's identifier is set with
		   Component::setComponentID(). So for example if you want your component to always be 50 pixels to the
		   right of the one called "xyz", you could set your left edge to be "xyz.right + 50".
		 - Instead of an [id], you can use the name "parent" to refer to this component's parent. Like
		   any other component, these values are relative to their component's parent, so "parent.right" won't be
		   very useful for positioning a component because it refers to a position with the parent's parent.. but
		   "parent.width" can be used for setting positions relative to the parent's size. E.g. to make a 10x10
		   component which remains 1 pixel away from its parent's bottom-right, you could use
		   "right - 10, bottom - 10, parent.width - 1, parent.height - 1".
		 - The name of one of the parent component's markers can also be used as a symbol. For markers to be
		   used, the parent component must implement its Component::getMarkers() method, and return at least one
		   valid MarkerList. So if you want your component's top edge to be 10 pixels below the
		   marker called "foobar", you'd set it to "foobar + 10".

		See the Expression class for details about the operators that are supported, but for example
		if you wanted to make your component remain centred within its parent with a size of 100, 100,
		you could express it as:
		@code myComp.setBounds (RelativeBounds ("parent.width / 2 - 50, parent.height / 2 - 50, left + 100, top + 100"));
		@endcode
		..or an alternative way to achieve the same thing:
		@code myComp.setBounds (RelativeBounds ("right - 100, bottom - 100, parent.width / 2 + 50, parent.height / 2 + 50"));
		@endcode

		Or if you wanted a 100x100 component whose top edge is lined up to a marker called "topMarker" and
		which is positioned 50 pixels to the right of another component called "otherComp", you could write:
		@code myComp.setBounds (RelativeBounds ("otherComp.right + 50, topMarker, left + 100, top + 100"));
		@endcode

		Be careful not to make your coordinate expressions recursive, though, or exceptions and assertions will
		be thrown!

		@see setBounds, RelativeRectangle::applyToComponent(), Expression
	*/
	void setBounds (const RelativeRectangle& newBounds);

	/** Sets the component's bounds with an expression.
		The string is parsed as a RelativeRectangle expression - see the notes for
		Component::setBounds (const RelativeRectangle&) for more information. This method is
		basically just a shortcut for writing setBounds (RelativeRectangle ("..."))
	*/
	void setBounds (const String& newBoundsExpression);

	/** Changes the component's position and size in terms of fractions of its parent's size.

		The values are factors of the parent's size, so for example
		setBoundsRelative (0.2f, 0.2f, 0.5f, 0.5f) would give it half the
		width and height of the parent, with its top-left position 20% of
		the way across and down the parent.

		@see setBounds
	*/
	void setBoundsRelative (float proportionalX, float proportionalY,
							float proportionalWidth, float proportionalHeight);

	/** Changes the component's position and size based on the amount of space to leave around it.

		This will position the component within its parent, leaving the specified number of
		pixels around each edge.

		@see setBounds
	*/
	void setBoundsInset (const BorderSize<int>& borders);

	/** Positions the component within a given rectangle, keeping its proportions
		unchanged.

		If onlyReduceInSize is false, the component will be resized to fill as much of the
		rectangle as possible without changing its aspect ratio (the component's
		current size is used to determine its aspect ratio, so a zero-size component
		won't work here). If onlyReduceInSize is true, it will only be resized if it's
		too big to fit inside the rectangle.

		It will then be positioned within the rectangle according to the justification flags
		specified.

		@see setBounds
	*/
	void setBoundsToFit (int x, int y, int width, int height,
						 const Justification& justification,
						 bool onlyReduceInSize);

	/** Changes the position of the component's centre.

		Leaves the component's size unchanged, but sets the position of its centre
		relative to its parent's top-left.

		@see setBounds
	*/
	void setCentrePosition (int x, int y);

	/** Changes the position of the component's centre.

		Leaves the position unchanged, but positions its centre relative to its
		parent's size. E.g. setCentreRelative (0.5f, 0.5f) would place it centrally in
		its parent.
	*/
	void setCentreRelative (float x, float y);

	/** Changes the component's size and centres it within its parent.

		After changing the size, the component will be moved so that it's
		centred within its parent. If the component is on the desktop (or has no
		parent component), then it'll be centred within the main monitor area.
	*/
	void centreWithSize (int width, int height);

	/** Sets a transform matrix to be applied to this component.

		If you set a transform for a component, the component's position will be warped by it, relative to
		the component's parent's top-left origin. This means that the values you pass into setBounds() will no
		longer reflect the actual area within the parent that the component covers, as the bounds will be
		transformed and the component will probably end up actually appearing somewhere else within its parent.

		When using transforms you need to be extremely careful when converting coordinates between the
		coordinate spaces of different components or the screen - you should always use getLocalPoint(),
		getLocalArea(), etc to do this, and never just manually add a component's position to a point in order to
		convert it between different components (but I'm sure you would never have done that anyway...).

		Currently, transforms are not supported for desktop windows, so the transform will be ignored if you
		put a component on the desktop.

		To remove a component's transform, simply pass AffineTransform::identity as the parameter to this method.
	*/
	void setTransform (const AffineTransform& transform);

	/** Returns the transform that is currently being applied to this component.
		For more details about transforms, see setTransform().
		@see setTransform
	*/
	AffineTransform getTransform() const;

	/** Returns true if a non-identity transform is being applied to this component.
		For more details about transforms, see setTransform().
		@see setTransform
	*/
	bool isTransformed() const noexcept;

	/** Returns a proportion of the component's width.

		This is a handy equivalent of (getWidth() * proportion).
	*/
	int proportionOfWidth (float proportion) const noexcept;

	/** Returns a proportion of the component's height.

		This is a handy equivalent of (getHeight() * proportion).
	*/
	int proportionOfHeight (float proportion) const noexcept;

	/** Returns the width of the component's parent.

		If the component has no parent (i.e. if it's on the desktop), this will return
		the width of the screen.
	*/
	int getParentWidth() const noexcept;

	/** Returns the height of the component's parent.

		If the component has no parent (i.e. if it's on the desktop), this will return
		the height of the screen.
	*/
	int getParentHeight() const noexcept;

	/** Returns the screen coordinates of the monitor that contains this component.

		If there's only one monitor, this will return its size - if there are multiple
		monitors, it will return the area of the monitor that contains the component's
		centre.
	*/
	Rectangle<int> getParentMonitorArea() const;

	/** Returns the number of child components that this component contains.

		@see getChildComponent, getIndexOfChildComponent
	*/
	int getNumChildComponents() const noexcept;

	/** Returns one of this component's child components, by it index.

		The component with index 0 is at the back of the z-order, the one at the
		front will have index (getNumChildComponents() - 1).

		If the index is out-of-range, this will return a null pointer.

		@see getNumChildComponents, getIndexOfChildComponent
	*/
	Component* getChildComponent (int index) const noexcept;

	/** Returns the index of this component in the list of child components.

		A value of 0 means it is first in the list (i.e. behind all other components). Higher
		values are further towards the front.

		Returns -1 if the component passed-in is not a child of this component.

		@see getNumChildComponents, getChildComponent, addChildComponent, toFront, toBack, toBehind
	*/
	int getIndexOfChildComponent (const Component* child) const noexcept;

	/** Looks for a child component with the specified ID.
		@see setComponentID, getComponentID
	*/
	Component* findChildWithID (const String& componentID) const noexcept;

	/** Adds a child component to this one.

		Adding a child component does not mean that the component will own or delete the child - it's
		your responsibility to delete the component. Note that it's safe to delete a component
		without first removing it from its parent - doing so will automatically remove it and
		send out the appropriate notifications before the deletion completes.

		If the child is already a child of this component, then no action will be taken, and its
		z-order will be left unchanged.

		@param child    the new component to add. If the component passed-in is already
						the child of another component, it'll first be removed from it current parent.
		@param zOrder   The index in the child-list at which this component should be inserted.
						A value of -1 will insert it in front of the others, 0 is the back.
		@see removeChildComponent, addAndMakeVisible, addChildAndSetID, getChild, ComponentListener::componentChildrenChanged
	*/
	void addChildComponent (Component* child, int zOrder = -1);

	/** Adds a child component to this one, and also makes the child visible if it isn't.

		Quite a useful function, this is just the same as calling setVisible (true) on the child
		and then addChildComponent(). See addChildComponent() for more details.
	*/
	void addAndMakeVisible (Component* child, int zOrder = -1);

	/** Adds a child component to this one, makes it visible, and sets its component ID.
		@see addAndMakeVisible, addChildComponent
	*/
	void addChildAndSetID (Component* child, const String& componentID);

	/** Removes one of this component's child-components.

		If the child passed-in isn't actually a child of this component (either because
		it's invalid or is the child of a different parent), then no action is taken.

		Note that removing a child will not delete it! But it's ok to delete a component
		without first removing it - doing so will automatically remove it and send out the
		appropriate notifications before the deletion completes.

		@see addChildComponent, ComponentListener::componentChildrenChanged
	*/
	void removeChildComponent (Component* childToRemove);

	/** Removes one of this component's child-components by index.

		This will return a pointer to the component that was removed, or null if
		the index was out-of-range.

		Note that removing a child will not delete it! But it's ok to delete a component
		without first removing it - doing so will automatically remove it and send out the
		appropriate notifications before the deletion completes.

		@see addChildComponent, ComponentListener::componentChildrenChanged
	*/
	Component* removeChildComponent (int childIndexToRemove);

	/** Removes all this component's children.

		Note that this won't delete them! To do that, use deleteAllChildren() instead.
	*/
	void removeAllChildren();

	/** Removes all this component's children, and deletes them.

		@see removeAllChildren
	*/
	void deleteAllChildren();

	/** Returns the component which this component is inside.

		If this is the highest-level component or hasn't yet been added to
		a parent, this will return null.
	*/
	Component* getParentComponent() const noexcept                  { return parentComponent; }

	/** Searches the parent components for a component of a specified class.

		For example findParentComponentOfClass \<MyComp\>() would return the first parent
		component that can be dynamically cast to a MyComp, or will return 0 if none
		of the parents are suitable.

		N.B. The dummy parameter is needed to work around a VC6 compiler bug.
	*/
	template <class TargetClass>
	TargetClass* findParentComponentOfClass (TargetClass* const dummyParameter = nullptr) const
	{
		(void) dummyParameter;

		for (Component* p = parentComponent; p != nullptr; p = p->parentComponent)
		{
			TargetClass* const target = dynamic_cast <TargetClass*> (p);
			if (target != nullptr)
				return target;
		}

		return nullptr;
	}

	/** Returns the highest-level component which contains this one or its parents.

		This will search upwards in the parent-hierarchy from this component, until it
		finds the highest one that doesn't have a parent (i.e. is on the desktop or
		not yet added to a parent), and will return that.
	*/
	Component* getTopLevelComponent() const noexcept;

	/** Checks whether a component is anywhere inside this component or its children.

		This will recursively check through this component's children to see if the
		given component is anywhere inside.
	*/
	bool isParentOf (const Component* possibleChild) const noexcept;

	/** Called to indicate that the component's parents have changed.

		When a component is added or removed from its parent, this method will
		be called on all of its children (recursively - so all children of its
		children will also be called as well).

		Subclasses can override this if they need to react to this in some way.

		@see getParentComponent, isShowing, ComponentListener::componentParentHierarchyChanged
	*/
	virtual void parentHierarchyChanged();

	/** Subclasses can use this callback to be told when children are added or removed.

		@see parentHierarchyChanged
	*/
	virtual void childrenChanged();

	/** Tests whether a given point inside the component.

		Overriding this method allows you to create components which only intercept
		mouse-clicks within a user-defined area.

		This is called to find out whether a particular x, y coordinate is
		considered to be inside the component or not, and is used by methods such
		as contains() and getComponentAt() to work out which component
		the mouse is clicked on.

		Components with custom shapes will probably want to override it to perform
		some more complex hit-testing.

		The default implementation of this method returns either true or false,
		depending on the value that was set by calling setInterceptsMouseClicks() (true
		is the default return value).

		Note that the hit-test region is not related to the opacity with which
		areas of a component are painted.

		Applications should never call hitTest() directly - instead use the
		contains() method, because this will also test for occlusion by the
		component's parent.

		Note that for components on the desktop, this method will be ignored, because it's
		not always possible to implement this behaviour on all platforms.

		@param x    the x coordinate to test, relative to the left hand edge of this
					component. This value is guaranteed to be greater than or equal to
					zero, and less than the component's width
		@param y    the y coordinate to test, relative to the top edge of this
					component. This value is guaranteed to be greater than or equal to
					zero, and less than the component's height
		@returns    true if the click is considered to be inside the component
		@see setInterceptsMouseClicks, contains
	*/
	virtual bool hitTest (int x, int y);

	/** Changes the default return value for the hitTest() method.

		Setting this to false is an easy way to make a component pass its mouse-clicks
		through to the components behind it.

		When a component is created, the default setting for this is true.

		@param allowClicksOnThisComponent   if true, hitTest() will always return true; if false, it will
											return false (or true for child components if allowClicksOnChildComponents
											is true)
		@param allowClicksOnChildComponents if this is true and allowClicksOnThisComponent is false, then child
											components can be clicked on as normal but clicks on this component pass
											straight through; if this is false and allowClicksOnThisComponent
											is false, then neither this component nor any child components can
											be clicked on
		@see hitTest, getInterceptsMouseClicks
	*/
	void setInterceptsMouseClicks (bool allowClicksOnThisComponent,
								   bool allowClicksOnChildComponents) noexcept;

	/** Retrieves the current state of the mouse-click interception flags.

		On return, the two parameters are set to the state used in the last call to
		setInterceptsMouseClicks().

		@see setInterceptsMouseClicks
	*/
	void getInterceptsMouseClicks (bool& allowsClicksOnThisComponent,
								   bool& allowsClicksOnChildComponents) const noexcept;

	/** Returns true if a given point lies within this component or one of its children.

		Never override this method! Use hitTest to create custom hit regions.

		@param localPoint    the coordinate to test, relative to this component's top-left.
		@returns    true if the point is within the component's hit-test area, but only if
					that part of the component isn't clipped by its parent component. Note
					that this won't take into account any overlapping sibling components
					which might be in the way - for that, see reallyContains()
		@see hitTest, reallyContains, getComponentAt
	*/
	bool contains (const Point<int>& localPoint);

	/** Returns true if a given point lies in this component, taking any overlapping
		siblings into account.

		@param localPoint    the coordinate to test, relative to this component's top-left.
		@param returnTrueIfWithinAChild     if the point actually lies within a child of this component,
											this determines whether that is counted as a hit.
		@see contains, getComponentAt
	*/
	bool reallyContains (const Point<int>& localPoint, bool returnTrueIfWithinAChild);

	/** Returns the component at a certain point within this one.

		@param x    the x coordinate to test, relative to this component's left edge.
		@param y    the y coordinate to test, relative to this component's top edge.
		@returns    the component that is at this position - which may be 0, this component,
					or one of its children. Note that overlapping siblings that might actually
					be in the way are not taken into account by this method - to account for these,
					instead call getComponentAt on the top-level parent of this component.
		@see hitTest, contains, reallyContains
	*/
	Component* getComponentAt (int x, int y);

	/** Returns the component at a certain point within this one.

		@param position  the coordinate to test, relative to this component's top-left.
		@returns    the component that is at this position - which may be 0, this component,
					or one of its children. Note that overlapping siblings that might actually
					be in the way are not taken into account by this method - to account for these,
					instead call getComponentAt on the top-level parent of this component.
		@see hitTest, contains, reallyContains
	*/
	Component* getComponentAt (const Point<int>& position);

	/** Marks the whole component as needing to be redrawn.

		Calling this will not do any repainting immediately, but will mark the component
		as 'dirty'. At some point in the near future the operating system will send a paint
		message, which will redraw all the dirty regions of all components.
		There's no guarantee about how soon after calling repaint() the redraw will actually
		happen, and other queued events may be delivered before a redraw is done.

		If the setBufferedToImage() method has been used to cause this component to use a
		buffer, the repaint() call will invalidate the cached buffer. If setCachedComponentImage()
		has been used to provide a custom image cache, that cache will be invalidated appropriately.

		To redraw just a subsection of the component rather than the whole thing,
		use the repaint (int, int, int, int) method.

		@see paint
	*/
	void repaint();

	/** Marks a subsection of this component as needing to be redrawn.

		Calling this will not do any repainting immediately, but will mark the given region
		of the component as 'dirty'. At some point in the near future the operating system
		will send a paint message, which will redraw all the dirty regions of all components.
		There's no guarantee about how soon after calling repaint() the redraw will actually
		happen, and other queued events may be delivered before a redraw is done.

		The region that is passed in will be clipped to keep it within the bounds of this
		component.

		@see repaint()
	*/
	void repaint (int x, int y, int width, int height);

	/** Marks a subsection of this component as needing to be redrawn.

		Calling this will not do any repainting immediately, but will mark the given region
		of the component as 'dirty'. At some point in the near future the operating system
		will send a paint message, which will redraw all the dirty regions of all components.
		There's no guarantee about how soon after calling repaint() the redraw will actually
		happen, and other queued events may be delivered before a redraw is done.

		The region that is passed in will be clipped to keep it within the bounds of this
		component.

		@see repaint()
	*/
	void repaint (const Rectangle<int>& area);

	/** Makes the component use an internal buffer to optimise its redrawing.

		Setting this flag to true will cause the component to allocate an
		internal buffer into which it paints itself, so that when asked to
		redraw itself, it can use this buffer rather than actually calling the
		paint() method.

		The buffer is kept until the repaint() method is called directly on
		this component (or until it is resized), when the image is invalidated
		and then redrawn the next time the component is painted.

		Note that only the drawing that happens within the component's paint()
		method is drawn into the buffer, it's child components are not buffered, and
		nor is the paintOverChildren() method.

		@see repaint, paint, createComponentSnapshot
	*/
	void setBufferedToImage (bool shouldBeBuffered);

	/** Generates a snapshot of part of this component.

		This will return a new Image, the size of the rectangle specified,
		containing a snapshot of the specified area of the component and all
		its children.

		The image may or may not have an alpha-channel, depending on whether the
		image is opaque or not.

		If the clipImageToComponentBounds parameter is true and the area is greater than
		the size of the component, it'll be clipped. If clipImageToComponentBounds is false
		then parts of the component beyond its bounds can be drawn.

		@see paintEntireComponent
	*/
	Image createComponentSnapshot (const Rectangle<int>& areaToGrab,
								   bool clipImageToComponentBounds = true);

	/** Draws this component and all its subcomponents onto the specified graphics
		context.

		You should very rarely have to use this method, it's simply there in case you need
		to draw a component with a custom graphics context for some reason, e.g. for
		creating a snapshot of the component.

		It calls paint(), paintOverChildren() and recursively calls paintEntireComponent()
		on its children in order to render the entire tree.

		The graphics context may be left in an undefined state after this method returns,
		so you may need to reset it if you're going to use it again.

		If ignoreAlphaLevel is false, then the component will be drawn with the opacity level
		specified by getAlpha(); if ignoreAlphaLevel is true, then this will be ignored and
		an alpha of 1.0 will be used.
	*/
	void paintEntireComponent (Graphics& context, bool ignoreAlphaLevel);

	/** This allows you to indicate that this component doesn't require its graphics
		context to be clipped when it is being painted.

		Most people will never need to use this setting, but in situations where you have a very large
		number of simple components being rendered, and where they are guaranteed never to do any drawing
		beyond their own boundaries, setting this to true will reduce the overhead involved in clipping
		the graphics context that gets passed to the component's paint() callback.
		If you enable this mode, you'll need to make sure your paint method doesn't call anything like
		Graphics::fillAll(), and doesn't draw beyond the component's bounds, because that'll produce
		artifacts. Your component also can't have any child components that may be placed beyond its
		bounds.
	*/
	void setPaintingIsUnclipped (bool shouldPaintWithoutClipping) noexcept;

	/** Adds an effect filter to alter the component's appearance.

		When a component has an effect filter set, then this is applied to the
		results of its paint() method. There are a few preset effects, such as
		a drop-shadow or glow, but they can be user-defined as well.

		The effect that is passed in will not be deleted by the component - the
		caller must take care of deleting it.

		To remove an effect from a component, pass a null pointer in as the parameter.

		@see ImageEffectFilter, DropShadowEffect, GlowEffect
	*/
	void setComponentEffect (ImageEffectFilter* newEffect);

	/** Returns the current component effect.

		@see setComponentEffect
	*/
	ImageEffectFilter* getComponentEffect() const noexcept              { return effect; }

	/** Finds the appropriate look-and-feel to use for this component.

		If the component hasn't had a look-and-feel explicitly set, this will
		return the parent's look-and-feel, or just the default one if there's no
		parent.

		@see setLookAndFeel, lookAndFeelChanged
	*/
	LookAndFeel& getLookAndFeel() const noexcept;

	/** Sets the look and feel to use for this component.

		This will also change the look and feel for any child components that haven't
		had their look set explicitly.

		The object passed in will not be deleted by the component, so it's the caller's
		responsibility to manage it. It may be used at any time until this component
		has been deleted.

		Calling this method will also invoke the sendLookAndFeelChange() method.

		@see getLookAndFeel, lookAndFeelChanged
	*/
	void setLookAndFeel (LookAndFeel* newLookAndFeel);

	/** Called to let the component react to a change in the look-and-feel setting.

		When the look-and-feel is changed for a component, this will be called in
		all its child components, recursively.

		It can also be triggered manually by the sendLookAndFeelChange() method, in case
		an application uses a LookAndFeel class that might have changed internally.

		@see sendLookAndFeelChange, getLookAndFeel
	*/
	virtual void lookAndFeelChanged();

	/** Calls the lookAndFeelChanged() method in this component and all its children.

		This will recurse through the children and their children, calling lookAndFeelChanged()
		on them all.

		@see lookAndFeelChanged
	*/
	void sendLookAndFeelChange();

	/** Indicates whether any parts of the component might be transparent.

		Components that always paint all of their contents with solid colour and
		thus completely cover any components behind them should use this method
		to tell the repaint system that they are opaque.

		This information is used to optimise drawing, because it means that
		objects underneath opaque windows don't need to be painted.

		By default, components are considered transparent, unless this is used to
		make it otherwise.

		@see isOpaque, getVisibleArea
	*/
	void setOpaque (bool shouldBeOpaque);

	/** Returns true if no parts of this component are transparent.

		@returns the value that was set by setOpaque, (the default being false)
		@see setOpaque
	*/
	bool isOpaque() const noexcept;

	/** Indicates whether the component should be brought to the front when clicked.

		Setting this flag to true will cause the component to be brought to the front
		when the mouse is clicked somewhere inside it or its child components.

		Note that a top-level desktop window might still be brought to the front by the
		operating system when it's clicked, depending on how the OS works.

		By default this is set to false.

		@see setMouseClickGrabsKeyboardFocus
	*/
	void setBroughtToFrontOnMouseClick (bool shouldBeBroughtToFront) noexcept;

	/** Indicates whether the component should be brought to the front when clicked-on.

		@see setBroughtToFrontOnMouseClick
	*/
	bool isBroughtToFrontOnMouseClick() const noexcept;

	// Keyboard focus methods

	/** Sets a flag to indicate whether this component needs keyboard focus or not.

		By default components aren't actually interested in gaining the
		focus, but this method can be used to turn this on.

		See the grabKeyboardFocus() method for details about the way a component
		is chosen to receive the focus.

		@see grabKeyboardFocus, getWantsKeyboardFocus
	*/
	void setWantsKeyboardFocus (bool wantsFocus) noexcept;

	/** Returns true if the component is interested in getting keyboard focus.

		This returns the flag set by setWantsKeyboardFocus(). The default
		setting is false.

		@see setWantsKeyboardFocus
	*/
	bool getWantsKeyboardFocus() const noexcept;

	/** Chooses whether a click on this component automatically grabs the focus.

		By default this is set to true, but you might want a component which can
		be focused, but where you don't want the user to be able to affect it directly
		by clicking.
	*/
	void setMouseClickGrabsKeyboardFocus (bool shouldGrabFocus);

	/** Returns the last value set with setMouseClickGrabsKeyboardFocus().

		See setMouseClickGrabsKeyboardFocus() for more info.
	*/
	bool getMouseClickGrabsKeyboardFocus() const noexcept;

	/** Tries to give keyboard focus to this component.

		When the user clicks on a component or its grabKeyboardFocus()
		method is called, the following procedure is used to work out which
		component should get it:

		- if the component that was clicked on actually wants focus (as indicated
		  by calling getWantsKeyboardFocus), it gets it.
		- if the component itself doesn't want focus, it will try to pass it
		  on to whichever of its children is the default component, as determined by
		  KeyboardFocusTraverser::getDefaultComponent()
		- if none of its children want focus at all, it will pass it up to its
		  parent instead, unless it's a top-level component without a parent,
		  in which case it just takes the focus itself.

		@see setWantsKeyboardFocus, getWantsKeyboardFocus, hasKeyboardFocus,
			 getCurrentlyFocusedComponent, focusGained, focusLost,
			 keyPressed, keyStateChanged
	*/
	void grabKeyboardFocus();

	/** Returns true if this component currently has the keyboard focus.

		@param trueIfChildIsFocused     if this is true, then the method returns true if
										either this component or any of its children (recursively)
										have the focus. If false, the method only returns true if
										this component has the focus.

		@see grabKeyboardFocus, setWantsKeyboardFocus, getCurrentlyFocusedComponent,
			 focusGained, focusLost
	*/
	bool hasKeyboardFocus (bool trueIfChildIsFocused) const;

	/** Returns the component that currently has the keyboard focus.

		@returns the focused component, or null if nothing is focused.
	*/
	static Component* JUCE_CALLTYPE getCurrentlyFocusedComponent() noexcept;

	/** Tries to move the keyboard focus to one of this component's siblings.

		This will try to move focus to either the next or previous component. (This
		is the method that is used when shifting focus by pressing the tab key).

		Components for which getWantsKeyboardFocus() returns false are not looked at.

		@param moveToNext   if true, the focus will move forwards; if false, it will
							move backwards
		@see grabKeyboardFocus, setFocusContainer, setWantsKeyboardFocus
	*/
	void moveKeyboardFocusToSibling (bool moveToNext);

	/** Creates a KeyboardFocusTraverser object to use to determine the logic by
		which focus should be passed from this component.

		The default implementation of this method will return a default
		KeyboardFocusTraverser if this component is a focus container (as determined
		by the setFocusContainer() method). If the component isn't a focus
		container, then it will recursively ask its parents for a KeyboardFocusTraverser.

		If you overrride this to return a custom KeyboardFocusTraverser, then
		this component and all its sub-components will use the new object to
		make their focusing decisions.

		The method should return a new object, which the caller is required to
		delete when no longer needed.
	*/
	virtual KeyboardFocusTraverser* createFocusTraverser();

	/** Returns the focus order of this component, if one has been specified.

		By default components don't have a focus order - in that case, this
		will return 0. Lower numbers indicate that the component will be
		earlier in the focus traversal order.

		To change the order, call setExplicitFocusOrder().

		The focus order may be used by the KeyboardFocusTraverser class as part of
		its algorithm for deciding the order in which components should be traversed.
		See the KeyboardFocusTraverser class for more details on this.

		@see moveKeyboardFocusToSibling, createFocusTraverser, KeyboardFocusTraverser
	*/
	int getExplicitFocusOrder() const;

	/** Sets the index used in determining the order in which focusable components
		should be traversed.

		A value of 0 or less is taken to mean that no explicit order is wanted, and
		that traversal should use other factors, like the component's position.

		@see getExplicitFocusOrder, moveKeyboardFocusToSibling
	*/
	void setExplicitFocusOrder (int newFocusOrderIndex);

	/** Indicates whether this component is a parent for components that can have
		their focus traversed.

		This flag is used by the default implementation of the createFocusTraverser()
		method, which uses the flag to find the first parent component (of the currently
		focused one) which wants to be a focus container.

		So using this method to set the flag to 'true' causes this component to
		act as the top level within which focus is passed around.

		@see isFocusContainer, createFocusTraverser, moveKeyboardFocusToSibling
	*/
	void setFocusContainer (bool shouldBeFocusContainer) noexcept;

	/** Returns true if this component has been marked as a focus container.

		See setFocusContainer() for more details.

		@see setFocusContainer, moveKeyboardFocusToSibling, createFocusTraverser
	*/
	bool isFocusContainer() const noexcept;

	/** Returns true if the component (and all its parents) are enabled.

		Components are enabled by default, and can be disabled with setEnabled(). Exactly
		what difference this makes to the component depends on the type. E.g. buttons
		and sliders will choose to draw themselves differently, etc.

		Note that if one of this component's parents is disabled, this will always
		return false, even if this component itself is enabled.

		@see setEnabled, enablementChanged
	*/
	bool isEnabled() const noexcept;

	/** Enables or disables this component.

		Disabling a component will also cause all of its child components to become
		disabled.

		Similarly, enabling a component which is inside a disabled parent
		component won't make any difference until the parent is re-enabled.

		@see isEnabled, enablementChanged
	*/
	void setEnabled (bool shouldBeEnabled);

	/** Callback to indicate that this component has been enabled or disabled.

		This can be triggered by one of the component's parent components
		being enabled or disabled, as well as changes to the component itself.

		The default implementation of this method does nothing; your class may
		wish to repaint itself or something when this happens.

		@see setEnabled, isEnabled
	*/
	virtual void enablementChanged();

	/** Changes the transparency of this component.
		When painted, the entire component and all its children will be rendered
		with this as the overall opacity level, where 0 is completely invisible, and
		1.0 is fully opaque (i.e. normal).

		@see getAlpha
	*/
	void setAlpha (float newAlpha);

	/** Returns the component's current transparancy level.
		See setAlpha() for more details.
	*/
	float getAlpha() const;

	/** Changes the mouse cursor shape to use when the mouse is over this component.

		Note that the cursor set by this method can be overridden by the getMouseCursor
		method.

		@see MouseCursor
	*/
	void setMouseCursor (const MouseCursor& cursorType);

	/** Returns the mouse cursor shape to use when the mouse is over this component.

		The default implementation will return the cursor that was set by setCursor()
		but can be overridden for more specialised purposes, e.g. returning different
		cursors depending on the mouse position.

		@see MouseCursor
	*/
	virtual MouseCursor getMouseCursor();

	/** Forces the current mouse cursor to be updated.

		If you're overriding the getMouseCursor() method to control which cursor is
		displayed, then this will only be checked each time the user moves the mouse. So
		if you want to force the system to check that the cursor being displayed is
		up-to-date (even if the mouse is just sitting there), call this method.

		(If you're changing the cursor using setMouseCursor(), you don't need to bother
		calling this).
	*/
	void updateMouseCursor() const;

	/** Components can override this method to draw their content.

		The paint() method gets called when a region of a component needs redrawing,
		either because the component's repaint() method has been called, or because
		something has happened on the screen that means a section of a window needs
		to be redrawn.

		Any child components will draw themselves over whatever this method draws. If
		you need to paint over the top of your child components, you can also implement
		the paintOverChildren() method to do this.

		If you want to cause a component to redraw itself, this is done asynchronously -
		calling the repaint() method marks a region of the component as "dirty", and the
		paint() method will automatically be called sometime later, by the message thread,
		to paint any bits that need refreshing. In Juce (and almost all modern UI frameworks),
		you never redraw something synchronously.

		You should never need to call this method directly - to take a snapshot of the
		component you could use createComponentSnapshot() or paintEntireComponent().

		@param g    the graphics context that must be used to do the drawing operations.
		@see repaint, paintOverChildren, Graphics
	*/
	virtual void paint (Graphics& g);

	/** Components can override this method to draw over the top of their children.

		For most drawing operations, it's better to use the normal paint() method,
		but if you need to overlay something on top of the children, this can be
		used.

		@see paint, Graphics
	*/
	virtual void paintOverChildren (Graphics& g);

	/** Called when the mouse moves inside this component.

		If the mouse button isn't pressed and the mouse moves over a component,
		this will be called to let the component react to this.

		A component will always get a mouseEnter callback before a mouseMove.

		@param e    details about the position and status of the mouse event
		@see mouseEnter, mouseExit, mouseDrag, contains
	*/
	virtual void mouseMove         (const MouseEvent& e);

	/** Called when the mouse first enters this component.

		If the mouse button isn't pressed and the mouse moves into a component,
		this will be called to let the component react to this.

		When the mouse button is pressed and held down while being moved in
		or out of a component, no mouseEnter or mouseExit callbacks are made - only
		mouseDrag messages are sent to the component that the mouse was originally
		clicked on, until the button is released.

		If you're writing a component that needs to repaint itself when the mouse
		enters and exits, it might be quicker to use the setRepaintsOnMouseActivity()
		method.

		@param e    details about the position and status of the mouse event
		@see mouseExit, mouseDrag, mouseMove, contains
	*/
	virtual void mouseEnter        (const MouseEvent& e);

	/** Called when the mouse moves out of this component.

		This will be called when the mouse moves off the edge of this
		component.

		If the mouse button was pressed, and it was then dragged off the
		edge of the component and released, then this callback will happen
		when the button is released, after the mouseUp callback.

		If you're writing a component that needs to repaint itself when the mouse
		enters and exits, it might be quicker to use the setRepaintsOnMouseActivity()
		method.

		@param e    details about the position and status of the mouse event
		@see mouseEnter, mouseDrag, mouseMove, contains
	*/
	virtual void mouseExit         (const MouseEvent& e);

	/** Called when a mouse button is pressed while it's over this component.

		The MouseEvent object passed in contains lots of methods for finding out
		which button was pressed, as well as which modifier keys (e.g. shift, ctrl)
		were held down at the time.

		Once a button is held down, the mouseDrag method will be called when the
		mouse moves, until the button is released.

		@param e    details about the position and status of the mouse event
		@see mouseUp, mouseDrag, mouseDoubleClick, contains
	*/
	virtual void mouseDown         (const MouseEvent& e);

	/** Called when the mouse is moved while a button is held down.

		When a mouse button is pressed inside a component, that component
		receives mouseDrag callbacks each time the mouse moves, even if the
		mouse strays outside the component's bounds.

		If you want to be able to drag things off the edge of a component
		and have the component scroll when you get to the edges, the
		beginDragAutoRepeat() method might be useful.

		@param e    details about the position and status of the mouse event
		@see mouseDown, mouseUp, mouseMove, contains, beginDragAutoRepeat
	*/
	virtual void mouseDrag         (const MouseEvent& e);

	/** Called when a mouse button is released.

		A mouseUp callback is sent to the component in which a button was pressed
		even if the mouse is actually over a different component when the
		button is released.

		The MouseEvent object passed in contains lots of methods for finding out
		which buttons were down just before they were released.

		@param e    details about the position and status of the mouse event
		@see mouseDown, mouseDrag, mouseDoubleClick, contains
	*/
	virtual void mouseUp           (const MouseEvent& e);

	/** Called when a mouse button has been double-clicked in this component.

		The MouseEvent object passed in contains lots of methods for finding out
		which button was pressed, as well as which modifier keys (e.g. shift, ctrl)
		were held down at the time.

		For altering the time limit used to detect double-clicks,
		see MouseEvent::setDoubleClickTimeout.

		@param e    details about the position and status of the mouse event
		@see mouseDown, mouseUp, MouseEvent::setDoubleClickTimeout,
			 MouseEvent::getDoubleClickTimeout
	*/
	virtual void mouseDoubleClick  (const MouseEvent& e);

	/** Called when the mouse-wheel is moved.

		This callback is sent to the component that the mouse is over when the
		wheel is moved.

		If not overridden, the component will forward this message to its parent, so
		that parent components can collect mouse-wheel messages that happen to
		child components which aren't interested in them.

		@param e                details about the position and status of the mouse event
		@param wheelIncrementX   the speed and direction of the horizontal scroll-wheel - a positive
								 value means the wheel has been pushed to the right, negative means it
								 was pushed to the left
		@param wheelIncrementY   the speed and direction of the vertical scroll-wheel - a positive
								 value means the wheel has been pushed upwards, negative means it
								 was pushed downwards
	*/
	virtual void mouseWheelMove    (const MouseEvent& e,
									float wheelIncrementX,
									float wheelIncrementY);

	/** Ensures that a non-stop stream of mouse-drag events will be sent during the
		current mouse-drag operation.

		This allows you to make sure that mouseDrag() events are sent continuously, even
		when the mouse isn't moving. This can be useful for things like auto-scrolling
		components when the mouse is near an edge.

		Call this method during a mouseDown() or mouseDrag() callback, specifying the
		minimum interval between consecutive mouse drag callbacks. The callbacks
		will continue until the mouse is released, and then the interval will be reset,
		so you need to make sure it's called every time you begin a drag event.
		Passing an interval of 0 or less will cancel the auto-repeat.

		@see mouseDrag, Desktop::beginDragAutoRepeat
	*/
	static void JUCE_CALLTYPE beginDragAutoRepeat (int millisecondsBetweenCallbacks);

	/** Causes automatic repaints when the mouse enters or exits this component.

		If turned on, then when the mouse enters/exits, or when the button is pressed/released
		on the component, it will trigger a repaint.

		This is handy for things like buttons that need to draw themselves differently when
		the mouse moves over them, and it avoids having to override all the different mouse
		callbacks and call repaint().

		@see mouseEnter, mouseExit, mouseDown, mouseUp
	*/
	void setRepaintsOnMouseActivity (bool shouldRepaint) noexcept;

	/** Registers a listener to be told when mouse events occur in this component.

		If you need to get informed about mouse events in a component but can't or
		don't want to override its methods, you can attach any number of listeners
		to the component, and these will get told about the events in addition to
		the component's own callbacks being called.

		Note that a MouseListener can also be attached to more than one component.

		@param newListener                              the listener to register
		@param wantsEventsForAllNestedChildComponents   if true, the listener will receive callbacks
														for events that happen to any child component
														within this component, including deeply-nested
														child components. If false, it will only be
														told about events that this component handles.
		@see MouseListener, removeMouseListener
	*/
	void addMouseListener (MouseListener* newListener,
						   bool wantsEventsForAllNestedChildComponents);

	/** Deregisters a mouse listener.
		@see addMouseListener, MouseListener
	*/
	void removeMouseListener (MouseListener* listenerToRemove);

	/** Adds a listener that wants to hear about keypresses that this component receives.

		The listeners that are registered with a component are called by its keyPressed() or
		keyStateChanged() methods (assuming these haven't been overridden to do something else).

		If you add an object as a key listener, be careful to remove it when the object
		is deleted, or the component will be left with a dangling pointer.

		@see keyPressed, keyStateChanged, removeKeyListener
	*/
	void addKeyListener (KeyListener* newListener);

	/** Removes a previously-registered key listener.

		@see addKeyListener
	*/
	void removeKeyListener (KeyListener* listenerToRemove);

	/** Called when a key is pressed.

		When a key is pressed, the component that has the keyboard focus will have this
		method called. Remember that a component will only be given the focus if its
		setWantsKeyboardFocus() method has been used to enable this.

		If your implementation returns true, the event will be consumed and not passed
		on to any other listeners. If it returns false, the key will be passed to any
		KeyListeners that have been registered with this component. As soon as one of these
		returns true, the process will stop, but if they all return false, the event will
		be passed upwards to this component's parent, and so on.

		The default implementation of this method does nothing and returns false.

		@see keyStateChanged, getCurrentlyFocusedComponent, addKeyListener
	*/
	virtual bool keyPressed (const KeyPress& key);

	/** Called when a key is pressed or released.

		Whenever a key on the keyboard is pressed or released (including modifier keys
		like shift and ctrl), this method will be called on the component that currently
		has the keyboard focus. Remember that a component will only be given the focus if
		its setWantsKeyboardFocus() method has been used to enable this.

		If your implementation returns true, the event will be consumed and not passed
		on to any other listeners. If it returns false, then any KeyListeners that have
		been registered with this component will have their keyStateChanged methods called.
		As soon as one of these returns true, the process will stop, but if they all return
		false, the event will be passed upwards to this component's parent, and so on.

		The default implementation of this method does nothing and returns false.

		To find out which keys are up or down at any time, see the KeyPress::isKeyCurrentlyDown()
		method.

		@param isKeyDown    true if a key has been pressed; false if it has been released

		@see keyPressed, KeyPress, getCurrentlyFocusedComponent, addKeyListener
	*/
	virtual bool keyStateChanged (bool isKeyDown);

	/** Called when a modifier key is pressed or released.

		Whenever the shift, control, alt or command keys are pressed or released,
		this method will be called on the component that currently has the keyboard focus.
		Remember that a component will only be given the focus if its setWantsKeyboardFocus()
		method has been used to enable this.

		The default implementation of this method actually calls its parent's modifierKeysChanged
		method, so that focused components which aren't interested in this will give their
		parents a chance to act on the event instead.

		@see keyStateChanged, ModifierKeys
	*/
	virtual void modifierKeysChanged (const ModifierKeys& modifiers);

	/** Enumeration used by the focusChanged() and focusLost() methods. */
	enum FocusChangeType
	{
		focusChangedByMouseClick,   /**< Means that the user clicked the mouse to change focus. */
		focusChangedByTabKey,       /**< Means that the user pressed the tab key to move the focus. */
		focusChangedDirectly        /**< Means that the focus was changed by a call to grabKeyboardFocus(). */
	};

	/** Called to indicate that this component has just acquired the keyboard focus.

		@see focusLost, setWantsKeyboardFocus, getCurrentlyFocusedComponent, hasKeyboardFocus
	*/
	virtual void focusGained (FocusChangeType cause);

	/** Called to indicate that this component has just lost the keyboard focus.

		@see focusGained, setWantsKeyboardFocus, getCurrentlyFocusedComponent, hasKeyboardFocus
	*/
	virtual void focusLost (FocusChangeType cause);

	/** Called to indicate that one of this component's children has been focused or unfocused.

		Essentially this means that the return value of a call to hasKeyboardFocus (true) has
		changed. It happens when focus moves from one of this component's children (at any depth)
		to a component that isn't contained in this one, (or vice-versa).

		@see focusGained, setWantsKeyboardFocus, getCurrentlyFocusedComponent, hasKeyboardFocus
	*/
	virtual void focusOfChildComponentChanged (FocusChangeType cause);

	/** Returns true if the mouse is currently over this component.

		If the mouse isn't over the component, this will return false, even if the
		mouse is currently being dragged - so you can use this in your mouseDrag
		method to find out whether it's really over the component or not.

		Note that when the mouse button is being held down, then the only component
		for which this method will return true is the one that was originally
		clicked on.

		If includeChildren is true, then this will also return true if the mouse is over
		any of the component's children (recursively) as well as the component itself.

		@see isMouseButtonDown. isMouseOverOrDragging, mouseDrag
	*/
	bool isMouseOver (bool includeChildren = false) const;

	/** Returns true if the mouse button is currently held down in this component.

		Note that this is a test to see whether the mouse is being pressed in this
		component, so it'll return false if called on component A when the mouse
		is actually being dragged in component B.

		@see isMouseButtonDownAnywhere, isMouseOver, isMouseOverOrDragging
	*/
	bool isMouseButtonDown() const;

	/** True if the mouse is over this component, or if it's being dragged in this component.

		This is a handy equivalent to (isMouseOver() || isMouseButtonDown()).

		@see isMouseOver, isMouseButtonDown, isMouseButtonDownAnywhere
	*/
	bool isMouseOverOrDragging() const;

	/** Returns true if a mouse button is currently down.

		Unlike isMouseButtonDown, this will test the current state of the
		buttons without regard to which component (if any) it has been
		pressed in.

		@see isMouseButtonDown, ModifierKeys
	*/
	static bool JUCE_CALLTYPE isMouseButtonDownAnywhere() noexcept;

	/** Returns the mouse's current position, relative to this component.
		The return value is relative to the component's top-left corner.
	*/
	Point<int> getMouseXYRelative() const;

	/** Called when this component's size has been changed.

		A component can implement this method to do things such as laying out its
		child components when its width or height changes.

		The method is called synchronously as a result of the setBounds or setSize
		methods, so repeatedly changing a components size will repeatedly call its
		resized method (unlike things like repainting, where multiple calls to repaint
		are coalesced together).

		If the component is a top-level window on the desktop, its size could also
		be changed by operating-system factors beyond the application's control.

		@see moved, setSize
	*/
	virtual void resized();

	/** Called when this component's position has been changed.

		This is called when the position relative to its parent changes, not when
		its absolute position on the screen changes (so it won't be called for
		all child components when a parent component is moved).

		The method is called synchronously as a result of the setBounds, setTopLeftPosition
		or any of the other repositioning methods, and like resized(), it will be
		called each time those methods are called.

		If the component is a top-level window on the desktop, its position could also
		be changed by operating-system factors beyond the application's control.

		@see resized, setBounds
	*/
	virtual void moved();

	/** Called when one of this component's children is moved or resized.

		If the parent wants to know about changes to its immediate children (not
		to children of its children), this is the method to override.

		@see moved, resized, parentSizeChanged
	*/
	virtual void childBoundsChanged (Component* child);

	/** Called when this component's immediate parent has been resized.

		If the component is a top-level window, this indicates that the screen size
		has changed.

		@see childBoundsChanged, moved, resized
	*/
	virtual void parentSizeChanged();

	/** Called when this component has been moved to the front of its siblings.

		The component may have been brought to the front by the toFront() method, or
		by the operating system if it's a top-level window.

		@see toFront
	*/
	virtual void broughtToFront();

	/** Adds a listener to be told about changes to the component hierarchy or position.

		Component listeners get called when this component's size, position or children
		change - see the ComponentListener class for more details.

		@param newListener  the listener to register - if this is already registered, it
							will be ignored.
		@see ComponentListener, removeComponentListener
	*/
	void addComponentListener (ComponentListener* newListener);

	/** Removes a component listener.

		@see addComponentListener
	*/
	void removeComponentListener (ComponentListener* listenerToRemove);

	/** Dispatches a numbered message to this component.

		This is a quick and cheap way of allowing simple asynchronous messages to
		be sent to components. It's also safe, because if the component that you
		send the message to is a null or dangling pointer, this won't cause an error.

		The command ID is later delivered to the component's handleCommandMessage() method by
		the application's message queue.

		@see handleCommandMessage
	*/
	void postCommandMessage (int commandId);

	/** Called to handle a command that was sent by postCommandMessage().

		This is called by the message thread when a command message arrives, and
		the component can override this method to process it in any way it needs to.

		@see postCommandMessage
	*/
	virtual void handleCommandMessage (int commandId);

	/** Runs a component modally, waiting until the loop terminates.

		This method first makes the component visible, brings it to the front and
		gives it the keyboard focus.

		It then runs a loop, dispatching messages from the system message queue, but
		blocking all mouse or keyboard messages from reaching any components other
		than this one and its children.

		This loop continues until the component's exitModalState() method is called (or
		the component is deleted), and then this method returns, returning the value
		passed into exitModalState().

		@see enterModalState, exitModalState, isCurrentlyModal, getCurrentlyModalComponent,
			 isCurrentlyBlockedByAnotherModalComponent, ModalComponentManager
	*/
   #if JUCE_MODAL_LOOPS_PERMITTED
	int runModalLoop();
   #endif

	/** Puts the component into a modal state.

		This makes the component modal, so that messages are blocked from reaching
		any components other than this one and its children, but unlike runModalLoop(),
		this method returns immediately.

		If takeKeyboardFocus is true, the component will use grabKeyboardFocus() to
		get the focus, which is usually what you'll want it to do. If not, it will leave
		the focus unchanged.

		The callback is an optional object which will receive a callback when the modal
		component loses its modal status, either by being hidden or when exitModalState()
		is called. If you pass an object in here, the system will take care of deleting it
		later, after making the callback

		If deleteWhenDismissed is true, then when it is dismissed, the component will be
		deleted and then the callback will be called. (This will safely handle the situation
		where the component is deleted before its exitModalState() method is called).

		@see exitModalState, runModalLoop, ModalComponentManager::attachCallback
	*/
	void enterModalState (bool takeKeyboardFocus = true,
						  ModalComponentManager::Callback* callback = nullptr,
						  bool deleteWhenDismissed = false);

	/** Ends a component's modal state.

		If this component is currently modal, this will turn of its modalness, and return
		a value to the runModalLoop() method that might have be running its modal loop.

		@see runModalLoop, enterModalState, isCurrentlyModal
	*/
	void exitModalState (int returnValue);

	/** Returns true if this component is the modal one.

		It's possible to have nested modal components, e.g. a pop-up dialog box
		that launches another pop-up, but this will only return true for
		the one at the top of the stack.

		@see getCurrentlyModalComponent
	*/
	bool isCurrentlyModal() const noexcept;

	/** Returns the number of components that are currently in a modal state.
		@see getCurrentlyModalComponent
	 */
	static int JUCE_CALLTYPE getNumCurrentlyModalComponents() noexcept;

	/** Returns one of the components that are currently modal.

		The index specifies which of the possible modal components to return. The order
		of the components in this list is the reverse of the order in which they became
		modal - so the component at index 0 is always the active component, and the others
		are progressively earlier ones that are themselves now blocked by later ones.

		@returns the modal component, or null if no components are modal (or if the
				index is out of range)
		@see getNumCurrentlyModalComponents, runModalLoop, isCurrentlyModal
	*/
	static Component* JUCE_CALLTYPE getCurrentlyModalComponent (int index = 0) noexcept;

	/** Checks whether there's a modal component somewhere that's stopping this one
		from receiving messages.

		If there is a modal component, its canModalEventBeSentToComponent() method
		will be called to see if it will still allow this component to receive events.

		@see runModalLoop, getCurrentlyModalComponent
	*/
	bool isCurrentlyBlockedByAnotherModalComponent() const;

	/** When a component is modal, this callback allows it to choose which other
		components can still receive events.

		When a modal component is active and the user clicks on a non-modal component,
		this method is called on the modal component, and if it returns true, the
		event is allowed to reach its target. If it returns false, the event is blocked
		and the inputAttemptWhenModal() callback is made.

		It called by the isCurrentlyBlockedByAnotherModalComponent() method. The default
		implementation just returns false in all cases.
	*/
	virtual bool canModalEventBeSentToComponent (const Component* targetComponent);

	/** Called when the user tries to click on a component that is blocked by another
		modal component.

		When a component is modal and the user clicks on one of the other components,
		the modal component will receive this callback.

		The default implementation of this method will play a beep, and bring the currently
		modal component to the front, but it can be overridden to do other tasks.

		@see isCurrentlyBlockedByAnotherModalComponent, canModalEventBeSentToComponent
	*/
	virtual void inputAttemptWhenModal();

	/** Returns the set of properties that belong to this component.
		Each component has a NamedValueSet object which you can use to attach arbitrary
		items of data to it.
	*/
	NamedValueSet& getProperties() noexcept                             { return properties; }

	/** Returns the set of properties that belong to this component.
		Each component has a NamedValueSet object which you can use to attach arbitrary
		items of data to it.
	*/
	const NamedValueSet& getProperties() const noexcept                 { return properties; }

	/** Looks for a colour that has been registered with the given colour ID number.

		If a colour has been set for this ID number using setColour(), then it is
		returned. If none has been set, the method will try calling the component's
		LookAndFeel class's findColour() method. If none has been registered with the
		look-and-feel either, it will just return black.

		The colour IDs for various purposes are stored as enums in the components that
		they are relevent to - for an example, see Slider::ColourIds,
		Label::ColourIds, TextEditor::ColourIds, TreeView::ColourIds, etc.

		@see setColour, isColourSpecified, colourChanged, LookAndFeel::findColour, LookAndFeel::setColour
	*/
	Colour findColour (int colourId, bool inheritFromParent = false) const;

	/** Registers a colour to be used for a particular purpose.

		Changing a colour will cause a synchronous callback to the colourChanged()
		method, which your component can override if it needs to do something when
		colours are altered.

		For more details about colour IDs, see the comments for findColour().

		@see findColour, isColourSpecified, colourChanged, LookAndFeel::findColour, LookAndFeel::setColour
	*/
	void setColour (int colourId, const Colour& colour);

	/** If a colour has been set with setColour(), this will remove it.

		This allows you to make a colour revert to its default state.
	*/
	void removeColour (int colourId);

	/** Returns true if the specified colour ID has been explicitly set for this
		component using the setColour() method.
	*/
	bool isColourSpecified (int colourId) const;

	/** This looks for any colours that have been specified for this component,
		and copies them to the specified target component.
	*/
	void copyAllExplicitColoursTo (Component& target) const;

	/** This method is called when a colour is changed by the setColour() method.

		@see setColour, findColour
	*/
	virtual void colourChanged();

	/** Components can implement this method to provide a MarkerList.
		The default implementation of this method returns 0, but you can override it to
		return a pointer to the component's marker list. If xAxis is true, it should
		return the X marker list; if false, it should return the Y markers.
	*/
	virtual MarkerList* getMarkers (bool xAxis);

	/** Returns the underlying native window handle for this component.

		This is platform-dependent and strictly for power-users only!
	*/
	void* getWindowHandle() const;

	/** Holds a pointer to some type of Component, which automatically becomes null if
		the component is deleted.

		If you're using a component which may be deleted by another event that's outside
		of your control, use a SafePointer instead of a normal pointer to refer to it,
		and you can test whether it's null before using it to see if something has deleted
		it.

		The ComponentType typedef must be Component, or some subclass of Component.

		You may also want to use a WeakReference<Component> object for the same purpose.
	*/
	template <class ComponentType>
	class SafePointer
	{
	public:
		/** Creates a null SafePointer. */
		SafePointer() noexcept {}

		/** Creates a SafePointer that points at the given component. */
		SafePointer (ComponentType* const component)        : weakRef (component) {}

		/** Creates a copy of another SafePointer. */
		SafePointer (const SafePointer& other) noexcept     : weakRef (other.weakRef) {}

		/** Copies another pointer to this one. */
		SafePointer& operator= (const SafePointer& other)           { weakRef = other.weakRef; return *this; }

		/** Copies another pointer to this one. */
		SafePointer& operator= (ComponentType* const newComponent)  { weakRef = newComponent; return *this; }

		/** Returns the component that this pointer refers to, or null if the component no longer exists. */
		ComponentType* getComponent() const noexcept        { return dynamic_cast <ComponentType*> (weakRef.get()); }

		/** Returns the component that this pointer refers to, or null if the component no longer exists. */
		operator ComponentType*() const noexcept            { return getComponent(); }

		/** Returns the component that this pointer refers to, or null if the component no longer exists. */
		ComponentType* operator->() noexcept                { return getComponent(); }

		/** Returns the component that this pointer refers to, or null if the component no longer exists. */
		const ComponentType* operator->() const noexcept    { return getComponent(); }

		/** If the component is valid, this deletes it and sets this pointer to null. */
		void deleteAndZero()                                { delete getComponent(); jassert (getComponent() == nullptr); }

		bool operator== (ComponentType* component) const noexcept   { return weakRef == component; }
		bool operator!= (ComponentType* component) const noexcept   { return weakRef != component; }

	private:
		WeakReference<Component> weakRef;
	};

	/** A class to keep an eye on a component and check for it being deleted.

		This is designed for use with the ListenerList::callChecked() methods, to allow
		the list iterator to stop cleanly if the component is deleted by a listener callback
		while the list is still being iterated.
	*/
	class JUCE_API  BailOutChecker
	{
	public:
		/** Creates a checker that watches one component. */
		BailOutChecker (Component* component);

		/** Returns true if either of the two components have been deleted since this object was created. */
		bool shouldBailOut() const noexcept;

	private:
		const WeakReference<Component> safePointer;

		JUCE_DECLARE_NON_COPYABLE (BailOutChecker);
	};

	/**
		Base class for objects that can be used to automatically position a component according to
		some kind of algorithm.

		The component class simply holds onto a reference to a Positioner, but doesn't actually do
		anything with it - all the functionality must be implemented by the positioner itself (e.g.
		it might choose to watch some kind of value and move the component when the value changes).
	*/
	class JUCE_API  Positioner
	{
	public:
		/** Creates a Positioner which can control the specified component. */
		explicit Positioner (Component& component) noexcept;
		/** Destructor. */
		virtual ~Positioner() {}

		/** Returns the component that this positioner controls. */
		Component& getComponent() const noexcept    { return component; }

		/** Attempts to set the component's position to the given rectangle.
			Unlike simply calling Component::setBounds(), this may involve the positioner
			being smart enough to adjust itself to fit the new bounds, e.g. a RelativeRectangle's
			positioner may try to reverse the expressions used to make them fit these new coordinates.
		*/
		virtual void applyNewBounds (const Rectangle<int>& newBounds) = 0;

	private:
		Component& component;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Positioner);
	};

	/** Returns the Positioner object that has been set for this component.
		@see setPositioner()
	*/
	Positioner* getPositioner() const noexcept;

	/** Sets a new Positioner object for this component.
		If there's currently another positioner set, it will be deleted. The object that is passed in
		will be deleted automatically by this component when it's no longer required. Pass a null pointer
		to clear the current positioner.
		@see getPositioner()
	*/
	void setPositioner (Positioner* newPositioner);

	/** Gives the component a CachedComponentImage that should be used to buffer its painting.
		The object that is passed-in will be owned by this component, and will be deleted automatically
		later on.
		@see setBufferedToImage
	*/
	void setCachedComponentImage (CachedComponentImage* newCachedImage);

	/** Returns the object that was set by setCachedComponentImage().
		@see setCachedComponentImage
	*/
	CachedComponentImage* getCachedComponentImage() const noexcept  { return cachedImage; }

   #ifndef DOXYGEN
	// These methods are deprecated - use localPointToGlobal, getLocalPoint, getLocalPoint, etc instead.
	JUCE_DEPRECATED (Point<int> relativePositionToGlobal (const Point<int>&) const);
	JUCE_DEPRECATED (Point<int> globalPositionToRelative (const Point<int>&) const);
	JUCE_DEPRECATED (Point<int> relativePositionToOtherComponent (const Component*, const Point<int>&) const);
   #endif

private:

	friend class ComponentPeer;
	friend class MouseInputSource;
	friend class MouseInputSourceInternal;

   #ifndef DOXYGEN
	static Component* currentlyFocusedComponent;

	String componentName, componentID;
	Component* parentComponent;
	Rectangle<int> bounds;
	ScopedPointer <Positioner> positioner;
	ScopedPointer <AffineTransform> affineTransform;
	Array <Component*> childComponentList;
	LookAndFeel* lookAndFeel;
	MouseCursor cursor;
	ImageEffectFilter* effect;
	ScopedPointer <CachedComponentImage> cachedImage;

	class MouseListenerList;
	friend class MouseListenerList;
	friend class ScopedPointer <MouseListenerList>;
	ScopedPointer <MouseListenerList> mouseListeners;
	ScopedPointer <Array <KeyListener*> > keyListeners;
	ListenerList <ComponentListener> componentListeners;
	NamedValueSet properties;

	friend class WeakReference<Component>;
	WeakReference<Component>::Master masterReference;

	struct ComponentFlags
	{
		bool hasHeavyweightPeerFlag     : 1;
		bool visibleFlag                : 1;
		bool opaqueFlag                 : 1;
		bool ignoresMouseClicksFlag     : 1;
		bool allowChildMouseClicksFlag  : 1;
		bool wantsFocusFlag             : 1;
		bool isFocusContainerFlag       : 1;
		bool dontFocusOnMouseClickFlag  : 1;
		bool alwaysOnTopFlag            : 1;
		bool bufferToImageFlag          : 1;
		bool bringToFrontOnClickFlag    : 1;
		bool repaintOnMouseActivityFlag : 1;
		bool currentlyModalFlag         : 1;
		bool isDisabledFlag             : 1;
		bool childCompFocusedFlag       : 1;
		bool dontClipGraphicsFlag       : 1;
	  #if JUCE_DEBUG
		bool isInsidePaintCall          : 1;
	  #endif
	};

	union
	{
		uint32 componentFlags;
		ComponentFlags flags;
	};

	uint8 componentTransparency;

	void internalMouseEnter (MouseInputSource&, const Point<int>&, const Time&);
	void internalMouseExit  (MouseInputSource&, const Point<int>&, const Time&);
	void internalMouseDown  (MouseInputSource&, const Point<int>&, const Time&);
	void internalMouseUp    (MouseInputSource&, const Point<int>&, const Time&, const ModifierKeys& oldModifiers);
	void internalMouseDrag  (MouseInputSource&, const Point<int>&, const Time&);
	void internalMouseMove  (MouseInputSource&, const Point<int>&, const Time&);
	void internalMouseWheel (MouseInputSource&, const Point<int>&, const Time&, float amountX, float amountY);
	void internalBroughtToFront();
	void internalFocusGain (const FocusChangeType, const WeakReference<Component>&);
	void internalFocusGain (const FocusChangeType);
	void internalFocusLoss (const FocusChangeType);
	void internalChildFocusChange (FocusChangeType, const WeakReference<Component>&);
	void internalModalInputAttempt();
	void internalModifierKeysChanged();
	void internalChildrenChanged();
	void internalHierarchyChanged();
	void internalRepaint (const Rectangle<int>&);
	void internalRepaintUnchecked (const Rectangle<int>&, bool);
	Component* removeChildComponent (int index, bool sendParentEvents, bool sendChildEvents);
	void reorderChildInternal (int sourceIndex, int destIndex);
	void paintComponentAndChildren (Graphics&);
	void paintWithinParentContext (Graphics&);
	void sendMovedResizedMessages (bool wasMoved, bool wasResized);
	void repaintParent();
	void sendFakeMouseMove() const;
	void takeKeyboardFocus (const FocusChangeType);
	void grabFocusInternal (const FocusChangeType, bool canTryParent);
	static void giveAwayFocus (bool sendFocusLossEvent);
	void sendEnablementChangeMessage();
	void sendVisibilityChangeMessage();

	struct ComponentHelpers;
	friend struct ComponentHelpers;

	/* Components aren't allowed to have copy constructors, as this would mess up parent hierarchies.
	   You might need to give your subclasses a private dummy constructor to avoid compiler warnings.
	*/
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Component);

   #if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// This is included here just to cause a compile error if your code is still handling
	// drag-and-drop with this method. If so, just update it to use the new FileDragAndDropTarget
	// class, which is easy (just make your class inherit from FileDragAndDropTarget, and
	// implement its methods instead of this Component method).
	virtual void filesDropped (const StringArray&, int, int) {}

	// This is included here to cause an error if you use or overload it - it has been deprecated in
	// favour of contains (const Point<int>&)
	void contains (int, int);
   #endif

protected:

	/** @internal */
	virtual ComponentPeer* createNewPeer (int styleFlags, void* nativeWindowToAttachTo);
   #endif
};

#endif   // __JUCE_COMPONENT_JUCEHEADER__

/*** End of inlined file: juce_Component.h ***/


#endif
#ifndef __JUCE_COMPONENTLISTENER_JUCEHEADER__

#endif
#ifndef __JUCE_DESKTOP_JUCEHEADER__

/*** Start of inlined file: juce_Desktop.h ***/
#ifndef __JUCE_DESKTOP_JUCEHEADER__
#define __JUCE_DESKTOP_JUCEHEADER__


/*** Start of inlined file: juce_ComponentAnimator.h ***/
#ifndef __JUCE_COMPONENTANIMATOR_JUCEHEADER__
#define __JUCE_COMPONENTANIMATOR_JUCEHEADER__

/**
	Animates a set of components, moving them to a new position and/or fading their
	alpha levels.

	To animate a component, create a ComponentAnimator instance or (preferably) use the
	global animator object provided by Desktop::getAnimator(), and call its animateComponent()
	method to commence the movement.

	If you're using your own ComponentAnimator instance, you'll need to make sure it isn't
	deleted before it finishes moving the components, or they'll be abandoned before reaching their
	destinations.

	It's ok to delete components while they're being animated - the animator will detect this
	and safely stop using them.

	The class is a ChangeBroadcaster and sends a notification when any components
	start or finish being animated.

	@see Desktop::getAnimator
*/
class JUCE_API  ComponentAnimator  : public ChangeBroadcaster,
									 private Timer
{
public:

	/** Creates a ComponentAnimator. */
	ComponentAnimator();

	/** Destructor. */
	~ComponentAnimator();

	/** Starts a component moving from its current position to a specified position.

		If the component is already in the middle of an animation, that will be abandoned,
		and a new animation will begin, moving the component from its current location.

		The start and end speed parameters let you apply some acceleration to the component's
		movement.

		@param component            the component to move
		@param finalBounds          the destination bounds to which the component should move. To leave the
									component in the same place, just pass component->getBounds() for this value
		@param finalAlpha           the alpha value that the component should have at the end of the animation
		@param animationDurationMilliseconds    how long the animation should last, in milliseconds
		@param useProxyComponent    if true, this means the component should be replaced by an internally
									managed temporary component which is a snapshot of the original component.
									This avoids the component having to paint itself as it moves, so may
									be more efficient. This option also allows you to delete the original
									component immediately after starting the animation, because the animation
									can proceed without it. If you use a proxy, the original component will be
									made invisible by this call, and then will become visible again at the end
									of the animation. It'll also mean that the proxy component will be temporarily
									added to the component's parent, so avoid it if this might confuse the parent
									component, or if there's a chance the parent might decide to delete its children.
		@param startSpeed           a value to indicate the relative start speed of the animation. If this is 0,
									the component will start by accelerating from rest; higher values mean that it
									will have an initial speed greater than zero. If the value if greater than 1, it
									will decelerate towards the middle of its journey. To move the component at a
									constant rate for its entire animation, set both the start and end speeds to 1.0
		@param endSpeed             a relative speed at which the component should be moving when the animation finishes.
									If this is 0, the component will decelerate to a standstill at its final position;
									higher values mean the component will still be moving when it stops. To move the component
									at a constant rate for its entire animation, set both the start and end speeds to 1.0
	*/
	void animateComponent (Component* component,
						   const Rectangle<int>& finalBounds,
						   float finalAlpha,
						   int animationDurationMilliseconds,
						   bool useProxyComponent,
						   double startSpeed,
						   double endSpeed);

	/** Begins a fade-out of this components alpha level.
		This is a quick way of invoking animateComponent() with a target alpha value of 0.0f, using
		a proxy. You're safe to delete the component after calling this method, and this won't
		interfere with the animation's progress.
	*/
	void fadeOut (Component* component, int millisecondsToTake);

	/** Begins a fade-in of a component.
		This is a quick way of invoking animateComponent() with a target alpha value of 1.0f.
	*/
	void fadeIn (Component* component, int millisecondsToTake);

	/** Stops a component if it's currently being animated.

		If moveComponentToItsFinalPosition is true, then the component will
		be immediately moved to its destination position and size. If false, it will be
		left in whatever location it currently occupies.
	*/
	void cancelAnimation (Component* component,
						  bool moveComponentToItsFinalPosition);

	/** Clears all of the active animations.

		If moveComponentsToTheirFinalPositions is true, all the components will
		be immediately set to their final positions. If false, they will be
		left in whatever locations they currently occupy.
	*/
	void cancelAllAnimations (bool moveComponentsToTheirFinalPositions);

	/** Returns the destination position for a component.

		If the component is being animated, this will return the target position that
		was specified when animateComponent() was called.

		If the specified component isn't currently being animated, this method will just
		return its current position.
	*/
	Rectangle<int> getComponentDestination (Component* component);

	/** Returns true if the specified component is currently being animated. */
	bool isAnimating (Component* component) const noexcept;

	/** Returns true if any components are currently being animated. */
	bool isAnimating() const noexcept;

private:

	class AnimationTask;
	OwnedArray <AnimationTask> tasks;
	uint32 lastTime;

	AnimationTask* findTaskFor (Component* component) const noexcept;
	void timerCallback();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentAnimator);
};

#endif   // __JUCE_COMPONENTANIMATOR_JUCEHEADER__

/*** End of inlined file: juce_ComponentAnimator.h ***/

class MouseInputSource;
class MouseInputSourceInternal;
class MouseListener;

/**
	Classes can implement this interface and register themselves with the Desktop class
	to receive callbacks when the currently focused component changes.

	@see Desktop::addFocusChangeListener, Desktop::removeFocusChangeListener
*/
class JUCE_API  FocusChangeListener
{
public:
	/** Destructor. */
	virtual ~FocusChangeListener()  {}

	/** Callback to indicate that the currently focused component has changed. */
	virtual void globalFocusChanged (Component* focusedComponent) = 0;
};

/**
	Describes and controls aspects of the computer's desktop.

*/
class JUCE_API  Desktop  : private DeletedAtShutdown,
						   private Timer,
						   private AsyncUpdater
{
public:

	/** There's only one dektop object, and this method will return it.
	*/
	static Desktop& JUCE_CALLTYPE getInstance();

	/** Returns the mouse position.

		The co-ordinates are relative to the top-left of the main monitor.

		Note that this is just a shortcut for calling getMainMouseSource().getScreenPosition(), and
		you should only resort to grabbing the global mouse position if there's really no
		way to get the coordinates via a mouse event callback instead.
	*/
	static Point<int> getMousePosition();

	/** Makes the mouse pointer jump to a given location.

		The co-ordinates are relative to the top-left of the main monitor.
	*/
	static void setMousePosition (const Point<int>& newPosition);

	/** Returns the last position at which a mouse button was pressed.

		Note that this is just a shortcut for calling getMainMouseSource().getLastMouseDownPosition(),
		and in a multi-touch environment, it doesn't make much sense. ALWAYS prefer to
		get this information via other means, such as MouseEvent::getMouseDownScreenPosition()
		if possible, and only ever call this as a last resort.
	*/
	static Point<int> getLastMouseDownPosition();

	/** Returns the number of times the mouse button has been clicked since the
		app started.

		Each mouse-down event increments this number by 1.
	*/
	static int getMouseButtonClickCounter();

	/** This lets you prevent the screensaver from becoming active.

		Handy if you're running some sort of presentation app where having a screensaver
		appear would be annoying.

		Pass false to disable the screensaver, and true to re-enable it. (Note that this
		won't enable a screensaver unless the user has actually set one up).

		The disablement will only happen while the Juce application is the foreground
		process - if another task is running in front of it, then the screensaver will
		be unaffected.

		@see isScreenSaverEnabled
	*/
	static void setScreenSaverEnabled (bool isEnabled);

	/** Returns true if the screensaver has not been turned off.

		This will return the last value passed into setScreenSaverEnabled(). Note that
		it won't tell you whether the user is actually using a screen saver, just
		whether this app is deliberately preventing one from running.

		@see setScreenSaverEnabled
	*/
	static bool isScreenSaverEnabled();

	/** Registers a MouseListener that will receive all mouse events that occur on
		any component.

		@see removeGlobalMouseListener
	*/
	void addGlobalMouseListener (MouseListener* listener);

	/** Unregisters a MouseListener that was added with the addGlobalMouseListener()
		method.

		@see addGlobalMouseListener
	*/
	void removeGlobalMouseListener (MouseListener* listener);

	/** Registers a MouseListener that will receive a callback whenever the focused
		component changes.
	*/
	void addFocusChangeListener (FocusChangeListener* listener);

	/** Unregisters a listener that was added with addFocusChangeListener(). */
	void removeFocusChangeListener (FocusChangeListener* listener);

	/** Takes a component and makes it full-screen, removing the taskbar, dock, etc.

		The component must already be on the desktop for this method to work. It will
		be resized to completely fill the screen and any extraneous taskbars, menu bars,
		etc will be hidden.

		To exit kiosk mode, just call setKioskModeComponent (nullptr). When this is called,
		the component that's currently being used will be resized back to the size
		and position it was in before being put into this mode.

		If allowMenusAndBars is true, things like the menu and dock (on mac) are still
		allowed to pop up when the mouse moves onto them. If this is false, it'll try
		to hide as much on-screen paraphenalia as possible.
	*/
	void setKioskModeComponent (Component* componentToUse,
								bool allowMenusAndBars = true);

	/** Returns the component that is currently being used in kiosk-mode.

		This is the component that was last set by setKioskModeComponent(). If none
		has been set, this returns 0.
	*/
	Component* getKioskModeComponent() const noexcept               { return kioskModeComponent; }

	/** Returns the number of components that are currently active as top-level
		desktop windows.

		@see getComponent, Component::addToDesktop
	*/
	int getNumComponents() const noexcept;

	/** Returns one of the top-level desktop window components.

		The index is from 0 to getNumComponents() - 1. This could return 0 if the
		index is out-of-range.

		@see getNumComponents, Component::addToDesktop
	*/
	Component* getComponent (int index) const noexcept;

	/** Finds the component at a given screen location.

		This will drill down into top-level windows to find the child component at
		the given position.

		Returns 0 if the co-ordinates are inside a non-Juce window.
	*/
	Component* findComponentAt (const Point<int>& screenPosition) const;

	/** The Desktop object has a ComponentAnimator instance which can be used for performing
		your animations.

		Having a single shared ComponentAnimator object makes it more efficient when multiple
		components are being moved around simultaneously. It's also more convenient than having
		to manage your own instance of one.

		@see ComponentAnimator
	*/
	ComponentAnimator& getAnimator() noexcept                       { return animator; }

	/** Returns the current default look-and-feel for components which don't have one
		explicitly set.
		@see setDefaultLookAndFeel
	*/
	LookAndFeel& getDefaultLookAndFeel() noexcept;

	/** Changes the default look-and-feel.
		@param newDefaultLookAndFeel    the new look-and-feel object to use - if this is
										set to nullptr, it will revert to using the system's
										default one. The object passed-in must be deleted by the
										caller when it's no longer needed.
		@see getDefaultLookAndFeel
	*/
	void setDefaultLookAndFeel (LookAndFeel* newDefaultLookAndFeel);

	/** Returns the number of MouseInputSource objects the system has at its disposal.
		In a traditional single-mouse system, there might be only one object. On a multi-touch
		system, there could be one input source per potential finger.
		To find out how many mouse events are currently happening, use getNumDraggingMouseSources().
		@see getMouseSource
	*/
	int getNumMouseSources() const noexcept                         { return mouseSources.size(); }

	/** Returns one of the system's MouseInputSource objects.
		The index should be from 0 to getNumMouseSources() - 1. Out-of-range indexes will return
		a null pointer.
		In a traditional single-mouse system, there might be only one object. On a multi-touch
		system, there could be one input source per potential finger.
	*/
	MouseInputSource* getMouseSource (int index) const noexcept     { return mouseSources [index]; }

	/** Returns the main mouse input device that the system is using.
		@see getNumMouseSources()
	*/
	MouseInputSource& getMainMouseSource() const noexcept           { return *mouseSources.getUnchecked(0); }

	/** Returns the number of mouse-sources that are currently being dragged.
		In a traditional single-mouse system, this will be 0 or 1, depending on whether a
		juce component has the button down on it. In a multi-touch system, this could
		be any number from 0 to the number of simultaneous touches that can be detected.
	*/
	int getNumDraggingMouseSources() const noexcept;

	/** Returns one of the mouse sources that's currently being dragged.
		The index should be between 0 and getNumDraggingMouseSources() - 1. If the index is
		out of range, or if no mice or fingers are down, this will return a null pointer.
	*/
	MouseInputSource* getDraggingMouseSource (int index) const noexcept;

	/** Ensures that a non-stop stream of mouse-drag events will be sent during the
		current mouse-drag operation.

		This allows you to make sure that mouseDrag() events are sent continuously, even
		when the mouse isn't moving. This can be useful for things like auto-scrolling
		components when the mouse is near an edge.

		Call this method during a mouseDown() or mouseDrag() callback, specifying the
		minimum interval between consecutive mouse drag callbacks. The callbacks
		will continue until the mouse is released, and then the interval will be reset,
		so you need to make sure it's called every time you begin a drag event.
		Passing an interval of 0 or less will cancel the auto-repeat.

		@see mouseDrag
	*/
	void beginDragAutoRepeat (int millisecondsBetweenCallbacks);

	/** In a tablet device which can be turned around, this is used to inidicate the orientation. */
	enum DisplayOrientation
	{
		upright                 = 1,  /**< Indicates that the display is the normal way up. */
		upsideDown              = 2,  /**< Indicates that the display is upside-down. */
		rotatedClockwise        = 4,  /**< Indicates that the display is turned 90 degrees clockwise from its upright position. */
		rotatedAntiClockwise    = 8,  /**< Indicates that the display is turned 90 degrees anti-clockwise from its upright position. */

		allOrientations         = 1 + 2 + 4 + 8   /**< A combination of all the orientation values */
	};

	/** In a tablet device which can be turned around, this returns the current orientation. */
	DisplayOrientation getCurrentOrientation() const;

	/** Sets which orientations the display is allowed to auto-rotate to.

		For devices that support rotating desktops, this lets you specify which of the orientations your app can use.

		The parameter is a bitwise or-ed combination of the values in DisplayOrientation, and must contain at least one
		set bit.
	*/
	void setOrientationsEnabled (int allowedOrientations);

	/** Returns whether the display is allowed to auto-rotate to the given orientation.
		Each orientation can be enabled using setOrientationEnabled(). By default, all orientations are allowed.
	*/
	bool isOrientationEnabled (DisplayOrientation orientation) const noexcept;

	class JUCE_API  Displays
	{
	public:
		/** Contains details about a display device. */
		struct Display
		{
			/** This is the bounds of the area of this display which isn't covered by
				OS-dependent objects like the taskbar, menu bar, etc. */
			Rectangle<int> userArea;

			/** This is the total physical area of this display, including any taskbars, etc */
			Rectangle<int> totalArea;

			/** This is the scale-factor of this display.
				For higher-resolution displays, this may be a value greater than 1.0
			*/
			double scale;

			/** This will be true if this is the user's main screen. */
			bool isMain;
		};

		/** Returns the display which acts as user's main screen. */
		const Display& getMainDisplay() const noexcept;

		/** Returns the display which contains a particular point.
			If the point lies outside all the displays, the nearest one will be returned.
		*/
		const Display& getDisplayContaining (const Point<int>& position) const noexcept;

		/** Returns a RectangleList made up of all the displays. */
		RectangleList getRectangleList (bool userAreasOnly) const;

		/** Returns the smallest bounding box which contains all the displays. */
		Rectangle<int> getTotalBounds (bool userAreasOnly) const;

		/** The list of displays. */
		Array<Display> displays;

	   #ifndef DOXYGEN
		/** @internal */
		void refresh();
	   #endif

	private:
		friend class Desktop;
		Displays();
		~Displays();
		void findDisplays();
	};

	const Displays& getDisplays() const noexcept       { return displays; }

	/** True if the OS supports semitransparent windows */
	static bool canUseSemiTransparentWindows() noexcept;

private:

	static Desktop* instance;

	friend class Component;
	friend class ComponentPeer;
	friend class MouseInputSource;
	friend class MouseInputSourceInternal;
	friend class DeletedAtShutdown;
	friend class TopLevelWindowManager;

	OwnedArray <MouseInputSource> mouseSources;
	void createMouseInputSources();

	ListenerList <MouseListener> mouseListeners;
	ListenerList <FocusChangeListener> focusListeners;

	Array <Component*> desktopComponents;

	Displays displays;

	Point<int> lastFakeMouseMove;
	void sendMouseMove();

	int mouseClickCounter;
	void incrementMouseClickCounter() noexcept;

	ScopedPointer<Timer> dragRepeater;

	ScopedPointer<LookAndFeel> defaultLookAndFeel;
	WeakReference<LookAndFeel> currentLookAndFeel;

	Component* kioskModeComponent;
	Rectangle<int> kioskComponentOriginalBounds;

	int allowedOrientations;

	ComponentAnimator animator;

	void timerCallback();
	void resetTimer();
	ListenerList <MouseListener>& getMouseListeners();

	void addDesktopComponent (Component*);
	void removeDesktopComponent (Component*);
	void componentBroughtToFront (Component*);

	void setKioskComponent (Component*, bool enableOrDisable, bool allowMenusAndBars);

	void triggerFocusCallback();
	void handleAsyncUpdate();

	Desktop();
	~Desktop();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Desktop);
};

#endif   // __JUCE_DESKTOP_JUCEHEADER__

/*** End of inlined file: juce_Desktop.h ***/


#endif
#ifndef __JUCE_MODALCOMPONENTMANAGER_JUCEHEADER__

#endif
#ifndef __JUCE_COMPONENTDRAGGER_JUCEHEADER__

/*** Start of inlined file: juce_ComponentDragger.h ***/
#ifndef __JUCE_COMPONENTDRAGGER_JUCEHEADER__
#define __JUCE_COMPONENTDRAGGER_JUCEHEADER__


/*** Start of inlined file: juce_ComponentBoundsConstrainer.h ***/
#ifndef __JUCE_COMPONENTBOUNDSCONSTRAINER_JUCEHEADER__
#define __JUCE_COMPONENTBOUNDSCONSTRAINER_JUCEHEADER__

/**
	A class that imposes restrictions on a Component's size or position.

	This is used by classes such as ResizableCornerComponent,
	ResizableBorderComponent and ResizableWindow.

	The base class can impose some basic size and position limits, but you can
	also subclass this for custom uses.

	@see ResizableCornerComponent, ResizableBorderComponent, ResizableWindow
*/
class JUCE_API  ComponentBoundsConstrainer
{
public:

	/** When first created, the object will not impose any restrictions on the components. */
	ComponentBoundsConstrainer() noexcept;

	/** Destructor. */
	virtual ~ComponentBoundsConstrainer();

	/** Imposes a minimum width limit. */
	void setMinimumWidth (int minimumWidth) noexcept;

	/** Returns the current minimum width. */
	int getMinimumWidth() const noexcept                        { return minW; }

	/** Imposes a maximum width limit. */
	void setMaximumWidth (int maximumWidth) noexcept;

	/** Returns the current maximum width. */
	int getMaximumWidth() const noexcept                        { return maxW; }

	/** Imposes a minimum height limit. */
	void setMinimumHeight (int minimumHeight) noexcept;

	/** Returns the current minimum height. */
	int getMinimumHeight() const noexcept                       { return minH; }

	/** Imposes a maximum height limit. */
	void setMaximumHeight (int maximumHeight) noexcept;

	/** Returns the current maximum height. */
	int getMaximumHeight() const noexcept                       { return maxH; }

	/** Imposes a minimum width and height limit. */
	void setMinimumSize (int minimumWidth,
						 int minimumHeight) noexcept;

	/** Imposes a maximum width and height limit. */
	void setMaximumSize (int maximumWidth,
						 int maximumHeight) noexcept;

	/** Set all the maximum and minimum dimensions. */
	void setSizeLimits (int minimumWidth,
						int minimumHeight,
						int maximumWidth,
						int maximumHeight) noexcept;

	/** Sets the amount by which the component is allowed to go off-screen.

		The values indicate how many pixels must remain on-screen when dragged off
		one of its parent's edges, so e.g. if minimumWhenOffTheTop is set to 10, then
		when the component goes off the top of the screen, its y-position will be
		clipped so that there are always at least 10 pixels on-screen. In other words,
		the lowest y-position it can take would be (10 - the component's height).

		If you pass 0 or less for one of these amounts, the component is allowed
		to move beyond that edge completely, with no restrictions at all.

		If you pass a very large number (i.e. larger that the dimensions of the
		component itself), then the component won't be allowed to overlap that
		edge at all. So e.g. setting minimumWhenOffTheLeft to 0xffffff will mean that
		the component will bump into the left side of the screen and go no further.
	*/
	void setMinimumOnscreenAmounts (int minimumWhenOffTheTop,
									int minimumWhenOffTheLeft,
									int minimumWhenOffTheBottom,
									int minimumWhenOffTheRight) noexcept;

	/** Returns the minimum distance the bounds can be off-screen. @see setMinimumOnscreenAmounts */
	int getMinimumWhenOffTheTop() const noexcept        { return minOffTop; }
	/** Returns the minimum distance the bounds can be off-screen. @see setMinimumOnscreenAmounts */
	int getMinimumWhenOffTheLeft() const noexcept       { return minOffLeft; }
	/** Returns the minimum distance the bounds can be off-screen. @see setMinimumOnscreenAmounts */
	int getMinimumWhenOffTheBottom() const noexcept     { return minOffBottom; }
	/** Returns the minimum distance the bounds can be off-screen. @see setMinimumOnscreenAmounts */
	int getMinimumWhenOffTheRight() const noexcept      { return minOffRight; }

	/** Specifies a width-to-height ratio that the resizer should always maintain.

		If the value is 0, no aspect ratio is enforced. If it's non-zero, the width
		will always be maintained as this multiple of the height.

		@see setResizeLimits
	*/
	void setFixedAspectRatio (double widthOverHeight) noexcept;

	/** Returns the aspect ratio that was set with setFixedAspectRatio().

		If no aspect ratio is being enforced, this will return 0.
	*/
	double getFixedAspectRatio() const noexcept;

	/** This callback changes the given co-ordinates to impose whatever the current
		constraints are set to be.

		@param bounds               the target position that should be examined and adjusted
		@param previousBounds       the component's current size
		@param limits               the region in which the component can be positioned
		@param isStretchingTop      whether the top edge of the component is being resized
		@param isStretchingLeft     whether the left edge of the component is being resized
		@param isStretchingBottom   whether the bottom edge of the component is being resized
		@param isStretchingRight    whether the right edge of the component is being resized
	*/
	virtual void checkBounds (Rectangle<int>& bounds,
							  const Rectangle<int>& previousBounds,
							  const Rectangle<int>& limits,
							  bool isStretchingTop,
							  bool isStretchingLeft,
							  bool isStretchingBottom,
							  bool isStretchingRight);

	/** This callback happens when the resizer is about to start dragging. */
	virtual void resizeStart();

	/** This callback happens when the resizer has finished dragging. */
	virtual void resizeEnd();

	/** Checks the given bounds, and then sets the component to the corrected size. */
	void setBoundsForComponent (Component* component,
								const Rectangle<int>& bounds,
								bool isStretchingTop,
								bool isStretchingLeft,
								bool isStretchingBottom,
								bool isStretchingRight);

	/** Performs a check on the current size of a component, and moves or resizes
		it if it fails the constraints.
	*/
	void checkComponentBounds (Component* component);

	/** Called by setBoundsForComponent() to apply a new constrained size to a
		component.

		By default this just calls setBounds(), but is virtual in case it's needed for
		extremely cunning purposes.
	*/
	virtual void applyBoundsToComponent (Component* component,
										 const Rectangle<int>& bounds);

private:

	int minW, maxW, minH, maxH;
	int minOffTop, minOffLeft, minOffBottom, minOffRight;
	double aspectRatio;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentBoundsConstrainer);
};

#endif   // __JUCE_COMPONENTBOUNDSCONSTRAINER_JUCEHEADER__

/*** End of inlined file: juce_ComponentBoundsConstrainer.h ***/

/**
	An object to take care of the logic for dragging components around with the mouse.

	Very easy to use - in your mouseDown() callback, call startDraggingComponent(),
	then in your mouseDrag() callback, call dragComponent().

	When starting a drag, you can give it a ComponentBoundsConstrainer to use
	to limit the component's position and keep it on-screen.

	e.g. @code
	class MyDraggableComp
	{
		ComponentDragger myDragger;

		void mouseDown (const MouseEvent& e)
		{
			myDragger.startDraggingComponent (this, e);
		}

		void mouseDrag (const MouseEvent& e)
		{
			myDragger.dragComponent (this, e, nullptr);
		}
	};
	@endcode
*/
class JUCE_API  ComponentDragger
{
public:

	/** Creates a ComponentDragger. */
	ComponentDragger();

	/** Destructor. */
	virtual ~ComponentDragger();

	/** Call this from your component's mouseDown() method, to prepare for dragging.

		@param componentToDrag      the component that you want to drag
		@param e                    the mouse event that is triggering the drag
		@see dragComponent
	*/
	void startDraggingComponent (Component* componentToDrag,
								 const MouseEvent& e);

	/** Call this from your mouseDrag() callback to move the component.

		This will move the component, but will first check the validity of the
		component's new position using the checkPosition() method, which you
		can override if you need to enforce special positioning limits on the
		component.

		@param componentToDrag      the component that you want to drag
		@param e                    the current mouse-drag event
		@param constrainer          an optional constrainer object that should be used
									to apply limits to the component's position. Pass
									null if you don't want to contrain the movement.
		@see startDraggingComponent
	*/
	void dragComponent (Component* componentToDrag,
						const MouseEvent& e,
						ComponentBoundsConstrainer* constrainer);

private:

	Point<int> mouseDownWithinTarget;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentDragger);
};

#endif   // __JUCE_COMPONENTDRAGGER_JUCEHEADER__

/*** End of inlined file: juce_ComponentDragger.h ***/


#endif
#ifndef __JUCE_DRAGANDDROPCONTAINER_JUCEHEADER__

/*** Start of inlined file: juce_DragAndDropContainer.h ***/
#ifndef __JUCE_DRAGANDDROPCONTAINER_JUCEHEADER__
#define __JUCE_DRAGANDDROPCONTAINER_JUCEHEADER__


/*** Start of inlined file: juce_DragAndDropTarget.h ***/
#ifndef __JUCE_DRAGANDDROPTARGET_JUCEHEADER__
#define __JUCE_DRAGANDDROPTARGET_JUCEHEADER__

/**
	Components derived from this class can have things dropped onto them by a DragAndDropContainer.

	To create a component that can receive things drag-and-dropped by a DragAndDropContainer,
	derive your component from this class, and make sure that it is somewhere inside a
	DragAndDropContainer component.

	Note: If all that you need to do is to respond to files being drag-and-dropped from
	the operating system onto your component, you don't need any of these classes: instead
	see the FileDragAndDropTarget class.

	@see DragAndDropContainer, FileDragAndDropTarget
*/
class JUCE_API  DragAndDropTarget
{
public:
	/** Destructor. */
	virtual ~DragAndDropTarget()  {}

	/** Contains details about the source of a drag-and-drop operation.
		The contents of this
	*/
	class JUCE_API  SourceDetails
	{
	public:
		/** Creates a SourceDetails object from its various settings. */
		SourceDetails (const var& description, Component* sourceComponent, const Point<int>& localPosition) noexcept;

		/** A descriptor for the drag - this is set DragAndDropContainer::startDragging(). */
		var description;

		/** The component from the drag operation was started. */
		WeakReference<Component> sourceComponent;

		/** The local position of the mouse, relative to the target component.
			Note that for calls such as isInterestedInDragSource(), this may be a null position.
		*/
		Point<int> localPosition;
	};

	/** Callback to check whether this target is interested in the type of object being
		dragged.

		@param dragSourceDetails    contains information about the source of the drag operation.
		@returns                    true if this component wants to receive the other callbacks regarging this
									type of object; if it returns false, no other callbacks will be made.
	*/
	virtual bool isInterestedInDragSource (const SourceDetails& dragSourceDetails) = 0;

	/** Callback to indicate that something is being dragged over this component.

		This gets called when the user moves the mouse into this component while dragging
		something.

		Use this callback as a trigger to make your component repaint itself to give the
		user feedback about whether the item can be dropped here or not.

		@param dragSourceDetails    contains information about the source of the drag operation.
		@see itemDragExit
	*/
	virtual void itemDragEnter (const SourceDetails& dragSourceDetails);

	/** Callback to indicate that the user is dragging something over this component.

		This gets called when the user moves the mouse over this component while dragging
		something. Normally overriding itemDragEnter() and itemDragExit() are enough, but
		this lets you know what happens in-between.

		@param dragSourceDetails    contains information about the source of the drag operation.
	*/
	virtual void itemDragMove (const SourceDetails& dragSourceDetails);

	/** Callback to indicate that something has been dragged off the edge of this component.

		This gets called when the user moves the mouse out of this component while dragging
		something.

		If you've used itemDragEnter() to repaint your component and give feedback, use this
		as a signal to repaint it in its normal state.

		@param dragSourceDetails    contains information about the source of the drag operation.
		@see itemDragEnter
	*/
	virtual void itemDragExit (const SourceDetails& dragSourceDetails);

	/** Callback to indicate that the user has dropped something onto this component.

		When the user drops an item this get called, and you can use the description to
		work out whether your object wants to deal with it or not.

		Note that after this is called, the itemDragExit method may not be called, so you should
		clean up in here if there's anything you need to do when the drag finishes.

		@param dragSourceDetails    contains information about the source of the drag operation.
	*/
	virtual void itemDropped (const SourceDetails& dragSourceDetails) = 0;

	/** Overriding this allows the target to tell the drag container whether to
		draw the drag image while the cursor is over it.

		By default it returns true, but if you return false, then the normal drag
		image will not be shown when the cursor is over this target.
	*/
	virtual bool shouldDrawDragImageWhenOver();

private:
   #if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// The parameters for these methods have changed - please update your code!
	virtual void isInterestedInDragSource (const String&, Component*) {}
	virtual int itemDragEnter (const String&, Component*, int, int) { return 0; }
	virtual int itemDragMove (const String&, Component*, int, int) { return 0; }
	virtual int itemDragExit (const String&, Component*) { return 0; }
	virtual int itemDropped (const String&, Component*, int, int) { return 0; }
   #endif
};

#endif   // __JUCE_DRAGANDDROPTARGET_JUCEHEADER__

/*** End of inlined file: juce_DragAndDropTarget.h ***/

/**
	Enables drag-and-drop behaviour for a component and all its sub-components.

	For a component to be able to make or receive drag-and-drop events, one of its parent
	components must derive from this class. It's probably best for the top-level
	component to implement it.

	Then to start a drag operation, any sub-component can just call the startDragging()
	method, and this object will take over, tracking the mouse and sending appropriate
	callbacks to any child components derived from DragAndDropTarget which the mouse
	moves over.

	Note: If all that you need to do is to respond to files being drag-and-dropped from
	the operating system onto your component, you don't need any of these classes: you can do this
	simply by overriding Component::filesDropped().

	@see DragAndDropTarget
*/
class JUCE_API  DragAndDropContainer
{
public:

	/** Creates a DragAndDropContainer.

		The object that derives from this class must also be a Component.
	*/
	DragAndDropContainer();

	/** Destructor. */
	virtual ~DragAndDropContainer();

	/** Begins a drag-and-drop operation.

		This starts a drag-and-drop operation - call it when the user drags the
		mouse in your drag-source component, and this object will track mouse
		movements until the user lets go of the mouse button, and will send
		appropriate messages to DragAndDropTarget objects that the mouse moves
		over.

		findParentDragContainerFor() is a handy method to call to find the
		drag container to use for a component.

		@param sourceDescription    a string or value to use as the description of the thing being dragged -
									this will be passed to the objects that might be dropped-onto so they can
									decide whether they want to handle it
		@param sourceComponent      the component that is being dragged
		@param dragImage            the image to drag around underneath the mouse. If this is a null image,
									a snapshot of the sourceComponent will be used instead.
		@param allowDraggingToOtherJuceWindows   if true, the dragged component will appear as a desktop
									window, and can be dragged to DragAndDropTargets that are the
									children of components other than this one.
		@param imageOffsetFromMouse if an image has been passed-in, this specifies the offset
									at which the image should be drawn from the mouse. If it isn't
									specified, then the image will be centred around the mouse. If
									an image hasn't been passed-in, this will be ignored.
	*/
	void startDragging (const var& sourceDescription,
						Component* sourceComponent,
						const Image& dragImage = Image::null,
						bool allowDraggingToOtherJuceWindows = false,
						const Point<int>* imageOffsetFromMouse = nullptr);

	/** Returns true if something is currently being dragged. */
	bool isDragAndDropActive() const;

	/** Returns the description of the thing that's currently being dragged.

		If nothing's being dragged, this will return an empty string, otherwise it's the
		string that was passed into startDragging().

		@see startDragging
	*/
	String getCurrentDragDescription() const;

	/** Utility to find the DragAndDropContainer for a given Component.

		This will search up this component's parent hierarchy looking for the first
		parent component which is a DragAndDropContainer.

		It's useful when a component wants to call startDragging but doesn't know
		the DragAndDropContainer it should to use.

		Obviously this may return 0 if it doesn't find a suitable component.
	*/
	static DragAndDropContainer* findParentDragContainerFor (Component* childComponent);

	/** This performs a synchronous drag-and-drop of a set of files to some external
		application.

		You can call this function in response to a mouseDrag callback, and it will
		block, running its own internal message loop and tracking the mouse, while it
		uses a native operating system drag-and-drop operation to move or copy some
		files to another application.

		@param files            a list of filenames to drag
		@param canMoveFiles     if true, the app that receives the files is allowed to move the files to a new location
								(if this is appropriate). If false, the receiver is expected to make a copy of them.
		@returns        true if the files were successfully dropped somewhere, or false if it
						was interrupted
		@see performExternalDragDropOfText
	*/
	static bool performExternalDragDropOfFiles (const StringArray& files, bool canMoveFiles);

	/** This performs a synchronous drag-and-drop of a block of text to some external
		application.

		You can call this function in response to a mouseDrag callback, and it will
		block, running its own internal message loop and tracking the mouse, while it
		uses a native operating system drag-and-drop operation to move or copy some
		text to another application.

		@param text     the text to copy
		@returns        true if the text was successfully dropped somewhere, or false if it
						was interrupted
		@see performExternalDragDropOfFiles
	*/
	static bool performExternalDragDropOfText (const String& text);

protected:
	/** Override this if you want to be able to perform an external drag a set of files
		when the user drags outside of this container component.

		This method will be called when a drag operation moves outside the Juce-based window,
		and if you want it to then perform a file drag-and-drop, add the filenames you want
		to the array passed in, and return true.

		@param sourceDetails    information about the source of the drag operation
		@param files            on return, the filenames you want to drag
		@param canMoveFiles     on return, true if it's ok for the receiver to move the files; false if
								it must make a copy of them (see the performExternalDragDropOfFiles() method)
		@see performExternalDragDropOfFiles
	*/
	virtual bool shouldDropFilesWhenDraggedExternally (const DragAndDropTarget::SourceDetails& sourceDetails,
													   StringArray& files, bool& canMoveFiles);

private:

	friend class DragImageComponent;
	ScopedPointer <Component> dragImageComponent;
	String currentDragDesc;

	JUCE_DEPRECATED (virtual bool shouldDropFilesWhenDraggedExternally (const String&, Component*, StringArray&, bool&)) { return false; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DragAndDropContainer);
};

#endif   // __JUCE_DRAGANDDROPCONTAINER_JUCEHEADER__

/*** End of inlined file: juce_DragAndDropContainer.h ***/


#endif
#ifndef __JUCE_DRAGANDDROPTARGET_JUCEHEADER__

#endif
#ifndef __JUCE_FILEDRAGANDDROPTARGET_JUCEHEADER__

/*** Start of inlined file: juce_FileDragAndDropTarget.h ***/
#ifndef __JUCE_FILEDRAGANDDROPTARGET_JUCEHEADER__
#define __JUCE_FILEDRAGANDDROPTARGET_JUCEHEADER__

/**
	Components derived from this class can have files dropped onto them by an external application.

	@see DragAndDropContainer
*/
class JUCE_API  FileDragAndDropTarget
{
public:
	/** Destructor. */
	virtual ~FileDragAndDropTarget()  {}

	/** Callback to check whether this target is interested in the set of files being offered.

		Note that this will be called repeatedly when the user is dragging the mouse around over your
		component, so don't do anything time-consuming in here, like opening the files to have a look
		inside them!

		@param files        the set of (absolute) pathnames of the files that the user is dragging
		@returns            true if this component wants to receive the other callbacks regarging this
							type of object; if it returns false, no other callbacks will be made.
	*/
	virtual bool isInterestedInFileDrag (const StringArray& files) = 0;

	/** Callback to indicate that some files are being dragged over this component.

		This gets called when the user moves the mouse into this component while dragging.

		Use this callback as a trigger to make your component repaint itself to give the
		user feedback about whether the files can be dropped here or not.

		@param files        the set of (absolute) pathnames of the files that the user is dragging
		@param x            the mouse x position, relative to this component
		@param y            the mouse y position, relative to this component
	*/
	virtual void fileDragEnter (const StringArray& files, int x, int y);

	/** Callback to indicate that the user is dragging some files over this component.

		This gets called when the user moves the mouse over this component while dragging.
		Normally overriding itemDragEnter() and itemDragExit() are enough, but
		this lets you know what happens in-between.

		@param files        the set of (absolute) pathnames of the files that the user is dragging
		@param x            the mouse x position, relative to this component
		@param y            the mouse y position, relative to this component
	*/
	virtual void fileDragMove (const StringArray& files, int x, int y);

	/** Callback to indicate that the mouse has moved away from this component.

		This gets called when the user moves the mouse out of this component while dragging
		the files.

		If you've used fileDragEnter() to repaint your component and give feedback, use this
		as a signal to repaint it in its normal state.

		@param files        the set of (absolute) pathnames of the files that the user is dragging
	*/
	virtual void fileDragExit (const StringArray& files);

	/** Callback to indicate that the user has dropped the files onto this component.

		When the user drops the files, this get called, and you can use the files in whatever
		way is appropriate.

		Note that after this is called, the fileDragExit method may not be called, so you should
		clean up in here if there's anything you need to do when the drag finishes.

		@param files        the set of (absolute) pathnames of the files that the user is dragging
		@param x            the mouse x position, relative to this component
		@param y            the mouse y position, relative to this component
	*/
	virtual void filesDropped (const StringArray& files, int x, int y) = 0;
};

#endif   // __JUCE_FILEDRAGANDDROPTARGET_JUCEHEADER__

/*** End of inlined file: juce_FileDragAndDropTarget.h ***/


#endif
#ifndef __JUCE_LASSOCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_LassoComponent.h ***/
#ifndef __JUCE_LASSOCOMPONENT_JUCEHEADER__
#define __JUCE_LASSOCOMPONENT_JUCEHEADER__


/*** Start of inlined file: juce_SelectedItemSet.h ***/
#ifndef __JUCE_SELECTEDITEMSET_JUCEHEADER__
#define __JUCE_SELECTEDITEMSET_JUCEHEADER__

/** Manages a list of selectable items.

	Use one of these to keep a track of things that the user has highlighted, like
	icons or things in a list.

	The class is templated so that you can use it to hold either a set of pointers
	to objects, or a set of ID numbers or handles, for cases where each item may
	not always have a corresponding object.

	To be informed when items are selected/deselected, register a ChangeListener with
	this object.

	@see SelectableObject
*/
template <class SelectableItemType>
class JUCE_API  SelectedItemSet   : public ChangeBroadcaster
{
public:

	typedef SelectableItemType ItemType;
	typedef PARAMETER_TYPE (SelectableItemType) ParameterType;

	/** Creates an empty set. */
	SelectedItemSet()
	{
	}

	/** Creates a set based on an array of items. */
	explicit SelectedItemSet (const Array <SelectableItemType>& items)
		: selectedItems (items)
	{
	}

	/** Creates a copy of another set. */
	SelectedItemSet (const SelectedItemSet& other)
		: selectedItems (other.selectedItems)
	{
	}

	/** Creates a copy of another set. */
	SelectedItemSet& operator= (const SelectedItemSet& other)
	{
		if (selectedItems != other.selectedItems)
		{
			selectedItems = other.selectedItems;
			changed();
		}

		return *this;
	}

	/** Clears any other currently selected items, and selects this item.

		If this item is already the only thing selected, no change notification
		will be sent out.

		@see addToSelection, addToSelectionBasedOnModifiers
	*/
	void selectOnly (ParameterType item)
	{
		if (isSelected (item))
		{
			for (int i = selectedItems.size(); --i >= 0;)
			{
				if (selectedItems.getUnchecked(i) != item)
				{
					deselect (selectedItems.getUnchecked(i));
					i = jmin (i, selectedItems.size());
				}
			}
		}
		else
		{
			deselectAll();
			changed();

			selectedItems.add (item);
			itemSelected (item);
		}
	}

	/** Selects an item.

		If the item is already selected, no change notification will be sent out.

		@see selectOnly, addToSelectionBasedOnModifiers
	*/
	void addToSelection (ParameterType item)
	{
		if (! isSelected (item))
		{
			changed();

			selectedItems.add (item);
			itemSelected (item);
		}
	}

	/** Selects or deselects an item.

		This will use the modifier keys to decide whether to deselect other items
		first.

		So if the shift key is held down, the item will be added without deselecting
		anything (same as calling addToSelection() )

		If no modifiers are down, the current selection will be cleared first (same
		as calling selectOnly() )

		If the ctrl (or command on the Mac) key is held down, the item will be toggled -
		so it'll be added to the set unless it's already there, in which case it'll be
		deselected.

		If the items that you're selecting can also be dragged, you may need to use the
		addToSelectionOnMouseDown() and addToSelectionOnMouseUp() calls to handle the
		subtleties of this kind of usage.

		@see selectOnly, addToSelection, addToSelectionOnMouseDown, addToSelectionOnMouseUp
	*/
	void addToSelectionBasedOnModifiers (ParameterType item,
										 const ModifierKeys& modifiers)
	{
		if (modifiers.isShiftDown())
		{
			addToSelection (item);
		}
		else if (modifiers.isCommandDown())
		{
			if (isSelected (item))
				deselect (item);
			else
				addToSelection (item);
		}
		else
		{
			selectOnly (item);
		}
	}

	/** Selects or deselects items that can also be dragged, based on a mouse-down event.

		If you call addToSelectionOnMouseDown() at the start of your mouseDown event,
		and then call addToSelectionOnMouseUp() at the end of your mouseUp event, this
		makes it easy to handle multiple-selection of sets of objects that can also
		be dragged.

		For example, if you have several items already selected, and you click on
		one of them (without dragging), then you'd expect this to deselect the other, and
		just select the item you clicked on. But if you had clicked on this item and
		dragged it, you'd have expected them all to stay selected.

		When you call this method, you'll need to store the boolean result, because the
		addToSelectionOnMouseUp() method will need to be know this value.

		@see addToSelectionOnMouseUp, addToSelectionBasedOnModifiers
	*/
	bool addToSelectionOnMouseDown (ParameterType item,
									const ModifierKeys& modifiers)
	{
		if (isSelected (item))
			return ! modifiers.isPopupMenu();

		addToSelectionBasedOnModifiers (item, modifiers);
		return false;
	}

	/** Selects or deselects items that can also be dragged, based on a mouse-up event.

		Call this during a mouseUp callback, when you have previously called the
		addToSelectionOnMouseDown() method during your mouseDown event.

		See addToSelectionOnMouseDown() for more info

		@param item             the item to select (or deselect)
		@param modifiers        the modifiers from the mouse-up event
		@param wasItemDragged   true if your item was dragged during the mouse click
		@param resultOfMouseDownSelectMethod    this is the boolean return value that came
								back from the addToSelectionOnMouseDown() call that you
								should have made during the matching mouseDown event
	*/
	void addToSelectionOnMouseUp (ParameterType item,
								  const ModifierKeys& modifiers,
								  const bool wasItemDragged,
								  const bool resultOfMouseDownSelectMethod)
	{
		if (resultOfMouseDownSelectMethod && ! wasItemDragged)
			addToSelectionBasedOnModifiers (item, modifiers);
	}

	/** Deselects an item. */
	void deselect (ParameterType item)
	{
		const int i = selectedItems.indexOf (item);

		if (i >= 0)
		{
			changed();
			itemDeselected (selectedItems.remove (i));
		}
	}

	/** Deselects all items. */
	void deselectAll()
	{
		if (selectedItems.size() > 0)
		{
			changed();

			for (int i = selectedItems.size(); --i >= 0;)
			{
				itemDeselected (selectedItems.remove (i));
				i = jmin (i, selectedItems.size());
			}
		}
	}

	/** Returns the number of currently selected items.

		@see getSelectedItem
	*/
	int getNumSelected() const noexcept
	{
		return selectedItems.size();
	}

	/** Returns one of the currently selected items.

		Returns 0 if the index is out-of-range.

		@see getNumSelected
	*/
	SelectableItemType getSelectedItem (const int index) const noexcept
	{
		return selectedItems [index];
	}

	/** True if this item is currently selected. */
	bool isSelected (ParameterType item) const noexcept
	{
		return selectedItems.contains (item);
	}

	const Array <SelectableItemType>& getItemArray() const noexcept         { return selectedItems; }

	/** Can be overridden to do special handling when an item is selected.

		For example, if the item is an object, you might want to call it and tell
		it that it's being selected.
	*/
	virtual void itemSelected (SelectableItemType item)                     { (void) item; }

	/** Can be overridden to do special handling when an item is deselected.

		For example, if the item is an object, you might want to call it and tell
		it that it's being deselected.
	*/
	virtual void itemDeselected (SelectableItemType item)                   { (void) item; }

	/** Used internally, but can be called to force a change message to be sent to the ChangeListeners.
	*/
	void changed (const bool synchronous = false)
	{
		if (synchronous)
			sendSynchronousChangeMessage();
		else
			sendChangeMessage();
	}

private:

	Array <SelectableItemType> selectedItems;

	JUCE_LEAK_DETECTOR (SelectedItemSet <SelectableItemType>);
};

#endif   // __JUCE_SELECTEDITEMSET_JUCEHEADER__

/*** End of inlined file: juce_SelectedItemSet.h ***/

/**
	A class used by the LassoComponent to manage the things that it selects.

	This allows the LassoComponent to find out which items are within the lasso,
	and to change the list of selected items.

	@see LassoComponent, SelectedItemSet
*/
template <class SelectableItemType>
class LassoSource
{
public:
	/** Destructor. */
	virtual ~LassoSource() {}

	/** Returns the set of items that lie within a given lassoable region.

		Your implementation of this method must find all the relevent items that lie
		within the given rectangle. and add them to the itemsFound array.

		The co-ordinates are relative to the top-left of the lasso component's parent
		component. (i.e. they are the same as the size and position of the lasso
		component itself).
	*/
	virtual void findLassoItemsInArea (Array <SelectableItemType>& itemsFound,
									   const Rectangle<int>& area) = 0;

	/** Returns the SelectedItemSet that the lasso should update.

		This set will be continuously updated by the LassoComponent as it gets
		dragged around, so make sure that you've got a ChangeListener attached to
		the set so that your UI objects will know when the selection changes and
		be able to update themselves appropriately.
	*/
	virtual SelectedItemSet <SelectableItemType>& getLassoSelection() = 0;
};

/**
	A component that acts as a rectangular selection region, which you drag with
	the mouse to select groups of objects (in conjunction with a SelectedItemSet).

	To use one of these:

	- In your mouseDown or mouseDrag event, add the LassoComponent to your parent
	  component, and call its beginLasso() method, giving it a
	  suitable LassoSource object that it can use to find out which items are in
	  the active area.

	- Each time your parent component gets a mouseDrag event, call dragLasso()
	  to update the lasso's position - it will use its LassoSource to calculate and
	  update the current selection.

	- After the drag has finished and you get a mouseUp callback, you should call
	  endLasso() to clean up. This will make the lasso component invisible, and you
	  can remove it from the parent component, or delete it.

	The class takes into account the modifier keys that are being held down while
	the lasso is being dragged, so if shift is pressed, then any lassoed items will
	be added to the original selection; if ctrl or command is pressed, they will be
	xor'ed with any previously selected items.

	@see LassoSource, SelectedItemSet
*/
template <class SelectableItemType>
class LassoComponent  : public Component
{
public:

	/** Creates a Lasso component.

		The fill colour is used to fill the lasso'ed rectangle, and the outline
		colour is used to draw a line around its edge.
	*/
	explicit LassoComponent (const int outlineThickness_ = 1)
		: source (nullptr),
		  outlineThickness (outlineThickness_)
	{
	}

	/** Destructor. */
	~LassoComponent()
	{
	}

	/** Call this in your mouseDown event, to initialise a drag.

		Pass in a suitable LassoSource object which the lasso will use to find
		the items and change the selection.

		After using this method to initialise the lasso, repeatedly call dragLasso()
		in your component's mouseDrag callback.

		@see dragLasso, endLasso, LassoSource
	*/
	void beginLasso (const MouseEvent& e,
					 LassoSource <SelectableItemType>* const lassoSource)
	{
		jassert (source == nullptr);  // this suggests that you didn't call endLasso() after the last drag...
		jassert (lassoSource != nullptr); // the source can't be null!
		jassert (getParentComponent() != nullptr);  // you need to add this to a parent component for it to work!

		source = lassoSource;

		if (lassoSource != nullptr)
			originalSelection = lassoSource->getLassoSelection().getItemArray();

		setSize (0, 0);
		dragStartPos = e.getMouseDownPosition();
	}

	/** Call this in your mouseDrag event, to update the lasso's position.

		This must be repeatedly calling when the mouse is dragged, after you've
		first initialised the lasso with beginLasso().

		This method takes into account the modifier keys that are being held down, so
		if shift is pressed, then the lassoed items will be added to any that were
		previously selected; if ctrl or command is pressed, then they will be xor'ed
		with previously selected items.

		@see beginLasso, endLasso
	*/
	void dragLasso (const MouseEvent& e)
	{
		if (source != nullptr)
		{
			setBounds (Rectangle<int> (dragStartPos, e.getPosition()));
			setVisible (true);

			Array <SelectableItemType> itemsInLasso;
			source->findLassoItemsInArea (itemsInLasso, getBounds());

			if (e.mods.isShiftDown())
			{
				itemsInLasso.removeValuesIn (originalSelection); //  to avoid duplicates
				itemsInLasso.addArray (originalSelection);
			}
			else if (e.mods.isCommandDown() || e.mods.isAltDown())
			{
				Array <SelectableItemType> originalMinusNew (originalSelection);
				originalMinusNew.removeValuesIn (itemsInLasso);

				itemsInLasso.removeValuesIn (originalSelection);
				itemsInLasso.addArray (originalMinusNew);
			}

			source->getLassoSelection() = SelectedItemSet <SelectableItemType> (itemsInLasso);
		}
	}

	/** Call this in your mouseUp event, after the lasso has been dragged.

		@see beginLasso, dragLasso
	*/
	void endLasso()
	{
		source = nullptr;
		originalSelection.clear();
		setVisible (false);
	}

	/** A set of colour IDs to use to change the colour of various aspects of the label.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		Note that you can also use the constants from TextEditor::ColourIds to change the
		colour of the text editor that is opened when a label is editable.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		lassoFillColourId       = 0x1000440, /**< The colour to fill the lasso rectangle with. */
		lassoOutlineColourId    = 0x1000441, /**< The colour to draw the outline with. */
	};

	/** @internal */
	void paint (Graphics& g)
	{
		g.fillAll (findColour (lassoFillColourId));

		g.setColour (findColour (lassoOutlineColourId));
		g.drawRect (0, 0, getWidth(), getHeight(), outlineThickness);

		// this suggests that you've left a lasso comp lying around after the
		// mouse drag has finished.. Be careful to call endLasso() when you get a
		// mouse-up event.
		jassert (isMouseButtonDownAnywhere());
	}

	/** @internal */
	bool hitTest (int, int)             { return false; }

private:

	Array <SelectableItemType> originalSelection;
	LassoSource <SelectableItemType>* source;
	int outlineThickness;
	Point<int> dragStartPos;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LassoComponent);
};

#endif   // __JUCE_LASSOCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_LassoComponent.h ***/


#endif
#ifndef __JUCE_MOUSECURSOR_JUCEHEADER__

#endif
#ifndef __JUCE_MOUSEEVENT_JUCEHEADER__

#endif
#ifndef __JUCE_MOUSEINPUTSOURCE_JUCEHEADER__

/*** Start of inlined file: juce_MouseInputSource.h ***/
#ifndef __JUCE_MOUSEINPUTSOURCE_JUCEHEADER__
#define __JUCE_MOUSEINPUTSOURCE_JUCEHEADER__

class MouseInputSourceInternal;

/**
	Represents a linear source of mouse events from a mouse device or individual finger
	in a multi-touch environment.

	Each MouseEvent object contains a reference to the MouseInputSource that generated
	it. In an environment with a single mouse for input, all events will come from the
	same source, but in a multi-touch system, there may be multiple MouseInputSource
	obects active, each representing a stream of events coming from a particular finger.

	Events coming from a single MouseInputSource are always sent in a fixed and predictable
	order: a mouseMove will never be called without a mouseEnter having been sent beforehand,
	the only events that can happen between a mouseDown and its corresponding mouseUp are
	mouseDrags, etc.
	When there are multiple touches arriving from multiple MouseInputSources, their
	event streams may arrive in an interleaved order, so you should use the getIndex()
	method to find out which finger each event came from.

	@see MouseEvent
*/
class JUCE_API  MouseInputSource
{
public:

	/** Creates a MouseInputSource.
		You should never actually create a MouseInputSource in your own code - the
		library takes care of managing these objects.
	*/
	MouseInputSource (int index, bool isMouseDevice);

	/** Destructor. */
	~MouseInputSource();

	/** Returns true if this object represents a normal desk-based mouse device. */
	bool isMouse() const;

	/** Returns true if this object represents a source of touch events - i.e. a finger or stylus. */
	bool isTouch() const;

	/** Returns true if this source has an on-screen pointer that can hover over
		items without clicking them.
	*/
	bool canHover() const;

	/** Returns true if this source may have a scroll wheel. */
	bool hasMouseWheel() const;

	/** Returns this source's index in the global list of possible sources.
		If the system only has a single mouse, there will only be a single MouseInputSource
		with an index of 0.

		If the system supports multi-touch input, then the index will represent a finger
		number, starting from 0. When the first touch event begins, it will have finger
		number 0, and then if a second touch happens while the first is still down, it
		will have index 1, etc.
	*/
	int getIndex() const;

	/** Returns true if this device is currently being pressed. */
	bool isDragging() const;

	/** Returns the last-known screen position of this source. */
	Point<int> getScreenPosition() const;

	/** Returns a set of modifiers that indicate which buttons are currently
		held down on this device.
	*/
	ModifierKeys getCurrentModifiers() const;

	/** Returns the component that was last known to be under this pointer. */
	Component* getComponentUnderMouse() const;

	/** Tells the device to dispatch a mouse-move or mouse-drag event.
		This is asynchronous - the event will occur on the message thread.
	*/
	void triggerFakeMove() const;

	/** Returns the number of clicks that should be counted as belonging to the
		current mouse event.
		So the mouse is currently down and it's the second click of a double-click, this
		will return 2.
	*/
	int getNumberOfMultipleClicks() const noexcept;

	/** Returns the time at which the last mouse-down occurred. */
	Time getLastMouseDownTime() const noexcept;

	/** Returns the screen position at which the last mouse-down occurred. */
	Point<int> getLastMouseDownPosition() const noexcept;

	/** Returns true if this mouse is currently down, and if it has been dragged more
		than a couple of pixels from the place it was pressed.
	*/
	bool hasMouseMovedSignificantlySincePressed() const noexcept;

	/** Returns true if this input source uses a visible mouse cursor. */
	bool hasMouseCursor() const noexcept;

	/** Changes the mouse cursor, (if there is one). */
	void showMouseCursor (const MouseCursor& cursor);

	/** Hides the mouse cursor (if there is one). */
	void hideCursor();

	/** Un-hides the mouse cursor if it was hidden by hideCursor(). */
	void revealCursor();

	/** Forces an update of the mouse cursor for whatever component it's currently over. */
	void forceMouseCursorUpdate();

	/** Returns true if this mouse can be moved indefinitely in any direction without running out of space. */
	bool canDoUnboundedMovement() const noexcept;

	/** Allows the mouse to move beyond the edges of the screen.

		Calling this method when the mouse button is currently pressed will remove the cursor
		from the screen and allow the mouse to (seem to) move beyond the edges of the screen.

		This means that the co-ordinates returned to mouseDrag() will be unbounded, and this
		can be used for things like custom slider controls or dragging objects around, where
		movement would be otherwise be limited by the mouse hitting the edges of the screen.

		The unbounded mode is automatically turned off when the mouse button is released, or
		it can be turned off explicitly by calling this method again.

		@param isEnabled                            whether to turn this mode on or off
		@param keepCursorVisibleUntilOffscreen      if set to false, the cursor will immediately be
													hidden; if true, it will only be hidden when it
													is moved beyond the edge of the screen
	*/
	void enableUnboundedMouseMovement (bool isEnabled, bool keepCursorVisibleUntilOffscreen = false);

	/** @internal */
	void handleEvent (ComponentPeer* peer, const Point<int>& positionWithinPeer, int64 time, const ModifierKeys& mods);
	/** @internal */
	void handleWheel (ComponentPeer* peer, const Point<int>& positionWithinPeer, int64 time, float x, float y);

private:

	friend class Desktop;
	friend class ComponentPeer;
	friend class MouseInputSourceInternal;
	ScopedPointer<MouseInputSourceInternal> pimpl;

	static Point<int> getCurrentMousePosition();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MouseInputSource);
};

#endif   // __JUCE_MOUSEINPUTSOURCE_JUCEHEADER__

/*** End of inlined file: juce_MouseInputSource.h ***/


#endif
#ifndef __JUCE_MOUSELISTENER_JUCEHEADER__

#endif
#ifndef __JUCE_SELECTEDITEMSET_JUCEHEADER__

#endif
#ifndef __JUCE_TOOLTIPCLIENT_JUCEHEADER__

/*** Start of inlined file: juce_TooltipClient.h ***/
#ifndef __JUCE_TOOLTIPCLIENT_JUCEHEADER__
#define __JUCE_TOOLTIPCLIENT_JUCEHEADER__

/**
	Components that want to use pop-up tooltips should implement this interface.

	A TooltipWindow will wait for the mouse to hover over a component that
	implements the TooltipClient interface, and when it finds one, it will display
	the tooltip returned by its getTooltip() method.

	@see TooltipWindow, SettableTooltipClient
*/
class JUCE_API  TooltipClient
{
public:
	/** Destructor. */
	virtual ~TooltipClient()  {}

	/** Returns the string that this object wants to show as its tooltip. */
	virtual String getTooltip() = 0;
};

/**
	An implementation of TooltipClient that stores the tooltip string and a method
	for changing it.

	This makes it easy to add a tooltip to a custom component, by simply adding this
	as a base class and calling setTooltip().

	Many of the Juce widgets already use this as a base class to implement their
	tooltips.

	@see TooltipClient, TooltipWindow
*/
class JUCE_API  SettableTooltipClient   : public TooltipClient
{
public:

	/** Destructor. */
	virtual ~SettableTooltipClient()                                {}

	/** Assigns a new tooltip to this object. */
	virtual void setTooltip (const String& newTooltip)              { tooltipString = newTooltip; }

	/** Returns the tooltip assigned to this object. */
	virtual String getTooltip()                                     { return tooltipString; }

protected:
	SettableTooltipClient() {}

private:
	String tooltipString;
};

#endif   // __JUCE_TOOLTIPCLIENT_JUCEHEADER__

/*** End of inlined file: juce_TooltipClient.h ***/


#endif
#ifndef __JUCE_CARETCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_CaretComponent.h ***/
#ifndef __JUCE_CARETCOMPONENT_JUCEHEADER__
#define __JUCE_CARETCOMPONENT_JUCEHEADER__

/**
*/
class JUCE_API  CaretComponent   : public Component,
								   public Timer
{
public:

	/** Creates the caret component.
		The keyFocusOwner is an optional component which the caret will check, making
		itself visible only when the keyFocusOwner has keyboard focus.
	*/
	CaretComponent (Component* keyFocusOwner);

	/** Destructor. */
	~CaretComponent();

	/** Sets the caret's position to place it next to the given character.
		The area is the rectangle containing the entire character that the caret is
		positioned on, so by default a vertical-line caret may choose to just show itself
		at the left of this area. You can override this method to customise its size.
		This method will also force the caret to reset its timer and become visible (if
		appropriate), so that as it moves, you can see where it is.
	*/
	virtual void setCaretPosition (const Rectangle<int>& characterArea);

	/** A set of colour IDs to use to change the colour of various aspects of the caret.
		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.
		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		caretColourId    = 0x1000204, /**< The colour with which to draw the caret. */
	};

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void timerCallback();

private:
	Component* owner;
	bool shouldBeShown() const;

	JUCE_DECLARE_NON_COPYABLE (CaretComponent);
};

#endif   // __JUCE_CARETCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_CaretComponent.h ***/


#endif
#ifndef __JUCE_KEYBOARDFOCUSTRAVERSER_JUCEHEADER__

#endif
#ifndef __JUCE_KEYLISTENER_JUCEHEADER__

#endif
#ifndef __JUCE_KEYPRESS_JUCEHEADER__

#endif
#ifndef __JUCE_MODIFIERKEYS_JUCEHEADER__

#endif
#ifndef __JUCE_SYSTEMCLIPBOARD_JUCEHEADER__

/*** Start of inlined file: juce_SystemClipboard.h ***/
#ifndef __JUCE_SYSTEMCLIPBOARD_JUCEHEADER__
#define __JUCE_SYSTEMCLIPBOARD_JUCEHEADER__

/**
	Handles reading/writing to the system's clipboard.
*/
class JUCE_API  SystemClipboard
{
public:
	/** Copies a string of text onto the clipboard */
	static void copyTextToClipboard (const String& text);

	/** Gets the current clipboard's contents.

		Obviously this might have come from another app, so could contain
		anything..
	*/
	static String getTextFromClipboard();
};

#endif   // __JUCE_SYSTEMCLIPBOARD_JUCEHEADER__

/*** End of inlined file: juce_SystemClipboard.h ***/


#endif
#ifndef __JUCE_TEXTEDITORKEYMAPPER_JUCEHEADER__

/*** Start of inlined file: juce_TextEditorKeyMapper.h ***/
#ifndef __JUCE_TEXTEDITORKEYMAPPER_JUCEHEADER__
#define __JUCE_TEXTEDITORKEYMAPPER_JUCEHEADER__

/** This class is used to invoke a range of text-editor navigation methods on
	an object, based upon a keypress event.

	It's currently used internally by the TextEditor and CodeEditorComponent.
*/
template <class CallbackClass>
struct TextEditorKeyMapper
{
	/** Checks the keypress and invokes one of a range of navigation functions that
		the target class must implement, based on the key event.
	*/
	static bool invokeKeyFunction (CallbackClass& target, const KeyPress& key)
	{
		const bool isShiftDown   = key.getModifiers().isShiftDown();
		const bool ctrlOrAltDown = key.getModifiers().isCtrlDown() || key.getModifiers().isAltDown();

		if (key == KeyPress (KeyPress::downKey, ModifierKeys::ctrlModifier, 0) && target.scrollUp())   return true;
		if (key == KeyPress (KeyPress::upKey,   ModifierKeys::ctrlModifier, 0) && target.scrollDown()) return true;

	   #if JUCE_MAC
		if (key.getModifiers().isCommandDown())
		{
			if (key.isKeyCode (KeyPress::upKey))        return target.moveCaretToTop (isShiftDown);
			if (key.isKeyCode (KeyPress::downKey))      return target.moveCaretToEnd (isShiftDown);
			if (key.isKeyCode (KeyPress::leftKey))      return target.moveCaretToStartOfLine (isShiftDown);
			if (key.isKeyCode (KeyPress::rightKey))     return target.moveCaretToEndOfLine (isShiftDown);
		}
	   #endif

		if (key.isKeyCode (KeyPress::upKey))        return target.moveCaretUp (isShiftDown);
		if (key.isKeyCode (KeyPress::downKey))      return target.moveCaretDown (isShiftDown);
		if (key.isKeyCode (KeyPress::leftKey))      return target.moveCaretLeft (ctrlOrAltDown, isShiftDown);
		if (key.isKeyCode (KeyPress::rightKey))     return target.moveCaretRight (ctrlOrAltDown, isShiftDown);
		if (key.isKeyCode (KeyPress::pageUpKey))    return target.pageUp (isShiftDown);
		if (key.isKeyCode (KeyPress::pageDownKey))  return target.pageDown (isShiftDown);

		if (key.isKeyCode (KeyPress::homeKey))  return ctrlOrAltDown ? target.moveCaretToTop (isShiftDown)
																	 : target.moveCaretToStartOfLine (isShiftDown);
		if (key.isKeyCode (KeyPress::endKey))   return ctrlOrAltDown ? target.moveCaretToEnd (isShiftDown)
																	 : target.moveCaretToEndOfLine (isShiftDown);

		if (key == KeyPress ('c', ModifierKeys::commandModifier, 0)
			  || key == KeyPress (KeyPress::insertKey, ModifierKeys::ctrlModifier, 0))
			return target.copyToClipboard();

		if (key == KeyPress ('x', ModifierKeys::commandModifier, 0)
			  || key == KeyPress (KeyPress::deleteKey, ModifierKeys::shiftModifier, 0))
			return target.cutToClipboard();

		if (key == KeyPress ('v', ModifierKeys::commandModifier, 0)
			  || key == KeyPress (KeyPress::insertKey, ModifierKeys::shiftModifier, 0))
			return target.pasteFromClipboard();

		if (key.isKeyCode (KeyPress::backspaceKey))     return target.deleteBackwards (ctrlOrAltDown);
		if (key.isKeyCode (KeyPress::deleteKey))        return target.deleteForwards (ctrlOrAltDown);

		if (key == KeyPress ('a', ModifierKeys::commandModifier, 0))
			return target.selectAll();

		if (key == KeyPress ('z', ModifierKeys::commandModifier, 0))
			return target.undo();

		if (key == KeyPress ('y', ModifierKeys::commandModifier, 0)
			 || key == KeyPress ('z', ModifierKeys::commandModifier | ModifierKeys::shiftModifier, 0))
			return target.redo();

		return false;
	}
};

#endif   // __JUCE_TEXTEDITORKEYMAPPER_JUCEHEADER__

/*** End of inlined file: juce_TextEditorKeyMapper.h ***/


#endif
#ifndef __JUCE_TEXTINPUTTARGET_JUCEHEADER__

/*** Start of inlined file: juce_TextInputTarget.h ***/
#ifndef __JUCE_TEXTINPUTTARGET_JUCEHEADER__
#define __JUCE_TEXTINPUTTARGET_JUCEHEADER__

/**
	An abstract base class which can be implemented by components that function as
	text editors.

	This class allows different types of text editor component to provide a uniform
	interface, which can be used by things like OS-specific input methods, on-screen
	keyboards, etc.
*/
class JUCE_API  TextInputTarget
{
public:

	/** */
	TextInputTarget() {}

	/** Destructor. */
	virtual ~TextInputTarget() {}

	/** Returns true if this input target is currently accepting input.
		For example, a text editor might return false if it's in read-only mode.
	*/
	virtual bool isTextInputActive() const = 0;

	/** Returns the extents of the selected text region, or an empty range if
		nothing is selected,
	*/
	virtual Range<int> getHighlightedRegion() const = 0;

	/** Sets the currently-selected text region. */
	virtual void setHighlightedRegion (const Range<int>& newRange) = 0;

	/** Sets a number of temporarily underlined sections.
		This is needed by MS Windows input method UI.
	*/
	virtual void setTemporaryUnderlining (const Array <Range<int> >& underlinedRegions) = 0;

	/** Returns a specified sub-section of the text. */
	virtual String getTextInRange (const Range<int>& range) const = 0;

	/** Inserts some text, overwriting the selected text region, if there is one. */
	virtual void insertTextAtCaret (const String& textToInsert) = 0;

	/** Returns the position of the caret, relative to the component's origin. */
	virtual Rectangle<int> getCaretRectangle() = 0;
};

#endif   // __JUCE_TEXTINPUTTARGET_JUCEHEADER__

/*** End of inlined file: juce_TextInputTarget.h ***/


#endif
#ifndef __JUCE_ARROWBUTTON_JUCEHEADER__

/*** Start of inlined file: juce_ArrowButton.h ***/
#ifndef __JUCE_ARROWBUTTON_JUCEHEADER__
#define __JUCE_ARROWBUTTON_JUCEHEADER__


/*** Start of inlined file: juce_Button.h ***/
#ifndef __JUCE_BUTTON_JUCEHEADER__
#define __JUCE_BUTTON_JUCEHEADER__


/*** Start of inlined file: juce_ApplicationCommandManager.h ***/
#ifndef __JUCE_APPLICATIONCOMMANDMANAGER_JUCEHEADER__
#define __JUCE_APPLICATIONCOMMANDMANAGER_JUCEHEADER__


/*** Start of inlined file: juce_ApplicationCommandTarget.h ***/
#ifndef __JUCE_APPLICATIONCOMMANDTARGET_JUCEHEADER__
#define __JUCE_APPLICATIONCOMMANDTARGET_JUCEHEADER__


/*** Start of inlined file: juce_ApplicationCommandInfo.h ***/
#ifndef __JUCE_APPLICATIONCOMMANDINFO_JUCEHEADER__
#define __JUCE_APPLICATIONCOMMANDINFO_JUCEHEADER__


/*** Start of inlined file: juce_ApplicationCommandID.h ***/
#ifndef __JUCE_APPLICATIONCOMMANDID_JUCEHEADER__
#define __JUCE_APPLICATIONCOMMANDID_JUCEHEADER__

/** A type used to hold the unique ID for an application command.

	This is a numeric type, so it can be stored as an integer.

	@see ApplicationCommandInfo, ApplicationCommandManager,
		 ApplicationCommandTarget, KeyPressMappingSet
*/
typedef int CommandID;

/** A set of general-purpose application command IDs.

	Because these commands are likely to be used in most apps, they're defined
	here to help different apps to use the same numeric values for them.

	Of course you don't have to use these, but some of them are used internally by
	Juce - e.g. the quit ID is recognised as a command by the JUCEApplication class.

	@see ApplicationCommandInfo, ApplicationCommandManager,
		 ApplicationCommandTarget, KeyPressMappingSet
*/
namespace StandardApplicationCommandIDs
{
	/** This command ID should be used to send a "Quit the App" command.

		This command is recognised by the JUCEApplication class, so if it is invoked
		and no other ApplicationCommandTarget handles the event first, the JUCEApplication
		object will catch it and call JUCEApplication::systemRequestedQuit().
	*/
	static const CommandID  quit           = 0x1001;

	/** The command ID that should be used to send a "Delete" command. */
	static const CommandID  del            = 0x1002;

	/** The command ID that should be used to send a "Cut" command. */
	static const CommandID  cut            = 0x1003;

	/** The command ID that should be used to send a "Copy to clipboard" command. */
	static const CommandID  copy           = 0x1004;

	/** The command ID that should be used to send a "Paste from clipboard" command. */
	static const CommandID  paste          = 0x1005;

	/** The command ID that should be used to send a "Select all" command. */
	static const CommandID  selectAll      = 0x1006;

	/** The command ID that should be used to send a "Deselect all" command. */
	static const CommandID  deselectAll    = 0x1007;
}

#endif   // __JUCE_APPLICATIONCOMMANDID_JUCEHEADER__

/*** End of inlined file: juce_ApplicationCommandID.h ***/

/**
	Holds information describing an application command.

	This object is used to pass information about a particular command, such as its
	name, description and other usage flags.

	When an ApplicationCommandTarget is asked to provide information about the commands
	it can perform, this is the structure gets filled-in to describe each one.

	@see ApplicationCommandTarget, ApplicationCommandTarget::getCommandInfo(),
		 ApplicationCommandManager
*/
struct JUCE_API  ApplicationCommandInfo
{

	explicit ApplicationCommandInfo (CommandID commandID) noexcept;

	/** Sets a number of the structures values at once.

		The meanings of each of the parameters is described below, in the appropriate
		member variable's description.
	*/
	void setInfo (const String& shortName,
				  const String& description,
				  const String& categoryName,
				  int flags) noexcept;

	/** An easy way to set or remove the isDisabled bit in the structure's flags field.

		If isActive is true, the flags member has the isDisabled bit cleared; if isActive
		is false, the bit is set.
	*/
	void setActive (bool isActive) noexcept;

	/** An easy way to set or remove the isTicked bit in the structure's flags field.
	*/
	void setTicked (bool isTicked) noexcept;

	/** Handy method for adding a keypress to the defaultKeypresses array.

		This is just so you can write things like:
		@code
		myinfo.addDefaultKeypress ('s', ModifierKeys::commandModifier);
		@endcode
		instead of
		@code
		myinfo.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier));
		@endcode
	*/
	void addDefaultKeypress (int keyCode,
							 const ModifierKeys& modifiers) noexcept;

	/** The command's unique ID number.
	*/
	CommandID commandID;

	/** A short name to describe the command.

		This should be suitable for use in menus, on buttons that trigger the command, etc.

		You can use the setInfo() method to quickly set this and some of the command's
		other properties.
	*/
	String shortName;

	/** A longer description of the command.

		This should be suitable for use in contexts such as a KeyMappingEditorComponent or
		pop-up tooltip describing what the command does.

		You can use the setInfo() method to quickly set this and some of the command's
		other properties.
	*/
	String description;

	/** A named category that the command fits into.

		You can give your commands any category you like, and these will be displayed in
		contexts such as the KeyMappingEditorComponent, where the category is used to group
		commands together.

		You can use the setInfo() method to quickly set this and some of the command's
		other properties.
	*/
	String categoryName;

	/** A list of zero or more keypresses that should be used as the default keys for
		this command.

		Methods such as KeyPressMappingSet::resetToDefaultMappings() will use the keypresses in
		this list to initialise the default set of key-to-command mappings.

		@see addDefaultKeypress
	*/
	Array <KeyPress> defaultKeypresses;

	/** Flags describing the ways in which this command should be used.

		A bitwise-OR of these values is stored in the ApplicationCommandInfo::flags
		variable.
	*/
	enum CommandFlags
	{
		/** Indicates that the command can't currently be performed.

			The ApplicationCommandTarget::getCommandInfo() method must set this flag if it's
			not currently permissable to perform the command. If the flag is set, then
			components that trigger the command, e.g. PopupMenu, may choose to grey-out the
			command or show themselves as not being enabled.

			@see ApplicationCommandInfo::setActive
		*/
		isDisabled                  = 1 << 0,

		/** Indicates that the command should have a tick next to it on a menu.

			If your command is shown on a menu and this is set, it'll show a tick next to
			it. Other components such as buttons may also use this flag to indicate that it
			is a value that can be toggled, and is currently in the 'on' state.

			@see ApplicationCommandInfo::setTicked
		*/
		isTicked                    = 1 << 1,

		/** If this flag is present, then when a KeyPressMappingSet invokes the command,
			it will call the command twice, once on key-down and again on key-up.

			@see ApplicationCommandTarget::InvocationInfo
		*/
		wantsKeyUpDownCallbacks     = 1 << 2,

		/** If this flag is present, then a KeyMappingEditorComponent will not display the
			command in its list.
		*/
		hiddenFromKeyEditor         = 1 << 3,

		/** If this flag is present, then a KeyMappingEditorComponent will display the
			command in its list, but won't allow the assigned keypress to be changed.
		*/
		readOnlyInKeyEditor         = 1 << 4,

		/** If this flag is present and the command is invoked from a keypress, then any
			buttons or menus that are also connected to the command will not flash to
			indicate that they've been triggered.
		*/
		dontTriggerVisualFeedback   = 1 << 5
	};

	/** A bitwise-OR of the values specified in the CommandFlags enum.

		You can use the setInfo() method to quickly set this and some of the command's
		other properties.
	*/
	int flags;
};

#endif   // __JUCE_APPLICATIONCOMMANDINFO_JUCEHEADER__

/*** End of inlined file: juce_ApplicationCommandInfo.h ***/

class Component;

/**
	A command target publishes a list of command IDs that it can perform.

	An ApplicationCommandManager despatches commands to targets, which must be
	able to provide information about what commands they can handle.

	To create a target, you'll need to inherit from this class, implementing all of
	its pure virtual methods.

	For info about how a target is chosen to receive a command, see
	ApplicationCommandManager::getFirstCommandTarget().

	@see ApplicationCommandManager, ApplicationCommandInfo
*/
class JUCE_API  ApplicationCommandTarget
{
public:

	/** Creates a command target. */
	ApplicationCommandTarget();

	/** Destructor. */
	virtual ~ApplicationCommandTarget();

	/**
	*/
	struct JUCE_API  InvocationInfo
	{

		InvocationInfo (const CommandID commandID);

		/** The UID of the command that should be performed. */
		CommandID commandID;

		/** The command's flags.

			See ApplicationCommandInfo for a description of these flag values.
		*/
		int commandFlags;

		/** The types of context in which the command might be called. */
		enum InvocationMethod
		{
			direct = 0,     /**< The command is being invoked directly by a piece of code. */
			fromKeyPress,   /**< The command is being invoked by a key-press. */
			fromMenu,       /**< The command is being invoked by a menu selection. */
			fromButton      /**< The command is being invoked by a button click. */
		};

		/** The type of event that triggered this command. */
		InvocationMethod invocationMethod;

		/** If triggered by a keypress or menu, this will be the component that had the
			keyboard focus at the time.

			If triggered by a button, it may be set to that component, or it may be null.
		*/
		Component* originatingComponent;

		/** The keypress that was used to invoke it.

			Note that this will be an invalid keypress if the command was invoked
			by some other means than a keyboard shortcut.
		*/
		KeyPress keyPress;

		/** True if the callback is being invoked when the key is pressed,
			false if the key is being released.

			@see KeyPressMappingSet::addCommand()
		*/
		bool isKeyDown;

		/** If the key is being released, this indicates how long it had been held
			down for.

			(Only relevant if isKeyDown is false.)
		*/
		int millisecsSinceKeyPressed;
	};

	/** This must return the next target to try after this one.

		When a command is being sent, and the first target can't handle
		that command, this method is used to determine the next target that should
		be tried.

		It may return 0 if it doesn't know of another target.

		If your target is a Component, you would usually use the findFirstTargetParentComponent()
		method to return a parent component that might want to handle it.

		@see invoke
	*/
	virtual ApplicationCommandTarget* getNextCommandTarget() = 0;

	/** This must return a complete list of commands that this target can handle.

		Your target should add all the command IDs that it handles to the array that is
		passed-in.
	*/
	virtual void getAllCommands (Array <CommandID>& commands) = 0;

	/** This must provide details about one of the commands that this target can perform.

		This will be called with one of the command IDs that the target provided in its
		getAllCommands() methods.

		It should fill-in all appropriate fields of the ApplicationCommandInfo structure with
		suitable information about the command. (The commandID field will already have been filled-in
		by the caller).

		The easiest way to set the info is using the ApplicationCommandInfo::setInfo() method to
		set all the fields at once.

		If the command is currently inactive for some reason, this method must use
		ApplicationCommandInfo::setActive() to make that clear, (or it should set the isDisabled
		bit of the ApplicationCommandInfo::flags field).

		Any default key-presses for the command should be appended to the
		ApplicationCommandInfo::defaultKeypresses field.

		Note that if you change something that affects the status of the commands
		that would be returned by this method (e.g. something that makes some commands
		active or inactive), you should call ApplicationCommandManager::commandStatusChanged()
		to cause the manager to refresh its status.
	*/
	virtual void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) = 0;

	/** This must actually perform the specified command.

		If this target is able to perform the command specified by the commandID field of the
		InvocationInfo structure, then it should do so, and must return true.

		If it can't handle this command, it should return false, which tells the caller to pass
		the command on to the next target in line.

		@see invoke, ApplicationCommandManager::invoke
	*/
	virtual bool perform (const InvocationInfo& info) = 0;

	/** Makes this target invoke a command.

		Your code can call this method to invoke a command on this target, but normally
		you'd call it indirectly via ApplicationCommandManager::invoke() or
		ApplicationCommandManager::invokeDirectly().

		If this target can perform the given command, it will call its perform() method to
		do so. If not, then getNextCommandTarget() will be used to determine the next target
		to try, and the command will be passed along to it.

		@param invocationInfo       this must be correctly filled-in, describing the context for
									the invocation.
		@param asynchronously       if false, the command will be performed before this method returns.
									If true, a message will be posted so that the command will be performed
									later on the message thread, and this method will return immediately.
		@see perform, ApplicationCommandManager::invoke
	*/
	bool invoke (const InvocationInfo& invocationInfo,
				 const bool asynchronously);

	/** Invokes a given command directly on this target.

		This is just an easy way to call invoke() without having to fill out the InvocationInfo
		structure.
	*/
	bool invokeDirectly (const CommandID commandID,
						 const bool asynchronously);

	/** Searches this target and all subsequent ones for the first one that can handle
		the specified command.

		This will use getNextCommandTarget() to determine the chain of targets to try
		after this one.
	*/
	ApplicationCommandTarget* getTargetForCommand (const CommandID commandID);

	/** Checks whether this command can currently be performed by this target.

		This will return true only if a call to getCommandInfo() doesn't set the
		isDisabled flag to indicate that the command is inactive.
	*/
	bool isCommandActive (const CommandID commandID);

	/** If this object is a Component, this method will seach upwards in its current
		UI hierarchy for the next parent component that implements the
		ApplicationCommandTarget class.

		If your target is a Component, this is a very handy method to use in your
		getNextCommandTarget() implementation.
	*/
	ApplicationCommandTarget* findFirstTargetParentComponent();

private:

	WeakReference<ApplicationCommandTarget>::Master masterReference;
	friend class WeakReference<ApplicationCommandTarget>;

	class CommandMessage;
	friend class CommandMessage;

	bool tryToInvoke (const InvocationInfo&, bool async);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationCommandTarget);
};

#endif   // __JUCE_APPLICATIONCOMMANDTARGET_JUCEHEADER__

/*** End of inlined file: juce_ApplicationCommandTarget.h ***/

class KeyPressMappingSet;
class ApplicationCommandManagerListener;
class Desktop;

/**
	One of these objects holds a list of all the commands your app can perform,
	and despatches these commands when needed.

	Application commands are a good way to trigger actions in your app, e.g. "Quit",
	"Copy", "Paste", etc. Menus, buttons and keypresses can all be given commands
	to invoke automatically, which means you don't have to handle the result of a menu
	or button click manually. Commands are despatched to ApplicationCommandTarget objects
	which can choose which events they want to handle.

	This architecture also allows for nested ApplicationCommandTargets, so that for example
	you could have two different objects, one inside the other, both of which can respond to
	a "delete" command. Depending on which one has focus, the command will be sent to the
	appropriate place, regardless of whether it was triggered by a menu, keypress or some other
	method.

	To set up your app to use commands, you'll need to do the following:

	- Create a global ApplicationCommandManager to hold the list of all possible
	  commands. (This will also manage a set of key-mappings for them).

	- Make some of your UI components (or other objects) inherit from ApplicationCommandTarget.
	  This allows the object to provide a list of commands that it can perform, and
	  to handle them.

	- Register each type of command using ApplicationCommandManager::registerAllCommandsForTarget(),
	  or ApplicationCommandManager::registerCommand().

	- If you want key-presses to trigger your commands, use the ApplicationCommandManager::getKeyMappings()
	  method to access the key-mapper object, which you will need to register as a key-listener
	  in whatever top-level component you're using. See the KeyPressMappingSet class for more help
	  about setting this up.

	- Use methods such as PopupMenu::addCommandItem() or Button::setCommandToTrigger() to
	  cause these commands to be invoked automatically.

	- Commands can be invoked directly by your code using ApplicationCommandManager::invokeDirectly().

	When a command is invoked, the ApplicationCommandManager will try to choose the best
	ApplicationCommandTarget to receive the specified command. To do this it will use the
	current keyboard focus to see which component might be interested, and will search the
	component hierarchy for those that also implement the ApplicationCommandTarget interface.
	If an ApplicationCommandTarget isn't interested in the command that is being invoked, then
	the next one in line will be tried (see the ApplicationCommandTarget::getNextCommandTarget()
	method), and so on until ApplicationCommandTarget::getNextCommandTarget() returns 0. At this
	point if the command still hasn't been performed, it will be passed to the current
	JUCEApplication object (which is itself an ApplicationCommandTarget).

	To exert some custom control over which ApplicationCommandTarget is chosen to invoke a command,
	you can override the ApplicationCommandManager::getFirstCommandTarget() method and choose
	the object yourself.

	@see ApplicationCommandTarget, ApplicationCommandInfo
*/
class JUCE_API  ApplicationCommandManager   : private AsyncUpdater,
											  private FocusChangeListener
{
public:

	/** Creates an ApplicationCommandManager.

		Once created, you'll need to register all your app's commands with it, using
		ApplicationCommandManager::registerAllCommandsForTarget() or
		ApplicationCommandManager::registerCommand().
	*/
	ApplicationCommandManager();

	/** Destructor.

		Make sure that you don't delete this if pointers to it are still being used by
		objects such as PopupMenus or Buttons.
	*/
	virtual ~ApplicationCommandManager();

	/** Clears the current list of all commands.

		Note that this will also clear the contents of the KeyPressMappingSet.
	*/
	void clearCommands();

	/** Adds a command to the list of registered commands.

		@see registerAllCommandsForTarget
	*/
	void registerCommand (const ApplicationCommandInfo& newCommand);

	/** Adds all the commands that this target publishes to the manager's list.

		This will use ApplicationCommandTarget::getAllCommands() and ApplicationCommandTarget::getCommandInfo()
		to get details about all the commands that this target can do, and will call
		registerCommand() to add each one to the manger's list.

		@see registerCommand
	*/
	void registerAllCommandsForTarget (ApplicationCommandTarget* target);

	/** Removes the command with a specified ID.

		Note that this will also remove any key mappings that are mapped to the command.
	*/
	void removeCommand (CommandID commandID);

	/** This should be called to tell the manager that one of its registered commands may have changed
		its active status.

		Because the command manager only finds out whether a command is active or inactive by querying
		the current ApplicationCommandTarget, this is used to tell it that things may have changed. It
		allows things like buttons to update their enablement, etc.

		This method will cause an asynchronous call to ApplicationCommandManagerListener::applicationCommandListChanged()
		for any registered listeners.
	*/
	void commandStatusChanged();

	/** Returns the number of commands that have been registered.

		@see registerCommand
	*/
	int getNumCommands() const noexcept                                             { return commands.size(); }

	/** Returns the details about one of the registered commands.

		The index is between 0 and (getNumCommands() - 1).
	*/
	const ApplicationCommandInfo* getCommandForIndex (int index) const noexcept     { return commands [index]; }

	/** Returns the details about a given command ID.

		This will search the list of registered commands for one with the given command
		ID number, and return its associated info. If no matching command is found, this
		will return 0.
	*/
	const ApplicationCommandInfo* getCommandForID (CommandID commandID) const noexcept;

	/** Returns the name field for a command.

		An empty string is returned if no command with this ID has been registered.
		@see getDescriptionOfCommand
	*/
	String getNameOfCommand (CommandID commandID) const noexcept;

	/** Returns the description field for a command.

		An empty string is returned if no command with this ID has been registered. If the
		command has no description, this will return its short name field instead.

		@see getNameOfCommand
	*/
	String getDescriptionOfCommand (CommandID commandID) const noexcept;

	/** Returns the list of categories.

		This will go through all registered commands, and return a list of all the distict
		categoryName values from their ApplicationCommandInfo structure.

		@see getCommandsInCategory()
	*/
	StringArray getCommandCategories() const;

	/** Returns a list of all the command UIDs in a particular category.

		@see getCommandCategories()
	*/
	Array<CommandID> getCommandsInCategory (const String& categoryName) const;

	/** Returns the manager's internal set of key mappings.

		This object can be used to edit the keypresses. To actually link this object up
		to invoke commands when a key is pressed, see the comments for the KeyPressMappingSet
		class.

		@see KeyPressMappingSet
	*/
	KeyPressMappingSet* getKeyMappings() const noexcept                         { return keyMappings; }

	/** Invokes the given command directly, sending it to the default target.

		This is just an easy way to call invoke() without having to fill out the InvocationInfo
		structure.
	*/
	bool invokeDirectly (CommandID commandID, bool asynchronously);

	/** Sends a command to the default target.

		This will choose a target using getFirstCommandTarget(), and send the specified command
		to it using the ApplicationCommandTarget::invoke() method. This means that if the
		first target can't handle the command, it will be passed on to targets further down the
		chain (see ApplicationCommandTarget::invoke() for more info).

		@param invocationInfo       this must be correctly filled-in, describing the context for
									the invocation.
		@param asynchronously       if false, the command will be performed before this method returns.
									If true, a message will be posted so that the command will be performed
									later on the message thread, and this method will return immediately.

		@see ApplicationCommandTarget::invoke
	*/
	bool invoke (const ApplicationCommandTarget::InvocationInfo& invocationInfo,
				 bool asynchronously);

	/** Chooses the ApplicationCommandTarget to which a command should be sent.

		Whenever the manager needs to know which target a command should be sent to, it calls
		this method to determine the first one to try.

		By default, this method will return the target that was set by calling setFirstCommandTarget().
		If no target is set, it will return the result of findDefaultComponentTarget().

		If you need to make sure all commands go via your own custom target, then you can
		either use setFirstCommandTarget() to specify a single target, or override this method
		if you need more complex logic to choose one.

		It may return 0 if no targets are available.

		@see getTargetForCommand, invoke, invokeDirectly
	*/
	virtual ApplicationCommandTarget* getFirstCommandTarget (CommandID commandID);

	/** Sets a target to be returned by getFirstCommandTarget().

		If this is set to 0, then getFirstCommandTarget() will by default return the
		result of findDefaultComponentTarget().

		If you use this to set a target, make sure you call setFirstCommandTarget (0) before
		deleting the target object.
	*/
	void setFirstCommandTarget (ApplicationCommandTarget* newTarget) noexcept;

	/** Tries to find the best target to use to perform a given command.

		This will call getFirstCommandTarget() to find the preferred target, and will
		check whether that target can handle the given command. If it can't, then it'll use
		ApplicationCommandTarget::getNextCommandTarget() to find the next one to try, and
		so on until no more are available.

		If no targets are found that can perform the command, this method will return 0.

		If a target is found, then it will get the target to fill-in the upToDateInfo
		structure with the latest info about that command, so that the caller can see
		whether the command is disabled, ticked, etc.
	*/
	ApplicationCommandTarget* getTargetForCommand (CommandID commandID,
												   ApplicationCommandInfo& upToDateInfo);

	/** Registers a listener that will be called when various events occur. */
	void addListener (ApplicationCommandManagerListener* listener);

	/** Deregisters a previously-added listener. */
	void removeListener (ApplicationCommandManagerListener* listener);

	/** Looks for a suitable command target based on which Components have the keyboard focus.

		This is used by the default implementation of ApplicationCommandTarget::getFirstCommandTarget(),
		but is exposed here in case it's useful.

		It tries to pick the best ApplicationCommandTarget by looking at focused components, top level
		windows, etc., and using the findTargetForComponent() method.
	*/
	static ApplicationCommandTarget* findDefaultComponentTarget();

	/** Examines this component and all its parents in turn, looking for the first one
		which is a ApplicationCommandTarget.

		Returns the first ApplicationCommandTarget that it finds, or 0 if none of them implement
		that class.
	*/
	static ApplicationCommandTarget* findTargetForComponent (Component* component);

private:

	OwnedArray <ApplicationCommandInfo> commands;
	ListenerList <ApplicationCommandManagerListener> listeners;
	ScopedPointer <KeyPressMappingSet> keyMappings;
	ApplicationCommandTarget* firstTarget;

	void sendListenerInvokeCallback (const ApplicationCommandTarget::InvocationInfo& info);
	void handleAsyncUpdate();
	void globalFocusChanged (Component*);

   #if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// This is just here to cause a compile error in old code that hasn't been changed to use the new
	// version of this method.
	virtual short getFirstCommandTarget() { return 0; }
   #endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationCommandManager);
};

/**
	A listener that receives callbacks from an ApplicationCommandManager when
	commands are invoked or the command list is changed.

	@see ApplicationCommandManager::addListener, ApplicationCommandManager::removeListener

*/
class JUCE_API  ApplicationCommandManagerListener
{
public:

	/** Destructor. */
	virtual ~ApplicationCommandManagerListener()  {}

	/** Called when an app command is about to be invoked. */
	virtual void applicationCommandInvoked (const ApplicationCommandTarget::InvocationInfo& info) = 0;

	/** Called when commands are registered or deregistered from the
		command manager, or when commands are made active or inactive.

		Note that if you're using this to watch for changes to whether a command is disabled,
		you'll need to make sure that ApplicationCommandManager::commandStatusChanged() is called
		whenever the status of your command might have changed.
	*/
	virtual void applicationCommandListChanged() = 0;
};

#endif   // __JUCE_APPLICATIONCOMMANDMANAGER_JUCEHEADER__

/*** End of inlined file: juce_ApplicationCommandManager.h ***/


/*** Start of inlined file: juce_TooltipWindow.h ***/
#ifndef __JUCE_TOOLTIPWINDOW_JUCEHEADER__
#define __JUCE_TOOLTIPWINDOW_JUCEHEADER__

/**
	A window that displays a pop-up tooltip when the mouse hovers over another component.

	To enable tooltips in your app, just create a single instance of a TooltipWindow
	object.

	The TooltipWindow object will then stay invisible, waiting until the mouse
	hovers for the specified length of time - it will then see if it's currently
	over a component which implements the TooltipClient interface, and if so,
	it will make itself visible to show the tooltip in the appropriate place.

	@see TooltipClient, SettableTooltipClient
*/
class JUCE_API  TooltipWindow  : public Component,
								 private Timer
{
public:

	/** Creates a tooltip window.

		Make sure your app only creates one instance of this class, otherwise you'll
		get multiple overlaid tooltips appearing. The window will initially be invisible
		and will make itself visible when it needs to display a tip.

		To change the style of tooltips, see the LookAndFeel class for its tooltip
		methods.

		@param parentComponent  if set to 0, the TooltipWindow will appear on the desktop,
								otherwise the tooltip will be added to the given parent
								component.
		@param millisecondsBeforeTipAppears     the time for which the mouse has to stay still
												before a tooltip will be shown

		@see TooltipClient, LookAndFeel::drawTooltip, LookAndFeel::getTooltipSize
	*/
	explicit TooltipWindow (Component* parentComponent = nullptr,
							int millisecondsBeforeTipAppears = 700);

	/** Destructor. */
	~TooltipWindow();

	/** Changes the time before the tip appears.
		This lets you change the value that was set in the constructor.
	*/
	void setMillisecondsBeforeTipAppears (int newTimeMs = 700) noexcept;

	/** A set of colour IDs to use to change the colour of various aspects of the tooltip.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId      = 0x1001b00,    /**< The colour to fill the background with. */
		textColourId            = 0x1001c00,    /**< The colour to use for the text. */
		outlineColourId         = 0x1001c10     /**< The colour to use to draw an outline around the tooltip. */
	};

private:

	int millisecondsBeforeTipAppears;
	Point<int> lastMousePos;
	int mouseClicks;
	unsigned int lastCompChangeTime, lastHideTime;
	Component* lastComponentUnderMouse;
	bool changedCompsSinceShown;
	String tipShowing, lastTipUnderMouse;

	void paint (Graphics& g);
	void mouseEnter (const MouseEvent& e);
	void timerCallback();

	static String getTipFor (Component* c);
	void showFor (const String& tip);
	void hide();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TooltipWindow);
};

#endif   // __JUCE_TOOLTIPWINDOW_JUCEHEADER__

/*** End of inlined file: juce_TooltipWindow.h ***/


/*** Start of inlined file: juce_ComponentBuilder.h ***/
#ifndef __JUCE_COMPONENTBUILDER_JUCEHEADER__
#define __JUCE_COMPONENTBUILDER_JUCEHEADER__

/**
	Loads and maintains a tree of Components from a ValueTree that represents them.

	To allow the state of a tree of components to be saved as a ValueTree and re-loaded,
	this class lets you register a set of type-handlers for the different components that
	are involved, and then uses these types to re-create a set of components from its
	stored state.

	Essentially, to use this, you need to create a ComponentBuilder with your ValueTree,
	then use registerTypeHandler() to give it a set of type handlers that can cope with
	all the items in your tree. Then you can call getComponent() to build the component.
	Once you've got the component you can either take it and delete the ComponentBuilder
	object, or if you keep the ComponentBuilder around, it'll monitor any changes in the
	ValueTree and automatically update the component to reflect these changes.
*/
class JUCE_API  ComponentBuilder  : public ValueTree::Listener
{
public:
	/** Creates a ComponentBuilder that will use the given state.
		Once you've created your builder, you should use registerTypeHandler() to register some
		type handlers for it, and then you can call createComponent() or getManagedComponent()
		to get the actual component.
	*/
	explicit ComponentBuilder (const ValueTree& state);

	/** Creates a builder that doesn't have a state object. */
	ComponentBuilder();

	/** Destructor. */
	~ComponentBuilder();

	/** This is the ValueTree data object that the builder is working with. */
	ValueTree state;

	/** Returns the builder's component (creating it if necessary).

		The first time that this method is called, the builder will attempt to create a component
		from the ValueTree, so you must have registered some suitable type handlers before calling
		this. If there's a problem and the component can't be created, this method returns 0.

		The component that is returned is owned by this ComponentBuilder, so you can put it inside
		your own parent components, but don't delete it! The ComponentBuilder will delete it automatically
		when the builder is destroyed. If you want to get a component that you can delete yourself,
		call createComponent() instead.

		The ComponentBuilder will update this component if any changes are made to the ValueTree, so if
		there's a chance that the tree might change, be careful not to keep any pointers to sub-components,
		as they may be changed or removed.
	*/
	Component* getManagedComponent();

	/** Creates and returns a new instance of the component that the ValueTree represents.
		The caller is responsible for using and deleting the object that is returned. Unlike
		getManagedComponent(), the component that is returned will not be updated by the builder.
	*/
	Component* createComponent();

	/**
		The class is a base class for objects that manage the loading of a type of component
		from a ValueTree.

		To store and re-load a tree of components as a ValueTree, each component type must have
		a TypeHandler to represent it.

		@see ComponentBuilder::registerTypeHandler(), Drawable::registerDrawableTypeHandlers()
	*/
	class JUCE_API  TypeHandler
	{
	public:

		/** Creates a TypeHandler.
			The valueTreeType must be the type name of the ValueTrees that this handler can parse.
		*/
		explicit TypeHandler (const Identifier& valueTreeType);

		/** Destructor. */
		virtual ~TypeHandler();

		/** Returns the type of the ValueTrees that this handler can parse. */
		const Identifier type;

		/** Returns the builder that this type is registered with. */
		ComponentBuilder* getBuilder() const noexcept;

		/** This method must create a new component from the given state, add it to the specified
			parent component (which may be null), and return it.

			The ValueTree will have been pre-checked to make sure that its type matches the type
			that this handler supports.

			There's no need to set the new Component's ID to match that of the state - the builder
			will take care of that itself.
		*/
		virtual Component* addNewComponentFromState (const ValueTree& state, Component* parent) = 0;

		/** This method must update an existing component from a new ValueTree state.

			A component that has been created with addNewComponentFromState() may need to be updated
			if the ValueTree changes, so this method is used to do that. Your implementation must do
			whatever's necessary to update the component from the new state provided.

			The ValueTree will have been pre-checked to make sure that its type matches the type
			that this handler supports, and the component will have been created by this type's
			addNewComponentFromState() method.
		*/
		virtual void updateComponentFromState (Component* component, const ValueTree& state) = 0;

	private:

		friend class ComponentBuilder;
		ComponentBuilder* builder;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TypeHandler);
	};

	/** Adds a type handler that the builder can use when trying to load components.
		@see Drawable::registerDrawableTypeHandlers()
	*/
	void registerTypeHandler (TypeHandler* type);

	/** Tries to find a registered type handler that can load a component from the given ValueTree. */
	TypeHandler* getHandlerForState (const ValueTree& state) const;

	/** Returns the number of registered type handlers.
		@see getHandler, registerTypeHandler
	*/
	int getNumHandlers() const noexcept;

	/** Returns one of the registered type handlers.
		@see getNumHandlers, registerTypeHandler
	*/
	TypeHandler* getHandler (int index) const noexcept;

	/** Registers handlers for various standard juce components. */
	void registerStandardComponentTypes();

	/** This class is used when references to images need to be stored in ValueTrees.

		An instance of an ImageProvider provides a mechanism for converting an Image to/from
		a reference, which may be a file, URL, ID string, or whatever system is appropriate in
		your app.

		When you're loading components from a ValueTree that may need a way of loading images, you
		should call ComponentBuilder::setImageProvider() to supply a suitable provider before
		trying to load the component.

		@see ComponentBuilder::setImageProvider()
	*/
	class JUCE_API  ImageProvider
	{
	public:
		ImageProvider() {}
		virtual ~ImageProvider() {}

		/** Retrieves the image associated with this identifier, which could be any
			kind of string, number, filename, etc.

			The image that is returned will be owned by the caller, but it may come
			from the ImageCache.
		*/
		virtual Image getImageForIdentifier (const var& imageIdentifier) = 0;

		/** Returns an identifier to be used to refer to a given image.
			This is used when a reference to an image is stored in a ValueTree.
		*/
		virtual var getIdentifierForImage (const Image& image) = 0;
	};

	/** Gives the builder an ImageProvider object that the type handlers can use when
		loading images from stored references.

		The object that is passed in is not owned by the builder, so the caller must delete
		it when it is no longer needed, but not while the builder may still be using it. To
		clear the image provider, just call setImageProvider (nullptr).
	*/
	void setImageProvider (ImageProvider* newImageProvider) noexcept;

	/** Returns the current image provider that this builder is using, or 0 if none has been set. */
	ImageProvider* getImageProvider() const noexcept;

	/** Updates the children of a parent component by updating them from the children of
		a given ValueTree.
	*/
	void updateChildComponents (Component& parent, const ValueTree& children);

	/** An identifier for the property of the ValueTrees that is used to store a unique ID
		for that component.
	*/
	static const Identifier idProperty;

	/**
	*/
	static void initialiseFromValueTree (Component& component,
										 const ValueTree& state,
										 ImageProvider* imageProvider);

	/** @internal */
	void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
	/** @internal */
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded);
	/** @internal */
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved);
	/** @internal */
	void valueTreeChildOrderChanged (ValueTree& parentTree);
	/** @internal */
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged);
	/** @internal */
	static void refreshBasicComponentProperties (Component&, const ValueTree&);
	/** @internal */
	static RelativeRectangle getComponentBounds (const ValueTree&);

private:

	OwnedArray <TypeHandler> types;
	ScopedPointer<Component> component;
	ImageProvider* imageProvider;
   #if JUCE_DEBUG
	WeakReference<Component> componentRef;
   #endif

	static const Identifier positionID;
	void initialiseRecursively (Component&, const ValueTree&);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentBuilder);
};

#endif   // __JUCE_COMPONENTBUILDER_JUCEHEADER__

/*** End of inlined file: juce_ComponentBuilder.h ***/

/**
	A base class for buttons.

	This contains all the logic for button behaviours such as enabling/disabling,
	responding to shortcut keystrokes, auto-repeating when held down, toggle-buttons
	and radio groups, etc.

	@see TextButton, DrawableButton, ToggleButton
*/
class JUCE_API  Button  : public Component,
						  public SettableTooltipClient,
						  public ApplicationCommandManagerListener,
						  public ValueListener,
						  private KeyListener
{
protected:

	/** Creates a button.

		@param buttonName           the text to put in the button (the component's name is also
									initially set to this string, but these can be changed later
									using the setName() and setButtonText() methods)
	*/
	explicit Button (const String& buttonName);

public:
	/** Destructor. */
	virtual ~Button();

	/** Changes the button's text.

		@see getButtonText
	*/
	void setButtonText (const String& newText);

	/** Returns the text displayed in the button.

		@see setButtonText
	*/
	const String& getButtonText() const               { return text; }

	/** Returns true if the button is currently being held down by the mouse.

		@see isOver
	*/
	bool isDown() const noexcept;

	/** Returns true if the mouse is currently over the button.

		This will be also be true if the mouse is being held down.

		@see isDown
	*/
	bool isOver() const noexcept;

	/** A button has an on/off state associated with it, and this changes that.

		By default buttons are 'off' and for simple buttons that you click to perform
		an action you won't change this. Toggle buttons, however will want to
		change their state when turned on or off.

		@param shouldBeOn               whether to set the button's toggle state to be on or
										off. If it's a member of a button group, this will
										always try to turn it on, and to turn off any other
										buttons in the group
		@param sendChangeNotification   if true, a callback will be made to clicked(); if false
										the button will be repainted but no notification will
										be sent
		@see getToggleState, setRadioGroupId
	*/
	void setToggleState (bool shouldBeOn,
						 bool sendChangeNotification);

	/** Returns true if the button is 'on'.

		By default buttons are 'off' and for simple buttons that you click to perform
		an action you won't change this. Toggle buttons, however will want to
		change their state when turned on or off.

		@see setToggleState
	*/
	bool getToggleState() const noexcept                        { return isOn.getValue(); }

	/** Returns the Value object that represents the botton's toggle state.
		You can use this Value object to connect the button's state to external values or setters,
		either by taking a copy of the Value, or by using Value::referTo() to make it point to
		your own Value object.
		@see getToggleState, Value
	*/
	Value& getToggleStateValue()                                { return isOn; }

	/** This tells the button to automatically flip the toggle state when
		the button is clicked.

		If set to true, then before the clicked() callback occurs, the toggle-state
		of the button is flipped.
	*/
	void setClickingTogglesState (bool shouldToggle) noexcept;

	/** Returns true if this button is set to be an automatic toggle-button.

		This returns the last value that was passed to setClickingTogglesState().
	*/
	bool getClickingTogglesState() const noexcept;

	/** Enables the button to act as a member of a mutually-exclusive group
		of 'radio buttons'.

		If the group ID is set to a non-zero number, then this button will
		act as part of a group of buttons with the same ID, only one of
		which can be 'on' at the same time. Note that when it's part of
		a group, clicking a toggle-button that's 'on' won't turn it off.

		To find other buttons with the same ID, this button will search through
		its sibling components for ToggleButtons, so all the buttons for a
		particular group must be placed inside the same parent component.

		Set the group ID back to zero if you want it to act as a normal toggle
		button again.

		@see getRadioGroupId
	*/
	void setRadioGroupId (int newGroupId);

	/** Returns the ID of the group to which this button belongs.

		(See setRadioGroupId() for an explanation of this).
	*/
	int getRadioGroupId() const noexcept                        { return radioGroupId; }

	/**
		Used to receive callbacks when a button is clicked.

		@see Button::addListener, Button::removeListener
	*/
	class JUCE_API  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener()                                     {}

		/** Called when the button is clicked. */
		virtual void buttonClicked (Button* button) = 0;

		/** Called when the button's state changes. */
		virtual void buttonStateChanged (Button*)               {}
	};

	/** Registers a listener to receive events when this button's state changes.
		If the listener is already registered, this will not register it again.
		@see removeListener
	*/
	void addListener (Listener* newListener);

	/** Removes a previously-registered button listener
		@see addListener
	*/
	void removeListener (Listener* listener);

	/** Causes the button to act as if it's been clicked.

		This will asynchronously make the button draw itself going down and up, and
		will then call back the clicked() method as if mouse was clicked on it.

		@see clicked
	*/
	virtual void triggerClick();

	/** Sets a command ID for this button to automatically invoke when it's clicked.

		When the button is pressed, it will use the given manager to trigger the
		command ID.

		Obviously be careful that the ApplicationCommandManager doesn't get deleted
		before this button is. To disable the command triggering, call this method and
		pass 0 for the parameters.

		If generateTooltip is true, then the button's tooltip will be automatically
		generated based on the name of this command and its current shortcut key.

		@see addShortcut, getCommandID
	*/
	void setCommandToTrigger (ApplicationCommandManager* commandManagerToUse,
							  int commandID,
							  bool generateTooltip);

	/** Returns the command ID that was set by setCommandToTrigger().
	*/
	int getCommandID() const noexcept               { return commandID; }

	/** Assigns a shortcut key to trigger the button.

		The button registers itself with its top-level parent component for keypresses.

		Note that a different way of linking buttons to keypresses is by using the
		setCommandToTrigger() method to invoke a command.

		@see clearShortcuts
	*/
	void addShortcut (const KeyPress& key);

	/** Removes all key shortcuts that had been set for this button.

		@see addShortcut
	*/
	void clearShortcuts();

	/** Returns true if the given keypress is a shortcut for this button.

		@see addShortcut
	*/
	bool isRegisteredForShortcut (const KeyPress& key) const;

	/** Sets an auto-repeat speed for the button when it is held down.

		(Auto-repeat is disabled by default).

		@param initialDelayInMillisecs      how long to wait after the mouse is pressed before
											triggering the next click. If this is zero, auto-repeat
											is disabled
		@param repeatDelayInMillisecs       the frequently subsequent repeated clicks should be
											triggered
		@param minimumDelayInMillisecs      if this is greater than 0, the auto-repeat speed will
											get faster, the longer the button is held down, up to the
											minimum interval specified here
	*/
	void setRepeatSpeed (int initialDelayInMillisecs,
						 int repeatDelayInMillisecs,
						 int minimumDelayInMillisecs = -1) noexcept;

	/** Sets whether the button click should happen when the mouse is pressed or released.

		By default the button is only considered to have been clicked when the mouse is
		released, but setting this to true will make it call the clicked() method as soon
		as the button is pressed.

		This is useful if the button is being used to show a pop-up menu, as it allows
		the click to be used as a drag onto the menu.
	*/
	void setTriggeredOnMouseDown (bool isTriggeredOnMouseDown) noexcept;

	/** Returns the number of milliseconds since the last time the button
		went into the 'down' state.
	*/
	uint32 getMillisecondsSinceButtonDown() const noexcept;

	/** Sets the tooltip for this button.

		@see TooltipClient, TooltipWindow
	*/
	void setTooltip (const String& newTooltip);

	// (implementation of the TooltipClient method)
	String getTooltip();

	/** A combination of these flags are used by setConnectedEdges().
	*/
	enum ConnectedEdgeFlags
	{
		ConnectedOnLeft = 1,
		ConnectedOnRight = 2,
		ConnectedOnTop = 4,
		ConnectedOnBottom = 8
	};

	/** Hints about which edges of the button might be connected to adjoining buttons.

		The value passed in is a bitwise combination of any of the values in the
		ConnectedEdgeFlags enum.

		E.g. if you are placing two buttons adjacent to each other, you could use this to
		indicate which edges are touching, and the LookAndFeel might choose to drawn them
		without rounded corners on the edges that connect. It's only a hint, so the
		LookAndFeel can choose to ignore it if it's not relevent for this type of
		button.
	*/
	void setConnectedEdges (int connectedEdgeFlags);

	/** Returns the set of flags passed into setConnectedEdges(). */
	int getConnectedEdgeFlags() const noexcept                  { return connectedEdgeFlags; }

	/** Indicates whether the button adjoins another one on its left edge.
		@see setConnectedEdges
	*/
	bool isConnectedOnLeft() const noexcept                     { return (connectedEdgeFlags & ConnectedOnLeft) != 0; }

	/** Indicates whether the button adjoins another one on its right edge.
		@see setConnectedEdges
	*/
	bool isConnectedOnRight() const noexcept                    { return (connectedEdgeFlags & ConnectedOnRight) != 0; }

	/** Indicates whether the button adjoins another one on its top edge.
		@see setConnectedEdges
	*/
	bool isConnectedOnTop() const noexcept                      { return (connectedEdgeFlags & ConnectedOnTop) != 0; }

	/** Indicates whether the button adjoins another one on its bottom edge.
		@see setConnectedEdges
	*/
	bool isConnectedOnBottom() const noexcept                   { return (connectedEdgeFlags & ConnectedOnBottom) != 0; }

	/** Used by setState(). */
	enum ButtonState
	{
		buttonNormal,
		buttonOver,
		buttonDown
	};

	/** Can be used to force the button into a particular state.

		This only changes the button's appearance, it won't trigger a click, or stop any mouse-clicks
		from happening.

		The state that you set here will only last until it is automatically changed when the mouse
		enters or exits the button, or the mouse-button is pressed or released.
	*/
	void setState (const ButtonState newState);

	struct Ids
	{
		static const Identifier text, radioGroup,
								connectedLeft, connectedRight, connectedTop, connectedBottom;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);
	static int getConnectedFlags (const ValueTree& state);

	// These are deprecated - please use addListener() and removeListener() instead!
	JUCE_DEPRECATED (void addButtonListener (Listener*));
	JUCE_DEPRECATED (void removeButtonListener (Listener*));

protected:

	/** This method is called when the button has been clicked.

		Subclasses can override this to perform whatever they actions they need
		to do.

		Alternatively, a ButtonListener can be added to the button, and these listeners
		will be called when the click occurs.

		@see triggerClick
	*/
	virtual void clicked();

	/** This method is called when the button has been clicked.

		By default it just calls clicked(), but you might want to override it to handle
		things like clicking when a modifier key is pressed, etc.

		@see ModifierKeys
	*/
	virtual void clicked (const ModifierKeys& modifiers);

	/** Subclasses should override this to actually paint the button's contents.

		It's better to use this than the paint method, because it gives you information
		about the over/down state of the button.

		@param g                    the graphics context to use
		@param isMouseOverButton    true if the button is either in the 'over' or
									'down' state
		@param isButtonDown         true if the button should be drawn in the 'down' position
	*/
	virtual void paintButton (Graphics& g,
							  bool isMouseOverButton,
							  bool isButtonDown) = 0;

	/** Called when the button's up/down/over state changes.

		Subclasses can override this if they need to do something special when the button
		goes up or down.

		@see isDown, isOver
	*/
	virtual void buttonStateChanged();

	/** @internal */
	virtual void internalClickCallback (const ModifierKeys& modifiers);
	/** @internal */
	void handleCommandMessage (int commandId);
	/** @internal */
	void mouseEnter (const MouseEvent& e);
	/** @internal */
	void mouseExit (const MouseEvent& e);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseUp (const MouseEvent& e);
	/** @internal */
	bool keyPressed (const KeyPress& key);
	/** @internal */
	bool keyPressed (const KeyPress& key, Component* originatingComponent);
	/** @internal */
	bool keyStateChanged (bool isKeyDown, Component* originatingComponent);
	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void parentHierarchyChanged();
	/** @internal */
	void visibilityChanged();
	/** @internal */
	void focusGained (FocusChangeType cause);
	/** @internal */
	void focusLost (FocusChangeType cause);
	/** @internal */
	void enablementChanged();
	/** @internal */
	void applicationCommandInvoked (const ApplicationCommandTarget::InvocationInfo&);
	/** @internal */
	void applicationCommandListChanged();
	/** @internal */
	void valueChanged (Value& value);

private:

	Array <KeyPress> shortcuts;
	WeakReference<Component> keySource;
	String text;
	ListenerList <Listener> buttonListeners;

	class RepeatTimer;
	friend class RepeatTimer;
	friend class ScopedPointer <RepeatTimer>;
	ScopedPointer <RepeatTimer> repeatTimer;
	uint32 buttonPressTime, lastRepeatTime;
	ApplicationCommandManager* commandManagerToUse;
	int autoRepeatDelay, autoRepeatSpeed, autoRepeatMinimumDelay;
	int radioGroupId, commandID, connectedEdgeFlags;
	ButtonState buttonState;

	Value isOn;
	bool lastToggleState : 1;
	bool clickTogglesState : 1;
	bool needsToRelease : 1;
	bool needsRepainting : 1;
	bool isKeyDown : 1;
	bool triggerOnMouseDown : 1;
	bool generateTooltip : 1;

	void repeatTimerCallback();
	RepeatTimer& getRepeatTimer();

	ButtonState updateState();
	ButtonState updateState (bool isOver, bool isDown);
	bool isShortcutPressed() const;
	void turnOffOtherButtonsInGroup (bool sendChangeNotification);

	void flashButtonState();
	void sendClickMessage (const ModifierKeys& modifiers);
	void sendStateMessage();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Button);
};

#ifndef DOXYGEN
 /** This typedef is just for compatibility with old code and VC6 - newer code should use Button::Listener instead. */
 typedef Button::Listener ButtonListener;
#endif

#endif   // __JUCE_BUTTON_JUCEHEADER__

/*** End of inlined file: juce_Button.h ***/

/**
	A button with an arrow in it.

	@see Button
*/
class JUCE_API  ArrowButton  : public Button
{
public:

	/** Creates an ArrowButton.

		@param buttonName       the name to give the button
		@param arrowDirection   the direction the arrow should point in, where 0.0 is
								pointing right, 0.25 is down, 0.5 is left, 0.75 is up
		@param arrowColour      the colour to use for the arrow
	*/
	ArrowButton (const String& buttonName,
				 float arrowDirection,
				 const Colour& arrowColour);

	/** Destructor. */
	~ArrowButton();

protected:

	/** @internal */
	void paintButton (Graphics& g,
					  bool isMouseOverButton,
					  bool isButtonDown);

	/** @internal */
	void buttonStateChanged();

private:

	Colour colour;
	DropShadowEffect shadow;
	Path path;
	int offset;

	void updateShadowAndOffset();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrowButton);
};

#endif   // __JUCE_ARROWBUTTON_JUCEHEADER__

/*** End of inlined file: juce_ArrowButton.h ***/


#endif
#ifndef __JUCE_BUTTON_JUCEHEADER__

#endif
#ifndef __JUCE_DRAWABLEBUTTON_JUCEHEADER__

/*** Start of inlined file: juce_DrawableButton.h ***/
#ifndef __JUCE_DRAWABLEBUTTON_JUCEHEADER__
#define __JUCE_DRAWABLEBUTTON_JUCEHEADER__


/*** Start of inlined file: juce_Drawable.h ***/
#ifndef __JUCE_DRAWABLE_JUCEHEADER__
#define __JUCE_DRAWABLE_JUCEHEADER__


/*** Start of inlined file: juce_RelativeCoordinate.h ***/
#ifndef __JUCE_RELATIVECOORDINATE_JUCEHEADER__
#define __JUCE_RELATIVECOORDINATE_JUCEHEADER__

/**
	Expresses a coordinate as a dynamically evaluated expression.

	@see RelativePoint, RelativeRectangle
*/
class JUCE_API  RelativeCoordinate
{
public:

	/** Creates a zero coordinate. */
	RelativeCoordinate();
	RelativeCoordinate (const Expression& expression);
	RelativeCoordinate (const RelativeCoordinate& other);
	RelativeCoordinate& operator= (const RelativeCoordinate& other);

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	RelativeCoordinate (RelativeCoordinate&& other) noexcept;
	RelativeCoordinate& operator= (RelativeCoordinate&& other) noexcept;
   #endif

	/** Creates an absolute position from the parent origin on either the X or Y axis.

		@param absoluteDistanceFromOrigin   the distance from the origin
	*/
	RelativeCoordinate (double absoluteDistanceFromOrigin);

	/** Recreates a coordinate from a string description.
		The string will be parsed by ExpressionParser::parse().
		@param stringVersion    the expression to use
		@see toString
	*/
	RelativeCoordinate (const String& stringVersion);

	/** Destructor. */
	~RelativeCoordinate();

	bool operator== (const RelativeCoordinate& other) const noexcept;
	bool operator!= (const RelativeCoordinate& other) const noexcept;

	/** Calculates the absolute position of this coordinate.

		You'll need to provide a suitable Expression::Scope for looking up any coordinates that may
		be needed to calculate the result.
	*/
	double resolve (const Expression::Scope* evaluationScope) const;

	/** Returns true if this coordinate uses the specified coord name at any level in its evaluation.
		This will recursively check any coordinates upon which this one depends.
	*/
	bool references (const String& coordName, const Expression::Scope* evaluationScope) const;

	/** Returns true if there's a recursive loop when trying to resolve this coordinate's position. */
	bool isRecursive (const Expression::Scope* evaluationScope) const;

	/** Returns true if this coordinate depends on any other coordinates for its position. */
	bool isDynamic() const;

	/** Changes the value of this coord to make it resolve to the specified position.

		Calling this will leave the anchor points unchanged, but will set this coordinate's absolute
		or relative position to whatever value is necessary to make its resultant position
		match the position that is provided.
	*/
	void moveToAbsolute (double absoluteTargetPosition, const Expression::Scope* evaluationScope);

	/** Returns the expression that defines this coordinate. */
	const Expression& getExpression() const         { return term; }

	/** Returns a string which represents this coordinate.
		For details of the string syntax, see the constructor notes.
	*/
	String toString() const;

	/** A set of static strings that are commonly used by the RelativeCoordinate class.

		As well as avoiding using string literals in your code, using these preset values
		has the advantage that all instances of the same string will share the same, reference-counted
		String object, so if you have thousands of points which all refer to the same
		anchor points, this can save a significant amount of memory allocation.
	*/
	struct Strings
	{
		static const String parent;         /**< "parent" */
		static const String left;           /**< "left" */
		static const String right;          /**< "right" */
		static const String top;            /**< "top" */
		static const String bottom;         /**< "bottom" */
		static const String x;              /**< "x" */
		static const String y;              /**< "y" */
		static const String width;          /**< "width" */
		static const String height;         /**< "height" */
	};

	struct StandardStrings
	{
		enum Type
		{
			left, right, top, bottom,
			x, y, width, height,
			parent,
			unknown
		};

		static Type getTypeOf (const String& s) noexcept;
	};

private:

	Expression term;
};

#endif   // __JUCE_RELATIVECOORDINATE_JUCEHEADER__

/*** End of inlined file: juce_RelativeCoordinate.h ***/


/*** Start of inlined file: juce_RelativeCoordinatePositioner.h ***/
#ifndef __JUCE_RELATIVECOORDINATEPOSITIONER_JUCEHEADER__
#define __JUCE_RELATIVECOORDINATEPOSITIONER_JUCEHEADER__


/*** Start of inlined file: juce_RelativePoint.h ***/
#ifndef __JUCE_RELATIVEPOINT_JUCEHEADER__
#define __JUCE_RELATIVEPOINT_JUCEHEADER__

/**
	An X-Y position stored as a pair of RelativeCoordinate values.

	@see RelativeCoordinate, RelativeRectangle
*/
class JUCE_API  RelativePoint
{
public:
	/** Creates a point at the origin. */
	RelativePoint();

	/** Creates an absolute point, relative to the origin. */
	RelativePoint (const Point<float>& absolutePoint);

	/** Creates an absolute point, relative to the origin. */
	RelativePoint (float absoluteX, float absoluteY);

	/** Creates an absolute point from two coordinates. */
	RelativePoint (const RelativeCoordinate& x, const RelativeCoordinate& y);

	/** Creates a point from a stringified representation.
		The string must contain a pair of coordinates, separated by space or a comma. The syntax for the coordinate
		strings is explained in the RelativeCoordinate class.
		@see toString
	*/
	RelativePoint (const String& stringVersion);

	bool operator== (const RelativePoint& other) const noexcept;
	bool operator!= (const RelativePoint& other) const noexcept;

	/** Calculates the absolute position of this point.

		You'll need to provide a suitable Expression::Scope for looking up any coordinates that may
		be needed to calculate the result.
	*/
	const Point<float> resolve (const Expression::Scope* evaluationContext) const;

	/** Changes the values of this point's coordinates to make it resolve to the specified position.

		Calling this will leave any anchor points unchanged, but will set any absolute
		or relative positions to whatever values are necessary to make the resultant position
		match the position that is provided.
	*/
	void moveToAbsolute (const Point<float>& newPos, const Expression::Scope* evaluationContext);

	/** Returns a string which represents this point.
		This returns a comma-separated pair of coordinates. For details of the string syntax used by the
		coordinates, see the RelativeCoordinate constructor notes.
		The string that is returned can be passed to the RelativePoint constructor to recreate the point.
	*/
	String toString() const;

	/** Returns true if this point depends on any other coordinates for its position. */
	bool isDynamic() const;

	// The actual X and Y coords...
	RelativeCoordinate x, y;
};

#endif   // __JUCE_RELATIVEPOINT_JUCEHEADER__

/*** End of inlined file: juce_RelativePoint.h ***/


/*** Start of inlined file: juce_MarkerList.h ***/
#ifndef __JUCE_MARKERLIST_JUCEHEADER__
#define __JUCE_MARKERLIST_JUCEHEADER__

class Component;

/**
	Holds a set of named marker points along a one-dimensional axis.

	This class is used to store sets of X and Y marker points in components.
	@see Component::getMarkers().
*/
class JUCE_API  MarkerList
{
public:

	/** Creates an empty marker list. */
	MarkerList();
	/** Creates a copy of another marker list. */
	MarkerList (const MarkerList& other);
	/** Copies another marker list to this one. */
	MarkerList& operator= (const MarkerList& other);
	/** Destructor. */
	~MarkerList();

	/** Represents a marker in a MarkerList. */
	class JUCE_API  Marker
	{
	public:
		/** Creates a copy of another Marker. */
		Marker (const Marker& other);
		/** Creates a Marker with a given name and position. */
		Marker (const String& name, const RelativeCoordinate& position);

		/** The marker's name. */
		String name;

		/** The marker's position.

			The expression used to define the coordinate may use the names of other
			markers, so that markers can be linked in arbitrary ways, but be careful
			not to create recursive loops of markers whose positions are based on each
			other! It can also refer to "parent.right" and "parent.bottom" so that you
			can set markers which are relative to the size of the component that contains
			them.

			To resolve the coordinate, you can use the MarkerList::getMarkerPosition() method.
		*/
		RelativeCoordinate position;

		/** Returns true if both the names and positions of these two markers match. */
		bool operator== (const Marker&) const noexcept;
		/** Returns true if either the name or position of these two markers differ. */
		bool operator!= (const Marker&) const noexcept;
	};

	/** Returns the number of markers in the list. */
	int getNumMarkers() const noexcept;

	/** Returns one of the markers in the list, by its index. */
	const Marker* getMarker (int index) const noexcept;

	/** Returns a named marker, or 0 if no such name is found.
		Note that name comparisons are case-sensitive.
	*/
	const Marker* getMarker (const String& name) const noexcept;

	/** Evaluates the given marker and returns its absolute position.
		The parent component must be supplied in case the marker's expression refers to
		the size of its parent component.
	*/
	double getMarkerPosition (const Marker& marker, Component* parentComponent) const;

	/** Sets the position of a marker.

		If the name already exists, then the existing marker is moved; if it doesn't exist, then a
		new marker is added.
	*/
	void setMarker (const String& name, const RelativeCoordinate& position);

	/** Deletes the marker at the given list index. */
	void removeMarker (int index);

	/** Deletes the marker with the given name. */
	void removeMarker (const String& name);

	/** Returns true if all the markers in these two lists match exactly. */
	bool operator== (const MarkerList& other) const noexcept;
	/** Returns true if not all the markers in these two lists match exactly. */
	bool operator!= (const MarkerList& other) const noexcept;

	/**
		A class for receiving events when changes are made to a MarkerList.

		You can register a MarkerList::Listener with a MarkerList using the MarkerList::addListener()
		method, and it will be called when markers are moved, added, or deleted.

		@see MarkerList::addListener, MarkerList::removeListener
	*/
	class JUCE_API  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}

		/** Called when something in the given marker list changes. */
		virtual void markersChanged (MarkerList* markerList) = 0;

		/** Called when the given marker list is being deleted. */
		virtual void markerListBeingDeleted (MarkerList* markerList);
	};

	/** Registers a listener that will be called when the markers are changed. */
	void addListener (Listener* listener);

	/** Deregisters a previously-registered listener. */
	void removeListener (Listener* listener);

	/** Synchronously calls markersChanged() on all the registered listeners. */
	void markersHaveChanged();

	/** Forms a wrapper around a ValueTree that can be used for storing a MarkerList. */
	class ValueTreeWrapper
	{
	public:
		ValueTreeWrapper (const ValueTree& state);

		ValueTree& getState() noexcept      { return state; }
		int getNumMarkers() const;
		ValueTree getMarkerState (int index) const;
		ValueTree getMarkerState (const String& name) const;
		bool containsMarker (const ValueTree& state) const;
		MarkerList::Marker getMarker (const ValueTree& state) const;
		void setMarker (const MarkerList::Marker& marker, UndoManager* undoManager);
		void removeMarker (const ValueTree& state, UndoManager* undoManager);

		void applyTo (MarkerList& markerList);
		void readFrom (const MarkerList& markerList, UndoManager* undoManager);

		static const Identifier markerTag, nameProperty, posProperty;

	private:
		ValueTree state;
	};

private:

	OwnedArray<Marker> markers;
	ListenerList<Listener> listeners;

	Marker* getMarkerByName (const String& name) const noexcept;

	JUCE_LEAK_DETECTOR (MarkerList);
};

#endif   // __JUCE_MARKERLIST_JUCEHEADER__

/*** End of inlined file: juce_MarkerList.h ***/

/**
	Base class for Component::Positioners that are based upon relative coordinates.
*/
class JUCE_API  RelativeCoordinatePositionerBase  : public Component::Positioner,
													public ComponentListener,
													public MarkerList::Listener
{
public:
	RelativeCoordinatePositionerBase (Component& component_);
	~RelativeCoordinatePositionerBase();

	void componentMovedOrResized (Component&, bool, bool);
	void componentParentHierarchyChanged (Component&);
	void componentChildrenChanged (Component& component);
	void componentBeingDeleted (Component& component);
	void markersChanged (MarkerList*);
	void markerListBeingDeleted (MarkerList* markerList);

	void apply();

	bool addCoordinate (const RelativeCoordinate& coord);
	bool addPoint (const RelativePoint& point);

	/** Used for resolving a RelativeCoordinate expression in the context of a component. */
	class ComponentScope  : public Expression::Scope
	{
	public:
		ComponentScope (Component& component_);

		Expression getSymbolValue (const String& symbol) const;
		void visitRelativeScope (const String& scopeName, Visitor& visitor) const;
		String getScopeUID() const;

	protected:
		Component& component;

		Component* findSiblingComponent (const String& componentID) const;

	private:
		JUCE_DECLARE_NON_COPYABLE (ComponentScope);
	};

protected:
	virtual bool registerCoordinates() = 0;
	virtual void applyToComponentBounds() = 0;

private:
	class DependencyFinderScope;
	friend class DependencyFinderScope;
	Array <Component*> sourceComponents;
	Array <MarkerList*> sourceMarkerLists;
	bool registeredOk;

	void registerComponentListener (Component& comp);
	void registerMarkerListListener (MarkerList* const list);
	void unregisterListeners();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RelativeCoordinatePositionerBase);
};

#endif   // __JUCE_RELATIVECOORDINATEPOSITIONER_JUCEHEADER__

/*** End of inlined file: juce_RelativeCoordinatePositioner.h ***/

class DrawableComposite;

/**
	The base class for objects which can draw themselves, e.g. polygons, images, etc.

	@see DrawableComposite, DrawableImage, DrawablePath, DrawableText
*/
class JUCE_API  Drawable  : public Component
{
protected:

	/** The base class can't be instantiated directly.

		@see DrawableComposite, DrawableImage, DrawablePath, DrawableText
	*/
	Drawable();

public:
	/** Destructor. */
	virtual ~Drawable();

	/** Creates a deep copy of this Drawable object.

		Use this to create a new copy of this and any sub-objects in the tree.
	*/
	virtual Drawable* createCopy() const = 0;

	/** Renders this Drawable object.

		Note that the preferred way to render a drawable in future is by using it
		as a component and adding it to a parent, so you might want to consider that
		before using this method.

		@see drawWithin
	*/
	void draw (Graphics& g, float opacity,
			   const AffineTransform& transform = AffineTransform::identity) const;

	/** Renders the Drawable at a given offset within the Graphics context.

		The co-ordinates passed-in are used to translate the object relative to its own
		origin before drawing it - this is basically a quick way of saying:

		@code
		draw (g, AffineTransform::translation (x, y)).
		@endcode

		Note that the preferred way to render a drawable in future is by using it
		as a component and adding it to a parent, so you might want to consider that
		before using this method.
	*/
	void drawAt (Graphics& g, float x, float y, float opacity) const;

	/** Renders the Drawable within a rectangle, scaling it to fit neatly inside without
		changing its aspect-ratio.

		The object can placed arbitrarily within the rectangle based on a Justification type,
		and can either be made as big as possible, or just reduced to fit.

		Note that the preferred way to render a drawable in future is by using it
		as a component and adding it to a parent, so you might want to consider that
		before using this method.

		@param g                        the graphics context to render onto
		@param destArea                 the target rectangle to fit the drawable into
		@param placement                defines the alignment and rescaling to use to fit
										this object within the target rectangle.
		@param opacity                  the opacity to use, in the range 0 to 1.0
	*/
	void drawWithin (Graphics& g,
					 const Rectangle<float>& destArea,
					 const RectanglePlacement& placement,
					 float opacity) const;

	/** Resets any transformations on this drawable, and positions its origin within
		its parent component.
	*/
	void setOriginWithOriginalSize (const Point<float>& originWithinParent);

	/** Sets a transform for this drawable that will position it within the specified
		area of its parent component.
	*/
	void setTransformToFit (const Rectangle<float>& areaInParent, const RectanglePlacement& placement);

	/** Returns the DrawableComposite that contains this object, if there is one. */
	DrawableComposite* getParent() const;

	/** Tries to turn some kind of image file into a drawable.

		The data could be an image that the ImageFileFormat class understands, or it
		could be SVG.
	*/
	static Drawable* createFromImageData (const void* data, size_t numBytes);

	/** Tries to turn a stream containing some kind of image data into a drawable.

		The data could be an image that the ImageFileFormat class understands, or it
		could be SVG.
	*/
	static Drawable* createFromImageDataStream (InputStream& dataSource);

	/** Tries to turn a file containing some kind of image data into a drawable.

		The data could be an image that the ImageFileFormat class understands, or it
		could be SVG.
	*/
	static Drawable* createFromImageFile (const File& file);

	/** Attempts to parse an SVG (Scalable Vector Graphics) document, and to turn this
		into a Drawable tree.

		The object returned must be deleted by the caller. If something goes wrong
		while parsing, it may return 0.

		SVG is a pretty large and complex spec, and this doesn't aim to be a full
		implementation, but it can return the basic vector objects.
	*/
	static Drawable* createFromSVG (const XmlElement& svgDocument);

	/** Tries to create a Drawable from a previously-saved ValueTree.
		The ValueTree must have been created by the createValueTree() method.
		If there are any images used within the drawable, you'll need to provide a valid
		ImageProvider object that can be used to retrieve these images from whatever type
		of identifier is used to represent them.
		Internally, this uses a ComponentBuilder, and registerDrawableTypeHandlers().
	*/
	static Drawable* createFromValueTree (const ValueTree& tree, ComponentBuilder::ImageProvider* imageProvider);

	/** Creates a ValueTree to represent this Drawable.
		The ValueTree that is returned can be turned back into a Drawable with createFromValueTree().
		If there are any images used in this drawable, you'll need to provide a valid ImageProvider
		object that can be used to create storable representations of them.
	*/
	virtual ValueTree createValueTree (ComponentBuilder::ImageProvider* imageProvider) const = 0;

	/** Returns the area that this drawble covers.
		The result is expressed in this drawable's own coordinate space, and does not take
		into account any transforms that may be applied to the component.
	*/
	virtual Rectangle<float> getDrawableBounds() const = 0;

	/** Internal class used to manage ValueTrees that represent Drawables. */
	class ValueTreeWrapperBase
	{
	public:
		ValueTreeWrapperBase (const ValueTree& state);

		ValueTree& getState() noexcept          { return state; }

		String getID() const;
		void setID (const String& newID);

		ValueTree state;
	};

	/** Registers a set of ComponentBuilder::TypeHandler objects that can be used to
		load all the different Drawable types from a saved state.
		@see ComponentBuilder::registerTypeHandler()
	*/
	static void registerDrawableTypeHandlers (ComponentBuilder& componentBuilder);

protected:

	friend class DrawableComposite;
	friend class DrawableShape;

	/** @internal */
	void transformContextToCorrectOrigin (Graphics&);
	/** @internal */
	void parentHierarchyChanged();
	/** @internal */
	void setBoundsToEnclose (const Rectangle<float>&);

	Point<int> originRelativeToComponent;

  #ifndef DOXYGEN
	/** Internal utility class used by Drawables. */
	template <class DrawableType>
	class Positioner  : public RelativeCoordinatePositionerBase
	{
	public:
		Positioner (DrawableType& component_)
			: RelativeCoordinatePositionerBase (component_),
			  owner (component_)
		{}

		bool registerCoordinates()      { return owner.registerCoordinates (*this); }
		void applyToComponentBounds()
		{
			ComponentScope scope (getComponent());
			owner.recalculateCoordinates (&scope);
		}

		void applyNewBounds (const Rectangle<int>&)
		{
			jassertfalse; // drawables can't be resized directly!
		}

	private:
		DrawableType& owner;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Positioner);
	};

	Drawable (const Drawable&);
  #endif

private:
	void nonConstDraw (Graphics& g, float opacity, const AffineTransform& transform);

	Drawable& operator= (const Drawable&);
	JUCE_LEAK_DETECTOR (Drawable);
};

#endif   // __JUCE_DRAWABLE_JUCEHEADER__

/*** End of inlined file: juce_Drawable.h ***/

/**
	A button that displays a Drawable.

	Up to three Drawable objects can be given to this button, to represent the
	'normal', 'over' and 'down' states.

	@see Button
*/
class JUCE_API  DrawableButton  : public Button
{
public:

	enum ButtonStyle
	{
		ImageFitted,                /**< The button will just display the images, but will resize and centre them to fit inside it. */
		ImageRaw,                   /**< The button will just display the images in their normal size and position.
										 This leaves it up to the caller to make sure the images are the correct size and position for the button. */
		ImageAboveTextLabel,        /**< Draws the button as a text label across the bottom with the image resized and scaled to fit above it. */
		ImageOnButtonBackground     /**< Draws the button as a standard rounded-rectangle button with the image on top. */
	};

	/** Creates a DrawableButton.

		After creating one of these, use setImages() to specify the drawables to use.

		@param buttonName           the name to give the component
		@param buttonStyle          the layout to use

		@see ButtonStyle, setButtonStyle, setImages
	*/
	DrawableButton (const String& buttonName,
					ButtonStyle buttonStyle);

	/** Destructor. */
	~DrawableButton();

	/** Sets up the images to draw for the various button states.

		The button will keep its own internal copies of these drawables.

		@param normalImage      the thing to draw for the button's 'normal' state. An internal copy
								will be made of the object passed-in if it is non-zero.
		@param overImage        the thing to draw for the button's 'over' state - if this is
								zero, the button's normal image will be used when the mouse is
								over it. An internal copy will be made of the object passed-in
								if it is non-zero.
		@param downImage        the thing to draw for the button's 'down' state - if this is
								zero, the 'over' image will be used instead (or the normal image
								as a last resort). An internal copy will be made of the object
								passed-in if it is non-zero.
		@param disabledImage    an image to draw when the button is disabled. If this is zero,
								the normal image will be drawn with a reduced opacity instead.
								An internal copy will be made of the object passed-in if it is
								non-zero.
		@param normalImageOn    same as the normalImage, but this is used when the button's toggle
								state is 'on'. If this is 0, the normal image is used instead
		@param overImageOn      same as the overImage, but this is used when the button's toggle
								state is 'on'. If this is 0, the normalImageOn is drawn instead
		@param downImageOn      same as the downImage, but this is used when the button's toggle
								state is 'on'. If this is 0, the overImageOn is drawn instead
		@param disabledImageOn  same as the disabledImage, but this is used when the button's toggle
								state is 'on'. If this is 0, the normal image will be drawn instead
								with a reduced opacity
	*/
	void setImages (const Drawable* normalImage,
					const Drawable* overImage = nullptr,
					const Drawable* downImage = nullptr,
					const Drawable* disabledImage = nullptr,
					const Drawable* normalImageOn = nullptr,
					const Drawable* overImageOn = nullptr,
					const Drawable* downImageOn = nullptr,
					const Drawable* disabledImageOn = nullptr);

	/** Changes the button's style.

		@see ButtonStyle
	*/
	void setButtonStyle (ButtonStyle newStyle);

	/** Changes the button's background colours.

		The toggledOffColour is the colour to use when the button's toggle state
		is off, and toggledOnColour when it's on.

		For an ImageOnly or ImageAboveTextLabel style, the background colour is
		used to fill the background of the component.

		For an ImageOnButtonBackground style, the colour is used to draw the
		button's lozenge shape and exactly how the colour's used will depend
		on the LookAndFeel.
	*/
	void setBackgroundColours (const Colour& toggledOffColour,
							   const Colour& toggledOnColour);

	/** Returns the current background colour being used.

		@see setBackgroundColour
	*/
	const Colour& getBackgroundColour() const noexcept;

	/** Gives the button an optional amount of space around the edge of the drawable.

		This will only apply to ImageFitted or ImageRaw styles, it won't affect the
		ones on a button background. If the button is too small for the given gap, a
		smaller gap will be used.

		By default there's a gap of about 3 pixels.
	*/
	void setEdgeIndent (int numPixelsIndent);

	/** Returns the image that the button is currently displaying. */
	Drawable* getCurrentImage() const noexcept;
	Drawable* getNormalImage() const noexcept;
	Drawable* getOverImage() const noexcept;
	Drawable* getDownImage() const noexcept;

	/** A set of colour IDs to use to change the colour of various aspects of the link.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		textColourId             = 0x1004010, /**< The colour to use for the URL text. */
	};

protected:

	/** @internal */
	void paintButton (Graphics& g,
					  bool isMouseOverButton,
					  bool isButtonDown);
	/** @internal */
	void buttonStateChanged();
	/** @internal */
	void resized();
	/** @internal */
	void enablementChanged();

private:

	ButtonStyle style;
	ScopedPointer <Drawable> normalImage, overImage, downImage, disabledImage;
	ScopedPointer <Drawable> normalImageOn, overImageOn, downImageOn, disabledImageOn;
	Drawable* currentImage;
	Colour backgroundOff, backgroundOn;
	int edgeIndent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrawableButton);
};

#endif   // __JUCE_DRAWABLEBUTTON_JUCEHEADER__

/*** End of inlined file: juce_DrawableButton.h ***/


#endif
#ifndef __JUCE_HYPERLINKBUTTON_JUCEHEADER__

/*** Start of inlined file: juce_HyperlinkButton.h ***/
#ifndef __JUCE_HYPERLINKBUTTON_JUCEHEADER__
#define __JUCE_HYPERLINKBUTTON_JUCEHEADER__

/**
	A button showing an underlined weblink, that will launch the link
	when it's clicked.

	@see Button
*/
class JUCE_API  HyperlinkButton  : public Button
{
public:

	/** Creates a HyperlinkButton.

		@param linkText     the text that will be displayed in the button - this is
							also set as the Component's name, but the text can be
							changed later with the Button::getButtonText() method
		@param linkURL      the URL to launch when the user clicks the button
	*/
	HyperlinkButton (const String& linkText,
					 const URL& linkURL);

	/** Creates a HyperlinkButton. */
	HyperlinkButton();

	/** Destructor. */
	~HyperlinkButton();

	/** Changes the font to use for the text.

		If resizeToMatchComponentHeight is true, the font's height will be adjusted
		to match the size of the component.
	*/
	void setFont (const Font& newFont,
				  bool resizeToMatchComponentHeight,
				  const Justification& justificationType = Justification::horizontallyCentred);

	/** A set of colour IDs to use to change the colour of various aspects of the link.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		textColourId             = 0x1001f00, /**< The colour to use for the URL text. */
	};

	/** Changes the URL that the button will trigger. */
	void setURL (const URL& newURL) noexcept;

	/** Returns the URL that the button will trigger. */
	const URL& getURL() const noexcept                          { return url; }

	/** Resizes the button horizontally to fit snugly around the text.

		This won't affect the button's height.
	*/
	void changeWidthToFitText();

	struct Ids
	{
		static const Identifier tagType, text, url;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);

protected:

	/** @internal */
	void clicked();
	/** @internal */
	void colourChanged();
	/** @internal */
	void paintButton (Graphics& g,
					  bool isMouseOverButton,
					  bool isButtonDown);

private:

	URL url;
	Font font;
	bool resizeFont;
	Justification justification;

	Font getFontToUse() const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HyperlinkButton);
};

#endif   // __JUCE_HYPERLINKBUTTON_JUCEHEADER__

/*** End of inlined file: juce_HyperlinkButton.h ***/


#endif
#ifndef __JUCE_IMAGEBUTTON_JUCEHEADER__

/*** Start of inlined file: juce_ImageButton.h ***/
#ifndef __JUCE_IMAGEBUTTON_JUCEHEADER__
#define __JUCE_IMAGEBUTTON_JUCEHEADER__

/**
	As the title suggests, this is a button containing an image.

	The colour and transparency of the image can be set to vary when the
	button state changes.

	@see Button, ShapeButton, TextButton
*/
class JUCE_API  ImageButton  : public Button
{
public:

	/** Creates an ImageButton.

		Use setImage() to specify the image to use. The colours and opacities that
		are specified here can be changed later using setDrawingOptions().

		@param name                 the name to give the component
	*/
	explicit ImageButton (const String& name = String::empty);

	/** Destructor. */
	~ImageButton();

	/** Sets up the images to draw in various states.

		@param resizeButtonNowToFitThisImage        if true, the button will be immediately
													resized to the same dimensions as the normal image
		@param rescaleImagesWhenButtonSizeChanges   if true, the image will be rescaled to fit the
													button when the button's size changes
		@param preserveImageProportions             if true then any rescaling of the image to fit
													the button will keep the image's x and y proportions
													correct - i.e. it won't distort its shape, although
													this might create gaps around the edges
		@param normalImage                          the image to use when the button is in its normal state.
													button no longer needs it.
		@param imageOpacityWhenNormal               the opacity to use when drawing the normal image.
		@param overlayColourWhenNormal              an overlay colour to use to fill the alpha channel of the
													normal image - if this colour is transparent, no overlay
													will be drawn. The overlay will be drawn over the top of the
													image, so you can basically add a solid or semi-transparent
													colour to the image to brighten or darken it
		@param overImage                            the image to use when the mouse is over the button. If
													you want to use the same image as was set in the normalImage
													parameter, this value can be a null image.
		@param imageOpacityWhenOver                 the opacity to use when drawing the image when the mouse
													is over the button
		@param overlayColourWhenOver                an overlay colour to use to fill the alpha channel of the
													image when the mouse is over - if this colour is transparent,
													no overlay will be drawn
		@param downImage                            an image to use when the button is pressed down. If set
													to a null image, the 'over' image will be drawn instead (or the
													normal image if there isn't an 'over' image either).
		@param imageOpacityWhenDown                 the opacity to use when drawing the image when the button
													is pressed
		@param overlayColourWhenDown                an overlay colour to use to fill the alpha channel of the
													image when the button is pressed down - if this colour is
													transparent, no overlay will be drawn
		@param hitTestAlphaThreshold                if set to zero, the mouse is considered to be over the button
													whenever it's inside the button's bounding rectangle. If
													set to values higher than 0, the mouse will only be
													considered to be over the image when the value of the
													image's alpha channel at that position is greater than
													this level.
	*/
	void setImages (bool resizeButtonNowToFitThisImage,
					bool rescaleImagesWhenButtonSizeChanges,
					bool preserveImageProportions,
					const Image& normalImage,
					float imageOpacityWhenNormal,
					const Colour& overlayColourWhenNormal,
					const Image& overImage,
					float imageOpacityWhenOver,
					const Colour& overlayColourWhenOver,
					const Image& downImage,
					float imageOpacityWhenDown,
					const Colour& overlayColourWhenDown,
					float hitTestAlphaThreshold = 0.0f);

	/** Returns the currently set 'normal' image. */
	Image getNormalImage() const;

	/** Returns the image that's drawn when the mouse is over the button.

		If a valid 'over' image has been set, this will return it; otherwise it'll
		just return the normal image.
	*/
	Image getOverImage() const;

	/** Returns the image that's drawn when the button is held down.

		If a valid 'down' image has been set, this will return it; otherwise it'll
		return the 'over' image or normal image, depending on what's available.
	*/
	Image getDownImage() const;

	struct Ids
	{
		static const Identifier tagType, upImage, overImage, downImage,
								upOverlay, overOverlay, downOverlay,
								upOpacity, overOpacity, downOpacity;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);

protected:

	/** @internal */
	bool hitTest (int x, int y);
	/** @internal */
	void paintButton (Graphics& g,
					  bool isMouseOverButton,
					  bool isButtonDown);

private:

	bool scaleImageToFit, preserveProportions;
	uint8 alphaThreshold;
	Rectangle<int> imageBounds;
	Image normalImage, overImage, downImage;
	float normalOpacity, overOpacity, downOpacity;
	Colour normalOverlay, overOverlay, downOverlay;

	Image getCurrentImage() const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageButton);
};

#endif   // __JUCE_IMAGEBUTTON_JUCEHEADER__

/*** End of inlined file: juce_ImageButton.h ***/


#endif
#ifndef __JUCE_SHAPEBUTTON_JUCEHEADER__

/*** Start of inlined file: juce_ShapeButton.h ***/
#ifndef __JUCE_SHAPEBUTTON_JUCEHEADER__
#define __JUCE_SHAPEBUTTON_JUCEHEADER__

/**
	A button that contains a filled shape.

	@see Button, ImageButton, TextButton, ArrowButton
*/
class JUCE_API  ShapeButton  : public Button
{
public:

	/** Creates a ShapeButton.

		@param name             a name to give the component - see Component::setName()
		@param normalColour     the colour to fill the shape with when the mouse isn't over
		@param overColour       the colour to use when the mouse is over the shape
		@param downColour       the colour to use when the button is in the pressed-down state
	*/
	ShapeButton (const String& name,
				 const Colour& normalColour,
				 const Colour& overColour,
				 const Colour& downColour);

	/** Destructor. */
	~ShapeButton();

	/** Sets the shape to use.

		@param newShape                 the shape to use
		@param resizeNowToFitThisShape  if true, the button will be resized to fit the shape's bounds
		@param maintainShapeProportions if true, the shape's proportions will be kept fixed when
										the button is resized
		@param hasDropShadow            if true, the button will be given a drop-shadow effect
	*/
	void setShape (const Path& newShape,
				   bool resizeNowToFitThisShape,
				   bool maintainShapeProportions,
				   bool hasDropShadow);

	/** Set the colours to use for drawing the shape.

		@param normalColour     the colour to fill the shape with when the mouse isn't over
		@param overColour       the colour to use when the mouse is over the shape
		@param downColour       the colour to use when the button is in the pressed-down state
	*/
	void setColours (const Colour& normalColour,
					 const Colour& overColour,
					 const Colour& downColour);

	/** Sets up an outline to draw around the shape.

		@param outlineColour        the colour to use
		@param outlineStrokeWidth   the thickness of line to draw
	*/
	void setOutline (const Colour& outlineColour,
					 float outlineStrokeWidth);

protected:
	/** @internal */
	void paintButton (Graphics& g,
					  bool isMouseOverButton,
					  bool isButtonDown);

private:

	Colour normalColour, overColour, downColour, outlineColour;
	DropShadowEffect shadow;
	Path shape;
	bool maintainShapeProportions;
	float outlineWidth;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShapeButton);
};

#endif   // __JUCE_SHAPEBUTTON_JUCEHEADER__

/*** End of inlined file: juce_ShapeButton.h ***/


#endif
#ifndef __JUCE_TEXTBUTTON_JUCEHEADER__

/*** Start of inlined file: juce_TextButton.h ***/
#ifndef __JUCE_TEXTBUTTON_JUCEHEADER__
#define __JUCE_TEXTBUTTON_JUCEHEADER__

/**
	A button that uses the standard lozenge-shaped background with a line of
	text on it.

	@see Button, DrawableButton
*/
class JUCE_API  TextButton  : public Button
{
public:

	/** Creates a TextButton.

		@param buttonName           the text to put in the button (the component's name is also
									initially set to this string, but these can be changed later
									using the setName() and setButtonText() methods)
		@param toolTip              an optional string to use as a toolip

		@see Button
	*/
	TextButton (const String& buttonName = String::empty,
				const String& toolTip = String::empty);

	/** Destructor. */
	~TextButton();

	/** A set of colour IDs to use to change the colour of various aspects of the button.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		buttonColourId                  = 0x1000100,  /**< The colour used to fill the button shape (when the button is toggled
														   'off'). The look-and-feel class might re-interpret this to add
														   effects, etc. */
		buttonOnColourId                = 0x1000101,  /**< The colour used to fill the button shape (when the button is toggled
														   'on'). The look-and-feel class might re-interpret this to add
														   effects, etc. */
		textColourOffId                 = 0x1000102,  /**< The colour to use for the button's text when the button's toggle state is "off". */
		textColourOnId                  = 0x1000103   /**< The colour to use for the button's text.when the button's toggle state is "on". */
	};

	/** Resizes the button to fit neatly around its current text.

		If newHeight is >= 0, the button's height will be changed to this
		value. If it's less than zero, its height will be unaffected.
	*/
	void changeWidthToFitText (int newHeight = -1);

	/** This can be overridden to use different fonts than the default one.

		Note that you'll need to set the font's size appropriately, too.
	*/
	virtual Font getFont();

	struct Ids
	{
		static const Identifier tagType;
	};

protected:
	/** @internal */
	void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
	/** @internal */
	void colourChanged();

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextButton);
};

#endif   // __JUCE_TEXTBUTTON_JUCEHEADER__

/*** End of inlined file: juce_TextButton.h ***/


#endif
#ifndef __JUCE_TOGGLEBUTTON_JUCEHEADER__

/*** Start of inlined file: juce_ToggleButton.h ***/
#ifndef __JUCE_TOGGLEBUTTON_JUCEHEADER__
#define __JUCE_TOGGLEBUTTON_JUCEHEADER__

/**
	A button that can be toggled on/off.

	All buttons can be toggle buttons, but this lets you create one of the
	standard ones which has a tick-box and a text label next to it.

	@see Button, DrawableButton, TextButton
*/
class JUCE_API  ToggleButton  : public Button
{
public:

	/** Creates a ToggleButton.

		@param buttonText   the text to put in the button (the component's name is also
							initially set to this string, but these can be changed later
							using the setName() and setButtonText() methods)
	*/
	explicit ToggleButton (const String& buttonText = String::empty);

	/** Destructor. */
	~ToggleButton();

	/** Resizes the button to fit neatly around its current text.

		The button's height won't be affected, only its width.
	*/
	void changeWidthToFitText();

	/** A set of colour IDs to use to change the colour of various aspects of the button.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		textColourId                    = 0x1006501   /**< The colour to use for the button's text. */
	};

	struct Ids
	{
		static const Identifier tagType;
	};

protected:
	/** @internal */
	void paintButton (Graphics& g,
					  bool isMouseOverButton,
					  bool isButtonDown);

	/** @internal */
	void colourChanged();

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToggleButton);
};

#endif   // __JUCE_TOGGLEBUTTON_JUCEHEADER__

/*** End of inlined file: juce_ToggleButton.h ***/


#endif
#ifndef __JUCE_TOOLBARBUTTON_JUCEHEADER__

/*** Start of inlined file: juce_ToolbarButton.h ***/
#ifndef __JUCE_TOOLBARBUTTON_JUCEHEADER__
#define __JUCE_TOOLBARBUTTON_JUCEHEADER__


/*** Start of inlined file: juce_ToolbarItemComponent.h ***/
#ifndef __JUCE_TOOLBARITEMCOMPONENT_JUCEHEADER__
#define __JUCE_TOOLBARITEMCOMPONENT_JUCEHEADER__


/*** Start of inlined file: juce_Toolbar.h ***/
#ifndef __JUCE_TOOLBAR_JUCEHEADER__
#define __JUCE_TOOLBAR_JUCEHEADER__

class ToolbarItemComponent;
class ToolbarItemFactory;

/**
	A toolbar component.

	A toolbar contains a horizontal or vertical strip of ToolbarItemComponents,
	and looks after their order and layout.

	Items (icon buttons or other custom components) are added to a toolbar using a
	ToolbarItemFactory - each type of item is given a unique ID number, and a
	toolbar might contain more than one instance of a particular item type.

	Toolbars can be interactively customised, allowing the user to drag the items
	around, and to drag items onto or off the toolbar, using the ToolbarItemPalette
	component as a source of new items.

	@see ToolbarItemFactory, ToolbarItemComponent, ToolbarItemPalette
*/
class JUCE_API  Toolbar   : public Component,
							public DragAndDropContainer,
							public DragAndDropTarget,
							private ButtonListener  // (can't use Button::Listener due to idiotic VC2005 bug)
{
public:

	/** Creates an empty toolbar component.

		To add some icons or other components to your toolbar, you'll need to
		create a ToolbarItemFactory class that can create a suitable set of
		ToolbarItemComponents.

		@see ToolbarItemFactory, ToolbarItemComponents
	*/
	Toolbar();

	/** Destructor.

		Any items on the bar will be deleted when the toolbar is deleted.
	*/
	~Toolbar();

	/** Changes the bar's orientation.
		@see isVertical
	*/
	void setVertical (bool shouldBeVertical);

	/** Returns true if the bar is set to be vertical, or false if it's horizontal.

		You can change the bar's orientation with setVertical().
	*/
	bool isVertical() const noexcept                 { return vertical; }

	/** Returns the depth of the bar.

		If the bar is horizontal, this will return its height; if it's vertical, it
		will return its width.

		@see getLength
	*/
	int getThickness() const noexcept;

	/** Returns the length of the bar.

		If the bar is horizontal, this will return its width; if it's vertical, it
		will return its height.

		@see getThickness
	*/
	int getLength() const noexcept;

	/** Deletes all items from the bar.
	*/
	void clear();

	/** Adds an item to the toolbar.

		The factory's ToolbarItemFactory::createItem() will be called by this method
		to create the component that will actually be added to the bar.

		The new item will be inserted at the specified index (if the index is -1, it
		will be added to the right-hand or bottom end of the bar).

		Once added, the component will be automatically deleted by this object when it
		is no longer needed.

		@see ToolbarItemFactory
	*/
	void addItem (ToolbarItemFactory& factory,
				  int itemId,
				  int insertIndex = -1);

	/** Deletes one of the items from the bar.
	*/
	void removeToolbarItem (int itemIndex);

	/** Returns the number of items currently on the toolbar.

		@see getItemId, getItemComponent
	*/
	int getNumItems() const noexcept;

	/** Returns the ID of the item with the given index.

		If the index is less than zero or greater than the number of items,
		this will return 0.

		@see getNumItems
	*/
	int getItemId (int itemIndex) const noexcept;

	/** Returns the component being used for the item with the given index.

		If the index is less than zero or greater than the number of items,
		this will return 0.

		@see getNumItems
	*/
	ToolbarItemComponent* getItemComponent (int itemIndex) const noexcept;

	/** Clears this toolbar and adds to it the default set of items that the specified
		factory creates.

		@see ToolbarItemFactory::getDefaultItemSet
	*/
	void addDefaultItems (ToolbarItemFactory& factoryToUse);

	/** Options for the way items should be displayed.
		@see setStyle, getStyle
	*/
	enum ToolbarItemStyle
	{
		iconsOnly,       /**< Means that the toolbar should just contain icons. */
		iconsWithText,   /**< Means that the toolbar should have text labels under each icon. */
		textOnly         /**< Means that the toolbar only display text labels for each item. */
	};

	/** Returns the toolbar's current style.
		@see ToolbarItemStyle, setStyle
	*/
	ToolbarItemStyle getStyle() const noexcept               { return toolbarStyle; }

	/** Changes the toolbar's current style.
		@see ToolbarItemStyle, getStyle, ToolbarItemComponent::setStyle
	*/
	void setStyle (const ToolbarItemStyle& newStyle);

	/** Flags used by the showCustomisationDialog() method. */
	enum CustomisationFlags
	{
		allowIconsOnlyChoice            = 1,    /**< If this flag is specified, the customisation dialog can
													 show the "icons only" option on its choice of toolbar styles. */
		allowIconsWithTextChoice        = 2,    /**< If this flag is specified, the customisation dialog can
													 show the "icons with text" option on its choice of toolbar styles. */
		allowTextOnlyChoice             = 4,    /**< If this flag is specified, the customisation dialog can
													 show the "text only" option on its choice of toolbar styles. */
		showResetToDefaultsButton       = 8,    /**< If this flag is specified, the customisation dialog can
													 show a button to reset the toolbar to its default set of items. */

		allCustomisationOptionsEnabled = (allowIconsOnlyChoice | allowIconsWithTextChoice | allowTextOnlyChoice | showResetToDefaultsButton)
	};

	/** Pops up a modal dialog box that allows this toolbar to be customised by the user.

		The dialog contains a ToolbarItemPalette and various controls for editing other
		aspects of the toolbar. The dialog box will be opened modally, but the method will
		return immediately.

		The factory is used to determine the set of items that will be shown on the
		palette.

		The optionFlags parameter is a bitwise-or of values from the CustomisationFlags
		enum.

		@see ToolbarItemPalette
	*/
	void showCustomisationDialog (ToolbarItemFactory& factory,
								  int optionFlags = allCustomisationOptionsEnabled);

	/** Turns on or off the toolbar's editing mode, in which its items can be
		rearranged by the user.

		(In most cases it's easier just to use showCustomisationDialog() instead of
		trying to enable editing directly).

		@see ToolbarItemPalette
	*/
	void setEditingActive (bool editingEnabled);

	/** A set of colour IDs to use to change the colour of various aspects of the toolbar.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId          = 0x1003200,  /**< A colour to use to fill the toolbar's background. For
													   more control over this, override LookAndFeel::paintToolbarBackground(). */
		separatorColourId           = 0x1003210,  /**< A colour to use to draw the separator lines. */

		buttonMouseOverBackgroundColourId = 0x1003220,  /**< A colour used to paint the background of buttons when the mouse is
															 over them. */
		buttonMouseDownBackgroundColourId = 0x1003230,  /**< A colour used to paint the background of buttons when the mouse is
															 held down on them. */

		labelTextColourId           = 0x1003240,        /**< A colour to use for drawing the text under buttons
															 when the style is set to iconsWithText or textOnly. */

		editingModeOutlineColourId  = 0x1003250   /**< A colour to use for an outline around buttons when
													   the customisation dialog is active and the mouse moves over them. */
	};

	/** Returns a string that represents the toolbar's current set of items.

		This lets you later restore the same item layout using restoreFromString().

		@see restoreFromString
	*/
	String toString() const;

	/** Restores a set of items that was previously stored in a string by the toString()
		method.

		The factory object is used to create any item components that are needed.

		@see toString
	*/
	bool restoreFromString (ToolbarItemFactory& factoryToUse,
							const String& savedVersion);

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void buttonClicked (Button*);
	/** @internal */
	void mouseDown (const MouseEvent&);
	/** @internal */
	bool isInterestedInDragSource (const SourceDetails&);
	/** @internal */
	void itemDragMove (const SourceDetails&);
	/** @internal */
	void itemDragExit (const SourceDetails&);
	/** @internal */
	void itemDropped (const SourceDetails&);
	/** @internal */
	void updateAllItemPositions (bool animate);
	/** @internal */
	static ToolbarItemComponent* createItem (ToolbarItemFactory&, int itemId);

private:

	ScopedPointer<Button> missingItemsButton;
	bool vertical, isEditingActive;
	ToolbarItemStyle toolbarStyle;
	class MissingItemsComponent;
	friend class MissingItemsComponent;
	OwnedArray <ToolbarItemComponent> items;

	friend class ItemDragAndDropOverlayComponent;
	static const char* const toolbarDragDescriptor;

	void addItemInternal (ToolbarItemFactory& factory, int itemId, int insertIndex);

	ToolbarItemComponent* getNextActiveComponent (int index, int delta) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Toolbar);
};

#endif   // __JUCE_TOOLBAR_JUCEHEADER__

/*** End of inlined file: juce_Toolbar.h ***/

class ItemDragAndDropOverlayComponent;

/**
	A component that can be used as one of the items in a Toolbar.

	Each of the items on a toolbar must be a component derived from ToolbarItemComponent,
	and these objects are always created by a ToolbarItemFactory - see the ToolbarItemFactory
	class for further info about creating them.

	The ToolbarItemComponent class is actually a button, but can be used to hold non-button
	components too. To do this, set the value of isBeingUsedAsAButton to false when
	calling the constructor, and override contentAreaChanged(), in which you can position
	any sub-components you need to add.

	To add basic buttons without writing a special subclass, have a look at the
	ToolbarButton class.

	@see ToolbarButton, Toolbar, ToolbarItemFactory
*/
class JUCE_API  ToolbarItemComponent  : public Button
{
public:

	/** Constructor.

		@param itemId       the ID of the type of toolbar item which this represents
		@param labelText    the text to display if the toolbar's style is set to
							Toolbar::iconsWithText or Toolbar::textOnly
		@param isBeingUsedAsAButton     set this to false if you don't want the button
							to draw itself with button over/down states when the mouse
							moves over it or clicks
	*/
	ToolbarItemComponent (int itemId,
						  const String& labelText,
						  bool isBeingUsedAsAButton);

	/** Destructor. */
	~ToolbarItemComponent();

	/** Returns the item type ID that this component represents.
		This value is in the constructor.
	*/
	int getItemId() const noexcept                                  { return itemId; }

	/** Returns the toolbar that contains this component, or 0 if it's not currently
		inside one.
	*/
	Toolbar* getToolbar() const;

	/** Returns true if this component is currently inside a toolbar which is vertical.
		@see Toolbar::isVertical
	*/
	bool isToolbarVertical() const;

	/** Returns the current style setting of this item.

		Styles are listed in the Toolbar::ToolbarItemStyle enum.
		@see setStyle, Toolbar::getStyle
	*/
	Toolbar::ToolbarItemStyle getStyle() const noexcept             { return toolbarStyle; }

	/** Changes the current style setting of this item.

		Styles are listed in the Toolbar::ToolbarItemStyle enum, and are automatically updated
		by the toolbar that holds this item.

		@see setStyle, Toolbar::setStyle
	*/
	virtual void setStyle (const Toolbar::ToolbarItemStyle& newStyle);

	/** Returns the area of the component that should be used to display the button image or
		other contents of the item.

		This content area may change when the item's style changes, and may leave a space around the
		edge of the component where the text label can be shown.

		@see contentAreaChanged
	*/
	const Rectangle<int>& getContentArea() const noexcept           { return contentArea; }

	/** This method must return the size criteria for this item, based on a given toolbar
		size and orientation.

		The preferredSize, minSize and maxSize values must all be set by your implementation
		method. If the toolbar is horizontal, these will be the width of the item; for a vertical
		toolbar, they refer to the item's height.

		The preferredSize is the size that the component would like to be, and this must be
		between the min and max sizes. For a fixed-size item, simply set all three variables to
		the same value.

		The toolbarThickness parameter tells you the depth of the toolbar - the same as calling
		Toolbar::getThickness().

		The isToolbarVertical parameter tells you whether the bar is oriented horizontally or
		vertically.
	*/
	virtual bool getToolbarItemSizes (int toolbarThickness,
									  bool isToolbarVertical,
									  int& preferredSize,
									  int& minSize,
									  int& maxSize) = 0;

	/** Your subclass should use this method to draw its content area.

		The graphics object that is passed-in will have been clipped and had its origin
		moved to fit the content area as specified get getContentArea(). The width and height
		parameters are the width and height of the content area.

		If the component you're writing isn't a button, you can just do nothing in this method.
	*/
	virtual void paintButtonArea (Graphics& g,
								  int width, int height,
								  bool isMouseOver, bool isMouseDown) = 0;

	/** Callback to indicate that the content area of this item has changed.

		This might be because the component was resized, or because the style changed and
		the space needed for the text label is different.

		See getContentArea() for a description of what the area is.
	*/
	virtual void contentAreaChanged (const Rectangle<int>& newBounds) = 0;

	/** Editing modes.
		These are used by setEditingMode(), but will be rarely needed in user code.
	*/
	enum ToolbarEditingMode
	{
		normalMode = 0,     /**< Means that the component is active, inside a toolbar. */
		editableOnToolbar,  /**< Means that the component is on a toolbar, but the toolbar is in
								 customisation mode, and the items can be dragged around. */
		editableOnPalette   /**< Means that the component is on an new-item palette, so it can be
								 dragged onto a toolbar to add it to that bar.*/
	};

	/** Changes the editing mode of this component.

		This is used by the ToolbarItemPalette and related classes for making the items draggable,
		and is unlikely to be of much use in end-user-code.
	*/
	void setEditingMode (const ToolbarEditingMode newMode);

	/** Returns the current editing mode of this component.

		This is used by the ToolbarItemPalette and related classes for making the items draggable,
		and is unlikely to be of much use in end-user-code.
	*/
	ToolbarEditingMode getEditingMode() const noexcept                  { return mode; }

	/** @internal */
	void paintButton (Graphics& g, bool isMouseOver, bool isMouseDown);
	/** @internal */
	void resized();

private:
	friend class Toolbar;
	friend class ItemDragAndDropOverlayComponent;
	const int itemId;
	ToolbarEditingMode mode;
	Toolbar::ToolbarItemStyle toolbarStyle;
	ScopedPointer <Component> overlayComp;
	int dragOffsetX, dragOffsetY;
	bool isActive, isBeingDragged, isBeingUsedAsAButton;
	Rectangle<int> contentArea;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolbarItemComponent);
};

#endif   // __JUCE_TOOLBARITEMCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_ToolbarItemComponent.h ***/

/**
	A type of button designed to go on a toolbar.

	This simple button can have two Drawable objects specified - one for normal
	use and another one (optionally) for the button's "on" state if it's a
	toggle button.

	@see Toolbar, ToolbarItemFactory, ToolbarItemComponent, Drawable, Button
*/
class JUCE_API  ToolbarButton   : public ToolbarItemComponent
{
public:

	/** Creates a ToolbarButton.

		@param itemId       the ID for this toolbar item type. This is passed through to the
							ToolbarItemComponent constructor
		@param labelText    the text to display on the button (if the toolbar is using a style
							that shows text labels). This is passed through to the
							ToolbarItemComponent constructor
		@param normalImage  a drawable object that the button should use as its icon. The object
							that is passed-in here will be kept by this object and will be
							deleted when no longer needed or when this button is deleted.
		@param toggledOnImage  a drawable object that the button can use as its icon if the button
							is in a toggled-on state (see the Button::getToggleState() method). If
							0 is passed-in here, then the normal image will be used instead, regardless
							of the toggle state. The object that is passed-in here will be kept by
							this object and will be deleted when no longer needed or when this button
							is deleted.
	*/
	ToolbarButton (int itemId,
				   const String& labelText,
				   Drawable* normalImage,
				   Drawable* toggledOnImage);

	/** Destructor. */
	~ToolbarButton();

	/** @internal */
	bool getToolbarItemSizes (int toolbarDepth, bool isToolbarVertical, int& preferredSize,
							  int& minSize, int& maxSize);
	/** @internal */
	void paintButtonArea (Graphics&, int width, int height, bool isMouseOver, bool isMouseDown);
	/** @internal */
	void contentAreaChanged (const Rectangle<int>&);
	/** @internal */
	void buttonStateChanged();
	/** @internal */
	void resized();
	/** @internal */
	void enablementChanged();

private:

	ScopedPointer<Drawable> normalImage, toggledOnImage;
	Drawable* currentImage;

	void updateDrawable();
	Drawable* getImageToUse() const;
	void setCurrentImage (Drawable*);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolbarButton);
};

#endif   // __JUCE_TOOLBARBUTTON_JUCEHEADER__

/*** End of inlined file: juce_ToolbarButton.h ***/


#endif
#ifndef __JUCE_DRAWABLE_JUCEHEADER__

#endif
#ifndef __JUCE_DRAWABLECOMPOSITE_JUCEHEADER__

/*** Start of inlined file: juce_DrawableComposite.h ***/
#ifndef __JUCE_DRAWABLECOMPOSITE_JUCEHEADER__
#define __JUCE_DRAWABLECOMPOSITE_JUCEHEADER__


/*** Start of inlined file: juce_RelativeParallelogram.h ***/
#ifndef __JUCE_RELATIVEPARALLELOGRAM_JUCEHEADER__
#define __JUCE_RELATIVEPARALLELOGRAM_JUCEHEADER__

/**
	A parallelogram defined by three RelativePoint positions.

	@see RelativePoint, RelativeCoordinate
*/
class JUCE_API  RelativeParallelogram
{
public:

	RelativeParallelogram();
	RelativeParallelogram (const Rectangle<float>& simpleRectangle);
	RelativeParallelogram (const RelativePoint& topLeft, const RelativePoint& topRight, const RelativePoint& bottomLeft);
	RelativeParallelogram (const String& topLeft, const String& topRight, const String& bottomLeft);
	~RelativeParallelogram();

	void resolveThreePoints (Point<float>* points, Expression::Scope* scope) const;
	void resolveFourCorners (Point<float>* points, Expression::Scope* scope) const;
	const Rectangle<float> getBounds (Expression::Scope* scope) const;
	void getPath (Path& path, Expression::Scope* scope) const;
	const AffineTransform resetToPerpendicular (Expression::Scope* scope);
	bool isDynamic() const;

	bool operator== (const RelativeParallelogram& other) const noexcept;
	bool operator!= (const RelativeParallelogram& other) const noexcept;

	static const Point<float> getInternalCoordForPoint (const Point<float>* parallelogramCorners, Point<float> point) noexcept;
	static const Point<float> getPointForInternalCoord (const Point<float>* parallelogramCorners, const Point<float>& internalPoint) noexcept;
	static const Rectangle<float> getBoundingBox (const Point<float>* parallelogramCorners) noexcept;

	RelativePoint topLeft, topRight, bottomLeft;
};

#endif   // __JUCE_RELATIVEPARALLELOGRAM_JUCEHEADER__

/*** End of inlined file: juce_RelativeParallelogram.h ***/


/*** Start of inlined file: juce_RelativeRectangle.h ***/
#ifndef __JUCE_RELATIVERECTANGLE_JUCEHEADER__
#define __JUCE_RELATIVERECTANGLE_JUCEHEADER__

class Component;

/**
	An rectangle stored as a set of RelativeCoordinate values.

	The rectangle's top, left, bottom and right edge positions are each stored as a RelativeCoordinate.

	@see RelativeCoordinate, RelativePoint
*/
class JUCE_API  RelativeRectangle
{
public:

	/** Creates a zero-size rectangle at the origin. */
	RelativeRectangle();

	/** Creates an absolute rectangle, relative to the origin. */
	explicit RelativeRectangle (const Rectangle<float>& rect);

	/** Creates a rectangle from four coordinates. */
	RelativeRectangle (const RelativeCoordinate& left, const RelativeCoordinate& right,
					   const RelativeCoordinate& top, const RelativeCoordinate& bottom);

	/** Creates a rectangle from a stringified representation.
		The string must contain a sequence of 4 coordinates, separated by commas, in the order
		left, top, right, bottom. The syntax for the coordinate strings is explained in the
		RelativeCoordinate class.
		@see toString
	*/
	explicit RelativeRectangle (const String& stringVersion);

	bool operator== (const RelativeRectangle& other) const noexcept;
	bool operator!= (const RelativeRectangle& other) const noexcept;

	/** Calculates the absolute position of this rectangle.

		You'll need to provide a suitable Expression::Scope for looking up any coordinates that may
		be needed to calculate the result.
	*/
	const Rectangle<float> resolve (const Expression::Scope* scope) const;

	/** Changes the values of this rectangle's coordinates to make it resolve to the specified position.

		Calling this will leave any anchor points unchanged, but will set any absolute
		or relative positions to whatever values are necessary to make the resultant position
		match the position that is provided.
	*/
	void moveToAbsolute (const Rectangle<float>& newPos, const Expression::Scope* scope);

	/** Returns true if this rectangle depends on any external symbols for its position.
		Coordinates that refer to symbols based on "this" are assumed not to be dynamic.
	*/
	bool isDynamic() const;

	/** Returns a string which represents this point.
		This returns a comma-separated list of coordinates, in the order left, top, right, bottom. For details of
		the string syntax used by the coordinates, see the RelativeCoordinate constructor notes.
		The string that is returned can be passed to the RelativeRectangle constructor to recreate the rectangle.
	*/
	String toString() const;

	/** Renames a symbol if it is used by any of the coordinates.
		This calls Expression::withRenamedSymbol() on the rectangle's coordinates.
	*/
	void renameSymbol (const Expression::Symbol& oldSymbol, const String& newName, const Expression::Scope& scope);

	/** Creates and sets an appropriate Component::Positioner object for the given component, which will
		keep it positioned with this rectangle.
	*/
	void applyToComponent (Component& component) const;

	// The actual rectangle coords...
	RelativeCoordinate left, right, top, bottom;
};

#endif   // __JUCE_RELATIVERECTANGLE_JUCEHEADER__

/*** End of inlined file: juce_RelativeRectangle.h ***/

/**
	A drawable object which acts as a container for a set of other Drawables.

	@see Drawable
*/
class JUCE_API  DrawableComposite  : public Drawable
{
public:

	/** Creates a composite Drawable. */
	DrawableComposite();

	/** Creates a copy of a DrawableComposite. */
	DrawableComposite (const DrawableComposite& other);

	/** Destructor. */
	~DrawableComposite();

	/** Sets the parallelogram that defines the target position of the content rectangle when the drawable is rendered.
		@see setContentArea
	*/
	void setBoundingBox (const RelativeParallelogram& newBoundingBox);

	/** Returns the parallelogram that defines the target position of the content rectangle when the drawable is rendered.
		@see setBoundingBox
	*/
	const RelativeParallelogram& getBoundingBox() const noexcept            { return bounds; }

	/** Changes the bounding box transform to match the content area, so that any sub-items will
		be drawn at their untransformed positions.
	*/
	void resetBoundingBoxToContentArea();

	/** Returns the main content rectangle.
		The content area is actually defined by the markers named "left", "right", "top" and
		"bottom", but this method is a shortcut that returns them all at once.
		@see contentLeftMarkerName, contentRightMarkerName, contentTopMarkerName, contentBottomMarkerName
	*/
	RelativeRectangle getContentArea() const;

	/** Changes the main content area.
		The content area is actually defined by the markers named "left", "right", "top" and
		"bottom", but this method is a shortcut that sets them all at once.
		@see setBoundingBox, contentLeftMarkerName, contentRightMarkerName, contentTopMarkerName, contentBottomMarkerName
	*/
	void setContentArea (const RelativeRectangle& newArea);

	/** Resets the content area and the bounding transform to fit around the area occupied
		by the child components (ignoring any markers).
	*/
	void resetContentAreaAndBoundingBoxToFitChildren();

	/** The name of the marker that defines the left edge of the content area. */
	static const char* const contentLeftMarkerName;
	/** The name of the marker that defines the right edge of the content area. */
	static const char* const contentRightMarkerName;
	/** The name of the marker that defines the top edge of the content area. */
	static const char* const contentTopMarkerName;
	/** The name of the marker that defines the bottom edge of the content area. */
	static const char* const contentBottomMarkerName;

	/** @internal */
	Drawable* createCopy() const;
	/** @internal */
	void refreshFromValueTree (const ValueTree& tree, ComponentBuilder& builder);
	/** @internal */
	ValueTree createValueTree (ComponentBuilder::ImageProvider* imageProvider) const;
	/** @internal */
	static const Identifier valueTreeType;
	/** @internal */
	Rectangle<float> getDrawableBounds() const;
	/** @internal */
	void childBoundsChanged (Component*);
	/** @internal */
	void childrenChanged();
	/** @internal */
	void parentHierarchyChanged();
	/** @internal */
	MarkerList* getMarkers (bool xAxis);

	/** Internally-used class for wrapping a DrawableComposite's state into a ValueTree. */
	class ValueTreeWrapper   : public Drawable::ValueTreeWrapperBase
	{
	public:
		ValueTreeWrapper (const ValueTree& state);

		ValueTree getChildList() const;
		ValueTree getChildListCreating (UndoManager* undoManager);

		RelativeParallelogram getBoundingBox() const;
		void setBoundingBox (const RelativeParallelogram& newBounds, UndoManager* undoManager);
		void resetBoundingBoxToContentArea (UndoManager* undoManager);

		RelativeRectangle getContentArea() const;
		void setContentArea (const RelativeRectangle& newArea, UndoManager* undoManager);

		MarkerList::ValueTreeWrapper getMarkerList (bool xAxis) const;
		MarkerList::ValueTreeWrapper getMarkerListCreating (bool xAxis, UndoManager* undoManager);

		static const Identifier topLeft, topRight, bottomLeft;

	private:
		static const Identifier childGroupTag, markerGroupTagX, markerGroupTagY;
	};

private:

	RelativeParallelogram bounds;
	MarkerList markersX, markersY;
	bool updateBoundsReentrant;

	friend class Drawable::Positioner<DrawableComposite>;
	bool registerCoordinates (RelativeCoordinatePositionerBase&);
	void recalculateCoordinates (Expression::Scope*);

	void updateBoundsToFitChildren();

	DrawableComposite& operator= (const DrawableComposite&);
	JUCE_LEAK_DETECTOR (DrawableComposite);
};

#endif   // __JUCE_DRAWABLECOMPOSITE_JUCEHEADER__

/*** End of inlined file: juce_DrawableComposite.h ***/


#endif
#ifndef __JUCE_DRAWABLEIMAGE_JUCEHEADER__

/*** Start of inlined file: juce_DrawableImage.h ***/
#ifndef __JUCE_DRAWABLEIMAGE_JUCEHEADER__
#define __JUCE_DRAWABLEIMAGE_JUCEHEADER__

/**
	A drawable object which is a bitmap image.

	@see Drawable
*/
class JUCE_API  DrawableImage  : public Drawable
{
public:

	DrawableImage();
	DrawableImage (const DrawableImage& other);

	/** Destructor. */
	~DrawableImage();

	/** Sets the image that this drawable will render. */
	void setImage (const Image& imageToUse);

	/** Returns the current image. */
	const Image& getImage() const noexcept                      { return image; }

	/** Sets the opacity to use when drawing the image. */
	void setOpacity (float newOpacity);

	/** Returns the image's opacity. */
	float getOpacity() const noexcept                           { return opacity; }

	/** Sets a colour to draw over the image's alpha channel.

		By default this is transparent so isn't drawn, but if you set a non-transparent
		colour here, then it will be overlaid on the image, using the image's alpha
		channel as a mask.

		This is handy for doing things like darkening or lightening an image by overlaying
		it with semi-transparent black or white.
	*/
	void setOverlayColour (const Colour& newOverlayColour);

	/** Returns the overlay colour. */
	const Colour& getOverlayColour() const noexcept             { return overlayColour; }

	/** Sets the bounding box within which the image should be displayed. */
	void setBoundingBox (const RelativeParallelogram& newBounds);

	/** Returns the position to which the image's top-left corner should be remapped in the target
		coordinate space when rendering this object.
		@see setTransform
	*/
	const RelativeParallelogram& getBoundingBox() const noexcept        { return bounds; }

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	bool hitTest (int x, int y);
	/** @internal */
	Drawable* createCopy() const;
	/** @internal */
	Rectangle<float> getDrawableBounds() const;
	/** @internal */
	void refreshFromValueTree (const ValueTree& tree, ComponentBuilder& builder);
	/** @internal */
	ValueTree createValueTree (ComponentBuilder::ImageProvider* imageProvider) const;
	/** @internal */
	static const Identifier valueTreeType;

	/** Internally-used class for wrapping a DrawableImage's state into a ValueTree. */
	class ValueTreeWrapper   : public Drawable::ValueTreeWrapperBase
	{
	public:
		ValueTreeWrapper (const ValueTree& state);

		var getImageIdentifier() const;
		void setImageIdentifier (const var& newIdentifier, UndoManager* undoManager);
		Value getImageIdentifierValue (UndoManager* undoManager);

		float getOpacity() const;
		void setOpacity (float newOpacity, UndoManager* undoManager);
		Value getOpacityValue (UndoManager* undoManager);

		Colour getOverlayColour() const;
		void setOverlayColour (const Colour& newColour, UndoManager* undoManager);
		Value getOverlayColourValue (UndoManager* undoManager);

		RelativeParallelogram getBoundingBox() const;
		void setBoundingBox (const RelativeParallelogram& newBounds, UndoManager* undoManager);

		static const Identifier opacity, overlay, image, topLeft, topRight, bottomLeft;
	};

private:

	Image image;
	float opacity;
	Colour overlayColour;
	RelativeParallelogram bounds;

	friend class Drawable::Positioner<DrawableImage>;
	bool registerCoordinates (RelativeCoordinatePositionerBase&);
	void recalculateCoordinates (Expression::Scope*);

	DrawableImage& operator= (const DrawableImage&);
	JUCE_LEAK_DETECTOR (DrawableImage);
};

#endif   // __JUCE_DRAWABLEIMAGE_JUCEHEADER__

/*** End of inlined file: juce_DrawableImage.h ***/


#endif
#ifndef __JUCE_DRAWABLEPATH_JUCEHEADER__

/*** Start of inlined file: juce_DrawablePath.h ***/
#ifndef __JUCE_DRAWABLEPATH_JUCEHEADER__
#define __JUCE_DRAWABLEPATH_JUCEHEADER__


/*** Start of inlined file: juce_DrawableShape.h ***/
#ifndef __JUCE_DRAWABLESHAPE_JUCEHEADER__
#define __JUCE_DRAWABLESHAPE_JUCEHEADER__

/**
	A base class implementing common functionality for Drawable classes which
	consist of some kind of filled and stroked outline.

	@see DrawablePath, DrawableRectangle
*/
class JUCE_API  DrawableShape   : public Drawable
{
protected:

	DrawableShape();
	DrawableShape (const DrawableShape&);

public:
	/** Destructor. */
	~DrawableShape();

	/** A FillType wrapper that allows the gradient coordinates to be implemented using RelativePoint.
	*/
	class RelativeFillType
	{
	public:
		RelativeFillType();
		RelativeFillType (const FillType& fill);
		RelativeFillType (const RelativeFillType&);
		RelativeFillType& operator= (const RelativeFillType&);

		bool operator== (const RelativeFillType&) const;
		bool operator!= (const RelativeFillType&) const;

		bool isDynamic() const;
		bool recalculateCoords (Expression::Scope* scope);

		void writeTo (ValueTree& v, ComponentBuilder::ImageProvider*, UndoManager*) const;
		bool readFrom (const ValueTree& v, ComponentBuilder::ImageProvider*);

		FillType fill;
		RelativePoint gradientPoint1, gradientPoint2, gradientPoint3;
	};

	/** Sets a fill type for the path.
		This colour is used to fill the path - if you don't want the path to be
		filled (e.g. if you're just drawing an outline), set this to a transparent
		colour.

		@see setPath, setStrokeFill
	*/
	void setFill (const FillType& newFill);

	/** Sets a fill type for the path.
		This colour is used to fill the path - if you don't want the path to be
		filled (e.g. if you're just drawing an outline), set this to a transparent
		colour.

		@see setPath, setStrokeFill
	*/
	void setFill (const RelativeFillType& newFill);

	/** Returns the current fill type.
		@see setFill
	*/
	const RelativeFillType& getFill() const noexcept                { return mainFill; }

	/** Sets the fill type with which the outline will be drawn.
		@see setFill
	*/
	void setStrokeFill (const FillType& newStrokeFill);

	/** Sets the fill type with which the outline will be drawn.
		@see setFill
	*/
	void setStrokeFill (const RelativeFillType& newStrokeFill);

	/** Returns the current stroke fill.
		@see setStrokeFill
	*/
	const RelativeFillType& getStrokeFill() const noexcept          { return strokeFill; }

	/** Changes the properties of the outline that will be drawn around the path.
		If the stroke has 0 thickness, no stroke will be drawn.
		@see setStrokeThickness, setStrokeColour
	*/
	void setStrokeType (const PathStrokeType& newStrokeType);

	/** Changes the stroke thickness.
		This is a shortcut for calling setStrokeType.
	*/
	void setStrokeThickness (float newThickness);

	/** Returns the current outline style. */
	const PathStrokeType& getStrokeType() const noexcept            { return strokeType; }

	/** @internal */
	class FillAndStrokeState  : public  Drawable::ValueTreeWrapperBase
	{
	public:
		FillAndStrokeState (const ValueTree& state);

		ValueTree getFillState (const Identifier& fillOrStrokeType);
		RelativeFillType getFill (const Identifier& fillOrStrokeType, ComponentBuilder::ImageProvider*) const;
		void setFill (const Identifier& fillOrStrokeType, const RelativeFillType& newFill,
					  ComponentBuilder::ImageProvider*, UndoManager*);

		PathStrokeType getStrokeType() const;
		void setStrokeType (const PathStrokeType& newStrokeType, UndoManager*);

		static const Identifier type, colour, colours, fill, stroke, path, jointStyle, capStyle, strokeWidth,
								gradientPoint1, gradientPoint2, gradientPoint3, radial, imageId, imageOpacity;
	};

	/** @internal */
	Rectangle<float> getDrawableBounds() const;
	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	bool hitTest (int x, int y);

protected:

	/** Called when the cached path should be updated. */
	void pathChanged();
	/** Called when the cached stroke should be updated. */
	void strokeChanged();
	/** True if there's a stroke with a non-zero thickness and non-transparent colour. */
	bool isStrokeVisible() const noexcept;
	/** Updates the details from a FillAndStrokeState object, returning true if something changed. */
	void refreshFillTypes (const FillAndStrokeState& newState, ComponentBuilder::ImageProvider*);
	/** Writes the stroke and fill details to a FillAndStrokeState object. */
	void writeTo (FillAndStrokeState& state, ComponentBuilder::ImageProvider*, UndoManager*) const;

	PathStrokeType strokeType;
	Path path, strokePath;

private:
	class RelativePositioner;
	RelativeFillType mainFill, strokeFill;
	ScopedPointer<RelativeCoordinatePositionerBase> mainFillPositioner, strokeFillPositioner;

	void setFillInternal (RelativeFillType& fill, const RelativeFillType& newFill,
						  ScopedPointer<RelativeCoordinatePositionerBase>& positioner);

	DrawableShape& operator= (const DrawableShape&);
};

#endif   // __JUCE_DRAWABLESHAPE_JUCEHEADER__

/*** End of inlined file: juce_DrawableShape.h ***/


/*** Start of inlined file: juce_RelativePointPath.h ***/
#ifndef __JUCE_RELATIVEPOINTPATH_JUCEHEADER__
#define __JUCE_RELATIVEPOINTPATH_JUCEHEADER__

class DrawablePath;

/**
	A path object that consists of RelativePoint coordinates rather than the normal fixed ones.

	One of these paths can be converted into a Path object for drawing and manipulation, but
	unlike a Path, its points can be dynamic instead of just fixed.

	@see RelativePoint, RelativeCoordinate
*/
class JUCE_API  RelativePointPath
{
public:

	RelativePointPath();
	RelativePointPath (const RelativePointPath& other);
	explicit RelativePointPath (const Path& path);
	~RelativePointPath();

	bool operator== (const RelativePointPath& other) const noexcept;
	bool operator!= (const RelativePointPath& other) const noexcept;

	/** Resolves this points in this path and adds them to a normal Path object. */
	void createPath (Path& path, Expression::Scope* scope) const;

	/** Returns true if the path contains any non-fixed points. */
	bool containsAnyDynamicPoints() const;

	/** Quickly swaps the contents of this path with another. */
	void swapWith (RelativePointPath& other) noexcept;

	/** The types of element that may be contained in this path.
		@see RelativePointPath::ElementBase
	*/
	enum ElementType
	{
		nullElement,
		startSubPathElement,
		closeSubPathElement,
		lineToElement,
		quadraticToElement,
		cubicToElement
	};

	/** Base class for the elements that make up a RelativePointPath.
	*/
	class JUCE_API  ElementBase
	{
	public:
		ElementBase (ElementType type);
		virtual ~ElementBase() {}
		virtual ValueTree createTree() const = 0;
		virtual void addToPath (Path& path, Expression::Scope*) const = 0;
		virtual RelativePoint* getControlPoints (int& numPoints) = 0;
		virtual ElementBase* clone() const = 0;
		bool isDynamic();

		const ElementType type;

	private:
		JUCE_DECLARE_NON_COPYABLE (ElementBase);
	};

	class JUCE_API  StartSubPath  : public ElementBase
	{
	public:
		StartSubPath (const RelativePoint& pos);
		ValueTree createTree() const;
		void addToPath (Path& path, Expression::Scope*) const;
		RelativePoint* getControlPoints (int& numPoints);
		ElementBase* clone() const;

		RelativePoint startPos;

	private:
		JUCE_DECLARE_NON_COPYABLE (StartSubPath);
	};

	class JUCE_API  CloseSubPath  : public ElementBase
	{
	public:
		CloseSubPath();
		ValueTree createTree() const;
		void addToPath (Path& path, Expression::Scope*) const;
		RelativePoint* getControlPoints (int& numPoints);
		ElementBase* clone() const;

	private:
		JUCE_DECLARE_NON_COPYABLE (CloseSubPath);
	};

	class JUCE_API  LineTo  : public ElementBase
	{
	public:
		LineTo (const RelativePoint& endPoint);
		ValueTree createTree() const;
		void addToPath (Path& path, Expression::Scope*) const;
		RelativePoint* getControlPoints (int& numPoints);
		ElementBase* clone() const;

		RelativePoint endPoint;

	private:
		JUCE_DECLARE_NON_COPYABLE (LineTo);
	};

	class JUCE_API  QuadraticTo  : public ElementBase
	{
	public:
		QuadraticTo (const RelativePoint& controlPoint, const RelativePoint& endPoint);
		ValueTree createTree() const;
		void addToPath (Path& path, Expression::Scope*) const;
		RelativePoint* getControlPoints (int& numPoints);
		ElementBase* clone() const;

		RelativePoint controlPoints[2];

	private:
		JUCE_DECLARE_NON_COPYABLE (QuadraticTo);
	};

	class JUCE_API  CubicTo  : public ElementBase
	{
	public:
		CubicTo (const RelativePoint& controlPoint1, const RelativePoint& controlPoint2, const RelativePoint& endPoint);
		ValueTree createTree() const;
		void addToPath (Path& path, Expression::Scope*) const;
		RelativePoint* getControlPoints (int& numPoints);
		ElementBase* clone() const;

		RelativePoint controlPoints[3];

	private:
		JUCE_DECLARE_NON_COPYABLE (CubicTo);
	};

	void addElement (ElementBase* newElement);

	OwnedArray <ElementBase> elements;
	bool usesNonZeroWinding;

private:
	class Positioner;
	friend class Positioner;
	bool containsDynamicPoints;

	void applyTo (DrawablePath& path) const;

	RelativePointPath& operator= (const RelativePointPath&);
	JUCE_LEAK_DETECTOR (RelativePointPath);
};

#endif   // __JUCE_RELATIVEPOINTPATH_JUCEHEADER__

/*** End of inlined file: juce_RelativePointPath.h ***/

/**
	A drawable object which renders a filled or outlined shape.

	For details on how to change the fill and stroke, see the DrawableShape class.

	@see Drawable, DrawableShape
*/
class JUCE_API  DrawablePath  : public DrawableShape
{
public:

	/** Creates a DrawablePath. */
	DrawablePath();
	DrawablePath (const DrawablePath& other);

	/** Destructor. */
	~DrawablePath();

	/** Changes the path that will be drawn.
		@see setFillColour, setStrokeType
	*/
	void setPath (const Path& newPath);

	/** Sets the path using a RelativePointPath.
		Calling this will set up a Component::Positioner to automatically update the path
		if any of the points in the source path are dynamic.
	*/
	void setPath (const RelativePointPath& newPath);

	/** Returns the current path. */
	const Path& getPath() const;

	/** Returns the current path for the outline. */
	const Path& getStrokePath() const;

	/** @internal */
	Drawable* createCopy() const;
	/** @internal */
	void refreshFromValueTree (const ValueTree& tree, ComponentBuilder& builder);
	/** @internal */
	ValueTree createValueTree (ComponentBuilder::ImageProvider* imageProvider) const;
	/** @internal */
	static const Identifier valueTreeType;

	/** Internally-used class for wrapping a DrawablePath's state into a ValueTree. */
	class ValueTreeWrapper   : public DrawableShape::FillAndStrokeState
	{
	public:
		ValueTreeWrapper (const ValueTree& state);

		bool usesNonZeroWinding() const;
		void setUsesNonZeroWinding (bool b, UndoManager* undoManager);

		class Element
		{
		public:
			explicit Element (const ValueTree& state);
			~Element();

			const Identifier getType() const noexcept   { return state.getType(); }
			int getNumControlPoints() const noexcept;

			RelativePoint getControlPoint (int index) const;
			Value getControlPointValue (int index, UndoManager*);
			RelativePoint getStartPoint() const;
			RelativePoint getEndPoint() const;
			void setControlPoint (int index, const RelativePoint& point, UndoManager*);
			float getLength (Expression::Scope*) const;

			ValueTreeWrapper getParent() const;
			Element getPreviousElement() const;

			String getModeOfEndPoint() const;
			void setModeOfEndPoint (const String& newMode, UndoManager*);

			void convertToLine (UndoManager*);
			void convertToCubic (Expression::Scope*, UndoManager*);
			void convertToPathBreak (UndoManager* undoManager);
			ValueTree insertPoint (const Point<float>& targetPoint, Expression::Scope*, UndoManager*);
			void removePoint (UndoManager* undoManager);
			float findProportionAlongLine (const Point<float>& targetPoint, Expression::Scope*) const;

			static const Identifier mode, startSubPathElement, closeSubPathElement,
									lineToElement, quadraticToElement, cubicToElement;
			static const char* cornerMode;
			static const char* roundedMode;
			static const char* symmetricMode;

			ValueTree state;
		};

		ValueTree getPathState();

		void readFrom (const RelativePointPath& path, UndoManager* undoManager);
		void writeTo (RelativePointPath& path) const;

		static const Identifier nonZeroWinding, point1, point2, point3;
	};

private:

	ScopedPointer<RelativePointPath> relativePath;

	class RelativePositioner;
	friend class RelativePositioner;
	void applyRelativePath (const RelativePointPath&, Expression::Scope*);

	DrawablePath& operator= (const DrawablePath&);
	JUCE_LEAK_DETECTOR (DrawablePath);
};

#endif   // __JUCE_DRAWABLEPATH_JUCEHEADER__

/*** End of inlined file: juce_DrawablePath.h ***/


#endif
#ifndef __JUCE_DRAWABLERECTANGLE_JUCEHEADER__

/*** Start of inlined file: juce_DrawableRectangle.h ***/
#ifndef __JUCE_DRAWABLERECTANGLE_JUCEHEADER__
#define __JUCE_DRAWABLERECTANGLE_JUCEHEADER__

/**
	A Drawable object which draws a rectangle.

	For details on how to change the fill and stroke, see the DrawableShape class.

	@see Drawable, DrawableShape
*/
class JUCE_API  DrawableRectangle  : public DrawableShape
{
public:

	DrawableRectangle();
	DrawableRectangle (const DrawableRectangle& other);

	/** Destructor. */
	~DrawableRectangle();

	/** Sets the rectangle's bounds. */
	void setRectangle (const RelativeParallelogram& newBounds);

	/** Returns the rectangle's bounds. */
	const RelativeParallelogram& getRectangle() const noexcept          { return bounds; }

	/** Returns the corner size to be used. */
	const RelativePoint& getCornerSize() const noexcept                 { return cornerSize; }

	/** Sets a new corner size for the rectangle */
	void setCornerSize (const RelativePoint& newSize);

	/** @internal */
	Drawable* createCopy() const;
	/** @internal */
	void refreshFromValueTree (const ValueTree& tree, ComponentBuilder& builder);
	/** @internal */
	ValueTree createValueTree (ComponentBuilder::ImageProvider* imageProvider) const;
	/** @internal */
	static const Identifier valueTreeType;

	/** Internally-used class for wrapping a DrawableRectangle's state into a ValueTree. */
	class ValueTreeWrapper   : public DrawableShape::FillAndStrokeState
	{
	public:
		ValueTreeWrapper (const ValueTree& state);

		RelativeParallelogram getRectangle() const;
		void setRectangle (const RelativeParallelogram& newBounds, UndoManager*);

		void setCornerSize (const RelativePoint& cornerSize, UndoManager*);
		RelativePoint getCornerSize() const;
		Value getCornerSizeValue (UndoManager*);

		static const Identifier topLeft, topRight, bottomLeft, cornerSize;
	};

private:
	friend class Drawable::Positioner<DrawableRectangle>;

	RelativeParallelogram bounds;
	RelativePoint cornerSize;

	void rebuildPath();
	bool registerCoordinates (RelativeCoordinatePositionerBase&);
	void recalculateCoordinates (Expression::Scope*);

	DrawableRectangle& operator= (const DrawableRectangle&);
	JUCE_LEAK_DETECTOR (DrawableRectangle);
};

#endif   // __JUCE_DRAWABLERECTANGLE_JUCEHEADER__

/*** End of inlined file: juce_DrawableRectangle.h ***/


#endif
#ifndef __JUCE_DRAWABLESHAPE_JUCEHEADER__

#endif
#ifndef __JUCE_DRAWABLETEXT_JUCEHEADER__

/*** Start of inlined file: juce_DrawableText.h ***/
#ifndef __JUCE_DRAWABLETEXT_JUCEHEADER__
#define __JUCE_DRAWABLETEXT_JUCEHEADER__

/**
	A drawable object which renders a line of text.

	@see Drawable
*/
class JUCE_API  DrawableText  : public Drawable
{
public:

	/** Creates a DrawableText object. */
	DrawableText();
	DrawableText (const DrawableText& other);

	/** Destructor. */
	~DrawableText();

	/** Sets the text to display.*/
	void setText (const String& newText);

	/** Returns the currently displayed text */
	const String& getText() const noexcept                              { return text;}

	/** Sets the colour of the text. */
	void setColour (const Colour& newColour);

	/** Returns the current text colour. */
	const Colour& getColour() const noexcept                            { return colour; }

	/** Sets the font to use.
		Note that the font height and horizontal scale are set as RelativeCoordinates using
		setFontHeight and setFontHorizontalScale. If applySizeAndScale is true, then these height
		and scale values will be changed to match the dimensions of the font supplied;
		if it is false, then the new font object's height and scale are ignored.
	*/
	void setFont (const Font& newFont, bool applySizeAndScale);

	/** Returns the current font. */
	const Font& getFont() const noexcept                                { return font; }

	/** Changes the justification of the text within the bounding box. */
	void setJustification (const Justification& newJustification);

	/** Returns the current justification. */
	const Justification& getJustification() const noexcept              { return justification; }

	/** Returns the parallelogram that defines the text bounding box. */
	const RelativeParallelogram& getBoundingBox() const noexcept        { return bounds; }

	/** Sets the bounding box that contains the text. */
	void setBoundingBox (const RelativeParallelogram& newBounds);

	const RelativeCoordinate& getFontHeight() const                     { return fontHeight; }
	void setFontHeight (const RelativeCoordinate& newHeight);

	const RelativeCoordinate& getFontHorizontalScale() const            { return fontHScale; }
	void setFontHorizontalScale (const RelativeCoordinate& newScale);

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	Drawable* createCopy() const;
	/** @internal */
	void refreshFromValueTree (const ValueTree& tree, ComponentBuilder& builder);
	/** @internal */
	ValueTree createValueTree (ComponentBuilder::ImageProvider* imageProvider) const;
	/** @internal */
	static const Identifier valueTreeType;
	/** @internal */
	Rectangle<float> getDrawableBounds() const;

	/** Internally-used class for wrapping a DrawableText's state into a ValueTree. */
	class ValueTreeWrapper   : public Drawable::ValueTreeWrapperBase
	{
	public:
		ValueTreeWrapper (const ValueTree& state);

		String getText() const;
		void setText (const String& newText, UndoManager* undoManager);
		Value getTextValue (UndoManager* undoManager);

		Colour getColour() const;
		void setColour (const Colour& newColour, UndoManager* undoManager);

		Justification getJustification() const;
		void setJustification (const Justification& newJustification, UndoManager* undoManager);

		Font getFont() const;
		void setFont (const Font& newFont, UndoManager* undoManager);
		Value getFontValue (UndoManager* undoManager);

		RelativeParallelogram getBoundingBox() const;
		void setBoundingBox (const RelativeParallelogram& newBounds, UndoManager* undoManager);

		RelativeCoordinate getFontHeight() const;
		void setFontHeight (const RelativeCoordinate& newHeight, UndoManager* undoManager);

		RelativeCoordinate getFontHorizontalScale() const;
		void setFontHorizontalScale (const RelativeCoordinate& newScale, UndoManager* undoManager);

		static const Identifier text, colour, font, justification, topLeft, topRight, bottomLeft, fontHeight, fontHScale;
	};

private:

	RelativeParallelogram bounds;
	RelativeCoordinate fontHeight, fontHScale;
	Point<float> resolvedPoints[3];
	Font font, scaledFont;
	String text;
	Colour colour;
	Justification justification;

	friend class Drawable::Positioner<DrawableText>;
	bool registerCoordinates (RelativeCoordinatePositionerBase&);
	void recalculateCoordinates (Expression::Scope*);
	void refreshBounds();
	const AffineTransform getArrangementAndTransform (GlyphArrangement& glyphs) const;

	DrawableText& operator= (const DrawableText&);
	JUCE_LEAK_DETECTOR (DrawableText);
};

#endif   // __JUCE_DRAWABLETEXT_JUCEHEADER__

/*** End of inlined file: juce_DrawableText.h ***/


#endif
#ifndef __JUCE_DIRECTORYCONTENTSDISPLAYCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_DirectoryContentsDisplayComponent.h ***/
#ifndef __JUCE_DIRECTORYCONTENTSDISPLAYCOMPONENT_JUCEHEADER__
#define __JUCE_DIRECTORYCONTENTSDISPLAYCOMPONENT_JUCEHEADER__


/*** Start of inlined file: juce_DirectoryContentsList.h ***/
#ifndef __JUCE_DIRECTORYCONTENTSLIST_JUCEHEADER__
#define __JUCE_DIRECTORYCONTENTSLIST_JUCEHEADER__


/*** Start of inlined file: juce_FileFilter.h ***/
#ifndef __JUCE_FILEFILTER_JUCEHEADER__
#define __JUCE_FILEFILTER_JUCEHEADER__

/**
	Interface for deciding which files are suitable for something.

	For example, this is used by DirectoryContentsList to select which files
	go into the list.

	@see WildcardFileFilter, DirectoryContentsList, FileListComponent, FileBrowserComponent
*/
class JUCE_API  FileFilter
{
public:

	/** Creates a filter with the given description.

		The description can be returned later with the getDescription() method.
	*/
	FileFilter (const String& filterDescription);

	/** Destructor. */
	virtual ~FileFilter();

	/** Returns the description that the filter was created with. */
	const String& getDescription() const noexcept;

	/** Should return true if this file is suitable for inclusion in whatever context
		the object is being used.
	*/
	virtual bool isFileSuitable (const File& file) const = 0;

	/** Should return true if this directory is suitable for inclusion in whatever context
		the object is being used.
	*/
	virtual bool isDirectorySuitable (const File& file) const = 0;

protected:

	String description;
};

#endif   // __JUCE_FILEFILTER_JUCEHEADER__

/*** End of inlined file: juce_FileFilter.h ***/

/**
	A class to asynchronously scan for details about the files in a directory.

	This keeps a list of files and some information about them, using a background
	thread to scan for more files. As files are found, it broadcasts change messages
	to tell any listeners.

	@see FileListComponent, FileBrowserComponent
*/
class JUCE_API  DirectoryContentsList   : public ChangeBroadcaster,
										  public TimeSliceClient
{
public:

	/** Creates a directory list.

		To set the directory it should point to, use setDirectory(), which will
		also start it scanning for files on the background thread.

		When the background thread finds and adds new files to this list, the
		ChangeBroadcaster class will send a change message, so you can register
		listeners and update them when the list changes.

		@param fileFilter       an optional filter to select which files are
								included in the list. If this is 0, then all files
								and directories are included. Make sure that the
								filter doesn't get deleted during the lifetime of this
								object
		@param threadToUse      a thread object that this list can use
								to scan for files as a background task. Make sure
								that the thread you give it has been started, or you
								won't get any files!
	*/
	DirectoryContentsList (const FileFilter* fileFilter,
						   TimeSliceThread& threadToUse);

	/** Destructor. */
	~DirectoryContentsList();

	/** Sets the directory to look in for files.

		If the directory that's passed in is different to the current one, this will
		also start the background thread scanning it for files.
	*/
	void setDirectory (const File& directory,
					   bool includeDirectories,
					   bool includeFiles);

	/** Returns the directory that's currently being used. */
	const File& getDirectory() const;

	/** Clears the list, and stops the thread scanning for files. */
	void clear();

	/** Clears the list and restarts scanning the directory for files. */
	void refresh();

	/** True if the background thread hasn't yet finished scanning for files. */
	bool isStillLoading() const;

	/** Tells the list whether or not to ignore hidden files.

		By default these are ignored.
	*/
	void setIgnoresHiddenFiles (bool shouldIgnoreHiddenFiles);

	/** Returns true if hidden files are ignored.
		@see setIgnoresHiddenFiles
	*/
	bool ignoresHiddenFiles() const;

	/** Contains cached information about one of the files in a DirectoryContentsList.
	*/
	struct FileInfo
	{

		/** The filename.

			This isn't a full pathname, it's just the last part of the path, same as you'd
			get from File::getFileName().

			To get the full pathname, use DirectoryContentsList::getDirectory().getChildFile (filename).
		*/
		String filename;

		/** File size in bytes. */
		int64 fileSize;

		/** File modification time.

			As supplied by File::getLastModificationTime().
		*/
		Time modificationTime;

		/** File creation time.

			As supplied by File::getCreationTime().
		*/
		Time creationTime;

		/** True if the file is a directory. */
		bool isDirectory;

		/** True if the file is read-only. */
		bool isReadOnly;
	};

	/** Returns the number of files currently available in the list.

		The info about one of these files can be retrieved with getFileInfo() or
		getFile().

		Obviously as the background thread runs and scans the directory for files, this
		number will change.

		@see getFileInfo, getFile
	*/
	int getNumFiles() const;

	/** Returns the cached information about one of the files in the list.

		If the index is in-range, this will return true and will copy the file's details
		to the structure that is passed-in.

		If it returns false, then the index wasn't in range, and the structure won't
		be affected.

		@see getNumFiles, getFile
	*/
	bool getFileInfo (int index, FileInfo& resultInfo) const;

	/** Returns one of the files in the list.

		@param index    should be less than getNumFiles(). If this is out-of-range, the
						return value will be File::nonexistent
		@see getNumFiles, getFileInfo
	*/
	File getFile (int index) const;

	/** Returns the file filter being used.

		The filter is specified in the constructor.
	*/
	const FileFilter* getFilter() const                     { return fileFilter; }

	/** Returns true if the list contains the specified file. */
	bool contains (const File&) const;

	/** @internal */
	int useTimeSlice();
	/** @internal */
	TimeSliceThread& getTimeSliceThread()                   { return thread; }
	/** @internal */
	static int compareElements (const DirectoryContentsList::FileInfo* first,
								const DirectoryContentsList::FileInfo* second);

private:
	File root;
	const FileFilter* fileFilter;
	TimeSliceThread& thread;
	int fileTypeFlags;

	CriticalSection fileListLock;
	OwnedArray <FileInfo> files;

	ScopedPointer <DirectoryIterator> fileFindHandle;
	bool volatile shouldStop;

	void stopSearching();
	void changed();
	bool checkNextFile (bool& hasChanged);
	bool addFile (const File& file, bool isDir,
				  const int64 fileSize, const Time& modTime,
				  const Time& creationTime, bool isReadOnly);
	void setTypeFlags (int newFlags);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectoryContentsList);
};

#endif   // __JUCE_DIRECTORYCONTENTSLIST_JUCEHEADER__

/*** End of inlined file: juce_DirectoryContentsList.h ***/


/*** Start of inlined file: juce_FileBrowserListener.h ***/
#ifndef __JUCE_FILEBROWSERLISTENER_JUCEHEADER__
#define __JUCE_FILEBROWSERLISTENER_JUCEHEADER__

/**
	A listener for user selection events in a file browser.

	This is used by a FileBrowserComponent or FileListComponent.
*/
class JUCE_API  FileBrowserListener
{
public:

	/** Destructor. */
	virtual ~FileBrowserListener();

	/** Callback when the user selects a different file in the browser. */
	virtual void selectionChanged() = 0;

	/** Callback when the user clicks on a file in the browser. */
	virtual void fileClicked (const File& file, const MouseEvent& e) = 0;

	/** Callback when the user double-clicks on a file in the browser. */
	virtual void fileDoubleClicked (const File& file) = 0;

	/** Callback when the browser's root folder changes. */
	virtual void browserRootChanged (const File& newRoot) = 0;
};

#endif   // __JUCE_FILEBROWSERLISTENER_JUCEHEADER__

/*** End of inlined file: juce_FileBrowserListener.h ***/

/**
	A base class for components that display a list of the files in a directory.

	@see DirectoryContentsList
*/
class JUCE_API  DirectoryContentsDisplayComponent
{
public:

	/** Creates a DirectoryContentsDisplayComponent for a given list of files. */
	DirectoryContentsDisplayComponent (DirectoryContentsList& listToShow);

	/** Destructor. */
	virtual ~DirectoryContentsDisplayComponent();

	/** Returns the number of files the user has got selected.
		@see getSelectedFile
	*/
	virtual int getNumSelectedFiles() const = 0;

	/** Returns one of the files that the user has currently selected.
		The index should be in the range 0 to (getNumSelectedFiles() - 1).
		@see getNumSelectedFiles
	*/
	virtual File getSelectedFile (int index) const = 0;

	/** Deselects any selected files. */
	virtual void deselectAllFiles() = 0;

	/** Scrolls this view to the top. */
	virtual void scrollToTop() = 0;

	/** If the specified file is in the list, it will become the only selected item
		(and if the file isn't in the list, all other items will be deselected). */
	virtual void setSelectedFile (const File&) = 0;

	/** Adds a listener to be told when files are selected or clicked.
		@see removeListener
	*/
	void addListener (FileBrowserListener* listener);

	/** Removes a listener.
		@see addListener
	*/
	void removeListener (FileBrowserListener* listener);

	/** A set of colour IDs to use to change the colour of various aspects of the list.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		highlightColourId      = 0x1000540, /**< The colour to use to fill a highlighted row of the list. */
		textColourId           = 0x1000541, /**< The colour for the text. */
	};

	/** @internal */
	void sendSelectionChangeMessage();
	/** @internal */
	void sendDoubleClickMessage (const File& file);
	/** @internal */
	void sendMouseClickMessage (const File& file, const MouseEvent& e);

protected:

	DirectoryContentsList& fileList;
	ListenerList <FileBrowserListener> listeners;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectoryContentsDisplayComponent);
};

#endif   // __JUCE_DIRECTORYCONTENTSDISPLAYCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_DirectoryContentsDisplayComponent.h ***/


#endif
#ifndef __JUCE_DIRECTORYCONTENTSLIST_JUCEHEADER__

#endif
#ifndef __JUCE_FILEBROWSERCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_FileBrowserComponent.h ***/
#ifndef __JUCE_FILEBROWSERCOMPONENT_JUCEHEADER__
#define __JUCE_FILEBROWSERCOMPONENT_JUCEHEADER__


/*** Start of inlined file: juce_FilePreviewComponent.h ***/
#ifndef __JUCE_FILEPREVIEWCOMPONENT_JUCEHEADER__
#define __JUCE_FILEPREVIEWCOMPONENT_JUCEHEADER__

/**
	Base class for components that live inside a file chooser dialog box and
	show previews of the files that get selected.

	One of these allows special extra information to be displayed for files
	in a dialog box as the user selects them. Each time the current file or
	directory is changed, the selectedFileChanged() method will be called
	to allow it to update itself appropriately.

	@see FileChooser, ImagePreviewComponent
*/
class JUCE_API  FilePreviewComponent  : public Component
{
public:

	/** Creates a FilePreviewComponent. */
	FilePreviewComponent();

	/** Destructor. */
	~FilePreviewComponent();

	/** Called to indicate that the user's currently selected file has changed.

		@param newSelectedFile  the newly selected file or directory, which may be
								File::nonexistent if none is selected.
	*/
	virtual void selectedFileChanged (const File& newSelectedFile) = 0;

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilePreviewComponent);
};

#endif   // __JUCE_FILEPREVIEWCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_FilePreviewComponent.h ***/


/*** Start of inlined file: juce_TextEditor.h ***/
#ifndef __JUCE_TEXTEDITOR_JUCEHEADER__
#define __JUCE_TEXTEDITOR_JUCEHEADER__


/*** Start of inlined file: juce_Viewport.h ***/
#ifndef __JUCE_VIEWPORT_JUCEHEADER__
#define __JUCE_VIEWPORT_JUCEHEADER__


/*** Start of inlined file: juce_ScrollBar.h ***/
#ifndef __JUCE_SCROLLBAR_JUCEHEADER__
#define __JUCE_SCROLLBAR_JUCEHEADER__

/**
	A scrollbar component.

	To use a scrollbar, set up its total range using the setRangeLimits() method - this
	sets the range of values it can represent. Then you can use setCurrentRange() to
	change the position and size of the scrollbar's 'thumb'.

	Registering a ScrollBar::Listener with the scrollbar will allow you to find out when
	the user moves it, and you can use the getCurrentRangeStart() to find out where
	they moved it to.

	The scrollbar will adjust its own visibility according to whether its thumb size
	allows it to actually be scrolled.

	For most purposes, it's probably easier to use a ViewportContainer or ListBox
	instead of handling a scrollbar directly.

	@see ScrollBar::Listener
*/
class JUCE_API  ScrollBar  : public Component,
							 public AsyncUpdater,
							 private Timer
{
public:

	/** Creates a Scrollbar.

		@param isVertical           whether it should be a vertical or horizontal bar
		@param buttonsAreVisible    whether to show the up/down or left/right buttons
	*/
	ScrollBar (bool isVertical,
			   bool buttonsAreVisible = true);

	/** Destructor. */
	~ScrollBar();

	/** Returns true if the scrollbar is vertical, false if it's horizontal. */
	bool isVertical() const noexcept                                { return vertical; }

	/** Changes the scrollbar's direction.

		You'll also need to resize the bar appropriately - this just changes its internal
		layout.

		@param shouldBeVertical     true makes it vertical; false makes it horizontal.
	*/
	void setOrientation (bool shouldBeVertical);

	/** Shows or hides the scrollbar's buttons. */
	void setButtonVisibility (bool buttonsAreVisible);

	/** Tells the scrollbar whether to make itself invisible when not needed.

		The default behaviour is for a scrollbar to become invisible when the thumb
		fills the whole of its range (i.e. when it can't be moved). Setting this
		value to false forces the bar to always be visible.
		@see autoHides()
	*/
	void setAutoHide (bool shouldHideWhenFullRange);

	/** Returns true if this scrollbar is set to auto-hide when its thumb is as big
		as its maximum range.
		@see setAutoHide
	*/
	bool autoHides() const noexcept;

	/** Sets the minimum and maximum values that the bar will move between.

		The bar's thumb will always be constrained so that the entire thumb lies
		within this range.

		@see setCurrentRange
	*/
	void setRangeLimits (const Range<double>& newRangeLimit);

	/** Sets the minimum and maximum values that the bar will move between.

		The bar's thumb will always be constrained so that the entire thumb lies
		within this range.

		@see setCurrentRange
	*/
	void setRangeLimits (double minimum, double maximum);

	/** Returns the current limits on the thumb position.
		@see setRangeLimits
	*/
	const Range<double> getRangeLimit() const noexcept              { return totalRange; }

	/** Returns the lower value that the thumb can be set to.

		This is the value set by setRangeLimits().
	*/
	double getMinimumRangeLimit() const noexcept                    { return totalRange.getStart(); }

	/** Returns the upper value that the thumb can be set to.

		This is the value set by setRangeLimits().
	*/
	double getMaximumRangeLimit() const noexcept                    { return totalRange.getEnd(); }

	/** Changes the position of the scrollbar's 'thumb'.

		If this method call actually changes the scrollbar's position, it will trigger an
		asynchronous call to ScrollBar::Listener::scrollBarMoved() for all the listeners that
		are registered.

		@see getCurrentRange. setCurrentRangeStart
	*/
	void setCurrentRange (const Range<double>& newRange);

	/** Changes the position of the scrollbar's 'thumb'.

		This sets both the position and size of the thumb - to just set the position without
		changing the size, you can use setCurrentRangeStart().

		If this method call actually changes the scrollbar's position, it will trigger an
		asynchronous call to ScrollBar::Listener::scrollBarMoved() for all the listeners that
		are registered.

		@param newStart     the top (or left) of the thumb, in the range
							getMinimumRangeLimit() <= newStart <= getMaximumRangeLimit(). If the
							value is beyond these limits, it will be clipped.
		@param newSize      the size of the thumb, such that
							getMinimumRangeLimit() <= newStart + newSize <= getMaximumRangeLimit(). If the
							size is beyond these limits, it will be clipped.
		@see setCurrentRangeStart, getCurrentRangeStart, getCurrentRangeSize
	*/
	void setCurrentRange (double newStart, double newSize);

	/** Moves the bar's thumb position.

		This will move the thumb position without changing the thumb size. Note
		that the maximum thumb start position is (getMaximumRangeLimit() - getCurrentRangeSize()).

		If this method call actually changes the scrollbar's position, it will trigger an
		asynchronous call to ScrollBar::Listener::scrollBarMoved() for all the listeners that
		are registered.

		@see setCurrentRange
	*/
	void setCurrentRangeStart (double newStart);

	/** Returns the current thumb range.
		@see getCurrentRange, setCurrentRange
	*/
	const Range<double> getCurrentRange() const noexcept            { return visibleRange; }

	/** Returns the position of the top of the thumb.
		@see getCurrentRange, setCurrentRangeStart
	*/
	double getCurrentRangeStart() const noexcept                    { return visibleRange.getStart(); }

	/** Returns the current size of the thumb.
		@see getCurrentRange, setCurrentRange
	*/
	double getCurrentRangeSize() const noexcept                     { return visibleRange.getLength(); }

	/** Sets the amount by which the up and down buttons will move the bar.

		The value here is in terms of the total range, and is added or subtracted
		from the thumb position when the user clicks an up/down (or left/right) button.
	*/
	void setSingleStepSize (double newSingleStepSize);

	/** Moves the scrollbar by a number of single-steps.

		This will move the bar by a multiple of its single-step interval (as
		specified using the setSingleStepSize() method).

		A positive value here will move the bar down or to the right, a negative
		value moves it up or to the left.
	*/
	void moveScrollbarInSteps (int howManySteps);

	/** Moves the scroll bar up or down in pages.

		This will move the bar by a multiple of its current thumb size, effectively
		doing a page-up or down.

		A positive value here will move the bar down or to the right, a negative
		value moves it up or to the left.
	*/
	void moveScrollbarInPages (int howManyPages);

	/** Scrolls to the top (or left).

		This is the same as calling setCurrentRangeStart (getMinimumRangeLimit());
	*/
	void scrollToTop();

	/** Scrolls to the bottom (or right).

		This is the same as calling setCurrentRangeStart (getMaximumRangeLimit() - getCurrentRangeSize());
	*/
	void scrollToBottom();

	/** Changes the delay before the up and down buttons autorepeat when they are held
		down.

		For an explanation of what the parameters are for, see Button::setRepeatSpeed().

		@see Button::setRepeatSpeed
	*/
	void setButtonRepeatSpeed (int initialDelayInMillisecs,
							   int repeatDelayInMillisecs,
							   int minimumDelayInMillisecs = -1);

	/** A set of colour IDs to use to change the colour of various aspects of the component.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId          = 0x1000300,    /**< The background colour of the scrollbar. */
		thumbColourId               = 0x1000400,    /**< A base colour to use for the thumb. The look and feel will probably use variations on this colour. */
		trackColourId               = 0x1000401     /**< A base colour to use for the slot area of the bar. The look and feel will probably use variations on this colour. */
	};

	/**
		A class for receiving events from a ScrollBar.

		You can register a ScrollBar::Listener with a ScrollBar using the ScrollBar::addListener()
		method, and it will be called when the bar's position changes.

		@see ScrollBar::addListener, ScrollBar::removeListener
	*/
	class JUCE_API  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}

		/** Called when a ScrollBar is moved.

			@param scrollBarThatHasMoved    the bar that has moved
			@param newRangeStart            the new range start of this bar
		*/
		virtual void scrollBarMoved (ScrollBar* scrollBarThatHasMoved,
									 double newRangeStart) = 0;
	};

	/** Registers a listener that will be called when the scrollbar is moved. */
	void addListener (Listener* listener);

	/** Deregisters a previously-registered listener. */
	void removeListener (Listener* listener);

	/** @internal */
	bool keyPressed (const KeyPress& key);
	/** @internal */
	void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
	/** @internal */
	void lookAndFeelChanged();
	/** @internal */
	void handleAsyncUpdate();
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseUp   (const MouseEvent& e);
	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();

private:

	Range <double> totalRange, visibleRange;
	double singleStepSize, dragStartRange;
	int thumbAreaStart, thumbAreaSize, thumbStart, thumbSize;
	int dragStartMousePos, lastMousePos;
	int initialDelayInMillisecs, repeatDelayInMillisecs, minimumDelayInMillisecs;
	bool vertical, isDraggingThumb, autohides;
	class ScrollbarButton;
	friend class ScopedPointer<ScrollbarButton>;
	ScopedPointer<ScrollbarButton> upButton, downButton;
	ListenerList <Listener> listeners;

	void updateThumbPosition();
	void timerCallback();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScrollBar);
};

/** This typedef is just for compatibility with old code - newer code should use the ScrollBar::Listener class directly. */
typedef ScrollBar::Listener ScrollBarListener;

#endif   // __JUCE_SCROLLBAR_JUCEHEADER__

/*** End of inlined file: juce_ScrollBar.h ***/

/**
	A Viewport is used to contain a larger child component, and allows the child
	to be automatically scrolled around.

	To use a Viewport, just create one and set the component that goes inside it
	using the setViewedComponent() method. When the child component changes size,
	the Viewport will adjust its scrollbars accordingly.

	A subclass of the viewport can be created which will receive calls to its
	visibleAreaChanged() method when the subcomponent changes position or size.

*/
class JUCE_API  Viewport  : public Component,
							private ComponentListener,
							private ScrollBar::Listener
{
public:

	/** Creates a Viewport.

		The viewport is initially empty - use the setViewedComponent() method to
		add a child component for it to manage.
	*/
	explicit Viewport (const String& componentName = String::empty);

	/** Destructor. */
	~Viewport();

	/** Sets the component that this viewport will contain and scroll around.

		This will add the given component to this Viewport and position it at (0, 0).

		(Don't add or remove any child components directly using the normal
		Component::addChildComponent() methods).

		@param newViewedComponent   the component to add to this viewport, or null to remove
									the current component.
		@param deleteComponentWhenNoLongerNeeded    if true, the component will be deleted
									automatically when the viewport is deleted or when a different
									component is added. If false, the caller must manage the lifetime
									of the component
		@see getViewedComponent
	*/
	void setViewedComponent (Component* newViewedComponent,
							 bool deleteComponentWhenNoLongerNeeded = true);

	/** Returns the component that's currently being used inside the Viewport.

		@see setViewedComponent
	*/
	Component* getViewedComponent() const noexcept                  { return contentComp; }

	/** Changes the position of the viewed component.

		The inner component will be moved so that the pixel at the top left of
		the viewport will be the pixel at position (xPixelsOffset, yPixelsOffset)
		within the inner component.

		This will update the scrollbars and might cause a call to visibleAreaChanged().

		@see getViewPositionX, getViewPositionY, setViewPositionProportionately
	*/
	void setViewPosition (int xPixelsOffset, int yPixelsOffset);

	/** Changes the position of the viewed component.

		The inner component will be moved so that the pixel at the top left of
		the viewport will be the pixel at the specified coordinates within the
		inner component.

		This will update the scrollbars and might cause a call to visibleAreaChanged().

		@see getViewPositionX, getViewPositionY, setViewPositionProportionately
	*/
	void setViewPosition (const Point<int>& newPosition);

	/** Changes the view position as a proportion of the distance it can move.

		The values here are from 0.0 to 1.0 - where (0, 0) would put the
		visible area in the top-left, and (1, 1) would put it as far down and
		to the right as it's possible to go whilst keeping the child component
		on-screen.
	*/
	void setViewPositionProportionately (double proportionX, double proportionY);

	/** If the specified position is at the edges of the viewport, this method scrolls
		the viewport to bring that position nearer to the centre.

		Call this if you're dragging an object inside a viewport and want to make it scroll
		when the user approaches an edge. You might also find Component::beginDragAutoRepeat()
		useful when auto-scrolling.

		@param mouseX       the x position, relative to the Viewport's top-left
		@param mouseY       the y position, relative to the Viewport's top-left
		@param distanceFromEdge     specifies how close to an edge the position needs to be
							before the viewport should scroll in that direction
		@param maximumSpeed the maximum number of pixels that the viewport is allowed
							to scroll by.
		@returns            true if the viewport was scrolled
	*/
	bool autoScroll (int mouseX, int mouseY, int distanceFromEdge, int maximumSpeed);

	/** Returns the position within the child component of the top-left of its visible area.
	*/
	const Point<int>& getViewPosition() const noexcept      { return lastVisibleArea.getPosition(); }

	/** Returns the position within the child component of the top-left of its visible area.
		@see getViewWidth, setViewPosition
	*/
	int getViewPositionX() const noexcept                   { return lastVisibleArea.getX(); }

	/** Returns the position within the child component of the top-left of its visible area.
		@see getViewHeight, setViewPosition
	*/
	int getViewPositionY() const noexcept                   { return lastVisibleArea.getY(); }

	/** Returns the width of the visible area of the child component.

		This may be less than the width of this Viewport if there's a vertical scrollbar
		or if the child component is itself smaller.
	*/
	int getViewWidth() const noexcept                       { return lastVisibleArea.getWidth(); }

	/** Returns the height of the visible area of the child component.

		This may be less than the height of this Viewport if there's a horizontal scrollbar
		or if the child component is itself smaller.
	*/
	int getViewHeight() const noexcept                      { return lastVisibleArea.getHeight(); }

	/** Returns the width available within this component for the contents.

		This will be the width of the viewport component minus the width of a
		vertical scrollbar (if visible).
	*/
	int getMaximumVisibleWidth() const;

	/** Returns the height available within this component for the contents.

		This will be the height of the viewport component minus the space taken up
		by a horizontal scrollbar (if visible).
	*/
	int getMaximumVisibleHeight() const;

	/** Callback method that is called when the visible area changes.

		This will be called when the visible area is moved either be scrolling or
		by calls to setViewPosition(), etc.
	*/
	virtual void visibleAreaChanged (const Rectangle<int>& newVisibleArea);

	/** Callback method that is called when the viewed component is added, removed or swapped. */
	virtual void viewedComponentChanged (Component* newComponent);

	/** Turns scrollbars on or off.

		If set to false, the scrollbars won't ever appear. When true (the default)
		they will appear only when needed.
	*/
	void setScrollBarsShown (bool showVerticalScrollbarIfNeeded,
							 bool showHorizontalScrollbarIfNeeded);

	/** True if the vertical scrollbar is enabled.
		@see setScrollBarsShown
	*/
	bool isVerticalScrollBarShown() const noexcept              { return showVScrollbar; }

	/** True if the horizontal scrollbar is enabled.
		@see setScrollBarsShown
	*/
	bool isHorizontalScrollBarShown() const noexcept            { return showHScrollbar; }

	/** Changes the width of the scrollbars.

		If this isn't specified, the default width from the LookAndFeel class will be used.

		@see LookAndFeel::getDefaultScrollbarWidth
	*/
	void setScrollBarThickness (int thickness);

	/** Returns the thickness of the scrollbars.

		@see setScrollBarThickness
	*/
	int getScrollBarThickness() const;

	/** Changes the distance that a single-step click on a scrollbar button
		will move the viewport.
	*/
	void setSingleStepSizes (int stepX, int stepY);

	/** Shows or hides the buttons on any scrollbars that are used.

		@see ScrollBar::setButtonVisibility
	*/
	void setScrollBarButtonVisibility (bool buttonsVisible);

	/** Returns a pointer to the scrollbar component being used.
		Handy if you need to customise the bar somehow.
	*/
	ScrollBar* getVerticalScrollBar() noexcept                  { return &verticalScrollBar; }

	/** Returns a pointer to the scrollbar component being used.
		Handy if you need to customise the bar somehow.
	*/
	ScrollBar* getHorizontalScrollBar() noexcept                { return &horizontalScrollBar; }

	struct Ids
	{
		static const Identifier showScrollBarV, showScrollBarH, scrollBarWidth;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);

	/** @internal */
	void resized();
	/** @internal */
	void scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart);
	/** @internal */
	void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
	/** @internal */
	bool keyPressed (const KeyPress& key);
	/** @internal */
	void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);
	/** @internal */
	bool useMouseWheelMoveIfNeeded (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);

private:

	WeakReference<Component> contentComp;
	Rectangle<int> lastVisibleArea;
	int scrollBarThickness;
	int singleStepX, singleStepY;
	bool showHScrollbar, showVScrollbar, deleteContent;
	Component contentHolder;
	ScrollBar verticalScrollBar;
	ScrollBar horizontalScrollBar;
	Point<int> viewportPosToCompPos (const Point<int>&) const;

	void updateVisibleArea();
	void deleteContentComp();

   #if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// If you get an error here, it's because this method's parameters have changed! See the new definition above..
	virtual int visibleAreaChanged (int, int, int, int) { return 0; }
   #endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Viewport);
};

#endif   // __JUCE_VIEWPORT_JUCEHEADER__

/*** End of inlined file: juce_Viewport.h ***/


/*** Start of inlined file: juce_PopupMenu.h ***/
#ifndef __JUCE_POPUPMENU_JUCEHEADER__
#define __JUCE_POPUPMENU_JUCEHEADER__

/** Creates and displays a popup-menu.

	To show a popup-menu, you create one of these, add some items to it, then
	call its show() method, which returns the id of the item the user selects.

	E.g. @code
	void MyWidget::mouseDown (const MouseEvent& e)
	{
		PopupMenu m;
		m.addItem (1, "item 1");
		m.addItem (2, "item 2");

		const int result = m.show();

		if (result == 0)
		{
			// user dismissed the menu without picking anything
		}
		else if (result == 1)
		{
			// user picked item 1
		}
		else if (result == 2)
		{
			// user picked item 2
		}
	}
	@endcode

	Submenus are easy too: @code

	void MyWidget::mouseDown (const MouseEvent& e)
	{
		PopupMenu subMenu;
		subMenu.addItem (1, "item 1");
		subMenu.addItem (2, "item 2");

		PopupMenu mainMenu;
		mainMenu.addItem (3, "item 3");
		mainMenu.addSubMenu ("other choices", subMenu);

		const int result = m.show();

		...etc
	}
	@endcode
*/
class JUCE_API  PopupMenu
{
private:
	class Window;

public:

	/** Creates an empty popup menu. */
	PopupMenu();

	/** Creates a copy of another menu. */
	PopupMenu (const PopupMenu& other);

	/** Destructor. */
	~PopupMenu();

	/** Copies this menu from another one. */
	PopupMenu& operator= (const PopupMenu& other);

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	PopupMenu (PopupMenu&& other) noexcept;
	PopupMenu& operator= (PopupMenu&& other) noexcept;
   #endif

	/** Resets the menu, removing all its items. */
	void clear();

	/** Appends a new text item for this menu to show.

		@param itemResultId     the number that will be returned from the show() method
								if the user picks this item. The value should never be
								zero, because that's used to indicate that the user didn't
								select anything.
		@param itemText         the text to show.
		@param isEnabled        if false, the item will be shown 'greyed-out' and can't be picked
		@param isTicked         if true, the item will be shown with a tick next to it
		@param iconToUse        if this is non-zero, it should be an image that will be
								displayed to the left of the item. This method will take its
								own copy of the image passed-in, so there's no need to keep
								it hanging around.

		@see addSeparator, addColouredItem, addCustomItem, addSubMenu
	*/
	void addItem (int itemResultId,
				  const String& itemText,
				  bool isEnabled = true,
				  bool isTicked = false,
				  const Image& iconToUse = Image::null);

	/** Adds an item that represents one of the commands in a command manager object.

		@param commandManager       the manager to use to trigger the command and get information
									about it
		@param commandID            the ID of the command
		@param displayName          if this is non-empty, then this string will be used instead of
									the command's registered name
	*/
	void addCommandItem (ApplicationCommandManager* commandManager,
						 int commandID,
						 const String& displayName = String::empty);

	/** Appends a text item with a special colour.

		This is the same as addItem(), but specifies a colour to use for the
		text, which will override the default colours that are used by the
		current look-and-feel. See addItem() for a description of the parameters.
	*/
	void addColouredItem (int itemResultId,
						  const String& itemText,
						  const Colour& itemTextColour,
						  bool isEnabled = true,
						  bool isTicked = false,
						  const Image& iconToUse = Image::null);

	/** Appends a custom menu item that can't be used to trigger a result.

		This will add a user-defined component to use as a menu item. Unlike the
		addCustomItem() method that takes a PopupMenu::CustomComponent, this version
		can't trigger a result from it, so doesn't take a menu ID. It also doesn't
		delete the component when it's finished, so it's the caller's responsibility
		to manage the component that is passed-in.

		if triggerMenuItemAutomaticallyWhenClicked is true, the menu itself will handle
		detection of a mouse-click on your component, and use that to trigger the
		menu ID specified in itemResultId. If this is false, the menu item can't
		be triggered, so itemResultId is not used.

		@see CustomComponent
	*/
	void addCustomItem (int itemResultId,
						Component* customComponent,
						int idealWidth, int idealHeight,
						bool triggerMenuItemAutomaticallyWhenClicked);

	/** Appends a sub-menu.

		If the menu that's passed in is empty, it will appear as an inactive item.
	*/
	void addSubMenu (const String& subMenuName,
					 const PopupMenu& subMenu,
					 bool isEnabled = true,
					 const Image& iconToUse = Image::null,
					 bool isTicked = false);

	/** Appends a separator to the menu, to help break it up into sections.

		The menu class is smart enough not to display separators at the top or bottom
		of the menu, and it will replace mutliple adjacent separators with a single
		one, so your code can be quite free and easy about adding these, and it'll
		always look ok.
	*/
	void addSeparator();

	/** Adds a non-clickable text item to the menu.

		This is a bold-font items which can be used as a header to separate the items
		into named groups.
	*/
	void addSectionHeader (const String& title);

	/** Returns the number of items that the menu currently contains.

		(This doesn't count separators).
	*/
	int getNumItems() const noexcept;

	/** Returns true if the menu contains a command item that triggers the given command. */
	bool containsCommandItem (int commandID) const;

	/** Returns true if the menu contains any items that can be used. */
	bool containsAnyActiveItems() const noexcept;

	/** Class used to create a set of options to pass to the show() method.
		You can chain together a series of calls to this class's methods to create
		a set of whatever options you want to specify.
		E.g. @code
		PopupMenu menu;
		...
		menu.showMenu (PopupMenu::Options().withMaximumWidth (100),
										   .withMaximumNumColumns (3)
										   .withTargetComponent (myComp));
		@endcode
	*/
	class JUCE_API  Options
	{
	public:
		Options();

		Options withTargetComponent (Component* targetComponent) const noexcept;
		Options withTargetScreenArea (const Rectangle<int>& targetArea) const noexcept;
		Options withMinimumWidth (int minWidth) const noexcept;
		Options withMaximumNumColumns (int maxNumColumns) const noexcept;
		Options withStandardItemHeight (int standardHeight) const noexcept;
		Options withItemThatMustBeVisible (int idOfItemToBeVisible) const noexcept;

	private:
		friend class PopupMenu;
		friend class PopupMenu::Window;
		Rectangle<int> targetArea;
		Component* targetComponent;
		int visibleItemID, minWidth, maxColumns, standardHeight;
	};

   #if JUCE_MODAL_LOOPS_PERMITTED
	/** Displays the menu and waits for the user to pick something.

		This will display the menu modally, and return the ID of the item that the
		user picks. If they click somewhere off the menu to get rid of it without
		choosing anything, this will return 0.

		The current location of the mouse will be used as the position to show the
		menu - to explicitly set the menu's position, use showAt() instead. Depending
		on where this point is on the screen, the menu will appear above, below or
		to the side of the point.

		@param itemIdThatMustBeVisible  if you set this to the ID of one of the menu items,
										then when the menu first appears, it will make sure
										that this item is visible. So if the menu has too many
										items to fit on the screen, it will be scrolled to a
										position where this item is visible.
		@param minimumWidth             a minimum width for the menu, in pixels. It may be wider
										than this if some items are too long to fit.
		@param maximumNumColumns        if there are too many items to fit on-screen in a single
										vertical column, the menu may be laid out as a series of
										columns - this is the maximum number allowed. To use the
										default value for this (probably about 7), you can pass
										in zero.
		@param standardItemHeight       if this is non-zero, it will be used as the standard
										height for menu items (apart from custom items)
		@param callback                 if this is non-zero, the menu will be launched asynchronously,
										returning immediately, and the callback will receive a
										call when the menu is either dismissed or has an item
										selected. This object will be owned and deleted by the
										system, so make sure that it works safely and that any
										pointers that it uses are safely within scope.
		@see showAt
	*/
	int show (int itemIdThatMustBeVisible = 0,
			  int minimumWidth = 0,
			  int maximumNumColumns = 0,
			  int standardItemHeight = 0,
			  ModalComponentManager::Callback* callback = nullptr);

	/** Displays the menu at a specific location.

		This is the same as show(), but uses a specific location (in global screen
		co-ordinates) rather than the current mouse position.

		The screenAreaToAttachTo parameter indicates a screen area to which the menu
		will be adjacent. Depending on where this is, the menu will decide which edge to
		attach itself to, in order to fit itself fully on-screen. If you just want to
		trigger a menu at a specific point, you can pass in a rectangle of size (0, 0)
		with the position that you want.

		@see show()
	*/
	int showAt (const Rectangle<int>& screenAreaToAttachTo,
				int itemIdThatMustBeVisible = 0,
				int minimumWidth = 0,
				int maximumNumColumns = 0,
				int standardItemHeight = 0,
				ModalComponentManager::Callback* callback = nullptr);

	/** Displays the menu as if it's attached to a component such as a button.

		This is similar to showAt(), but will position it next to the given component, e.g.
		so that the menu's edge is aligned with that of the component. This is intended for
		things like buttons that trigger a pop-up menu.
	*/
	int showAt (Component* componentToAttachTo,
				int itemIdThatMustBeVisible = 0,
				int minimumWidth = 0,
				int maximumNumColumns = 0,
				int standardItemHeight = 0,
				ModalComponentManager::Callback* callback = nullptr);

	/** Displays and runs the menu modally, with a set of options.
	*/
	int showMenu (const Options& options);
   #endif

	/** Runs the menu asynchronously, with a user-provided callback that will receive the result. */
	void showMenuAsync (const Options& options,
						ModalComponentManager::Callback* callback);

	/** Closes any menus that are currently open.

		This might be useful if you have a situation where your window is being closed
		by some means other than a user action, and you'd like to make sure that menus
		aren't left hanging around.
	*/
	static bool JUCE_CALLTYPE dismissAllActiveMenus();

	/** Specifies a look-and-feel for the menu and any sub-menus that it has.

		This can be called before show() if you need a customised menu. Be careful
		not to delete the LookAndFeel object before the menu has been deleted.
	*/
	void setLookAndFeel (LookAndFeel* newLookAndFeel);

	/** A set of colour IDs to use to change the colour of various aspects of the menu.

		These constants can be used either via the LookAndFeel::setColour()
		method for the look and feel that is set for this menu with setLookAndFeel()

		@see setLookAndFeel, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId             = 0x1000700,  /**< The colour to fill the menu's background with. */
		textColourId                   = 0x1000600,  /**< The colour for normal menu item text, (unless the
														  colour is specified when the item is added). */
		headerTextColourId             = 0x1000601,  /**< The colour for section header item text (see the
														  addSectionHeader() method). */
		highlightedBackgroundColourId  = 0x1000900,  /**< The colour to fill the background of the currently
														  highlighted menu item. */
		highlightedTextColourId        = 0x1000800,  /**< The colour to use for the text of the currently
														  highlighted item. */
	};

	/**
		Allows you to iterate through the items in a pop-up menu, and examine
		their properties.

		To use this, just create one and repeatedly call its next() method. When this
		returns true, all the member variables of the iterator are filled-out with
		information describing the menu item. When it returns false, the end of the
		list has been reached.
	*/
	class JUCE_API  MenuItemIterator
	{
	public:

		/** Creates an iterator that will scan through the items in the specified
			menu.

			Be careful not to add any items to a menu while it is being iterated,
			or things could get out of step.
		*/
		MenuItemIterator (const PopupMenu& menu);

		/** Destructor. */
		~MenuItemIterator();

		/** Returns true if there is another item, and sets up all this object's
			member variables to reflect that item's properties.
		*/
		bool next();

		String itemName;
		const PopupMenu* subMenu;
		int itemId;
		bool isSeparator;
		bool isTicked;
		bool isEnabled;
		bool isCustomComponent;
		bool isSectionHeader;
		const Colour* customColour;
		Image customImage;
		ApplicationCommandManager* commandManager;

	private:

		const PopupMenu& menu;
		int index;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuItemIterator);
	};

	/** A user-defined copmonent that can be used as an item in a popup menu.
		@see PopupMenu::addCustomItem
	*/
	class JUCE_API  CustomComponent  : public Component,
									   public SingleThreadedReferenceCountedObject
	{
	public:
		/** Creates a custom item.
			If isTriggeredAutomatically is true, then the menu will automatically detect
			a mouse-click on this component and use that to invoke the menu item. If it's
			false, then it's up to your class to manually trigger the item when it wants to.
		*/
		CustomComponent (bool isTriggeredAutomatically = true);

		/** Destructor. */
		~CustomComponent();

		/** Returns a rectangle with the size that this component would like to have.

			Note that the size which this method returns isn't necessarily the one that
			the menu will give it, as the items will be stretched to have a uniform width.
		*/
		virtual void getIdealSize (int& idealWidth, int& idealHeight) = 0;

		/** Dismisses the menu, indicating that this item has been chosen.

			This will cause the menu to exit from its modal state, returning
			this item's id as the result.
		*/
		void triggerMenuItem();

		/** Returns true if this item should be highlighted because the mouse is over it.
			You can call this method in your paint() method to find out whether
			to draw a highlight.
		*/
		bool isItemHighlighted() const noexcept                 { return isHighlighted; }

		/** @internal */
		bool isTriggeredAutomatically() const noexcept          { return triggeredAutomatically; }
		/** @internal */
		void setHighlighted (bool shouldBeHighlighted);

	private:

		bool isHighlighted, triggeredAutomatically;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomComponent);
	};

	/** Appends a custom menu item.

		This will add a user-defined component to use as a menu item. The component
		passed in will be deleted by this menu when it's no longer needed.

		@see CustomComponent
	*/
	void addCustomItem (int itemResultId, CustomComponent* customComponent);

private:

	class Item;
	class ItemComponent;

	friend class MenuItemIterator;
	friend class ItemComponent;
	friend class Window;
	friend class CustomComponent;
	friend class MenuBarComponent;
	friend class OwnedArray <Item>;
	friend class OwnedArray <ItemComponent>;
	friend class ScopedPointer <Window>;

	OwnedArray <Item> items;
	LookAndFeel* lookAndFeel;

	Component* createWindow (const Options&, ApplicationCommandManager**) const;
	int showWithOptionalCallback (const Options&, ModalComponentManager::Callback*, bool);

	JUCE_LEAK_DETECTOR (PopupMenu);
};

#endif   // __JUCE_POPUPMENU_JUCEHEADER__

/*** End of inlined file: juce_PopupMenu.h ***/

/**
	A component containing text that can be edited.

	A TextEditor can either be in single- or multi-line mode, and supports mixed
	fonts and colours.

	@see TextEditor::Listener, Label
*/
class JUCE_API  TextEditor  : public Component,
							  public TextInputTarget,
							  public SettableTooltipClient
{
public:

	/** Creates a new, empty text editor.

		@param componentName        the name to pass to the component for it to use as its name
		@param passwordCharacter    if this is not zero, this character will be used as a replacement
									for all characters that are drawn on screen - e.g. to create
									a password-style textbox containing circular blobs instead of text,
									you could set this value to 0x25cf, which is the unicode character
									for a black splodge (not all fonts include this, though), or 0x2022,
									which is a bullet (probably the best choice for linux).
	*/
	explicit TextEditor (const String& componentName = String::empty,
						 juce_wchar passwordCharacter = 0);

	/** Destructor. */
	virtual ~TextEditor();

	/** Puts the editor into either multi- or single-line mode.

		By default, the editor will be in single-line mode, so use this if you need a multi-line
		editor.

		See also the setReturnKeyStartsNewLine() method, which will also need to be turned
		on if you want a multi-line editor with line-breaks.

		@see isMultiLine, setReturnKeyStartsNewLine
	*/
	void setMultiLine (bool shouldBeMultiLine,
					   bool shouldWordWrap = true);

	/** Returns true if the editor is in multi-line mode.
	*/
	bool isMultiLine() const;

	/** Changes the behaviour of the return key.

		If set to true, the return key will insert a new-line into the text; if false
		it will trigger a call to the TextEditor::Listener::textEditorReturnKeyPressed()
		method. By default this is set to false, and when true it will only insert
		new-lines when in multi-line mode (see setMultiLine()).
	*/
	void setReturnKeyStartsNewLine (bool shouldStartNewLine);

	/** Returns the value set by setReturnKeyStartsNewLine().

		See setReturnKeyStartsNewLine() for more info.
	*/
	bool getReturnKeyStartsNewLine() const                      { return returnKeyStartsNewLine; }

	/** Indicates whether the tab key should be accepted and used to input a tab character,
		or whether it gets ignored.

		By default the tab key is ignored, so that it can be used to switch keyboard focus
		between components.
	*/
	void setTabKeyUsedAsCharacter (bool shouldTabKeyBeUsed);

	/** Returns true if the tab key is being used for input.
		@see setTabKeyUsedAsCharacter
	*/
	bool isTabKeyUsedAsCharacter() const                        { return tabKeyUsed; }

	/** Changes the editor to read-only mode.

		By default, the text editor is not read-only. If you're making it read-only, you
		might also want to call setCaretVisible (false) to get rid of the caret.

		The text can still be highlighted and copied when in read-only mode.

		@see isReadOnly, setCaretVisible
	*/
	void setReadOnly (bool shouldBeReadOnly);

	/** Returns true if the editor is in read-only mode.
	*/
	bool isReadOnly() const;

	/** Makes the caret visible or invisible.
		By default the caret is visible.
		@see setCaretColour, setCaretPosition
	*/
	void setCaretVisible (bool shouldBeVisible);

	/** Returns true if the caret is enabled.
		@see setCaretVisible
	*/
	bool isCaretVisible() const                                 { return caret != nullptr; }

	/** Enables/disables a vertical scrollbar.

		(This only applies when in multi-line mode). When the text gets too long to fit
		in the component, a scrollbar can appear to allow it to be scrolled. Even when
		this is enabled, the scrollbar will be hidden unless it's needed.

		By default the scrollbar is enabled.
	*/
	void setScrollbarsShown (bool shouldBeEnabled);

	/** Returns true if scrollbars are enabled.
		@see setScrollbarsShown
	*/
	bool areScrollbarsShown() const                                 { return scrollbarVisible; }

	/** Changes the password character used to disguise the text.

		@param passwordCharacter    if this is not zero, this character will be used as a replacement
									for all characters that are drawn on screen - e.g. to create
									a password-style textbox containing circular blobs instead of text,
									you could set this value to 0x25cf, which is the unicode character
									for a black splodge (not all fonts include this, though), or 0x2022,
									which is a bullet (probably the best choice for linux).
	*/
	void setPasswordCharacter (juce_wchar passwordCharacter);

	/** Returns the current password character.
		@see setPasswordCharacter
	*/
	juce_wchar getPasswordCharacter() const                         { return passwordCharacter; }

	/** Allows a right-click menu to appear for the editor.

		(This defaults to being enabled).

		If enabled, right-clicking (or command-clicking on the Mac) will pop up a menu
		of options such as cut/copy/paste, undo/redo, etc.
	*/
	void setPopupMenuEnabled (bool menuEnabled);

	/** Returns true if the right-click menu is enabled.
		@see setPopupMenuEnabled
	*/
	bool isPopupMenuEnabled() const                                 { return popupMenuEnabled; }

	/** Returns true if a popup-menu is currently being displayed.
	*/
	bool isPopupMenuCurrentlyActive() const                         { return menuActive; }

	/** A set of colour IDs to use to change the colour of various aspects of the editor.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId       = 0x1000200, /**< The colour to use for the text component's background - this can be
												   transparent if necessary. */

		textColourId             = 0x1000201, /**< The colour that will be used when text is added to the editor. Note
												   that because the editor can contain multiple colours, calling this
												   method won't change the colour of existing text - to do that, call
												   applyFontToAllText() after calling this method.*/

		highlightColourId        = 0x1000202, /**< The colour with which to fill the background of highlighted sections of
												   the text - this can be transparent if you don't want to show any
												   highlighting.*/

		highlightedTextColourId  = 0x1000203, /**< The colour with which to draw the text in highlighted sections. */

		outlineColourId          = 0x1000205, /**< If this is non-transparent, it will be used to draw a box around
												   the edge of the component. */

		focusedOutlineColourId   = 0x1000206, /**< If this is non-transparent, it will be used to draw a box around
												   the edge of the component when it has focus. */

		shadowColourId           = 0x1000207, /**< If this is non-transparent, it'll be used to draw an inner shadow
												   around the edge of the editor. */
	};

	/** Sets the font to use for newly added text.

		This will change the font that will be used next time any text is added or entered
		into the editor. It won't change the font of any existing text - to do that, use
		applyFontToAllText() instead.

		@see applyFontToAllText
	*/
	void setFont (const Font& newFont);

	/** Applies a font to all the text in the editor.

		This will also set the current font to use for any new text that's added.

		@see setFont
	*/
	void applyFontToAllText (const Font& newFont);

	/** Returns the font that's currently being used for new text.

		@see setFont
	*/
	const Font& getFont() const;

	/** If set to true, focusing on the editor will highlight all its text.

		(Set to false by default).

		This is useful for boxes where you expect the user to re-enter all the
		text when they focus on the component, rather than editing what's already there.
	*/
	void setSelectAllWhenFocused (bool b);

	/** Sets limits on the characters that can be entered.

		@param maxTextLength        if this is > 0, it sets a maximum length limit; if 0, no
									limit is set
		@param allowedCharacters    if this is non-empty, then only characters that occur in
									this string are allowed to be entered into the editor.
	*/
	void setInputRestrictions (int maxTextLength,
							   const String& allowedCharacters = String::empty);

	/** When the text editor is empty, it can be set to display a message.

		This is handy for things like telling the user what to type in the box - the
		string is only displayed, it's not taken to actually be the contents of
		the editor.
	*/
	void setTextToShowWhenEmpty (const String& text, const Colour& colourToUse);

	/** Changes the size of the scrollbars that are used.

		Handy if you need smaller scrollbars for a small text box.
	*/
	void setScrollBarThickness (int newThicknessPixels);

	/** Shows or hides the buttons on any scrollbars that are used.

		@see ScrollBar::setButtonVisibility
	*/
	void setScrollBarButtonVisibility (bool buttonsVisible);

	/**
		Receives callbacks from a TextEditor component when it changes.

		@see TextEditor::addListener
	*/
	class JUCE_API  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener()  {}

		/** Called when the user changes the text in some way. */
		virtual void textEditorTextChanged (TextEditor& editor);

		/** Called when the user presses the return key. */
		virtual void textEditorReturnKeyPressed (TextEditor& editor);

		/** Called when the user presses the escape key. */
		virtual void textEditorEscapeKeyPressed (TextEditor& editor);

		/** Called when the text editor loses focus. */
		virtual void textEditorFocusLost (TextEditor& editor);
	};

	/** Registers a listener to be told when things happen to the text.

		@see removeListener
	*/
	void addListener (Listener* newListener);

	/** Deregisters a listener.

		@see addListener
	*/
	void removeListener (Listener* listenerToRemove);

	/** Returns the entire contents of the editor. */
	String getText() const;

	/** Returns a section of the contents of the editor. */
	String getTextInRange (const Range<int>& textRange) const;

	/** Returns true if there are no characters in the editor.

		This is more efficient than calling getText().isEmpty().
	*/
	bool isEmpty() const;

	/** Sets the entire content of the editor.

		This will clear the editor and insert the given text (using the current text colour
		and font). You can set the current text colour using
		@code setColour (TextEditor::textColourId, ...);
		@endcode

		@param newText                  the text to add
		@param sendTextChangeMessage    if true, this will cause a change message to
										be sent to all the listeners.
		@see insertText
	*/
	void setText (const String& newText,
				  bool sendTextChangeMessage = true);

	/** Returns a Value object that can be used to get or set the text.

		Bear in mind that this operate quite slowly if your text box contains large
		amounts of text, as it needs to dynamically build the string that's involved. It's
		best used for small text boxes.
	*/
	Value& getTextValue();

	/** Inserts some text at the current caret position.

		If a section of the text is highlighted, it will be replaced by
		this string, otherwise it will be inserted.

		To delete a section of text, you can use setHighlightedRegion() to
		highlight it, and call insertTextAtCursor (String::empty).

		@see setCaretPosition, getCaretPosition, setHighlightedRegion
	*/
	void insertTextAtCaret (const String& textToInsert);

	/** Deletes all the text from the editor. */
	void clear();

	/** Deletes the currently selected region.
		This doesn't copy the deleted section to the clipboard - if you need to do that, call copy() first.
		@see copy, paste, SystemClipboard
	*/
	void cut();

	/** Copies the currently selected region to the clipboard.
		@see cut, paste, SystemClipboard
	*/
	void copy();

	/** Pastes the contents of the clipboard into the editor at the caret position.
		@see cut, copy, SystemClipboard
	*/
	void paste();

	/** Returns the current index of the caret.
		@see setCaretPosition
	*/
	int getCaretPosition() const;

	/** Moves the caret to be in front of a given character.
		@see getCaretPosition, moveCaretToEnd
	*/
	void setCaretPosition (int newIndex);

	/** Moves the caret to be the end of all the text.
		@see setCaretPosition
	*/
	void moveCaretToEnd();

	/** Attempts to scroll the text editor so that the caret ends up at
		a specified position.

		This won't affect the caret's position within the text, it tries to scroll
		the entire editor vertically and horizontally so that the caret is sitting
		at the given position (relative to the top-left of this component).

		Depending on the amount of text available, it might not be possible to
		scroll far enough for the caret to reach this exact position, but it
		will go as far as it can in that direction.
	*/
	void scrollEditorToPositionCaret (int desiredCaretX, int desiredCaretY);

	/** Get the graphical position of the caret.

		The rectangle returned is relative to the component's top-left corner.
		@see scrollEditorToPositionCaret
	*/
	Rectangle<int> getCaretRectangle();

	/** Selects a section of the text. */
	void setHighlightedRegion (const Range<int>& newSelection);

	/** Returns the range of characters that are selected.
		If nothing is selected, this will return an empty range.
		@see setHighlightedRegion
	*/
	Range<int> getHighlightedRegion() const                   { return selection; }

	/** Returns the section of text that is currently selected. */
	String getHighlightedText() const;

	/** Finds the index of the character at a given position.

		The co-ordinates are relative to the component's top-left.
	*/
	int getTextIndexAt (int x, int y);

	/** Counts the number of characters in the text.

		This is quicker than getting the text as a string if you just need to know
		the length.
	*/
	int getTotalNumChars() const;

	/** Returns the total width of the text, as it is currently laid-out.

		This may be larger than the size of the TextEditor, and can change when
		the TextEditor is resized or the text changes.
	*/
	int getTextWidth() const;

	/** Returns the maximum height of the text, as it is currently laid-out.

		This may be larger than the size of the TextEditor, and can change when
		the TextEditor is resized or the text changes.
	*/
	int getTextHeight() const;

	/** Changes the size of the gap at the top and left-edge of the editor.

		By default there's a gap of 4 pixels.
	*/
	void setIndents (int newLeftIndent, int newTopIndent);

	/** Changes the size of border left around the edge of the component.

		@see getBorder
	*/
	void setBorder (const BorderSize<int>& border);

	/** Returns the size of border around the edge of the component.

		@see setBorder
	*/
	BorderSize<int> getBorder() const;

	/** Used to disable the auto-scrolling which keeps the caret visible.

		If true (the default), the editor will scroll when the caret moves offscreen. If
		set to false, it won't.
	*/
	void setScrollToShowCursor (bool shouldScrollToShowCaret);

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void paintOverChildren (Graphics& g);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseUp (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseDoubleClick (const MouseEvent& e);
	/** @internal */
	void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
	/** @internal */
	bool keyPressed (const KeyPress& key);
	/** @internal */
	bool keyStateChanged (bool isKeyDown);
	/** @internal */
	void focusGained (FocusChangeType cause);
	/** @internal */
	void focusLost (FocusChangeType cause);
	/** @internal */
	void resized();
	/** @internal */
	void enablementChanged();
	/** @internal */
	void colourChanged();
	/** @internal */
	void lookAndFeelChanged();
	/** @internal */
	bool isTextInputActive() const;
	/** @internal */
	void setTemporaryUnderlining (const Array <Range<int> >&);

	bool moveCaretLeft (bool moveInWholeWordSteps, bool selecting);
	bool moveCaretRight (bool moveInWholeWordSteps, bool selecting);
	bool moveCaretUp (bool selecting);
	bool moveCaretDown (bool selecting);
	bool pageUp (bool selecting);
	bool pageDown (bool selecting);
	bool scrollDown();
	bool scrollUp();
	bool moveCaretToTop (bool selecting);
	bool moveCaretToStartOfLine (bool selecting);
	bool moveCaretToEnd (bool selecting);
	bool moveCaretToEndOfLine (bool selecting);
	bool deleteBackwards (bool moveInWholeWordSteps);
	bool deleteForwards (bool moveInWholeWordSteps);
	bool copyToClipboard();
	bool cutToClipboard();
	bool pasteFromClipboard();
	bool selectAll();
	bool undo();
	bool redo();

	/** This adds the items to the popup menu.

		By default it adds the cut/copy/paste items, but you can override this if
		you need to replace these with your own items.

		If you want to add your own items to the existing ones, you can override this,
		call the base class's addPopupMenuItems() method, then append your own items.

		When the menu has been shown, performPopupMenuAction() will be called to
		perform the item that the user has chosen.

		The default menu items will be added using item IDs in the range
		0x7fff0000 - 0x7fff1000, so you should avoid those values for your own
		menu IDs.

		If this was triggered by a mouse-click, the mouseClickEvent parameter will be
		a pointer to the info about it, or may be null if the menu is being triggered
		by some other means.

		@see performPopupMenuAction, setPopupMenuEnabled, isPopupMenuEnabled
	*/
	virtual void addPopupMenuItems (PopupMenu& menuToAddTo,
									const MouseEvent* mouseClickEvent);

	/** This is called to perform one of the items that was shown on the popup menu.

		If you've overridden addPopupMenuItems(), you should also override this
		to perform the actions that you've added.

		If you've overridden addPopupMenuItems() but have still left the default items
		on the menu, remember to call the superclass's performPopupMenuAction()
		so that it can perform the default actions if that's what the user clicked on.

		@see addPopupMenuItems, setPopupMenuEnabled, isPopupMenuEnabled
	*/
	virtual void performPopupMenuAction (int menuItemID);

	struct Ids
	{
		static const Identifier tagType, text, font, mode, readOnly, scrollbarsShown,
								caretVisible, popupMenuEnabled;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);

protected:

	/** Scrolls the minimum distance needed to get the caret into view. */
	void scrollToMakeSureCursorIsVisible();

	/** @internal */
	void moveCaret (int newCaretPos);

	/** @internal */
	void moveCaretTo (int newPosition, bool isSelecting);

	/** Used internally to dispatch a text-change message. */
	void textChanged();

	/** Begins a new transaction in the UndoManager. */
	void newTransaction();

	/** Used internally to trigger an undo or redo. */
	void doUndoRedo (bool isRedo);

	/** Can be overridden to intercept return key presses directly */
	virtual void returnPressed();

	/** Can be overridden to intercept escape key presses directly */
	virtual void escapePressed();

	/** @internal */
	void handleCommandMessage (int commandId);

private:

	class Iterator;
	class UniformTextSection;
	class TextHolderComponent;
	class InsertAction;
	class RemoveAction;
	friend class InsertAction;
	friend class RemoveAction;

	ScopedPointer <Viewport> viewport;
	TextHolderComponent* textHolder;
	BorderSize<int> borderSize;

	bool readOnly                   : 1;
	bool multiline                  : 1;
	bool wordWrap                   : 1;
	bool returnKeyStartsNewLine     : 1;
	bool popupMenuEnabled           : 1;
	bool selectAllTextWhenFocused   : 1;
	bool scrollbarVisible           : 1;
	bool wasFocused                 : 1;
	bool keepCaretOnScreen          : 1;
	bool tabKeyUsed                 : 1;
	bool menuActive                 : 1;
	bool valueTextNeedsUpdating     : 1;

	UndoManager undoManager;
	ScopedPointer<CaretComponent> caret;
	int maxTextLength;
	Range<int> selection;
	int leftIndent, topIndent;
	unsigned int lastTransactionTime;
	Font currentFont;
	mutable int totalNumChars;
	int caretPosition;
	Array <UniformTextSection*> sections;
	String textToShowWhenEmpty;
	Colour colourForTextWhenEmpty;
	juce_wchar passwordCharacter;
	Value textValue;

	enum
	{
		notDragging,
		draggingSelectionStart,
		draggingSelectionEnd
	} dragType;

	String allowedCharacters;
	ListenerList <Listener> listeners;
	Array <Range<int> > underlinedSections;

	void coalesceSimilarSections();
	void splitSection (int sectionIndex, int charToSplitAt);
	void clearInternal (UndoManager* um);
	void insert (const String& text, int insertIndex, const Font& font,
				 const Colour& colour, UndoManager* um, int caretPositionToMoveTo);
	void reinsert (int insertIndex, const Array <UniformTextSection*>& sections);
	void remove (const Range<int>& range, UndoManager* um, int caretPositionToMoveTo);
	void getCharPosition (int index, float& x, float& y, float& lineHeight) const;
	void updateCaretPosition();
	void updateValueFromText();
	void textWasChangedByValue();
	int indexAtPosition (float x, float y);
	int findWordBreakAfter (int position) const;
	int findWordBreakBefore (int position) const;
	bool moveCaretWithTransation (int newPos, bool selecting);
	friend class TextHolderComponent;
	friend class TextEditorViewport;
	void drawContent (Graphics& g);
	void updateTextHolderSize();
	float getWordWrapWidth() const;
	void timerCallbackInt();
	void repaintText (const Range<int>& range);
	void scrollByLines (int deltaLines);
	bool undoOrRedo (bool shouldUndo);
	UndoManager* getUndoManager() noexcept;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextEditor);
};

/** This typedef is just for compatibility with old code - newer code should use the TextEditor::Listener class directly. */
typedef TextEditor::Listener TextEditorListener;

#endif   // __JUCE_TEXTEDITOR_JUCEHEADER__

/*** End of inlined file: juce_TextEditor.h ***/


/*** Start of inlined file: juce_ComboBox.h ***/
#ifndef __JUCE_COMBOBOX_JUCEHEADER__
#define __JUCE_COMBOBOX_JUCEHEADER__


/*** Start of inlined file: juce_Label.h ***/
#ifndef __JUCE_LABEL_JUCEHEADER__
#define __JUCE_LABEL_JUCEHEADER__

/**
	A component that displays a text string, and can optionally become a text
	editor when clicked.
*/
class JUCE_API  Label  : public Component,
						 public SettableTooltipClient,
						 protected TextEditorListener,
						 private ComponentListener,
						 private ValueListener
{
public:

	/** Creates a Label.

		@param componentName    the name to give the component
		@param labelText        the text to show in the label
	*/
	Label (const String& componentName = String::empty,
		   const String& labelText = String::empty);

	/** Destructor. */
	~Label();

	/** Changes the label text.

		If broadcastChangeMessage is true and the new text is different to the current
		text, then the class will broadcast a change message to any Label::Listener objects
		that are registered.
	*/
	void setText (const String& newText, bool broadcastChangeMessage);

	/** Returns the label's current text.

		@param returnActiveEditorContents   if this is true and the label is currently
											being edited, then this method will return the
											text as it's being shown in the editor. If false,
											then the value returned here won't be updated until
											the user has finished typing and pressed the return
											key.
	*/
	String getText (bool returnActiveEditorContents = false) const;

	/** Returns the text content as a Value object.
		You can call Value::referTo() on this object to make the label read and control
		a Value object that you supply.
	*/
	Value& getTextValue()                               { return textValue; }

	/** Changes the font to use to draw the text.

		@see getFont
	*/
	void setFont (const Font& newFont);

	/** Returns the font currently being used.

		@see setFont
	*/
	const Font& getFont() const noexcept;

	/** A set of colour IDs to use to change the colour of various aspects of the label.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		Note that you can also use the constants from TextEditor::ColourIds to change the
		colour of the text editor that is opened when a label is editable.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId     = 0x1000280, /**< The background colour to fill the label with. */
		textColourId           = 0x1000281, /**< The colour for the text. */
		outlineColourId        = 0x1000282  /**< An optional colour to use to draw a border around the label.
												 Leave this transparent to not have an outline. */
	};

	/** Sets the style of justification to be used for positioning the text.

		(The default is Justification::centredLeft)
	*/
	void setJustificationType (const Justification& justification);

	/** Returns the type of justification, as set in setJustificationType(). */
	Justification getJustificationType() const noexcept                         { return justification; }

	/** Changes the gap that is left between the edge of the component and the text.
		By default there's a small gap left at the sides of the component to allow for
		the drawing of the border, but you can change this if necessary.
	*/
	void setBorderSize (int horizontalBorder, int verticalBorder);

	/** Returns the size of the horizontal gap being left around the text.
	*/
	int getHorizontalBorderSize() const noexcept                                { return horizontalBorderSize; }

	/** Returns the size of the vertical gap being left around the text.
	*/
	int getVerticalBorderSize() const noexcept                                  { return verticalBorderSize; }

	/** Makes this label "stick to" another component.

		This will cause the label to follow another component around, staying
		either to its left or above it.

		@param owner    the component to follow
		@param onLeft   if true, the label will stay on the left of its component; if
						false, it will stay above it.
	*/
	void attachToComponent (Component* owner, bool onLeft);

	/** If this label has been attached to another component using attachToComponent, this
		returns the other component.

		Returns 0 if the label is not attached.
	*/
	Component* getAttachedComponent() const;

	/** If the label is attached to the left of another component, this returns true.

		Returns false if the label is above the other component. This is only relevent if
		attachToComponent() has been called.
	*/
	bool isAttachedOnLeft() const noexcept                                      { return leftOfOwnerComp; }

	/** Specifies the minimum amount that the font can be squashed horizantally before it starts
		using ellipsis.

		@see Graphics::drawFittedText
	*/
	void setMinimumHorizontalScale (float newScale);

	float getMinimumHorizontalScale() const noexcept                            { return minimumHorizontalScale; }

	/**
		A class for receiving events from a Label.

		You can register a Label::Listener with a Label using the Label::addListener()
		method, and it will be called when the text of the label changes, either because
		of a call to Label::setText() or by the user editing the text (if the label is
		editable).

		@see Label::addListener, Label::removeListener
	*/
	class JUCE_API  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}

		/** Called when a Label's text has changed.
		*/
		virtual void labelTextChanged (Label* labelThatHasChanged) = 0;
	};

	/** Registers a listener that will be called when the label's text changes. */
	void addListener (Listener* listener);

	/** Deregisters a previously-registered listener. */
	void removeListener (Listener* listener);

	/** Makes the label turn into a TextEditor when clicked.

		By default this is turned off.

		If turned on, then single- or double-clicking will turn the label into
		an editor. If the user then changes the text, then the ChangeBroadcaster
		base class will be used to send change messages to any listeners that
		have registered.

		If the user changes the text, the textWasEdited() method will be called
		afterwards, and subclasses can override this if they need to do anything
		special.

		@param editOnSingleClick            if true, just clicking once on the label will start editing the text
		@param editOnDoubleClick            if true, a double-click is needed to start editing
		@param lossOfFocusDiscardsChanges   if true, clicking somewhere else while the text is being
											edited will discard any changes; if false, then this will
											commit the changes.
		@see showEditor, setEditorColours, TextEditor
	*/
	void setEditable (bool editOnSingleClick,
					  bool editOnDoubleClick = false,
					  bool lossOfFocusDiscardsChanges = false);

	/** Returns true if this option was set using setEditable(). */
	bool isEditableOnSingleClick() const noexcept                       { return editSingleClick; }

	/** Returns true if this option was set using setEditable(). */
	bool isEditableOnDoubleClick() const noexcept                       { return editDoubleClick; }

	/** Returns true if this option has been set in a call to setEditable(). */
	bool doesLossOfFocusDiscardChanges() const noexcept                 { return lossOfFocusDiscardsChanges; }

	/** Returns true if the user can edit this label's text. */
	bool isEditable() const noexcept                                    { return editSingleClick || editDoubleClick; }

	/** Makes the editor appear as if the label had been clicked by the user.

		@see textWasEdited, setEditable
	*/
	void showEditor();

	/** Hides the editor if it was being shown.

		@param discardCurrentEditorContents     if true, the label's text will be
												reset to whatever it was before the editor
												was shown; if false, the current contents of the
												editor will be used to set the label's text
												before it is hidden.
	*/
	void hideEditor (bool discardCurrentEditorContents);

	/** Returns true if the editor is currently focused and active. */
	bool isBeingEdited() const noexcept;

	struct Ids
	{
		static const Identifier tagType, text, font, editMode, justification, focusLossDiscardsChanges;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);

protected:

	/** Creates the TextEditor component that will be used when the user has clicked on the label.
		Subclasses can override this if they need to customise this component in some way.
	*/
	virtual TextEditor* createEditorComponent();

	/** Called after the user changes the text. */
	virtual void textWasEdited();

	/** Called when the text has been altered. */
	virtual void textWasChanged();

	/** Called when the text editor has just appeared, due to a user click or other focus change. */
	virtual void editorShown (TextEditor* editorComponent);

	/** Called when the text editor is going to be deleted, after editing has finished. */
	virtual void editorAboutToBeHidden (TextEditor* editorComponent);

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void mouseUp (const MouseEvent& e);
	/** @internal */
	void mouseDoubleClick (const MouseEvent& e);
	/** @internal */
	void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);
	/** @internal */
	void componentParentHierarchyChanged (Component& component);
	/** @internal */
	void componentVisibilityChanged (Component& component);
	/** @internal */
	void inputAttemptWhenModal();
	/** @internal */
	void focusGained (FocusChangeType);
	/** @internal */
	void enablementChanged();
	/** @internal */
	KeyboardFocusTraverser* createFocusTraverser();
	/** @internal */
	void textEditorTextChanged (TextEditor& editor);
	/** @internal */
	void textEditorReturnKeyPressed (TextEditor& editor);
	/** @internal */
	void textEditorEscapeKeyPressed (TextEditor& editor);
	/** @internal */
	void textEditorFocusLost (TextEditor& editor);
	/** @internal */
	void colourChanged();
	/** @internal */
	void valueChanged (Value&);

private:

	Value textValue;
	String lastTextValue;
	Font font;
	Justification justification;
	ScopedPointer<TextEditor> editor;
	ListenerList<Listener> listeners;
	WeakReference<Component> ownerComponent;
	int horizontalBorderSize, verticalBorderSize;
	float minimumHorizontalScale;
	bool editSingleClick : 1;
	bool editDoubleClick : 1;
	bool lossOfFocusDiscardsChanges : 1;
	bool leftOfOwnerComp : 1;

	bool updateFromTextEditorContents (TextEditor&);
	void callChangeListeners();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Label);
};

/** This typedef is just for compatibility with old code - newer code should use the Label::Listener class directly. */
typedef Label::Listener LabelListener;

#endif   // __JUCE_LABEL_JUCEHEADER__

/*** End of inlined file: juce_Label.h ***/

/**
	A component that lets the user choose from a drop-down list of choices.

	The combo-box has a list of text strings, each with an associated id number,
	that will be shown in the drop-down list when the user clicks on the component.

	The currently selected choice is displayed in the combo-box, and this can
	either be read-only text, or editable.

	To find out when the user selects a different item or edits the text, you
	can register a ComboBox::Listener to receive callbacks.

	@see ComboBox::Listener
*/
class JUCE_API  ComboBox  : public Component,
							public SettableTooltipClient,
							public LabelListener,  // (can't use Label::Listener due to idiotic VC2005 bug)
							public ValueListener,
							private AsyncUpdater
{
public:

	/** Creates a combo-box.

		On construction, the text field will be empty, so you should call the
		setSelectedId() or setText() method to choose the initial value before
		displaying it.

		@param componentName    the name to set for the component (see Component::setName())
	*/
	explicit ComboBox (const String& componentName = String::empty);

	/** Destructor. */
	~ComboBox();

	/** Sets whether the test in the combo-box is editable.

		The default state for a new ComboBox is non-editable, and can only be changed
		by choosing from the drop-down list.
	*/
	void setEditableText (bool isEditable);

	/** Returns true if the text is directly editable.
		@see setEditableText
	*/
	bool isTextEditable() const noexcept;

	/** Sets the style of justification to be used for positioning the text.

		The default is Justification::centredLeft. The text is displayed using a
		Label component inside the ComboBox.
	*/
	void setJustificationType (const Justification& justification);

	/** Returns the current justification for the text box.
		@see setJustificationType
	*/
	Justification getJustificationType() const noexcept;

	/** Adds an item to be shown in the drop-down list.

		@param newItemText      the text of the item to show in the list
		@param newItemId        an associated ID number that can be set or retrieved - see
								getSelectedId() and setSelectedId(). Note that this value can not
								be 0!
		@see setItemEnabled, addSeparator, addSectionHeading, removeItem, getNumItems, getItemText, getItemId
	*/
	void addItem (const String& newItemText, int newItemId);

	/** Adds an array of items to the drop-down list.
		The item ID of each item will be its index in the StringArray + firstItemIdOffset.
	*/
	void addItemList (const StringArray& items, int firstItemIdOffset);

	/** Adds a separator line to the drop-down list.

		This is like adding a separator to a popup menu. See PopupMenu::addSeparator().
	*/
	void addSeparator();

	/** Adds a heading to the drop-down list, so that you can group the items into
		different sections.

		The headings are indented slightly differently to set them apart from the
		items on the list, and obviously can't be selected. You might want to add
		separators between your sections too.

		@see addItem, addSeparator
	*/
	void addSectionHeading (const String& headingName);

	/** This allows items in the drop-down list to be selectively disabled.

		When you add an item, it's enabled by default, but you can call this
		method to change its status.

		If you disable an item which is already selected, this won't change the
		current selection - it just stops the user choosing that item from the list.
	*/
	void setItemEnabled (int itemId, bool shouldBeEnabled);

	/** Returns true if the given item is enabled. */
	bool isItemEnabled (int itemId) const noexcept;

	/** Changes the text for an existing item.
	*/
	void changeItemText (int itemId, const String& newText);

	/** Removes all the items from the drop-down list.

		If this call causes the content to be cleared, then a change-message
		will be broadcast unless dontSendChangeMessage is true.

		@see addItem, removeItem, getNumItems
	*/
	void clear (bool dontSendChangeMessage = false);

	/** Returns the number of items that have been added to the list.

		Note that this doesn't include headers or separators.
	*/
	int getNumItems() const noexcept;

	/** Returns the text for one of the items in the list.
		Note that this doesn't include headers or separators.
		@param index    the item's index from 0 to (getNumItems() - 1)
	*/
	String getItemText (int index) const;

	/** Returns the ID for one of the items in the list.

		Note that this doesn't include headers or separators.

		@param index    the item's index from 0 to (getNumItems() - 1)
	*/
	int getItemId (int index) const noexcept;

	/** Returns the index in the list of a particular item ID.
		If no such ID is found, this will return -1.
	*/
	int indexOfItemId (int itemId) const noexcept;

	/** Returns the ID of the item that's currently shown in the box.

		If no item is selected, or if the text is editable and the user
		has entered something which isn't one of the items in the list, then
		this will return 0.

		@see setSelectedId, getSelectedItemIndex, getText
	*/
	int getSelectedId() const noexcept;

	/** Returns a Value object that can be used to get or set the selected item's ID.

		You can call Value::referTo() on this object to make the combo box control
		another Value object.
	*/
	Value& getSelectedIdAsValue()                       { return currentId; }

	/** Sets one of the items to be the current selection.

		This will set the ComboBox's text to that of the item that matches
		this ID.

		@param newItemId                the new item to select
		@param dontSendChangeMessage    if set to true, this method won't trigger a
										change notification
		@see getSelectedId, setSelectedItemIndex, setText
	*/
	void setSelectedId (int newItemId, bool dontSendChangeMessage = false);

	/** Returns the index of the item that's currently shown in the box.

		If no item is selected, or if the text is editable and the user
		has entered something which isn't one of the items in the list, then
		this will return -1.

		@see setSelectedItemIndex, getSelectedId, getText
	*/
	int getSelectedItemIndex() const;

	/** Sets one of the items to be the current selection.

		This will set the ComboBox's text to that of the item at the given
		index in the list.

		@param newItemIndex             the new item to select
		@param dontSendChangeMessage    if set to true, this method won't trigger a
										change notification
		@see getSelectedItemIndex, setSelectedId, setText
	*/
	void setSelectedItemIndex (int newItemIndex, bool dontSendChangeMessage = false);

	/** Returns the text that is currently shown in the combo-box's text field.

		If the ComboBox has editable text, then this text may have been edited
		by the user; otherwise it will be one of the items from the list, or
		possibly an empty string if nothing was selected.

		@see setText, getSelectedId, getSelectedItemIndex
	*/
	String getText() const;

	/** Sets the contents of the combo-box's text field.

		The text passed-in will be set as the current text regardless of whether
		it is one of the items in the list. If the current text isn't one of the
		items, then getSelectedId() will return -1, otherwise it wil return
		the approriate ID.

		@param newText                  the text to select
		@param dontSendChangeMessage    if set to true, this method won't trigger a
										change notification
		@see getText
	*/
	void setText (const String& newText, bool dontSendChangeMessage = false);

	/** Programmatically opens the text editor to allow the user to edit the current item.

		This is the same effect as when the box is clicked-on.
		@see Label::showEditor();
	*/
	void showEditor();

	/** Pops up the combo box's list. */
	void showPopup();

	/**
		A class for receiving events from a ComboBox.

		You can register a ComboBox::Listener with a ComboBox using the ComboBox::addListener()
		method, and it will be called when the selected item in the box changes.

		@see ComboBox::addListener, ComboBox::removeListener
	*/
	class JUCE_API  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}

		/** Called when a ComboBox has its selected item changed. */
		virtual void comboBoxChanged (ComboBox* comboBoxThatHasChanged) = 0;
	};

	/** Registers a listener that will be called when the box's content changes. */
	void addListener (Listener* listener);

	/** Deregisters a previously-registered listener. */
	void removeListener (Listener* listener);

	/** Sets a message to display when there is no item currently selected.

		@see getTextWhenNothingSelected
	*/
	void setTextWhenNothingSelected (const String& newMessage);

	/** Returns the text that is shown when no item is selected.

		@see setTextWhenNothingSelected
	*/
	String getTextWhenNothingSelected() const;

	/** Sets the message to show when there are no items in the list, and the user clicks
		on the drop-down box.

		By default it just says "no choices", but this lets you change it to something more
		meaningful.
	*/
	void setTextWhenNoChoicesAvailable (const String& newMessage);

	/** Returns the text shown when no items have been added to the list.
		@see setTextWhenNoChoicesAvailable
	*/
	String getTextWhenNoChoicesAvailable() const;

	/** Gives the ComboBox a tooltip. */
	void setTooltip (const String& newTooltip);

	/** A set of colour IDs to use to change the colour of various aspects of the combo box.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		To change the colours of the menu that pops up

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId  = 0x1000b00,    /**< The background colour to fill the box with. */
		textColourId        = 0x1000a00,    /**< The colour for the text in the box. */
		outlineColourId     = 0x1000c00,    /**< The colour for an outline around the box. */
		buttonColourId      = 0x1000d00,    /**< The base colour for the button (a LookAndFeel class will probably use variations on this). */
		arrowColourId       = 0x1000e00,    /**< The colour for the arrow shape that pops up the menu */
	};

	struct Ids
	{
		static const Identifier tagType, items, editable, textJustification, unselectedText, noItemsText;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);

	/** @internal */
	void labelTextChanged (Label*);
	/** @internal */
	void enablementChanged();
	/** @internal */
	void colourChanged();
	/** @internal */
	void focusGained (Component::FocusChangeType cause);
	/** @internal */
	void focusLost (Component::FocusChangeType cause);
	/** @internal */
	void handleAsyncUpdate();
	/** @internal */
	String getTooltip()                                       { return label->getTooltip(); }
	/** @internal */
	void mouseDown (const MouseEvent&);
	/** @internal */
	void mouseDrag (const MouseEvent&);
	/** @internal */
	void mouseUp (const MouseEvent&);
	/** @internal */
	void lookAndFeelChanged();
	/** @internal */
	void paint (Graphics&);
	/** @internal */
	void resized();
	/** @internal */
	bool keyStateChanged (bool isKeyDown);
	/** @internal */
	bool keyPressed (const KeyPress&);
	/** @internal */
	void valueChanged (Value&);

private:

	struct ItemInfo
	{
		ItemInfo (const String& name, int itemId, bool isEnabled, bool isHeading);
		bool isSeparator() const noexcept;
		bool isRealItem() const noexcept;

		String name;
		int itemId;
		bool isEnabled : 1, isHeading : 1;
	};

	OwnedArray <ItemInfo> items;
	Value currentId;
	int lastCurrentId;
	bool isButtonDown, separatorPending, menuActive, textIsCustom;
	ListenerList <Listener> listeners;
	ScopedPointer<Label> label;
	String textWhenNothingSelected, noChoicesMessage;

	ItemInfo* getItemForId (int itemId) const noexcept;
	ItemInfo* getItemForIndex (int index) const noexcept;
	bool selectIfEnabled (int index);
	static void popupMenuFinishedCallback (int, ComboBox*);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComboBox);
};

/** This typedef is just for compatibility with old code - newer code should use the ComboBox::Listener class directly. */
typedef ComboBox::Listener ComboBoxListener;

#endif   // __JUCE_COMBOBOX_JUCEHEADER__

/*** End of inlined file: juce_ComboBox.h ***/

/**
	A component for browsing and selecting a file or directory to open or save.

	This contains a FileListComponent and adds various boxes and controls for
	navigating and selecting a file. It can work in different modes so that it can
	be used for loading or saving a file, or for choosing a directory.

	@see FileChooserDialogBox, FileChooser, FileListComponent
*/
class JUCE_API  FileBrowserComponent  : public Component,
										private FileBrowserListener,
										private TextEditorListener,
										private ButtonListener,
										private ComboBoxListener,  // (can't use ComboBox::Listener due to idiotic VC2005 bug)
										private FileFilter
{
public:

	/** Various options for the browser.

		A combination of these is passed into the FileBrowserComponent constructor.
	*/
	enum FileChooserFlags
	{
		openMode                = 1,    /**< specifies that the component should allow the user to
											 choose an existing file with the intention of opening it. */
		saveMode                = 2,    /**< specifies that the component should allow the user to specify
											 the name of a file that will be used to save something. */
		canSelectFiles          = 4,    /**< specifies that the user can select files (can be used in
											 conjunction with canSelectDirectories). */
		canSelectDirectories    = 8,    /**< specifies that the user can select directories (can be used in
											 conjuction with canSelectFiles). */
		canSelectMultipleItems  = 16,   /**< specifies that the user can select multiple items. */
		useTreeView             = 32,   /**< specifies that a tree-view should be shown instead of a file list. */
		filenameBoxIsReadOnly   = 64    /**< specifies that the user can't type directly into the filename box. */
	};

	/** Creates a FileBrowserComponent.

		@param flags                    A combination of flags from the FileChooserFlags enumeration, used to
										specify the component's behaviour. The flags must contain either openMode
										or saveMode, and canSelectFiles and/or canSelectDirectories.
		@param initialFileOrDirectory   The file or directory that should be selected when the component begins.
										If this is File::nonexistent, a default directory will be chosen.
		@param fileFilter               an optional filter to use to determine which files are shown.
										If this is nullptr then all files are displayed. Note that a pointer
										is kept internally to this object, so make sure that it is not deleted
										before the FileBrowserComponent object is deleted.
		@param previewComp              an optional preview component that will be used to show previews of
										files that the user selects
	*/
	FileBrowserComponent (int flags,
						  const File& initialFileOrDirectory,
						  const FileFilter* fileFilter,
						  FilePreviewComponent* previewComp);

	/** Destructor. */
	~FileBrowserComponent();

	/** Returns the number of files that the user has got selected.
		If multiple select isn't active, this will only be 0 or 1. To get the complete
		list of files they've chosen, pass an index to getCurrentFile().
	*/
	int getNumSelectedFiles() const noexcept;

	/** Returns one of the files that the user has chosen.
		If the box has multi-select enabled, the index lets you specify which of the files
		to get - see getNumSelectedFiles() to find out how many files were chosen.
		@see getHighlightedFile
	*/
	File getSelectedFile (int index) const noexcept;

	/** Deselects any files that are currently selected.
	*/
	void deselectAllFiles();

	/** Returns true if the currently selected file(s) are usable.

		This can be used to decide whether the user can press "ok" for the
		current file. What it does depends on the mode, so for example in an "open"
		mode, this only returns true if a file has been selected and if it exists.
		In a "save" mode, a non-existent file would also be valid.
	*/
	bool currentFileIsValid() const;

	/** This returns the last item in the view that the user has highlighted.
		This may be different from getCurrentFile(), which returns the value
		that is shown in the filename box, and if there are multiple selections,
		this will only return one of them.
		@see getSelectedFile
	*/
	File getHighlightedFile() const noexcept;

	/** Returns the directory whose contents are currently being shown in the listbox. */
	const File& getRoot() const;

	/** Changes the directory that's being shown in the listbox. */
	void setRoot (const File& newRootDirectory);

	/** Changes the name that is currently shown in the filename box. */
	void setFileName (const String& newName);

	/** Equivalent to pressing the "up" button to browse the parent directory. */
	void goUp();

	/** Refreshes the directory that's currently being listed. */
	void refresh();

	/** Changes the filter that's being used to sift the files. */
	void setFileFilter (const FileFilter* newFileFilter);

	/** Returns a verb to describe what should happen when the file is accepted.

		E.g. if browsing in "load file" mode, this will be "Open", if in "save file"
		mode, it'll be "Save", etc.
	*/
	virtual String getActionVerb() const;

	/** Returns true if the saveMode flag was set when this component was created.
	*/
	bool isSaveMode() const noexcept;

	/** Sets the label that will be displayed next to the filename entry box.
		By default this is just "file", but you might want to change it to something more
		appropriate for your app.
	*/
	void setFilenameBoxLabel (const String& name);

	/** Adds a listener to be told when the user selects and clicks on files.
		@see removeListener
	*/
	void addListener (FileBrowserListener* listener);

	/** Removes a listener.
		@see addListener
	*/
	void removeListener (FileBrowserListener* listener);

	/** @internal */
	void resized();
	/** @internal */
	void buttonClicked (Button*);
	/** @internal */
	void comboBoxChanged (ComboBox*);
	/** @internal */
	void textEditorTextChanged (TextEditor&);
	/** @internal */
	void textEditorReturnKeyPressed (TextEditor&);
	/** @internal */
	void textEditorEscapeKeyPressed (TextEditor&);
	/** @internal */
	void textEditorFocusLost (TextEditor&);
	/** @internal */
	bool keyPressed (const KeyPress&);
	/** @internal */
	void selectionChanged();
	/** @internal */
	void fileClicked (const File&, const MouseEvent&);
	/** @internal */
	void fileDoubleClicked (const File&);
	/** @internal */
	void browserRootChanged (const File&);
	/** @internal */
	bool isFileSuitable (const File&) const;
	/** @internal */
	bool isDirectorySuitable (const File&) const;

	/** @internal */
	FilePreviewComponent* getPreviewComponent() const noexcept;

	/** @internal */
	DirectoryContentsDisplayComponent* getDisplayComponent() const noexcept;

protected:
	/** Returns a list of names and paths for the default places the user might want to look.
		Use an empty string to indicate a section break.
	*/
	virtual void getRoots (StringArray& rootNames, StringArray& rootPaths);

	/** Updates the items in the dropdown list of recent paths with the values from getRoots(). */
	void resetRecentPaths();

private:

	ScopedPointer <DirectoryContentsList> fileList;
	const FileFilter* fileFilter;

	int flags;
	File currentRoot;
	Array<File> chosenFiles;
	ListenerList <FileBrowserListener> listeners;

	ScopedPointer<DirectoryContentsDisplayComponent> fileListComponent;
	FilePreviewComponent* previewComp;
	ComboBox currentPathBox;
	TextEditor filenameBox;
	Label fileLabel;
	ScopedPointer<Button> goUpButton;

	TimeSliceThread thread;

	void sendListenerChangeMessage();
	bool isFileOrDirSuitable (const File& f) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileBrowserComponent);
};

#endif   // __JUCE_FILEBROWSERCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_FileBrowserComponent.h ***/


#endif
#ifndef __JUCE_FILEBROWSERLISTENER_JUCEHEADER__

#endif
#ifndef __JUCE_FILECHOOSER_JUCEHEADER__

/*** Start of inlined file: juce_FileChooser.h ***/
#ifndef __JUCE_FILECHOOSER_JUCEHEADER__
#define __JUCE_FILECHOOSER_JUCEHEADER__

/**
	Creates a dialog box to choose a file or directory to load or save.

	To use a FileChooser:
	- create one (as a local stack variable is the neatest way)
	- call one of its browseFor.. methods
	- if this returns true, the user has selected a file, so you can retrieve it
	  with the getResult() method.

	e.g. @code
	void loadMooseFile()
	{
		FileChooser myChooser ("Please select the moose you want to load...",
							   File::getSpecialLocation (File::userHomeDirectory),
							   "*.moose");

		if (myChooser.browseForFileToOpen())
		{
			File mooseFile (myChooser.getResult());

			loadMoose (mooseFile);
		}
	}
	@endcode
*/
class JUCE_API  FileChooser
{
public:

	/** Creates a FileChooser.

		After creating one of these, use one of the browseFor... methods to display it.

		@param dialogBoxTitle           a text string to display in the dialog box to
										tell the user what's going on
		@param initialFileOrDirectory   the file or directory that should be selected when
										the dialog box opens. If this parameter is set to
										File::nonexistent, a sensible default directory
										will be used instead.
		@param filePatternsAllowed      a set of file patterns to specify which files can be
										selected - each pattern should be separated by a
										comma or semi-colon, e.g. "*" or "*.jpg;*.gif". An
										empty string means that all files are allowed
		@param useOSNativeDialogBox     if true, then a native dialog box will be used if
										possible; if false, then a Juce-based browser dialog
										box will always be used
		@see browseForFileToOpen, browseForFileToSave, browseForDirectory
	*/
	FileChooser (const String& dialogBoxTitle,
				 const File& initialFileOrDirectory = File::nonexistent,
				 const String& filePatternsAllowed = String::empty,
				 bool useOSNativeDialogBox = true);

	/** Destructor. */
	~FileChooser();

	/** Shows a dialog box to choose a file to open.

		This will display the dialog box modally, using an "open file" mode, so that
		it won't allow non-existent files or directories to be chosen.

		@param previewComponent   an optional component to display inside the dialog
								  box to show special info about the files that the user
								  is browsing. The component will not be deleted by this
								  object, so the caller must take care of it.
		@returns    true if the user selected a file, in which case, use the getResult()
					method to find out what it was. Returns false if they cancelled instead.
		@see browseForFileToSave, browseForDirectory
	*/
	bool browseForFileToOpen (FilePreviewComponent* previewComponent = 0);

	/** Same as browseForFileToOpen, but allows the user to select multiple files.

		The files that are returned can be obtained by calling getResults(). See
		browseForFileToOpen() for more info about the behaviour of this method.
	*/
	bool browseForMultipleFilesToOpen (FilePreviewComponent* previewComponent = 0);

	/** Shows a dialog box to choose a file to save.

		This will display the dialog box modally, using an "save file" mode, so it
		will allow non-existent files to be chosen, but not directories.

		@param warnAboutOverwritingExistingFiles     if true, the dialog box will ask
					the user if they're sure they want to overwrite a file that already
					exists
		@returns    true if the user chose a file and pressed 'ok', in which case, use
					the getResult() method to find out what the file was. Returns false
					if they cancelled instead.
		@see browseForFileToOpen, browseForDirectory
	*/
	bool browseForFileToSave (bool warnAboutOverwritingExistingFiles);

	/** Shows a dialog box to choose a directory.

		This will display the dialog box modally, using an "open directory" mode, so it
		will only allow directories to be returned, not files.

		@returns    true if the user chose a directory and pressed 'ok', in which case, use
					the getResult() method to find out what they chose. Returns false
					if they cancelled instead.
		@see browseForFileToOpen, browseForFileToSave
	*/
	bool browseForDirectory();

	/** Same as browseForFileToOpen, but allows the user to select multiple files and directories.

		The files that are returned can be obtained by calling getResults(). See
		browseForFileToOpen() for more info about the behaviour of this method.
	*/
	bool browseForMultipleFilesOrDirectories (FilePreviewComponent* previewComponent = 0);

	/** Returns the last file that was chosen by one of the browseFor methods.

		After calling the appropriate browseFor... method, this method lets you
		find out what file or directory they chose.

		Note that the file returned is only valid if the browse method returned true (i.e.
		if the user pressed 'ok' rather than cancelling).

		If you're using a multiple-file select, then use the getResults() method instead,
		to obtain the list of all files chosen.

		@see getResults
	*/
	File getResult() const;

	/** Returns a list of all the files that were chosen during the last call to a
		browse method.

		This array may be empty if no files were chosen, or can contain multiple entries
		if multiple files were chosen.

		@see getResult
	*/
	const Array<File>& getResults() const;

private:

	String title, filters;
	File startingFile;
	Array<File> results;
	bool useNativeDialogBox;

	bool showDialog (bool selectsDirectories, bool selectsFiles, bool isSave,
					 bool warnAboutOverwritingExistingFiles, bool selectMultipleFiles,
					 FilePreviewComponent* previewComponent);

	static void showPlatformDialog (Array<File>& results, const String& title, const File& file,
									const String& filters, bool selectsDirectories, bool selectsFiles,
									bool isSave, bool warnAboutOverwritingExistingFiles, bool selectMultipleFiles,
									FilePreviewComponent* previewComponent);
	static bool isPlatformDialogAvailable();

	JUCE_LEAK_DETECTOR (FileChooser);
};

#endif   // __JUCE_FILECHOOSER_JUCEHEADER__

/*** End of inlined file: juce_FileChooser.h ***/


#endif
#ifndef __JUCE_FILECHOOSERDIALOGBOX_JUCEHEADER__

/*** Start of inlined file: juce_FileChooserDialogBox.h ***/
#ifndef __JUCE_FILECHOOSERDIALOGBOX_JUCEHEADER__
#define __JUCE_FILECHOOSERDIALOGBOX_JUCEHEADER__


/*** Start of inlined file: juce_ResizableWindow.h ***/
#ifndef __JUCE_RESIZABLEWINDOW_JUCEHEADER__
#define __JUCE_RESIZABLEWINDOW_JUCEHEADER__


/*** Start of inlined file: juce_TopLevelWindow.h ***/
#ifndef __JUCE_TOPLEVELWINDOW_JUCEHEADER__
#define __JUCE_TOPLEVELWINDOW_JUCEHEADER__


/*** Start of inlined file: juce_DropShadower.h ***/
#ifndef __JUCE_DROPSHADOWER_JUCEHEADER__
#define __JUCE_DROPSHADOWER_JUCEHEADER__

/**
	Adds a drop-shadow to a component.

	This object creates and manages a set of components which sit around a
	component, creating a gaussian shadow around it. The components will track
	the position of the component and if it's brought to the front they'll also
	follow this.

	For desktop windows you don't need to use this class directly - just
	set the Component::windowHasDropShadow flag when calling
	Component::addToDesktop(), and the system will create one of these if it's
	needed (which it obviously isn't on the Mac, for example).
*/
class JUCE_API  DropShadower  : public ComponentListener
{
public:

	/** Creates a DropShadower.

		@param alpha        the opacity of the shadows, from 0 to 1.0
		@param xOffset      the horizontal displacement of the shadow, in pixels
		@param yOffset      the vertical displacement of the shadow, in pixels
		@param blurRadius   the radius of the blur to use for creating the shadow
	*/
	DropShadower (float alpha = 0.5f,
				  int xOffset = 1,
				  int yOffset = 5,
				  float blurRadius = 10.0f);

	/** Destructor. */
	virtual ~DropShadower();

	/** Attaches the DropShadower to the component you want to shadow. */
	void setOwner (Component* componentToFollow);

	/** @internal */
	void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);
	/** @internal */
	void componentBroughtToFront (Component& component);
	/** @internal */
	void componentParentHierarchyChanged (Component& component);
	/** @internal */
	void componentVisibilityChanged (Component& component);

private:

	Component* owner;
	OwnedArray<Component> shadowWindows;
	Image shadowImageSections[12];
	const int xOffset, yOffset;
	const float alpha, blurRadius;
	bool reentrant;

	void updateShadows();
	void setShadowImage (const Image& src, int num, int w, int h, int sx, int sy);
	void bringShadowWindowsToFront();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropShadower);
};

#endif   // __JUCE_DROPSHADOWER_JUCEHEADER__

/*** End of inlined file: juce_DropShadower.h ***/

/**
	A base class for top-level windows.

	This class is used for components that are considered a major part of your
	application - e.g. ResizableWindow, DocumentWindow, DialogWindow, AlertWindow,
	etc. Things like menus that pop up briefly aren't derived from it.

	A TopLevelWindow is probably on the desktop, but this isn't mandatory - it
	could itself be the child of another component.

	The class manages a list of all instances of top-level windows that are in use,
	and each one is also given the concept of being "active". The active window is
	one that is actively being used by the user. This isn't quite the same as the
	component with the keyboard focus, because there may be a popup menu or other
	temporary window which gets keyboard focus while the active top level window is
	unchanged.

	A top-level window also has an optional drop-shadow.

	@see ResizableWindow, DocumentWindow, DialogWindow
*/
class JUCE_API  TopLevelWindow  : public Component
{
public:

	/** Creates a TopLevelWindow.

		@param name                 the name to give the component
		@param addToDesktop         if true, the window will be automatically added to the
									desktop; if false, you can use it as a child component
	*/
	TopLevelWindow (const String& name, bool addToDesktop);

	/** Destructor. */
	~TopLevelWindow();

	/** True if this is currently the TopLevelWindow that is actively being used.

		This isn't quite the same as having keyboard focus, because the focus may be
		on a child component or a temporary pop-up menu, etc, while this window is
		still considered to be active.

		@see activeWindowStatusChanged
	*/
	bool isActiveWindow() const noexcept                    { return windowIsActive_; }

	/** This will set the bounds of the window so that it's centred in front of another
		window.

		If your app has a few windows open and want to pop up a dialog box for one of
		them, you can use this to show it in front of the relevent parent window, which
		is a bit neater than just having it appear in the middle of the screen.

		If componentToCentreAround is 0, then the currently active TopLevelWindow will
		be used instead. If no window is focused, it'll just default to the middle of the
		screen.
	*/
	void centreAroundComponent (Component* componentToCentreAround,
								int width, int height);

	/** Turns the drop-shadow on and off. */
	void setDropShadowEnabled (bool useShadow);

	/** True if drop-shadowing is enabled. */
	bool isDropShadowEnabled() const noexcept               { return useDropShadow; }

	/** Sets whether an OS-native title bar will be used, or a Juce one.

		@see isUsingNativeTitleBar
	*/
	void setUsingNativeTitleBar (bool useNativeTitleBar);

	/** Returns true if the window is currently using an OS-native title bar.

		@see setUsingNativeTitleBar
	*/
	bool isUsingNativeTitleBar() const noexcept             { return useNativeTitleBar && isOnDesktop(); }

	/** Returns the number of TopLevelWindow objects currently in use.

		@see getTopLevelWindow
	*/
	static int getNumTopLevelWindows() noexcept;

	/** Returns one of the TopLevelWindow objects currently in use.

		The index is 0 to (getNumTopLevelWindows() - 1).
	*/
	static TopLevelWindow* getTopLevelWindow (int index) noexcept;

	/** Returns the currently-active top level window.

		There might not be one, of course, so this can return 0.
	*/
	static TopLevelWindow* getActiveTopLevelWindow() noexcept;

	/** @internal */
	virtual void addToDesktop (int windowStyleFlags, void* nativeWindowToAttachTo = nullptr);

protected:

	/** This callback happens when this window becomes active or inactive.

		@see isActiveWindow
	*/
	virtual void activeWindowStatusChanged();

	/** @internal */
	void focusOfChildComponentChanged (FocusChangeType cause);
	/** @internal */
	void parentHierarchyChanged();
	/** @internal */
	virtual int getDesktopWindowStyleFlags() const;
	/** @internal */
	void recreateDesktopWindow();
	/** @internal */
	void visibilityChanged();

private:
	friend class TopLevelWindowManager;
	bool useDropShadow, useNativeTitleBar, windowIsActive_;
	ScopedPointer <DropShadower> shadower;

	void setWindowActive (bool isNowActive);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopLevelWindow);
};

#endif   // __JUCE_TOPLEVELWINDOW_JUCEHEADER__

/*** End of inlined file: juce_TopLevelWindow.h ***/


/*** Start of inlined file: juce_ResizableBorderComponent.h ***/
#ifndef __JUCE_RESIZABLEBORDERCOMPONENT_JUCEHEADER__
#define __JUCE_RESIZABLEBORDERCOMPONENT_JUCEHEADER__

/**
	A component that resizes its parent component when dragged.

	This component forms a frame around the edge of a component, allowing it to
	be dragged by the edges or corners to resize it - like the way windows are
	resized in MSWindows or Linux.

	To use it, just add it to your component, making it fill the entire parent component
	(there's a mouse hit-test that only traps mouse-events which land around the
	edge of the component, so it's even ok to put it on top of any other components
	you're using). Make sure you rescale the resizer component to fill the parent
	each time the parent's size changes.

	@see ResizableCornerComponent
*/
class JUCE_API  ResizableBorderComponent  : public Component
{
public:

	/** Creates a resizer.

		Pass in the target component which you want to be resized when this one is
		dragged.

		The target component will usually be a parent of the resizer component, but this
		isn't mandatory.

		Remember that when the target component is resized, it'll need to move and
		resize this component to keep it in place, as this won't happen automatically.

		If the constrainer parameter is non-zero, then this object will be used to enforce
		limits on the size and position that the component can be stretched to. Make sure
		that the constrainer isn't deleted while still in use by this object.

		@see ComponentBoundsConstrainer
	*/
	ResizableBorderComponent (Component* componentToResize,
							  ComponentBoundsConstrainer* constrainer);

	/** Destructor. */
	~ResizableBorderComponent();

	/** Specifies how many pixels wide the draggable edges of this component are.

		@see getBorderThickness
	*/
	void setBorderThickness (const BorderSize<int>& newBorderSize);

	/** Returns the number of pixels wide that the draggable edges of this component are.

		@see setBorderThickness
	*/
	BorderSize<int> getBorderThickness() const;

	/** Represents the different sections of a resizable border, which allow it to
		resized in different ways.
	*/
	class Zone
	{
	public:

		enum Zones
		{
			centre  = 0,
			left    = 1,
			top     = 2,
			right   = 4,
			bottom  = 8
		};

		/** Creates a Zone from a combination of the flags in \enum Zones. */
		explicit Zone (int zoneFlags = 0) noexcept;
		Zone (const Zone& other) noexcept;
		Zone& operator= (const Zone& other) noexcept;

		bool operator== (const Zone& other) const noexcept;
		bool operator!= (const Zone& other) const noexcept;

		/** Given a point within a rectangle with a resizable border, this returns the
			zone that the point lies within.
		*/
		static const Zone fromPositionOnBorder (const Rectangle<int>& totalSize,
												const BorderSize<int>& border,
												const Point<int>& position);

		/** Returns an appropriate mouse-cursor for this resize zone. */
		MouseCursor getMouseCursor() const noexcept;

		/** Returns true if dragging this zone will move the enire object without resizing it. */
		bool isDraggingWholeObject() const noexcept     { return zone == centre; }
		/** Returns true if dragging this zone will move the object's left edge. */
		bool isDraggingLeftEdge() const noexcept        { return (zone & left) != 0; }
		/** Returns true if dragging this zone will move the object's right edge. */
		bool isDraggingRightEdge() const noexcept       { return (zone & right) != 0; }
		/** Returns true if dragging this zone will move the object's top edge. */
		bool isDraggingTopEdge() const noexcept         { return (zone & top) != 0; }
		/** Returns true if dragging this zone will move the object's bottom edge. */
		bool isDraggingBottomEdge() const noexcept      { return (zone & bottom) != 0; }

		/** Resizes this rectangle by the given amount, moving just the edges that this zone
			applies to.
		*/
		template <typename ValueType>
		const Rectangle<ValueType> resizeRectangleBy (Rectangle<ValueType> original,
													  const Point<ValueType>& distance) const noexcept
		{
			if (isDraggingWholeObject())
				return original + distance;

			if (isDraggingLeftEdge())
				original.setLeft (jmin (original.getRight(), original.getX() + distance.x));

			if (isDraggingRightEdge())
				original.setWidth (jmax (ValueType(), original.getWidth() + distance.x));

			if (isDraggingTopEdge())
				original.setTop (jmin (original.getBottom(), original.getY() + distance.y));

			if (isDraggingBottomEdge())
				original.setHeight (jmax (ValueType(), original.getHeight() + distance.y));

			return original;
		}

		/** Returns the raw flags for this zone. */
		int getZoneFlags() const noexcept               { return zone; }

	private:

		int zone;
	};

protected:

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void mouseEnter (const MouseEvent& e);
	/** @internal */
	void mouseMove (const MouseEvent& e);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseUp (const MouseEvent& e);
	/** @internal */
	bool hitTest (int x, int y);

private:
	WeakReference<Component> component;
	ComponentBoundsConstrainer* constrainer;
	BorderSize<int> borderSize;
	Rectangle<int> originalBounds;
	Zone mouseZone;

	void updateMouseZone (const MouseEvent& e);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResizableBorderComponent);
};

#endif   // __JUCE_RESIZABLEBORDERCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_ResizableBorderComponent.h ***/


/*** Start of inlined file: juce_ResizableCornerComponent.h ***/
#ifndef __JUCE_RESIZABLECORNERCOMPONENT_JUCEHEADER__
#define __JUCE_RESIZABLECORNERCOMPONENT_JUCEHEADER__

/** A component that resizes a parent component when dragged.

	This is the small triangular stripey resizer component you get in the bottom-right
	of windows (more commonly on the Mac than Windows). Put one in the corner of
	a larger component and it will automatically resize its parent when it gets dragged
	around.

	@see ResizableFrameComponent
*/
class JUCE_API  ResizableCornerComponent  : public Component
{
public:

	/** Creates a resizer.

		Pass in the target component which you want to be resized when this one is
		dragged.

		The target component will usually be a parent of the resizer component, but this
		isn't mandatory.

		Remember that when the target component is resized, it'll need to move and
		resize this component to keep it in place, as this won't happen automatically.

		If the constrainer parameter is non-zero, then this object will be used to enforce
		limits on the size and position that the component can be stretched to. Make sure
		that the constrainer isn't deleted while still in use by this object. If you
		pass a zero in here, no limits will be put on the sizes it can be stretched to.

		@see ComponentBoundsConstrainer
	*/
	ResizableCornerComponent (Component* componentToResize,
							  ComponentBoundsConstrainer* constrainer);

	/** Destructor. */
	~ResizableCornerComponent();

protected:

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseUp (const MouseEvent& e);
	/** @internal */
	bool hitTest (int x, int y);

private:

	WeakReference<Component> component;
	ComponentBoundsConstrainer* constrainer;
	Rectangle<int> originalBounds;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResizableCornerComponent);
};

#endif   // __JUCE_RESIZABLECORNERCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_ResizableCornerComponent.h ***/

/**
	A base class for top-level windows that can be dragged around and resized.

	To add content to the window, use its setContentOwned() or setContentNonOwned() methods
	to give it a component that will remain positioned inside it (leaving a gap around
	the edges for a border).

	It's not advisable to add child components directly to a ResizableWindow: put them
	inside your content component instead. And overriding methods like resized(), moved(), etc
	is also not recommended - instead override these methods for your content component.
	(If for some obscure reason you do need to override these methods, always remember to
	call the super-class's resized() method too, otherwise it'll fail to lay out the window
	decorations correctly).

	By default resizing isn't enabled - use the setResizable() method to enable it and
	to choose the style of resizing to use.

	@see TopLevelWindow
*/
class JUCE_API  ResizableWindow  : public TopLevelWindow
{
public:

	/** Creates a ResizableWindow.

		This constructor doesn't specify a background colour, so the LookAndFeel's default
		background colour will be used.

		@param name                 the name to give the component
		@param addToDesktop         if true, the window will be automatically added to the
									desktop; if false, you can use it as a child component
	*/
	ResizableWindow (const String& name,
					 bool addToDesktop);

	/** Creates a ResizableWindow.

		@param name                 the name to give the component
		@param backgroundColour     the colour to use for filling the window's background.
		@param addToDesktop         if true, the window will be automatically added to the
									desktop; if false, you can use it as a child component
	*/
	ResizableWindow (const String& name,
					 const Colour& backgroundColour,
					 bool addToDesktop);

	/** Destructor.
		If a content component has been set with setContentOwned(), it will be deleted.
	*/
	~ResizableWindow();

	/** Returns the colour currently being used for the window's background.

		As a convenience the window will fill itself with this colour, but you
		can override the paint() method if you need more customised behaviour.

		This method is the same as retrieving the colour for ResizableWindow::backgroundColourId.

		@see setBackgroundColour
	*/
	Colour getBackgroundColour() const noexcept;

	/** Changes the colour currently being used for the window's background.

		As a convenience the window will fill itself with this colour, but you
		can override the paint() method if you need more customised behaviour.

		Note that the opaque state of this window is altered by this call to reflect
		the opacity of the colour passed-in. On window systems which can't support
		semi-transparent windows this might cause problems, (though it's unlikely you'll
		be using this class as a base for a semi-transparent component anyway).

		You can also use the ResizableWindow::backgroundColourId colour id to set
		this colour.

		@see getBackgroundColour
	*/
	void setBackgroundColour (const Colour& newColour);

	/** Make the window resizable or fixed.

		@param shouldBeResizable            whether it's resizable at all
		@param useBottomRightCornerResizer  if true, it'll add a ResizableCornerComponent at the
											bottom-right; if false, it'll use a ResizableBorderComponent
											around the edge
		@see setResizeLimits, isResizable
	*/
	void setResizable (bool shouldBeResizable,
					   bool useBottomRightCornerResizer);

	/** True if resizing is enabled.

		@see setResizable
	*/
	bool isResizable() const noexcept;

	/** This sets the maximum and minimum sizes for the window.

		If the window's current size is outside these limits, it will be resized to
		make sure it's within them.

		Calling setBounds() on the component will bypass any size checking - it's only when
		the window is being resized by the user that these values are enforced.

		@see setResizable, setFixedAspectRatio
	*/
	void setResizeLimits (int newMinimumWidth,
						  int newMinimumHeight,
						  int newMaximumWidth,
						  int newMaximumHeight) noexcept;

	/** Returns the bounds constrainer object that this window is using.

		You can access this to change its properties.
	*/
	ComponentBoundsConstrainer* getConstrainer() noexcept           { return constrainer; }

	/** Sets the bounds-constrainer object to use for resizing and dragging this window.

		A pointer to the object you pass in will be kept, but it won't be deleted
		by this object, so it's the caller's responsiblity to manage it.

		If you pass 0, then no contraints will be placed on the positioning of the window.
	*/
	void setConstrainer (ComponentBoundsConstrainer* newConstrainer);

	/** Calls the window's setBounds method, after first checking these bounds
		with the current constrainer.

		@see setConstrainer
	*/
	void setBoundsConstrained (const Rectangle<int>& bounds);

	/** Returns true if the window is currently in full-screen mode.

		@see setFullScreen
	*/
	bool isFullScreen() const;

	/** Puts the window into full-screen mode, or restores it to its normal size.

		If true, the window will become full-screen; if false, it will return to the
		last size it was before being made full-screen.

		@see isFullScreen
	*/
	void setFullScreen (bool shouldBeFullScreen);

	/** Returns true if the window is currently minimised.

		@see setMinimised
	*/
	bool isMinimised() const;

	/** Minimises the window, or restores it to its previous position and size.

		When being un-minimised, it'll return to the last position and size it
		was in before being minimised.

		@see isMinimised
	*/
	void setMinimised (bool shouldMinimise);

	/** Adds the window to the desktop using the default flags. */
	void addToDesktop();

	/** Returns a string which encodes the window's current size and position.

		This string will encapsulate the window's size, position, and whether it's
		in full-screen mode. It's intended for letting your application save and
		restore a window's position.

		Use the restoreWindowStateFromString() to restore from a saved state.

		@see restoreWindowStateFromString
	*/
	String getWindowStateAsString();

	/** Restores the window to a previously-saved size and position.

		This restores the window's size, positon and full-screen status from an
		string that was previously created with the getWindowStateAsString()
		method.

		@returns false if the string wasn't a valid window state
		@see getWindowStateAsString
	*/
	bool restoreWindowStateFromString (const String& previousState);

	/** Returns the current content component.

		This will be the component set by setContentOwned() or setContentNonOwned, or 0 if none
		has yet been specified.

		@see setContentOwned, setContentNonOwned
	*/
	Component* getContentComponent() const noexcept                 { return contentComponent; }

	/** Changes the current content component.

		This sets a component that will be placed in the centre of the ResizableWindow,
		(leaving a space around the edge for the border).

		You should never add components directly to a ResizableWindow (or any of its subclasses)
		with addChildComponent(). Instead, add them to the content component.

		@param newContentComponent  the new component to use - this component will be deleted when it's
									no longer needed (i.e. when the window is deleted or a new content
									component is set for it). To set a component that this window will not
									delete, call setContentNonOwned() instead.
		@param resizeToFitWhenContentChangesSize  if true, then the ResizableWindow will maintain its size
									such that it always fits around the size of the content component. If false,
									the new content will be resized to fit the current space available.
	*/
	void setContentOwned (Component* newContentComponent,
						  bool resizeToFitWhenContentChangesSize);

	/** Changes the current content component.

		This sets a component that will be placed in the centre of the ResizableWindow,
		(leaving a space around the edge for the border).

		You should never add components directly to a ResizableWindow (or any of its subclasses)
		with addChildComponent(). Instead, add them to the content component.

		@param newContentComponent  the new component to use - this component will NOT be deleted by this
									component, so it's the caller's responsibility to manage its lifetime (it's
									ok to delete it while this window is still using it). To set a content
									component that the window will delete, call setContentOwned() instead.
		@param resizeToFitWhenContentChangesSize  if true, then the ResizableWindow will maintain its size
									such that it always fits around the size of the content component. If false,
									the new content will be resized to fit the current space available.
	*/
	void setContentNonOwned (Component* newContentComponent,
							 bool resizeToFitWhenContentChangesSize);

	/** Removes the current content component.
		If the previous content component was added with setContentOwned(), it will also be deleted. If
		it was added with setContentNonOwned(), it will simply be removed from this component.
	*/
	void clearContentComponent();

	/** Changes the window so that the content component ends up with the specified size.

		This is basically a setSize call on the window, but which adds on the borders,
		so you can specify the content component's target size.
	*/
	void setContentComponentSize (int width, int height);

	/** Returns the width of the frame to use around the window.
		@see getContentComponentBorder
	*/
	virtual BorderSize<int> getBorderThickness();

	/** Returns the insets to use when positioning the content component.
		@see getBorderThickness
	*/
	virtual BorderSize<int> getContentComponentBorder();

	/** A set of colour IDs to use to change the colour of various aspects of the window.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId          = 0x1005700,  /**< A colour to use to fill the window's background. */
	};

	/** @deprecated - use setContentOwned() and setContentNonOwned() instead. */
	JUCE_DEPRECATED (void setContentComponent (Component* newContentComponent,
											   bool deleteOldOne = true,
											   bool resizeToFit = false));

protected:

	/** @internal */
	void paint (Graphics& g);
	/** (if overriding this, make sure you call ResizableWindow::moved() in your subclass) */
	void moved();
	/** (if overriding this, make sure you call ResizableWindow::resized() in your subclass) */
	void resized();
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void lookAndFeelChanged();
	/** @internal */
	void childBoundsChanged (Component* child);
	/** @internal */
	void parentSizeChanged();
	/** @internal */
	void visibilityChanged();
	/** @internal */
	void activeWindowStatusChanged();
	/** @internal */
	int getDesktopWindowStyleFlags() const;

   #if JUCE_DEBUG
	/** Overridden to warn people about adding components directly to this component
		instead of using setContentOwned().

		If you know what you're doing and are sure you really want to add a component, specify
		a base-class method call to Component::addAndMakeVisible(), to side-step this warning.
	*/
	void addChildComponent (Component* child, int zOrder = -1);
	/** Overridden to warn people about adding components directly to this component
		instead of using setContentOwned().

		If you know what you're doing and are sure you really want to add a component, specify
		a base-class method call to Component::addAndMakeVisible(), to side-step this warning.
	*/
	void addAndMakeVisible (Component* child, int zOrder = -1);
   #endif

	ScopedPointer <ResizableCornerComponent> resizableCorner;
	ScopedPointer <ResizableBorderComponent> resizableBorder;

private:

	Component::SafePointer <Component> contentComponent;
	bool ownsContentComponent, resizeToFitContent, fullscreen;
	ComponentDragger dragger;
	Rectangle<int> lastNonFullScreenPos;
	ComponentBoundsConstrainer defaultConstrainer;
	ComponentBoundsConstrainer* constrainer;
	#if JUCE_DEBUG
	bool hasBeenResized;
	#endif

	void initialise (bool addToDesktop);
	void updateLastPos();
	void setContent (Component*, bool takeOwnership, bool resizeToFit);

   #if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// The parameters for these methods have changed - please update your code!
	JUCE_DEPRECATED (void getBorderThickness (int& left, int& top, int& right, int& bottom));
	JUCE_DEPRECATED (void getContentComponentBorder (int& left, int& top, int& right, int& bottom));
   #endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResizableWindow);
};

#endif   // __JUCE_RESIZABLEWINDOW_JUCEHEADER__

/*** End of inlined file: juce_ResizableWindow.h ***/


/*** Start of inlined file: juce_AlertWindow.h ***/
#ifndef __JUCE_ALERTWINDOW_JUCEHEADER__
#define __JUCE_ALERTWINDOW_JUCEHEADER__


/*** Start of inlined file: juce_ProgressBar.h ***/
#ifndef __JUCE_PROGRESSBAR_JUCEHEADER__
#define __JUCE_PROGRESSBAR_JUCEHEADER__

/**
	A progress bar component.

	To use this, just create one and make it visible. It'll run its own timer
	to keep an eye on a variable that you give it, and will automatically
	redraw itself when the variable changes.

	For an easy way of running a background task with a dialog box showing its
	progress, see the ThreadWithProgressWindow class.

	@see ThreadWithProgressWindow
*/
class JUCE_API  ProgressBar  : public Component,
							   public SettableTooltipClient,
							   private Timer
{
public:

	/** Creates a ProgressBar.

		@param progress     pass in a reference to a double that you're going to
							update with your task's progress. The ProgressBar will
							monitor the value of this variable and will redraw itself
							when the value changes. The range is from 0 to 1.0. Obviously
							you'd better be careful not to delete this variable while the
							ProgressBar still exists!
	*/
	explicit ProgressBar (double& progress);

	/** Destructor. */
	~ProgressBar();

	/** Turns the percentage display on or off.

		By default this is on, and the progress bar will display a text string showing
		its current percentage.
	*/
	void setPercentageDisplay (bool shouldDisplayPercentage);

	/** Gives the progress bar a string to display inside it.

		If you call this, it will turn off the percentage display.
		@see setPercentageDisplay
	*/
	void setTextToDisplay (const String& text);

	/** A set of colour IDs to use to change the colour of various aspects of the bar.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId              = 0x1001900,    /**< The background colour, behind the bar. */
		foregroundColourId              = 0x1001a00,    /**< The colour to use to draw the bar itself. LookAndFeel
															 classes will probably use variations on this colour. */
	};

protected:

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void lookAndFeelChanged();
	/** @internal */
	void visibilityChanged();
	/** @internal */
	void colourChanged();

private:
	double& progress;
	double currentValue;
	bool displayPercentage;
	String displayedMessage, currentMessage;
	uint32 lastCallbackTime;

	void timerCallback();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgressBar);
};

#endif   // __JUCE_PROGRESSBAR_JUCEHEADER__

/*** End of inlined file: juce_ProgressBar.h ***/

/** A window that displays a message and has buttons for the user to react to it.

	For simple dialog boxes with just a couple of buttons on them, there are
	some static methods for running these.

	For more complex dialogs, an AlertWindow can be created, then it can have some
	buttons and components added to it, and its runModalLoop() method is then used to
	show it. The value returned by runModalLoop() shows which button the
	user pressed to dismiss the box.

	@see ThreadWithProgressWindow
*/
class JUCE_API  AlertWindow  : public TopLevelWindow,
							   private ButtonListener  // (can't use Button::Listener due to idiotic VC2005 bug)
{
public:

	/** The type of icon to show in the dialog box. */
	enum AlertIconType
	{
		NoIcon,         /**< No icon will be shown on the dialog box. */
		QuestionIcon,   /**< A question-mark icon, for dialog boxes that need the
							 user to answer a question. */
		WarningIcon,    /**< An exclamation mark to indicate that the dialog is a
							 warning about something and shouldn't be ignored. */
		InfoIcon        /**< An icon that indicates that the dialog box is just
							 giving the user some information, which doesn't require
							 a response from them. */
	};

	/** Creates an AlertWindow.

		@param title    the headline to show at the top of the dialog box
		@param message  a longer, more descriptive message to show underneath the
						headline
		@param iconType the type of icon to display
		@param associatedComponent   if this is non-null, it specifies the component that the
						alert window should be associated with. Depending on the look
						and feel, this might be used for positioning of the alert window.
	*/
	AlertWindow (const String& title,
				 const String& message,
				 AlertIconType iconType,
				 Component* associatedComponent = nullptr);

	/** Destroys the AlertWindow */
	~AlertWindow();

	/** Returns the type of alert icon that was specified when the window
		was created. */
	AlertIconType getAlertType() const noexcept             { return alertIconType; }

	/** Changes the dialog box's message.

		This will also resize the window to fit the new message if required.
	*/
	void setMessage (const String& message);

	/** Adds a button to the window.

		@param name         the text to show on the button
		@param returnValue  the value that should be returned from runModalLoop()
							if this is the button that the user presses.
		@param shortcutKey1 an optional key that can be pressed to trigger this button
		@param shortcutKey2 a second optional key that can be pressed to trigger this button
	*/
	void addButton (const String& name,
					int returnValue,
					const KeyPress& shortcutKey1 = KeyPress(),
					const KeyPress& shortcutKey2 = KeyPress());

	/** Returns the number of buttons that the window currently has. */
	int getNumButtons() const;

	/** Invokes a click of one of the buttons. */
	void triggerButtonClick (const String& buttonName);

	/** If set to true and the window contains no buttons, then pressing the escape key will make
		the alert cancel its modal state.
		By default this setting is true - turn it off if you don't want the box to respond to
		the escape key. Note that it is ignored if you have any buttons, and in that case you
		should give the buttons appropriate keypresses to trigger cancelling if you want to.
	*/
	void setEscapeKeyCancels (bool shouldEscapeKeyCancel);

	/** Adds a textbox to the window for entering strings.

		@param name             an internal name for the text-box. This is the name to pass to
								the getTextEditorContents() method to find out what the
								user typed-in.
		@param initialContents  a string to show in the text box when it's first shown
		@param onScreenLabel    if this is non-empty, it will be displayed next to the
								text-box to label it.
		@param isPasswordBox    if true, the text editor will display asterisks instead of
								the actual text
		@see getTextEditorContents
	*/
	void addTextEditor (const String& name,
						const String& initialContents,
						const String& onScreenLabel = String::empty,
						bool isPasswordBox = false);

	/** Returns the contents of a named textbox.

		After showing an AlertWindow that contains a text editor, this can be
		used to find out what the user has typed into it.

		@param nameOfTextEditor     the name of the text box that you're interested in
		@see addTextEditor
	*/
	String getTextEditorContents (const String& nameOfTextEditor) const;

	/** Returns a pointer to a textbox that was added with addTextEditor(). */
	TextEditor* getTextEditor (const String& nameOfTextEditor) const;

	/** Adds a drop-down list of choices to the box.

		After the box has been shown, the getComboBoxComponent() method can
		be used to find out which item the user picked.

		@param name     the label to use for the drop-down list
		@param items    the list of items to show in it
		@param onScreenLabel    if this is non-empty, it will be displayed next to the
								combo-box to label it.
		@see getComboBoxComponent
	*/
	void addComboBox (const String& name,
					  const StringArray& items,
					  const String& onScreenLabel = String::empty);

	/** Returns a drop-down list that was added to the AlertWindow.

		@param nameOfList   the name that was passed into the addComboBox() method
							when creating the drop-down
		@returns the ComboBox component, or 0 if none was found for the given name.
	*/
	ComboBox* getComboBoxComponent (const String& nameOfList) const;

	/** Adds a block of text.

		This is handy for adding a multi-line note next to a textbox or combo-box,
		to provide more details about what's going on.
	*/
	void addTextBlock (const String& text);

	/** Adds a progress-bar to the window.

		@param progressValue    a variable that will be repeatedly checked while the
								dialog box is visible, to see how far the process has
								got. The value should be in the range 0 to 1.0
	*/
	void addProgressBarComponent (double& progressValue);

	/** Adds a user-defined component to the dialog box.

		@param component    the component to add - its size should be set up correctly
							before it is passed in. The caller is responsible for deleting
							the component later on - the AlertWindow won't delete it.
	*/
	void addCustomComponent (Component* component);

	/** Returns the number of custom components in the dialog box.

		@see getCustomComponent, addCustomComponent
	*/
	int getNumCustomComponents() const;

	/** Returns one of the custom components in the dialog box.

		@param index    a value 0 to (getNumCustomComponents() - 1). Out-of-range indexes
						will return 0
		@see getNumCustomComponents, addCustomComponent
	*/
	Component* getCustomComponent (int index) const;

	/** Removes one of the custom components in the dialog box.

		Note that this won't delete it, it just removes the component from the window

		@param index    a value 0 to (getNumCustomComponents() - 1). Out-of-range indexes
						will return 0
		@returns        the component that was removed (or null)
		@see getNumCustomComponents, addCustomComponent
	*/
	Component* removeCustomComponent (int index);

	/** Returns true if the window contains any components other than just buttons.*/
	bool containsAnyExtraComponents() const;

	// easy-to-use message box functions:

	/** Shows a dialog box that just has a message and a single button to get rid of it.

		If the callback parameter is null, the box is shown modally, and the method will
		block until the user has clicked the button (or pressed the escape or return keys).
		If the callback parameter is non-null, the box will be displayed and placed into a
		modal state, but this method will return immediately, and the callback will be invoked
		later when the user dismisses the box.

		@param iconType     the type of icon to show
		@param title        the headline to show at the top of the box
		@param message      a longer, more descriptive message to show underneath the
							headline
		@param buttonText   the text to show in the button - if this string is empty, the
							default string "ok" (or a localised version) will be used.
		@param associatedComponent   if this is non-null, it specifies the component that the
							alert window should be associated with. Depending on the look
							and feel, this might be used for positioning of the alert window.
	*/
   #if JUCE_MODAL_LOOPS_PERMITTED
	static void JUCE_CALLTYPE showMessageBox (AlertIconType iconType,
											  const String& title,
											  const String& message,
											  const String& buttonText = String::empty,
											  Component* associatedComponent = nullptr);
   #endif

	/** Shows a dialog box that just has a message and a single button to get rid of it.

		If the callback parameter is null, the box is shown modally, and the method will
		block until the user has clicked the button (or pressed the escape or return keys).
		If the callback parameter is non-null, the box will be displayed and placed into a
		modal state, but this method will return immediately, and the callback will be invoked
		later when the user dismisses the box.

		@param iconType     the type of icon to show
		@param title        the headline to show at the top of the box
		@param message      a longer, more descriptive message to show underneath the
							headline
		@param buttonText   the text to show in the button - if this string is empty, the
							default string "ok" (or a localised version) will be used.
		@param associatedComponent   if this is non-null, it specifies the component that the
							alert window should be associated with. Depending on the look
							and feel, this might be used for positioning of the alert window.
	*/
	static void JUCE_CALLTYPE showMessageBoxAsync (AlertIconType iconType,
												   const String& title,
												   const String& message,
												   const String& buttonText = String::empty,
												   Component* associatedComponent = nullptr);

	/** Shows a dialog box with two buttons.

		Ideal for ok/cancel or yes/no choices. The return key can also be used
		to trigger the first button, and the escape key for the second button.

		If the callback parameter is null, the box is shown modally, and the method will
		block until the user has clicked the button (or pressed the escape or return keys).
		If the callback parameter is non-null, the box will be displayed and placed into a
		modal state, but this method will return immediately, and the callback will be invoked
		later when the user dismisses the box.

		@param iconType     the type of icon to show
		@param title        the headline to show at the top of the box
		@param message      a longer, more descriptive message to show underneath the
							headline
		@param button1Text  the text to show in the first button - if this string is
							empty, the default string "ok" (or a localised version of it)
							will be used.
		@param button2Text  the text to show in the second button - if this string is
							empty, the default string "cancel" (or a localised version of it)
							will be used.
		@param associatedComponent   if this is non-null, it specifies the component that the
							alert window should be associated with. Depending on the look
							and feel, this might be used for positioning of the alert window.
		@param callback     if this is non-null, the menu will be launched asynchronously,
							returning immediately, and the callback will receive a call to its
							modalStateFinished() when the box is dismissed, with its parameter
							being 1 if the ok button was pressed, or 0 for cancel, The callback object
							will be owned and deleted by the system, so make sure that it works
							safely and doesn't keep any references to objects that might be deleted
							before it gets called.
		@returns true if button 1 was clicked, false if it was button 2. If the callback parameter
				 is not null, the method always returns false, and the user's choice is delivered
				 later by the callback.
	*/
	static bool JUCE_CALLTYPE showOkCancelBox (AlertIconType iconType,
											   const String& title,
											   const String& message,
											#if JUCE_MODAL_LOOPS_PERMITTED
											   const String& button1Text = String::empty,
											   const String& button2Text = String::empty,
											   Component* associatedComponent = nullptr,
											   ModalComponentManager::Callback* callback = nullptr);
											#else
											   const String& button1Text,
											   const String& button2Text,
											   Component* associatedComponent,
											   ModalComponentManager::Callback* callback);
											#endif

	/** Shows a dialog box with three buttons.

		Ideal for yes/no/cancel boxes.

		The escape key can be used to trigger the third button.

		If the callback parameter is null, the box is shown modally, and the method will
		block until the user has clicked the button (or pressed the escape or return keys).
		If the callback parameter is non-null, the box will be displayed and placed into a
		modal state, but this method will return immediately, and the callback will be invoked
		later when the user dismisses the box.

		@param iconType     the type of icon to show
		@param title        the headline to show at the top of the box
		@param message      a longer, more descriptive message to show underneath the
							headline
		@param button1Text  the text to show in the first button - if an empty string, then
							"yes" will be used (or a localised version of it)
		@param button2Text  the text to show in the first button - if an empty string, then
							"no" will be used (or a localised version of it)
		@param button3Text  the text to show in the first button - if an empty string, then
							"cancel" will be used (or a localised version of it)
		@param associatedComponent   if this is non-null, it specifies the component that the
							alert window should be associated with. Depending on the look
							and feel, this might be used for positioning of the alert window.
		@param callback     if this is non-null, the menu will be launched asynchronously,
							returning immediately, and the callback will receive a call to its
							modalStateFinished() when the box is dismissed, with its parameter
							being 1 if the "yes" button was pressed, 2 for the "no" button, or 0
							if it was cancelled, The callback object will be owned and deleted by the
							system, so make sure that it works safely and doesn't keep any references
							to objects that might be deleted before it gets called.

		@returns If the callback parameter has been set, this returns 0. Otherwise, it
				 returns one of the following values:
				 - 0 if the third button was pressed (normally used for 'cancel')
				 - 1 if the first button was pressed (normally used for 'yes')
				 - 2 if the middle button was pressed (normally used for 'no')
	*/
	static int JUCE_CALLTYPE showYesNoCancelBox (AlertIconType iconType,
												 const String& title,
												 const String& message,
											   #if JUCE_MODAL_LOOPS_PERMITTED
												 const String& button1Text = String::empty,
												 const String& button2Text = String::empty,
												 const String& button3Text = String::empty,
												 Component* associatedComponent = nullptr,
												 ModalComponentManager::Callback* callback = nullptr);
											   #else
												 const String& button1Text,
												 const String& button2Text,
												 const String& button3Text,
												 Component* associatedComponent,
												 ModalComponentManager::Callback* callback);
											   #endif

	/** Shows an operating-system native dialog box.

		@param title        the title to use at the top
		@param bodyText     the longer message to show
		@param isOkCancel   if true, this will show an ok/cancel box, if false,
							it'll show a box with just an ok button
		@returns true if the ok button was pressed, false if they pressed cancel.
	*/
   #if JUCE_MODAL_LOOPS_PERMITTED
	static bool JUCE_CALLTYPE showNativeDialogBox (const String& title,
												   const String& bodyText,
												   bool isOkCancel);
   #endif

	/** A set of colour IDs to use to change the colour of various aspects of the alert box.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId          = 0x1001800,  /**< The background colour for the window. */
		textColourId                = 0x1001810,  /**< The colour for the text. */
		outlineColourId             = 0x1001820   /**< An optional colour to use to draw a border around the window. */
	};

protected:

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	bool keyPressed (const KeyPress& key);
	/** @internal */
	void buttonClicked (Button* button);
	/** @internal */
	void lookAndFeelChanged();
	/** @internal */
	void userTriedToCloseWindow();
	/** @internal */
	int getDesktopWindowStyleFlags() const;

private:

	String text;
	TextLayout textLayout;
	AlertIconType alertIconType;
	ComponentBoundsConstrainer constrainer;
	ComponentDragger dragger;
	Rectangle<int> textArea;
	OwnedArray<TextButton> buttons;
	OwnedArray<TextEditor> textBoxes;
	OwnedArray<ComboBox> comboBoxes;
	OwnedArray<ProgressBar> progressBars;
	Array<Component*> customComps;
	OwnedArray<Component> textBlocks;
	Array<Component*> allComps;
	StringArray textboxNames, comboBoxNames;
	Component* associatedComponent;
	bool escapeKeyCancels;

	void updateLayout (bool onlyIncreaseSize);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlertWindow);
};

#endif   // __JUCE_ALERTWINDOW_JUCEHEADER__

/*** End of inlined file: juce_AlertWindow.h ***/

/**
	A file open/save dialog box.

	This is a Juce-based file dialog box; to use a native file chooser, see the
	FileChooser class.

	To use one of these, create it and call its show() method. e.g.

	@code
	{
		WildcardFileFilter wildcardFilter ("*.foo", String::empty, "Foo files");

		FileBrowserComponent browser (FileBrowserComponent::canSelectFiles,
									  File::nonexistent,
									  &wildcardFilter,
									  nullptr);

		FileChooserDialogBox dialogBox ("Open some kind of file",
										"Please choose some kind of file that you want to open...",
										browser,
										false,
										Colours::lightgrey);

		if (dialogBox.show())
		{
			File selectedFile = browser.getSelectedFile (0);

			...etc..
		}
	}
	@endcode

	@see FileChooser
*/
class JUCE_API  FileChooserDialogBox : public ResizableWindow,
									   public ButtonListener,  // (can't use Button::Listener due to idiotic VC2005 bug)
									   public FileBrowserListener
{
public:

	/** Creates a file chooser box.

		@param title            the main title to show at the top of the box
		@param instructions     an optional longer piece of text to show below the title in
								a smaller font, describing in more detail what's required.
		@param browserComponent a FileBrowserComponent that will be shown inside this dialog
								box. Make sure you delete this after (but not before!) the
								dialog box has been deleted.
		@param warnAboutOverwritingExistingFiles     if true, then the user will be asked to confirm
								if they try to select a file that already exists. (This
								flag is only used when saving files)
		@param backgroundColour the background colour for the top level window

		@see FileBrowserComponent, FilePreviewComponent
	*/
	FileChooserDialogBox (const String& title,
						  const String& instructions,
						  FileBrowserComponent& browserComponent,
						  bool warnAboutOverwritingExistingFiles,
						  const Colour& backgroundColour);

	/** Destructor. */
	~FileChooserDialogBox();

   #if JUCE_MODAL_LOOPS_PERMITTED
	/** Displays and runs the dialog box modally.

		This will show the box with the specified size, returning true if the user
		pressed 'ok', or false if they cancelled.

		Leave the width or height as 0 to use the default size
	*/
	bool show (int width = 0, int height = 0);

	/** Displays and runs the dialog box modally.

		This will show the box with the specified size at the specified location,
		returning true if the user pressed 'ok', or false if they cancelled.

		Leave the width or height as 0 to use the default size.
	*/
	bool showAt (int x, int y, int width, int height);
   #endif

	/** Sets the size of this dialog box to its default and positions it either in the
		centre of the screen, or centred around a component that is provided.
	*/
	void centreWithDefaultSize (Component* componentToCentreAround = 0);

	/** A set of colour IDs to use to change the colour of various aspects of the box.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		titleTextColourId      = 0x1000850, /**< The colour to use to draw the box's title. */
	};

	/** @internal */
	void buttonClicked (Button*);
	/** @internal */
	void closeButtonPressed();
	/** @internal */
	void selectionChanged();
	/** @internal */
	void fileClicked (const File&, const MouseEvent&);
	/** @internal */
	void fileDoubleClicked (const File&);
	/** @internal */
	void browserRootChanged (const File&);

private:
	class ContentComponent;
	ContentComponent* content;
	const bool warnAboutOverwritingExistingFiles;

	void okButtonPressed();
	void createNewFolder();
	void createNewFolderConfirmed (const String& name);

	static void okToOverwriteFileCallback (int result, FileChooserDialogBox*);
	static void createNewFolderCallback (int result, FileChooserDialogBox*, Component::SafePointer<AlertWindow>);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileChooserDialogBox);
};

#endif   // __JUCE_FILECHOOSERDIALOGBOX_JUCEHEADER__

/*** End of inlined file: juce_FileChooserDialogBox.h ***/


#endif
#ifndef __JUCE_FILEFILTER_JUCEHEADER__

#endif
#ifndef __JUCE_FILELISTCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_FileListComponent.h ***/
#ifndef __JUCE_FILELISTCOMPONENT_JUCEHEADER__
#define __JUCE_FILELISTCOMPONENT_JUCEHEADER__


/*** Start of inlined file: juce_ListBox.h ***/
#ifndef __JUCE_LISTBOX_JUCEHEADER__
#define __JUCE_LISTBOX_JUCEHEADER__

/**
	A subclass of this is used to drive a ListBox.

	@see ListBox
*/
class JUCE_API  ListBoxModel
{
public:

	/** Destructor. */
	virtual ~ListBoxModel()  {}

	/** This has to return the number of items in the list.

		@see ListBox::getNumRows()
	*/
	virtual int getNumRows() = 0;

	/** This method must be implemented to draw a row of the list.
	*/
	virtual void paintListBoxItem (int rowNumber,
								   Graphics& g,
								   int width, int height,
								   bool rowIsSelected) = 0;

	/** This is used to create or update a custom component to go in a row of the list.

		Any row may contain a custom component, or can just be drawn with the paintListBoxItem() method
		and handle mouse clicks with listBoxItemClicked().

		This method will be called whenever a custom component might need to be updated - e.g.
		when the table is changed, or TableListBox::updateContent() is called.

		If you don't need a custom component for the specified row, then return nullptr.
		(Bear in mind that even if you're not creating a new component, you may still need to
		delete existingComponentToUpdate if it's non-null).

		If you do want a custom component, and the existingComponentToUpdate is null, then
		this method must create a suitable new component and return it.

		If the existingComponentToUpdate is non-null, it will be a pointer to a component previously created
		by this method. In this case, the method must either update it to make sure it's correctly representing
		the given row (which may be different from the one that the component was created for), or it can
		delete this component and return a new one.

		The component that your method returns will be deleted by the ListBox when it is no longer needed.
	*/
	virtual Component* refreshComponentForRow (int rowNumber, bool isRowSelected,
											   Component* existingComponentToUpdate);

	/** This can be overridden to react to the user clicking on a row.

		@see listBoxItemDoubleClicked
	*/
	virtual void listBoxItemClicked (int row, const MouseEvent& e);

	/** This can be overridden to react to the user double-clicking on a row.

		@see listBoxItemClicked
	*/
	virtual void listBoxItemDoubleClicked (int row, const MouseEvent& e);

	/** This can be overridden to react to the user double-clicking on a part of the list where
		there are no rows.

		@see listBoxItemClicked
	*/
	virtual void backgroundClicked();

	/** Override this to be informed when rows are selected or deselected.

		This will be called whenever a row is selected or deselected. If a range of
		rows is selected all at once, this will just be called once for that event.

		@param lastRowSelected      the last row that the user selected. If no
									rows are currently selected, this may be -1.
	*/
	virtual void selectedRowsChanged (int lastRowSelected);

	/** Override this to be informed when the delete key is pressed.

		If no rows are selected when they press the key, this won't be called.

		@param lastRowSelected   the last row that had been selected when they pressed the
								 key - if there are multiple selections, this might not be
								 very useful
	*/
	virtual void deleteKeyPressed (int lastRowSelected);

	/** Override this to be informed when the return key is pressed.

		If no rows are selected when they press the key, this won't be called.

		@param lastRowSelected   the last row that had been selected when they pressed the
								 key - if there are multiple selections, this might not be
								  very useful
	*/
	virtual void returnKeyPressed (int lastRowSelected);

	/** Override this to be informed when the list is scrolled.

		This might be caused by the user moving the scrollbar, or by programmatic changes
		to the list position.
	*/
	virtual void listWasScrolled();

	/** To allow rows from your list to be dragged-and-dropped, implement this method.

		If this returns a non-null variant then when the user drags a row, the listbox will
		try to find a DragAndDropContainer in its parent hierarchy, and will use it to trigger
		a drag-and-drop operation, using this string as the source description, with the listbox
		itself as the source component.

		@see DragAndDropContainer::startDragging
	*/
	virtual var getDragSourceDescription (const SparseSet<int>& currentlySelectedRows);

	/** You can override this to provide tool tips for specific rows.
		@see TooltipClient
	*/
	virtual String getTooltipForRow (int row);
};

/**
	A list of items that can be scrolled vertically.

	To create a list, you'll need to create a subclass of ListBoxModel. This can
	either paint each row of the list and respond to events via callbacks, or for
	more specialised tasks, it can supply a custom component to fill each row.

	@see ComboBox, TableListBox
*/
class JUCE_API  ListBox  : public Component,
						   public SettableTooltipClient
{
public:

	/** Creates a ListBox.

		The model pointer passed-in can be null, in which case you can set it later
		with setModel().
	*/
	ListBox (const String& componentName = String::empty,
			 ListBoxModel* model = nullptr);

	/** Destructor. */
	~ListBox();

	/** Changes the current data model to display. */
	void setModel (ListBoxModel* newModel);

	/** Returns the current list model. */
	ListBoxModel* getModel() const noexcept                     { return model; }

	/** Causes the list to refresh its content.

		Call this when the number of rows in the list changes, or if you want it
		to call refreshComponentForRow() on all the row components.

		This must only be called from the main message thread.
	*/
	void updateContent();

	/** Turns on multiple-selection of rows.

		By default this is disabled.

		When your row component gets clicked you'll need to call the
		selectRowsBasedOnModifierKeys() method to tell the list that it's been
		clicked and to get it to do the appropriate selection based on whether
		the ctrl/shift keys are held down.
	*/
	void setMultipleSelectionEnabled (bool shouldBeEnabled);

	/** Makes the list react to mouse moves by selecting the row that the mouse if over.

		This function is here primarily for the ComboBox class to use, but might be
		useful for some other purpose too.
	*/
	void setMouseMoveSelectsRows (bool shouldSelect);

	/** Selects a row.

		If the row is already selected, this won't do anything.

		@param rowNumber                the row to select
		@param dontScrollToShowThisRow  if true, the list's position won't change; if false and
										the selected row is off-screen, it'll scroll to make
										sure that row is on-screen
		@param deselectOthersFirst      if true and there are multiple selections, these will
										first be deselected before this item is selected
		@see isRowSelected, selectRowsBasedOnModifierKeys, flipRowSelection, deselectRow,
			 deselectAllRows, selectRangeOfRows
	*/
	void selectRow (int rowNumber,
					bool dontScrollToShowThisRow = false,
					bool deselectOthersFirst = true);

	/** Selects a set of rows.

		This will add these rows to the current selection, so you might need to
		clear the current selection first with deselectAllRows()

		@param firstRow     the first row to select (inclusive)
		@param lastRow      the last row to select (inclusive)
	*/
	void selectRangeOfRows (int firstRow,
							int lastRow);

	/** Deselects a row.

		If it's not currently selected, this will do nothing.

		@see selectRow, deselectAllRows
	*/
	void deselectRow (int rowNumber);

	/** Deselects any currently selected rows.

		@see deselectRow
	*/
	void deselectAllRows();

	/** Selects or deselects a row.

		If the row's currently selected, this deselects it, and vice-versa.
	*/
	void flipRowSelection (int rowNumber);

	/** Returns a sparse set indicating the rows that are currently selected.
		@see setSelectedRows
	*/
	SparseSet<int> getSelectedRows() const;

	/** Sets the rows that should be selected, based on an explicit set of ranges.

		If sendNotificationEventToModel is true, the ListBoxModel::selectedRowsChanged()
		method will be called. If it's false, no notification will be sent to the model.

		@see getSelectedRows
	*/
	void setSelectedRows (const SparseSet<int>& setOfRowsToBeSelected,
						  bool sendNotificationEventToModel = true);

	/** Checks whether a row is selected.
	*/
	bool isRowSelected (int rowNumber) const;

	/** Returns the number of rows that are currently selected.

		@see getSelectedRow, isRowSelected, getLastRowSelected
	*/
	int getNumSelectedRows() const;

	/** Returns the row number of a selected row.

		This will return the row number of the Nth selected row. The row numbers returned will
		be sorted in order from low to high.

		@param index    the index of the selected row to return, (from 0 to getNumSelectedRows() - 1)
		@returns        the row number, or -1 if the index was out of range or if there aren't any rows
						selected
		@see getNumSelectedRows, isRowSelected, getLastRowSelected
	*/
	int getSelectedRow (int index = 0) const;

	/** Returns the last row that the user selected.

		This isn't the same as the highest row number that is currently selected - if the user
		had multiply-selected rows 10, 5 and then 6 in that order, this would return 6.

		If nothing is selected, it will return -1.
	*/
	int getLastRowSelected() const;

	/** Multiply-selects rows based on the modifier keys.

		If no modifier keys are down, this will select the given row and
		deselect any others.

		If the ctrl (or command on the Mac) key is down, it'll flip the
		state of the selected row.

		If the shift key is down, it'll select up to the given row from the
		last row selected.

		@see selectRow
	*/
	void selectRowsBasedOnModifierKeys (int rowThatWasClickedOn,
										const ModifierKeys& modifiers,
										bool isMouseUpEvent);

	/** Scrolls the list to a particular position.

		The proportion is between 0 and 1.0, so 0 scrolls to the top of the list,
		1.0 scrolls to the bottom.

		If the total number of rows all fit onto the screen at once, then this
		method won't do anything.

		@see getVerticalPosition
	*/
	void setVerticalPosition (double newProportion);

	/** Returns the current vertical position as a proportion of the total.

		This can be used in conjunction with setVerticalPosition() to save and restore
		the list's position. It returns a value in the range 0 to 1.

		@see setVerticalPosition
	*/
	double getVerticalPosition() const;

	/** Scrolls if necessary to make sure that a particular row is visible.
	*/
	void scrollToEnsureRowIsOnscreen (int row);

	/** Returns a pointer to the vertical scrollbar. */
	ScrollBar* getVerticalScrollBar() const noexcept;

	/** Returns a pointer to the horizontal scrollbar. */
	ScrollBar* getHorizontalScrollBar() const noexcept;

	/** Finds the row index that contains a given x,y position.
		The position is relative to the ListBox's top-left.
		If no row exists at this position, the method will return -1.
		@see getComponentForRowNumber
	*/
	int getRowContainingPosition (int x, int y) const noexcept;

	/** Finds a row index that would be the most suitable place to insert a new
		item for a given position.

		This is useful when the user is e.g. dragging and dropping onto the listbox,
		because it lets you easily choose the best position to insert the item that
		they drop, based on where they drop it.

		If the position is out of range, this will return -1. If the position is
		beyond the end of the list, it will return getNumRows() to indicate the end
		of the list.

		@see getComponentForRowNumber
	*/
	int getInsertionIndexForPosition (int x, int y) const noexcept;

	/** Returns the position of one of the rows, relative to the top-left of
		the listbox.

		This may be off-screen, and the range of the row number that is passed-in is
		not checked to see if it's a valid row.
	*/
	Rectangle<int> getRowPosition (int rowNumber,
								   bool relativeToComponentTopLeft) const noexcept;

	/** Finds the row component for a given row in the list.

		The component returned will have been created using createRowComponent().

		If the component for this row is off-screen or if the row is out-of-range,
		this will return 0.

		@see getRowContainingPosition
	*/
	Component* getComponentForRowNumber (int rowNumber) const noexcept;

	/** Returns the row number that the given component represents.
		If the component isn't one of the list's rows, this will return -1.
	*/
	int getRowNumberOfComponent (Component* rowComponent) const noexcept;

	/** Returns the width of a row (which may be less than the width of this component
		if there's a scrollbar).
	*/
	int getVisibleRowWidth() const noexcept;

	/** Sets the height of each row in the list.
		The default height is 22 pixels.
		@see getRowHeight
	*/
	void setRowHeight (int newHeight);

	/** Returns the height of a row in the list.
		@see setRowHeight
	*/
	int getRowHeight() const noexcept                   { return rowHeight; }

	/** Returns the number of rows actually visible.

		This is the number of whole rows which will fit on-screen, so the value might
		be more than the actual number of rows in the list.
	*/
	int getNumRowsOnScreen() const noexcept;

	/** A set of colour IDs to use to change the colour of various aspects of the label.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId      = 0x1002800, /**< The background colour to fill the list with.
												  Make this transparent if you don't want the background to be filled. */
		outlineColourId         = 0x1002810, /**< An optional colour to use to draw a border around the list.
												  Make this transparent to not have an outline. */
		textColourId            = 0x1002820  /**< The preferred colour to use for drawing text in the listbox. */
	};

	/** Sets the thickness of a border that will be drawn around the box.

		To set the colour of the outline, use @code setColour (ListBox::outlineColourId, colourXYZ); @endcode
		@see outlineColourId
	*/
	void setOutlineThickness (int outlineThickness);

	/** Returns the thickness of outline that will be drawn around the listbox.

		@see setOutlineColour
	*/
	int getOutlineThickness() const noexcept            { return outlineThickness; }

	/** Sets a component that the list should use as a header.

		This will position the given component at the top of the list, maintaining the
		height of the component passed-in, but rescaling it horizontally to match the
		width of the items in the listbox.

		The component will be deleted when setHeaderComponent() is called with a
		different component, or when the listbox is deleted.
	*/
	void setHeaderComponent (Component* newHeaderComponent);

	/** Changes the width of the rows in the list.

		This can be used to make the list's row components wider than the list itself - the
		width of the rows will be either the width of the list or this value, whichever is
		greater, and if the rows become wider than the list, a horizontal scrollbar will
		appear.

		The default value for this is 0, which means that the rows will always
		be the same width as the list.
	*/
	void setMinimumContentWidth (int newMinimumWidth);

	/** Returns the space currently available for the row items, taking into account
		borders, scrollbars, etc.
	*/
	int getVisibleContentWidth() const noexcept;

	/** Repaints one of the rows.

		This does not invoke updateContent(), it just invokes a straightforward repaint
		for the area covered by this row.
	*/
	void repaintRow (int rowNumber) noexcept;

	/** This fairly obscure method creates an image that just shows the currently
		selected row components.

		It's a handy method for doing drag-and-drop, as it can be passed to the
		DragAndDropContainer for use as the drag image.

		Note that it will make the row components temporarily invisible, so if you're
		using custom components this could affect them if they're sensitive to that
		sort of thing.

		@see Component::createComponentSnapshot
	*/
	virtual Image createSnapshotOfSelectedRows (int& x, int& y);

	/** Returns the viewport that this ListBox uses.

		You may need to use this to change parameters such as whether scrollbars
		are shown, etc.
	*/
	Viewport* getViewport() const noexcept;

	struct Ids
	{
		static const Identifier rowHeight, borderThickness;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);

	/** @internal */
	bool keyPressed (const KeyPress&);
	/** @internal */
	bool keyStateChanged (bool isKeyDown);
	/** @internal */
	void paint (Graphics&);
	/** @internal */
	void paintOverChildren (Graphics&);
	/** @internal */
	void resized();
	/** @internal */
	void visibilityChanged();
	/** @internal */
	void mouseWheelMove (const MouseEvent&, float wheelIncrementX, float wheelIncrementY);
	/** @internal */
	void mouseMove (const MouseEvent&);
	/** @internal */
	void mouseExit (const MouseEvent&);
	/** @internal */
	void mouseUp (const MouseEvent&);
	/** @internal */
	void colourChanged();
	/** @internal */
	void startDragAndDrop (const MouseEvent&, const var& dragDescription, bool allowDraggingToOtherWindows);

private:

	class ListViewport;
	friend class ListViewport;
	friend class TableListBox;
	ListBoxModel* model;
	ScopedPointer<ListViewport> viewport;
	ScopedPointer<Component> headerComponent;
	int totalItems, rowHeight, minimumRowWidth;
	int outlineThickness;
	int lastRowSelected;
	bool mouseMoveSelects, multipleSelection, hasDoneInitialUpdate;
	SparseSet <int> selected;

	void selectRowInternal (int rowNumber, bool dontScrollToShowThisRow,
							bool deselectOthersFirst, bool isMouseClick);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ListBox);
};

#endif   // __JUCE_LISTBOX_JUCEHEADER__

/*** End of inlined file: juce_ListBox.h ***/


/*** Start of inlined file: juce_TreeView.h ***/
#ifndef __JUCE_TREEVIEW_JUCEHEADER__
#define __JUCE_TREEVIEW_JUCEHEADER__

class TreeView;

/**
	An item in a treeview.

	A TreeViewItem can either be a leaf-node in the tree, or it can contain its
	own sub-items.

	To implement an item that contains sub-items, override the itemOpennessChanged()
	method so that when it is opened, it adds the new sub-items to itself using the
	addSubItem method. Depending on the nature of the item it might choose to only
	do this the first time it's opened, or it might want to refresh itself each time.
	It also has the option of deleting its sub-items when it is closed, or leaving them
	in place.
*/
class JUCE_API  TreeViewItem
{
public:

	/** Constructor. */
	TreeViewItem();

	/** Destructor. */
	virtual ~TreeViewItem();

	/** Returns the number of sub-items that have been added to this item.

		Note that this doesn't mean much if the node isn't open.

		@see getSubItem, mightContainSubItems, addSubItem
	*/
	int getNumSubItems() const noexcept;

	/** Returns one of the item's sub-items.

		Remember that the object returned might get deleted at any time when its parent
		item is closed or refreshed, depending on the nature of the items you're using.

		@see getNumSubItems
	*/
	TreeViewItem* getSubItem (int index) const noexcept;

	/** Removes any sub-items. */
	void clearSubItems();

	/** Adds a sub-item.

		@param newItem  the object to add to the item's sub-item list. Once added, these can be
						found using getSubItem(). When the items are later removed with
						removeSubItem() (or when this item is deleted), they will be deleted.
		@param insertPosition   the index which the new item should have when it's added. If this
								value is less than 0, the item will be added to the end of the list.
	*/
	void addSubItem (TreeViewItem* newItem, int insertPosition = -1);

	/** Removes one of the sub-items.

		@param index        the item to remove
		@param deleteItem   if true, the item that is removed will also be deleted.
	*/
	void removeSubItem (int index, bool deleteItem = true);

	/** Returns the TreeView to which this item belongs. */
	TreeView* getOwnerView() const noexcept             { return ownerView; }

	/** Returns the item within which this item is contained. */
	TreeViewItem* getParentItem() const noexcept        { return parentItem; }

	/** True if this item is currently open in the treeview. */
	bool isOpen() const noexcept;

	/** Opens or closes the item.

		When opened or closed, the item's itemOpennessChanged() method will be called,
		and a subclass should use this callback to create and add any sub-items that
		it needs to.

		@see itemOpennessChanged, mightContainSubItems
	*/
	void setOpen (bool shouldBeOpen);

	/** True if this item is currently selected.

		Use this when painting the node, to decide whether to draw it as selected or not.
	*/
	bool isSelected() const noexcept;

	/** Selects or deselects the item.

		This will cause a callback to itemSelectionChanged()
	*/
	void setSelected (bool shouldBeSelected,
					  bool deselectOtherItemsFirst);

	/** Returns the rectangle that this item occupies.

		If relativeToTreeViewTopLeft is true, the co-ordinates are relative to the
		top-left of the TreeView comp, so this will depend on the scroll-position of
		the tree. If false, it is relative to the top-left of the topmost item in the
		tree (so this would be unaffected by scrolling the view).
	*/
	Rectangle<int> getItemPosition (bool relativeToTreeViewTopLeft) const noexcept;

	/** Sends a signal to the treeview to make it refresh itself.

		Call this if your items have changed and you want the tree to update to reflect
		this.
	*/
	void treeHasChanged() const noexcept;

	/** Sends a repaint message to redraw just this item.

		Note that you should only call this if you want to repaint a superficial change. If
		you're altering the tree's nodes, you should instead call treeHasChanged().
	*/
	void repaintItem() const;

	/** Returns the row number of this item in the tree.

		The row number of an item will change according to which items are open.

		@see TreeView::getNumRowsInTree(), TreeView::getItemOnRow()
	*/
	int getRowNumberInTree() const noexcept;

	/** Returns true if all the item's parent nodes are open.

		This is useful to check whether the item might actually be visible or not.
	*/
	bool areAllParentsOpen() const noexcept;

	/** Changes whether lines are drawn to connect any sub-items to this item.

		By default, line-drawing is turned on.
	*/
	void setLinesDrawnForSubItems (bool shouldDrawLines) noexcept;

	/** Tells the tree whether this item can potentially be opened.

		If your item could contain sub-items, this should return true; if it returns
		false then the tree will not try to open the item. This determines whether or
		not the item will be drawn with a 'plus' button next to it.
	*/
	virtual bool mightContainSubItems() = 0;

	/** Returns a string to uniquely identify this item.

		If you're planning on using the TreeView::getOpennessState() method, then
		these strings will be used to identify which nodes are open. The string
		should be unique amongst the item's sibling items, but it's ok for there
		to be duplicates at other levels of the tree.

		If you're not going to store the state, then it's ok not to bother implementing
		this method.
	*/
	virtual String getUniqueName() const;

	/** Called when an item is opened or closed.

		When setOpen() is called and the item has specified that it might
		have sub-items with the mightContainSubItems() method, this method
		is called to let the item create or manage its sub-items.

		So when this is called with isNowOpen set to true (i.e. when the item is being
		opened), a subclass might choose to use clearSubItems() and addSubItem() to
		refresh its sub-item list.

		When this is called with isNowOpen set to false, the subclass might want
		to use clearSubItems() to save on space, or it might choose to leave them,
		depending on the nature of the tree.

		You could also use this callback as a trigger to start a background process
		which asynchronously creates sub-items and adds them, if that's more
		appropriate for the task in hand.

		@see mightContainSubItems
	*/
	virtual void itemOpennessChanged (bool isNowOpen);

	/** Must return the width required by this item.

		If your item needs to have a particular width in pixels, return that value; if
		you'd rather have it just fill whatever space is available in the treeview,
		return -1.

		If all your items return -1, no horizontal scrollbar will be shown, but if any
		items have fixed widths and extend beyond the width of the treeview, a
		scrollbar will appear.

		Each item can be a different width, but if they change width, you should call
		treeHasChanged() to update the tree.
	*/
	virtual int getItemWidth() const                                { return -1; }

	/** Must return the height required by this item.

		This is the height in pixels that the item will take up. Items in the tree
		can be different heights, but if they change height, you should call
		treeHasChanged() to update the tree.
	*/
	virtual int getItemHeight() const                               { return 20; }

	/** You can override this method to return false if you don't want to allow the
		user to select this item.
	*/
	virtual bool canBeSelected() const                              { return true; }

	/** Creates a component that will be used to represent this item.

		You don't have to implement this method - if it returns 0 then no component
		will be used for the item, and you can just draw it using the paintItem()
		callback. But if you do return a component, it will be positioned in the
		treeview so that it can be used to represent this item.

		The component returned will be managed by the treeview, so always return
		a new component, and don't keep a reference to it, as the treeview will
		delete it later when it goes off the screen or is no longer needed. Also
		bear in mind that if the component keeps a reference to the item that
		created it, that item could be deleted before the component. Its position
		and size will be completely managed by the tree, so don't attempt to move it
		around.

		Something you may want to do with your component is to give it a pointer to
		the TreeView that created it. This is perfectly safe, and there's no danger
		of it becoming a dangling pointer because the TreeView will always delete
		the component before it is itself deleted.

		As long as you stick to these rules you can return whatever kind of
		component you like. It's most useful if you're doing things like drag-and-drop
		of items, or want to use a Label component to edit item names, etc.
	*/
	virtual Component* createItemComponent()                        { return nullptr; }

	/** Draws the item's contents.

		You can choose to either implement this method and draw each item, or you
		can use createItemComponent() to create a component that will represent the
		item.

		If all you need in your tree is to be able to draw the items and detect when
		the user selects or double-clicks one of them, it's probably enough to
		use paintItem(), itemClicked() and itemDoubleClicked(). If you need more
		complicated interactions, you may need to use createItemComponent() instead.

		@param g        the graphics context to draw into
		@param width    the width of the area available for drawing
		@param height   the height of the area available for drawing
	*/
	virtual void paintItem (Graphics& g, int width, int height);

	/** Draws the item's open/close button.

		If you don't implement this method, the default behaviour is to
		call LookAndFeel::drawTreeviewPlusMinusBox(), but you can override
		it for custom effects.
	*/
	virtual void paintOpenCloseButton (Graphics& g, int width, int height, bool isMouseOver);

	/** Called when the user clicks on this item.

		If you're using createItemComponent() to create a custom component for the
		item, the mouse-clicks might not make it through to the treeview, but this
		is how you find out about clicks when just drawing each item individually.

		The associated mouse-event details are passed in, so you can find out about
		which button, where it was, etc.

		@see itemDoubleClicked
	*/
	virtual void itemClicked (const MouseEvent& e);

	/** Called when the user double-clicks on this item.

		If you're using createItemComponent() to create a custom component for the
		item, the mouse-clicks might not make it through to the treeview, but this
		is how you find out about clicks when just drawing each item individually.

		The associated mouse-event details are passed in, so you can find out about
		which button, where it was, etc.

		If not overridden, the base class method here will open or close the item as
		if the 'plus' button had been clicked.

		@see itemClicked
	*/
	virtual void itemDoubleClicked (const MouseEvent& e);

	/** Called when the item is selected or deselected.

		Use this if you want to do something special when the item's selectedness
		changes. By default it'll get repainted when this happens.
	*/
	virtual void itemSelectionChanged (bool isNowSelected);

	/** The item can return a tool tip string here if it wants to.
		@see TooltipClient
	*/
	virtual String getTooltip();

	/** To allow items from your treeview to be dragged-and-dropped, implement this method.

		If this returns a non-null variant then when the user drags an item, the treeview will
		try to find a DragAndDropContainer in its parent hierarchy, and will use it to trigger
		a drag-and-drop operation, using this string as the source description, with the treeview
		itself as the source component.

		If you need more complex drag-and-drop behaviour, you can use custom components for
		the items, and use those to trigger the drag.

		To accept drag-and-drop in your tree, see isInterestedInDragSource(),
		isInterestedInFileDrag(), etc.

		@see DragAndDropContainer::startDragging
	*/
	virtual var getDragSourceDescription();

	/** If you want your item to be able to have files drag-and-dropped onto it, implement this
		method and return true.

		If you return true and allow some files to be dropped, you'll also need to implement the
		filesDropped() method to do something with them.

		Note that this will be called often, so make your implementation very quick! There's
		certainly no time to try opening the files and having a think about what's inside them!

		For responding to internal drag-and-drop of other types of object, see isInterestedInDragSource().
		@see FileDragAndDropTarget::isInterestedInFileDrag, isInterestedInDragSource
	*/
	virtual bool isInterestedInFileDrag (const StringArray& files);

	/** When files are dropped into this item, this callback is invoked.

		For this to work, you'll need to have also implemented isInterestedInFileDrag().
		The insertIndex value indicates where in the list of sub-items the files were dropped.
		If files are dropped onto an area of the tree where there are no visible items, this
		method is called on the root item of the tree, with an insert index of 0.
		@see FileDragAndDropTarget::filesDropped, isInterestedInFileDrag
	*/
	virtual void filesDropped (const StringArray& files, int insertIndex);

	/** If you want your item to act as a DragAndDropTarget, implement this method and return true.

		If you implement this method, you'll also need to implement itemDropped() in order to handle
		the items when they are dropped.
		To respond to drag-and-drop of files from external applications, see isInterestedInFileDrag().
		@see DragAndDropTarget::isInterestedInDragSource, itemDropped
	*/
	virtual bool isInterestedInDragSource (const DragAndDropTarget::SourceDetails& dragSourceDetails);

	/** When a things are dropped into this item, this callback is invoked.

		For this to work, you need to have also implemented isInterestedInDragSource().
		The insertIndex value indicates where in the list of sub-items the new items should be placed.
		If files are dropped onto an area of the tree where there are no visible items, this
		method is called on the root item of the tree, with an insert index of 0.
		@see isInterestedInDragSource, DragAndDropTarget::itemDropped
	*/
	virtual void itemDropped (const DragAndDropTarget::SourceDetails& dragSourceDetails, int insertIndex);

	/** Sets a flag to indicate that the item wants to be allowed
		to draw all the way across to the left edge of the treeview.

		By default this is false, which means that when the paintItem()
		method is called, its graphics context is clipped to only allow
		drawing within the item's rectangle. If this flag is set to true,
		then the graphics context isn't clipped on its left side, so it
		can draw all the way across to the left margin. Note that the
		context will still have its origin in the same place though, so
		the coordinates of anything to its left will be negative. It's
		mostly useful if you want to draw a wider bar behind the
		highlighted item.
	*/
	void setDrawsInLeftMargin (bool canDrawInLeftMargin) noexcept;

	/** Saves the current state of open/closed nodes so it can be restored later.

		This takes a snapshot of which sub-nodes have been explicitly opened or closed,
		and records it as XML. To identify node objects it uses the
		TreeViewItem::getUniqueName() method to create named paths. This
		means that the same state of open/closed nodes can be restored to a
		completely different instance of the tree, as long as it contains nodes
		whose unique names are the same.

		You'd normally want to use TreeView::getOpennessState() rather than call it
		for a specific item, but this can be handy if you need to briefly save the state
		for a section of the tree.

		The caller is responsible for deleting the object that is returned.
		@see TreeView::getOpennessState, restoreOpennessState
	*/
	XmlElement* getOpennessState() const noexcept;

	/** Restores the openness of this item and all its sub-items from a saved state.

		See TreeView::restoreOpennessState for more details.

		You'd normally want to use TreeView::restoreOpennessState() rather than call it
		for a specific item, but this can be handy if you need to briefly save the state
		for a section of the tree.

		@see TreeView::restoreOpennessState, getOpennessState
	*/
	void restoreOpennessState (const XmlElement& xml) noexcept;

	/** Returns the index of this item in its parent's sub-items. */
	int getIndexInParent() const noexcept;

	/** Returns true if this item is the last of its parent's sub-itens. */
	bool isLastOfSiblings() const noexcept;

	/** Creates a string that can be used to uniquely retrieve this item in the tree.

		The string that is returned can be passed to TreeView::findItemFromIdentifierString().
		The string takes the form of a path, constructed from the getUniqueName() of this
		item and all its parents, so these must all be correctly implemented for it to work.
		@see TreeView::findItemFromIdentifierString, getUniqueName
	*/
	String getItemIdentifierString() const;

	/**
		This handy class takes a copy of a TreeViewItem's openness when you create it,
		and restores that openness state when its destructor is called.

		This can very handy when you're refreshing sub-items - e.g.
		@code
		void MyTreeViewItem::updateChildItems()
		{
			OpennessRestorer openness (*this);  //  saves the openness state here..

			clearSubItems();

			// add a bunch of sub-items here which may or may not be the same as the ones that
			// were previously there
			addSubItem (...

			// ..and at this point, the old openness is restored, so any items that haven't
			// changed will have their old openness retained.
		}
		@endcode
	*/
	class OpennessRestorer
	{
	public:
		OpennessRestorer (TreeViewItem& treeViewItem);
		~OpennessRestorer();

	private:
		TreeViewItem& treeViewItem;
		ScopedPointer <XmlElement> oldOpenness;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpennessRestorer);
	};

private:

	TreeView* ownerView;
	TreeViewItem* parentItem;
	OwnedArray <TreeViewItem> subItems;
	int y, itemHeight, totalHeight, itemWidth, totalWidth;
	int uid;
	bool selected           : 1;
	bool redrawNeeded       : 1;
	bool drawLinesInside    : 1;
	bool drawsInLeftMargin  : 1;
	unsigned int openness   : 2;

	friend class TreeView;
	friend class TreeViewContentComponent;

	void updatePositions (int newY);
	int getIndentX() const noexcept;
	void setOwnerView (TreeView*) noexcept;
	void paintRecursively (Graphics&, int width);
	TreeViewItem* getTopLevelItem() noexcept;
	TreeViewItem* findItemRecursively (int y) noexcept;
	TreeViewItem* getDeepestOpenParentItem() noexcept;
	int getNumRows() const noexcept;
	TreeViewItem* getItemOnRow (int index) noexcept;
	void deselectAllRecursively();
	int countSelectedItemsRecursively (int depth) const noexcept;
	TreeViewItem* getSelectedItemWithIndex (int index) noexcept;
	TreeViewItem* getNextVisibleItem (bool recurse) const noexcept;
	TreeViewItem* findItemFromIdentifierString (const String&);

   #if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// The parameters for these methods have changed - please update your code!
	virtual void isInterestedInDragSource (const String&, Component*) {}
	virtual int itemDropped (const String&, Component*, int) { return 0; }
   #endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TreeViewItem);
};

/**
	A tree-view component.

	Use one of these to hold and display a structure of TreeViewItem objects.

*/
class JUCE_API  TreeView  : public Component,
							public SettableTooltipClient,
							public FileDragAndDropTarget,
							public DragAndDropTarget
{
public:

	/** Creates an empty treeview.

		Once you've got a treeview component, you'll need to give it something to
		display, using the setRootItem() method.
	*/
	TreeView (const String& componentName = String::empty);

	/** Destructor. */
	~TreeView();

	/** Sets the item that is displayed in the treeview.

		A tree has a single root item which contains as many sub-items as it needs. If
		you want the tree to contain a number of root items, you should still use a single
		root item above these, but hide it using setRootItemVisible().

		You can pass in 0 to this method to clear the tree and remove its current root item.

		The object passed in will not be deleted by the treeview, it's up to the caller
		to delete it when no longer needed. BUT make absolutely sure that you don't delete
		this item until you've removed it from the tree, either by calling setRootItem (nullptr),
		or by deleting the tree first. You can also use deleteRootItem() as a quick way
		to delete it.
	*/
	void setRootItem (TreeViewItem* newRootItem);

	/** Returns the tree's root item.

		This will be the last object passed to setRootItem(), or 0 if none has been set.
	*/
	TreeViewItem* getRootItem() const noexcept                      { return rootItem; }

	/** This will remove and delete the current root item.

		It's a convenient way of deleting the item and calling setRootItem (nullptr).
	*/
	void deleteRootItem();

	/** Changes whether the tree's root item is shown or not.

		If the root item is hidden, only its sub-items will be shown in the treeview - this
		lets you make the tree look as if it's got many root items. If it's hidden, this call
		will also make sure the root item is open (otherwise the treeview would look empty).
	*/
	void setRootItemVisible (bool shouldBeVisible);

	/** Returns true if the root item is visible.

		@see setRootItemVisible
	*/
	bool isRootItemVisible() const noexcept                         { return rootItemVisible; }

	/** Sets whether items are open or closed by default.

		Normally, items are closed until the user opens them, but you can use this
		to make them default to being open until explicitly closed.

		@see areItemsOpenByDefault
	*/
	void setDefaultOpenness (bool isOpenByDefault);

	/** Returns true if the tree's items default to being open.

		@see setDefaultOpenness
	*/
	bool areItemsOpenByDefault() const noexcept                     { return defaultOpenness; }

	/** This sets a flag to indicate that the tree can be used for multi-selection.

		You can always select multiple items internally by calling the
		TreeViewItem::setSelected() method, but this flag indicates whether the user
		is allowed to multi-select by clicking on the tree.

		By default it is disabled.

		@see isMultiSelectEnabled
	*/
	void setMultiSelectEnabled (bool canMultiSelect);

	/** Returns whether multi-select has been enabled for the tree.

		@see setMultiSelectEnabled
	*/
	bool isMultiSelectEnabled() const noexcept                      { return multiSelectEnabled; }

	/** Sets a flag to indicate whether to hide the open/close buttons.

		@see areOpenCloseButtonsVisible
	*/
	void setOpenCloseButtonsVisible (bool shouldBeVisible);

	/** Returns whether open/close buttons are shown.

		@see setOpenCloseButtonsVisible
	*/
	bool areOpenCloseButtonsVisible() const noexcept                { return openCloseButtonsVisible; }

	/** Deselects any items that are currently selected. */
	void clearSelectedItems();

	/** Returns the number of items that are currently selected.
		If maximumDepthToSearchTo is >= 0, it lets you specify a maximum depth to which the
		tree will be recursed.
		@see getSelectedItem, clearSelectedItems
	*/
	int getNumSelectedItems (int maximumDepthToSearchTo = -1) const noexcept;

	/** Returns one of the selected items in the tree.

		@param index    the index, 0 to (getNumSelectedItems() - 1)
	*/
	TreeViewItem* getSelectedItem (int index) const noexcept;

	/** Returns the number of rows the tree is using.

		This will depend on which items are open.

		@see TreeViewItem::getRowNumberInTree()
	*/
	int getNumRowsInTree() const;

	/** Returns the item on a particular row of the tree.

		If the index is out of range, this will return 0.

		@see getNumRowsInTree, TreeViewItem::getRowNumberInTree()
	*/
	TreeViewItem* getItemOnRow (int index) const;

	/** Returns the item that contains a given y position.
		The y is relative to the top of the TreeView component.
	*/
	TreeViewItem* getItemAt (int yPosition) const noexcept;

	/** Tries to scroll the tree so that this item is on-screen somewhere. */
	void scrollToKeepItemVisible (TreeViewItem* item);

	/** Returns the treeview's Viewport object. */
	Viewport* getViewport() const noexcept;

	/** Returns the number of pixels by which each nested level of the tree is indented.
		@see setIndentSize
	*/
	int getIndentSize() const noexcept                              { return indentSize; }

	/** Changes the distance by which each nested level of the tree is indented.
		@see getIndentSize
	*/
	void setIndentSize (int newIndentSize);

	/** Searches the tree for an item with the specified identifier.
		The identifer string must have been created by calling TreeViewItem::getItemIdentifierString().
		If no such item exists, this will return false. If the item is found, all of its items
		will be automatically opened.
	*/
	TreeViewItem* findItemFromIdentifierString (const String& identifierString) const;

	/** Saves the current state of open/closed nodes so it can be restored later.

		This takes a snapshot of which nodes have been explicitly opened or closed,
		and records it as XML. To identify node objects it uses the
		TreeViewItem::getUniqueName() method to create named paths. This
		means that the same state of open/closed nodes can be restored to a
		completely different instance of the tree, as long as it contains nodes
		whose unique names are the same.

		The caller is responsible for deleting the object that is returned.

		@param alsoIncludeScrollPosition    if this is true, the state will also
											include information about where the
											tree has been scrolled to vertically,
											so this can also be restored
		@see restoreOpennessState
	*/
	XmlElement* getOpennessState (bool alsoIncludeScrollPosition) const;

	/** Restores a previously saved arrangement of open/closed nodes.

		This will try to restore a snapshot of the tree's state that was created by
		the getOpennessState() method. If any of the nodes named in the original
		XML aren't present in this tree, they will be ignored.

		If restoreStoredSelection is true, it will also try to re-select any items that
		were selected in the stored state.

		@see getOpennessState
	*/
	void restoreOpennessState (const XmlElement& newState,
							   bool restoreStoredSelection);

	/** A set of colour IDs to use to change the colour of various aspects of the treeview.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId            = 0x1000500, /**< A background colour to fill the component with. */
		linesColourId                 = 0x1000501, /**< The colour to draw the lines with.*/
		dragAndDropIndicatorColourId  = 0x1000502  /**< The colour to use for the drag-and-drop target position indicator. */
	};

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	bool keyPressed (const KeyPress& key);
	/** @internal */
	void colourChanged();
	/** @internal */
	void enablementChanged();
	/** @internal */
	bool isInterestedInFileDrag (const StringArray& files);
	/** @internal */
	void fileDragEnter (const StringArray& files, int x, int y);
	/** @internal */
	void fileDragMove (const StringArray& files, int x, int y);
	/** @internal */
	void fileDragExit (const StringArray& files);
	/** @internal */
	void filesDropped (const StringArray& files, int x, int y);
	/** @internal */
	bool isInterestedInDragSource (const SourceDetails&);
	/** @internal */
	void itemDragEnter (const SourceDetails&);
	/** @internal */
	void itemDragMove (const SourceDetails&);
	/** @internal */
	void itemDragExit (const SourceDetails&);
	/** @internal */
	void itemDropped (const SourceDetails&);

private:
	friend class TreeViewItem;
	friend class TreeViewContentComponent;
	class TreeViewport;
	class InsertPointHighlight;
	class TargetGroupHighlight;
	friend class ScopedPointer<TreeViewport>;
	friend class ScopedPointer<InsertPointHighlight>;
	friend class ScopedPointer<TargetGroupHighlight>;
	ScopedPointer<TreeViewport> viewport;
	CriticalSection nodeAlterationLock;
	TreeViewItem* rootItem;
	ScopedPointer<InsertPointHighlight> dragInsertPointHighlight;
	ScopedPointer<TargetGroupHighlight> dragTargetGroupHighlight;
	int indentSize;
	bool defaultOpenness : 1;
	bool needsRecalculating : 1;
	bool rootItemVisible : 1;
	bool multiSelectEnabled : 1;
	bool openCloseButtonsVisible : 1;

	void itemsChanged() noexcept;
	void recalculateIfNeeded();
	void moveSelectedRow (int delta);
	void updateButtonUnderMouse (const MouseEvent&);
	void showDragHighlight (TreeViewItem*, int insertIndex, int x, int y) noexcept;
	void hideDragHighlight() noexcept;
	void handleDrag (const StringArray& files, const SourceDetails&);
	void handleDrop (const StringArray& files, const SourceDetails&);
	TreeViewItem* getInsertPosition (int& x, int& y, int& insertIndex,
									 const StringArray& files, const SourceDetails&) const noexcept;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TreeView);
};

#endif   // __JUCE_TREEVIEW_JUCEHEADER__

/*** End of inlined file: juce_TreeView.h ***/

/**
	A component that displays the files in a directory as a listbox.

	This implements the DirectoryContentsDisplayComponent base class so that
	it can be used in a FileBrowserComponent.

	To attach a listener to it, use its DirectoryContentsDisplayComponent base
	class and the FileBrowserListener class.

	@see DirectoryContentsList, FileTreeComponent
*/
class JUCE_API  FileListComponent  : public ListBox,
									 public DirectoryContentsDisplayComponent,
									 private ListBoxModel,
									 private ChangeListener
{
public:

	/** Creates a listbox to show the contents of a specified directory.
	*/
	FileListComponent (DirectoryContentsList& listToShow);

	/** Destructor. */
	~FileListComponent();

	/** Returns the number of files the user has got selected.
		@see getSelectedFile
	*/
	int getNumSelectedFiles() const;

	/** Returns one of the files that the user has currently selected.
		The index should be in the range 0 to (getNumSelectedFiles() - 1).
		@see getNumSelectedFiles
	*/
	File getSelectedFile (int index = 0) const;

	/** Deselects any files that are currently selected. */
	void deselectAllFiles();

	/** Scrolls to the top of the list. */
	void scrollToTop();

	/** If the specified file is in the list, it will become the only selected item
		(and if the file isn't in the list, all other items will be deselected). */
	void setSelectedFile (const File&);

	/** @internal */
	void changeListenerCallback (ChangeBroadcaster*);
	/** @internal */
	int getNumRows();
	/** @internal */
	void paintListBoxItem (int, Graphics&, int, int, bool);
	/** @internal */
	Component* refreshComponentForRow (int rowNumber, bool isRowSelected, Component* existingComponentToUpdate);
	/** @internal */
	void selectedRowsChanged (int lastRowSelected);
	/** @internal */
	void deleteKeyPressed (int currentSelectedRow);
	/** @internal */
	void returnKeyPressed (int currentSelectedRow);

private:

	File lastDirectory;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileListComponent);
};

#endif   // __JUCE_FILELISTCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_FileListComponent.h ***/


#endif
#ifndef __JUCE_FILENAMECOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_FilenameComponent.h ***/
#ifndef __JUCE_FILENAMECOMPONENT_JUCEHEADER__
#define __JUCE_FILENAMECOMPONENT_JUCEHEADER__

class FilenameComponent;

/**
	Listens for events happening to a FilenameComponent.

	Use FilenameComponent::addListener() and FilenameComponent::removeListener() to
	register one of these objects for event callbacks when the filename is changed.

	@see FilenameComponent
*/
class JUCE_API  FilenameComponentListener
{
public:
	/** Destructor. */
	virtual ~FilenameComponentListener() {}

	/** This method is called after the FilenameComponent's file has been changed. */
	virtual void filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged) = 0;
};

/**
	Shows a filename as an editable text box, with a 'browse' button and a
	drop-down list for recently selected files.

	A handy component for dialogue boxes where you want the user to be able to
	select a file or directory.

	Attach an FilenameComponentListener using the addListener() method, and it will
	get called each time the user changes the filename, either by browsing for a file
	and clicking 'ok', or by typing a new filename into the box and pressing return.

	@see FileChooser, ComboBox
*/
class JUCE_API  FilenameComponent  : public Component,
									 public SettableTooltipClient,
									 public FileDragAndDropTarget,
									 private AsyncUpdater,
									 private ButtonListener,  // (can't use Button::Listener due to idiotic VC2005 bug)
									 private ComboBoxListener
{
public:

	/** Creates a FilenameComponent.

		@param name             the name for this component.
		@param currentFile      the file to initially show in the box
		@param canEditFilename  if true, the user can manually edit the filename; if false,
								they can only change it by browsing for a new file
		@param isDirectory      if true, the file will be treated as a directory, and
								an appropriate directory browser used
		@param isForSaving      if true, the file browser will allow non-existent files to
								be picked, as the file is assumed to be used for saving rather
								than loading
		@param fileBrowserWildcard  a wildcard pattern to use in the file browser - e.g. "*.txt;*.foo".
								If an empty string is passed in, then the pattern is assumed to be "*"
		@param enforcedSuffix   if this is non-empty, it is treated as a suffix that will be added
								to any filenames that are entered or chosen
		@param textWhenNothingSelected  the message to display in the box before any filename is entered. (This
								will only appear if the initial file isn't valid)
	*/
	FilenameComponent (const String& name,
					   const File& currentFile,
					   bool canEditFilename,
					   bool isDirectory,
					   bool isForSaving,
					   const String& fileBrowserWildcard,
					   const String& enforcedSuffix,
					   const String& textWhenNothingSelected);

	/** Destructor. */
	~FilenameComponent();

	/** Returns the currently displayed filename. */
	File getCurrentFile() const;

	/** Changes the current filename.

		If addToRecentlyUsedList is true, the filename will also be added to the
		drop-down list of recent files.

		If sendChangeNotification is false, then the listeners won't be told of the
		change.
	*/
	void setCurrentFile (File newFile,
						 bool addToRecentlyUsedList,
						 bool sendChangeNotification = true);

	/** Changes whether the use can type into the filename box.
	*/
	void setFilenameIsEditable (bool shouldBeEditable);

	/** Sets a file or directory to be the default starting point for the browser to show.

		This is only used if the current file hasn't been set.
	*/
	void setDefaultBrowseTarget (const File& newDefaultDirectory);

	/** Returns all the entries on the recent files list.

		This can be used in conjunction with setRecentlyUsedFilenames() for saving the
		state of this list.

		@see setRecentlyUsedFilenames
	*/
	StringArray getRecentlyUsedFilenames() const;

	/** Sets all the entries on the recent files list.

		This can be used in conjunction with getRecentlyUsedFilenames() for saving the
		state of this list.

		@see getRecentlyUsedFilenames, addRecentlyUsedFile
	*/
	void setRecentlyUsedFilenames (const StringArray& filenames);

	/** Adds an entry to the recently-used files dropdown list.

		If the file is already in the list, it will be moved to the top. A limit
		is also placed on the number of items that are kept in the list.

		@see getRecentlyUsedFilenames, setRecentlyUsedFilenames, setMaxNumberOfRecentFiles
	*/
	void addRecentlyUsedFile (const File& file);

	/** Changes the limit for the number of files that will be stored in the recent-file list.
	*/
	void setMaxNumberOfRecentFiles (int newMaximum);

	/** Changes the text shown on the 'browse' button.

		By default this button just says "..." but you can change it. The button itself
		can be changed using the look-and-feel classes, so it might not actually have any
		text on it.
	*/
	void setBrowseButtonText (const String& browseButtonText);

	/** Adds a listener that will be called when the selected file is changed. */
	void addListener (FilenameComponentListener* listener);

	/** Removes a previously-registered listener. */
	void removeListener (FilenameComponentListener* listener);

	/** Gives the component a tooltip. */
	void setTooltip (const String& newTooltip);

	/** @internal */
	void paintOverChildren (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void lookAndFeelChanged();
	/** @internal */
	bool isInterestedInFileDrag (const StringArray& files);
	/** @internal */
	void filesDropped (const StringArray& files, int, int);
	/** @internal */
	void fileDragEnter (const StringArray& files, int, int);
	/** @internal */
	void fileDragExit (const StringArray& files);

private:

	ComboBox filenameBox;
	String lastFilename;
	ScopedPointer<Button> browseButton;
	int maxRecentFiles;
	bool isDir, isSaving, isFileDragOver;
	String wildcard, enforcedSuffix, browseButtonText;
	ListenerList <FilenameComponentListener> listeners;
	File defaultBrowseFile;

	void comboBoxChanged (ComboBox*);
	void buttonClicked (Button* button);
	void handleAsyncUpdate();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilenameComponent);
};

#endif   // __JUCE_FILENAMECOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_FilenameComponent.h ***/


#endif
#ifndef __JUCE_FILEPREVIEWCOMPONENT_JUCEHEADER__

#endif
#ifndef __JUCE_FILESEARCHPATHLISTCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_FileSearchPathListComponent.h ***/
#ifndef __JUCE_FILESEARCHPATHLISTCOMPONENT_JUCEHEADER__
#define __JUCE_FILESEARCHPATHLISTCOMPONENT_JUCEHEADER__

/**
	Shows a set of file paths in a list, allowing them to be added, removed or
	re-ordered.

	@see FileSearchPath
*/
class JUCE_API  FileSearchPathListComponent  : public Component,
											   public SettableTooltipClient,
											   public FileDragAndDropTarget,
											   private ButtonListener,  // (can't use Button::Listener due to idiotic VC2005 bug)
											   private ListBoxModel
{
public:

	/** Creates an empty FileSearchPathListComponent. */
	FileSearchPathListComponent();

	/** Destructor. */
	~FileSearchPathListComponent();

	/** Returns the path as it is currently shown. */
	const FileSearchPath& getPath() const noexcept                  { return path; }

	/** Changes the current path. */
	void setPath (const FileSearchPath& newPath);

	/** Sets a file or directory to be the default starting point for the browser to show.

		This is only used if the current file hasn't been set.
	*/
	void setDefaultBrowseTarget (const File& newDefaultDirectory);

	/** A set of colour IDs to use to change the colour of various aspects of the label.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId      = 0x1004100, /**< The background colour to fill the component with.
												  Make this transparent if you don't want the background to be filled. */
	};

	/** @internal */
	int getNumRows();
	/** @internal */
	void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
	/** @internal */
	void deleteKeyPressed (int lastRowSelected);
	/** @internal */
	void returnKeyPressed (int lastRowSelected);
	/** @internal */
	void listBoxItemDoubleClicked (int row, const MouseEvent&);
	/** @internal */
	void selectedRowsChanged (int lastRowSelected);
	/** @internal */
	void resized();
	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	bool isInterestedInFileDrag (const StringArray& files);
	/** @internal */
	void filesDropped (const StringArray& files, int, int);
	/** @internal */
	void buttonClicked (Button* button);

private:

	FileSearchPath path;
	File defaultBrowseTarget;

	ListBox listBox;
	TextButton addButton, removeButton, changeButton;
	DrawableButton upButton, downButton;

	void changed();
	void updateButtons();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileSearchPathListComponent);
};

#endif   // __JUCE_FILESEARCHPATHLISTCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_FileSearchPathListComponent.h ***/


#endif
#ifndef __JUCE_FILETREECOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_FileTreeComponent.h ***/
#ifndef __JUCE_FILETREECOMPONENT_JUCEHEADER__
#define __JUCE_FILETREECOMPONENT_JUCEHEADER__

/**
	A component that displays the files in a directory as a treeview.

	This implements the DirectoryContentsDisplayComponent base class so that
	it can be used in a FileBrowserComponent.

	To attach a listener to it, use its DirectoryContentsDisplayComponent base
	class and the FileBrowserListener class.

	@see DirectoryContentsList, FileListComponent
*/
class JUCE_API  FileTreeComponent  : public TreeView,
									 public DirectoryContentsDisplayComponent
{
public:

	/** Creates a listbox to show the contents of a specified directory.
	*/
	FileTreeComponent (DirectoryContentsList& listToShow);

	/** Destructor. */
	~FileTreeComponent();

	/** Returns the number of files the user has got selected.
		@see getSelectedFile
	*/
	int getNumSelectedFiles() const                 { return TreeView::getNumSelectedItems(); }

	/** Returns one of the files that the user has currently selected.
		The index should be in the range 0 to (getNumSelectedFiles() - 1).
		@see getNumSelectedFiles
	*/
	File getSelectedFile (int index = 0) const;

	/** Deselects any files that are currently selected. */
	void deselectAllFiles();

	/** Scrolls the list to the top. */
	void scrollToTop();

	/** If the specified file is in the list, it will become the only selected item
		(and if the file isn't in the list, all other items will be deselected). */
	void setSelectedFile (const File&);

	/** Updates the files in the list. */
	void refresh();

	/** Setting a name for this allows tree items to be dragged.

		The string that you pass in here will be returned by the getDragSourceDescription()
		of the items in the tree. For more info, see TreeViewItem::getDragSourceDescription().
	*/
	void setDragAndDropDescription (const String& description);

	/** Returns the last value that was set by setDragAndDropDescription().
	*/
	const String& getDragAndDropDescription() const noexcept     { return dragAndDropDescription; }

private:

	String dragAndDropDescription;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileTreeComponent);
};

#endif   // __JUCE_FILETREECOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_FileTreeComponent.h ***/


#endif
#ifndef __JUCE_IMAGEPREVIEWCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_ImagePreviewComponent.h ***/
#ifndef __JUCE_IMAGEPREVIEWCOMPONENT_JUCEHEADER__
#define __JUCE_IMAGEPREVIEWCOMPONENT_JUCEHEADER__

/**
	A simple preview component that shows thumbnails of image files.

	@see FileChooserDialogBox, FilePreviewComponent
*/
class JUCE_API  ImagePreviewComponent  : public FilePreviewComponent,
										 private Timer
{
public:

	/** Creates an ImagePreviewComponent. */
	ImagePreviewComponent();

	/** Destructor. */
	~ImagePreviewComponent();

	/** @internal */
	void selectedFileChanged (const File& newSelectedFile);
	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void timerCallback();

private:
	File fileToLoad;
	Image currentThumbnail;
	String currentDetails;

	void getThumbSize (int& w, int& h) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImagePreviewComponent);
};

#endif   // __JUCE_IMAGEPREVIEWCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_ImagePreviewComponent.h ***/


#endif
#ifndef __JUCE_WILDCARDFILEFILTER_JUCEHEADER__

/*** Start of inlined file: juce_WildcardFileFilter.h ***/
#ifndef __JUCE_WILDCARDFILEFILTER_JUCEHEADER__
#define __JUCE_WILDCARDFILEFILTER_JUCEHEADER__

/**
	A type of FileFilter that works by wildcard pattern matching.

	This filter only allows files that match one of the specified patterns, but
	allows all directories through.

	@see FileFilter, DirectoryContentsList, FileListComponent, FileBrowserComponent
*/
class JUCE_API  WildcardFileFilter  : public FileFilter
{
public:

	/**
		Creates a wildcard filter for one or more patterns.

		The wildcardPatterns parameter is a comma or semicolon-delimited set of
		patterns, e.g. "*.wav;*.aiff" would look for files ending in either .wav
		or .aiff.

		Passing an empty string as a pattern will fail to match anything, so by leaving
		either the file or directory pattern parameter empty means you can control
		whether files or directories are found.

		The description is a name to show the user in a list of possible patterns, so
		for the wav/aiff example, your description might be "audio files".
	*/
	WildcardFileFilter (const String& fileWildcardPatterns,
						const String& directoryWildcardPatterns,
						const String& description);

	/** Destructor. */
	~WildcardFileFilter();

	/** Returns true if the filename matches one of the patterns specified. */
	bool isFileSuitable (const File& file) const;

	/** This always returns true. */
	bool isDirectorySuitable (const File& file) const;

private:

	StringArray fileWildcards, directoryWildcards;

	static void parse (const String& pattern, StringArray& result);
	static bool match (const File& file, const StringArray& wildcards);

	JUCE_LEAK_DETECTOR (WildcardFileFilter);
};

#endif   // __JUCE_WILDCARDFILEFILTER_JUCEHEADER__

/*** End of inlined file: juce_WildcardFileFilter.h ***/


#endif
#ifndef __JUCE_COMPONENTANIMATOR_JUCEHEADER__

#endif
#ifndef __JUCE_COMPONENTBOUNDSCONSTRAINER_JUCEHEADER__

#endif
#ifndef __JUCE_COMPONENTBUILDER_JUCEHEADER__

#endif
#ifndef __JUCE_COMPONENTMOVEMENTWATCHER_JUCEHEADER__

/*** Start of inlined file: juce_ComponentMovementWatcher.h ***/
#ifndef __JUCE_COMPONENTMOVEMENTWATCHER_JUCEHEADER__
#define __JUCE_COMPONENTMOVEMENTWATCHER_JUCEHEADER__

/** An object that watches for any movement of a component or any of its parent components.

	This makes it easy to check when a component is moved relative to its top-level
	peer window. The normal Component::moved() method is only called when a component
	moves relative to its immediate parent, and sometimes you want to know if any of
	components higher up the tree have moved (which of course will affect the overall
	position of all their sub-components).

	It also includes a callback that lets you know when the top-level peer is changed.

	This class is used by specialised components like WebBrowserComponent or QuickTimeComponent
	because they need to keep their custom windows in the right place and respond to
	changes in the peer.
*/
class JUCE_API  ComponentMovementWatcher    : public ComponentListener
{
public:

	/** Creates a ComponentMovementWatcher to watch a given target component. */
	ComponentMovementWatcher (Component* component);

	/** Destructor. */
	~ComponentMovementWatcher();

	/** This callback happens when the component that is being watched is moved
		relative to its top-level peer window, or when it is resized. */
	virtual void componentMovedOrResized (bool wasMoved, bool wasResized) = 0;

	/** This callback happens when the component's top-level peer is changed. */
	virtual void componentPeerChanged() = 0;

	/** This callback happens when the component's visibility state changes, possibly due to
		one of its parents being made visible or invisible.
	*/
	virtual void componentVisibilityChanged() = 0;

	/** Returns the component that's being watched. */
	Component* getComponent() const noexcept         { return component; }

	/** @internal */
	void componentParentHierarchyChanged (Component& component);
	/** @internal */
	void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);
	/** @internal */
	void componentBeingDeleted (Component& component);
	/** @internal */
	void componentVisibilityChanged (Component& component);

private:

	WeakReference<Component> component;
	uint32 lastPeerID;
	Array <Component*> registeredParentComps;
	bool reentrant, wasShowing;
	Rectangle<int> lastBounds;

	void unregister();
	void registerWithParentComps();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentMovementWatcher);
};

#endif   // __JUCE_COMPONENTMOVEMENTWATCHER_JUCEHEADER__

/*** End of inlined file: juce_ComponentMovementWatcher.h ***/


#endif
#ifndef __JUCE_GROUPCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_GroupComponent.h ***/
#ifndef __JUCE_GROUPCOMPONENT_JUCEHEADER__
#define __JUCE_GROUPCOMPONENT_JUCEHEADER__

/**
	A component that draws an outline around itself and has an optional title at
	the top, for drawing an outline around a group of controls.

*/
class JUCE_API  GroupComponent    : public Component
{
public:

	/** Creates a GroupComponent.

		@param componentName    the name to give the component
		@param labelText        the text to show at the top of the outline
	*/
	GroupComponent (const String& componentName = String::empty,
					const String& labelText = String::empty);

	/** Destructor. */
	~GroupComponent();

	/** Changes the text that's shown at the top of the component. */
	void setText (const String& newText);

	/** Returns the currently displayed text label. */
	String getText() const;

	/** Sets the positioning of the text label.

		(The default is Justification::left)

		@see getTextLabelPosition
	*/
	void setTextLabelPosition (const Justification& justification);

	/** Returns the current text label position.

		@see setTextLabelPosition
	*/
	const Justification getTextLabelPosition() const noexcept           { return justification; }

	/** A set of colour IDs to use to change the colour of various aspects of the component.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		outlineColourId     = 0x1005400,    /**< The colour to use for drawing the line around the edge. */
		textColourId        = 0x1005410     /**< The colour to use to draw the text label. */
	};

	struct Ids
	{
		static const Identifier tagType, text, justification;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void enablementChanged();
	/** @internal */
	void colourChanged();

private:
	String text;
	Justification justification;

	JUCE_DECLARE_NON_COPYABLE (GroupComponent);
};

#endif   // __JUCE_GROUPCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_GroupComponent.h ***/


#endif
#ifndef __JUCE_MULTIDOCUMENTPANEL_JUCEHEADER__

/*** Start of inlined file: juce_MultiDocumentPanel.h ***/
#ifndef __JUCE_MULTIDOCUMENTPANEL_JUCEHEADER__
#define __JUCE_MULTIDOCUMENTPANEL_JUCEHEADER__


/*** Start of inlined file: juce_TabbedComponent.h ***/
#ifndef __JUCE_TABBEDCOMPONENT_JUCEHEADER__
#define __JUCE_TABBEDCOMPONENT_JUCEHEADER__


/*** Start of inlined file: juce_TabbedButtonBar.h ***/
#ifndef __JUCE_TABBEDBUTTONBAR_JUCEHEADER__
#define __JUCE_TABBEDBUTTONBAR_JUCEHEADER__

class TabbedButtonBar;

/** In a TabbedButtonBar, this component is used for each of the buttons.

	If you want to create a TabbedButtonBar with custom tab components, derive
	your component from this class, and override the TabbedButtonBar::createTabButton()
	method to create it instead of the default one.

	@see TabbedButtonBar
*/
class JUCE_API  TabBarButton  : public Button
{
public:

	/** Creates the tab button. */
	TabBarButton (const String& name, TabbedButtonBar& ownerBar);

	/** Destructor. */
	~TabBarButton();

	/** Chooses the best length for the tab, given the specified depth.

		If the tab is horizontal, this should return its width, and the depth
		specifies its height. If it's vertical, it should return the height, and
		the depth is actually its width.
	*/
	virtual int getBestTabLength (int depth);

	void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
	void clicked (const ModifierKeys& mods);
	bool hitTest (int x, int y);

protected:

	friend class TabbedButtonBar;
	TabbedButtonBar& owner;
	int overlapPixels;
	DropShadowEffect shadow;

	/** Returns an area of the component that's safe to draw in.

		This deals with the orientation of the tabs, which affects which side is
		touching the tabbed box's content component.
	*/
	Rectangle<int> getActiveArea();

	/** Returns this tab's index in its tab bar. */
	int getIndex() const;

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabBarButton);
};

/**
	A vertical or horizontal bar containing tabs that you can select.

	You can use one of these to generate things like a dialog box that has
	tabbed pages you can flip between. Attach a ChangeListener to the
	button bar to be told when the user changes the page.

	An easier method than doing this is to use a TabbedComponent, which
	contains its own TabbedButtonBar and which takes care of the layout
	and other housekeeping.

	@see TabbedComponent
*/
class JUCE_API  TabbedButtonBar  : public Component,
								   public ChangeBroadcaster
{
public:

	/** The placement of the tab-bar

		@see setOrientation, getOrientation
	*/
	enum Orientation
	{
		TabsAtTop,
		TabsAtBottom,
		TabsAtLeft,
		TabsAtRight
	};

	/** Creates a TabbedButtonBar with a given placement.

		You can change the orientation later if you need to.
	*/
	TabbedButtonBar (Orientation orientation);

	/** Destructor. */
	~TabbedButtonBar();

	/** Changes the bar's orientation.

		This won't change the bar's actual size - you'll need to do that yourself,
		but this determines which direction the tabs go in, and which side they're
		stuck to.
	*/
	void setOrientation (Orientation orientation);

	/** Returns the current orientation.

		@see setOrientation
	*/
	Orientation getOrientation() const noexcept                     { return orientation; }

	/** Changes the minimum scale factor to which the tabs can be compressed when trying to
		fit a lot of tabs on-screen.
	*/
	void setMinimumTabScaleFactor (double newMinimumScale);

	/** Deletes all the tabs from the bar.

		@see addTab
	*/
	void clearTabs();

	/** Adds a tab to the bar.

		Tabs are added in left-to-right reading order.

		If this is the first tab added, it'll also be automatically selected.
	*/
	void addTab (const String& tabName,
				 const Colour& tabBackgroundColour,
				 int insertIndex = -1);

	/** Changes the name of one of the tabs. */
	void setTabName (int tabIndex,
					 const String& newName);

	/** Gets rid of one of the tabs. */
	void removeTab (int tabIndex);

	/** Moves a tab to a new index in the list.

		Pass -1 as the index to move it to the end of the list.
	*/
	void moveTab (int currentIndex, int newIndex);

	/** Returns the number of tabs in the bar. */
	int getNumTabs() const;

	/** Returns a list of all the tab names in the bar. */
	StringArray getTabNames() const;

	/** Changes the currently selected tab.

		This will send a change message and cause a synchronous callback to
		the currentTabChanged() method. (But if the given tab is already selected,
		nothing will be done).

		To deselect all the tabs, use an index of -1.
	*/
	void setCurrentTabIndex (int newTabIndex, bool sendChangeMessage = true);

	/** Returns the name of the currently selected tab.

		This could be an empty string if none are selected.
	*/
	String getCurrentTabName() const;

	/** Returns the index of the currently selected tab.

		This could return -1 if none are selected.
	*/
	int getCurrentTabIndex() const noexcept                             { return currentTabIndex; }

	/** Returns the button for a specific tab.

		The button that is returned may be deleted later by this component, so don't hang
		on to the pointer that is returned. A null pointer may be returned if the index is
		out of range.
	*/
	TabBarButton* getTabButton (int index) const;

	/** Returns the index of a TabBarButton if it belongs to this bar. */
	int indexOfTabButton (const TabBarButton* button) const;

	/** Callback method to indicate the selected tab has been changed.

		@see setCurrentTabIndex
	*/
	virtual void currentTabChanged (int newCurrentTabIndex,
									const String& newCurrentTabName);

	/** Callback method to indicate that the user has right-clicked on a tab.

		(Or ctrl-clicked on the Mac)
	*/
	virtual void popupMenuClickOnTab (int tabIndex, const String& tabName);

	/** Returns the colour of a tab.

		This is the colour that was specified in addTab().
	*/
	Colour getTabBackgroundColour (int tabIndex);

	/** Changes the background colour of a tab.

		@see addTab, getTabBackgroundColour
	*/
	void setTabBackgroundColour (int tabIndex, const Colour& newColour);

	/** A set of colour IDs to use to change the colour of various aspects of the component.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		tabOutlineColourId              = 0x1005812,    /**< The colour to use to draw an outline around the tabs.  */
		tabTextColourId                 = 0x1005813,    /**< The colour to use to draw the tab names. If this isn't specified,
															 the look and feel will choose an appropriate colour. */
		frontOutlineColourId            = 0x1005814,    /**< The colour to use to draw an outline around the currently-selected tab.  */
		frontTextColourId               = 0x1005815,    /**< The colour to use to draw the currently-selected tab name. If
															 this isn't specified, the look and feel will choose an appropriate
															 colour. */
	};

	/** @internal */
	void resized();
	/** @internal */
	void lookAndFeelChanged();

protected:

	/** This creates one of the tabs.

		If you need to use custom tab components, you can override this method and
		return your own class instead of the default.
	*/
	virtual TabBarButton* createTabButton (const String& tabName, int tabIndex);

private:
	Orientation orientation;

	struct TabInfo
	{
		ScopedPointer<TabBarButton> component;
		String name;
		Colour colour;
	};

	OwnedArray <TabInfo> tabs;

	double minimumScale;
	int currentTabIndex;

	class BehindFrontTabComp;
	friend class BehindFrontTabComp;
	friend class ScopedPointer<BehindFrontTabComp>;
	ScopedPointer<BehindFrontTabComp> behindFrontTab;
	ScopedPointer<Button> extraTabsButton;

	void showExtraItemsMenu();
	static void extraItemsMenuCallback (int, TabbedButtonBar*);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabbedButtonBar);
};

#endif   // __JUCE_TABBEDBUTTONBAR_JUCEHEADER__

/*** End of inlined file: juce_TabbedButtonBar.h ***/

/**
	A component with a TabbedButtonBar along one of its sides.

	This makes it easy to create a set of tabbed pages, just add a bunch of tabs
	with addTab(), and this will take care of showing the pages for you when the
	user clicks on a different tab.

	@see TabbedButtonBar
*/
class JUCE_API  TabbedComponent  : public Component
{
public:

	/** Creates a TabbedComponent, specifying where the tabs should be placed.

		Once created, add some tabs with the addTab() method.
	*/
	explicit TabbedComponent (TabbedButtonBar::Orientation orientation);

	/** Destructor. */
	~TabbedComponent();

	/** Changes the placement of the tabs.

		This will rearrange the layout to place the tabs along the appropriate
		side of this component, and will shift the content component accordingly.

		@see TabbedButtonBar::setOrientation
	*/
	void setOrientation (TabbedButtonBar::Orientation orientation);

	/** Returns the current tab placement.

		@see setOrientation, TabbedButtonBar::getOrientation
	*/
	TabbedButtonBar::Orientation getOrientation() const noexcept;

	/** Specifies how many pixels wide or high the tab-bar should be.

		If the tabs are placed along the top or bottom, this specified the height
		of the bar; if they're along the left or right edges, it'll be the width
		of the bar.
	*/
	void setTabBarDepth (int newDepth);

	/** Returns the current thickness of the tab bar.

		@see setTabBarDepth
	*/
	int getTabBarDepth() const noexcept                         { return tabDepth; }

	/** Specifies the thickness of an outline that should be drawn around the content component.

		If this thickness is > 0, a line will be drawn around the three sides of the content
		component which don't touch the tab-bar, and the content component will be inset by this amount.

		To set the colour of the line, use setColour (outlineColourId, ...).
	*/
	void setOutline (int newThickness);

	/** Specifies a gap to leave around the edge of the content component.

		Each edge of the content component will be indented by the given number of pixels.
	*/
	void setIndent (int indentThickness);

	/** Removes all the tabs from the bar.

		@see TabbedButtonBar::clearTabs
	*/
	void clearTabs();

	/** Adds a tab to the tab-bar.

		The component passed in will be shown for the tab, and if deleteComponentWhenNotNeeded
		is true, it will be deleted when the tab is removed or when this object is
		deleted.

		@see TabbedButtonBar::addTab
	*/
	void addTab (const String& tabName,
				 const Colour& tabBackgroundColour,
				 Component* contentComponent,
				 bool deleteComponentWhenNotNeeded,
				 int insertIndex = -1);

	/** Changes the name of one of the tabs. */
	void setTabName (int tabIndex, const String& newName);

	/** Gets rid of one of the tabs. */
	void removeTab (int tabIndex);

	/** Returns the number of tabs in the bar. */
	int getNumTabs() const;

	/** Returns a list of all the tab names in the bar. */
	StringArray getTabNames() const;

	/** Returns the content component that was added for the given index.

		Be sure not to use or delete the components that are returned, as this may interfere
		with the TabbedComponent's use of them.
	*/
	Component* getTabContentComponent (int tabIndex) const noexcept;

	/** Returns the colour of one of the tabs. */
	Colour getTabBackgroundColour (int tabIndex) const noexcept;

	/** Changes the background colour of one of the tabs. */
	void setTabBackgroundColour (int tabIndex, const Colour& newColour);

	/** Changes the currently-selected tab.

		To deselect all the tabs, pass -1 as the index.

		@see TabbedButtonBar::setCurrentTabIndex
	*/
	void setCurrentTabIndex (int newTabIndex, bool sendChangeMessage = true);

	/** Returns the index of the currently selected tab.

		@see addTab, TabbedButtonBar::getCurrentTabIndex()
	*/
	int getCurrentTabIndex() const;

	/** Returns the name of the currently selected tab.

		@see addTab, TabbedButtonBar::getCurrentTabName()
	*/
	String getCurrentTabName() const;

	/** Returns the current component that's filling the panel.

		This will return 0 if there isn't one.
	*/
	Component* getCurrentContentComponent() const noexcept          { return panelComponent; }

	/** Callback method to indicate the selected tab has been changed.

		@see setCurrentTabIndex
	*/
	virtual void currentTabChanged (int newCurrentTabIndex,
									const String& newCurrentTabName);

	/** Callback method to indicate that the user has right-clicked on a tab.

		(Or ctrl-clicked on the Mac)
	*/
	virtual void popupMenuClickOnTab (int tabIndex,
									  const String& tabName);

	/** Returns the tab button bar component that is being used.
	*/
	TabbedButtonBar& getTabbedButtonBar() const noexcept            { return *tabs; }

	/** A set of colour IDs to use to change the colour of various aspects of the component.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId          = 0x1005800,    /**< The colour to fill the background behind the tabs. */
		outlineColourId             = 0x1005801,    /**< The colour to use to draw an outline around the content.
														 (See setOutline)  */
	};

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void lookAndFeelChanged();

protected:

	/** This creates one of the tab buttons.

		If you need to use custom tab components, you can override this method and
		return your own class instead of the default.
	*/
	virtual TabBarButton* createTabButton (const String& tabName, int tabIndex);

	/** @internal */
	ScopedPointer<TabbedButtonBar> tabs;

private:

	Array <WeakReference<Component> > contentComponents;
	WeakReference<Component> panelComponent;
	int tabDepth;
	int outlineThickness, edgeIndent;

	class ButtonBar;
	friend class ButtonBar;
	void changeCallback (int newCurrentTabIndex, const String& newTabName);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabbedComponent);
};

#endif   // __JUCE_TABBEDCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_TabbedComponent.h ***/


/*** Start of inlined file: juce_DocumentWindow.h ***/
#ifndef __JUCE_DOCUMENTWINDOW_JUCEHEADER__
#define __JUCE_DOCUMENTWINDOW_JUCEHEADER__


/*** Start of inlined file: juce_MenuBarModel.h ***/
#ifndef __JUCE_MENUBARMODEL_JUCEHEADER__
#define __JUCE_MENUBARMODEL_JUCEHEADER__

/**
	A class for controlling MenuBar components.

	This class is used to tell a MenuBar what menus to show, and to respond
	to a menu being selected.

	@see MenuBarModel::Listener, MenuBarComponent, PopupMenu
*/
class JUCE_API  MenuBarModel      : private AsyncUpdater,
									private ApplicationCommandManagerListener
{
public:

	MenuBarModel() noexcept;

	/** Destructor. */
	virtual ~MenuBarModel();

	/** Call this when some of your menu items have changed.

		This method will cause a callback to any MenuBarListener objects that
		are registered with this model.

		If this model is displaying items from an ApplicationCommandManager, you
		can use the setApplicationCommandManagerToWatch() method to cause
		change messages to be sent automatically when the ApplicationCommandManager
		is changed.

		@see addListener, removeListener, MenuBarListener
	*/
	void menuItemsChanged();

	/** Tells the menu bar to listen to the specified command manager, and to update
		itself when the commands change.

		This will also allow it to flash a menu name when a command from that menu
		is invoked using a keystroke.
	*/
	void setApplicationCommandManagerToWatch (ApplicationCommandManager* manager) noexcept;

	/** A class to receive callbacks when a MenuBarModel changes.

		@see MenuBarModel::addListener, MenuBarModel::removeListener, MenuBarModel::menuItemsChanged
	*/
	class JUCE_API  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}

		/** This callback is made when items are changed in the menu bar model.
		*/
		virtual void menuBarItemsChanged (MenuBarModel* menuBarModel) = 0;

		/** This callback is made when an application command is invoked that
			is represented by one of the items in the menu bar model.
		*/
		virtual void menuCommandInvoked (MenuBarModel* menuBarModel,
										 const ApplicationCommandTarget::InvocationInfo& info) = 0;
	};

	/** Registers a listener for callbacks when the menu items in this model change.

		The listener object will get callbacks when this object's menuItemsChanged()
		method is called.

		@see removeListener
	*/
	void addListener (Listener* listenerToAdd) noexcept;

	/** Removes a listener.

		@see addListener
	*/
	void removeListener (Listener* listenerToRemove) noexcept;

	/** This method must return a list of the names of the menus. */
	virtual const StringArray getMenuBarNames() = 0;

	/** This should return the popup menu to display for a given top-level menu.

		@param topLevelMenuIndex    the index of the top-level menu to show
		@param menuName             the name of the top-level menu item to show
	*/
	virtual const PopupMenu getMenuForIndex (int topLevelMenuIndex,
											 const String& menuName) = 0;

	/** This is called when a menu item has been clicked on.

		@param menuItemID           the item ID of the PopupMenu item that was selected
		@param topLevelMenuIndex    the index of the top-level menu from which the item was
									chosen (just in case you've used duplicate ID numbers
									on more than one of the popup menus)
	*/
	virtual void menuItemSelected (int menuItemID,
								   int topLevelMenuIndex) = 0;

   #if JUCE_MAC || DOXYGEN
	/** MAC ONLY - Sets the model that is currently being shown as the main
		menu bar at the top of the screen on the Mac.

		You can pass 0 to stop the current model being displayed. Be careful
		not to delete a model while it is being used.

		An optional extra menu can be specified, containing items to add to the top of
		the apple menu. (Confusingly, the 'apple' menu isn't the one with a picture of
		an apple, it's the one next to it, with your application's name at the top
		and the services menu etc on it). When one of these items is selected, the
		menu bar model will be used to invoke it, and in the menuItemSelected() callback
		the topLevelMenuIndex parameter will be -1. If you pass in an extraAppleMenuItems
		object then newMenuBarModel must be non-null.
	*/
	static void setMacMainMenu (MenuBarModel* newMenuBarModel,
								const PopupMenu* extraAppleMenuItems = nullptr);

	/** MAC ONLY - Returns the menu model that is currently being shown as
		the main menu bar.
	*/
	static MenuBarModel* getMacMainMenu();
   #endif

	/** @internal */
	void applicationCommandInvoked (const ApplicationCommandTarget::InvocationInfo& info);
	/** @internal */
	void applicationCommandListChanged();
	/** @internal */
	void handleAsyncUpdate();

private:
	ApplicationCommandManager* manager;
	ListenerList <Listener> listeners;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuBarModel);
};

/** This typedef is just for compatibility with old code - newer code should use the MenuBarModel::Listener class directly. */
typedef MenuBarModel::Listener MenuBarModelListener;

#endif   // __JUCE_MENUBARMODEL_JUCEHEADER__

/*** End of inlined file: juce_MenuBarModel.h ***/

/**
	A resizable window with a title bar and maximise, minimise and close buttons.

	This subclass of ResizableWindow creates a fairly standard type of window with
	a title bar and various buttons. The name of the component is shown in the
	title bar, and an icon can optionally be specified with setIcon().

	All the methods available to a ResizableWindow are also available to this,
	so it can easily be made resizable, minimised, maximised, etc.

	It's not advisable to add child components directly to a DocumentWindow: put them
	inside your content component instead. And overriding methods like resized(), moved(), etc
	is also not recommended - instead override these methods for your content component.
	(If for some obscure reason you do need to override these methods, always remember to
	call the super-class's resized() method too, otherwise it'll fail to lay out the window
	decorations correctly).

	You can also automatically add a menu bar to the window, using the setMenuBar()
	method.

	@see ResizableWindow, DialogWindow
*/
class JUCE_API  DocumentWindow   : public ResizableWindow
{
public:

	/** The set of available button-types that can be put on the title bar.

		@see setTitleBarButtonsRequired
	*/
	enum TitleBarButtons
	{
		minimiseButton = 1,
		maximiseButton = 2,
		closeButton = 4,

		/** A combination of all the buttons above. */
		allButtons = 7
	};

	/** Creates a DocumentWindow.

		@param name             the name to give the component - this is also
								the title shown at the top of the window. To change
								this later, use setName()
		@param backgroundColour the colour to use for filling the window's background.
		@param requiredButtons  specifies which of the buttons (close, minimise, maximise)
								should be shown on the title bar. This value is a bitwise
								combination of values from the TitleBarButtons enum. Note
								that it can be "allButtons" to get them all. You
								can change this later with the setTitleBarButtonsRequired()
								method, which can also specify where they are positioned.
		@param addToDesktop     if true, the window will be automatically added to the
								desktop; if false, you can use it as a child component
		@see TitleBarButtons
	*/
	DocumentWindow (const String& name,
					const Colour& backgroundColour,
					int requiredButtons,
					bool addToDesktop = true);

	/** Destructor.
		If a content component has been set with setContentOwned(), it will be deleted.
	*/
	~DocumentWindow();

	/** Changes the component's name.

		(This is overridden from Component::setName() to cause a repaint, as
		the name is what gets drawn across the window's title bar).
	*/
	void setName (const String& newName);

	/** Sets an icon to show in the title bar, next to the title.

		A copy is made internally of the image, so the caller can delete the
		image after calling this. If 0 is passed-in, any existing icon will be
		removed.
	*/
	void setIcon (const Image& imageToUse);

	/** Changes the height of the title-bar. */
	void setTitleBarHeight (int newHeight);

	/** Returns the current title bar height. */
	int getTitleBarHeight() const;

	/** Changes the set of title-bar buttons being shown.

		@param requiredButtons  specifies which of the buttons (close, minimise, maximise)
								should be shown on the title bar. This value is a bitwise
								combination of values from the TitleBarButtons enum. Note
								that it can be "allButtons" to get them all.
		@param positionTitleBarButtonsOnLeft    if true, the buttons should go at the
								left side of the bar; if false, they'll be placed at the right
	*/
	void setTitleBarButtonsRequired (int requiredButtons,
									 bool positionTitleBarButtonsOnLeft);

	/** Sets whether the title should be centred within the window.

		If true, the title text is shown in the middle of the title-bar; if false,
		it'll be shown at the left of the bar.
	*/
	void setTitleBarTextCentred (bool textShouldBeCentred);

	/** Creates a menu inside this window.

		@param menuBarModel     this specifies a MenuBarModel that should be used to
								generate the contents of a menu bar that will be placed
								just below the title bar, and just above any content
								component. If this value is zero, any existing menu bar
								will be removed from the component; if non-zero, one will
								be added if it's required.
		@param menuBarHeight    the height of the menu bar component, if one is needed. Pass a value of zero
								or less to use the look-and-feel's default size.
	*/
	void setMenuBar (MenuBarModel* menuBarModel,
					 int menuBarHeight = 0);

	/** Returns the current menu bar component, or null if there isn't one.
		This is probably a MenuBarComponent, unless a custom one has been set using
		setMenuBarComponent().
	*/
	Component* getMenuBarComponent() const noexcept;

	/** Replaces the current menu bar with a custom component.
		The component will be owned and deleted by the document window.
	*/
	void setMenuBarComponent (Component* newMenuBarComponent);

	/** This method is called when the user tries to close the window.

		This is triggered by the user clicking the close button, or using some other
		OS-specific key shortcut or OS menu for getting rid of a window.

		If the window is just a pop-up, you should override this closeButtonPressed()
		method and make it delete the window in whatever way is appropriate for your
		app. E.g. you might just want to call "delete this".

		If your app is centred around this window such that the whole app should quit when
		the window is closed, then you will probably want to use this method as an opportunity
		to call JUCEApplication::quit(), and leave the window to be deleted later by your
		JUCEApplication::shutdown() method. (Doing it this way means that your window will
		still get cleaned-up if the app is quit by some other means (e.g. a cmd-Q on the mac
		or closing it via the taskbar icon on Windows).

		(Note that the DocumentWindow class overrides Component::userTriedToCloseWindow() and
		redirects it to call this method, so any methods of closing the window that are
		caught by userTriedToCloseWindow() will also end up here).
	*/
	virtual void closeButtonPressed();

	/** Callback that is triggered when the minimise button is pressed.

		The default implementation of this calls ResizableWindow::setMinimised(), but
		you can override it to do more customised behaviour.
	*/
	virtual void minimiseButtonPressed();

	/** Callback that is triggered when the maximise button is pressed, or when the
		title-bar is double-clicked.

		The default implementation of this calls ResizableWindow::setFullScreen(), but
		you can override it to do more customised behaviour.
	*/
	virtual void maximiseButtonPressed();

	/** Returns the close button, (or 0 if there isn't one). */
	Button* getCloseButton() const noexcept;

	/** Returns the minimise button, (or 0 if there isn't one). */
	Button* getMinimiseButton() const noexcept;

	/** Returns the maximise button, (or 0 if there isn't one). */
	Button* getMaximiseButton() const noexcept;

	/** A set of colour IDs to use to change the colour of various aspects of the window.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		textColourId                = 0x1005701,  /**< The colour to draw any text with. It's up to the look
													   and feel class how this is used. */
	};

   #ifndef DOXYGEN
	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void lookAndFeelChanged();
	/** @internal */
	BorderSize<int> getBorderThickness();
	/** @internal */
	BorderSize<int> getContentComponentBorder();
	/** @internal */
	void mouseDoubleClick (const MouseEvent& e);
	/** @internal */
	void userTriedToCloseWindow();
	/** @internal */
	void activeWindowStatusChanged();
	/** @internal */
	int getDesktopWindowStyleFlags() const;
	/** @internal */
	void parentHierarchyChanged();
	/** @internal */
	Rectangle<int> getTitleBarArea();
   #endif

private:

	int titleBarHeight, menuBarHeight, requiredButtons;
	bool positionTitleBarButtonsOnLeft, drawTitleTextCentred;
	ScopedPointer <Button> titleBarButtons [3];
	Image titleBarIcon;
	ScopedPointer <Component> menuBar;
	MenuBarModel* menuBarModel;

	class ButtonListenerProxy;
	friend class ScopedPointer <ButtonListenerProxy>;
	ScopedPointer <ButtonListenerProxy> buttonListener;

	void repaintTitleBar();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DocumentWindow);
};

#endif   // __JUCE_DOCUMENTWINDOW_JUCEHEADER__

/*** End of inlined file: juce_DocumentWindow.h ***/

class MultiDocumentPanel;
class MDITabbedComponentInternal;

/**
	This is a derivative of DocumentWindow that is used inside a MultiDocumentPanel
	component.

	It's like a normal DocumentWindow but has some extra functionality to make sure
	everything works nicely inside a MultiDocumentPanel.

	@see MultiDocumentPanel
*/
class JUCE_API  MultiDocumentPanelWindow  : public DocumentWindow
{
public:

	/**
	*/
	MultiDocumentPanelWindow (const Colour& backgroundColour);

	/** Destructor. */
	~MultiDocumentPanelWindow();

	/** @internal */
	void maximiseButtonPressed();
	/** @internal */
	void closeButtonPressed();
	/** @internal */
	void activeWindowStatusChanged();
	/** @internal */
	void broughtToFront();

private:

	void updateOrder();
	MultiDocumentPanel* getOwner() const noexcept;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiDocumentPanelWindow);
};

/**
	A component that contains a set of other components either in floating windows
	or tabs.

	This acts as a panel that can be used to hold a set of open document windows, with
	different layout modes.

	Use addDocument() and closeDocument() to add or remove components from the
	panel - never use any of the Component methods to access the panel's child
	components directly, as these are managed internally.
*/
class JUCE_API  MultiDocumentPanel  : public Component,
									  private ComponentListener
{
public:

	/** Creates an empty panel.

		Use addDocument() and closeDocument() to add or remove components from the
		panel - never use any of the Component methods to access the panel's child
		components directly, as these are managed internally.
	*/
	MultiDocumentPanel();

	/** Destructor.

		When deleted, this will call closeAllDocuments (false) to make sure all its
		components are deleted. If you need to make sure all documents are saved
		before closing, then you should call closeAllDocuments (true) and check that
		it returns true before deleting the panel.
	*/
	~MultiDocumentPanel();

	/** Tries to close all the documents.

		If checkItsOkToCloseFirst is true, then the tryToCloseDocument() method will
		be called for each open document, and any of these calls fails, this method
		will stop and return false, leaving some documents still open.

		If checkItsOkToCloseFirst is false, then all documents will be closed
		unconditionally.

		@see closeDocument
	*/
	bool closeAllDocuments (bool checkItsOkToCloseFirst);

	/** Adds a document component to the panel.

		If the number of documents would exceed the limit set by setMaximumNumDocuments() then
		this will fail and return false. (If it does fail, the component passed-in will not be
		deleted, even if deleteWhenRemoved was set to true).

		The MultiDocumentPanel will deal with creating a window border to go around your component,
		so just pass in the bare content component here, no need to give it a ResizableWindow
		or DocumentWindow.

		@param component            the component to add
		@param backgroundColour     the background colour to use to fill the component's
									window or tab
		@param deleteWhenRemoved    if true, then when the component is removed by closeDocument()
									or closeAllDocuments(), then it will be deleted. If false, then
									the caller must handle the component's deletion
	*/
	bool addDocument (Component* component,
					  const Colour& backgroundColour,
					  bool deleteWhenRemoved);

	/** Closes one of the documents.

		If checkItsOkToCloseFirst is true, then the tryToCloseDocument() method will
		be called, and if it fails, this method will return false without closing the
		document.

		If checkItsOkToCloseFirst is false, then the documents will be closed
		unconditionally.

		The component will be deleted if the deleteWhenRemoved parameter was set to
		true when it was added with addDocument.

		@see addDocument, closeAllDocuments
	*/
	bool closeDocument (Component* component,
						bool checkItsOkToCloseFirst);

	/** Returns the number of open document windows.

		@see getDocument
	*/
	int getNumDocuments() const noexcept;

	/** Returns one of the open documents.

		The order of the documents in this array may change when they are added, removed
		or moved around.

		@see getNumDocuments
	*/
	Component* getDocument (int index) const noexcept;

	/** Returns the document component that is currently focused or on top.

		If currently using floating windows, then this will be the component in the currently
		active window, or the top component if none are active.

		If it's currently in tabbed mode, then it'll return the component in the active tab.

		@see setActiveDocument
	*/
	Component* getActiveDocument() const noexcept;

	/** Makes one of the components active and brings it to the top.

		@see getActiveDocument
	*/
	void setActiveDocument (Component* component);

	/** Callback which gets invoked when the currently-active document changes. */
	virtual void activeDocumentChanged();

	/** Sets a limit on how many windows can be open at once.

		If this is zero or less there's no limit (the default). addDocument() will fail
		if this number is exceeded.
	*/
	void setMaximumNumDocuments (int maximumNumDocuments);

	/** Sets an option to make the document fullscreen if there's only one document open.

		If set to true, then if there's only one document, it'll fill the whole of this
		component without tabs or a window border. If false, then tabs or a window
		will always be shown, even if there's only one document. If there's more than
		one document open, then this option makes no difference.
	*/
	void useFullscreenWhenOneDocument (bool shouldUseTabs);

	/** Returns the result of the last time useFullscreenWhenOneDocument() was called.
	*/
	bool isFullscreenWhenOneDocument() const noexcept;

	/** The different layout modes available. */
	enum LayoutMode
	{
		FloatingWindows,            /**< In this mode, there are overlapping DocumentWindow components for each document. */
		MaximisedWindowsWithTabs    /**< In this mode, a TabbedComponent is used to show one document at a time. */
	};

	/** Changes the panel's mode.

		@see LayoutMode, getLayoutMode
	*/
	void setLayoutMode (LayoutMode newLayoutMode);

	/** Returns the current layout mode. */
	LayoutMode getLayoutMode() const noexcept                           { return mode; }

	/** Sets the background colour for the whole panel.

		Each document has its own background colour, but this is the one used to fill the areas
		behind them.
	*/
	void setBackgroundColour (const Colour& newBackgroundColour);

	/** Returns the current background colour.

		@see setBackgroundColour
	*/
	const Colour& getBackgroundColour() const noexcept                  { return backgroundColour; }

	/** If the panel is being used in tabbed mode, this returns the TabbedComponent that's involved. */
	TabbedComponent* getCurrentTabbedComponent() const noexcept         { return tabComponent; }

	/** A subclass must override this to say whether its currently ok for a document
		to be closed.

		This method is called by closeDocument() and closeAllDocuments() to indicate that
		a document should be saved if possible, ready for it to be closed.

		If this method returns true, then it means the document is ok and can be closed.

		If it returns false, then it means that the closeDocument() method should stop
		and not close.

		Normally, you'd use this method to ask the user if they want to save any changes,
		then return true if the save operation went ok. If the user cancelled the save
		operation you could return false here to abort the close operation.

		If your component is based on the FileBasedDocument class, then you'd probably want
		to call FileBasedDocument::saveIfNeededAndUserAgrees() and return true if this returned
		FileBasedDocument::savedOk

		@see closeDocument, FileBasedDocument::saveIfNeededAndUserAgrees()
	*/
	virtual bool tryToCloseDocument (Component* component) = 0;

	/** Creates a new window to be used for a document.

		The default implementation of this just returns a basic MultiDocumentPanelWindow object,
		but you might want to override it to return a custom component.
	*/
	virtual MultiDocumentPanelWindow* createNewDocumentWindow();

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void componentNameChanged (Component&);

private:

	LayoutMode mode;
	Array <Component*> components;
	ScopedPointer<TabbedComponent> tabComponent;
	Colour backgroundColour;
	int maximumNumDocuments, numDocsBeforeTabsUsed;

	friend class MultiDocumentPanelWindow;
	friend class MDITabbedComponentInternal;

	Component* getContainerComp (Component* c) const;
	void updateOrder();

	void addWindow (Component* component);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiDocumentPanel);
};

#endif   // __JUCE_MULTIDOCUMENTPANEL_JUCEHEADER__

/*** End of inlined file: juce_MultiDocumentPanel.h ***/


#endif
#ifndef __JUCE_RESIZABLEBORDERCOMPONENT_JUCEHEADER__

#endif
#ifndef __JUCE_RESIZABLECORNERCOMPONENT_JUCEHEADER__

#endif
#ifndef __JUCE_RESIZABLEEDGECOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_ResizableEdgeComponent.h ***/
#ifndef __JUCE_RESIZABLEEDGECOMPONENT_JUCEHEADER__
#define __JUCE_RESIZABLEEDGECOMPONENT_JUCEHEADER__

/**
	A component that resizes its parent component when dragged.

	This component forms a bar along one edge of a component, allowing it to
	be dragged by that edges to resize it.

	To use it, just add it to your component, positioning it along the appropriate
	edge. Make sure you reposition the resizer component each time the parent's size
	changes, to keep it in the correct position.

	@see ResizbleBorderComponent, ResizableCornerComponent
*/
class JUCE_API  ResizableEdgeComponent  : public Component
{
public:

	enum Edge
	{
		leftEdge,   /**< Indicates a vertical bar that can be dragged left/right to move the component's left-hand edge. */
		rightEdge,  /**< Indicates a vertical bar that can be dragged left/right to move the component's right-hand edge. */
		topEdge,    /**< Indicates a horizontal bar that can be dragged up/down to move the top of the component. */
		bottomEdge  /**< Indicates a horizontal bar that can be dragged up/down to move the bottom of the component. */
	};

	/** Creates a resizer bar.

		Pass in the target component which you want to be resized when this one is
		dragged. The target component will usually be this component's parent, but this
		isn't mandatory.

		Remember that when the target component is resized, it'll need to move and
		resize this component to keep it in place, as this won't happen automatically.

		If the constrainer parameter is non-zero, then this object will be used to enforce
		limits on the size and position that the component can be stretched to. Make sure
		that the constrainer isn't deleted while still in use by this object.

		@see ComponentBoundsConstrainer
	*/
	ResizableEdgeComponent (Component* componentToResize,
							ComponentBoundsConstrainer* constrainer,
							Edge edgeToResize);

	/** Destructor. */
	~ResizableEdgeComponent();

	bool isVertical() const noexcept;

protected:

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseUp (const MouseEvent& e);

private:
	WeakReference<Component> component;
	ComponentBoundsConstrainer* constrainer;
	Rectangle<int> originalBounds;
	const Edge edge;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResizableEdgeComponent);
};

#endif   // __JUCE_RESIZABLEEDGECOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_ResizableEdgeComponent.h ***/


#endif
#ifndef __JUCE_SCROLLBAR_JUCEHEADER__

#endif
#ifndef __JUCE_STRETCHABLELAYOUTMANAGER_JUCEHEADER__

/*** Start of inlined file: juce_StretchableLayoutManager.h ***/
#ifndef __JUCE_STRETCHABLELAYOUTMANAGER_JUCEHEADER__
#define __JUCE_STRETCHABLELAYOUTMANAGER_JUCEHEADER__

/**
	For laying out a set of components, where the components have preferred sizes
	and size limits, but where they are allowed to stretch to fill the available
	space.

	For example, if you have a component containing several other components, and
	each one should be given a share of the total size, you could use one of these
	to resize the child components when the parent component is resized. Then
	you could add a StretchableLayoutResizerBar to easily let the user rescale them.

	A StretchableLayoutManager operates only in one dimension, so if you have a set
	of components stacked vertically on top of each other, you'd use one to manage their
	heights. To build up complex arrangements of components, e.g. for applications
	with multiple nested panels, you would use more than one StretchableLayoutManager.
	E.g. by using two (one vertical, one horizontal), you could create a resizable
	spreadsheet-style table.

	E.g.
	@code
	class MyComp  : public Component
	{
		StretchableLayoutManager myLayout;

		MyComp()
		{
			myLayout.setItemLayout (0,          // for item 0
									50, 100,    // must be between 50 and 100 pixels in size
									-0.6);      // and its preferred size is 60% of the total available space

			myLayout.setItemLayout (1,          // for item 1
									-0.2, -0.6, // size must be between 20% and 60% of the available space
									50);        // and its preferred size is 50 pixels
		}

		void resized()
		{
			// make a list of two of our child components that we want to reposition
			Component* comps[] = { myComp1, myComp2 };

			// this will position the 2 components, one above the other, to fit
			// vertically into the rectangle provided.
			myLayout.layOutComponents (comps, 2,
									   0, 0, getWidth(), getHeight(),
									   true);
		}
	};
	@endcode

	@see StretchableLayoutResizerBar
*/
class JUCE_API  StretchableLayoutManager
{
public:

	/** Creates an empty layout.

		You'll need to add some item properties to the layout before it can be used
		to resize things - see setItemLayout().
	*/
	StretchableLayoutManager();

	/** Destructor. */
	~StretchableLayoutManager();

	/** For a numbered item, this sets its size limits and preferred size.

		@param itemIndex        the index of the item to change.
		@param minimumSize      the minimum size that this item is allowed to be - a positive number
								indicates an absolute size in pixels. A negative number indicates a
								proportion of the available space (e.g -0.5 is 50%)
		@param maximumSize      the maximum size that this item is allowed to be - a positive number
								indicates an absolute size in pixels. A negative number indicates a
								proportion of the available space
		@param preferredSize    the size that this item would like to be, if there's enough room. A
								positive number indicates an absolute size in pixels. A negative number
								indicates a proportion of the available space
		@see getItemLayout
	*/
	void setItemLayout (int itemIndex,
						double minimumSize,
						double maximumSize,
						double preferredSize);

	/** For a numbered item, this returns its size limits and preferred size.

		@param itemIndex        the index of the item.
		@param minimumSize      the minimum size that this item is allowed to be - a positive number
								indicates an absolute size in pixels. A negative number indicates a
								proportion of the available space (e.g -0.5 is 50%)
		@param maximumSize      the maximum size that this item is allowed to be - a positive number
								indicates an absolute size in pixels. A negative number indicates a
								proportion of the available space
		@param preferredSize    the size that this item would like to be, if there's enough room. A
								positive number indicates an absolute size in pixels. A negative number
								indicates a proportion of the available space
		@returns false if the item's properties hadn't been set
		@see setItemLayout
	*/
	bool getItemLayout (int itemIndex,
						double& minimumSize,
						double& maximumSize,
						double& preferredSize) const;

	/** Clears all the properties that have been set with setItemLayout() and resets
		this object to its initial state.
	*/
	void clearAllItems();

	/** Takes a set of components that correspond to the layout's items, and positions
		them to fill a space.

		This will try to give each item its preferred size, whether that's a relative size
		or an absolute one.

		@param components       an array of components that correspond to each of the
								numbered items that the StretchableLayoutManager object
								has been told about with setItemLayout()
		@param numComponents    the number of components in the array that is passed-in. This
								should be the same as the number of items this object has been
								told about.
		@param x                the left of the rectangle in which the components should
								be laid out
		@param y                the top of the rectangle in which the components should
								be laid out
		@param width            the width of the rectangle in which the components should
								be laid out
		@param height           the height of the rectangle in which the components should
								be laid out
		@param vertically       if true, the components will be positioned in a vertical stack,
								so that they fill the height of the rectangle. If false, they
								will be placed side-by-side in a horizontal line, filling the
								available width
		@param resizeOtherDimension     if true, this means that the components will have their
								other dimension resized to fit the space - i.e. if the 'vertically'
								parameter is true, their x-positions and widths are adjusted to fit
								the x and width parameters; if 'vertically' is false, their y-positions
								and heights are adjusted to fit the y and height parameters.
	*/
	void layOutComponents (Component** components,
						   int numComponents,
						   int x, int y, int width, int height,
						   bool vertically,
						   bool resizeOtherDimension);

	/** Returns the current position of one of the items.

		This is only a valid call after layOutComponents() has been called, as it
		returns the last position that this item was placed at. If the layout was
		vertical, the value returned will be the y position of the top of the item,
		relative to the top of the rectangle in which the items were placed (so for
		example, item 0 will always have position of 0, even in the rectangle passed
		in to layOutComponents() wasn't at y = 0). If the layout was done horizontally,
		the position returned is the item's left-hand position, again relative to the
		x position of the rectangle used.

		@see getItemCurrentSize, setItemPosition
	*/
	int getItemCurrentPosition (int itemIndex) const;

	/** Returns the current size of one of the items.

		This is only meaningful after layOutComponents() has been called, as it
		returns the last size that this item was given. If the layout was done
		vertically, it'll return the item's height in pixels; if it was horizontal,
		it'll return its width.

		@see getItemCurrentRelativeSize
	*/
	int getItemCurrentAbsoluteSize (int itemIndex) const;

	/** Returns the current size of one of the items.

		This is only meaningful after layOutComponents() has been called, as it
		returns the last size that this item was given. If the layout was done
		vertically, it'll return a negative value representing the item's height relative
		to the last size used for laying the components out; if the layout was done
		horizontally it'll be the proportion of its width.

		@see getItemCurrentAbsoluteSize
	*/
	double getItemCurrentRelativeSize (int itemIndex) const;

	/** Moves one of the items, shifting along any other items as necessary in
		order to get it to the desired position.

		Calling this method will also update the preferred sizes of the items it
		shuffles along, so that they reflect their new positions.

		(This is the method that a StretchableLayoutResizerBar uses to shift the items
		about when it's dragged).

		@param itemIndex        the item to move
		@param newPosition      the absolute position that you'd like this item to move
								to. The item might not be able to always reach exactly this position,
								because other items may have minimum sizes that constrain how
								far it can go
	*/
	void setItemPosition (int itemIndex,
						  int newPosition);

private:

	struct ItemLayoutProperties
	{
		int itemIndex;
		int currentSize;
		double minSize, maxSize, preferredSize;
	};

	OwnedArray <ItemLayoutProperties> items;
	int totalSize;

	static int sizeToRealSize (double size, int totalSpace);
	ItemLayoutProperties* getInfoFor (int itemIndex) const;
	void setTotalSize (int newTotalSize);
	int fitComponentsIntoSpace (int startIndex, int endIndex, int availableSpace, int startPos);
	int getMinimumSizeOfItems (int startIndex, int endIndex) const;
	int getMaximumSizeOfItems (int startIndex, int endIndex) const;
	void updatePrefSizesToMatchCurrentPositions();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StretchableLayoutManager);
};

#endif   // __JUCE_STRETCHABLELAYOUTMANAGER_JUCEHEADER__

/*** End of inlined file: juce_StretchableLayoutManager.h ***/


#endif
#ifndef __JUCE_STRETCHABLELAYOUTRESIZERBAR_JUCEHEADER__

/*** Start of inlined file: juce_StretchableLayoutResizerBar.h ***/
#ifndef __JUCE_STRETCHABLELAYOUTRESIZERBAR_JUCEHEADER__
#define __JUCE_STRETCHABLELAYOUTRESIZERBAR_JUCEHEADER__

/**
	A component that acts as one of the vertical or horizontal bars you see being
	used to resize panels in a window.

	One of these acts with a StretchableLayoutManager to resize the other components.

	@see StretchableLayoutManager
*/
class JUCE_API  StretchableLayoutResizerBar  : public Component
{
public:

	/** Creates a resizer bar for use on a specified layout.

		@param layoutToUse          the layout that will be affected when this bar
									is dragged
		@param itemIndexInLayout    the item index in the layout that corresponds to
									this bar component. You'll need to set up the item
									properties in a suitable way for a divider bar, e.g.
									for an 8-pixel wide bar which, you could call
									myLayout->setItemLayout (barIndex, 8, 8, 8)
		@param isBarVertical        true if it's an upright bar that you drag left and
									right; false for a horizontal one that you drag up and
									down
	*/
	StretchableLayoutResizerBar (StretchableLayoutManager* layoutToUse,
								 int itemIndexInLayout,
								 bool isBarVertical);

	/** Destructor. */
	~StretchableLayoutResizerBar();

	/** This is called when the bar is dragged.

		This method must update the positions of any components whose position is
		determined by the StretchableLayoutManager, because they might have just
		moved.

		The default implementation calls the resized() method of this component's
		parent component, because that's often where you're likely to apply the
		layout, but it can be overridden for more specific needs.
	*/
	virtual void hasBeenMoved();

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);

private:

	StretchableLayoutManager* layout;
	int itemIndex, mouseDownPos;
	bool isVertical;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StretchableLayoutResizerBar);
};

#endif   // __JUCE_STRETCHABLELAYOUTRESIZERBAR_JUCEHEADER__

/*** End of inlined file: juce_StretchableLayoutResizerBar.h ***/


#endif
#ifndef __JUCE_STRETCHABLEOBJECTRESIZER_JUCEHEADER__

/*** Start of inlined file: juce_StretchableObjectResizer.h ***/
#ifndef __JUCE_STRETCHABLEOBJECTRESIZER_JUCEHEADER__
#define __JUCE_STRETCHABLEOBJECTRESIZER_JUCEHEADER__

/**
	A utility class for fitting a set of objects whose sizes can vary between
	a minimum and maximum size, into a space.

	This is a trickier algorithm than it would first seem, so I've put it in this
	class to allow it to be shared by various bits of code.

	To use it, create one of these objects, call addItem() to add the list of items
	you need, then call resizeToFit(), which will change all their sizes. You can
	then retrieve the new sizes with getItemSize() and getNumItems().

	It's currently used by the TableHeaderComponent for stretching out the table
	headings to fill the table's width.
*/
class StretchableObjectResizer
{
public:

	/** Creates an empty object resizer. */
	StretchableObjectResizer();

	/** Destructor. */
	~StretchableObjectResizer();

	/** Adds an item to the list.

		The order parameter lets you specify groups of items that are resized first when some
		space needs to be found. Those items with an order of 0 will be the first ones to be
		resized, and if that doesn't provide enough space to meet the requirements, the algorithm
		will then try resizing the items with an order of 1, then 2, and so on.
	*/
	void addItem (double currentSize,
				  double minSize,
				  double maxSize,
				  int order = 0);

	/** Resizes all the items to fit this amount of space.

		This will attempt to fit them in without exceeding each item's miniumum and
		maximum sizes. In cases where none of the items can be expanded or enlarged any
		further, the final size may be greater or less than the size passed in.

		After calling this method, you can retrieve the new sizes with the getItemSize()
		method.
	*/
	void resizeToFit (double targetSize);

	/** Returns the number of items that have been added. */
	int getNumItems() const noexcept                        { return items.size(); }

	/** Returns the size of one of the items. */
	double getItemSize (int index) const noexcept;

private:

	struct Item
	{
		double size;
		double minSize;
		double maxSize;
		int order;
	};

	OwnedArray <Item> items;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StretchableObjectResizer);
};

#endif   // __JUCE_STRETCHABLEOBJECTRESIZER_JUCEHEADER__

/*** End of inlined file: juce_StretchableObjectResizer.h ***/


#endif
#ifndef __JUCE_TABBEDBUTTONBAR_JUCEHEADER__

#endif
#ifndef __JUCE_TABBEDCOMPONENT_JUCEHEADER__

#endif
#ifndef __JUCE_VIEWPORT_JUCEHEADER__

#endif
#ifndef __JUCE_LOOKANDFEEL_JUCEHEADER__

/*** Start of inlined file: juce_LookAndFeel.h ***/
#ifndef __JUCE_LOOKANDFEEL_JUCEHEADER__
#define __JUCE_LOOKANDFEEL_JUCEHEADER__


/*** Start of inlined file: juce_Slider.h ***/
#ifndef __JUCE_SLIDER_JUCEHEADER__
#define __JUCE_SLIDER_JUCEHEADER__

/**
	A slider control for changing a value.

	The slider can be horizontal, vertical, or rotary, and can optionally have
	a text-box inside it to show an editable display of the current value.

	To use it, create a Slider object and use the setSliderStyle() method
	to set up the type you want. To set up the text-entry box, use setTextBoxStyle().

	To define the values that it can be set to, see the setRange() and setValue() methods.

	There are also lots of custom tweaks you can do by subclassing and overriding
	some of the virtual methods, such as changing the scaling, changing the format of
	the text display, custom ways of limiting the values, etc.

	You can register Slider::Listener objects with a slider, and they'll be called when
	the value changes.

	@see Slider::Listener
*/
class JUCE_API  Slider  : public Component,
						  public SettableTooltipClient,
						  public AsyncUpdater,
						  public ButtonListener,  // (can't use Button::Listener due to idiotic VC2005 bug)
						  public LabelListener,
						  public ValueListener
{
public:

	/** Creates a slider.

		When created, you'll need to set up the slider's style and range with setSliderStyle(),
		setRange(), etc.
	*/
	explicit Slider (const String& componentName = String::empty);

	/** Destructor. */
	~Slider();

	/** The types of slider available.

		@see setSliderStyle, setRotaryParameters
	*/
	enum SliderStyle
	{
		LinearHorizontal,       /**< A traditional horizontal slider. */
		LinearVertical,         /**< A traditional vertical slider. */
		LinearBar,              /**< A horizontal bar slider with the text label drawn on top of it. */
		Rotary,                 /**< A rotary control that you move by dragging the mouse in a circular motion, like a knob.
									 @see setRotaryParameters */
		RotaryHorizontalDrag,   /**< A rotary control that you move by dragging the mouse left-to-right.
									 @see setRotaryParameters */
		RotaryVerticalDrag,     /**< A rotary control that you move by dragging the mouse up-and-down.
									 @see setRotaryParameters */
		IncDecButtons,          /**< A pair of buttons that increment or decrement the slider's value by the increment set in setRange(). */

		TwoValueHorizontal,     /**< A horizontal slider that has two thumbs instead of one, so it can show a minimum and maximum value.
									 @see setMinValue, setMaxValue */
		TwoValueVertical,       /**< A vertical slider that has two thumbs instead of one, so it can show a minimum and maximum value.
									 @see setMinValue, setMaxValue */

		ThreeValueHorizontal,   /**< A horizontal slider that has three thumbs instead of one, so it can show a minimum and maximum
									 value, with the current value being somewhere between them.
									 @see setMinValue, setMaxValue */
		ThreeValueVertical,     /**< A vertical slider that has three thumbs instead of one, so it can show a minimum and maximum
									 value, with the current value being somewhere between them.
									 @see setMinValue, setMaxValue */
	};

	/** Changes the type of slider interface being used.

		@param newStyle         the type of interface
		@see setRotaryParameters, setVelocityBasedMode,
	*/
	void setSliderStyle (SliderStyle newStyle);

	/** Returns the slider's current style.

		@see setSliderStyle
	*/
	SliderStyle getSliderStyle() const noexcept                 { return style; }

	/** Changes the properties of a rotary slider.

		@param startAngleRadians        the angle (in radians, clockwise from the top) at which
										the slider's minimum value is represented
		@param endAngleRadians          the angle (in radians, clockwise from the top) at which
										the slider's maximum value is represented. This must be
										greater than startAngleRadians
		@param stopAtEnd                if true, then when the slider is dragged around past the
										minimum or maximum, it'll stop there; if false, it'll wrap
										back to the opposite value
	*/
	void setRotaryParameters (float startAngleRadians,
							  float endAngleRadians,
							  bool stopAtEnd);

	/** Sets the distance the mouse has to move to drag the slider across
		the full extent of its range.

		This only applies when in modes like RotaryHorizontalDrag, where it's using
		relative mouse movements to adjust the slider.
	*/
	void setMouseDragSensitivity (int distanceForFullScaleDrag);

	/** Returns the current sensitivity value set by setMouseDragSensitivity(). */
	int getMouseDragSensitivity() const noexcept                { return pixelsForFullDragExtent; }

	/** Changes the way the the mouse is used when dragging the slider.

		If true, this will turn on velocity-sensitive dragging, so that
		the faster the mouse moves, the bigger the movement to the slider. This
		helps when making accurate adjustments if the slider's range is quite large.

		If false, the slider will just try to snap to wherever the mouse is.
	*/
	void setVelocityBasedMode (bool isVelocityBased);

	/** Returns true if velocity-based mode is active.
		@see setVelocityBasedMode
	*/
	bool getVelocityBasedMode() const noexcept                  { return isVelocityBased; }

	/** Changes aspects of the scaling used when in velocity-sensitive mode.

		These apply when you've used setVelocityBasedMode() to turn on velocity mode,
		or if you're holding down ctrl.

		@param sensitivity      higher values than 1.0 increase the range of acceleration used
		@param threshold        the minimum number of pixels that the mouse needs to move for it
								to be treated as a movement
		@param offset           values greater than 0.0 increase the minimum speed that will be used when
								the threshold is reached
		@param userCanPressKeyToSwapMode    if true, then the user can hold down the ctrl or command
								key to toggle velocity-sensitive mode
	*/
	void setVelocityModeParameters (double sensitivity = 1.0,
									int threshold = 1,
									double offset = 0.0,
									bool userCanPressKeyToSwapMode = true);

	/** Returns the velocity sensitivity setting.
		@see setVelocityModeParameters
	*/
	double getVelocitySensitivity() const noexcept              { return velocityModeSensitivity; }

	/** Returns the velocity threshold setting.
		@see setVelocityModeParameters
	*/
	int getVelocityThreshold() const noexcept                   { return velocityModeThreshold; }

	/** Returns the velocity offset setting.
		@see setVelocityModeParameters
	*/
	double getVelocityOffset() const noexcept                   { return velocityModeOffset; }

	/** Returns the velocity user key setting.
		@see setVelocityModeParameters
	*/
	bool getVelocityModeIsSwappable() const noexcept            { return userKeyOverridesVelocity; }

	/** Sets up a skew factor to alter the way values are distributed.

		You may want to use a range of values on the slider where more accuracy
		is required towards one end of the range, so this will logarithmically
		spread the values across the length of the slider.

		If the factor is < 1.0, the lower end of the range will fill more of the
		slider's length; if the factor is > 1.0, the upper end of the range
		will be expanded instead. A factor of 1.0 doesn't skew it at all.

		To set the skew position by using a mid-point, use the setSkewFactorFromMidPoint()
		method instead.

		@see getSkewFactor, setSkewFactorFromMidPoint
	*/
	void setSkewFactor (double factor);

	/** Sets up a skew factor to alter the way values are distributed.

		This allows you to specify the slider value that should appear in the
		centre of the slider's visible range.

		@see setSkewFactor, getSkewFactor
	 */
	void setSkewFactorFromMidPoint (double sliderValueToShowAtMidPoint);

	/** Returns the current skew factor.

		See setSkewFactor for more info.

		@see setSkewFactor, setSkewFactorFromMidPoint
	*/
	double getSkewFactor() const noexcept                       { return skewFactor; }

	/** Used by setIncDecButtonsMode().
	*/
	enum IncDecButtonMode
	{
		incDecButtonsNotDraggable,
		incDecButtonsDraggable_AutoDirection,
		incDecButtonsDraggable_Horizontal,
		incDecButtonsDraggable_Vertical
	};

	/** When the style is IncDecButtons, this lets you turn on a mode where the mouse
		can be dragged on the buttons to drag the values.

		By default this is turned off. When enabled, clicking on the buttons still works
		them as normal, but by holding down the mouse on a button and dragging it a little
		distance, it flips into a mode where the value can be dragged. The drag direction can
		either be set explicitly to be vertical or horizontal, or can be set to
		incDecButtonsDraggable_AutoDirection so that it depends on whether the buttons
		are side-by-side or above each other.
	*/
	void setIncDecButtonsMode (IncDecButtonMode mode);

	/** The position of the slider's text-entry box.

		@see setTextBoxStyle
	*/
	enum TextEntryBoxPosition
	{
		NoTextBox,              /**< Doesn't display a text box.  */
		TextBoxLeft,            /**< Puts the text box to the left of the slider, vertically centred.  */
		TextBoxRight,           /**< Puts the text box to the right of the slider, vertically centred.  */
		TextBoxAbove,           /**< Puts the text box above the slider, horizontally centred.  */
		TextBoxBelow            /**< Puts the text box below the slider, horizontally centred.  */
	};

	/** Changes the location and properties of the text-entry box.

		@param newPosition          where it should go (or NoTextBox to not have one at all)
		@param isReadOnly           if true, it's a read-only display
		@param textEntryBoxWidth    the width of the text-box in pixels. Make sure this leaves enough
									room for the slider as well!
		@param textEntryBoxHeight   the height of the text-box in pixels. Make sure this leaves enough
									room for the slider as well!

		@see setTextBoxIsEditable, getValueFromText, getTextFromValue
	*/
	void setTextBoxStyle (TextEntryBoxPosition newPosition,
						  bool isReadOnly,
						  int textEntryBoxWidth,
						  int textEntryBoxHeight);

	/** Returns the status of the text-box.
		@see setTextBoxStyle
	*/
	const TextEntryBoxPosition getTextBoxPosition() const noexcept          { return textBoxPos; }

	/** Returns the width used for the text-box.
		@see setTextBoxStyle
	*/
	int getTextBoxWidth() const noexcept                                    { return textBoxWidth; }

	/** Returns the height used for the text-box.
		@see setTextBoxStyle
	*/
	int getTextBoxHeight() const noexcept                                   { return textBoxHeight; }

	/** Makes the text-box editable.

		By default this is true, and the user can enter values into the textbox,
		but it can be turned off if that's not suitable.

		@see setTextBoxStyle, getValueFromText, getTextFromValue
	*/
	void setTextBoxIsEditable (bool shouldBeEditable);

	/** Returns true if the text-box is read-only.
		@see setTextBoxStyle
	*/
	bool isTextBoxEditable() const                                          { return editableText; }

	/** If the text-box is editable, this will give it the focus so that the user can
		type directly into it.

		This is basically the effect as the user clicking on it.
	*/
	void showTextBox();

	/** If the text-box currently has focus and is being edited, this resets it and takes keyboard
		focus away from it.

		@param discardCurrentEditorContents     if true, the slider's value will be left
												unchanged; if false, the current contents of the
												text editor will be used to set the slider position
												before it is hidden.
	*/
	void hideTextBox (bool discardCurrentEditorContents);

	/** Changes the slider's current value.

		This will trigger a callback to Slider::Listener::sliderValueChanged() for any listeners
		that are registered, and will synchronously call the valueChanged() method in case subclasses
		want to handle it.

		@param newValue                 the new value to set - this will be restricted by the
										minimum and maximum range, and will be snapped to the
										nearest interval if one has been set
		@param sendUpdateMessage        if false, a change to the value will not trigger a call to
										any Slider::Listeners or the valueChanged() method
		@param sendMessageSynchronously if true, then a call to the Slider::Listeners will be made
										synchronously; if false, it will be asynchronous
	*/
	void setValue (double newValue,
				   bool sendUpdateMessage = true,
				   bool sendMessageSynchronously = false);

	/** Returns the slider's current value. */
	double getValue() const;

	/** Returns the Value object that represents the slider's current position.
		You can use this Value object to connect the slider's position to external values or setters,
		either by taking a copy of the Value, or by using Value::referTo() to make it point to
		your own Value object.
		@see Value, getMaxValue, getMinValueObject
	*/
	Value& getValueObject()                                                 { return currentValue; }

	/** Sets the limits that the slider's value can take.

		@param newMinimum   the lowest value allowed
		@param newMaximum   the highest value allowed
		@param newInterval  the steps in which the value is allowed to increase - if this
							is not zero, the value will always be (newMinimum + (newInterval * an integer)).
	*/
	void setRange (double newMinimum,
				   double newMaximum,
				   double newInterval = 0);

	/** Returns the current maximum value.
		@see setRange
	*/
	double getMaximum() const                                               { return maximum; }

	/** Returns the current minimum value.
		@see setRange
	*/
	double getMinimum() const                                               { return minimum; }

	/** Returns the current step-size for values.
		@see setRange
	*/
	double getInterval() const                                              { return interval; }

	/** For a slider with two or three thumbs, this returns the lower of its values.

		For a two-value slider, the values are controlled with getMinValue() and getMaxValue().
		A slider with three values also uses the normal getValue() and setValue() methods to
		control the middle value.

		@see setMinValue, getMaxValue, TwoValueHorizontal, TwoValueVertical, ThreeValueHorizontal, ThreeValueVertical
	*/
	double getMinValue() const;

	/** For a slider with two or three thumbs, this returns the lower of its values.
		You can use this Value object to connect the slider's position to external values or setters,
		either by taking a copy of the Value, or by using Value::referTo() to make it point to
		your own Value object.
		@see Value, getMinValue, getMaxValueObject
	*/
	Value& getMinValueObject() noexcept                                     { return valueMin; }

	/** For a slider with two or three thumbs, this sets the lower of its values.

		This will trigger a callback to Slider::Listener::sliderValueChanged() for any listeners
		that are registered, and will synchronously call the valueChanged() method in case subclasses
		want to handle it.

		@param newValue                 the new value to set - this will be restricted by the
										minimum and maximum range, and will be snapped to the nearest
										interval if one has been set.
		@param sendUpdateMessage        if false, a change to the value will not trigger a call to
										any Slider::Listeners or the valueChanged() method
		@param sendMessageSynchronously if true, then a call to the Slider::Listeners will be made
										synchronously; if false, it will be asynchronous
		@param allowNudgingOfOtherValues  if false, this value will be restricted to being below the
										max value (in a two-value slider) or the mid value (in a three-value
										slider). If true, then if this value goes beyond those values,
										it will push them along with it.
		@see getMinValue, setMaxValue, setValue
	*/
	void setMinValue (double newValue,
					  bool sendUpdateMessage = true,
					  bool sendMessageSynchronously = false,
					  bool allowNudgingOfOtherValues = false);

	/** For a slider with two or three thumbs, this returns the higher of its values.

		For a two-value slider, the values are controlled with getMinValue() and getMaxValue().
		A slider with three values also uses the normal getValue() and setValue() methods to
		control the middle value.

		@see getMinValue, TwoValueHorizontal, TwoValueVertical, ThreeValueHorizontal, ThreeValueVertical
	*/
	double getMaxValue() const;

	/** For a slider with two or three thumbs, this returns the higher of its values.
		You can use this Value object to connect the slider's position to external values or setters,
		either by taking a copy of the Value, or by using Value::referTo() to make it point to
		your own Value object.
		@see Value, getMaxValue, getMinValueObject
	*/
	Value& getMaxValueObject() noexcept                                     { return valueMax; }

	/** For a slider with two or three thumbs, this sets the lower of its values.

		This will trigger a callback to Slider::Listener::sliderValueChanged() for any listeners
		that are registered, and will synchronously call the valueChanged() method in case subclasses
		want to handle it.

		@param newValue                 the new value to set - this will be restricted by the
										minimum and maximum range, and will be snapped to the nearest
										interval if one has been set.
		@param sendUpdateMessage        if false, a change to the value will not trigger a call to
										any Slider::Listeners or the valueChanged() method
		@param sendMessageSynchronously if true, then a call to the Slider::Listeners will be made
										synchronously; if false, it will be asynchronous
		@param allowNudgingOfOtherValues  if false, this value will be restricted to being above the
										min value (in a two-value slider) or the mid value (in a three-value
										slider). If true, then if this value goes beyond those values,
										it will push them along with it.
		@see getMaxValue, setMinValue, setValue
	*/
	void setMaxValue (double newValue,
					  bool sendUpdateMessage = true,
					  bool sendMessageSynchronously = false,
					  bool allowNudgingOfOtherValues = false);

	/** For a slider with two or three thumbs, this sets the minimum and maximum thumb positions.

		This will trigger a callback to Slider::Listener::sliderValueChanged() for any listeners
		that are registered, and will synchronously call the valueChanged() method in case subclasses
		want to handle it.

		@param newMinValue              the new minimum value to set - this will be snapped to the
										nearest interval if one has been set.
		@param newMaxValue              the new minimum value to set - this will be snapped to the
										nearest interval if one has been set.
		@param sendUpdateMessage        if false, a change to the value will not trigger a call to
										any Slider::Listeners or the valueChanged() method
		@param sendMessageSynchronously if true, then a call to the Slider::Listeners will be made
										synchronously; if false, it will be asynchronous
		@see setMaxValue, setMinValue, setValue
	*/
	void setMinAndMaxValues (double newMinValue, double newMaxValue,
							 bool sendUpdateMessage = true,
							 bool sendMessageSynchronously = false);

	/** A class for receiving callbacks from a Slider.

		To be told when a slider's value changes, you can register a Slider::Listener
		object using Slider::addListener().

		@see Slider::addListener, Slider::removeListener
	*/
	class JUCE_API  Listener
	{
	public:

		/** Destructor. */
		virtual ~Listener() {}

		/** Called when the slider's value is changed.

			This may be caused by dragging it, or by typing in its text entry box,
			or by a call to Slider::setValue().

			You can find out the new value using Slider::getValue().

			@see Slider::valueChanged
		*/
		virtual void sliderValueChanged (Slider* slider) = 0;

		/** Called when the slider is about to be dragged.

			This is called when a drag begins, then it's followed by multiple calls
			to sliderValueChanged(), and then sliderDragEnded() is called after the
			user lets go.

			@see sliderDragEnded, Slider::startedDragging
		*/
		virtual void sliderDragStarted (Slider* slider);

		/** Called after a drag operation has finished.

			@see sliderDragStarted, Slider::stoppedDragging
		*/
		virtual void sliderDragEnded (Slider* slider);
	};

	/** Adds a listener to be called when this slider's value changes. */
	void addListener (Listener* listener);

	/** Removes a previously-registered listener. */
	void removeListener (Listener* listener);

	/** This lets you choose whether double-clicking moves the slider to a given position.

		By default this is turned off, but it's handy if you want a double-click to act
		as a quick way of resetting a slider. Just pass in the value you want it to
		go to when double-clicked.

		@see getDoubleClickReturnValue
	*/
	void setDoubleClickReturnValue (bool isDoubleClickEnabled,
									double valueToSetOnDoubleClick);

	/** Returns the values last set by setDoubleClickReturnValue() method.

		Sets isEnabled to true if double-click is enabled, and returns the value
		that was set.

		@see setDoubleClickReturnValue
	*/
	double getDoubleClickReturnValue (bool& isEnabled) const;

	/** Tells the slider whether to keep sending change messages while the user
		is dragging the slider.

		If set to true, a change message will only be sent when the user has
		dragged the slider and let go. If set to false (the default), then messages
		will be continuously sent as they drag it while the mouse button is still
		held down.
	*/
	void setChangeNotificationOnlyOnRelease (bool onlyNotifyOnRelease);

	/** This lets you change whether the slider thumb jumps to the mouse position
		when you click.

		By default, this is true. If it's false, then the slider moves with relative
		motion when you drag it.

		This only applies to linear bars, and won't affect two- or three- value
		sliders.
	*/
	void setSliderSnapsToMousePosition (bool shouldSnapToMouse);

	/** Returns true if setSliderSnapsToMousePosition() has been enabled. */
	bool getSliderSnapsToMousePosition() const noexcept         { return snapsToMousePos; }

	/** If enabled, this gives the slider a pop-up bubble which appears while the
		slider is being dragged.

		This can be handy if your slider doesn't have a text-box, so that users can
		see the value just when they're changing it.

		If you pass a component as the parentComponentToUse parameter, the pop-up
		bubble will be added as a child of that component when it's needed. If you
		pass 0, the pop-up will be placed on the desktop instead (note that it's a
		transparent window, so if you're using an OS that can't do transparent windows
		you'll have to add it to a parent component instead).
	*/
	void setPopupDisplayEnabled (bool isEnabled,
								 Component* parentComponentToUse);

	/** If a popup display is enabled and is currently visible, this returns the component
		that is being shown, or nullptr if none is currently in use.
		@see setPopupDisplayEnabled
	*/
	Component* getCurrentPopupDisplay() const noexcept;

	/** If this is set to true, then right-clicking on the slider will pop-up
		a menu to let the user change the way it works.

		By default this is turned off, but when turned on, the menu will include
		things like velocity sensitivity, and for rotary sliders, whether they
		use a linear or rotary mouse-drag to move them.
	*/
	void setPopupMenuEnabled (bool menuEnabled);

	/** This can be used to stop the mouse scroll-wheel from moving the slider.

		By default it's enabled.
	*/
	void setScrollWheelEnabled (bool enabled);

	/** Returns a number to indicate which thumb is currently being dragged by the
		mouse.

		This will return 0 for the main thumb, 1 for the minimum-value thumb, 2 for
		the maximum-value thumb, or -1 if none is currently down.
	*/
	int getThumbBeingDragged() const noexcept               { return sliderBeingDragged; }

	/** Callback to indicate that the user is about to start dragging the slider.

		@see Slider::Listener::sliderDragStarted
	*/
	virtual void startedDragging();

	/** Callback to indicate that the user has just stopped dragging the slider.

		@see Slider::Listener::sliderDragEnded
	*/
	virtual void stoppedDragging();

	/** Callback to indicate that the user has just moved the slider.

		@see Slider::Listener::sliderValueChanged
	*/
	virtual void valueChanged();

	/** Subclasses can override this to convert a text string to a value.

		When the user enters something into the text-entry box, this method is
		called to convert it to a value.

		The default routine just tries to convert it to a double.

		@see getTextFromValue
	*/
	virtual double getValueFromText (const String& text);

	/** Turns the slider's current value into a text string.

		Subclasses can override this to customise the formatting of the text-entry box.

		The default implementation just turns the value into a string, using
		a number of decimal places based on the range interval. If a suffix string
		has been set using setTextValueSuffix(), this will be appended to the text.

		@see getValueFromText
	*/
	virtual String getTextFromValue (double value);

	/** Sets a suffix to append to the end of the numeric value when it's displayed as
		a string.

		This is used by the default implementation of getTextFromValue(), and is just
		appended to the numeric value. For more advanced formatting, you can override
		getTextFromValue() and do something else.
	*/
	void setTextValueSuffix (const String& suffix);

	/** Returns the suffix that was set by setTextValueSuffix(). */
	String getTextValueSuffix() const;

	/** Allows a user-defined mapping of distance along the slider to its value.

		The default implementation for this performs the skewing operation that
		can be set up in the setSkewFactor() method. Override it if you need
		some kind of custom mapping instead, but make sure you also implement the
		inverse function in valueToProportionOfLength().

		@param proportion       a value 0 to 1.0, indicating a distance along the slider
		@returns                the slider value that is represented by this position
		@see valueToProportionOfLength
	*/
	virtual double proportionOfLengthToValue (double proportion);

	/** Allows a user-defined mapping of value to the position of the slider along its length.

		The default implementation for this performs the skewing operation that
		can be set up in the setSkewFactor() method. Override it if you need
		some kind of custom mapping instead, but make sure you also implement the
		inverse function in proportionOfLengthToValue().

		@param value            a valid slider value, between the range of values specified in
								setRange()
		@returns                a value 0 to 1.0 indicating the distance along the slider that
								represents this value
		@see proportionOfLengthToValue
	*/
	virtual double valueToProportionOfLength (double value);

	/** Returns the X or Y coordinate of a value along the slider's length.

		If the slider is horizontal, this will be the X coordinate of the given
		value, relative to the left of the slider. If it's vertical, then this will
		be the Y coordinate, relative to the top of the slider.

		If the slider is rotary, this will throw an assertion and return 0. If the
		value is out-of-range, it will be constrained to the length of the slider.
	*/
	float getPositionOfValue (double value);

	/** This can be overridden to allow the slider to snap to user-definable values.

		If overridden, it will be called when the user tries to move the slider to
		a given position, and allows a subclass to sanity-check this value, possibly
		returning a different value to use instead.

		@param attemptedValue       the value the user is trying to enter
		@param userIsDragging       true if the user is dragging with the mouse; false if
									they are entering the value using the text box
		@returns                    the value to use instead
	*/
	virtual double snapValue (double attemptedValue, bool userIsDragging);

	/** This can be called to force the text box to update its contents.

		(Not normally needed, as this is done automatically).
	*/
	void updateText();

	/** True if the slider moves horizontally. */
	bool isHorizontal() const;
	/** True if the slider moves vertically. */
	bool isVertical() const;

	/** A set of colour IDs to use to change the colour of various aspects of the slider.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId          = 0x1001200,  /**< A colour to use to fill the slider's background. */
		thumbColourId               = 0x1001300,  /**< The colour to draw the thumb with. It's up to the look
													   and feel class how this is used. */
		trackColourId               = 0x1001310,  /**< The colour to draw the groove that the thumb moves along. */
		rotarySliderFillColourId    = 0x1001311,  /**< For rotary sliders, this colour fills the outer curve. */
		rotarySliderOutlineColourId = 0x1001312,  /**< For rotary sliders, this colour is used to draw the outer curve's outline. */

		textBoxTextColourId         = 0x1001400,  /**< The colour for the text in the text-editor box used for editing the value. */
		textBoxBackgroundColourId   = 0x1001500,  /**< The background colour for the text-editor box. */
		textBoxHighlightColourId    = 0x1001600,  /**< The text highlight colour for the text-editor box. */
		textBoxOutlineColourId      = 0x1001700   /**< The colour to use for a border around the text-editor box. */
	};

	struct Ids
	{
		static const Identifier tagType, min, max, interval, type, editable,
								textBoxPos, textBoxWidth, textBoxHeight, skew;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);

protected:

	/** @internal */
	void labelTextChanged (Label*);
	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseUp (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseDoubleClick (const MouseEvent& e);
	/** @internal */
	void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
	/** @internal */
	void modifierKeysChanged (const ModifierKeys& modifiers);
	/** @internal */
	void buttonClicked (Button* button);
	/** @internal */
	void lookAndFeelChanged();
	/** @internal */
	void enablementChanged();
	/** @internal */
	void focusOfChildComponentChanged (FocusChangeType cause);
	/** @internal */
	void handleAsyncUpdate();
	/** @internal */
	void colourChanged();
	/** @internal */
	void valueChanged (Value& value);

	/** Returns the best number of decimal places to use when displaying numbers.
		This is calculated from the slider's interval setting.
	*/
	int getNumDecimalPlacesToDisplay() const noexcept       { return numDecimalPlaces; }

private:

	ListenerList <Listener> listeners;
	Value currentValue, valueMin, valueMax;
	double lastCurrentValue, lastValueMin, lastValueMax;
	double minimum, maximum, interval, doubleClickReturnValue;
	double valueWhenLastDragged, valueOnMouseDown, skewFactor, lastAngle;
	double velocityModeSensitivity, velocityModeOffset, minMaxDiff;
	int velocityModeThreshold;
	float rotaryStart, rotaryEnd;
	int numDecimalPlaces;
	Point<int> mouseDragStartPos, mousePosWhenLastDragged;
	int sliderRegionStart, sliderRegionSize;
	int sliderBeingDragged;
	int pixelsForFullDragExtent;
	Rectangle<int> sliderRect;
	String textSuffix;

	SliderStyle style;
	TextEntryBoxPosition textBoxPos;
	int textBoxWidth, textBoxHeight;
	IncDecButtonMode incDecButtonMode;

	bool editableText : 1, doubleClickToValue : 1;
	bool isVelocityBased : 1, userKeyOverridesVelocity : 1, rotaryStop : 1;
	bool incDecButtonsSideBySide : 1, sendChangeOnlyOnRelease : 1, popupDisplayEnabled : 1;
	bool menuEnabled : 1, menuShown : 1, mouseWasHidden : 1, incDecDragged : 1;
	bool scrollWheelEnabled : 1, snapsToMousePos : 1;
	ScopedPointer<Label> valueBox;
	ScopedPointer<Button> incButton, decButton;

	class PopupDisplayComponent;
	friend class PopupDisplayComponent;
	friend class ScopedPointer <PopupDisplayComponent>;
	ScopedPointer <PopupDisplayComponent> popupDisplay;
	Component* parentForPopupDisplay;

	void showPopupMenu();
	int getThumbIndexAt (const MouseEvent&);
	void handleRotaryDrag (const MouseEvent&);
	void handleAbsoluteDrag (const MouseEvent&);
	void handleVelocityDrag (const MouseEvent&);
	float getLinearSliderPos (double value);
	void restoreMouseIfHidden();
	void sendDragStart();
	void sendDragEnd();
	double constrainedValue (double value) const;
	void triggerChangeMessage (bool synchronous);
	bool incDecDragDirectionIsHorizontal() const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Slider);
};

/** This typedef is just for compatibility with old code - newer code should use the Slider::Listener class directly. */
typedef Slider::Listener SliderListener;

#endif   // __JUCE_SLIDER_JUCEHEADER__

/*** End of inlined file: juce_Slider.h ***/

class ToggleButton;
class TextButton;
class AlertWindow;
class TextLayout;
class ScrollBar;
class ComboBox;
class Button;
class FilenameComponent;
class DocumentWindow;
class ResizableWindow;
class GroupComponent;
class MenuBarComponent;
class DropShadower;
class GlyphArrangement;
class PropertyComponent;
class TableHeaderComponent;
class Toolbar;
class ToolbarItemComponent;
class PopupMenu;
class ProgressBar;
class FileBrowserComponent;
class DirectoryContentsDisplayComponent;
class FilePreviewComponent;
class ImageButton;
class CallOutBox;
class Drawable;
class CaretComponent;

/**
	LookAndFeel objects define the appearance of all the JUCE widgets, and subclasses
	can be used to apply different 'skins' to the application.

*/
class JUCE_API  LookAndFeel
{
public:

	/** Creates the default JUCE look and feel. */
	LookAndFeel();

	/** Destructor. */
	virtual ~LookAndFeel();

	/** Returns the current default look-and-feel for a component to use when it
		hasn't got one explicitly set.

		@see setDefaultLookAndFeel
	*/
	static LookAndFeel& getDefaultLookAndFeel() noexcept;

	/** Changes the default look-and-feel.

		@param newDefaultLookAndFeel    the new look-and-feel object to use - if this is
										set to null, it will revert to using the default one. The
										object passed-in must be deleted by the caller when
										it's no longer needed.
		@see getDefaultLookAndFeel
	*/
	static void setDefaultLookAndFeel (LookAndFeel* newDefaultLookAndFeel) noexcept;

	/** Looks for a colour that has been registered with the given colour ID number.

		If a colour has been set for this ID number using setColour(), then it is
		returned. If none has been set, it will just return Colours::black.

		The colour IDs for various purposes are stored as enums in the components that
		they are relevent to - for an example, see Slider::ColourIds,
		Label::ColourIds, TextEditor::ColourIds, TreeView::ColourIds, etc.

		If you're looking up a colour for use in drawing a component, it's usually
		best not to call this directly, but to use the Component::findColour() method
		instead. That will first check whether a suitable colour has been registered
		directly with the component, and will fall-back on calling the component's
		LookAndFeel's findColour() method if none is found.

		@see setColour, Component::findColour, Component::setColour
	*/
	Colour findColour (int colourId) const noexcept;

	/** Registers a colour to be used for a particular purpose.

		For more details, see the comments for findColour().

		@see findColour, Component::findColour, Component::setColour
	*/
	void setColour (int colourId, const Colour& colour) noexcept;

	/** Returns true if the specified colour ID has been explicitly set using the
		setColour() method.
	*/
	bool isColourSpecified (int colourId) const noexcept;

	virtual const Typeface::Ptr getTypefaceForFont (const Font& font);

	/** Allows you to change the default sans-serif font.

		If you need to supply your own Typeface object for any of the default fonts, rather
		than just supplying the name (e.g. if you want to use an embedded font), then
		you should instead override getTypefaceForFont() to create and return the typeface.
	*/
	void setDefaultSansSerifTypefaceName (const String& newName);

	/** Override this to get the chance to swap a component's mouse cursor for a
		customised one.
	*/
	virtual MouseCursor getMouseCursorFor (Component& component);

	// Creates a new graphics context object.
	virtual LowLevelGraphicsContext* createGraphicsContext (const Image& imageToRenderOn,
															const Point<int>& origin,
															const RectangleList& initialClip);

	/** Draws the lozenge-shaped background for a standard button. */
	virtual void drawButtonBackground (Graphics& g,
									   Button& button,
									   const Colour& backgroundColour,
									   bool isMouseOverButton,
									   bool isButtonDown);

	virtual const Font getFontForTextButton (TextButton& button);

	/** Draws the text for a TextButton. */
	virtual void drawButtonText (Graphics& g,
								 TextButton& button,
								 bool isMouseOverButton,
								 bool isButtonDown);

	/** Draws the contents of a standard ToggleButton. */
	virtual void drawToggleButton (Graphics& g,
								   ToggleButton& button,
								   bool isMouseOverButton,
								   bool isButtonDown);

	virtual void changeToggleButtonWidthToFitText (ToggleButton& button);

	virtual void drawTickBox (Graphics& g,
							  Component& component,
							  float x, float y, float w, float h,
							  bool ticked,
							  bool isEnabled,
							  bool isMouseOverButton,
							  bool isButtonDown);

	/* AlertWindow handling..
	*/
	virtual AlertWindow* createAlertWindow (const String& title,
											const String& message,
											const String& button1,
											const String& button2,
											const String& button3,
											AlertWindow::AlertIconType iconType,
											int numButtons,
											Component* associatedComponent);

	virtual void drawAlertBox (Graphics& g,
							   AlertWindow& alert,
							   const Rectangle<int>& textArea,
							   TextLayout& textLayout);

	virtual int getAlertBoxWindowFlags();

	virtual int getAlertWindowButtonHeight();

	virtual const Font getAlertWindowMessageFont();
	virtual const Font getAlertWindowFont();

	void setUsingNativeAlertWindows (bool shouldUseNativeAlerts);
	bool isUsingNativeAlertWindows();

	/** Draws a progress bar.

		If the progress value is less than 0 or greater than 1.0, this should draw a spinning
		bar that fills the whole space (i.e. to say that the app is still busy but the progress
		isn't known). It can use the current time as a basis for playing an animation.

		(Used by progress bars in AlertWindow).
	*/
	virtual void drawProgressBar (Graphics& g, ProgressBar& progressBar,
								  int width, int height,
								  double progress, const String& textToShow);

	// Draws a small image that spins to indicate that something's happening..
	// This method should use the current time to animate itself, so just keep
	// repainting it every so often.
	virtual void drawSpinningWaitAnimation (Graphics& g, const Colour& colour,
											int x, int y, int w, int h);

	/** Draws one of the buttons on a scrollbar.

		@param g                    the context to draw into
		@param scrollbar            the bar itself
		@param width                the width of the button
		@param height               the height of the button
		@param buttonDirection      the direction of the button, where 0 = up, 1 = right, 2 = down, 3 = left
		@param isScrollbarVertical  true if it's a vertical bar, false if horizontal
		@param isMouseOverButton    whether the mouse is currently over the button (also true if it's held down)
		@param isButtonDown         whether the mouse button's held down
	*/
	virtual void drawScrollbarButton (Graphics& g,
									  ScrollBar& scrollbar,
									  int width, int height,
									  int buttonDirection,
									  bool isScrollbarVertical,
									  bool isMouseOverButton,
									  bool isButtonDown);

	/** Draws the thumb area of a scrollbar.

		@param g                    the context to draw into
		@param scrollbar            the bar itself
		@param x                    the x position of the left edge of the thumb area to draw in
		@param y                    the y position of the top edge of the thumb area to draw in
		@param width                the width of the thumb area to draw in
		@param height               the height of the thumb area to draw in
		@param isScrollbarVertical  true if it's a vertical bar, false if horizontal
		@param thumbStartPosition   for vertical bars, the y co-ordinate of the top of the
									thumb, or its x position for horizontal bars
		@param thumbSize            for vertical bars, the height of the thumb, or its width for
									horizontal bars. This may be 0 if the thumb shouldn't be drawn.
		@param isMouseOver          whether the mouse is over the thumb area, also true if the mouse is
									currently dragging the thumb
		@param isMouseDown          whether the mouse is currently dragging the scrollbar
	*/
	virtual void drawScrollbar (Graphics& g,
								ScrollBar& scrollbar,
								int x, int y,
								int width, int height,
								bool isScrollbarVertical,
								int thumbStartPosition,
								int thumbSize,
								bool isMouseOver,
								bool isMouseDown);

	/** Returns the component effect to use for a scrollbar */
	virtual ImageEffectFilter* getScrollbarEffect();

	/** Returns the minimum length in pixels to use for a scrollbar thumb. */
	virtual int getMinimumScrollbarThumbSize (ScrollBar& scrollbar);

	/** Returns the default thickness to use for a scrollbar. */
	virtual int getDefaultScrollbarWidth();

	/** Returns the length in pixels to use for a scrollbar button. */
	virtual int getScrollbarButtonSize (ScrollBar& scrollbar);

	/** Returns a tick shape for use in yes/no boxes, etc. */
	virtual const Path getTickShape (float height);
	/** Returns a cross shape for use in yes/no boxes, etc. */
	virtual const Path getCrossShape (float height);

	/** Draws the + or - box in a treeview. */
	virtual void drawTreeviewPlusMinusBox (Graphics& g, int x, int y, int w, int h, bool isPlus, bool isMouseOver);

	virtual void fillTextEditorBackground (Graphics& g, int width, int height, TextEditor& textEditor);
	virtual void drawTextEditorOutline (Graphics& g, int width, int height, TextEditor& textEditor);

	virtual CaretComponent* createCaretComponent (Component* keyFocusOwner);

	// These return a pointer to an internally cached drawable - make sure you don't keep
	// a copy of this pointer anywhere, as it may become invalid in the future.
	virtual const Drawable* getDefaultFolderImage();
	virtual const Drawable* getDefaultDocumentFileImage();

	virtual void createFileChooserHeaderText (const String& title,
											  const String& instructions,
											  GlyphArrangement& destArrangement,
											  int width);

	virtual void drawFileBrowserRow (Graphics& g, int width, int height,
									 const String& filename, Image* icon,
									 const String& fileSizeDescription,
									 const String& fileTimeDescription,
									 bool isDirectory,
									 bool isItemSelected,
									 int itemIndex,
									 DirectoryContentsDisplayComponent& component);

	virtual Button* createFileBrowserGoUpButton();

	virtual void layoutFileBrowserComponent (FileBrowserComponent& browserComp,
											 DirectoryContentsDisplayComponent* fileListComponent,
											 FilePreviewComponent* previewComp,
											 ComboBox* currentPathBox,
											 TextEditor* filenameBox,
											 Button* goUpButton);

	virtual void drawBubble (Graphics& g,
							 float tipX, float tipY,
							 float boxX, float boxY, float boxW, float boxH);

	/** Fills the background of a popup menu component. */
	virtual void drawPopupMenuBackground (Graphics& g, int width, int height);

	/** Draws one of the items in a popup menu. */
	virtual void drawPopupMenuItem (Graphics& g,
									int width, int height,
									bool isSeparator,
									bool isActive,
									bool isHighlighted,
									bool isTicked,
									bool hasSubMenu,
									const String& text,
									const String& shortcutKeyText,
									Image* image,
									const Colour* const textColour);

	/** Returns the size and style of font to use in popup menus. */
	virtual const Font getPopupMenuFont();

	virtual void drawPopupMenuUpDownArrow (Graphics& g,
										   int width, int height,
										   bool isScrollUpArrow);

	/** Finds the best size for an item in a popup menu. */
	virtual void getIdealPopupMenuItemSize (const String& text,
											bool isSeparator,
											int standardMenuItemHeight,
											int& idealWidth,
											int& idealHeight);

	virtual int getMenuWindowFlags();

	virtual void drawMenuBarBackground (Graphics& g, int width, int height,
										bool isMouseOverBar,
										MenuBarComponent& menuBar);

	virtual int getMenuBarItemWidth (MenuBarComponent& menuBar, int itemIndex, const String& itemText);

	virtual const Font getMenuBarFont (MenuBarComponent& menuBar, int itemIndex, const String& itemText);

	virtual void drawMenuBarItem (Graphics& g,
								  int width, int height,
								  int itemIndex,
								  const String& itemText,
								  bool isMouseOverItem,
								  bool isMenuOpen,
								  bool isMouseOverBar,
								  MenuBarComponent& menuBar);

	virtual void drawComboBox (Graphics& g, int width, int height,
							   bool isButtonDown,
							   int buttonX, int buttonY,
							   int buttonW, int buttonH,
							   ComboBox& box);

	virtual const Font getComboBoxFont (ComboBox& box);

	virtual Label* createComboBoxTextBox (ComboBox& box);

	virtual void positionComboBoxText (ComboBox& box, Label& labelToPosition);

	virtual void drawLabel (Graphics& g, Label& label);

	virtual void drawLinearSlider (Graphics& g,
								   int x, int y,
								   int width, int height,
								   float sliderPos,
								   float minSliderPos,
								   float maxSliderPos,
								   const Slider::SliderStyle style,
								   Slider& slider);

	virtual void drawLinearSliderBackground (Graphics& g,
											 int x, int y,
											 int width, int height,
											 float sliderPos,
											 float minSliderPos,
											 float maxSliderPos,
											 const Slider::SliderStyle style,
											 Slider& slider);

	virtual void drawLinearSliderThumb (Graphics& g,
										int x, int y,
										int width, int height,
										float sliderPos,
										float minSliderPos,
										float maxSliderPos,
										const Slider::SliderStyle style,
										Slider& slider);

	virtual int getSliderThumbRadius (Slider& slider);

	virtual void drawRotarySlider (Graphics& g,
								   int x, int y,
								   int width, int height,
								   float sliderPosProportional,
								   float rotaryStartAngle,
								   float rotaryEndAngle,
								   Slider& slider);

	virtual Button* createSliderButton (bool isIncrement);
	virtual Label* createSliderTextBox (Slider& slider);

	virtual ImageEffectFilter* getSliderEffect();

	virtual void getTooltipSize (const String& tipText, int& width, int& height);

	virtual void drawTooltip (Graphics& g, const String& text, int width, int height);

	virtual Button* createFilenameComponentBrowseButton (const String& text);

	virtual void layoutFilenameComponent (FilenameComponent& filenameComp,
										  ComboBox* filenameBox, Button* browseButton);

	virtual void drawCornerResizer (Graphics& g,
									int w, int h,
									bool isMouseOver,
									bool isMouseDragging);

	virtual void drawResizableFrame (Graphics& g,
									int w, int h,
									const BorderSize<int>& borders);

	virtual void fillResizableWindowBackground (Graphics& g, int w, int h,
												const BorderSize<int>& border,
												ResizableWindow& window);

	virtual void drawResizableWindowBorder (Graphics& g,
											int w, int h,
											const BorderSize<int>& border,
											ResizableWindow& window);

	virtual void drawDocumentWindowTitleBar (DocumentWindow& window,
											 Graphics& g, int w, int h,
											 int titleSpaceX, int titleSpaceW,
											 const Image* icon,
											 bool drawTitleTextOnLeft);

	virtual Button* createDocumentWindowButton (int buttonType);

	virtual void positionDocumentWindowButtons (DocumentWindow& window,
												int titleBarX, int titleBarY,
												int titleBarW, int titleBarH,
												Button* minimiseButton,
												Button* maximiseButton,
												Button* closeButton,
												bool positionTitleBarButtonsOnLeft);

	virtual int getDefaultMenuBarHeight();

	virtual DropShadower* createDropShadowerForComponent (Component* component);

	virtual void drawStretchableLayoutResizerBar (Graphics& g,
												  int w, int h,
												  bool isVerticalBar,
												  bool isMouseOver,
												  bool isMouseDragging);

	virtual void drawGroupComponentOutline (Graphics& g, int w, int h,
											const String& text,
											const Justification& position,
											GroupComponent& group);

	virtual void createTabButtonShape (Path& p,
									   int width, int height,
									   int tabIndex,
									   const String& text,
									   Button& button,
									   TabbedButtonBar::Orientation orientation,
									   bool isMouseOver,
									   bool isMouseDown,
									   bool isFrontTab);

	virtual void fillTabButtonShape (Graphics& g,
									 const Path& path,
									 const Colour& preferredBackgroundColour,
									 int tabIndex,
									 const String& text,
									 Button& button,
									 TabbedButtonBar::Orientation orientation,
									 bool isMouseOver,
									 bool isMouseDown,
									 bool isFrontTab);

	virtual void drawTabButtonText (Graphics& g,
									int x, int y, int w, int h,
									const Colour& preferredBackgroundColour,
									int tabIndex,
									const String& text,
									Button& button,
									TabbedButtonBar::Orientation orientation,
									bool isMouseOver,
									bool isMouseDown,
									bool isFrontTab);

	virtual int getTabButtonOverlap (int tabDepth);
	virtual int getTabButtonSpaceAroundImage();

	virtual int getTabButtonBestWidth (int tabIndex,
									   const String& text,
									   int tabDepth,
									   Button& button);

	virtual void drawTabButton (Graphics& g,
								int w, int h,
								const Colour& preferredColour,
								int tabIndex,
								const String& text,
								Button& button,
								TabbedButtonBar::Orientation orientation,
								bool isMouseOver,
								bool isMouseDown,
								bool isFrontTab);

	virtual void drawTabAreaBehindFrontButton (Graphics& g,
											   int w, int h,
											   TabbedButtonBar& tabBar,
											   TabbedButtonBar::Orientation orientation);

	virtual Button* createTabBarExtrasButton();

	virtual void drawImageButton (Graphics& g, Image* image,
								  int imageX, int imageY, int imageW, int imageH,
								  const Colour& overlayColour,
								  float imageOpacity,
								  ImageButton& button);

	virtual void drawTableHeaderBackground (Graphics& g, TableHeaderComponent& header);

	virtual void drawTableHeaderColumn (Graphics& g, const String& columnName, int columnId,
										int width, int height,
										bool isMouseOver, bool isMouseDown,
										int columnFlags);

	virtual void paintToolbarBackground (Graphics& g, int width, int height, Toolbar& toolbar);

	virtual Button* createToolbarMissingItemsButton (Toolbar& toolbar);

	virtual void paintToolbarButtonBackground (Graphics& g, int width, int height,
											   bool isMouseOver, bool isMouseDown,
											   ToolbarItemComponent& component);

	virtual void paintToolbarButtonLabel (Graphics& g, int x, int y, int width, int height,
										  const String& text, ToolbarItemComponent& component);

	virtual void drawPropertyPanelSectionHeader (Graphics& g, const String& name,
												 bool isOpen, int width, int height);

	virtual void drawPropertyComponentBackground (Graphics& g, int width, int height,
												  PropertyComponent& component);

	virtual void drawPropertyComponentLabel (Graphics& g, int width, int height,
											 PropertyComponent& component);

	virtual const Rectangle<int> getPropertyComponentContentPosition (PropertyComponent& component);

	virtual void drawCallOutBoxBackground (CallOutBox& box, Graphics& g, const Path& path);

	virtual void drawLevelMeter (Graphics& g, int width, int height, float level);

	virtual void drawKeymapChangeButton (Graphics& g, int width, int height, Button& button, const String& keyDescription);

	/** Plays the system's default 'beep' noise, to alert the user about something very important.
	*/
	virtual void playAlertSound();

	/** Utility function to draw a shiny, glassy circle (for round LED-type buttons). */
	static void drawGlassSphere (Graphics& g,
								 float x, float y,
								 float diameter,
								 const Colour& colour,
								 float outlineThickness) noexcept;

	static void drawGlassPointer (Graphics& g,
								  float x, float y,
								  float diameter,
								  const Colour& colour, float outlineThickness,
								  int direction) noexcept;

	/** Utility function to draw a shiny, glassy oblong (for text buttons). */
	static void drawGlassLozenge (Graphics& g,
								  float x, float y,
								  float width, float height,
								  const Colour& colour,
								  float outlineThickness,
								  float cornerSize,
								  bool flatOnLeft, bool flatOnRight,
								  bool flatOnTop, bool flatOnBottom) noexcept;

	static Drawable* loadDrawableFromData (const void* data, size_t numBytes);

private:

	friend class WeakReference<LookAndFeel>;
	WeakReference<LookAndFeel>::Master masterReference;

	Array <int> colourIds;
	Array <Colour> colours;

	// default typeface names
	String defaultSans, defaultSerif, defaultFixed;

	ScopedPointer<Drawable> folderImage, documentImage;

	bool useNativeAlertWindows;

	void drawShinyButtonShape (Graphics& g,
							   float x, float y, float w, float h, float maxCornerSize,
							   const Colour& baseColour,
							   float strokeWidth,
							   bool flatOnLeft,
							   bool flatOnRight,
							   bool flatOnTop,
							   bool flatOnBottom) noexcept;

	// This has been deprecated - see the new parameter list..
	virtual int drawFileBrowserRow (Graphics&, int, int, const String&, Image*, const String&, const String&, bool, bool, int) { return 0; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeel);
};

#endif   // __JUCE_LOOKANDFEEL_JUCEHEADER__

/*** End of inlined file: juce_LookAndFeel.h ***/


#endif
#ifndef __JUCE_MENUBARCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_MenuBarComponent.h ***/
#ifndef __JUCE_MENUBARCOMPONENT_JUCEHEADER__
#define __JUCE_MENUBARCOMPONENT_JUCEHEADER__

/**
	A menu bar component.

	@see MenuBarModel
*/
class JUCE_API  MenuBarComponent  : public Component,
									private MenuBarModel::Listener,
									private Timer
{
public:

	/** Creates a menu bar.

		@param model        the model object to use to control this bar. You can
							pass 0 into this if you like, and set the model later
							using the setModel() method
	*/
	MenuBarComponent (MenuBarModel* model);

	/** Destructor. */
	~MenuBarComponent();

	/** Changes the model object to use to control the bar.

		This can be a null pointer, in which case the bar will be empty. Don't delete the object
		that is passed-in while it's still being used by this MenuBar.
	*/
	void setModel (MenuBarModel* newModel);

	/** Returns the current menu bar model being used.
	*/
	MenuBarModel* getModel() const noexcept;

	/** Pops up one of the menu items.

		This lets you manually open one of the menus - it could be triggered by a
		key shortcut, for example.
	*/
	void showMenu (int menuIndex);

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void mouseEnter (const MouseEvent& e);
	/** @internal */
	void mouseExit (const MouseEvent& e);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseUp (const MouseEvent& e);
	/** @internal */
	void mouseMove (const MouseEvent& e);
	/** @internal */
	void handleCommandMessage (int commandId);
	/** @internal */
	bool keyPressed (const KeyPress& key);
	/** @internal */
	void menuBarItemsChanged (MenuBarModel* menuBarModel);
	/** @internal */
	void menuCommandInvoked (MenuBarModel* menuBarModel,
							 const ApplicationCommandTarget::InvocationInfo& info);

private:

	MenuBarModel* model;

	StringArray menuNames;
	Array<int> xPositions;
	Point<int> lastMousePos;
	int itemUnderMouse, currentPopupIndex, topLevelIndexClicked;

	int getItemAt (const Point<int>&);
	void setItemUnderMouse (int index);
	void setOpenItem (int index);
	void updateItemUnderMouse (const Point<int>&);
	void timerCallback();
	void repaintMenuItem (int index);
	void menuDismissed (int topLevelIndex, int itemId);
	static void menuBarMenuDismissedCallback (int, MenuBarComponent*, int);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuBarComponent);
};

#endif   // __JUCE_MENUBARCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_MenuBarComponent.h ***/


#endif
#ifndef __JUCE_MENUBARMODEL_JUCEHEADER__

#endif
#ifndef __JUCE_POPUPMENU_JUCEHEADER__

#endif
#ifndef __JUCE_MARKERLIST_JUCEHEADER__

#endif
#ifndef __JUCE_RELATIVECOORDINATE_JUCEHEADER__

#endif
#ifndef __JUCE_RELATIVECOORDINATEPOSITIONER_JUCEHEADER__

#endif
#ifndef __JUCE_RELATIVEPARALLELOGRAM_JUCEHEADER__

#endif
#ifndef __JUCE_RELATIVEPOINT_JUCEHEADER__

#endif
#ifndef __JUCE_RELATIVEPOINTPATH_JUCEHEADER__

#endif
#ifndef __JUCE_RELATIVERECTANGLE_JUCEHEADER__

#endif
#ifndef __JUCE_BOOLEANPROPERTYCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_BooleanPropertyComponent.h ***/
#ifndef __JUCE_BOOLEANPROPERTYCOMPONENT_JUCEHEADER__
#define __JUCE_BOOLEANPROPERTYCOMPONENT_JUCEHEADER__


/*** Start of inlined file: juce_PropertyComponent.h ***/
#ifndef __JUCE_PROPERTYCOMPONENT_JUCEHEADER__
#define __JUCE_PROPERTYCOMPONENT_JUCEHEADER__

class EditableProperty;

/**
	A base class for a component that goes in a PropertyPanel and displays one of
	an item's properties.

	Subclasses of this are used to display a property in various forms, e.g. a
	ChoicePropertyComponent shows its value as a combo box; a SliderPropertyComponent
	shows its value as a slider; a TextPropertyComponent as a text box, etc.

	A subclass must implement the refresh() method which will be called to tell the
	component to update itself, and is also responsible for calling this it when the
	item that it refers to is changed.

	@see PropertyPanel, TextPropertyComponent, SliderPropertyComponent,
		 ChoicePropertyComponent, ButtonPropertyComponent, BooleanPropertyComponent
*/
class JUCE_API  PropertyComponent  : public Component,
									 public SettableTooltipClient
{
public:

	/** Creates a PropertyComponent.

		@param propertyName     the name is stored as this component's name, and is
								used as the name displayed next to this component in
								a property panel
		@param preferredHeight  the height that the component should be given - some
								items may need to be larger than a normal row height.
								This value can also be set if a subclass changes the
								preferredHeight member variable.
	*/
	PropertyComponent (const String& propertyName,
					   int preferredHeight = 25);

	/** Destructor. */
	~PropertyComponent();

	/** Returns this item's preferred height.

		This value is specified either in the constructor or by a subclass changing the
		preferredHeight member variable.
	*/
	int getPreferredHeight() const noexcept                 { return preferredHeight; }

	void setPreferredHeight (int newHeight) noexcept        { preferredHeight = newHeight; }

	/** Updates the property component if the item it refers to has changed.

		A subclass must implement this method, and other objects may call it to
		force it to refresh itself.

		The subclass should be economical in the amount of work is done, so for
		example it should check whether it really needs to do a repaint rather than
		just doing one every time this method is called, as it may be called when
		the value being displayed hasn't actually changed.
	*/
	virtual void refresh() = 0;

	/** The default paint method fills the background and draws a label for the
		item's name.

		@see LookAndFeel::drawPropertyComponentBackground(), LookAndFeel::drawPropertyComponentLabel()
	*/
	void paint (Graphics& g);

	/** The default resize method positions any child component to the right of this
		one, based on the look and feel's default label size.
	*/
	void resized();

	/** By default, this just repaints the component. */
	void enablementChanged();

protected:
	/** Used by the PropertyPanel to determine how high this component needs to be.
		A subclass can update this value in its constructor but shouldn't alter it later
		as changes won't necessarily be picked up.
	*/
	int preferredHeight;

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertyComponent);
};

#endif   // __JUCE_PROPERTYCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_PropertyComponent.h ***/

/**
	A PropertyComponent that contains an on/off toggle button.

	This type of property component can be used if you have a boolean value to
	toggle on/off.

	@see PropertyComponent
*/
class JUCE_API  BooleanPropertyComponent  : public PropertyComponent,
											private ButtonListener // (can't use Button::Listener due to idiotic VC2005 bug)
{
protected:

	/** Creates a button component.

		If you use this constructor, you must override the getState() and setState()
		methods.

		@param propertyName         the property name to be passed to the PropertyComponent
		@param buttonTextWhenTrue   the text shown in the button when the value is true
		@param buttonTextWhenFalse  the text shown in the button when the value is false
	*/
	BooleanPropertyComponent (const String& propertyName,
							  const String& buttonTextWhenTrue,
							  const String& buttonTextWhenFalse);

public:
	/** Creates a button component.

		@param valueToControl       a Value object that this property should refer to.
		@param propertyName         the property name to be passed to the PropertyComponent
		@param buttonText           the text shown in the ToggleButton component
	*/
	BooleanPropertyComponent (const Value& valueToControl,
							  const String& propertyName,
							  const String& buttonText);

	/** Destructor. */
	~BooleanPropertyComponent();

	/** Called to change the state of the boolean value. */
	virtual void setState (bool newState);

	/** Must return the current value of the property. */
	virtual bool getState() const;

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void refresh();
	/** @internal */
	void buttonClicked (Button*);

private:
	ToggleButton button;
	String onText, offText;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BooleanPropertyComponent);
};

#endif   // __JUCE_BOOLEANPROPERTYCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_BooleanPropertyComponent.h ***/


#endif
#ifndef __JUCE_BUTTONPROPERTYCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_ButtonPropertyComponent.h ***/
#ifndef __JUCE_BUTTONPROPERTYCOMPONENT_JUCEHEADER__
#define __JUCE_BUTTONPROPERTYCOMPONENT_JUCEHEADER__

/**
	A PropertyComponent that contains a button.

	This type of property component can be used if you need a button to trigger some
	kind of action.

	@see PropertyComponent
*/
class JUCE_API  ButtonPropertyComponent  : public PropertyComponent,
										   private ButtonListener // (can't use Button::Listener due to idiotic VC2005 bug)
{
public:

	/** Creates a button component.

		@param propertyName         the property name to be passed to the PropertyComponent
		@param triggerOnMouseDown   this is passed to the Button::setTriggeredOnMouseDown() method
	*/
	ButtonPropertyComponent (const String& propertyName,
							 bool triggerOnMouseDown);

	/** Destructor. */
	~ButtonPropertyComponent();

	/** Called when the user clicks the button.
	*/
	virtual void buttonClicked() = 0;

	/** Returns the string that should be displayed in the button.

		If you need to change this string, call refresh() to update the component.
	*/
	virtual String getButtonText() const = 0;

	/** @internal */
	void refresh();
	/** @internal */
	void buttonClicked (Button*);

private:
	TextButton button;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonPropertyComponent);
};

#endif   // __JUCE_BUTTONPROPERTYCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_ButtonPropertyComponent.h ***/


#endif
#ifndef __JUCE_CHOICEPROPERTYCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_ChoicePropertyComponent.h ***/
#ifndef __JUCE_CHOICEPROPERTYCOMPONENT_JUCEHEADER__
#define __JUCE_CHOICEPROPERTYCOMPONENT_JUCEHEADER__

/**
	A PropertyComponent that shows its value as a combo box.

	This type of property component contains a list of options and has a
	combo box to choose one.

	Your subclass's constructor must add some strings to the choices StringArray
	and these are shown in the list.

	The getIndex() method will be called to find out which option is the currently
	selected one. If you call refresh() it will call getIndex() to check whether
	the value has changed, and will update the combo box if needed.

	If the user selects a different item from the list, setIndex() will be
	called to let your class process this.

	@see PropertyComponent, PropertyPanel
*/
class JUCE_API  ChoicePropertyComponent    : public PropertyComponent,
											 private ComboBoxListener  // (can't use ComboBox::Listener due to idiotic VC2005 bug)
{
protected:
	/** Creates the component.

		Your subclass's constructor must add a list of options to the choices
		member variable.
	*/
	ChoicePropertyComponent (const String& propertyName);

public:
	/** Creates the component.

		@param valueToControl       the value that the combo box will read and control
		@param propertyName         the name of the property
		@param choices              the list of possible values that the drop-down list will contain
		@param correspondingValues  a list of values corresponding to each item in the 'choices' StringArray.
									These are the values that will be read and written to the
									valueToControl value. This array must contain the same number of items
									as the choices array
	*/
	ChoicePropertyComponent (const Value& valueToControl,
							 const String& propertyName,
							 const StringArray& choices,
							 const Array <var>& correspondingValues);

	/** Destructor. */
	~ChoicePropertyComponent();

	/** Called when the user selects an item from the combo box.

		Your subclass must use this callback to update the value that this component
		represents. The index is the index of the chosen item in the choices
		StringArray.
	*/
	virtual void setIndex (int newIndex);

	/** Returns the index of the item that should currently be shown.

		This is the index of the item in the choices StringArray that will be
		shown.
	*/
	virtual int getIndex() const;

	/** Returns the list of options. */
	const StringArray& getChoices() const;

	/** @internal */
	void refresh();
	/** @internal */
	void comboBoxChanged (ComboBox*);

protected:
	/** The list of options that will be shown in the combo box.

		Your subclass must populate this array in its constructor. If any empty
		strings are added, these will be replaced with horizontal separators (see
		ComboBox::addSeparator() for more info).
	*/
	StringArray choices;

private:
	ComboBox comboBox;
	bool isCustomClass;

	class RemapperValueSource;
	void createComboBox();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChoicePropertyComponent);
};

#endif   // __JUCE_CHOICEPROPERTYCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_ChoicePropertyComponent.h ***/


#endif
#ifndef __JUCE_PROPERTYCOMPONENT_JUCEHEADER__

#endif
#ifndef __JUCE_PROPERTYPANEL_JUCEHEADER__

/*** Start of inlined file: juce_PropertyPanel.h ***/
#ifndef __JUCE_PROPERTYPANEL_JUCEHEADER__
#define __JUCE_PROPERTYPANEL_JUCEHEADER__

/**
	A panel that holds a list of PropertyComponent objects.

	This panel displays a list of PropertyComponents, and allows them to be organised
	into collapsible sections.

	To use, simply create one of these and add your properties to it with addProperties()
	or addSection().

	@see PropertyComponent
*/
class JUCE_API  PropertyPanel  : public Component
{
public:

	/** Creates an empty property panel. */
	PropertyPanel();

	/** Destructor. */
	~PropertyPanel();

	/** Deletes all property components from the panel.
	*/
	void clear();

	/** Adds a set of properties to the panel.

		The components in the list will be owned by this object and will be automatically
		deleted later on when no longer needed.

		These properties are added without them being inside a named section. If you
		want them to be kept together in a collapsible section, use addSection() instead.
	*/
	void addProperties (const Array <PropertyComponent*>& newPropertyComponents);

	/** Adds a set of properties to the panel.

		These properties are added at the bottom of the list, under a section heading with
		a plus/minus button that allows it to be opened and closed.

		The components in the list will be owned by this object and will be automatically
		deleted later on when no longer needed.

		To add properies without them being in a section, use addProperties().
	*/
	void addSection (const String& sectionTitle,
					 const Array <PropertyComponent*>& newPropertyComponents,
					 bool shouldSectionInitiallyBeOpen = true);

	/** Calls the refresh() method of all PropertyComponents in the panel */
	void refreshAll() const;

	/** Returns a list of all the names of sections in the panel.

		These are the sections that have been added with addSection().
	*/
	StringArray getSectionNames() const;

	/** Returns true if the section at this index is currently open.

		The index is from 0 up to the number of items returned by getSectionNames().
	*/
	bool isSectionOpen (int sectionIndex) const;

	/** Opens or closes one of the sections.

		The index is from 0 up to the number of items returned by getSectionNames().
	*/
	void setSectionOpen (int sectionIndex, bool shouldBeOpen);

	/** Enables or disables one of the sections.

		The index is from 0 up to the number of items returned by getSectionNames().
	*/
	void setSectionEnabled (int sectionIndex, bool shouldBeEnabled);

	/** Saves the current state of open/closed sections so it can be restored later.

		The caller is responsible for deleting the object that is returned.

		To restore this state, use restoreOpennessState().

		@see restoreOpennessState
	*/
	XmlElement* getOpennessState() const;

	/** Restores a previously saved arrangement of open/closed sections.

		This will try to restore a snapshot of the panel's state that was created by
		the getOpennessState() method. If any of the sections named in the original
		XML aren't present, they will be ignored.

		@see getOpennessState
	*/
	void restoreOpennessState (const XmlElement& newState);

	/** Sets a message to be displayed when there are no properties in the panel.

		The default message is "nothing selected".
	*/
	void setMessageWhenEmpty (const String& newMessage);

	/** Returns the message that is displayed when there are no properties.
		@see setMessageWhenEmpty
	*/
	const String& getMessageWhenEmpty() const;

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();

private:
	Viewport viewport;
	class PropertyHolderComponent;
	PropertyHolderComponent* propertyHolderComponent;
	String messageWhenEmpty;

	void updatePropHolderLayout() const;
	void updatePropHolderLayout (int width) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertyPanel);
};

#endif   // __JUCE_PROPERTYPANEL_JUCEHEADER__

/*** End of inlined file: juce_PropertyPanel.h ***/


#endif
#ifndef __JUCE_SLIDERPROPERTYCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_SliderPropertyComponent.h ***/
#ifndef __JUCE_SLIDERPROPERTYCOMPONENT_JUCEHEADER__
#define __JUCE_SLIDERPROPERTYCOMPONENT_JUCEHEADER__

/**
	A PropertyComponent that shows its value as a slider.

	@see PropertyComponent, Slider
*/
class JUCE_API  SliderPropertyComponent   : public PropertyComponent,
											private SliderListener  // (can't use Slider::Listener due to idiotic VC2005 bug)
{
protected:

	/** Creates the property component.

		The ranges, interval and skew factor are passed to the Slider component.

		If you need to customise the slider in other ways, your constructor can
		access the slider member variable and change it directly.
	*/
	SliderPropertyComponent (const String& propertyName,
							 double rangeMin,
							 double rangeMax,
							 double interval,
							 double skewFactor = 1.0);

public:

	/** Creates the property component.

		The ranges, interval and skew factor are passed to the Slider component.

		If you need to customise the slider in other ways, your constructor can
		access the slider member variable and change it directly.
	*/
	SliderPropertyComponent (const Value& valueToControl,
							 const String& propertyName,
							 double rangeMin,
							 double rangeMax,
							 double interval,
							 double skewFactor = 1.0);

	/** Destructor. */
	~SliderPropertyComponent();

	/** Called when the user moves the slider to change its value.

		Your subclass must use this method to update whatever item this property
		represents.
	*/
	virtual void setValue (double newValue);

	/** Returns the value that the slider should show. */
	virtual double getValue() const;

	/** @internal */
	void refresh();
	/** @internal */
	void sliderValueChanged (Slider*);

protected:
	/** The slider component being used in this component.
		Your subclass has access to this in case it needs to customise it in some way.
	*/
	Slider slider;

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderPropertyComponent);
};

#endif   // __JUCE_SLIDERPROPERTYCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_SliderPropertyComponent.h ***/


#endif
#ifndef __JUCE_TEXTPROPERTYCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_TextPropertyComponent.h ***/
#ifndef __JUCE_TEXTPROPERTYCOMPONENT_JUCEHEADER__
#define __JUCE_TEXTPROPERTYCOMPONENT_JUCEHEADER__

/**
	A PropertyComponent that shows its value as editable text.

	@see PropertyComponent
*/
class JUCE_API  TextPropertyComponent  : public PropertyComponent
{
protected:

	/** Creates a text property component.

		The maxNumChars is used to set the length of string allowable, and isMultiLine
		sets whether the text editor allows carriage returns.

		@see TextEditor
	*/
	TextPropertyComponent (const String& propertyName,
						   int maxNumChars,
						   bool isMultiLine);

public:
	/** Creates a text property component.

		The maxNumChars is used to set the length of string allowable, and isMultiLine
		sets whether the text editor allows carriage returns.

		@see TextEditor
	*/
	TextPropertyComponent (const Value& valueToControl,
						   const String& propertyName,
						   int maxNumChars,
						   bool isMultiLine);

	/** Destructor. */
	~TextPropertyComponent();

	/** Called when the user edits the text.

		Your subclass must use this callback to change the value of whatever item
		this property component represents.
	*/
	virtual void setText (const String& newText);

	/** Returns the text that should be shown in the text editor.
	*/
	virtual String getText() const;

	/** @internal */
	void refresh();
	/** @internal */
	void textWasEdited();

private:
	ScopedPointer<Label> textEditor;

	void createEditor (int maxNumChars, bool isMultiLine);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextPropertyComponent);
};

#endif   // __JUCE_TEXTPROPERTYCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_TextPropertyComponent.h ***/


#endif
#ifndef __JUCE_COMBOBOX_JUCEHEADER__

#endif
#ifndef __JUCE_IMAGECOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_ImageComponent.h ***/
#ifndef __JUCE_IMAGECOMPONENT_JUCEHEADER__
#define __JUCE_IMAGECOMPONENT_JUCEHEADER__

/**
	A component that simply displays an image.

	Use setImage to give it an image, and it'll display it - simple as that!
*/
class JUCE_API  ImageComponent  : public Component,
								  public SettableTooltipClient
{
public:

	/** Creates an ImageComponent. */
	ImageComponent (const String& componentName = String::empty);

	/** Destructor. */
	~ImageComponent();

	/** Sets the image that should be displayed. */
	void setImage (const Image& newImage);

	/** Sets the image that should be displayed, and its placement within the component. */
	void setImage (const Image& newImage,
				   const RectanglePlacement& placementToUse);

	/** Returns the current image. */
	const Image& getImage() const;

	/** Sets the method of positioning that will be used to fit the image within the component's bounds.
		By default the positioning is centred, and will fit the image inside the component's bounds
		whilst keeping its aspect ratio correct, but you can change it to whatever layout you need.
	*/
	void setImagePlacement (const RectanglePlacement& newPlacement);

	/** Returns the current image placement. */
	const RectanglePlacement getImagePlacement() const;

	struct Ids
	{
		static const Identifier tagType, image, placement;
	};

	void refreshFromValueTree (const ValueTree&, ComponentBuilder&);
	static RectanglePlacement getPlacement (const ValueTree& state);

	/** @internal */
	void paint (Graphics& g);

private:
	Image image;
	RectanglePlacement placement;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageComponent);
};

#endif   // __JUCE_IMAGECOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_ImageComponent.h ***/


#endif
#ifndef __JUCE_LABEL_JUCEHEADER__

#endif
#ifndef __JUCE_LISTBOX_JUCEHEADER__

#endif
#ifndef __JUCE_PROGRESSBAR_JUCEHEADER__

#endif
#ifndef __JUCE_SLIDER_JUCEHEADER__

#endif
#ifndef __JUCE_TABLEHEADERCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_TableHeaderComponent.h ***/
#ifndef __JUCE_TABLEHEADERCOMPONENT_JUCEHEADER__
#define __JUCE_TABLEHEADERCOMPONENT_JUCEHEADER__

/**
	A component that displays a strip of column headings for a table, and allows these
	to be resized, dragged around, etc.

	This is just the component that goes at the top of a table. You can use it
	directly for custom components, or to create a simple table, use the
	TableListBox class.

	To use one of these, create it and use addColumn() to add all the columns that you need.
	Each column must be given a unique ID number that's used to refer to it.

	@see TableListBox, TableHeaderComponent::Listener
*/
class JUCE_API  TableHeaderComponent   : public Component,
										 private AsyncUpdater
{
public:

	/** Creates an empty table header.
	*/
	TableHeaderComponent();

	/** Destructor. */
	~TableHeaderComponent();

	/** A combination of these flags are passed into the addColumn() method to specify
		the properties of a column.
	*/
	enum ColumnPropertyFlags
	{
		visible                     = 1,    /**< If this is set, the column will be shown; if not, it will be hidden until the user enables it with the pop-up menu. */
		resizable                   = 2,    /**< If this is set, the column can be resized by dragging it. */
		draggable                   = 4,    /**< If this is set, the column can be dragged around to change its order in the table. */
		appearsOnColumnMenu         = 8,    /**< If this is set, the column will be shown on the pop-up menu allowing it to be hidden/shown. */
		sortable                    = 16,   /**< If this is set, then clicking on the column header will set it to be the sort column, and clicking again will reverse the order. */
		sortedForwards              = 32,   /**< If this is set, the column is currently the one by which the table is sorted (forwards). */
		sortedBackwards             = 64,   /**< If this is set, the column is currently the one by which the table is sorted (backwards). */

		/** This set of default flags is used as the default parameter value in addColumn(). */
		defaultFlags                = (visible | resizable | draggable | appearsOnColumnMenu | sortable),

		/** A quick way of combining flags for a column that's not resizable. */
		notResizable                = (visible | draggable | appearsOnColumnMenu | sortable),

		/** A quick way of combining flags for a column that's not resizable or sortable. */
		notResizableOrSortable      = (visible | draggable | appearsOnColumnMenu),

		/** A quick way of combining flags for a column that's not sortable. */
		notSortable                 = (visible | resizable | draggable | appearsOnColumnMenu)
	};

	/** Adds a column to the table.

		This will add a column, and asynchronously call the tableColumnsChanged() method of any
		registered listeners.

		@param columnName       the name of the new column. It's ok to have two or more columns with the same name
		@param columnId         an ID for this column. The ID can be any number apart from 0, but every column must have
								a unique ID. This is used to identify the column later on, after the user may have
								changed the order that they appear in
		@param width            the initial width of the column, in pixels
		@param maximumWidth     a maximum width that the column can take when the user is resizing it. This only applies
								if the 'resizable' flag is specified for this column
		@param minimumWidth     a minimum width that the column can take when the user is resizing it. This only applies
								if the 'resizable' flag is specified for this column
		@param propertyFlags    a combination of some of the values from the ColumnPropertyFlags enum, to define the
								properties of this column
		@param insertIndex      the index at which the column should be added. A value of 0 puts it at the start (left-hand side)
								and -1 puts it at the end (right-hand size) of the table. Note that the index the index within
								all columns, not just the index amongst those that are currently visible
	*/
	void addColumn (const String& columnName,
					int columnId,
					int width,
					int minimumWidth = 30,
					int maximumWidth = -1,
					int propertyFlags = defaultFlags,
					int insertIndex = -1);

	/** Removes a column with the given ID.

		If there is such a column, this will asynchronously call the tableColumnsChanged() method of any
		registered listeners.
	*/
	void removeColumn (int columnIdToRemove);

	/** Deletes all columns from the table.

		If there are any columns to remove, this will asynchronously call the tableColumnsChanged() method of any
		registered listeners.
	*/
	void removeAllColumns();

	/** Returns the number of columns in the table.

		If onlyCountVisibleColumns is true, this will return the number of visible columns; otherwise it'll
		return the total number of columns, including hidden ones.

		@see isColumnVisible
	*/
	int getNumColumns (bool onlyCountVisibleColumns) const;

	/** Returns the name for a column.
		@see setColumnName
	*/
	String getColumnName (int columnId) const;

	/** Changes the name of a column. */
	void setColumnName (int columnId, const String& newName);

	/** Moves a column to a different index in the table.

		@param columnId             the column to move
		@param newVisibleIndex      the target index for it, from 0 to the number of columns currently visible.
	*/
	void moveColumn (int columnId, int newVisibleIndex);

	/** Returns the width of one of the columns.
	*/
	int getColumnWidth (int columnId) const;

	/** Changes the width of a column.

		This will cause an asynchronous callback to the tableColumnsResized() method of any registered listeners.
	*/
	void setColumnWidth (int columnId, int newWidth);

	/** Shows or hides a column.

		This can cause an asynchronous callback to the tableColumnsChanged() method of any registered listeners.
		@see isColumnVisible
	*/
	void setColumnVisible (int columnId, bool shouldBeVisible);

	/** Returns true if this column is currently visible.
		@see setColumnVisible
	*/
	bool isColumnVisible (int columnId) const;

	/** Changes the column which is the sort column.

		This can cause an asynchronous callback to the tableSortOrderChanged() method of any registered listeners.

		If this method doesn't actually change the column ID, then no re-sort will take place (you can
		call reSortTable() to force a re-sort to happen if you've modified the table's contents).

		@see getSortColumnId, isSortedForwards, reSortTable
	*/
	void setSortColumnId (int columnId, bool sortForwards);

	/** Returns the column ID by which the table is currently sorted, or 0 if it is unsorted.

		@see setSortColumnId, isSortedForwards
	*/
	int getSortColumnId() const;

	/** Returns true if the table is currently sorted forwards, or false if it's backwards.
		@see setSortColumnId
	*/
	bool isSortedForwards() const;

	/** Triggers a re-sort of the table according to the current sort-column.

		If you modifiy the table's contents, you can call this to signal that the table needs
		to be re-sorted.

		(This doesn't do any sorting synchronously - it just asynchronously sends a call to the
		tableSortOrderChanged() method of any listeners).
	*/
	void reSortTable();

	/** Returns the total width of all the visible columns in the table.
	*/
	int getTotalWidth() const;

	/** Returns the index of a given column.

		If there's no such column ID, this will return -1.

		If onlyCountVisibleColumns is true, this will return the index amoungst the visible columns;
		otherwise it'll return the index amongst all the columns, including any hidden ones.
	*/
	int getIndexOfColumnId (int columnId, bool onlyCountVisibleColumns) const;

	/** Returns the ID of the column at a given index.

		If onlyCountVisibleColumns is true, this will count the index amoungst the visible columns;
		otherwise it'll count it amongst all the columns, including any hidden ones.

		If the index is out-of-range, it'll return 0.
	*/
	int getColumnIdOfIndex (int index, bool onlyCountVisibleColumns) const;

	/** Returns the rectangle containing of one of the columns.

		The index is an index from 0 to the number of columns that are currently visible (hidden
		ones are not counted). It returns a rectangle showing the position of the column relative
		to this component's top-left. If the index is out-of-range, an empty rectangle is retrurned.
	*/
	Rectangle<int> getColumnPosition (int index) const;

	/** Finds the column ID at a given x-position in the component.

		If there is a column at this point this returns its ID, or if not, it will return 0.
	*/
	int getColumnIdAtX (int xToFind) const;

	/** If set to true, this indicates that the columns should be expanded or shrunk to fill the
		entire width of the component.

		By default this is disabled. Turning it on also means that when resizing a column, those
		on the right will be squashed to fit.
	*/
	void setStretchToFitActive (bool shouldStretchToFit);

	/** Returns true if stretch-to-fit has been enabled.
		@see setStretchToFitActive
	*/
	bool isStretchToFitActive() const;

	/** If stretch-to-fit is enabled, this will resize all the columns to make them fit into the
		specified width, keeping their relative proportions the same.

		If the minimum widths of the columns are too wide to fit into this space, it may
		actually end up wider.
	*/
	void resizeAllColumnsToFit (int targetTotalWidth);

	/** Enables or disables the pop-up menu.

		The default menu allows the user to show or hide columns. You can add custom
		items to this menu by overloading the addMenuItems() and reactToMenuItem() methods.

		By default the menu is enabled.

		@see isPopupMenuActive, addMenuItems, reactToMenuItem
	*/
	void setPopupMenuActive (bool hasMenu);

	/** Returns true if the pop-up menu is enabled.
		@see setPopupMenuActive
	*/
	bool isPopupMenuActive() const;

	/** Returns a string that encapsulates the table's current layout.

		This can be restored later using restoreFromString(). It saves the order of
		the columns, the currently-sorted column, and the widths.

		@see restoreFromString
	*/
	String toString() const;

	/** Restores the state of the table, based on a string previously created with
		toString().

		@see toString
	*/
	void restoreFromString (const String& storedVersion);

	/**
		Receives events from a TableHeaderComponent when columns are resized, moved, etc.

		You can register one of these objects for table events using TableHeaderComponent::addListener()
		and TableHeaderComponent::removeListener().

		@see TableHeaderComponent
	*/
	class JUCE_API  Listener
	{
	public:

		Listener() {}

		/** Destructor. */
		virtual ~Listener() {}

		/** This is called when some of the table's columns are added, removed, hidden,
			or rearranged.
		*/
		virtual void tableColumnsChanged (TableHeaderComponent* tableHeader) = 0;

		/** This is called when one or more of the table's columns are resized.
		*/
		virtual void tableColumnsResized (TableHeaderComponent* tableHeader) = 0;

		/** This is called when the column by which the table should be sorted is changed.
		*/
		virtual void tableSortOrderChanged (TableHeaderComponent* tableHeader) = 0;

		/** This is called when the user begins or ends dragging one of the columns around.

			When the user starts dragging a column, this is called with the ID of that
			column. When they finish dragging, it is called again with 0 as the ID.
		*/
		virtual void tableColumnDraggingChanged (TableHeaderComponent* tableHeader,
												 int columnIdNowBeingDragged);
	};

	/** Adds a listener to be informed about things that happen to the header. */
	void addListener (Listener* newListener);

	/** Removes a previously-registered listener. */
	void removeListener (Listener* listenerToRemove);

	/** This can be overridden to handle a mouse-click on one of the column headers.

		The default implementation will use this click to call getSortColumnId() and
		change the sort order.
	*/
	virtual void columnClicked (int columnId, const ModifierKeys& mods);

	/** This can be overridden to add custom items to the pop-up menu.

		If you override this, you should call the superclass's method to add its
		column show/hide items, if you want them on the menu as well.

		Then to handle the result, override reactToMenuItem().

		@see reactToMenuItem
	*/
	virtual void addMenuItems (PopupMenu& menu, int columnIdClicked);

	/** Override this to handle any custom items that you have added to the
		pop-up menu with an addMenuItems() override.

		If the menuReturnId isn't one of your own custom menu items, you'll need to
		call TableHeaderComponent::reactToMenuItem() to allow the base class to
		handle the items that it had added.

		@see addMenuItems
	*/
	virtual void reactToMenuItem (int menuReturnId, int columnIdClicked);

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void mouseMove (const MouseEvent&);
	/** @internal */
	void mouseEnter (const MouseEvent&);
	/** @internal */
	void mouseExit (const MouseEvent&);
	/** @internal */
	void mouseDown (const MouseEvent&);
	/** @internal */
	void mouseDrag (const MouseEvent&);
	/** @internal */
	void mouseUp (const MouseEvent&);
	/** @internal */
	MouseCursor getMouseCursor();

	/** Can be overridden for more control over the pop-up menu behaviour. */
	virtual void showColumnChooserMenu (int columnIdClicked);

private:
	struct ColumnInfo
	{
		String name;
		int id, propertyFlags, width, minimumWidth, maximumWidth;
		double lastDeliberateWidth;

		bool isVisible() const;
	};

	OwnedArray <ColumnInfo> columns;
	Array <Listener*> listeners;
	ScopedPointer <Component> dragOverlayComp;

	bool columnsChanged, columnsResized, sortChanged, menuActive, stretchToFit;
	int columnIdBeingResized, columnIdBeingDragged, initialColumnWidth;
	int columnIdUnderMouse, draggingColumnOffset, draggingColumnOriginalIndex, lastDeliberateWidth;

	ColumnInfo* getInfoForId (int columnId) const;
	int visibleIndexToTotalIndex (int visibleIndex) const;
	void sendColumnsChanged();
	void handleAsyncUpdate();
	void beginDrag (const MouseEvent&);
	void endDrag (int finalIndex);
	int getResizeDraggerAt (int mouseX) const;
	void updateColumnUnderMouse (const MouseEvent&);
	void setColumnUnderMouse (int columnId);
	void resizeColumnsToFit (int firstColumnIndex, int targetTotalWidth);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TableHeaderComponent);
};

/** This typedef is just for compatibility with old code - newer code should use the TableHeaderComponent::Listener class directly. */
typedef TableHeaderComponent::Listener TableHeaderListener;

#endif   // __JUCE_TABLEHEADERCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_TableHeaderComponent.h ***/


#endif
#ifndef __JUCE_TABLELISTBOX_JUCEHEADER__

/*** Start of inlined file: juce_TableListBox.h ***/
#ifndef __JUCE_TABLELISTBOX_JUCEHEADER__
#define __JUCE_TABLELISTBOX_JUCEHEADER__

/**
	One of these is used by a TableListBox as the data model for the table's contents.

	The virtual methods that you override in this class take care of drawing the
	table cells, and reacting to events.

	@see TableListBox
*/
class JUCE_API  TableListBoxModel
{
public:

	TableListBoxModel()  {}

	/** Destructor. */
	virtual ~TableListBoxModel()  {}

	/** This must return the number of rows currently in the table.

		If the number of rows changes, you must call TableListBox::updateContent() to
		cause it to refresh the list.
	*/
	virtual int getNumRows() = 0;

	/** This must draw the background behind one of the rows in the table.

		The graphics context has its origin at the row's top-left, and your method
		should fill the area specified by the width and height parameters.
	*/
	virtual void paintRowBackground (Graphics& g,
									 int rowNumber,
									 int width, int height,
									 bool rowIsSelected) = 0;

	/** This must draw one of the cells.

		The graphics context's origin will already be set to the top-left of the cell,
		whose size is specified by (width, height).
	*/
	virtual void paintCell (Graphics& g,
							int rowNumber,
							int columnId,
							int width, int height,
							bool rowIsSelected) = 0;

	/** This is used to create or update a custom component to go in a cell.

		Any cell may contain a custom component, or can just be drawn with the paintCell() method
		and handle mouse clicks with cellClicked().

		This method will be called whenever a custom component might need to be updated - e.g.
		when the table is changed, or TableListBox::updateContent() is called.

		If you don't need a custom component for the specified cell, then return nullptr.
		(Bear in mind that even if you're not creating a new component, you may still need to
		delete existingComponentToUpdate if it's non-null).

		If you do want a custom component, and the existingComponentToUpdate is null, then
		this method must create a new component suitable for the cell, and return it.

		If the existingComponentToUpdate is non-null, it will be a pointer to a component previously created
		by this method. In this case, the method must either update it to make sure it's correctly representing
		the given cell (which may be different from the one that the component was created for), or it can
		delete this component and return a new one.
	*/
	virtual Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected,
												Component* existingComponentToUpdate);

	/** This callback is made when the user clicks on one of the cells in the table.

		The mouse event's coordinates will be relative to the entire table row.
		@see cellDoubleClicked, backgroundClicked
	*/
	virtual void cellClicked (int rowNumber, int columnId, const MouseEvent& e);

	/** This callback is made when the user clicks on one of the cells in the table.

		The mouse event's coordinates will be relative to the entire table row.
		@see cellClicked, backgroundClicked
	*/
	virtual void cellDoubleClicked (int rowNumber, int columnId, const MouseEvent& e);

	/** This can be overridden to react to the user double-clicking on a part of the list where
		there are no rows.

		@see cellClicked
	*/
	virtual void backgroundClicked();

	/** This callback is made when the table's sort order is changed.

		This could be because the user has clicked a column header, or because the
		TableHeaderComponent::setSortColumnId() method was called.

		If you implement this, your method should re-sort the table using the given
		column as the key.
	*/
	virtual void sortOrderChanged (int newSortColumnId, bool isForwards);

	/** Returns the best width for one of the columns.

		If you implement this method, you should measure the width of all the items
		in this column, and return the best size.

		Returning 0 means that the column shouldn't be changed.

		This is used by TableListBox::autoSizeColumn() and TableListBox::autoSizeAllColumns().
	*/
	virtual int getColumnAutoSizeWidth (int columnId);

	/** Returns a tooltip for a particular cell in the table.
	*/
	virtual String getCellTooltip (int rowNumber, int columnId);

	/** Override this to be informed when rows are selected or deselected.

		@see ListBox::selectedRowsChanged()
	*/
	virtual void selectedRowsChanged (int lastRowSelected);

	/** Override this to be informed when the delete key is pressed.

		@see ListBox::deleteKeyPressed()
	*/
	virtual void deleteKeyPressed (int lastRowSelected);

	/** Override this to be informed when the return key is pressed.

		@see ListBox::returnKeyPressed()
	*/
	virtual void returnKeyPressed (int lastRowSelected);

	/** Override this to be informed when the list is scrolled.

		This might be caused by the user moving the scrollbar, or by programmatic changes
		to the list position.
	*/
	virtual void listWasScrolled();

	/** To allow rows from your table to be dragged-and-dropped, implement this method.

		If this returns a non-null variant then when the user drags a row, the table will try to
		find a DragAndDropContainer in its parent hierarchy, and will use it to trigger a
		drag-and-drop operation, using this string as the source description, and the listbox
		itself as the source component.

		@see getDragSourceCustomData, DragAndDropContainer::startDragging
	*/
	virtual var getDragSourceDescription (const SparseSet<int>& currentlySelectedRows);
};

/**
	A table of cells, using a TableHeaderComponent as its header.

	This component makes it easy to create a table by providing a TableListBoxModel as
	the data source.

	@see TableListBoxModel, TableHeaderComponent
*/
class JUCE_API  TableListBox   : public ListBox,
								 private ListBoxModel,
								 private TableHeaderComponent::Listener
{
public:

	/** Creates a TableListBox.

		The model pointer passed-in can be null, in which case you can set it later
		with setModel().
	*/
	TableListBox (const String& componentName = String::empty,
				  TableListBoxModel* model = 0);

	/** Destructor. */
	~TableListBox();

	/** Changes the TableListBoxModel that is being used for this table.
	*/
	void setModel (TableListBoxModel* newModel);

	/** Returns the model currently in use. */
	TableListBoxModel* getModel() const                             { return model; }

	/** Returns the header component being used in this table. */
	TableHeaderComponent& getHeader() const                         { return *header; }

	/** Sets the header component to use for the table.
		The table will take ownership of the component that you pass in, and will delete it
		when it's no longer needed.
	*/
	void setHeader (TableHeaderComponent* newHeader);

	/** Changes the height of the table header component.
		@see getHeaderHeight
	*/
	void setHeaderHeight (int newHeight);

	/** Returns the height of the table header.
		@see setHeaderHeight
	*/
	int getHeaderHeight() const;

	/** Resizes a column to fit its contents.

		This uses TableListBoxModel::getColumnAutoSizeWidth() to find the best width,
		and applies that to the column.

		@see autoSizeAllColumns, TableHeaderComponent::setColumnWidth
	*/
	void autoSizeColumn (int columnId);

	/** Calls autoSizeColumn() for all columns in the table. */
	void autoSizeAllColumns();

	/** Enables or disables the auto size options on the popup menu.

		By default, these are enabled.
	*/
	void setAutoSizeMenuOptionShown (bool shouldBeShown);

	/** True if the auto-size options should be shown on the menu.
		@see setAutoSizeMenuOptionsShown
	*/
	bool isAutoSizeMenuOptionShown() const;

	/** Returns the position of one of the cells in the table.

		If relativeToComponentTopLeft is true, the co-ordinates are relative to
		the table component's top-left. The row number isn't checked to see if it's
		in-range, but the column ID must exist or this will return an empty rectangle.

		If relativeToComponentTopLeft is false, the co-ords are relative to the
		top-left of the table's top-left cell.
	*/
	Rectangle<int> getCellPosition (int columnId, int rowNumber,
									bool relativeToComponentTopLeft) const;

	/** Returns the component that currently represents a given cell.
		If the component for this cell is off-screen or if the position is out-of-range,
		this may return 0.
		@see getCellPosition
	*/
	Component* getCellComponent (int columnId, int rowNumber) const;

	/** Scrolls horizontally if necessary to make sure that a particular column is visible.

		@see ListBox::scrollToEnsureRowIsOnscreen
	*/
	void scrollToEnsureColumnIsOnscreen (int columnId);

	/** @internal */
	int getNumRows();
	/** @internal */
	void paintListBoxItem (int, Graphics&, int, int, bool);
	/** @internal */
	Component* refreshComponentForRow (int rowNumber, bool isRowSelected, Component* existingComponentToUpdate);
	/** @internal */
	void selectedRowsChanged (int lastRowSelected);
	/** @internal */
	void deleteKeyPressed (int currentSelectedRow);
	/** @internal */
	void returnKeyPressed (int currentSelectedRow);
	/** @internal */
	void backgroundClicked();
	/** @internal */
	void listWasScrolled();
	/** @internal */
	void tableColumnsChanged (TableHeaderComponent*);
	/** @internal */
	void tableColumnsResized (TableHeaderComponent*);
	/** @internal */
	void tableSortOrderChanged (TableHeaderComponent*);
	/** @internal */
	void tableColumnDraggingChanged (TableHeaderComponent*, int);
	/** @internal */
	void resized();

private:

	TableHeaderComponent* header;
	TableListBoxModel* model;
	int columnIdNowBeingDragged;
	bool autoSizeOptionsShown;

	void updateColumnComponents() const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TableListBox);
};

#endif   // __JUCE_TABLELISTBOX_JUCEHEADER__

/*** End of inlined file: juce_TableListBox.h ***/


#endif
#ifndef __JUCE_TEXTEDITOR_JUCEHEADER__

#endif
#ifndef __JUCE_TOOLBAR_JUCEHEADER__

#endif
#ifndef __JUCE_TOOLBARITEMCOMPONENT_JUCEHEADER__

#endif
#ifndef __JUCE_TOOLBARITEMFACTORY_JUCEHEADER__

/*** Start of inlined file: juce_ToolbarItemFactory.h ***/
#ifndef __JUCE_TOOLBARITEMFACTORY_JUCEHEADER__
#define __JUCE_TOOLBARITEMFACTORY_JUCEHEADER__

/**
	A factory object which can create ToolbarItemComponent objects.

	A subclass of ToolbarItemFactory publishes a set of types of toolbar item
	that it can create.

	Each type of item is identified by a unique ID, and multiple instances of an
	item type can exist at once (even on the same toolbar, e.g. spacers or separator
	bars).

	@see Toolbar, ToolbarItemComponent, ToolbarButton
*/
class JUCE_API  ToolbarItemFactory
{
public:

	ToolbarItemFactory();

	/** Destructor. */
	virtual ~ToolbarItemFactory();

	/** A set of reserved item ID values, used for the built-in item types.
	*/
	enum SpecialItemIds
	{
		separatorBarId      = -1,   /**< The item ID for a vertical (or horizontal) separator bar that
										 can be placed between sets of items to break them into groups. */
		spacerId            = -2,   /**< The item ID for a fixed-width space that can be placed between
										 items.*/
		flexibleSpacerId    = -3    /**< The item ID for a gap that pushes outwards against the things on
										 either side of it, filling any available space. */
	};

	/** Must return a list of the IDs for all the item types that this factory can create.

		The ids should be added to the array that is passed-in.

		An item ID can be any integer you choose, except for 0, which is considered a null ID,
		and the predefined IDs in the SpecialItemIds enum.

		You should also add the built-in types (separatorBarId, spacerId and flexibleSpacerId)
		to this list if you want your toolbar to be able to contain those items.

		The list returned here is used by the ToolbarItemPalette class to obtain its list
		of available items, and their order on the palette will reflect the order in which
		they appear on this list.

		@see ToolbarItemPalette
	*/
	virtual void getAllToolbarItemIds (Array <int>& ids) = 0;

	/** Must return the set of items that should be added to a toolbar as its default set.

		This method is used by Toolbar::addDefaultItems() to determine which items to
		create.

		The items that your method adds to the array that is passed-in will be added to the
		toolbar in the same order. Items can appear in the list more than once.
	*/
	virtual void getDefaultItemSet (Array <int>& ids) = 0;

	/** Must create an instance of one of the items that the factory lists in its
		getAllToolbarItemIds() method.

		The itemId parameter can be any of the values listed by your getAllToolbarItemIds()
		method, except for the built-in item types from the SpecialItemIds enum, which
		are created internally by the toolbar code.

		Try not to keep a pointer to the object that is returned, as it will be deleted
		automatically by the toolbar, and remember that multiple instances of the same
		item type are likely to exist at the same time.
	*/
	virtual ToolbarItemComponent* createItem (int itemId) = 0;
};

#endif   // __JUCE_TOOLBARITEMFACTORY_JUCEHEADER__

/*** End of inlined file: juce_ToolbarItemFactory.h ***/


#endif
#ifndef __JUCE_TOOLBARITEMPALETTE_JUCEHEADER__

/*** Start of inlined file: juce_ToolbarItemPalette.h ***/
#ifndef __JUCE_TOOLBARITEMPALETTE_JUCEHEADER__
#define __JUCE_TOOLBARITEMPALETTE_JUCEHEADER__

/**
	A component containing a list of toolbar items, which the user can drag onto
	a toolbar to add them.

	You can use this class directly, but it's a lot easier to call Toolbar::showCustomisationDialog(),
	which automatically shows one of these in a dialog box with lots of extra controls.

	@see Toolbar
*/
class JUCE_API  ToolbarItemPalette    : public Component,
										public DragAndDropContainer
{
public:

	/** Creates a palette of items for a given factory, with the aim of adding them
		to the specified toolbar.

		The ToolbarItemFactory::getAllToolbarItemIds() method is used to create the
		set of items that are shown in this palette.

		The toolbar and factory must not be deleted while this object exists.
	*/
	ToolbarItemPalette (ToolbarItemFactory& factory,
						Toolbar* toolbar);

	/** Destructor. */
	~ToolbarItemPalette();

	/** @internal */
	void resized();

private:
	ToolbarItemFactory& factory;
	Toolbar* toolbar;
	Viewport viewport;
	OwnedArray <ToolbarItemComponent> items;

	friend class Toolbar;
	void replaceComponent (ToolbarItemComponent* comp);
	void addComponent (int itemId, int index);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolbarItemPalette);
};

#endif   // __JUCE_TOOLBARITEMPALETTE_JUCEHEADER__

/*** End of inlined file: juce_ToolbarItemPalette.h ***/


#endif
#ifndef __JUCE_TREEVIEW_JUCEHEADER__

#endif
#ifndef __JUCE_ALERTWINDOW_JUCEHEADER__

#endif
#ifndef __JUCE_CALLOUTBOX_JUCEHEADER__

/*** Start of inlined file: juce_CallOutBox.h ***/
#ifndef __JUCE_CALLOUTBOX_JUCEHEADER__
#define __JUCE_CALLOUTBOX_JUCEHEADER__

/**
	A box with a small arrow that can be used as a temporary pop-up window to show
	extra controls when a button or other component is clicked.

	Using one of these is similar to having a popup menu attached to a button or
	other component - but it looks fancier, and has an arrow that can indicate the
	object that it applies to.

	Normally, you'd create one of these on the stack and run it modally, e.g.

	@code
	void mouseUp (const MouseEvent& e)
	{
		MyContentComponent content;
		content.setSize (300, 300);

		CallOutBox callOut (content, *this, nullptr);
		callOut.runModalLoop();
	}
	@endcode

	The call-out will resize and position itself when the content changes size.
*/
class JUCE_API  CallOutBox    : public Component
{
public:

	/** Creates a CallOutBox.

		@param contentComponent     the component to display inside the call-out. This should
									already have a size set (although the call-out will also
									update itself when the component's size is changed later).
									Obviously this component must not be deleted until the
									call-out box has been deleted.
		@param componentToPointTo   the component that the call-out's arrow should point towards
		@param parentComponent      if non-zero, this is the component to add the call-out to. If
									this is zero, the call-out will be added to the desktop.
	*/
	CallOutBox (Component& contentComponent,
				Component& componentToPointTo,
				Component* parentComponent);

	/** Destructor. */
	~CallOutBox();

	/** Changes the length of the arrow. */
	void setArrowSize (float newSize);

	/** Updates the position and size of the box.

		You shouldn't normally need to call this, unless you need more precise control over the
		layout.

		@param newAreaToPointTo     the rectangle to make the box's arrow point to
		@param newAreaToFitIn       the area within which the box's position should be constrained
	*/
	void updatePosition (const Rectangle<int>& newAreaToPointTo,
						 const Rectangle<int>& newAreaToFitIn);

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void moved();
	/** @internal */
	void childBoundsChanged (Component*);
	/** @internal */
	bool hitTest (int x, int y);
	/** @internal */
	void inputAttemptWhenModal();
	/** @internal */
	bool keyPressed (const KeyPress& key);
	/** @internal */
	void handleCommandMessage (int commandId);

private:

	int borderSpace;
	float arrowSize;
	Component& content;
	Path outline;
	Point<float> targetPoint;
	Rectangle<int> availableArea, targetArea;
	Image background;

	void refreshPath();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CallOutBox);
};

#endif   // __JUCE_CALLOUTBOX_JUCEHEADER__

/*** End of inlined file: juce_CallOutBox.h ***/


#endif
#ifndef __JUCE_COMPONENTPEER_JUCEHEADER__

/*** Start of inlined file: juce_ComponentPeer.h ***/
#ifndef __JUCE_COMPONENTPEER_JUCEHEADER__
#define __JUCE_COMPONENTPEER_JUCEHEADER__

class ComponentBoundsConstrainer;

/**
	The Component class uses a ComponentPeer internally to create and manage a real
	operating-system window.

	This is an abstract base class - the platform specific code contains implementations of
	it for the various platforms.

	User-code should very rarely need to have any involvement with this class.

	@see Component::createNewPeer
*/
class JUCE_API  ComponentPeer
{
public:

	/** A combination of these flags is passed to the ComponentPeer constructor. */
	enum StyleFlags
	{
		windowAppearsOnTaskbar      = (1 << 0),    /**< Indicates that the window should have a corresponding
														entry on the taskbar (ignored on MacOSX) */
		windowIsTemporary           = (1 << 1),    /**< Indicates that the window is a temporary popup, like a menu,
														tooltip, etc. */
		windowIgnoresMouseClicks    = (1 << 2),    /**< Indicates that the window should let mouse clicks pass
														through it (may not be possible on some platforms). */
		windowHasTitleBar           = (1 << 3),    /**< Indicates that the window should have a normal OS-specific
														title bar and frame. if not specified, the window will be
														borderless. */
		windowIsResizable           = (1 << 4),    /**< Indicates that the window should have a resizable border. */
		windowHasMinimiseButton     = (1 << 5),    /**< Indicates that if the window has a title bar, it should have a
														minimise button on it. */
		windowHasMaximiseButton     = (1 << 6),    /**< Indicates that if the window has a title bar, it should have a
														maximise button on it. */
		windowHasCloseButton        = (1 << 7),    /**< Indicates that if the window has a title bar, it should have a
														close button on it. */
		windowHasDropShadow         = (1 << 8),    /**< Indicates that the window should have a drop-shadow (this may
														not be possible on all platforms). */
		windowRepaintedExplictly    = (1 << 9),    /**< Not intended for public use - this tells a window not to
														do its own repainting, but only to repaint when the
														performAnyPendingRepaintsNow() method is called. */
		windowIgnoresKeyPresses     = (1 << 10),   /**< Tells the window not to catch any keypresses. This can
														be used for things like plugin windows, to stop them interfering
														with the host's shortcut keys */
		windowIsSemiTransparent     = (1 << 31)    /**< Not intended for public use - makes a window transparent. */

	};

	/** Creates a peer.

		The component is the one that we intend to represent, and the style flags are
		a combination of the values in the StyleFlags enum
	*/
	ComponentPeer (Component* component, int styleFlags);

	/** Destructor. */
	virtual ~ComponentPeer();

	/** Returns the component being represented by this peer. */
	Component* getComponent() const noexcept                { return component; }

	/** Returns the set of style flags that were set when the window was created.

		@see Component::addToDesktop
	*/
	int getStyleFlags() const noexcept                      { return styleFlags; }

	/** Returns a unique ID for this peer.
		Each peer that is created is given a different ID.
	*/
	uint32 getUniqueID() const noexcept                     { return uniqueID; }

	/** Returns the raw handle to whatever kind of window is being used.

		On windows, this is probably a HWND, on the mac, it's likely to be a WindowRef,
		but rememeber there's no guarantees what you'll get back.
	*/
	virtual void* getNativeHandle() const = 0;

	/** Shows or hides the window. */
	virtual void setVisible (bool shouldBeVisible) = 0;

	/** Changes the title of the window. */
	virtual void setTitle (const String& title) = 0;

	/** Moves the window without changing its size.

		If the native window is contained in another window, then the co-ordinates are
		relative to the parent window's origin, not the screen origin.

		This should result in a callback to handleMovedOrResized().
	*/
	virtual void setPosition (int x, int y) = 0;

	/** Resizes the window without changing its position.

		This should result in a callback to handleMovedOrResized().
	*/
	virtual void setSize (int w, int h) = 0;

	/** Moves and resizes the window.

		If the native window is contained in another window, then the co-ordinates are
		relative to the parent window's origin, not the screen origin.

		This should result in a callback to handleMovedOrResized().
	*/
	virtual void setBounds (int x, int y, int w, int h, bool isNowFullScreen) = 0;

	/** Returns the current position and size of the window.

		If the native window is contained in another window, then the co-ordinates are
		relative to the parent window's origin, not the screen origin.
	*/
	virtual Rectangle<int> getBounds() const = 0;

	/** Returns the x-position of this window, relative to the screen's origin. */
	virtual Point<int> getScreenPosition() const = 0;

	/** Converts a position relative to the top-left of this component to screen co-ordinates. */
	virtual Point<int> localToGlobal (const Point<int>& relativePosition) = 0;

	/** Converts a rectangle relative to the top-left of this component to screen co-ordinates. */
	virtual Rectangle<int> localToGlobal (const Rectangle<int>& relativePosition);

	/** Converts a screen co-ordinate to a position relative to the top-left of this component. */
	virtual Point<int> globalToLocal (const Point<int>& screenPosition) = 0;

	/** Converts a screen area to a position relative to the top-left of this component. */
	virtual Rectangle<int> globalToLocal (const Rectangle<int>& screenPosition);

	/** Minimises the window. */
	virtual void setMinimised (bool shouldBeMinimised) = 0;

	/** True if the window is currently minimised. */
	virtual bool isMinimised() const = 0;

	/** Enable/disable fullscreen mode for the window. */
	virtual void setFullScreen (bool shouldBeFullScreen) = 0;

	/** True if the window is currently full-screen. */
	virtual bool isFullScreen() const = 0;

	/** Sets the size to restore to if fullscreen mode is turned off. */
	void setNonFullScreenBounds (const Rectangle<int>& newBounds) noexcept;

	/** Returns the size to restore to if fullscreen mode is turned off. */
	const Rectangle<int>& getNonFullScreenBounds() const noexcept;

	/** Attempts to change the icon associated with this window.
	*/
	virtual void setIcon (const Image& newIcon) = 0;

	/** Sets a constrainer to use if the peer can resize itself.

		The constrainer won't be deleted by this object, so the caller must manage its lifetime.
	*/
	void setConstrainer (ComponentBoundsConstrainer* newConstrainer) noexcept;

	/** Returns the current constrainer, if one has been set. */
	ComponentBoundsConstrainer* getConstrainer() const noexcept             { return constrainer; }

	/** Checks if a point is in the window.

		Coordinates are relative to the top-left of this window. If trueIfInAChildWindow
		is false, then this returns false if the point is actually inside a child of this
		window.
	*/
	virtual bool contains (const Point<int>& position, bool trueIfInAChildWindow) const = 0;

	/** Returns the size of the window frame that's around this window.

		Whether or not the window has a normal window frame depends on the flags
		that were set when the window was created by Component::addToDesktop()
	*/
	virtual BorderSize<int> getFrameSize() const = 0;

	/** This is called when the window's bounds change.

		A peer implementation must call this when the window is moved and resized, so that
		this method can pass the message on to the component.
	*/
	void handleMovedOrResized();

	/** This is called if the screen resolution changes.

		A peer implementation must call this if the monitor arrangement changes or the available
		screen size changes.
	*/
	virtual void handleScreenSizeChange();

	/** This is called to repaint the component into the given context. */
	void handlePaint (LowLevelGraphicsContext& contextToPaintTo);

	/** Sets this window to either be always-on-top or normal.

		Some kinds of window might not be able to do this, so should return false.
	*/
	virtual bool setAlwaysOnTop (bool alwaysOnTop) = 0;

	/** Brings the window to the top, optionally also giving it focus. */
	virtual void toFront (bool makeActive) = 0;

	/** Moves the window to be just behind another one. */
	virtual void toBehind (ComponentPeer* other) = 0;

	/** Called when the window is brought to the front, either by the OS or by a call
		to toFront().
	*/
	void handleBroughtToFront();

	/** True if the window has the keyboard focus. */
	virtual bool isFocused() const = 0;

	/** Tries to give the window keyboard focus. */
	virtual void grabFocus() = 0;

	/** Called when the window gains keyboard focus. */
	void handleFocusGain();
	/** Called when the window loses keyboard focus. */
	void handleFocusLoss();

	Component* getLastFocusedSubcomponent() const noexcept;

	/** Called when a key is pressed.

		For keycode info, see the KeyPress class.
		Returns true if the keystroke was used.
	*/
	bool handleKeyPress (int keyCode, juce_wchar textCharacter);

	/** Called whenever a key is pressed or released.
		Returns true if the keystroke was used.
	*/
	bool handleKeyUpOrDown (bool isKeyDown);

	/** Called whenever a modifier key is pressed or released. */
	void handleModifierKeysChange();

	/** Tells the window that text input may be required at the given position.
		This may cause things like a virtual on-screen keyboard to appear, depending
		on the OS.
	*/
	virtual void textInputRequired (const Point<int>& position) = 0;

	/** If there's some kind of OS input-method in progress, this should dismiss it. */
	virtual void dismissPendingTextInput();

	/** Returns the currently focused TextInputTarget, or null if none is found. */
	TextInputTarget* findCurrentTextInputTarget();

	/** Invalidates a region of the window to be repainted asynchronously. */
	virtual void repaint (const Rectangle<int>& area) = 0;

	/** This can be called (from the message thread) to cause the immediate redrawing
		of any areas of this window that need repainting.

		You shouldn't ever really need to use this, it's mainly for special purposes
		like supporting audio plugins where the host's event loop is out of our control.
	*/
	virtual void performAnyPendingRepaintsNow() = 0;

	/** Changes the window's transparency. */
	virtual void setAlpha (float newAlpha) = 0;

	void handleMouseEvent (int touchIndex, const Point<int>& positionWithinPeer, const ModifierKeys& newMods, int64 time);
	void handleMouseWheel (int touchIndex, const Point<int>& positionWithinPeer, int64 time, float x, float y);

	void handleUserClosingWindow();

	bool handleFileDragMove (const StringArray& files, const Point<int>& position);
	bool handleFileDragExit (const StringArray& files);
	bool handleFileDragDrop (const StringArray& files, const Point<int>& position);

	/** Resets the masking region.

		The subclass should call this every time it's about to call the handlePaint
		method.

		@see addMaskedRegion
	*/
	void clearMaskedRegion();

	/** Adds a rectangle to the set of areas not to paint over.

		A component can call this on its peer during its paint() method, to signal
		that the painting code should ignore a given region. The reason
		for this is to stop embedded windows (such as OpenGL) getting painted over.

		The masked region is cleared each time before a paint happens, so a component
		will have to make sure it calls this every time it's painted.
	*/
	void addMaskedRegion (const Rectangle<int>& area);

	/** Returns the number of currently-active peers.

		@see getPeer
	*/
	static int getNumPeers() noexcept;

	/** Returns one of the currently-active peers.

		@see getNumPeers
	*/
	static ComponentPeer* getPeer (int index) noexcept;

	/** Checks if this peer object is valid.

		@see getNumPeers
	*/
	static bool isValidPeer (const ComponentPeer* peer) noexcept;

	virtual StringArray getAvailableRenderingEngines();
	virtual int getCurrentRenderingEngine() const;
	virtual void setCurrentRenderingEngine (int index);

protected:

	Component* const component;
	const int styleFlags;
	RectangleList maskedRegion;
	Rectangle<int> lastNonFullscreenBounds;
	uint32 lastPaintTime;
	ComponentBoundsConstrainer* constrainer;

	static void updateCurrentModifiers() noexcept;

private:

	WeakReference<Component> lastFocusedComponent, dragAndDropTargetComponent;
	Component* lastDragAndDropCompUnderMouse;
	const uint32 uniqueID;
	bool fakeMouseMessageSent : 1, isWindowMinimised : 1;

	friend class Component;
	friend class Desktop;
	static ComponentPeer* getPeerFor (const Component* component) noexcept;

	void setLastDragDropTarget (Component* comp);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentPeer);
};

#endif   // __JUCE_COMPONENTPEER_JUCEHEADER__

/*** End of inlined file: juce_ComponentPeer.h ***/


#endif
#ifndef __JUCE_DIALOGWINDOW_JUCEHEADER__

/*** Start of inlined file: juce_DialogWindow.h ***/
#ifndef __JUCE_DIALOGWINDOW_JUCEHEADER__
#define __JUCE_DIALOGWINDOW_JUCEHEADER__

/**
	A dialog-box style window.

	This class is a convenient way of creating a DocumentWindow with a close button
	that can be triggered by pressing the escape key.

	Any of the methods available to a DocumentWindow or ResizableWindow are also
	available to this, so it can be made resizable, have a menu bar, etc.

	To add items to the box, see the ResizableWindow::setContentOwned() or
	ResizableWindow::setContentNonOwned() methods. Don't add components directly to this
	class - always put them in a content component!

	You'll need to override the DocumentWindow::closeButtonPressed() method to handle
	the user clicking the close button - for more info, see the DocumentWindow
	help.

	@see DocumentWindow, ResizableWindow
*/
class JUCE_API  DialogWindow   : public DocumentWindow
{
public:

	/** Creates a DialogWindow.

		@param name                 the name to give the component - this is also
									the title shown at the top of the window. To change
									this later, use setName()
		@param backgroundColour     the colour to use for filling the window's background.
		@param escapeKeyTriggersCloseButton if true, then pressing the escape key will cause the
											close button to be triggered
		@param addToDesktop         if true, the window will be automatically added to the
									desktop; if false, you can use it as a child component
	*/
	DialogWindow (const String& name,
				  const Colour& backgroundColour,
				  bool escapeKeyTriggersCloseButton,
				  bool addToDesktop = true);

	/** Destructor.
		If a content component has been set with setContentOwned(), it will be deleted.
	*/
	~DialogWindow();

	/** Easy way of quickly showing a dialog box containing a given component.

		This will open and display a DialogWindow containing a given component, making it
		modal, but returning immediately to allow the dialog to finish in its own time. If
		you want to block and run a modal loop until the dialog is dismissed, use showModalDialog()
		instead.

		To close the dialog programatically, you should call exitModalState (returnValue) on
		the DialogWindow that is created. To find a pointer to this window from your
		contentComponent, you can do something like this:
		@code
		Dialogwindow* dw = contentComponent->findParentComponentOfClass<DialogWindow>();

		if (dw != nullptr)
			dw->exitModalState (1234);
		@endcode

		@param dialogTitle          the dialog box's title
		@param contentComponent     the content component for the dialog box. Make sure
									that this has been set to the size you want it to
									be before calling this method. The component won't
									be deleted by this call, so you can re-use it or delete
									it afterwards
		@param componentToCentreAround  if this is non-zero, it indicates a component that
									you'd like to show this dialog box in front of. See the
									DocumentWindow::centreAroundComponent() method for more
									info on this parameter
		@param backgroundColour     a colour to use for the dialog box's background colour
		@param escapeKeyTriggersCloseButton if true, then pressing the escape key will cause the
											close button to be triggered
		@param shouldBeResizable    if true, the dialog window has either a resizable border, or
									a corner resizer
		@param useBottomRightCornerResizer     if shouldBeResizable is true, this indicates whether
									to use a border or corner resizer component. See ResizableWindow::setResizable()
	*/
	static void showDialog (const String& dialogTitle,
							Component* contentComponent,
							Component* componentToCentreAround,
							const Colour& backgroundColour,
							bool escapeKeyTriggersCloseButton,
							bool shouldBeResizable = false,
							bool useBottomRightCornerResizer = false);

	/** Easy way of quickly showing a dialog box containing a given component.

		This will open and display a DialogWindow containing a given component, returning
		when the user clicks its close button.

		It returns the value that was returned by the dialog box's runModalLoop() call.

		To close the dialog programatically, you should call exitModalState (returnValue) on
		the DialogWindow that is created. To find a pointer to this window from your
		contentComponent, you can do something like this:
		@code
		Dialogwindow* dw = contentComponent->findParentComponentOfClass<DialogWindow>();

		if (dw != nullptr)
			dw->exitModalState (1234);
		@endcode

		@param dialogTitle          the dialog box's title
		@param contentComponent     the content component for the dialog box. Make sure
									that this has been set to the size you want it to
									be before calling this method. The component won't
									be deleted by this call, so you can re-use it or delete
									it afterwards
		@param componentToCentreAround  if this is non-zero, it indicates a component that
									you'd like to show this dialog box in front of. See the
									DocumentWindow::centreAroundComponent() method for more
									info on this parameter
		@param backgroundColour     a colour to use for the dialog box's background colour
		@param escapeKeyTriggersCloseButton if true, then pressing the escape key will cause the
											close button to be triggered
		@param shouldBeResizable    if true, the dialog window has either a resizable border, or
									a corner resizer
		@param useBottomRightCornerResizer     if shouldBeResizable is true, this indicates whether
									to use a border or corner resizer component. See ResizableWindow::setResizable()
	*/
   #if JUCE_MODAL_LOOPS_PERMITTED || DOXYGEN
	static int showModalDialog (const String& dialogTitle,
								Component* contentComponent,
								Component* componentToCentreAround,
								const Colour& backgroundColour,
								bool escapeKeyTriggersCloseButton,
								bool shouldBeResizable = false,
								bool useBottomRightCornerResizer = false);
   #endif

protected:
	/** @internal */
	void resized();

private:

	bool escapeKeyTriggersCloseButton;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DialogWindow);
};

#endif   // __JUCE_DIALOGWINDOW_JUCEHEADER__

/*** End of inlined file: juce_DialogWindow.h ***/


#endif
#ifndef __JUCE_DOCUMENTWINDOW_JUCEHEADER__

#endif
#ifndef __JUCE_NATIVEMESSAGEBOX_JUCEHEADER__

/*** Start of inlined file: juce_NativeMessageBox.h ***/
#ifndef __JUCE_NATIVEMESSAGEBOX_JUCEHEADER__
#define __JUCE_NATIVEMESSAGEBOX_JUCEHEADER__

class NativeMessageBox
{
public:
	/** Shows a dialog box that just has a message and a single 'ok' button to close it.

		If the callback parameter is null, the box is shown modally, and the method will
		block until the user has clicked the button (or pressed the escape or return keys).
		If the callback parameter is non-null, the box will be displayed and placed into a
		modal state, but this method will return immediately, and the callback will be invoked
		later when the user dismisses the box.

		@param iconType     the type of icon to show
		@param title        the headline to show at the top of the box
		@param message      a longer, more descriptive message to show underneath the title
		@param associatedComponent   if this is non-null, it specifies the component that the
							alert window should be associated with. Depending on the look
							and feel, this might be used for positioning of the alert window.
	*/
   #if JUCE_MODAL_LOOPS_PERMITTED
	static void JUCE_CALLTYPE showMessageBox (AlertWindow::AlertIconType iconType,
											  const String& title,
											  const String& message,
											  Component* associatedComponent = nullptr);
   #endif

	/** Shows a dialog box that just has a message and a single 'ok' button to close it.

		If the callback parameter is null, the box is shown modally, and the method will
		block until the user has clicked the button (or pressed the escape or return keys).
		If the callback parameter is non-null, the box will be displayed and placed into a
		modal state, but this method will return immediately, and the callback will be invoked
		later when the user dismisses the box.

		@param iconType     the type of icon to show
		@param title        the headline to show at the top of the box
		@param message      a longer, more descriptive message to show underneath the title
		@param associatedComponent   if this is non-null, it specifies the component that the
							alert window should be associated with. Depending on the look
							and feel, this might be used for positioning of the alert window.
	*/
	static void JUCE_CALLTYPE showMessageBoxAsync (AlertWindow::AlertIconType iconType,
												   const String& title,
												   const String& message,
												   Component* associatedComponent = nullptr);

	/** Shows a dialog box with two buttons.

		Ideal for ok/cancel or yes/no choices. The return key can also be used
		to trigger the first button, and the escape key for the second button.

		If the callback parameter is null, the box is shown modally, and the method will
		block until the user has clicked the button (or pressed the escape or return keys).
		If the callback parameter is non-null, the box will be displayed and placed into a
		modal state, but this method will return immediately, and the callback will be invoked
		later when the user dismisses the box.

		@param iconType     the type of icon to show
		@param title        the headline to show at the top of the box
		@param message      a longer, more descriptive message to show underneath the title
		@param associatedComponent   if this is non-null, it specifies the component that the
							alert window should be associated with. Depending on the look
							and feel, this might be used for positioning of the alert window.
		@param callback     if this is non-null, the menu will be launched asynchronously,
							returning immediately, and the callback will receive a call to its
							modalStateFinished() when the box is dismissed, with its parameter
							being 1 if the ok button was pressed, or 0 for cancel, The callback object
							will be owned and deleted by the system, so make sure that it works
							safely and doesn't keep any references to objects that might be deleted
							before it gets called.
		@returns true if button 1 was clicked, false if it was button 2. If the callback parameter
				 is not null, the method always returns false, and the user's choice is delivered
				 later by the callback.
	*/
	static bool JUCE_CALLTYPE showOkCancelBox (AlertWindow::AlertIconType iconType,
											   const String& title,
											   const String& message,
											#if JUCE_MODAL_LOOPS_PERMITTED
											   Component* associatedComponent = nullptr,
											   ModalComponentManager::Callback* callback = nullptr);
											#else
											   Component* associatedComponent,
											   ModalComponentManager::Callback* callback);
											#endif

	/** Shows a dialog box with three buttons.

		Ideal for yes/no/cancel boxes.

		The escape key can be used to trigger the third button.

		If the callback parameter is null, the box is shown modally, and the method will
		block until the user has clicked the button (or pressed the escape or return keys).
		If the callback parameter is non-null, the box will be displayed and placed into a
		modal state, but this method will return immediately, and the callback will be invoked
		later when the user dismisses the box.

		@param iconType     the type of icon to show
		@param title        the headline to show at the top of the box
		@param message      a longer, more descriptive message to show underneath the title
		@param associatedComponent   if this is non-null, it specifies the component that the
							alert window should be associated with. Depending on the look
							and feel, this might be used for positioning of the alert window.
		@param callback     if this is non-null, the menu will be launched asynchronously,
							returning immediately, and the callback will receive a call to its
							modalStateFinished() when the box is dismissed, with its parameter
							being 1 if the "yes" button was pressed, 2 for the "no" button, or 0
							if it was cancelled, The callback object will be owned and deleted by the
							system, so make sure that it works safely and doesn't keep any references
							to objects that might be deleted before it gets called.

		@returns If the callback parameter has been set, this returns 0. Otherwise, it returns one
				 of the following values:
				 - 0 if 'cancel' was pressed
				 - 1 if 'yes' was pressed
				 - 2 if 'no' was pressed
	*/
	static int JUCE_CALLTYPE showYesNoCancelBox (AlertWindow::AlertIconType iconType,
												 const String& title,
												 const String& message,
											   #if JUCE_MODAL_LOOPS_PERMITTED
												 Component* associatedComponent = nullptr,
												 ModalComponentManager::Callback* callback = nullptr);
											   #else
												 Component* associatedComponent,
												 ModalComponentManager::Callback* callback);
											   #endif
};

#endif   // __JUCE_NATIVEMESSAGEBOX_JUCEHEADER__

/*** End of inlined file: juce_NativeMessageBox.h ***/


#endif
#ifndef __JUCE_RESIZABLEWINDOW_JUCEHEADER__

#endif
#ifndef __JUCE_THREADWITHPROGRESSWINDOW_JUCEHEADER__

/*** Start of inlined file: juce_ThreadWithProgressWindow.h ***/
#ifndef __JUCE_THREADWITHPROGRESSWINDOW_JUCEHEADER__
#define __JUCE_THREADWITHPROGRESSWINDOW_JUCEHEADER__

/**
	A thread that automatically pops up a modal dialog box with a progress bar
	and cancel button while it's busy running.

	These are handy for performing some sort of task while giving the user feedback
	about how long there is to go, etc.

	E.g. @code
	class MyTask  : public ThreadWithProgressWindow
	{
	public:
		MyTask()    : ThreadWithProgressWindow ("busy...", true, true)
		{
		}

		void run()
		{
			for (int i = 0; i < thingsToDo; ++i)
			{
				// must check this as often as possible, because this is
				// how we know if the user's pressed 'cancel'
				if (threadShouldExit())
					break;

				// this will update the progress bar on the dialog box
				setProgress (i / (double) thingsToDo);

				//   ... do the business here...
			}
		}
	};

	void doTheTask()
	{
		MyTask m;

		if (m.runThread())
		{
			// thread finished normally..
		}
		else
		{
			// user pressed the cancel button..
		}
	}

	@endcode

	@see Thread, AlertWindow
*/
class JUCE_API  ThreadWithProgressWindow  : public Thread,
											private Timer
{
public:

	/** Creates the thread.

		Initially, the dialog box won't be visible, it'll only appear when the
		runThread() method is called.

		@param windowTitle              the title to go at the top of the dialog box
		@param hasProgressBar           whether the dialog box should have a progress bar (see
										setProgress() )
		@param hasCancelButton          whether the dialog box should have a cancel button
		@param timeOutMsWhenCancelling  when 'cancel' is pressed, this is how long to wait for
										the thread to stop before killing it forcibly (see
										Thread::stopThread() )
		@param cancelButtonText         the text that should be shown in the cancel button
										(if it has one)
	*/
	ThreadWithProgressWindow (const String& windowTitle,
							  bool hasProgressBar,
							  bool hasCancelButton,
							  int timeOutMsWhenCancelling = 10000,
							  const String& cancelButtonText = "Cancel");

	/** Destructor. */
	~ThreadWithProgressWindow();

	/** Starts the thread and waits for it to finish.

		This will start the thread, make the dialog box appear, and wait until either
		the thread finishes normally, or until the cancel button is pressed.

		Before returning, the dialog box will be hidden.

		@param threadPriority   the priority to use when starting the thread - see
								Thread::startThread() for values
		@returns true if the thread finished normally; false if the user pressed cancel
	*/
	bool runThread (int threadPriority = 5);

	/** The thread should call this periodically to update the position of the progress bar.

		@param newProgress  the progress, from 0.0 to 1.0
		@see setStatusMessage
	*/
	void setProgress (double newProgress);

	/** The thread can call this to change the message that's displayed in the dialog box.
	*/
	void setStatusMessage (const String& newStatusMessage);

	/** Returns the AlertWindow that is being used.
	*/
	AlertWindow* getAlertWindow() const noexcept        { return alertWindow; }

private:

	void timerCallback();

	double progress;
	ScopedPointer <AlertWindow> alertWindow;
	String message;
	CriticalSection messageLock;
	const int timeOutMsWhenCancelling;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThreadWithProgressWindow);
};

#endif   // __JUCE_THREADWITHPROGRESSWINDOW_JUCEHEADER__

/*** End of inlined file: juce_ThreadWithProgressWindow.h ***/


#endif
#ifndef __JUCE_TOOLTIPWINDOW_JUCEHEADER__

#endif
#ifndef __JUCE_TOPLEVELWINDOW_JUCEHEADER__

#endif
#ifndef __JUCE_APPLICATIONCOMMANDID_JUCEHEADER__

#endif
#ifndef __JUCE_APPLICATIONCOMMANDINFO_JUCEHEADER__

#endif
#ifndef __JUCE_APPLICATIONCOMMANDMANAGER_JUCEHEADER__

#endif
#ifndef __JUCE_APPLICATIONCOMMANDTARGET_JUCEHEADER__

#endif
#ifndef __JUCE_KEYPRESSMAPPINGSET_JUCEHEADER__

/*** Start of inlined file: juce_KeyPressMappingSet.h ***/
#ifndef __JUCE_KEYPRESSMAPPINGSET_JUCEHEADER__
#define __JUCE_KEYPRESSMAPPINGSET_JUCEHEADER__

/**
	Manages and edits a list of keypresses, which it uses to invoke the appropriate
	command in a ApplicationCommandManager.

	Normally, you won't actually create a KeyPressMappingSet directly, because
	each ApplicationCommandManager contains its own KeyPressMappingSet, so typically
	you'd create yourself an ApplicationCommandManager, and call its
	ApplicationCommandManager::getKeyMappings() method to get a pointer to its
	KeyPressMappingSet.

	For one of these to actually use keypresses, you'll need to add it as a KeyListener
	to the top-level component for which you want to handle keystrokes. So for example:

	@code
	class MyMainWindow  : public Component
	{
		ApplicationCommandManager* myCommandManager;

	public:
		MyMainWindow()
		{
			myCommandManager = new ApplicationCommandManager();

			// first, make sure the command manager has registered all the commands that its
			// targets can perform..
			myCommandManager->registerAllCommandsForTarget (myCommandTarget1);
			myCommandManager->registerAllCommandsForTarget (myCommandTarget2);

			// this will use the command manager to initialise the KeyPressMappingSet with
			// the default keypresses that were specified when the targets added their commands
			// to the manager.
			myCommandManager->getKeyMappings()->resetToDefaultMappings();

			// having set up the default key-mappings, you might now want to load the last set
			// of mappings that the user configured.
			myCommandManager->getKeyMappings()->restoreFromXml (lastSavedKeyMappingsXML);

			// Now tell our top-level window to send any keypresses that arrive to the
			// KeyPressMappingSet, which will use them to invoke the appropriate commands.
			addKeyListener (myCommandManager->getKeyMappings());
		}

		...
	}
	@endcode

	KeyPressMappingSet derives from ChangeBroadcaster so that interested parties can
	register to be told when a command or mapping is added, removed, etc.

	There's also a UI component called KeyMappingEditorComponent that can be used
	to easily edit the key mappings.

	@see Component::addKeyListener(), KeyMappingEditorComponent, ApplicationCommandManager
*/
class JUCE_API  KeyPressMappingSet  : public KeyListener,
									  public ChangeBroadcaster,
									  public FocusChangeListener
{
public:

	/** Creates a KeyPressMappingSet for a given command manager.

		Normally, you won't actually create a KeyPressMappingSet directly, because
		each ApplicationCommandManager contains its own KeyPressMappingSet, so the
		best thing to do is to create your ApplicationCommandManager, and use the
		ApplicationCommandManager::getKeyMappings() method to access its mappings.

		When a suitable keypress happens, the manager's invoke() method will be
		used to invoke the appropriate command.

		@see ApplicationCommandManager
	*/
	explicit KeyPressMappingSet (ApplicationCommandManager* commandManager);

	/** Creates an copy of a KeyPressMappingSet. */
	KeyPressMappingSet (const KeyPressMappingSet& other);

	/** Destructor. */
	~KeyPressMappingSet();

	ApplicationCommandManager* getCommandManager() const noexcept       { return commandManager; }

	/** Returns a list of keypresses that are assigned to a particular command.

		@param commandID        the command's ID
	*/
	Array<KeyPress> getKeyPressesAssignedToCommand (CommandID commandID) const;

	/** Assigns a keypress to a command.

		If the keypress is already assigned to a different command, it will first be
		removed from that command, to avoid it triggering multiple functions.

		@param commandID    the ID of the command that you want to add a keypress to. If
							this is 0, the keypress will be removed from anything that it
							was previously assigned to, but not re-assigned
		@param newKeyPress  the new key-press
		@param insertIndex  if this is less than zero, the key will be appended to the
							end of the list of keypresses; otherwise the new keypress will
							be inserted into the existing list at this index
	*/
	void addKeyPress (CommandID commandID,
					  const KeyPress& newKeyPress,
					  int insertIndex = -1);

	/** Reset all mappings to the defaults, as dictated by the ApplicationCommandManager.

		@see resetToDefaultMapping
	*/
	void resetToDefaultMappings();

	/** Resets all key-mappings to the defaults for a particular command.

		@see resetToDefaultMappings
	*/
	void resetToDefaultMapping (CommandID commandID);

	/** Removes all keypresses that are assigned to any commands. */
	void clearAllKeyPresses();

	/** Removes all keypresses that are assigned to a particular command. */
	void clearAllKeyPresses (CommandID commandID);

	/** Removes one of the keypresses that are assigned to a command.

		See the getKeyPressesAssignedToCommand() for the list of keypresses to
		which the keyPressIndex refers.
	*/
	void removeKeyPress (CommandID commandID, int keyPressIndex);

	/** Removes a keypress from any command that it may be assigned to.
	*/
	void removeKeyPress (const KeyPress& keypress);

	/** Returns true if the given command is linked to this key. */
	bool containsMapping (CommandID commandID, const KeyPress& keyPress) const noexcept;

	/** Looks for a command that corresponds to a keypress.

		@returns the UID of the command or 0 if none was found
	*/
	CommandID findCommandForKeyPress (const KeyPress& keyPress) const noexcept;

	/** Tries to recreate the mappings from a previously stored state.

		The XML passed in must have been created by the createXml() method.

		If the stored state makes any reference to commands that aren't
		currently available, these will be ignored.

		If the set of mappings being loaded was a set of differences (using createXml (true)),
		then this will call resetToDefaultMappings() and then merge the saved mappings
		on top. If the saved set was created with createXml (false), then this method
		will first clear all existing mappings and load the saved ones as a complete set.

		@returns true if it manages to load the XML correctly
		@see createXml
	*/
	bool restoreFromXml (const XmlElement& xmlVersion);

	/** Creates an XML representation of the current mappings.

		This will produce a lump of XML that can be later reloaded using
		restoreFromXml() to recreate the current mapping state.

		The object that is returned must be deleted by the caller.

		@param saveDifferencesFromDefaultSet    if this is false, then all keypresses
							will be saved into the XML. If it's true, then the XML will
							only store the differences between the current mappings and
							the default mappings you'd get from calling resetToDefaultMappings().
							The advantage of saving a set of differences from the default is that
							if you change the default mappings (in a new version of your app, for
							example), then these will be merged into a user's saved preferences.

		@see restoreFromXml
	*/
	XmlElement* createXml (bool saveDifferencesFromDefaultSet) const;

	/** @internal */
	bool keyPressed (const KeyPress& key, Component* originatingComponent);
	/** @internal */
	bool keyStateChanged (bool isKeyDown, Component* originatingComponent);
	/** @internal */
	void globalFocusChanged (Component* focusedComponent);

private:

	ApplicationCommandManager* commandManager;

	struct CommandMapping
	{
		CommandID commandID;
		Array <KeyPress> keypresses;
		bool wantsKeyUpDownCallbacks;
	};

	OwnedArray <CommandMapping> mappings;

	struct KeyPressTime
	{
		KeyPress key;
		uint32 timeWhenPressed;
	};

	OwnedArray <KeyPressTime> keysDown;

	void handleMessage (const Message& message);

	void invokeCommand (const CommandID commandID,
						const KeyPress& keyPress,
						const bool isKeyDown,
						const int millisecsSinceKeyPressed,
						Component* const originatingComponent) const;

	KeyPressMappingSet& operator= (const KeyPressMappingSet&);
	JUCE_LEAK_DETECTOR (KeyPressMappingSet);
};

#endif   // __JUCE_KEYPRESSMAPPINGSET_JUCEHEADER__

/*** End of inlined file: juce_KeyPressMappingSet.h ***/


#endif
#ifndef __JUCE_APPLICATION_JUCEHEADER__

/*** Start of inlined file: juce_Application.h ***/
#ifndef __JUCE_APPLICATION_JUCEHEADER__
#define __JUCE_APPLICATION_JUCEHEADER__

/**
	An instance of this class is used to specify initialisation and shutdown
	code for the application.

	An application that wants to run in the JUCE framework needs to declare a
	subclass of JUCEApplication and implement its various pure virtual methods.

	It then needs to use the START_JUCE_APPLICATION macro somewhere in a cpp file
	to declare an instance of this class and generate a suitable platform-specific
	main() function.

	e.g. @code
		class MyJUCEApp  : public JUCEApplication
		{
		public:
			MyJUCEApp()
			{
			}

			~MyJUCEApp()
			{
			}

			void initialise (const String& commandLine)
			{
				myMainWindow = new MyApplicationWindow();
				myMainWindow->setBounds (100, 100, 400, 500);
				myMainWindow->setVisible (true);
			}

			void shutdown()
			{
				myMainWindow = 0;
			}

			const String getApplicationName()
			{
				return "Super JUCE-o-matic";
			}

			const String getApplicationVersion()
			{
				return "1.0";
			}

		private:
			ScopedPointer <MyApplicationWindow> myMainWindow;
		};

		// this creates wrapper code to actually launch the app properly.
		START_JUCE_APPLICATION (MyJUCEApp)
	@endcode

	@see MessageManager
*/
class JUCE_API  JUCEApplication  : public JUCEApplicationBase,
								   public ApplicationCommandTarget
{
protected:

	/** Constructs a JUCE app object.

		If subclasses implement a constructor or destructor, they shouldn't call any
		JUCE code in there - put your startup/shutdown code in initialise() and
		shutdown() instead.
	*/
	JUCEApplication();

public:
	/** Destructor.

		If subclasses implement a constructor or destructor, they shouldn't call any
		JUCE code in there - put your startup/shutdown code in initialise() and
		shutdown() instead.
	*/
	virtual ~JUCEApplication();

	/** Returns the global instance of the application object being run. */
	static JUCEApplication* getInstance() noexcept          { return dynamic_cast <JUCEApplication*> (JUCEApplicationBase::getInstance()); }

	/** Returns true if the application hasn't yet completed its initialise() method
		and entered the main event loop.

		This is handy for things like splash screens to know when the app's up-and-running
		properly.
	*/
	bool isInitialising() const noexcept                    { return stillInitialising; }

	/** Returns the application's name.

		An application must implement this to name itself.
	*/
	virtual const String getApplicationName() = 0;

	/** Returns the application's version number.
	*/
	virtual const String getApplicationVersion() = 0;

	/** Checks whether multiple instances of the app are allowed.

		If you application class returns true for this, more than one instance is
		permitted to run (except on OSX where the OS automatically stops you launching
		a second instance of an app without explicitly starting it from the command-line).

		If it's false, the second instance won't start, but it you will still get a
		callback to anotherInstanceStarted() to tell you about this - which
		gives you a chance to react to what the user was trying to do.
	*/
	virtual bool moreThanOneInstanceAllowed();

	/** Indicates that the user has tried to start up another instance of the app.

		This will get called even if moreThanOneInstanceAllowed() is false.
	*/
	virtual void anotherInstanceStarted (const String& commandLine);

	/** Called when the operating system is trying to close the application.

		The default implementation of this method is to call quit(), but it may
		be overloaded to ignore the request or do some other special behaviour
		instead. For example, you might want to offer the user the chance to save
		their changes before quitting, and give them the chance to cancel.

		If you want to send a quit signal to your app, this is the correct method
		to call, because it means that requests that come from the system get handled
		in the same way as those from your own application code. So e.g. you'd
		call this method from a "quit" item on a menu bar.
	*/
	virtual void systemRequestedQuit();

	/** If any unhandled exceptions make it through to the message dispatch loop, this
		callback will be triggered, in case you want to log them or do some other
		type of error-handling.

		If the type of exception is derived from the std::exception class, the pointer
		passed-in will be valid. If the exception is of unknown type, this pointer
		will be null.
	*/
	virtual void unhandledException (const std::exception* e,
									 const String& sourceFilename,
									 int lineNumber);

	/** Signals that the main message loop should stop and the application should terminate.

		This isn't synchronous, it just posts a quit message to the main queue, and
		when this message arrives, the message loop will stop, the shutdown() method
		will be called, and the app will exit.

		Note that this will cause an unconditional quit to happen, so if you need an
		extra level before this, e.g. to give the user the chance to save their work
		and maybe cancel the quit, you'll need to handle this in the systemRequestedQuit()
		method - see that method's help for more info.

		@see MessageManager
	*/
	static void quit();

	/** Sets the value that should be returned as the application's exit code when the
		app quits.

		This is the value that's returned by the main() function. Normally you'd leave this
		as 0 unless you want to indicate an error code.

		@see getApplicationReturnValue
	*/
	void setApplicationReturnValue (int newReturnValue) noexcept;

	/** Returns the value that has been set as the application's exit code.
		@see setApplicationReturnValue
	*/
	int getApplicationReturnValue() const noexcept                  { return appReturnValue; }

	/** Returns the application's command line parameters. */
	const String& getCommandLineParameters() const noexcept         { return commandLineParameters; }

	/** Returns true if this executable is running as an app (as opposed to being a plugin
		or other kind of shared library. */
	static inline bool isStandaloneApp() noexcept                   { return createInstance != nullptr; }

	/** @internal */
	ApplicationCommandTarget* getNextCommandTarget();
	/** @internal */
	void getCommandInfo (CommandID, ApplicationCommandInfo&);
	/** @internal */
	void getAllCommands (Array <CommandID>&);
	/** @internal */
	bool perform (const InvocationInfo&);

   #ifndef DOXYGEN
	// The following methods are internal calls - not for public use.
	static int main (const String& commandLine);
	static int main (int argc, const char* argv[]);
	static void sendUnhandledException (const std::exception*, const char* sourceFile, int lineNumber);
	bool initialiseApp (const String& commandLine);
	int shutdownApp();
   #endif

private:

	String commandLineParameters;
	ScopedPointer<InterProcessLock> appLock;
	ScopedPointer<ActionListener> broadcastCallback;
	int appReturnValue;
	bool stillInitialising;

	JUCE_DECLARE_NON_COPYABLE (JUCEApplication);
};

#endif   // __JUCE_APPLICATION_JUCEHEADER__

/*** End of inlined file: juce_Application.h ***/


#endif
#ifndef __JUCE_INITIALISATION_JUCEHEADER__

/*** Start of inlined file: juce_Initialisation.h ***/
#ifndef __JUCE_INITIALISATION_JUCEHEADER__
#define __JUCE_INITIALISATION_JUCEHEADER__

/** Initialises Juce's GUI classes.

	If you're embedding Juce into an application that uses its own event-loop rather
	than using the START_JUCE_APPLICATION macro, call this function before making any
	Juce calls, to make sure things are initialised correctly.

	Note that if you're creating a Juce DLL for Windows, you may also need to call the
	Process::setCurrentModuleInstanceHandle() method.

	@see shutdownJuce_GUI()
*/
JUCE_API void JUCE_CALLTYPE  initialiseJuce_GUI();

/** Clears up any static data being used by Juce's GUI classes.

	If you're embedding Juce into an application that uses its own event-loop rather
	than using the START_JUCE_APPLICATION macro, call this function in your shutdown
	code to clean up any juce objects that might be lying around.

	@see initialiseJuce_GUI()
*/
JUCE_API void JUCE_CALLTYPE  shutdownJuce_GUI();

/** A utility object that helps you initialise and shutdown Juce correctly
	using an RAII pattern.

	When an instance of this class is created, it calls initialiseJuce_GUI(),
	and when it's deleted, it calls shutdownJuce_GUI(), which lets you easily
	make sure that these functions are matched correctly.

	This class is particularly handy to use at the beginning of a console app's
	main() function, because it'll take care of shutting down whenever you return
	from the main() call.

	@see ScopedJuceInitialiser_NonGUI
*/
class ScopedJuceInitialiser_GUI
{
public:
	/** The constructor simply calls initialiseJuce_GUI(). */
	ScopedJuceInitialiser_GUI()         { initialiseJuce_GUI(); }

	/** The destructor simply calls shutdownJuce_GUI(). */
	~ScopedJuceInitialiser_GUI()        { shutdownJuce_GUI(); }
};

/*
	To start a JUCE app, use this macro: START_JUCE_APPLICATION (AppSubClass) where
	AppSubClass is the name of a class derived from JUCEApplication.

	See the JUCEApplication class documentation (juce_Application.h) for more details.

*/
#if JUCE_ANDROID
  #define START_JUCE_APPLICATION(AppClass) \
	juce::JUCEApplication* juce_CreateApplication() { return new AppClass(); }

#elif defined (JUCE_GCC) || defined (__MWERKS__)

  #define START_JUCE_APPLICATION(AppClass) \
	static juce::JUCEApplicationBase* juce_CreateApplication() { return new AppClass(); } \
	int main (int argc, char* argv[]) \
	{ \
		juce::JUCEApplication::createInstance = &juce_CreateApplication; \
		return juce::JUCEApplication::main (argc, (const char**) argv); \
	}

#elif JUCE_WINDOWS

  #ifdef _CONSOLE
	#define START_JUCE_APPLICATION(AppClass) \
		static juce::JUCEApplicationBase* juce_CreateApplication() { return new AppClass(); } \
		int main (int, char* argv[]) \
		{ \
			juce::JUCEApplication::createInstance = &juce_CreateApplication; \
			return juce::JUCEApplication::main (juce::Process::getCurrentCommandLineParams()); \
		}
  #elif ! defined (_AFXDLL)
	#ifdef _WINDOWS_
	  #define START_JUCE_APPLICATION(AppClass) \
		  static juce::JUCEApplicationBase* juce_CreateApplication() { return new AppClass(); } \
		  int WINAPI WinMain (HINSTANCE, HINSTANCE, LPSTR, int) \
		  { \
			  juce::JUCEApplication::createInstance = &juce_CreateApplication; \
			  return juce::JUCEApplication::main (juce::Process::getCurrentCommandLineParams()); \
		  }
	#else
	  #define START_JUCE_APPLICATION(AppClass) \
		  static juce::JUCEApplicationBase* juce_CreateApplication() { return new AppClass(); } \
		  int __stdcall WinMain (void*, void*, const char*, int) \
		  { \
			  juce::JUCEApplication::createInstance = &juce_CreateApplication; \
			  return juce::JUCEApplication::main (juce::Process::getCurrentCommandLineParams()); \
		  }
	#endif
  #endif

#endif

#endif   // __JUCE_INITIALISATION_JUCEHEADER__

/*** End of inlined file: juce_Initialisation.h ***/


#endif
#ifndef __JUCE_BUBBLECOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_BubbleComponent.h ***/
#ifndef __JUCE_BUBBLECOMPONENT_JUCEHEADER__
#define __JUCE_BUBBLECOMPONENT_JUCEHEADER__

/**
	A component for showing a message or other graphics inside a speech-bubble-shaped
	outline, pointing at a location on the screen.

	This is a base class that just draws and positions the bubble shape, but leaves
	the drawing of any content up to a subclass. See BubbleMessageComponent for a subclass
	that draws a text message.

	To use it, create your subclass, then either add it to a parent component or
	put it on the desktop with addToDesktop (0), use setPosition() to
	resize and position it, then make it visible.

	@see BubbleMessageComponent
*/
class JUCE_API  BubbleComponent  : public Component
{
protected:

	/** Creates a BubbleComponent.

		Your subclass will need to implement the getContentSize() and paintContent()
		methods to draw the bubble's contents.
	*/
	BubbleComponent();

public:
	/** Destructor. */
	~BubbleComponent();

	/** A list of permitted placements for the bubble, relative to the co-ordinates
		at which it should be pointing.

		@see setAllowedPlacement
	*/
	enum BubblePlacement
	{
		above   = 1,
		below   = 2,
		left    = 4,
		right   = 8
	};

	/** Tells the bubble which positions it's allowed to put itself in, relative to the
		point at which it's pointing.

		By default when setPosition() is called, the bubble will place itself either
		above, below, left, or right of the target area. You can pass in a bitwise-'or' of
		the values in BubblePlacement to restrict this choice.

		E.g. if you only want your bubble to appear above or below the target area,
		use setAllowedPlacement (above | below);

		@see BubblePlacement
	*/
	void setAllowedPlacement (int newPlacement);

	/** Moves and resizes the bubble to point at a given component.

		This will resize the bubble to fit its content, then find a position for it
		so that it's next to, but doesn't overlap the given component.

		It'll put itself either above, below, or to the side of the component depending
		on where there's the most space, honouring any restrictions that were set
		with setAllowedPlacement().
	*/
	void setPosition (Component* componentToPointTo);

	/** Moves and resizes the bubble to point at a given point.

		This will resize the bubble to fit its content, then position it
		so that the tip of the bubble points to the given co-ordinate. The co-ordinates
		are relative to either the bubble component's parent component if it has one, or
		they are screen co-ordinates if not.

		It'll put itself either above, below, or to the side of this point, depending
		on where there's the most space, honouring any restrictions that were set
		with setAllowedPlacement().
	*/
	void setPosition (int arrowTipX,
					  int arrowTipY);

	/** Moves and resizes the bubble to point at a given rectangle.

		This will resize the bubble to fit its content, then find a position for it
		so that it's next to, but doesn't overlap the given rectangle. The rectangle's
		co-ordinates are relative to either the bubble component's parent component
		if it has one, or they are screen co-ordinates if not.

		It'll put itself either above, below, or to the side of the component depending
		on where there's the most space, honouring any restrictions that were set
		with setAllowedPlacement().
	*/
	void setPosition (const Rectangle<int>& rectangleToPointTo);

protected:

	/** Subclasses should override this to return the size of the content they
		want to draw inside the bubble.
	*/
	virtual void getContentSize (int& width, int& height) = 0;

	/** Subclasses should override this to draw their bubble's contents.

		The graphics object's clip region and the dimensions passed in here are
		set up to paint just the rectangle inside the bubble.
	*/
	virtual void paintContent (Graphics& g, int width, int height) = 0;

public:
	/** @internal */
	void paint (Graphics& g);

private:
	Rectangle<int> content;
	int side, allowablePlacements;
	float arrowTipX, arrowTipY;
	DropShadowEffect shadow;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BubbleComponent);
};

#endif   // __JUCE_BUBBLECOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_BubbleComponent.h ***/


#endif
#ifndef __JUCE_DROPSHADOWER_JUCEHEADER__

#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_GUI_BASICS_JUCEHEADER__

/*** End of inlined file: juce_gui_basics.h ***/

