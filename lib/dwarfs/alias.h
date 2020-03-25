#pragma once
#include <memory>
#include <string.h>
#include <functional>
#include <tuple>
#include <deque>
#include <map>

namespace dw {

template <typename K, typename V>
using map = std::map<K, V>;

template <typename T>
using uptr = std::unique_ptr<T>;

using str = std::string;

template <typename T>
using func = std::function<T>;

template <typename... Args>
using tup = std::tuple<Args...>;

template <typename T>
using arr = std::vector<T>;

template <typename T>
using deq = std::deque<T>;

}
