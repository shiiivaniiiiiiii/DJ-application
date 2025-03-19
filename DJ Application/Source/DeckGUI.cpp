#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

// Initialises the graphical user interface for a DJ deck
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                AudioFormatManager & formatManagerToUse,
                AudioThumbnailCache & cacheToUse
           ) : player(_player),
               waveformDisplay(formatManagerToUse, cacheToUse)
{
    // Custom colour theme for deck gui
    Colour backgroundColour = Colour::fromRGB(5, 5, 16);
    Colour primaryAccent = Colour::fromRGB(0, 245, 212);     // Electric teal
    Colour secondaryAccent = Colour::fromRGB(255, 0, 184);   // Neon magenta
    Colour tertiaryAccent = Colour::fromRGB(255, 240, 31);   // Cyber yellow
    Colour quaternaryAccent = Colour::fromRGB(112, 0, 255);  // Plasma blue
    Colour textColour = Colour::fromRGB(248, 253, 255);      // Ice white
    Colour waveformColour = Colour::fromRGB(123, 255, 0);    // Radioactive green

    // Sliders and buttons with custom colors
    getLookAndFeel().setColour(Slider::thumbColourId, primaryAccent);
    getLookAndFeel().setColour(Slider::trackColourId, secondaryAccent.withAlpha(0.6f));
    getLookAndFeel().setColour(Slider::backgroundColourId, Colours::black.withAlpha(0.8f));
    getLookAndFeel().setColour(Slider::textBoxTextColourId, textColour);
    getLookAndFeel().setColour(Slider::textBoxOutlineColourId, quaternaryAccent.withAlpha(0.5f));
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, primaryAccent);
    getLookAndFeel().setColour(Slider::rotarySliderOutlineColourId, secondaryAccent);
    getLookAndFeel().setColour(Label::textColourId, textColour);
    getLookAndFeel().setColour(TextButton::buttonColourId, quaternaryAccent.withAlpha(0.8f));
    getLookAndFeel().setColour(TextButton::textColourOffId, textColour);
    getLookAndFeel().setColour(TextButton::textColourOnId, tertiaryAccent);

    // Add control buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    
    // Style the buttons
    playButton.setColour(TextButton::buttonColourId, primaryAccent);
    stopButton.setColour(TextButton::buttonColourId, secondaryAccent);
    loadButton.setColour(TextButton::buttonColourId, quaternaryAccent);
    reverbButton.setColour(TextButton::buttonColourId, tertiaryAccent);
    
    // Set button text
    playButton.setButtonText("PLAY");
    stopButton.setButtonText("STOP");
    loadButton.setButtonText("LOAD");
    reverbButton.setButtonText("REVERB OFF");
    
    // Set text color to black for all buttons
    playButton.setColour(TextButton::textColourOffId, Colours::black);
    playButton.setColour(TextButton::textColourOnId, Colours::black);
    stopButton.setColour(TextButton::textColourOffId, Colours::black);
    stopButton.setColour(TextButton::textColourOnId, Colours::black);
    loadButton.setColour(TextButton::textColourOffId, Colours::black);
    loadButton.setColour(TextButton::textColourOnId, Colours::black);
    reverbButton.setColour(TextButton::textColourOffId, Colours::black);
    reverbButton.setColour(TextButton::textColourOnId, Colours::black);

    // Set up button listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    reverbButton.addListener(this);
       
    // Add sliders
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    // Set up text box for sliders to show numbers
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    posSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    
    // Format numbers in sliders
    speedSlider.textFromValueFunction = [](double value) { return String(value, 1) + "x"; };
    volSlider.textFromValueFunction = [](double value) { return String(value * 100, 1) + "%"; };
    posSlider.textFromValueFunction = [](double value) { return String(value * 100, 1) + "%"; };
    
    // Custom slider colours and style
    volSlider.setColour(Slider::thumbColourId, primaryAccent);
    volSlider.setColour(Slider::rotarySliderFillColourId, primaryAccent);
    speedSlider.setColour(Slider::rotarySliderFillColourId, tertiaryAccent);
    speedSlider.setColour(Slider::thumbColourId, tertiaryAccent);
    posSlider.setColour(Slider::thumbColourId, quaternaryAccent);
    posSlider.setColour(Slider::trackColourId, quaternaryAccent.brighter(1.5f).withAlpha(0.8f));
    
    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    
    // Slider ranges
    speedSlider.setRange(0.5, 2, 0);
    speedSlider.setValue(1);
    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    posSlider.setSliderStyle(Slider::LinearHorizontal);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);
    
    // Add event listeners
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    // Add label for sliders
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);
    
    // Set text for labels
    volLabel.setText("VOLUME", dontSendNotification);
    speedLabel.setText("SPEED", dontSendNotification);
    posLabel.setText("POSITION", dontSendNotification);
    
    // Align labels to be centered
    volLabel.setJustificationType(Justification::centred);
    speedLabel.setJustificationType(Justification::centred);
    posLabel.setJustificationType(Justification::centred);
    
    // Custom font for labels
    Font labelFont("Arial", 14.0f, Font::bold);
    volLabel.setFont(labelFont);
    speedLabel.setFont(labelFont);
    posLabel.setFont(labelFont);
    
    // Set up custom waveform display
    addAndMakeVisible(waveformDisplay);
    // In DeckGUI constructor
    waveformDisplay.setColours(waveformColour, backgroundColour.darker(0.8f), primaryAccent);
    
    // Add reverb effect controls
    addAndMakeVisible(reverbButton);
    addAndMakeVisible(wetDrySlider);
    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(dampingSlider);
    
    // Set up sliders for reverb
    wetDrySlider.setRange(0.0, 1.0);
    wetDrySlider.setValue(0.5);
    wetDrySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    wetDrySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    wetDrySlider.textFromValueFunction = [](double value) { return String(value * 100, 1) + "%"; };
    
    roomSizeSlider.setRange(0.0, 1.0);
    roomSizeSlider.setValue(0.5);
    roomSizeSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    roomSizeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    roomSizeSlider.textFromValueFunction = [](double value) { return String(value * 100, 1) + "%"; };
    
    dampingSlider.setRange(0.0, 1.0);
    dampingSlider.setValue(0.5);
    dampingSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    dampingSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    dampingSlider.textFromValueFunction = [](double value) { return String(value * 100, 1) + "%"; };

    // Custom slider colors for reverb controls
    wetDrySlider.setColour(Slider::thumbColourId, primaryAccent);
    wetDrySlider.setColour(Slider::rotarySliderFillColourId, primaryAccent);
    
    roomSizeSlider.setColour(Slider::thumbColourId, tertiaryAccent);
    roomSizeSlider.setColour(Slider::rotarySliderFillColourId, tertiaryAccent);
    
    dampingSlider.setColour(Slider::thumbColourId, secondaryAccent);
    dampingSlider.setColour(Slider::rotarySliderFillColourId, secondaryAccent);
    
    // Add listener for reverb sliders
    wetDrySlider.addListener(this);
    roomSizeSlider.addListener(this);
    dampingSlider.addListener(this);
    
    // Ad labels for reverb controls
    addAndMakeVisible(wetDryLabel);
    addAndMakeVisible(roomSizeLabel);
    addAndMakeVisible(dampingLabel);
    
    // Set text for labels
    wetDryLabel.setText("WET/DRY", dontSendNotification);
    roomSizeLabel.setText("ROOM SIZE", dontSendNotification);
    dampingLabel.setText("DAMPING", dontSendNotification);
    
    // Center labels and set font
    wetDryLabel.setJustificationType(Justification::centred);
    roomSizeLabel.setJustificationType(Justification::centred);
    dampingLabel.setJustificationType(Justification::centred);
    
    wetDryLabel.setFont(labelFont);
    roomSizeLabel.setFont(labelFont);
    dampingLabel.setFont(labelFont);
    
    // Start timer for waveform
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

// Draws the custom background with subtle gradients and glow effects
void DeckGUI::paint(Graphics& g)
{
    // Create a subtle gradient background
    ColourGradient background(Colour::fromRGB(5, 5, 16), 0, 0,
                              Colour::fromRGB(10, 10, 30), getWidth(), getHeight(),
                              false);
    g.setGradientFill(background);
    g.fillAll();

    // Add a subtle grid pattern
    g.setColour(Colours::white.withAlpha(0.03f));
    for (int x = 0; x < getWidth(); x += 20) {
        g.drawLine(x, 0, x, getHeight(), 0.5f);
    }
    for (int y = 0; y < getHeight(); y += 20) {
        g.drawLine(0, y, getWidth(), y, 0.5f);
    }

    // Draw a glowing border
    g.setColour(Colour::fromRGB(0, 245, 212).withAlpha(0.4f));
    g.drawRoundedRectangle(1, 1, getWidth()-2, getHeight()-2, 4.0f, 1.5f);
    
    // Add a subtle pulsing effect
    float pulseAlpha = 0.3f + 0.2f * sin(Time::getMillisecondCounter() / 500.0f);
    g.setColour(Colour::fromRGB(255, 0, 184).withAlpha(pulseAlpha));
    g.drawRoundedRectangle(3, 3, getWidth()-6, getHeight()-6, 4.0f, 1.0f);
}

// Position
void DeckGUI::resized()
{
    double rowH = getHeight() / 12;
    double width = getWidth();

    double buttonWidth = width / 4;
    double buttonHeight = rowH * 2;
    
    // Set bounds of buttons
    playButton.setBounds(0, 0, buttonWidth, buttonHeight);
    stopButton.setBounds(buttonWidth, 0, buttonWidth, buttonHeight);
    loadButton.setBounds(buttonWidth * 2, 0, buttonWidth, buttonHeight);
    reverbButton.setBounds(buttonWidth * 3, 0, buttonWidth, buttonHeight);
    
    // Set bounds of waveform display
    waveformDisplay.setBounds(0, rowH * 2, width, rowH * 3);

    // Position slider
    double posSliderHeight = rowH * 1.5;
    posSlider.setBounds(0, rowH * 5, width, posSliderHeight);
    posLabel.setBounds(0, posSlider.getBottom() + 5, width, 20);

    // Calculate positions for all rotary controls and position them
    double sliderWidth = width / 5;
    double sliderHeight = rowH * 3;
    double sliderTop = rowH * 7;
    double padding = width / 20;
    double x = padding;
    
    // Volume slider
    volSlider.setBounds(x, sliderTop, sliderWidth, sliderHeight);
    volLabel.setBounds(volSlider.getX(), volSlider.getBottom() + 5, sliderWidth, 20);
    x += sliderWidth + padding;
    
    // Speed slider
    speedSlider.setBounds(x, sliderTop, sliderWidth, sliderHeight);
    speedLabel.setBounds(speedSlider.getX(), speedSlider.getBottom() + 5, sliderWidth, 20);
    x += sliderWidth + padding;
    
    // Wet/dry slider
    wetDrySlider.setBounds(x - 50, sliderTop, sliderWidth, sliderHeight);
    wetDryLabel.setBounds(wetDrySlider.getX(), wetDrySlider.getBottom() + 5, sliderWidth, 20);
    x += sliderWidth + padding;
    
    // Room size slider
    roomSizeSlider.setBounds(x-82, sliderTop, sliderWidth, sliderHeight);
    roomSizeLabel.setBounds(roomSizeSlider.getX(), roomSizeSlider.getBottom() + 5, sliderWidth, 20);
    x += sliderWidth + padding;
    
    // Damping slider
    dampingSlider.setBounds(x-130, sliderTop, sliderWidth, sliderHeight);
    dampingLabel.setBounds(dampingSlider.getX(), dampingSlider.getBottom() + 5, sliderWidth, 20);
    x += sliderWidth + padding;
    
    // Show/hide reverb controls based on whether reverb is active or not
    bool isReverbActive = player->isReverbActive();
    wetDrySlider.setVisible(isReverbActive);
    roomSizeSlider.setVisible(isReverbActive);
    dampingSlider.setVisible(isReverbActive);
    wetDryLabel.setVisible(isReverbActive);
    roomSizeLabel.setVisible(isReverbActive);
    dampingLabel.setVisible(isReverbActive);
}

// Handles button clicked events
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton)
    {
        auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay.loadURL(URL{chooser.getResult()}); 
        });
        
    }
    
    if (button == &reverbButton)
    {
        bool isReverbActive = !player->isReverbActive();
        player->setReverbActive(isReverbActive);
        
        // Update button text
        reverbButton.setButtonText(isReverbActive ? "REVERB ON" : "REVERB OFF");
        
        // Show/hide reverb controls
        wetDrySlider.setVisible(isReverbActive);
        roomSizeSlider.setVisible(isReverbActive);
        dampingSlider.setVisible(isReverbActive);
        wetDryLabel.setVisible(isReverbActive);
        roomSizeLabel.setVisible(isReverbActive);
        dampingLabel.setVisible(isReverbActive);
    }
}

// Handles slider value changes
void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &wetDrySlider)
    {
        player->setReverbWetDry(slider->getValue());
    }
    if (slider == &roomSizeSlider)
    {
        player->setReverbRoomSize(slider->getValue());
    }
    if (slider == &dampingSlider)
    {
        player->setReverbDamping(slider->getValue());
    }
    
}

// Handles file drag and drop events
bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

// Updates waveform playhead position
void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}



    

