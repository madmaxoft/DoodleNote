// Stroke.h

// Declares the Stroke class representing a single graphic stroke in a Page





#ifndef STROKE_H
#define STROKE_H





#include <vector>
#include <memory>
#include <chrono>
#include <QPainterPath>





class Stroke
{
public:

	/** Representation of a single point of the stroke. */
	struct Point
	{
		qreal m_X;
		qreal m_Y;
		std::chrono::milliseconds m_Timestamp;  // Time elapsed since the first point

		Point(qreal a_X, qreal a_Y, std::chrono::milliseconds a_Timestamp):
			m_X(a_X),
			m_Y(a_Y),
			m_Timestamp(a_Timestamp)
		{
		}
	};


	/** Creates a new empty stroke. */
	Stroke();

	/** Creates a new stroke with an initial point. */
	Stroke(qreal a_X, qreal a_Y);

	/** Creates a QPainterPath out of the points. */
	QPainterPath toPath() const;

	/** Adds a new point to the stroke. */
	void addPoint(qreal a_X, qreal a_Y, std::chrono::milliseconds a_Timestamp);

	/** Returns the (read only) points in this stroke. */
	const std::vector<Point> & points() const { return m_Points; }

protected:

	/** Individual points in the stroke. */
	std::vector<Point> m_Points;
};

typedef std::shared_ptr<Stroke> StrokePtr;
typedef std::vector<StrokePtr> StrokePtrs;





#endif // STROKE_H
