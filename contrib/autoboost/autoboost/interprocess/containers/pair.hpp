//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2008-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef AUTOBOOST_INTERPROCESS_CONTAINERS_PAIR_HPP
#define AUTOBOOST_INTERPROCESS_CONTAINERS_PAIR_HPP

#ifndef AUTOBOOST_CONFIG_HPP
#  include <autoboost/config.hpp>
#endif
#
#if defined(AUTOBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <autoboost/interprocess/detail/config_begin.hpp>
#include <autoboost/container/detail/pair.hpp>
#include <autoboost/interprocess/containers/containers_fwd.hpp>

namespace autoboost {
namespace interprocess {

using autoboost::container::container_detail::pair;

}  //namespace interprocess {
}  //namespace autoboost {

#include <autoboost/interprocess/detail/config_end.hpp>

#endif //   #ifndef  AUTOBOOST_INTERPROCESS_CONTAINERS_PAIR_HPP
