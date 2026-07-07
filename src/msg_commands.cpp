#include "message.hh"
#include <string>

/* This is a mess. Thank you. */

bool msg_commands(
  const std::string& message,
  sendit::AdminPassword& admin
) {
  if (message.starts_with("admin-setpwd:")) {
    std::cout << "using admin-setpwd with a password in the command is a security hole!\n";
    admin.setpassword(message);
    return SHOULD_EXIT;
  } else if (message == "admin-setpwd") {
    admin.setpassword();
    return SHOULD_EXIT;
  } else if (message.starts_with("admin-setmotd:")) {
    auto parts = sendit::helpers::split(message);
    if (parts.size() != 2) {
      std::cerr << "invalid arguments" << std::endl;
    }
    admin.set_motd(parts[1]);
    return SHOULD_EXIT;
  } else {
    return RUN;
  }
}
