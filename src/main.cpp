#include <iostream>
#include <string>

#include "../include/mal.hpp"

int main()
{
        while (true)
        {
                std::cout << "user> ";

                std::string input;
                std::getline(std::cin, input);

                if (!std::cin)
                        return 1;

                std::cout << rep(input) << "\n";
        }

        return 0;
}
