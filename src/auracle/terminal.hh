#ifndef AURACLE_TERMINAL_HH_
#define AURACLE_TERMINAL_HH_

#include <sstream>
#include <string>

namespace terminal {

enum class WantColor : short {
  YES,
  NO,
  AUTO,
};

void Init(WantColor);

int Columns();

std::string Bold(const std::string& s);
std::string BoldCyan(const std::string& s);
std::string BoldGreen(const std::string& s);
std::string BoldMagenta(const std::string& s);
std::string BoldRed(const std::string& s);

}  // namespace terminal

#endif  // AURACLE_TERMINAL_HH_
