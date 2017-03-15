// Page.h

// Declares the Page class representing a single visual page of content





#ifndef PAGE_H
#define PAGE_H





#include <memory>
#include <QGraphicsScene>
#include "Stroke.h"





// fwd:
class QDataStream;





class Page:
	public QObject
{
	Q_OBJECT
	typedef QObject Super;


public:

	/** Creates a new empty page. */
	Page();

	/** Saves all page data into the specified stream. */
	void saveToStream(QDataStream & a_Stream) const;

	/** Loads all page data from the specified stream.
	Throws an exception on failure. */
	void loadFromStream(QDataStream & a_Stream);

	/** Adds a new empty stroke to the internal representation, returns a shared ptr to it. */
	StrokePtr addStroke();

	/** Adds a new stroke with a single point to the internal representation.
	Returns a shared ptr to the new stroke. */
	StrokePtr addStroke(qreal a_X, qreal a_Y);

	/** Returns all the strokes in the page, read-only. */
	const StrokePtrs & strokes() const { return m_Strokes; }

	/** Returns a preview image for the page of the specified dimensions. */
	QPixmap render(int a_Width, int a_Height);

	/** Returns the bounding box of all items in this page.
	If there are no items, returns an empty QRect. */
	QRectF getBoundingBox() const;

signals:

	/** Emitted when the data in the page is changed. */
	void changed();


protected:

	/** Individual strokes comprising the page. */
	StrokePtrs m_Strokes;


	/** Loads all page data from the specified stream, after it was decided that it is a version 0 stream.
	Throws an exception on failure. */
	void loadFromStreamV0(QDataStream & a_Stream);
};

typedef std::shared_ptr<Page> PagePtr;





#endif // PAGE_H
