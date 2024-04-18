#include <cmath>
#include <pybind11/pybind11.h>
#include <matplot/matplot.h>

namespace py = pybind11;
namespace mp = matplot;

void generate_signal() {
    std::vector<double> x = mp::linspace(0, 2 * mp::pi);
    std::vector<double> y = mp::transform(x, [](auto x) { return sin(x); });
    mp::plot(x, y, "-o");
    mp::show();
}
PYBIND11_MODULE(siganalysis, handle) {
    handle.doc() = "Module that adds two float numbers";
    handle.def("generate_signal", &generate_signal, "Test");
}