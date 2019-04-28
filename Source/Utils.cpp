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
