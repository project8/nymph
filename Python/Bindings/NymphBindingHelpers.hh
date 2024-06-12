#ifndef NYMPH_PYBIND_BINDING_HELPERS
#define NYMPH_PYBIND_BINDING_HELPERS

#include "pybind11/iostream.h"

//************
// Call guards
//************

#define NYMPH_BIND_CALL_GUARD_STREAMS \
    pybind11::call_guard< pybind11::scoped_ostream_redirect, pybind11::scoped_estream_redirect >()

#define NYMPH_BIND_CALL_GUARD_GIL \
    pybind11::call_guard< pybind11::gil_scoped_release >()

#define NYMPH_BIND_CALL_GUARD_STREAMS_AND_GIL \
    pybind11::call_guard< pybind11::scoped_ostream_redirect, pybind11::scoped_estream_redirect, pybind11::gil_scoped_release >()

//****************
// Memvar Bindings
//****************

#define PROPERTY( name, getter, setter ) \
    property( name, &getter, &setter )


//Accessibles begin
#define MEMVAR_PY(PYNAME, MEMNAME, CLNAME) \
    .def_property(#PYNAME, &CLNAME::Get##MEMNAME, &CLNAME::Set##MEMNAME)

#define MEMVAR_NOSET_PY(PYNAME, MEMNAME, CLNAME) \
    .def_property_readonly(#PYNAME, &CLNAME::##MEMNAME)

#define MEMVAR_STATIC_PY(PYNAME, MEMNAME, CLNAME) \
    .def_property_static(#PYNAME, &CLNAME::##MEMNAME)

#define MEMVAR_STATIC_NOSET_PY(PYNAME, MEMNAME, CLNAME) \
    .def_property_readonly_static(#PYNAME, &CLNAME::##MEMNAME)

// The following macros assumes that MEMVAR_MUTABLE == MEMVAR and MEMVAR_MUTABLE_NOSET == MEMVAR_NOSET for pythonic purposes
#define MEMVAR_MUTABLE_PY(PYNAME, MEMNAME, CLNAME) \
    .def_property(#PYNAME, &CLNAME::Get##MEMNAME, &CLNAME::Set##MEMNAME)

#define MEMVAR_MUTABLE_NOSET_PY(PYNAME, MEMNAME, CLNAME) \
    .def_property_readonly(#PYNAME, &CLNAME::##MEMNAME)

//Accessibles begin

//Referrables begin
#define MEMVAR_REF_PY(PYNAME, MEMNAME, RETTYPE, CLNAME) \
    .def_property(#PYNAME, static_cast< const RETTYPE& (CLNAME::*)() const>(&CLNAME::MEMNAME),\
        [](CLNAME& anObject, const RETTYPE& aMember){anObject.MEMNAME() = aMember;} )

#define MEMVAR_REF_CONST_PY(PYNAME, MEMNAME, RETTYPE, CLNAME) \
    .def_property_readonly(PYNAME, static_cast< const RETTYPE& (CLNAME::*)() const>(&CLNAME::MEMNAME))

#define MEMVAR_REF_STATIC_PY(PYNAME, MEMNAME, RETTYPE, CLNAME) \
    .def_property_readonly_static(PYNAME, static_cast< RETTYPE& (CLNAME::*)()>(&CLNAME::MEMNAME))

// The following macro assumes that MEMVAR_REF_MUTABLE macro is equivalent to MEMVAR_REF for pythonic purposes
#define MEMVAR_REF_MUTABLE_PY(PYNAME, MEMNAME, RETTYPE, CLNAME) \
    .def_property(#PYNAME, static_cast< RETTYPE& (CLNAME::*)() const>(&CLNAME::MEMNAME),\
        [](CLNAME& anObject, const RETTYPE& aMember){anObject.MEMNAME() = aMember;} ) 

#define MEMVAR_REF_MUTABLE_CONST_PY(PYNAME, MEMNAME, RETTYPE, CLNAME) \
    .def_property_readonly(PYNAME, static_cast< RETTYPE& (CLNAME::*)() const>(&CLNAME::MEMNAME))
//Referrables end

#endif /* NYMPH_PYBIND_BINDING_HELPERS */
