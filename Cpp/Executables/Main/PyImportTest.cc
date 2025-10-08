
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

int main()
{
    py::scoped_interpreter guard{};
    
    py::module_ sys = py::module_::import("sys");
    py::print(sys.attr("path"));

    return 0;
}
