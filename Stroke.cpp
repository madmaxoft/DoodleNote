// Stroke.cpp

// Implements the Stroke class representing a single graphic stroke in a Page





#include "Stroke.h"





Stroke::Stroke()
{

}





Stroke::Stroke(qreal a_X, qreal a_Y)
{
	m_Points.emplace_back(a_X, a_Y, std::chrono::milliseconds(0));
}





QPainterPath Stroke::toPath() const
{
	// If the stroke is empty, return an empty path:
	QPainterPath res;
	if (m_Points.empty())
	{
		return res;
	}

	// Add the stroke:
	res.moveTo(m_Points.front().m_X, m_Points.front().m_Y);
	for (const auto & p: m_Points)
	{
		res.lineTo(p.m_X, p.m_Y);
	}
	return res;
}





void Stroke::addPoint(qreal a_X, qreal a_Y, std::chrono::milliseconds a_Timestamp)
{
	m_Points.emplace_back(a_X, a_Y, a_Timestamp);
	emit changed();
}





void Stroke::saveToStream(QDataStream & a_Stream) const
{
	a_Stream << static_cast<quint32>(0);  // Version
	a_Stream << static_cast<quint32>(m_Points.size());
	for (const auto & p: m_Points)
	{
		a_Stream << p.m_X << p.m_Y;
		a_Stream << static_cast<quint32>(p.m_Timestamp.count());
	}
}





void Stroke::loadFromStream(QDataStream & a_Stream)
{
	quint32 version = 0;
	a_Stream >> version;
	if (version == 0)
	{
		loadFromStreamV0(a_Stream);
		return;
	}
	throw std::runtime_error("Stroke uses an unknown version");
}





QRectF Stroke::getBoundingBox() const
{
	if (m_Points.empty())
	{
		return QRectF();
	}

	auto t = m_Points[0].m_Y;
	auto l = m_Points[0].m_X;
	auto b = t;
	auto r = l;
	for (const auto & p: m_Points)
	{
		if (p.m_X < l)
		{
			l = p.m_X;
		}
		if (p.m_X > r)
		{
			r = p.m_X;
		}
		if (p.m_Y < t)
		{
			t = p.m_Y;
		}
		if (p.m_Y > b)
		{
			b = p.m_Y;
		}
	}
	return QRectF(l, t, r - l, b - t);
}





void Stroke::loadFromStreamV0(QDataStream & a_Stream)
{
	quint32 numPoints = 0;
	a_Stream >> numPoints;
	if (numPoints >= 300000)
	{
		throw std::runtime_error("Stroke has a suspicious amount of points");
	}
	for (quint32 i = 0; i < numPoints; ++i)
	{
		qreal x, y;
		quint32 timestamp;
		a_Stream >> x >> y >> timestamp;
		m_Points.emplace_back(x, y, std::chrono::milliseconds(timestamp));
	}
}




