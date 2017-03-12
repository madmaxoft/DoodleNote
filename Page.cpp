// Page.cpp

// Implements the Page class representing a single visual page of content





#include "Page.h"





Page::Page()
{
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
}





StrokePtr Page::addStroke()
{
	auto stroke = std::make_shared<Stroke>();
	m_Strokes.push_back(stroke);
	return stroke;
}





StrokePtr Page::addStroke(qreal a_X, qreal a_Y)
{
	auto stroke = std::make_shared<Stroke>(a_X, a_Y);
	m_Strokes.push_back(stroke);
	return stroke;
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




