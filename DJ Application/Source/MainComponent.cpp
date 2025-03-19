#include "MainComponent.h"

// Initialises main component
MainComponent::MainComponent()
{
    setSize (800, 600);
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        setAudioChannels (0, 2);
    }  

    // Add GUI and playlist component
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    
    addAndMakeVisible(playlistComponent);
    
    // Register audio file formate
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

// Prepares to play, gets next audio source and relases resources
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

// Handles background rendering
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour(Colours::white);
    g.setFont(14.0f);
    
}

// Handles layout of components
void MainComponent::resized()
{
    int margin = 20;
    int playlistHeight = getHeight() / 3;

    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() - playlistHeight - margin);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() - playlistHeight - margin);

    playlistComponent.setBounds(0, getHeight() - playlistHeight, getWidth(), playlistHeight);
}


