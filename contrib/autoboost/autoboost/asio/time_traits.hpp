//
// time_traits.hpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef AUTOBOOST_ASIO_TIME_TRAITS_HPP
#define AUTOBOOST_ASIO_TIME_TRAITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <autoboost/asio/detail/socket_types.hpp> // Must come before posix_time.

#if defined(AUTOBOOST_ASIO_HAS_AUTOBOOST_DATE_TIME) \
  || defined(GENERATING_DOCUMENTATION)

#include <autoboost/date_time/posix_time/posix_time_types.hpp>

#include <autoboost/asio/detail/push_options.hpp>

namespace autoboost {
namespace asio {

/// Time traits suitable for use with the deadline timer.
template <typename Time>
struct time_traits;

/// Time traits specialised for posix_time.
template <>
struct time_traits<autoboost::posix_time::ptime>
{
  /// The time type.
  typedef autoboost::posix_time::ptime time_type;

  /// The duration type.
  typedef autoboost::posix_time::time_duration duration_type;

  /// Get the current time.
  static time_type now()
  {
#if defined(AUTOBOOST_DATE_TIME_HAS_HIGH_PRECISION_CLOCK)
    return autoboost::posix_time::microsec_clock::universal_time();
#else // defined(AUTOBOOST_DATE_TIME_HAS_HIGH_PRECISION_CLOCK)
    return autoboost::posix_time::second_clock::universal_time();
#endif // defined(AUTOBOOST_DATE_TIME_HAS_HIGH_PRECISION_CLOCK)
  }

  /// Add a duration to a time.
  static time_type add(const time_type& t, const duration_type& d)
  {
    return t + d;
  }

  /// Subtract one time from another.
  static duration_type subtract(const time_type& t1, const time_type& t2)
  {
    return t1 - t2;
  }

  /// Test whether one time is less than another.
  static bool less_than(const time_type& t1, const time_type& t2)
  {
    return t1 < t2;
  }

  /// Convert to POSIX duration type.
  static autoboost::posix_time::time_duration to_posix_duration(
      const duration_type& d)
  {
    return d;
  }
};

} // namespace asio
} // namespace autoboost

#include <autoboost/asio/detail/pop_options.hpp>

#endif // defined(AUTOBOOST_ASIO_HAS_AUTOBOOST_DATE_TIME)
       // || defined(GENERATING_DOCUMENTATION)

#endif // AUTOBOOST_ASIO_TIME_TRAITS_HPP
