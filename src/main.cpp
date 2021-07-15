#include <iostream>
#include <string>

#include "../include/mal.hpp"
#include "../include/readline.hpp"

int main()
{
        std::string input;

        while (readline::read(input))
        {
                auto out = rep(input);
                std::cout << out << std::endl;
        }

        return 0;
}
