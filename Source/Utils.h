/*
  ==============================================================================

    Utils.h
    Created: 27 Apr 2019 12:30:25am
    Author:  Emilio Molina

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <memory.h>
#include "Mediator.h"

void readFromAudioFile(const File& inputAudioFile,
                       AudioSampleBuffer& buffer,
                       AudioFormatManager& formatManager);

void writeToAudioFile(AudioSampleBuffer& buffer,
                    int sampleRate,
                    String outputAudioPath);

bool isReadableAudioFile(const File& inputFile,
                         AudioFormatManager &formatManager);

void medianfilterSymmetric(std::vector<float>& input,
                           std::vector<float>& output,
                           int wsize);

void horizontalMedianFiltering(std::vector<std::vector<float>> &input,
                               std::vector<std::vector<float>> &output,
                               int N);

void verticalMedianFiltering(std::vector<std::vector<float>> input,
                             std::vector<std::vector<float>> &output,
                             int N);

void vectorToBuffer(std::vector<float> &inputL,
                    std::vector<float> &inputR,
                    AudioSampleBuffer &output);

void hpss(std::vector<std::vector<float>> &X,
          int win_harm,
          int win_perc,
          float power,
          float margin_harm,
          float margin_perc,
          std::vector<std::vector<float>> &harm,
          std::vector<std::vector<float>> &perc);
