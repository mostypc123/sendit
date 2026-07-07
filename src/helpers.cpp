#include <cstdlib>
#include <ctime>
#include <string>
#include <openssl/evp.h>
#include <format>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <vector>
#include <sstream>
#include <iostream>
#include "message.hh"
#include "state_dir.hh"

namespace sendit::helpers {

  /* Hash a string with SHA256.
   * To use, add #include "sha256.hh".
   */
  std::string sha256(const std::string& data) {
      EVP_MD_CTX* ctx = EVP_MD_CTX_new();
      EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
      EVP_DigestUpdate(ctx, data.data(), data.size());

      unsigned char hash[EVP_MAX_MD_SIZE];
      unsigned int len;
      EVP_DigestFinal_ex(ctx, hash, &len);
      EVP_MD_CTX_free(ctx);

      std::string result;
      for (unsigned int i = 0; i < len; i++)
          result += std::format("{:02x}", hash[i]);
      return result;
  }

  /* Disable terminal echo.
   * To use, add #include "terminal.hh".
   */
  void echo_off() {
      termios tty;
      tcgetattr(STDIN_FILENO, &tty);
      tty.c_lflag &= ~ECHO;
      tcsetattr(STDIN_FILENO, TCSANOW, &tty);
  }

  /* Enable terminal echo.
   * To use, add #include "terminal.hh".
   */
  void echo_on() {
      termios tty;
      tcgetattr(STDIN_FILENO, &tty);
      tty.c_lflag |= ECHO;
      tcsetattr(STDIN_FILENO, TCSANOW, &tty);
  }

  /* Split a string by delimeter. Returns a vector of strings.
   * To use, add #include "split.hh".
   */
  std::vector<std::string> split(const std::string& s, char delim) {
      std::vector<std::string> parts;
      std::stringstream ss(s);
      std::string item;
      while (std::getline(ss, item, delim)) {
          parts.push_back(item);
      }
      return parts;
  }

  /* Generates a random RGB color. Returns a vector with 3 items.
   * To use, add #include "random_rgb.hh".
   */
  std::vector<int> random_rgb_color() {
    std::srand(std::time(0));
    std::vector<int> result;
    result.reserve(3);
    for ([[maybe_unused]] int i : {0, 1, 2}) {
      result.push_back(std::rand() % 256);
    }
    return result;
  }

  /* Clears the screen.
   * To use, add #include "terminal.hh".
   */
  void cls() {
    std::cout << "\033[H\033[2J\033[3J";
  }

  /* Prints a separator from a character, default '-'.
   * To use, add #include "terminal.hh".
   */
  void separator(char fill) {
      struct winsize w;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

      std::size_t width = w.ws_col > 0 ? w.ws_col : 80;

      for (std::size_t i = 0; i < width; ++i) {
          std::cout << fill;
      }
      std::cout << '\n';
  }

  /* Because of some input resolving issue, a func to send a message.
   * To use, add #include "altmsg.hh".
   */
  void alt_msg(
    std::string message,
    std::string username,
    sendit::StateDir& state
  ) {
    sendit::Message msg(message, username, state);
    msg.write();
  }

} // namespace sendit::helpers
