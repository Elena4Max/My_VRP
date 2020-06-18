#include "display.h"

void display_vehicles(const Json::Value &vpr_task_root, const int &i, int &N, int &M)
{
    	std::string _id = vpr_task_root["vehicles"][i]["id"].asString();
	long long _capacity = vpr_task_root["vehicles"][i]["capacity"].asUInt();
	long long _distance_price = vpr_task_root["vehicles"][i]["distance_price"].asUInt();
	long long _coord_x = vpr_task_root["vehicles"][i]["coord"]["x"].asUInt();
	long long _coord_y = vpr_task_root["vehicles"][i]["coord"]["y"].asUInt();
	_bonus_map[_coord_x][_coord_y] = 'V';
	if(M < _coord_x) {
		M = _coord_x;		
	}	
	if(N < _coord_y) {
		N = _coord_y;		
	}
}
void display_shipments(const Json::Value &vpr_task_root, const int &j, int &N, int &M)
{
    	std::string _id = vpr_task_root["shipments"][j]["id"].asString();
	long long _pickup_x = vpr_task_root["shipments"][j]["pickup"]["x"].asUInt();
	long long _pickup_y = vpr_task_root["shipments"][j]["pickup"]["y"].asUInt();
	long long _delivery_x = vpr_task_root["shipments"][j]["delivery"]["x"].asUInt();
	long long _delivery_y = vpr_task_root["shipments"][j]["delivery"]["y"].asUInt();
	long long _capacityDemand = vpr_task_root["shipments"][j]["capacityDemand"].asUInt();
	long long _max_x = std::max(_pickup_x, _delivery_x);
	long long _max_y = std::max(_pickup_y, _delivery_y);
	_bonus_map[_pickup_x][_pickup_y] = 'S';
	_bonus_map[_delivery_x][_delivery_y] = 'D';
	if(M < _max_x) {
		M = _max_x;		
	}	
	if(N < _max_y) {
		N = _max_y;		
	}
}

