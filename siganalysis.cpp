#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <matplot/matplot.h>

namespace py = pybind11;
namespace mp = matplot;

enum SignalTypes {
    sig_sin,
    sig_cos,
    sig_square,
    sig_one,
    sig_sawtooth,
    sig_triangle
};
std::vector<double> generate_signal(SignalTypes signal_type, double time, double sampling, double amplitude, double frequency) {
    std::vector<double> x = mp::linspace(0, time, sampling);
    std::vector<double> y;
    if (signal_type == sig_sin) {
        y = mp::transform(x, [&](auto x) { return amplitude * sin(frequency * x); });
    }
    if (signal_type == sig_cos) {
        y = mp::transform(x, [&](auto x) { return amplitude * cos(frequency * x); });
    }
    if (signal_type == sig_square) {
        y = mp::transform(x, [&](auto x) { return (sin(frequency * x) > 0) ? amplitude : -amplitude; });
    }
    if (signal_type == sig_one) {
        y = mp::transform(x, [&](auto x) { return (x >= frequency) ? amplitude : 0; });
    }
    if (signal_type == sig_sawtooth) {
        y = mp::transform(x, [&](auto x) { return amplitude * (x - floor(x)); });
    }
    mp::plot(x, y);
    mp::show();
    return y;
}

PYBIND11_MODULE(siganalysis, handle) {
    handle.doc() = "Module that generates signals, visualises them and manipulates them";
    handle.def("generate_signal", &generate_signal, "Generates signal");
    py::enum_<SignalTypes>(handle, "SignalTypes")
        .value("sig_sin", SignalTypes::sig_sin)
        .value("sig_cos", SignalTypes::sig_cos)
        .value("sig_square", SignalTypes::sig_square)
        .value("sig_one", SignalTypes::sig_one)
        .value("sig_sawtooth", SignalTypes::sig_sawtooth)
        .value("sig_triangle", SignalTypes::sig_triangle)
        .export_values();
}