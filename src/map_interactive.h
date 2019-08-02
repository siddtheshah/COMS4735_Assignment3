#pragma once
#include "building_info.h"
#include "utils.h"

const char kInteractive[] = "Columbia Campus Map Interactive";

enum InteractiveState {
	CLEAN,
	FIRST_CLICK,
	SECOND_CLICK,
};

void handle_event(int event, int x, int y, int flags, void* map_interactive);

class MapInteractive {
public:
	MapInteractive(cv::Mat map, std::map<int, BuildingInfo>* buildings, ReductionMatrixHolder reductions) 
		: map_(map), buildings_(buildings), reductions_(reductions) {
		map_.copyTo(shown_);
		cv::namedWindow(kInteractive, 1);
		cv::setMouseCallback(kInteractive, handle_event, this);
		cv::imshow(kInteractive, get_shown());
	}
	void update() { cv::imshow(kInteractive, shown_); }
	cv::Mat get_shown() { return shown_; }
	void clear() { map_.copyTo(shown_);	}

	void add_point_cloud(const BuildingInfo& source, cv::Scalar color);
	BuildingInfo create_fake_building(cv::Point point_source);

	cv::Mat map_;
	cv::Mat shown_;

	ReductionMatrixHolder reductions_;

	std::map<int, BuildingInfo>* buildings_;
	BuildingInfo s_;
	BuildingInfo t_;
	InteractiveState state_;
};

void map_interactive(std::string mapFileName, std::map<int, BuildingInfo>* buildings, ReductionMatrixHolder reductions);