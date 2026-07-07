#include "state_dir.hh"
#include "random_rgb.hh"
#include "split.hh"
#include <filesystem>
#include <fstream>

void assign_user_color(
  std::string username,
  sendit::StateDir& state
) {
  if (!std::filesystem::exists(
    state.get_path() + "/user-color-" + username
  )) {
    auto color = sendit::helpers::random_rgb_color();
    std::ofstream file(state.get_path() + "/user-color-" + username);
    file << "\e[38;2;" << color[0] << ";" << color[1] << ";" << color[2] << "m";
    file.close();
  }
}

namespace sendit {

  int read(sendit::StateDir& state) {
    std::ifstream file(state.get_path() + "/messages");
    if (!file.is_open()) {
      std::cerr << "failed to open messages stream\n";
      return 1;
    }
    std::string data;
    bool admin_msg_next = false;
    while (std::getline(file, data)) {
      // instead of looping multiple times, process now
      if (data != "VERIFIED ADMIN MESSAGE:" && !admin_msg_next) {
        auto parts = sendit::helpers::split(data);
        if (parts.size() == 1) {
          parts.push_back("\e[3mempty message\e[0m");
        }
        assign_user_color(parts[0], state);
        std::ifstream file(state.get_path() + "/user-color-" + parts[0]);
        std::string color;
        getline(file, color); // no newline expected, no loop
        std::cout << color << parts[0] << "\e[0m:" << parts[1] << "\n";
        continue;
      } if (admin_msg_next) {
        data = "\e[93m" + data + "\e[0m";
        admin_msg_next = false;
      } else {
        data = "\e[91mVERIFIED ADMIN MESSAGE:\e[0m";
        admin_msg_next = true;
      }

      std::cout << data << "\n";
    }

    return 0;
  }

} // namespace sendit
