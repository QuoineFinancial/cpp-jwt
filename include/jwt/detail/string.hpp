#pragma once

#include <string>
#include <string_view>

namespace jwt::detail
{
    inline auto
    to_string(std::string_view sv) -> std::string
    {
        return std::string(sv.begin(), sv.end());
    }

}