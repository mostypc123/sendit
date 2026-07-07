#pragma once

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "sha256.hh"
#include "split.hh"
#include "state_dir.hh"
#include "terminal.hh"
#include "altmsg.hh"

namespace sendit {

  class AdminPassword {
  private:
    StateDir *statedir;
    std::string sha256;
    std::string& hash = sha256;
    bool no_ask_pwd = false;

    void set(
      std::string& sha256,
      const std::string& input
    ) {
      sha256 = sendit::helpers::sha256(input);
    }

    void write() {
      std::ofstream file(statedir->get_path() + "/admin-pwd");
      if (!file.is_open()) {
        std::cerr << "sendit: failed to open password file\n";
        std::exit(1);
      }
      file << hash;
      file.close();
    }

  public:
    AdminPassword(StateDir& state) {
      statedir = &state;

      bool set = is_set();
      if (set) {
        // set the sha256 variable to the already used one
        std::ifstream file(statedir->get_path() + "/admin-pwd");
        std::getline(file, sha256); // no loop since no newlines are expected
        file.close();
      } else {
        std::cout << "no admin password set, to claim admin rights, send:\n" <<
          "  admin-setpwd\n" <<
          "which asks for input, or just send:\n" <<
          "  admin-setpwd:MyPassword\n" << std::endl;
      }
    }

    bool is_set() {
      return std::filesystem::exists(statedir->get_path() + "/admin-pwd") ? true : false;
    }

    /* use when message is just admin-setpwd */
    void setpassword() {
      if (is_set()) { std::cout << "already set\n"; std::exit(1); }

      std::cout << "set a password: ";
      std::string password;
      sendit::helpers::echo_off();
      std::cin >> password;
      sendit::helpers::echo_on();

      std::cout << "hashing...\n";
      set(sha256, password);

      std::cout << "writing...\n";
      write();
    }

    /* use when message is admin-setpwd:... */
    void setpassword(const std::string& message) {
      if (is_set()) { std::cout << "already set\n"; std::exit(1); }

      auto passwordmsg = sendit::helpers::split(message);
      if (passwordmsg.size() != 2) {
        std::cerr << "you must enter a password" << std::endl;
        setpassword();
        std::exit(0);
      }

      std::string password = passwordmsg[1];
      std::cout << "hashing...\n";
      set(hash, password);

      std::cout << "writing...\n";
      write();
    }

    void verify() {
      if (!no_ask_pwd) {
        std::cout << "enter admin password: ";
        std::string pwd;
        sendit::helpers::echo_off();
        std::cin >> pwd;
        sendit::helpers::echo_on();

        pwd = sendit::helpers::sha256(pwd);
        if (pwd != sha256) {
          std::cerr << "wrong password" << std::endl;
          std::exit(1);
        }

        std::cout << "\n";
      }
    }

    /* dangerous - used ONLY in the interactive clients login */
    void dont_ask_for_pwd() {
      no_ask_pwd = true;
    }

    void set_motd(const std::string& msg) {
      if (msg == "unset") {
        try {
          std::filesystem::remove(statedir->get_path() + "/motd");
        } catch (const std::filesystem::filesystem_error& e) {
          std::cout << e.what() << "\n";
          std::exit(1);
        }
        sendit::helpers::alt_msg("motd was unset by admin", "motd", *statedir);
        return;
      }

      std::ofstream motd(statedir->get_path() + "/motd");
      if (!motd.is_open()) {
        std::cerr << "failed to open stream\n";
        std::exit(1);
      }
      motd << msg;
      motd.close();

      sendit::helpers::alt_msg("motd was set by admin", "motd", *statedir);
    }
  };

} // namespace sendit
