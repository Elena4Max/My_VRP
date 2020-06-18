#pragma once
#include <json/value.h>
#include <jsoncpp/json/json.h>
#include "Vehicle.h"

void display_vehicles(const Json::Value &vpr_task_root, const int &i, int &N, int &M);
void display_shipments(const Json::Value &vpr_task_root, const int &j, int &N, int &M);
