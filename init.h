#pragma once
#include <json/value.h>
#include <jsoncpp/json/json.h>
#include "Vehicle.h"

Vehicle init(const Json::Value &vpr_task_root, const int &i, int N, int M);
