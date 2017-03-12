// PageScene.cpp

// Implements the PageScene object representing the binding between the Page data and a QGraphicScene





#include "PageScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>





PageScene::PageScene():
	m_CurrentTool(toolSelect)
{
	setSceneRect(0, 0, 5000, 5000);
}





void PageScene::setCurrentTool(PageScene::Tool a_Tool)
{
	m_CurrentTool = a_Tool;
}





void PageScene::setPage(PagePtr a_Page)
{
	// Disconnect the previous page:
	if (m_Page != nullptr)
	{
		disconnect(m_Page.get());
	}

	// Connect the new page:
	m_Page = a_Page;

	// Re-create all graphic items:
	clear();
	for (const auto & s: m_Page->strokes())
	{
		addPath(s->toPath());
	}
}





void PageScene::mousePressEvent(QGraphicsSceneMouseEvent * a_MouseEvent)
{
	if (a_MouseEvent->button() != Qt::LeftButton)
	{
		Super::mousePressEvent(a_MouseEvent);
		return;
	}

	switch (m_CurrentTool)
	{
		case toolSelect:
		{
			// Selection is built-in (TODO: only in QGraphicsView::RubberBandDrag mode)
			Super::mousePressEvent(a_MouseEvent);
			return;
		}
		case toolDraw:
		{
			// The line is to be created after moving the mouse for the first time
			// Nothing to do here.
			return;
		}
	}
	Super::mousePressEvent(a_MouseEvent);
}





void PageScene::mouseMoveEvent(QGraphicsSceneMouseEvent * a_MouseEvent)
{
	switch (m_CurrentTool)
	{
		case toolSelect:
		{
			// Selection is built-in (TODO: only in QGraphicsView::RubberBandDrag mode)
			Super::mouseMoveEvent(a_MouseEvent);
			return;
		}
		case toolDraw:
		{
			if (a_MouseEvent->buttons().testFlag(Qt::LeftButton))
			{
				if (m_CurrentlyDrawnStroke == nullptr)
				{
					auto lastPressScenePos = a_MouseEvent->buttonDownScenePos(Qt::LeftButton);
					m_CurrentlyDrawnStroke = m_Page->addStroke(lastPressScenePos.x(), lastPressScenePos.y());
					m_CurrentlyDrawnStrokePath = addPath(m_CurrentlyDrawnStroke->toPath());
					// m_CurrentlyDrawnStrokePath->setPen(QPen(QColor(0, 0, 0), 3.5f));
				}
				else
				{
					const auto pos = a_MouseEvent->scenePos();
					const auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
						std::chrono::high_resolution_clock::now() - m_CurrentlyDrawnStrokeTimeStart
					);
					m_CurrentlyDrawnStroke->addPoint(pos.x(), pos.y(), timestamp);
					m_CurrentlyDrawnStrokePath->setPath(m_CurrentlyDrawnStroke->toPath());
				}
			}
			return;
		}
	}
	Super::mouseMoveEvent(a_MouseEvent);
}





void PageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * a_MouseEvent)
{
	if (a_MouseEvent->button() != Qt::LeftButton)
	{
		Super::mouseReleaseEvent(a_MouseEvent);
		return;
	}

	switch (m_CurrentTool)
	{
		case toolSelect:
		{
			// Selection is built-in (TODO: only in QGraphicsView::RubberBandDrag mode)
			Super::mouseReleaseEvent(a_MouseEvent);
			return;
		}
		case toolDraw:
		{
			if (m_CurrentlyDrawnStroke != nullptr)
			{
				qDebug(
					"Finished drawing a stroke, currently has %u points",
					static_cast<unsigned>(m_CurrentlyDrawnStroke->points().size())
				);
			}
			m_CurrentlyDrawnStroke = nullptr;
			m_CurrentlyDrawnStrokePath = nullptr;
			return;
		}
	}
	Super::mouseReleaseEvent(a_MouseEvent);
}





