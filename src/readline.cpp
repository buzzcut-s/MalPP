#include <string>

#include "../lib/linenoise.hpp"

namespace readline
{
bool read(std::string& input, const char* prompt = "user> ",
          const char* history_path = "malpp_history")
{
        linenoise::LoadHistory(history_path);

        const auto eof = linenoise::Readline(prompt, input);

        if (eof)
                return false;

        linenoise::AddHistory(input.c_str());
        linenoise::SaveHistory(history_path);

        return true;
}

};  // namespace readline
