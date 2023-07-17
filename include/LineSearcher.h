#pragma once

#include <opencv2/opencv.hpp>

#include <limits>

using namespace cv;
using namespace std;

class LineSearcher
{
private:
	Mat m_image;

	int m_numTrials = 0;
	int m_numCandidates = 1;
	double m_threshold = numeric_limits<double>::max();

public:
	LineSearcher() = default;

};

