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
		restrict_color_ = cv::Vec3b(128, 128, 128);
		map_.copyTo(shown_);
		cv::namedWindow(kInteractive, 1);
		cv::setMouseCallback(kInteractive, handle_event, this);
		cv::imshow(kInteractive, get_shown());
	}
	void update() { cv::imshow(kInteractive, shown_); }
	cv::Mat get_shown() { return shown_; }
	void clear() { map_.copyTo(shown_);	}

	int add_point_cloud(const BuildingInfo& source, cv::Vec3b color);
	bool is_valid_point(cv::Point p);
	bool addable_neighbor(cv::Point p, cv::Vec3b color);
	BuildingInfo create_fake_building(cv::Point point_source);

	cv::Mat map_;
	cv::Mat shown_;

	ReductionMatrixHolder reductions_;

	std::map<int, BuildingInfo>* buildings_;
	BuildingInfo s_;
	BuildingInfo t_;
	InteractiveState state_;
	cv::Vec3b restrict_color_;
};

void map_interactive(std::string mapFileName, std::map<int, BuildingInfo>* buildings, ReductionMatrixHolder reductions);