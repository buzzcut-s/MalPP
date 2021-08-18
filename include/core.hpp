#ifndef CORE_HPP
#define CORE_HPP

#include <functional>
#include <string>
#include <unordered_map>

namespace mal
{
class Data;
}

namespace core
{

using Fn = std::function<mal::Data*(const std::size_t argc, mal::Data* const* args)>;

std::unordered_map<std::string, Fn> ns_init();

}  // namespace core

#endif  // CORE_HPP
