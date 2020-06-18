#include "Shipment.h"

bool operator == (const Shipment &s1, const Shipment &s2)
{
    return 	(s1._id == s2._id &&
		s1._pickup_x == s2._pickup_x &&
		s1._pickup_y == s1._pickup_y &&
		s1._delivery_x == s2._delivery_x &&
		s1._delivery_y == s2._delivery_y &&
		s1._capacityDemand == s2._capacityDemand);
}
 
bool operator != (const Shipment &s1, const Shipment &s2)
{
    return !(s1 == s2);
}
