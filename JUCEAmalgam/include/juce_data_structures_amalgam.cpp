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

/*** Start of inlined file: juce_data_structures.cpp ***/
#if defined (__JUCE_DATA_STRUCTURES_JUCEHEADER__) && ! JUCE_AMALGAMATED_INCLUDE
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

#include "juce_data_structures_amalgam.h"

namespace juce
{

// START_AUTOINCLUDE values/*.cpp, undomanager/*.cpp, app_properties/*.cpp

/*** Start of inlined file: juce_Value.cpp ***/
Value::ValueSource::ValueSource()
{
}

Value::ValueSource::~ValueSource()
{
}

void Value::ValueSource::sendChangeMessage (const bool synchronous)
{
	if (synchronous)
	{
		// (hold a local reference to this object in case it's freed during the callbacks)
		const ReferenceCountedObjectPtr<ValueSource> localRef (this);

		for (int i = valuesWithListeners.size(); --i >= 0;)
		{
			Value* const v = valuesWithListeners[i];

			if (v != nullptr)
				v->callListeners();
		}
	}
	else
	{
		if (valuesWithListeners.size() > 0)
			triggerAsyncUpdate();
	}
}

void Value::ValueSource::handleAsyncUpdate()
{
	sendChangeMessage (true);
}

class SimpleValueSource  : public Value::ValueSource
{
public:
	SimpleValueSource()
	{
	}

	SimpleValueSource (const var& initialValue)
		: value (initialValue)
	{
	}

	var getValue() const
	{
		return value;
	}

	void setValue (const var& newValue)
	{
		if (! newValue.equalsWithSameType (value))
		{
			value = newValue;
			sendChangeMessage (false);
		}
	}

private:
	var value;

	JUCE_DECLARE_NON_COPYABLE (SimpleValueSource);
};

Value::Value()
	: value (new SimpleValueSource())
{
}

Value::Value (ValueSource* const value_)
	: value (value_)
{
	jassert (value_ != nullptr);
}

Value::Value (const var& initialValue)
	: value (new SimpleValueSource (initialValue))
{
}

Value::Value (const Value& other)
	: value (other.value)
{
}

Value& Value::operator= (const Value& other)
{
	value = other.value;
	return *this;
}

#if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
Value::Value (Value&& other) noexcept
	: value (static_cast <ReferenceCountedObjectPtr <ValueSource>&&> (other.value))
{
}

Value& Value::operator= (Value&& other) noexcept
{
	value = static_cast <ReferenceCountedObjectPtr <ValueSource>&&> (other.value);
	return *this;
}
#endif

Value::~Value()
{
	if (listeners.size() > 0)
		value->valuesWithListeners.removeValue (this);
}

var Value::getValue() const
{
	return value->getValue();
}

Value::operator var() const
{
	return value->getValue();
}

void Value::setValue (const var& newValue)
{
	value->setValue (newValue);
}

String Value::toString() const
{
	return value->getValue().toString();
}

Value& Value::operator= (const var& newValue)
{
	value->setValue (newValue);
	return *this;
}

void Value::referTo (const Value& valueToReferTo)
{
	if (valueToReferTo.value != value)
	{
		if (listeners.size() > 0)
		{
			value->valuesWithListeners.removeValue (this);
			valueToReferTo.value->valuesWithListeners.add (this);
		}

		value = valueToReferTo.value;
		callListeners();
	}
}

bool Value::refersToSameSourceAs (const Value& other) const
{
	return value == other.value;
}

bool Value::operator== (const Value& other) const
{
	return value == other.value || value->getValue() == other.getValue();
}

bool Value::operator!= (const Value& other) const
{
	return value != other.value && value->getValue() != other.getValue();
}

void Value::addListener (ValueListener* const listener)
{
	if (listener != nullptr)
	{
		if (listeners.size() == 0)
			value->valuesWithListeners.add (this);

		listeners.add (listener);
	}
}

void Value::removeListener (ValueListener* const listener)
{
	listeners.remove (listener);

	if (listeners.size() == 0)
		value->valuesWithListeners.removeValue (this);
}

void Value::callListeners()
{
	Value v (*this); // (create a copy in case this gets deleted by a callback)
	listeners.call (&ValueListener::valueChanged, v);
}

OutputStream& JUCE_CALLTYPE operator<< (OutputStream& stream, const Value& value)
{
	return stream << value.toString();
}

/*** End of inlined file: juce_Value.cpp ***/



/*** Start of inlined file: juce_ValueTree.cpp ***/
class ValueTree::SharedObject  : public ReferenceCountedObject
{
public:
	typedef ReferenceCountedObjectPtr<SharedObject> Ptr;

	explicit SharedObject (const Identifier& type_) noexcept
		: type (type_), parent (nullptr)
	{
	}

	SharedObject (const SharedObject& other)
		: type (other.type), properties (other.properties), parent (nullptr)
	{
		for (int i = 0; i < other.children.size(); ++i)
		{
			SharedObject* const child = new SharedObject (*other.children.getObjectPointerUnchecked(i));
			child->parent = this;
			children.add (child);
		}
	}

	~SharedObject()
	{
		jassert (parent == nullptr); // this should never happen unless something isn't obeying the ref-counting!

		for (int i = children.size(); --i >= 0;)
		{
			const Ptr c (children.getObjectPointerUnchecked(i));
			c->parent = nullptr;
			children.remove (i);
			c->sendParentChangeMessage();
		}
	}

	void sendPropertyChangeMessage (ValueTree& tree, const Identifier& property)
	{
		for (int i = valueTreesWithListeners.size(); --i >= 0;)
		{
			ValueTree* const v = valueTreesWithListeners[i];
			if (v != nullptr)
				v->listeners.call (&ValueTree::Listener::valueTreePropertyChanged, tree, property);
		}
	}

	void sendPropertyChangeMessage (const Identifier& property)
	{
		ValueTree tree (this);

		for (ValueTree::SharedObject* t = this; t != nullptr; t = t->parent)
			t->sendPropertyChangeMessage (tree, property);
	}

	void sendChildAddedMessage (ValueTree& tree, ValueTree& child)
	{
		for (int i = valueTreesWithListeners.size(); --i >= 0;)
		{
			ValueTree* const v = valueTreesWithListeners[i];
			if (v != nullptr)
				v->listeners.call (&ValueTree::Listener::valueTreeChildAdded, tree, child);
		}
	}

	void sendChildAddedMessage (ValueTree child)
	{
		ValueTree tree (this);

		for (ValueTree::SharedObject* t = this; t != nullptr; t = t->parent)
			t->sendChildAddedMessage (tree, child);
	}

	void sendChildRemovedMessage (ValueTree& tree, ValueTree& child)
	{
		for (int i = valueTreesWithListeners.size(); --i >= 0;)
		{
			ValueTree* const v = valueTreesWithListeners[i];
			if (v != nullptr)
				v->listeners.call (&ValueTree::Listener::valueTreeChildRemoved, tree, child);
		}
	}

	void sendChildRemovedMessage (ValueTree child)
	{
		ValueTree tree (this);

		for (ValueTree::SharedObject* t = this; t != nullptr; t = t->parent)
			t->sendChildRemovedMessage (tree, child);
	}

	void sendChildOrderChangedMessage (ValueTree& tree)
	{
		for (int i = valueTreesWithListeners.size(); --i >= 0;)
		{
			ValueTree* const v = valueTreesWithListeners[i];
			if (v != nullptr)
				v->listeners.call (&ValueTree::Listener::valueTreeChildOrderChanged, tree);
		}
	}

	void sendChildOrderChangedMessage()
	{
		ValueTree tree (this);

		for (ValueTree::SharedObject* t = this; t != nullptr; t = t->parent)
			t->sendChildOrderChangedMessage (tree);
	}

	void sendParentChangeMessage()
	{
		ValueTree tree (this);

		for (int j = children.size(); --j >= 0;)
		{
			SharedObject* const child = children.getObjectPointer (j);
			if (child != nullptr)
				child->sendParentChangeMessage();
		}

		for (int i = valueTreesWithListeners.size(); --i >= 0;)
		{
			ValueTree* const v = valueTreesWithListeners[i];
			if (v != nullptr)
				v->listeners.call (&ValueTree::Listener::valueTreeParentChanged, tree);
		}
	}

	const var& getProperty (const Identifier& name) const noexcept
	{
		return properties [name];
	}

	var getProperty (const Identifier& name, const var& defaultReturnValue) const
	{
		return properties.getWithDefault (name, defaultReturnValue);
	}

	void setProperty (const Identifier& name, const var& newValue, UndoManager* const undoManager)
	{
		if (undoManager == nullptr)
		{
			if (properties.set (name, newValue))
				sendPropertyChangeMessage (name);
		}
		else
		{
			const var* const existingValue = properties.getVarPointer (name);

			if (existingValue != nullptr)
			{
				if (*existingValue != newValue)
					undoManager->perform (new SetPropertyAction (this, name, newValue, *existingValue, false, false));
			}
			else
			{
				undoManager->perform (new SetPropertyAction (this, name, newValue, var::null, true, false));
			}
		}
	}

	bool hasProperty (const Identifier& name) const noexcept
	{
		return properties.contains (name);
	}

	void removeProperty (const Identifier& name, UndoManager* const undoManager)
	{
		if (undoManager == nullptr)
		{
			if (properties.remove (name))
				sendPropertyChangeMessage (name);
		}
		else
		{
			if (properties.contains (name))
				undoManager->perform (new SetPropertyAction (this, name, var::null, properties [name], false, true));
		}
	}

	void removeAllProperties (UndoManager* const undoManager)
	{
		if (undoManager == nullptr)
		{
			while (properties.size() > 0)
			{
				const Identifier name (properties.getName (properties.size() - 1));
				properties.remove (name);
				sendPropertyChangeMessage (name);
			}
		}
		else
		{
			for (int i = properties.size(); --i >= 0;)
				undoManager->perform (new SetPropertyAction (this, properties.getName(i), var::null,
															 properties.getValueAt(i), false, true));
		}
	}

	ValueTree getChildWithName (const Identifier& typeToMatch) const
	{
		for (int i = 0; i < children.size(); ++i)
		{
			SharedObject* const s = children.getObjectPointerUnchecked (i);
			if (s->type == typeToMatch)
				return ValueTree (s);
		}

		return ValueTree::invalid;
	}

	ValueTree getOrCreateChildWithName (const Identifier& typeToMatch, UndoManager* undoManager)
	{
		for (int i = 0; i < children.size(); ++i)
		{
			SharedObject* const s = children.getObjectPointerUnchecked (i);
			if (s->type == typeToMatch)
				return ValueTree (s);
		}

		SharedObject* const newObject = new SharedObject (typeToMatch);
		addChild (newObject, -1, undoManager);
		return ValueTree (newObject);

	}

	ValueTree getChildWithProperty (const Identifier& propertyName, const var& propertyValue) const
	{
		for (int i = 0; i < children.size(); ++i)
		{
			SharedObject* const s = children.getObjectPointerUnchecked (i);
			if (s->getProperty (propertyName) == propertyValue)
				return ValueTree (s);
		}

		return ValueTree::invalid;
	}

	bool isAChildOf (const SharedObject* const possibleParent) const noexcept
	{
		const SharedObject* p = parent;

		while (p != nullptr)
		{
			if (p == possibleParent)
				return true;

			p = p->parent;
		}

		return false;
	}

	int indexOf (const ValueTree& child) const noexcept
	{
		return children.indexOf (child.object);
	}

	void addChild (SharedObject* child, int index, UndoManager* const undoManager)
	{
		if (child != nullptr && child->parent != this)
		{
			if (child != this && ! isAChildOf (child))
			{
				// You should always make sure that a child is removed from its previous parent before
				// adding it somewhere else - otherwise, it's ambiguous as to whether a different
				// undomanager should be used when removing it from its current parent..
				jassert (child->parent == nullptr);

				if (child->parent != nullptr)
				{
					jassert (child->parent->children.indexOf (child) >= 0);
					child->parent->removeChild (child->parent->children.indexOf (child), undoManager);
				}

				if (undoManager == nullptr)
				{
					children.insert (index, child);
					child->parent = this;
					sendChildAddedMessage (ValueTree (child));
					child->sendParentChangeMessage();
				}
				else
				{
					if (index < 0)
						index = children.size();

					undoManager->perform (new AddOrRemoveChildAction (this, index, child));
				}
			}
			else
			{
				// You're attempting to create a recursive loop! A node
				// can't be a child of one of its own children!
				jassertfalse;
			}
		}
	}

	void removeChild (const int childIndex, UndoManager* const undoManager)
	{
		const Ptr child (children.getObjectPointer (childIndex));

		if (child != nullptr)
		{
			if (undoManager == nullptr)
			{
				children.remove (childIndex);
				child->parent = nullptr;
				sendChildRemovedMessage (ValueTree (child));
				child->sendParentChangeMessage();
			}
			else
			{
				undoManager->perform (new AddOrRemoveChildAction (this, childIndex, nullptr));
			}
		}
	}

	void removeAllChildren (UndoManager* const undoManager)
	{
		while (children.size() > 0)
			removeChild (children.size() - 1, undoManager);
	}

	void moveChild (int currentIndex, int newIndex, UndoManager* undoManager)
	{
		// The source index must be a valid index!
		jassert (isPositiveAndBelow (currentIndex, children.size()));

		if (currentIndex != newIndex
			 && isPositiveAndBelow (currentIndex, children.size()))
		{
			if (undoManager == nullptr)
			{
				children.move (currentIndex, newIndex);
				sendChildOrderChangedMessage();
			}
			else
			{
				if (! isPositiveAndBelow (newIndex, children.size()))
					newIndex = children.size() - 1;

				undoManager->perform (new MoveChildAction (this, currentIndex, newIndex));
			}
		}
	}

	void reorderChildren (const OwnedArray<ValueTree>& newOrder, UndoManager* undoManager)
	{
		jassert (newOrder.size() == children.size());

		if (undoManager == nullptr)
		{
			children.clear();
			children.ensureStorageAllocated (newOrder.size());

			for (int i = 0; i < newOrder.size(); ++i)
				children.add (newOrder.getUnchecked(i)->object);

			sendChildOrderChangedMessage();
		}
		else
		{
			for (int i = 0; i < children.size(); ++i)
			{
				SharedObject* const child = newOrder.getUnchecked(i)->object;

				if (children.getObjectPointerUnchecked (i) != child)
				{
					const int oldIndex = children.indexOf (child);
					jassert (oldIndex >= 0);
					moveChild (oldIndex, i, undoManager);
				}
			}
		}
	}

	bool isEquivalentTo (const SharedObject& other) const
	{
		if (type != other.type
			 || properties.size() != other.properties.size()
			 || children.size() != other.children.size()
			 || properties != other.properties)
			return false;

		for (int i = 0; i < children.size(); ++i)
			if (! children.getObjectPointerUnchecked(i)->isEquivalentTo (*other.children.getObjectPointerUnchecked(i)))
				return false;

		return true;
	}

	XmlElement* createXml() const
	{
		XmlElement* const xml = new XmlElement (type.toString());
		properties.copyToXmlAttributes (*xml);

		for (int i = 0; i < children.size(); ++i)
			xml->addChildElement (children.getObjectPointerUnchecked(i)->createXml());

		return xml;
	}

	void writeToStream (OutputStream& output) const
	{
		output.writeString (type.toString());
		output.writeCompressedInt (properties.size());

		for (int j = 0; j < properties.size(); ++j)
		{
			output.writeString (properties.getName (j).toString());
			properties.getValueAt(j).writeToStream (output);
		}

		output.writeCompressedInt (children.size());

		for (int i = 0; i < children.size(); ++i)
			writeObjectToStream (output, children.getObjectPointerUnchecked(i));
	}

	static void writeObjectToStream (OutputStream& output, const SharedObject* const object)
	{
		if (object != nullptr)
		{
			object->writeToStream (output);
		}
		else
		{
			output.writeString (String::empty);
			output.writeCompressedInt (0);
			output.writeCompressedInt (0);
		}
	}

	class SetPropertyAction  : public UndoableAction
	{
	public:
		SetPropertyAction (SharedObject* const target_, const Identifier& name_,
						   const var& newValue_, const var& oldValue_,
						   const bool isAddingNewProperty_, const bool isDeletingProperty_)
			: target (target_), name (name_), newValue (newValue_), oldValue (oldValue_),
			  isAddingNewProperty (isAddingNewProperty_), isDeletingProperty (isDeletingProperty_)
		{
		}

		bool perform()
		{
			jassert (! (isAddingNewProperty && target->hasProperty (name)));

			if (isDeletingProperty)
				target->removeProperty (name, nullptr);
			else
				target->setProperty (name, newValue, nullptr);

			return true;
		}

		bool undo()
		{
			if (isAddingNewProperty)
				target->removeProperty (name, nullptr);
			else
				target->setProperty (name, oldValue, nullptr);

			return true;
		}

		int getSizeInUnits()
		{
			return (int) sizeof (*this); //xxx should be more accurate
		}

		UndoableAction* createCoalescedAction (UndoableAction* nextAction)
		{
			if (! (isAddingNewProperty || isDeletingProperty))
			{
				SetPropertyAction* const next = dynamic_cast <SetPropertyAction*> (nextAction);

				if (next != nullptr && next->target == target && next->name == name
					 && ! (next->isAddingNewProperty || next->isDeletingProperty))
				{
					return new SetPropertyAction (target, name, next->newValue, oldValue, false, false);
				}
			}

			return nullptr;
		}

	private:
		const Ptr target;
		const Identifier name;
		const var newValue;
		var oldValue;
		const bool isAddingNewProperty : 1, isDeletingProperty : 1;

		JUCE_DECLARE_NON_COPYABLE (SetPropertyAction);
	};

	class AddOrRemoveChildAction  : public UndoableAction
	{
	public:
		AddOrRemoveChildAction (SharedObject* target_, const int childIndex_, SharedObject* newChild_)
			: target (target_),
			  child (newChild_ != nullptr ? newChild_ : target_->children.getObjectPointer (childIndex_)),
			  childIndex (childIndex_),
			  isDeleting (newChild_ == nullptr)
		{
			jassert (child != nullptr);
		}

		bool perform()
		{
			if (isDeleting)
				target->removeChild (childIndex, nullptr);
			else
				target->addChild (child, childIndex, nullptr);

			return true;
		}

		bool undo()
		{
			if (isDeleting)
			{
				target->addChild (child, childIndex, nullptr);
			}
			else
			{
				// If you hit this, it seems that your object's state is getting confused - probably
				// because you've interleaved some undoable and non-undoable operations?
				jassert (childIndex < target->children.size());
				target->removeChild (childIndex, nullptr);
			}

			return true;
		}

		int getSizeInUnits()
		{
			return (int) sizeof (*this); //xxx should be more accurate
		}

	private:
		const Ptr target, child;
		const int childIndex;
		const bool isDeleting;

		JUCE_DECLARE_NON_COPYABLE (AddOrRemoveChildAction);
	};

	class MoveChildAction  : public UndoableAction
	{
	public:
		MoveChildAction (SharedObject* const parent_, const int startIndex_, const int endIndex_) noexcept
			: parent (parent_), startIndex (startIndex_), endIndex (endIndex_)
		{
		}

		bool perform()
		{
			parent->moveChild (startIndex, endIndex, nullptr);
			return true;
		}

		bool undo()
		{
			parent->moveChild (endIndex, startIndex, nullptr);
			return true;
		}

		int getSizeInUnits()
		{
			return (int) sizeof (*this); //xxx should be more accurate
		}

		UndoableAction* createCoalescedAction (UndoableAction* nextAction)
		{
			MoveChildAction* next = dynamic_cast <MoveChildAction*> (nextAction);

			if (next != nullptr && next->parent == parent && next->startIndex == endIndex)
				return new MoveChildAction (parent, startIndex, next->endIndex);

			return nullptr;
		}

	private:
		const Ptr parent;
		const int startIndex, endIndex;

		JUCE_DECLARE_NON_COPYABLE (MoveChildAction);
	};

	const Identifier type;
	NamedValueSet properties;
	ReferenceCountedArray<SharedObject> children;
	SortedSet<ValueTree*> valueTreesWithListeners;
	SharedObject* parent;

private:
	SharedObject& operator= (const SharedObject&);
	JUCE_LEAK_DETECTOR (SharedObject);
};

ValueTree::ValueTree() noexcept
{
}

const ValueTree ValueTree::invalid;

ValueTree::ValueTree (const Identifier& type_)
	: object (new ValueTree::SharedObject (type_))
{
	jassert (type_.toString().isNotEmpty()); // All objects should be given a sensible type name!
}

ValueTree::ValueTree (SharedObject* const object_)
	: object (object_)
{
}

ValueTree::ValueTree (const ValueTree& other)
	: object (other.object)
{
}

ValueTree& ValueTree::operator= (const ValueTree& other)
{
	if (listeners.size() > 0)
	{
		if (object != nullptr)
			object->valueTreesWithListeners.removeValue (this);

		if (other.object != nullptr)
			other.object->valueTreesWithListeners.add (this);
	}

	object = other.object;
	return *this;
}

#if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
ValueTree::ValueTree (ValueTree&& other) noexcept
	: object (static_cast <SharedObject::Ptr&&> (other.object))
{
}

ValueTree& ValueTree::operator= (ValueTree&& other) noexcept
{
	object = static_cast <SharedObject::Ptr&&> (other.object);
	return *this;
}
#endif

ValueTree::~ValueTree()
{
	if (listeners.size() > 0 && object != nullptr)
		object->valueTreesWithListeners.removeValue (this);
}

bool ValueTree::operator== (const ValueTree& other) const noexcept
{
	return object == other.object;
}

bool ValueTree::operator!= (const ValueTree& other) const noexcept
{
	return object != other.object;
}

bool ValueTree::isEquivalentTo (const ValueTree& other) const
{
	return object == other.object
			|| (object != nullptr && other.object != nullptr
				 && object->isEquivalentTo (*other.object));
}

ValueTree ValueTree::createCopy() const
{
	return ValueTree (createCopyIfNotNull (object.getObject()));
}

bool ValueTree::hasType (const Identifier& typeName) const
{
	return object != nullptr && object->type == typeName;
}

Identifier ValueTree::getType() const
{
	return object != nullptr ? object->type : Identifier();
}

ValueTree ValueTree::getParent() const
{
	return ValueTree (object != nullptr ? object->parent
										: static_cast <SharedObject*> (nullptr));
}

ValueTree ValueTree::getSibling (const int delta) const
{
	if (object == nullptr || object->parent == nullptr)
		return invalid;

	const int index = object->parent->indexOf (*this) + delta;
	return ValueTree (object->parent->children.getObjectPointer (index));
}

const var& ValueTree::operator[] (const Identifier& name) const
{
	return object == nullptr ? var::null : object->getProperty (name);
}

const var& ValueTree::getProperty (const Identifier& name) const
{
	return object == nullptr ? var::null : object->getProperty (name);
}

var ValueTree::getProperty (const Identifier& name, const var& defaultReturnValue) const
{
	return object == nullptr ? defaultReturnValue
							 : object->getProperty (name, defaultReturnValue);
}

ValueTree& ValueTree::setProperty (const Identifier& name, const var& newValue,
								   UndoManager* const undoManager)
{
	jassert (name.toString().isNotEmpty());

	if (object != nullptr)
		object->setProperty (name, newValue, undoManager);

	return *this;
}

bool ValueTree::hasProperty (const Identifier& name) const
{
	return object != nullptr && object->hasProperty (name);
}

void ValueTree::removeProperty (const Identifier& name, UndoManager* const undoManager)
{
	if (object != nullptr)
		object->removeProperty (name, undoManager);
}

void ValueTree::removeAllProperties (UndoManager* const undoManager)
{
	if (object != nullptr)
		object->removeAllProperties (undoManager);
}

int ValueTree::getNumProperties() const
{
	return object == nullptr ? 0 : object->properties.size();
}

Identifier ValueTree::getPropertyName (const int index) const
{
	return object == nullptr ? Identifier()
							 : object->properties.getName (index);
}

class ValueTreePropertyValueSource  : public Value::ValueSource,
									  public ValueTree::Listener
{
public:
	ValueTreePropertyValueSource (const ValueTree& tree_, const Identifier& property_,
								  UndoManager* const undoManager_)
		: tree (tree_), property (property_), undoManager (undoManager_)
	{
		tree.addListener (this);
	}

	~ValueTreePropertyValueSource()
	{
		tree.removeListener (this);
	}

	var getValue() const                 { return tree [property]; }
	void setValue (const var& newValue)  { tree.setProperty (property, newValue, undoManager); }

	void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged,
								   const Identifier& changedProperty)
	{
		if (tree == treeWhosePropertyHasChanged && property == changedProperty)
			sendChangeMessage (false);
	}

	void valueTreeChildAdded (ValueTree&, ValueTree&) {}
	void valueTreeChildRemoved (ValueTree&, ValueTree&) {}
	void valueTreeChildOrderChanged (ValueTree&) {}
	void valueTreeParentChanged (ValueTree&) {}

private:
	ValueTree tree;
	const Identifier property;
	UndoManager* const undoManager;

	JUCE_DECLARE_NON_COPYABLE (ValueTreePropertyValueSource);
};

Value ValueTree::getPropertyAsValue (const Identifier& name, UndoManager* const undoManager)
{
	return Value (new ValueTreePropertyValueSource (*this, name, undoManager));
}

int ValueTree::getNumChildren() const
{
	return object == nullptr ? 0 : object->children.size();
}

ValueTree ValueTree::getChild (int index) const
{
	return ValueTree (object != nullptr ? object->children.getObjectPointer (index)
										: static_cast <SharedObject*> (nullptr));
}

ValueTree ValueTree::getChildWithName (const Identifier& type) const
{
	return object != nullptr ? object->getChildWithName (type) : ValueTree::invalid;
}

ValueTree ValueTree::getOrCreateChildWithName (const Identifier& type, UndoManager* undoManager)
{
	return object != nullptr ? object->getOrCreateChildWithName (type, undoManager) : ValueTree::invalid;
}

ValueTree ValueTree::getChildWithProperty (const Identifier& propertyName, const var& propertyValue) const
{
	return object != nullptr ? object->getChildWithProperty (propertyName, propertyValue) : ValueTree::invalid;
}

bool ValueTree::isAChildOf (const ValueTree& possibleParent) const
{
	return object != nullptr && object->isAChildOf (possibleParent.object);
}

int ValueTree::indexOf (const ValueTree& child) const
{
	return object != nullptr ? object->indexOf (child) : -1;
}

void ValueTree::addChild (const ValueTree& child, int index, UndoManager* const undoManager)
{
	if (object != nullptr)
		object->addChild (child.object, index, undoManager);
}

void ValueTree::removeChild (const int childIndex, UndoManager* const undoManager)
{
	if (object != nullptr)
		object->removeChild (childIndex, undoManager);
}

void ValueTree::removeChild (const ValueTree& child, UndoManager* const undoManager)
{
	if (object != nullptr)
		object->removeChild (object->children.indexOf (child.object), undoManager);
}

void ValueTree::removeAllChildren (UndoManager* const undoManager)
{
	if (object != nullptr)
		object->removeAllChildren (undoManager);
}

void ValueTree::moveChild (int currentIndex, int newIndex, UndoManager* undoManager)
{
	if (object != nullptr)
		object->moveChild (currentIndex, newIndex, undoManager);
}

void ValueTree::createListOfChildren (OwnedArray<ValueTree>& list) const
{
	jassert (object != nullptr);

	for (int i = 0; i < object->children.size(); ++i)
		list.add (new ValueTree (object->children.getObjectPointerUnchecked(i)));
}

void ValueTree::reorderChildren (const OwnedArray<ValueTree>& newOrder, UndoManager* undoManager)
{
	jassert (object != nullptr);
	object->reorderChildren (newOrder, undoManager);
}

void ValueTree::addListener (Listener* listener)
{
	if (listener != nullptr)
	{
		if (listeners.size() == 0 && object != nullptr)
			object->valueTreesWithListeners.add (this);

		listeners.add (listener);
	}
}

void ValueTree::removeListener (Listener* listener)
{
	listeners.remove (listener);

	if (listeners.size() == 0 && object != nullptr)
		object->valueTreesWithListeners.removeValue (this);
}

void ValueTree::sendPropertyChangeMessage (const Identifier& property)
{
	if (object != nullptr)
		object->sendPropertyChangeMessage (property);
}

XmlElement* ValueTree::createXml() const
{
	return object != nullptr ? object->createXml() : nullptr;
}

ValueTree ValueTree::fromXml (const XmlElement& xml)
{
	ValueTree v (xml.getTagName());
	v.object->properties.setFromXmlAttributes (xml);

	forEachXmlChildElement (xml, e)
		v.addChild (fromXml (*e), -1, nullptr);

	return v;
}

void ValueTree::writeToStream (OutputStream& output)
{
	SharedObject::writeObjectToStream (output, object);
}

ValueTree ValueTree::readFromStream (InputStream& input)
{
	const String type (input.readString());

	if (type.isEmpty())
		return ValueTree::invalid;

	ValueTree v (type);

	const int numProps = input.readCompressedInt();

	if (numProps < 0)
	{
		jassertfalse;  // trying to read corrupted data!
		return v;
	}

	int i;
	for (i = 0; i < numProps; ++i)
	{
		const String name (input.readString());
		jassert (name.isNotEmpty());
		const var value (var::readFromStream (input));
		v.object->properties.set (name, value);
	}

	const int numChildren = input.readCompressedInt();

	for (i = 0; i < numChildren; ++i)
	{
		ValueTree child (readFromStream (input));

		v.object->children.add (child.object);
		child.object->parent = v.object;
	}

	return v;
}

ValueTree ValueTree::readFromData (const void* const data, const size_t numBytes)
{
	MemoryInputStream in (data, numBytes, false);
	return readFromStream (in);
}

ValueTree ValueTree::readFromGZIPData (const void* const data, const size_t numBytes)
{
	MemoryInputStream in (data, numBytes, false);
	GZIPDecompressorInputStream gzipStream (in);
	return readFromStream (gzipStream);
}

#if JUCE_UNIT_TESTS

class ValueTreeTests  : public UnitTest
{
public:
	ValueTreeTests() : UnitTest ("ValueTrees") {}

	static String createRandomIdentifier()
	{
		char buffer[50] = { 0 };
		const char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-:";
		Random r;

		for (int i = 1 + r.nextInt (numElementsInArray (buffer) - 2); --i >= 0;)
			buffer[i] = chars [r.nextInt (sizeof (chars) - 1)];

		return CharPointer_ASCII (buffer);
	}

	static String createRandomWideCharString()
	{
		juce_wchar buffer[50] = { 0 };
		Random r;

		for (int i = r.nextInt (numElementsInArray (buffer) - 1); --i >= 0;)
		{
			if (r.nextBool())
			{
				do
				{
					buffer[i] = (juce_wchar) (1 + r.nextInt (0x10ffff - 1));
				}
				while (! CharPointer_UTF16::canRepresent (buffer[i]));
			}
			else
				buffer[i] = (juce_wchar) (1 + r.nextInt (0x7e));
		}

		return CharPointer_UTF32 (buffer);
	}

	static ValueTree createRandomTree (UndoManager* undoManager, int depth)
	{
		Random r;
		ValueTree v (createRandomIdentifier());

		for (int i = r.nextInt (10); --i >= 0;)
		{
			switch (r.nextInt (5))
			{
				case 0: v.setProperty (createRandomIdentifier(), createRandomWideCharString(), undoManager); break;
				case 1: v.setProperty (createRandomIdentifier(), r.nextInt(), undoManager); break;
				case 2: if (depth < 5) v.addChild (createRandomTree (undoManager, depth + 1), r.nextInt (v.getNumChildren() + 1), undoManager); break;
				case 3: v.setProperty (createRandomIdentifier(), r.nextBool(), undoManager); break;
				case 4: v.setProperty (createRandomIdentifier(), r.nextDouble(), undoManager); break;
				default: break;
			}
		}

		return v;
	}

	void runTest()
	{
		beginTest ("ValueTree");

		for (int i = 10; --i >= 0;)
		{
			MemoryOutputStream mo;
			ValueTree v1 (createRandomTree (nullptr, 0));
			v1.writeToStream (mo);

			MemoryInputStream mi (mo.getData(), mo.getDataSize(), false);
			ValueTree v2 = ValueTree::readFromStream (mi);
			expect (v1.isEquivalentTo (v2));

			ScopedPointer <XmlElement> xml1 (v1.createXml());
			ScopedPointer <XmlElement> xml2 (v2.createCopy().createXml());
			expect (xml1->isEquivalentTo (xml2, false));

			ValueTree v4 = v2.createCopy();
			expect (v1.isEquivalentTo (v4));
		}
	}
};

static ValueTreeTests valueTreeTests;

#endif

/*** End of inlined file: juce_ValueTree.cpp ***/


/*** Start of inlined file: juce_UndoManager.cpp ***/
UndoManager::UndoManager (const int maxNumberOfUnitsToKeep,
						  const int minimumTransactions)
   : totalUnitsStored (0),
	 nextIndex (0),
	 newTransaction (true),
	 reentrancyCheck (false)
{
	setMaxNumberOfStoredUnits (maxNumberOfUnitsToKeep,
							   minimumTransactions);
}

UndoManager::~UndoManager()
{
	clearUndoHistory();
}

void UndoManager::clearUndoHistory()
{
	transactions.clear();
	transactionNames.clear();
	totalUnitsStored = 0;
	nextIndex = 0;
	sendChangeMessage();
}

int UndoManager::getNumberOfUnitsTakenUpByStoredCommands() const
{
	return totalUnitsStored;
}

void UndoManager::setMaxNumberOfStoredUnits (const int maxNumberOfUnitsToKeep,
											 const int minimumTransactions)
{
	maxNumUnitsToKeep          = jmax (1, maxNumberOfUnitsToKeep);
	minimumTransactionsToKeep  = jmax (1, minimumTransactions);
}

bool UndoManager::perform (UndoableAction* const command_, const String& actionName)
{
	if (command_ != nullptr)
	{
		ScopedPointer<UndoableAction> command (command_);

		if (actionName.isNotEmpty())
			currentTransactionName = actionName;

		if (reentrancyCheck)
		{
			jassertfalse;    // don't call perform() recursively from the UndoableAction::perform() or
							 // undo() methods, or else these actions won't actually get done.

			return false;
		}
		else if (command->perform())
		{
			OwnedArray<UndoableAction>* commandSet = transactions [nextIndex - 1];

			if (commandSet != nullptr && ! newTransaction)
			{
				UndoableAction* lastAction = commandSet->getLast();

				if (lastAction != nullptr)
				{
					UndoableAction* coalescedAction = lastAction->createCoalescedAction (command);

					if (coalescedAction != nullptr)
					{
						command = coalescedAction;
						totalUnitsStored -= lastAction->getSizeInUnits();
						commandSet->removeLast();
					}
				}
			}
			else
			{
				commandSet = new OwnedArray<UndoableAction>();
				transactions.insert (nextIndex, commandSet);
				transactionNames.insert (nextIndex, currentTransactionName);
				++nextIndex;
			}

			totalUnitsStored += command->getSizeInUnits();
			commandSet->add (command.release());
			newTransaction = false;

			while (nextIndex < transactions.size())
			{
				const OwnedArray <UndoableAction>* const lastSet = transactions.getLast();

				for (int i = lastSet->size(); --i >= 0;)
					totalUnitsStored -= lastSet->getUnchecked (i)->getSizeInUnits();

				transactions.removeLast();
				transactionNames.remove (transactionNames.size() - 1);
			}

			while (nextIndex > 0
				   && totalUnitsStored > maxNumUnitsToKeep
				   && transactions.size() > minimumTransactionsToKeep)
			{
				const OwnedArray <UndoableAction>* const firstSet = transactions.getFirst();

				for (int i = firstSet->size(); --i >= 0;)
					totalUnitsStored -= firstSet->getUnchecked (i)->getSizeInUnits();

				jassert (totalUnitsStored >= 0); // something fishy going on if this fails!

				transactions.remove (0);
				transactionNames.remove (0);
				--nextIndex;
			}

			sendChangeMessage();

			return true;
		}
	}

	return false;
}

void UndoManager::beginNewTransaction (const String& actionName)
{
	newTransaction = true;
	currentTransactionName = actionName;
}

void UndoManager::setCurrentTransactionName (const String& newName)
{
	currentTransactionName = newName;
}

bool UndoManager::canUndo() const
{
	return nextIndex > 0;
}

bool UndoManager::canRedo() const
{
	return nextIndex < transactions.size();
}

String UndoManager::getUndoDescription() const
{
	return transactionNames [nextIndex - 1];
}

String UndoManager::getRedoDescription() const
{
	return transactionNames [nextIndex];
}

bool UndoManager::undo()
{
	const OwnedArray<UndoableAction>* const commandSet = transactions [nextIndex - 1];

	if (commandSet == nullptr)
		return false;

	bool failed = false;

	{
		const ScopedValueSetter<bool> setter (reentrancyCheck, true);

		for (int i = commandSet->size(); --i >= 0;)
		{
			if (! commandSet->getUnchecked(i)->undo())
			{
				jassertfalse;
				failed = true;
				break;
			}
		}
	}

	if (failed)
		clearUndoHistory();
	else
		--nextIndex;

	beginNewTransaction();

	sendChangeMessage();
	return true;
}

bool UndoManager::redo()
{
	const OwnedArray<UndoableAction>* const commandSet = transactions [nextIndex];

	if (commandSet == nullptr)
		return false;

	bool failed = false;

	{
		const ScopedValueSetter<bool> setter (reentrancyCheck, true);

		for (int i = 0; i < commandSet->size(); ++i)
		{
			if (! commandSet->getUnchecked(i)->perform())
			{
				jassertfalse;
				failed = true;
				break;
			}
		}
	}

	if (failed)
		clearUndoHistory();
	else
		++nextIndex;

	beginNewTransaction();

	sendChangeMessage();
	return true;
}

bool UndoManager::undoCurrentTransactionOnly()
{
	return newTransaction ? false : undo();
}

void UndoManager::getActionsInCurrentTransaction (Array <const UndoableAction*>& actionsFound) const
{
	const OwnedArray <UndoableAction>* const commandSet = transactions [nextIndex - 1];

	if (commandSet != nullptr && ! newTransaction)
	{
		for (int i = 0; i < commandSet->size(); ++i)
			actionsFound.add (commandSet->getUnchecked(i));
	}
}

int UndoManager::getNumActionsInCurrentTransaction() const
{
	const OwnedArray <UndoableAction>* const commandSet = transactions [nextIndex - 1];

	if (commandSet != nullptr && ! newTransaction)
		return commandSet->size();

	return 0;
}

/*** End of inlined file: juce_UndoManager.cpp ***/


/*** Start of inlined file: juce_ApplicationProperties.cpp ***/
ApplicationProperties::ApplicationProperties()
	: commonSettingsAreReadOnly (0)
{
}

ApplicationProperties::~ApplicationProperties()
{
	closeFiles();
}

void ApplicationProperties::setStorageParameters (const PropertiesFile::Options& newOptions)
{
	options = newOptions;
}

void ApplicationProperties::openFiles()
{
	// You need to call setStorageParameters() before trying to get hold of the properties!
	jassert (options.applicationName.isNotEmpty());

	if (options.applicationName.isNotEmpty())
	{
		PropertiesFile::Options o (options);

		if (userProps == nullptr)
		{
			o.commonToAllUsers = false;
			userProps = new PropertiesFile (o);
		}

		if (commonProps == nullptr)
		{
			o.commonToAllUsers = true;
			commonProps = new PropertiesFile (o);
		}

		userProps->setFallbackPropertySet (commonProps);
	}
}

PropertiesFile* ApplicationProperties::getUserSettings()
{
	if (userProps == nullptr)
		openFiles();

	return userProps;
}

PropertiesFile* ApplicationProperties::getCommonSettings (const bool returnUserPropsIfReadOnly)
{
	if (commonProps == nullptr)
		openFiles();

	if (returnUserPropsIfReadOnly)
	{
		if (commonSettingsAreReadOnly == 0)
			commonSettingsAreReadOnly = commonProps->save() ? -1 : 1;

		if (commonSettingsAreReadOnly > 0)
			return userProps;
	}

	return commonProps;
}

bool ApplicationProperties::saveIfNeeded()
{
	return (userProps == nullptr || userProps->saveIfNeeded())
		 && (commonProps == nullptr || commonProps->saveIfNeeded());
}

void ApplicationProperties::closeFiles()
{
	userProps = nullptr;
	commonProps = nullptr;
}

/*** End of inlined file: juce_ApplicationProperties.cpp ***/


/*** Start of inlined file: juce_PropertiesFile.cpp ***/
namespace PropertyFileConstants
{
	static const int magicNumber            = (int) ByteOrder::littleEndianInt ("PROP");
	static const int magicNumberCompressed  = (int) ByteOrder::littleEndianInt ("CPRP");

	static const char* const fileTag        = "PROPERTIES";
	static const char* const valueTag       = "VALUE";
	static const char* const nameAttribute  = "name";
	static const char* const valueAttribute = "val";
}

PropertiesFile::Options::Options()
	: commonToAllUsers (false),
	  ignoreCaseOfKeyNames (false),
	  millisecondsBeforeSaving (3000),
	  storageFormat (PropertiesFile::storeAsXML),
	  processLock (nullptr)
{
}

File PropertiesFile::Options::getDefaultFile() const
{
	// mustn't have illegal characters in this name..
	jassert (applicationName == File::createLegalFileName (applicationName));

  #if JUCE_MAC || JUCE_IOS
	File dir (commonToAllUsers ?  "/Library/"
							   : "~/Library/");

	if (osxLibrarySubFolder != "Preferences" && osxLibrarySubFolder != "Application Support")
	{
		/* The PropertiesFile class always used to put its settings files in "Library/Preferences", but Apple
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
		jassertfalse;

		dir = dir.getChildFile ("Application Support");
	}
	else
	{
		dir = dir.getChildFile (osxLibrarySubFolder);
	}

	if (folderName.isNotEmpty())
		dir = dir.getChildFile (folderName);

  #elif JUCE_LINUX || JUCE_ANDROID
	const File dir ((commonToAllUsers ? "/var/" : "~/")
						+ (folderName.isNotEmpty() ? folderName
												   : ("." + applicationName)));

  #elif JUCE_WINDOWS
	File dir (File::getSpecialLocation (commonToAllUsers ? File::commonApplicationDataDirectory
														 : File::userApplicationDataDirectory));

	if (dir == File::nonexistent)
		return File::nonexistent;

	dir = dir.getChildFile (folderName.isNotEmpty() ? folderName
													: applicationName);
  #endif

	return dir.getChildFile (applicationName)
			  .withFileExtension (filenameSuffix);
}

PropertiesFile::PropertiesFile (const File& file_, const Options& options_)
	: PropertySet (options_.ignoreCaseOfKeyNames),
	  file (file_), options (options_),
	  loadedOk (false), needsWriting (false)
{
	initialise();
}

PropertiesFile::PropertiesFile (const Options& options_)
	: PropertySet (options_.ignoreCaseOfKeyNames),
	  file (options_.getDefaultFile()), options (options_),
	  loadedOk (false), needsWriting (false)
{
	initialise();
}

void PropertiesFile::initialise()
{
	// You need to correctly specify just one storage format for the file
	ProcessScopedLock pl (createProcessLock());

	if (pl != nullptr && ! pl->isLocked())
		return; // locking failure..

	ScopedPointer<InputStream> fileStream (file.createInputStream());

	if (fileStream != nullptr)
	{
		int magicNumber = fileStream->readInt();

		if (magicNumber == PropertyFileConstants::magicNumberCompressed)
		{
			fileStream = new GZIPDecompressorInputStream (new SubregionStream (fileStream.release(), 4, -1, true), true);
			magicNumber = PropertyFileConstants::magicNumber;
		}

		if (magicNumber == PropertyFileConstants::magicNumber)
		{
			loadedOk = true;
			BufferedInputStream in (fileStream.release(), 2048, true);

			int numValues = in.readInt();

			while (--numValues >= 0 && ! in.isExhausted())
			{
				const String key (in.readString());
				const String value (in.readString());

				jassert (key.isNotEmpty());
				if (key.isNotEmpty())
					getAllProperties().set (key, value);
			}
		}
		else
		{
			// Not a binary props file - let's see if it's XML..
			fileStream = nullptr;

			XmlDocument parser (file);
			ScopedPointer<XmlElement> doc (parser.getDocumentElement (true));

			if (doc != nullptr && doc->hasTagName (PropertyFileConstants::fileTag))
			{
				doc = parser.getDocumentElement();

				if (doc != nullptr)
				{
					loadedOk = true;

					forEachXmlChildElementWithTagName (*doc, e, PropertyFileConstants::valueTag)
					{
						const String name (e->getStringAttribute (PropertyFileConstants::nameAttribute));

						if (name.isNotEmpty())
						{
							getAllProperties().set (name,
													e->getFirstChildElement() != nullptr
														? e->getFirstChildElement()->createDocument (String::empty, true)
														: e->getStringAttribute (PropertyFileConstants::valueAttribute));
						}
					}
				}
				else
				{
					// must be a pretty broken XML file we're trying to parse here,
					// or a sign that this object needs an InterProcessLock,
					// or just a failure reading the file.  This last reason is why
					// we don't jassertfalse here.
				}
			}
		}
	}
	else
	{
		loadedOk = ! file.exists();
	}
}

PropertiesFile::~PropertiesFile()
{
	if (! saveIfNeeded())
		jassertfalse;
}

InterProcessLock::ScopedLockType* PropertiesFile::createProcessLock() const
{
	return options.processLock != nullptr ? new InterProcessLock::ScopedLockType (*options.processLock) : nullptr;
}

bool PropertiesFile::saveIfNeeded()
{
	const ScopedLock sl (getLock());
	return (! needsWriting) || save();
}

bool PropertiesFile::needsToBeSaved() const
{
	const ScopedLock sl (getLock());
	return needsWriting;
}

void PropertiesFile::setNeedsToBeSaved (const bool needsToBeSaved_)
{
	const ScopedLock sl (getLock());
	needsWriting = needsToBeSaved_;
}

bool PropertiesFile::save()
{
	const ScopedLock sl (getLock());

	stopTimer();

	if (file == File::nonexistent
		 || file.isDirectory()
		 || ! file.getParentDirectory().createDirectory())
		return false;

	if (options.storageFormat == storeAsXML)
	{
		XmlElement doc (PropertyFileConstants::fileTag);

		for (int i = 0; i < getAllProperties().size(); ++i)
		{
			XmlElement* const e = doc.createNewChildElement (PropertyFileConstants::valueTag);
			e->setAttribute (PropertyFileConstants::nameAttribute, getAllProperties().getAllKeys() [i]);

			// if the value seems to contain xml, store it as such..
			XmlElement* const childElement = XmlDocument::parse (getAllProperties().getAllValues() [i]);

			if (childElement != nullptr)
				e->addChildElement (childElement);
			else
				e->setAttribute (PropertyFileConstants::valueAttribute,
								 getAllProperties().getAllValues() [i]);
		}

		ProcessScopedLock pl (createProcessLock());

		if (pl != nullptr && ! pl->isLocked())
			return false; // locking failure..

		if (doc.writeToFile (file, String::empty))
		{
			needsWriting = false;
			return true;
		}
	}
	else
	{
		ProcessScopedLock pl (createProcessLock());

		if (pl != nullptr && ! pl->isLocked())
			return false; // locking failure..

		TemporaryFile tempFile (file);
		ScopedPointer <OutputStream> out (tempFile.getFile().createOutputStream());

		if (out != nullptr)
		{
			if (options.storageFormat == storeAsCompressedBinary)
			{
				out->writeInt (PropertyFileConstants::magicNumberCompressed);
				out->flush();

				out = new GZIPCompressorOutputStream (out.release(), 9, true);
			}
			else
			{
				// have you set up the storage option flags correctly?
				jassert (options.storageFormat == storeAsBinary);

				out->writeInt (PropertyFileConstants::magicNumber);
			}

			const int numProperties = getAllProperties().size();

			out->writeInt (numProperties);

			for (int i = 0; i < numProperties; ++i)
			{
				out->writeString (getAllProperties().getAllKeys() [i]);
				out->writeString (getAllProperties().getAllValues() [i]);
			}

			out = nullptr;

			if (tempFile.overwriteTargetFileWithTemporary())
			{
				needsWriting = false;
				return true;
			}
		}
	}

	return false;
}

void PropertiesFile::timerCallback()
{
	saveIfNeeded();
}

void PropertiesFile::propertyChanged()
{
	sendChangeMessage();

	needsWriting = true;

	if (options.millisecondsBeforeSaving > 0)
		startTimer (options.millisecondsBeforeSaving);
	else if (options.millisecondsBeforeSaving == 0)
		saveIfNeeded();
}

/*** End of inlined file: juce_PropertiesFile.cpp ***/

// END_AUTOINCLUDE

}

/*** End of inlined file: juce_data_structures.cpp ***/


#endif

