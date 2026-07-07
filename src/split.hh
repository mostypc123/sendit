#pragma once

#include <string>
#include <vector>

namespace sendit::helpers {
  std::vector<std::string> split(const std::string& s, char delim = ':');
}
