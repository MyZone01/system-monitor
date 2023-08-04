#pragma once
#define FAN_H

#include <fstream>
#include <string>

std::string GetFanStatusOnLinux();
int GetFanSpeedOnLinux();
int GetFanLevelOnLinux();
float GetCPUTemperatureOnLinux();
float GetBatteryLevel();
