#ifndef NYMPH_PYBIND_BINDING_HELPERS
#define NYMPH_PYBIND_BINDING_HELPERS

#include "pybind11/iostream.h"

#define NYMPH_BIND_CALL_GUARD_STREAMS \
    pybind11::call_guard< pybind11::scoped_ostream_redirect, pybind11::scoped_estream_redirect >()

#define NYMPH_BIND_CALL_GUARD_GIL \
    pybind11::call_guard< pybind11::gil_scoped_release >()

#define NYMPH_BIND_CALL_GUARD_STREAMS_AND_GIL \
    pybind11::call_guard< pybind11::scoped_ostream_redirect, pybind11::scoped_estream_redirect, pybind11::gil_scoped_release >()

#endif /* NYMPH_PYBIND_BINDING_HELPERS */