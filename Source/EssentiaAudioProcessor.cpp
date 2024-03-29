/*
  ==============================================================================

    AudioProcessor.cpp
    Created: 27 Apr 2019 12:23:52am
    Author:  Emilio Molina

  ==============================================================================
*/

#include "EssentiaAudioProcessor.h"
#include "Utils.h"

EssentiaAudioProcessor::EssentiaAudioProcessor() {
    essentia::init();
    _frameSize = 2048;
    _hopSize = 512;

    essentia::standard::AlgorithmFactory& factory = essentia::standard::AlgorithmFactory::instance();
    _frameCutterAlgorithm = factory.create("FrameCutter",
                                           "frameSize", _frameSize,
                                           "hopSize", _hopSize);
    _windowAlgorithm = factory.create("Windowing",
                                      "type", "blackmanharris62");
    _fftAlgorithm = factory.create("FFT");
    _ifftAlgorithm = factory.create("IFFT");
    _overlapAddAlgorithm = factory.create("OverlapAdd",
                                          "frameSize", _frameSize,
                                          "gain", 1.0f/(float)_frameSize,
                                          "hopSize", _hopSize);
    
    // --- connect network
    
    _frameCutterAlgorithm->input("signal").set(_samples);
    _frameCutterAlgorithm->output("frame").set(_frame);
    
    _windowAlgorithm->input("frame").set(_frame);
    _windowAlgorithm->output("frame").set(_windowedFrame);
    
    _fftAlgorithm->input("frame").set(_windowedFrame);
    _fftAlgorithm->output("fft").set(_fft);
    // --- connect network for synthesis
    
    _ifftAlgorithm->input("fft").set(_stftFrame);
    _ifftAlgorithm->output("frame").set(_synthesizedFrame);
    _overlapAddAlgorithm->input("signal").set(_synthesizedFrame);
    _overlapAddAlgorithm->output("signal").set(_synthesizedFrameOLA);
}

EssentiaAudioProcessor::~EssentiaAudioProcessor() {
    essentia::shutdown();
}

void EssentiaAudioProcessor::readSignalFromInputBuffer(AudioSampleBuffer &inputBuffer,
                                                       int channel) {
    if (inputBuffer.getNumChannels() == 1)
        channel = 0;
    const float* readPtr = inputBuffer.getReadPointer(channel);
    int numSamples = inputBuffer.getNumSamples();
    _samples.resize(numSamples);
    for (int i=0; i<numSamples; i++) {
        _samples[i] = readPtr[i];
    }
}


void EssentiaAudioProcessor::process(float gain_harm, float gain_perc, float gain_residual) {
    _frameCutterAlgorithm->reset();
    _stft.clear();
    int num_frames = 0;
    while (true) {
        // compute a frame
        _frameCutterAlgorithm->compute();
        
        if (!_frame.size()) {
            break;
        }
        num_frames++;
        _windowAlgorithm->compute();
        _fftAlgorithm->compute();
        _stft.push_back(_fft);
    }
    computeHPSS(gain_harm, gain_perc, gain_residual);
    computeISTFT(_stft);
}


void EssentiaAudioProcessor::computeISTFT(std::vector<std::vector<std::complex<float>>> &stft) {
    _synthesizedSamples.clear();
    _synthesizedFrameOLA.clear();
    _overlapAddAlgorithm->reset();
    for (int i=0; i<stft.size(); i++) {
        _stftFrame = stft[i];
        _ifftAlgorithm->compute();
        _overlapAddAlgorithm->compute();
        // skip first half window
        if (i >= _frameSize / (_hopSize * 2)){
            _synthesizedSamples.insert(_synthesizedSamples.end(),
                                       _synthesizedFrameOLA.begin(),
                                       _synthesizedFrameOLA.end());
        }
    }
}


void EssentiaAudioProcessor::getSynthesizedSamples(std::vector<float> &synthesizedSamples) {
    synthesizedSamples = _synthesizedSamples;
}

void EssentiaAudioProcessor::computeHPSS(float gain_harm, float gain_perc, float gain_residual) {
    std::vector<std::vector<float>> stftMag(_stft.size(), std::vector<float>(_stft[0].size()));
    for (int i=0; i<_stft.size(); i++) {
        for (int j=0; j<_stft[i].size(); j++) {
            stftMag[i][j] = std::abs(_stft[i][j]);
        }
    }
    std::vector<std::vector<float>> stftHarmMag(_stft.size(), std::vector<float>(_stft[0].size()));
    std::vector<std::vector<float>> stftPercMag(_stft.size(), std::vector<float>(_stft[0].size()));
    hpss(stftMag, 131, 51, 2.5f, 1.0f, 1.0f, stftHarmMag, stftPercMag);
    float phase, mag;
    for (int i=0; i<_stft.size(); i++) {
        for (int j=0; j<_stft[i].size(); j++) {
            phase = std::arg(_stft[i][j]);
            mag =  ((gain_harm * stftHarmMag[i][j]) +
                    (gain_perc * stftPercMag[i][j]) +
                    (gain_residual * (stftMag[i][j] - stftHarmMag[i][j] - stftPercMag[i][j])));
            _stft[i][j] = std::polar(mag, phase);
        }
    }
}
