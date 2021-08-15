#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <memory>
#include <string>

namespace mal
{
class Data;
}

std::string pr_str(std::unique_ptr<mal::Data> value);

#endif  // PRINTER_HPP
