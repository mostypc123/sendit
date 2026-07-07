#include <cstdlib>
#include <ctime>
#include <string>
#include <openssl/evp.h>
#include <format>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <sstream>

namespace sendit::helpers {

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

  void echo_off() {
      termios tty;
      tcgetattr(STDIN_FILENO, &tty);
      tty.c_lflag &= ~ECHO;
      tcsetattr(STDIN_FILENO, TCSANOW, &tty);
  }

  void echo_on() {
      termios tty;
      tcgetattr(STDIN_FILENO, &tty);
      tty.c_lflag |= ECHO;
      tcsetattr(STDIN_FILENO, TCSANOW, &tty);
  }

  std::vector<std::string> split(const std::string& s, char delim = ':') {
      std::vector<std::string> parts;
      std::stringstream ss(s);
      std::string item;
      while (std::getline(ss, item, delim)) {
          parts.push_back(item);
      }
      return parts;
  }

  std::vector<int> random_rgb_color() {
    std::srand(std::time(0));
    std::vector<int> result;
    for ([[maybe_unused]] int i : {0, 1, 2}) {
      result.push_back(std::rand() % 256);
    }
    return result;
  }

} // namespace sendit::helpers
