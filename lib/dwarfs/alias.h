#pragma once
#include <memory>
#include <string.h>

template <typename T>
using uptr = std::unique_ptr<T>;

using str = std::string;

