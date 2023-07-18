#pragma once

#include "Point2.hpp"
#include <array>
#include <list>
#include <set>

using namespace std;

class LineSearcher
{
private:
	list<Point2> m_points;
	array<Point2, 2> m_trendLine; // \Parametric (ta + (1-t)b) form

public:
	LineSearcher() = default;
	LineSearcher(const LineSearcher& rhs) { copy(rhs); }
	~LineSearcher() = default;

	LineSearcher& operator=(const LineSearcher& rhs);
	void copy(const LineSearcher& rhs);

	const list<Point2>& get_points() const { return m_points; }
	const array<Point2, 2>& get_trend_line() const { return m_trendLine; }

	void set_points(const list<Point2>& points) { m_points = points; }
	void set_trend_line(const array<Point2, 2>& trendLine) { m_trendLine = trendLine; }

	void compute_trend_line_by_RANSAC(const list<Point2>& points, const int& numTrials, const int& numCandidates);

	

private:
	array<const Point2*, 2> pick_two_random_points(const list<Point2>& points) const;
	double calculate_initial_threshold_by_vertical_line(const list<Point2>& points) const;
	double calculate_residual_against_line(const array<const Point2*, 2>& line, const Point2& point) const;
	double calculate_standard_deviation_of_residuals_against_line(const array<const Point2*, 2>& line, const set<const Point2*>& inliers) const;

};

