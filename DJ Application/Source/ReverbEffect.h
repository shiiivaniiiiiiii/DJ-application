#pragma once

#include "DJAudioEffect.h"

// Class to implement reverb effect
class ReverbEffect : public DJAudioEffect
{
public:
    ReverbEffect();
    ~ReverbEffect() override;
    
    // Process the audio block with reverb effect
    void process(juce::AudioBuffer<float>& buffer, int numSamples) override;
    
    // Prepare the reverb for playback
    void prepare(double sampleRate, int samplesPerBlock) override;
    
    // Set the room size parameter (0.0 to 1.0)
    void setRoomSize(float size);
    
    // Set the damping parameter (0.0 to 1.0)
    void setDamping(float dampAmount);
    
private:
    juce::Reverb reverb;
    juce::Reverb::Parameters params;
    juce::AudioBuffer<float> dryBuffer;
};

