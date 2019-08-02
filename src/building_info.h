#pragma once

#include <opencv2/opencv.hpp>

struct BuildingInfo {
	std::string name = "";
	int blob_index = -1;
	cv::Mat mask;               // Binary image
	cv::Rect bounding_box;
	cv::Point upper_left;
	cv::Point lower_right;
	cv::Point centroid;
	int pixel_area = 0;
	std::vector<std::string> shape_descriptions;
	std::vector<std::string> absolute_space_descriptions;
	
	std::vector<std::string> north_of;
	std::vector<std::string> south_of;
	std::vector<std::string> east_of;
	std::vector<std::string> west_of;
	std::vector<std::string> near;

	std::vector<cv::Point> contour;
	std::vector<cv::Vec4i> defects;
	std::vector<int> hull;
	int corners = 0;

	double macro_score = 0.0;
	std::string macro_descriptor = "";
};

struct ReductionMatrixHolder {
	cv::Mat near;
	cv::Mat north;
	cv::Mat south;
	cv::Mat east;
	cv::Mat west;
};