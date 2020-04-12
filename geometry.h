#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#define M_PI 3.14159265358979323846

struct Line;

struct Point
{
	Point() = default;
	Point(const double& X, const double& Y);
	Point& operator= (const Point& b) = default;
	Point(const Point& b) = default;
	bool operator== (const Point& b) const;
	bool operator!= (const Point& b) const;
	void rotate(const Point& center, const double& angle);
	void reflex(const Point& center);
	void reflex(const Line& axis);
	void scale(const Point& center, const double& coefficient);
	double x = 0;
	double y = 0;
};

//#################################################################################

struct Line
{
	Line() = default;
	Line(const Point& M, const Point& N);
	Line(const Point& M, const double& k);
	Line(const double& k, const double& b);
	Line& operator= (const Line& b) = default;
	Line(const Line& b) = default;
	bool operator== (const Line& b) const;
	bool operator!= (const Line& b) const;
	void rotate(const Point& center, const double& angle);
	void reflex(const Point& center);
	void reflex(const Line& axis);
	void scale(const Point& center, const double& coefficient);
	void move(const double& dx, const double& dy);
	bool isParallelTo(const Line& b) const;
	Point m = Point(0, 0);
	Point n = Point(0, 1);
};

//#################################################################################

bool _isRight(const Point& f, const Point& s, const Point& point)
{
	if (f.x == s.x)
	{
		return (f.y < s.y) == (point.x > f.x);
	}
	if (f.y == s.y)
	{
		return (f.x < s.x) == (point.y < f.y);
	}
	double k = (s.y - f.y) / (s.x - f.x);
	return (f.x < s.x) == (point.y < (f.y + (point.x - f.x) * k));
}

bool _isInside(const Point& start, const Point& end, const Point& point)
{
	return Line(start, point).isParallelTo(Line(end, point)) &&
		((point.x <= start.x && point.x >= end.x) || (point.x >= start.x && point.x <= end.x)) &&
		((point.y <= start.y && point.y >= end.y) || (point.y >= start.y && point.y <= end.y));
}

double _distance(const Point& first, const Point& second)
{
	return std::sqrt(std::pow(first.x - second.x, 2) + std::pow(first.y - second.y, 2));
}

double _underLineArea(const Point& first, const Point& second)
{
	if (first.y < 0 || second.y < 0)
		throw;
	return (second.x - first.x) * (first.y + second.y) / 2;
}

static Line _normal(const Line& line, const Point& point)
{
	if (std::abs(line.m.y - line.n.y) < 0.0000001)
	{
		return Line(point, Point(point.x, point.y + 1));
	}
	return Line(point, -(line.m.x - line.n.x) / (line.m.y - line.n.y));
}

static Point _crossing(const Line& first, const Line& second)
{
	if (first.isParallelTo(second))
	{
		if (first == second)
		{
			return first.m;
		}
		throw;
	}
	Point result;
	result.x = (((first.m.y - second.m.y) * (first.n.x - first.m.x) * (second.n.x - second.m.x) -
		(first.n.y - first.m.y) * (second.n.x - second.m.x) * first.m.x + (second.n.y - second.m.y) * (first.n.x - first.m.x) * second.m.x) /
		((second.n.y - second.m.y) * (first.n.x - first.m.x) - (first.n.y - first.m.y) * (second.n.x - second.m.x)));
	if (std::abs(first.n.x - first.m.x) >= 0.0000001)
	{
		result.y = first.m.y - (first.m.x - result.x) * (first.n.y - first.m.y) / (first.n.x - first.m.x);
	}
	else
	{
		result.y = second.m.y - (second.m.x - result.x) * (second.n.y - second.m.y) / (second.n.x - second.m.x);
	}
	return result;
}

//#################################################################################

Point::Point(const double& X, const double& Y)
{
	x = X;
	y = Y;
}
bool Point::operator== (const Point& b) const
{
	return (std::abs(x - b.x) < 0.0000001 && std::abs(y - b.y) < 0.0000001);
}
bool Point::operator!= (const Point& b) const
{
	return !(*this == b);
}
void Point::rotate(const Point& center, const double& angle)
{
	double z = (x - center.x) * std::cos(angle * M_PI / 180) - (y - center.y) * std::sin(angle * M_PI / 180) + center.x;
	y = (x - center.x) * std::sin(angle * M_PI / 180) + (y - center.y) * std::cos(angle * M_PI / 180) + center.y;
	x = z;
}
void Point::reflex(const Point& center)
{
	scale(center, -1);
}
void Point::reflex(const Line& axis)
{
	rotate(_crossing(_normal(axis, *this), axis), 180);
}
void Point::scale(const Point& center, const double& coefficient)
{
	x = coefficient * (x - center.x) + center.x;
	y = coefficient * (y - center.y) + center.y;
}

std::ostream& operator<< (std::ostream& os, const Point& a)
{
	std::cout << "(" << a.x << ", " << a.y << ")";
	return os;
}

std::istream& operator>> (std::istream& i, Point& a)
{
	std::cin >> a.x >> a.y;
	return i;
}

//#################################################################################

Line::Line(const Point& M, const Point& N)
{
	m = M;
	n = N;
}
Line::Line(const Point& M, const double& k)
{
	m = M;
	n = Point(M.x + 1, M.y + k);
}
Line::Line(const double& k, const double& b)
{
	m = Point(0, b);
	n = Point(1, k + b);
}
bool Line::operator== (const Line& b) const
{
	return isParallelTo(b) && (m == b.n ? true : isParallelTo(Line(m, b.n)));
}
bool Line::operator!= (const Line& b) const
{
	return !(*this == b);
}
void Line::rotate(const Point& center, const double& angle)
{
	m.rotate(center, angle);
	n.rotate(center, angle);
}
void Line::reflex(const Point& center)
{
	scale(center, -1);
}
void Line::reflex(const Line& axis)
{
	n.reflex(axis);
	m.reflex(axis);
}
void Line::scale(const Point& center, const double& coefficient)
{
	m.scale(center, coefficient);
	n.scale(center, coefficient);
}
bool Line::isParallelTo(const Line& b) const
{
	return std::abs((m.y - n.y) * (b.m.x - b.n.x) - (b.m.y - b.n.y) * (m.x - n.x)) < 0.0000001;
}
void Line::move(const double& dx, const double& dy)
{
	n.x += dx;
	m.x += dx;
	n.y += dy;
	m.y += dy;
}

std::ostream& operator<< (std::ostream& os, const Line& a)
{
	std::cout << "{" << a.m << ", " << a.n << "}";
	return os;
}

std::istream& operator>> (std::istream& i, Line& a)
{
	std::cin >> a.m >> a.n;
	return i;
}

//#################################################################################

class Shape
{
public:
	virtual ~Shape() {}
	virtual double perimeter() const = 0;
	virtual double area() const = 0;
	virtual bool operator==(const Shape& another) const = 0;
	bool operator!=(const Shape& another) const { return !(*this == another); }
	virtual bool isCongruentTo(const Shape& another) const = 0;
	virtual bool isSimilarTo(const Shape& another) const = 0;
	virtual bool containsPoint(const Point& point) const = 0;
	virtual void rotate(const Point& center, double angle) = 0;
	virtual void reflex(const Point& center) = 0;
	virtual void reflex(const Line& axis) = 0;
	virtual void scale(const Point& center, const double& coefficient) = 0;
};

//#################################################################################

class Polygon : public Shape
{
public:
	~Polygon() = default;
	Polygon(const std::vector<Point>& circle) : vertexes(circle) { ; }
	template <class... V>
	Polygon(const V& ... points);
	size_t verticesCount() const;
	const std::vector<Point>& getVertices() const;
	bool isConvex() const;
	double perimeter() const;
	double area() const;
	bool operator==(const Shape& another) const;
	bool isCongruentTo(const Shape& another) const;
	bool isSimilarTo(const Shape& another) const;
	bool containsPoint(const Point& point) const;
	void rotate(const Point& center, double angle);
	void reflex(const Point& center);
	void reflex(const Line& axis);
	void scale(const Point& center, const double& coefficient);
protected:
	std::vector<Point> vertexes;
private:
	bool isSimilarWithCoefficient(const Polygon& another, const double& c) const;
	template <class V, class... Vs>
	void push(const V& first, const Vs& ... other);
	void push() { ; }
};

template <class... V>
Polygon::Polygon(const V& ... points)
{
	push(points...);
}

template <class V, class... Vs>
void Polygon::push(const V& first, const Vs& ... other)
{
	vertexes.push_back(first);
	push(other...);
}

bool Polygon::operator==(const Shape& another) const
{
	const Polygon* anotherPolygon = dynamic_cast<const Polygon*>(&another);
	if (anotherPolygon == nullptr)
		return false;
	if (vertexes.size() != anotherPolygon->vertexes.size())
	{
		return false;
	}
	size_t matching = vertexes.size();
	for (size_t i = 0; i < vertexes.size(); i++)
	{
		if (vertexes[0] == anotherPolygon->vertexes[i])
		{
			matching = i;
			break;
		}
	}
	if (matching == vertexes.size())
	{
		return false;
	}
	for (size_t i = 0, j = matching; i < vertexes.size(); i++, j++)
	{
		if (j >= vertexes.size())
		{
			j %= vertexes.size();
		}
		if (vertexes[i] != anotherPolygon->vertexes[j])
		{
			break;
		}
		if (i == vertexes.size() - 1)
		{
			return true;
		}
	}
	for (size_t i = 0; i < vertexes.size(); i++, matching--)
	{
		if (vertexes[i] != anotherPolygon->vertexes[matching])
		{
			break;
		}
		if (i == vertexes.size() - 1)
		{
			return true;
		}
		if (matching == 0)
			matching = vertexes.size();
	}
	return false;
}

bool Polygon::isCongruentTo(const Shape& another) const
{
	const Polygon* anotherPolygon = dynamic_cast<const Polygon*>(&another);
	if (anotherPolygon == nullptr)
		return false;
	return isSimilarWithCoefficient(*anotherPolygon, 1);
}

bool Polygon::isSimilarTo(const Shape& another) const
{
	const Polygon* anotherPolygon = dynamic_cast<const Polygon*>(&another);
	if (anotherPolygon == nullptr)
		return false;
	if (vertexes.size() != anotherPolygon->vertexes.size())
	{
		return false;
	}
	double maxThis = _distance(vertexes[vertexes.size() - 1], vertexes[0]);
	double maxAnother = _distance(anotherPolygon->vertexes[vertexes.size() - 1], anotherPolygon->vertexes[0]);
	for (size_t i = 0; i < vertexes.size() - 1; i++)
	{
		maxThis = maxThis < _distance(vertexes[i], vertexes[i + 1]) ? _distance(vertexes[i], vertexes[i + 1]) : maxThis;
		maxAnother = maxAnother < _distance(anotherPolygon->vertexes[i], anotherPolygon->vertexes[i + 1]) ?
			_distance(anotherPolygon->vertexes[i], anotherPolygon->vertexes[i + 1]) : maxAnother;
	}
	return isSimilarWithCoefficient(*anotherPolygon, maxAnother / maxThis);
}

bool Polygon::isSimilarWithCoefficient(const Polygon& another, const double& c) const
{
	if (vertexes.size() != another.vertexes.size())
	{
		return false;
	}
	for (size_t i = 0; i < vertexes.size(); i++)
	{
		if (std::abs(_distance(vertexes[0], vertexes[1]) * c - _distance(another.vertexes[i], another.vertexes[(i + 1) % vertexes.size()])) < 0.0000001)
		{
			for (size_t j = 2; j < vertexes.size(); j++)
			{
				if (std::abs(_distance(vertexes[0], vertexes[j]) * c -
					_distance(another.vertexes[i], another.vertexes[(i + j) % vertexes.size()])) < 0.0000001 &&
					std::abs(_distance(vertexes[1], vertexes[j]) * c -
						_distance(another.vertexes[(i + 1) % vertexes.size()], another.vertexes[(i + j) % vertexes.size()])) < 0.0000001)
				{
					if (j == vertexes.size() - 1)
					{
						return true;
					}
				}
				else
					break;
			}
			for (size_t j = 2; j < vertexes.size(); j++)
			{
				if (std::abs(_distance(vertexes[1], vertexes[j]) * c -
					_distance(another.vertexes[i], another.vertexes[(i + 1 + vertexes.size() - j) % vertexes.size()])) < 0.0000001 &&
					std::abs(_distance(vertexes[0], vertexes[j]) * c -
						_distance(another.vertexes[(i + 1) % vertexes.size()], another.vertexes[(i + 1 + vertexes.size() - j) % vertexes.size()])) < 0.0000001)
				{
					if (j == vertexes.size() - 1)
					{
						return true;
					}
				}
				else
					break;
			}
		}
	}
	return false;
}

bool Polygon::containsPoint(const Point& point) const
{
	Point cross;
	Line line(point, Point(point.x, point.y + 1));
	if (_isInside(vertexes[0], vertexes[vertexes.size() - 1], point))
		return true;
	int crossingNumber = 0;
	size_t i = 0;
	if (!line.isParallelTo(Line(vertexes[vertexes.size() - 1], vertexes[0])))
	{
		cross = _crossing(Line(vertexes[vertexes.size() - 1], vertexes[0]), line);
		if (!((cross == vertexes[0] && vertexes[0].y <= vertexes[1].y && vertexes[0].y < vertexes[vertexes.size() - 1].y) ||
			(cross == vertexes[vertexes.size() - 1] && vertexes[vertexes.size() - 1].y < vertexes[vertexes.size() - 2].y &&
				vertexes[vertexes.size() - 1].y <= vertexes[0].y)))
		{
			if (cross.y >= point.y && _isInside(vertexes[vertexes.size() - 1], vertexes[0], cross))
			{
				crossingNumber++;
				if (cross == vertexes[0])
					i++;
			}
		}
	}
	for (; i < vertexes.size() - 1; i++)
	{
		if (_isInside(vertexes[i + 1], vertexes[i], point))
			return true;
		if (!line.isParallelTo(Line(vertexes[i], vertexes[i + 1])))
		{
			cross = _crossing(Line(vertexes[i], vertexes[i + 1]), line);
			if (!((cross == vertexes[i + 1] && vertexes[i + 1].y <= vertexes[(i + 2) % vertexes.size()].y && vertexes[i + 1].y < vertexes[i].y) ||
				(cross == vertexes[i] && vertexes[i].y < vertexes[i == 0 ? vertexes.size() - 1 : i - 1].y && vertexes[i].y <= vertexes[i + 1].y)))
			{
				if (cross.y >= point.y && _isInside(vertexes[i], vertexes[i + 1], cross))
				{
					crossingNumber++;
					if (cross == vertexes[i + 1])
						i++;
				}

			}
		}
	}
	return crossingNumber % 2 != 0;
}

size_t Polygon::verticesCount() const
{
	return vertexes.size();
}

const std::vector<Point>& Polygon::getVertices() const
{
	return vertexes;
}

bool Polygon::isConvex() const
{
	bool right = _isRight(vertexes[vertexes.size() - 1], vertexes[0], vertexes[1]);
	for (size_t i = 1; i < vertexes.size(); i++)
	{
		if (_isRight(vertexes[i - 1], vertexes[i], vertexes[(i + 1) % vertexes.size()]) != right)
		{
			return false;
		}
	}
	return true;
}

double Polygon::perimeter() const
{
	double result = _distance(vertexes[vertexes.size() - 1], vertexes[0]);
	for (size_t i = 1; i < vertexes.size(); i++)
	{
		result += _distance(vertexes[i - 1], vertexes[i]);
	}
	return result;
}

double Polygon::area() const
{
	double minY = 0;
	for (size_t i = 0; i < vertexes.size(); i++)
	{
		minY = vertexes[i].y < minY ? vertexes[i].y : minY;
	}
	Point vFirst(vertexes[vertexes.size() - 1].x, vertexes[vertexes.size() - 1].y - minY);
	Point vSecond(vertexes[0].x, vertexes[0].y - minY);
	double result = _underLineArea(vFirst, vSecond);
	for (size_t i = 1; i < vertexes.size(); i++)
	{
		Point vFirst(vertexes[i - 1].x, vertexes[i - 1].y - minY);
		Point vSecond(vertexes[i].x, vertexes[i].y - minY);
		result += _underLineArea(vFirst, vSecond);
	}
	return std::abs(result);
}

void Polygon::rotate(const Point& center, double angle)
{
	for (size_t i = 0; i < vertexes.size(); i++)
	{
		vertexes[i].rotate(center, angle);
	}
}

void Polygon::reflex(const Point& center)
{
	scale(center, -1);
}

void Polygon::reflex(const Line& axis)
{
	for (size_t i = 0; i < vertexes.size(); i++)
	{
		vertexes[i].reflex(axis);
	}
}

void Polygon::scale(const Point& center, const double& coefficient)
{
	for (size_t i = 0; i < vertexes.size(); i++)
	{
		vertexes[i].scale(center, coefficient);
	}
}

//#################################################################################

class Ellipse : public Shape
{
public:
	~Ellipse() = default;
	Ellipse() = default;
	Ellipse(const Point& fFirst, const Point& fSecond, const double& dist);
	std::pair<Point, Point> focuses() const;
	std::pair<Line, Line> directrices() const;
	double eccentricity() const;
	Point center() const;
	double perimeter() const;
	double area() const;
	bool operator== (const Shape& other) const;
	bool isCongruentTo(const Shape& other) const;
	bool isSimilarTo(const Shape& other) const;
	bool containsPoint(const Point& point) const;
	void rotate(const Point& center, double angle);
	void reflex(const Point& center);
	void reflex(const Line& axis);
	void scale(const Point& center, const double& coefficient);
protected:
	Point fF;
	Point fS;
	double a = 1;
	double b = 1;
};

Ellipse::Ellipse(const Point& fFirst, const Point& fSecond, const double& dist)
{
	fF = fFirst;
	fS = fSecond;
	a = dist / 2;
	b = std::sqrt(a * a - std::pow(_distance(fF, fS) / 2, 2));
}

std::pair<Point, Point> Ellipse::focuses() const
{
	return std::pair<Point, Point>(fF, fS);
}

std::pair<Line, Line> Ellipse::directrices() const
{
	Line dF = Line(Point(b * b / (a * eccentricity() * (1 + eccentricity())), 0), Point(0, 0));
	Line dS = dF;
	dS.reflex(Point(0, 0));
	Point d(fS.x - fF.x, fS.y - fF.y);
	double angle = std::acos(d.x / std::sqrt(d.x * d.x + d.y * d.y));
	if (_isRight(Point(0, 0), Point(1, 0), d))
	{
		angle *= -1;
	}
	dF.rotate(Point(0, 0), angle);
	dS.rotate(Point(0, 0), angle);
	dF.move(center().x, center().y);
	dS.move(center().x, center().y);
	return std::pair<Line, Line>(dF, dS);
}

double Ellipse::eccentricity() const
{
	return std::sqrt(1 - std::pow(b / a, 2));
}

Point Ellipse::center() const
{
	return Point((fF.x + fS.x) / 2, (fF.y + fS.y) / 2);
}

double Ellipse::area() const
{
	return M_PI * a * b;
}

double Ellipse::perimeter() const
{
	return M_PI * (3 * (a + b) - std::sqrt((3 * a + b) * (3 * b + a)));
}

void Ellipse::rotate(const Point& center, double angle)
{
	fF.rotate(center, angle);
	fS.rotate(center, angle);
}

void Ellipse::reflex(const Point& center)
{
	fF.reflex(center);
	fS.reflex(center);
}

void Ellipse::reflex(const Line& axis)
{
	fF.reflex(axis);
	fS.reflex(axis);
}

void Ellipse::scale(const Point& center, const double& coefficient)
{
	fF.scale(center, coefficient);
	fS.scale(center, coefficient);
	a *= std::abs(coefficient);
	b *= std::abs(coefficient);
}

bool Ellipse::operator==(const Shape& another) const
{
	const Ellipse* anotherEllipse = dynamic_cast<const Ellipse*>(&another);
	if (anotherEllipse == nullptr)
		return false;
	if (std::abs(a - anotherEllipse->a) >= 0.0000001)
	{
		return false;
	}
	if (fF == anotherEllipse->fF)
	{
		return fS == anotherEllipse->fS;
	}
	else
	{
		return fF == anotherEllipse->fS && fS == anotherEllipse->fF;
	}
}

bool Ellipse::isCongruentTo(const Shape& another) const
{
	const Ellipse* anotherEllipse = dynamic_cast<const Ellipse*>(&another);
	if (anotherEllipse == nullptr)
		return false;
	return std::abs(a - anotherEllipse->a) < 0.0000001 && std::abs(b - anotherEllipse->b) < 0.0000001;
}

bool Ellipse::isSimilarTo(const Shape& another) const
{
	const Ellipse* anotherEllipse = dynamic_cast<const Ellipse*>(&another);
	if (anotherEllipse == nullptr)
		return false;
	return std::abs(a * anotherEllipse->b - b * anotherEllipse->a) < 0.0000001;
}

bool Ellipse::containsPoint(const Point& point) const
{
	return _distance(point, fF) + _distance(point, fS) <= 2 * a + 0.0000001;
}

//#################################################################################

class Circle : public Ellipse
{
public:
	~Circle() { ; }
	Circle() : Ellipse() {}
	Circle(const Point& center, const double& radius) : Ellipse(center, center, 2 * radius) {};
	std::pair<Line, Line> directrices() const = delete;
	double radius() const;
};

double Circle::radius() const
{
	return a;
}

//#################################################################################

class Rectangle : public Polygon
{
public:
	Rectangle(const Point& dF, const Point& dS, double coefficient);
	Point center() const;
	std::pair<Line, Line> diagonals() const;
};

Rectangle::Rectangle(const Point& dF, const Point& dS, double coefficient) : Polygon(dF, dS, dS, dF)
{
	if (coefficient < 1)
	{
		coefficient = 1 / coefficient;
	}
	vertexes[1].scale(dF, 1 / std::sqrt(coefficient * coefficient + 1));
	vertexes[1].rotate(dF, 180 / M_PI * std::asin(coefficient / std::sqrt(coefficient * coefficient + 1)));
	vertexes[3] = vertexes[1];
	vertexes[3].reflex(center());
}

Point Rectangle::center() const
{
	return Point((vertexes[0].x + vertexes[2].x) / 2, (vertexes[0].y + vertexes[2].y) / 2);
}

std::pair<Line, Line> Rectangle::diagonals() const
{
	return std::pair<Line, Line>(Line(vertexes[0], vertexes[2]), Line(vertexes[1], vertexes[3]));
}

//#################################################################################

class Square : public Rectangle
{
public:
	Square(const Point& a, const Point& b) : Rectangle(a, b, 1) { ; }
	Circle circumscribedCircle() const;
	Circle inscribedCircle() const;
};

Circle Square::circumscribedCircle() const
{
	return Circle(center(), _distance(vertexes[2], vertexes[0]) / 2);
}

Circle Square::inscribedCircle() const
{
	return Circle(center(), _distance(vertexes[1], vertexes[0]) / 2);
}

//#################################################################################

class Triangle : public Polygon
{
public:
	Triangle(const Point& first, const Point& second, const Point& third) : Polygon(first, second, third) {}
	Circle circumscribedCircle() const;
	Circle inscribedCircle() const;
	Point centroid() const;
	Point incenter() const;
	Point circumcenter() const;
	Point orthocenter() const;
	Line EulerLine() const;
	Circle ninePointsCircle() const;
};

Point Triangle::incenter() const
{
	double a = _distance(vertexes[1], vertexes[2]);
	double b = _distance(vertexes[0], vertexes[2]);
	double c = _distance(vertexes[1], vertexes[0]);
	return Point((a * vertexes[0].x + b * vertexes[1].x + c * vertexes[2].x) / (a + b + c),
		(a * vertexes[0].y + b * vertexes[1].y + c * vertexes[2].y) / (a + b + c));
}

Point Triangle::centroid() const
{
	return _crossing(Line(vertexes[0], Point((vertexes[1].x + vertexes[2].x) / 2, (vertexes[1].y + vertexes[2].y) / 2)),
		Line(vertexes[1], Point((vertexes[0].x + vertexes[2].x) / 2, (vertexes[0].y + vertexes[2].y) / 2)));
}

Point Triangle::orthocenter() const
{
	return _crossing(_normal(Line(vertexes[1], vertexes[2]), vertexes[0]), _normal(Line(vertexes[0], vertexes[2]), vertexes[1]));
}

Point Triangle::circumcenter() const
{
	double a = pow(vertexes[0].x, 2) + pow(vertexes[0].y, 2);
	double b = pow(vertexes[1].x, 2) + pow(vertexes[1].y, 2);
	double c = pow(vertexes[2].x, 2) + pow(vertexes[2].y, 2);
	double D = 2 * (vertexes[0].x * (vertexes[1].y - vertexes[2].y) + vertexes[1].x * (vertexes[2].y - vertexes[0].y)
		+ vertexes[2].x * (vertexes[0].y - vertexes[1].y));
	return Point((a * (vertexes[1].y - vertexes[2].y) + b * (vertexes[2].y - vertexes[0].y) + c * (vertexes[0].y - vertexes[1].y)) / D,
		(a * (vertexes[2].x - vertexes[1].x) + b * (vertexes[0].x - vertexes[2].x) + c * (vertexes[1].x - vertexes[0].x)) / D);
}

Circle Triangle::circumscribedCircle() const
{
	return Circle(circumcenter(),
		_distance(vertexes[0], vertexes[1]) * _distance(vertexes[2], vertexes[1]) * _distance(vertexes[0], vertexes[2]) / (4 * area()));
}

Circle Triangle::inscribedCircle() const
{
	return Circle(incenter(),
		2 * area() / perimeter());
}

Line Triangle::EulerLine() const
{
	return Line(orthocenter(), circumcenter());
}

Circle Triangle::ninePointsCircle() const
{
	Circle result = circumscribedCircle();
	result.scale(orthocenter(), 1. / 2);
	return result;
}