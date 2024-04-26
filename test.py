from build.siganalysis import *

generate_signal(SignalTypes.sig_square, 11, 10000, 1, 1)
print(dft(generate_signal(SignalTypes.sig_square, 11, 11, 1, 1)))