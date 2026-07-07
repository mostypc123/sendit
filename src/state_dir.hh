#pragma once

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

namespace sendit::helpers {

  inline void statedir_mkdir_wrap(const std::string& path) {
    try {
      if (!std::filesystem::create_directory(path)) {
        std::cout << "sendit: using already existing state dir " << path << "\n";
      }
    } catch (const std::filesystem::filesystem_error& e) {
      std::cerr << "sendit: \e[91mfailed to create state dir\e[0m\n" <<
        "        " << e.what() << std::endl;
      std::exit(1);
    }
  }

} // namespace sendit::helpers

namespace sendit {

  class StateDir {
  private:
    const std::string path;

    static std::string init_path() {
      uid_t uid = getuid();
      if (uid == 0) {
        sendit::helpers::statedir_mkdir_wrap("/run/sendit");
        return "/run/sendit/";
      }

      char buf[1024];
      getcwd(buf, sizeof(buf));
      std::string nbuf(buf);
      nbuf += "/.sendit/";
      sendit::helpers::statedir_mkdir_wrap(nbuf);
      return nbuf;
    }

  public:
    StateDir() : path(init_path()) {}

    std::string get_path() {
      return path;
    }
  };

} // namespace sendit
