#pragma once
#define UPDATER_H
#include "system.h"

namespace Updater
{
    void ProcessesUpdater(System *);
    void CPUUpdater(System *, int *);
}