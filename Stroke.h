// Stroke.h

// Declares the Stroke class representing a single graphic stroke in a Page





#ifndef STROKE_H
#define STROKE_H





#include <vector>
#include <memory>
#include <chrono>
#include <QPainterPath>





// fwd:
class QDataStream;





class Stroke:
	public QObject
{
	Q_OBJECT
	typedef QObject Super;


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

	/** Saves all stroke data into the specified stream. */
	void saveToStream(QDataStream & a_Stream) const;

	/** Loads all stroke data from the specified stream.
	Throws an exception on failure. */
	void loadFromStream(QDataStream & a_Stream);

	/** Returns the bounding box of all points in this stroke.
	If there are no points, returns an empty QRect. */
	QRectF getBoundingBox() const;


signals:

	/** Emitted when the data in the stroke is changed. */
	void changed();


protected:

	/** Individual points in the stroke. */
	std::vector<Point> m_Points;


	/** Loads all stroke data from the specified stream, after it was decided that it is a version 0 stream.
	Throws an exception on failure. */
	void loadFromStreamV0(QDataStream & a_Stream);
};

typedef std::shared_ptr<Stroke> StrokePtr;
typedef std::vector<StrokePtr> StrokePtrs;





#endif // STROKE_H
