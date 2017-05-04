/*
 * KTPythonMacros.hh
 *
 * Created on: April 26, 2017
 *     Author: laroque
 */

#ifndef KTPYTHONMACROS_HH_
#define KTPYTHONMACROS_HH_

#define PROPERTYMEMBER( CLASS, MEMBER ) \
    .add_property( #MEMBER, &CLASS::Get##MEMBER, &CLASS::Set##MEMBER )

#define PROPERTYMEMBER_NOSET( CLASS, MEMBER ) \
    .add_property( #MEMBER, &CLASS::Get##MEMBER, boost::python::no_set )

#define PROPERTYMEMBER_RETPOLICY( CLASS, MEMBER, RETPOLICY ) \
    .add_property( #MEMBER, make_function( &CLASS::Get##MEMBER, return_value_policy<RETPOLICY>()), &CLASS::Set##MEMBER )

#define PROPERTYMEMBER_RETPOLICY_NOSET( CLASS, MEMBER, RETPOLICY ) \
    .add_property( #MEMBER, make_function( &CLASS::Get##MEMBER, return_value_policy<RETPOLICY>()), boost::python::no_set )

#endif /* KTPYTHONMACROS_HH_ */
