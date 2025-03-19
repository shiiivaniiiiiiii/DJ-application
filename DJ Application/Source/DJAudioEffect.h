#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Class for audio effects
class DJAudioEffect
{
public:
    DJAudioEffect() : wetDryMix(0.0f), isActive(false) {}
    virtual ~DJAudioEffect() {}
    
    // Set the wet/dry mix of the effect (0.0 = dry, 1.0 = wet)
    void setWetDryMix(float mix)
    {
        wetDryMix = juce::jlimit(0.0f, 1.0f, mix);
    }
    
    // Get the current wet/dry mix value
    float getWetDryMix() const { return wetDryMix; }
    
    // Enable or disable the effect
    void setActive(bool shouldBeActive) { isActive = shouldBeActive; }
    
    // Check if the effect is currently active
    bool getActive() const { return isActive; }
    
    // Process the audio block with the effect
    virtual void process(juce::AudioBuffer<float>& buffer, int numSamples) = 0;
    
    // Prepare the effect for playback
    virtual void prepare(double sampleRate, int samplesPerBlock) = 0;
    
    protected:
    float wetDryMix;  // 0.0 = dry (no effect), 1.0 = wet (full effect)
    bool isActive;    // Whether the effect is enabled
};
