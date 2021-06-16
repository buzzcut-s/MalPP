#ifndef READLINE_HPP
#define READLINE_HPP

#include <string>

namespace readline
{

bool read(std::string& input, const std::string& prompt = "user> ",
          const std::string& history_path = "malpp_history");

};  // namespace readline

#endif  // READLINE_HPP
