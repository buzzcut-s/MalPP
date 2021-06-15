#include "../include/readline.hpp"

#include <string>

#include "../lib/linenoise.hpp"

bool Readline::read(std::string& input, const char* prompt, const char* history_path)
{
        linenoise::LoadHistory(history_path);

        const auto eof = linenoise::Readline(prompt, input);

        if (eof)
                return false;

        linenoise::AddHistory(input.c_str());
        linenoise::SaveHistory(history_path);

        return true;
}
