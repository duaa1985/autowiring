/*
Copyright Rene Rivera 2008-2013
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef AUTOBOOST_PREDEF_OS_CYGWIN_H
#define AUTOBOOST_PREDEF_OS_CYGWIN_H

#include <autoboost/predef/version_number.h>
#include <autoboost/predef/make.h>

/*`
[heading `AUTOBOOST_OS_CYGWIN`]

[@http://en.wikipedia.org/wiki/Cygwin Cygwin] evironment.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__CYGWIN__`] [__predef_detection__]]
    ]
 */

#define AUTOBOOST_OS_CYGWIN AUTOBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(AUTOBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__CYGWIN__) \
    )
#   undef AUTOBOOST_OS_CYGWIN
#   define AUTOBOOST_OS_CGYWIN AUTOBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if AUTOBOOST_OS_CYGWIN
#   define AUTOBOOST_OS_CYGWIN_AVAILABLE
#   include <autoboost/predef/detail/os_detected.h>
#endif

#define AUTOBOOST_OS_CYGWIN_NAME "Cygwin"

#include <autoboost/predef/detail/test.h>
AUTOBOOST_PREDEF_DECLARE_TEST(AUTOBOOST_OS_CYGWIN,AUTOBOOST_OS_CYGWIN_NAME)


#endif
