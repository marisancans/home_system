#pragma once
#include "alias.h"
#include <algorithm>

namespace dw {


template <typename T, typename C>
bool exists(T const& key, C const& c){
    auto it = c.find(key);
    return it != c.end();
}

template <typename T>
T min(T const& a, T const& b){
    return std::min(a, b);
}

}
