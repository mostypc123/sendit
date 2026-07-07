#pragma once

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#include "state_dir.hh"
#include "admin.hh"

#define SHOULD_EXIT false
#define RUN true

bool msg_commands(
  const std::string& message,
  sendit::AdminPassword& admin
);

namespace sendit {

  class Message {
  protected:
    const std::string content; // message content
    const std::string user; // username
    const std::string lockfile; // <cfg>/typing
    const std::string messagesfile; // <cfg>/messages

    #define LOCK 1 // lock file (get_conf_path())
    #define MSGS 2 // msgs file (get_conf_path())

    /* Get configuration path for LOCK or MSGS. */
    static inline std::string get_conf_path(
      sendit::StateDir& statedir,
      int8_t type
    ) {
      if (type == LOCK)
        return statedir.get_path() + "/typing";
      else if (type == MSGS)
        return statedir.get_path() + "/messages";
      else
        return statedir.get_path() + "/dead-file";
    }

    /* Returns formatted message to write. */
    static inline std::string fmt_message(
      const std::string& content,
      const std::string& name
    ) {
      if (name == "admin") {
        return "VERIFIED ADMIN MESSAGE:\n  << " + content + " >>";
      }
      return name + ": " + content;
    }

  public:
    Message(
      std::string content,
      std::string user,
      sendit::StateDir& statedir
    ): content(content), user(user),
       lockfile(get_conf_path(statedir, 1)),
       messagesfile(get_conf_path(statedir, 2)) {}

    void write() {
      int fd = open(lockfile.c_str(), O_CREAT | O_RDWR, 0644);
      if (fd < 0) {
        std::cerr << "sendit: failed to open " << lockfile << std::endl;
        std::exit(1);
      }

      if (flock(fd, LOCK_EX | LOCK_NB) != 0) {
        std::cout << "sendit: waiting for another user to send their message" << std::endl;
        if (flock(fd, LOCK_EX) != 0) {
          std::cerr << "sendit: failed to lock " << lockfile << std::endl;
          close(fd);
          std::exit(1);
        }
      }

      std::ofstream messagefile(messagesfile, std::ios::app);
      if (!messagefile.is_open()) {
        std::cerr << "sendit: failed to open " << messagesfile << std::endl;
        flock(fd, LOCK_UN);
        close(fd);
        std::exit(1);
      }

      const std::string msg = fmt_message(content, user);
      messagefile << msg << std::endl;
      std::cout << msg << std::endl;

      messagefile.close();

      flock(fd, LOCK_UN);
      close(fd);
    }
  };

} // namespace sendit
