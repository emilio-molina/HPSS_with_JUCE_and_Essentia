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
    Slider _harmSlider;
    Label _harmSliderAux;
    Slider _percSlider;
    Label _percSliderAux;
    Slider _resiSlider;
    Label _resiSliderAux;
    Label _winharm;
    Label _winharmAux;
    Label _winperc;
    Label _winpercAux;
    Label _power;
    Label _powerAux;
    Label _marginHarm;
    Label _marginHarmAux;
    Label _marginPerc;
    Label _marginPercAux;
    Label _fftSize;
    Label _fftSizeAux;
    Label _hopSize;
    Label _hopSizeAux;
    Label _log;
    TextButton _playComponents;
    TextButton _exportComponents;
    TextButton _playOriginal;
    void _selectFolder();
    void _processCurrentBuffer(AudioSampleBuffer &outBuffer);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

