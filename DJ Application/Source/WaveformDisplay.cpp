#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

// Component shows visual representation of audio waveform
WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0)
                          
{
    // Listener to detect changes in audio thumbnail
    audioThumb.addChangeListener(this);
}

// Destructor
WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(Graphics& g)
{
    // Use custom background color
    g.fillAll(backgroundColour);

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    g.setColour(waveformColour);
    
    // Check if audio file is loaded
    if(fileLoaded)
    {
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f
        );
        
        // Draw waveform
        g.setColour(positionColour);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
                   Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{
}

// Loads new audio file
void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    std::cout << "wfd: loaded! " << std::endl;
    repaint();
  }
  else {
    std::cout << "wfd: not loaded! " << std::endl;
  }

}

// When thumbnail is changed or when file is loaded
void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;

    repaint();

}

// Update position
void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }

  
}

// Update waveform and colours
void WaveformDisplay::setColours(Colour waveColour, Colour bgColour, Colour posColour)
{
    waveformColour = waveColour;
    backgroundColour = bgColour;
    positionColour = posColour;
    repaint();
}




