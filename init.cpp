#include "init.h"

Vehicle init(const Json::Value &vpr_task_root, const int &i, int N, int M)
{
    	std::string _id = vpr_task_root["vehicles"][i]["id"].asString();
	long long _capacity = vpr_task_root["vehicles"][i]["capacity"].asUInt();
	long long _distance_price = vpr_task_root["vehicles"][i]["distance_price"].asUInt();
	long long _coord_x = vpr_task_root["vehicles"][i]["coord"]["x"].asUInt();
	long long _coord_y = vpr_task_root["vehicles"][i]["coord"]["y"].asUInt();
	Vehicle temp(_id, _capacity, _distance_price, _coord_x, _coord_y, N, M);
	return temp;
	
}
