// Copyright (C) 2003, Fernando Luis Cacciola Carballal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/optional for documentation.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
#ifndef AUTOBOOST_NONE_T_17SEP2003_HPP
#define AUTOBOOST_NONE_T_17SEP2003_HPP

namespace autoboost {

namespace detail { struct none_helper{}; }

typedef int detail::none_helper::*none_t ;

} // namespace autoboost

#endif

