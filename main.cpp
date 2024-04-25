#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

std::vector<std::complex<double>> dft(std::vector<double> X)
{
    int N = X.size();
    int K = N;

    std::complex<double> sum;
    std::vector<std::complex<double>> output;

    for(int k=0; k<K; k++){
        sum = std::complex<double>(0, 0);
        for(int n=0; n<N; n++){
            double realPart = cos((2*M_PI*k*n)/N);
            double imagPart = sin((2*M_PI*k*n)/N);
            std::complex<double> w (realPart, -imagPart);
            sum += X[n]*w;
        }
        output.push_back(sum);
    }
    return output;
}

int main()
{
    std::vector<double> signal{1.0, 2.0, 3.0, 4.0};
    std::vector<std::complex<double>> after_dft = dft(signal);
    for(int i=0; i<after_dft.size(); i++){
        std::cout<<after_dft[i]<<std::endl;
    }

    return 0;
}
