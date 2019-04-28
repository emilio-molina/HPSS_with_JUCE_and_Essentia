/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Utils.h"
#include "SimpleList.h"
#include "EssentiaAudioProcessor.h"
#include <mutex>
//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */



class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    void selectRow(int rowId);
private:
    std::mutex _mutex;
    bool _playing;
    int _position;
    SimpleList _filesBrowser;
    Array<File> _files;
    AudioFormatManager _formatManager;
    AudioSampleBuffer _currentAudioSampleBuffer;
    EssentiaAudioProcessor _essentiaAudioProcessor;
    void _selectFolder();
    void _processCurrentBuffer();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

