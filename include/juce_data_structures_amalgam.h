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


/*** Start of inlined file: juce_data_structures.h ***/
#ifndef __JUCE_DATA_STRUCTURES_JUCEHEADER__
#define __JUCE_DATA_STRUCTURES_JUCEHEADER__

#include "juce_events_amalgam.h"

namespace juce
{

// START_AUTOINCLUDE values, undomanager, app_properties
#ifndef __JUCE_VALUE_JUCEHEADER__

/*** Start of inlined file: juce_Value.h ***/
#ifndef __JUCE_VALUE_JUCEHEADER__
#define __JUCE_VALUE_JUCEHEADER__

/**
	Represents a shared variant value.

	A Value object contains a reference to a var object, and can get and set its value.
	Listeners can be attached to be told when the value is changed.

	The Value class is a wrapper around a shared, reference-counted underlying data
	object - this means that multiple Value objects can all refer to the same piece of
	data, allowing all of them to be notified when any of them changes it.

	When you create a Value with its default constructor, it acts as a wrapper around a
	simple var object, but by creating a Value that refers to a custom subclass of ValueSource,
	you can map the Value onto any kind of underlying data.
*/
class JUCE_API  Value
{
public:

	/** Creates an empty Value, containing a void var. */
	Value();

	/** Creates a Value that refers to the same value as another one.

		Note that this doesn't make a copy of the other value - both this and the other
		Value will share the same underlying value, so that when either one alters it, both
		will see it change.
	*/
	Value (const Value& other);

	/** Creates a Value that is set to the specified value. */
	explicit Value (const var& initialValue);

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	Value (Value&&) noexcept;
	Value& operator= (Value&&) noexcept;
   #endif

	/** Destructor. */
	~Value();

	/** Returns the current value. */
	var getValue() const;

	/** Returns the current value. */
	operator var() const;

	/** Returns the value as a string.

		This is alternative to writing things like "myValue.getValue().toString()".
	*/
	String toString() const;

	/** Sets the current value.

		You can also use operator= to set the value.

		If there are any listeners registered, they will be notified of the
		change asynchronously.
	*/
	void setValue (const var& newValue);

	/** Sets the current value.

		This is the same as calling setValue().

		If there are any listeners registered, they will be notified of the
		change asynchronously.
	*/
	Value& operator= (const var& newValue);

	/** Makes this object refer to the same underlying ValueSource as another one.

		Once this object has been connected to another one, changing either one
		will update the other.

		Existing listeners will still be registered after you call this method, and
		they'll continue to receive messages when the new value changes.
	*/
	void referTo (const Value& valueToReferTo);

	/** Returns true if this value and the other one are references to the same value.
	*/
	bool refersToSameSourceAs (const Value& other) const;

	/** Compares two values.
		This is a compare-by-value comparison, so is effectively the same as
		saying (this->getValue() == other.getValue()).
	*/
	bool operator== (const Value& other) const;

	/** Compares two values.
		This is a compare-by-value comparison, so is effectively the same as
		saying (this->getValue() != other.getValue()).
	*/
	bool operator!= (const Value& other) const;

	/** Receives callbacks when a Value object changes.
		@see Value::addListener
	*/
	class JUCE_API  Listener
	{
	public:
		Listener()          {}
		virtual ~Listener() {}

		/** Called when a Value object is changed.

			Note that the Value object passed as a parameter may not be exactly the same
			object that you registered the listener with - it might be a copy that refers
			to the same underlying ValueSource. To find out, you can call Value::refersToSameSourceAs().
		*/
		virtual void valueChanged (Value& value) = 0;
	};

	/** Adds a listener to receive callbacks when the value changes.

		The listener is added to this specific Value object, and not to the shared
		object that it refers to. When this object is deleted, all the listeners will
		be lost, even if other references to the same Value still exist. So when you're
		adding a listener, make sure that you add it to a ValueTree instance that will last
		for as long as you need the listener. In general, you'd never want to add a listener
		to a local stack-based ValueTree, but more likely to one that's a member variable.

		@see removeListener
	*/
	void addListener (Listener* listener);

	/** Removes a listener that was previously added with addListener(). */
	void removeListener (Listener* listener);

	/**
		Used internally by the Value class as the base class for its shared value objects.

		The Value class is essentially a reference-counted pointer to a shared instance
		of a ValueSource object. If you're feeling adventurous, you can create your own custom
		ValueSource classes to allow Value objects to represent your own custom data items.
	*/
	class JUCE_API  ValueSource   : public SingleThreadedReferenceCountedObject,
									public AsyncUpdater
	{
	public:
		ValueSource();
		virtual ~ValueSource();

		/** Returns the current value of this object. */
		virtual var getValue() const = 0;

		/** Changes the current value.
			This must also trigger a change message if the value actually changes.
		*/
		virtual void setValue (const var& newValue) = 0;

		/** Delivers a change message to all the listeners that are registered with
			this value.

			If dispatchSynchronously is true, the method will call all the listeners
			before returning; otherwise it'll dispatch a message and make the call later.
		*/
		void sendChangeMessage (bool dispatchSynchronously);

	protected:

		friend class Value;
		SortedSet <Value*> valuesWithListeners;

		void handleAsyncUpdate();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueSource);
	};

	/** Creates a Value object that uses this valueSource object as its underlying data. */
	explicit Value (ValueSource* valueSource);

	/** Returns the ValueSource that this value is referring to. */
	ValueSource& getValueSource() noexcept          { return *value; }

private:

	friend class ValueSource;
	ReferenceCountedObjectPtr <ValueSource> value;
	ListenerList <Listener> listeners;

	void callListeners();

	// This is disallowed to avoid confusion about whether it should
	// do a by-value or by-reference copy.
	Value& operator= (const Value&);
};

/** Writes a Value to an OutputStream as a UTF8 string. */
OutputStream& JUCE_CALLTYPE operator<< (OutputStream&, const Value&);

/** This typedef is just for compatibility with old code - newer code should use the Value::Listener class directly. */
typedef Value::Listener ValueListener;

#endif   // __JUCE_VALUE_JUCEHEADER__

/*** End of inlined file: juce_Value.h ***/


#endif
#ifndef __JUCE_VALUETREE_JUCEHEADER__

/*** Start of inlined file: juce_ValueTree.h ***/
#ifndef __JUCE_VALUETREE_JUCEHEADER__
#define __JUCE_VALUETREE_JUCEHEADER__


/*** Start of inlined file: juce_UndoManager.h ***/
#ifndef __JUCE_UNDOMANAGER_JUCEHEADER__
#define __JUCE_UNDOMANAGER_JUCEHEADER__


/*** Start of inlined file: juce_UndoableAction.h ***/
#ifndef __JUCE_UNDOABLEACTION_JUCEHEADER__
#define __JUCE_UNDOABLEACTION_JUCEHEADER__

/**
	Used by the UndoManager class to store an action which can be done
	and undone.

	@see UndoManager
*/
class JUCE_API  UndoableAction
{
protected:
	/** Creates an action. */
	UndoableAction() noexcept   {}

public:
	/** Destructor. */
	virtual ~UndoableAction()   {}

	/** Overridden by a subclass to perform the action.

		This method is called by the UndoManager, and shouldn't be used directly by
		applications.

		Be careful not to make any calls in a perform() method that could call
		recursively back into the UndoManager::perform() method

		@returns    true if the action could be performed.
		@see UndoManager::perform
	*/
	virtual bool perform() = 0;

	/** Overridden by a subclass to undo the action.

		This method is called by the UndoManager, and shouldn't be used directly by
		applications.

		Be careful not to make any calls in an undo() method that could call
		recursively back into the UndoManager::perform() method

		@returns    true if the action could be undone without any errors.
		@see UndoManager::perform
	*/
	virtual bool undo() = 0;

	/** Returns a value to indicate how much memory this object takes up.

		Because the UndoManager keeps a list of UndoableActions, this is used
		to work out how much space each one will take up, so that the UndoManager
		can work out how many to keep.

		The default value returned here is 10 - units are arbitrary and
		don't have to be accurate.

		@see UndoManager::getNumberOfUnitsTakenUpByStoredCommands,
			 UndoManager::setMaxNumberOfStoredUnits
	*/
	virtual int getSizeInUnits()    { return 10; }

	/** Allows multiple actions to be coalesced into a single action object, to reduce storage space.

		If possible, this method should create and return a single action that does the same job as
		this one followed by the supplied action.

		If it's not possible to merge the two actions, the method should return zero.
	*/
	virtual UndoableAction* createCoalescedAction (UndoableAction* nextAction)  { (void) nextAction; return nullptr; }
};

#endif   // __JUCE_UNDOABLEACTION_JUCEHEADER__

/*** End of inlined file: juce_UndoableAction.h ***/

/**
	Manages a list of undo/redo commands.

	An UndoManager object keeps a list of past actions and can use these actions
	to move backwards and forwards through an undo history.

	To use it, create subclasses of UndoableAction which perform all the
	actions you need, then when you need to actually perform an action, create one
	and pass it to the UndoManager's perform() method.

	The manager also uses the concept of 'transactions' to group the actions
	together - all actions performed between calls to beginNewTransaction() are
	grouped together and are all undone/redone as a group.

	The UndoManager is a ChangeBroadcaster, so listeners can register to be told
	when actions are performed or undone.

	@see UndoableAction
*/
class JUCE_API  UndoManager  : public ChangeBroadcaster
{
public:

	/** Creates an UndoManager.

		@param maxNumberOfUnitsToKeep       each UndoableAction object returns a value
											to indicate how much storage it takes up
											(UndoableAction::getSizeInUnits()), so this
											lets you specify the maximum total number of
											units that the undomanager is allowed to
											keep in memory before letting the older actions
											drop off the end of the list.
		@param minimumTransactionsToKeep    this specifies the minimum number of transactions
											that will be kept, even if this involves exceeding
											the amount of space specified in maxNumberOfUnitsToKeep
	*/
	UndoManager (int maxNumberOfUnitsToKeep = 30000,
				 int minimumTransactionsToKeep = 30);

	/** Destructor. */
	~UndoManager();

	/** Deletes all stored actions in the list. */
	void clearUndoHistory();

	/** Returns the current amount of space to use for storing UndoableAction objects.

		@see setMaxNumberOfStoredUnits
	*/
	int getNumberOfUnitsTakenUpByStoredCommands() const;

	/** Sets the amount of space that can be used for storing UndoableAction objects.

		@param maxNumberOfUnitsToKeep       each UndoableAction object returns a value
											to indicate how much storage it takes up
											(UndoableAction::getSizeInUnits()), so this
											lets you specify the maximum total number of
											units that the undomanager is allowed to
											keep in memory before letting the older actions
											drop off the end of the list.
		@param minimumTransactionsToKeep    this specifies the minimum number of transactions
											that will be kept, even if this involves exceeding
											the amount of space specified in maxNumberOfUnitsToKeep
		@see getNumberOfUnitsTakenUpByStoredCommands
	*/
	void setMaxNumberOfStoredUnits (int maxNumberOfUnitsToKeep,
									int minimumTransactionsToKeep);

	/** Performs an action and adds it to the undo history list.

		@param action   the action to perform - this will be deleted by the UndoManager
						when no longer needed
		@param actionName   if this string is non-empty, the current transaction will be
							given this name; if it's empty, the current transaction name will
							be left unchanged. See setCurrentTransactionName()
		@returns true if the command succeeds - see UndoableAction::perform
		@see beginNewTransaction
	*/
	bool perform (UndoableAction* action,
				  const String& actionName = String::empty);

	/** Starts a new group of actions that together will be treated as a single transaction.

		All actions that are passed to the perform() method between calls to this
		method are grouped together and undone/redone together by a single call to
		undo() or redo().

		@param actionName   a description of the transaction that is about to be
							performed
	*/
	void beginNewTransaction (const String& actionName = String::empty);

	/** Changes the name stored for the current transaction.

		Each transaction is given a name when the beginNewTransaction() method is
		called, but this can be used to change that name without starting a new
		transaction.
	*/
	void setCurrentTransactionName (const String& newName);

	/** Returns true if there's at least one action in the list to undo.

		@see getUndoDescription, undo, canRedo
	*/
	bool canUndo() const;

	/** Returns the description of the transaction that would be next to get undone.

		The description returned is the one that was passed into beginNewTransaction
		before the set of actions was performed.

		@see undo
	*/
	String getUndoDescription() const;

	/** Tries to roll-back the last transaction.

		@returns    true if the transaction can be undone, and false if it fails, or
					if there aren't any transactions to undo
	*/
	bool undo();

	/** Tries to roll-back any actions that were added to the current transaction.

		This will perform an undo() only if there are some actions in the undo list
		that were added after the last call to beginNewTransaction().

		This is useful because it lets you call beginNewTransaction(), then
		perform an operation which may or may not actually perform some actions, and
		then call this method to get rid of any actions that might have been done
		without it rolling back the previous transaction if nothing was actually
		done.

		@returns true if any actions were undone.
	*/
	bool undoCurrentTransactionOnly();

	/** Returns a list of the UndoableAction objects that have been performed during the
		transaction that is currently open.

		Effectively, this is the list of actions that would be undone if undoCurrentTransactionOnly()
		were to be called now.

		The first item in the list is the earliest action performed.
	*/
	void getActionsInCurrentTransaction (Array <const UndoableAction*>& actionsFound) const;

	/** Returns the number of UndoableAction objects that have been performed during the
		transaction that is currently open.
		@see getActionsInCurrentTransaction
	*/
	int getNumActionsInCurrentTransaction() const;

	/** Returns true if there's at least one action in the list to redo.

		@see getRedoDescription, redo, canUndo
	*/
	bool canRedo() const;

	/** Returns the description of the transaction that would be next to get redone.

		The description returned is the one that was passed into beginNewTransaction
		before the set of actions was performed.

		@see redo
	*/
	String getRedoDescription() const;

	/** Tries to redo the last transaction that was undone.

		@returns    true if the transaction can be redone, and false if it fails, or
					if there aren't any transactions to redo
	*/
	bool redo();

private:

	OwnedArray <OwnedArray <UndoableAction> > transactions;
	StringArray transactionNames;
	String currentTransactionName;
	int totalUnitsStored, maxNumUnitsToKeep, minimumTransactionsToKeep, nextIndex;
	bool newTransaction, reentrancyCheck;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UndoManager);
};

#endif   // __JUCE_UNDOMANAGER_JUCEHEADER__

/*** End of inlined file: juce_UndoManager.h ***/

/**
	A powerful tree structure that can be used to hold free-form data, and which can
	handle its own undo and redo behaviour.

	A ValueTree contains a list of named properties as var objects, and also holds
	any number of sub-trees.

	Create ValueTree objects on the stack, and don't be afraid to copy them around, as
	they're simply a lightweight reference to a shared data container. Creating a copy
	of another ValueTree simply creates a new reference to the same underlying object - to
	make a separate, deep copy of a tree you should explicitly call createCopy().

	Each ValueTree has a type name, in much the same way as an XmlElement has a tag name,
	and much of the structure of a ValueTree is similar to an XmlElement tree.
	You can convert a ValueTree to and from an XmlElement, and as long as the XML doesn't
	contain text elements, the conversion works well and makes a good serialisation
	format. They can also be serialised to a binary format, which is very fast and compact.

	All the methods that change data take an optional UndoManager, which will be used
	to track any changes to the object. For this to work, you have to be careful to
	consistently always use the same UndoManager for all operations to any node inside
	the tree.

	A ValueTree can only be a child of one parent at a time, so if you're moving one from
	one tree to another, be careful to always remove it first, before adding it. This
	could also mess up your undo/redo chain, so be wary! In a debug build you should hit
	assertions if you try to do anything dangerous, but there are still plenty of ways it
	could go wrong.

	Listeners can be added to a ValueTree to be told when properies change and when
	nodes are added or removed.

	@see var, XmlElement
*/
class JUCE_API  ValueTree
{
public:

	/** Creates an empty, invalid ValueTree.

		A ValueTree that is created with this constructor can't actually be used for anything,
		it's just a default 'null' ValueTree that can be returned to indicate some sort of failure.
		To create a real one, use the constructor that takes a string.

		@see ValueTree::invalid
	*/
	ValueTree() noexcept;

	/** Creates an empty ValueTree with the given type name.
		Like an XmlElement, each ValueTree node has a type, which you can access with
		getType() and hasType().
	*/
	explicit ValueTree (const Identifier& type);

	/** Creates a reference to another ValueTree. */
	ValueTree (const ValueTree& other);

	/** Makes this object reference another node. */
	ValueTree& operator= (const ValueTree& other);

   #if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
	ValueTree (ValueTree&& other) noexcept;
	ValueTree& operator= (ValueTree&& other) noexcept;
   #endif

	/** Destructor. */
	~ValueTree();

	/** Returns true if both this and the other tree node refer to the same underlying structure.
		Note that this isn't a value comparison - two independently-created trees which
		contain identical data are not considered equal.
	*/
	bool operator== (const ValueTree& other) const noexcept;

	/** Returns true if this and the other node refer to different underlying structures.
		Note that this isn't a value comparison - two independently-created trees which
		contain identical data are not considered equal.
	*/
	bool operator!= (const ValueTree& other) const noexcept;

	/** Performs a deep comparison between the properties and children of two trees.
		If all the properties and children of the two trees are the same (recursively), this
		returns true.
		The normal operator==() only checks whether two trees refer to the same shared data
		structure, so use this method if you need to do a proper value comparison.
	*/
	bool isEquivalentTo (const ValueTree& other) const;

	/** Returns true if this node refers to some valid data.
		It's hard to create an invalid node, but you might get one returned, e.g. by an out-of-range
		call to getChild().
	*/
	bool isValid() const                            { return object != nullptr; }

	/** Returns a deep copy of this tree and all its sub-nodes. */
	ValueTree createCopy() const;

	/** Returns the type of this node.
		The type is specified when the ValueTree is created.
		@see hasType
	*/
	Identifier getType() const;

	/** Returns true if the node has this type.
		The comparison is case-sensitive.
	*/
	bool hasType (const Identifier& typeName) const;

	/** Returns the value of a named property.
		If no such property has been set, this will return a void variant.
		You can also use operator[] to get a property.
		@see var, setProperty, hasProperty
	*/
	const var& getProperty (const Identifier& name) const;

	/** Returns the value of a named property, or a user-specified default if the property doesn't exist.
		If no such property has been set, this will return the value of defaultReturnValue.
		You can also use operator[] and getProperty to get a property.
		@see var, getProperty, setProperty, hasProperty
	*/
	var getProperty (const Identifier& name, const var& defaultReturnValue) const;

	/** Returns the value of a named property.
		If no such property has been set, this will return a void variant. This is the same as
		calling getProperty().
		@see getProperty
	*/
	const var& operator[] (const Identifier& name) const;

	/** Changes a named property of the node.
		The name identifier must not be an empty string.
		If the undoManager parameter is non-null, its UndoManager::perform() method will be used,
		so that this change can be undone.
		@see var, getProperty, removeProperty
		@returns a reference to the value tree, so that you can daisy-chain calls to this method.
	*/
	ValueTree& setProperty (const Identifier& name, const var& newValue, UndoManager* undoManager);

	/** Returns true if the node contains a named property. */
	bool hasProperty (const Identifier& name) const;

	/** Removes a property from the node.
		If the undoManager parameter is non-null, its UndoManager::perform() method will be used,
		so that this change can be undone.
	*/
	void removeProperty (const Identifier& name, UndoManager* undoManager);

	/** Removes all properties from the node.
		If the undoManager parameter is non-null, its UndoManager::perform() method will be used,
		so that this change can be undone.
	*/
	void removeAllProperties (UndoManager* undoManager);

	/** Returns the total number of properties that the node contains.
		@see getProperty.
	*/
	int getNumProperties() const;

	/** Returns the identifier of the property with a given index.
		@see getNumProperties
	*/
	Identifier getPropertyName (int index) const;

	/** Returns a Value object that can be used to control and respond to one of the tree's properties.

		The Value object will maintain a reference to this tree, and will use the undo manager when
		it needs to change the value. Attaching a Value::Listener to the value object will provide
		callbacks whenever the property changes.
	*/
	Value getPropertyAsValue (const Identifier& name, UndoManager* undoManager);

	/** Returns the number of child nodes belonging to this one.
		@see getChild
	*/
	int getNumChildren() const;

	/** Returns one of this node's child nodes.
		If the index is out of range, it'll return an invalid node. (See isValid() to find out
		whether a node is valid).
	*/
	ValueTree getChild (int index) const;

	/** Returns the first child node with the speficied type name.
		If no such node is found, it'll return an invalid node. (See isValid() to find out
		whether a node is valid).
		@see getOrCreateChildWithName
	*/
	ValueTree getChildWithName (const Identifier& type) const;

	/** Returns the first child node with the speficied type name, creating and adding
		a child with this name if there wasn't already one there.

		The only time this will return an invalid object is when the object that you're calling
		the method on is itself invalid.
		@see getChildWithName
	*/
	ValueTree getOrCreateChildWithName (const Identifier& type, UndoManager* undoManager);

	/** Looks for the first child node that has the speficied property value.

		This will scan the child nodes in order, until it finds one that has property that matches
		the specified value.

		If no such node is found, it'll return an invalid node. (See isValid() to find out
		whether a node is valid).
	*/
	ValueTree getChildWithProperty (const Identifier& propertyName, const var& propertyValue) const;

	/** Adds a child to this node.

		Make sure that the child is removed from any former parent node before calling this, or
		you'll hit an assertion.

		If the index is < 0 or greater than the current number of child nodes, the new node will
		be added at the end of the list.

		If the undoManager parameter is non-null, its UndoManager::perform() method will be used,
		so that this change can be undone.
	*/
	void addChild (const ValueTree& child, int index, UndoManager* undoManager);

	/** Removes the specified child from this node's child-list.
		If the undoManager parameter is non-null, its UndoManager::perform() method will be used,
		so that this change can be undone.
	*/
	void removeChild (const ValueTree& child, UndoManager* undoManager);

	/** Removes a child from this node's child-list.
		If the undoManager parameter is non-null, its UndoManager::perform() method will be used,
		so that this change can be undone.
	*/
	void removeChild (int childIndex, UndoManager* undoManager);

	/** Removes all child-nodes from this node.
		If the undoManager parameter is non-null, its UndoManager::perform() method will be used,
		so that this change can be undone.
	*/
	void removeAllChildren (UndoManager* undoManager);

	/** Moves one of the children to a different index.

		This will move the child to a specified index, shuffling along any intervening
		items as required. So for example, if you have a list of { 0, 1, 2, 3, 4, 5 }, then
		calling move (2, 4) would result in { 0, 1, 3, 4, 2, 5 }.

		@param currentIndex     the index of the item to be moved. If this isn't a
								valid index, then nothing will be done
		@param newIndex         the index at which you'd like this item to end up. If this
								is less than zero, the value will be moved to the end
								of the list
		@param undoManager      the optional UndoManager to use to store this transaction
	*/
	void moveChild (int currentIndex, int newIndex, UndoManager* undoManager);

	/** Returns true if this node is anywhere below the specified parent node.
		This returns true if the node is a child-of-a-child, as well as a direct child.
	*/
	bool isAChildOf (const ValueTree& possibleParent) const;

	/** Returns the index of a child item in this parent.
		If the child isn't found, this returns -1.
	*/
	int indexOf (const ValueTree& child) const;

	/** Returns the parent node that contains this one.
		If the node has no parent, this will return an invalid node. (See isValid() to find out
		whether a node is valid).
	*/
	ValueTree getParent() const;

	/** Returns one of this node's siblings in its parent's child list.

		The delta specifies how far to move through the list, so a value of 1 would return the node
		that follows this one, -1 would return the node before it, 0 will return this node itself, etc.
		If the requested position is beyond the range of available nodes, this will return ValueTree::invalid.
	*/
	ValueTree getSibling (int delta) const;

	/** Creates an XmlElement that holds a complete image of this node and all its children.

		If this node is invalid, this may return 0. Otherwise, the XML that is produced can
		be used to recreate a similar node by calling fromXml()
		@see fromXml
	*/
	XmlElement* createXml() const;

	/** Tries to recreate a node from its XML representation.

		This isn't designed to cope with random XML data - for a sensible result, it should only
		be fed XML that was created by the createXml() method.
	*/
	static ValueTree fromXml (const XmlElement& xml);

	/** Stores this tree (and all its children) in a binary format.

		Once written, the data can be read back with readFromStream().

		It's much faster to load/save your tree in binary form than as XML, but
		obviously isn't human-readable.
	*/
	void writeToStream (OutputStream& output);

	/** Reloads a tree from a stream that was written with writeToStream(). */
	static ValueTree readFromStream (InputStream& input);

	/** Reloads a tree from a data block that was written with writeToStream(). */
	static ValueTree readFromData (const void* data, size_t numBytes);

	/** Reloads a tree from a data block that was written with writeToStream() and
		then zipped using GZIPCompressorOutputStream.
	*/
	static ValueTree readFromGZIPData (const void* data, size_t numBytes);

	/** Listener class for events that happen to a ValueTree.

		To get events from a ValueTree, make your class implement this interface, and use
		ValueTree::addListener() and ValueTree::removeListener() to register it.
	*/
	class JUCE_API  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}

		/** This method is called when a property of this node (or of one of its sub-nodes) has
			changed.

			The tree parameter indicates which tree has had its property changed, and the property
			parameter indicates the property.

			Note that when you register a listener to a tree, it will receive this callback for
			property changes in that tree, and also for any of its children, (recursively, at any depth).
			If your tree has sub-trees but you only want to know about changes to the top level tree,
			simply check the tree parameter in this callback to make sure it's the tree you're interested in.
		*/
		virtual void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged,
											   const Identifier& property) = 0;

		/** This method is called when a child sub-tree is added.

			Note that when you register a listener to a tree, it will receive this callback for
			child changes in both that tree and any of its children, (recursively, at any depth).
			If your tree has sub-trees but you only want to know about changes to the top level tree,
			just check the parentTree parameter to make sure it's the one that you're interested in.
		*/
		virtual void valueTreeChildAdded (ValueTree& parentTree,
										  ValueTree& childWhichHasBeenAdded) = 0;

		/** This method is called when a child sub-tree is removed.

			Note that when you register a listener to a tree, it will receive this callback for
			child changes in both that tree and any of its children, (recursively, at any depth).
			If your tree has sub-trees but you only want to know about changes to the top level tree,
			just check the parentTree parameter to make sure it's the one that you're interested in.
		*/
		virtual void valueTreeChildRemoved (ValueTree& parentTree,
											ValueTree& childWhichHasBeenRemoved) = 0;

		/** This method is called when a tree's children have been re-shuffled.

			Note that when you register a listener to a tree, it will receive this callback for
			child changes in both that tree and any of its children, (recursively, at any depth).
			If your tree has sub-trees but you only want to know about changes to the top level tree,
			just check the parameter to make sure it's the tree that you're interested in.
		*/
		virtual void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved) = 0;

		/** This method is called when a tree has been added or removed from a parent node.

			This callback happens when the tree to which the listener was registered is added or
			removed from a parent. Unlike the other callbacks, it applies only to the tree to which
			the listener is registered, and not to any of its children.
		*/
		virtual void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) = 0;
	};

	/** Adds a listener to receive callbacks when this node is changed.

		The listener is added to this specific ValueTree object, and not to the shared
		object that it refers to. When this object is deleted, all the listeners will
		be lost, even if other references to the same ValueTree still exist. And if you
		use the operator= to make this refer to a different ValueTree, any listeners will
		begin listening to changes to the new tree instead of the old one.

		When you're adding a listener, make sure that you add it to a ValueTree instance that
		will last for as long as you need the listener. In general, you'd never want to add a
		listener to a local stack-based ValueTree, and would usually add one to a member variable.

		@see removeListener
	*/
	void addListener (Listener* listener);

	/** Removes a listener that was previously added with addListener(). */
	void removeListener (Listener* listener);

	/** Causes a property-change callback to be triggered for the specified property,
		calling any listeners that are registered.
	*/
	void sendPropertyChangeMessage (const Identifier& property);

	/** This method uses a comparator object to sort the tree's children into order.

		The object provided must have a method of the form:
		@code
		int compareElements (const ValueTree& first, const ValueTree& second);
		@endcode

		..and this method must return:
		  - a value of < 0 if the first comes before the second
		  - a value of 0 if the two objects are equivalent
		  - a value of > 0 if the second comes before the first

		To improve performance, the compareElements() method can be declared as static or const.

		@param comparator   the comparator to use for comparing elements.
		@param undoManager  optional UndoManager for storing the changes
		@param retainOrderOfEquivalentItems     if this is true, then items which the comparator says are
							equivalent will be kept in the order in which they currently appear in the array.
							This is slower to perform, but may be important in some cases. If it's false, a
							faster algorithm is used, but equivalent elements may be rearranged.
	*/
	template <typename ElementComparator>
	void sort (ElementComparator& comparator, UndoManager* undoManager, bool retainOrderOfEquivalentItems)
	{
		if (object != nullptr)
		{
			OwnedArray<ValueTree> sortedList;
			createListOfChildren (sortedList);
			ComparatorAdapter <ElementComparator> adapter (comparator);
			sortedList.sort (adapter, retainOrderOfEquivalentItems);
			reorderChildren (sortedList, undoManager);
		}
	}

	/** An invalid ValueTree that can be used if you need to return one as an error condition, etc.
		This invalid object is equivalent to ValueTree created with its default constructor.
	*/
	static const ValueTree invalid;

private:

	class SharedObject;
	friend class SharedObject;

	ReferenceCountedObjectPtr<SharedObject> object;
	ListenerList<Listener> listeners;

	template <typename ElementComparator>
	struct ComparatorAdapter
	{
		ComparatorAdapter (ElementComparator& comparator_) noexcept : comparator (comparator_) {}

		int compareElements (const ValueTree* const first, const ValueTree* const second)
		{
			return comparator.compareElements (*first, *second);
		}

	private:
		ElementComparator& comparator;
		JUCE_DECLARE_NON_COPYABLE (ComparatorAdapter);
	};

	void createListOfChildren (OwnedArray<ValueTree>&) const;
	void reorderChildren (const OwnedArray<ValueTree>&, UndoManager*);

	explicit ValueTree (SharedObject*);
};

#endif   // __JUCE_VALUETREE_JUCEHEADER__

/*** End of inlined file: juce_ValueTree.h ***/


#endif
#ifndef __JUCE_UNDOABLEACTION_JUCEHEADER__

#endif
#ifndef __JUCE_UNDOMANAGER_JUCEHEADER__

#endif
#ifndef __JUCE_APPLICATIONPROPERTIES_JUCEHEADER__

/*** Start of inlined file: juce_ApplicationProperties.h ***/
#ifndef __JUCE_APPLICATIONPROPERTIES_JUCEHEADER__
#define __JUCE_APPLICATIONPROPERTIES_JUCEHEADER__


/*** Start of inlined file: juce_PropertiesFile.h ***/
#ifndef __JUCE_PROPERTIESFILE_JUCEHEADER__
#define __JUCE_PROPERTIESFILE_JUCEHEADER__

/** Wrapper on a file that stores a list of key/value data pairs.

	Useful for storing application settings, etc. See the PropertySet class for
	the interfaces that read and write values.

	Not designed for very large amounts of data, as it keeps all the values in
	memory and writes them out to disk lazily when they are changed.

	Because this class derives from ChangeBroadcaster, ChangeListeners can be registered
	with it, and these will be signalled when a value changes.

	@see PropertySet
*/
class JUCE_API  PropertiesFile  : public PropertySet,
								  public ChangeBroadcaster,
								  private Timer
{
public:

	enum StorageFormat
	{
		storeAsBinary,
		storeAsCompressedBinary,
		storeAsXML
	};

	struct JUCE_API  Options
	{
		/** Creates an empty Options structure.
			You'll need to fill-in the data memebers appropriately before using this structure.
		*/
		Options();

		/** The name of your application - this is used to help generate the path and filename
			at which the properties file will be stored. */
		String applicationName;

		/** The suffix to use for your properties file.
			It doesn't really matter what this is - you may want to use ".settings" or
			".properties" or something.
		*/
		String filenameSuffix;

		/** The name of a subfolder in which you'd like your properties file to live.
			See the getDefaultFile() method for more details about how this is used.
		*/
		String folderName;

		/** If you're using properties files on a Mac, you must set this value - failure to
			do so will cause a runtime assertion.

			The PropertiesFile class always used to put its settings files in "Library/Preferences", but Apple
			have changed their advice, and now stipulate that settings should go in "Library/Application Support".

			Because older apps would be broken by a silent change in this class's behaviour, you must now
			explicitly set the osxLibrarySubFolder value to indicate which path you want to use.

			In newer apps, you should always set this to "Application Support".

			If your app needs to load settings files that were created by older versions of juce and
			you want to maintain backwards-compatibility, then you can set this to "Preferences".
			But.. for better Apple-compliance, the recommended approach would be to write some code that
			finds your old settings files in ~/Library/Preferences, moves them to ~/Library/Application Support,
			and then uses the new path.
		*/
		String osxLibrarySubFolder;

		/** If true, the file will be created in a location that's shared between users.
			The default constructor initialises this value to false.
		*/
		bool commonToAllUsers;

		/** If true, this means that property names are matched in a case-insensitive manner.
			See the PropertySet constructor for more info.
			The default constructor initialises this value to false.
		*/
		bool ignoreCaseOfKeyNames;

		/** If this is zero or greater, then after a value is changed, the object will wait
			for this amount of time and then save the file. If this zero, the file will be
			written to disk immediately on being changed (which might be slow, as it'll re-write
			synchronously each time a value-change method is called). If it is less than zero,
			the file won't be saved until save() or saveIfNeeded() are explicitly called.
			The default constructor sets this to a reasonable value of a few seconds, so you
			only need to change it if you need a special case.
		*/
		int millisecondsBeforeSaving;

		/** Specifies whether the file should be written as XML, binary, etc.
			The default constructor sets this to storeAsXML, so you only need to set it explicitly
			if you want to use a different format.
		*/
		StorageFormat storageFormat;

		/** An optional InterprocessLock object that will be used to prevent multiple threads or
			processes from writing to the file at the same time. The PropertiesFile will keep a
			pointer to this object but will not take ownership of it - the caller is responsible for
			making sure that the lock doesn't get deleted before the PropertiesFile has been deleted.
			The default constructor initialises this value to nullptr, so you don't need to touch it
			unless you want to use a lock.
		*/
		InterProcessLock* processLock;

		/** This can be called to suggest a file that should be used, based on the values
			in this structure.

			So on a Mac, this will return a file called:
			~/Library/[osxLibrarySubFolder]/[folderName]/[applicationName].[filenameSuffix]

			On Windows it'll return something like:
			C:\\Documents and Settings\\username\\Application Data\\[folderName]\\[applicationName].[filenameSuffix]

			On Linux it'll return
			~/.[folderName]/[applicationName].[filenameSuffix]

			If the folderName variable is empty, it'll use the app name for this (or omit the
			folder name on the Mac).

			The paths will also vary depending on whether commonToAllUsers is true.
		*/
		File getDefaultFile() const;
	};

	/** Creates a PropertiesFile object.
		The file used will be chosen by calling PropertiesFile::Options::getDefaultFile()
		for the options provided. To set the file explicitly, use the other constructor.
	*/
	explicit PropertiesFile (const Options& options);

	/** Creates a PropertiesFile object.
		Unlike the other constructor, this one allows you to explicitly set the file that you
		want to be used, rather than using the default one.
	*/
	PropertiesFile (const File& file,
					const Options& options);

	/** Destructor.
		When deleted, the file will first call saveIfNeeded() to flush any changes to disk.
	*/
	~PropertiesFile();

	/** Returns true if this file was created from a valid (or non-existent) file.
		If the file failed to load correctly because it was corrupt or had insufficient
		access, this will be false.
	*/
	bool isValidFile() const noexcept               { return loadedOk; }

	/** This will flush all the values to disk if they've changed since the last
		time they were saved.

		Returns false if it fails to write to the file for some reason (maybe because
		it's read-only or the directory doesn't exist or something).

		@see save
	*/
	bool saveIfNeeded();

	/** This will force a write-to-disk of the current values, regardless of whether
		anything has changed since the last save.

		Returns false if it fails to write to the file for some reason (maybe because
		it's read-only or the directory doesn't exist or something).

		@see saveIfNeeded
	*/
	bool save();

	/** Returns true if the properties have been altered since the last time they were saved.
		The file is flagged as needing to be saved when you change a value, but you can
		explicitly set this flag with setNeedsToBeSaved().
	*/
	bool needsToBeSaved() const;

	/** Explicitly sets the flag to indicate whether the file needs saving or not.
		@see needsToBeSaved
	*/
	void setNeedsToBeSaved (bool needsToBeSaved);

	/** Returns the file that's being used. */
	File getFile() const                              { return file; }

protected:
	/** @internal */
	virtual void propertyChanged();

private:

	File file;
	Options options;
	bool loadedOk, needsWriting;

	typedef const ScopedPointer<InterProcessLock::ScopedLockType> ProcessScopedLock;
	InterProcessLock::ScopedLockType* createProcessLock() const;

	void timerCallback();
	void initialise();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertiesFile);
};

#endif   // __JUCE_PROPERTIESFILE_JUCEHEADER__

/*** End of inlined file: juce_PropertiesFile.h ***/

/**
	Manages a collection of properties.

	This is a slightly higher-level wrapper for managing PropertiesFile objects.

	It holds two different PropertiesFile objects internally, one for user-specific
	settings (stored in your user directory), and one for settings that are common to
	all users (stored in a folder accessible to all users).

	The class manages the creation of these files on-demand, allowing access via the
	getUserSettings() and getCommonSettings() methods. It also has a few handy
	methods like testWriteAccess() to check that the files can be saved.

	After creating an instance of an ApplicationProperties object, you should first
	of all call setStorageParameters() to tell it the parameters to use to create
	its files.

	@see PropertiesFile
*/
class JUCE_API  ApplicationProperties
{
public:

	/**
		Creates an ApplicationProperties object.

		Before using it, you must call setStorageParameters() to give it the info
		it needs to create the property files.
	*/
	ApplicationProperties();

	/** Destructor. */
	~ApplicationProperties();

	/** Gives the object the information it needs to create the appropriate properties files.
		See the PropertiesFile::Options class for details about what options you need to set.
	*/
	void setStorageParameters (const PropertiesFile::Options& options);

	/** Returns the user settings file.

		The first time this is called, it will create and load the properties file.

		Note that when you search the user PropertiesFile for a value that it doesn't contain,
		the common settings are used as a second-chance place to look. This is done via the
		PropertySet::setFallbackPropertySet() method - by default the common settings are set
		to the fallback for the user settings.

		@see getCommonSettings
	*/
	PropertiesFile* getUserSettings();

	/** Returns the common settings file.

		The first time this is called, it will create and load the properties file.

		@param returnUserPropsIfReadOnly  if this is true, and the common properties file is
							read-only (e.g. because the user doesn't have permission to write
							to shared files), then this will return the user settings instead,
							(like getUserSettings() would do). This is handy if you'd like to
							write a value to the common settings, but if that's no possible,
							then you'd rather write to the user settings than none at all.
							If returnUserPropsIfReadOnly is false, this method will always return
							the common settings, even if any changes to them can't be saved.
		@see getUserSettings
	*/
	PropertiesFile* getCommonSettings (bool returnUserPropsIfReadOnly);

	/** Saves both files if they need to be saved.

		@see PropertiesFile::saveIfNeeded
	*/
	bool saveIfNeeded();

	/** Flushes and closes both files if they are open.

		This flushes any pending changes to disk with PropertiesFile::saveIfNeeded()
		and closes both files. They will then be re-opened the next time getUserSettings()
		or getCommonSettings() is called.
	*/
	void closeFiles();

private:

	PropertiesFile::Options options;
	ScopedPointer <PropertiesFile> userProps, commonProps;
	int commonSettingsAreReadOnly;

	void openFiles();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationProperties);
};

#endif   // __JUCE_APPLICATIONPROPERTIES_JUCEHEADER__

/*** End of inlined file: juce_ApplicationProperties.h ***/


#endif
#ifndef __JUCE_PROPERTIESFILE_JUCEHEADER__

#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_DATA_STRUCTURES_JUCEHEADER__

/*** End of inlined file: juce_data_structures.h ***/

