#include "admin.hh"
#include "message.hh"
#include "state_dir.hh"
#include "reader.hh"
#include "terminal.hh"
#include <filesystem>
#include <iostream>
#include <string>

/* Inbuilt sendit client, very basic. */

namespace sendit {

  int interactive_client() {
    sendit::StateDir state;

    sendit::read(state);

    std::cout << "login as user: ";
    std::string username;
    std::cin >> username;
    std::cin.ignore();

    sendit::AdminPassword admin(state);

    if (username == "admin") {
      if (admin.is_set()) {
        admin.verify();
        admin.dont_ask_for_pwd(); // logged in, dont ask
      } else {
        // password is not set
        // does not ask for password again cos i hate when apps do that
        admin.setpassword();
        admin.dont_ask_for_pwd();
      }
    }

    sendit::Message joined(username, "someone joined", state);
    joined.write();

    while (true) {
      sendit::helpers::cls();
      sendit::read(state);

      if (std::filesystem::exists(state.get_path() + "/motd")) {
        std::ifstream motd(state.get_path() + "/motd");
        if (!motd.is_open()) {
          std::cerr << "failed to open motd stream\n";
        } else {
          std::string motd_content;
          getline(motd, motd_content); // no loop needed for oneline
          std::cout << "\n" << motd_content << "\n";
        }
      }

      sendit::helpers::separator();

      std::cout << "-> ";
      std::string msg;
      std::getline(std::cin, msg);

      // reload chat content (you ask? i did say this client is minimal)
      if (msg == "reload") continue;

      if (
        username == "admin" &&
        msg_commands(msg, admin) == SHOULD_EXIT
      ) continue;

      sendit::Message message(msg, username, state);
      message.write();
    }

    return 0;
  }

} // namespace sendit
