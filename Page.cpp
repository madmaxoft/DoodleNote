// Page.cpp

// Implements the Page class representing a single visual page of content





#include "Page.h"
#include <QPainter>





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
	connect(stroke.get(), &Stroke::changed, this, &Page::changed);
	emit changed();
	return stroke;
}





StrokePtr Page::addStroke(qreal a_X, qreal a_Y)
{
	auto stroke = std::make_shared<Stroke>(a_X, a_Y);
	m_Strokes.push_back(stroke);
	connect(stroke.get(), &Stroke::changed, this, &Page::changed);
	emit changed();
	return stroke;
}





QPixmap Page::render(int a_Width, int a_Height)
{
	QPixmap res(a_Width, a_Height);
	res.fill(Qt::white);
	auto boundingBox = getBoundingBox();
	if (boundingBox.isEmpty())
	{
		return res;
	}
	QPainter painter(&res);
	QTransform transform;
	auto factor = std::min(a_Width / boundingBox.width(), a_Height / boundingBox.height());
	transform.scale(factor, factor);
	transform.translate(-boundingBox.left(), -boundingBox.top());
	painter.setTransform(transform);
	for (const auto & s: m_Strokes)
	{
		painter.drawPath(s->toPath());
	}
	return res;
}





QRectF Page::getBoundingBox() const
{
	QRectF res;
	for (const auto & s: m_Strokes)
	{
		auto bb = s->getBoundingBox();
		res = res.united(bb);
	}
	return res;
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




