
#ifndef AUTOBOOST_MPL_LIST_C_HPP_INCLUDED
#define AUTOBOOST_MPL_LIST_C_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#if !defined(AUTOBOOST_MPL_PREPROCESSING_MODE)
#   include <autoboost/mpl/limits/list.hpp>
#   include <autoboost/mpl/aux_/nttp_decl.hpp>
#   include <autoboost/mpl/aux_/config/preprocessor.hpp>

#   include <autoboost/preprocessor/inc.hpp>
#   include <autoboost/preprocessor/cat.hpp>
#   include <autoboost/preprocessor/stringize.hpp>

#if !defined(AUTOBOOST_NEEDS_TOKEN_PASTING_OP_FOR_TOKENS_JUXTAPOSING)
#   define AUX778076_LIST_C_HEADER \
    AUTOBOOST_PP_CAT(AUTOBOOST_PP_CAT(list,AUTOBOOST_MPL_LIMIT_LIST_SIZE),_c).hpp \
    /**/
#else
#   define AUX778076_LIST_C_HEADER \
    AUTOBOOST_PP_CAT(AUTOBOOST_PP_CAT(list,AUTOBOOST_MPL_LIMIT_LIST_SIZE),_c)##.hpp \
    /**/
#endif

#   include AUTOBOOST_PP_STRINGIZE(autoboost/mpl/list/AUX778076_LIST_C_HEADER)
#   undef AUX778076_LIST_C_HEADER
#   include <climits>
#endif

#include <autoboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(AUTOBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(AUTOBOOST_MPL_PREPROCESSING_MODE)

#   define AUTOBOOST_MPL_PREPROCESSED_HEADER list_c.hpp
#   include <autoboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <autoboost/mpl/limits/list.hpp>

#   define AUX778076_SEQUENCE_NAME list_c
#   define AUX778076_SEQUENCE_LIMIT AUTOBOOST_MPL_LIMIT_LIST_SIZE
#   define AUX778076_SEQUENCE_NAME_N(n) AUTOBOOST_PP_CAT(AUTOBOOST_PP_CAT(list,n),_c)
#   define AUX778076_SEQUENCE_INTEGRAL_WRAPPER
#   include <autoboost/mpl/aux_/sequence_wrapper.hpp>

#endif // AUTOBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // AUTOBOOST_MPL_LIST_C_HPP_INCLUDED