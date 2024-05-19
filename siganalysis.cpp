#include <cmath>
#include <thread>
#include <future>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <matplot/matplot.h>


namespace py = pybind11;
namespace mp = matplot;

enum SignalTypes {
    sig_sin,
    sig_cos,
    sig_square,
    sig_sawtooth,
};

std::vector<double> generate_signal(const SignalTypes& signal_type, const double& time, const int& sampling, const double& amplitude, const double& frequency) {
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
    if (signal_type == sig_sawtooth) {
        y = mp::transform(x, [&](auto x) { return amplitude * (x - floor(x)); });
    }
    return y;
}

void visualize_signal(const std::vector<double> &signal, const double& time, const std::string& filename) {
    std::vector<double> t = mp::linspace(0, time, signal.size());
    mp::title("Signal");
    mp::xlabel("Time");
    mp::ylabel("Value");
    mp::grid(mp::on);
    mp::plot(t, signal);
    mp::save(filename);
}

std::vector<std::complex<double>> dft(const std::vector<double>& X)
{
    int N = X.size();
    std::vector<std::complex<double>> output(N, 0.0);

    for(int k = 0; k < N; k++){
        std::complex<double> sum(0, 0);
        for(int n = 0; n < N; n++){
            double angle = -2 * M_PI * k * n / N;
            std::complex<double> w(cos(angle), sin(angle));
            sum += X[n] * w;
        }
        output[k] = sum;
    }
    return output;
}

std::vector<double> dft(const std::vector<std::complex<double>>& X)
{
    int N = X.size();
    std::vector<double> output(N, 0.0);
    
    for(int n = 0; n < N; n++){
        std::complex<double> sum(0, 0);
        for(int k = 0; k < N; k++){
            double angle = 2 * M_PI * k * n / N;
            std::complex<double> w(cos(angle), sin(angle));
            sum += X[k] * w;
        }
        output[n] = (sum / static_cast<double>(N)).real();
    }
    return output;
}

std::vector<double> frequency_filter(const std::vector<double>& signal, const double& cutoff) {
    auto signalAfterDFT = dft(signal);
    int size = signal.size();;
    for (int i = 0; i < cutoff && i < size; ++i) {
        signalAfterDFT[i] = std::complex<double>(0, 0);
    }
    for (int i = size - 1; i >= size - cutoff; --i) {
        signalAfterDFT[i] = std::complex<double>(0, 0);
    }
    return dft(signalAfterDFT);

}

std::vector<double> complexToPowerVector(const std::vector<std::complex<double>>& X) {
    int size = X.size();
    std::vector<double> Power(size);
    for (int i = 0; i < size; i++) {
        Power[i] = X[i].real() * X[i].real() + X[i].imag() * X[i].imag();
    }
    return Power;
}

PYBIND11_MODULE(siganalysis, handle) {
    handle.doc() = "Module that generates signals, visualises them and manipulates them";
    handle.def("generate_signal", &generate_signal, "Generates signal");
    handle.def("dft", py::overload_cast<const std::vector<double>&>(&dft), "Harmonizing the signal, also can reverse it");
    handle.def("dft", py::overload_cast<const std::vector<std::complex<double>>&>(&dft), "Harmonizing the signal, also can reverse it");
    handle.def("visualize_signal", &visualize_signal, "Visualizes the signal");
    handle.def("complexToPowerVector", &complexToPowerVector, "Converts a complex vector into a double vector with the powers of the complex numbers");
    handle.def("frequency_filter", &frequency_filter, "Filters the signal by frequency");
    py::enum_<SignalTypes>(handle, "SignalTypes")
        .value("sig_sin", SignalTypes::sig_sin)
        .value("sig_cos", SignalTypes::sig_cos)
        .value("sig_square", SignalTypes::sig_square)
        .value("sig_sawtooth", SignalTypes::sig_sawtooth)
        .export_values();
}
