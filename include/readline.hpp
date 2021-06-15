#ifndef READLINE_H
#define READLINE_H

#include <string>

class Readline
{
public:
        static bool read(std::string& input, const char* prompt = "user> ",
                         const char* history_path = "malpp_history");
};

#endif  // READLINE_H
