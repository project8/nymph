/*
 * MemberVariable.hh
 *
 *  Created on: Aug 5, 2014
 *      Author: nsoblath
 */

#ifndef NYMPH_MEMBERVARIABLE_HH_
#define NYMPH_MEMBERVARIABLE_HH_

/**
 * Macros for class member variables
 *
 * In all cases remember to initialize the variables!
 *
 * For "normal" variables
 * Defines accessors [type GetMyVar() const], [void SetMyVar( type )], and member variable [type fMyVar]
 * The Set function is not available if the _NOSET macros are used
 *  - MEMVAR
 *  - MEMVAR_NOSET
 *  - MEMVAR_STATIC
 *  - MEMVAR_STATIC_NOSET
 *
 * For variables accessed by reference
 * Defines accessors [const type& MyVar() const], [type& MyVar()], and member variable [type fMyVar]
 * The non-const function is not available if the _CONST macros are used
 *  - MEMVAR_REF
 *  - MEMVAR_REF_CONST
 *  - MEMVAR_REF_STATIC
 *  - MEMVAR_REF_STATIC_CONST
 *
 * For pointer variables
 * Defines accessors [type* GetMyVar() const], [void SetMyVar( type* )], and member variable [type* fMyVar]
 * The Set function is not available if the _NOSET macros are used
 *  - MEMVAR_PTR
 *  - MEMVAR_PTR_NOSET
 *  - MEMVAR_PTR_STATIC
 *  - MEMVAR_PTR_STATIC_NOSET
 *
 * For shared_ptr's
 * Defines accessors [const std::shared_ptr< type > MyVar() const], [std::shared_ptr< type > MyVar()], and member variable [std::shared_ptr< type > fMyVar]
 * The non-const function is not available if the _CONST macros are used
 *  - MEMVAR_SHARED_PTR
 *  - MEMVAR_SHARED_PTR_CONST
 *  - MEMVAR_SHARED_PTR_STATIC
 *  - MEMVAR_SHARED_PTR_STATIC_CONST
 *
 * For atomic variables
 * Defines accessors [type GetMyVar() const], [void SetMyVar( type )], and member variable [std::atomic< type > fMyVar]
 * The Set function is not available if the _NOSET macros are used
 *  - MEMVAR_ATOMIC
 *  - MEMVAR_ATOMIC_NOSET
 *  - MEMVAR_ATOMIC_STATIC
 *  - MEMVAR_ATOMIC_STATIC_NOSET
 *
 */

#include "_camel_case_member_variables.hh"

#define MEMVAR                         camel_case_mv_accessible
#define MEMVAR_NOSET                   camel_case_mv_accessible_noset
#define MEMVAR_STATIC                  camel_case_mv_accessible_static
#define MEMVAR_STATIC_NOSET            camel_case_mv_accessible_static_noset
#define MEMVAR_MUTABLE                 camel_case_mv_accessible_mutable
#define MEMVAR_MUTABLE_NOSET           camel_case_mv_accessible_mutable_noset

#define MEMVAR_REF                     camel_case_mv_referrable
#define MEMVAR_REF_CONST               camel_case_mv_referrable_const
#define MEMVAR_REF_STATIC              camel_case_mv_referrable_static
#define MEMVAR_REF_MUTABLE             camel_case_mv_referrable_mutable
#define MEMVAR_REF_MUTABLE_CONST       camel_case_mv_referrable_mutable

#define MEMVAR_PTR                     camel_case_mv_assignable
#define MEMVAR_PTR_NOSET               camel_case_mv_assignable_noset
#define MEMVAR_PTR_STATIC              camel_case_mv_assignable_static
#define MEMVAR_PTR_STATIC_NOSET        camel_case_mv_assignable_static_noset
#define MEMVAR_PTR_MUTABLE             camel_case_mv_assignable_mutable
#define MEMVAR_PTR_MUTABLE_NOSET       camel_case_mv_assignable_mutable_noset

#define MEMVAR_SHARED_PTR              camel_case_mv_shared_ptr
#define MEMVAR_SHARED_PTR_CONST        camel_case_mv_shared_ptr_const
#define MEMVAR_SHARED_PTR_STATIC       camel_case_mv_shared_ptr_static
#define MEMVAR_SHARED_PTR_MUTABLE      camel_case_mv_shared_ptr_mutable
#define MEMVAR_SHARED_PTR_MUTABLE_CONST camel_case_mv_shared_ptr_mutable

#define MEMVAR_ATOMIC                  camel_case_mv_atomic
#define MEMVAR_ATOMIC_NOSET            camel_case_mv_atomic_noset
#define MEMVAR_ATOMIC_STATIC           camel_case_mv_atomic_static
#define MEMVAR_ATOMIC_STATIC_NOSET     camel_case_mv_atomic_static_noset
#define MEMVAR_ATOMIC_MUTABLE          camel_case_mv_atomic_mutable
#define MEMVAR_ATOMIC_MUTABLE_NOSET    camel_case_mv_atomic_mutable_noset

#endif /* NYMPH_MEMBERVARIABLE_HH_ */
