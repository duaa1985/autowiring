//  get_current_process.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba
//  Copyright 2015 Andrey Semashev

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef AUTOBOOST_DETAIL_WINAPI_GET_CURRENT_PROCESS_HPP
#define AUTOBOOST_DETAIL_WINAPI_GET_CURRENT_PROCESS_HPP

#include <autoboost/detail/winapi/basic_types.hpp>

#ifdef AUTOBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

// Windows CE define GetCurrentProcess as an inline function in kfuncs.h
#if !defined( AUTOBOOST_USE_WINDOWS_H ) && !defined( UNDER_CE )
extern "C" {
AUTOBOOST_SYMBOL_IMPORT autoboost::detail::winapi::HANDLE_ WINAPI GetCurrentProcess(AUTOBOOST_DETAIL_WINAPI_VOID);
}
#endif

namespace autoboost {
namespace detail {
namespace winapi {
using ::GetCurrentProcess;
}
}
}

#endif // AUTOBOOST_DETAIL_WINAPI_GET_CURRENT_PROCESS_HPP