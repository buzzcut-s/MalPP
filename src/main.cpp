#include <iostream>
#include <string>

#include "../include/mal.hpp"
#include "../include/readline.hpp"

int main()
{
        std::string input;

        while (readline::read(input))
        {
                std::cout << rep(input) << "\n";
        }

        return 0;
}
