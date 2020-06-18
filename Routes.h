#include <vector>
#include "Act.h"

struct Routes
{
	std::string _vehicleId;
	long long _start;
	long long _end;
	std::vector< Act > _act;
};
