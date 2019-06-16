/*
 ==============================================================================

 This file was auto-generated!

 ==============================================================================
 */

#include "MainComponent.h"
#include <string>
//==============================================================================
MainComponent::MainComponent() : _playing(false)
{
    // Make sure you set the size of the component after
    // you add any child components.
    _rowId = 0;
    setSize (800, 600);
    setAudioChannels(0, 2);
    addAndMakeVisible(_filesBrowser);
    addAndMakeVisible(_log);
    _log.setText("File duration: 0.0s \nElapsed time: 0.0s", dontSendNotification);
    _log.setFont (Font (16.0f, Font::bold));
    _log.setJustificationType (Justification::left);

    addAndMakeVisible(_harmSlider);
    _harmSlider.setRange(0.0, 1.0, 0.01);
    _harmSlider.setValue(1.0);
    addAndMakeVisible(_harmSliderAux);
    _harmSliderAux.setText("Harmonic volume", dontSendNotification);
    _harmSliderAux.setFont (Font (16.0f, Font::bold));
    _harmSliderAux.setJustificationType (Justification::right);
    addAndMakeVisible(_percSlider);
    _percSlider.setRange(0.0, 1.0, 0.01);
    _percSlider.setValue(1.0);
    addAndMakeVisible(_percSliderAux);
    _percSliderAux.setText("Percussive volume", dontSendNotification);
    _percSliderAux.setFont (Font (16.0f, Font::bold));
    _percSliderAux.setJustificationType (Justification::right);
    addAndMakeVisible(_resiSlider);
    _resiSlider.setRange(0.0, 1.0, 0.01);
    _resiSlider.setValue(1.0);
    addAndMakeVisible(_resiSliderAux);
    _resiSliderAux.setText("Residual volume", dontSendNotification);
    _resiSliderAux.setFont (Font (16.0f, Font::bold));
    _resiSliderAux.setJustificationType (Justification::right);
    addAndMakeVisible(_winharm);
    _winharm.setEditable(true);
    _winharm.setText("131", dontSendNotification);
    _winharm.setColour (Label::backgroundColourId, Colours::darkblue);
    addAndMakeVisible(_winharmAux);
    _winharmAux.setText("Harmonic filter size", dontSendNotification);
    _winharmAux.setFont (Font (16.0f, Font::bold));
    _winharmAux.attachToComponent(&_winharm, true);
    _winharmAux.setJustificationType (Justification::right);
    addAndMakeVisible(_winperc);
    _winperc.setEditable(true);
    _winperc.setText("51", dontSendNotification);
    _winperc.setColour (Label::backgroundColourId, Colours::darkblue);
    addAndMakeVisible(_winpercAux);
    _winpercAux.setText("Percussive filter size", dontSendNotification);
    _winpercAux.attachToComponent(&_winperc, true);
    _winpercAux.setFont (Font (16.0f, Font::bold));
    _winpercAux.setJustificationType (Justification::right);
    addAndMakeVisible(_power);
    _power.setEditable(true);
    _power.setText("2.5", dontSendNotification);
    _power.setColour (Label::backgroundColourId, Colours::darkblue);
    addAndMakeVisible(_powerAux);
    _powerAux.setText("Mask power", dontSendNotification);
    _powerAux.attachToComponent(&_power, true);
    _powerAux.setFont (Font (16.0f, Font::bold));
    _powerAux.setJustificationType (Justification::right);
    addAndMakeVisible(_marginHarm);
    _marginHarm.setEditable(true);
    _marginHarm.setText("1.0", dontSendNotification);
    _marginHarm.setColour (Label::backgroundColourId, Colours::darkblue);
    addAndMakeVisible(_marginHarmAux);
    _marginHarmAux.attachToComponent(&_marginHarm, true);
    _marginHarmAux.setText("Mask harm. size", dontSendNotification);
    _marginHarmAux.setFont (Font (16.0f, Font::bold));
    _marginHarmAux.setJustificationType (Justification::right);
    addAndMakeVisible(_marginPerc);
    _marginPerc.setEditable(true);
    _marginPerc.setText("1.0", dontSendNotification);
    _marginPerc.setColour (Label::backgroundColourId, Colours::darkblue);
    addAndMakeVisible(_marginPercAux);
    _marginPercAux.attachToComponent(&_marginPerc, true);
    _marginPercAux.setText("Mask harm. size", dontSendNotification);
    _marginPercAux.setFont (Font (16.0f, Font::bold));
    _marginPercAux.setJustificationType (Justification::right);
    addAndMakeVisible(_fftSize);
    _fftSize.setEditable(true);
    _fftSize.setText("2048", dontSendNotification);
    _fftSize.setColour (Label::backgroundColourId, Colours::darkblue);
    addAndMakeVisible(_fftSizeAux);
    _fftSizeAux.attachToComponent(&_fftSize, true);
    _fftSizeAux.setText("FFT size", dontSendNotification);
    _fftSizeAux.setFont (Font (16.0f, Font::bold));
    _fftSizeAux.setJustificationType (Justification::right);
    addAndMakeVisible(_hopSize);
    _hopSize.setEditable(true);
    _hopSize.setText("512", dontSendNotification);
    _hopSize.setColour (Label::backgroundColourId, Colours::darkblue);
    addAndMakeVisible(_hopSizeAux);
    _hopSizeAux.attachToComponent(&_hopSize, true);
    _hopSizeAux.setText("Hop size", dontSendNotification);
    _hopSizeAux.setFont (Font (16.0f, Font::bold));
    _hopSizeAux.setJustificationType (Justification::right);
    addAndMakeVisible(_playComponents);
    _playComponents.setButtonText("Compute and play mix");
    _playComponents.onClick = [this] { _playComponentsClicked(); };
    addAndMakeVisible(_exportComponents);
    _exportComponents.setButtonText("Export mix");
    _exportComponents.onClick = [this] { _exportComponentsClicked(); };
    addAndMakeVisible(_playOriginal);
    _playOriginal.onClick = [this] { _playOriginalClicked(); };
    _playOriginal.setButtonText("Play original");
    std::function<void(int)> f = [=](int rowId) {
        this->_rowId = rowId;
    };
    _filesBrowser.setSelectedRowsChangedCallback(f);
    _selectFolder();
    _position = 0;
}

void MainComponent::_playComponentsClicked() {
    juce::Logger::writeToLog(_files[_rowId].getFullPathName());
    _mutex.lock();
    readFromAudioFile(_files[_rowId],
                      _currentAudioSampleBuffer,
                      _formatManager);
    _position = 0;
    int fftSize = (int)(std::stof(_fftSize.getText().toStdString()));
    int hopSize = (int)(std::stof(_hopSize.getText().toStdString()));
    float gainHarm = _harmSlider.getValue();
    float gainPerc = _percSlider.getValue();
    float gainResi = _resiSlider.getValue();
    int winHarm = (int)(std::stof(_winharm.getText().toStdString()));
    int winPerc = (int)(std::stof(_winperc.getText().toStdString()));
    _processCurrentBuffer(_currentAudioSampleBuffer, gainHarm, gainPerc, gainResi);
    _playing = true;
    _mutex.unlock();
}


void MainComponent::_playOriginalClicked() {
    juce::Logger::writeToLog(_files[_rowId].getFullPathName());
    _mutex.lock();
    readFromAudioFile(_files[_rowId],
                      _currentAudioSampleBuffer,
                      _formatManager);
    _position = 0;
    _playing = true;
    _mutex.unlock();
}


void MainComponent::_exportComponentsClicked() {
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    if (!_playing)
        return;
    if (!_mutex.try_lock())
        return;
    auto numFileChannels = _currentAudioSampleBuffer.getNumChannels();
    auto numOutputChannels = bufferToFill.buffer->getNumChannels();
    auto outputSamplesRemaining = bufferToFill.numSamples;
    auto outputSamplesOffset = bufferToFill.startSample;
    auto bufferSamplesRemaining = _currentAudioSampleBuffer.getNumSamples() - _position;
    auto samplesThisTime = jmin (outputSamplesRemaining, bufferSamplesRemaining);
    for (auto channel = 0; channel < numOutputChannels; ++channel)
    {
        bufferToFill.buffer->copyFrom (channel,
                                       outputSamplesOffset,
                                       _currentAudioSampleBuffer,
                                       channel % numFileChannels,
                                       _position,
                                       samplesThisTime);
    }
    _position += samplesThisTime;
    if (_position == _currentAudioSampleBuffer.getNumSamples())
        _playing = false;
    _mutex.unlock();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    float p_x = (float)getWidth() / 100.0f;
    float p_y = (float)getHeight() / 100.0f;
    _filesBrowser.setBounds(p_x * 1, p_y * 1,
                            p_x * 99, p_y * 15);
    _harmSliderAux.setBounds(p_x * 5, p_y * 20,
                             p_x * 15, p_y * 3);
    _harmSlider.setBounds(p_x * 21, p_y * 20,
                          p_x * 50, p_y * 3);
    _percSliderAux.setBounds(p_x * 5, p_y * 25,
                             p_x * 15, p_y * 3);
    _percSlider.setBounds(p_x * 21, p_y * 25,
                          p_x * 50, p_y * 3);
    _resiSliderAux.setBounds(p_x * 5, p_y * 30,
                             p_x * 15, p_y * 3);
    _resiSlider.setBounds(p_x * 21, p_y * 30,
                          p_x * 50, p_y * 3);
    
    _fftSize.setBounds(p_x * 26, p_y * 40,
                       p_x * 20, p_y * 3);
    _hopSize.setBounds(p_x * 26, p_y * 45,
                       p_x * 20, p_y * 3);
    _winharm.setBounds(p_x * 26, p_y * 55,
                       p_x * 25, p_y * 3);
    _winperc.setBounds(p_x * 26, p_y * 60,
                       p_x * 25, p_y * 3);
    _marginHarm.setBounds(p_x * 26, p_y * 70,
                          p_x * 20, p_y * 3);
    _marginPerc.setBounds(p_x * 26, p_y * 75,
                          p_x * 20, p_y * 3);
    _power.setBounds(p_x * 26, p_y * 80,
                     p_x * 20, p_y * 3);

    _log.setBounds(p_x * 75, p_y * 20,
                   p_x * 20, p_y * 10);
    
    _playOriginal.setBounds(p_x * 75, p_y * 35,
                              p_x * 20, p_y * 10);
    _playComponents.setBounds(p_x * 75, p_y * 50,
                              p_x * 20, p_y * 10);
    _exportComponents.setBounds(p_x * 75, p_y * 65,
                              p_x * 20, p_y * 10);
}

void MainComponent::_selectFolder() {
    FileChooser fc ("Choose a directory",
                    File::getCurrentWorkingDirectory(),
                    "*.",
                    true);
    if (fc.browseForDirectory())
    {
        File chosenDirectory = fc.getResult();
        String fullDirectoryPath = chosenDirectory.getFullPathName().toStdString();
        DirectoryIterator iter (File(fullDirectoryPath), true);
        _files.clear();
        _formatManager.registerBasicFormats();
        while (iter.next()) {
            if (isReadableAudioFile(iter.getFile(),
                                    _formatManager)) {
                _files.add(iter.getFile());
                _filesBrowser.addItem(iter.getFile().getFullPathName());
            }
        }
    } else{
        JUCEApplication::quit();
    }
}

void MainComponent::_processCurrentBuffer(AudioSampleBuffer &outBuffer,
                                          float gain_harm, float gain_perc, float gain_resi) {
    std::vector<std::vector<float>> signal_L_and_R;
    EssentiaAudioProcessor _essentiaAudioProcessor;
    for (int channel=0; channel<2; channel++) {
        _essentiaAudioProcessor.readSignalFromInputBuffer(_currentAudioSampleBuffer,
                                                          channel);
        _essentiaAudioProcessor.process(gain_harm, gain_perc, gain_resi);
        std::vector<float> signalChannel;
        _essentiaAudioProcessor.getSynthesizedSamples(signalChannel);
        signal_L_and_R.push_back(signalChannel);
    }
    vectorToBuffer(signal_L_and_R[0],
                   signal_L_and_R[1],
                   outBuffer);
}
