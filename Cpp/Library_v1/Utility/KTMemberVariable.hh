/*
 * KTMemberVariable.hh
 *
 *  Created on: Aug 5, 2014
 *      Author: nsoblath
 */

#ifndef KTMEMBERVARIABLE_HH_
#define KTMEMBERVARIABLE_HH_

/**
 * Macros for class member variables
 *
 * In all cases remember to initialize the variables!
 *
 * For "normal" variables
 * Defines accessors [type GetMyVar() const], [void SetMyVar( type )], and member variable [type fMyVar]
 * The Set function is not available if the _NOSET macros are used
 *  - MEMBERVARIABLE
 *  - MEMBERVARIABLE_NOSET
 *  - MEMBERVARIABLE_STATIC
 *  - MEMBERVARIABLE_STATIC_NOSET
 *
 * For variables accessed by reference
 * Defines accessors [const type& MyVar() const], [type& MyVar()], and member variable [type fMyVar]
 * The non-const function is not available if the _CONST macros are used
 *  - MEMBERVARIABLE_REF
 *  - MEMBERVARIABLE_REF_CONST
 *  - MEMBERVARIABLE_REF_STATIC
 *  - MEMBERVARIABLE_REF_STATIC_CONST
 *
 * For pointer variables
 * Defines accessors [type* GetMyVar() const], [void SetMyVar( type* )], and member variable [type* fMyVar]
 * The Set function is not available if the _NOSET macros are used
 *  - MEMBERVARIABLE_PTR
 *  - MEMBERVARIABLE_PTR_NOSET
 *  - MEMBERVARIABLE_PTR_STATIC
 *  - MEMBERVARIABLE_PTR_STATIC_NOSET
 *
 * For shared_ptr's
 * Defines accessors [const std::shared_ptr< type > MyVar() const], [std::shared_ptr< type > MyVar()], and member variable [std::shared_ptr< type > fMyVar]
 * The non-const function is not available if the _CONST macros are used
 *  - MEMBERVARIABLE_SHARED_PTR
 *  - MEMBERVARIABLE_SHARED_PTR_CONST
 *  - MEMBERVARIABLE_SHARED_PTR_STATIC
 *  - MEMBERVARIABLE_SHARED_PTR_STATIC_CONST
 *
 * For atomic variables
 * Defines accessors [type GetMyVar() const], [void SetMyVar( type )], and member variable [std::atomic< type > fMyVar]
 * The Set function is not available if the _NOSET macros are used
 *  - MEMBERVARIABLE_ATOMIC
 *  - MEMBERVARIABLE_ATOMIC_NOSET
 *  - MEMBERVARIABLE_ATOMIC_STATIC
 *  - MEMBERVARIABLE_ATOMIC_STATIC_NOSET
 *
 */

#define set_prefix Set
#define get_prefix Get
#define var_prefix f
#define static_prefix s

#include "_camel_case_member_variables.hh"

#define MEMBERVARIABLE                         camel_case_mv_accessible
#define MEMBERVARIABLE_NOSET                   camel_case_mv_accessible_noset
#define MEMBERVARIABLE_STATIC                  camel_case_mv_accessible_static
#define MEMBERVARIABLE_STATIC_NOSET            camel_case_mv_accessible_static_noset
#define MEMBERVARIABLE_MUTABLE                 camel_case_mv_accessible_mutable
#define MEMBERVARIABLE_MUTABLE_NOSET           camel_case_mv_accessible_mutable_noset

#define MEMBERVARIABLE_REF                     camel_case_mv_referrable
#define MEMBERVARIABLE_REF_CONST               camel_case_mv_referrable_const
#define MEMBERVARIABLE_REF_STATIC              camel_case_mv_referrable_static
#define MEMBERVARIABLE_REF_MUTABLE             camel_case_mv_referrable_mutable
#define MEMBERVARIABLE_REF_MUTABLE_CONST       camel_case_mv_referrable_mutable

#define MEMBERVARIABLE_PTR                     camel_case_mv_assignable
#define MEMBERVARIABLE_PTR_NOSET               camel_case_mv_assignable_noset
#define MEMBERVARIABLE_PTR_STATIC              camel_case_mv_assignable_static
#define MEMBERVARIABLE_PTR_STATIC_NOSET        camel_case_mv_assignable_static_noset
#define MEMBERVARIABLE_PTR_MUTABLE             camel_case_mv_assignable_mutable
#define MEMBERVARIABLE_PTR_MUTABLE_NOSET       camel_case_mv_assignable_mutable_noset

#define MEMBERVARIABLE_SHARED_PTR              camel_case_mv_shared_ptr
#define MEMBERVARIABLE_SHARED_PTR_CONST        camel_case_mv_shared_ptr_const
#define MEMBERVARIABLE_SHARED_PTR_STATIC       camel_case_mv_shared_ptr_static
#define MEMBERVARIABLE_SHARED_PTR_MUTABLE      camel_case_mv_shared_ptr_mutable
#define MEMBERVARIABLE_SHARED_PTR_MUTABLE_CONST camel_case_mv_shared_ptr_mutable

#define MEMBERVARIABLE_ATOMIC                  camel_case_mv_atomic
#define MEMBERVARIABLE_ATOMIC_NOSET            camel_case_mv_atomic_noset
#define MEMBERVARIABLE_ATOMIC_STATIC           camel_case_mv_atomic_static
#define MEMBERVARIABLE_ATOMIC_STATIC_NOSET     camel_case_mv_atomic_static_noset
#define MEMBERVARIABLE_ATOMIC_MUTABLE          camel_case_mv_atomic_mutable
#define MEMBERVARIABLE_ATOMIC_MUTABLE_NOSET    camel_case_mv_atomic_mutable_noset

#endif /* KTMEMBERVARIABLE_HH_ */
