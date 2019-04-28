/*
  ==============================================================================

    Utils.h
    Created: 27 Apr 2019 12:30:25am
    Author:  Emilio Molina

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

void readFromAudioFile(const File& inputAudioFile,
                       AudioSampleBuffer& buffer,
                       AudioFormatManager& formatManager);

void writeToAudioFile(AudioSampleBuffer& buffer,
                    int sampleRate,
                    String& outputAudioPath);

bool isReadableAudioFile(const File& inputFile,
                         AudioFormatManager &formatManager);
