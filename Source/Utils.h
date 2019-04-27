/*
  ==============================================================================

    Utils.h
    Created: 27 Apr 2019 12:30:25am
    Author:  Emilio Molina

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

void readAudioFile(File& inputAudioFile, AudioSampleBuffer& buffer);

void writeAudioFile(AudioSampleBuffer& buffer,
                    int sampleRate,
                    String& outputAudioPath);

bool isReadableAudioFile(const File& inputFile,
                         AudioFormatManager &formatManager);
