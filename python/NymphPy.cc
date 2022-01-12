#include <boost/python.hpp>

/* include components here*/
#include "KTDataPy.hh"
#include "KTProcessorPy.hh"
#include "KTProcessorToolboxPy.hh"

BOOST_PYTHON_MODULE(NymphPy)
{
    using namespace boost::python;
    export_DataPy();
    export_Processor();
    export_ProcessorToolbox();
}
