#pragma once

#include "state_dir.hh"
void assign_user_color(std::string username, sendit::StateDir& state);
namespace sendit {
  int read(sendit::StateDir& state);
}
