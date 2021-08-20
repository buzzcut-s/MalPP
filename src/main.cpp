#include <iostream>
#include <string>

#include "../include/Environment.hpp"  // MalPP environment
#include "../include/core.hpp"         // MAL core functions
#include "../include/mal.hpp"          // REPL support
#include "../include/readline.hpp"     // linenoise

constexpr auto OUTER = nullptr;

int main()
{
        std::string input;
        const auto  repl_env = std::make_unique<mal::Environment>(OUTER);

        auto core_ns = core::ns_init();
        repl_env->init(std::move(core_ns));

        while (readline::read(input))
        {
                auto out = rep(input, *repl_env);
                std::cout << out << std::endl;
        }

        repl_env->uninit();
        return 0;
}
