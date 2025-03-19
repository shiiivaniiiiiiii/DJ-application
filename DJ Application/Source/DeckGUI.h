#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

// Class that manages user interface
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    // Constructor that intialises deck gui
    DeckGUI(DJAudioPlayer* player,
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

    // Implement button listener
    void buttonClicked (Button *) override;

    // Implement slider listener
    void sliderValueChanged (Slider *slider) override;

    // Implements file drag and drop
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override; 

private:

    // Playback controls
    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};
  
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    // Labels
    Label volLabel;
    Label speedLabel;
    Label posLabel;
    
    // Reverb effect controls
    TextButton reverbButton{"REVERB OFF"};
    Slider wetDrySlider;
    Slider roomSizeSlider;
    Slider dampingSlider;
       
    // Labels
    Label wetDryLabel;
    Label roomSizeLabel;
    Label dampingLabel;
    
    FileChooser fChooser{"Select a file..."};

    WaveformDisplay waveformDisplay;

    // Pointer to the DJAudioPlayer instance
    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
