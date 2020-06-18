#include <string>

struct Shipment
{
	std::string _id;
	long long _pickup_x;
	long long _pickup_y;
	long long _delivery_x;
	long long _delivery_y;
	long long _capacityDemand;
	long long _cost;
	friend bool operator == (const Shipment &s1, const Shipment &s2);
    	friend bool operator != (const Shipment &s1, const Shipment &s2);
};
