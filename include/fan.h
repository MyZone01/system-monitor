#pragma once
#define FAN_H

#include <fstream>
#include <string>

class Fan {
   public:
    Fan();
    float fan_speed[30];
    std::string GetFanStatusOnLinux();
    float GetFanSpeedOnLinux();
    float GetCPUTemperatureOnLinux();
    std::string GetBatteryLevel();
   private:
};
