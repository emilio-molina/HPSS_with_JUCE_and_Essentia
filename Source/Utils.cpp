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
