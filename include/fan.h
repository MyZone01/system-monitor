#pragma once
#define FAN_H

#include <fstream>
#include <string>

std::string GetFanStatusOnLinux();
float GetFanSpeedOnLinux();
float GetCPUTemperatureOnLinux();
std::string  GetBatteryLevel();
