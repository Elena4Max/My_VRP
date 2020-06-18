#include "Vehicle.h"

long long TIMER;
long long JOB;
std::list< Shipment > pick_upped;

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
		s._cost += _distance_price * (_map[s._pickup_x][s._pickup_y] - 1);
#ifdef _BONUS2
		long long _t_coord_x = _coord_x, _t_coord_y = _coord_y;
		char c = ' ';
		for(int i = 0; i < _M; i++) {
			for(int j = 0; j < _N; j++) {
				if(std::abs(_t_coord_x - s._pickup_x) > std::abs(_t_coord_y - s._pickup_y)) {
					if(_t_coord_x > s._pickup_x) {
						_bonus_map[_t_coord_x][_t_coord_y] = c;
						_t_coord_x--;
						c = _bonus_map[_t_coord_x][_t_coord_y];
						_bonus_map[_t_coord_x][_t_coord_y] = 'V';
						print_bonus_map(_N, _M);
						if(c == 'S' && _t_coord_x == s._pickup_x && _t_coord_y == s._pickup_y) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}
					else {
						if(_t_coord_x < s._pickup_x) {
							_bonus_map[_t_coord_x][_t_coord_y] = c;
							_t_coord_x++;
							c = _bonus_map[_t_coord_x][_t_coord_y];
							_bonus_map[_t_coord_x][_t_coord_y] = 'V';
							print_bonus_map(_N, _M);
							if(c == 'S' && _t_coord_x == s._pickup_x && _t_coord_y == s._pickup_y) {
								std::this_thread::sleep_for(std::chrono::milliseconds(100));
							}
						}
					}
				}
				else {
					if(_t_coord_y > s._pickup_y) {
						_bonus_map[_t_coord_x][_t_coord_y] = c;
						_t_coord_y--;
						c = _bonus_map[_t_coord_x][_t_coord_y];
						_bonus_map[_t_coord_x][_t_coord_y] = 'V';
						print_bonus_map(_N, _M);
						if(c == 'S' && _t_coord_x == s._pickup_x && _t_coord_y == s._pickup_y) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}
					else {
						if(_t_coord_y < s._pickup_y) {
							_bonus_map[_t_coord_x][_t_coord_y] = c;
							_t_coord_y++;
							c = _bonus_map[_t_coord_x][_t_coord_y];
							_bonus_map[_t_coord_x][_t_coord_y] = 'V';
							print_bonus_map(_N, _M);
							if(c == 'S' && _t_coord_x == s._pickup_x && _t_coord_y == s._pickup_y) {
								std::this_thread::sleep_for(std::chrono::milliseconds(100));
							}
						}
					}
				}
			}
		}
		_bonus_map[_coord_x][_coord_y] = 'V';
		_bonus_map[s._pickup_x][s._pickup_y] = 'S';
		print_bonus_map(_N, _M);
#endif
	}
}

void Vehicle::calc_shipments_delivery()
{
	for(auto &d : _delivery) {
		d._cost += _distance_price*(_map[d._delivery_x][d._delivery_y] - 1);
#ifdef _BONUS2
		long long _t_coord_x = _coord_x, _t_coord_y = _coord_y;
		char c = ' ';
		for(int i = 0; i < _M; i++) {
			for(int j = 0; j < _N; j++) {
				if(std::abs(_t_coord_x - d._delivery_x) > std::abs(_t_coord_y - d._delivery_y)) {
					if(_t_coord_x > d._delivery_x) {
						_bonus_map[_t_coord_x][_t_coord_y] = c;
						_t_coord_x--;
						c = _bonus_map[_t_coord_x][_t_coord_y];
						_bonus_map[_t_coord_x][_t_coord_y] = 'V';
						print_bonus_map(_N, _M);
						if(c == 'D' && _t_coord_x == d._delivery_x && _t_coord_y == d._delivery_y) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}
					else {
						if(_t_coord_x < d._delivery_x) {
							_bonus_map[_t_coord_x][_t_coord_y] = c;
							_t_coord_x++;
							c = _bonus_map[_t_coord_x][_t_coord_y];
							_bonus_map[_t_coord_x][_t_coord_y] = 'V';
							print_bonus_map(_N, _M);
							if(c == 'D' && _t_coord_x == d._delivery_x && _t_coord_y == d._delivery_y) {
								std::this_thread::sleep_for(std::chrono::milliseconds(100));
							}
						}
					}
				}
				else {
					if(_t_coord_y > d._delivery_y) {
						_bonus_map[_t_coord_x][_t_coord_y] = c;
						_t_coord_y--;
						c = _bonus_map[_t_coord_x][_t_coord_y];
						_bonus_map[_t_coord_x][_t_coord_y] = 'V';
						print_bonus_map(_N, _M);
						if(c == 'D' && _t_coord_x == d._delivery_x && _t_coord_y == d._delivery_y) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}
					else {
						if(_t_coord_y < d._delivery_y) {
							_bonus_map[_t_coord_x][_t_coord_y] = c;
							_t_coord_y++;
							c = _bonus_map[_t_coord_x][_t_coord_y];
							_bonus_map[_t_coord_x][_t_coord_y] = 'V';
							print_bonus_map(_N, _M);
							if(c == 'D' && _t_coord_x == d._delivery_x && _t_coord_y == d._delivery_y) {
								std::this_thread::sleep_for(std::chrono::milliseconds(100));
							}
						}
					}
				}
			}
		}
		_bonus_map[_coord_x][_coord_y] = 'V';
		_bonus_map[d._delivery_x][d._delivery_y] = 'D';
		print_bonus_map(_N, _M);
#endif
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
	long long _t_coord_x = _coord_x, _t_coord_y = _coord_y;
	for(auto &s : _shipments) {
		if(s._id == std::to_string(shipment_id)) {
			_coord_x = s._pickup_x;
			_coord_y = s._pickup_y;
			_capacity_picked += s._capacityDemand;
			_delivery.push_back(s);
			pick_upped.push_back(s);
		}	
	}
#ifdef _BONUS1
	char c = ' ';
	for(int i = 0; i < _M; i++) {
		for(int j = 0; j < _N; j++) {
			if(std::abs(_t_coord_x - _coord_x) > std::abs(_t_coord_y - _coord_y)) {
				if(_t_coord_x > _coord_x) {
					_bonus_map[_t_coord_x][_t_coord_y] = c;
					_t_coord_x--;
					c = _bonus_map[_t_coord_x][_t_coord_y];
					_bonus_map[_t_coord_x][_t_coord_y] = 'V';
					print_bonus_map(_N, _M);
					if(c == 'S' && _t_coord_y == _coord_y && _t_coord_x == _coord_x) {
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
				}
				else {
					if(_t_coord_x < _coord_x) {
						_bonus_map[_t_coord_x][_t_coord_y] = c;
						_t_coord_x++;
						c = _bonus_map[_t_coord_x][_t_coord_y];
						_bonus_map[_t_coord_x][_t_coord_y] = 'V';
						print_bonus_map(_N, _M);
						if(c == 'S' && _t_coord_y == _coord_y && _t_coord_x == _coord_x) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}
				}
			}
			else {
				if(_t_coord_y > _coord_y) {
					_bonus_map[_t_coord_x][_t_coord_y] = c;
					_t_coord_y--;
					c = _bonus_map[_t_coord_x][_t_coord_y];
					_bonus_map[_t_coord_x][_t_coord_y] = 'V';
					print_bonus_map(_N, _M);
					if(c == 'S' && _t_coord_y == _coord_y && _t_coord_x == _coord_x) {
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
				}
				else {
					if(_t_coord_y < _coord_y) {
						_bonus_map[_t_coord_x][_t_coord_y] = c;
						_t_coord_y++;
						c = _bonus_map[_t_coord_x][_t_coord_y];
						_bonus_map[_t_coord_x][_t_coord_y] = 'V';
						print_bonus_map(_N, _M);
						if(c == 'S' && _t_coord_y == _coord_y && _t_coord_x == _coord_x) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}
				}
			}
		}
	}
#endif
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
	long long _t_coord_x = _coord_x, _t_coord_y = _coord_y;
	for(auto &d : _delivery) {
		if(d._id == std::to_string(shipment_id)) {
			_coord_x = d._delivery_x;
			_coord_y = d._delivery_y;
			_capacity_picked -= d._capacityDemand;
			_delivery.remove(d);
			break;
		}	
	}
#ifdef _BONUS1
	char c = ' ';
	for(int i = 0; i < _M; i++) {
		for(int j = 0; j < _N; j++) {
			if(std::abs(_t_coord_x - _coord_x) > std::abs(_t_coord_y - _coord_y)) {
				if(_t_coord_x > _coord_x) {
					_bonus_map[_t_coord_x][_t_coord_y] = c;
					_t_coord_x--;
					c = _bonus_map[_t_coord_x][_t_coord_y];
					_bonus_map[_t_coord_x][_t_coord_y] = 'V';
					print_bonus_map(_N, _M);
					if(c == 'D'  && _t_coord_y == _coord_y && _t_coord_x == _coord_x) {
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
				}
				else {
					if(_t_coord_x < _coord_x) {
						_bonus_map[_t_coord_x][_t_coord_y] = c;
						_t_coord_x++;
						c = _bonus_map[_t_coord_x][_t_coord_y];
						_bonus_map[_t_coord_x][_t_coord_y] = 'V';
						print_bonus_map(_N, _M);
						if(c == 'D' && _t_coord_y == _coord_y && _t_coord_x == _coord_x) { 
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}
				}
			}
			else {
				if(_t_coord_y > _coord_y) {
					_bonus_map[_t_coord_x][_t_coord_y] = c;
					_t_coord_y--;
					c = _bonus_map[_t_coord_x][_t_coord_y];
					_bonus_map[_t_coord_x][_t_coord_y] = 'V';
					print_bonus_map(_N, _M);
					if(c == 'D' && _t_coord_y == _coord_y && _t_coord_x == _coord_x) {
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
				}
				else {
					if(_t_coord_y < _coord_y) {
						_bonus_map[_t_coord_x][_t_coord_y] = c;
						_t_coord_y++;
						c = _bonus_map[_t_coord_x][_t_coord_y];
						_bonus_map[_t_coord_x][_t_coord_y] = 'V';
						print_bonus_map(_N, _M);
						if(c == 'D' && _t_coord_y == _coord_y && _t_coord_x == _coord_x) { 
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}
				}
			}
		}
	}
#endif
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
	long long _t_coord_x = _coord_x, _t_coord_y = _coord_y;
	_coord_x = _home_coord_x;
	_coord_y = _home_coord_y;
#ifdef _BONUS1
	char c = ' ';
	for(int i = 0; i < _M; i++) {
		for(int j = 0; j < _N; j++) {;
			if(std::abs(_t_coord_x - _coord_x) > std::abs(_t_coord_y - _coord_y)) {
				if(_t_coord_x > _coord_x) {
					_bonus_map[_t_coord_x][_t_coord_y] = c;
					_t_coord_x--;
					c = _bonus_map[_t_coord_x][_t_coord_y];
					_bonus_map[_t_coord_x][_t_coord_y] = 'V';
					print_bonus_map(_N, _M);
				}
				else {
					if(_t_coord_x < _coord_x) {
						_bonus_map[_t_coord_x][_t_coord_y] = c;
						_t_coord_x++;
						c = _bonus_map[_t_coord_x][_t_coord_y];
						_bonus_map[_t_coord_x][_t_coord_y] = 'V';
						print_bonus_map(_N, _M);
					}
				}
			}
			else {
				if(_t_coord_y > _coord_y) {
					_bonus_map[_t_coord_x][_t_coord_y] = c;
					_t_coord_y--;
					c = _bonus_map[_t_coord_x][_t_coord_y];
					_bonus_map[_t_coord_x][_t_coord_y] = 'V';
					print_bonus_map(_N, _M);
				}
				else {
					if(_t_coord_y < _coord_y) {
						_bonus_map[_t_coord_x][_t_coord_y] = c;
						_t_coord_y++;
						c = _bonus_map[_t_coord_x][_t_coord_y];
						_bonus_map[_t_coord_x][_t_coord_y] = 'V';
						print_bonus_map(_N, _M);
					}
				}
			}
		}
	}
#endif
	_rout._end = TIMER;
	return (_map[_home_coord_x][_home_coord_y] - 1) * _distance_price;
}
