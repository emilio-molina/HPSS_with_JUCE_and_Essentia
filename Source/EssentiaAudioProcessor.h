/*
  ==============================================================================

    AudioProcessor.h
    Created: 27 Apr 2019 12:23:52am
    Author:  Emilio Molina

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <essentia/algorithmfactory.h>
#include <essentia/essentiamath.h>
#include <essentia/pool.h>
#include <vector>
#include <complex>

class EssentiaAudioProcessor {
    essentia::standard::Algorithm* _frameCutterAlgorithm;
    essentia::standard::Algorithm* _windowAlgorithm;
    essentia::standard::Algorithm* _fftAlgorithm;
    essentia::standard::Algorithm* _ifftAlgorithm;
    essentia::standard::Algorithm* _overlapAddAlgorithm;
    essentia::standard::Algorithm* _fluxAlgorithm;
    std::vector<float> _samples, _synthesizedSamples;
    std::vector<float> _frame, _synthesizedFrame, _synthesizedFrameOLA, _windowedFrame;
    std::vector<std::complex<float>> _fft, _stftFrame;
    std::vector<std::vector<std::complex<float>>> _stft;
    std::vector<float> _flux;
    int _frameSize, _hopSize;
public:
    EssentiaAudioProcessor();
    ~EssentiaAudioProcessor();
    void readSignalFromInputBuffer(AudioSampleBuffer &inputBuffer,
                                   int channel);
    void process();
    void computeISTFT(std::vector<std::vector<std::complex<float>>> &stft);
    void computeHPSS(float gain_harm, float gain_perc, float gain_residual);
    void getSynthesizedSamples(std::vector<float> &output);
};
