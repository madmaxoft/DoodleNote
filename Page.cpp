// Page.cpp

// Implements the Page class representing a single visual page of content





#include "Page.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>





Page::Page():
	m_CurrentTool(toolSelect)
{
	setSceneRect(0, 0, 5000, 5000);
}





void Page::setCurrentTool(Page::Tool a_Tool)
{
	m_CurrentTool = a_Tool;
}





void Page::saveToStream(QDataStream & a_Stream) const
{
	a_Stream << static_cast<quint32>(0);  // Version
	a_Stream << static_cast<quint32>(m_Strokes.size());
	for (const auto & stroke: m_Strokes)
	{
		stroke->saveToStream(a_Stream);
	}
}





void Page::loadFromStream(QDataStream & a_Stream)
{
	quint32 version = 0;
	a_Stream >> version;
	if (version == 0)
	{
		loadFromStreamV0(a_Stream);
	}
	else
	{
		throw std::runtime_error("Page uses an unknown datastream version");
	}

	// Re-build the entire QGraphicScene:
	clear();
	for (const auto & s: m_Strokes)
	{
		addPath(s->toPath());
	}
}





StrokePtr Page::addStroke()
{
	auto stroke = std::make_shared<Stroke>();
	m_Strokes.push_back(stroke);
	return stroke;
}





void Page::mousePressEvent(QGraphicsSceneMouseEvent * a_MouseEvent)
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





void Page::mouseMoveEvent(QGraphicsSceneMouseEvent * a_MouseEvent)
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
					m_CurrentlyDrawnStroke = std::make_shared<Stroke>(lastPressScenePos.x(), lastPressScenePos.y());
					m_Strokes.push_back(m_CurrentlyDrawnStroke);
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





void Page::mouseReleaseEvent(QGraphicsSceneMouseEvent * a_MouseEvent)
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





void Page::loadFromStreamV0(QDataStream & a_Stream)
{
	quint32 numStrokes = 0;
	a_Stream >> numStrokes;
	if (numStrokes > 300000)
	{
		throw std::runtime_error("Suspicious stroke count");
	}
	for (quint32 i = 0; i < numStrokes; ++i)
	{
		auto stroke = addStroke();
		stroke->loadFromStream(a_Stream);
	}
}




