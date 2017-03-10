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
}




