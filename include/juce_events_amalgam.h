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


/*** Start of inlined file: juce_events.h ***/
#ifndef __JUCE_EVENTS_JUCEHEADER__
#define __JUCE_EVENTS_JUCEHEADER__

#include "juce_core_amalgam.h"

namespace juce
{

// START_AUTOINCLUDE messages, broadcasters, timers,
// interprocess, native/juce_ScopedXLock*
#ifndef __JUCE_APPLICATIONBASE_JUCEHEADER__

/*** Start of inlined file: juce_ApplicationBase.h ***/
#ifndef __JUCE_APPLICATIONBASE_JUCEHEADER__
#define __JUCE_APPLICATIONBASE_JUCEHEADER__

/**
	Abstract base class for application classes.

	This class shouldn't be used directly - you'll normally use JUCEApplication as
	the base for your app, and that inherits from this, adding some more functionality
	to it.

	@see JUCEApplication
*/
class JUCE_API  JUCEApplicationBase
{
protected:

	JUCEApplicationBase();

public:
	/** Destructor. */
	virtual ~JUCEApplicationBase();

	/** Returns the global instance of the application object that's running. */
	static JUCEApplicationBase* getInstance() noexcept          { return appInstance; }

	/** Returns the application's name.
		An application must implement this to name itself.
	*/
	virtual const String getApplicationName() = 0;

	/** Returns the application's version number.
	*/
	virtual const String getApplicationVersion() = 0;

	/** Checks whether multiple instances of the app are allowed.

		If you application class returns true for this, more than one instance is
		permitted to run (except on the Mac where this isn't possible).

		If it's false, the second instance won't start, but it you will still get a
		callback to anotherInstanceStarted() to tell you about this - which
		gives you a chance to react to what the user was trying to do.
	*/
	virtual bool moreThanOneInstanceAllowed() = 0;

	/** Called when the application starts.

		This will be called once to let the application do whatever initialisation
		it needs, create its windows, etc.

		After the method returns, the normal event-dispatch loop will be run,
		until the quit() method is called, at which point the shutdown()
		method will be called to let the application clear up anything it needs
		to delete.

		If during the initialise() method, the application decides not to start-up
		after all, it can just call the quit() method and the event loop won't be run.

		@param commandLineParameters    the line passed in does not include the
										name of the executable, just the parameter list.
		@see shutdown, quit
	*/
	virtual void initialise (const String& commandLineParameters) = 0;

	/* Called to allow the application to clear up before exiting.

	   After JUCEApplication::quit() has been called, the event-dispatch loop will
	   terminate, and this method will get called to allow the app to sort itself
	   out.

	   Be careful that nothing happens in this method that might rely on messages
	   being sent, or any kind of window activity, because the message loop is no
	   longer running at this point.

		@see DeletedAtShutdown
	*/
	virtual void shutdown() = 0;

	/** Indicates that the user has tried to start up another instance of the app.

		This will get called even if moreThanOneInstanceAllowed() is false.
	*/
	virtual void anotherInstanceStarted (const String& commandLine) = 0;

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
	virtual void systemRequestedQuit() = 0;

	/** If any unhandled exceptions make it through to the message dispatch loop, this
		callback will be triggered, in case you want to log them or do some other
		type of error-handling.

		If the type of exception is derived from the std::exception class, the pointer
		passed-in will be valid. If the exception is of unknown type, this pointer
		will be null.
	*/
	virtual void unhandledException (const std::exception*,
									 const String& sourceFilename,
									 int lineNumber) = 0;

	/** Returns true if this executable is running as an app (as opposed to being a plugin
		or other kind of shared library. */
	static inline bool isStandaloneApp() noexcept                   { return createInstance != nullptr; }

   #ifndef DOXYGEN
	static void appWillTerminateByForce();
	typedef JUCEApplicationBase* (*CreateInstanceFunction)();
	static CreateInstanceFunction createInstance;

protected:
	virtual int shutdownApp() = 0;
   #endif

private:

	static JUCEApplicationBase* appInstance;

	JUCE_DECLARE_NON_COPYABLE (JUCEApplicationBase);
};

#endif   // __JUCE_APPLICATIONBASE_JUCEHEADER__

/*** End of inlined file: juce_ApplicationBase.h ***/


#endif
#ifndef __JUCE_CALLBACKMESSAGE_JUCEHEADER__

/*** Start of inlined file: juce_CallbackMessage.h ***/
#ifndef __JUCE_CALLBACKMESSAGE_JUCEHEADER__
#define __JUCE_CALLBACKMESSAGE_JUCEHEADER__


/*** Start of inlined file: juce_MessageManager.h ***/
#ifndef __JUCE_MESSAGEMANAGER_JUCEHEADER__
#define __JUCE_MESSAGEMANAGER_JUCEHEADER__

class MessageManagerLock;
class ThreadPoolJob;
class ActionListener;
class ActionBroadcaster;

/** See MessageManager::callFunctionOnMessageThread() for use of this function type
*/
typedef void* (MessageCallbackFunction) (void* userData);

/**
	This class is in charge of the application's event-dispatch loop.

	@see Message, CallbackMessage, MessageManagerLock, JUCEApplication
*/
class JUCE_API  MessageManager
{
public:

	/** Returns the global instance of the MessageManager. */
	static MessageManager* getInstance();

	/** Returns the global instance of the MessageManager, or nullptr if it doesn't exist. */
	static MessageManager* getInstanceWithoutCreating() noexcept;

	/** Deletes the global MessageManager instance.
		Does nothing if no instance had been created.
	*/
	static void deleteInstance();

	/** Runs the event dispatch loop until a stop message is posted.

		This method is only intended to be run by the application's startup routine,
		as it blocks, and will only return after the stopDispatchLoop() method has been used.

		@see stopDispatchLoop
	*/
	void runDispatchLoop();

	/** Sends a signal that the dispatch loop should terminate.

		After this is called, the runDispatchLoop() or runDispatchLoopUntil() methods
		will be interrupted and will return.

		@see runDispatchLoop
	*/
	void stopDispatchLoop();

	/** Returns true if the stopDispatchLoop() method has been called.
	*/
	bool hasStopMessageBeenSent() const noexcept        { return quitMessagePosted; }

   #if JUCE_MODAL_LOOPS_PERMITTED
	/** Synchronously dispatches messages until a given time has elapsed.

		Returns false if a quit message has been posted by a call to stopDispatchLoop(),
		otherwise returns true.
	*/
	bool runDispatchLoopUntil (int millisecondsToRunFor);
   #endif

	/** Calls a function using the message-thread.

		This can be used by any thread to cause this function to be called-back
		by the message thread. If it's the message-thread that's calling this method,
		then the function will just be called; if another thread is calling, a message
		will be posted to the queue, and this method will block until that message
		is delivered, the function is called, and the result is returned.

		Be careful not to cause any deadlocks with this! It's easy to do - e.g. if the caller
		thread has a critical section locked, which an unrelated message callback then tries to lock
		before the message thread gets round to processing this callback.

		@param callback     the function to call - its signature must be @code
							void* myCallbackFunction (void*) @endcode
		@param userData     a user-defined pointer that will be passed to the function that gets called
		@returns            the value that the callback function returns.
		@see MessageManagerLock
	*/
	void* callFunctionOnMessageThread (MessageCallbackFunction* callback, void* userData);

	/** Returns true if the caller-thread is the message thread. */
	bool isThisTheMessageThread() const noexcept;

	/** Called to tell the manager that the current thread is the one that's running the dispatch loop.

		(Best to ignore this method unless you really know what you're doing..)
		@see getCurrentMessageThread
	*/
	void setCurrentThreadAsMessageThread();

	/** Returns the ID of the current message thread, as set by setCurrentThreadAsMessageThread().

		(Best to ignore this method unless you really know what you're doing..)
		@see setCurrentThreadAsMessageThread
	*/
	Thread::ThreadID getCurrentMessageThread() const noexcept            { return messageThreadId; }

	/** Returns true if the caller thread has currenltly got the message manager locked.

		see the MessageManagerLock class for more info about this.

		This will be true if the caller is the message thread, because that automatically
		gains a lock while a message is being dispatched.
	*/
	bool currentThreadHasLockedMessageManager() const noexcept;

	/** Sends a message to all other JUCE applications that are running.

		@param messageText      the string that will be passed to the actionListenerCallback()
								method of the broadcast listeners in the other app.
		@see registerBroadcastListener, ActionListener
	*/
	static void broadcastMessage (const String& messageText);

	/** Registers a listener to get told about broadcast messages.

		The actionListenerCallback() callback's string parameter
		is the message passed into broadcastMessage().

		@see broadcastMessage
	*/
	void registerBroadcastListener (ActionListener* listener);

	/** Deregisters a broadcast listener. */
	void deregisterBroadcastListener (ActionListener* listener);

	/** Internal class used as the base class for all message objects.
		You shouldn't need to use this directly - see the CallbackMessage or Message
		classes instead.
	*/
	class JUCE_API  MessageBase  : public ReferenceCountedObject
	{
	public:
		MessageBase() noexcept {}
		virtual ~MessageBase() {}

		virtual void messageCallback() = 0;
		void post();

		typedef ReferenceCountedObjectPtr<MessageBase> Ptr;

		JUCE_DECLARE_NON_COPYABLE (MessageBase);
	};

   #ifndef DOXYGEN
	// Internal methods - do not use!
	void deliverBroadcastMessage (const String&);
	~MessageManager() noexcept;
   #endif

private:

	MessageManager() noexcept;

	static MessageManager* instance;

	friend class MessageBase;
	class QuitMessage;
	friend class QuitMessage;
	friend class MessageManagerLock;

	ScopedPointer <ActionBroadcaster> broadcaster;
	bool quitMessagePosted, quitMessageReceived;
	Thread::ThreadID messageThreadId;
	Thread::ThreadID volatile threadWithLock;
	CriticalSection lockingLock;

	static bool postMessageToSystemQueue (MessageBase*);
	static void* exitModalLoopCallback (void*);
	static void doPlatformSpecificInitialisation();
	static void doPlatformSpecificShutdown();
	static bool dispatchNextMessageOnSystemQueue (bool returnIfNoPendingMessages);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MessageManager);
};

/** Used to make sure that the calling thread has exclusive access to the message loop.

	Because it's not thread-safe to call any of the Component or other UI classes
	from threads other than the message thread, one of these objects can be used to
	lock the message loop and allow this to be done. The message thread will be
	suspended for the lifetime of the MessageManagerLock object, so create one on
	the stack like this: @code
	void MyThread::run()
	{
		someData = 1234;

		const MessageManagerLock mmLock;
		// the event loop will now be locked so it's safe to make a few calls..

		myComponent->setBounds (newBounds);
		myComponent->repaint();

		// ..the event loop will now be unlocked as the MessageManagerLock goes out of scope
	}
	@endcode

	Obviously be careful not to create one of these and leave it lying around, or
	your app will grind to a halt!

	Another caveat is that using this in conjunction with other CriticalSections
	can create lots of interesting ways of producing a deadlock! In particular, if
	your message thread calls stopThread() for a thread that uses these locks,
	you'll get an (occasional) deadlock..

	@see MessageManager, MessageManager::currentThreadHasLockedMessageManager
*/
class JUCE_API MessageManagerLock
{
public:

	/** Tries to acquire a lock on the message manager.

		The constructor attempts to gain a lock on the message loop, and the lock will be
		kept for the lifetime of this object.

		Optionally, you can pass a thread object here, and while waiting to obtain the lock,
		this method will keep checking whether the thread has been given the
		Thread::signalThreadShouldExit() signal. If this happens, then it will return
		without gaining the lock. If you pass a thread, you must check whether the lock was
		successful by calling lockWasGained(). If this is false, your thread is being told to
		die, so you should take evasive action.

		If you pass nullptr for the thread object, it will wait indefinitely for the lock - be
		careful when doing this, because it's very easy to deadlock if your message thread
		attempts to call stopThread() on a thread just as that thread attempts to get the
		message lock.

		If the calling thread already has the lock, nothing will be done, so it's safe and
		quick to use these locks recursively.

		E.g.
		@code
		void run()
		{
			...

			while (! threadShouldExit())
			{
				MessageManagerLock mml (Thread::getCurrentThread());

				if (! mml.lockWasGained())
					return; // another thread is trying to kill us!

				..do some locked stuff here..
			}

			..and now the MM is now unlocked..
		}
		@endcode

	*/
	MessageManagerLock (Thread* threadToCheckForExitSignal = nullptr);

	/** This has the same behaviour as the other constructor, but takes a ThreadPoolJob
		instead of a thread.

		See the MessageManagerLock (Thread*) constructor for details on how this works.
	*/
	MessageManagerLock (ThreadPoolJob* jobToCheckForExitSignal);

	/** Releases the current thread's lock on the message manager.

		Make sure this object is created and deleted by the same thread,
		otherwise there are no guarantees what will happen!
   */
	~MessageManagerLock() noexcept;

	/** Returns true if the lock was successfully acquired.
		(See the constructor that takes a Thread for more info).
	*/
	bool lockWasGained() const noexcept                     { return locked; }

private:
	class BlockingMessage;
	friend class ReferenceCountedObjectPtr<BlockingMessage>;
	ReferenceCountedObjectPtr<BlockingMessage> blockingMessage;
	bool locked;

	bool attemptLock (Thread*, ThreadPoolJob*);

	JUCE_DECLARE_NON_COPYABLE (MessageManagerLock);
};

#endif   // __JUCE_MESSAGEMANAGER_JUCEHEADER__

/*** End of inlined file: juce_MessageManager.h ***/

/**
	A message that invokes a callback method when it gets delivered.

	You can use this class to fire off actions that you want to be performed later
	on the message thread.

	To use it, create a subclass of CallbackMessage which implements the messageCallback()
	method, then call post() to dispatch it. The event thread will then invoke your
	messageCallback() method later on, and will automatically delete the message object
	afterwards.

	Always create a new instance of a CallbackMessage on the heap, as it will be
	deleted automatically after the message has been delivered.

	@see MessageManager, MessageListener, ActionListener, ChangeListener
*/
class JUCE_API  CallbackMessage   : public MessageManager::MessageBase
{
public:

	CallbackMessage() noexcept {}

	/** Destructor. */
	~CallbackMessage() {}

	/** Called when the message is delivered.

		You should implement this method and make it do whatever action you want
		to perform.

		Note that like all other messages, this object will be deleted immediately
		after this method has been invoked.
	*/
	virtual void messageCallback() = 0;

private:
	// Avoid the leak-detector because for plugins, the host can unload our DLL with undelivered
	// messages still in the system event queue. These aren't harmful, but can cause annoying assertions.
	JUCE_DECLARE_NON_COPYABLE (CallbackMessage);
};

#endif   // __JUCE_CALLBACKMESSAGE_JUCEHEADER__

/*** End of inlined file: juce_CallbackMessage.h ***/


#endif
#ifndef __JUCE_DELETEDATSHUTDOWN_JUCEHEADER__

/*** Start of inlined file: juce_DeletedAtShutdown.h ***/
#ifndef __JUCE_DELETEDATSHUTDOWN_JUCEHEADER__
#define __JUCE_DELETEDATSHUTDOWN_JUCEHEADER__

/**
	Classes derived from this will be automatically deleted when the application exits.

	After JUCEApplication::shutdown() has been called, any objects derived from
	DeletedAtShutdown which are still in existence will be deleted in the reverse
	order to that in which they were created.

	So if you've got a singleton and don't want to have to explicitly delete it, just
	inherit from this and it'll be taken care of.
*/
class JUCE_API  DeletedAtShutdown
{
protected:
	/** Creates a DeletedAtShutdown object. */
	DeletedAtShutdown();

	/** Destructor.

		It's ok to delete these objects explicitly - it's only the ones left
		dangling at the end that will be deleted automatically.
	*/
	virtual ~DeletedAtShutdown();

public:
	/** Deletes all extant objects.

		This shouldn't be used by applications, as it's called automatically
		in the shutdown code of the JUCEApplication class.
	*/
	static void deleteAll();

private:
	static Array <DeletedAtShutdown*>& getObjects();

	JUCE_DECLARE_NON_COPYABLE (DeletedAtShutdown);
};

#endif   // __JUCE_DELETEDATSHUTDOWN_JUCEHEADER__

/*** End of inlined file: juce_DeletedAtShutdown.h ***/


#endif
#ifndef __JUCE_MESSAGE_JUCEHEADER__

/*** Start of inlined file: juce_Message.h ***/
#ifndef __JUCE_MESSAGE_JUCEHEADER__
#define __JUCE_MESSAGE_JUCEHEADER__

class MessageListener;

/** The base class for objects that can be sent to a MessageListener.

	If you want to send a message that carries some kind of custom data, just
	create a subclass of Message with some appropriate member variables to hold
	your data.

	Always create a new instance of a Message object on the heap, as it will be
	deleted automatically after the message has been delivered.

	@see MessageListener, MessageManager, ActionListener, ChangeListener
*/
class JUCE_API  Message  : public MessageManager::MessageBase
{
public:

	/** Creates an uninitialised message. */
	Message() noexcept;
	~Message();

	typedef ReferenceCountedObjectPtr<Message> Ptr;

private:
	friend class MessageListener;
	WeakReference<MessageListener> recipient;
	void messageCallback();

	// Avoid the leak-detector because for plugins, the host can unload our DLL with undelivered
	// messages still in the system event queue. These aren't harmful, but can cause annoying assertions.
	JUCE_DECLARE_NON_COPYABLE (Message);
};

#endif   // __JUCE_MESSAGE_JUCEHEADER__

/*** End of inlined file: juce_Message.h ***/


#endif
#ifndef __JUCE_MESSAGELISTENER_JUCEHEADER__

/*** Start of inlined file: juce_MessageListener.h ***/
#ifndef __JUCE_MESSAGELISTENER_JUCEHEADER__
#define __JUCE_MESSAGELISTENER_JUCEHEADER__

/**
	MessageListener subclasses can post and receive Message objects.

	@see Message, MessageManager, ActionListener, ChangeListener
*/
class JUCE_API  MessageListener
{
public:

	MessageListener() noexcept;

	/** Destructor. */
	virtual ~MessageListener();

	/** This is the callback method that receives incoming messages.

		This is called by the MessageManager from its dispatch loop.

		@see postMessage
	*/
	virtual void handleMessage (const Message& message) = 0;

	/** Sends a message to the message queue, for asynchronous delivery to this listener
		later on.

		This method can be called safely by any thread.

		@param message      the message object to send - this will be deleted
							automatically by the message queue, so make sure it's
							allocated on the heap, not the stack!
		@see handleMessage
	*/
	void postMessage (Message* message) const;

private:
	WeakReference<MessageListener>::Master masterReference;
	friend class WeakReference<MessageListener>;
};

#endif   // __JUCE_MESSAGELISTENER_JUCEHEADER__

/*** End of inlined file: juce_MessageListener.h ***/


#endif
#ifndef __JUCE_MESSAGEMANAGER_JUCEHEADER__

#endif
#ifndef __JUCE_ACTIONBROADCASTER_JUCEHEADER__

/*** Start of inlined file: juce_ActionBroadcaster.h ***/
#ifndef __JUCE_ACTIONBROADCASTER_JUCEHEADER__
#define __JUCE_ACTIONBROADCASTER_JUCEHEADER__


/*** Start of inlined file: juce_ActionListener.h ***/
#ifndef __JUCE_ACTIONLISTENER_JUCEHEADER__
#define __JUCE_ACTIONLISTENER_JUCEHEADER__

/**
	Receives callbacks to indicate that some kind of event has occurred.

	Used by various classes, e.g. buttons when they are pressed, to tell listeners
	about something that's happened.

	@see ActionBroadcaster, ChangeListener
*/
class JUCE_API  ActionListener
{
public:
	/** Destructor. */
	virtual ~ActionListener()  {}

	/** Overridden by your subclass to receive the callback.

		@param message  the string that was specified when the event was triggered
						by a call to ActionBroadcaster::sendActionMessage()
	*/
	virtual void actionListenerCallback (const String& message) = 0;
};

#endif   // __JUCE_ACTIONLISTENER_JUCEHEADER__

/*** End of inlined file: juce_ActionListener.h ***/

/** Manages a list of ActionListeners, and can send them messages.

	To quickly add methods to your class that can add/remove action
	listeners and broadcast to them, you can derive from this.

	@see ActionListener, ChangeListener
*/
class JUCE_API  ActionBroadcaster
{
public:

	/** Creates an ActionBroadcaster. */
	ActionBroadcaster();

	/** Destructor. */
	virtual ~ActionBroadcaster();

	/** Adds a listener to the list.
		Trying to add a listener that's already on the list will have no effect.
	*/
	void addActionListener (ActionListener* listener);

	/** Removes a listener from the list.
		If the listener isn't on the list, this won't have any effect.
	*/
	void removeActionListener (ActionListener* listener);

	/** Removes all listeners from the list. */
	void removeAllActionListeners();

	/** Broadcasts a message to all the registered listeners.
		@see ActionListener::actionListenerCallback
	*/
	void sendActionMessage (const String& message) const;

private:

	friend class WeakReference<ActionBroadcaster>;
	WeakReference<ActionBroadcaster>::Master masterReference;

	class ActionMessage;
	friend class ActionMessage;

	SortedSet <ActionListener*> actionListeners;
	CriticalSection actionListenerLock;

	JUCE_DECLARE_NON_COPYABLE (ActionBroadcaster);
};

#endif   // __JUCE_ACTIONBROADCASTER_JUCEHEADER__

/*** End of inlined file: juce_ActionBroadcaster.h ***/


#endif
#ifndef __JUCE_ACTIONLISTENER_JUCEHEADER__

#endif
#ifndef __JUCE_ASYNCUPDATER_JUCEHEADER__

/*** Start of inlined file: juce_AsyncUpdater.h ***/
#ifndef __JUCE_ASYNCUPDATER_JUCEHEADER__
#define __JUCE_ASYNCUPDATER_JUCEHEADER__

/**
	Has a callback method that is triggered asynchronously.

	This object allows an asynchronous callback function to be triggered, for
	tasks such as coalescing multiple updates into a single callback later on.

	Basically, one or more calls to the triggerAsyncUpdate() will result in the
	message thread calling handleAsyncUpdate() as soon as it can.
*/
class JUCE_API  AsyncUpdater
{
public:

	/** Creates an AsyncUpdater object. */
	AsyncUpdater();

	/** Destructor.

		If there are any pending callbacks when the object is deleted, these are lost.
	*/
	virtual ~AsyncUpdater();

	/** Causes the callback to be triggered at a later time.

		This method returns immediately, having made sure that a callback
		to the handleAsyncUpdate() method will occur as soon as possible.

		If an update callback is already pending but hasn't happened yet, calls
		to this method will be ignored.

		It's thread-safe to call this method from any number of threads without
		needing to worry about locking.
	*/
	void triggerAsyncUpdate();

	/** This will stop any pending updates from happening.

		If called after triggerAsyncUpdate() and before the handleAsyncUpdate()
		callback happens, this will cancel the handleAsyncUpdate() callback.

		Note that this method simply cancels the next callback - if a callback is already
		in progress on a different thread, this won't block until it finishes, so there's
		no guarantee that the callback isn't still running when you return from
	*/
	void cancelPendingUpdate() noexcept;

	/** If an update has been triggered and is pending, this will invoke it
		synchronously.

		Use this as a kind of "flush" operation - if an update is pending, the
		handleAsyncUpdate() method will be called immediately; if no update is
		pending, then nothing will be done.

		Because this may invoke the callback, this method must only be called on
		the main event thread.
	*/
	void handleUpdateNowIfNeeded();

	/** Returns true if there's an update callback in the pipeline. */
	bool isUpdatePending() const noexcept;

	/** Called back to do whatever your class needs to do.

		This method is called by the message thread at the next convenient time
		after the triggerAsyncUpdate() method has been called.
	*/
	virtual void handleAsyncUpdate() = 0;

private:

	class AsyncUpdaterMessage;
	friend class ReferenceCountedObjectPtr<AsyncUpdaterMessage>;
	ReferenceCountedObjectPtr<AsyncUpdaterMessage> message;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AsyncUpdater);
};

#endif   // __JUCE_ASYNCUPDATER_JUCEHEADER__

/*** End of inlined file: juce_AsyncUpdater.h ***/


#endif
#ifndef __JUCE_CHANGEBROADCASTER_JUCEHEADER__

/*** Start of inlined file: juce_ChangeBroadcaster.h ***/
#ifndef __JUCE_CHANGEBROADCASTER_JUCEHEADER__
#define __JUCE_CHANGEBROADCASTER_JUCEHEADER__


/*** Start of inlined file: juce_ChangeListener.h ***/
#ifndef __JUCE_CHANGELISTENER_JUCEHEADER__
#define __JUCE_CHANGELISTENER_JUCEHEADER__

class ChangeBroadcaster;

/**
	Receives change event callbacks that are sent out by a ChangeBroadcaster.

	A ChangeBroadcaster keeps a set of listeners to which it broadcasts a message when
	the ChangeBroadcaster::sendChangeMessage() method is called. A subclass of
	ChangeListener is used to receive these callbacks.

	Note that the major difference between an ActionListener and a ChangeListener
	is that for a ChangeListener, multiple changes will be coalesced into fewer
	callbacks, but ActionListeners perform one callback for every event posted.

	@see ChangeBroadcaster, ActionListener
*/
class JUCE_API  ChangeListener
{
public:
	/** Destructor. */
	virtual ~ChangeListener()  {}

	/** Your subclass should implement this method to receive the callback.
		@param source the ChangeBroadcaster that triggered the callback.
	*/
	virtual void changeListenerCallback (ChangeBroadcaster* source) = 0;

   #if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// This method's signature has changed to take a ChangeBroadcaster parameter - please update your code!
	private: virtual int changeListenerCallback (void*) { return 0; }
   #endif
};

#endif   // __JUCE_CHANGELISTENER_JUCEHEADER__

/*** End of inlined file: juce_ChangeListener.h ***/


/*** Start of inlined file: juce_ListenerList.h ***/
#ifndef __JUCE_LISTENERLIST_JUCEHEADER__
#define __JUCE_LISTENERLIST_JUCEHEADER__

/**
	Holds a set of objects and can invoke a member function callback on each object
	in the set with a single call.

	Use a ListenerList to manage a set of objects which need a callback, and you
	can invoke a member function by simply calling call() or callChecked().

	E.g.
	@code
	class MyListenerType
	{
	public:
		void myCallbackMethod (int foo, bool bar);
	};

	ListenerList <MyListenerType> listeners;
	listeners.add (someCallbackObjects...);

	// This will invoke myCallbackMethod (1234, true) on each of the objects
	// in the list...
	listeners.call (&MyListenerType::myCallbackMethod, 1234, true);
	@endcode

	If you add or remove listeners from the list during one of the callbacks - i.e. while
	it's in the middle of iterating the listeners, then it's guaranteed that no listeners
	will be mistakenly called after they've been removed, but it may mean that some of the
	listeners could be called more than once, or not at all, depending on the list's order.

	Sometimes, there's a chance that invoking one of the callbacks might result in the
	list itself being deleted while it's still iterating - to survive this situation, you can
	use callChecked() instead of call(), passing it a local object to act as a "BailOutChecker".
	The BailOutChecker must implement a method of the form "bool shouldBailOut()", and
	the list will check this after each callback to determine whether it should abort the
	operation. For an example of a bail-out checker, see the Component::BailOutChecker class,
	which can be used to check when a Component has been deleted. See also
	ListenerList::DummyBailOutChecker, which is a dummy checker that always returns false.
*/
template <class ListenerClass,
		  class ArrayType = Array <ListenerClass*> >
class ListenerList
{
	// Horrible macros required to support VC7..
	#ifndef DOXYGEN
	 #if JUCE_VC8_OR_EARLIER
	   #define LL_TEMPLATE(a)   typename P##a, typename Q##a
	   #define LL_PARAM(a)      Q##a& param##a
	 #else
	   #define LL_TEMPLATE(a)   typename P##a
	   #define LL_PARAM(a)      PARAMETER_TYPE(P##a) param##a
	 #endif
	#endif

public:

	/** Creates an empty list. */
	ListenerList()
	{
	}

	/** Destructor. */
	~ListenerList()
	{
	}

	/** Adds a listener to the list.
		A listener can only be added once, so if the listener is already in the list,
		this method has no effect.
		@see remove
	*/
	void add (ListenerClass* const listenerToAdd)
	{
		// Listeners can't be null pointers!
		jassert (listenerToAdd != nullptr);

		if (listenerToAdd != nullptr)
			listeners.addIfNotAlreadyThere (listenerToAdd);
	}

	/** Removes a listener from the list.
		If the listener wasn't in the list, this has no effect.
	*/
	void remove (ListenerClass* const listenerToRemove)
	{
		// Listeners can't be null pointers!
		jassert (listenerToRemove != nullptr);

		listeners.removeValue (listenerToRemove);
	}

	/** Returns the number of registered listeners. */
	int size() const noexcept
	{
		return listeners.size();
	}

	/** Returns true if any listeners are registered. */
	bool isEmpty() const noexcept
	{
		return listeners.size() == 0;
	}

	/** Clears the list. */
	void clear()
	{
		listeners.clear();
	}

	/** Returns true if the specified listener has been added to the list. */
	bool contains (ListenerClass* const listener) const noexcept
	{
		return listeners.contains (listener);
	}

	/** Calls a member function on each listener in the list, with no parameters. */
	void call (void (ListenerClass::*callbackFunction) ())
	{
		callChecked (static_cast <const DummyBailOutChecker&> (DummyBailOutChecker()), callbackFunction);
	}

	/** Calls a member function on each listener in the list, with no parameters and a bail-out-checker.
		See the class description for info about writing a bail-out checker. */
	template <class BailOutCheckerType>
	void callChecked (const BailOutCheckerType& bailOutChecker,
					  void (ListenerClass::*callbackFunction) ())
	{
		for (Iterator<BailOutCheckerType, ThisType> iter (*this); iter.next (bailOutChecker);)
			(iter.getListener()->*callbackFunction) ();
	}

	/** Calls a member function on each listener in the list, with 1 parameter. */
	template <LL_TEMPLATE(1)>
	void call (void (ListenerClass::*callbackFunction) (P1), LL_PARAM(1))
	{
		for (Iterator<DummyBailOutChecker, ThisType> iter (*this); iter.next();)
			(iter.getListener()->*callbackFunction) (param1);
	}

	/** Calls a member function on each listener in the list, with one parameter and a bail-out-checker.
		See the class description for info about writing a bail-out checker. */
	template <class BailOutCheckerType, LL_TEMPLATE(1)>
	void callChecked (const BailOutCheckerType& bailOutChecker,
					  void (ListenerClass::*callbackFunction) (P1),
					  LL_PARAM(1))
	{
		for (Iterator<BailOutCheckerType, ThisType> iter (*this); iter.next (bailOutChecker);)
			(iter.getListener()->*callbackFunction) (param1);
	}

	/** Calls a member function on each listener in the list, with 2 parameters. */
	template <LL_TEMPLATE(1), LL_TEMPLATE(2)>
	void call (void (ListenerClass::*callbackFunction) (P1, P2),
			   LL_PARAM(1), LL_PARAM(2))
	{
		for (Iterator<DummyBailOutChecker, ThisType> iter (*this); iter.next();)
			(iter.getListener()->*callbackFunction) (param1, param2);
	}

	/** Calls a member function on each listener in the list, with 2 parameters and a bail-out-checker.
		See the class description for info about writing a bail-out checker. */
	template <class BailOutCheckerType, LL_TEMPLATE(1), LL_TEMPLATE(2)>
	void callChecked (const BailOutCheckerType& bailOutChecker,
					  void (ListenerClass::*callbackFunction) (P1, P2),
					  LL_PARAM(1), LL_PARAM(2))
	{
		for (Iterator<BailOutCheckerType, ThisType> iter (*this); iter.next (bailOutChecker);)
			(iter.getListener()->*callbackFunction) (param1, param2);
	}

	/** Calls a member function on each listener in the list, with 3 parameters. */
	template <LL_TEMPLATE(1), LL_TEMPLATE(2), LL_TEMPLATE(3)>
	void call (void (ListenerClass::*callbackFunction) (P1, P2, P3),
			   LL_PARAM(1), LL_PARAM(2), LL_PARAM(3))
	{
		for (Iterator<DummyBailOutChecker, ThisType> iter (*this); iter.next();)
			(iter.getListener()->*callbackFunction) (param1, param2, param3);
	}

	/** Calls a member function on each listener in the list, with 3 parameters and a bail-out-checker.
		See the class description for info about writing a bail-out checker. */
	template <class BailOutCheckerType, LL_TEMPLATE(1), LL_TEMPLATE(2), LL_TEMPLATE(3)>
	void callChecked (const BailOutCheckerType& bailOutChecker,
					  void (ListenerClass::*callbackFunction) (P1, P2, P3),
					  LL_PARAM(1), LL_PARAM(2), LL_PARAM(3))
	{
		for (Iterator<BailOutCheckerType, ThisType> iter (*this); iter.next (bailOutChecker);)
			(iter.getListener()->*callbackFunction) (param1, param2, param3);
	}

	/** Calls a member function on each listener in the list, with 4 parameters. */
	template <LL_TEMPLATE(1), LL_TEMPLATE(2), LL_TEMPLATE(3), LL_TEMPLATE(4)>
	void call (void (ListenerClass::*callbackFunction) (P1, P2, P3, P4),
			   LL_PARAM(1), LL_PARAM(2), LL_PARAM(3), LL_PARAM(4))
	{
		for (Iterator<DummyBailOutChecker, ThisType> iter (*this); iter.next();)
			(iter.getListener()->*callbackFunction) (param1, param2, param3, param4);
	}

	/** Calls a member function on each listener in the list, with 4 parameters and a bail-out-checker.
		See the class description for info about writing a bail-out checker. */
	template <class BailOutCheckerType, LL_TEMPLATE(1), LL_TEMPLATE(2), LL_TEMPLATE(3), LL_TEMPLATE(4)>
	void callChecked (const BailOutCheckerType& bailOutChecker,
					  void (ListenerClass::*callbackFunction) (P1, P2, P3, P4),
					  LL_PARAM(1), LL_PARAM(2), LL_PARAM(3), LL_PARAM(4))
	{
		for (Iterator<BailOutCheckerType, ThisType> iter (*this); iter.next (bailOutChecker);)
			(iter.getListener()->*callbackFunction) (param1, param2, param3, param4);
	}

	/** Calls a member function on each listener in the list, with 5 parameters. */
	template <LL_TEMPLATE(1), LL_TEMPLATE(2), LL_TEMPLATE(3), LL_TEMPLATE(4), LL_TEMPLATE(5)>
	void call (void (ListenerClass::*callbackFunction) (P1, P2, P3, P4, P5),
			   LL_PARAM(1), LL_PARAM(2), LL_PARAM(3), LL_PARAM(4), LL_PARAM(5))
	{
		for (Iterator<DummyBailOutChecker, ThisType> iter (*this); iter.next();)
			(iter.getListener()->*callbackFunction) (param1, param2, param3, param4, param5);
	}

	/** Calls a member function on each listener in the list, with 5 parameters and a bail-out-checker.
		See the class description for info about writing a bail-out checker. */
	template <class BailOutCheckerType, LL_TEMPLATE(1), LL_TEMPLATE(2), LL_TEMPLATE(3), LL_TEMPLATE(4), LL_TEMPLATE(5)>
	void callChecked (const BailOutCheckerType& bailOutChecker,
					  void (ListenerClass::*callbackFunction) (P1, P2, P3, P4, P5),
					  LL_PARAM(1), LL_PARAM(2), LL_PARAM(3), LL_PARAM(4), LL_PARAM(5))
	{
		for (Iterator<BailOutCheckerType, ThisType> iter (*this); iter.next (bailOutChecker);)
			(iter.getListener()->*callbackFunction) (param1, param2, param3, param4, param5);
	}

	/** A dummy bail-out checker that always returns false.
		See the ListenerList notes for more info about bail-out checkers.
	*/
	class DummyBailOutChecker
	{
	public:
		inline bool shouldBailOut() const noexcept     { return false; }
	};

	/** Iterates the listeners in a ListenerList. */
	template <class BailOutCheckerType, class ListType>
	class Iterator
	{
	public:

		Iterator (const ListType& list_) noexcept
			: list (list_), index (list_.size())
		{}

		~Iterator() noexcept {}

		bool next() noexcept
		{
			if (index <= 0)
				return false;

			const int listSize = list.size();

			if (--index < listSize)
				return true;

			index = listSize - 1;
			return index >= 0;
		}

		bool next (const BailOutCheckerType& bailOutChecker) noexcept
		{
			return (! bailOutChecker.shouldBailOut()) && next();
		}

		typename ListType::ListenerType* getListener() const noexcept
		{
			return list.getListeners().getUnchecked (index);
		}

	private:
		const ListType& list;
		int index;

		JUCE_DECLARE_NON_COPYABLE (Iterator);
	};

	typedef ListenerList<ListenerClass, ArrayType> ThisType;
	typedef ListenerClass ListenerType;

	const ArrayType& getListeners() const noexcept          { return listeners; }

private:

	ArrayType listeners;

	JUCE_DECLARE_NON_COPYABLE (ListenerList);

	#undef LL_TEMPLATE
	#undef LL_PARAM
};

#endif   // __JUCE_LISTENERLIST_JUCEHEADER__

/*** End of inlined file: juce_ListenerList.h ***/

/**
	Holds a list of ChangeListeners, and sends messages to them when instructed.

	@see ChangeListener
*/
class JUCE_API  ChangeBroadcaster
{
public:

	/** Creates an ChangeBroadcaster. */
	ChangeBroadcaster() noexcept;

	/** Destructor. */
	virtual ~ChangeBroadcaster();

	/** Registers a listener to receive change callbacks from this broadcaster.
		Trying to add a listener that's already on the list will have no effect.
	*/
	void addChangeListener (ChangeListener* listener);

	/** Unregisters a listener from the list.
		If the listener isn't on the list, this won't have any effect.
	*/
	void removeChangeListener (ChangeListener* listener);

	/** Removes all listeners from the list. */
	void removeAllChangeListeners();

	/** Causes an asynchronous change message to be sent to all the registered listeners.

		The message will be delivered asynchronously by the main message thread, so this
		method will return immediately. To call the listeners synchronously use
		sendSynchronousChangeMessage().
	*/
	void sendChangeMessage();

	/** Sends a synchronous change message to all the registered listeners.

		This will immediately call all the listeners that are registered. For thread-safety
		reasons, you must only call this method on the main message thread.

		@see dispatchPendingMessages
	*/
	void sendSynchronousChangeMessage();

	/** If a change message has been sent but not yet dispatched, this will call
		sendSynchronousChangeMessage() to make the callback immediately.

		For thread-safety reasons, you must only call this method on the main message thread.
	*/
	void dispatchPendingMessages();

private:

	class ChangeBroadcasterCallback  : public AsyncUpdater
	{
	public:
		ChangeBroadcasterCallback();
		void handleAsyncUpdate();

		ChangeBroadcaster* owner;
	};

	friend class ChangeBroadcasterCallback;
	ChangeBroadcasterCallback callback;
	ListenerList <ChangeListener> changeListeners;

	void callListeners();

	JUCE_DECLARE_NON_COPYABLE (ChangeBroadcaster);
};

#endif   // __JUCE_CHANGEBROADCASTER_JUCEHEADER__

/*** End of inlined file: juce_ChangeBroadcaster.h ***/


#endif
#ifndef __JUCE_CHANGELISTENER_JUCEHEADER__

#endif
#ifndef __JUCE_LISTENERLIST_JUCEHEADER__

#endif
#ifndef __JUCE_MULTITIMER_JUCEHEADER__

/*** Start of inlined file: juce_MultiTimer.h ***/
#ifndef __JUCE_MULTITIMER_JUCEHEADER__
#define __JUCE_MULTITIMER_JUCEHEADER__


/*** Start of inlined file: juce_Timer.h ***/
#ifndef __JUCE_TIMER_JUCEHEADER__
#define __JUCE_TIMER_JUCEHEADER__

/**
	Makes repeated callbacks to a virtual method at a specified time interval.

	A Timer's timerCallback() method will be repeatedly called at a given
	interval. When you create a Timer object, it will do nothing until the
	startTimer() method is called, which will cause the message thread to
	start making callbacks at the specified interval, until stopTimer() is called
	or the object is deleted.

	The time interval isn't guaranteed to be precise to any more than maybe
	10-20ms, and the intervals may end up being much longer than requested if the
	system is busy. Because the callbacks are made by the main message thread,
	anything that blocks the message queue for a period of time will also prevent
	any timers from running until it can carry on.

	If you need to have a single callback that is shared by multiple timers with
	different frequencies, then the MultiTimer class allows you to do that - its
	structure is very similar to the Timer class, but contains multiple timers
	internally, each one identified by an ID number.

	@see MultiTimer
*/
class JUCE_API  Timer
{
protected:

	/** Creates a Timer.

		When created, the timer is stopped, so use startTimer() to get it going.
	*/
	Timer() noexcept;

	/** Creates a copy of another timer.

		Note that this timer won't be started, even if the one you're copying
		is running.
	*/
	Timer (const Timer& other) noexcept;

public:

	/** Destructor. */
	virtual ~Timer();

	/** The user-defined callback routine that actually gets called periodically.

		It's perfectly ok to call startTimer() or stopTimer() from within this
		callback to change the subsequent intervals.
	*/
	virtual void timerCallback() = 0;

	/** Starts the timer and sets the length of interval required.

		If the timer is already started, this will reset it, so the
		time between calling this method and the next timer callback
		will not be less than the interval length passed in.

		@param  intervalInMilliseconds  the interval to use (any values less than 1 will be
										rounded up to 1)
	*/
	void startTimer (int intervalInMilliseconds) noexcept;

	/** Stops the timer.

		No more callbacks will be made after this method returns.

		If this is called from a different thread, any callbacks that may
		be currently executing may be allowed to finish before the method
		returns.
	*/
	void stopTimer() noexcept;

	/** Checks if the timer has been started.

		@returns true if the timer is running.
	*/
	bool isTimerRunning() const noexcept                    { return periodMs > 0; }

	/** Returns the timer's interval.

		@returns the timer's interval in milliseconds if it's running, or 0 if it's not.
	*/
	int getTimerInterval() const noexcept                   { return periodMs; }

	/** For internal use only: invokes any timers that need callbacks.
		Don't call this unless you really know what you're doing!
	*/
	static void JUCE_CALLTYPE callPendingTimersSynchronously();

private:
	class TimerThread;
	friend class TimerThread;
	int countdownMs, periodMs;
	Timer* previous;
	Timer* next;

	Timer& operator= (const Timer&);
};

#endif   // __JUCE_TIMER_JUCEHEADER__

/*** End of inlined file: juce_Timer.h ***/

/**
	A type of timer class that can run multiple timers with different frequencies,
	all of which share a single callback.

	This class is very similar to the Timer class, but allows you run multiple
	separate timers, where each one has a unique ID number. The methods in this
	class are exactly equivalent to those in Timer, but with the addition of
	this ID number.

	To use it, you need to create a subclass of MultiTimer, implementing the
	timerCallback() method. Then you can start timers with startTimer(), and
	each time the callback is triggered, it passes in the ID of the timer that
	caused it.

	@see Timer
*/
class JUCE_API  MultiTimer
{
protected:

	/** Creates a MultiTimer.

		When created, no timers are running, so use startTimer() to start things off.
	*/
	MultiTimer() noexcept;

	/** Creates a copy of another timer.

		Note that this timer will not contain any running timers, even if the one you're
		copying from was running.
	*/
	MultiTimer (const MultiTimer& other) noexcept;

public:

	/** Destructor. */
	virtual ~MultiTimer();

	/** The user-defined callback routine that actually gets called by each of the
		timers that are running.

		It's perfectly ok to call startTimer() or stopTimer() from within this
		callback to change the subsequent intervals.
	*/
	virtual void timerCallback (int timerId) = 0;

	/** Starts a timer and sets the length of interval required.

		If the timer is already started, this will reset it, so the
		time between calling this method and the next timer callback
		will not be less than the interval length passed in.

		@param timerId                  a unique Id number that identifies the timer to
										start. This is the id that will be passed back
										to the timerCallback() method when this timer is
										triggered
		@param  intervalInMilliseconds  the interval to use (any values less than 1 will be
										rounded up to 1)
	*/
	void startTimer (int timerId, int intervalInMilliseconds) noexcept;

	/** Stops a timer.

		If a timer has been started with the given ID number, it will be cancelled.
		No more callbacks will be made for the specified timer after this method returns.

		If this is called from a different thread, any callbacks that may
		be currently executing may be allowed to finish before the method
		returns.
	*/
	void stopTimer (int timerId) noexcept;

	/** Checks whether a timer has been started for a specified ID.

		@returns true if a timer with the given ID is running.
	*/
	bool isTimerRunning (int timerId) const noexcept;

	/** Returns the interval for a specified timer ID.

		@returns    the timer's interval in milliseconds if it's running, or 0 if it's no timer
					is running for the ID number specified.
	*/
	int getTimerInterval (int timerId) const noexcept;

private:
	class MultiTimerCallback;
	SpinLock timerListLock;
	OwnedArray <MultiTimerCallback> timers;

	MultiTimer& operator= (const MultiTimer&);
};

#endif   // __JUCE_MULTITIMER_JUCEHEADER__

/*** End of inlined file: juce_MultiTimer.h ***/


#endif
#ifndef __JUCE_TIMER_JUCEHEADER__

#endif
#ifndef __JUCE_INTERPROCESSCONNECTION_JUCEHEADER__

/*** Start of inlined file: juce_InterprocessConnection.h ***/
#ifndef __JUCE_INTERPROCESSCONNECTION_JUCEHEADER__
#define __JUCE_INTERPROCESSCONNECTION_JUCEHEADER__

class InterprocessConnectionServer;
class MemoryBlock;

/**
	Manages a simple two-way messaging connection to another process, using either
	a socket or a named pipe as the transport medium.

	To connect to a waiting socket or an open pipe, use the connectToSocket() or
	connectToPipe() methods. If this succeeds, messages can be sent to the other end,
	and incoming messages will result in a callback via the messageReceived()
	method.

	To open a pipe and wait for another client to connect to it, use the createPipe()
	method.

	To act as a socket server and create connections for one or more client, see the
	InterprocessConnectionServer class.

	@see InterprocessConnectionServer, Socket, NamedPipe
*/
class JUCE_API  InterprocessConnection    : public Thread
{
public:

	/** Creates a connection.

		Connections are created manually, connecting them with the connectToSocket()
		or connectToPipe() methods, or they are created automatically by a InterprocessConnectionServer
		when a client wants to connect.

		@param callbacksOnMessageThread     if true, callbacks to the connectionMade(),
											connectionLost() and messageReceived() methods will
											always be made using the message thread; if false,
											these will be called immediately on the connection's
											own thread.
		@param magicMessageHeaderNumber     a magic number to use in the header to check the
											validity of the data blocks being sent and received. This
											can be any number, but the sender and receiver must obviously
											use matching values or they won't recognise each other.
	*/
	InterprocessConnection (bool callbacksOnMessageThread = true,
							uint32 magicMessageHeaderNumber = 0xf2b49e2c);

	/** Destructor. */
	~InterprocessConnection();

	/** Tries to connect this object to a socket.

		For this to work, the machine on the other end needs to have a InterprocessConnectionServer
		object waiting to receive client connections on this port number.

		@param hostName             the host computer, either a network address or name
		@param portNumber           the socket port number to try to connect to
		@param timeOutMillisecs     how long to keep trying before giving up
		@returns true if the connection is established successfully
		@see Socket
	*/
	bool connectToSocket (const String& hostName,
						  int portNumber,
						  int timeOutMillisecs);

	/** Tries to connect the object to an existing named pipe.

		For this to work, another process on the same computer must already have opened
		an InterprocessConnection object and used createPipe() to create a pipe for this
		to connect to.

		You can optionally specify a timeout length to be passed to the NamedPipe::read() method.

		@returns true if it connects successfully.
		@see createPipe, NamedPipe
	*/
	bool connectToPipe (const String& pipeName,
						int pipeReceiveMessageTimeoutMs = -1);

	/** Tries to create a new pipe for other processes to connect to.

		This creates a pipe with the given name, so that other processes can use
		connectToPipe() to connect to the other end.

		You can optionally specify a timeout length to be passed to the NamedPipe::read() method.

		If another process is already using this pipe, this will fail and return false.
	*/
	bool createPipe (const String& pipeName,
					 int pipeReceiveMessageTimeoutMs = -1);

	/** Disconnects and closes any currently-open sockets or pipes. */
	void disconnect();

	/** True if a socket or pipe is currently active. */
	bool isConnected() const;

	/** Returns the socket that this connection is using (or null if it uses a pipe). */
	StreamingSocket* getSocket() const noexcept                 { return socket; }

	/** Returns the pipe that this connection is using (or null if it uses a socket). */
	NamedPipe* getPipe() const noexcept                         { return pipe; }

	/** Returns the name of the machine at the other end of this connection.

		This will return an empty string if the other machine isn't known for
		some reason.
	*/
	String getConnectedHostName() const;

	/** Tries to send a message to the other end of this connection.

		This will fail if it's not connected, or if there's some kind of write error. If
		it succeeds, the connection object at the other end will receive the message by
		a callback to its messageReceived() method.

		@see messageReceived
	*/
	bool sendMessage (const MemoryBlock& message);

	/** Called when the connection is first connected.

		If the connection was created with the callbacksOnMessageThread flag set, then
		this will be called on the message thread; otherwise it will be called on a server
		thread.
	*/
	virtual void connectionMade() = 0;

	/** Called when the connection is broken.

		If the connection was created with the callbacksOnMessageThread flag set, then
		this will be called on the message thread; otherwise it will be called on a server
		thread.
	*/
	virtual void connectionLost() = 0;

	/** Called when a message arrives.

		When the object at the other end of this connection sends us a message with sendMessage(),
		this callback is used to deliver it to us.

		If the connection was created with the callbacksOnMessageThread flag set, then
		this will be called on the message thread; otherwise it will be called on a server
		thread.

		@see sendMessage
	*/
	virtual void messageReceived (const MemoryBlock& message) = 0;

private:

	WeakReference<InterprocessConnection>::Master masterReference;
	friend class WeakReference<InterprocessConnection>;
	CriticalSection pipeAndSocketLock;
	ScopedPointer <StreamingSocket> socket;
	ScopedPointer <NamedPipe> pipe;
	bool callbackConnectionState;
	const bool useMessageThread;
	const uint32 magicMessageHeader;
	int pipeReceiveMessageTimeout;

	friend class InterprocessConnectionServer;
	void initialiseWithSocket (StreamingSocket*);
	void initialiseWithPipe (NamedPipe*);
	void connectionMadeInt();
	void connectionLostInt();
	void deliverDataInt (const MemoryBlock&);
	bool readNextMessageInt();
	void run();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterprocessConnection);
};

#endif   // __JUCE_INTERPROCESSCONNECTION_JUCEHEADER__

/*** End of inlined file: juce_InterprocessConnection.h ***/


#endif
#ifndef __JUCE_INTERPROCESSCONNECTIONSERVER_JUCEHEADER__

/*** Start of inlined file: juce_InterprocessConnectionServer.h ***/
#ifndef __JUCE_INTERPROCESSCONNECTIONSERVER_JUCEHEADER__
#define __JUCE_INTERPROCESSCONNECTIONSERVER_JUCEHEADER__

/**
	An object that waits for client sockets to connect to a port on this host, and
	creates InterprocessConnection objects for each one.

	To use this, create a class derived from it which implements the createConnectionObject()
	method, so that it creates suitable connection objects for each client that tries
	to connect.

	@see InterprocessConnection
*/
class JUCE_API  InterprocessConnectionServer    : private Thread
{
public:

	/** Creates an uninitialised server object.
	*/
	InterprocessConnectionServer();

	/** Destructor. */
	~InterprocessConnectionServer();

	/** Starts an internal thread which listens on the given port number.

		While this is running, in another process tries to connect with the
		InterprocessConnection::connectToSocket() method, this object will call
		createConnectionObject() to create a connection to that client.

		Use stop() to stop the thread running.

		@see createConnectionObject, stop
	*/
	bool beginWaitingForSocket (int portNumber);

	/** Terminates the listener thread, if it's active.

		@see beginWaitingForSocket
	*/
	void stop();

protected:
	/** Creates a suitable connection object for a client process that wants to
		connect to this one.

		This will be called by the listener thread when a client process tries
		to connect, and must return a new InterprocessConnection object that will
		then run as this end of the connection.

		@see InterprocessConnection
	*/
	virtual InterprocessConnection* createConnectionObject() = 0;

private:

	ScopedPointer <StreamingSocket> socket;

	void run();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterprocessConnectionServer);
};

#endif   // __JUCE_INTERPROCESSCONNECTIONSERVER_JUCEHEADER__

/*** End of inlined file: juce_InterprocessConnectionServer.h ***/


#endif
#ifndef __JUCE_SCOPEDXLOCK_JUCEHEADER__

/*** Start of inlined file: juce_ScopedXLock.h ***/
#ifndef __JUCE_SCOPEDXLOCK_JUCEHEADER__
#define __JUCE_SCOPEDXLOCK_JUCEHEADER__

#if JUCE_LINUX || DOXYGEN

/** A handy class that uses XLockDisplay and XUnlockDisplay to lock the X server
	using RAII (Only available in Linux!).
*/
class ScopedXLock
{
public:
	/** Creating a ScopedXLock object locks the X display.
		This uses XLockDisplay() to grab the display that Juce is using.
	*/
	ScopedXLock();

	/** Deleting a ScopedXLock object unlocks the X display.
		This calls XUnlockDisplay() to release the lock.
	*/
	~ScopedXLock();
};

#endif
#endif   // __JUCE_SCOPEDXLOCK_JUCEHEADER__

/*** End of inlined file: juce_ScopedXLock.h ***/


#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_EVENTS_JUCEHEADER__

/*** End of inlined file: juce_events.h ***/

