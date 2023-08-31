#pragma once
#define FAN_H

#include <fstream>
#include <string>

class Fan {
   public:
    float fan_speed_log[100];
    float temp_log[100];
    std::string GetFanStatusOnLinux();
    std::string GetFanLevelOnLinux();
    float GetFanSpeedOnLinux();
    float GetCPUTemperatureOnLinux();
    std::string GetBatteryLevel();

   private:
};
