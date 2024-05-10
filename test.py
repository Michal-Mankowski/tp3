import wave
import struct
from build.siganalysis import *

def read_wave(filename: str) -> tuple[float, list[float]]:
    with wave.open(filename, 'rb') as f:
        data = f.readframes(f.getnframes())
        duration = f.getnframes() / f.getframerate()
        samples = struct.unpack('{n}h'.format(n=f.getnframes()*f.getnchannels()), data)
        return duration, samples

def write_wave(filename: str, signal: list, duration: int):
    channels = 2
    framerate = int(len(signal) / (duration * channels)) 
    samplewidth = 2;
    with wave.open(filename, 'wb') as f:
        f.setnchannels(channels)
        f.setsampwidth(samplewidth)
        f.setframerate(framerate)
        signal_bytes = struct.pack('<' + 'h' * len(signal), *[int(sample) for sample in signal])
        f.writeframes(signal_bytes)

audio = read_wave('Power_of_NEO.wav')
signal = generate_signal(SignalTypes.sig_sin, 10, 50000, 1, 1)
signal2 = dft(signal)
signal3 = reverse_dft(signal2)
