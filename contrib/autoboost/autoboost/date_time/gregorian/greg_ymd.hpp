#ifndef AB_DATE_TIME_GREG_YMD_HPP__
#define AB_DATE_TIME_GREG_YMD_HPP__

/* Copyright (c) 2002,2003 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 * Author: Jeff Garland
 * $Date$
 */

#include "autoboost/date_time/year_month_day.hpp"
#include "autoboost/date_time/special_defs.hpp"
#include "autoboost/date_time/gregorian/greg_day.hpp"
#include "autoboost/date_time/gregorian/greg_year.hpp"
#include "autoboost/date_time/gregorian/greg_month.hpp"

namespace autoboost {
namespace gregorian {

  typedef date_time::year_month_day_base<greg_year,
                                         greg_month,
                                         greg_day> greg_year_month_day;



} } //namespace gregorian




#endif

