// PageScene.h

// Declares the PageScene object representing the binding between the Page data and a QGraphicScene





#ifndef PAGESCENE_H
#define PAGESCENE_H





#include <QGraphicsScene>
#include "Page.h"





class PageScene:
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


	PageScene();

	/** Returns the currently selected tool. */
	Tool currentTool();

	/** Selects the specified tool. */
	void setCurrentTool(Tool a_Tool);

	/** Sets the currently represented page.
	Connects to the page's signals, disconnects any signals from the previously-connected page. */
	void setPage(PagePtr a_Page);


protected:

	/** The tool currently selected. */
	Tool m_CurrentTool;

	/** The page being currently represented. */
	PagePtr m_Page;

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





#endif // PAGESCENE_H




