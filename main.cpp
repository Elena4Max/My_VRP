#include <fstream>
#include <ctime>
#include <exception>
#include "display.h"
#include "init.h"

long long COST;
extern long long k;

int main()
{

    	long long start_time = clock();
	Json::Reader reader;
	Json::Value vpr_task_root;

	try { 
		std::ifstream vpr_task_file("vrp_task1.json");
    		vpr_task_file >> vpr_task_root;
	}
	catch(std::exception& e) {
		printf("Something wrong!\n");
		return 0;
	}
	int i = 0, j = 0, M = 0, N = 0;

	std::vector< Vehicle > vehicles;

	_bonus_map.resize(200);
	for(long long i = 0; i < 200; i++) {
		_bonus_map[i].resize(200);
	}
	while(!vpr_task_root["vehicles"][i]["id"].isNull()) {
	    	display_vehicles(vpr_task_root, i, N, M);
		i++;
	}
	while(!vpr_task_root["shipments"][j]["id"].isNull()) {
	    	display_shipments(vpr_task_root, j, N, M);
		j++;
	}
#ifdef _BONUS1
	try { 
		print_bonus_map(N + 1, M + 1);
	}
	catch(const char *e) {
		printf("Something wrong!\n%s\n", e);
		return 0;
	}
#endif
	for(long long v = 0; v < i; v++) {
		vehicles.push_back( init(vpr_task_root, v, N + 1, M + 1) );	
	}	
for(k = 0; k < 2 * j; k++) { 
	for(int s = 0; s < j; s++) {
		Shipment shipment;
		shipment._id = vpr_task_root["shipments"][s]["id"].asString();;
		shipment._pickup_x = vpr_task_root["shipments"][s]["pickup"]["x"].asUInt();
		shipment._pickup_y = vpr_task_root["shipments"][s]["pickup"]["y"].asUInt();
		shipment._delivery_x = vpr_task_root["shipments"][s]["delivery"]["x"].asUInt();
		shipment._delivery_y = vpr_task_root["shipments"][s]["delivery"]["y"].asUInt();
		shipment._capacityDemand = vpr_task_root["shipments"][s]["capacityDemand"].asUInt();
		shipment._cost = 0;

		for(long long v = 0; v < i; v++) {
			if( vehicles[v].get_capacity() >= shipment._capacityDemand ) {
				vehicles[v].add_task(shipment);
			}	
		}	
	}	
////////////////////////////////////////////////////////////////////////////////////////////////
  	std::vector< std::thread > tcalc_map;
	for (long long v = 0; v < i; v++) {
		tcalc_map.push_back( std::thread(&Vehicle::calc_map, &vehicles[v], vehicles[v]._coord_x, vehicles[v]._coord_y) );
	}
	for (auto & th : tcalc_map)
		th.join();
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _BONUS2
	for (long long v = 0; v < i; v++) {
		vehicles[v].calc_shipments_pickup();
	}
#else
  	std::vector< std::thread > tcalc_shipments_pickup;
	for (long long v = 0; v < i; v++) {
		tcalc_shipments_pickup.push_back( std::thread(&Vehicle::calc_shipments_pickup, &vehicles[v]) );
	}
	for (auto & th : tcalc_shipments_pickup)
		th.join();
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _BONUS2
	for (long long v = 0; v < i; v++) {
		vehicles[v].calc_shipments_delivery();
	}
#else
  	std::vector< std::thread > tcalc_shipments_delivery;
	for (long long v = 0; v < i; v++) {
		tcalc_shipments_delivery.push_back( std::thread(&Vehicle::calc_shipments_delivery, &vehicles[v]) );
	}
	for (auto & th : tcalc_shipments_delivery)
		th.join();
#endif
////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector< std::vector< long long > > _distance;
	_distance.resize(i);
	for(long long v = 0; v < i; v++) {
		_distance[v].resize(j);
		for(long long s = 0; s < j; s++) {
			_distance[v][s] = std::min(	vehicles[v].shipment_cost(std::to_string(s)),
							vehicles[v].delivery_cost(std::to_string(s)));	
		}	
	}
////////////////////////////////////////////////////////////////////////////////////////////////
//greedy
////////////////////////////////////////////////////////////////////////////////////////////////
	long long mini = INF;
	std::pair< long long, long long > coord_to_go;
	for(long long v = 0; v < i; v++) {
		for(long long s = 0; s < j; s++) {
			if(_distance[v][s] < mini) {
				mini = _distance[v][s];
				coord_to_go = std::make_pair(v, s);
			}
		}	
	}
////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
	for(long long v = 0; v < i; v++) {
		if(vehicles[v]._id == std::to_string(coord_to_go.first + 1)) {
			Shipment s = vehicles[v].find_task_in_shipments(coord_to_go.second);
			Shipment d = vehicles[v].find_task_in_delivery(coord_to_go.second);
			if(s._id != "NULL") {
				COST += vehicles[v].pick_up(coord_to_go.second);
			}
			else {
				COST += vehicles[v].delivery(coord_to_go.second);
			}
		}
	}
	for(long long v = 0; v < i; v++) {
		vehicles[v].clear_cost();
	}
}
	for(long long v = 0; v < i; v++) {
		COST += vehicles[v].go_home();
	}
////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector <Routes> routes;
	for(long long v = 0; v < i; v++) {
		if(vehicles[v]._rout._act.size() != 0) {
			routes.push_back(vehicles[v]._rout);
		}
	}
	Json::Value root;
    	root["cost"] = COST;
	Json::Value rout;
	int idx = 0;
	for (const auto& r : routes) {
		Json::Value routInfo;
		routInfo["end"] = r._end;
		routInfo["start"] = r._start;
		routInfo["vehicleId"] = r._vehicleId;
		int idy = 0;
		Json::Value act;
		for (const auto& a : r._act) {
			Json::Value actInfo;
			actInfo["endTime"] = a._endTime;
			actInfo["arrTime"] = a._arrTime;
			actInfo["jobId"] = a._jobId;
			actInfo["type"] = a._type;
			act[idy] = actInfo;
			idy++;
		}
		routInfo["act"] = act;
		rout[idx] = routInfo;
		idx++;
	}
    	root["routes"] = rout;

	Json::StyledWriter writer;
	std::string strJson = writer.write(root);
	std::ofstream out("vrp_task_solution.json", std::ios::app);
	if (out.is_open())
	{
		out << strJson << std::endl;
	}
	out.close();
////////////////////////////////////////////////////////////////////////////////////////////////
    	long long end_time = clock();
	long long search_time = end_time - start_time;
	printf("search_time = %f\n", search_time / 1000.0);
	return 0;
}       

