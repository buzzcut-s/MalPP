#include <iostream>
#include <string>

#include "../include/Environment.hpp"
#include "../include/mal.hpp"
#include "../include/readline.hpp"

int main()
{
        std::string input;
        while (readline::read(input))
        {
                static auto repl_env = mal::Environment{};

                auto out = rep(input, repl_env);
                std::cout << out << std::endl;
        }

        return 0;
}
