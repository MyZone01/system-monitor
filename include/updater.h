#pragma once
#define UPDATER_H
#include "fan.h"
#include "system.h"

namespace Updater {
void ProcessesUpdater(System *);
void CPUUpdater(System *, int *);
void FanSpeedUpdater(Fan *, int *);
}  // namespace Updater