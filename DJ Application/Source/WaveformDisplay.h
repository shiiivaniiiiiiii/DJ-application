#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Component that visualises audio waveform
class WaveformDisplay    : public Component,
                           public ChangeListener
{
public:
    // Initialises waveform display
    WaveformDisplay( AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse );
    ~WaveformDisplay();

    // Draws waveform
    void paint (Graphics&) override;
    void resized() override;

    // Handles changes in audio thumbnail
    void changeListenerCallback (ChangeBroadcaster *source) override;

    // Loads audio file
    void loadURL(URL audioURL);

    // Sets position of playhead
    void setPositionRelative(double pos);
    
    void setColours(Colour waveColour, Colour bgColour, Colour posColour = Colours::lightgreen);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded; 
    double position;
    
    // Custom UI colours
    Colour waveformColour = Colours::orange;
    Colour positionColour = Colours::lightgreen;
    Colour backgroundColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};

