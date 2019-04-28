/*
 ==============================================================================

 This file was auto-generated!

 ==============================================================================
 */

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : _playing(false)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    addAndMakeVisible(_filesBrowser);
    std::function<void(int)> f = [=](int rowId) {
        this->selectRow(rowId);
    };
    _filesBrowser.setSelectedRowsChangedCallback(f);
    _selectFolder();
    setAudioChannels(0, 2);
    _position = 0;
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
    float percentage_x = (float)getWidth() / 100.0f;
    float percentage_y = (float)getHeight() / 100.0f;
    _filesBrowser.setBounds(percentage_x * 1, percentage_y * 1,
                          percentage_x * 99, percentage_y * 15);
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

void MainComponent::selectRow(int rowId) {
    juce::Logger::writeToLog(_files[rowId].getFullPathName());
    _mutex.lock();
    readFromAudioFile(_files[rowId],
                      _currentAudioSampleBuffer,
                      _formatManager);
    _position = 0;
    _playing = true;
    _mutex.unlock();
}
