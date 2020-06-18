#pragma once
#include <list>
#include <iostream>
#include "global.h"
#include "Point.h"
#include "Routes.h"
#include "Shipment.h"

extern const long long INF;
extern std::vector< std::vector<char> > _bonus_map;

extern void print_bonus_map(int N, int M);

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

