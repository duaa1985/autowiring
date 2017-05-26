/*
Copyright 2015 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef AUTOBOOST_ALIGN_ALIGN_UP_HPP
#define AUTOBOOST_ALIGN_ALIGN_UP_HPP

#include <autoboost/align/detail/align_up.hpp>

namespace autoboost {
namespace alignment {

AUTOBOOST_CONSTEXPR inline std::size_t
align_up(std::size_t value, std::size_t alignment) AUTOBOOST_NOEXCEPT
{
    return (value + alignment - 1) & ~(alignment - 1);
}

} /* alignment */
} /* autoboost */

#endif