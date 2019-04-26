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

using namespace std;
using namespace essentia;
using namespace essentia::standard;

class AudioProcessor {

public:
    AudioProcessor();
    ~AudioProcessor();
};
