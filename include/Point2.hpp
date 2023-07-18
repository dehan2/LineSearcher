#pragma once

class Point2
{
private:
	double m_x = 0.0;
	double m_y = 0.0;

public:
	Point2() = default;
	Point2(double x, double y) : m_x(x), m_y(y) {}
	Point2(const Point2& rhs) { copy(rhs); }
	~Point2() = default;

	Point2& operator=(const Point2& rhs);

	Point2 operator+(const Point2& rhs) const;
	Point2 operator-(const Point2& rhs) const ;
	Point2 operator*(const double& rhs) const;
	Point2 operator/(const double& rhs) const;

	Point2& operator+=(const Point2& rhs);
	Point2& operator-=(const Point2& rhs);
	Point2& operator*=(const double& rhs);
	Point2& operator/=(const double& rhs);

	friend Point2 operator*(const double& lhs, const Point2& rhs);

	const double& x() const { return m_x; }
	const double& y() const { return m_y; }

	void set_x(const double& x) { m_x = x; }
	void set_y(const double& y) { m_y = y; }

	void copy(const Point2& rhs);

	double norm() const { sqrt(pow(m_x, 2) + pow(m_y, 2)); }
	double distance_to(const Point2& rhs) const;
	double dot_product(const Point2& rhs) const { return m_x * rhs.m_x + m_y * rhs.m_y; }
	double cross_product(const Point2& rhs) const { return m_x * rhs.m_y - m_y * rhs.m_x; }
};

