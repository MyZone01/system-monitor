#pragma once
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
std::string ReadableSize(long bytes);
};  // namespace Format
