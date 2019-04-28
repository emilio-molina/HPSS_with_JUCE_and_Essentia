/*
  ==============================================================================

    Utils.cpp
    Created: 27 Apr 2019 12:30:25am
    Author:  Emilio Molina

  ==============================================================================
*/

#include "Utils.h"

bool isReadableAudioFile(const File& inputFile,
                         AudioFormatManager& formatManager) {
    auto* reader = formatManager.createReaderFor (inputFile);
    bool isValid = (reader != nullptr);
    delete reader;
    return isValid;
}

void readFromAudioFile(const File& inputAudioFile,
                       AudioSampleBuffer& buffer,
                       AudioFormatManager& formatManager) {
    auto* reader = formatManager.createReaderFor(inputAudioFile);
    if (reader != nullptr)
    {
        //auto duration = reader->lengthInSamples / reader->sampleRate;
        buffer.setSize (reader->numChannels, (int) reader->lengthInSamples);
        reader->read (&buffer,
                      0,  // destination start sample
                      (int) reader->lengthInSamples,  // num samples
                      0,  // reading start sample
                      true,  // true: read left channel
                      true); // true: read right channel
        delete reader;
    }
}

void vectorToBuffer(std::vector<float> &inputL,
                    std::vector<float> &inputR,
                    AudioSampleBuffer &output) {
    output.clear();
    int numChannels = 2;
    output.setSize (numChannels, (int)inputL.size());
    output.copyFrom(0, 0, &inputL[0], (int)inputL.size());
    output.copyFrom(1, 0, &inputR[0], (int)inputR.size());
}

void medianfilter(float* signal, float* result, int N) {
    //   Move window through all elements of the signal
    for (int i = 2; i < N - 2; ++i)
    {
        //   Pick up window elements
        float window[5];
        for (int j = 0; j < 5; ++j)
            window[j] = signal[i - 2 + j];
        //   Order elements (only half of them)
        for (int j = 0; j < 3; ++j)
        {
            //   Find position of minimum element
            int min = j;
            for (int k = j + 1; k < 5; ++k)
                if (window[k] < window[min])
                    min = k;
            //   Put found minimum element in its place
            const float temp = window[j];
            window[j] = window[min];
            window[min] = temp;
        }
        //   Get result - the middle element
        result[i - 2] = window[2];
    }
}
