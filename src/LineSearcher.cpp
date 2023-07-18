#include "LineSearcher.hpp"
#include "ConstForLineSearcher.hpp"

#include <vector>
#include <random>
#include <chrono>
#include <iostream>

LineSearcher& LineSearcher::operator=(const LineSearcher& rhs)
{
	if (this != &rhs)
		copy(rhs);

	return *this;
}



void LineSearcher::copy(const LineSearcher& rhs)
{
	m_points = rhs.m_points;
	m_trendLine = rhs.m_trendLine;
}



void LineSearcher::compute_trend_line_by_RANSAC(const list<Point2>& points, const int& numTrials, const int& numCandidates)
{
	double threshold = calculate_initial_threshold_by_vertical_line(points);

	pair<array<const Point2*, 2>, int> bestLine; // First: line, Second: number of inliers

	for (int i = 0; i < numTrials; ++i)
	{
		// Randomly select two points.
		auto pickedPoints = pick_two_random_points(points);

		set<const Point2*> inliers;
		for (auto& point : points)
		{
			if (&point != pickedPoints.at(0) && &point != pickedPoints.at(1))
			{
				double residual = calculate_residual_against_line(pickedPoints, point);
				if(residual < threshold)
					inliers.insert(&point);	
			}
		}

		if (inliers.size() > bestLine.second)
		{
			bestLine.first = pickedPoints;
			bestLine.second = inliers.size();

			// Update the threshold.
			double newThreshold = calculate_standard_deviation_of_residuals_against_line(pickedPoints, inliers);
			if (newThreshold < threshold)
			{
				threshold = newThreshold;
			}
		}
	}

	cout<<"RANSAC finish  - inliers of the best estimation: "<<bestLine.second<<" ("<< 100.0* bestLine.second / points.size()<<"%)"<< endl;
	m_trendLine = { *bestLine.first.at(0), *bestLine.first.at(1) };
}



array<const Point2*, 2> LineSearcher::pick_two_random_points(const list<Point2>& points) const
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, points.size() - 1);

	int index1 = dis(gen);
	int index2 = dis(gen);
	while (index2 == index1)
		index2 = dis(gen);

	const Point2* p1 = &*next(points.begin(), index1);
	const Point2* p2 = &*next(points.begin(), index2);

	return { p1, p2 };
}



double LineSearcher::calculate_initial_threshold_by_vertical_line(const list<Point2>& points) const
{
	double xMean = 0.0;
	for (auto& point : points)
	{
		xMean += point.x();
	}

	xMean /= points.size();

	double standardDeviation = 0.0;
	for (auto& point : points)
	{
		standardDeviation += (point.x() - xMean) * (point.x() - xMean);
	}
	standardDeviation /= points.size();
	standardDeviation = sqrt(standardDeviation);

	return THRESHOLD_MULTIPLIER *standardDeviation;
}



double LineSearcher::calculate_residual_against_line(const array<const Point2*, 2>& line, const Point2& point) const
{
	// Calculate the distance between the point and the line.
	Point2 vectorToPoint = point - *line.at(0);
	Point2 directionVector = *line.at(1) - *line.at(0);

	double residual = abs(vectorToPoint.cross_product(directionVector) / directionVector.norm());
	return residual;
}



double LineSearcher::calculate_standard_deviation_of_residuals_against_line(const array<const Point2*, 2>& line, const set<const Point2*>& inliers) const
{
	double standardDeviation = 0.0;

	for (auto& point : inliers)
	{
		if (point != line.at(0) && point != line.at(1))
		{
			double residual = calculate_residual_against_line(line, *point);
			standardDeviation += residual * residual;
		}
	}
	standardDeviation /= inliers.size();

	return THRESHOLD_MULTIPLIER *standardDeviation;
}
