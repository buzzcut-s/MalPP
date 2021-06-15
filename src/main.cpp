#include <iostream>
#include <string>
#include <string_view>

#include "../include/mal.hpp"
#include "../lib/linenoise.hpp"

constexpr auto HISTORY_PATH = "malpp_history";
constexpr auto PROMPT       = "user> ";

int main()
{
        while (true)
        {
                linenoise::LoadHistory(HISTORY_PATH);
                std::string input;

                auto quit = linenoise::Readline(PROMPT, input);

                if (quit)
                        break;

                std::cout << rep(input) << "\n";

                linenoise::AddHistory(input.c_str());
        }

        linenoise::SaveHistory(HISTORY_PATH);

        return 0;
}
