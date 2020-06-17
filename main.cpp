#include <iostream>
#include <json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <vector>
#include <list>
#include <thread>
#include <stdio.h>
#include <ctime>

const long long INF = 1e9;
long long COST;
long long TIMER;
long long JOB;

struct Act
{
	std::string _type;
	std::string _jobId;
	long long _arrTime;
	long long _endTime;
};
struct Routes
{
	std::string _vehicleId;
	long long _start;
	long long _end;
	std::vector< Act > _act;
};
struct Point {long long i, j;} point;
struct Shipment
{
	std::string _id;
	long long _pickup_x;
	long long _pickup_y;
	long long _delivery_x;
	long long _delivery_y;
	long long _capacityDemand;
	long long _cost;
	friend bool operator== (const Shipment &s1, const Shipment &s2);
    	friend bool operator!= (const Shipment &s1, const Shipment &s2);
};
bool operator== (const Shipment &s1, const Shipment &s2)
{
    return 	(s1._id == s2._id &&
		s1._pickup_x == s2._pickup_x &&
		s1._pickup_y == s1._pickup_y &&
		s1._delivery_x == s2._delivery_x &&
		s1._delivery_y == s2._delivery_y &&
		s1._capacityDemand == s2._capacityDemand);
}
 
bool operator!= (const Shipment &s1, const Shipment &s2)
{
    return !(s1 == s2);
}
std::list< Shipment > pick_upped;

class Vehicle
{
public:
	Vehicle(std::string id, long long capacity, long long distance_price, long long home_coord_x, long long home_coord_y, long long N, long long M);
	void add_task(const Shipment &shipment);
	void delete_task(const Shipment &shipment);
	const Shipment find_task_in_shipments(const long long shipment_id);
	const Shipment find_task_in_delivery(const long long shipment_id);
	void print();
	void calc_map(long long x, long long y);
	const long long get_capacity();
	void calc_shipments_pickup();
	void calc_shipments_delivery();
	void calc_map_from_pickup_to_delivery();
	void calc_map_home();
	long long _coord_x;
	long long _coord_y;
	std::string _id; 
	long long shipment_cost(std::string id);
	long long delivery_cost(std::string id);
	long long pick_up(long long shipment_id);
	long long delivery(long long shipment_id);
	bool is_shipments_empty();
	bool is_delivery_empty();
	void clear_cost();
	long long go_home();
	Routes _rout;
private:
	const long long _capacity; 
	long long _capacity_picked; 
	const long long _distance_price;
	const long long _home_coord_x;
	const long long _home_coord_y;
	std::vector< std::vector< long long > > _map;
	std::list< Shipment > _shipments;
	std::list< Shipment > _delivery;
	long long _N, _M;
	long long _start, _end;
};

Vehicle::Vehicle(std::string id, long long capacity, long long distance_price, long long home_coord_x, long long home_coord_y, long long N, long long M) :
	_id(id),
	_capacity(capacity), 
	_capacity_picked(0),
	_distance_price(distance_price), 
	_home_coord_x(home_coord_x), 
	_home_coord_y(home_coord_y),
	_coord_x(home_coord_x), 
	_coord_y(home_coord_y),
	_N(N),
	_M(M)
{
	_map.resize(M);
	for(long long i = 0; i < M; i++) {
		_map[i].resize(N);
	}
	_rout._vehicleId = _id;
	_rout._start = -1;
	_rout._end = -1;
}
void Vehicle::add_task(const Shipment &shipment)
{
	for(auto &s : pick_upped) {
		if(s == shipment) {
			return;
		}	
	}
	for(auto &s : _shipments) {
		if(s == shipment) {
			return;
		}	
	}
	 _shipments.push_back(shipment);
}
void Vehicle::delete_task(const Shipment &shipment)
{
	_shipments.remove(shipment);
}
const Shipment Vehicle::find_task_in_shipments(const long long shipment_id)
{
	for(auto &s : _shipments) {
		if(s._id == std::to_string(shipment_id)) {
			return s;
		}	
	}
	Shipment s;
	s._id = "NULL";
	return s;
}
const Shipment Vehicle::find_task_in_delivery(const long long shipment_id) 
{
	for(auto &d : _delivery) {
		if(d._id == std::to_string(shipment_id)) {
			return d;
		}	
	}
	Shipment s;
	s._id = "NULL";
	return s;
}
void Vehicle::print()
{
	std::cout << "-----------------Vehicle------------------" << std::endl;
	std::cout <<    "_id " << _id << " " <<
			"_capacity_picked " << _capacity_picked << " " <<
			"_capacity " << _capacity << " " <<
			"_distance_price " << _distance_price << " " <<
			"_home_coord_x " << _home_coord_x <<   " " <<
			"_home_coord_y " << _home_coord_y << " " <<
			"_coord_x " << _coord_x << " " <<
			"_coord_y " << _coord_y << std::endl;
	std::cout << "-----------------Shipment-----------------" << std::endl;
	for(auto s : _shipments) {
		std::cout << s._id << " cost = " << s._cost << std::endl;
	}
	std::cout << std::endl;
	std::cout << "-----------------Delivery-----------------" << std::endl;
	for(auto d : _delivery) {
		std::cout << d._id << " cost = " << d._cost << std::endl;
	}
	std::cout << std::endl;
	std::cout << "-----------------Map----------------------" << std::endl;
	for(auto m : _map) {
		for(auto mm : m) {
			std::cout << mm << " ";
		}
		std::cout << std::endl;
	}
}
const long long Vehicle::get_capacity()
{
	return (_capacity - _capacity_picked);
}
void Vehicle::calc_map(long long x, long long y)
{
	for(long long i = 0; i < _map.size(); i++) {
		for(long long j = 0; j < _map[i].size(); j++) {
			_map[i][j] = 0;
		}
	}
	_map[x][y] = 1;
	long long head,tail;         
	Point q[_N * _M];      
	long long di[4] = { -1, 0, 1,  0};
	long long dj[4] = {  0, 1, 0, -1};
	head = tail = 0;
	q[tail].i = x;
	q[tail++].j = y;  

	while (head < tail) {
		Point p = q[head++];          
		for (long long k = 0; k < 4; k++) {
			Point newp;
			newp.i = p.i + di[k];
			newp.j = p.j + dj[k];
			if (0 <= newp.i && newp.i < _M && 0 <= newp.j && newp.j < _N)
				if ( _map[newp.i][newp.j] == 0) {
					_map[newp.i][newp.j] = _map[p.i][p.j] + 1;      
					q[tail++] = newp;             
				}
		}
	}
}
void Vehicle::calc_shipments_pickup()
{
	for(auto &s : _shipments) {
		s._cost += 2 * _distance_price * (_map[s._pickup_x][s._pickup_y] - 1);
	}
}

void Vehicle::calc_shipments_delivery()
{
	for(auto &d : _delivery) {
		d._cost += _distance_price*(_map[d._delivery_x][d._delivery_y] - 1);
	}
}

void Vehicle::calc_map_from_pickup_to_delivery()
{
	for(auto &s : _shipments) {
		calc_map(s._pickup_x, s._pickup_y);
		s._cost += _distance_price*_map[s._delivery_x][s._delivery_y];
	}
}

void Vehicle::calc_map_home()
{
	for(auto &s : _shipments) {
		calc_map(s._delivery_x, s._delivery_y);
		s._cost += _distance_price*_map[_home_coord_x][_home_coord_y];
	}
	for(auto &d : _delivery) {
		calc_map(d._delivery_x, d._delivery_y);
		d._cost += _distance_price*_map[_home_coord_x][_home_coord_y];
	}
}
long long Vehicle::shipment_cost(std::string id)
{
	for(auto &s : _shipments) {
		if(s._id == id)
			return s._cost;
	}
	return INF;
}
long long Vehicle::delivery_cost(std::string id)
{
	for(auto &d : _delivery) {
		if(d._id == id)
			return d._cost;
	}
	return INF;
}
long long Vehicle::pick_up(long long shipment_id)
{
	calc_map(_coord_x, _coord_y);
	for(auto &s : _shipments) {
		if(s._id == std::to_string(shipment_id)) {
			_coord_x = s._pickup_x;
			_coord_y = s._pickup_y;
			_capacity_picked += s._capacityDemand;
			_delivery.push_back(s);
			pick_upped.push_back(s);
		}	
	}
	if(_rout._start == -1) {
		_rout._start = TIMER;
	}
	TIMER += _map[_coord_x][_coord_y] - 1;
	Act act;
	act._type = "pickupShipment";
	act._jobId = std::to_string(JOB); JOB++;
	act._arrTime = TIMER; TIMER++;
	act._endTime = TIMER;
	_rout._act.push_back(act);
	return _map[_coord_x][_coord_y] * _distance_price;
}
long long Vehicle::delivery(long long shipment_id)
{
	calc_map(_coord_x, _coord_y);
	for(auto &d : _delivery) {
		if(d._id == std::to_string(shipment_id)) {
			_coord_x = d._delivery_x;
			_coord_y = d._delivery_y;
			_capacity_picked -= d._capacityDemand;
			_delivery.remove(d);
			break;
		}	
	}
	TIMER += _map[_coord_x][_coord_y] - 1;
	Act act;
	act._type = "deliveryShipment";
	act._jobId = std::to_string(JOB); JOB++;
	act._arrTime = TIMER; TIMER++;
	act._endTime = TIMER;
	_rout._act.push_back(act);
	return _map[_coord_x][_coord_y] * _distance_price;
}
bool Vehicle::is_shipments_empty()
{
	return _shipments.empty();
}
bool Vehicle::is_delivery_empty()
{
	return _delivery.empty();
}
void Vehicle::clear_cost()
{
	_shipments.erase(_shipments.begin(),_shipments.end());
	for(auto &d : _delivery) {
		d._cost = 0;
	}
}
long long Vehicle::go_home()
{
	calc_map(_coord_x, _coord_y);
	_rout._end = TIMER;
	return (_map[_home_coord_x][_home_coord_y] - 1) * _distance_price;
}

void display_vehicles(const Json::Value &vpr_task_root, const int &i, int &N, int &M);
void display_shipments(const Json::Value &vpr_task_root, const int &j, int &N, int &M);
Vehicle init(const Json::Value &vpr_task_root, const int &i, int N, int M);

int main()
{
    	long long start_time = clock();
	Json::Reader reader;
	Json::Value vpr_task_root;

	std::ifstream vpr_task_file("vrp_task7.json");
    	vpr_task_file >> vpr_task_root;
	int i = 0, j = 0, M = 0, N = 0;

	std::vector< Vehicle > vehicles;

	while(!vpr_task_root["vehicles"][i]["id"].isNull()) {
	    	display_vehicles(vpr_task_root, i, N, M);
		i++;
	}
	while(!vpr_task_root["shipments"][j]["id"].isNull()) {
	    	display_shipments(vpr_task_root, j, N, M);
		j++;
	}
	for(long long v = 0; v < i; v++) {
		vehicles.push_back( init(vpr_task_root, v, N + 1, M + 1) );	
	}	
for(long long k = 0; k < 2 * j; k++) { 

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

  	std::vector< std::thread > tcalc_shipments_pickup;
	for (long long v = 0; v < i; v++) {
		tcalc_shipments_pickup.push_back( std::thread(&Vehicle::calc_shipments_pickup, &vehicles[v]) );
	}
	for (auto & th : tcalc_shipments_pickup)
		th.join();
////////////////////////////////////////////////////////////////////////////////////////////////
  	std::vector< std::thread > tcalc_shipments_delivery;
	for (long long v = 0; v < i; v++) {
		tcalc_shipments_delivery.push_back( std::thread(&Vehicle::calc_shipments_delivery, &vehicles[v]) );
	}
	for (auto & th : tcalc_shipments_delivery)
		th.join();
////////////////////////////////////////////////////////////////////////////////////////////////
/*
  	std::vector< std::thread > tcalc_map_from_pickup_to_delivery;
	for (long long v = 0; v < i; v++) {
		tcalc_map_from_pickup_to_delivery.push_back( std::thread(&Vehicle::calc_map_from_pickup_to_delivery, &vehicles[v]) );
	}
	for (auto & th : tcalc_map_from_pickup_to_delivery)
		th.join();
*/
////////////////////////////////////////////////////////////////////////////////////////////////
/*
  	std::vector< std::thread > tcalc_map_home;
	for (long long v = 0; v < i; v++) {
		tcalc_map_home.push_back( std::thread(&Vehicle::calc_map_home, &vehicles[v]) );
	}
	for (auto & th : tcalc_map_home)
		th.join();	
*/
	std::vector< std::vector< long long > > _distance;
	_distance.resize(i);
	for(long long v = 0; v < i; v++) {
		_distance[v].resize(j);
		for(long long s = 0; s < j; s++) {
			_distance[v][s] = std::min(	vehicles[v].shipment_cost(std::to_string(s)),
							vehicles[v].delivery_cost(std::to_string(s)));	
		}	
	}
/*
	std::cout << "-----------------Distance-----------------" << std::endl;

	for(long long v = 0; v < i; v++) {
		for(long long s = 0; s < j; s++) {
			std::cout << _distance[v][s] << " ";
		}
		std::cout << std::endl;	
	}
*/
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
	printf("\033[31mVEHICLE %llu %s %llu %s", coord_to_go.first, " GOES TO ",  coord_to_go.second, "\033[0m\n");
////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
	for(long long v = 0; v < i; v++) {
		if(vehicles[v]._id == std::to_string(coord_to_go.first + 1)) {
			//// найти вид работы
			Shipment s = vehicles[v].find_task_in_shipments(coord_to_go.second);
			Shipment d = vehicles[v].find_task_in_delivery(coord_to_go.second);
			if(s._id != "NULL") {
				COST += vehicles[v].pick_up(coord_to_go.second);
			}
			else {
				COST += vehicles[v].delivery(coord_to_go.second);
			}
			////
		}
	}
	for(long long v = 0; v < i; v++) {
		vehicles[v].clear_cost();
	}
	std::cout << "COST = " << COST << std::endl;
}
	for(long long v = 0; v < i; v++) {
		COST += vehicles[v].go_home();
	}

///////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////////

    	long long end_time = clock();
	long long search_time = end_time - start_time;
	std::cout << "search_time = " << search_time / 1000.0 << std::endl;
	return 0;
}       

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
void display_vehicles(const Json::Value &vpr_task_root, const int &i, int &N, int &M)
{
    	std::string _id = vpr_task_root["vehicles"][i]["id"].asString();
	long long _capacity = vpr_task_root["vehicles"][i]["capacity"].asUInt();
	long long _distance_price = vpr_task_root["vehicles"][i]["distance_price"].asUInt();
	long long _coord_x = vpr_task_root["vehicles"][i]["coord"]["x"].asUInt();
	long long _coord_y = vpr_task_root["vehicles"][i]["coord"]["y"].asUInt();
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

	if(M < _max_x) {
		M = _max_x;		
	}	
	if(N < _max_y) {
		N = _max_y;		
	}
}

