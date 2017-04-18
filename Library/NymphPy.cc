#include <boost/python.hpp>

/* include components here*/
#include "KTDataPy.hh"

BOOST_PYTHON_MODULE(nymphPy)
{
    using namespace boost::python;
    export_DataPy();
}
