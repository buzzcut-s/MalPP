#include <iostream>
#include <string>

#include "../include/Environment.hpp"
#include "../include/mal.hpp"
#include "../include/readline.hpp"

constexpr auto OUTER = nullptr;

int main()
{
        std::string input;
        const auto  repl_env = std::make_unique<mal::Environment>(OUTER);

        repl_env->init();
        while (readline::read(input))
        {
                auto out = rep(input, *repl_env);
                std::cout << out << std::endl;

                repl_env->clear_inner();
        }
        repl_env->uninit();

        return 0;
}
