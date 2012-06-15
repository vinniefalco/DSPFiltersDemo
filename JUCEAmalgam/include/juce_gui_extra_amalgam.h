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


/*** Start of inlined file: juce_gui_extra.h ***/
#ifndef __JUCE_GUI_EXTRA_JUCEHEADER__
#define __JUCE_GUI_EXTRA_JUCEHEADER__

#include "juce_gui_basics_amalgam.h"

/** Config: JUCE_WEB_BROWSER
	This lets you disable the WebBrowserComponent class (Mac and Windows).
	If you're not using any embedded web-pages, turning this off may reduce your code size.
*/
#ifndef JUCE_WEB_BROWSER
 #define JUCE_WEB_BROWSER 1
#endif

namespace juce
{

// START_AUTOINCLUDE documents, code_editor, embedding, lookandfeel, misc
#ifndef __JUCE_FILEBASEDDOCUMENT_JUCEHEADER__

/*** Start of inlined file: juce_FileBasedDocument.h ***/
#ifndef __JUCE_FILEBASEDDOCUMENT_JUCEHEADER__
#define __JUCE_FILEBASEDDOCUMENT_JUCEHEADER__

/**
	A class to take care of the logic involved with the loading/saving of some kind
	of document.

	There's quite a lot of tedious logic involved in writing all the load/save/save-as
	functions you need for documents that get saved to a file, so this class attempts
	to abstract most of the boring stuff.

	Your subclass should just implement all the pure virtual methods, and you can
	then use the higher-level public methods to do the load/save dialogs, to warn the user
	about overwriting files, etc.

	The document object keeps track of whether it has changed since it was last saved or
	loaded, so when you change something, call its changed() method. This will set a
	flag so it knows it needs saving, and will also broadcast a change message using the
	ChangeBroadcaster base class.

	@see ChangeBroadcaster
*/
class JUCE_API FileBasedDocument  : public ChangeBroadcaster
{
public:
	/** Creates a FileBasedDocument.

		@param fileExtension            the extension to use when loading/saving files, e.g. ".doc"
		@param fileWildCard             the wildcard to use in file dialogs, e.g. "*.doc"
		@param openFileDialogTitle      the title to show on an open-file dialog, e.g. "Choose a file to open.."
		@param saveFileDialogTitle      the title to show on an save-file dialog, e.g. "Choose a file to save as.."
	*/
	FileBasedDocument (const String& fileExtension,
					   const String& fileWildCard,
					   const String& openFileDialogTitle,
					   const String& saveFileDialogTitle);

	/** Destructor. */
	virtual ~FileBasedDocument();

	/** Returns true if the changed() method has been called since the file was
		last saved or loaded.

		@see resetChangedFlag, changed
	*/
	bool hasChangedSinceSaved() const                           { return changedSinceSave; }

	/** Called to indicate that the document has changed and needs saving.

		This method will also trigger a change message to be sent out using the
		ChangeBroadcaster base class.

		After calling the method, the hasChangedSinceSaved() method will return true, until
		it is reset either by saving to a file or using the resetChangedFlag() method.

		@see hasChangedSinceSaved, resetChangedFlag
	*/
	virtual void changed();

	/** Sets the state of the 'changed' flag.

		The 'changed' flag is set to true when the changed() method is called - use this method
		to reset it or to set it without also broadcasting a change message.

		@see changed, hasChangedSinceSaved
	*/
	void setChangedFlag (bool hasChanged);

	/** Tries to open a file.

		If the file opens correctly, the document's file (see the getFile() method) is set
		to this new one; if it fails, the document's file is left unchanged, and optionally
		a message box is shown telling the user there was an error.

		@returns true if the new file loaded successfully
		@see loadDocument, loadFromUserSpecifiedFile
	*/
	bool loadFrom (const File& fileToLoadFrom,
				   bool showMessageOnFailure);

	/** Asks the user for a file and tries to load it.

		This will pop up a dialog box using the title, file extension and
		wildcard specified in the document's constructor, and asks the user
		for a file. If they pick one, the loadFrom() method is used to
		try to load it, optionally showing a message if it fails.

		@returns    true if a file was loaded; false if the user cancelled or if they
					picked a file which failed to load correctly
		@see loadFrom
	*/
	bool loadFromUserSpecifiedFile (bool showMessageOnFailure);

	/** A set of possible outcomes of one of the save() methods
	*/
	enum SaveResult
	{
		savedOk = 0,            /**< indicates that a file was saved successfully. */
		userCancelledSave,      /**< indicates that the user aborted the save operation. */
		failedToWriteToFile     /**< indicates that it tried to write to a file but this failed. */
	};

	/** Tries to save the document to the last file it was saved or loaded from.

		This will always try to write to the file, even if the document isn't flagged as
		having changed.

		@param askUserForFileIfNotSpecified     if there's no file currently specified and this is
												true, it will prompt the user to pick a file, as if
												saveAsInteractive() was called.
		@param showMessageOnFailure             if true it will show a warning message when if the
												save operation fails
		@see saveIfNeededAndUserAgrees, saveAs, saveAsInteractive
	*/
	SaveResult save (bool askUserForFileIfNotSpecified,
					 bool showMessageOnFailure);

	/** If the file needs saving, it'll ask the user if that's what they want to do, and save
		it if they say yes.

		If you've got a document open and want to close it (e.g. to quit the app), this is the
		method to call.

		If the document doesn't need saving it'll return the value savedOk so
		you can go ahead and delete the document.

		If it does need saving it'll prompt the user, and if they say "discard changes" it'll
		return savedOk, so again, you can safely delete the document.

		If the user clicks "cancel", it'll return userCancelledSave, so if you can abort the
		close-document operation.

		And if they click "save changes", it'll try to save and either return savedOk, or
		failedToWriteToFile if there was a problem.

		@see save, saveAs, saveAsInteractive
	*/
	SaveResult saveIfNeededAndUserAgrees();

	/** Tries to save the document to a specified file.

		If this succeeds, it'll also change the document's internal file (as returned by
		the getFile() method). If it fails, the file will be left unchanged.

		@param newFile                      the file to try to write to
		@param warnAboutOverwritingExistingFiles    if true and the file exists, it'll ask
											the user first if they want to overwrite it
		@param askUserForFileIfNotSpecified if the file is non-existent and this is true, it'll
											use the saveAsInteractive() method to ask the user for a
											filename
		@param showMessageOnFailure         if true and the write operation fails, it'll show
											a message box to warn the user
		@see saveIfNeededAndUserAgrees, save, saveAsInteractive
	*/
	SaveResult saveAs (const File& newFile,
					   bool warnAboutOverwritingExistingFiles,
					   bool askUserForFileIfNotSpecified,
					   bool showMessageOnFailure);

	/** Prompts the user for a filename and tries to save to it.

		This will pop up a dialog box using the title, file extension and
		wildcard specified in the document's constructor, and asks the user
		for a file. If they pick one, the saveAs() method is used to try to save
		to this file.

		@param warnAboutOverwritingExistingFiles    if true and the file exists, it'll ask
											the user first if they want to overwrite it
		@see saveIfNeededAndUserAgrees, save, saveAs
	*/
	SaveResult saveAsInteractive (bool warnAboutOverwritingExistingFiles);

	/** Returns the file that this document was last successfully saved or loaded from.

		When the document object is created, this will be set to File::nonexistent.

		It is changed when one of the load or save methods is used, or when setFile()
		is used to explicitly set it.
	*/
	const File& getFile() const                             { return documentFile; }

	/** Sets the file that this document thinks it was loaded from.

		This won't actually load anything - it just changes the file stored internally.

		@see getFile
	*/
	void setFile (const File& newFile);

protected:

	/** Overload this to return the title of the document.

		This is used in message boxes, filenames and file choosers, so it should be
		something sensible.
	*/
	virtual const String getDocumentTitle() = 0;

	/** This method should try to load your document from the given file.

		If it fails, it should return an error message. If it succeeds, it should return
		an empty string.
	*/
	virtual const String loadDocument (const File& file) = 0;

	/** This method should try to write your document to the given file.

		If it fails, it should return an error message. If it succeeds, it should return
		an empty string.
	*/
	virtual const String saveDocument (const File& file) = 0;

	/** This is used for dialog boxes to make them open at the last folder you
		were using.

		getLastDocumentOpened() and setLastDocumentOpened() are used to store
		the last document that was used - you might want to store this value
		in a static variable, or even in your application's properties. It should
		be a global setting rather than a property of this object.

		This method works very well in conjunction with a RecentlyOpenedFilesList
		object to manage your recent-files list.

		As a default value, it's ok to return File::nonexistent, and the document
		object will use a sensible one instead.

		@see RecentlyOpenedFilesList
	*/
	virtual const File getLastDocumentOpened() = 0;

	/** This is used for dialog boxes to make them open at the last folder you
		were using.

		getLastDocumentOpened() and setLastDocumentOpened() are used to store
		the last document that was used - you might want to store this value
		in a static variable, or even in your application's properties. It should
		be a global setting rather than a property of this object.

		This method works very well in conjunction with a RecentlyOpenedFilesList
		object to manage your recent-files list.

		@see RecentlyOpenedFilesList
	*/
	virtual void setLastDocumentOpened (const File& file) = 0;

   #if JUCE_MODAL_LOOPS_PERMITTED
	/** This is called by saveAsInteractive() to allow you to optionally customise the
		filename that the user is presented with in the save dialog.
		The defaultFile parameter is an initial suggestion based on what the class knows
		about the current document - you can return a variation on this file with a different
		extension, etc, or just return something completely different.
	*/
	virtual File getSuggestedSaveAsFile (const File& defaultFile);
   #endif

private:

	File documentFile;
	bool changedSinceSave;
	String fileExtension, fileWildcard, openFileDialogTitle, saveFileDialogTitle;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileBasedDocument);
};

#endif   // __JUCE_FILEBASEDDOCUMENT_JUCEHEADER__

/*** End of inlined file: juce_FileBasedDocument.h ***/


#endif
#ifndef __JUCE_CODEDOCUMENT_JUCEHEADER__

/*** Start of inlined file: juce_CodeDocument.h ***/
#ifndef __JUCE_CODEDOCUMENT_JUCEHEADER__
#define __JUCE_CODEDOCUMENT_JUCEHEADER__

class CodeDocumentLine;

/**
	A class for storing and manipulating a source code file.

	When using a CodeEditorComponent, it takes one of these as its source object.

	The CodeDocument stores its content as an array of lines, which makes it
	quick to insert and delete.

	@see CodeEditorComponent
*/
class JUCE_API  CodeDocument
{
public:
	/** Creates a new, empty document.
	*/
	CodeDocument();

	/** Destructor. */
	~CodeDocument();

	/** A position in a code document.

		Using this class you can find a position in a code document and quickly get its
		character position, line, and index. By calling setPositionMaintained (true), the
		position is automatically updated when text is inserted or deleted in the document,
		so that it maintains its original place in the text.
	*/
	class JUCE_API  Position
	{
	public:
		/** Creates an uninitialised postion.
			Don't attempt to call any methods on this until you've given it an owner document
			to refer to!
		*/
		Position() noexcept;

		/** Creates a position based on a line and index in a document.

			Note that this index is NOT the column number, it's the number of characters from the
			start of the line. The "column" number isn't quite the same, because if the line
			contains any tab characters, the relationship of the index to its visual column depends on
			the number of spaces per tab being used!

			Lines are numbered from zero, and if the line or index are beyond the bounds of the document,
			they will be adjusted to keep them within its limits.
		*/
		Position (const CodeDocument* ownerDocument,
				  int line, int indexInLine) noexcept;

		/** Creates a position based on a character index in a document.
			This position is placed at the specified number of characters from the start of the
			document. The line and column are auto-calculated.

			If the position is beyond the range of the document, it'll be adjusted to keep it
			inside.
		*/
		Position (const CodeDocument* ownerDocument,
				  int charactersFromStartOfDocument) noexcept;

		/** Creates a copy of another position.

			This will copy the position, but the new object will not be set to maintain its position,
			even if the source object was set to do so.
		*/
		Position (const Position& other) noexcept;

		/** Destructor. */
		~Position();

		Position& operator= (const Position& other);
		bool operator== (const Position& other) const noexcept;
		bool operator!= (const Position& other) const noexcept;

		/** Points this object at a new position within the document.

			If the position is beyond the range of the document, it'll be adjusted to keep it
			inside.
			@see getPosition, setLineAndIndex
		*/
		void setPosition (int charactersFromStartOfDocument);

		/** Returns the position as the number of characters from the start of the document.
			@see setPosition, getLineNumber, getIndexInLine
		*/
		int getPosition() const noexcept            { return characterPos; }

		/** Moves the position to a new line and index within the line.

			Note that the index is NOT the column at which the position appears in an editor.
			If the line contains any tab characters, the relationship of the index to its
			visual position depends on the number of spaces per tab being used!

			Lines are numbered from zero, and if the line or index are beyond the bounds of the document,
			they will be adjusted to keep them within its limits.
		*/
		void setLineAndIndex (int newLine, int newIndexInLine);

		/** Returns the line number of this position.
			The first line in the document is numbered zero, not one!
		*/
		int getLineNumber() const noexcept          { return line; }

		/** Returns the number of characters from the start of the line.

			Note that this value is NOT the column at which the position appears in an editor.
			If the line contains any tab characters, the relationship of the index to its
			visual position depends on the number of spaces per tab being used!
		*/
		int getIndexInLine() const noexcept         { return indexInLine; }

		/** Allows the position to be automatically updated when the document changes.

			If this is set to true, the positon will register with its document so that
			when the document has text inserted or deleted, this position will be automatically
			moved to keep it at the same position in the text.
		*/
		void setPositionMaintained (bool isMaintained);

		/** Moves the position forwards or backwards by the specified number of characters.
			@see movedBy
		*/
		void moveBy (int characterDelta);

		/** Returns a position which is the same as this one, moved by the specified number of
			characters.
			@see moveBy
		*/
		const Position movedBy (int characterDelta) const;

		/** Returns a position which is the same as this one, moved up or down by the specified
			number of lines.
			@see movedBy
		*/
		const Position movedByLines (int deltaLines) const;

		/** Returns the character in the document at this position.
			@see getLineText
		*/
		const juce_wchar getCharacter() const;

		/** Returns the line from the document that this position is within.
			@see getCharacter, getLineNumber
		*/
		String getLineText() const;

	private:
		CodeDocument* owner;
		int characterPos, line, indexInLine;
		bool positionMaintained;
	};

	/** Returns the full text of the document. */
	String getAllContent() const;

	/** Returns a section of the document's text. */
	String getTextBetween (const Position& start, const Position& end) const;

	/** Returns a line from the document. */
	String getLine (int lineIndex) const noexcept;

	/** Returns the number of characters in the document. */
	int getNumCharacters() const noexcept;

	/** Returns the number of lines in the document. */
	int getNumLines() const noexcept                    { return lines.size(); }

	/** Returns the number of characters in the longest line of the document. */
	int getMaximumLineLength() noexcept;

	/** Deletes a section of the text.

		This operation is undoable.
	*/
	void deleteSection (const Position& startPosition, const Position& endPosition);

	/** Inserts some text into the document at a given position.

		This operation is undoable.
	*/
	void insertText (const Position& position, const String& text);

	/** Clears the document and replaces it with some new text.

		This operation is undoable - if you're trying to completely reset the document, you
		might want to also call clearUndoHistory() and setSavePoint() after using this method.
	*/
	void replaceAllContent (const String& newContent);

	/** Replaces the editor's contents with the contents of a stream.
		This will also reset the undo history and save point marker.
	*/
	bool loadFromStream (InputStream& stream);

	/** Writes the editor's current contents to a stream. */
	bool writeToStream (OutputStream& stream);

	/** Returns the preferred new-line characters for the document.
		This will be either "\n", "\r\n", or (rarely) "\r".
		@see setNewLineCharacters
	*/
	String getNewLineCharacters() const noexcept          { return newLineChars; }

	/** Sets the new-line characters that the document should use.
		The string must be either "\n", "\r\n", or (rarely) "\r".
		@see getNewLineCharacters
	*/
	void setNewLineCharacters (const String& newLine) noexcept;

	/** Begins a new undo transaction.

		The document itself will not call this internally, so relies on whatever is using the
		document to periodically call this to break up the undo sequence into sensible chunks.
		@see UndoManager::beginNewTransaction
	*/
	void newTransaction();

	/** Undo the last operation.
		@see UndoManager::undo
	*/
	void undo();

	/** Redo the last operation.
		@see UndoManager::redo
	*/
	void redo();

	/** Clears the undo history.
		@see UndoManager::clearUndoHistory
	*/
	void clearUndoHistory();

	/** Returns the document's UndoManager */
	UndoManager& getUndoManager() noexcept              { return undoManager; }

	/** Makes a note that the document's current state matches the one that is saved.

		After this has been called, hasChangedSinceSavePoint() will return false until
		the document has been altered, and then it'll start returning true. If the document is
		altered, but then undone until it gets back to this state, hasChangedSinceSavePoint()
		will again return false.

		@see hasChangedSinceSavePoint
	*/
	void setSavePoint() noexcept;

	/** Returns true if the state of the document differs from the state it was in when
		setSavePoint() was last called.

		@see setSavePoint
	*/
	bool hasChangedSinceSavePoint() const noexcept;

	/** Searches for a word-break. */
	const Position findWordBreakAfter (const Position& position) const noexcept;

	/** Searches for a word-break. */
	const Position findWordBreakBefore (const Position& position) const noexcept;

	/** An object that receives callbacks from the CodeDocument when its text changes.
		@see CodeDocument::addListener, CodeDocument::removeListener
	*/
	class JUCE_API  Listener
	{
	public:
		Listener() {}
		virtual ~Listener() {}

		/** Called by a CodeDocument when it is altered.
		*/
		virtual void codeDocumentChanged (const Position& affectedTextStart,
										  const Position& affectedTextEnd) = 0;
	};

	/** Registers a listener object to receive callbacks when the document changes.
		If the listener is already registered, this method has no effect.
		@see removeListener
	*/
	void addListener (Listener* listener) noexcept;

	/** Deregisters a listener.
		@see addListener
	*/
	void removeListener (Listener* listener) noexcept;

	/** Iterates the text in a CodeDocument.

		This class lets you read characters from a CodeDocument. It's designed to be used
		by a SyntaxAnalyser object.

		@see CodeDocument, SyntaxAnalyser
	*/
	class JUCE_API  Iterator
	{
	public:
		Iterator (CodeDocument* document);
		Iterator (const Iterator& other);
		Iterator& operator= (const Iterator& other) noexcept;
		~Iterator() noexcept;

		/** Reads the next character and returns it.
			@see peekNextChar
		*/
		juce_wchar nextChar();

		/** Reads the next character without advancing the current position. */
		juce_wchar peekNextChar() const;

		/** Advances the position by one character. */
		void skip();

		/** Returns the position of the next character as its position within the
			whole document.
		*/
		int getPosition() const noexcept        { return position; }

		/** Skips over any whitespace characters until the next character is non-whitespace. */
		void skipWhitespace();

		/** Skips forward until the next character will be the first character on the next line */
		void skipToEndOfLine();

		/** Returns the line number of the next character. */
		int getLine() const noexcept            { return line; }

		/** Returns true if the iterator has reached the end of the document. */
		bool isEOF() const noexcept;

	private:
		CodeDocument* document;
		mutable String::CharPointerType charPointer;
		int line, position;
	};

private:

	friend class CodeDocumentInsertAction;
	friend class CodeDocumentDeleteAction;
	friend class Iterator;
	friend class Position;

	OwnedArray <CodeDocumentLine> lines;
	Array <Position*> positionsToMaintain;
	UndoManager undoManager;
	int currentActionIndex, indexOfSavedState;
	int maximumLineLength;
	ListenerList <Listener> listeners;
	String newLineChars;

	void sendListenerChangeMessage (int startLine, int endLine);

	void insert (const String& text, int insertPos, bool undoable);
	void remove (int startPos, int endPos, bool undoable);
	void checkLastLineStatus();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CodeDocument);
};

#endif   // __JUCE_CODEDOCUMENT_JUCEHEADER__

/*** End of inlined file: juce_CodeDocument.h ***/


#endif
#ifndef __JUCE_CODEEDITORCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_CodeEditorComponent.h ***/
#ifndef __JUCE_CODEEDITORCOMPONENT_JUCEHEADER__
#define __JUCE_CODEEDITORCOMPONENT_JUCEHEADER__


/*** Start of inlined file: juce_CodeTokeniser.h ***/
#ifndef __JUCE_CODETOKENISER_JUCEHEADER__
#define __JUCE_CODETOKENISER_JUCEHEADER__

/**
	A base class for tokenising code so that the syntax can be displayed in a
	code editor.

	@see CodeDocument, CodeEditorComponent
*/
class JUCE_API  CodeTokeniser
{
public:
	CodeTokeniser()                 {}
	virtual ~CodeTokeniser()        {}

	/** Reads the next token from the source and returns its token type.

		This must leave the source pointing to the first character in the
		next token.
	*/
	virtual int readNextToken (CodeDocument::Iterator& source) = 0;

	/** Returns a list of the names of the token types this analyser uses.

		The index in this list must match the token type numbers that are
		returned by readNextToken().
	*/
	virtual StringArray getTokenTypes() = 0;

	/** Returns a suggested syntax highlighting colour for a specified
		token type.
	*/
	virtual Colour getDefaultColour (int tokenType) = 0;

private:

	JUCE_LEAK_DETECTOR (CodeTokeniser);
};

#endif   // __JUCE_CODETOKENISER_JUCEHEADER__

/*** End of inlined file: juce_CodeTokeniser.h ***/

/**
	A text editor component designed specifically for source code.

	This is designed to handle syntax highlighting and fast editing of very large
	files.
*/
class JUCE_API  CodeEditorComponent   : public Component,
										public TextInputTarget,
										public Timer,
										public ScrollBar::Listener,
										public CodeDocument::Listener,
										public AsyncUpdater
{
public:

	/** Creates an editor for a document.

		The tokeniser object is optional - pass 0 to disable syntax highlighting.
		The object that you pass in is not owned or deleted by the editor - you must
		make sure that it doesn't get deleted while this component is still using it.

		@see CodeDocument
	*/
	CodeEditorComponent (CodeDocument& document,
						 CodeTokeniser* codeTokeniser);

	/** Destructor. */
	~CodeEditorComponent();

	/** Returns the code document that this component is editing. */
	CodeDocument& getDocument() const noexcept          { return document; }

	/** Loads the given content into the document.
		This will completely reset the CodeDocument object, clear its undo history,
		and fill it with this text.
	*/
	void loadContent (const String& newContent);

	/** Returns the standard character width. */
	float getCharWidth() const noexcept                         { return charWidth; }

	/** Returns the height of a line of text, in pixels. */
	int getLineHeight() const noexcept                          { return lineHeight; }

	/** Returns the number of whole lines visible on the screen,
		This doesn't include a cut-off line that might be visible at the bottom if the
		component's height isn't an exact multiple of the line-height.
	*/
	int getNumLinesOnScreen() const noexcept                    { return linesOnScreen; }

	/** Returns the number of whole columns visible on the screen.
		This doesn't include any cut-off columns at the right-hand edge.
	*/
	int getNumColumnsOnScreen() const noexcept                  { return columnsOnScreen; }

	/** Returns the current caret position. */
	CodeDocument::Position getCaretPos() const                  { return caretPos; }

	/** Returns the position of the caret, relative to the editor's origin. */
	Rectangle<int> getCaretRectangle();

	/** Moves the caret.
		If selecting is true, the section of the document between the current
		caret position and the new one will become selected. If false, any currently
		selected region will be deselected.
	*/
	void moveCaretTo (const CodeDocument::Position& newPos, bool selecting);

	/** Returns the on-screen position of a character in the document.
		The rectangle returned is relative to this component's top-left origin.
	*/
	Rectangle<int> getCharacterBounds (const CodeDocument::Position& pos) const;

	/** Finds the character at a given on-screen position.
		The co-ordinates are relative to this component's top-left origin.
	*/
	CodeDocument::Position getPositionAt (int x, int y);

	bool moveCaretLeft (bool moveInWholeWordSteps, bool selecting);
	bool moveCaretRight (bool moveInWholeWordSteps, bool selecting);
	bool moveCaretUp (bool selecting);
	bool moveCaretDown (bool selecting);
	bool scrollDown();
	bool scrollUp();
	bool pageUp (bool selecting);
	bool pageDown (bool selecting);
	bool moveCaretToTop (bool selecting);
	bool moveCaretToStartOfLine (bool selecting);
	bool moveCaretToEnd (bool selecting);
	bool moveCaretToEndOfLine (bool selecting);
	bool deleteBackwards (bool moveInWholeWordSteps);
	bool deleteForwards (bool moveInWholeWordSteps);
	bool copyToClipboard();
	bool cutToClipboard();
	bool pasteFromClipboard();
	bool undo();
	bool redo();

	bool selectAll();
	void deselectAll();

	void scrollToLine (int newFirstLineOnScreen);
	void scrollBy (int deltaLines);
	void scrollToColumn (int newFirstColumnOnScreen);
	void scrollToKeepCaretOnScreen();

	void insertTextAtCaret (const String& textToInsert);
	void insertTabAtCaret();

	Range<int> getHighlightedRegion() const;
	void setHighlightedRegion (const Range<int>& newRange);
	String getTextInRange (const Range<int>& range) const;

	/** Changes the current tab settings.
		This lets you change the tab size and whether pressing the tab key inserts a
		tab character, or its equivalent number of spaces.
	*/
	void setTabSize (int numSpacesPerTab, bool insertSpacesInsteadOfTabCharacters);

	/** Returns the current number of spaces per tab.
		@see setTabSize
	*/
	int getTabSize() const noexcept                     { return spacesPerTab; }

	/** Returns true if the tab key will insert spaces instead of actual tab characters.
		@see setTabSize
	*/
	bool areSpacesInsertedForTabs() const               { return useSpacesForTabs; }

	/** Changes the font.
		Make sure you only use a fixed-width font, or this component will look pretty nasty!
	*/
	void setFont (const Font& newFont);

	/** Returns the font that the editor is using. */
	const Font& getFont() const noexcept                { return font; }

	/** Resets the syntax highlighting colours to the default ones provided by the
		code tokeniser.
		@see CodeTokeniser::getDefaultColour
	*/
	void resetToDefaultColours();

	/** Changes one of the syntax highlighting colours.
		The token type values are dependent on the tokeniser being used - use
		CodeTokeniser::getTokenTypes() to get a list of the token types.
		@see getColourForTokenType
	*/
	void setColourForTokenType (int tokenType, const Colour& colour);

	/** Returns one of the syntax highlighting colours.
		The token type values are dependent on the tokeniser being used - use
		CodeTokeniser::getTokenTypes() to get a list of the token types.
		@see setColourForTokenType
	*/
	Colour getColourForTokenType (int tokenType) const;

	/** A set of colour IDs to use to change the colour of various aspects of the editor.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId          = 0x1004500,  /**< A colour to use to fill the editor's background. */
		highlightColourId           = 0x1004502,  /**< The colour to use for the highlighted background under
													   selected text. */
		defaultTextColourId         = 0x1004503   /**< The colour to use for text when no syntax colouring is
													   enabled. */
	};

	/** Changes the size of the scrollbars. */
	void setScrollbarThickness (int thickness);

	/** Returns the thickness of the scrollbars. */
	int getScrollbarThickness() const noexcept          { return scrollbarThickness; }

	/** @internal */
	void resized();
	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	bool keyPressed (const KeyPress& key);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseUp (const MouseEvent& e);
	/** @internal */
	void mouseDoubleClick (const MouseEvent& e);
	/** @internal */
	void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
	/** @internal */
	void focusGained (FocusChangeType cause);
	/** @internal */
	void focusLost (FocusChangeType cause);
	/** @internal */
	void timerCallback();
	/** @internal */
	void scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart);
	/** @internal */
	void handleAsyncUpdate();
	/** @internal */
	void codeDocumentChanged (const CodeDocument::Position& affectedTextStart,
							  const CodeDocument::Position& affectedTextEnd);
	/** @internal */
	bool isTextInputActive() const;
	/** @internal */
	void setTemporaryUnderlining (const Array <Range<int> >&);

private:

	CodeDocument& document;

	Font font;
	int firstLineOnScreen, gutter, spacesPerTab;
	float charWidth;
	int lineHeight, linesOnScreen, columnsOnScreen;
	int scrollbarThickness, columnToTryToMaintain;
	bool useSpacesForTabs;
	double xOffset;

	CodeDocument::Position caretPos;
	CodeDocument::Position selectionStart, selectionEnd;

	ScopedPointer<CaretComponent> caret;
	ScrollBar verticalScrollBar, horizontalScrollBar;

	enum DragType
	{
		notDragging,
		draggingSelectionStart,
		draggingSelectionEnd
	};

	DragType dragType;

	CodeTokeniser* codeTokeniser;
	Array <Colour> coloursForTokenCategories;

	class CodeEditorLine;
	OwnedArray <CodeEditorLine> lines;
	void rebuildLineTokens();

	OwnedArray <CodeDocument::Iterator> cachedIterators;
	void clearCachedIterators (int firstLineToBeInvalid);
	void updateCachedIterators (int maxLineNum);
	void getIteratorForPosition (int position, CodeDocument::Iterator& result);
	void moveLineDelta (int delta, bool selecting);

	void updateCaretPosition();
	void updateScrollBars();
	void scrollToLineInternal (int line);
	void scrollToColumnInternal (double column);
	void newTransaction();
	void cut();

	int indexToColumn (int line, int index) const noexcept;
	int columnToIndex (int line, int column) const noexcept;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CodeEditorComponent);
};

#endif   // __JUCE_CODEEDITORCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_CodeEditorComponent.h ***/


#endif
#ifndef __JUCE_CODETOKENISER_JUCEHEADER__

#endif
#ifndef __JUCE_CPLUSPLUSCODETOKENISER_JUCEHEADER__

/*** Start of inlined file: juce_CPlusPlusCodeTokeniser.h ***/
#ifndef __JUCE_CPLUSPLUSCODETOKENISER_JUCEHEADER__
#define __JUCE_CPLUSPLUSCODETOKENISER_JUCEHEADER__

/**
	A simple lexical analyser for syntax colouring of C++ code.

	@see SyntaxAnalyser, CodeEditorComponent, CodeDocument
*/
class JUCE_API  CPlusPlusCodeTokeniser    : public CodeTokeniser
{
public:

	CPlusPlusCodeTokeniser();
	~CPlusPlusCodeTokeniser();

	enum TokenType
	{
		tokenType_error = 0,
		tokenType_comment,
		tokenType_builtInKeyword,
		tokenType_identifier,
		tokenType_integerLiteral,
		tokenType_floatLiteral,
		tokenType_stringLiteral,
		tokenType_operator,
		tokenType_bracket,
		tokenType_punctuation,
		tokenType_preprocessor
	};

	int readNextToken (CodeDocument::Iterator& source);
	StringArray getTokenTypes();
	Colour getDefaultColour (int tokenType);

	/** This is a handy method for checking whether a string is a c++ reserved keyword. */
	static bool isReservedKeyword (const String& token) noexcept;

private:

	JUCE_LEAK_DETECTOR (CPlusPlusCodeTokeniser);
};

#endif   // __JUCE_CPLUSPLUSCODETOKENISER_JUCEHEADER__

/*** End of inlined file: juce_CPlusPlusCodeTokeniser.h ***/


#endif
#ifndef __JUCE_ACTIVEXCONTROLCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_ActiveXControlComponent.h ***/
#ifndef __JUCE_ACTIVEXCONTROLCOMPONENT_JUCEHEADER__
#define __JUCE_ACTIVEXCONTROLCOMPONENT_JUCEHEADER__

#if JUCE_WINDOWS || DOXYGEN

/**
	A Windows-specific class that can create and embed an ActiveX control inside
	itself.

	To use it, create one of these, put it in place and make sure it's visible in a
	window, then use createControl() to instantiate an ActiveX control. The control
	will then be moved and resized to follow the movements of this component.

	Of course, since the control is a heavyweight window, it'll obliterate any
	juce components that may overlap this component, but that's life.
*/
class JUCE_API  ActiveXControlComponent   : public Component
{
public:

	/** Create an initially-empty container. */
	ActiveXControlComponent();

	/** Destructor. */
	~ActiveXControlComponent();

	/** Tries to create an ActiveX control and embed it in this peer.

		The peer controlIID is a pointer to an IID structure - it's treated
		as a void* because when including the Juce headers, you might not always
		have included windows.h first, in which case IID wouldn't be defined.

		e.g. @code
		const IID myIID = __uuidof (QTControl);
		myControlComp->createControl (&myIID);
		@endcode
	*/
	bool createControl (const void* controlIID);

	/** Deletes the ActiveX control, if one has been created.
	*/
	void deleteControl();

	/** Returns true if a control is currently in use. */
	bool isControlOpen() const noexcept                 { return control != nullptr; }

	/** Does a QueryInterface call on the embedded control object.

		This allows you to cast the control to whatever type of COM object you need.

		The iid parameter is a pointer to an IID structure - it's treated
		as a void* because when including the Juce headers, you might not always
		have included windows.h first, in which case IID wouldn't be defined, but
		you should just pass a pointer to an IID.

		e.g. @code
		const IID iid = __uuidof (IOleWindow);

		IOleWindow* oleWindow = (IOleWindow*) myControlComp->queryInterface (&iid);

		if (oleWindow != nullptr)
		{
			HWND hwnd;
			oleWindow->GetWindow (&hwnd);

			...

			oleWindow->Release();
		}
		@endcode
	*/
	void* queryInterface (const void* iid) const;

	/** Set this to false to stop mouse events being allowed through to the control.
	*/
	void setMouseEventsAllowed (bool eventsCanReachControl);

	/** Returns true if mouse events are allowed to get through to the control.
	*/
	bool areMouseEventsAllowed() const noexcept                 { return mouseEventsAllowed; }

	/** @internal */
	void paint (Graphics& g);

private:
	class Pimpl;
	friend class ScopedPointer <Pimpl>;
	ScopedPointer <Pimpl> control;
	bool mouseEventsAllowed;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ActiveXControlComponent);
};

#endif

#endif   // __JUCE_ACTIVEXCONTROLCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_ActiveXControlComponent.h ***/


#endif
#ifndef __JUCE_NSVIEWCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_NSViewComponent.h ***/
#ifndef __JUCE_NSVIEWCOMPONENT_JUCEHEADER__
#define __JUCE_NSVIEWCOMPONENT_JUCEHEADER__

#if JUCE_MAC || DOXYGEN

/**
	A Mac-specific class that can create and embed an NSView inside itself.

	To use it, create one of these, put it in place and make sure it's visible in a
	window, then use setView() to assign an NSView to it. The view will then be
	moved and resized to follow the movements of this component.

	Of course, since the view is a native object, it'll obliterate any
	juce components that may overlap this component, but that's life.
*/
class JUCE_API  NSViewComponent   : public Component
{
public:

	/** Create an initially-empty container. */
	NSViewComponent();

	/** Destructor. */
	~NSViewComponent();

	/** Assigns an NSView to this peer.

		The view will be retained and released by this component for as long as
		it is needed. To remove the current view, just call setView (nullptr).

		Note: a void* is used here to avoid including the cocoa headers as
		part of the juce.h, but the method expects an NSView*.
	*/
	void setView (void* nsView);

	/** Returns the current NSView.

		Note: a void* is returned here to avoid the needing to include the cocoa
		headers, so you should just cast the return value to an NSView*.
	*/
	void* getView() const;

	/** Resizes this component to fit the view that it contains. */
	void resizeToFitView();

	/** @internal */
	void paint (Graphics& g);

	/** @internal */
	static ReferenceCountedObject* attachViewToComponent (Component&, void*);

private:
	ReferenceCountedObjectPtr<ReferenceCountedObject> attachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NSViewComponent);
};

#endif
#endif   // __JUCE_NSVIEWCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_NSViewComponent.h ***/


#endif
#ifndef __JUCE_UIVIEWCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_UIViewComponent.h ***/
#ifndef __JUCE_UIVIEWCOMPONENT_JUCEHEADER__
#define __JUCE_UIVIEWCOMPONENT_JUCEHEADER__

#if JUCE_IOS || DOXYGEN

/**
	An iOS-specific class that can create and embed an UIView inside itself.

	To use it, create one of these, put it in place and make sure it's visible in a
	window, then use setView() to assign an NSView to it. The view will then be
	moved and resized to follow the movements of this component.

	Of course, since the view is a native object, it'll obliterate any
	juce components that may overlap this component, but that's life.
*/
class JUCE_API  UIViewComponent   : public Component
{
public:

	/** Create an initially-empty container. */
	UIViewComponent();

	/** Destructor. */
	~UIViewComponent();

	/** Assigns an UIView to this peer.

		The view will be retained and released by this component for as long as
		it is needed. To remove the current view, just call setView (nullptr).

		Note: a void* is used here to avoid including the cocoa headers as
		part of the juce.h, but the method expects an UIView*.
	*/
	void setView (void* uiView);

	/** Returns the current UIView.

		Note: a void* is returned here to avoid the needing to include the cocoa
		headers, so you should just cast the return value to an UIView*.
	*/
	void* getView() const;

	/** Resizes this component to fit the view that it contains. */
	void resizeToFitView();

	/** @internal */
	void paint (Graphics& g);

private:
	class Pimpl;
	friend class Pimpl;
	ScopedPointer<Pimpl> pimpl;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIViewComponent);
};

#endif
#endif   // __JUCE_UIVIEWCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_UIViewComponent.h ***/


#endif
#ifndef __JUCE_OLDSCHOOLLOOKANDFEEL_JUCEHEADER__

/*** Start of inlined file: juce_OldSchoolLookAndFeel.h ***/
#ifndef __JUCE_OLDSCHOOLLOOKANDFEEL_JUCEHEADER__
#define __JUCE_OLDSCHOOLLOOKANDFEEL_JUCEHEADER__

/**
	The original Juce look-and-feel.

*/
class JUCE_API  OldSchoolLookAndFeel    : public LookAndFeel
{
public:

	/** Creates the default JUCE look and feel. */
	OldSchoolLookAndFeel();

	/** Destructor. */
	virtual ~OldSchoolLookAndFeel();

	/** Draws the lozenge-shaped background for a standard button. */
	virtual void drawButtonBackground (Graphics& g,
									   Button& button,
									   const Colour& backgroundColour,
									   bool isMouseOverButton,
									   bool isButtonDown);

	/** Draws the contents of a standard ToggleButton. */
	virtual void drawToggleButton (Graphics& g,
								   ToggleButton& button,
								   bool isMouseOverButton,
								   bool isButtonDown);

	virtual void drawTickBox (Graphics& g,
							  Component& component,
							  float x, float y, float w, float h,
							  bool ticked,
							  bool isEnabled,
							  bool isMouseOverButton,
							  bool isButtonDown);

	virtual void drawProgressBar (Graphics& g, ProgressBar& progressBar,
								  int width, int height,
								  double progress, const String& textToShow);

	virtual void drawScrollbarButton (Graphics& g,
									  ScrollBar& scrollbar,
									  int width, int height,
									  int buttonDirection,
									  bool isScrollbarVertical,
									  bool isMouseOverButton,
									  bool isButtonDown);

	virtual void drawScrollbar (Graphics& g,
								ScrollBar& scrollbar,
								int x, int y,
								int width, int height,
								bool isScrollbarVertical,
								int thumbStartPosition,
								int thumbSize,
								bool isMouseOver,
								bool isMouseDown);

	virtual ImageEffectFilter* getScrollbarEffect();

	virtual void drawTextEditorOutline (Graphics& g,
										int width, int height,
										TextEditor& textEditor);

	/** Fills the background of a popup menu component. */
	virtual void drawPopupMenuBackground (Graphics& g, int width, int height);

	virtual void drawMenuBarBackground (Graphics& g, int width, int height,
										bool isMouseOverBar,
										MenuBarComponent& menuBar);

	virtual void drawComboBox (Graphics& g, int width, int height,
							   bool isButtonDown,
							   int buttonX, int buttonY,
							   int buttonW, int buttonH,
							   ComboBox& box);

	virtual const Font getComboBoxFont (ComboBox& box);

	virtual void drawLinearSlider (Graphics& g,
								   int x, int y,
								   int width, int height,
								   float sliderPos,
								   float minSliderPos,
								   float maxSliderPos,
								   const Slider::SliderStyle style,
								   Slider& slider);

	virtual int getSliderThumbRadius (Slider& slider);

	virtual Button* createSliderButton (bool isIncrement);

	virtual ImageEffectFilter* getSliderEffect();

	virtual void drawCornerResizer (Graphics& g,
									int w, int h,
									bool isMouseOver,
									bool isMouseDragging);

	virtual Button* createDocumentWindowButton (int buttonType);

	virtual void positionDocumentWindowButtons (DocumentWindow& window,
												int titleBarX, int titleBarY,
												int titleBarW, int titleBarH,
												Button* minimiseButton,
												Button* maximiseButton,
												Button* closeButton,
												bool positionTitleBarButtonsOnLeft);

private:

	DropShadowEffect scrollbarShadow;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OldSchoolLookAndFeel);
};

#endif   // __JUCE_OLDSCHOOLLOOKANDFEEL_JUCEHEADER__

/*** End of inlined file: juce_OldSchoolLookAndFeel.h ***/


#endif
#ifndef __JUCE_APPLEREMOTE_JUCEHEADER__

/*** Start of inlined file: juce_AppleRemote.h ***/
#ifndef __JUCE_APPLEREMOTE_JUCEHEADER__
#define __JUCE_APPLEREMOTE_JUCEHEADER__

#if JUCE_MAC || DOXYGEN
/**
	Receives events from an Apple IR remote control device (Only available in OSX!).

	To use it, just create a subclass of this class, implementing the buttonPressed()
	callback, then call start() and stop() to start or stop receiving events.
*/
class JUCE_API  AppleRemoteDevice
{
public:

	AppleRemoteDevice();
	virtual ~AppleRemoteDevice();

	/** The set of buttons that may be pressed.
		@see buttonPressed
	*/
	enum ButtonType
	{
		menuButton = 0,     /**< The menu button (if it's held for a short time). */
		playButton,         /**< The play button. */
		plusButton,         /**< The plus or volume-up button. */
		minusButton,        /**< The minus or volume-down button. */
		rightButton,        /**< The right button (if it's held for a short time). */
		leftButton,         /**< The left button (if it's held for a short time). */
		rightButton_Long,   /**< The right button (if it's held for a long time). */
		leftButton_Long,    /**< The menu button (if it's held for a long time). */
		menuButton_Long,    /**< The menu button (if it's held for a long time). */
		playButtonSleepMode,
		switched
	};

	/** Override this method to receive the callback about a button press.

		The callback will happen on the application's message thread.

		Some buttons trigger matching up and down events, in which the isDown parameter
		will be true and then false. Others only send a single event when the
		button is pressed.
	*/
	virtual void buttonPressed (ButtonType buttonId, bool isDown) = 0;

	/** Starts the device running and responding to events.

		Returns true if it managed to open the device.

		@param inExclusiveMode  if true, the remote will be grabbed exclusively for this app,
								and will not be available to any other part of the system. If
								false, it will be shared with other apps.
		@see stop
	*/
	bool start (bool inExclusiveMode);

	/** Stops the device running.
		@see start
	*/
	void stop();

	/** Returns true if the device has been started successfully.
	*/
	bool isActive() const;

	/** Returns the ID number of the remote, if it has sent one.
	*/
	int getRemoteId() const                     { return remoteId; }

	/** @internal */
	void handleCallbackInternal();

private:
	void* device;
	void* queue;
	int remoteId;

	bool open (bool openInExclusiveMode);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AppleRemoteDevice);
};

#endif
#endif   // __JUCE_APPLEREMOTE_JUCEHEADER__

/*** End of inlined file: juce_AppleRemote.h ***/


#endif
#ifndef __JUCE_BUBBLEMESSAGECOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_BubbleMessageComponent.h ***/
#ifndef __JUCE_BUBBLEMESSAGECOMPONENT_JUCEHEADER__
#define __JUCE_BUBBLEMESSAGECOMPONENT_JUCEHEADER__

/**
	A speech-bubble component that displays a short message.

	This can be used to show a message with the tail of the speech bubble
	pointing to a particular component or location on the screen.

	@see BubbleComponent
*/
class JUCE_API  BubbleMessageComponent  : public BubbleComponent,
										  private Timer
{
public:

	/** Creates a bubble component.

		After creating one a BubbleComponent, do the following:
		- add it to an appropriate parent component, or put it on the
		  desktop with Component::addToDesktop (0).
		- use the showAt() method to show a message.
		- it will make itself invisible after it times-out (and can optionally
		  also delete itself), or you can reuse it somewhere else by calling
		  showAt() again.
	*/
	BubbleMessageComponent (int fadeOutLengthMs = 150);

	/** Destructor. */
	~BubbleMessageComponent();

	/** Shows a message bubble at a particular position.

		This shows the bubble with its stem pointing to the given location
		(co-ordinates being relative to its parent component).

		For details about exactly how it decides where to position itself, see
		BubbleComponent::updatePosition().

		@param x                                the x co-ordinate of end of the bubble's tail
		@param y                                the y co-ordinate of end of the bubble's tail
		@param message                          the text to display
		@param numMillisecondsBeforeRemoving    how long to leave it on the screen before removing itself
												from its parent compnent. If this is 0 or less, it
												will stay there until manually removed.
		@param removeWhenMouseClicked           if this is true, the bubble will disappear as soon as a
												mouse button is pressed (anywhere on the screen)
		@param deleteSelfAfterUse               if true, then the component will delete itself after
												it becomes invisible
	*/
	void showAt (int x, int y,
				 const String& message,
				 int numMillisecondsBeforeRemoving,
				 bool removeWhenMouseClicked = true,
				 bool deleteSelfAfterUse = false);

	/** Shows a message bubble next to a particular component.

		This shows the bubble with its stem pointing at the given component.

		For details about exactly how it decides where to position itself, see
		BubbleComponent::updatePosition().

		@param component                        the component that you want to point at
		@param message                          the text to display
		@param numMillisecondsBeforeRemoving    how long to leave it on the screen before removing itself
												from its parent compnent. If this is 0 or less, it
												will stay there until manually removed.
		@param removeWhenMouseClicked           if this is true, the bubble will disappear as soon as a
												mouse button is pressed (anywhere on the screen)
		@param deleteSelfAfterUse               if true, then the component will delete itself after
												it becomes invisible
	*/
	void showAt (Component* component,
				 const String& message,
				 int numMillisecondsBeforeRemoving,
				 bool removeWhenMouseClicked = true,
				 bool deleteSelfAfterUse = false);

	/** @internal */
	void getContentSize (int& w, int& h);
	/** @internal */
	void paintContent (Graphics& g, int w, int h);
	/** @internal */
	void timerCallback();

private:

	int fadeOutLength, mouseClickCounter;
	TextLayout textLayout;
	int64 expiryTime;
	bool deleteAfterUse;

	void createLayout (const String&);
	void init (int numMillisecondsBeforeRemoving,
			   bool removeWhenMouseClicked,
			   bool deleteSelfAfterUse);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BubbleMessageComponent);
};

#endif   // __JUCE_BUBBLEMESSAGECOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_BubbleMessageComponent.h ***/


#endif
#ifndef __JUCE_COLOURSELECTOR_JUCEHEADER__

/*** Start of inlined file: juce_ColourSelector.h ***/
#ifndef __JUCE_COLOURSELECTOR_JUCEHEADER__
#define __JUCE_COLOURSELECTOR_JUCEHEADER__

/**
	A component that lets the user choose a colour.

	This shows RGB sliders and a colourspace that the user can pick colours from.

	This class is also a ChangeBroadcaster, so listeners can register to be told
	when the colour changes.
*/
class JUCE_API  ColourSelector  : public Component,
								  public ChangeBroadcaster,
								  protected SliderListener
{
public:

	/** Options for the type of selector to show. These are passed into the constructor. */
	enum ColourSelectorOptions
	{
		showAlphaChannel    = 1 << 0,   /**< if set, the colour's alpha channel can be changed as well as its RGB. */

		showColourAtTop     = 1 << 1,   /**< if set, a swatch of the colour is shown at the top of the component. */
		showSliders         = 1 << 2,   /**< if set, RGB sliders are shown at the bottom of the component. */
		showColourspace     = 1 << 3    /**< if set, a big HSV selector is shown. */
	};

	/** Creates a ColourSelector object.

		The flags are a combination of values from the ColourSelectorOptions enum, specifying
		which of the selector's features should be visible.

		The edgeGap value specifies the amount of space to leave around the edge.

		gapAroundColourSpaceComponent indicates how much of a gap to put around the
		colourspace and hue selector components.
	*/
	ColourSelector (int sectionsToShow = (showAlphaChannel | showColourAtTop | showSliders | showColourspace),
					int edgeGap = 4,
					int gapAroundColourSpaceComponent = 7);

	/** Destructor. */
	~ColourSelector();

	/** Returns the colour that the user has currently selected.

		The ColourSelector class is also a ChangeBroadcaster, so listeners can
		register to be told when the colour changes.

		@see setCurrentColour
	*/
	Colour getCurrentColour() const;

	/** Changes the colour that is currently being shown.
	*/
	void setCurrentColour (const Colour& newColour);

	/** Tells the selector how many preset colour swatches you want to have on the component.

		To enable swatches, you'll need to override getNumSwatches(), getSwatchColour(), and
		setSwatchColour(), to return the number of colours you want, and to set and retrieve
		their values.
	*/
	virtual int getNumSwatches() const;

	/** Called by the selector to find out the colour of one of the swatches.

		Your subclass should return the colour of the swatch with the given index.

		To enable swatches, you'll need to override getNumSwatches(), getSwatchColour(), and
		setSwatchColour(), to return the number of colours you want, and to set and retrieve
		their values.
	*/
	virtual Colour getSwatchColour (int index) const;

	/** Called by the selector when the user puts a new colour into one of the swatches.

		Your subclass should change the colour of the swatch with the given index.

		To enable swatches, you'll need to override getNumSwatches(), getSwatchColour(), and
		setSwatchColour(), to return the number of colours you want, and to set and retrieve
		their values.
	*/
	virtual void setSwatchColour (int index, const Colour& newColour) const;

	/** A set of colour IDs to use to change the colour of various aspects of the keyboard.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId              = 0x1007000,    /**< the colour used to fill the component's background. */
		labelTextColourId               = 0x1007001     /**< the colour used for the labels next to the sliders. */
	};

private:

	class ColourSpaceView;
	class HueSelectorComp;
	class SwatchComponent;
	friend class ColourSpaceView;
	friend class ScopedPointer<ColourSpaceView>;
	friend class HueSelectorComp;
	friend class ScopedPointer<HueSelectorComp>;

	Colour colour;
	float h, s, v;
	ScopedPointer<Slider> sliders[4];
	ScopedPointer<ColourSpaceView> colourSpace;
	ScopedPointer<HueSelectorComp> hueSelector;
	OwnedArray <SwatchComponent> swatchComponents;
	const int flags;
	int edgeGap;
	Rectangle<int> previewArea;

	void setHue (float newH);
	void setSV (float newS, float newV);
	void updateHSV();
	void update();
	void sliderValueChanged (Slider*);
	void paint (Graphics& g);
	void resized();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourSelector);

   #if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// This constructor is here temporarily to prevent old code compiling, because the parameters
	// have changed - if you get an error here, update your code to use the new constructor instead..
	ColourSelector (bool);
   #endif
};

#endif   // __JUCE_COLOURSELECTOR_JUCEHEADER__

/*** End of inlined file: juce_ColourSelector.h ***/


#endif
#ifndef __JUCE_KEYMAPPINGEDITORCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_KeyMappingEditorComponent.h ***/
#ifndef __JUCE_KEYMAPPINGEDITORCOMPONENT_JUCEHEADER__
#define __JUCE_KEYMAPPINGEDITORCOMPONENT_JUCEHEADER__

/**
	A component to allow editing of the keymaps stored by a KeyPressMappingSet
	object.

	@see KeyPressMappingSet
*/
class JUCE_API  KeyMappingEditorComponent  : public Component
{
public:

	/** Creates a KeyMappingEditorComponent.

		@param mappingSet   this is the set of mappings to display and edit. Make sure the
							mappings object is not deleted before this component!
		@param showResetToDefaultButton     if true, then at the bottom of the list, the
											component will include a 'reset to defaults' button.
	*/
	KeyMappingEditorComponent (KeyPressMappingSet& mappingSet,
							   bool showResetToDefaultButton);

	/** Destructor. */
	virtual ~KeyMappingEditorComponent();

	/** Sets up the colours to use for parts of the component.

		@param mainBackground       colour to use for most of the background
		@param textColour           colour to use for the text
	*/
	void setColours (const Colour& mainBackground,
					 const Colour& textColour);

	/** Returns the KeyPressMappingSet that this component is acting upon. */
	KeyPressMappingSet& getMappings() const noexcept                { return mappings; }

	/** Can be overridden if some commands need to be excluded from the list.

		By default this will use the KeyPressMappingSet's shouldCommandBeVisibleInEditor()
		method to decide what to return, but you can override it to handle special cases.
	*/
	virtual bool shouldCommandBeIncluded (CommandID commandID);

	/** Can be overridden to indicate that some commands are shown as read-only.

		By default this will use the KeyPressMappingSet's shouldCommandBeReadOnlyInEditor()
		method to decide what to return, but you can override it to handle special cases.
	*/
	virtual bool isCommandReadOnly (CommandID commandID);

	/** This can be overridden to let you change the format of the string used
		to describe a keypress.

		This is handy if you're using non-standard KeyPress objects, e.g. for custom
		keys that are triggered by something else externally. If you override the
		method, be sure to let the base class's method handle keys you're not
		interested in.
	*/
	virtual String getDescriptionForKeyPress (const KeyPress& key);

	/** A set of colour IDs to use to change the colour of various aspects of the editor.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		To change the colours of the menu that pops up

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		backgroundColourId  = 0x100ad00,    /**< The background colour to fill the editor background. */
		textColourId        = 0x100ad01,    /**< The colour for the text. */
	};

	/** @internal */
	void parentHierarchyChanged();
	/** @internal */
	void resized();

private:

	KeyPressMappingSet& mappings;
	TreeView tree;
	TextButton resetButton;

	class TopLevelItem;
	class ChangeKeyButton;
	class MappingItem;
	class CategoryItem;
	class ItemComponent;
	friend class TopLevelItem;
	friend class OwnedArray <ChangeKeyButton>;
	friend class ScopedPointer<TopLevelItem>;
	ScopedPointer<TopLevelItem> treeItem;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyMappingEditorComponent);
};

#endif   // __JUCE_KEYMAPPINGEDITORCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_KeyMappingEditorComponent.h ***/


#endif
#ifndef __JUCE_PREFERENCESPANEL_JUCEHEADER__

/*** Start of inlined file: juce_PreferencesPanel.h ***/
#ifndef __JUCE_PREFERENCESPANEL_JUCEHEADER__
#define __JUCE_PREFERENCESPANEL_JUCEHEADER__

/**
	A component with a set of buttons at the top for changing between pages of
	preferences.

	This is just a handy way of writing a Mac-style preferences panel where you
	have a row of buttons along the top for the different preference categories,
	each button having an icon above its name. Clicking these will show an
	appropriate prefs page below it.

	You can either put one of these inside your own component, or just use the
	showInDialogBox() method to show it in a window and run it modally.

	To use it, just add a set of named pages with the addSettingsPage() method,
	and implement the createComponentForPage() method to create suitable components
	for each of these pages.
*/
class JUCE_API  PreferencesPanel  : public Component,
									private ButtonListener // (can't use Button::Listener due to idiotic VC2005 bug)
{
public:

	/** Creates an empty panel.

		Use addSettingsPage() to add some pages to it in your constructor.
	*/
	PreferencesPanel();

	/** Destructor. */
	~PreferencesPanel();

	/** Creates a page using a set of drawables to define the page's icon.

		Note that the other version of this method is much easier if you're using
		an image instead of a custom drawable.

		@param pageTitle    the name of this preferences page - you'll need to
							make sure your createComponentForPage() method creates
							a suitable component when it is passed this name
		@param normalIcon   the drawable to display in the page's button normally
		@param overIcon     the drawable to display in the page's button when the mouse is over
		@param downIcon     the drawable to display in the page's button when the button is down
		@see DrawableButton
	*/
	void addSettingsPage (const String& pageTitle,
						  const Drawable* normalIcon,
						  const Drawable* overIcon,
						  const Drawable* downIcon);

	/** Creates a page using a set of drawables to define the page's icon.

		The other version of this method gives you more control over the icon, but this
		one is much easier if you're just loading it from a file.

		@param pageTitle        the name of this preferences page - you'll need to
								make sure your createComponentForPage() method creates
								a suitable component when it is passed this name
		@param imageData        a block of data containing an image file, e.g. a jpeg, png or gif.
								For this to look good, you'll probably want to use a nice
								transparent png file.
		@param imageDataSize    the size of the image data, in bytes
	*/
	void addSettingsPage (const String& pageTitle,
						  const void* imageData,
						  int imageDataSize);

	/** Utility method to display this panel in a DialogWindow.

		Calling this will create a DialogWindow containing this panel with the
		given size and title, and will run it modally, returning when the user
		closes the dialog box.
	*/
	void showInDialogBox (const String& dialogTitle,
						  int dialogWidth,
						  int dialogHeight,
						  const Colour& backgroundColour = Colours::white);

	/** Subclasses must override this to return a component for each preferences page.

		The subclass should return a pointer to a new component representing the named
		page, which the panel will then display.

		The panel will delete the component later when the user goes to another page
		or deletes the panel.
	*/
	virtual Component* createComponentForPage (const String& pageName) = 0;

	/** Changes the current page being displayed. */
	void setCurrentPage (const String& pageName);

	/** Returns the size of the buttons shown along the top. */
	int getButtonSize() const noexcept;

	/** Changes the size of the buttons shown along the top. */
	void setButtonSize (int newSize);

	/** @internal */
	void resized();
	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void buttonClicked (Button* button);

private:

	String currentPageName;
	ScopedPointer <Component> currentPage;
	OwnedArray<DrawableButton> buttons;
	int buttonSize;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreferencesPanel);
};

#endif   // __JUCE_PREFERENCESPANEL_JUCEHEADER__

/*** End of inlined file: juce_PreferencesPanel.h ***/


#endif
#ifndef __JUCE_RECENTLYOPENEDFILESLIST_JUCEHEADER__

/*** Start of inlined file: juce_RecentlyOpenedFilesList.h ***/
#ifndef __JUCE_RECENTLYOPENEDFILESLIST_JUCEHEADER__
#define __JUCE_RECENTLYOPENEDFILESLIST_JUCEHEADER__

/**
	Manages a set of files for use as a list of recently-opened documents.

	This is a handy class for holding your list of recently-opened documents, with
	helpful methods for things like purging any non-existent files, automatically
	adding them to a menu, and making persistence easy.

	@see File, FileBasedDocument
*/
class JUCE_API  RecentlyOpenedFilesList
{
public:

	/** Creates an empty list.
	*/
	RecentlyOpenedFilesList();

	/** Destructor. */
	~RecentlyOpenedFilesList();

	/** Sets a limit for the number of files that will be stored in the list.

		When addFile() is called, then if there is no more space in the list, the
		least-recently added file will be dropped.

		@see getMaxNumberOfItems
	*/
	void setMaxNumberOfItems (int newMaxNumber);

	/** Returns the number of items that this list will store.
		@see setMaxNumberOfItems
	*/
	int getMaxNumberOfItems() const noexcept                            { return maxNumberOfItems; }

	/** Returns the number of files in the list.

		The most recently added file is always at index 0.
	*/
	int getNumFiles() const;

	/** Returns one of the files in the list.

		The most recently added file is always at index 0.
	*/
	File getFile (int index) const;

	/** Returns an array of all the absolute pathnames in the list.
	*/
	const StringArray& getAllFilenames() const noexcept                 { return files; }

	/** Clears all the files from the list. */
	void clear();

	/** Adds a file to the list.

		The file will be added at index 0. If this file is already in the list, it will
		be moved up to index 0, but a file can only appear once in the list.

		If the list already contains the maximum number of items that is permitted, the
		least-recently added file will be dropped from the end.
	*/
	void addFile (const File& file);

	/** Removes a file from the list. */
	void removeFile (const File& file);

	/** Checks each of the files in the list, removing any that don't exist.

		You might want to call this after reloading a list of files, or before putting them
		on a menu.
	*/
	void removeNonExistentFiles();

	/** Adds entries to a menu, representing each of the files in the list.

		This is handy for creating an "open recent file..." menu in your app. The
		menu items are numbered consecutively starting with the baseItemId value,
		and can either be added as complete pathnames, or just the last part of the
		filename.

		If dontAddNonExistentFiles is true, then each file will be checked and only those
		that exist will be added.

		If filesToAvoid is non-zero, then it is considered to be a zero-terminated array of
		pointers to file objects. Any files that appear in this list will not be added to the
		menu - the reason for this is that you might have a number of files already open, so
		might not want these to be shown in the menu.

		It returns the number of items that were added.
	*/
	int createPopupMenuItems (PopupMenu& menuToAddItemsTo,
							  int baseItemId,
							  bool showFullPaths,
							  bool dontAddNonExistentFiles,
							  const File** filesToAvoid = nullptr);

	/** Returns a string that encapsulates all the files in the list.

		The string that is returned can later be passed into restoreFromString() in
		order to recreate the list. This is handy for persisting your list, e.g. in
		a PropertiesFile object.

		@see restoreFromString
	*/
	String toString() const;

	/** Restores the list from a previously stringified version of the list.

		Pass in a stringified version created with toString() in order to persist/restore
		your list.

		@see toString
	*/
	void restoreFromString (const String& stringifiedVersion);

private:

	StringArray files;
	int maxNumberOfItems;

	JUCE_LEAK_DETECTOR (RecentlyOpenedFilesList);
};

#endif   // __JUCE_RECENTLYOPENEDFILESLIST_JUCEHEADER__

/*** End of inlined file: juce_RecentlyOpenedFilesList.h ***/


#endif
#ifndef __JUCE_SPLASHSCREEN_JUCEHEADER__

/*** Start of inlined file: juce_SplashScreen.h ***/
#ifndef __JUCE_SPLASHSCREEN_JUCEHEADER__
#define __JUCE_SPLASHSCREEN_JUCEHEADER__

/** A component for showing a splash screen while your app starts up.

	This will automatically position itself, and delete itself when the app has
	finished initialising (it uses the JUCEApplication::isInitialising() to detect
	this).

	To use it, just create one of these in your JUCEApplication::initialise() method,
	call its show() method and let the object delete itself later.

	E.g. @code

	void MyApp::initialise (const String& commandLine)
	{
		SplashScreen* splash = new SplashScreen();

		splash->show ("welcome to my app",
					  ImageCache::getFromFile (File ("/foobar/splash.jpg")),
					  4000, false);

		.. no need to delete the splash screen - it'll do that itself.
	}

	@endcode
*/
class JUCE_API  SplashScreen  : public Component,
								public Timer,
								private DeletedAtShutdown
{
public:

	/** Creates a SplashScreen object.

		After creating one of these (or your subclass of it), call one of the show()
		methods to display it.
	*/
	SplashScreen();

	/** Destructor. */
	~SplashScreen();

	/** Creates a SplashScreen object that will display an image.

		As soon as this is called, the SplashScreen will be displayed in the centre of the
		screen. This method will also dispatch any pending messages to make sure that when
		it returns, the splash screen has been completely drawn, and your initialisation
		code can carry on.

		@param title            the name to give the component
		@param backgroundImage  an image to draw on the component. The component's size
								will be set to the size of this image, and if the image is
								semi-transparent, the component will be made semi-transparent
								too. This image will be deleted (or released from the ImageCache
								if that's how it was created) by the splash screen object when
								it is itself deleted.
		@param minimumTimeToDisplayFor    how long (in milliseconds) the splash screen
								should stay visible for. If the initialisation takes longer than
								this time, the splash screen will wait for it to finish before
								disappearing, but if initialisation is very quick, this lets
								you make sure that people get a good look at your splash.
		@param useDropShadow    if true, the window will have a drop shadow
		@param removeOnMouseClick   if true, the window will go away as soon as the user clicks
								the mouse (anywhere)
	*/
	void show (const String& title,
			   const Image& backgroundImage,
			   int minimumTimeToDisplayFor,
			   bool useDropShadow,
			   bool removeOnMouseClick = true);

	/** Creates a SplashScreen object with a specified size.

		For a custom splash screen, you can use this method to display it at a certain size
		and then override the paint() method yourself to do whatever's necessary.

		As soon as this is called, the SplashScreen will be displayed in the centre of the
		screen. This method will also dispatch any pending messages to make sure that when
		it returns, the splash screen has been completely drawn, and your initialisation
		code can carry on.

		@param title            the name to give the component
		@param width            the width to use
		@param height           the height to use
		@param minimumTimeToDisplayFor    how long (in milliseconds) the splash screen
								should stay visible for. If the initialisation takes longer than
								this time, the splash screen will wait for it to finish before
								disappearing, but if initialisation is very quick, this lets
								you make sure that people get a good look at your splash.
		@param useDropShadow    if true, the window will have a drop shadow
		@param removeOnMouseClick   if true, the window will go away as soon as the user clicks
								the mouse (anywhere)
	*/
	void show (const String& title,
			   int width,
			   int height,
			   int minimumTimeToDisplayFor,
			   bool useDropShadow,
			   bool removeOnMouseClick = true);

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void timerCallback();

private:

	Image backgroundImage;
	Time earliestTimeToDelete;
	int originalClickCounter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SplashScreen);
};

#endif   // __JUCE_SPLASHSCREEN_JUCEHEADER__

/*** End of inlined file: juce_SplashScreen.h ***/


#endif
#ifndef __JUCE_SYSTEMTRAYICONCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_SystemTrayIconComponent.h ***/
#ifndef __JUCE_SYSTEMTRAYICONCOMPONENT_JUCEHEADER__
#define __JUCE_SYSTEMTRAYICONCOMPONENT_JUCEHEADER__

#if JUCE_WINDOWS || JUCE_LINUX || DOXYGEN

/**
	On Windows and Linux only, this component sits in the taskbar tray as a small icon.

	To use it, just create one of these components, but don't attempt to make it
	visible, add it to a parent, or put it on the desktop.

	You can then call setIconImage() to create an icon for it in the taskbar.

	To change the icon's tooltip, you can use setIconTooltip().

	To respond to mouse-events, you can override the normal mouseDown(),
	mouseUp(), mouseDoubleClick() and mouseMove() methods, and although the x, y
	position will not be valid, you can use this to respond to clicks. Traditionally
	you'd use a left-click to show your application's window, and a right-click
	to show a pop-up menu.
*/
class JUCE_API  SystemTrayIconComponent  : public Component
{
public:

	SystemTrayIconComponent();

	/** Destructor. */
	~SystemTrayIconComponent();

	/** Changes the image shown in the taskbar.
	*/
	void setIconImage (const Image& newImage);

	/** Changes the tooltip that Windows shows above the icon. */
	void setIconTooltip (const String& tooltip);

   #if JUCE_LINUX
	/** @internal */
	void paint (Graphics& g);
   #endif

private:

	class Pimpl;
	ScopedPointer<Pimpl> pimpl;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SystemTrayIconComponent);
};

#endif
#endif   // __JUCE_SYSTEMTRAYICONCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_SystemTrayIconComponent.h ***/


#endif
#ifndef __JUCE_WEBBROWSERCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_WebBrowserComponent.h ***/
#ifndef __JUCE_WEBBROWSERCOMPONENT_JUCEHEADER__
#define __JUCE_WEBBROWSERCOMPONENT_JUCEHEADER__

#if JUCE_WEB_BROWSER || DOXYGEN

#if ! DOXYGEN
 class WebBrowserComponentInternal;
#endif

/**
	A component that displays an embedded web browser.

	The browser itself will be platform-dependent. On the Mac, probably Safari, on
	Windows, probably IE.

*/
class JUCE_API  WebBrowserComponent      : public Component
{
public:

	/** Creates a WebBrowserComponent.

		Once it's created and visible, send the browser to a URL using goToURL().

		@param unloadPageWhenBrowserIsHidden  if this is true, then when the browser
							component is taken offscreen, it'll clear the current page
							and replace it with a blank page - this can be handy to stop
							the browser using resources in the background when it's not
							actually being used.
	*/
	explicit WebBrowserComponent (bool unloadPageWhenBrowserIsHidden = true);

	/** Destructor. */
	~WebBrowserComponent();

	/** Sends the browser to a particular URL.

		@param url      the URL to go to.
		@param headers  an optional set of parameters to put in the HTTP header. If
						you supply this, it should be a set of string in the form
						"HeaderKey: HeaderValue"
		@param postData an optional block of data that will be attached to the HTTP
						POST request
	*/
	void goToURL (const String& url,
				  const StringArray* headers = nullptr,
				  const MemoryBlock* postData = nullptr);

	/** Stops the current page loading.
	*/
	void stop();

	/** Sends the browser back one page.
	*/
	void goBack();

	/** Sends the browser forward one page.
	*/
	void goForward();

	/** Refreshes the browser.
	*/
	void refresh();

	/** This callback is called when the browser is about to navigate
		to a new location.

		You can override this method to perform some action when the user
		tries to go to a particular URL. To allow the operation to carry on,
		return true, or return false to stop the navigation happening.
	*/
	virtual bool pageAboutToLoad (const String& newURL);

	/** This callback happens when the browser has finished loading a page. */
	virtual void pageFinishedLoading (const String& url);

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void parentHierarchyChanged();
	/** @internal */
	void visibilityChanged();

private:

	WebBrowserComponentInternal* browser;
	bool blankPageShown, unloadPageWhenBrowserIsHidden;
	String lastURL;
	StringArray lastHeaders;
	MemoryBlock lastPostData;

	void reloadLastURL();
	void checkWindowAssociation();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebBrowserComponent);
};

#endif
#endif   // __JUCE_WEBBROWSERCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_WebBrowserComponent.h ***/


#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_GUI_EXTRA_JUCEHEADER__

/*** End of inlined file: juce_gui_extra.h ***/

