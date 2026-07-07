#pragma once

#include <string>
#include "state_dir.hh"

namespace sendit::helpers {
  void alt_msg(
    std::string message,
    std::string username,
    sendit::StateDir& state
  );
}
