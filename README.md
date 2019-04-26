# Harmonic-Percussive separation using Essentia and JUCE

This project uses a light-weight, static version of Essentia, that should be easy to compile and to integrate into JUCE project.

## Init submodules

```
$ git submodule init
$ git submodule update
```

## Compile Essentia

In my case, the only dependency use is `fftw` (because I was able to easily install it with `brew`).
```
$ cd third_party/essentia
$ ./waf configure --lightweight=fftw --build-static --ignore-algos=Spline,CubicSpline
$ #./waf configure --lightweight= --fft=KISS --ignore-algos=Spline,CubicSpline --with-static-examples  # if fftw not installed, KISSfft can be used instead
$./waf
```
