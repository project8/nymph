
#include <pybind11/pybind11.h>

namespace py = pybind11;

int main()
{
    py::module_ sys = py::module_::import("sys");
    py::print(sys.attr("path"));

    return 0;
}
