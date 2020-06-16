#include <iostream>
#include <json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <vector>
#include <list>
#include <thread>

const unsigned int INF = 1e9;
unsigned int COST;

struct Point {int i, j;} point;
struct Shipment
{
	std::string _id;
	unsigned int _pickup_x;
	unsigned int _pickup_y;
	unsigned int _delivery_x;
	unsigned int _delivery_y;
	unsigned int _capacityDemand;
	unsigned int _cost;
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


struct Act
{
        std::string _type;
        std::string _jobId;
	int _arrTime;
	int _endTime;
};

class Vehicle
{
public:
	Vehicle(std::string id, unsigned int capacity, unsigned int distance_price, unsigned int home_coord_x, unsigned int home_coord_y, int N, int M);
	void add_task(const Shipment &shipment);
	void delete_task(const Shipment &shipment);
	const Shipment find_task_in_shipments(const unsigned int shipment_id);
	const Shipment find_task_in_delivery(const unsigned int shipment_id);
	void print();
	void calc_map(int x, int y);
	const unsigned int get_capacity();
	void calc_shipments_pickup();
	void calc_shipments_delivery();
	void calc_map_from_pickup_to_delivery();
	void calc_map_home();
	unsigned int _coord_x;
	unsigned int _coord_y;
	std::string _id; 
	unsigned int shipment_cost(std::string id);
	unsigned int delivery_cost(std::string id);
	unsigned int pick_up(int shipment_id);
	unsigned int delivery(int shipment_id);
	bool is_shipments_empty();
	bool is_delivery_empty();
	void clear_cost();
private:
	const unsigned int _capacity; 
	unsigned int _capacity_picked; 
	const unsigned int _distance_price;
	const unsigned int _home_coord_x;
	const unsigned int _home_coord_y;
	std::vector< std::vector< unsigned int > > _map;
	std::list< Shipment > _shipments;
	std::list< Shipment > _delivery;
	int _N, _M;
	int _start, _end;
	Act _act;
};

Vehicle::Vehicle(std::string id, unsigned int capacity, unsigned int distance_price, unsigned int home_coord_x, unsigned int home_coord_y, int N, int M) :
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
	for(int i = 0; i < M; i++) {
		_map[i].resize(N);
	}

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
const Shipment Vehicle::find_task_in_shipments(const unsigned int shipment_id)
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
const Shipment Vehicle::find_task_in_delivery(const unsigned int shipment_id) 
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
	std::cout << std::endl << std::endl;
}
const unsigned int Vehicle::get_capacity()
{
	return (_capacity - _capacity_picked);
}
void Vehicle::calc_map(int x, int y)
{
	for(int i = 0; i < _map.size(); i++) {
		for(int j = 0; j < _map[i].size(); j++) {
			_map[i][j] = 0;
		}
	}
	_map[x][y] = 1;
	int head,tail;         
	Point q[_N * _M];      
	int di[4] = { -1, 0, 1,  0};
	int dj[4] = {  0, 1, 0, -1};
	head = tail = 0;
	q[tail].i = x;
	q[tail++].j = y;  

	while (head < tail) {
		Point p = q[head++];          
		for (int k = 0; k < 4; k++) {
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
		s._cost += _distance_price*(_map[s._pickup_x][s._pickup_y] - 1);
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
unsigned int Vehicle::shipment_cost(std::string id)
{
	for(auto &s : _shipments) {
		if(s._id == id)
			return s._cost;
	}
	return INF;
}
unsigned int Vehicle::delivery_cost(std::string id)
{
	for(auto &d : _delivery) {
		if(d._id == id)
			return d._cost;
	}
	return INF;
}
unsigned int Vehicle::pick_up(int shipment_id)
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
	return _map[_coord_x][_coord_y] * _distance_price;
}
unsigned int Vehicle::delivery(int shipment_id)
{
	calc_map(_coord_x, _coord_y);
	for(auto &d : _delivery) {
		std::cout << "id = " << d._id << std::endl;
		if(d._id == std::to_string(shipment_id)) {
			_coord_x = d._delivery_x;
			_coord_y = d._delivery_y;
			_capacity_picked -= d._capacityDemand;
			_delivery.remove(d);
			break;
		}	
	}
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


void display_vehicles(const Json::Value &vpr_task_root, const int &i, int &N, int &M);
void display_shipments(const Json::Value &vpr_task_root, const int &j, int &N, int &M);
Vehicle init(const Json::Value &vpr_task_root, const int &i, int N, int M);
void preprocessing(std::vector< std::vector< unsigned int > > &_distance, int i, int j);
std::pair<unsigned int, unsigned int> assessment(std::vector< std::vector< unsigned int > > &_distance);
unsigned int min_horizontal(std::vector< std::vector< unsigned int > > &_distance,  unsigned int i, unsigned int j);
unsigned int min_vertical(std::vector< std::vector< unsigned int > > &_distance,  unsigned int i, unsigned int j);


int main()
{
	Json::Reader reader;
	Json::Value vpr_task_root;

	std::ifstream vpr_task_file("vpr_task.json");
    	vpr_task_file >> vpr_task_root;
	int i = 0, j = 0, M = 0, N = 0;
	
	std::vector< Vehicle > vehicles;

	while(!vpr_task_root["vehicles"][i]["id"].isNull()) {
	    	display_vehicles(vpr_task_root, i, N, M);
		i++;
	}
	
	for(int v = 0; v < i; v++) {
		vehicles.push_back( init(vpr_task_root, v, N + 1, M + 1) );	
		//vehicles[v].print();
	}
	
	while(!vpr_task_root["shipments"][j]["id"].isNull()) {
	    	display_shipments(vpr_task_root, j, N, M);
		j++;
	}
for(int k = 0; k < 2 * j; k++) {

	for(int s = 0; s < j; s++) {
		Shipment shipment;
		shipment._id = vpr_task_root["shipments"][s]["id"].asString();;
		shipment._pickup_x = vpr_task_root["shipments"][s]["pickup"]["x"].asUInt();
		shipment._pickup_y = vpr_task_root["shipments"][s]["pickup"]["y"].asUInt();
		shipment._delivery_x = vpr_task_root["shipments"][s]["delivery"]["x"].asUInt();
		shipment._delivery_y = vpr_task_root["shipments"][s]["delivery"]["y"].asUInt();
		shipment._capacityDemand = vpr_task_root["shipments"][s]["capacityDemand"].asUInt();
		shipment._cost = 0;

		for(int v = 0; v < i; v++) {
			if( vehicles[v].get_capacity() >= shipment._capacityDemand ) {
				vehicles[v].add_task(shipment);
			}	
		}	
	}

////////////////////////////////////////////////////////////////////////////////////////////////
  	std::vector< std::thread > tcalc_map;
	for (int v = 0; v < i; v++) {
		tcalc_map.push_back( std::thread(&Vehicle::calc_map, &vehicles[v], vehicles[v]._coord_x, vehicles[v]._coord_y) );
	}
	for (auto & th : tcalc_map)
		th.join();
vehicles[0].print();vehicles[1].print();
////////////////////////////////////////////////////////////////////////////////////////////////
  	std::vector< std::thread > tcalc_shipments_pickup;
	for (int v = 0; v < i; v++) {
		tcalc_shipments_pickup.push_back( std::thread(&Vehicle::calc_shipments_pickup, &vehicles[v]) );
	}
	for (auto & th : tcalc_shipments_pickup)
		th.join();
////////////////////////////////////////////////////////////////////////////////////////////////
  	std::vector< std::thread > tcalc_shipments_delivery;
	for (int v = 0; v < i; v++) {
		tcalc_shipments_delivery.push_back( std::thread(&Vehicle::calc_shipments_delivery, &vehicles[v]) );
	}
	for (auto & th : tcalc_shipments_delivery)
		th.join();
////////////////////////////////////////////////////////////////////////////////////////////////
  	std::vector< std::thread > tcalc_map_from_pickup_to_delivery;
	for (int v = 0; v < i; v++) {
		tcalc_map_from_pickup_to_delivery.push_back( std::thread(&Vehicle::calc_map_from_pickup_to_delivery, &vehicles[v]) );
	}
	for (auto & th : tcalc_map_from_pickup_to_delivery)
		th.join();
////////////////////////////////////////////////////////////////////////////////////////////////
  	std::vector< std::thread > tcalc_map_home;
	for (int v = 0; v < i; v++) {
		tcalc_map_home.push_back( std::thread(&Vehicle::calc_map_home, &vehicles[v]) );
	}
	for (auto & th : tcalc_map_home)
		th.join();

	std::vector< std::vector< unsigned int > > _distance;
	_distance.resize(i);
	for(int v = 0; v < i; v++) {
		_distance[v].resize(j);
		for(int s = 0; s < j; s++) {
			_distance[v][s] = std::min(	vehicles[v].shipment_cost(std::to_string(s)),
							vehicles[v].delivery_cost(std::to_string(s)));	
		}	
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//branches and borders
////////////////////////////////////////////////////////////////////////////////////////////////
	preprocessing(_distance, i, j);
	std::pair< unsigned int, unsigned int > coord_to_go = assessment(_distance);
	std::cout << "VEHICLE " << coord_to_go.first << " GOES TO  " << coord_to_go.second << std::endl;
////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////

	for(int v = 0; v < i; v++) {
		if(vehicles[v]._id == std::to_string(coord_to_go.first + 1)) {
			//// найти вид работы
			Shipment s = vehicles[v].find_task_in_shipments(coord_to_go.second);
			Shipment d = vehicles[v].find_task_in_delivery(coord_to_go.second);
			std::cout << "----FIND----" << std::endl;
			if(s._id != "NULL") {
				COST += vehicles[v].pick_up(coord_to_go.second);
				std::cout << "----NOTNULL----" << std::endl;
			}
			else {
				std::cout << "----NULL----" << std::endl;
				COST += vehicles[v].delivery(coord_to_go.second);
				//vehicles[v].print();	
			}
			////
		}
	}
	for(int v = 0; v < i; v++) {
		vehicles[v].clear_cost();
	}


	std::cout << "COST = " << COST << std::endl;

}


	std::cout << "N = " << N << " M = " << M << std::endl;

	return 0;
}       




Vehicle init(const Json::Value &vpr_task_root, const int &i, int N, int M)
{
    	std::string _id = vpr_task_root["vehicles"][i]["id"].asString();
	unsigned int _capacity = vpr_task_root["vehicles"][i]["capacity"].asUInt();
	unsigned int _distance_price = vpr_task_root["vehicles"][i]["distance_price"].asUInt();
	unsigned int _coord_x = vpr_task_root["vehicles"][i]["coord"]["x"].asUInt();
	unsigned int _coord_y = vpr_task_root["vehicles"][i]["coord"]["y"].asUInt();
	Vehicle temp(_id, _capacity, _distance_price, _coord_x, _coord_y, N, M);
	return temp;
	
}
void display_vehicles(const Json::Value &vpr_task_root, const int &i, int &N, int &M)
{
    	std::string _id = vpr_task_root["vehicles"][i]["id"].asString();
	unsigned int _capacity = vpr_task_root["vehicles"][i]["capacity"].asUInt();
	unsigned int _distance_price = vpr_task_root["vehicles"][i]["distance_price"].asUInt();
	unsigned int _coord_x = vpr_task_root["vehicles"][i]["coord"]["x"].asUInt();
	unsigned int _coord_y = vpr_task_root["vehicles"][i]["coord"]["y"].asUInt();
	if(M < _coord_x) {
		M = _coord_x;		
	}	
	if(N < _coord_y) {
		N = _coord_y;		
	}
/*
	std::cout << "In display_vpr_task" << std::endl;
	std::cout << "id             :" << _id << std::endl;
	std::cout << "capacity       :" << _capacity << std::endl;
	std::cout << "distance_price :" << _distance_price << std::endl;
	std::cout << "coord_x        :" << _coord_x << std::endl;
	std::cout << "coord_y        :" << _coord_y << std::endl;
*/
}

void display_shipments(const Json::Value &vpr_task_root, const int &j, int &N, int &M)
{
    	std::string _id = vpr_task_root["shipments"][j]["id"].asString();
	unsigned int _pickup_x = vpr_task_root["shipments"][j]["pickup"]["x"].asUInt();
	unsigned int _pickup_y = vpr_task_root["shipments"][j]["pickup"]["y"].asUInt();
	unsigned int _delivery_x = vpr_task_root["shipments"][j]["delivery"]["x"].asUInt();
	unsigned int _delivery_y = vpr_task_root["shipments"][j]["delivery"]["y"].asUInt();
	unsigned int _capacityDemand = vpr_task_root["shipments"][j]["capacityDemand"].asUInt();
	unsigned int _max_x = std::max(_pickup_x, _delivery_x);
	unsigned int _max_y = std::max(_pickup_y, _delivery_y);

	if(M < _max_x) {
		M = _max_x;		
	}	
	if(N < _max_y) {
		N = _max_y;		
	}
}

void preprocessing(std::vector< std::vector< unsigned int > > &_distance, int  i, int j)
{
	unsigned int mini = INF;
	for(int v = 0; v < i; v++) {
		mini = INF;
		for(int s = 0; s < j; s++) {
			mini = std::min(mini, _distance[v][s]);	
		}
		for(int s = 0; s < j; s++) {
			if(_distance[v][s] != INF) {
				_distance[v][s] -= mini;
			}	
		}	
	}

	for(int s = 0; s < j; s++) {
		mini = INF;
		for(int v = 0; v < i; v++) {
			mini = std::min(mini, _distance[v][s]);	
		}
		for(int v = 0; v < i; v++) {
			if(_distance[v][s] != INF) {
				_distance[v][s] -= mini;
			}
		}	
	}
}

std::pair<unsigned int, unsigned int> assessment(std::vector< std::vector< unsigned int > > &_distance)
{
	std::vector< std::vector< unsigned int > > rating;
	rating.resize(_distance.size());
	for(int v = 0; v < _distance.size(); v++) {
		rating[v].resize(_distance[v].size());
		std::fill(rating[v].begin(), rating[v].end(), 0);
		for(int s = 0; s < _distance[v].size(); s++) {
			if(_distance[v][s] == 0) {
				rating[v][s] = min_horizontal(_distance, v, s) + min_vertical(_distance, v, s);
			}
		}	
	}
/*
	for(int v = 0; v < _distance.size(); v++) {
		for(int s = 0; s < _distance[v].size(); s++) {
			std::cout << rating[v][s] << " \t";	
		}
	}
*/
	unsigned int i, j, maxi = 0;
	for(int v = 0; v < _distance.size(); v++) {
		for(int s = 0; s < _distance[v].size(); s++) {
			if(maxi <= rating[v][s])	{
				maxi = rating[v][s];
				i = v;
				j = s;
			}
		}
	}
	return std::make_pair(i,j);
}

unsigned int min_horizontal(std::vector< std::vector< unsigned int > > &_distance, unsigned int i, unsigned int j)
{
	unsigned int mini = INF;
	for(int s = 0; s < _distance[i].size(); s++) {
		if(_distance[i][s] < mini && s != j) {
			mini = _distance[i][s];
		}
	}
	return mini == INF ? 0 : mini;
}
unsigned int min_vertical(std::vector< std::vector< unsigned int > > &_distance, unsigned int i, unsigned int j)
{
	unsigned int mini = INF;
	for(int v = 0; v < _distance.size(); v++) {
		if(_distance[v][j] < mini && v != i) {
			mini = _distance[v][j];
		}
	}
	return mini == INF ? 0 : mini;
}
