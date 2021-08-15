#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <memory>
#include <string>

namespace mal
{
class Data;
using DataPtr = std::unique_ptr<mal::Data>;
}  // namespace mal

std::string pr_str(mal::DataPtr value);

#endif  // PRINTER_HPP
