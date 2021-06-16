#include <string>

#include "../lib/linenoise.hpp"

namespace readline
{

bool read(std::string& input, const std::string& prompt,
          const std::string& history_path)
{
        linenoise::LoadHistory(history_path.c_str());

        const auto eof = linenoise::Readline(prompt.c_str(), input);

        if (eof)
                return false;

        linenoise::AddHistory(input.c_str());
        linenoise::SaveHistory(history_path.c_str());

        return true;
}

};  // namespace readline
