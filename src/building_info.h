#pragma once

#include <opencv2/opencv.hpp>

struct BuildingInfo {
	std::string name;
	cv::Mat mask;
	cv::Point upper_left;
	cv::Point lower_right;
	cv::Point centroid;
	int pixel_area;
	std::vector<std::string> shape_descriptions;
	std::vector<std::string> absolute_space_descriptions;
	std::vector<std::string> relative_space_descriptions;
	std::vector<cv::Point> contour;
};