#include "fan.h"

#include <iostream>
#include <fstream>
#include <string>

std::string Fan::GetFanStatusOnLinux() {
    // Replace "/sys/class/hwmon/hwmon1/fanX_enable" with the correct path to the fan status file
    std::ifstream statusFile("/sys/class/hwmon/hwmon1/temp1_input");
    std::string status;
    statusFile >> status;
    auto _status = atoi(status.c_str());
    return _status >= 1 ? "Enabled" : "Disabled";
}

float Fan::GetFanSpeedOnLinux() {
    // Replace "/sys/class/hwmon/hwmon1/fanX_input" with the correct path to the fan speed file
    std::ifstream speedFile1("/sys/class/hwmon/hwmon1/temp1_input");
    std::ifstream speedFile2("/sys/class/hwmon/hwmon2/ino0_input");
    std::ifstream speedFile3("/sys/class/hwmon/hwmon3/temp1_input");
    std::ifstream speedFile4("/sys/class/hwmon/hwmon4/pwm1_input");
    std::ifstream speedFile5("/sys/class/hwmon/hwmon5/temp1_input");
    std::ifstream speedFile6("/sys/class/hwmon/hwmon6/temp1_input");
    int _speed;
    int temp;
    speedFile1 >> temp;
    _speed += temp;
    speedFile2 >> temp;
    _speed += temp;
    speedFile3 >> temp;
    _speed += temp;
    speedFile4 >> temp;
    _speed += temp;
    speedFile5 >> temp;
    _speed += temp;
    speedFile6 >> temp;
    _speed += temp;
    float speed = static_cast<float>(_speed);

    for (int i = 0; i < 29; i++) {
        Fan::fan_speed_log[i] = Fan::fan_speed_log[i + 1];
    }
    Fan::fan_speed_log[29] = speed / 1000.0f;
    // std::cout << fan_speed_log[29] << std::endl;
    return speed;
}

float Fan::GetCPUTemperatureOnLinux() {
    // Replace "/sys/class/thermal/thermal_zoneX/temp" with the correct path to the CPU temperature file
    std::ifstream tempFile("/sys/class/thermal/thermal_zone6/temp");
    int temp;
    tempFile >> temp;
    return static_cast<float>(temp);  // Temperature in degrees Celsius
}

std::string Fan::GetBatteryLevel() {
    std::string batteryPath = "/sys/class/power_supply/BAT1/capacity";  // Update the path based on your system
    std::ifstream file(batteryPath);
    std::string level;
    file >> level;
    return level.c_str();
}

Fan::Fan() {
}