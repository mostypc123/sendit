#include <iostream>
#include "message.hh"
#include "state_dir.hh"
#include "admin.hh"
#include "reader.hh"

int main(int argc, char* argv[]) {
  std::string message = "Sending the default message.";
  std::string username = "anonymous";

  for (int i = 1; i < argc; i++) {
    std::string cmd(argv[i]);
    if (cmd == "-n") {
      username = argv[++i];
    } else if (cmd == "-m") {
      message = argv[++i];
    }
  }

  sendit::StateDir state;
  const std::string statedir = state.get_path();

  if (argc == 1) {
    std::cout << "sendit: no arguments given, reading sent messages." << std::endl;
    return sendit::read(state);
  }

  sendit::AdminPassword admin(state);

  if (message.starts_with("admin-setpwd:")) {
    admin.setpassword(message);
    std::exit(0);
  } else if (message == "admin-setpwd") {
    admin.setpassword();
    std::exit(0);
  }

  if (username == "admin") {
    std::cout << "writing as the admin user requires you to enter the password.\n";
    admin.verify();

    sendit::AdminMessage msg(message, username, state);
    msg.write();

    return 0;
  }

  sendit::Message msg(message, username, state);
  msg.write();

  return 0;
}
