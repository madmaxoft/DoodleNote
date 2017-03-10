// Page.h

// Declares the Page class representing a single visual page of content





#ifndef PAGE_H
#define PAGE_H





#include <QGraphicsScene>
#include "Stroke.h"





class Page:
	public QGraphicsScene
{
	Q_OBJECT
	typedef QGraphicsScene Super;


public:

	/** Identifies the tool currently selected. */
	enum Tool
	{
		toolSelect,
		toolDraw,
	};


	/** Creates a new empty page. */
	Page();

	/** Returns the currently selected tool. */
	Tool currentTool();

	/** Selects the specified tool. */
	void setCurrentTool(Tool a_Tool);

protected:

	/** The tool currently selected. */
	Tool m_CurrentTool;

	/** Individual strokes comprising the page. */
	StrokePtrs m_Strokes;

	/** The stroke currentply being drawn (nullptr if none). */
	StrokePtr m_CurrentlyDrawnStroke;

	/** The representation of m_CurrentlyDrawnStroke in the scene (nullptr if none). */
	QGraphicsPathItem * m_CurrentlyDrawnStrokePath;

	/** The timepoint when the currently drawn stroke was started. */
	std::chrono::high_resolution_clock::time_point m_CurrentlyDrawnStrokeTimeStart;


	// QGraphicsScene overrides:
	void mousePressEvent  (QGraphicsSceneMouseEvent * a_MouseEvent) override;
	void mouseMoveEvent   (QGraphicsSceneMouseEvent * a_MouseEvent) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * a_MouseEvent) override;
};





#endif // PAGE_H
