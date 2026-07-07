#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "message.hh"
#include "state_dir.hh"
#include "admin.hh"
#include "reader.hh"
#include "interactive_client.hh"

int main(int argc, char* argv[]) {
  std::srand(std::time(0));
  std::string message = "Sending the default message.";
  std::string username = "anonymous";

  for (int i = 1; i < argc; i++) {
    std::string cmd(argv[i]);
    if (cmd == "-n") {
      if (!(argc > i + 1)) { std::cerr << "not enough args\n"; return 2; }
      username = argv[++i];
    } else if (cmd == "-m") {
      if (!(argc > i + 1)) { std::cerr << "not enough args\n"; return 2; }
      message = argv[++i];
    } else if (cmd == "-i") {
      return sendit::interactive_client();
    }
  }

  sendit::StateDir state;
  const std::string statedir = state.get_path();

  if (argc == 1) {
    std::cout << "sendit: no arguments given, reading sent messages." << std::endl;
    return sendit::read(state);
  }

  sendit::AdminPassword admin(state);

  if (!msg_commands(message, admin)) {
    return 1;
  }

  if (username == "admin") {
    std::cout << "writing as the admin user requires you to enter the password.\n";
    admin.verify();

    return 0;
  }

  sendit::Message msg(message, username, state);
  msg.write();

  std::ifstream motd(state.get_path() + "/motd");
  if (!motd.is_open()) {
    // motd is not set, whatever
    return 0;
  }
  std::string motd_content;
  getline(motd, motd_content); // no loop needed for oneline
  std::cout << motd_content << "\n";

  return 0;
}
