// Document.h

// Declares the Document class representing a single document consisting of multiple pages





#ifndef DOCUMENT_H
#define DOCUMENT_H





#include <memory>
#include <vector>
#include <QObject>





// fwd:
class QDataStream;
class Page;
typedef std::shared_ptr<Page> PagePtr;





class Document:
	public QObject
{
	Q_OBJECT

public:
	/** Creates a new document with one empty page. */
	Document();

	/** Creates a new Document object with the specified filename.
	No pages are added to the document, caller must call loadFromStream(). */
	Document(const QString & a_FileName);

	/** Returns all the pages in the document. */
	const std::vector<PagePtr> & pages() const { return m_Pages; }

	/** Saves all document data into the specified stream. */
	void saveToStream(QDataStream & a_Stream) const;

	/** Loads all document data from the specified stream.
	Throws an exception on failure. */
	void loadFromStream(QDataStream & a_Stream);

	/** Returns the page that is to be currently displayed / edited. */
	PagePtr currentPage() const { return m_CurrentPage; }

	/** Returns the name of the file from which the document was loaded. */
	const QString & fileName() const { return m_FileName; }

	/** Sets the name of the file from which the document was loaded. */
	void setFileName(const QString & a_FileName) { m_FileName = a_FileName; }

	/** Returns true if the document has changed since it was loaded / created. */
	bool isModified() const { return m_IsModified; }

	/** Adds a new page. */
	PagePtr addPage();

protected:

	/** All pages in the document. */
	std::vector<PagePtr> m_Pages;

	/** The page (out of m_Pages) that is currently being edited. */
	PagePtr m_CurrentPage;

	/** The name of the file from which the document was loaded. */
	QString m_FileName;

	/** Indicates whether the document has changed since it was loaded / created. */
	bool m_IsModified;


	/** Loads the document data from the specified stream, after it was decided that it is a version 0 stream.
	Throws an exception on failure. */
	void loadFromStreamV0(QDataStream & a_Stream);

protected slots:

	/** Emitted by a page when its contents change (due to user input). */
	void pageChanged();
};





#endif // DOCUMENT_H
