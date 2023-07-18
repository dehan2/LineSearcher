#include <Point2.hpp>
#include <LineSearcher.hpp>
#include  <opencv2/opencv.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include <list>
#include <iostream>
#include <fstream>

using namespace std;

list<Point2> generate_points_around_random_line(const int& radius, const int& numPoints, const string& fileName);
list<Point2> load_points_from_file(const string& fileName);

int main(int argv, char** argc)
{
	const int radius = 300;
	const int numPoints = 1000;
	auto points = generate_points_around_random_line(radius, numPoints, "points.txt");
	//auto points = load_points_from_file("points.txt");

	// Draw the points.
	cv::Mat image(2 * radius, 2 * radius, CV_8U, 255);
	for (auto& point : points)
	{
		image.at<uchar>(point.y(), point.x()) = 0;
	}

	// Compute the trend line.
	LineSearcher lineSearcher;
	lineSearcher.set_points(points);
	lineSearcher.compute_trend_line_by_RANSAC(points, 100, 10);

	// Draw the trend line.
	auto trendLine = lineSearcher.get_trend_line();
	
	auto tMin_x = -trendLine[0].x() / (trendLine[1].x() - trendLine[0].x());
	auto tMin_y = -trendLine[0].y() / (trendLine[1].y() - trendLine[0].y());
	auto tMax_x = (2 * radius - trendLine[0].x()) / (trendLine[1].x() - trendLine[0].x());
	auto tMax_y = (2 * radius - trendLine[0].y()) / (trendLine[1].y() - trendLine[0].y());

	double tMin = 0.0;
	double tMax = 0.0;
	if (tMin_x < 0)
		tMin = max(tMin_x, tMin_y);
	else
		tMin = min(tMin_x, tMin_y);

	if (tMax_x < 0)
		tMax = max(tMax_x, tMax_y);
	else
		tMax = min(tMax_x, tMax_y);

	Point2 pMin = tMin * trendLine[1] + (1 - tMin) * trendLine[0];
	Point2 pMax = tMax * trendLine[1] + (1 - tMax) * trendLine[0];
	cout << "tMin: " << tMin << ", tMax: " << tMax << ", pMin: [" << pMin.x() << ", " << pMin.y() << "], pMax: [" << pMax.x() << ", " << pMax.y() << "]" << endl;

	cv::line(image, cv::Point(pMin.x(), pMin.y()), cv::Point(pMax.x(), pMax.y()), cv::Scalar(0), 2);

	cv::imshow("img", image);
	cv::waitKey(0);
}



list<Point2> generate_points_around_random_line(const int& radius, const int& numPoints, const string& fileName)
{
	// Create a list of points
	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<double> angleDistribution(-M_PI, M_PI);
	double lineAngle = angleDistribution(gen);

	uniform_real_distribution<double> centerDistribution(-radius, radius);
	normal_distribution<double> deviationDistribution(0, radius / 10);

	list<Point2> points;
	while (points.size() < numPoints)
	{
		double centerParameter = centerDistribution(gen);
		Point2 center(radius + centerParameter * cos(lineAngle), radius + centerParameter * sin(lineAngle));
		double residual = deviationDistribution(gen);
		Point2 generatedPoint(round(center.x() + residual * cos(lineAngle + M_PI_2)), round(center.y() + residual * sin(lineAngle + M_PI_2)));

		// Judge whether the generated point is in the image.
		if (generatedPoint.x() >= 0 && generatedPoint.x() <= 2 * radius && generatedPoint.y() >= 0 && generatedPoint.y() <= 2 * radius)
		{
			// Check whether the generated point is already in the list.
			bool isAlreadyInList = false;
			for (auto& point : points)
			{
				if (point == generatedPoint)
				{
					isAlreadyInList = true;
					break;
				}
			}

			if (isAlreadyInList == false)
				points.push_back(generatedPoint);
		}
	}

	fstream file(fileName, ios::out);
	for (auto& point : points)
	{
		file << point.x() << " " << point.y() << endl;
	}

	return points;
}



list<Point2> load_points_from_file(const string& fileName)
{
	list<Point2> points;
	fstream file(fileName, ios::in);
	while (!file.eof())
	{
		double x, y;
		file >> x >> y;
		points.push_back(Point2(x, y));
	}

	return points;
}