// Document.h

// Declares the Document class representing a single document consisting of multiple pages





#ifndef DOCUMENT_H
#define DOCUMENT_H





#include <memory>
#include <vector>





// fwd:
class Page;
typedef std::shared_ptr<Page> PagePtr;





class Document
{
public:
	/** Creates a new document with one empty page. */
	Document();

	/** Returns all the pages in the document. */
	const std::vector<PagePtr> & pages() const { return m_Pages; }


protected:

	/** All pages in the document. */
	std::vector<PagePtr> m_Pages;

};

#endif // DOCUMENT_H
