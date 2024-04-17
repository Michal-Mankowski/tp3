#include <pybind11/pybind11.h>

namespace py = pybind11;

float add(float a, float b) {
    return a + b;
}

PYBIND11_MODULE(module_name, handle) {
    handle.doc() = "Module that adds two float numbers";
    handle.def("add", &add, "A function which adds two floats");
}